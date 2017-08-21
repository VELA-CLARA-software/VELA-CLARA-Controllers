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
void cameraDAQInterface::killMonitor(cameraStructs::monitorDAQStruct * ms)
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
    vcDAQCamera = allCamDAQData["VC"]; //MAGIC_STRING
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
            ms->interface->updateAcquiring( *(unsigned short*)args.dbr, ms->objName);
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_CAPTURE_RBV:
            ms->interface->updateCapturing( *(unsigned short*)args.dbr, ms->objName);
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_STATUS:
            ms->interface->updateState( *(unsigned short*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_RBV:
            ms->interface->updateWriteState( *(unsigned short*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_CHECK:
            ms->interface->updateWriteCheck( *(unsigned short*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURED:
            ms->interface->updateNumCaptured( *(unsigned long*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE_RBV:
            ms->interface->updateNumCapture( *(unsigned long*)args.dbr, ms->objName );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_MESSAGE:
            ms->interface->updateWriteErrorMessage(args.dbr, ms->objName );
            break;
        default:
            ms->interface->debugMessage( "!!! ERROR !!! Unknown Monitor Type passed to cameraDAQInterface::staticEntryMonitor" );
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
        // this is also copying the pvstructs maps that we know will never be used (danger?)
        if(cameraName==selectedCamera())
            selectedDAQCamera.state = allCamDAQData[cameraName].state;//Update the selected camera object
        if(cameraName=="VC") // MAGIC_STRING
            vcDAQCamera.state = allCamDAQData[cameraName].state;//Update the VC camera object
        message(cameraName, ": State is ", ENUM_TO_STRING( allCamDAQData[cameraName].state));
    }
}
void cameraDAQInterface::updateWriteCheck(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].writeCheck = cameraStructs::WRITE_CHECK::WRITE_CHECK_OK;
                break;
            case 1:
                allCamDAQData[cameraName].writeCheck = cameraStructs::WRITE_CHECK::WRITE_CHECK_ERROR;
                break;
            default:
                allCamDAQData[cameraName].writeCheck = cameraStructs::WRITE_CHECK::WRITE_CHECK_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.writeCheck = allCamDAQData[cameraName].writeCheck;//Update the selected camera object
        if(cameraName=="VC")
            vcDAQCamera.writeCheck = allCamDAQData[cameraName].writeCheck;//Update the VC camera object
        message(cameraName, ": Write Status is ", ENUM_TO_STRING( allCamDAQData[cameraName].writeCheck));
    }
}
void cameraDAQInterface::updateWriteState(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].writeState = cameraStructs::WRITE_STATE::NOT_WRITING;
                break;
            case 1:
                allCamDAQData[cameraName].writeState = cameraStructs::WRITE_STATE::WRITING;
                break;
            default:
                allCamDAQData[cameraName].writeState = cameraStructs::WRITE_STATE::WRITING_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.writeState = allCamDAQData[cameraName].writeState;//Update the selected camera object
        if(cameraName=="VC")// MAGIC_STRING
            vcDAQCamera.writeState = allCamDAQData[cameraName].writeState;//Update the VC camera object
        message(cameraName, ": Write Status is ", ENUM_TO_STRING( allCamDAQData[cameraName].writeState));
    }
}
void cameraDAQInterface::updateAcquiring(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].acquireState = cameraStructs::ACQUIRE_STATE::NOT_ACQUIRING;
                break;
            case 1:
                allCamDAQData[cameraName].acquireState = cameraStructs::ACQUIRE_STATE::ACQUIRING;
                break;
            default:
                allCamDAQData[cameraName].acquireState = cameraStructs::ACQUIRE_STATE::ACQUIRING_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.acquireState = allCamDAQData[cameraName].acquireState;//Update the selected camera object
        if(cameraName=="VC")
            vcDAQCamera.acquireState = allCamDAQData[cameraName].acquireState;//Update the VC camera object
        message(cameraName, ": Aquiring state is ", ENUM_TO_STRING( allCamDAQData[cameraName].acquireState));
    }
}
void cameraDAQInterface::updateCapturing(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamDAQData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamDAQData[cameraName].captureState = cameraStructs::CAPTURE_STATE::NOT_CAPTURING;
                break;
            case 1:
                allCamDAQData[cameraName].captureState = cameraStructs::CAPTURE_STATE::CAPTURING;
                break;
            default:
                allCamDAQData[cameraName].captureState = cameraStructs::CAPTURE_STATE::CAPTURING_ERROR;
        }
        if(cameraName==selectedCamera())
            selectedDAQCamera.captureState = allCamDAQData[cameraName].captureState;//Update the selected camera object
        if(cameraName=="VC")
            vcDAQCamera.captureState = allCamDAQData[cameraName].captureState;//Update the VC camera object
        message(cameraName, ": Capturing state is ", ENUM_TO_STRING( allCamDAQData[cameraName].captureState));
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
void cameraDAQInterface::updateWriteErrorMessage(const void *value,const std::string&cameraName)
{
    char dummy[256];//MAGIC NUMBER
    for (int i = 0;i!=256; i++)
        dummy[i] = ((char *)value)[i];
    allCamDAQData[cameraName].writeErrorMessage = dummy;
    if(cameraName==selectedCamera())
        selectedDAQCamera.writeErrorMessage = allCamDAQData[cameraName].writeErrorMessage;//Update the selected camera object
    if(cameraName=="VC")
        vcDAQCamera.writeErrorMessage = allCamDAQData[cameraName].writeErrorMessage;//Update the VC camera object
}
///Functions Accessible to Python Controller///
bool cameraDAQInterface::collect(unsigned short &comm, const int & numbOfShots)
{   //Setup Camera for collection
    makeANewDirectory();
    bool ans=false;
    if(numbOfShots<=selectedDAQCamera.maxShots)
    {
        setNumberOfShots(numbOfShots);
        if( isAquiring(selectedCamera()) )
        {
            ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_CAPTURE].CHTYPE,
                   selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_CAPTURE].CHID,
                   &comm);
            int status = sendToEpics("ca_put", "",
                                     "Timeout trying to send Capture state.");
            if(status == ECA_NORMAL)
            {
                ans = true;
            }
            message("Caputure set to ",comm," on ",selectedDAQCamera.name," camera.");
        }
    }
    else
    {
        message("Requested number of shots too large.",
                " Please set number of shot to be less than or equal to ",
                 selectedDAQCamera.maxShots,".");
    }
    return ans;
}
bool cameraDAQInterface::save(unsigned short &comm)
{
    int startNumber(1);// MAGIC_NUMBER should this be a one or a zero?
    setStartFileNumber(startNumber);

    bool ans=false;
    if( isCollecting(selectedCamera())==0 )
    {
        ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE].CHTYPE,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE].CHID,
                &comm);
        int status = sendToEpics("ca_put", "", "Timeout trying to send Write state.");
        if(status == ECA_NORMAL)
        {
            ans = true;
        }
        message("WriteFile set to ",comm," on ",selectedDAQCamera.name," camera.");
    }
    else
        message("Camera was still collecting images when save function was called.");
    return ans;
}
bool cameraDAQInterface::collectAndSave(const int & numbOfShots)
{
    //need to make sure your python is running while this thread is active
    new std::thread(&cameraDAQInterface::staticCollectAndSave,this,numbOfShots);
    return true;
}
bool cameraDAQInterface::staticCollectAndSave(const int & numbOfShots)
{
    this->attachTo_thisCAContext();
    bool success = false;
    unsigned short go(1);
    //starting collecting images
    this->collect(go,numbOfShots);
    //wait until collecting is done....
    bool collecting=true;
    while(collecting)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); //MAGIC_NUMBER
        collecting=this->isCollecting(this->selectedCamera());;
    }
    //now start saving
    this->save(go);
    //wait until saving is done....
    bool saving=true;
    while(saving==true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); //MAGIC_NUMBER
        saving=isSaving(this->selectedCamera());
    }
    //Ensure messages update by waiting a second (is saving is quick)
    std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); //MAGIC_NUMBER
    //check how status of save
    if (this->allCamDAQData[this->selectedCamera()].writeCheck==cameraStructs::WRITE_CHECK::WRITE_CHECK_OK)
    {
        success = true;
        this->message("Successful wrote image to disk.");
    }
    else
        this->debugMessage("WRITE ERROR: ",this->getWriteErrorMessage());

    return success;

}
bool cameraDAQInterface::killCollectAndSave()
{
    bool killed = false;
    //if collecting then stop collecting
    if (isCollecting(selectedCamera()))
    {
        message("Killing while collecting...");
        unsigned short c(0);
        int dummyNumber(1);
        killed=collect(c,dummyNumber);
    }
    //if saving then stop saving
    else if (isSaving(selectedCamera()))
    {
        message("Killing while saving...");
        unsigned short s(0);
        killed=save(s);
    }
    //currently this function works but setting the save to 0 does not forcable stop the process!!!!!
    return killed;
}
const cameraStructs::cameraDAQObject &cameraDAQInterface::getCamDAQObjConstRef(const std::string &cam)
{
    std::string cameraName = useCameraFrom(cam);
    if(entryExists(allCamDAQData, cameraName))
    {
        return allCamDAQData[ cameraName ];
    }
    else
    {
        for(auto && it : allCamDAQData)
        {
            if(it.second.pvRoot == cameraName)
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
const cameraStructs::cameraDAQObject &cameraDAQInterface::getVCDAQRef()
{
    return vcDAQCamera;
}
///Useful Functions for the Class///
bool cameraDAQInterface::makeANewDirectory()
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

    // this data needs to go into the object as a string

   // char dir[256];//MAGIC NUMBER
   // strncpy(dir, newPath.c_str(),256);//MAGIC_NUMBER

    ca_array_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH].CHTYPE,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH].COUNT,
                selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH].CHID,
                &newPath);

    int status = sendToEpics("ca_put", "", "Timeout trying to send new file path state.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        message("New path set to ",newPath," on ",selectedDAQCamera.name," camera.");
    }
    return ans;
}
// this should be monitored...
// and in the object
std::string cameraDAQInterface::getWriteErrorMessage()
{
    return selectedDAQCamera.writeErrorMessage;
}
// there is actuall ya STE and RBV version of this parameter
bool cameraDAQInterface::setNumberOfShots(const int &numberOfShots)
{
    bool ans = false;
    ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE].CHTYPE,
           selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE].CHID,
           &numberOfShots);
    int status = sendToEpics("ca_put", "", "Timeout trying to send NumCapture.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        message("New number of shots to take to be ", numberOfShots);
    }
    return ans;
}
bool cameraDAQInterface::setStartFileNumber(const int &startNumber)
{
    bool ans = false;
    ca_put(selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_NUMBER].CHTYPE,
           selectedDAQCamera.pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_FILE_NUMBER].CHID,
           &startNumber);
    int status = sendToEpics("ca_put", "", "Timeout trying to send start number.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        message("New File numbering start from ", startNumber);
    }
    return ans;
}
