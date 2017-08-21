//
// Tim Price May 2017
//
// Holds all the stucts required by different classes
// one place to make updates
//
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

namespace cameraStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_PV_TYPE,
        (CAM_FILE_PATH)(CAM_FILE_NAME)(CAM_FILE_NUMBER)(CAM_FILE_TEMPLATE)
        (CAM_FILE_WRITE)(CAM_FILE_WRITE_RBV)(CAM_FILE_WRITE_CHECK)
        (CAM_FILE_WRITE_MESSAGE)(CAM_STATUS)(CAM_ACQUIRE)(CAM_CAPTURE)
        (CAM_CAPTURE_RBV)(CAM_ACQUIRE_RBV)(CAM_NUM_CAPTURE)
        (CAM_NUM_CAPTURE_RBV)(CAM_NUM_CAPTURED)(CAM_DATA)(CAM_BKGRND_DATA)(X)
        (Y)(SIGMA_X)(SIGMA_Y)(COV_XY)(UNKNOWN_CAM_PV_TYPE))

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

    // bit depth of the camera... could be dynamic (!)
    typedef long camDataType;
    struct pvStruct;
    struct monitorDAQStruct;
    struct cameraObject;
    struct cameraDAQObject;
    struct cameraIAObject;

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
    // eventually (aspiration) harmonize this with VELA cams ...
    struct cameraIAObject// image analysis object
    {
        cameraIAObject() :
            x(9999.9999),y(9999.9999),sigmaX(9999.9999),sigmaY(9999.9999),
            covXY(9999.9999),xPix2mm(9999.9999),yPix2mm(9999.9999),xPix(9999),
            yPix(9999),xSigmaPix(9999),ySigmaPix(9999),xyCovPix(999),
            xCenterPix(9999),yCenterPix(9999),xRad(9999),yRad(9999),
            bitDepth(9999),imageHeight(9999),imageWidth(9999){}
        double x,y,sigmaX,sigmaY,covXY,
               xPix2mm, yPix2mm;
        size_t xPix, yPix,xSigmaPix,ySigmaPix,xyCovPix,
               xCenterPix,yCenterPix,xRad,yRad,
               bitDepth,imageHeight,imageWidth;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };
    struct cameraDAQObject
    {
        cameraDAQObject() : name(UTL::UNKNOWN_NAME),
                            pvRoot(UTL::UNKNOWN_PVROOT),
                            screenName(UTL::UNKNOWN_STRING),
                            state(CAM_ERROR),
                            acquireState(ACQUIRING_ERROR),
                            captureState(CAPTURING_ERROR),
                            writeState(WRITING_ERROR),
                            writeCheck(WRITE_CHECK_ERROR),
                            shotsTaken(UTL::DUMMY_INT),
                            numberOfShots(UTL::DUMMY_INT),
                            frequency(UTL::DUMMY_DOUBLE),
                            exposureTime(UTL::DUMMY_DOUBLE) {}
        //ID
        std::string name, pvRoot, screenName;
        //On/Off
        CAM_STATE state;
        // Rolling a
        ACQUIRE_STATE acquireState;
        // actually "capturing" images to then save to disc
        CAPTURE_STATE captureState;
        // write state indicates if saving to disc / or
        WRITE_STATE writeState;
        // write check is whether the last write was succesful
        WRITE_CHECK writeCheck;
        // If error this string will get updated
        std::string writeErrorMessage;
        int shotsTaken, numberOfShots, maxShots;
        double frequency,exposureTime;
        // doesn't exist for CLARA
        std::vector<camDataType> rawData;
        // we're going to store a background image array ion a PV
        std::vector<camDataType> rawBackgroundData;
        VELA_ENUM::MACHINE_AREA  machineArea;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };
    ///Not using this yet but will use it eventually for DAQ and IA
    struct cameraObject
    {
        cameraObject() : name("NO_NAME"), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), state(CAM_ERROR) {}
        std::string name, pvRoot, screenPV;
        CAM_STATE state;
        std::vector<camDataType> rawData;
        std::vector<camDataType> rawBackgroundData;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
        VELA_ENUM::MACHINE_AREA  machineArea;
        cameraDAQObject CDO;
        cameraIAObject CIO;
    };
}
#endif // CAM_STRUCTS_H_
