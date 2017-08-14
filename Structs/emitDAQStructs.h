#ifndef _EMIT_DAQ_STRUCTS_H_
#define _EMIT_DAQ_STRUCTS_H_
//djs
//#include "velaMagnetStructs.h"
#include "globalVelaCams.h"
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

namespace emitDAQStructs
{
    /// this is the main DAQ config that holds all info about the data aquisition
#pragma pack(push)
#pragma pack(1)
    typedef struct emitDAQconfig
    {
        int numMags, numScrs;
        char savDir[256], rfPhase[25], charge[25], Comments[256], fn[25];
        std::vector< std::string > magNames, screenNames;
        std::vector< double > startSIs, endSIs;
        std::vector< int > numSISteps, numImagesWithBeam, numImagesNoBeam, numCycles;
    } emitDAQconfig;

    typedef struct emitScanSaveDataStruct
    {
        emitDAQconfig config;
        int totalNumMags;
        std::vector< std::string > allMagNames;
        std::vector< double > allMagSIs;
        //std::vector< velaMagStructs::MAG_PSU_STATE  > allMagPSUStates;
        int numScreens;
        std::vector< std::string > screenNames;
        std::vector< int > numImagesWithBeamm, numImagesWithNoBeam;
        int numpixelsPerImage;
        int dataWithBeamSize, dataWithNoBeamSize;
        std::vector< std::vector< unsigned char > > dataWithBeam, dataWithNoBeam;
    } emitScanSaveDataStruct;


    typedef struct beamSizeDataStruct
    {
        int numDataScreens;
        std::vector< std::string > screenNames;
        std::vector< velaCamStructs::imgCutStruct > screenImgCutStructs;
        int numMagnets;
        std::vector< std::string > magNames;
        std::vector< double > magSIs;
        std::vector< std::string > scanningMagnets; /// i.e. the magnets that are being scanned
        std::vector< std::vector< unsigned char > > cutDataWithBeam, cutDataWithNoBeam;

    } beamSizeDataStruct;


    typedef struct magnetScanStruct
    {
        size_t numMagnets, numScans, numBeamImages, numBackgroundImages, numRepeats;
        std::string screen;
        std::vector< std::string > magNames;
        std::vector< std::vector< double > > siValues; /// A vector of vectors, each numMagnets long, SI value, for each iteration
    } magnetScanStruct;


    typedef struct emitScanSaveDataStruct2
    {
        int version;
        char comments[512];
        //velaMagStructs::magnetStateStruct magState;
        std::string screen;
        int numImagesWithBeamm, numImagesWithNoBeam, numpixelsPerImage, dataWithBeamSize, dataWithNoBeamSize;
        std::vector< unsigned char > dataWithBeam, dataWithNoBeam;
    } emitScanSaveDataStruct2;






#pragma pack (pop)

    /// add in RF PARAMETERS
    /// raw bpm signals
    /// Need to have a timestamped BPM data and time stamped camera data
}
#endif
