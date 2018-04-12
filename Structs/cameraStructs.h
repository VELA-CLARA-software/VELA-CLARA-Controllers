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
#ifndef CAM_STRUCTS_H_
#define CAM_STRUCTS_H_
//stl
#include <vector>
#include <string>
#include <thread>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

#include "structs.h"
#include "configDefinitions.h"


class cameraDAQInterface;

class cameraIAInterface;

class editor;

class offlineImageAnalyser;

namespace cameraStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_PV_TYPE,
        (TEST)(CAM_FILE_PATH)(CAM_FILE_NAME)(CAM_FILE_NUMBER)(CAM_FILE_TEMPLATE)
        (CAM_FILE_WRITE)(CAM_FILE_WRITE_RBV)(CAM_FILE_WRITE_CHECK)
        (CAM_FILE_WRITE_MESSAGE)(CAM_STATUS)(CAM_START_ACQUIRE)(CAM_STOP_ACQUIRE)
        (CAM_CAPTURE)(CAM_CAPTURE_RBV)(CAM_ACQUIRE_RBV)(CAM_NUM_CAPTURE)
        (CAM_NUM_CAPTURE_RBV)(CAM_NUM_CAPTURED)(CAM_DATA)(CAM_BKGRND_DATA)
        (JPG_FILE_NAME) (JPG_FILE_PATH) (JPG_FILE_NUMBER)
        (JPG_FILE_WRITE)(JPG_FILE_WRITE_RBV)(JPG_FILE_WRITE_CHECK)
        (JPG_FILE_WRITE_MESSAGE)
        (JPG_CAPTURE)(JPG_CAPTURE_RBV)(JPG_NUM_CAPTURE)
        (JPG_NUM_CAPTURE_RBV)(JPG_NUM_CAPTURED)
        (START_IA)(PIX_MM)(STEP_SIZE)(SET_BKGRND)(USE_BKGRND)(USE_NPOINT)
        (X)(Y)(SIGMA_X)(SIGMA_Y)(COV_XY)(SUMMED_PIX_INETSITY)
        (X_PIX)(Y_PIX)(SIGMA_X_PIX)(SIGMA_Y_PIX)(COV_XY_PIX)
        (MASK_X)(MASK_Y)(MASK_X_RAD)(MASK_Y_RAD)(X_CENTER)(Y_CENTER)
        (BIT_DEPTH)(UNKNOWN_CAM_PV_TYPE)
        (CAM_EXPOSURE_TIME) (CAM_ACQUIRE_PERIOD) (CAM_FREQ) (CAM_SENSOR_TEMP)
        (START_IA_RBV) (USE_BKGRND_RBV) (USE_NPOINT_RBV))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_STATE,
        (CAM_OFF) (CAM_ON) (CAM_ERROR))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(ACQUIRE_STATE,
        (NOT_ACQUIRING) (ACQUIRING) (ACQUIRING_ERROR))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAPTURE_STATE,
        (NOT_CAPTURING) (CAPTURING) (CAPTURING_ERROR))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(WRITE_STATE,
        (NOT_WRITING) (WRITING) (WRITING_ERROR))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(WRITE_CHECK,
        (WRITE_CHECK_OK) (WRITE_CHECK_ERROR))

    enum EstMethod {FWHM,moments};
    enum projAxis {x,y,maskX,maskY};
    // bit depth of the camera... could be dynamic (!)
    typedef long camDataType;
    struct pvStruct;
    struct monitorDAQStruct;
    struct cameraObject;
    struct cameraDAQObject;
    struct cameraIAObject;
    struct cameraOfflineIAObject;

    struct pvStruct
    {
        pvStruct() : pvSuffix("UNKNOWN"), objName("UNKNOWN"), COUNT(0), MASK(0){}
        CAM_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };
    struct monitorDAQStruct
    {
        monitorDAQStruct(): monType(UNKNOWN_CAM_PV_TYPE),objName("UNKNOWN"), interface(nullptr),EVID(nullptr){}
        CAM_PV_TYPE      monType;
        std::string      objName;
        chtype           CHTYPE;
        cameraDAQInterface *interface;
        evid             EVID;
    };
    struct monitorIAStruct
    {
        monitorIAStruct(): monType(UNKNOWN_CAM_PV_TYPE),objName("UNKNOWN"), interface(nullptr),EVID(nullptr){}
        CAM_PV_TYPE      monType;
        std::string      objName;
        chtype           CHTYPE;
        cameraIAInterface *interface;
        evid             EVID;
    };
    // eventually (aspiration) harmonize this with VELA cams ...
    struct cameraIAObject// image analysis object
    {
        cameraIAObject() : x(UTL::DUMMY_DOUBLE),
                           y(UTL::DUMMY_DOUBLE),
                           sigmaX(UTL::DUMMY_DOUBLE),
                           sigmaY(UTL::DUMMY_DOUBLE),
                           covXY(UTL::DUMMY_DOUBLE),
                           pix2mm(UTL::DUMMY_DOUBLE),
                           xPix(UTL::DUMMY_INT),
                           yPix(UTL::DUMMY_INT),
                           sigmaXPix(UTL::DUMMY_INT),
                           sigmaYPix(UTL::DUMMY_INT),
                           covXYPix(UTL::DUMMY_INT),
                           xCenterPix(UTL::DUMMY_INT),
                           yCenterPix(UTL::DUMMY_INT),
                           maskX(UTL::DUMMY_INT),
                           maskY(UTL::DUMMY_INT),
                           maskXRad(UTL::DUMMY_INT),
                           maskYRad(UTL::DUMMY_INT),
                           bitDepth(UTL::DUMMY_INT),
                           summedIntensity(UTL::DUMMY_INT){}
                           //imageHeight(UTL::DUMMY_INT),
                           //imageWidth(UTL::DUMMY_INT){}

        double x,y,sigmaX,sigmaY,covXY,pix2mm;
        size_t xPix, yPix,sigmaXPix,sigmaYPix,covXYPix,
               xCenterPix,yCenterPix,maskXRad,maskYRad,maskX,maskY,
               bitDepth,summedIntensity;
        bool useBkgrnd, analysisState, useNPoint;
               //imageHeight,imageWidth;
        //std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        //std::map< CAM_PV_TYPE, pvStruct > pvComStructs;

        //Time of last background set
        //.......put here....
    };
    struct cameraDAQObject
    {
        cameraDAQObject() : name(UTL::UNKNOWN_NAME),
                            pvRoot(UTL::UNKNOWN_PVROOT),
                            screenName(UTL::UNKNOWN_STRING),
                            //state(CAM_ERROR),
                            //acquireState(ACQUIRING_ERROR),
                            captureState(CAPTURING_ERROR),
                            writeState(WRITING_ERROR),
                            writeCheck(WRITE_CHECK_ERROR),
                            captureStateJPG(CAPTURING_ERROR),
                            writeStateJPG(WRITING_ERROR),
                            writeCheckJPG(WRITE_CHECK_ERROR),
                            shotsTaken(UTL::DUMMY_INT),
                            numberOfShots(UTL::DUMMY_INT),
                            shotsTakenJPG(UTL::DUMMY_INT),
                            numberOfShotsJPG(UTL::DUMMY_INT),
                            frequency(UTL::DUMMY_DOUBLE),
                            exposureTime(UTL::DUMMY_DOUBLE) {}
        //ID
        std::string name, pvRoot, screenName;
        //On/Off
        //CAM_STATE state;
        // Rolling a
        //ACQUIRE_STATE acquireState;
        // actually "capturing" images to then save to disc
        CAPTURE_STATE captureState;
        // write state indicates if saving to disc / or
        WRITE_STATE writeState;
        // write check is whether the last write was succesful
        WRITE_CHECK writeCheck;
        // If error this string will get updated
        std::string writeErrorMessage, latestDirectory;
        int shotsTaken, numberOfShots, maxShots, shotsTakenJPG, numberOfShotsJPG;
        double frequency,exposureTime,acquisitionPeriod, sensorTemp;
        // doesn't exist for CLARA
        std::vector<camDataType> rawData;
        // we're going to store a background image array ion a PV
        std::vector<camDataType> rawBackgroundData;
        //HWC_ENUM::MACHINE_AREA  machineArea;
        //std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        //std::map< CAM_PV_TYPE, pvStruct > pvComStructs;

        CAPTURE_STATE captureStateJPG;
        // write state indicates if saving to disc / or
        WRITE_STATE writeStateJPG;
        // write check is whether the last write was succesful
        WRITE_CHECK writeCheckJPG;
        // If error this string will get updated
        std::string writeErrorMessageJPG;
    };
    struct cameraOfflineIAObject
    {
        cameraOfflineIAObject(): dataSize(0), imageName("None"), bkgrndName("None"){}
        //Images
        std::string imageName;
        std::string bkgrndName;
        std::vector<double> rawData;
        std::vector<double> rawBackgroundData;
        std::vector<double> mask;
        std::vector<double> xPos;
        std::vector<double> yPos;
        //Projections of X and Y
        std::vector<double> xProjection;
        std::vector<double> yProjection;
        std::vector<double> maskXProjection;
        std::vector<double> maskYProjection;

        //Metadata
        int dataSize=0;
        int imageHeight=0, imageWidth=0;
        int rotation=0; // degrees
        int x0=0,y0=0,xRad=0,yRad=0;
        double pixToMM=1.0;
        double pixelValueError = 3.07;//MAGIC NUMBER got from finding the sigma on the pixel image on a black image from clara (tested cameras S01-CAM-01 and S02-CAM-02)

        //Results
        double xBVN=0,yBVN=0,sxBVN=0,syBVN=0,cxyBVN=0;
        double xMLE=0,yMLE=0,sxMLE=0,syMLE=0,cxyMLE=0;
        double xBVNerr=0,yBVNerr=0,sxBVNerr=0,syBVNerr=0,cxyBVNerr=0;
        double xMLEerr=0,yMLEerr=0,sxMLEerr=0,syMLEerr=0,cxyMLEerr=0;
        int totalPixelIntensity;

        //Settings
        bool useBkgrnd=false;
        bool useManualCrop=false;
        bool useBVN=false;

        //Expert Settings (ES) Options
        bool useMaskFromES=false;
        int maskXES=0,maskYES=0,maskRXES=0,maskRYES=0;
        bool usePixToMmFromES=false;
        double pixToMmES=0.;
        bool useRRThresholdFromES=false;
        double RRThresholdES=0.;
        bool useSigmaCutFromES=false;
        double sigmaCutES=0.;
        bool useFilterFromES=false;
        int filterES=0;
        bool useDirectCutLevelFromES=false;
        double DirectCutLevelES=0.;
        int manualCropX=0,manualCropY=0,manualCropW=0,manualCropH=0;
        int savedCroppedX=0,savedCroppedY=0;

        void clear(){
            cameraOfflineIAObject wipedData;
            (*this) = wipedData;
        }
        void operator=(const cameraOfflineIAObject& ID){
            (*this).rawData=ID.rawData;
            (*this).mask=ID.mask;
            (*this).xPos=ID.xPos;
            (*this).yPos=ID.yPos;
            (*this).rawBackgroundData=ID.rawBackgroundData;
            (*this).xProjection=ID.xProjection;
            (*this).yProjection=ID.yProjection;
            (*this).maskXProjection=ID.maskXProjection;
            (*this).maskYProjection=ID.maskYProjection;

            (*this).dataSize=ID.dataSize;
            (*this).imageWidth=ID.imageWidth;
            (*this).imageHeight=ID.imageHeight;
            (*this).x0=ID.x0;
            (*this).y0=ID.y0;
            (*this).xRad=ID.xRad;
            (*this).yRad=ID.yRad;
            (*this).xBVN=ID.xBVN;
            (*this).yBVN=ID.yBVN;
            (*this).sxBVN=ID.sxBVN;
            (*this).syBVN=ID.syBVN;
            (*this).cxyBVN=ID.cxyBVN;
            (*this).xMLE=ID.xMLE;
            (*this).yMLE=ID.yMLE;
            (*this).sxMLE=ID.sxMLE;
            (*this).syMLE=ID.syMLE;
            (*this).cxyMLE=ID.cxyMLE;
            (*this).xBVNerr=ID.xBVNerr;
            (*this).yBVNerr=ID.yBVNerr;
            (*this).sxBVNerr=ID.sxBVNerr;
            (*this).syBVNerr=ID.syBVNerr;
            (*this).cxyBVNerr=ID.cxyBVNerr;
            (*this).xMLEerr=ID.xMLEerr;
            (*this).yMLEerr=ID.yMLEerr;
            (*this).sxMLEerr=ID.sxMLEerr;
            (*this).syMLEerr=ID.syMLEerr;
            (*this).cxyMLEerr=ID.cxyMLEerr;
            (*this).pixToMM=ID.pixToMM;
            (*this).totalPixelIntensity=ID.totalPixelIntensity;
            (*this).useBkgrnd=ID.useBkgrnd;
            (*this).useManualCrop=ID.useManualCrop;
            (*this).useBVN=ID.useBVN;
            (*this).useMaskFromES=ID.useMaskFromES;
            (*this).maskXES=ID.maskXES;
            (*this).maskYES=ID.maskYES;
            (*this).maskRXES=ID.maskRXES;
            (*this).maskRYES=ID.maskRYES;
            (*this).pixToMmES=ID.pixToMmES;
            (*this).useRRThresholdFromES=ID.useRRThresholdFromES;
            (*this).RRThresholdES=ID.RRThresholdES;
            (*this).useSigmaCutFromES=ID.useSigmaCutFromES;
            (*this).sigmaCutES=ID.sigmaCutES;
            (*this).useFilterFromES=ID.useFilterFromES;
            (*this).filterES=ID.filterES;
            (*this).useDirectCutLevelFromES=ID.useDirectCutLevelFromES;
            (*this).DirectCutLevelES=ID.DirectCutLevelES;
            (*this).manualCropX=ID.manualCropX;
            (*this).manualCropY=ID.manualCropY;
            (*this).manualCropW=ID.manualCropW;
            (*this).manualCropH=ID.manualCropH;
            (*this).savedCroppedX=ID.savedCroppedX;
            (*this).savedCroppedY=ID.savedCroppedY;
            return;
        }
    };
    ///Not using this yet but will use it eventually for DAQ and IA
    struct cameraObject
    {
        cameraObject() : name(UTL::UNKNOWN_NAME),
                         pvRoot(UTL::UNKNOWN_PVROOT),
                         screenName(UTL::UNKNOWN_STRING),
                         streamingIPAddress(UTL::UNKNOWN_STRING),
                         state(CAM_ERROR) {}
        std::string name, pvRoot, screenName, streamingIPAddress;
        CAM_STATE state;
        // Rolling acquire
        ACQUIRE_STATE acquireState;
        //std::vector<camDataType> rawData;
        //std::vector<camDataType> rawBackgroundData;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
        HWC_ENUM::MACHINE_AREA  machineArea;
        cameraDAQObject DAQ;
        cameraIAObject IA;
    };


}
#endif // CAM_STRUCTS_H_
