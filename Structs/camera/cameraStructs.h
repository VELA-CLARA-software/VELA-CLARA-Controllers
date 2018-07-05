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
//  Author:      TP, then DJS
//  Last edit:   16-05-2018
//  FileName:    cameraStructs.h
//  Description:
//
//
//
//
//*/
#ifndef _VC_CAMERA_STRUCTS_H_
#define _VC_CAMERA_STRUCTS_H_
//stl
#include <vector>
#include <string>
#include <thread>
#include <deque>
#include <map>
#include <thread>
//epics
#include <cadef.h>
// project includes
#include "structs.h"
#include "configDefinitions.h"
#include "running_stat.h"

#ifdef BUILD_DLL
#include <boost/python.hpp>
#endif
/*
    forward declare as imageCollectStruct has a cameraBase* member
*/
class cameraBase;
//______________________________________________________________________________
namespace cameraStructs
{
    /*
        forward declare as imageCollectStruct has a cameraObject* member
    */
    struct cameraObject;
    /*
        enum for all PVs that are monitored or commands
    */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_PV_TYPE,
            (TEST)// ??

            (CAM_FILE_PATH)(CAM_FILE_NAME)(CAM_FILE_NUMBER)(CAM_FILE_TEMPLATE)
            (CAM_FILE_WRITE)(CAM_WRITE_FILE_RBV)(CAM_FILE_WRITE_STATUS)
            (CAM_FILE_WRITE_ERROR_MESSAGE_RBV)(CAM_STATUS)(CAM_START_ACQUIRE)(CAM_STOP_ACQUIRE)

            (CAM_NUM_CAPTURED_RBV)

            (CAM_CAPTURE)(CAM_CAPTURE_RBV)(CAM_ACQUIRE_RBV)(CAM_NUM_CAPTURE)
            (CAM_NUM_CAPTURE_RBV)(CAM_NUM_CAPTURED)(CAM_DATA)(CAM_BKGRND_DATA)


            (JPG_FILE_NAME) (JPG_FILE_PATH) (JPG_FILE_NUMBER)
            (JPG_CAPTURE)(JPG_FILE_WRITE)(JPG_NUM_CAPTURED)

            (JPG_FILE_WRITE_RBV)(JPG_FILE_WRITE_CHECK)
            (JPG_FILE_WRITE_MESSAGE)
            (JPG_CAPTURE_RBV)(JPG_NUM_CAPTURE)
            (JPG_NUM_CAPTURE_RBV)

            (STEP_SIZE)//how ROI is decimated before online analysis
            (STEP_SIZE_RBV)//how ROI is decimated before online analysis

            (START_IA)(SET_BKGRND)(USE_BKGRND)(USE_NPOINT)
            (START_IA_RBV)(USE_BKGRND_RBV) (USE_NPOINT_RBV)

            (ARRAY_DATA)

            (X_RBV)(Y_RBV)(SIGMA_X_RBV)(SIGMA_Y_RBV)(COV_XY_RBV)(AVG_PIX_INTENSITY_RBV)
                                        (SUM_PIX_INTENSITY_RBV)
            (X_PIX_RBV)(Y_PIX_RBV)(SIGMA_X_PIX_RBV)(SIGMA_Y_PIX_RBV)(COV_XY_PIX_RBV)

            (PIXEL_RESULTS_RBV)

            (MASK_X)(MASK_Y)(MASK_X_RAD)(MASK_Y_RAD)
            (MASK_X_RBV)(MASK_Y_RBV)(MASK_X_RAD_RBV)(MASK_Y_RAD_RBV)

            (X_CENTER_RBV)(Y_CENTER_RBV)(PIX_MM_RBV)
            (X_CENTER)(Y_CENTER)(PIX_MM)

            (BIT_DEPTH)(UNKNOWN_CAM_PV_TYPE)

            (CAM_EXPOSURE_TIME_RBV) (CAM_ACQUIRE_PERIOD_RBV) (CAM_FREQ_RBV) (CAM_SENSOR_TEMP_RBV)
             )

    // so far, two types of camera
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_TYPE,
                                        (VELA_CAM)(CLARA_CAM)(UNKNOWN_CAM))
    // ON/OFF state of camera
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_STATE,
                                        (CAM_OFF)(CAM_ON)(CAM_ERROR))
    // is acquiring data state of camera
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(ACQUIRE_STATE,
                                        (NOT_ACQUIRING)(ACQUIRING)(ACQUIRING_ERROR))

    /*
        DAQ states, i.e when collecting or saving to disk
    */
    // is capturing state of camera (saving???)
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(COLLECTING_STATE,
                                        (NOT_COLLECTING)(COLLECTING)(COLLECTING_ERROR))
    // write state of camera (writing data to disk)
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SAVE_STATE,
                                        (NOT_SAVING)(SAVING)(SAVING_ERROR))
    // not sure yet ...
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(WRITE_CHECK,
                                        (WRITE_CHECK_OK) (WRITE_CHECK_ERROR))


    struct pvStruct
    {
        pvStruct() :
            pvSuffix(UTL::UNKNOWN_PVSUFFIX),
            objName(UTL::UNKNOWN_NAME),
            COUNT(UTL::ZERO_INT),
            MASK(UTL::ZERO_INT),
            pvType(CAM_PV_TYPE::UNKNOWN_CAM_PV_TYPE)
            {}
        CAM_PV_TYPE     pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };

    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN_CAM_PV_TYPE),
            objName(UTL::UNKNOWN_NAME),
            camInterface(nullptr),
            EVID(nullptr)
            {}
        CAM_PV_TYPE   monType;
        std::string   objName;
        chtype        CHTYPE;
        evid          EVID;
        /*
            https://stackoverflow.com/questions/4937180/a-base-class-pointer-can-point-to-a-derived-class-object-why-is-the-vice-versa
        */
        cameraBase*    camInterface;
    };

    /*
        Image collection and saving happens in a new thread,
        this struct is passed to the new thread function
    */
    struct imageCollectStruct
    {
        imageCollectStruct():
            camInterface(nullptr),
            thread(nullptr),
            isBusy(false),
            success(false),
            numShots(UTL::ZERO_INT)
            {}
        /*
            https://stackoverflow.com/questions/4937180/a-base-class-pointer-can-point-to-a-derived-class-object-why-is-the-vice-versa
        */
        cameraBase*    camInterface;
        cameraObject*  camObj;
        std::thread*   thread;
        int            numShots;
        bool           isBusy;
        bool           success;
    };

    /*
        analysis_mask define an simple ellipse ROI that is used
        for the analysis
    */
    struct analysis_mask
    {
        analysis_mask():
            name(UTL::UNKNOWN_NAME),
            mask_x(UTL::DUMMY_INT),
            mask_y(UTL::DUMMY_INT),
            mask_x_rad(UTL::DUMMY_INT),
            mask_y_rad(UTL::DUMMY_INT),
            mask_x_def(UTL::DUMMY_INT),
            mask_y_def(UTL::DUMMY_INT),
            mask_x_rad_def(UTL::DUMMY_INT),
            mask_y_rad_def(UTL::DUMMY_INT),
            maskInterface(nullptr)
            {};
        std::string name;
        /*
            These are in pixels counting from the bottom LHS (?)
        */
        int mask_x,mask_y,mask_x_rad,mask_y_rad;
        /*
            default states housl dbe set to comver screen,
            when you need to reset th emask you would use these values,
            the actual values are defined in the config file
        */
        int mask_x_def,mask_y_def,mask_x_rad_def,mask_y_rad_def;
        /*
            setter functions
        */
        void maskX(int v);
        void maskY(int v);
        void maskXrad(int v);
        void maskYrad(int v);
        cameraBase* maskInterface;
    };
    /*
        camera_state defines what analysis methods to be using
    */
    struct camera_state
    {
        camera_state():
            use_npoint(false),
            use_background(false),
            analysing(false),
            power(CAM_ERROR),
            acquire(ACQUIRING_ERROR)
            {}
        /*
            flags for analysis
        */
        bool analysing,use_npoint,use_background;
        /*
            is camera switched ON / OFF (PSU state)
        */
        CAM_STATE power;
        /*
            is the camera acquiring data
        */
        ACQUIRE_STATE acquire;
    };

    /*
        analysis_data, all online analysis goes here, and is buffered and updating in runningStats
        also
    */
    struct analysis_data
    {
        analysis_data():
            name(UTL::UNKNOWN_NAME),
            pix_2_mm(UTL::DUMMY_DOUBLE),
            x(UTL::DUMMY_DOUBLE),
            y(UTL::DUMMY_DOUBLE),
            sig_x(UTL::DUMMY_DOUBLE),
            sig_y(UTL::DUMMY_DOUBLE),
            sig_xy(UTL::DUMMY_DOUBLE),
            x_pix(UTL::DUMMY_DOUBLE),
            y_pix(UTL::DUMMY_DOUBLE),
            sig_x_pix(UTL::DUMMY_DOUBLE),
            sig_y_pix(UTL::DUMMY_DOUBLE),
            sig_xy_pix(UTL::DUMMY_DOUBLE),
            avg_pix(UTL::DUMMY_DOUBLE),
            sum_pix(UTL::DUMMY_DOUBLE),
            x_centre(UTL::ZERO_SIZET),
            y_centre(UTL::ZERO_SIZET),
            x_centre_def(UTL::ZERO_SIZET),
            y_centre_def(UTL::ZERO_SIZET),
            num_pix_values(UTL::ZERO_SIZET),
            max_buffer_count(UTL::TEN_SIZET),
            buffer_count(UTL::ZERO_SIZET),
            step_size(UTL::DUMMY_INT),
            buffer_full(false)
            {};
        std::string name;
        /*
            how to decimate the image when analysing
        */
        int step_size;
        /*
            how many pixels to mm,
            same in both horizontal and vertical?
            _def version is set in config file
        */
        double pix_2_mm, pix_2_mm_def;
        /*
            where is the centre of the image
            which pixels are defined as (0,0) mm co-ord
            _def version to be set in config file
        */
        size_t x_centre, y_centre;
        size_t x_centre_def, y_centre_def;
        /*
            individual analysis results
        */
        double x,y,sig_x,sig_y,sig_xy,x_pix,y_pix,
               avg_pix,sum_pix,
               sig_x_pix,sig_y_pix,sig_xy_pix;
        /*
            buffers for pixel values
        */
        size_t max_buffer_count,buffer_count;
        bool buffer_full;
        std::deque<double> x_buf,y_buf,sig_x_buf,sig_y_buf,sig_xy_buf,
                            avg_pix_buf,sum_pix_buf,
                            x_pix_buf,y_pix_buf,sig_x_pix_buf,sig_y_pix_buf,sig_xy_pix_buf;
        /*
            time stamped pixel array readback
            This is the analysis Data, in a single waveform
        */
        size_t num_pix_values;
        std::vector<double> pix_values;
        std::deque<std::vector<double>> pix_values_buf;
        std::string pix_values_time;
        /*
            this map is defined in the config file
            and tells us which element is which
            analysis data for the pixel array RBV
            e.g.
                pix_values[0] = x
                pix_values[1] = y
                pix_values[2] = sig_x
                pix_values[3] = sig_y
                pix_values[4] = sig_xy   /// IS THIS CORRECT ????
        */
        std::map<size_t, std::string> pixel_values_pos;
        std::map<std::string, double> pixel_values;
#ifdef BUILD_DLL
        boost::python::dict pixel_values_dict;
#endif
        /*
            running stats of the analysis results
            rs_count how many stats to add to running stat,
            0 will be 'continuous adding'
            rs_complete collected rs_count values true/false
        */
        runningStat x_rs,y_rs,sig_x_rs,sig_y_rs,sig_xy_rs,x_pix_rs,
                    avg_pix_rs,sum_pix_rs,
                    y_pix_rs,sig_x_pix_rs,sig_y_pix_rs,sig_xy_pix_rs;
        /*
            running-stats "getters" and clear
            you can also set a biffer size for _rs, but not yet implemented
        */
        void x_clear();
        void y_clear();
        void sig_x_clear();
        void sig_y_clear();
        void sig_xy_clear();
        void x_pix_clear();
        void y_pix_clear();
        void sig_x_pix_clear();
        void sig_y_pix_clear();
        void sig_xy_pix_clear();
        void avg_pix_clear();
        void sum_pix_clear();
        double x_mean();
        double y_mean();
        double sig_x_mean();
        double sig_y_mean();
        double sig_xy_mean();
        double x_pix_mean();
        double y_pix_mean();
        double sig_x_pix_mean();
        double sig_y_pix_mean();
        double sig_xy_pix_mean();
        double avg_pix_mean();
        double sum_pix_mean();
        double x_sd();
        double y_sd();
        double sig_x_sd();
        double sig_y_sd();
        double sig_xy_sd();
        double x_pix_sd();
        double y_pix_sd();
        double sig_x_pix_sd();
        double sig_y_pix_sd();
        double sig_xy_pix_sd();
        double avg_pix_sd();
        double sum_pix_sd();
        size_t x_n();
        size_t y_n();
        size_t sig_x_n();
        size_t sig_y_n();
        size_t sig_xy_n();
        size_t x_pix_n();
        size_t y_pix_n();
        size_t sig_x_pix_n();
        size_t sig_y_pix_n();
        size_t sig_xy_pix_n();
        size_t avg_pix_n();
        size_t sum_pix_n();
    };
    /*
        fast_image is an VELA pixel array or decimated CLARA pixel array
    */
    struct fast_image
    {
        fast_image():
            name(UTL::UNKNOWN_NAME),
            num_pix_x(UTL::ZERO_SIZET),
            num_pix_y(UTL::ZERO_SIZET),
            x_pix_to_mm(UTL::DUMMY_DOUBLE),
            y_pix_to_mm(UTL::DUMMY_DOUBLE),
            array_data_sum(UTL::ZERO_SIZET),
            bit_depth(UTL::ZERO_SIZET),
            x_pix_scale_factor(UTL::ZERO_SIZET),
            y_pix_scale_factor(UTL::ZERO_SIZET),
            fast_image_interface(nullptr),
            array_data_min(UTL::DUMMY_INT),
            array_data_max(UTL::DUMMY_INT)
            {}
        std::string name;
        size_t num_pix_x,num_pix_y, bit_depth,x_pix_scale_factor,y_pix_scale_factor;
        // not sure if these are ever used ...
        double x_pix_to_mm,y_pix_to_mm;
        /*
          Actual Array Data
        */
        std::vector<int> array_data, background_array;
        #ifdef BUILD_DLL
//            boost::python::list array_data_Py();
//            boost::python::list array_data2D_Py();
//            boost::python::list background_data_Py();
//            boost::python::list background_data_2D_Py();
            boost::python::list data,data2D,background,background2D;
        #endif
        cameraBase*    fast_image_interface;
        size_t array_data_sum, background_data_sum;
        int array_data_min, array_data_max;
    };
    /*
        combination of all strucst that make up image analysis data
    */
    struct camera_image_data
    {

        fast_image    image;
        analysis_data analysis;
        analysis_mask mask;
    };

    struct clara_DAQ
    {
        clara_DAQ():
            name(UTL::UNKNOWN_NAME),
            screenName(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            /*
                state of colecting images and  writing
            */
            collectingState(COLLECTING_ERROR),
            writeState(SAVING_ERROR),
            /*
                writing error message goes here
            */
            writeCheck(WRITE_CHECK_ERROR),
            /*
                shotsTaken - how many 'shots'(images) acquired
                numberOfShots - how many 'shots'(images) to acquire
                maxShots is set in config file
            */
            shotsTaken(UTL::DUMMY_INT),
            numberOfShots(UTL::DUMMY_INT),
            maxShots(UTL::DUMMY_INT),
            /*
                not sure about JPG versions
            */
            collectingStateJPG(COLLECTING_ERROR),
            writeStateJPG(SAVING_ERROR),
            writeCheckJPG(WRITE_CHECK_ERROR),
            shotsTakenJPG(UTL::DUMMY_INT),
            numberOfShotsJPG(UTL::DUMMY_INT),
            /*
                cam settings
            */
            frequency(UTL::DUMMY_DOUBLE),
            exposureTime(UTL::DUMMY_DOUBLE),
            writeErrorMessageJPG(UTL::UNKNOWN_STRING),
            latestFilename(UTL::UNKNOWN_STRING),
            latestDirectory(UTL::UNKNOWN_STRING),
            /*
                camera sensor temps
            */
            sensorTemp(UTL::DUMMY_DOUBLE),
            sensorMaxTemp(UTL::DUMMY_DOUBLE),
            sensorMinTemp(UTL::DUMMY_DOUBLE)
            {}
        //ID
        std::string name, pvRoot, screenName;
        //On/Off
        //CAM_STATE state;
        // Rolling a
        //ACQUIRE_STATE acquireState;
        // actually "capturing" images to then save to disc
        COLLECTING_STATE collectingState;
        // write state indicates if saving to disc / or
        SAVE_STATE writeState;
        // write check is whether the last write was succesful
        WRITE_CHECK writeCheck;
        // If error this string will get updated
        std::string writeErrorMessage;
        int shotsTaken, numberOfShots, maxShots, shotsTakenJPG, numberOfShotsJPG;
        double frequency,exposureTime,acquisitionPeriod,
               sensorTemp,sensorMaxTemp,sensorMinTemp;
        // we're going to store a background image array in a PV
        //HWC_ENUM::MACHINE_AREA  machineArea;
        //std::map<CAM_PV_TYPE, pvStruct> pvMonStructs;
        //std::map<CAM_PV_TYPE, pvStruct> pvComStructs;
        COLLECTING_STATE collectingStateJPG;
        SAVE_STATE writeStateJPG;
        WRITE_CHECK writeCheckJPG;
        std::string writeErrorMessageJPG,latestFilename,latestDirectory;
    };

    /*
        This is the main object it needs to
        to contain everything a camera could ...
    */
    struct cameraObject
    {
        cameraObject() :
            name(UTL::UNKNOWN_NAME),
            type(CAM_TYPE::UNKNOWN_CAM),
            machineArea(HWC_ENUM::UNKNOWN_AREA),
            pvRoot(UTL::UNKNOWN_PVROOT),
            screenName(UTL::UNKNOWN_STRING),
            streamingIPAddress(UTL::UNKNOWN_STRING),
            numIlocks(UTL::ZERO_SIZET)
            {}
        HWC_ENUM::MACHINE_AREA  machineArea;


        std::string name, pvRoot, screenName, streamingIPAddress;
        /*
            type of camera, VELA, CLARA ...
        */
        CAM_TYPE type;
        /*
            conatins vairous states of the camera
        */
        camera_state state;
        /*
            saving data struct for CLARA system
        */
        clara_DAQ daq;
        /*
            a camera_image_data conatains
            The mask used to analys data  (if clara cam type)
            The data results from analysis (if clara cam type)
            fast image data
        */
        camera_image_data data;
        /*
            ALL PVs should end up here !!!
        */
        std::map<CAM_PV_TYPE, pvStruct> pvMonStructs;
        std::map<CAM_PV_TYPE, pvStruct> pvComStructs;
        size_t numIlocks;
    };



}
#endif //_VC_CAMERA_STRUCTS_H_
