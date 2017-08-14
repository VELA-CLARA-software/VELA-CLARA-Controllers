///
/// Duncan Scott October 2014
///
/// Holds all the stucts required by different classes
/// one place to make updates
///
#ifndef _VELA_CAMERA_STRUCTS_H_
#define _VELA_CAMERA_STRUCTS_H_
//stl
#include <vector>
#include <string>
#include <map>
//epics
#ifndef __CINT__
#ifndef _OFFLINE_MODE
#include <cadef.h>
#endif
#endif


class velaCamInterface;

namespace velaCamStructs
{
    enum CAM_SERVER_STATE{ k_OFF, k_ON, k_ERR, k_NONE, k_DUMMY };

    enum MON_TYPE{ k_serverStatus, k_data }; /// types of magnet parameters to monitor

    enum DATA_TYPE{ k_DATA, k_BACK }; /// For beam ON, or beam OFF data

    typedef struct camObject
    {
        CAM_SERVER_STATE camServerState;
        unsigned short epicsCamServerState;
        bool aquiringData;
    std::string name, serverPV, dataPV, serverNum;
    #ifndef _OFFLINE_MODE
    #ifndef __CINT__ /// Channel IDs for camera server status
        chid serv_Start_chid, serv_Stop_chid, serv_Status_chid, data_chid;
    #endif
    #endif
    } camObject;

    typedef struct monitorStuct
    {
        enum MON_TYPE monType;
        velaCamInterface * interface;
        double value;
        size_t numShots, shotCount;
        unsigned char * vecToFill;
        camObject * camObj;
    #ifndef _OFFLINE_MODE
    #ifndef __CINT__
        evid EventID;
        struct dbr_time_char * pTD;
    #endif
    #endif
    } monitorStuct;

    typedef struct imgCutStruct
    {
        int x0, y0, xRad, yRad;
        size_t xstart, xstop, ystart, ystop, numPixX, numPixY, totNumPix, numX, numY;
        double xPix2mm, yPix2mm;
    } imgCutStruct;

    typedef struct camDataStruct
    {
        size_t rawDataSize;
        std::string camName;
        imgCutStruct imgParam;
        unsigned char * rawData;
    } camDataStruct;

    /// imageCutStruct has all data required to cut an image,
    /// ! Always use x0 y0 and xRad yRrad to define cuts!

    typedef struct imageCutStruct
    {
        int x0, y0, xRad, yRad;
        size_t xstart, xstop, ystart, ystop, numPixX, numPixY, totNumPix, numX, numY;
    } imageCutStruct;

    /// These are handy little structs to help pass vectors around for cropping
    /// numX and numY tell you how to partition the data

    typedef struct imgDatC{
        std::vector< unsigned char > data;
        size_t numX, numY;
    } imgDatC;

    typedef struct projectedMomentsStruct
    {
        double meanX, meanY, rmsX, rmsY;
    } projectedMomentsStruct;


    typedef struct imgDatD{
        std::vector< double > data;
        size_t numX, numY;
    } imgDatD;

    typedef struct bvnFit{
        int status;
        double Bac,Amp,mux,muy,sxx,syy,sxy;
        double BacErr,AmpErr,muxErr,muyErr,sxxErr,syyErr,sxyErr;
    } bvnFit;



}
#endif
