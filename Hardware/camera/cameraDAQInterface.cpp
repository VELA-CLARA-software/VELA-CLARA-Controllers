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
#include <thread>

//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
cameraInterface::cameraInterface( const std::string &Conf,
                                  const bool startVirtualMachine,
                                  const bool* show_messages_ptr,
                                  const bool* show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const VELA_ENUM::MACHINE_AREA myMachineArea ):
configReader( Conf, startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface( show_messages_ptr, show_debug_messages_ptr ),
shouldStartEPICs( shouldStartEPICs ), myMachineArea(myMachineArea),selectedCamera(allCamData["VC"])
{
    initialise();
}
cameraInterface::~cameraInterface()
{
}
/*void cameraInterface::killMonitor( cameraStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
        debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}*/
void cameraInterface::initialise()
{
    /// The config file reader
    message("The cameraInterface is going to try and read the config file.");
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if(configFileRead)
    {
        message("The cameraInterface has read the config file, acquiring objects");
        // initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                message("The cameraInterface has acquired objects, connecting to EPICS");
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions
                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
            }
            else
             message("The cameraInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message( "!!!The cameraInterface received an Error while getting magnet data!!!" );
    }
    else
        message("The cameraInterface  Failed to Read the configFile.");
}
bool cameraInterface::initObjects()
{
    bool camDatSuccess = configReader.getCamData( allCamDAQData );
    //degStruct = configReader.getDeguassStruct();
    //addDummyElementToAllMAgnetData();
    // set the machine area on each magent, this allows for flavour switching functions, such as switchON etc..
    for( auto && it : allCamDAQData )
        it.second.machineArea = myMachineArea;
    return camDatSuccess;
}
void cameraInterface::initChids()
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

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Camera Monitors", "!!TIMEOUT!! Subscription to Camera monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
void cameraInterface::staticEntryMonitor(const event_handler_args args)
{
    cameraStructs::monitorStruct*ms = static_cast<  cameraStructs::monitorStruct *>(args.usr);
    switch( ms -> monType )
    {
        case cameraStructs::CAM_PV_TYPE::CAM_STATE:
            ms->interface->updateState( *(unsigned short*)args.dbr, ms->objName);
            break;
        default:
            ms->interface->debugMessage( "!!! ERROR !!! Unknown Monitor Type passed to cameraInterface::staticEntryMonitor" );
            break;
    }
}
void cameraInterface::updateState(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].state = cameraStructs::CAM_STATUS::CAM_OFF;
                break;
            case 1:
                allCamDAQData[cameraName].state = cameraStructs::CAM_STATUS::CAM_ON;
                break;
            default:
                allCamDAQData[cameraName].state = cameraStructs::CAM_STATUS::CAM_ERROR;
        }
    message(cameraName, "State is ", ENUM_TO_STRING( allCamDAQData[cameraName].state));
    }
/*
}
void cameraInterface::updateX(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.x = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.x,value,allCamData[cameraName].imageStruct.xTolerance))
            debugMessage(cameraName," NEW X = ", allCamData[cameraName].rawData.x);
    }
}
void cameraInterface::updateY(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.y = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.y,value,allCamData[cameraName].imageStruct.yTolerance))
            debugMessage(cameraName," NEW Y = ", allCamData[cameraName].rawData.y);
    }
}
void cameraInterface::updateSigmaX(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.xSigma = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.xSigma,value,allCamData[cameraName].imageStruct.xSigmaTolerance))
            debugMessage(cameraName," NEW SIGMA X = ", allCamData[cameraName].rawData.xSigma);
    }
}
void cameraInterface::updateSigmaY(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.ySigma = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.ySigma,value,allCamData[cameraName].imageStruct.ySigmaTolerance))
            debugMessage(cameraName," NEW SIGMA Y = ", allCamData[cameraName].rawData.ySigma);
    }
}
void cameraInterface::updateSigmaXY(const double value,const std::string& cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        allCamData[cameraName].rawData.xySigma = roundToN(value,4); /// MAGIC_NUMBER AAAAAAHAHAHAH
        if( areNotSame(allCamData[cameraName].rawData.xySigma,value,allCamData[cameraName].imageStruct.xySigmaTolerance))
            debugMessage(cameraName," NEW SIGMA XY = ", allCamData[cameraName].rawData.xySigma);
    }
}
bool cameraInterface::isON ( const std::string & cam )
{
    bool ans = false;
    if( entryExists( allCamData, cam ) )
        if( allCamData[cam].state == cameraStructs::CAM_STATUS::CAM_ON )
            ans = true;
    return ans;
}
bool cameraInterface::isOFF( const std::string & cam )
{
    bool ans = false;
    if( entryExists( allCamData, cam ) )
        if( allCamData[cam].state == cameraStructs::CAM_STATUS::CAM_OFF )
            ans = true;
    return ans;
}
bool cameraInterface::isMonitoring ( const std::string & cam )///WRITE
{
    return false;
}
bool cameraInterface::isNotMonitoring ( const std::string & cam )///WRITE
{
    return false;
}
std::string cameraInterface::cameraName()
{
    return selectedCamera.name;
}
bool cameraInterface::setCamera(const std::string & cam)
{
    bool ans = false;
    if( entryExists( allCamData, cam ) )
    {
            selectedCamera = allCamData[cam];
            ans = true;
    }
    return ans;
}
bool cameraInterface::calibrate()///WRITE
{
    return false;
}
bool cameraInterface::setXRatio(const double & r)///WRITE
{
   return false;
}
bool cameraInterface::setYRatio(const double & r)///WRITE
{
    return false;
}
bool cameraInterface::start()
{
    message("start() called");
    bool ret = false;
    unsigned short comm = (selectedCamera.state == cameraStructs::CAM_STATUS::CAM_ON) ? EPICS_SEND : EPICS_ACTIVATE;
    message("comm = ", comm);
        ca_put(selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHTYPE,
               selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHID,
               &comm);
        message("Trying to turn on camera named ", selectedCamera.name);
    int status = sendToEpics( "ca_put", "", "Timeout sending start value");
    if ( status == ECA_NORMAL )
        ret = true; message("status return was ECA_NORMAL");
    return ret;
}
bool cameraInterface::stop()
{
    message("start() called");
    bool ret = false;
    unsigned short comm = (selectedCamera.state == cameraStructs::CAM_STATUS::CAM_OFF) ? EPICS_SEND : EPICS_ACTIVATE;
    message("comm = ", comm);
        ca_put(selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHTYPE,
               selectedCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_START].CHID,
               &comm);
        message("Trying to turn on camera named ", selectedCamera.name);
    int status = sendToEpics( "ca_put", "", "Timeout sending start value");
    if ( status == ECA_NORMAL )
        ret = true; message("status return was ECA_NORMAL");
    return ret;
}
*/
bool cameraInterface::collectAndSave(int & numbOfShots)///WRITE
{
    return false;
}
/*
std::vector< cameraStructs::camDataType > cameraInterface::getRawData()
{
    return selectedCamera.rawData.cameraData;
}
std::vector< cameraStructs::camDataType > cameraInterface::getBackgroundRawData()
{
    return selectedCamera.rawData.cameraBackgroundData;
}
double cameraInterface::getX()///WRITE
{
    return false;
}
double cameraInterface::getY()///WRITE
{
    return false;
}
double cameraInterface::getSigmaX()///WRITE
{
    return false;
}
double cameraInterface::getSigmaY()///WRITE
{
    return false;
}
double cameraInterface::getSigmaXY()///WRITE
{
    return false;
}*/
const cameraStructs::cameraObject & cameraInterface::getCamObjConstRef( const std::string & camName  )
{
    if( entryExists( allCamData, camName ) )
        return allCamData[ camName ];
    else
        return allCamData[ camName ];

}
