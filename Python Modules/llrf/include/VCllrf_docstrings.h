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
//  Last edit:   16-10-2018
//  FileName:    VCllrf_docstrings.h
//  Description:
//
//
//
//*/
#ifndef _VCLLRF_DOCSTRING_H_
#define _VCLLRF_DOCSTRING_H_

const char* setIndividualTraceBufferSize_1_ds = "Set the number of traces to buffer for trace 'name' to 'value'";
const char* setIndividualTraceBufferSize_2_ds = "Set the number of traces to buffer for each individual traces to 'value'";
const char* setAllTraceBufferSize_ds          = "Set the number of traces to buffer for the all-traces record to 'value'";



//--------------------------------------------------------------------------------------------------
/*       __   __                    __                ___  __        __   ___  __
        |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
        |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/
*/
const char* clearAllRollingAverage_ds   = "Clear All Rolling Average Data for each trace.";
const char* clearTraceRollingAverage_ds = "Clear Rolling Average Data for trace 'name'.";
const char* setShouldKeepRollingAverage_1_ds = "Set keep rolling average to TRUE for trace 'name'.";
const char* setShouldKeepRollingAverage_2_ds = "Set keep rolling average to TRUE for all traces.";
const char* setShouldNotKeepRollingAverage_1_ds = "Set keep rolling average to FALSE for trace 'name'.";
const char* setShouldNotKeepRollingAverage_2_ds = "Set keep rolling average to FALSE for all traces.";
const char* setKeepRollingAverage_ds = "Set Keep Rolling Average for all Traces.";
const char* setTraceRollingAverageSize_ds = "Set the number of traces to average for the Rolling Average for trace 'name' to 'value'.";
const char* setAllRollingAverageSize_ds  = "Set the number of traces to average for the Rolling Average of every trace to 'value'.";
const char* getTraceRollingAverage_Py_ds = "Get the latest Rolling Average for trace 'name'.";
const char* getTraceRollingAverageSize_ds= "Return the number of traces to average for the Rolling Average of trace 'name'.";
const char* getTraceRollingAverageCount_ds= "Return the number of traces that have been added to the Rolling Average for tarce 'name'.";
const char* isKeepingRollingAverage_ds   = "Returns true if trace 'name' is keeping a Rolling Average.";
const char* hasRollingAverage_ds = "Returns true if trace 'name' has a rolling average trace.";
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/

*/


//--------------------------------------------------------------------------------------------------
const char* setUsePercentMask_ds = "Automatic Masks for Trace 'name' will use percentages of Rolling Average.";
const char* setUseAbsoluteMask_ds = "Automatic Masks for Trace 'name' will use absolute values.";
const char* setMaskValue_ds = "Automatic Masks will use 'value' (either percentage or an absolute value) applied to the trace 'name' Rolling Average.";
const char* setMaskStartIndex_ds = "Start index for trace 'name' will be set to 'value'.";
const char* setMaskEndIndex_ds = "End index for trace 'name' will be set to 'value'.";
const char* setMaskFloor_ds       = "Mask floor for trace 'name' will be set to 'value'.";
const char* setMaskAbsMinValue_ds = "mask_abs_min for trace 'name' will be set to 'value'.";
const char* setMaskWindowIndices_ds = "Mask Window indices for trace 'name' will be set to 'start_value' and 'end_value'.";
const char* setMaskStartTime_ds = "Start time for trace 'name' will be set to 'value' (the time will be converted to the nearest index).";
const char* setMaskEndTime_ds = "Start time for trace 'name' will be set to 'value' (the time will be converted to the nearest index).";
const char* setMaskWindowTimes_ds = "Mask Window timea for trace 'name' will be set to 'start_value' and 'end_value' (times will be converted to the nearest index).";
const char* setShouldCheckMask_ds = "Enable checking masks for trace 'name'.";
const char* setShouldNotCheckMask_ds = "Disable checking masks for trace 'name'.";
const char* setCheckMask_ds= "Set checking masks for trace 'name' to 'value' (true/false).";
const char* clearMask_ds = "clear the hi and lo msks for trace 'name' and set hi/lo_mask_set to false";
const char* setHiMask_ds = "Set trace 'name' hi_mask to 'value'.";
const char* setLoMask_ds = "Set trace 'name' lo_mask to 'value'.";
const char* setMaskParamatersIndices_ds = "Set trace 'name' mask parameters using trace indices,"
                                          "'isPercent', true, use percentage masks, false defaults to absolute masks, "
                                          "calls setMaskValue('name', mask_value'), "
                                          "calls setMaskFloor('name','mask_floor'), "
                                          "calls setMaskAbsMinValue('name','mask_abs_min'), "
                                          "calls setMaskStartIndex('name','start'), "
                                          "calls setMaskEndIndex('name','end'), "
                                          "calls setMaskWindowIndices('name','window_start', 'window_end')";
const char* setMaskParamatersTimes_ds = "Set trace 'name' mask parameters using trace times,"
                                        "'isPercent', true, use percentage masks, false defaults to absolute masks, "
                                        "calls setMaskValue('name', mask_value'), "
                                        "calls setMaskFloor('name','mask_floor'), "
                                        "calls setMaskAbsMinValue('name','mask_abs_min'), "
                                        "calls setMaskStartTime('name','start'), "
                                        "calls setMaskEndTime('name','end'), "
                                        "calls setMaskWindowTimes('name','window_start', 'window_end')";
const char* setGlobalCheckMask_ds          = "Set Global Check Mask Flag to 'value' (true or false)";
const char* setGlobalShouldCheckMask_ds    = "Set Global Check Mask Flag to True";
const char* setGlobalShouldNotCheckMask_ds = "Set Global check mask flag to False";

const char* setNumContinuousOutsideMaskCount_1_ds = "Set the number of continuous points outside the mask required to trigger an "
                                                    "'outiside mask event' for trace 'name'  to 'value' ";
const char* setNumContinuousOutsideMaskCount_2_ds = "Set the number of continuous points outside the mask required to trigger an "
                                                    "'outiside mask event' for all traces to 'value' ";

const char* getHiMask_ds = "Get trace 'name' hi_mask.";
const char* getLoMask_ds = "Get trace 'name' lo_mask.";



const char* getMaskValue_ds = "Get trace 'name' mask_value";
const char* getMaskStartIndex_ds = "Get trace 'name' mask_start as an index";
const char* getMaskEndIndex_ds = "Get trace 'name' mask_end as an index";
const char* getMaskFloor_ds = "Get trace 'name' mask_value as an index";
const char* getMaskWindowStartIndex_ds = "Get trace 'name' mask_window_start as an index";
const char* getMaskWindowEndIndex_ds = "Get trace 'name' mask_window_end as an index";
const char* getMaskStartTime_ds = "Get trace 'name' mask_start as a time (us)";
const char* getMaskEndTime_ds = "Get trace 'name' mask_end as a time (us)";
const char* getMaskWindowStartTime_ds = "Get trace 'name' mask_window_start as a time (us)";
const char* getMaskWindowEndTime_ds = "Get trace 'name' mask_window_end as a time (us)";


const char* setTracesToSaveOnOutsideMaskEvent_Py_ds = "Set the traces to save when an outside_mask_event occurs to list 'names'.";
const char* setTracesToSaveOnOutsideMaskEvent_2_ds  = "Set the traces to save when an outside_mask_event occurs to 'name'.";
const char* getTracesToSaveOnOutsideMaskEvent_Py_ds = "Get the names of traces to save when an outside_mask_event occurs";


const char* setNumExtraTracesOnOutsideMaskEvent_ds = "Set the number of extra traces to save after an outside_mask_event to 'value'.";
const char* getNumExtraTracesOnOutsideMaskEvent_ds = "Get the number of extra traces to save after an outside_mask_event.";

const char* setDropAmpOnOutsideMaskEvent_ds = "On outside_mask_event for trace 'name' the amplitude will be dropped to 'value', if enabled by 'bool_arg'.";
const char* setDropAmpOnOutsideMaskEventValue_ds = "Sets if to drop the amplitude  on outside_mask_event to 'bool_arg' for trace 'name'.";


const char* getOutsideMaskEventDataSize_ds = "Return the number of elements in outside_mask_traces";
const char* getOutsideMaskEventData_ds = "Get the outside_mask_traces data as python dictionary.";
const char* getOutsideMaskEventDataPart_ds =  "Return data at index 'part' from outside_mask_traces as python dictionary.";
const char* isOutsideMaskEventDataCollecting_ds = "Return true if out_side_mask_traces is collecting future traces.";
const char* canGetOutsideMaskEventData_ds = "Return omed.can_get_data (if true new omed is ready to collect).";
const char* getOutsideMaskEventCount_ds = "Return latest count of outside_mask_event";



const char* setActivePulsePowerLimit_ds = "Set active_pulse_kly_power_limit, the minimum klystron forward power to enable active_pulse_count "
                                          "to increment (below this value active_pulse_count won't increment.).";
const char* getActivePulsePowerLimit_ds = "Get active_pulse_kly_power_limit, the minimum klystron forward power required to allow active_pulse_count to increment.";
const char* getActivePulseCount_ds      = "Get active_pulse_count: the number of active pulses.";
const char* addActivePulseCountOffset_ds= "Add 'value' (positive integer) to the active_pulse_count.";
const char* setActivePulseCount_ds      = "Set active_pulse_count to  'value'.";



const char* resetToValue_ds = "Resets llrf interlock, RF Ouptput, feed-forward phase and amplitude locks, and sets amplitude setpoint to 'value'";


const char* setUnwrapPhaseTolerance_1_ds = "Sets phase_tolerance for trace 'name' to 'value', when phase jumps are > phase_tolerance (and within a mask) the phase is unwrapped ";
const char* setUnwrapPhaseTolerance_2_ds = "Sets phase_tolerance for all traces to  'value', when phase jumps are > phase_tolerance (and within a mask) the phase is unwrapped ";
//


//char const* LLRF_PV_TYPE_docs = "enums for the llrf PVs";
//char const* LIB_RF_OUTPUT_docs = "RF Output ";
//char const* LIB_AMP_FF_docs = "Amplitude Feed-Forward";
//char const* LIB_AMP_SP_docs = "Amplitude Set point";
//char const* LIB_PHI_FF_docs = "Phase Feed-Forward";
//char const* LIB_PHI_SP_docs = "Phase Set Point";
//char const* LIB_CH1_PWR_REM_docs = "Channel 1 Power Remote Trace";
//char const* LIB_CH2_PWR_REM_docs = "Channel 1 Power Remote Trace";
//char const* LIB_CH3_PWR_REM_docs = "Channel 1 Power Remote Trace";
//char const* LIB_CH4_PWR_REM_docs = "Channel 4 Power Remote Trace";
//char const* LIB_CH5_PWR_REM_docs = "Channel 5 Power Remote Trace";
//char const* LIB_CH6_PWR_REM_docs = "Channel 6 Power Remote Trace";
//char const* LIB_CH7_PWR_REM_docs = "Channel 7 Power Remote Trace";
//char const* LIB_CH8_PWR_REM_docs = "Channel 8 Power Remote Trace";
//char const* LIB_CH1_PHASE_REM_docs = "";
//char const* LIB_CH2_PHASE_REM_docs = "";
//char const* LIB_CH3_PHASE_REM_docs = "";
//char const* LIB_CH4_PHASE_REM_docs = "";
//char const* LIB_CH5_PHASE_REM_docs = "";
//char const* LIB_CH6_PHASE_REM_docs = "";
//char const* LIB_CH7_PHASE_REM_docs = "";
//char const* LIB_CH8_PHASE_REM_docs = "";
//char const* LIB_TIME_VECTOR_docs = "";
//char const* LIB_PULSE_LENGTH_docs = "";
//char const* LIB_PULSE_OFFSET_docs = "";
//char const* TRIG_SOURCE_docs = "";
//char const* AMP_MVM_docs = "";
//char const* PHI_DEG_docs = "";
//char const* UNKNOWN_docs = "";
//
//char const* LIB_AMP_DER_SCAN_docs = "";
//char const* LIB_CH1_AMP_DER_SCAN_docs = "";
//char const* LIB_CH2_AMP_DER_SCAN_docs = "";
//char const* LIB_CH3_AMP_DER_SCAN_docs = "";
//char const* LIB_CH4_AMP_DER_SCAN_docs = "";
//char const* LIB_CH5_AMP_DER_SCAN_docs = "";
//char const* LIB_CH6_AMP_DER_SCAN_docs = "";
//char const* LIB_CH7_AMP_DER_SCAN_docs = "";
//char const* LIB_CH8_AMP_DER_SCAN = "";
//char const* LIB_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH1_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH2_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH3_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH4_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH5_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH6_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH7_PHASE_DER_SCAN_docs = "";
//char const* LIB_CH8_PHASE_DER_SCAN_docs = "";
//char const* LIB_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH2_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH1_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH3_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH4_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH5_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH6_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH7_PWR_LOCAL_SCAN_docs = "";
//char const* LIB_CH8_PWR_LOCAL_SCAN_docs = "";
//

//        ;                                                                 _docs = "";
//    enum_<llrfStructs::LLRF_TYPE>("LLRF_TYPE")                            _docs = "";
//        .value("CLARA_HRRG",LLRF_TYPE::CLARA_HRRG)                       _docs = "";
//        .value("CLARA_LRRG",LLRF_TYPE::CLARA_LRRG)                       _docs = "";
//        .value("VELA_HRRG", LLRF_TYPE::VELA_HRRG)                         _docs = "";
//        .value("VELA_LRRG", LLRF_TYPE::VELA_LRRG)
//        .value("L01", LLRF_TYPE::L01)
//        .value("UNKNOWN_TYPE", LLRF_TYPE::UNKNOWN_TYPE)
//        ;
//    enum_<llrfStructs::TRIG>("TRIG")
//        .value("OFF",TRIG::OFF)
//        .value("EXTERNAL",TRIG::EXTERNAL)
//        .value("INTERNAL", TRIG::INTERNAL)
//        .value("UNKNOWN_TRIG", TRIG::UNKNOWN_TRIG)
//        ;
//
//    enum_<llrfStructs::LLRF_SCAN>("LLRF_SCAN")
//        .value("PASSIVE",LLRF_SCAN::PASSIVE)
//        .value("EVENT",LLRF_SCAN::EVENT)
//        .value("IO_INTR", LLRF_SCAN::IO_INTR)
//        .value("TEN", LLRF_SCAN::TEN)
//        .value("FIVE", LLRF_SCAN::FIVE)
//        .value("TWO", LLRF_SCAN::TWO)
//        .value("ZERO_POINT_TWO", LLRF_SCAN::ZERO_POINT_TWO)
//        .value("ZERO_POINT_FIVE", LLRF_SCAN::ZERO_POINT_FIVE)
//        .value("ZERO_POINT_ONE", LLRF_SCAN::ZERO_POINT_ONE)
//        .value("UNKNOWN_SCAN", LLRF_SCAN::UNKNOWN_SCAN)
//        ;
//
//
//
//
//    class_<llrfStructs::rf_trace,boost::noncopyable>
//        ("rf_trace","rf_trace Doc String", no_init)
//        .def_readonly("value",    &rf_trace::value  ,"trace values")
//        .def_readonly("time",     &rf_trace::time   ,"Epics TimeStamp ns since epoch (double)")
//        .def_readonly("timeStr",  &rf_trace::timeStr,"Epics TimeStamp ns since epoch (string)")
//        .def_readonly("EVID",     &rf_trace::EVID   ,"Trace EVID string")
//        .def_readonly("EVID_time",&rf_trace::EVID_time   ,"Trace EVID time in ns since the epoch")
//        .def_readonly("EVID_timeStr",&rf_trace::EVID_timeStr   ,"Trace EVID time as a string")
//        .def_readonly("mean_start_index",&rf_trace::mean_start_index,"start index for mean trace calculation.")
//        .def_readonly("mean_stop_index", &rf_trace::mean_stop_index,"stop index for mean trace calculation.")
//        .def_readonly("mean", &rf_trace::mean,"mean pwoer between mean_start_index and mean_stop_index.")
//        ;
//
//
//
//    // rf_trace_data object, contains  struct to be exposed, used when returning a liberallrfObject reference
//    class_<llrfStructs::rf_trace_data,boost::noncopyable>
//        ("rf_trace_data","rf_trace_data object Doc-String", no_init)
//        .def_readonly("name",      &rf_trace_data::name,"tarce name")
//        .def_readonly("check_mask",      &rf_trace_data::check_mask,"should check mask")
//        .def_readonly("hi_mask_set",     &rf_trace_data::hi_mask_set,"is hi mask set")
//        .def_readonly("low_mask_set",    &rf_trace_data::low_mask_set,"is low mask set")
//        .def_readonly("keep_rolling_average",&rf_trace_data::keep_rolling_average,"should keep rolling average")
//        .def_readonly("keep_next_trace",&rf_trace_data::keep_next_trace,"should keep next trace")
//        .def_readonly("has_average",     &rf_trace_data::has_average,"has the trace calcualted an an average yet?")
//        .def_readonly("buffersize",      &rf_trace_data::buffersize,"number of traces in buffer")
//        .def_readonly("trace_size",      &rf_trace_data::trace_size,"number of elements in a trace")
//        .def_readonly("average_size",    &rf_trace_data::average_size,"number of traces to average")
////        .def_readonly("rolling_sum_counter", &rf_trace_data::rolling_sum_counter,"Total number of traces that have been added to the rolling sum (NOT the number of traces IN the rolling sum)")
//        .def_readonly("high_mask",       &rf_trace_data::high_mask,"high mask values")
//        .def_readonly("low_mask",        &rf_trace_data::low_mask,"low mask values")
//        .def_readonly("rolling_average", &rf_trace_data::rolling_average,"rolling average values")
//        .def_readonly("rolling_sum",     &rf_trace_data::rolling_sum,"rolling sum values")
//        .def_readonly("rolling_min",     &rf_trace_data::rolling_min,"rolling max values")
//        .def_readonly("rolling_max",     &rf_trace_data::rolling_max,"rolling min values")
//        .def_readonly("rolling_sd",      &rf_trace_data::rolling_sd,"rolling standard deviation values")
//        .def_readonly("traces",          &rf_trace_data::traces,"all trace data in buffer of rf_trace objects (stored in c++ as std::vector<llrfStructs::rf_trace> does this work?)")
//        .def_readonly("mean_start_index",&rf_trace_data::mean_start_index,"start index for mean trace calculation.")
//        .def_readonly("mean_stop_index", &rf_trace_data::mean_stop_index,"stop index for mean trace calculation.")
//        .def_readonly("EVID",              &rf_trace_data::EVID,"Latest EVID for this trace.")
//        .def_readonly("shot",     &rf_trace_data::shot   ,"shot number, (currently number of traces since monitoring started, in future will be timing system shotnumber?)")
//        //maybe have a ferernce to the latest trace??
//        .def_readonly("latest_trace_index",&rf_trace_data::latest_trace_index,"index for trace last updated.")
//        .def_readonly("current_trace",&rf_trace_data::current_trace,"index for current trace.")
//
//        .def_readonly("amp_drop_value", &rf_trace_data::amp_drop_value,"(when enabled) amp value to set on detecting outside mask trace.")
//        .def_readonly("drop_amp_on_breakdown", &rf_trace_data::drop_amp_on_breakdown,"If the amplitude should automatically be changed on detecting an outside mask trace.")
//        .def_readonly("mask_floor", &rf_trace_data::mask_floor,"Mask floor level.")
//        .def_readonly("num_continuous_outside_mask_count", &rf_trace_data::num_continuous_outside_mask_count,"number of continuous outside mask.")
////        .def_readonly("add_next_trace", &rf_trace_data::add_next_trace,"number of traces still to add to outside mask traces.")
//        .def_readonly("outside_mask_index", &rf_trace_data::outside_mask_index,"index of elmenet that caused outside mask event.")
//        .def_readonly("latest_max", &rf_trace_data::latest_max,"maximum value of latest trace.")
//        .def_readonly("scan", &rf_trace_data::scan,"SCAN value.")
//        ;
//
//    // The map with all the TRACE data (keyed by trace name from conifg file
//    class_< std::map<std::string, llrfStructs::rf_trace_data> >("A map of all the rf_trace_data (keyed by trace name from config file)", no_init)
//        .def(map_indexing_suite<std::map<std::string,rf_trace_data>>())
//        ;
//
//
//    // The map with all the TRACE data (keyed by trace name from conifg file
//    class_< std::vector<llrfStructs::rf_trace>>("A vector of rf_trace structs ", no_init)
//        .def(vector_indexing_suite< std::vector<llrfStructs::rf_trace> >())
//        ;
//
//    // outside_mask_trace is a sturct that holds flagged traces...
//    class_<outside_mask_trace>
//        ("outside_mask_trace","outside_mask_trace Doc String", no_init)
//        .def_readonly("rf_trace",   &outside_mask_trace::traces   ,  "rf_trace object")
//        .def_readonly("trace_name", &outside_mask_trace::trace_name,"Channel name trace came from")
//        .def_readonly("message", &outside_mask_trace::message,"a useful message")
//        .def_readonly("high_mask",  &outside_mask_trace::high_mask, "High mask values")
//        .def_readonly("low_mask",   &outside_mask_trace::low_mask,  "Low mask values")
//        .def_readonly("time",       &outside_mask_trace::low_mask,  "ms (approx) between timner start and trace flagged")
//        .def_readonly("num_traces_to_collect",       &outside_mask_trace::num_traces_to_collect,  "expeted number of traces")
//        .def_readonly("time_vector",       &outside_mask_trace::time_vector,  "time_vector (us)")
//        .def_readonly("mask_floor",       &outside_mask_trace::mask_floor,  "mask_floor (W)")
//        .def_readonly("is_collecting",       &outside_mask_trace::is_collecting,  "finished collecting traces")
//        ;
//
//    class_<std::vector<outside_mask_trace>,boost::noncopyable>("std_vector_outside_mask_trace", no_init)
//        .def(vector_indexing_suite< std::vector<outside_mask_trace>>())
//        ;
//
//    // liberallrfObject object struct to be exposed, used when returning a liberallrfObject reference
//    class_<liberallrfObject,boost::noncopyable>
//        ("liberallrfObject","liberallrfObject Doc String: LLRF virtual hardware object data members", no_init)
//        .def_readonly("name",          &liberallrfObject::name,"LLRF Object Name")
//        .def_readonly("pvRoot",        &liberallrfObject::pvRoot,"PV root")
//        .def_readonly("phiCalibration",&liberallrfObject::phiCalibration,"Linear Phase in LLRF units to degrees")
//        .def_readonly("ampCalibration",&liberallrfObject::ampCalibration,"Linear Amplitude LLRF units to MV/m")
//        .def_readonly("phi_DEG",  &liberallrfObject::phi_DEG,"Current Phase in degrees relative to crestPhi.")
//        .def_readonly("amp_MVM",  &liberallrfObject::amp_MVM,"Current Amplitude in MV/m.")
//        .def_readonly("phi_sp",   &liberallrfObject::phi_sp,"Current Libera Phase SP.")
//        .def_readonly("phi_ff",   &liberallrfObject::phi_ff,"Current Libera Phase FF.")
//        .def_readonly("amp_sp",   &liberallrfObject::amp_sp,"Current Libera Amplitude SP.")
//        .def_readonly("amp_ff",   &liberallrfObject::amp_ff,"Current Libera Amplitude FF.")
//        .def_readonly("crestPhi", &liberallrfObject::crestPhi,"Crest Phase.")
//        .def_readonly("maxAmp",   &liberallrfObject::maxAmp,"Maximum Amplitude Setting.")
//        .def_readonly("type",     &liberallrfObject::type,"LLRF Controller Type.")
//        .def_readonly("rf_output",&liberallrfObject::rf_output,"LLRF RF Output Status")
//        .def_readonly("pulse_length",&liberallrfObject::pulse_length,"LLRF RF pulse length")
//        .def_readonly("pulse_offset",&liberallrfObject::pulse_offset,"LLRF RF pulse offset")
//        .def_readonly("ff_amp_lock_state",  &liberallrfObject::ff_amp_lock_state,"status of the the amplitide FF check box.")
//        .def_readonly("ff_ph_lock_state",  &liberallrfObject::ff_ph_lock_state,"status of the the phase FF check box.")
//        .def_readonly("interlock_state",&liberallrfObject::interlock_state,"Libera interlock  state.")
//        .def_readonly("traceLength", &liberallrfObject::traceLength,"Number of elements in a trace.")
//        .def_readonly("trace_data",  &liberallrfObject::trace_data,"Map of rf_trace_data objects, keyed by the Trace Name (defined in config file).")
//        .def_readonly("time_vector", &liberallrfObject::time_vector,"The time vector, stored in a rf_trace_data object.")
//        .def_readonly("outside_mask_traces", &liberallrfObject::outside_mask_traces,"The saved outside_mask_traces, stored in a vector of outside_mask_trace objects.")
//        .def_readonly("num_outside_mask_traces", &liberallrfObject::num_outside_mask_traces,"The number of outside_mask_traces.")
//        .def_readonly("tracesToSaveOnBreakDown", &liberallrfObject::tracesToSaveOnBreakDown,"The names of the traces to save on break down event.")
//        .def_readonly("pulse_latency", &liberallrfObject::pulse_latency,"The number of elements in a pulse trace before the RF pulse is active (approx.).")
//        .def_readonly("timer_start", &liberallrfObject::timer_start,"ms since epoch since timer was started.")
//        .def_readonly("breakdown_rate", &liberallrfObject::breakdown_rate,"estimate of number breakdowns per second.")
//        .def_readonly("amp_drop_value", &liberallrfObject::amp_drop_value,"(when enabled) amp value to set on detecting outside mask trace.")
//        .def_readonly("drop_amp_on_breakdown", &liberallrfObject::drop_amp_on_breakdown,"If the amplitude should automatically be changed on detecting an outside mask trace.")
//        .def_readonly("activePulseCount", &liberallrfObject::activePulseCount,"(Total) Number of pulses with amp > 0 since connection.")
//        .def_readonly("pulseCount", &liberallrfObject::previous_pulseCount,"EVID as number.")
//        .def_readonly("pulseCountOffset", &liberallrfObject::pulseCountOffset,"offset to the pulseCountOffset.")
//        .def_readonly("trig_source", &liberallrfObject::trig_source,"the trigger source.")
//        .def_readonly("num_extra_traces", &liberallrfObject::num_extra_traces,"number of extra traces to save after outside mask event.")
//        .def_readonly("active_pulse_kly_power_limit", &liberallrfObject::active_pulse_kly_power_limit,"Power limit above which active pulser counter is increased.")
//        .def_readonly("kly_fwd_power_max", &liberallrfObject::kly_fwd_power_max,"Max value from latest Klystron Forward Power trace. (If monitored).")
//        .def_readonly("check_mask",      &liberallrfObject::check_mask,"should check mask")
//        .def_readonly("can_increase_active_pulses",      &liberallrfObject::can_increase_active_pulses,"can_increase_active_pulses value")
//        ;
//
//    class_<liberaLLRFController, bases<controller>, boost::noncopyable>
//        ("liberaLLRFController","liberaLLRFController Doc String: Main functions for LLRF control",no_init )
//        .def("getILockStates",    &liberaLLRFController::getILockStates)
//
//        .def("getType",    &liberaLLRFController::getType,"Return the Type of LLRF controller.")
//        .def("getAmpFF",   &liberaLLRFController::getAmpFF,"Return Amplitude(FF) in LLRF Units")
//        .def("getAmpSP",   &liberaLLRFController::getAmpSP,"Return Amplitude(SP) in LLRF Units")
//        .def("getAmpLLRF", &liberaLLRFController::getAmpLLRF,"Return Amplitude in LLRF Units")
//        .def("getAmpMVM",  &liberaLLRFController::getAmpMVM,"Return Amplitude in MV/m")
//        .def("getPhiFF",   &liberaLLRFController::getPhiFF,"Return the Phase(FF) in LLRF Units")
//        .def("getPhiSP",   &liberaLLRFController::getPhiSP,"Return the Phase(SP) in LLRF Units")
//        .def("getPhiLLRF", &liberaLLRFController::getPhiLLRF,"Return the Phase in LLRF Units")
//        .def("getNumOutsideMaskTraces", &liberaLLRFController::getNumOutsideMaskTraces,"Return the Number of elements in outside_mask_traces")
//        .def("getBreakDownRate", &liberaLLRFController::getBreakDownRate,"Return estimate of breakdowns per second.")
//
//        .def("getPhiCalibration", &liberaLLRFController::getPhiCalibration,"Return Linear Conversion of Phase from LLRF units to degrees")
//        .def("getAmpCalibration", &liberaLLRFController::getAmpCalibration,"Return Linear Conversion of Amplitude from LLRF units to MV/m")
//        .def("getCrestPhiLLRF",   &liberaLLRFController::getCrestPhiLLRF,"Return the Crest Phase in LLRF Units")
//
//        .def("setActivePulsePowerLimit",   &liberaLLRFController::setActivePulsePowerLimit,(arg("value")),"Set minimum Klystron Forward Power to enable active pulse increasing (below this value active pulses won't increment.).")
//        .def("getActivePulsePowerLimit",   &liberaLLRFController::getActivePulsePowerLimit,"Get minimum Klystron Forward Power to enable active pulse increasing.")
//
//        .def("getLLRFObjConstRef",&liberaLLRFController::getLLRFObjConstRef,return_value_policy<reference_existing_object>(),"Return LLRF Object Reference")
//        .def("getTraceDataConstRef",&liberaLLRFController::getTraceDataConstRef,return_value_policy<reference_existing_object>(),(arg("name")),"Return reference to LLRF Trace Object 'name'")
//
//        .def("getIndex",&liberaLLRFController::getIndex,(arg("time")),"Return index from trace at time 'time' (at lest thsi time?")
//        .def("getTime",&liberaLLRFController::getTime,(arg("index")),"Return trace_time at this index")
//        .def("getIndex",&liberaLLRFController::getIndex,(arg("time")),"Return index from trace at time 'time' (at lest thsi time?")
//
//
//
//        .def("getPhiDEG",  &liberaLLRFController::getPhiDEG,"Get Current Phase relative to Crest [degrees] +ve sense?")
//        .def("getPulseLength",  &liberaLLRFController::getPulseLength,"Return RF pulse length [micro-s]")
//        .def("getPulseOffset",  &liberaLLRFController::getPulseOffset,"Return RF Pulse Offset [micro-s]")
//        .def("getActivePulseCount",  &liberaLLRFController::getActivePulseCount,"Return number of pulses with amplitude > 0 sinze connection")
//        .def("addPulseCountOffset",  &liberaLLRFController::addPulseCountOffset,(arg("value")),"set an offset to the pulse counter (must be postive")
//
//        .def("isFFLocked",  &liberaLLRFController::isFFLocked,"Return true if FF check box is checked.")
//        .def("isFFNotLocked",  &liberaLLRFController::isFFNotLocked,"Return true if FF check box is not checked.")
//        .def("RFOutput",  &liberaLLRFController::RFOutput,"Return true if RF output is enabled.")
//
//        .def("interlockActive",  &liberaLLRFController::interlockActive,"Return true if interlock is Active.")
//        .def("interlockNotActive",  &liberaLLRFController::interlockNotActive,"Return true if interlock is not Active.")
//
//        .def("getTraceLength",  &liberaLLRFController::getTraceLength,"Return Number of elements in a power trace")
//
//        .def("getChannelNames",&liberaLLRFController::getChannelNames_Py,"Return Channel names (defined in config file)")
//        .def("getTraceNames",&liberaLLRFController::getTraceNames_Py,"Return Trace names (defined in config file)")
//
//        .def("getOutsideMaskData",&liberaLLRFController::getOutsideMaskData_Py,"Return Saved Data of traces outside masks")
//        .def("getOutsideMaskDataPart",&liberaLLRFController::getOutsideMaskData2_Py,(arg("part")),"Return index [part] from saved data of traces outside masks")
//        .def("isOutsideMaskDataFinishedCollecting",&liberaLLRFController::isOutsideMaskDataFinishedCollecting,(arg("part")),"Return treu if out_side_mask_traces 'part is still collecting data")
//
//
//        .def("getTraceData",   &liberaLLRFController::getTraceData,(arg("name")),"Return latest rf_trace object for Channel 'name'")
//        .def("getAverageTraceData",   &liberaLLRFController::getAverageTraceData_Py,(arg("name")),"Return latest average trace data for Channel 'name'")
//        .def("getTraceValues", &liberaLLRFController::getTraceValues_Py,(arg("name")),"Return latest Trace Values for channel 'name'")
//        .def("getTraceBuffer", &liberaLLRFController::getTraceBuffer_Py,(arg("name")),"Return buffer of rf_trace objects for channel 'name'")
//        .def("dump_traces", &liberaLLRFController::dump_traces,"Dump trace history")
//        .def("setKeepRollingAverageNoReset", setKeepRollingAverageNoReset_1,(arg("name"),arg("value")),"set tKeepRollingAverage flag, but don't chnage current average data")
//        .def("setKeepRollingAverageNoReset", setKeepRollingAverageNoReset_2,(arg("value")),"set tKeepRollingAverage flag, but don't chnage current average data")
//
//        .def("setDropAmpOnOutsideMaskDetection", &liberaLLRFController::setDropAmpOnOutsideMaskDetection,(arg("name"),arg("state"),arg("amp_val")),"Sets the 'state' for dropping the llrf amplitude to 'amp_val' when an outside mask is detected for trace 'name', returns true on success")
//        .def("setDropAmpValue", &liberaLLRFController::setDropAmpValue,(arg("name"),arg("amp_val")),"Sets the value to drop the amplitude to (when enabled) on detecting an outside mask trace for trace 'name' returns true on success")
//
//        .def("getCavRevPowerData",&liberaLLRFController::getCavRevPowerData,"Return latest cavity reverse power rf_trace object")
//        .def("getCavFwdPowerData",&liberaLLRFController::getCavFwdPowerData,"Return latest cavity forward power rf_trace object")
//        .def("getKlyRevPowerData",&liberaLLRFController::getKlyRevPowerData,"Return latest klystron reverse power rf_trace object")
//        .def("getKlyFwdPowerData",&liberaLLRFController::getKlyFwdPowerData,"Return latest klystron forward power rf_trace object")
//        .def("getCavRevPhaseData",&liberaLLRFController::getCavRevPhaseData,"Return latest cavity reverse phase rf_trace object")
//        .def("getCavFwdPhaseData",&liberaLLRFController::getCavFwdPhaseData,"Return latest cavity forward phase rf_trace object")
//        .def("getKlyRevPhaseData",&liberaLLRFController::getKlyRevPhaseData,"Return latest klystron reverse phase rf_trace object")
//        .def("getKlyFwdPhaseData",&liberaLLRFController::getKlyFwdPhaseData,"Return latest klystron forward phase rf_trace object")
//
//        .def("getCavRevPowerBuffer",&liberaLLRFController::getCavRevPowerBuffer_Py,"Return buffer of cavity reverse power rf_trace objects")
//        .def("getCavFwdPowerBuffer",&liberaLLRFController::getCavFwdPowerBuffer_Py,"Return buffer of cavity forward power rf_trace objects")
//        .def("getKlyRevPowerBuffer",&liberaLLRFController::getKlyRevPowerBuffer_Py,"Return buffer of klystron reverse power rf_trace objects")
//        .def("getKlyFwdPowerBuffer",&liberaLLRFController::getKlyFwdPowerBuffer_Py,"Return buffer of klystron forward power rf_trace objects")
//        .def("getCavRevPhaseBuffer",&liberaLLRFController::getCavRevPhaseBuffer_Py,"Return buffer of cavity reverse phase rf_trace objects")
//        .def("getCavFwdPhaseBuffer",&liberaLLRFController::getCavFwdPhaseBuffer_Py,"Return buffer of cavity forward phase rf_trace objects")
//        .def("getKlyRevPhaseBuffer",&liberaLLRFController::getKlyRevPhaseBuffer_Py,"Return buffer of klystron reverse phase rf_trace objects")
//        .def("getKlyFwdPhaseBuffer",&liberaLLRFController::getKlyFwdPhaseBuffer_Py,"Return buffer of klystron forward phase rf_trace objects")
//
//        .def("getCavRevPower",  &liberaLLRFController::getCavRevPower_Py,"Return latest cavity reverse power trace")
//        .def("getCavFwdPower",  &liberaLLRFController::getCavFwdPower_Py,"Return latest cavity forward power trace")
//        .def("getKlyRevPower",  &liberaLLRFController::getKlyRevPower_Py,"Return latest klystron reverse power trace")
//        .def("getKlyFwdPower",  &liberaLLRFController::getKlyFwdPower_Py,"Return latest klystron forward power trace")
//        .def("getCavRevPhase",  &liberaLLRFController::getCavRevPhase_Py,"Return latest cavity reverse phase trace")
//        .def("getCavFwdPhase",  &liberaLLRFController::getCavFwdPhase_Py,"Return latest cavity forward phase trace")
//        .def("getKlyRevPhase",  &liberaLLRFController::getKlyRevPhase_Py,"Return latest klystron reverse phase trace")
//        .def("getKlyFwdPhase",  &liberaLLRFController::getKlyFwdPhase_Py,"Return latest klystron forward phase trace")
//
//        .def("getCavRevPowerAv",  &liberaLLRFController::getCavRevPowerAv_Py,"Return latest cavity reverse power average trace")
//        .def("getCavFwdPowerAv",  &liberaLLRFController::getCavFwdPowerAv_Py,"Return latest cavity forward power average trace")
//        .def("getKlyRevPowerAv",  &liberaLLRFController::getKlyRevPowerAv_Py,"Return latest klystron reverse power average trace")
//        .def("getKlyFwdPowerAv",  &liberaLLRFController::getKlyFwdPowerAv_Py,"Return latest klystron forward power average trace")
//        .def("getCavRevPhaseAv",  &liberaLLRFController::getCavRevPhaseAv_Py,"Return latest cavity reverse phase average trace")
//        .def("getCavFwdPhaseAv",  &liberaLLRFController::getCavFwdPhaseAv_Py,"Return latest cavity forward phase average trace")
//        .def("getKlyRevPhaseAv",  &liberaLLRFController::getKlyRevPhaseAv_Py,"Return latest klystron reverse phase average trace")
//
//        .def("getProbePower",  &liberaLLRFController::getProbePower_Py,"Return cavity probe power")
//        .def("getProbePhase",  &liberaLLRFController::getProbePhase_Py,"Return cavity probe phase")
//
//        .def("getProbePowerAv",  &liberaLLRFController::getProbePowerAv_Py,"Return cavity probe power average ")
//        .def("getProbePhaseAv",  &liberaLLRFController::getProbePhaseAv_Py,"Return cavity probe phase average ")
//
//        //.def("setTracesToSaveOnBreakDown",  &liberaLLRFController::setTracesToSaveOnBreakDown,"Set the Names of Traces to Save when a break down occurs")
//        .def("setTracesToSaveOnBreakDown",  &liberaLLRFController::setTracesToSaveOnBreakDown_Py,"Set the Names of Traces to Save when a break down occurs")
//        .def("setTracesToSaveOnBreakDown",  &liberaLLRFController::setTracesToSaveOnBreakDown_2,"Set the Names of Traces to Save when a break down occurs")
//        .def("getTracesToSaveOnBreakDown",  &liberaLLRFController::getTracesToSaveOnBreakDown_Py,"Get the Names of Traces to Save when a break down occurs")
//
//        .def("setPhiLLRF", &liberaLLRFController::setPhiLLRF,(arg("value")),"Set Phase in LLRF Units")
//        .def("setAmpLLRF",&liberaLLRFController::setAmpLLRF,(arg("value")),"Set Amplitude in LLRF units")
//        .def("setPhiDEG",  &liberaLLRFController::setPhiDEG,(arg("value")),"Set Cavity Phase Relative to Creset [degrees] (+ve sense?)")
//        .def("setAmpMVM",  &liberaLLRFController::setAmpMVM,(arg("value")),"Set Cavity Amplitude [MV/m]")
//        .def("setPhiSP",  &liberaLLRFController::setPhiSP,(arg("value")),"Set Phase(SP) in LLRF Units")
//        .def("setPhiFF",  &liberaLLRFController::setPhiFF,(arg("value")),"Set Phase(FF) in LLRF Units")
//        .def("setAmpSP",  &liberaLLRFController::setAmpSP,(arg("value")),"Set Amplitude(SP) in LLRF Units")
//        .def("setAmpHP",  &liberaLLRFController::setAmpHP,(arg("value")),"Set Amplitude(HP) in LLRF Units")
//        .def("setAmpFF",  &liberaLLRFController::setAmpFF,(arg("value")),"Set Amplitude(FF) in LLRF Units")
//
//        .def("trigOff",  &liberaLLRFController::trigOff,"Set the LLRF trigger to off.")
//        .def("trigExt",  &liberaLLRFController::trigExt,"Set the LLRF trigger to external.")
//        .def("trigInt",  &liberaLLRFController::trigInt,"Set the LLRF trigger to internal.")
//
//        .def("getTrigSource",  &liberaLLRFController::getTrigSource,"Return current trigger source.")
//
//        .def("setMeanStartEndTime",  &liberaLLRFController::setMeanStartEndTime,(arg("start"),arg("end"),arg("name")),"Set trace 'name' start and end times for mean")
//        .def("setMeanStartIndex",  &liberaLLRFController::setMeanStartIndex,(arg("name"),arg("value")),"Set trace 'name' start index for mean calculation")
//        .def("setMeanStopIndex",  &liberaLLRFController::setMeanStopIndex,(arg("name"),arg("value")),"Set trace 'name' stop index for mean calculation")
//
//        .def("setTraceSCAN",  &liberaLLRFController::setTraceSCAN,(arg("name"),arg("value")),"Set trace 'name' SCAN rate to 'value' (if monitoring)")
//        .def("setAllTraceSCAN",  &liberaLLRFController::setAllTraceSCAN,(arg("value")),"Set all monitoring traces SCAN rate to 'value'")
//        .def("setAllSCANToPassive",  &liberaLLRFController::setAllSCANToPassive,"Set all SCAN to Passive")
//        .def("getHiMask",  &liberaLLRFController::getHiMask_Py,(arg("name")),"Get High mask for trace 'name'")
//        .def("getLoMask",  &liberaLLRFController::getLoMask_Py,(arg("name")),"Get Low mask for trace 'name'")
//
//        .def("resetAverageTraces",  &liberaLLRFController::resetAverageTraces,"Reset All Rolling Averages")
//
//        .def("clearMask",  &liberaLLRFController::clearMask,(arg("name")),"Clear the masks for trace 'name'")
//
//        .def("fullCavityTraceName",  &liberaLLRFController::fullCavityTraceName,(arg("name")),"Returns trace name being used")
//
//        .def("setPulseLength",  &liberaLLRFController::setPulseLength,(arg("value")),"Set RF pulse length [micro-s]")
//        .def("setPulseOffset",  &liberaLLRFController::setPulseOffset,(arg("value")),"Set RF pulse offset [micro-sec]")
//        .def("setPhiCalibration",  &liberaLLRFController::setPhiCalibration,(arg("value")),"Set linear calibration of phase from LLRF units to degrees")
//        .def("setAmpCalibration",  &liberaLLRFController::setAmpCalibration,(arg("value")),"Set linear calibration of amplitude from LLRF units to MV/m")
//        .def("setCrestPhiLLRF",  &liberaLLRFController::setCrestPhiLLRF,(arg("value")),"Set the Crest Phi value in LLRF Units")
//
//
//        .def("setInfiniteMasks",  &liberaLLRFController::setInfiniteMasks,(arg("name")),"Set infitine hi/lo masks for for trace 'name'")
//        .def("setHighMask",  setHighMask_1,(arg("name"),arg("value")),"Set the Hi mask for trace 'name'")
//        .def("setHighMask",  setHighMask_2,(arg("name"),arg("value")),"Set the Hi mask for trace 'name'")
//        .def("setLowMask",  setLowMask_1,(arg("name"),arg("value")),"Set the Lo mask for trace 'name'")
//        .def("setLowMask",  setLowMask_2,(arg("name"),arg("value")),"Set the Lo mask for trace 'name'")
//        .def("setCavRevPwrHiMask",  &liberaLLRFController::setCavRevPwrHiMask_Py,(arg("value")),"Set the Hi mask for cavity reverse power (channel defined in config file)")
//        .def("setCavRevPwrLoMask",  &liberaLLRFController::setCavRevPwrLoMask_Py,(arg("value")),"Set the Lo mask for cavity reverse power (channel defined in config file)")
//
//
//        .def("setPercentTimeMask",  &liberaLLRFController::setPercentTimeMask,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value"),arg("name")),"Set the mask for trace 'name' giving time co-oords for mask points")
//        .def("setAbsoluteTimeMask",  &liberaLLRFController::setAbsoluteTimeMask,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value"),arg("name")),"Set the mask for trace 'name' giving time co-oords for mask points")
//        .def("setPercentMask",  &liberaLLRFController::setPercentMask,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value"),arg("name")),"Set the mask for trace 'name' giving indixes for mask points")
//        .def("setAbsoluteMask",  &liberaLLRFController::setAbsoluteMask,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value"),arg("name")),"Set the mask for trace 'name' giving indixes for mask points")
//
//        .def("setMaskFloor",  &liberaLLRFController::setMaskFloor,(arg("name"),arg("value")),"Set the mask floor for trace 'name'")
//
//        .def("setNumExtraTraces",  &liberaLLRFController::setNumExtraTraces,(arg("value")),"Set the number of extra traces after out_side mask event")
//        .def("getNumExtraTraces",  &liberaLLRFController::getNumExtraTraces,"Get the number of extra traces after out_side mask event")
//
//        .def("setNumContinuousOutsideMaskCount",  &liberaLLRFController::setNumContinuousOutsideMaskCount,(arg("name"),arg("value")),"Set the number of continuous outside mask hits to trigger event, for trace 'name'")
//
//
//                //bool setPercentMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name);
//        //bool setAbsoluteMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name);
//
//
//        .def("setCavRevPwrMaskPercent",  &liberaLLRFController::setCavRevPwrMaskPercent,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
//            "set the Cavity Reverse Power Mask based on the rolling_average for cavity_rev_power trace/n"
//            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
//            "between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average/n"
//            "between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
//            "between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average/n"
//            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")
//
//        .def("setCavRevPwrMaskAbsolute",  &liberaLLRFController::setCavRevPwrMaskAbsolute,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
//            "set the Cavity Reverse Power Mask based on the rolling_average for cavity_rev_power trace/n"
//            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
//            "between element s1+1 and s2 will be set by rolling_average +/- value/n"
//            "between element s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
//            "between element s3+1 and s4 will be set by rolling_average +/- value/n"
//            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")
//
//        .def("setCavFwdPwrMaskPercent",  &liberaLLRFController::setCavFwdPwrMaskPercent,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
//            "set the Cavity Forward Power Mask based on the rolling_average for cavity_rev_power trace/n"
//            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
//            "between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average/n"
//            "between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
//            "between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average/n"
//            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")
//
//        .def("setCavFwdPwrMaskAbsolute",  &liberaLLRFController::setCavFwdPwrMaskAbsolute,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
//            "set the Cavity Forward Power Mask based on the rolling_average for cavity_rev_power trace/n"
//            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
//            "between element s1+1 and s2 will be set by rolling_average +/- value/n"
//            "between element s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
//            "between element s3+1 and s4 will be set by rolling_average +/- value/n"
//            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")
//
//        .def("setNumBufferTraces",  setNumBufferTraces_1,(arg("name"),arg("value")),"Set the number of buffer traces to keep for trace 'name' to 'value'")
//        .def("setNumBufferTraces",  setNumBufferTraces_2,(arg("value")),"Set the number of buffer traces for all traces to 'value'")
//
//        .def("startTimer", &liberaLLRFController::startTimer,"Starts (or resets) a local timer")
//        .def("elapsedTime",&liberaLLRFController::elapsedTime,"ms since the last startTimer call, (if negative then no call to startTimerhas been made)")
//        .def("offsetTimer",&liberaLLRFController::offsetTimer,"offeswt the timer by an integer amount")
//
//
//        .def("setCheckMask",  &liberaLLRFController::setCheckMask,(arg("name"),arg("value")),"Set whether to check (or not check) new traces against the mask (pass 'name' and true or false)")
//        .def("setShouldCheckMask",  &liberaLLRFController::setShouldCheckMask,(arg("name")),"Set check mask to true for trace 'name'")
//
//        .def("shouldCheckMasks",  &liberaLLRFController::shouldCheckMasks,(arg("name")),"return true if checking masks for trace 'name'")
//
//        .def("setShouldNotCheckMask",  &liberaLLRFController::setShouldNotCheckMask,(arg("name")),"Set check mask to false for trace 'name'")
//
//        .def("setGlobalCheckMask",  &liberaLLRFController::setGlobalCheckMask,(arg("value")),"Set Global check mask flag to  'value'")
//        .def("setGlobalShouldCheckMask",  &liberaLLRFController::setGlobalShouldCheckMask,"Set Global check mask flag to True")
//        .def("setGlobalShouldNotCheckMask",  &liberaLLRFController::setGlobalShouldNotCheckMask,"Set Global check mask flag to False")
//        .def("setKeepRollingAverage",  &liberaLLRFController::setKeepRollingAverage,(arg("name"),arg("value")),"Set whetrher to keep a rolling average of previous traces (pass 'name' and true or false)")
//        .def("setNumRollingAverageTraces",  setNumRollingAverageTraces_1,(arg("name"),arg("value")),"Set the number of traces used for the rolling average")
//        .def("setNumRollingAverageTraces",  setNumRollingAverageTraces_2,(arg("name")),"Set the number of traces used for the rolling average")
//
//
//        .def("setShouldKeepRollingAverage",   setShouldKeepRollingAverage_1,(arg("name")),"Set keep rolling average to true for trace 'name'")
//        .def("setShouldKeepRollingAverage",   setShouldKeepRollingAverage_2,"Set keep rolling average to true for all traces  ")
//
//        .def("setShouldNotKeepRollingAverage",setShouldNotKeepRollingAverage_1,(arg("name")),"Set keep rolling average to false for trace 'name'")
//        .def("setShouldNotKeepRollingAverage",setShouldNotKeepRollingAverage_2,"Set keep rolling average to false for all traces")
//
//        .def("getNumRollingAverageTraces",  &liberaLLRFController::getNumRollingAverageTraces,(arg("name")),"Get the number of traces to average for trace 'name'")
//        .def("clearRollingAverage",  &liberaLLRFController::clearRollingAverage,(arg("name")),"Clear the Rolling Average data for trace 'name' This also rests other counters to zero, meaning any/all current data in the rolling averages will be lost. ")
//
//        .def("isTracePV",  &liberaLLRFController::isTracePV,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE a power trace?")
//        .def("isNotTracePV",  &liberaLLRFController::isNotTracePV,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT a power trace?")
//
//        .def("isMonitoring",  isMonitoring_1,(arg("name")),"Is the trace 'name' being monitored?")
//        .def("isMonitoring",  isMonitoring_2,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE being monitored?")
//
//        .def("isNotMonitoring",  isNotMonitoring_1,(arg("name")),"Is the trace 'name' NOT being monitored?")
//        .def("isNotMonitoring",  isNotMonitoring_2,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT being monitored?")
//
//        .def("isCheckingMask", isCheckingMask_1,(arg("name")),"Is trace 'name' checking against a mask?")
//        .def("isCheckingMask", isCheckingMask_2,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE checking against a  mask?")
//
//        .def("isNotCheckingMask", isNotCheckingMask_1,(arg("name")),"Is trace 'name' NOT Checking against a mask?")
//        .def("isNotCheckingMask", isNotCheckingMask_2,(arg("LLRF_PV_TYPE")),"Is trace LLRF_PV_TYPE  NOT Checking against a mask?")
//
//
//        .def("getShotCount", &liberaLLRFController::getShotCount,(arg("name")),"Get Number of traces recieved by this controller for trace 'name'")
//
//
//        .def("startTraceMonitoring", startTraceMonitoring_1,"Start all LLRF Trace Monitors")
//        .def("startTraceMonitoring", startTraceMonitoring_2,(arg("LLRF_PV_TYPE")),"Start Trace Monitoring for LLRF_PV_TYPE")
//        .def("startTraceMonitoring", startTraceMonitoring_3,(arg("name")),"Start Trace Monitoring for 'name'")
//        .def("startCavFwdTraceMonitor", &liberaLLRFController::startCavFwdTraceMonitor,"Start Cavity Forward Power Trace Monitoring")
//        .def("startCavRevTraceMonitor", &liberaLLRFController::startCavRevTraceMonitor,"Start Cavity Reverse Power Trace Monitoring")
//        .def("startKlyFwdTraceMonitor", &liberaLLRFController::startKlyFwdTraceMonitor,"Start Klystron Forward Power Trace Monitoring")
//        .def("startKlyRevTraceMonitor", &liberaLLRFController::startKlyRevTraceMonitor,"Start Klystron Reverse Power Trace Monitoring")
//
//        .def("stopTraceMonitoring",stopTraceMonitoring_1,"Stop all LLRF Trace Monitors")
//        .def("stopTraceMonitoring",stopTraceMonitoring_2,(arg("LLRF_PV_TYPE")),"Stop Trace Monitoring for LLRF_PV_TYPE")
//        .def("stopTraceMonitoring",stopTraceMonitoring_3,(arg("name")),"Stop Trace Monitoring for trace 'name'")
//
//        .def("stopCavFwdTraceMonitor", &liberaLLRFController::stopCavFwdTraceMonitor,"Stop Cavity Forward Power Trace Monitoring")
//        .def("stopCavRevTraceMonitor", &liberaLLRFController::stopCavRevTraceMonitor,"Stop Cavity Reverse Power Trace Monitoring")
//        .def("stopKlyFwdTraceMonitor", &liberaLLRFController::stopKlyFwdTraceMonitor,"Stop Klystron Forward Power Trace Monitoring")
//        .def("stopKlyRevTraceMonitor", &liberaLLRFController::stopKlyRevTraceMonitor,"Stop Klystron Reverse Power Trace Monitoring")
//
//        // general methods
//        .def("get_CA_PEND_IO_TIMEOUT", &liberaLLRFController::get_CA_PEND_IO_TIMEOUT,"Time before Timeout when getting values")
//        .def("set_CA_PEND_IO_TIMEOUT", &liberaLLRFController::set_CA_PEND_IO_TIMEOUT,"Time before Timeout when setting values")
//        ;
//
//    // The main class that creates all the controller objects
//    class_<VCllrf,boost::noncopyable> ("init")
//        .def("virtual_CLARA_LRRG_LLRF_Controller", &VCllrf::virtual_CLARA_LRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_CLARA_LRRG_LLRF_Controller", &VCllrf::physical_CLARA_LRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_CLARA_LRRG_LLRF_Controller", &VCllrf::offline_CLARA_LRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("virtual_VELA_HRRG_LLRF_Controller", &VCllrf::virtual_VELA_HRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_VELA_HRRG_LLRF_Controller", &VCllrf::physical_VELA_HRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_VELA_HRRG_LLRF_Controller", &VCllrf::offline_VELA_HRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("virtual_VELA_LRRG_LLRF_Controller", &VCllrf::virtual_VELA_LRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_VELA_LRRG_LLRF_Controller", &VCllrf::physical_VELA_LRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_VELA_LRRG_LLRF_Controller", &VCllrf::offline_VELA_LRRG_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("virtual_L01_LLRF_Controller", &VCllrf::virtual_L01_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_L01_LLRF_Controller", &VCllrf::physical_L01_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_L01_LLRF_Controller", &VCllrf::offline_L01_LLRF_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("getLLRFController", &VCllrf::getLLRFController,
//             return_value_policy<reference_existing_object>())
//        .def("killGun",          &VCllrf::killGun)
//        .def("setQuiet",         &VCllrf::setQuiet)
//        .def("setVerbose",       &VCllrf::setVerbose)
//        .def("setMessage",       &VCllrf::setMessage)
//        .def("setDebugMessage",  &VCllrf::setDebugMessage)


#endif
