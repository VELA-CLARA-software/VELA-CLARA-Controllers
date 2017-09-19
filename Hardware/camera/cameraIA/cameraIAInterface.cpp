//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //

#include "cameraIAInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>

cameraIAInterface::cameraIAInterface( const std::string &Conf,
                                  const bool startVirtualMachine,
                                  const bool* show_messages_ptr,
                                  const bool* show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const VELA_ENUM::MACHINE_AREA myMachineArea):
configReader(Conf,startVirtualMachine,show_messages_ptr,show_debug_messages_ptr),
cameraInterface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs(shouldStartEPICs),
myMachineArea(myMachineArea)
{
    initialise();
}
cameraIAInterface::~cameraIAInterface()
{
}
void cameraIAInterface::initialise()
{
    /// The config file reader
    message("The cameraIAInterface is going to try and read the config file.");
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));//MAGIC_NUMBER
    if(configFileRead)
    {
        message("The cameraIAInterface has read the config file, acquiring objects");
        // initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The cameraIAInterface has acquired objects, connecting to EPICS");
                initChids();// subscribe to the channel ids
                startMonitors();// start the monitors: set up the callback functions
                // The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));//MAGIC_NUMBER
            }
            else
                message("The cameraIAInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message("!!!The cameraIAInterface received an Error while getting Camera DAQ data!!!");
    }
    else
        message("The cameraIAInterface  Failed to Read the configFile.");
}
void cameraIAInterface::killMonitor(monitorDAQStruct *ms)
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        debugMessage(ms->objName, " ",
                 ENUM_TO_STRING(ms->monType),
                 " monitoring = false ");
    }
    else
    {
        debugMessage("ERROR: in killMonitor: ca_clear_subscription failed for ",
                     ms->objName, " ", ENUM_TO_STRING(ms->monType));
    }
}
bool cameraIAInterface::initObjects()
{
    bool camDatSuccess = configReader.getCamData( allCamData );
    vcCameraObj = allCamData.at("VC"); //MAGIC_STRING
    setCamera("VC");
    for( auto && it : allCamData )
        it.second.machineArea = myMachineArea;
    return camDatSuccess;
}
void cameraIAInterface::initChids()
{
    message( "\n", "Searching for Camera chids...");
    for( auto && camObjIt : allCamData )
    {
        for(auto && it2 : camObjIt.second.pvComStructs)
        {
            addChannel(camObjIt.second.pvRoot, it2.second);
        }
        for(auto && it2 : camObjIt.second.pvMonStructs)
        {
            addChannel(camObjIt.second.pvRoot, it2.second);
        }
    }
    int status = sendToEpics( "ca_create_channel",
                             "Found Camera ChIds.",
                             "!!TIMEOUT!! Not all Camera ChIds found.");
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));//MAGIC_NUMBER
        for( const auto & camObjIt : allCamData )
        {
            message("\n", "Checking Chids for ", camObjIt.first );
            for( auto & it2 : camObjIt.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
            for( auto & it2 : camObjIt.second.pvComStructs )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
        }
        message("");
        std::this_thread::sleep_for(std::chrono::milliseconds( 5000 )); // MAGIC_NUMBER
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
void cameraIAInterface::startMonitors()
{
    continuousMonitorIAStructs.clear();

    for(auto && it : allCamData)
    {
        for(auto && it2 : it.second.pvMonStructs)
        {
            continuousMonitorIAStructs.push_back(new cameraStructs::monitorIAStruct());
            continuousMonitorIAStructs.back()->monType = it2.first;
            continuousMonitorIAStructs.back()->objName = it.second.name;
            continuousMonitorIAStructs.back()->interface = this;
            continuousMonitorIAStructs.back() -> CHTYPE  = it2.second.CHTYPE;
            continuousMonitorIAStructs.back() -> EVID  = it2.second.EVID;
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   cameraIAInterface::staticEntryIAMonitor,
                                   (void*)continuousMonitorIAStructs.back(),
                                   &continuousMonitorIAStructs.back()->EVID );
            debugMessage("Adding monitor for ",  it.second.name, " ", ENUM_TO_STRING(it2.first));
        }
    }
    int status = sendToEpics( "ca_create_subscription",
                             "Succesfully Subscribed to Camera  IA Monitors",
                             "!!TIMEOUT!! Subscription to Camera IA monitors failed");
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
void cameraIAInterface::staticEntryIAMonitor(const event_handler_args args)
{
    monitorIAStruct*ms = static_cast<monitorIAStruct*>(args.usr);
    switch(ms -> monType)
    {
        case CAM_PV_TYPE::X:
            ms->interface->updateX( *(unsigned short*)args.dbr, ms->objName);
            break;
        case CAM_PV_TYPE::Y:
            ms->interface->updateY( *(unsigned short*)args.dbr, ms->objName);
            break;
        case CAM_PV_TYPE::SIGMA_X:
            ms->interface->updateSigmaX( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::SIGMA_Y:
            ms->interface->updateSigmaY( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::COV_XY:
            ms->interface->updateCovXY( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::X_PIX:
            ms->interface->updateXPix( *(unsigned short*)args.dbr, ms->objName);
            break;
        case CAM_PV_TYPE::Y_PIX:
            ms->interface->updateYPix( *(unsigned short*)args.dbr, ms->objName);
            break;
        case CAM_PV_TYPE::SIGMA_X_PIX:
            ms->interface->updateSigmaXPix( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::SIGMA_Y_PIX:
            ms->interface->updateSigmaYPix( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::COV_XY_PIX:
            ms->interface->updateCovXYPix( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::X_RAD:
            ms->interface->updateXRad( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::Y_RAD:
            ms->interface->updateYRad( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::X_CENTER_PIX:
            ms->interface->updateXCenterPix( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::Y_CENTER_PIX:
            ms->interface->updateYCenterPix( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::BIT_DEPTH:
            ms->interface->updateBitDepth( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::IMAGE_HEIGHT:
            ms->interface->updateImageHeight( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::IMAGE_WIDTH:
            ms->interface->updateImageWidth( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_ACQUIRE_RBV:
            ms->interface->updateAcquiring( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_STATUS:
            ms->interface->updateState( *(unsigned short*)args.dbr, ms->objName );
            break;
        default:
            ms->interface->debugMessage("!!! ERROR !!! Unknown Monitor Type passed to cameraDAQInterface::staticEntryMonitor");
            break;
    }
}
void cameraIAInterface::updateState(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).state = CAM_STATE::CAM_OFF;
                break;
            case 1:
                allCamData.at(cameraName).state = CAM_STATE::CAM_ON;
                break;
            default:
                allCamData.at(cameraName).state = CAM_STATE::CAM_ERROR;
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        updateSelectedOrVC(cameraName);
        message(cameraName,": State is ",
                ENUM_TO_STRING(allCamData.at(cameraName).state));
    }
}
void cameraIAInterface::updateAcquiring(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).acquireState = ACQUIRE_STATE::NOT_ACQUIRING;
                break;
            case 1:
                allCamData.at(cameraName).acquireState = ACQUIRE_STATE::ACQUIRING;
                break;
            default:
                allCamData.at(cameraName).acquireState = ACQUIRE_STATE::ACQUIRING_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Aquiring state is ",
                ENUM_TO_STRING(allCamData.at(cameraName).acquireState));
    }
}
void cameraIAInterface::updateX(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.x = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateY(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.y = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateSigmaX(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.sigmaX = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateSigmaY(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.sigmaY = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateCovXY(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.covXY = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateXPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.xPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateYPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.yPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateSigmaXPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.sigmaXPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateSigmaYPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.sigmaYPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateCovXYPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.covXYPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateXRad(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.xRad = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateYRad(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.yRad = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateXCenterPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.xCenterPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateYCenterPix(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.yCenterPix = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateBitDepth(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.bitDepth = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateImageHeight(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.imageHeight = value;
    updateSelectedOrVC(cameraName);
}
void cameraIAInterface::updateImageWidth(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).IA.imageWidth = value;
    updateSelectedOrVC(cameraName);
}
///Functions Accessible to Python Controller///
bool cameraIAInterface::setBackground()
{
    return false;
}
const cameraObject &cameraIAInterface::getCamIAObjConstRef(const std::string &cam)///THIS NEEDS FIXING  at "NOPE"
{
    std::string cameraName = useCameraFrom(cam);
    if(entryExists(allCamData, cameraName))
    {
        return allCamData.at(cameraName);
    }
    else
    {
        for(auto && it : allCamData)
        {
            if(it.second.pvRoot == cameraName)
                return it.second;
        }
    }
    std::string dummyName("NOPE");
    return allCamData.at(dummyName);
}
const cameraObject &cameraIAInterface::getSelectedIARef()
{
    return selectedCameraObj;
}
const cameraObject &cameraIAInterface::getVCIARef()
{
    return vcCameraObj;
}

///Useful Functions for the Class///
/*bool cameraIAInterface::makeANewDirectory()
{
    bool ans=false;
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);
    //tm local_tm = *localtime(&t);
    struct tm local_tm;
    localtime_s(&local_tm, &t);
    char  newPath[256]="/home/controls/tim/";
    message("File Directory would be: ",
                        std::to_string(local_tm.tm_year + 1900)+
                        "/"+std::to_string(local_tm.tm_mon + 1)+
                        "/"+std::to_string(local_tm.tm_mday)+
                        "/"+std::to_string(local_tm.tm_hour)+
                        "_"+std::to_string(local_tm.tm_min)+
                        "_"+std::to_string(local_tm.tm_sec));

    ca_array_put(selectedDAQCamera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).CHTYPE,
                selectedDAQCamera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).COUNT,

                selectedDAQCamera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).CHID,
                &newPath);

    int status = sendToEpics("ca_put","","Timeout trying to send new file path state.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        selectedDAQCamera.latestDirectory = newPath;
        message("New path set to ",newPath," on ",selectedDAQCamera.name," camera.");
    }

    return ans;
}*/

void cameraIAInterface::updateSelectedOrVC(const std::string cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        if(cameraName==selectedCamera())
            selectedCameraObj = allCamData.at(cameraName);
        if(cameraName=="VC")
            vcCameraObj = allCamData.at(cameraName);
    }

}

