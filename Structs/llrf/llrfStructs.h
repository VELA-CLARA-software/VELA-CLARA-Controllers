/*
//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
//
//  Author:      DJS
//  Last edit:   16-11-2018
//  FileName:    llrfStructs.h
//  Description:
//
//
//*/
#ifndef __VELA_CLARA_LLRFSTRUCTS_H__
#define __VELA_CLARA_LLRFSTRUCTS_H__
//
#include "structs.h"
#include "configDefinitions.h"
#include "running_stat.h"
//stl
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <tuple>
#include <utility>//std::pair
//epics
#include <cadef.h>
// boost python
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

class liberallrfInterface;

namespace llrfStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;
    struct liberallrfObject;
    struct scan;
    struct pvStruct;
    // Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    // the LIB_ prefix is for libera LLRF controls, maybe in teh fuitre we get different ones?
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_PV_TYPE,(LIB_FF_AMP_LOCK_STATE)
                                                     (LIB_FF_PHASE_LOCK_STATE)
                                                     (LIB_RF_OUTPUT)
                                                     (LIB_AMP_FF)
                                                     (LIB_AMP_SP)
                                                     (LIB_PHI_FF)
                                                     (LIB_PHI_SP)
                                                     (LIB_PWR_REM)
                                                     (LIB_PWR_REM_SCAN)
                                                     (LIB_PHASE_REM_SCAN)
                                                     (LIB_AMP_DER_SCAN)
                                                     (LIB_PHASE_DER_SCAN)
                                                     (LIB_PWR_LOCAL_SCAN)
                                                     (ALL_TRACES)
//                                                     (ALL_TRACES_EVID)
                                                     (ALL_TRACES_SCAN)
                                                     (ALL_TRACES_ACQM)
                                                     (LIB_TIME_VECTOR)
                                                     (LIB_PULSE_LENGTH)
                                                     (LIB_PULSE_OFFSET)
                                                     (AMP_MVM)
                                                     (TRIG_SOURCE)
                                                     //(TIMING_TRIGGER)
                                                     (PHI_DEG)
                                                     (INTERLOCK)
                                                     (PHASE_LOOP_LOCK)
                                                     (PULSE_SHAPE)
                                                     (PULSE_SHAPE_APPLY)
                                                     (UNKNOWN)
                                                  )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_TYPE,(CLARA_HRRG)
                                                  (CLARA_LRRG)
                                                  (VELA_HRRG)
                                                  (VELA_LRRG)
                                                  (L01)
                                                  (UNKNOWN_TYPE))
    /*
        the order of these enums matters! PASSIVE is SCAN = 0 etc.
    */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_SCAN,(PASSIVE)   // 0
                                                  (EVENT)     // 1
                                                  (IO_INTR)   // 2
                                                  (TEN)       // 3
                                                  (FIVE)      // 4
                                                  (TWO)       // 5
                                                  (ONE)       // 6
                                                  (ZERO_POINT_FIVE) // 7
                                                  (ZERO_POINT_TWO)  // 8
                                                  (ZERO_POINT_ONE)  // 9
                                                  (ZERO_POINT_ZERO_FIVE)   // 10
                                                  (UNKNOWN_SCAN))

    /*
        the order of these enums matters! NOW is  1 etc.
    */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(LLRF_ACQM,(UNKNOWN_ACQM)
                                                  (ACQM_NOW)    // 1
                                                  (ACQM_EVENT)) // 2


    DEFINE_ENUM_WITH_STRING_CONVERSIONS(INTERLOCK_STATE,(NON_ACTIVE)
                                                        (ACTIVE)
                                                        (UNKNOWN_INTERLOCK_STATE))


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
        liberallrfInterface* interface;
        std::thread* thread;
        bool         can_kill;
        double       value;
    };

    struct rf_trace
    {
        rf_trace():
            time(UTL::ZERO_DOUBLE),
            //mean_start_index(UTL::ZERO_SIZET),
            //mean_stop_index(UTL::ZERO_SIZET),
            mean(UTL::DUMMY_DOUBLE),
            timeStr(UTL::UNKNOWN_STRING),
            name(UTL::UNKNOWN_STRING),
            EVID_timeStr(UTL::UNKNOWN_STRING),
            EVID(UTL::UNKNOWN_STRING),
            EVID_time(UTL::ZERO_DOUBLE),
            procStart(0),
            procEnd(0)
            {}
        std::string    name;    // LLRF EVID string
        //size_t mean_start_index,mean_stop_index;
        bool   in_mask;
        std::vector<double> value;
        epicsTimeStamp etime;   // epics timestamp for value
        double         time;    // epics timestamp converted into nano-sec
        double         mean;    // epics timestamp converted into nano-sec
        std::string    timeStr; // epics timestamp converted into nano-sec
        std::string    EVID;    // LLRF EVID string
        double         EVID_time;    // LLRF EVID string
        std::string    EVID_timeStr; // LLRF EVID string
        epicsTimeStamp EVID_etime;   // epics timestamp for value




        // when exposing a vector of rf_trace to python I ran into trouble...
        long long procStart, procEnd;
        //https://stackoverflow.com/questions/43107005/exposing-stdvectorstruct-with-boost-python
        bool operator==(const rf_trace& rhs)
        {
            return this == &rhs; //< implement your own rules.
        }
    };

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(TRACE_TYPE,(POWER)
                                                   (PHASE)
                                                   (DUMMY_TRACE_TYPE))
    /*
        Holds data that tells us how to chunk the one trace data
    */
    struct one_trace_info
    {
        one_trace_info():
            name(UTL::UNKNOWN_NAME),
            position(UTL::ZERO_SIZET),
            type(TRACE_TYPE::DUMMY_TRACE_TYPE)
            {}
        std::string name;
        size_t position;
        TRACE_TYPE type;
        size_t start_index;
        size_t stop_index;
    };
    /*
        All the info associated with a ONE record for all traces
    */
    struct llrf_trace_one_shot
    {
        llrf_trace_one_shot():

            trace_time(UTL::ZERO_DOUBLE),
            trace_timeStr(UTL::UNKNOWN_STRING),

//            EVID_time(UTL::ZERO_DOUBLE),
//            EVID_timeStr(UTL::UNKNOWN_STRING),
//            EVID_Str(UTL::UNKNOWN_STRING),
//            EVID(UTL::DUMMY_INT),

            scan(LLRF_SCAN::UNKNOWN_SCAN),
            num_traces(UTL::ZERO_SIZET),
            num_elements_per_trace(UTL::DUMMY_SIZET),
            num_elements_per_trace_used(UTL::DUMMY_SIZET),
            num_start_zeroes_per_trace(UTL::DUMMY_SIZET),
            buffer_size(UTL::BUFFER_500),
            data_buffer(buffer_size),
            acqm(LLRF_ACQM::UNKNOWN_ACQM)
            {}
        size_t buffer_size;
        /*
            all the trace data goes here (online data)
        */
        boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> data_buffer;

        epicsTimeStamp trace_time_0; // time first trace was acquired (ns since epoch)
        epicsTimeStamp trace_etime;   // epics timestamp for value

        double         trace_time;    // LLRF EVID string
        std::string    trace_timeStr; // LLRF EVID string
        /*
            order of traces in data
            and the indices in data for each trace
            (all offline data)
        */
        //std::vector<epicsTimeStamp> trace_order;
        /*
            number of elemnts in a trace and how many starting zeroes to ignore
        */
        size_t num_traces;
        size_t num_elements_per_trace;
        size_t num_start_zeroes_per_trace;
        size_t num_elements_per_trace_used;
        /*
            which indices are for which trace
        */
        std::vector<std::pair<size_t,size_t>> trace_indices;
        /*
            online data for Event ID (EVID) and time stamps
        */
//        double         EVID_time;    // LLRF EVID string
//        std::string    EVID_timeStr; // LLRF EVID string
//        epicsTimeStamp EVID_etime;   // epics timestamp for value
//        int EVID;                    // LLRF EVID
//        boost::circular_buffer<std::pair<int,std::string>>  EVID_buffer;
//        std::string EVID_Str;        // LLRF EVID string (comes from EPICS this way)
        /*
            online data, SCAN & ACQM
        */
        LLRF_SCAN  scan;
        LLRF_ACQM  acqm;
        /*
            how to split up the record into individual records
        */
        std::map<std::string, one_trace_info> trace_info;
    };


    struct rf_trace_data
    {
        rf_trace_data():
            name(UTL::UNKNOWN_NAME),
            check_mask(false), //inside the mask or not


            keep_rolling_average(false),
            has_average(false),
            keep_next_trace(true),
            trace_size(UTL::ZERO_SIZET),
            rolling_average_size(UTL::ONE_SIZET),
            rolling_average_count(UTL::ZERO_SIZET),

            // how many traces have been added to rolling sums
            // rolling_sum_counter(UTL::ZERO_SIZET),
            latest_trace_index(UTL::ZERO_SIZET),
            current_trace(UTL::ZERO_SIZET),

            mean_start_index(UTL::ZERO_SIZET),
            mean_stop_index(UTL::ZERO_SIZET),


            // NOT USED???
            //outside_mask_index(UTL::ZERO_SIZET),


            num_continuous_outside_mask_count(UTL::ONE_SIZET),

            outside_mask_trace_message(UTL::UNKNOWN_STRING),

            latest_max(UTL::ZERO_DOUBLE),

            buffersize(UTL::FIVE_SIZET),

            trace_max(UTL::DUMMY_DOUBLE),

            // mask
            hi_mask_set(false),
            lo_mask_set(false),
            mask_value(UTL::ZERO_DOUBLE),
            mask_abs_min(UTL::ZERO_DOUBLE),
            mask_floor(UTL::ZERO_DOUBLE),
            mask_start(UTL::ZERO_SIZET),
            mask_end(UTL::ZERO_SIZET),
            mask_window_start(UTL::ZERO_SIZET),
            mask_window_end(UTL::ZERO_SIZET),

            use_percent_mask(false),
            use_abs_mask(false),

            drop_amp_on_breakdown(false),
            amp_drop_value(UTL::DUMMY_DOUBLE),
            phase_tolerance(UTL::DUMMY_DOUBLE),
            endInfiniteMask_Trace_Set(false)
            {}


        std::string name;
        std::string outside_mask_trace_message;

        /*
            MASK STUFF
        */
        bool hi_mask_set,lo_mask_set;
        double mask_floor;// values must be ABOVE this to be checked as outside_mask_trace
        size_t mask_start, mask_end, mask_window_start, mask_window_end;
        std::vector<double> hi_mask, lo_mask;

        size_t num_continuous_outside_mask_count,outside_mask_index;
        bool   check_mask, use_percent_mask, use_abs_mask;
        double mask_value, mask_abs_min;
        size_t mask_end_by_power_index;
        double phase_tolerance;


        bool endInfiniteMask_Trace_Set;

        bool keep_next_trace;

        size_t buffersize, trace_size;



        ///?????????????????  OLD OLD
        size_t num_traces_at_this_power;
        // Counter allowing you to access/update the correct part of  traces
        size_t  latest_trace_index, current_trace, evid_current_trace;//,add_next_trace;
        //int previous_previous_trace, previous_trace, previous_evid_trace;
        std::pair<std::string, double> endMaskTrace_Bound; // ????????
        //std::vector<rf_trace> traces;// ????????
        ///?????????????????????????   OLD OLD
/*       __   __                    __                ___  __        __   ___  __
        |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
        |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

        // rolling sum is the sum of the traces to be averaged,
        // when a new trace arrives it adds it to rolling_sum and subtracts traces[sub_trace]
        // rolling_average is then calculated by dividing rolling_sum by average_size
*/
        bool keep_rolling_average,has_average;
        size_t rolling_average_size, rolling_average_count;
        std::vector<double> rolling_average, rolling_sum, rolling_max, rolling_min, rolling_sd;
        std::vector<std::vector<double>> average_trace_values;


        /*
            all the trace data goes here (online data)
        */
        boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> data_buffer;
        double trace_max;
        std::vector<std::pair<size_t,size_t>> outside_mask_data_index_to_add_future_traces_to;
        /* trace cut mean */
        size_t mean_start_index, mean_stop_index;//rolling_sum_counter
        double mean;


        //size_t outside_mask_trace_part;

        bool drop_amp_on_breakdown;
        double amp_drop_value,latest_max;
    };



    struct outside_mask_event_trace_data
    {
        outside_mask_event_trace_data():
            name(UTL::UNKNOWN_NAME)
            {}
        std::string name;
        std::vector<double> hi_mask, lo_mask;
        std::vector< std::pair<std::string, std::vector<double> > > data_buffer;

    };

    struct outside_mask_event_data
    {
        outside_mask_event_data():
            is_collecting(false),
            can_get_data(false),
            num_still_to_collect(UTL::ZERO_SIZET),
            trace_that_caused_OME(UTL::UNKNOWN_STRING),
            extra_traces_on_outside_mask_event(UTL::ZERO_SIZET),
            num_collected(UTL::ZERO_SIZET),
            outside_mask_index(UTL::ZERO_SIZET),
            mask_floor(UTL::DUMMY_DOUBLE),
            num_events(UTL::ZERO_SIZET)
            {}

        bool is_collecting, can_get_data;
        double mask_floor;
        size_t num_still_to_collect, num_collected;
        std::string trace_that_caused_OME;
        std::string outside_mask_trace_message;
        size_t extra_traces_on_outside_mask_event;
        size_t outside_mask_index;
        std::vector< std::string> traces_to_save_on_outside_mask_event;
        std::vector<double> hi_mask, lo_mask;
        size_t num_events;
        std::vector< outside_mask_event_trace_data > trace_data;
    };

    // scan struct for each traces SCAN aparamters
    struct scan
    {
        scan():
            name(UTL::UNKNOWN_NAME),
            pvroot(UTL::UNKNOWN_PVROOT)
            {}
        std::string name;
        std::string pvroot;
        /* actual value for the scan types for each scn aobject */
        std::map<LLRF_PV_TYPE,LLRF_SCAN> value;
        std::map<LLRF_PV_TYPE,pvStruct> pvSCANStructs;
    };


    // The main hardware object
    struct liberallrfObject
    {
        liberallrfObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            pvRoot_oneTrace(UTL::UNKNOWN_PVROOT),
            //pvRoot_SCAN(UTL::UNKNOWN_PVROOT),

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
            phase_loop_lock(false),
            traceLength(UTL::ZERO_SIZET),
            rf_output(false),
            maxAmp(UTL::DUMMY_DOUBLE),
            pulse_length(UTL::DUMMY_DOUBLE),
            pulse_offset(UTL::DUMMY_DOUBLE),
            event_count(UTL::ZERO_SIZET),
            check_mask(false),
            ff_amp_lock_state(false),
            ff_ph_lock_state(false),
            num_outside_mask_traces(UTL::ZERO_SIZET),
            pulse_latency(UTL::ZERO_SIZET),
            timer_start(UTL::ZERO_SIZET),
            breakdown_rate(UTL::ZERO_DOUBLE),
            amp_drop_value(UTL::ZERO_DOUBLE),
            drop_amp_on_breakdown(false),

            // how many active pulses have there been
            active_pulse_count(UTL::ZERO_SIZET),
            duplicate_pulse_count(UTL::ZERO_SIZET),
            // how many pulses, in total have there
            pulse_count(UTL::ZERO_SIZET),

            pulse_shape_up_to_date(false),

            pulseCountOffset(UTL::ZERO_SIZET),
            previous_pulseCount(UTL::ZERO_SIZET),
            trig_source(TRIG::UNKNOWN_TRIG),
            num_extra_traces(UTL::ZERO_SIZET),
            kly_fwd_power_max(UTL::ZERO_DOUBLE),
            last_kly_fwd_power_max(UTL::DUMMY_DOUBLE), // init to something different than kly_fwd_power_max
            can_increase_active_pulses(false),
            //collecting_outside_mask_data(false),
            active_pulse_kly_power_limit(UTL::ZERO_DOUBLE),
            interlock_state(INTERLOCK_STATE::UNKNOWN_INTERLOCK_STATE),
            omed_count(UTL::ZERO_SIZET),
            new_outside_mask_event(false),
            keep_kly_fwd_pow_running_stat(false),
            interface(nullptr),
            mean_trace_freq_timer_count(UTL::ZERO_SIZET),
            mean_trace_freq_timer_max(UTL::TEN_SIZET),
            shot_to_shot_freq(UTL::DUMMY_DOUBLE),
            num_traces_to_estimate_rep_rate(UTL::HUNDRED_SIZET),
            trace_rep_rate(UTL::DUMMY_DOUBLE)
            {}
        liberallrfInterface* interface;
        double kly_fwd_power_max,last_kly_fwd_power_max,active_pulse_kly_power_limit;

        bool phase_loop_lock;
        bool pulse_shape_up_to_date;
        std::vector<double> pulse_shape;
        std::vector<double> last_kly_fwd_pwr_trace;


        /* This list can be used when getting all_traces_data_buffer to cut the data  */
        std::vector<std::string> traces_to_get_when_dumping_all_traces_data_buffer;


        short timing_trigger; // is the external timing on? needs updating to a proper enum at some point

        // some flags
        bool can_increase_active_pulses;
        bool new_outside_mask_event;
        bool keep_kly_fwd_pow_running_stat;

        TRIG trig_source;
        INTERLOCK_STATE interlock_state;

    ///---------------------------------------------------------------------------------------
        outside_mask_event_data omed;

        size_t omed_count;


        size_t active_pulse_count;
        size_t duplicate_pulse_count;
        size_t pulse_count;


        std::chrono::high_resolution_clock::time_point shot_to_shot_trace_freq_timer;
        std::chrono::high_resolution_clock::time_point mean_trace_freq_timer;
        size_t mean_trace_freq_timer_count, mean_trace_freq_timer_max;
        double mean_freq, shot_to_shot_freq;

        //bool collecting_outside_mask_data;
        //std::vector<std::string> outside_mask_trace_name;//????????????????

        std::string name, pvRoot, EVIDStr, pvRoot_oneTrace;//, pvRoot_SCAN;


        double phiCalibration,ampCalibration,phi_DEG,amp_MVM;
        // WE SHOULD ONLY USE one of ff or sp (ff i think) but we have access to all of them)
        double phi_sp, phi_ff, crestPhi;
        double amp_sp, amp_ff,maxAmp;
        double pulse_length,pulse_offset;
        double breakdown_rate, amp_drop_value;

        bool rf_output, check_mask, ff_amp_lock_state,ff_ph_lock_state; //,interlock_state;

        //long ampR,phiLLRF,ampW,crestPhi,maxAmp;
        int numIlocks;
        LLRF_TYPE type;
        size_t traceLength,event_count,num_outside_mask_traces,
           // the llrf doesn't fire immediatly after recieving a trigger
        // this is the (approx) number of elements of noise in an rf trace
        pulse_latency, previous_pulseCount, pulseCountOffset;
        long long timer_start;
        //a map of 8 channels times 2 traces (power and phase) keys come from config and can't be changed
        // they name the channle source (i.e. KLYSTRON_FORWARD and the trac etype, PHASE or POWER)
        //rf_trace time_vector;
        std::vector<double> time_vector;
    #ifdef BUILD_DLL
        boost::python::list getTimeVector();
        void setTimeVector();
    #endif


        std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::iLockPVStruct> iLockPVStructs;


        size_t num_traces_to_estimate_rep_rate;
        std::vector<std::chrono::high_resolution_clock::time_point> trace_times_for_rep_rate;
        double trace_rep_rate;

        /*
            this is the new all traces in one record object
        */
        llrf_trace_one_shot all_traces;

        /*
            Klystron Forward Power Running Stats
            These are the running mean adn sigma for the KlyFwdPow
            Note, they depend on th eposition of
            We keep the amp_set vs KLY_FWD_POWER running stat variables
            m_n, m_oldM, m_oldS, see running_stat.h
        */
        int this_pulse_amp_set;
        // these are the running stats
        std::map<int, runningStat> amp_set_kly_fwd_rs;
        // this is the state of each running state, so we can save and re-apply settings
        std::map<int, std::tuple<size_t, double, double>> amp_set_kly_fwd_rs_state;


        /*
            Individual traces go here
        */
        std::map<std::string, rf_trace_data> trace_data;


        // OLD get riud of this at some point

        /*
            This is so we can change the SCAN rate for each individual trace
            ACQM not currently used)
        */
        std::map<std::string, scan> trace_scans;
        std::map<std::string, LLRF_ACQM> trace_acqm_map;

        std::map<LLRF_PV_TYPE,pvStruct> pvMonStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvComStructs;

        std::map<LLRF_PV_TYPE,pvStruct> pvOneTraceMonStructs;
        std::map<LLRF_PV_TYPE,pvStruct> pvOneTraceComStructs;


        std::vector<std::string> tracesToSaveOnBreakDown;

        bool drop_amp_on_breakdown;
        size_t num_extra_traces;
    };
}
#endif
