/*
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
//
//  Author:      DJS
//  Last edit:   07-06-2018
//  FileName:    cameraBase.cpp
//  Description: implementation for all cameras (hoepfully), inherit from this
//               class to actually create a camera controller
//
//
//*/
// project includes
#include "cameraBase.h"
// stl includes
#include <limits>
#include <iterator>
#include <set>
#include <list>
using namespace cameraStructs;
//---------------------------------------------------------------------------------
cameraBase::cameraBase(bool& show_messages,
                       bool& show_debug_messages,
                       const bool startVirtualMachine,
                       const bool shouldStartEPICs):
cameraBase(show_messages,show_debug_messages,
           startVirtualMachine,shouldStartEPICs,
           UTL::UNKNOWN_STRING)
{}
//---------------------------------------------------------------------------------
cameraBase::cameraBase(bool& show_messages,
                       bool& show_debug_messages,
                       const bool startVirtualMachine,
                       const bool shouldStartEPICs,
                       const std::string& claraCamConfig):
cameraBase(show_messages,show_debug_messages,
           startVirtualMachine,shouldStartEPICs,
           claraCamConfig,UTL::UNKNOWN_STRING,
           HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA)
{}
//---------------------------------------------------------------------------------
cameraBase::cameraBase(bool& show_messages,
                       bool& show_debug_messages,
                       const bool startVirtualMachine,
                       const bool shouldStartEPICs,
                       const std::string& claraCamConfig,
                       const std::string& velaCamConfig,
                       const HWC_ENUM::MACHINE_AREA area
                      ):
claraCamConfigReader(claraCamConfig, show_messages, show_debug_messages,startVirtualMachine),
velaCamConfigReader(velaCamConfig, show_messages, show_debug_messages, startVirtualMachine),
interface(show_messages,show_debug_messages, shouldStartEPICs,startVirtualMachine),
selectedCamPtr(nullptr),
vcCamPtr(nullptr),
myarea(area)
{
    dummyCamObject.name = UTL::DUMMY_NAME;
    attachTo_thisCAContext();
    message("cameraBase, ",thisCaContext);
}
//---------------------------------------------------------------------------------
cameraBase::~cameraBase()
{}
//---------------------------------------------------------------------------------
void cameraBase::initialise(bool VConly = false)
{
    std::cout << "cameraBase::initialise()" << std::endl;

    configFileRead = readCamConfig();

    if(configFileRead)
    {
        message("The cameraBase has read the config file, "
                "acquiring objects");
        /*initialise the objects based on what is read from the config file */
        bool getDataSuccess = getCamObjects();
        if(VConly)
        {
            message("Getting ", UTL::VIRTUAL_CATHODE," camera only");
            getDataSuccess  = vcOnly();
        }
        else
        {
            initImageCollectStructs();
            // set the camera References
            std::string cameraName = useCameraFrom(UTL::VIRTUAL_CATHODE);
            if(entryExists( allCamData, cameraName))
            {
                vcCamPtr = &allCamData.at(cameraName);
                message("Set vcCamPtr to camera = ",vcCamPtr->name);
                /*
                    selectedCamPtr should be set to something so why not VC?
                    This is a bit dangerous, potentially points to nothing ...
                */
                selectedCamPtr = &allCamData.at(cameraName);
            }
        }
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The cameraBase has acquired objects, "
                        "connecting to EPICS");
                /*
                    subscribe to the channel ids
                    true sets flag to send to EPICS
                */
                initCamChids(true);
                /*
                    start the monitors: set up the callback functions
                */
                startCamMonitors(true);
                /*
                    The pause allows EPICS callbacks to catch up.
                */
                pause_500();
            }
            else
                message("The cameraBase has acquired objects, "
                        "NOT connecting to EPICS");
        }
        else
            message("!!!The cameraBase received an Error "
                    "while getting camera data!!!");
    }
}
//---------------------------------------------------------------------------------
bool cameraBase::readCamConfig()
{
    /*
        read config files, vela cam not yet written
    */
    bool clara_camera_data_read = claraCamConfigReader.readConfig();
    bool vela_camera_data_read = velaCamConfigReader.readConfig();;
    if(clara_camera_data_read && vela_camera_data_read)
        return true;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::getCamObjects()
{
    /*
        copy objects from config reader
        depending on the machine area we kepp on the vela or clara VIRTUAL_CATHODE
    */
    bool ans = false;
    bool cut_vela_vc = false;
    bool cut_clara_vc = false;
    if(myarea == HWC_ENUM::MACHINE_AREA::CLARA)
    {
        message("Machine Area = CLARA removing VELA VIRTUAL_CATHODE");
        cut_vela_vc  = true;
        cut_clara_vc = false;
    }
    else if(myarea == HWC_ENUM::MACHINE_AREA::VELA)
    {
        message("Machine Area = VELA removing CLARA VIRTUAL_CATHODE");
        cut_vela_vc  = false;
        cut_clara_vc = true;
    }
    ans = claraCamConfigReader.getCamData(allCamData, cut_clara_vc);
    if(ans)
        ans = velaCamConfigReader.getCamData(allCamData, cut_vela_vc);
    if(ans)
    {
        /*
            some camerStructs objects hold pointers to this interface,
            set them here;
        */
        for(auto&& it : allCamData)
        {
            it.second.data.mask.maskInterface = this;
            it.second.data.image.fast_image_interface = this;
        }
        /*
            set the "fast" image array size
        */
        size_t s;
        for(auto&& it : allCamData)
        {
            s = (size_t)it.second.pvComStructs.at(CAM_PV_TYPE::ARRAY_DATA).COUNT;
            it.second.data.image.array_data.clear();

            it.second.data.image.array_data.resize(s);

            message("Resizing camera ",it.first,
                    " array_data to ",
                    it.second.data.image.array_data.size());
        }
        /*
            set names of objects to match camera/screen name
        */
        for(auto&& it : allCamData)
        {
            it.second.data.image.name = it.first;
            it.second.data.mask.name = it.first;
            it.second.daq.name   = it.first;
            it.second.daq.screenName = it.second.screenName;
        }
        /*
            check that the screen names are all unique (we tend to refer to cameras
            using the name of teh screen they display
        */
        std::vector<std::string> temp_screen_name,intermediate;
        for(auto&& it : allCamData)
        {
            temp_screen_name.push_back(it.second.screenName);
        }
        std::sort(temp_screen_name.begin(), temp_screen_name.end());
        std::set<std::string> uvec(temp_screen_name.begin(), temp_screen_name.end());
        std::list<std::string> output;
        std::set_difference(temp_screen_name.begin(), temp_screen_name.end(),
                            uvec.begin(), uvec.end(),
                            std::back_inserter(output));

        message("temp_screen_name.size() =  ",temp_screen_name.size() );
        message("uvec.size() =  ",uvec.size() );
        message("output.size() =  ",output.size() );

        if(output.size() != UTL::ZERO_SIZET)
        {
            message("!!ERROR!! CONFIG FILES DO NOT HAVE UNIQUE NAMES FOR THE CAMERA SCREENS, STOPPING ");
            message("Below are the non-unique screen names:");
            ans = false;
            for(auto&&it:output)
            {
                message(it);
            }
        }
    }
    return ans;
}
//---------------------------------------------------------------------------------
bool cameraBase::vcOnly()
{
    /*
        deletes all cameraObjects apart from the VIRTUAL_CATHODE
    */
    std::string cameraName = useCameraFrom(UTL::VIRTUAL_CATHODE);
    if(entryExists( allCamData, cameraName))
    {
        for (auto it = allCamData.cbegin(); it != allCamData.cend() /* not hoisted */; /* no increment */)
        {
            if ( it->first != cameraName)
            {
                it = allCamData.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    if( allCamData.size() == UTL::ONE_SIZET)
    {
        for(auto&&it:allCamData)
        {
            if( it.first == cameraName)
            {
                message("Succesfully cut cameraobjects to just ", cameraName);
                /*
                    all references must refer to VC
                */
                selectedCamPtr = &allCamData.at(cameraName);
                vcCamPtr       = &allCamData.at(cameraName);
                initImageCollectStructs();
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------------
void cameraBase::initImageCollectStructs()
{
    /*
        imageCollectStructs is a map of imageCollectStruct, keyed by camname
        they are passed to the new threads that are created when asking a CLARA-CAM
        to collect and save (write)
    */
    for(auto&&it:allCamData)
    {
        debugMessage("initImageCollectStructs added ", it.first);
        imageCollectStructs[it.first] = imageCollectStruct();
        imageCollectStructs.at(it.first).camInterface = this;
        imageCollectStructs.at(it.first).camObj = &it.second;
        imageCollectStructs.at(it.first).thread = nullptr;
        imageCollectStructs.at(it.first).success =false;
        imageCollectStructs.at(it.first).isBusy = false;
    }
}
//---------------------------------------------------------------------------------
void cameraBase::initCamChids(bool sendToEPICS = false)
{
    for(auto&& it:allCamData)
    {
        debugMessage("\ncameraBase Create channel to monitor PVs\n");
        for(auto&& mon_it:it.second.pvMonStructs)
        {
            addCamChannel(it.second.pvRoot, mon_it.second);
        }
        debugMessage("\ncameraBase Create channel to command PVs\n");
        for(auto&& com_it:it.second.pvComStructs)
        {
            addCamChannel(it.second.pvRoot, com_it.second);
        }
    }
    if(sendToEPICS)
    {
        int status = sendToEpics("ca_create_channel",
                                 "Found Camera ChIds.",
                                 "!!TIMEOUT!! Not all Camera ChIds found.");
        if(status == ECA_TIMEOUT)
        {
            pause_500();
            message("\n", "Checking camera ChIds.");
            for(auto&& it:allCamData)
            {
                message("\n", it.first);
                for(auto&& mon_it:it.second.pvMonStructs)
                {
                    checkCHIDState(mon_it.second.CHID, ENUM_TO_STRING(mon_it.first));
                }
                for(auto&& com_it:it.second.pvComStructs)
                {
                    checkCHIDState(com_it.second.CHID, ENUM_TO_STRING(com_it.first));
                }
            }
            pause_2000();
        }
        else if(status == ECA_NORMAL)
        {
            for(auto&& it:allCamData)
            {
                debugMessage("\ncameraBase Create channel to monitor PVs\n");
                for(auto&& mon_it: it.second.pvMonStructs)
                {
                    updateChannelCountandType(mon_it.second);
                }
                debugMessage("\ncameraBase Create channel to command PVs\n");
                for(auto&& com_it: it.second.pvComStructs)
                {
                    updateChannelCountandType(com_it.second);
                }
            }
            allChidsInitialised = true;  /// interface base class member
        }
    }
}
//---------------------------------------------------------------------------------
void cameraBase::addCamChannel(const std::string& pvRoot, pvStruct& pv)
{
//    something is goin gwrong, these channels dont work when called from th eparent calss
//    is it the pointer in th ecamCOntrolerlbase?
//    also you can't return the  data from this classes allCamadataObjects
//    instead you get th edumy alues !!

    const std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    //pv.CHTYPE = ca_field_type(pv.CHID);
    //pv.COUNT  = ca_element_count(pv.CHID);
    debugMessage("Create channel to ", s);
}
//---------------------------------------------------------------------------------
void cameraBase::updateChannelCountandType(pvStruct& pv)
{
    auto ch = pv.CHTYPE;
    auto co = pv.COUNT;
    setChannelCountandType(pv.CHID, pv.CHTYPE, pv.COUNT);
    if(ch != pv.CHTYPE)
    {
        if(co != pv.COUNT)
        {
            message(ENUM_TO_STRING(pv.pvType)," ",ch," ", co);
            message(ENUM_TO_STRING(pv.pvType)," ",pv.CHTYPE," ", pv.COUNT);
        }
    }
}
//---------------------------------------------------------------------------------
bool cameraBase::startCamMonitors(bool sendToEPICS = false)
{
    continuousCamMonitorStructs.clear();

    for(auto && it : allCamData)
    {
        for(auto && it2 : it.second.pvMonStructs)
        {
            continuousCamMonitorStructs.push_back(new monitorStruct());
            continuousCamMonitorStructs.back()->monType = it2.first;
            continuousCamMonitorStructs.back()->objName = it.first;
            continuousCamMonitorStructs.back()->camInterface = this;
            continuousCamMonitorStructs.back() -> CHTYPE  = it2.second.CHTYPE;
            continuousCamMonitorStructs.back() -> EVID  = it2.second.EVID;
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   cameraBase::staticEntryCamMonitor,
                                   (void*)continuousCamMonitorStructs.back(),
                                   &continuousCamMonitorStructs.back()->EVID);
            debugMessage("Adding monitor for ",  it.second.name, " ", ENUM_TO_STRING(it2.first));
        }
    }
    bool ans = true;
    if(sendToEPICS)
    {
        int status = sendToEpics("ca_create_channel",
                                 "Found Camera ChIds.",
                                 "!!TIMEOUT!! Not all Camera ChIds found.");
        if(status == ECA_TIMEOUT )
        {
            ans = false;
        }
        else if (status == ECA_NORMAL)
            allChidsInitialised = true;  /// interface base class member
    }
    return ans;
}
//--------------------------------------------------------------------------------
void cameraBase::staticEntryCamMonitor(const event_handler_args args)
{
    monitorStruct* ms = static_cast<monitorStruct*>(args.usr);
    ms->camInterface->updateCamValue(ms->monType,ms->objName, args);
}
//--------------------------------------------------------------------------------
void cameraBase::updateCamValue(const CAM_PV_TYPE pv, const std::string& objName,
                                const event_handler_args& args)
{

    using namespace cameraStructs;
    cameraObject& camObj  = allCamData.at(objName);
    switch(pv)
    {
/*
            CLARA CAMER STATES, ON/OFF, ACQUIRING, ANALYSING
*/
        case CAM_PV_TYPE::CAM_WRITE_FILE_RBV:
            updateWriteState(args, camObj.daq.writeState);
            message(camObj.name, " (",camObj.screenName, "): Write Status is ", ENUM_TO_STRING(camObj.daq.writeState));
            break;
        case CAM_PV_TYPE::CAM_FILE_WRITE_STATUS:
            updateWriteCheck(args, camObj.daq.writeCheck);
            message(camObj.name, " (",camObj.screenName, "): Write Status is ", ENUM_TO_STRING(camObj.daq.writeCheck));
            break;
        case CAM_PV_TYPE::CAM_FILE_WRITE_ERROR_MESSAGE_RBV:
            updateWriteErrorMessage(args, camObj.daq.writeErrorMessage);
            message(camObj.name, " (",camObj.screenName, "): Write Error message is: ", camObj.daq.writeErrorMessage);
            break;
        case CAM_PV_TYPE::CAM_STATUS:
            updateCamState(args,camObj.state.power);
            message(camObj.name, " (",camObj.screenName, "): power is ", ENUM_TO_STRING(camObj.state.power));
            break;
        case CAM_PV_TYPE::CAM_CAPTURE_RBV:
            updateCollectingState(args,camObj.daq.collectingState);
            message(camObj.name, " (",camObj.screenName, "): Collecting state is ", ENUM_TO_STRING(camObj.daq.collectingState));
            break;
        case CAM_PV_TYPE::CAM_ACQUIRE_RBV:
            updateAcquiring(args,camObj.state.acquire);
            message(camObj.name, " (",camObj.screenName, "): Aquiring state is ", ENUM_TO_STRING(camObj.state.acquire));
            if(isAcquiring(camObj))
                updateSelectedCamRef(camObj);
            break;
        /*
            How many shots have been captured
        */
        case CAM_PV_TYPE::CAM_NUM_CAPTURED_RBV:
            camObj.daq.shotsTaken = getDBRlong(args);
            break;
        /*
            How many shots to capture
        */
        case CAM_PV_TYPE::CAM_NUM_CAPTURE_RBV:
            camObj.daq.numberOfShots = getDBRlong(args);
            break;


//        case CAM_PV_TYPE::CAM_BKGRND_DATA:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_FILE_NAME:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_FILE_PATH:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_CAPTURE:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_FILE_WRITE:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_NUM_CAPTURED:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_FILE_WRITE_RBV:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//
//        case CAM_PV_TYPE::JPG_FILE_WRITE_CHECK:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_FILE_WRITE_MESSAGE:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_CAPTURE_RBV:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_NUM_CAPTURE:
//            //pilaser.HWP = getDBRdouble(args);
//            break;
//        case CAM_PV_TYPE::JPG_NUM_CAPTURE_RBV:
//            //pilaser.HWP = getDBRdouble(args);
//            break;

        case CAM_PV_TYPE::BLACKLEVEL_RBV:
            camObj.state.Blacklevel = (int)getDBRlong(args);
            break;

        case CAM_PV_TYPE::GAINRAW_RBV:
            camObj.state.gain = (int)getDBRlong(args);
            break;

        case CAM_PV_TYPE::STEP_SIZE_RBV:
            camObj.data.analysis.step_size = getDBRint(args);
            break;

        case CAM_PV_TYPE::START_IA_RBV:
            camObj.state.analysing = (bool)getDBRunsignedShort(args);
            message(camObj.name  ," analysing_data = ", camObj.state.analysing);
            //pilaser.HWP = getDBRdouble(args);
            break;

        case CAM_PV_TYPE::USE_BKGRND_RBV:
            camObj.state.use_background = (bool)getDBRunsignedShort(args);
            message(camObj.name  ," use_background = ", camObj.state.use_background );
            break;

        case CAM_PV_TYPE::USE_NPOINT_RBV:
            camObj.state.use_npoint = (bool)getDBRunsignedShort(args);
            message(camObj.name  ," use_npoint = ", camObj.state.use_npoint );
            break;


        case CAM_PV_TYPE::BIT_DEPTH:
            /*
                DONT KNOW PV YET !!!!!!
            */
            break;
/*
            CLARA CAMERAS SET-UP READ BACK VALUES
*/
        case CAM_PV_TYPE::CAM_EXPOSURE_TIME_RBV:
            camObj.daq.exposureTime = getDBRdouble(args);
            break;

        case CAM_PV_TYPE::CAM_ACQUIRE_PERIOD_RBV:
            camObj.daq.acquisitionPeriod = getDBRdouble(args);
            break;
        case CAM_PV_TYPE::CAM_FREQ_RBV:
            camObj.daq.frequency = getDBRdouble(args);
            break;
        case CAM_PV_TYPE::CAM_SENSOR_TEMP_RBV:
            updateSensorTemp(args, camObj);
            break;
/*
            IMAGE ANALYSIS RESULTS
*/
        case CAM_PV_TYPE::X_RBV:
            updateAnalysislResult(args,camObj.data.analysis.x,
                                  camObj.data.analysis.x_buf,
                                  camObj.data.analysis.x_rs,
                                  camObj.data.analysis);
            //message("x = ",camObj.data.analysis.x);
            break;
        case CAM_PV_TYPE::Y_RBV:
            updateAnalysislResult(args,camObj.data.analysis.y,
                                  camObj.data.analysis.y_buf,
                                  camObj.data.analysis.y_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::SIGMA_X_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sig_x,
                                  camObj.data.analysis.sig_x_buf,
                                  camObj.data.analysis.sig_x_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::SIGMA_Y_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sig_y,
                                  camObj.data.analysis.sig_y_buf,
                                  camObj.data.analysis.sig_y_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::COV_XY_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sig_xy,
                                  camObj.data.analysis.sig_xy_buf,
                                  camObj.data.analysis.sig_xy_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::X_PIX_RBV:
            updateAnalysislResult(args,camObj.data.analysis.x_pix,
                                  camObj.data.analysis.x_pix_buf,
                                  camObj.data.analysis.x_pix_rs,
                                  camObj.data.analysis);
            //message("new x_pix = ",camObj.data.analysis.x_pix);
            break;
        case CAM_PV_TYPE::Y_PIX_RBV:
            updateAnalysislResult(args,camObj.data.analysis.y_pix,
                                  camObj.data.analysis.y_pix_buf,
                                  camObj.data.analysis.y_pix_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::SIGMA_X_PIX_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sig_x_pix,
                                  camObj.data.analysis.sig_x_pix_buf,
                                  camObj.data.analysis.sig_x_pix_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::SIGMA_Y_PIX_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sig_y_pix,
                                  camObj.data.analysis.sig_y_pix_buf,
                                  camObj.data.analysis.sig_y_pix_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::COV_XY_PIX_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sig_xy_pix,
                                  camObj.data.analysis.sig_xy_pix_buf,
                                  camObj.data.analysis.sig_xy_pix_rs,
                                  camObj.data.analysis);
            break;
        case CAM_PV_TYPE::AVG_PIX_INTENSITY_RBV:
            updateAnalysislResult(args,camObj.data.analysis.avg_pix,
                                  camObj.data.analysis.avg_pix_buf,
                                  camObj.data.analysis.avg_pix_rs,
                                  camObj.data.analysis);
            //message(camObj.name,", avg_pix = ", camObj.data.analysis.avg_pix);
            break;
        case CAM_PV_TYPE::SUM_PIX_INTENSITY_RBV:
            updateAnalysislResult(args,camObj.data.analysis.sum_pix,
                                  camObj.data.analysis.sum_pix_buf,
                                  camObj.data.analysis.sum_pix_rs,
                                  camObj.data.analysis);
            //message(camObj.name,", sum_pix = ", camObj.data.analysis.sum_pix);
            break;
        case CAM_PV_TYPE::PIXEL_RESULTS_RBV:
            updatePixelResults(args,camObj.data.analysis);
            break;
/*
            MASK READ BACK VALUES
*/
        case CAM_PV_TYPE::MASK_X_RBV:
            //camObj.data.mask.mask_x = getDBRunsignedShort(args);
            camObj.data.mask.mask_x = getDBRunsignedShort(args);
            //message(camObj.name," mask_x = ",camObj.data.mask.mask_x, " ", getDBRunsignedShort(args) );
            break;
        case CAM_PV_TYPE::MASK_Y_RBV:
            camObj.data.mask.mask_y = getDBRunsignedShort(args);
            //message(camObj.name," mask_y = ",camObj.data.mask.mask_y, " ", getDBRunsignedShort(args) );
            break;
        case CAM_PV_TYPE::MASK_X_RAD_RBV:
            camObj.data.mask.mask_x_rad = getDBRunsignedShort(args);
            //message(camObj.name," mask_x_rad = ",camObj.data.mask.mask_x_rad, " ", getDBRunsignedShort(args) );
            break;
        case CAM_PV_TYPE::MASK_Y_RAD_RBV:
            camObj.data.mask.mask_y_rad = getDBRunsignedShort(args);
            //message(camObj.name," mask_y_rad = ",camObj.data.mask.mask_y_rad, " ", getDBRunsignedShort(args) );
            break;
/*
            IMAGE CENTRE AND PIXEL TO MM
*/
        case CAM_PV_TYPE::X_CENTER_RBV:
            camObj.data.analysis.x_centre = (size_t)getDBRunsignedLong(args);
            break;
        case CAM_PV_TYPE::Y_CENTER_RBV:
            camObj.data.analysis.y_centre = (size_t)getDBRunsignedLong(args);
            break;
        case CAM_PV_TYPE::PIX_MM_RBV:
            camObj.data.analysis.pix_2_mm = getDBRdouble(args);
            break;
        default:
            message("!!WARNING!! Unknown PV passed to updateCamValue, pv = ", ENUM_TO_STRING(pv));
    }
}
//--------------------------------------------------------------------------------------------------
//
//       __   __       ___  ___     __  ___      ___  ___  __
// |  | |__) |  \  /\   |  |__     /__`  |   /\   |  |__  /__`
// \__/ |    |__/ /~~\  |  |___    .__/  |  /~~\  |  |___ .__/
//
//
//--------------------------------------------------------------------------------------------------
void cameraBase::updateCamState(const event_handler_args& args, CAM_STATE& s)
{
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            s = CAM_STATE::CAM_OFF;
            break;
        case UTL::ONE_US:
            s = CAM_STATE::CAM_ON;
            break;
        default:
            s = CAM_STATE::CAM_ERROR;
    }
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateWriteCheck(const event_handler_args& args, WRITE_CHECK& w)
{
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            w = WRITE_CHECK::WRITE_CHECK_OK;
            break;
        case UTL::ONE_US:
            w = WRITE_CHECK::WRITE_CHECK_ERROR;
            break;
        default:
            w = WRITE_CHECK::WRITE_CHECK_ERROR;
    }
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateWriteState(const event_handler_args& args, SAVE_STATE& w)
{
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            w = SAVE_STATE::NOT_SAVING;
            break;
        case UTL::ONE_US:
            w = SAVE_STATE::SAVING;
            break;
        default:
            w = SAVE_STATE::SAVING_ERROR;
    }
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateAcquiring(const event_handler_args& args, ACQUIRE_STATE& s)
{
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            s = ACQUIRE_STATE::NOT_ACQUIRING;
            break;
        case UTL::ONE_US:
            s = ACQUIRE_STATE::ACQUIRING;
            break;
        default:
            s = ACQUIRE_STATE::ACQUIRING_ERROR;
    }
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateSelectedCamRef(cameraObject& cam)
{
    //message("updateSelectedCamRef passed camera = ",cam.name);
    if( isNotVC(cam.name))
    {
        //message(cam.name," is NOT VC");
        selectedCamPtr = &cam;
    }
    else
    {
        //message(cam.name," is VC");
        vcCamPtr = &cam;
    }
    if(selectedCamPtr)
        message("selectedCamPtr is ", selectedCamPtr->screenName, ", (",selectedCamPtr->name,")");
    if(vcCamPtr)
        message("vcCamPtr       is ", vcCamPtr->screenName, ", (",vcCamPtr->name,")");
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateCollectingState(const event_handler_args& args, COLLECTING_STATE& s)
{
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            s = COLLECTING_STATE::NOT_COLLECTING;
            break;
        case UTL::ONE_US:
            s = COLLECTING_STATE::COLLECTING;
            break;
        default:
            s = COLLECTING_STATE::COLLECTING_ERROR;
    }
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateWriteErrorMessage(const event_handler_args& args, std::string& mess)
{
    char dummy[256];//MAGIC NUMBER
    for (int i = 0; i != 256; ++i)
    {
        dummy[i] = ((char *)args.dbr)[i];
    }
    mess = dummy;
}
//--------------------------------------------------------------------------------------------------
void cameraBase::updateSensorTemp(const event_handler_args& args, cameraObject& cam)
{
    cam.daq.sensorTemp = getDBRdouble(args);
    /*
        If the sensor temp of the camera is out of operable range (5-10 celcius)
        kill any collecting of data then stop any rolling aquiring that is in progress.
        NOT ANY MORE !!!
    */
    if(cam.daq.sensorTemp > cam.daq.sensorMaxTemp)
    {
        message(cam.daq.sensorTemp, " > ", cam.daq.sensorMaxTemp);
    }
    if(cam.daq.sensorTemp < cam.daq.sensorMinTemp)
    {
        message(cam.daq.sensorTemp, " < ", cam.daq.sensorMinTemp);
    }

    if((cam.daq.sensorTemp > cam.daq.sensorMaxTemp)  || (cam.daq.sensorTemp < cam.daq.sensorMinTemp))
    {
        message("!!WARNING!! Sensor temp is outside correct operable temperature!!");
        message("   1. ",cam.name," temperatue is: ",cam.daq.sensorTemp,"C, (outside of ", cam.daq.sensorMinTemp,",",cam.daq.sensorMaxTemp ,")");
        message("   2. Please CHECK FAN and FILTER (CONSULT SOMEONE).");
        message("Consider stopping Saving, Collecting or Acquiring...");
        //killCollectAndSave();
        //stopAcquiring();
    }
}
//---------------------------------------------------------------------------------______________
/*
    All analysis results call this function
*/
void cameraBase::updateAnalysislResult(const event_handler_args& args,
                                       double& value_to_update,
                                       std::deque<double>& buffer_to_update,
                                       runningStat& rs_to_update,
                                       analysis_data& data)
{
    if(args.type == DBR_TIME_DOUBLE)
    {
        getDBRdouble_timestamp(args, value_to_update);
    }
    else if(args.type == DBR_DOUBLE)
    {
        value_to_update = getDBRdouble(args);
    }
    //getDBRdouble_timestamp(args, value_to_update);
    addToBuffer(value_to_update, buffer_to_update, data);
    rs_to_update.Push(value_to_update);
}
//---------------------------------------------------------------------------------______________
void cameraBase::addToBuffer(const double val, std::deque<double>& buffer, analysis_data& data)
{
    buffer.push_back(val);
    if(buffer.size()> data.max_buffer_count)
    {
        buffer.pop_front();
        data.buffer_full = true;
        //message("BUFFER FULL!!! ");
    }
    else
    {
        data.buffer_full = false;
    }
    data.buffer_count = buffer.size();
}
//---------------------------------------------------------------------------------______________
void cameraBase::updatePixelResults(const event_handler_args& args, analysis_data& data)
{
    const double* pValue;
    /* if time _type get time and set where pValue points to */
    if(isTimeType(args.type))
    {
        const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
        pValue = &p->value;
        //updateTime(trace.etime, trace.time, trace.timeStr);

        /* for hints look in epicsTime.h */
        char timeString[UTL::BUFFER_36];
        epicsTimeToStrftime(timeString, sizeof(timeString),
                            "%a %b %d %Y %H:%M:%S.%f", &p->stamp);
        /*
            prove it works
            std::cout <<std::setprecision(15) <<std::showpoint<< val <<std::endl;
        */
        data.pix_values_time = timeString;
        std::copy(pValue, pValue + data.pix_values.size(), data.pix_values.begin());
        /*
            Now we have the new values, update Buffers
        */
        data.pix_values_buf.push_back(data.pix_values);
        if(data.pix_values_buf.size()> data.max_buffer_count)
        {
            data.pix_values_buf.pop_front();
        }
    }
    else /* set where pValue points to */
    {
        pValue = (dbr_double_t*)args.dbr;
    }
}
// for all getters and setters we need three versions
// one where you pass a name,
// one for the 'selected' camera
// one for the 'VC' camera
// these functions are not used, but could be in Python?
bool cameraBase::setCamera_VC()
{
    return setCamera(UTL::VIRTUAL_CATHODE);
}
//---------------------------------------------------------------------------------
bool cameraBase::setCamera(const std::string &cam)
{
    std::string cameraName = useCameraFrom(cam);
    bool ans = false;
    if(entryExists(allCamData, cameraName))
    {
        selectedCamPtr = &allCamData.at(cameraName);
        ans = true;
        message("new setCamera = ",selectedCamPtr->name);
    }
    return ans;
}
//
//  __   __             ___  __  ___               __      __             ___                  __   ___
// /  ` /  \ |    |    |__  /  `  |      /\  |\ | |  \    /__`  /\  \  / |__     |  |\/|  /\  / _` |__
// \__, \__/ |___ |___ |___ \__,  |     /~~\ | \| |__/    .__/ /~~\  \/  |___    |  |  | /~~\ \__> |___
//
//
//-------------------------------------------------------------------------------------------------------
//
//
bool cameraBase::collectAndSave_VC(const int numbOfShots)
{
    message("collectAndSave_VC passed ",vcCamPtr->name," shots = ",numbOfShots);
    return collectAndSave(vcCamPtr->name, numbOfShots);
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::collectAndSave(const int numbOfShots)
{
    message("cameraBase::collectAndSave");
    message("collectAndSave passed ",selectedCamPtr->name," shots = ",numbOfShots);
    if(isAcquiringAndClaraCam(*selectedCamPtr))
        return collectAndSave(selectedCamPtr->name, numbOfShots);
    else
    {
        message(selectedCamPtr->name," is not acquiring, can't collectAndSave");
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::collectAndSave(const std::string& n, const int numbOfShots)
{
    if(isNotAcquiring(n))
        return false;
    if(isVelaCam(n))
        return false;
    else
    {
        message("collectAndSave passed ",n);
        /*
            kill any finished threads
        */
        killFinishedImageCollectThreads();
        message("killFinishedImageCollectThreads fin");
        if(entryExists(imageCollectStructs, n))
        {
            message(n," exists in imageCollectStructs");

             if( !imageCollectStructs.at(n).isBusy)
             {

                if(numbOfShots <= allCamData.at(n).daq.maxShots)
                {
                    message("create new imageCollectStructs");

                    imageCollectStructs.at(n).isBusy   = true;
                    imageCollectStructs.at(n).success  = false;
                    imageCollectStructs.at(n).numShots = numbOfShots;
                    imageCollectStructs.at(n).thread
                        = new std::thread(staticEntryImageCollectAndSave, std::ref(imageCollectStructs.at(n)));
                    message("new imageCollectStruct");

                    return true;
                }
                else
                {
                    message("!!ERROR!! Requested number of camera images too large. Please set to be less than or equal to ",
                            allCamData.at(n).daq.maxShots);
                }
             }
             else
             {
                message(n," imageCollectStructs is busy ");
             }
        }
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
void cameraBase::staticEntryImageCollectAndSave(imageCollectStruct& ics)
{
    ics.camInterface -> attachTo_thisCAContext();
    ics.camInterface -> imageCollectAndSave(ics);
    ics.camInterface -> detachFrom_thisCAContext();
}
//-------------------------------------------------------------------------------------------------------
void cameraBase::imageCollectAndSave(imageCollectStruct& ics)
{
    /*
        steps through each stage of collecting and saving
    */
    cameraObject& cam = *(ics.camObj);
    if(setNumberOfCapture(cam, ics.numShots))
    {
        if(collect(cam))
        {
            while(isCollecting(cam))   //wait until collecting is done...
            {
                pause_50(); //MAGIC_NUMBER
            }
            if(makeANewDirectoryAndName(cam, ics.numShots))
            {
                pause_500();
                if(write(cam))
                {
                    while( isSaving(cam))   //wait until saving is done...
                    {
                        pause_50();
                    }
                    // pause and wait for EPICS to UPDATE
                    pause_500();
                    //check status of save/write
                    if (cam.daq.writeCheck == WRITE_CHECK::WRITE_CHECK_OK)
                    {
                        ics.success = true;//this->message("Successful wrote image to disk.");
                    }
                    else
                    {
                        message("!!SAVE ERROR!!: camera = ",cam.name,", error message =  ", cam.daq.writeErrorMessage);
                        ics.success = false;
                    }
                    ics.isBusy = false;

                }//if(write(cam))
                else
                {
                    message("!!!ERROR WRITING DATA!!! ",cam.name," ");
                }

            }//if(makeANewDirectoryAndName(cam, ics.numShots);

        }//if(collect(cam))
        else
        {
            message("!!!ERROR COLLECTING DATA!!! ",cam.name," ");
        }

    }//(setNumberOfShots(cam, ics.numShots))

}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::setNumberOfCapture(cameraObject& cam, int numberOfShots)
{
    return cam_caput<int>(cam, numberOfShots, CAM_PV_TYPE::CAM_NUM_CAPTURE);
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::collect(cameraObject& cam)
{
    bool ans = false;
    if(isAcquiringAndClaraCam(cam))
    {
        unsigned short c = 1;
        ans = cam_caput( cam, c, CAM_PV_TYPE::CAM_CAPTURE);
        message("Capture set to 1 on camera ", cam.name);
    }
    return ans;
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::makeANewDirectoryAndName(cameraObject& camera,int numbOfShots)///YUCK (make it look nice)
{
    bool ans=false;
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);
    //tm local_tm = *localtime(&t);
    struct tm local_tm;
    localtime_s(&local_tm, &t);
    char  newPath[256]= "/CameraImages/";
    std::string strPath = "/CameraImages/"+
                        std::to_string(local_tm.tm_year + 1900)+
                        "/"+std::to_string(local_tm.tm_mon + 1)+
                        "/"+std::to_string(local_tm.tm_mday)+"/";
    strcpy(newPath, strPath.c_str());

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
   // message("File Directory would be: ",
     //                   std::to_string(local_tm.tm_year + 1900)+
     //                   "\\"+std::to_string(local_tm.tm_mon + 1)+
     //                   "\\"+std::to_string(local_tm.tm_mday)+"\\");
    message("File name is: ",newName);

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
        camera.daq.latestDirectory = newPath;
        camera.daq.latestFilename  = newName;

        allCamData.at(camera.name).daq.latestDirectory = newPath;
        allCamData.at(camera.name).daq.latestFilename = newName;

        message("New latestDirectory set to ",newPath," on ",camera.name," camera.");
        message("New latestFilename set to  ",newName," on ",camera.name," camera.");
    }

    return ans;
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::write(cameraObject& cam)
{
    bool ans=false;
    int startNumber(1);// MAGIC_NUMBER should this be a one or a zero?

    // WHAT IS THIS DOING????
    //setStartFileNumberJPG(startNumber);

    if(isNotCollecting(cam))
    {
        unsigned short c = 1;
        ans = cam_caput( cam, c, CAM_PV_TYPE::CAM_FILE_WRITE);
        message("WriteFile set to 1 on camera = ",cam.name);
    }
    else
        message("Still collecting images when save function was called.");
    return ans;
}
//-------------------------------------------------------------------------------------------------------
void cameraBase::killFinishedImageCollectThreads()
{
    message("killFinishedImageCollectThreads");
    for(auto&& it:imageCollectStructs)
    {
        message(it.first," cehcking imageCollectStructs");
        if(it.second.isBusy)
        {
            message(it.first," is busy");
        }
        else
        {
            message(it.first," is NOT busy");
            if(it.second.thread)
            {
                message(it.first," thread is not nullptr");
                /*
                    join before deleting...
                    http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
                */
                it.second.thread->join();
                delete it.second.thread;
                it.second.thread = nullptr;
            }
        }
    }
}

bool cameraBase::latestCollectAndSaveSuccess_VC()const
{
    return latestCollectAndSaveSuccess(*vcCamPtr);
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::latestCollectAndSaveSuccess()const
{
    return latestCollectAndSaveSuccess(*selectedCamPtr);
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::latestCollectAndSaveSuccess(const cameraObject& cam)const
{
    if(entryExists(imageCollectStructs, cam.name))
    {
        return imageCollectStructs.at(cam.name).success  == true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
bool cameraBase::killCollectAndSaveVC()///can only kill while saving.
{
    bool killed = false;
//    unsigned short c(0);
//    int dummyNumber(1);
//    if (isCollecting("FAKE_VC"))//stop collecting
//    {
//        message("Killing while collecting...");
//        killed=collect(vcCameraObj,c,dummyNumber);
//    }
//    else if (isSaving("FAKE_VC"))//stop saving
//    {
//        message("Killing while saving...");
//        killed=save(vcCameraObj,c);
//    }
    return killed;
}
//---------------------------------------------------------------------------------
bool cameraBase::collectJPG(cameraObject camera, unsigned short &comm, const int & numbOfShots)
{
    bool ans=false;
//    pvStruct S(camera.pvComStructs.at(CAM_PV_TYPE::JPG_CAPTURE));
//
//        //int oneShot(1);
//        setNumberOfShotsJPG(numbOfShots);
//        if( isAcquiring(camera.name))
//        {
//            ans=shortCaput(comm,S);
//            message(" JPG Caputure set to ",comm,
//                    " on ",camera.name," camera.");
//        }
    return ans;
}

bool cameraBase::saveJPG(cameraObject camera, unsigned short &comm)
{
    bool ans=false;
//    int startNumber(1);// MAGIC_NUMBER
//    pvStruct S(camera.pvComStructs.at(CAM_PV_TYPE::JPG_FILE_WRITE));
//
//    //setStartFileNumberJPG(startNumber);
//
//    if( isCollectingJPG(camera.name)==0)
//    {
//        ans=shortCaput(comm,S);
//        message("WriteFile set to ",comm,
//                " on ",camera.name," camera.");
//    }
//    else
//        message("Still collecting images when save function was called.");

    return ans;
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::useBackground_VC(bool v)
{
    return useBackground(v,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::useBackground(bool v,const std::string& cam)
{
    return useBackground(v,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::useBackground(bool v,cameraStructs::cameraObject& cam)
{
    if(isClaraCam(cam))
    {
        unsigned short comm = v ? UTL::ONE_US : UTL::ZERO_US;
        return cam_caput(cam, comm, CAM_PV_TYPE::USE_BKGRND);
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::useBackground(bool v)
{
    return useBackground(v,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::useNPoint_VC(bool v)
{
    return useNPoint(v,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::useNPoint(bool v,const std::string& cam)
{
    return useNPoint(v,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::useNPoint(bool v,cameraStructs::cameraObject& cam)
{
    if(isClaraCam(cam))
    {
        unsigned short comm = v ? UTL::ONE_US : UTL::ZERO_US;
        return cam_caput(cam, comm, CAM_PV_TYPE::USE_NPOINT);
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::useNPoint(bool v)
{
    return useNPoint(v,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::setMaskX_VC(int x)
{
    return setMaskX(x,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskX(int x,const std::string& cam)
{
    return setMaskX(x,getCamObj(cam));
}
using namespace cameraStructs;
//---------------------------------------------------------------------------------
bool cameraBase::setMaskX(int x,cameraObject& cam)
{
    if(isClaraCam(cam))
        return cam_caput(cam, x, CAM_PV_TYPE::MASK_X);
    else
    {
        cam.data.mask.mask_x = x;
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskX(int x)
{
    return setMaskX(x,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::setMaskY_VC(int x)
{
    return setMaskY(x,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskY(int x,const std::string& cam )
{
    return setMaskY(x,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskY(int x,cameraObject& cam)
{
    if(isClaraCam(cam))
        return cam_caput(cam, x, CAM_PV_TYPE::MASK_Y);
    else
    {
        cam.data.mask.mask_y = x;
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskY(int x)
{
    return setMaskY(x,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::setMaskXrad_VC(int x)
{
    return setMaskXrad(x,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskXrad(int x,const std::string& cam)
{
    return setMaskXrad(x,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskXrad(int x,cameraObject& cam)
{
    if(isClaraCam(cam))
        return cam_caput(cam, x, CAM_PV_TYPE::MASK_X_RAD);
    else
    {
        cam.data.mask.mask_x_rad = x;
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskXrad(int x)
{
    return setMaskXrad(x,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::setMaskYrad_VC(int x)
{
    return setMaskYrad(x,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskYrad(int x,const std::string& cam)
{
    return setMaskYrad(x,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskYrad(int x,cameraObject& cam)
{
    if(isClaraCam(cam))
        return cam_caput(cam, x, CAM_PV_TYPE::MASK_Y_RAD);
    else
    {
        cam.data.mask.mask_y_rad = x;
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::setMaskYrad(int x)
{
    return setMaskYrad(x,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::setMask_VC(int x,int y,int xr,int yr)
{
    return setMask(x,y,xr,yr,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask(int x,int y,int xr,int yr,const std::string& cam)
{
    return setMask(x,y,xr,yr,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask(int x,int y,int xr,int yr,cameraObject& cam)
{
    bool x_result  =  setMaskX(x,cam);
    bool y_result  =  setMaskY(y,cam);
    bool xr_result =  setMaskXrad(xr,cam);
    bool yr_result =  setMaskYrad(yr,cam);
    if( x_result && y_result && xr_result && yr_result)
        return true;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask(int x,int y,int xr,int yr)
{
    return setMask(x,y,xr,yr,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask_VC(const std::vector<int>& v)
{
    return setMask(v,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask(const std::vector<int>& v,const std::string& cam)
{
    return setMask(v,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask(const std::vector<int>& v,cameraObject& cam)
{
    if(v.size() == 4)// MAGIC_NUMBER
    {
        return setMask(v[0], v[1], v[2], v[3], cam);// MAGIC_NUMBER
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::setMask(const std::vector<int>& v)
{
    return setMask(v,*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::setBackground_VC()
{
    return setBackground(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setBackground(const std::string& cam)
{
    return setBackground(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setBackground(cameraObject& cam)
{
    bool ans = false;
    unsigned short on = 1;
    unsigned short off = 1;
    if(isAcquiringAndClaraCam(cam))
    {
        ans = cam_caput(cam, off, CAM_PV_TYPE::SET_BKGRND);
        if(ans)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));//MAGIC_NUMBER!
            ans =  cam_caput(cam, on, CAM_PV_TYPE::SET_BKGRND);
            if(ans)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));//MAGIC_NUMBER!
                ans = cam_caput(cam,off,CAM_PV_TYPE::SET_BKGRND);
                if(ans)
                {
                    message("Setting background with next live image from camera = ",cam.name);
                }
            }
        }
    }
    return ans;
}
//---------------------------------------------------------------------------------
bool cameraBase::setBackground()
{
    return setBackground(*selectedCamPtr);
}
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
bool cameraBase::setGain_VC(const long value)
{
    return setGain(*vcCamPtr, value);
}
//---------------------------------------------------------------------------------
bool cameraBase::setGain(const std::string& cam,const long value)
{
    return setGain(getCamObj(cam), value);
}
//---------------------------------------------------------------------------------
bool cameraBase::setGain(const long value)
{
    return setGain(*selectedCamPtr, value);
}
//---------------------------------------------------------------------------------
bool cameraBase::setGain(cameraStructs::cameraObject& cam, const long value)
{
    if(isVelaCam(cam))
        return cam_caput<const long>(cam, value, CAM_PV_TYPE::GAINRAW);
    return false;
}
//---------------------------------------------------------------------------------




//---------------------------------------------------------------------------------
bool cameraBase::setBlacklevel_VC(const long value)
{
    return setGain(*vcCamPtr, value);
}
//---------------------------------------------------------------------------------
bool cameraBase::setBlacklevel(const std::string& cam,const long value)
{
    return setBlacklevel(getCamObj(cam), value);
}
//---------------------------------------------------------------------------------
bool cameraBase::setBlacklevel(const long value)
{
    return setBlacklevel(*selectedCamPtr, value);
}
//---------------------------------------------------------------------------------
bool cameraBase::setBlacklevel(cameraStructs::cameraObject& cam, const long value)
{
    if(isVelaCam(cam))
        return cam_caput<const long>(cam, value, CAM_PV_TYPE::Blacklevel);
    return false;
}
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
///
///  __  ___      ___  ___     __                 ___    ___    __       ___    __
/// /__`  |   /\   |  |__     /  \ |  |  /\  |\ |  |  | |__  | /  `  /\   |  | /  \ |\ |
/// .__/  |  /~~\  |  |___    \__X \__/ /~~\ | \|  |  | |    | \__, /~~\  |  | \__/ | \|
///
///
///
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
bool cameraBase::isVelaCam_VC()const
{
    return isVelaCam(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isVelaCam(const cameraStructs::cameraObject& cam)const
{
    return cam.type == CAM_TYPE::VELA_CAM;
}
//---------------------------------------------------------------------------------
bool cameraBase::isVelaCam(const std::string& cam)const
{
    return isVelaCam(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isVelaCam()const
{
    return isVelaCam(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isClaraCam_VC()const
{
    return isClaraCam(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isClaraCam(const cameraStructs::cameraObject& cam)const
{
    return cam.type == CAM_TYPE::VELA_CAM;
}
//---------------------------------------------------------------------------------
bool cameraBase::isClaraCam(const std::string& cam)const
{
    return isClaraCam(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isClaraCam()const
{
    return isClaraCam(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::isAcquiringAndClaraCam(const cameraStructs::cameraObject& cam)const
{
    if(isAcquiring(cam))
        if(isClaraCam(cam))
            return true;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::isAcquiringAndVelaCam(const cameraStructs::cameraObject& cam)const
{
    if(isAcquiring(cam))
        if(isVelaCam(cam))
            return true;
    return false;
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraBase::isBusy_VC()const
{
    return isBusy(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isBusy(const cameraStructs::cameraObject& cam)const
{
    if(entryExists(imageCollectStructs,cam.name))
        return imageCollectStructs.at(cam.name).isBusy;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::isBusy(const std::string& cam)const
{
    return isBusy(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isBusy()const
{
    return isBusy(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotBusy_VC()const
{
    return isNotBusy(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotBusy(const cameraStructs::cameraObject& cam)const
{
    if(entryExists(imageCollectStructs,cam.name))
        return !imageCollectStructs.at(cam.name).isBusy;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotBusy(const std::string& cam)const
{
    return isNotBusy(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotBusy()const
{
    return isNotBusy(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingBackground(const std::string& cam)const
{
    return isUsingBackground(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingBackground()const
{
    return isUsingBackground(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingBackground_VC()const
{
    return isUsingBackground(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingBackground(const cameraStructs::cameraObject& cam)const
{
    return cam.state.use_background;
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingBackground(const std::string& cam)const
{
    return isUsingBackground(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingBackground()const
{
    return isNotUsingBackground(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingBackground_VC()const
{
    return isNotUsingBackground(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingBackground(const cameraStructs::cameraObject& cam)const
{
    return !isUsingBackground(cam);
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingNPoint(const std::string& cam)const
{
    return isUsingNPoint(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingNPoint()const
{
    return isUsingNPoint(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingNPoint_VC()const
{
    return isUsingNPoint(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isUsingNPoint(const cameraStructs::cameraObject& cam)const
{
    return cam.state.use_npoint;
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingNPoint(const std::string& cam)const
{
    return isNotUsingNPoint(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingNPoint()const
{
    return isNotUsingNPoint(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingNPoint_VC()const
{
    return isNotUsingNPoint(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotUsingNPoint(const cameraStructs::cameraObject& cam)const
{
    return !isUsingNPoint(cam);
}
//---------------------------------------------------------------------------------
bool cameraBase::isVC(const std::string& name)const
{
    return useCameraFrom(name) == UTL::VIRTUAL_CATHODE;
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotVC(const std::string& name)const
{
    return !isVC(name);
}
//---------------------------------------------------------------------------------
bool cameraBase::isON(const std::string& cam)const
{
    return isON(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isON()const
{
    return isON(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isON_VC()const
{
    return isON(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isON(const cameraObject& cam)const
{
    return cam.state.power == CAM_STATE::CAM_ON;
}
//---------------------------------------------------------------------------------
bool cameraBase::isOFF(const std::string& cam)const
{
    return isOFF(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isOFF()const
{
    return isOFF(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isOFF_VC()const
{
    return isOFF(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isOFF(const cameraObject& cam)const
{
    return cam.state.power == CAM_STATE::CAM_OFF;
}
//---------------------------------------------------------------------------------
bool cameraBase::isAcquiring(const std::string& cam)const
{
    return isAcquiring(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isAcquiring()const
{
    return isAcquiring(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isAcquiring_VC()const
{
    return isAcquiring(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isAcquiring(const cameraObject& cam)const
{
    message("cam.name = ", cam.name );
    return cam.state.acquire == ACQUIRE_STATE::ACQUIRING;
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAcquiring(const std::string& cam)const
{
    return isNotAcquiring(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAcquiring()const
{
    return isNotAcquiring(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAcquiring_VC()const
{
    return isNotAcquiring(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAcquiring(const cameraObject& cam)const
{
    return cam.state.acquire == ACQUIRE_STATE::NOT_ACQUIRING;
}
//---------------------------------------------------------------------------------
bool cameraBase::isAnalysing(const std::string& cam)const
{
    return isAnalysing(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isAnalysing()const
{
    return isAnalysing(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isAnalysing_VC()const
{
    return isAnalysing(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isAnalysing(const cameraObject& cam)const
{
    return cam.state.analysing == true;
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAnalysing(const std::string& cam)const
{
    return isNotAnalysing(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAnalysing()const
{
    return isNotAnalysing(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAnalysing_VC()const
{
    return isNotAnalysing(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotAnalysing(const cameraObject& cam)const
{
    return !isAnalysing(cam);
}
//---------------------------------------------------------------------------------
bool cameraBase::isSelectedCamera(const std::string& name)const
{
    return selectedCamPtr->name == useCameraFrom(name);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotSelectedCamera(const std::string& name)const
{
    return !isSelectedCamera(name);
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollecting_VC()const
{
    return isCollecting(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollecting(const std::string& cam)const
{
    return isCollecting(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollecting(const cameraObject& cam)const
{
    return cam.daq.collectingState == COLLECTING_STATE::COLLECTING;
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollecting()const
{
    return isCollecting(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollecting_VC()const
{
    return isNotCollecting(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollecting(const std::string& cam)const
{
    return isNotCollecting(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollecting(const cameraObject& cam)const
{
    return !isCollecting(cam);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollecting()const
{
    return isNotCollecting(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isSaving_VC()const
{
    return isSaving(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isSaving(const std::string& cam)const
{
    return isSaving(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isSaving(const cameraObject& cam)const
{
    return cam.daq.writeState == SAVE_STATE::SAVING;
}
//---------------------------------------------------------------------------------
bool cameraBase::isSaving()const
{
    return isSaving(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingJPG_VC()const
{
    return isCollectingJPG(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingJPG(const std::string& cam)const
{
    return isCollectingJPG(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingJPG(const cameraObject& cam)const
{
    return cam.daq.collectingStateJPG == COLLECTING_STATE::COLLECTING;
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingJPG()const
{
    return isCollectingJPG(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isSavingJPG_VC()const
{
    return isSavingJPG(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isSavingJPG(const std::string& cam)const
{
    return isSavingJPG(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isSavingJPG(const cameraObject& cam)const
{
    return cam.daq.writeStateJPG == SAVE_STATE::SAVING;
}
//---------------------------------------------------------------------------------
bool cameraBase::isSavingJPG()const
{
    return isSavingJPG(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingOrSaving_VC()const
{
    return isCollectingOrSaving(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingOrSaving(const std::string& cam)const
{
    return isCollectingOrSaving(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingOrSaving(const cameraObject& cam)const
{
    if(isCollecting(cam))
        return true;
    if(isSaving(cam))
        return true;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::isCollectingOrSaving()const
{
    return isCollectingOrSaving(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollectingOrSaving_VC()const
{
    return isNotCollectingOrSaving(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollectingOrSaving(const std::string& cam)const
{
    return isNotCollectingOrSaving(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollectingOrSaving(const cameraObject& cam)const
{
    return !isCollectingOrSaving(cam);
}
//---------------------------------------------------------------------------------
bool cameraBase::isNotCollectingOrSaving()const
{
    return isCollectingOrSaving(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get pixel values buffer
///
bool cameraBase::isBufferFull_VC()
{
    return isBufferFull(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isBufferFull(const std::string& cam)
{
    return isBufferFull(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isBufferFull(const cameraObject& cam)
{
    return cam.data.analysis.buffer_full;
}
//---------------------------------------------------------------------------------
bool cameraBase::isBufferFull()
{
    return isBufferFull(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// isBufferNotFull
///
bool cameraBase::isBufferNotFull_VC()
{
    return isBufferNotFull(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::isBufferNotFull(const std::string& cam)
{
    return isBufferNotFull(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::isBufferNotFull(const cameraObject& cam)
{
    return !isBufferFull(cam);
}
//---------------------------------------------------------------------------------
bool cameraBase::isBufferNotFull()
{
    return isBufferNotFull(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
///
///  __   ___ ___ ___  ___  __   __
/// / _` |__   |   |  |__  |__) /__`
/// \__> |___  |   |  |___ |  \ .__/
///
///
//---------------------------------------------------------------------------------
std::string cameraBase::useCameraFrom(const std::string& camOrScreen)const
{
    /*
        checks if the passed string is the name of a camera or screen
        if its a screen name then it returns the camera that screen
        is associated with, else it returns the camera name
        else it returns the passed name
    */
    std::string cameraName(camOrScreen);
    if (entryExists(allCamData,camOrScreen))
    {
        cameraName = camOrScreen;
    }
    else
    {
        bool usingAScreenName = false;
        for (auto && it : allCamData)
        {
            if (it.second.screenName == camOrScreen)
            {
                cameraName = it.second.name;
                usingAScreenName= true;
            }
        }
        if (usingAScreenName == false)
        {
            message("!!ERROR!!: camera controller does not recognise the name '",camOrScreen,"'");
            cameraName = UTL::UNKNOWN_NAME;
        }
    }
    message("useCameraFrom passed, ",camOrScreen, " returning ",cameraName);
    return cameraName;
}
//---------------------------------------------------------------------------------
cameraObject& cameraBase::getCamObj(const std::string& cam)
{
    const std::string cameraName = useCameraFrom(cam);
    if(entryExists(allCamData, cameraName))
    {
        return allCamData.at(cameraName);
    }
    return dummyCamObject;
}
//---------------------------------------------------------------------------------
const cameraObject& cameraBase::getCamObj(const std::string& cam)const
{
    const std::string cameraName = useCameraFrom(cam);
    if(entryExists(allCamData, cameraName))
    {
        return allCamData.at(cameraName);
    }
    return dummyCamObject;
}
//
//            __           __   ___ ___ ___  ___  __   __
// |\/|  /\  /__` |__/    / _` |__   |   |  |__  |__) /__`
// |  | /~~\ .__/ |  \    \__> |___  |   |  |___ |  \ .__/
//
//
///
/// MASK X
///
int cameraBase::getMaskX_VC()const
{
    return getMaskX(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskX(const std::string& cam)const
{
    return getMaskX(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskX(const cameraObject& cam)const
{
    return cam.data.mask.mask_x;
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskX()const
{
    return getMaskX(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// MASK Y
///
int cameraBase::getMaskY_VC()const
{
    return getMaskY(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskY(const std::string& cam)const
{
    return getMaskY(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskY(const cameraObject& cam)const
{
    message("getMaskY ", cam.name, " ", cam.data.mask.mask_y);
    return cam.data.mask.mask_y;
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskY()const
{
    return getMaskY(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// MASK X Radius
///
int cameraBase::getMaskXrad_VC()const
{
    return getMaskXrad(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskXrad(const std::string& cam)const
{
    return getMaskXrad(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskXrad(const cameraObject& cam)const
{
    return cam.data.mask.mask_x_rad;
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskXrad()const
{
    return getMaskXrad(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// MASK X Radius
///
int cameraBase::getMaskYrad_VC()const
{
    return getMaskYrad(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskYrad(const std::string& cam)const
{
    return getMaskYrad(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskYrad(const cameraObject& cam)const
{
    return cam.data.mask.mask_y_rad;
}
//---------------------------------------------------------------------------------
int cameraBase::getMaskYrad()const
{
    return getMaskYrad(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getStepSize_VC()const
{
    return getStepSize(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getStepSize(const std::string& cam)const
{
    return getStepSize(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getStepSize(const cameraStructs::cameraObject& cam)const
{
    return cam.data.analysis.step_size;
}
//---------------------------------------------------------------------------------
int cameraBase::getStepSize()const
{
    return getStepSize(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get x Buffer
///
std::deque<double> cameraBase::getXBuffer_VC()const
{
    return getXBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getXBuffer(const std::string& cam)const
{
    return getXBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getXBuffer()const
{
    return getXBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getXBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.x_buf;
}
//---------------------------------------------------------------------------------
///
/// get y Buffer
///
std::deque<double> cameraBase::getYBuffer_VC()const
{
    return getYBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getYBuffer(const std::string& cam)const
{
    return getYBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getYBuffer()const
{
    return getYBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getYBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.y_buf;
}
//---------------------------------------------------------------------------------
///
/// get sig x Buffer
///
std::deque<double> cameraBase::getSigXBuffer_VC()const
{
    return getSigXBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXBuffer(const std::string& cam)const
{
    return getSigXBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXBuffer()const
{
    return getSigXBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sig_x_buf;
}
///
/// get sig y Buffer
///
std::deque<double> cameraBase::getSigYBuffer_VC()const
{
    return getSigYBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigYBuffer(const std::string& cam)const
{
    return getSigYBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigYBuffer()const
{
    return getSigYBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigYBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sig_y_buf;
}
///
/// get sig xy Buffer
///
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYBuffer_VC()const
{
    return getSigXYBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYBuffer(const std::string& cam)const
{
    return getSigXYBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYBuffer()const
{
    return getSigXYBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sig_xy_buf;
}
//---------------------------------------------------------------------------------
///
/// get x pix Buffer
///
std::deque<double> cameraBase::getXPixBuffer_VC()const
{
    return getXPixBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getXPixBuffer(const std::string& cam)const
{
    return getXPixBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getXPixBuffer()const
{
    return getXPixBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getXPixBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.x_pix_buf;
}
//---------------------------------------------------------------------------------
///
/// get y pix Buffer
///
std::deque<double> cameraBase::getYPixBuffer_VC()const
{
    return getYPixBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getYPixBuffer(const std::string& cam)const
{
    return getYPixBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getYPixBuffer()const
{
    return getYPixBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getYPixBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.y_pix_buf;
}
//---------------------------------------------------------------------------------
///
/// get sig x pix Buffer
///
std::deque<double> cameraBase::getSigXPixBuffer_VC()const
{
    return getSigXPixBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXPixBuffer(const std::string& cam)const
{
    return getSigXPixBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXPixBuffer()const
{
    return getSigXPixBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXPixBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sig_x_pix_buf;
}
//---------------------------------------------------------------------------------
///
/// get sig y pix Buffer
///
std::deque<double> cameraBase::getSigYPixBuffer_VC()const
{
    return getSigYPixBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigYPixBuffer(const std::string& cam)const
{
    return getSigYPixBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigYPixBuffer()const
{
    return getSigYPixBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigYPixBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sig_y_pix_buf;
}
//---------------------------------------------------------------------------------
///
/// get sig xy pix Buffer
///
std::deque<double> cameraBase::getSigXYPixBuffer_VC()const
{
    return getSigXYPixBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYPixBuffer(const std::string& cam)const
{
    return getSigXYPixBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYPixBuffer()const
{
    return getSigXYPixBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSigXYPixBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sig_xy_pix_buf;
}
//---------------------------------------------------------------------------------
///
/// get sum pixel buffer
///
std::deque<double> cameraBase::getSumIntensityBuffer_VC()const
{
    return getSumIntensityBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSumIntensityBuffer(const std::string& cam)const
{
    return getSumIntensityBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSumIntensityBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.sum_pix_buf;
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getSumIntensityBuffer()const
{
    return getSumIntensityBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get average pixel buffer
///
std::deque<double> cameraBase::getAvgIntensityBuffer_VC()const
{
    return getAvgIntensityBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getAvgIntensityBuffer(const std::string& cam)const
{
    return getAvgIntensityBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getAvgIntensityBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.avg_pix_buf;
}
//---------------------------------------------------------------------------------
std::deque<double> cameraBase::getAvgIntensityBuffer()const
{
    return getAvgIntensityBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get pixel values
///
std::vector<double> cameraBase::getPixelValues_VC()const
{
    return getPixelValues(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::vector<double> cameraBase::getPixelValues(const std::string& cam)const
{
    return getPixelValues(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::vector<double> cameraBase::getPixelValues()const
{
    return getPixelValues(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::vector<double> cameraBase::getPixelValues(const cameraObject& cam)const
{
    return cam.data.analysis.pix_values;
}
//---------------------------------------------------------------------------------
///
/// get pixel values buffer
///
std::deque<std::vector<double>> cameraBase::getPixelValuesBuffer_VC()const
{
    return getPixelValuesBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<std::vector<double>> cameraBase::getPixelValuesBuffer(const std::string& cam)const
{
    return getPixelValuesBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::deque<std::vector<double>> cameraBase::getPixelValuesBuffer()const
{
    return getPixelValuesBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::deque<std::vector<double>> cameraBase::getPixelValuesBuffer(const cameraObject& cam)const
{
    return cam.data.analysis.pix_values_buf;
}
//---------------------------------------------------------------------------------
///
/// get pixel values buffer
///
std::vector<int> cameraBase::getFastImage_VC()const
{
    return getFastImage(*vcCamPtr);
}
//---------------------------------------------------------------------------------
std::vector<int> cameraBase::getFastImage(const std::string& cam)const
{
    return getFastImage(getCamObj(cam));
}
//---------------------------------------------------------------------------------
std::vector<int> cameraBase::getFastImage()const
{
    return getFastImage(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::vector<int> cameraBase::getFastImage(const cameraObject& cam)const
{
    return  cam.data.image.array_data;
}
//---------------------------------------------------------------------------------
bool cameraBase::takeFastImage_VC()
{
    return takeFastImage(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::takeFastImage()
{
    return takeFastImage(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::takeFastImage(const std::string& cam)
{
    return takeFastImage(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::takeFastImage(cameraObject& cam)
{
    if(isAcquiring(cam))
    {
        int a = ca_array_get(
                    cam.pvComStructs.at(CAM_PV_TYPE::ARRAY_DATA).CHTYPE,
                    cam.pvComStructs.at(CAM_PV_TYPE::ARRAY_DATA).COUNT,
                    cam.pvComStructs.at(CAM_PV_TYPE::ARRAY_DATA).CHID,
                    (void*)&(cam.data.image.array_data[UTL::ZERO_SIZET]));
        SEVCHK( a, "ca_array_get()" );

        if(a == ECA_NORMAL)
        {
            a = sendToEpics("ca_array_get","","");
            if(a == ECA_NORMAL)
            {
                /*
                    update the local version of min/max pixel values etc
                */
                cam.data.image.array_data_sum = UTL::ZERO_SIZET;
                cam.data.image.array_data_max = std::numeric_limits<int>::min();
                cam.data.image.array_data_min = std::numeric_limits<int>::max();
                for(auto&& n : cam.data.image.array_data)
                {
                    /* add to sum */
                    cam.data.image.array_data_sum += (size_t)n;
                    /* set min and max values */
                    if(n > cam.data.image.array_data_max)
                        cam.data.image.array_data_max = n;
                    if(n < cam.data.image.array_data_min)
                        cam.data.image.array_data_min = n;
                }

#ifdef BUILD_DLL
                cam.data.image.data   = toPythonList(cam.data.image.array_data);

                // toPythonList2D does not work yet!!!
                cam.data.image.data2D = toPythonList2D(cam.data.image.array_data,
                                                            cam.data.image.num_pix_x,
                                                            cam.data.image.num_pix_y);
            //boost::python::list data,data2D;
#endif
//                message("ca_array_get success");
                return true;
            }
        }
        else
        {
            message("ca_array_get did not return ECA_NORMAL");
        }
    }
    else
    {
        message(cam.name," is not acquiring");
    }
    return false;
}
//---------------------------------------------------------------------------------
#ifdef BUILD_DLL
boost::python::list cameraBase::takeAndGetFastImage2D_VC()
{
    return takeAndGetFastImage2D(*vcCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage2D(const std::string& cam)
{
    return takeAndGetFastImage2D(getCamObj(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage2D()
{
    return takeAndGetFastImage2D(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage2D(cameraStructs::cameraObject& cam)
{
    takeFastImage(cam);
    return cam.data.image.data2D;
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage_VC()
{
    return takeAndGetFastImage(*vcCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage(const std::string& cam)
{
    return takeAndGetFastImage(getCamObj(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage()
{
    return takeAndGetFastImage(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::takeAndGetFastImage(cameraStructs::cameraObject& cam)
{
    takeFastImage(cam);
    return cam.data.image.data;
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage_VC_Py()const
{
    return getFastImage_Py(*vcCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage_Py(const std::string& cam)const
{
    return getFastImage_Py(getCamObj(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage_Py()const
{
    return getFastImage_Py(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage_Py(const cameraStructs::cameraObject& cam)const
{
    return cam.data.image.data;
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage2D_VC_Py()const
{
    return getFastImage2D_Py(*vcCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage2D_Py(const std::string& cam)const
{
    return getFastImage2D_Py(getCamObj(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage2D_Py()const
{
    return getFastImage2D_Py(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
boost::python::list cameraBase::getFastImage2D_Py(const cameraStructs::cameraObject& cam)const
{
    return cam.data.image.data2D;
}
//---------------------------------------------------------------------------------


#endif
///
/// get x value
///
double cameraBase::getX_VC()const
{
    return getX(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getX(const std::string& cam)const
{
    return getX(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getX(const cameraObject& cam)const
{
    return cam.data.analysis.x;
}
//---------------------------------------------------------------------------------
double cameraBase::getX()const
{
    return getX(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get y value
///
double cameraBase::getY_VC()const
{
    return getY(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getY(const std::string& cam)const
{
    return getY(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getY(const cameraObject& cam)const
{
    return cam.data.analysis.y;
}
//---------------------------------------------------------------------------------
double cameraBase::getY()const
{
    return getY(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get sig x value
///
double cameraBase::getSigX_VC()const
{
    return getSigX(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getSigX(const std::string& cam)const
{
    return getSigX(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getSigX(const cameraObject& cam)const
{
    return cam.data.analysis.sig_x;
}
//---------------------------------------------------------------------------------
double cameraBase::getSigX()const
{
    return getSigX(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get sig_y value
///
double cameraBase::getSigY_VC()const
{
    return getSigY(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getSigY(const std::string& cam)const
{
    return getSigY(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getSigY(const cameraObject& cam)const
{
    return cam.data.analysis.sig_y;
}
//---------------------------------------------------------------------------------
double cameraBase::getSigY()const
{
    return getSigY(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get sig xy value
///
double cameraBase::getSigXY_VC()const
{
    return getSigXY(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXY(const std::string& cam)const
{
    return getSigXY(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXY(const cameraObject& cam)const
{
    return cam.data.analysis.sig_xy;
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXY()const
{
    return getSigXY(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get x pix value
///
double cameraBase::getXPix_VC()const
{
    return getXPix(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getXPix(const std::string& cam)const
{
    return getXPix(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getXPix(const cameraObject& cam)const
{
    return cam.data.analysis.x_pix;
}
//---------------------------------------------------------------------------------
double cameraBase::getXPix()const
{
    return getXPix(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get y pix value
///
double cameraBase::getYPix_VC()const
{
    return getYPix(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getYPix(const std::string& cam)const
{
    return getYPix(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getYPix(const cameraObject& cam)const
{
    return cam.data.analysis.y_pix;
}
//---------------------------------------------------------------------------------
double cameraBase::getYPix()const
{
    return getYPix(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get sig x pix value
///
double cameraBase::getSigXPix_VC()const
{
    return getSigXPix(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXPix(const std::string& cam)const
{
    return getSigXPix(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXPix(const cameraObject& cam)const
{
    return cam.data.analysis.sig_x_pix;
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXPix()const
{
    return getSigXPix(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get sig y pix value
///
double cameraBase::getSigYPix_VC()const
{
    return getSigYPix(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getSigYPix(const std::string& cam)const
{
    return getSigYPix(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getSigYPix(const cameraObject& cam)const
{
    return cam.data.analysis.sig_y_pix;
}
//---------------------------------------------------------------------------------
double cameraBase::getSigYPix()const
{
    return getSigYPix(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get sig xy pix value
///
double cameraBase::getSigXYPix_VC()const
{
    return getSigXYPix(*vcCamPtr);
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXYPix(const std::string& cam)const
{
    return getSigXYPix(getCamObj(cam));
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXYPix(const cameraObject& cam)const
{
    return cam.data.analysis.sig_xy_pix;
}
//---------------------------------------------------------------------------------
double cameraBase::getSigXYPix()const
{
    return getSigXYPix(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get buffer count clear
///
size_t cameraBase::getBufferCount_VC()const
{
    return getBufferCount(*vcCamPtr);
}
//---------------------------------------------------------------------------------
size_t cameraBase::getBufferCount(const std::string& cam)const
{
    return getBufferCount(getCamObj(cam));
}
//---------------------------------------------------------------------------------
size_t cameraBase::getBufferCount(const cameraObject& cam)const
{
    return cam.data.analysis.buffer_count;
}
//---------------------------------------------------------------------------------
size_t cameraBase::getBufferCount()const
{
    return getBufferCount(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// get buffer max count
///
size_t cameraBase::getBufferMaxCount_VC()const
{
    return getBufferMaxCount(*vcCamPtr);
}
//---------------------------------------------------------------------------------
size_t cameraBase::getBufferMaxCount(const std::string& cam)const
{
    return getBufferMaxCount(getCamObj(cam));
}
//---------------------------------------------------------------------------------

size_t cameraBase::getBufferMaxCount(const cameraObject& cam)const
{
    return cam.data.analysis.max_buffer_count;
}
//---------------------------------------------------------------------------------
size_t cameraBase::getBufferMaxCount()const
{
    return getBufferMaxCount(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
std::vector<std::string> cameraBase::getCameraNames()const
{
    std::vector<std::string> r;
    for(auto && it:allCamData)
        r.push_back(it.first);
    return r;
}
//---------------------------------------------------------------------------------
std::vector<std::string> cameraBase::getCameraScreenNames()const
{
    std::vector<std::string> r;
    for(auto && it:allCamData)
        r.push_back(it.second.screenName);
    return r;
}
//---------------------------------------------------------------------------------
int cameraBase::getGain_VC()const
{
    return getGain(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getGain(const std::string& cam)const
{
    return getGain(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getGain()const
{
    return getGain(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getGain(const cameraStructs::cameraObject& cam)const
{
    return cam.state.gain;
}
//---------------------------------------------------------------------------------
int cameraBase::getBlacklevel_VC()const
{
    return getBlacklevel(*vcCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getBlacklevel(const std::string& cam)const
{
    return getBlacklevel(getCamObj(cam));
}
//---------------------------------------------------------------------------------
int cameraBase::getBlacklevel()const
{
    return getBlacklevel(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
int cameraBase::getBlacklevel(const cameraStructs::cameraObject& cam)const
{
    return cam.state.Blacklevel;
}
//---------------------------------------------------------------------------------















bool cameraBase::setStepSize_VC(unsigned short step)
{
    return setStepSize(step,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setStepSize(unsigned short step,const std::string& cam               )
{
    return setStepSize(step,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setStepSize(unsigned short step,cameraObject& cam)
{
    bool ans = false;
    if(isAcquiring(cam))
    {
        ans = cam_caput(cam, step, CAM_PV_TYPE::STEP_SIZE);
        //message("Setting Step Size over image on camera = ", cam.name,", to be ", step);
    }
    return ans;
}
//---------------------------------------------------------------------------------
bool cameraBase::setStepSize(unsigned short step)
{
    return setStepSize(step,*selectedCamPtr);
}
//---------------------------------------------------------------------------------



bool cameraBase::setCenterXPixel_VC(unsigned short  xC)
{
    return setCenterXPixel(xC,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setCenterXPixel(unsigned short  xC,const std::string& cam)
{
    return setCenterXPixel(xC,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setCenterXPixel(unsigned short  xC,cameraObject& cam)
{
    return cam_caput(cam, xC, CAM_PV_TYPE::X_CENTER);
}
//---------------------------------------------------------------------------------
bool cameraBase::setCenterXPixel(unsigned short  xC)
{
    return setCenterXPixel(xC,*selectedCamPtr);
}
//---------------------------------------------------------------------------------



bool cameraBase::setCenterYPixel_VC(unsigned short yC)
{
    return setCenterYPixel(yC,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setCenterYPixel(unsigned short yC,const std::string& cam               )
{
    return setCenterYPixel(yC,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setCenterYPixel(unsigned short yC,cameraObject& cam)
{
    return cam_caput(cam, yC, CAM_PV_TYPE::Y_CENTER);
}
//---------------------------------------------------------------------------------
bool cameraBase::setCenterYPixel(unsigned short yC)
{
    return setCenterYPixel(yC,*selectedCamPtr);
}
//---------------------------------------------------------------------------------

bool cameraBase::setPixMM_VC(const double pmm)
{
    return setPixMM(pmm,*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::setPixMM(const double pmm,const std::string& cam)
{
    return setPixMM(pmm,getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::setPixMM(const double pmm,cameraObject& cam)
{
    return cam_caput(cam, pmm, CAM_PV_TYPE::PIX_MM);
}
//---------------------------------------------------------------------------------
bool cameraBase::setPixMM(const double pmm)
{
    return setPixMM(pmm, *selectedCamPtr);
}

//---------------------------------------------------------------------------------
bool cameraBase::startAnalysis_VC()
{
    return startAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAnalysis(const std::string& cam)
{
    return startAnalysis(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::startAnalysis(cameraObject& cam)
{
    bool ans = false;
    if(isAcquiring(cam))
    {
        return cam_caput(cam, UTL::ONE_US,CAM_PV_TYPE::START_IA);
    }
    return ans;
}
//---------------------------------------------------------------------------------
bool cameraBase::startAnalysis()
{
    return startAnalysis(*selectedCamPtr);
}
//---------------------------------------------------------------------------------

bool cameraBase::stopAnalysis_VC()
{
    return stopAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysis(const std::string& cam)
{
    return stopAnalysis(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysis(cameraObject& cam)
{
    bool ans = false;
    if(isAcquiring(cam))
    {
        return cam_caput(cam, UTL::ZERO_US, CAM_PV_TYPE::START_IA);
    }
    return ans;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysis()
{
    return stopAnalysis(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquiring_VC()
{
    return startAcquiring(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquiring(const std::string& cam)
{
    if(isVC(cam))
        return startAcquiring_VC();
    else
        return startAcquiring(getCamObj(cam));
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquiring(cameraObject& cam)
{
    //message("isNotAcquiring(",cam.name,") = ",isNotAcquiring(cam));
    if(isNotAcquiring(cam))
    {
        if(isNotVC(cam.name))
        {
            stopAllAcquiringExceptVC();
        }
        //message("Try startAcquiring ",cam.name);
        unsigned short c = UTL::ONE_US;
        return cam_caput<unsigned short>(cam, c, CAM_PV_TYPE::CAM_START_ACQUIRE);
    }
    else if(isAcquiring(cam))
        return true;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquiring()
{
        return startAcquiring(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquiring_VC()
{
    return stopAcquiring(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquiring(const std::string& cam)
{
    return stopAcquiring(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquiring(cameraStructs::cameraObject& cam)
{
    if(isAcquiring(cam))
    {
        dbr_enum_t c = (dbr_enum_t)UTL::ZERO_US;
        return cam_caput<dbr_enum_t>(cam, c, CAM_PV_TYPE::CAM_STOP_ACQUIRE);
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquiring()
{
    return stopAcquiring(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAllAcquiring()
{
    int ans = UTL::ZERO_INT;
    int sum = UTL::ZERO_INT;
    for(auto&&it:allCamData)
    {
        if(stopAcquiring(it.second))
        {
            sum += UTL::ONE_INT;
        }
        ans += UTL::ONE_INT;
    }
    return ans == sum;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAllAcquiringExceptVC()
{
    int ans = UTL::ZERO_INT;
    int sum = UTL::ZERO_INT;
    for(auto&&it:allCamData)
    {
        if(isVC(it.first))
        {

        }
        else
        {
            if(stopAcquiring(it.second))
            {
                sum += UTL::ONE_INT;
            }
            ans += UTL::ONE_INT;
        }
    }
    return ans == sum;
}
//---------------------------------------------------------------------------------




bool cameraBase::startAnalysing_VC()
{
    return startAnalysing(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAnalysing(const std::string& cam)
{
    if(isVC(cam))
        return startAnalysing_VC();
    else
        return startAnalysing(getCamObj(cam));
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::startAnalysing(cameraObject& cam)
{
    if(isNotAnalysing(cam))
    {
        unsigned short c = UTL::ONE_US;
        return cam_caput<unsigned short>(cam, c, CAM_PV_TYPE::START_IA);
    }
    else if(isAnalysing(cam))
        return true;
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::startAnalysing()
{
    return startAnalysing(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysing_VC()
{
    return stopAnalysing(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysing(const std::string& cam)
{
    return stopAnalysing(getCamObj(cam));
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysing(cameraStructs::cameraObject& cam)
{
    if(isAnalysing(cam))
    {
        dbr_enum_t c = (dbr_enum_t)UTL::ZERO_US;
        return cam_caput<dbr_enum_t>(cam, c, CAM_PV_TYPE::START_IA);
    }
    return false;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAnalysing()
{
    return stopAnalysing(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAllAnalysing()
{
    int ans = UTL::ZERO_INT;
    int sum = UTL::ZERO_INT;
    for(auto&&it:allCamData)
    {
        if(stopAnalysing(it.second))
        {
            sum += UTL::ONE_INT;
        }
        ans += UTL::ONE_INT;
    }
    return ans == sum;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAllAnalysingExceptVC()
{
    int ans = UTL::ZERO_INT;
    int sum = UTL::ZERO_INT;
    for(auto&&it:allCamData)
    {
        if(isVC(it.first))
        {

        }
        else
        {
            if(stopAnalysing(it.second))
            {
                sum += UTL::ONE_INT;
            }
            ans += UTL::ONE_INT;
        }
    }
    return ans == sum;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquireAndAnalysis_VC()
{
    return stopAcquireAndAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquireAndAnalysis(const std::string& cam)
{
    return stopAcquireAndAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquireAndAnalysis(cameraStructs::cameraObject& cam)
{
    bool s_1 = stopAcquiring(cam);
    bool s_2 = stopAnalysing(cam);
    return s_1 && s_2;
}
//---------------------------------------------------------------------------------
bool cameraBase::stopAcquireAndAnalysis()
{
    return stopAcquireAndAnalysis(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquireAndAnalysis_VC()
{
    return startAcquireAndAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquireAndAnalysis(const std::string& cam)
{
    return startAcquireAndAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquireAndAnalysis(cameraStructs::cameraObject& cam)
{
    bool s_1 = startAcquiring(cam);
    bool s_2 = startAnalysing(cam);
    return s_1 && s_2;
}
//---------------------------------------------------------------------------------
bool cameraBase::startAcquireAndAnalysis()
{
    return startAcquireAndAnalysis(*vcCamPtr);
}
//---------------------------------------------------------------------------------








//---------------------------------------------------------------------------------
///
/// clear running values
///
void cameraBase::clearRunningValues_VC()
{
    return clearRunningValues(*vcCamPtr);
}
//---------------------------------------------------------------------------------
void cameraBase::clearRunningValues(const std::string& cam)
{
    return clearRunningValues(getCamObj(cam));
}
//---------------------------------------------------------------------------------
void cameraBase::clearRunningValues()
{
    return clearRunningValues(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
void cameraBase::clearRunningValues(cameraObject& cam)
{
    cam.data.analysis.x_rs.Clear();
    cam.data.analysis.y_rs.Clear();
    cam.data.analysis.sig_x_rs.Clear();
    cam.data.analysis.sig_y_rs.Clear();
    cam.data.analysis.sig_xy_rs.Clear();
    cam.data.analysis.x_pix_rs.Clear();
    cam.data.analysis.avg_pix_rs.Clear();
    cam.data.analysis.sum_pix_rs.Clear();
    cam.data.analysis.y_pix_rs.Clear();
    cam.data.analysis.sig_x_pix_rs.Clear();
    cam.data.analysis.sig_y_pix_rs.Clear();
    cam.data.analysis.sig_xy_pix_rs.Clear();
}
//---------------------------------------------------------------------------------




///
/// set buffer max count
///
void cameraBase::setBufferMaxCount_VC(const size_t s)
{
    setBufferMaxCount(s,*vcCamPtr);
}
//---------------------------------------------------------------------------------
void cameraBase::setBufferMaxCount(const size_t s,const std::string& cam)
{
    setBufferMaxCount(s,getCamObj(cam));
}
//---------------------------------------------------------------------------------
void cameraBase::setBufferMaxCount(const size_t s,cameraObject& cam)
{
    cam.data.analysis.max_buffer_count = s;
}
//---------------------------------------------------------------------------------
void cameraBase::setBufferMaxCount(const size_t s)
{
    setBufferMaxCount(s, *selectedCamPtr);
}
//---------------------------------------------------------------------------------
///
/// clear buffer value
///
void cameraBase::clearBuffer_VC()
{
    clearBuffer(*vcCamPtr);
}
//---------------------------------------------------------------------------------
void cameraBase::clearBuffer(const std::string& cam)
{
    clearBuffer(getCamObj(cam));
}
//---------------------------------------------------------------------------------
void cameraBase::clearBuffer(cameraObject& cam)
{
    cam.data.analysis.buffer_full  = false;
    cam.data.analysis.buffer_count = UTL::ZERO_SIZET;
    cam.data.analysis.x_buf.clear();
    cam.data.analysis.y_buf.clear();
    cam.data.analysis.sig_x_buf.clear();
    cam.data.analysis.sig_y_buf.clear();
    cam.data.analysis.sig_xy_buf.clear();
    cam.data.analysis.avg_pix_buf.clear();
    cam.data.analysis.sum_pix_buf.clear();
    cam.data.analysis.x_pix_buf.clear();
    cam.data.analysis.y_pix_buf.clear();
    cam.data.analysis.sig_x_pix_buf.clear();
    cam.data.analysis.sig_y_pix_buf.clear();
    cam.data.analysis.sig_xy_pix_buf.clear();
}
//---------------------------------------------------------------------------------
void cameraBase::clearBuffer()
{
    clearBuffer(*selectedCamPtr);
}
//---------------------------------------------------------------------------------


//______________________________________________________________________________
///
///  __   __        ___  __  ___     __   __        __  ___     __   ___  ___  __
/// /  \ |__)    | |__  /  `  |     /  ` /  \ |\ | /__`  |     |__) |__  |__  /__`
/// \__/ |__) \__/ |___ \__,  |     \__, \__/ | \| .__/  |     |  \ |___ |    .__/
///
///
/// MASK OBJECTS
const analysis_mask& cameraBase::getMaskObj(const std::string& name)const
{
    return getMaskObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const analysis_mask& cameraBase::getMaskObj_VC()const
{
    return getMaskObj(*vcCamPtr);
}
//---------------------------------------------------------------------------------
const analysis_mask& cameraBase::getMaskObj()const
{
    return getMaskObj(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
const analysis_mask& cameraBase::getMaskObj(const cameraStructs::cameraObject& cam)const
{
    return cam.data.mask;
}
//---------------------------------------------------------------------------------
/// ANALYSIS DATA OBJECTS
const analysis_data& cameraBase::getAnalysisObj(const std::string& name)const
{
    return getAnalysisObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const analysis_data& cameraBase::getAnalysisObj_VC()const
{
    return getAnalysisObj(*vcCamPtr);
}
//---------------------------------------------------------------------------------
const analysis_data& cameraBase::getAnalysisObj()const
{
    return getAnalysisObj(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
const analysis_data& cameraBase::getAnalysisObj(const cameraStructs::cameraObject& cam)const
{
    return cam.data.analysis;
}
//---------------------------------------------------------------------------------
/// FAST IMAGE OBJECTS
const fast_image& cameraBase::getImageObj(const std::string& name)const
{
    return getImageObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const fast_image& cameraBase::getImageObj_VC()const
{
    return getImageObj(*vcCamPtr);
}
//---------------------------------------------------------------------------------
const fast_image& cameraBase::getImageObj()const
{
    return getImageObj(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
const fast_image& cameraBase::getImageObj(const cameraStructs::cameraObject& cam)const
{
    return cam.data.image;
}
//---------------------------------------------------------------------------------
/// camera_image_data OBJECTS
const camera_image_data& cameraBase::getImageDataObj(const std::string& name)const
{
    return getImageDataObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const camera_image_data& cameraBase::getImageDataObj_VC()const
{
    return getImageDataObj(*vcCamPtr);
}
//---------------------------------------------------------------------------------
const camera_image_data& cameraBase::getImageDataObj()const
{
    return getImageDataObj(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
const camera_image_data& cameraBase::getImageDataObj(const cameraStructs::cameraObject& cam)const
{
    return cam.data;
}
//---------------------------------------------------------------------------------
/// clara_DAQ OBJECTS
const clara_DAQ& cameraBase::getClaraDAQObj(const std::string& name)const
{
    return getClaraDAQObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const clara_DAQ& cameraBase::getClaraDAQObj_VC()const
{
    return getClaraDAQObj(*vcCamPtr);
}
//---------------------------------------------------------------------------------
const clara_DAQ& cameraBase::getClaraDAQObj()const
{
    return getClaraDAQObj(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
const clara_DAQ& cameraBase::getClaraDAQObj(const cameraStructs::cameraObject& cam)const
{
    return cam.daq;
}
//---------------------------------------------------------------------------------
/// cameraObject OBJECTS
const cameraObject& cameraBase::getCameraObj(const std::string& name)const
{
    return getCameraObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const cameraObject& cameraBase::getCameraObj_VC()const
{
    return *vcCamPtr;
}
//---------------------------------------------------------------------------------
const cameraObject& cameraBase::getCameraObj()const
{
    return *selectedCamPtr;
}
//---------------------------------------------------------------------------------
const cameraObject& cameraBase::getCameraObj(const cameraStructs::cameraObject& cam)const
{
    return cam;
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraBase::getStateObj(const std::string& name)const
{
    return getStateObj(getCamObj(name));
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraBase::getStateObj_VC()const
{
    return getStateObj(*vcCamPtr);
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraBase::getStateObj()const
{
    return getStateObj(*selectedCamPtr);
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraBase::getStateObj(const cameraStructs::cameraObject& cam)const
{
    return cam.state;
}
//---------------------------------------------------------------------------------

