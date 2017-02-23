#ifndef _VELA_PIL_SHUTTER_STRUCTS_H_
#define _VELA_PIL_SHUTTER_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
//epics
#include <cadef.h>

class llrfInterface;

namespace llrfStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;
    struct llrfObject;
    struct pvStruct;
    // Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_PV_TYPE,(AMP_R)(AMP_W)(PHI)(AMP_MVM)(PHI_DEG)(UNKNOWN))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_TYPE,(CLARA_HRRG)(CLARA_LRRG)(VELA_HRRG)(VELA_LRRG)
                                                  (L01)(UNKNOWN_TYPE))
    // monType is to switch in the staticCallbackFunction
    struct monitorStruct
    {
        monitorStruct():monType(UNKNOWN),interface(nullptr),EVID(nullptr),
                        llrfObj(nullptr){}
        LLRF_PV_TYPE    monType;
        llrfInterface*  interface;
        chtype          CHTYPE;
        evid            EVID;
        llrfObject*     llrfObj;
    };
    // The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    // The rest are paramaters passed to EPICS, ca_create_channel, ca_create_subscription etc..
    struct pvStruct
    {
        LLRF_PV_TYPE  pvType;
        chid          CHID;
        std::string   pvSuffix;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };
    // The main hardware object
    struct llrfObject
    {
        llrfObject():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),type(UNKNOWN_TYPE),
                       ampR(UTL::DUMMY_LONG),ampW(0),phiLLRF(UTL::DUMMY_LONG),
                       maxAmp(UTL::DUMMY_LONG),crestPhi(UTL::DUMMY_LONG),numIlocks(UTL::DUMMY_INT),
                       phiCalibration(UTL::DUMMY_DOUBLE),ampCalibration(UTL::DUMMY_DOUBLE),
                       phi_DEG(UTL::DUMMY_DOUBLE),amp_MVM(UTL::DUMMY_DOUBLE){}
        std::string name, pvRoot;
        double phiCalibration,ampCalibration,phi_DEG,amp_MVM;
        long ampR,phiLLRF,ampW,crestPhi,maxAmp;
        int numIlocks;
        LLRF_TYPE type;
        std::map<LLRF_PV_TYPE,pvStruct> pvMonStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvComStructs;
        std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::iLockPVStruct> iLockPVStructs;
    };
}
#endif
