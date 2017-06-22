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

class cameraInterface;

namespace cameraStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(DAQ_PV_TYPE, (CAM_START) (CAM_STOP) (CAM_DATA) (CAM_BKGRND_DATA)
                                                     (CAM_FILE_PATH) (CAM_FILE_NAME) (CAM_FILE_TEMPLATE) (PV_SUFFIX_FILE_WRITE)
                                                     (CAM_STATE) (UNKNOWN_CAM_PV_TYPE))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(IA_PV_TYPE,  (CAM_START) (CAM_STOP)
                                                     (X) (Y) (SIGMA_X) (SIGMA_Y) (COV_XY)
                                                     (CAM_STATE) (UNKNOWN_CAM_PV_TYPE))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_STATUS,  (CAM_ACQUIRING) (CAM_NOT ACQUIRING) (CAM_ERROR))

    typedef long camDataType;
    struct pvStruct;
    struct monitorStruct;
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
    struct monitorStruct
    {
        monitorStruct(): monType( UNKNOWN_CAM_PV_TYPE),objName("UNKNOWN"),
                         interface(nullptr),EVID(nullptr){}
        CAM_PV_TYPE      monType;
        std::string      objName;
        chtype           CHTYPE;
        cameraInterface *interface;
        evid             EVID;
    };
    struct cameraIAObject
    {
        cameraIAObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), numIlocks(0), state(CAM_ERROR) {}
        std::string name, pvRoot, screenPV;
        //size_t numIlocks;
        CAM_STATUS state;
        int shotsTaken, numberOfShots;
        size_t bit_depth, image_height, image_width;
        double x,y,sigmaX,sigmaY,covXY
               xPix2mm, yPix2mm;
        size_t xPix, yPix,xSigmaPix,ySigmaPix,xyCovPix,
               xCenterPix,yCenterPix,xRad,yRad,
               bitDepth,imageHeight,imageWidth;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };
    struct cameraDAQStruct
    {
        camDataStruct() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), numIlocks(0), state(CAM_ERROR) {}
        std::string name, pvRoot, screenPV;
        size_t numIlocks;
        CAM_STATUS state;
        int shotsTaken, numberOfShots;
        double frequency,exposureTime;
        std::vector<camDataType> rawData;
        std::vector<camDataType> rawBackgroundData;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };

    struct cameraObject
    { // proviude a default constructor
        cameraObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screenPV("NO_SCREEN_PV"), numIlocks(0), state(CAM_ERROR) {}
        //size_t numIlocks;
        std::string name, pvRoot, screenPV;
        camImageStruct imageStruct;
        CAM_STATUS state;
        camDataStruct rawData;
        VELA_ENUM::MACHINE_AREA  machineArea;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };
}
#endif // CAM_STRUCTS_H_
