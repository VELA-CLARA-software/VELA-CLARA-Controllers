#ifndef __DJS_GLOBAL_VELA_CAMS_H
#define __DJS_GLOBAL_VELA_CAMS_H
// djs
#include "velaCamStructs.h"
// stl
#include <string>
#include <vector>
#include <map>


//#ifndef __CINT__
//#ifndef _OFFLINE_MODE
//#include <cadef.h>
//#endif
//#endif

namespace globalVelaCams
{
//    const std::vector< std::string > getCamPVs();
    const std::vector< std::string > getCamServerPVs();
    const std::vector< std::string > getCamServerNums();
    const std::vector< std::string > getCamDataPVs();
    const std::vector< std::string > getCamNames();
    const std::vector< std::string > getScreenNames();

    extern const size_t numCams;
    extern const size_t standardNumPixX;
    extern const size_t standardNumPixY;
    extern const size_t standardTotPix;

    extern const velaCamStructs::imgCutStruct dummyImgCutStruct;

    const size_t getNumPix();

    const std::map< std::string, std::string > getCamServerNumMap();
    const std::map< std::string, std::string > getCamServerPVMap();
    const std::map< std::string, std::string > getCamDataPVMap();

    /// We refer to cameras by the associated screen, it's much less ambiguous that way
    /// so the following should not be used often

//    const std::map< std::string, std::string > getCamDataPVMap();
//    const std::map< std::string, std::string > getCamServerPVMap();
//    const std::map< std::string, std::string > getCamServerNumMap();
//
//    extern const std::vector< std::string > camServerNums;
//    extern const std::vector< std::string > camServerPVs;
//    extern const std::vector< std::string > camDataPVs;
//    extern const std::vector< std::string > camNames;
//    extern const std::vector< std::string > screenNames;


    extern velaCamStructs::imgCutStruct getImgCutParam( std::string screenName );

    const std::map< std::string, velaCamStructs::imgCutStruct > getImgCutParamMap();
    const std::map< std::string, velaCamStructs::imgCutStruct > getImgCutParamMapDummies();
    void calcICSStartAnsStops( velaCamStructs::imgCutStruct & icsData );

    extern std::map< std::string, velaCamStructs::imgCutStruct > imgCutParamMap;

}



#endif
