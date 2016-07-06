///
/// Duncan Scott October 2014
///
/// Holds all the stucts required by different classes
/// one place to make updates
///
#ifndef _VELA_CAM_STRUCTS_H_
#define _VELA_CAM_STRUCTS_H_
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
    /// make this a typedef so ity can easily be changed...
    /// we could make template classes out of everything to make this truely fleixble... but that's a lot of work..
    typedef long camDataType;

    struct camObject;
    struct pvStruct;
    struct camImageStruct;
    struct camIOCObject;
    struct monitorStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( CAM_PV_TYPE, (CAM_START) (CAM_STOP) (CAM_GAIN) (CAM_GAIN_RBV) (CAM_BLACK) (CAM_BLACK_RBV) (CAM_DATA) (CAM_NUM_ACTIVE)
                                                      (CAM_STATE) (CAM_LIMIT)  )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( CAM_STATUS, (CAM_ON) (CAM_OFF) (CAM_ERROR)  )


    /// These can't go in VELA_ENUM as they need a pvType.
    struct pvStruct
    { // proviude a default constructor
        pvStruct() : pvSuffix( "UNKNOWN" ), objName( "UNKNOWN"), COUNT( 0 ), MASK(0){}
        CAM_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };
    struct camImageStruct
    { // proviude a default constructor
        camImageStruct() : x0( 0 ), y0( 0 ), xRad( 0 ), xPix2mm(0.0), yPix2mm(0.0),
        xstart( 0 ), xstop( 0 ), ystart( 0 ), ystop( 0 ), numPixX( 0 ), numPixY( 0 ), totNumPix( 0 ), numX( 0 ), numY( 0 ) {}
        size_t x0, y0, xRad, yRad; // these come from the config file
        double xPix2mm, yPix2mm;   // these come from the config file
        size_t xstart, xstop, ystart, ystop, numPixX, numPixY, totNumPix, numX, numY;

    };

    /// currently we only have access to one parameter here

    struct camIOCObject
    { // proviude a default constructor
        camIOCObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), numActiveCameras(0), numIlocks(0), numCamLimit(0) {}
        std::string name, pvRoot;
        size_t numActiveCameras, numIlocks, numCamLimit;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs; // none of these are known about to us
    };

    struct camDataStruct
    {
        camDataStruct() : shotCount( 0 ), numShots( 0 ){}

        /// i moved this so re-compiling should break this...

        int shotCount, numShots; /// we allow -1 values here so NOT a size_t
        std::vector< double > timeStamps;
        std::vector< std::string > strTimeStamps;
        std::vector< std::vector< camDataType > > cameraData;
    };


    struct camObject
    { // proviude a default constructor
        camObject() : name( "NO_NAME" ), pvRoot("NO_PV_ROOT"), screen("NO_SCREEN"), numIlocks(0), state(CAM_ERROR ) {}
        size_t numIlocks;
        std::string name, pvRoot, screen;
        camImageStruct imageStruct;
        CAM_STATUS state;
        long gainRBV, blackRBV;
        camDataStruct rawData;
        std::map< CAM_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CAM_PV_TYPE, pvStruct > pvComStructs;
    };

    struct monitorStruct
    { // proviude a default constructor
        monitorStruct(): objName( "NO_NAME" ), interface(nullptr) {}
        CAM_PV_TYPE       monType;
        std::string       objName;
        chtype            CHTYPE;
        cameraInterface *interface;
        evid              EVID;
    };


    struct camDAQStruct
    { // proviude a default constructor
        camDAQStruct(): objName( "NO_NAME" ), interface(nullptr), threadID( 0 ), thread(nullptr), shotCount( -999 ), numShots (-999 ), shouldProcess( false ) {}
        std::string       objName;
        chtype            CHTYPE;
        cameraInterface *interface;
        evid              EVID;
        std::thread      *thread;
        size_t            threadID;
        int               shotCount, numShots; /// we allow -1 values here so NOT a size_t
        bool              shouldProcess;
    };

//    enum CAM_SERVER_STATE{ k_OFF, k_ON, k_ERR, k_NONE, k_DUMMY };
//
//    enum MON_TYPE{ k_serverStatus, k_data }; /// types of magnet parameters to monitor
//
//    enum DATA_TYPE{ k_DATA, k_BACK }; /// For beam ON, or beam OFF data
//
//    typedef long camType;
//
//    typedef struct camObject
//    {
//        CAM_SERVER_STATE camServerState;
//        camType epicsCamServerState;
//        bool aquiringData;
//    std::string name, serverPV, dataPV, serverNum;
//    #ifndef _OFFLINE_MODE
//    #ifndef __CINT__ /// Channel IDs for camera server status
//        chid serv_Start_chid, serv_Stop_chid, serv_Status_chid, data_chid;
//    #endif
//    #endif
//    } camObject;
//
//    typedef struct monitorStuct
//    {
//        enum MON_TYPE monType;
//        cameraInterface * interface;
//        double value;
//        size_t numShots, shotCount;
//        camType * vecToFill;
//        camObject * camObj;
//    #ifndef _OFFLINE_MODE
//    #ifndef __CINT__
//        evid EventID;
//        struct dbr_time_char * pTD;
//    #endif
//    #endif
//    } monitorStuct;
//
//    typedef struct imgCutStruct
//    {
//        int x0, y0, xRad, yRad;
//        size_t xstart, xstop, ystart, ystop, numPixX, numPixY, totNumPix, numX, numY;
//        double xPix2mm, yPix2mm;
//    } imgCutStruct;
//
//    typedef struct camDataStruct
//    {
//        size_t rawDataSize;
//        std::string camName;
//        imgCutStruct imgParam;
//        camType * rawData;
//    } camDataStruct;
//
//    /// imageCutStruct has all data required to cut an image,
//    /// ! Always use x0 y0 and xRad yRrad to define cuts!
//
//    typedef struct imageCutStruct
//    {
//        int x0, y0, xRad, yRad;
//        size_t xstart, xstop, ystart, ystop, numPixX, numPixY, totNumPix, numX, numY;
//    } imageCutStruct;
//
//    /// These are handy little structs to help pass vectors around for cropping
//    /// numX and numY tell you how to partition the data
//
//    typedef struct imgDatC{
//        std::vector< camType > data;
//        size_t numX, numY;
//    } imgDatC;
//
//    typedef struct projectedMomentsStruct
//    {
//        double meanX, meanY, rmsX, rmsY;
//    } projectedMomentsStruct;
//
//
//    typedef struct imgDatD{
//        std::vector< double > data;
//        size_t numX, numY;
//    } imgDatD;
//
//    typedef struct bvnFit{
//        int status;
//        double Bac,Amp,mux,muy,sxx,syy,sxy;
//        double BacErr,AmpErr,muxErr,muyErr,sxxErr,syyErr,sxyErr;
//    } bvnFit;



}
#endif // _VELA_CAM_STRUCTS_H_
