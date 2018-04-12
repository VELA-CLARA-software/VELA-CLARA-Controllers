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
                                  bool& show_messages_ptr,
                                  bool& show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const HWC_ENUM::MACHINE_AREA myMachineArea):
configReader(Conf,startVirtualMachine,show_messages_ptr,show_debug_messages_ptr),
cameraInterface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs(shouldStartEPICs),
myMachineArea(myMachineArea)
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
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));//MAGIC_NUMBER
    if(configFileRead)
    {
        message("The cameraDAQInterface has read the config file, acquiring objects");
        // initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The cameraDAQInterface has acquired objects, connecting to EPICS");
                initChids();// subscribe to the channel ids
                startMonitors();// start the monitors: set up the callback functions
                // The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));//MAGIC_NUMBER
            }
            else
                message("The cameraDAQInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message("!!!The cameraDAQInterface received an Error while getting Camera DAQ data!!!");
    }
    else
        message("The cameraDAQInterface  Failed to Read the configFile.");
}
void cameraDAQInterface::killMonitor(monitorDAQStruct *ms)
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
bool cameraDAQInterface::initObjects()
{
    bool camDatSuccess = configReader.getCamDAQData( allCamData );
    vcCameraObj = allCamData.at("VC"); //MAGIC_STRING
    setCamera("VC");
    for( auto && it : allCamData )
        it.second.machineArea = myMachineArea;
    return camDatSuccess;
}
void cameraDAQInterface::initChids()
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
void cameraDAQInterface::startMonitors()
{
    continuousMonitorDAQStructs.clear();

    for(auto && it : allCamData)
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
    int status = sendToEpics( "ca_create_subscription",
                             "Succesfully Subscribed to Camera  DAQ Monitors",
                             "!!TIMEOUT!! Subscription to Camera DAQ monitors failed");
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
void cameraDAQInterface::staticEntryDAQMonitor(const event_handler_args args)
{
    monitorDAQStruct*ms = static_cast<monitorDAQStruct*>(args.usr);
    switch(ms -> monType)
    {
        case CAM_PV_TYPE::CAM_ACQUIRE_RBV:
            ms->interface->updateAcquiring( *(unsigned short*)args.dbr, ms->objName);
            break;
        case CAM_PV_TYPE::CAM_CAPTURE_RBV:
            ms->interface->updateCapturing( *(unsigned short*)args.dbr, ms->objName);
            break;
        case CAM_PV_TYPE::CAM_STATUS:
            ms->interface->updateState( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_FILE_WRITE_RBV:
            ms->interface->updateWriteState( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_FILE_WRITE_CHECK:
            ms->interface->updateWriteCheck( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_NUM_CAPTURED:
            ms->interface->updateNumCaptured( *(unsigned long*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_NUM_CAPTURE_RBV:
            ms->interface->updateNumCapture( *(unsigned long*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_FILE_WRITE_MESSAGE:
            ms->interface->updateWriteErrorMessage(args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_EXPOSURE_TIME:
            ms->interface->updateExposure(*(double*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_ACQUIRE_PERIOD:
            ms->interface->updateAcquirePeriod(*(double*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_FREQ:
            ms->interface->updateFrequency(*(double*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::CAM_SENSOR_TEMP:
            ms->interface->updateSensorTemp(*(double*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::JPG_CAPTURE_RBV:
            ms->interface->updateCapturingJPG( *(unsigned short*)args.dbr, ms->objName);
            break;

        case CAM_PV_TYPE::JPG_NUM_CAPTURED:
            ms->interface->updateNumCapturedJPG(*(double*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::JPG_NUM_CAPTURE_RBV:
            ms->interface->updateNumCaptureJPG(*(double*)args.dbr, ms->objName );
            break;

        case CAM_PV_TYPE::JPG_FILE_WRITE_RBV:
            ms->interface->updateWriteStateJPG( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::JPG_FILE_WRITE_CHECK:
            ms->interface->updateWriteCheckJPG( *(unsigned short*)args.dbr, ms->objName );
            break;
        case CAM_PV_TYPE::JPG_FILE_WRITE_MESSAGE:
            ms->interface->updateWriteErrorMessageJPG(args.dbr, ms->objName );
            break;
        default:
            ms->interface->debugMessage("!!! ERROR !!! Unknown Monitor Type passed to cameraDAQInterface::staticEntryMonitor");
            break;
    }
}
void cameraDAQInterface::updateState(const unsigned short value,const std::string&cameraName)
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
void cameraDAQInterface::updateWriteCheck(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).DAQ.writeCheck = WRITE_CHECK::WRITE_CHECK_OK;
                break;
            case 1:
                allCamData.at(cameraName).DAQ.writeCheck = WRITE_CHECK::WRITE_CHECK_ERROR;
                break;
            default:
                allCamData.at(cameraName).DAQ.writeCheck = WRITE_CHECK::WRITE_CHECK_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Write Status is ",
                ENUM_TO_STRING(allCamData.at(cameraName).DAQ.writeCheck));
    }
}
void cameraDAQInterface::updateWriteState(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).DAQ.writeState = WRITE_STATE::NOT_WRITING;
                break;
            case 1:
                allCamData.at(cameraName).DAQ.writeState = WRITE_STATE::WRITING;
                break;
            default:
                allCamData.at(cameraName).DAQ.writeState = WRITE_STATE::WRITING_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Write Status is ",
                ENUM_TO_STRING(allCamData.at(cameraName).DAQ.writeState));
    }
}
void cameraDAQInterface::updateAcquiring(const unsigned short value,const std::string&cameraName)
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
void cameraDAQInterface::updateCapturing(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).DAQ.captureState = CAPTURE_STATE::NOT_CAPTURING;
                break;
            case 1:
                allCamData.at(cameraName).DAQ.captureState = CAPTURE_STATE::CAPTURING;
                break;
            default:
                allCamData.at(cameraName).DAQ.captureState = CAPTURE_STATE::CAPTURING_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Capturing state is ",
                ENUM_TO_STRING(allCamData.at(cameraName).DAQ.captureState));
    }
}
void cameraDAQInterface::updateNumCaptured(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.shotsTaken = value;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateNumCapture(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.numberOfShots = value;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateNumCaptureJPG(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.numberOfShotsJPG = value;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateNumCapturedJPG(const unsigned long value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.shotsTakenJPG = value;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateWriteErrorMessage(const void *value,const std::string&cameraName)
{
    char dummy[256];//MAGIC NUMBER
    for (int i = 0;i!=256; i++)//MAGIC_NUMBER
        dummy[i] = ((char *)value)[i];
    allCamData.at(cameraName).DAQ.writeErrorMessage = dummy;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateExposure(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.exposureTime = value;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateAcquirePeriod(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.acquisitionPeriod = value;

    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateFrequency(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.frequency = value;
    updateSelectedOrVC(cameraName);
}
void cameraDAQInterface::updateSensorTemp(const double value,const std::string&cameraName)
{
    allCamData.at(cameraName).DAQ.sensorTemp = value;
    updateSelectedOrVC(cameraName);
    // If the sensor temp of the camera is out of operable range (5-10 celcius)
    //kill any collecting of data then stop any rolling aquiring that is in progress.
    if (selectedCameraObj.DAQ.sensorTemp>10||selectedCameraObj.DAQ.sensorTemp<5)
    {
        killCollectAndSave();
        stopAcquiring();
        message("WARNING: Sensor temp is out correct operable temperature!!");
        message("   1. Temperature of sensor on ",selectedCameraObj.name," is: ",selectedCameraObj.DAQ.sensorTemp,"C");
        message("   2. Please CHECK FAN and FILTER (CONSULT SOMEONE).");
    }
    if (vcCameraObj.DAQ.sensorTemp>10||vcCameraObj.DAQ.sensorTemp<5)
    {
        killCollectAndSaveVC();
        stopVCAcquiring();
        message("WARNING: Sensor temp is out correct operable temperature!!");
        message("   1. Temperature of sensor on ",vcCameraObj.name," is: ",vcCameraObj.DAQ.sensorTemp,"C");
        message("   2. Please CHECK FAN and FILTER (CONSULT SOMEONE).");
    }
}
void cameraDAQInterface::updateWriteCheckJPG(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).DAQ.writeCheckJPG = WRITE_CHECK::WRITE_CHECK_OK;
                break;
            case 1:
                allCamData.at(cameraName).DAQ.writeCheckJPG = WRITE_CHECK::WRITE_CHECK_ERROR;
                break;
            default:
                allCamData.at(cameraName).DAQ.writeCheckJPG = WRITE_CHECK::WRITE_CHECK_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Write Status is ",
                ENUM_TO_STRING(allCamData.at(cameraName).DAQ.writeCheckJPG));
    }
}
void cameraDAQInterface::updateWriteStateJPG(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).DAQ.writeStateJPG = WRITE_STATE::NOT_WRITING;
                break;
            case 1:
                allCamData.at(cameraName).DAQ.writeStateJPG = WRITE_STATE::WRITING;
                break;
            default:
                allCamData.at(cameraName).DAQ.writeStateJPG = WRITE_STATE::WRITING_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Write Status is ",
                ENUM_TO_STRING(allCamData.at(cameraName).DAQ.writeStateJPG));
    }
}
void cameraDAQInterface::updateCapturingJPG(const unsigned short value,const std::string&cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        switch(value)
        {
            case 0:
                allCamData.at(cameraName).DAQ.captureStateJPG = CAPTURE_STATE::NOT_CAPTURING;
                break;
            case 1:
                allCamData.at(cameraName).DAQ.captureStateJPG = CAPTURE_STATE::CAPTURING;
                break;
            default:
                allCamData.at(cameraName).DAQ.captureStateJPG = CAPTURE_STATE::CAPTURING_ERROR;
        }
        updateSelectedOrVC(cameraName);
        message(cameraName, ": Capturing state is ",
                ENUM_TO_STRING(allCamData.at(cameraName).DAQ.captureStateJPG));
    }
}
void cameraDAQInterface::updateWriteErrorMessageJPG(const void *value,const std::string&cameraName)
{
    char dummy[256];//MAGIC NUMBER
    for (int i = 0;i!=256; i++)//MAGIC_NUMBER
        dummy[i] = ((char *)value)[i];
    allCamData.at(cameraName).DAQ.writeErrorMessageJPG = dummy;
    updateSelectedOrVC(cameraName);
}
///Functions Accessible to Python Controller///
bool cameraDAQInterface::collect(cameraObject camera, unsigned short &comm, const int & numbOfShots)
{
    bool ans=false;
    pvStruct S(camera.pvComStructs.at(CAM_PV_TYPE::CAM_CAPTURE));

    //makeANewDirectory();

    if(numbOfShots<=camera.DAQ.maxShots)
    {
        setNumberOfShots(numbOfShots);
        if( isAcquiring(camera.name) )
        {
            ans=shortCaput(comm,S);
            message("Caputure set to ",comm,
                    " on ",camera.name," camera.");
        }
    }
    else
    {
        message("Requested number of shots too large.",
                " Please set number of shot to be less than or equal to ",
                 camera.DAQ.maxShots,".");
    }

    return ans;
}
bool cameraDAQInterface::save(cameraObject camera, unsigned short &comm)
{
    bool ans=false;
    int startNumber(1);// MAGIC_NUMBER should this be a one or a zero?
    pvStruct S(camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_WRITE));

    setStartFileNumberJPG(startNumber);

    if( isCollecting(camera.name)==0 )
    {
        ans=shortCaput(comm,S);
        message("WriteFile set to ",comm,
                " on ",camera.name," camera.");
    }
    else
        message("Still collecting images when save function was called.");

    return ans;
}
bool cameraDAQInterface::collectJPG(cameraObject camera, unsigned short &comm)
{
    bool ans=false;
    pvStruct S(camera.pvComStructs.at(CAM_PV_TYPE::JPG_CAPTURE));

        int oneShot(1);//MAGIG NUMBE ONLY ALLOWING THIS FUNCTION TO TAKE ONE JPG!!!!!
        setNumberOfShotsJPG(oneShot);
        if( isAcquiring(camera.name) )
        {
            ans=shortCaput(comm,S);
            message(" JPG Caputure set to ",comm,
                    " on ",camera.name," camera.");
        }


    return ans;
}
bool cameraDAQInterface::saveJPG(cameraObject camera, unsigned short &comm)
{
    bool ans=false;
    int startNumber(1);// MAGIC_NUMBER
    pvStruct S(camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_WRITE));

    setStartFileNumberJPG(startNumber);

    if( isCollectingJPG(camera.name)==0 )
    {
        ans=shortCaput(comm,S);
        message("WriteFile set to ",comm,
                " on ",camera.name," camera.");
    }
    else
        message("Still collecting images when save function was called.");

    return ans;
}
bool cameraDAQInterface::collectAndSave(const int & numbOfShots)
{
    //need to make sure your python is running while this thread is active
    new std::thread(&cameraDAQInterface::staticCollectAndSave,this,selectedCameraObj,numbOfShots);
    return true;
}
bool cameraDAQInterface::collectAndSaveVC(const int & numbOfShots)
{
    //need to make sure your python is running while this thread is active
    new std::thread(&cameraDAQInterface::staticCollectAndSave,this,vcCameraObj, numbOfShots);
    return true;
}
bool cameraDAQInterface::collectAndSaveJPG()
{
    //need to make sure your python is running while this thread is active
    new std::thread(&cameraDAQInterface::staticCollectAndSaveJPG,this,selectedCameraObj);
    return true;
}
bool cameraDAQInterface::staticCollectAndSave(cameraObject camera, const int & numbOfShots)
{
    this->attachTo_thisCAContext();
    bool success = false;
    unsigned short go(1);

    this->collect(camera,go,numbOfShots);//starting collecting images

    bool collecting=true;
    while(collecting)   //wait until collecting is done...
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); //MAGIC_NUMBER
        collecting=this->isCollecting(camera.name);;
    }

    makeANewDirectoryAndName(camera,numbOfShots);

    this->save(camera,go);//start saving

    bool saving=true;
    while(saving==true)    //wait until saving is done...
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); //MAGIC_NUMBER
        saving=isSaving(camera.name);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); //MAGIC_NUMBER

    //check status of save
    if (this->allCamData.at(camera.name).DAQ.writeCheck==WRITE_CHECK::WRITE_CHECK_OK)
        success = true;//this->message("Successful wrote image to disk.");
    else
        this->debugMessage("WRITE ERROR: ",camera.DAQ.writeErrorMessage);///!!!!!!!!!!!!!!!!!!!!!!!

    return success;

}
bool cameraDAQInterface::staticCollectAndSaveJPG(cameraObject camera)
{
    this->attachTo_thisCAContext();
    bool success = false;
    unsigned short go(1);

    this->collectJPG(camera,go);//starting collecting images

    bool collecting=true;
    while(collecting)   //wait until collecting is done...
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); //MAGIC_NUMBER
        collecting=this->isCollectingJPG(camera.name);;
    }

    makeANewDirectoryAndNameJPG(camera,1);

    this->saveJPG(camera,go);//start saving

    bool saving=true;
    while(saving==true)    //wait until saving is done...
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); //MAGIC_NUMBER
        saving=isSavingJPG(camera.name);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); //MAGIC_NUMBER

    //check status of save
    if (this->allCamData.at(camera.name).DAQ.writeCheckJPG==WRITE_CHECK::WRITE_CHECK_OK)
        success = true;//this->message("Successful wrote image to disk.");
    else
        this->debugMessage("WRITE ERROR: ",camera.DAQ.writeErrorMessageJPG);///!!!!!!!!!!!!!!!!!!!!!!!

    return success;

}
bool cameraDAQInterface::killCollectAndSave()///can only kill while saving.
{
    bool killed = false;
    unsigned short c(0);
    int dummyNumber(1);
    if (isCollecting(selectedCamera()))//stop collecting
    {
        message("Killing while collecting...");
        killed=collect(selectedCameraObj,c,dummyNumber);
    }
    else if (isSaving(selectedCamera()))//stop saving
    {
        message("Killing while saving...");
        killed=save(selectedCameraObj,c);
    }

    return killed;
}
bool cameraDAQInterface::killCollectAndSaveVC()///can only kill while saving.
{
    bool killed = false;
    unsigned short c(0);
    int dummyNumber(1);
    if (isCollecting("FAKE_VC"))//stop collecting
    {
        message("Killing while collecting...");
        killed=collect(vcCameraObj,c,dummyNumber);
    }
    else if (isSaving("FAKE_VC"))//stop saving
    {
        message("Killing while saving...");
        killed=save(vcCameraObj,c);
    }

    return killed;
}
bool cameraDAQInterface::killCollectAndSaveJPG()///can only kill while saving.
{
    bool killed = false;
    unsigned short c(0);
    int dummyNumber(1);
    if (isCollectingJPG(selectedCamera()))//stop collecting
    {
        message("Killing JPG while collecting...");
        killed=collectJPG(selectedCameraObj,c);
    }
    else if (isSavingJPG(selectedCamera()))//stop sa
    {
        message("Killing JPG while saving...");
        killed=saveJPG(selectedCameraObj,c);
    }

    return killed;
}
std::string cameraDAQInterface::getlatestDirectory()
{
    return selectedCameraObj.DAQ.latestDirectory;
}
const cameraObject &cameraDAQInterface::getCamDAQObjConstRef(const std::string &cam)///THIS NEEDS FIXING  at "NOPE"
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
const cameraObject &cameraDAQInterface::getSelectedDAQRef()
{
    return selectedCameraObj;
}
const cameraObject &cameraDAQInterface::getVCDAQRef()
{
    return vcCameraObj;
}

///Useful Functions for the Class///
bool cameraDAQInterface::makeANewDirectoryAndName(cameraObject &camera,const int &numbOfShots)///YUCK (make it look nice)
{
    bool ans=false;
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);
    //tm local_tm = *localtime(&t);
    struct tm local_tm;
    localtime_s(&local_tm, &t);
    char  newPath[256]="/home/images/";

    char  newName[256]="defaultname";
    std::string strName = camera.name+
                        "_"+std::to_string(local_tm.tm_year + 1900)+
                        "-"+std::to_string(local_tm.tm_mon + 1)+
                        "-"+std::to_string(local_tm.tm_mday)+
                        "_"+std::to_string(local_tm.tm_hour)+
                        "-"+std::to_string(local_tm.tm_min)+
                        "-"+std::to_string(local_tm.tm_sec)+
                        "_"+std::to_string(numbOfShots)+"images";
    strcpy(newName, strName.c_str());
    message("File Directory would be: ",
                        std::to_string(local_tm.tm_year + 1900)+
                        "\\"+std::to_string(local_tm.tm_mon + 1)+
                        "\\"+std::to_string(local_tm.tm_mday)+"\\");
    message("File name would be: ",newName);

    ca_array_put(camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NAME).CHTYPE,
                 camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NAME).COUNT,
                 camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NAME).CHID,
                 &newName);
    ca_array_put(camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).CHTYPE,
                 camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).COUNT,
                 camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).CHID,
                 &newPath);

    int status = sendToEpics("ca_put","","Timeout trying to send new file path state.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        camera.DAQ.latestDirectory = newPath;
        message("New path set to ",newPath," on ",camera.name," camera.");
    }

    return ans;
}
bool cameraDAQInterface::makeANewDirectoryAndNameJPG(cameraObject &camera,const int &numbOfShots)///YUCK (make it look nice)
{
    bool ans=false;
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);
    //tm local_tm = *localtime(&t);
    struct tm local_tm;
    localtime_s(&local_tm, &t);
    char  newPath[256]="/home/images/";

    char  newName[256]="defaultname";
    std::string strName = camera.name+
                        "_"+std::to_string(local_tm.tm_year + 1900)+
                        "-"+std::to_string(local_tm.tm_mon + 1)+
                        "-"+std::to_string(local_tm.tm_mday)+
                        "_"+std::to_string(local_tm.tm_hour)+
                        "-"+std::to_string(local_tm.tm_min)+
                        "-"+std::to_string(local_tm.tm_sec)+
                        "_"+std::to_string(numbOfShots)+"images";
    strcpy(newName, strName.c_str());
    message("File Directory would be: ",
                        std::to_string(local_tm.tm_year + 1900)+
                        "\\"+std::to_string(local_tm.tm_mon + 1)+
                        "\\"+std::to_string(local_tm.tm_mday)+"\\");
    message("File name would be: ",newName);

    ca_array_put(camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_NAME).CHTYPE,
                 camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_NAME).COUNT,
                 camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_NAME).CHID,
                 &newName);
    ca_array_put(camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_PATH).CHTYPE,
                 camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_PATH).COUNT,
                 camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_PATH).CHID,
                 &newPath);

    int status = sendToEpics("ca_put","","Timeout trying to send new file path state.");
    if(status == ECA_NORMAL)
    {
        ans = true;
        camera.DAQ.latestDirectory = newPath;
        message("New path set to ",newPath," on ",camera.name," camera.");
    }

    return ans;
}
std::string cameraDAQInterface::getWriteErrorMessage()
{
    return selectedCameraObj.DAQ.writeErrorMessage;
}
std::string cameraDAQInterface::getWriteErrorMessageJPG()
{
    return selectedCameraObj.DAQ.writeErrorMessageJPG;
}
bool cameraDAQInterface::setNumberOfShots(const int &numberOfShots)
{
    bool ans(false);
    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::CAM_NUM_CAPTURE));
    ans=shortCaput(numberOfShots,S);
    if (ans==true)
        message("New number of shots to take to be ", numberOfShots);
    return ans;
}
bool cameraDAQInterface::setNumberOfShotsJPG(const int &numberOfShots)
{
    bool ans(false);
    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::JPG_NUM_CAPTURE));
    ans=shortCaput(numberOfShots,S);
    if (ans==true)
        message("New number of shots to take to be ", numberOfShots);
    return ans;
}
bool cameraDAQInterface::setStartFileNumber(const int &startNumber)
{
    bool ans(false);
    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NUMBER));
    ans=shortCaput(startNumber,S);
    if (ans==true)
        message("New File numbering start from ", startNumber);
    return ans;
}
bool cameraDAQInterface::setStartFileNumberJPG(const int &startNumber)
{
    bool ans(false);
    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_NUMBER));
    ans=shortCaput(startNumber,S);
    if (ans==true)
        message("New File numbering start from ", startNumber);
    return ans;
}
void cameraDAQInterface::updateSelectedOrVC(const std::string cameraName)
{
    if(entryExists(allCamData,cameraName))
    {
        if(cameraName==selectedCamera())
            selectedCameraObj = allCamData.at(cameraName);
        if(cameraName=="VC")
            vcCameraObj = allCamData.at(cameraName);
    }

}

