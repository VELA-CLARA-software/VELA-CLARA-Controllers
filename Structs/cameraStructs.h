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

class cameraDAQInterface;

namespace cameraStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_PV_TYPE, (CAM_FILE_PATH) (CAM_FILE_NAME) (CAM_FILE_NUMBER) (CAM_FILE_TEMPLATE) (CAM_FILE_WRITE) (CAM_FILE_WRITE_RBV) (CAM_FILE_WRITE_STATUS) (CAM_FILE_WRITE_MESSAGE)
                                                     (CAM_STATUS) (CAM_ACQUIRE) (CAM_CAPTURE) (CAM_CAPTURE_RBV) (CAM_ACQUIRE_RBV)(CAM_NUM_CAPTURE)(CAM_NUM_CAPTURED)
                                                     (CAM_DATA) (CAM_BKGRND_DATA)
                                                     (X) (Y) (SIGMA_X) (SIGMA_Y) (COV_XY)
                                                     (UNKNOWN_CAM_PV_TYPE))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_STATE, (CAM_OFF) (CAM_ON) (CAM_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(AQUIRE_STATE, (NOT_ACQUIRING) (ACQUIRING) (AQUIRING_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(WRITE_CHECK, (WRITE_OK) (WRITE_CHECK_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(WRITE_STATE, (WRITE_DONE) (WRITING) (WRITE_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAPTURE_STATE, (CAPTURE_DONE) (CAPTURING) (CAPTURE_ERROR))

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
    struct cameraIAObject
    {
        cameraIAObject() : x(9999.9999),y(9999.9999),sigmaX(9999.9999),sigmaY(9999.9999),covXY(9999.9999),xPix2mm(9999.9999),yPix2mm(9999.9999),
                           xPix(9999),yPix(9999),xSigmaPix(9999),ySigmaPix(9999),xyCovPix(999),xCenterPix(9999),yCenterPix(9999),xRad(9999),yRad(9999),bitDepth(9999),imageHeight(9999),imageWidth(9999){}
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
        cameraDAQObject() : name("NO_NAME"), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), state(CAM_ERROR) {}
        std::string name, pvRoot, screenPV;
        char writeMessage[256];//MAGIC number from EPICS
        CAM_STATE state;
        AQUIRE_STATE aquireState;
        WRITE_STATE writeState;
        WRITE_CHECK writeCheck;
        CAPTURE_STATE captureState;
        int shotsTaken, numberOfShots;
        double frequency,exposureTime;
        std::vector<camDataType> rawData;
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
