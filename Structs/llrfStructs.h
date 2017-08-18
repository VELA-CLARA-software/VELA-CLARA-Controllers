#ifndef _VELA_PIL_SHUTTER_STRUCTS_H_
#define _VELA_PIL_SHUTTER_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
#include <deque>
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
                                                     (LIB_CH1_PWR_REM)
                                                     (LIB_CH2_PWR_REM)
                                                     (LIB_CH3_PWR_REM)
                                                     (LIB_CH4_PWR_REM)
                                                     (LIB_CH5_PWR_REM)
                                                     (LIB_CH6_PWR_REM)
                                                     (LIB_CH7_PWR_REM)
                                                     (LIB_CH8_PWR_REM)
                                                     (LIB_CH1_PHASE_REM)
                                                     (LIB_CH2_PHASE_REM)
                                                     (LIB_CH3_PHASE_REM)
                                                     (LIB_CH4_PHASE_REM)
                                                     (LIB_CH5_PHASE_REM)
                                                     (LIB_CH6_PHASE_REM)
                                                     (LIB_CH7_PHASE_REM)
                                                     (LIB_CH8_PHASE_REM)
                                                     (LIB_TIME_VECTOR)
                                                     (LIB_PULSE_LENGTH)
                                                     (LIB_PULSE_OFFSET)
                                                     (AMP_MVM)
                                                     (PHI_DEG)
                                                     (UNKNOWN)
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
        std::string     name;
    };
    // The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    // The rest are paramaters passed to EPICS, ca_create_channel, ca_create_subscription etc..
    struct pvStruct
    {
        pvStruct():name("NO_NAME"){}
        LLRF_PV_TYPE  pvType;
        chid          CHID;
        std::string   pvSuffix,name;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };

    //a custom struct will always stand up better under maintenance.
    struct rf_trace
    {
        rf_trace():
            shot(0),
            time(0),
            timeStr("")
            {}
        size_t              shot;
        std::vector<double> value;
        epicsTimeStamp etime;   // epics timestamp for value
        double         time;    // epics timestamp converted into nano-sec
        std::string    timeStr; // epics timestamp converted into nano-sec
    };
    //a custom struct will always stand up better under maintenance.
    struct rf_trace_data
    {
        rf_trace_data():
            buffersize(2),
            check_mask(false), //inside the mask or not
            hi_mask_set(false),
            low_mask_set(false),
            keep_rolling_average(false),
            trace_size(0),
            average_size(1)
            {}
        bool                check_mask, hi_mask_set,low_mask_set,keep_rolling_average;
        size_t              buffersize, trace_size, average_size;
        std::deque<rf_trace> traces;
        std::vector<double> high_mask, low_mask, rolling_average;
    };

    struct outside_mask_trace
    {
        size_t              shot;
        std::string         trace_name;
        std::vector<double> value,high_mask,low_mask;
        epicsTimeStamp etime;   // epics timestamp for value
        double         time;    // epics timestamp converted into nano-sec
        std::string    timeStr; // epics timestamp converted into nano-sec
        // when exposing a vector of outside_mask_trace to python I ran into trouble...
        //https://stackoverflow.com/questions/43107005/exposing-stdvectorstruct-with-boost-python
        bool operator==(const outside_mask_trace& rhs)
        {
            return this == &rhs; //< implement your own rules.
        }
    };

    // The main hardware object
    struct liberallrfObject
    {
        liberallrfObject():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),type(UNKNOWN_TYPE),
                     crestPhi(UTL::DUMMY_DOUBLE),numIlocks(UTL::DUMMY_INT),
                     phiCalibration(UTL::DUMMY_DOUBLE),ampCalibration(UTL::DUMMY_DOUBLE),
                     phi_DEG(UTL::DUMMY_DOUBLE),amp_MVM(UTL::DUMMY_DOUBLE),
                     powerTraceLength(0),islocked(false),maxAmp(UTL::DUMMY_DOUBLE),
                     pulse_length(UTL::DUMMY_DOUBLE),pulse_offset(UTL::DUMMY_DOUBLE),
                     event_count(0),check_mask(false)
                     {}
        std::string name, pvRoot,EVIDStr;
        double phiCalibration,ampCalibration,phi_DEG,amp_MVM;
        double phi_sp, phi_ff, crestPhi;
        double amp_sp, amp_ff,maxAmp;
        double pulse_length,pulse_offset;
        bool islocked, check_mask;
        //long ampR,phiLLRF,ampW,crestPhi,maxAmp;
        int numIlocks;
        LLRF_TYPE type;
        size_t powerTraceLength,event_count;
        //a map of 8 channels times 2 traces (power and phase) keys come from config and can't be changed
        std::map<std::string, rf_trace_data> trace_data;
        std::vector<outside_mask_trace> outside_mask_traces;
        rf_trace_data time_vector;
        std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::iLockPVStruct> iLockPVStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvMonStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvComStructs;
    };
}
#endif
