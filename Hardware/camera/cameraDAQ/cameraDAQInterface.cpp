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

#include "cameraDAQInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>
cameraDAQInterface::cameraDAQInterface( const std::string &Conf,
                                  const bool startVirtualMachine,
                                  const bool* show_messages_ptr,
                                  const bool* show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const VELA_ENUM::MACHINE_AREA myMachineArea ):
configReader( Conf, startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
cameraInterface( show_messages_ptr, show_debug_messages_ptr ),
shouldStartEPICs( shouldStartEPICs ), myMachineArea(myMachineArea)
{
    initialise();
}
cameraDAQInterface::~cameraDAQInterface()
{
}
void cameraDAQInterface::initialise()
{
    /// The config file reader
    message("The cameraDAQInterface is going to try and read the config file.");
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if(configFileRead)
    {
        message("The cameraDAQInterface has read the config file, acquiring objects");
        // initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                message("The cameraDAQInterface has acquired objects, connecting to EPICS");
                //std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions
                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
            }
            else
             message("The cameraDAQInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message( "!!!The cameraDAQInterface received an Error while getting Camera DAQ data!!!" );
    }
    else
        message("The cameraDAQInterface  Failed to Read the configFile.");
}
void cameraDAQInterface::killMonitor( cameraStructs::monitorDAQStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
        debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}
bool cameraDAQInterface::initObjects()
{
    bool camDatSuccess = configReader.getCamDAQData( allCamDAQData );
    vcDAQCamera = allCamDAQData["VC"]; //MAGIC
    //selectedDAQCamera = &allCamDAQData["VC"];
    setCamera("VC");
    for( auto && it : allCamDAQData )
        it.second.machineArea = myMachineArea;
    return camDatSuccess;
}
void cameraDAQInterface::initChids()
{
    message( "\n", "Searching for Camera chids...");
    for( auto && camObjIt : allCamDAQData )
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
    int status = sendToEpics( "ca_create_channel", "Found Camera ChIds.", "!!TIMEOUT!! Not all Camera ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));//MAGIC_NUMBER
        for( const auto & camObjIt : allCamDAQData )
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
void cameraDAQInterface::startMonitors()
{
    continuousMonitorDAQStructs.clear();

    for(auto && it : allCamDAQData)
    {
        for(auto && it2 : it.second.pvMonStructs)
        {
            continuousMonitorDAQStructs.push_back(new cameraStructs::monitorDAQStruct());
            continuousMonitorDAQStructs.back()->monType = it2.first;
            continuousMonitorDAQStructs.back()->objName = it.second.name;
            continuousMonitorDAQStructs.back()->interface = this;
            continuousMonitorDAQStructs.back() -> CHTYPE  = it2.second.CHTYPE;
            continuousMonitorDAQStructs.back() -> EVID  = it2.second.EVID;
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   cameraDAQInterface::staticEntryDAQMonitor,
                                   (void*)continuousMonitorDAQStructs.back(),
                                   &continuousMonitorDAQStructs.back()->EVID );
            debugMessage("Adding monitor for ",  it.second.name, " ", ENUM_TO_STRING(it2.first));
        }
    }
    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Camera  DAQ Monitors", "!!TIMEOUT!! Subscription to Camera DAQ monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
void cameraDAQInterface::staticEntryDAQMonitor(const event_handler_args args)
{
    cameraStructs::monitorDAQStruct*ms = static_cast<  cameraStructs::monitorDAQStruct *>(args.usr);
    switch( ms -> monType )
    {
        case cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE_RBV:
            ms->interface->updateAquiring( *(unsigned short*)args.dbr, ms->objName);
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_STATUS:
            ms->interface->updateState( *(unsigned short*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_STATUS:
            ms->interface->updateWriteState( *(unsigned short*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURED:
            ms->interface->updateNumCaptured( *(unsigned long*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE:
            ms->interface->updateNumCapture( *(unsigned long*)args.dbr, ms->objName );
            break;
        default:
            ms->interface->debugMessage( "!!! ERROR !!! Unknown Monitor Type passed to cameraInterface::staticEntryMonitor" );
            break;
    }
}
void cameraDAQInterface::updateState(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].state = cameraStructs::CAM_STATE::CAM_OFF;
                break;
            case 1:
                allCamDAQData[cameraName].state = cameraStructs::CAM_STATE::CAM_ON;
                break;
            default:
                allCamDAQData[cameraName].state = cameraStructs::CAM_STATE::CAM_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.state = allCamDAQData[cameraName].state;//Update the selected camera object
        if(cameraName=="VC")
            vcDAQCamera.state = allCamDAQData[cameraName].state;//Update the VC camera object
        message(cameraName, ": State is ", ENUM_TO_STRING( allCamDAQData[cameraName].state));
    }
}
void cameraDAQInterface::updateWriteState(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].writeState = cameraStructs::WRITE_STATE::WRITE_OK;
                break;
            case 1:
                allCamDAQData[cameraName].writeState = cameraStructs::WRITE_STATE::WRITE_ERROR;
                break;
            default:
                allCamDAQData[cameraName].writeState = cameraStructs::WRITE_STATE::WRITE_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.writeState = allCamDAQData[cameraName].writeState;//Update the selected camera object
        if(cameraName=="VC")
            vcDAQCamera.writeState = allCamDAQData[cameraName].writeState;//Update the VC camera object
        message(cameraName, ": Write Status is ", ENUM_TO_STRING( allCamDAQData[cameraName].writeState));
    }
}
void cameraDAQInterface::updateAquiring(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].aquireState = cameraStructs::AQUIRE_STATE::NOT_ACQUIRING;
                break;
            case 1:
                allCamDAQData[cameraName].aquireState = cameraStructs::AQUIRE_STATE::ACQUIRING;
                break;
            default:
                allCamDAQData[cameraName].aquireState = cameraStructs::AQUIRE_STATE::AQUIRING_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.aquireState = allCamDAQData[cameraName].aquireState;//Update the selected camera object
        if(cameraName=="VC")
            vcDAQCamera.aquireState = allCamDAQData[cameraName].aquireState;//Update the VC camera object
        message(cameraName, ": Aquiring state is ", ENUM_TO_STRING( allCamDAQData[cameraName].aquireState));
    }
}
void cameraDAQInterface::updateNumCaptured(const unsigned long value,const std::string&cameraName)
{
    allCamDAQData[cameraName].shotsTaken = value;
    if(cameraName==selectedCamera())
        selectedDAQCamera.shotsTaken = allCamDAQData[cameraName].shotsTaken;//UPdate the selected camera object
    if(cameraName=="VC")
        vcDAQCamera.shotsTaken = allCamDAQData[cameraName].shotsTaken;//Update the VC camera object
}
void cameraDAQInterface::updateNumCapture(const unsigned long value,const std::string&cameraName)
{
    allCamDAQData[cameraName].numberOfShots = value;
    if(cameraName==selectedCamera())
        selectedDAQCamera.numberOfShots = allCamDAQData[cameraName].numberOfShots;//Update the selected camera object
    if(cameraName=="VC")
        vcDAQCamera.numberOfShots = allCamDAQData[cameraName].numberOfShots;//Update the VC camera object
}
bool cameraDAQInterface::makeANewDirectory()
{
    bool ans=false;
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);
    tm local_tm = *localtime(&t);

    std::string newPath("/home/controls/tim/");/* +std::to_string(local_tm.tm_year + 1900)+
                        "/"+std::to_string(local_tm.tm_mon + 1)+
                        "/"+std::to_string(local_tm.tm_mday)+
                        "/"+std::to_string(local_tm.tm_hour)+"_"+std::to_string(local_tm.tm_min)+"_"+std::to_string(local_tm.tm_sec));*/

    char dir[256];//MAGIC NUMBER
    strncpy(dir, newPath.c_str(),sizeof(dir));

    ca_array_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH].CHTYPE,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH].COUNT,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH].CHID,
                &dir);

    int status = sendToEpics("ca_put", "", "Timeout trying to send new file path state.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        message("New path set to ",dir," on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
std::string cameraDAQInterface::getWriteMessage()
{
    std::string out("Failed to get message!!!");
    char note[256];

    ca_array_get(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_MESSAGE].CHTYPE,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_MESSAGE].COUNT,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_MESSAGE].CHID,
                &note);

    int status = sendToEpics("ca_put", "", "Timeout trying to get write message.");
    if(status == ECA_NORMAL)
    {
        out = note;
    }

    return out;
}
bool cameraDAQInterface::collect(unsigned short &comm)
{
    bool ans=false;
    if( isAquiring(selectedCamera()) )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_CAPTURE].CHTYPE,
               selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_CAPTURE].CHID,
               &comm);
        //message("sendToEpics ");
        int status = sendToEpics("ca_put", "", "Timeout trying to send Capture state.");
        if(status == ECA_NORMAL)
        {
            ans = true;
            //message("status return was ECA_NORMAL");
        }
        message("Caputure set to ",comm," on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraDAQInterface::write(unsigned short &comm)
{
    bool ans=false;
    if( isAquiring(selectedCamera()) )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE].CHTYPE,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE].CHID,
                &comm);
        //message("sendToEpics ");
        int status = sendToEpics("ca_put", "", "Timeout trying to send Write state.");
        if(status == ECA_NORMAL)
        {
            ans = true;
            //message("status return was ECA_NORMAL");
        }
        message("WriteFile set to ",comm," on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
bool cameraDAQInterface::collectAndSave(const int & numbOfShots)
{
    bool success = false;
    makeANewDirectory();

    ca_put(selectedDAQCamera.pvMonStructs[cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE].CHTYPE,
            selectedDAQCamera.pvMonStructs[cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE].CHID,
            &numbOfShots);
    int status = sendToEpics("ca_put", "", "Timeout trying to send NumCapture.");
    unsigned short go(1);
    collect(go);
    //when collection is done....
    std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); //MAGIC_NUMBER
    write(go);
    //Wait for messages to update!
    std::this_thread::sleep_for(std::chrono::milliseconds( 3000 )); //MAGIC_NUMBER
    if (allCamDAQData[selectedCamera()].writeState==0)//0 means okay! I know its weird but that comes from epics.... what you gonna do
    {
        success = true;
        message("Successful wrote image to disk.");
    }
    else
        debugMessage("WRITE ERROR: ",getWriteMessage());

    return success;

}
bool cameraDAQInterface::killCollectAndSave()///WRITE
{
    bool killed = false;
  /*  unsigned short capture,save;
    ca_get(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_CAPTURE].CHTYPE,
            selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_CAPTURE].CHID,
            &capture);
    ca_get(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE].CHTYPE,
            selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE].CHID,
            &save);
    if (capture==1)
    {
        message("Caught capturing...");
        capture=0;
        killed=collect(capture);
    }
    else if (save==1)
    {
        message("Caught writing...");
        save=0;
        killed=write(save);
    }*/ ///THIS DOES NOT WORK (TRIED IT AND THE IOC CRASHES)
    return killed;
}
const cameraStructs::cameraDAQObject & cameraDAQInterface::getCamDAQObjConstRef( const std::string & camName  )
{
    if(entryExists(allCamDAQData, camName))
    {
        return allCamDAQData[ camName ];
    }
    else
    {
        for(auto && it : allCamDAQData)
        {
            if(it.second.pvRoot == camName)
                return it.second;
        }
    }
    std::string dummyName("NOPE");
    return allCamDAQData[ dummyName ];
}
const cameraStructs::cameraDAQObject &cameraDAQInterface::getSelectedDAQRef()
{
    return selectedDAQCamera;
}
cameraStructs::cameraDAQObject* cameraDAQInterface::getSelectedDAQPtr()
{
    return &selectedDAQCamera;
}
const cameraStructs::cameraDAQObject & cameraDAQInterface::getVCDAQRef()
{
    return vcDAQCamera;
}
