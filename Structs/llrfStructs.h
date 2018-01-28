#ifndef _VELA_PIL_SHUTTER_STRUCTS_H_
#define _VELA_PIL_SHUTTER_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
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
                                                     (LIB_ILOCK_STATE)
                                                     (LIB_FF_AMP_LOCK_STATE)
                                                     (LIB_FF_PHASE_LOCK_STATE)
                                                     (LIB_RF_OUTPUT)
                                                     (LIB_AMP_FF)
                                                     (LIB_AMP_SP)
                                                     (LIB_PHI_FF)
                                                     (LIB_PHI_SP)
                                                     (LIB_PWR_REM)
                                                     (LIB_CH1_PWR_REM)
                                                     (LIB_CH2_PWR_REM)
                                                     (LIB_CH3_PWR_REM)
                                                     (LIB_CH4_PWR_REM)
                                                     (LIB_CH5_PWR_REM)
                                                     (LIB_CH6_PWR_REM)
                                                     (LIB_CH7_PWR_REM)
                                                     (LIB_CH8_PWR_REM)
                                                     (LIB_PHASE_REM)
                                                     (LIB_CH1_PHASE_REM)
                                                     (LIB_CH2_PHASE_REM)
                                                     (LIB_CH3_PHASE_REM)
                                                     (LIB_CH4_PHASE_REM)
                                                     (LIB_CH5_PHASE_REM)
                                                     (LIB_CH6_PHASE_REM)
                                                     (LIB_CH7_PHASE_REM)
                                                     (LIB_CH8_PHASE_REM)
                                                     (LIB_PWR_REM_EVID)
                                                     (LIB_CH1_PWR_REM_EVID)
                                                     (LIB_CH2_PWR_REM_EVID)
                                                     (LIB_CH3_PWR_REM_EVID)
                                                     (LIB_CH4_PWR_REM_EVID)
                                                     (LIB_CH5_PWR_REM_EVID)
                                                     (LIB_CH6_PWR_REM_EVID)
                                                     (LIB_CH7_PWR_REM_EVID)
                                                     (LIB_CH8_PWR_REM_EVID)
                                                     (LIB_PHASE_REM_EVID)
                                                     (LIB_CH1_PHASE_REM_EVID)
                                                     (LIB_CH2_PHASE_REM_EVID)
                                                     (LIB_CH3_PHASE_REM_EVID)
                                                     (LIB_CH4_PHASE_REM_EVID)
                                                     (LIB_CH5_PHASE_REM_EVID)
                                                     (LIB_CH6_PHASE_REM_EVID)
                                                     (LIB_CH7_PHASE_REM_EVID)
                                                     (LIB_CH8_PHASE_REM_EVID)
                                                     (LIB_PWR_REM_SCAN)
                                                     (LIB_CH1_PWR_REM_SCAN)
                                                     (LIB_CH2_PWR_REM_SCAN)
                                                     (LIB_CH3_PWR_REM_SCAN)
                                                     (LIB_CH4_PWR_REM_SCAN)
                                                     (LIB_CH5_PWR_REM_SCAN)
                                                     (LIB_CH6_PWR_REM_SCAN)
                                                     (LIB_CH7_PWR_REM_SCAN)
                                                     (LIB_CH8_PWR_REM_SCAN)
                                                     (LIB_PHASE_REM_SCAN)
                                                     (LIB_CH1_PHASE_REM_SCAN)
                                                     (LIB_CH2_PHASE_REM_SCAN)
                                                     (LIB_CH3_PHASE_REM_SCAN)
                                                     (LIB_CH4_PHASE_REM_SCAN)
                                                     (LIB_CH5_PHASE_REM_SCAN)
                                                     (LIB_CH6_PHASE_REM_SCAN)
                                                     (LIB_CH7_PHASE_REM_SCAN)
                                                     (LIB_CH8_PHASE_REM_SCAN)
                                                     (LIB_TIME_VECTOR)
                                                     (LIB_PULSE_LENGTH)
                                                     (LIB_PULSE_OFFSET)
                                                     (AMP_MVM)
                                                     (TRIG_SOURCE)
                                                     (PHI_DEG)
                                                     (UNKNOWN)
                                                   )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_TYPE,(CLARA_HRRG)(CLARA_LRRG)(VELA_HRRG)(VELA_LRRG)
                                                  (L01)(UNKNOWN_TYPE))


    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_SCAN,(PASSIVE)
                                                  (EVENT)
                                                  (IO_INTR)
                                                  (TEN)
                                                  (FIVE)(TWO)(ONE)(ZERO_POINT_FIVE)
                                                  (ZERO_POINT_TWO)
                                                  (ZERO_POINT_ONE)
                                                  (UNKNOWN_SCAN))


    DEFINE_ENUM_WITH_STRING_CONVERSIONS(TRIG,(OFF)
                                                  (EXTERNAL)
                                                  (INTERNAL)
                                                  (UNKNOWN_TRIG))

    // monType is to switch in the staticCallbackFunction
    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN),
            interface(nullptr),
            EVID(nullptr),
            llrfObj(nullptr)
            {}
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
        pvStruct():
            name(UTL::UNKNOWN_NAME)
            {}
        LLRF_PV_TYPE  pvType;
        chid          CHID;
        std::string   pvSuffix,name;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };

    struct setAmpHP_Struct
    {
        setAmpHP_Struct():
            can_kill(false),
            interface(nullptr),
            thread(nullptr),
            value(UTL::ZERO_DOUBLE)
            {}
        liberallrfInterface *interface;
        std::thread *thread;
        bool         can_kill;
        double       value;
    };

    //a custom struct will always stand up better under maintenance.
    struct rf_trace
    {
        rf_trace():
            time(UTL::ZERO_DOUBLE),
            mean_start_index(UTL::ZERO_SIZET),
            mean_stop_index(UTL::ZERO_SIZET),
            mean(UTL::DUMMY_DOUBLE),
            timeStr("unknown"),
            name("unknown"),
            EVID_timeStr("unknown"),
            EVID("unknown"),
            EVID_time(UTL::ZERO_DOUBLE),
            procStart(0),
            procEnd(0)
            {}
        size_t mean_start_index,mean_stop_index;
        bool   in_mask;
        std::vector<double> value;
        epicsTimeStamp etime;   // epics timestamp for value
        double         time;    // epics timestamp converted into nano-sec
        double         mean;    // epics timestamp converted into nano-sec
        std::string    timeStr; // epics timestamp converted into nano-sec
        std::string    EVID;    // LLRF EVID string
        std::string    name;    // LLRF EVID string
        double         EVID_time;    // LLRF EVID string
        std::string    EVID_timeStr;    // LLRF EVID string
        epicsTimeStamp EVID_etime;   // epics timestamp for value
        // when exposing a vector of rf_trace to python I ran into trouble...
        long long procStart,procEnd;
        //https://stackoverflow.com/questions/43107005/exposing-stdvectorstruct-with-boost-python
        bool operator==(const rf_trace& rhs)
        {
            return this == &rhs; //< implement your own rules.
        }
    };
    //a custom struct will always stand up better under maintenance.
    struct rf_trace_data
    {
        rf_trace_data():
            name("UNKNOWN"),
            scan(LLRF_SCAN::UNKNOWN_SCAN),
            shot(UTL::ZERO_SIZET),
            buffersize(UTL::TEN_SIZET),
            check_mask(false), //inside the mask or not
            hi_mask_set(false),
            low_mask_set(false),
            keep_rolling_average(false),
            has_average(false),
            keep_next_trace(true),
            trace_size(UTL::ZERO_SIZET),
            average_size(UTL::ONE_SIZET),
            // how many traces have been added to rolling sums
            rolling_sum_counter(UTL::ZERO_SIZET),
            latest_trace_index(UTL::ZERO_SIZET),
            current_trace(UTL::ZERO_SIZET),
            evid_current_trace(UTL::ZERO_SIZET),
            previous_evid_trace(UTL::MINUS_ONE_INT),
            sub_trace(UTL::ZERO_SIZET),
            mean_start_index(UTL::ZERO_SIZET),
            mean_stop_index(UTL::ZERO_SIZET),
            previous_trace(UTL::MINUS_ONE_INT),
            previous_previous_trace(UTL::MINUS_TWO_INT),
            //add_next_trace_to_outside_mask_trace(false),
            //outside_mask_trace_part(UTL::ZERO_SIZET),
            amp_drop_value(UTL::ZERO_DOUBLE),
            drop_amp_on_breakdown(false),
            num_continuous_outside_mask_count(UTL::ONE_SIZET),
            mask_floor(UTL::ZERO_DOUBLE),
            EVID("NONE"),//MAGIC_STRING
            add_next_trace(UTL::ZERO_SIZET),
            outside_mask_index(UTL::ZERO_SIZET),
            num_traces_at_this_power(UTL::ZERO_SIZET)
            {}
        size_t shot,num_continuous_outside_mask_count,outside_mask_index;
        bool    check_mask,hi_mask_set,low_mask_set,keep_rolling_average,has_average,keep_next_trace;
        size_t  buffersize, trace_size, average_size,rolling_sum_counter,mean_start_index,mean_stop_index;
        size_t num_traces_at_this_power;
        // Counter allowing you to access/update the correct part of  traces
        size_t  latest_trace_index,current_trace,evid_current_trace,add_next_trace;
        int previous_previous_trace,previous_trace,previous_evid_trace;
        // Counter allowing you to access the correct traces to delete off the rolling average
        size_t  sub_trace;
        // EVID are always monitored and updated to the traces[evid_current_trace]
        // when monitoring traces is enabled, new trace data is updated to traces[current_trace]
        // this means these two can go out of synch
        // traces has a length, buffersize
        std::string    EVID;
        std::string    name;
        std::vector<rf_trace> traces;
        // rolling sum is the sum of the traces to be averaged,
        // when a new trace arrives it adds it to rolling_sum and subtracts traces[sub_trace]
        // rolling_average is then calculated by dividing rolling_sum by average_size
        std::vector<double> high_mask, low_mask, rolling_average,rolling_sum,rolling_max,rolling_min,rolling_sd;
        //std::vector<double> last_good_trace;
        std::vector<std::vector<double>> average_trace_values;
        double mask_floor;// values must be ABOVE this to be checked as outside_mask_trace
        // whether to add the next trace to outside_mask_trace
        // and the position in outside_mask_trace to add to
        bool drop_amp_on_breakdown;
        //std::map<size_t,std::vector<bool>> add_next_trace_to_outside_mask_trace;
        std::vector<size_t> add_next_trace_to_outside_mask_trace;
        //size_t outside_mask_trace_part;
        double amp_drop_value;
        LLRF_SCAN scan;
    };

    struct outside_mask_trace
    {
        outside_mask_trace():
            trace_name(UTL::UNKNOWN_NAME),
            time(UTL::ZERO_SIZET),
            mask_floor(UTL::ZERO_DOUBLE),
            is_collecting(false),
            num_traces_to_collect(UTL::ZERO_SIZET),
            outside_mask_index(UTL::ZERO_SIZET)
            {}
        std::vector<rf_trace>  traces;
        std::vector<double>  time_vector;
        std::string trace_name,message;
        bool is_collecting;
        long long time;
        double mask_floor;
        size_t num_traces_to_collect,outside_mask_index;
        std::vector<double> high_mask,low_mask;
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
        liberallrfObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            type(UNKNOWN_TYPE),
            crestPhi(UTL::DUMMY_DOUBLE),
            numIlocks(UTL::DUMMY_INT),
            phiCalibration(UTL::DUMMY_DOUBLE),
            ampCalibration(UTL::DUMMY_DOUBLE),
            phi_DEG(UTL::DUMMY_DOUBLE),
            phi_sp(UTL::DUMMY_DOUBLE),
            phi_ff(UTL::DUMMY_DOUBLE),
            amp_MVM(UTL::DUMMY_DOUBLE),
            amp_sp(UTL::DUMMY_DOUBLE),
            amp_ff(UTL::DUMMY_DOUBLE),
            traceLength(UTL::ZERO_SIZET),
            rf_output(false),
            maxAmp(UTL::DUMMY_DOUBLE),
            pulse_length(UTL::DUMMY_DOUBLE),
            pulse_offset(UTL::DUMMY_DOUBLE),
            event_count(UTL::ZERO_SIZET),
            check_mask(false),
            ff_amp_lock_state(false),
            ff_ph_lock_state(false),
            interlock_state(false),
            num_outside_mask_traces(UTL::ZERO_SIZET),
            pulse_latency(UTL::ZERO_SIZET),
            timer_start(UTL::ZERO_SIZET),
            breakdown_rate(UTL::ZERO_DOUBLE),
            amp_drop_value(UTL::ZERO_DOUBLE),
            drop_amp_on_breakdown(false),
            activePulseCount(UTL::ZERO_SIZET),
            pulseCountOffset(UTL::ZERO_SIZET),
            previous_pulseCount(UTL::ZERO_SIZET),
            trig_source(TRIG::UNKNOWN_TRIG),
            num_extra_traces(0),
            kly_fwd_power_max(0.0),
            can_increase_active_pulses(false),
            active_pulse_kly_power_limit(1000.0)
                     {}
        double kly_fwd_power_max,active_pulse_kly_power_limit;
        bool can_increase_active_pulses;
        TRIG trig_source;
        std::vector<std::string> tracesToSaveOnBreakDown;
        std::string name, pvRoot, EVIDStr;
        double phiCalibration,ampCalibration,phi_DEG,amp_MVM;
        // WE SHOULD ONLY USE one of ff or sp (ff i think) but we have access to all of them)
        double phi_sp, phi_ff, crestPhi;
        double amp_sp, amp_ff,maxAmp;
        double pulse_length,pulse_offset;
        double breakdown_rate, amp_drop_value;
        bool rf_output, check_mask, ff_amp_lock_state,ff_ph_lock_state,interlock_state,drop_amp_on_breakdown;
        //long ampR,phiLLRF,ampW,crestPhi,maxAmp;
        int numIlocks;
        LLRF_TYPE type;
        size_t traceLength,event_count,num_outside_mask_traces,
           // the llrf doesn't fire immediatly after recieving a trigger
        // this is the (approx) number of elements of noise in an rf trace
        pulse_latency, activePulseCount, previous_pulseCount, pulseCountOffset, num_extra_traces;
        long long timer_start;
        //a map of 8 channels times 2 traces (power and phase) keys come from config and can't be changed
        // they name the channle source (i.e. KLYSTRON_FORWARD and the trac etype, PHASE or POWER )
        std::map<std::string, rf_trace_data> trace_data;
        std::vector<outside_mask_trace> outside_mask_traces;
        std::vector<std::string> outside_mask_trace_name;
        rf_trace time_vector;
        std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::iLockPVStruct> iLockPVStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvMonStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvComStructs;
    };
}
#endif
