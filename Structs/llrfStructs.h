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

class liberallrfInterface;

namespace llrfStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;
    struct liberallrfObject;
    struct pvStruct;
    // Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    // the LIB_ prefix is for libera LLRF controls, maybe in teh fuitre we get different ones?
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_PV_TYPE,
                                                     (LIB_LOCK)
                                                     (LIB_AMP_FF)
                                                     (LIB_AMP_SP)
                                                     (LIB_PHI_FF)
                                                     (LIB_PHI_SP)
                                                     (LIB_CAV_FWD)
                                                     (LIB_CAV_REV)
                                                     (LIB_KLY_FWD)
                                                     (LIB_KLY_REV)
                                                     (LIB_TIME_VECTOR)
                                                     (LIB_PULSE_LENGTH)

                                                     (LIB_PULSE_OFFSET)

                                                     (AMP_MVM)(PHI_DEG)(UNKNOWN)
                                                    )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_TYPE,(CLARA_HRRG)(CLARA_LRRG)(VELA_HRRG)(VELA_LRRG)
                                                  (L01)(UNKNOWN_TYPE))
    // monType is to switch in the staticCallbackFunction
    struct monitorStruct
    {
        monitorStruct():monType(UNKNOWN),interface(nullptr),EVID(nullptr),
                        llrfObj(nullptr){}
        LLRF_PV_TYPE    monType;
        liberallrfInterface*  interface;
        chtype          CHTYPE;
        evid            EVID;
        liberallrfObject*     llrfObj;
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

    //a custom struct will always stand up better under maintenance.
    struct rf_trace_data
    {
        rf_trace_data():
            shot(0),
            time(0)
            {}
        size_t              shot;
        std::vector<double> value;
        epicsTimeStamp etime; // epics timestamp for value
        double         time; // epics timestamp doncerted into nano-sec
        std::string    timeStr; // epics timestamp doncerted into nano-sec
    };
    // The main hardware object
    struct liberallrfObject
    {
        liberallrfObject():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),type(UNKNOWN_TYPE),
                     crestPhi(UTL::DUMMY_DOUBLE),numIlocks(UTL::DUMMY_INT),
                     phiCalibration(UTL::DUMMY_DOUBLE),ampCalibration(UTL::DUMMY_DOUBLE),
                     phi_DEG(UTL::DUMMY_DOUBLE),amp_MVM(UTL::DUMMY_DOUBLE),
                     powerTraceLength(0),islocked(false),maxAmp(UTL::DUMMY_DOUBLE),
                     pulse_length(UTL::DUMMY_DOUBLE),pulse_offset(UTL::DUMMY_DOUBLE)
                     //ampR(UTL::DUMMY_LONG),ampW(0),phiLLRF(UTL::DUMMY_LONG),maxAmp(UTL::DUMMY_LONG),
                     {}
        std::string name, pvRoot;
        double phiCalibration,ampCalibration,phi_DEG,amp_MVM;
        double phi_sp, phi_ff, crestPhi;
        double amp_sp, amp_ff,maxAmp;
        double pulse_length,pulse_offset;
        bool islocked;
        //long ampR,phiLLRF,ampW,crestPhi,maxAmp;
        int numIlocks;
        LLRF_TYPE type;
        size_t powerTraceLength;
        //a custom struct will always stand up better under maintenance.
        rf_trace_data cav_r_power,cav_f_power,kly_f_power,kly_r_power,time_vector;
        std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::iLockPVStruct> iLockPVStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvMonStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvComStructs;
    };
}
#endif
