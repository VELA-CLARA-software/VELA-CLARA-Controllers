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
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_PV_TYPE, (CAM_FILE_PATH) (CAM_FILE_NAME) (CAM_FILE_TEMPLATE) (CAM_FILE_WRITE) (CAM_FILE_WRITE_STATUS) (CAM_FILE_WRITE_MESSAGE)
                                                     (CAM_STATUS) (CAM_DATA) (CAM_BKGRND_DATA)
                                                     (X) (Y) (SIGMA_X) (SIGMA_Y) (COV_XY)
                                                     (UNKNOWN_CAM_PV_TYPE) (CAM_ACQUIRE) (CAM_ACQUIRE_RBV) (CAM_CAPTURE) (CAM_NUM_CAPTURE)(CAM_NUM_CAPTURED))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_STATE, (CAM_OFF) (CAM_ON) (CAM_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(AQUIRE_STATE, (NOT_ACQUIRING) (ACQUIRING) (AQUIRING_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(WRITE_STATE, (WRITE_OK) (WRITE_ERROR))

    typedef long camDataType;
    struct pvStruct;
    struct monitorDAQStruct;
    struct cameraObject;
    struct cameraDAQObject;
    struct cameraIAObject;

    struct pvStruct
    {
        pvStruct() : pvSuffix( "UNKNOWN" ), objName( "UNKNOWN"), COUNT( 0 ), MASK(0){}
        CAM_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };
    struct monitorDAQStruct
    {
        monitorDAQStruct(): monType( UNKNOWN_CAM_PV_TYPE),objName("UNKNOWN"), interface(nullptr),EVID(nullptr){}
        CAM_PV_TYPE      monType;
        std::string      objName;
        chtype           CHTYPE;
        cameraDAQInterface *interface;
        evid             EVID;
    };
    struct cameraIAObject
    {
        cameraIAObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), state(CAM_ERROR) {}
        std::string name, pvRoot, screenPV;
        //size_t numIlocks;
        CAM_STATE state;
        int shotsTaken, numberOfShots;
        size_t bit_depth, image_height, image_width;
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
        cameraDAQObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), state(CAM_ERROR) {}
        std::string name, pvRoot, screenPV;
        char writeMessage[256];//MAGIC number from EPICS
        CAM_STATE state;
        AQUIRE_STATE aquireState;
        WRITE_STATE writeState;
        int shotsTaken, numberOfShots;
        double frequency,exposureTime;
        std::vector<camDataType> rawData;
        std::vector<camDataType> rawBackgroundData;
        VELA_ENUM::MACHINE_AREA  machineArea;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };

    struct cameraObject
    {
        cameraObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), state(CAM_ERROR) {}
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
