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
//  Last edit:   01-10-2018
//  FileName:    VCllrf.h
//  Description:
//
//
//*/
#ifndef _VC_LLRF_CONTROLLER_H_
#define _VC_LLRF_CONTROLLER_H_
//
#include <string>
//
#include "liberaLLRFController.h"
#include "llrfStructs.h"
//
#define MODULE_NAME VELA_CLARA_LLRF_Control
#include "VCheader.h"
#include "VCbase.h"
//______________________________________________________________________________
class VCllrf : public VCbase
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix

    public:
        VCllrf();
        ~VCllrf();
        liberaLLRFController& virtual_CLARA_LRRG_LLRF_Controller();
        liberaLLRFController& physical_CLARA_LRRG_LLRF_Controller();
        liberaLLRFController& offline_CLARA_LRRG_LLRF_Controller();

        liberaLLRFController& virtual_CLARA_HRRG_LLRF_Controller();
        liberaLLRFController& physical_CLARA_HRRG_LLRF_Controller();
        liberaLLRFController& offline_CLARA_HRRG_LLRF_Controller();

        liberaLLRFController& virtual_VELA_HRRG_LLRF_Controller();
        liberaLLRFController& physical_VELA_HRRG_LLRF_Controller();
        liberaLLRFController& offline_VELA_HRRG_LLRF_Controller();

        liberaLLRFController& virtual_VELA_LRRG_LLRF_Controller();
        liberaLLRFController& physical_VELA_LRRG_LLRF_Controller();
        liberaLLRFController& offline_VELA_LRRG_LLRF_Controller();

        liberaLLRFController& virtual_L01_LLRF_Controller();
        liberaLLRFController& physical_L01_LLRF_Controller();
        liberaLLRFController& offline_L01_LLRF_Controller();


        liberaLLRFController& getLLRFController(HWC_ENUM::MACHINE_MODE mode, llrfStructs::LLRF_TYPE type);

        void killGun();

        //liberaLLRFController& getliberaLLRFController(HWC_ENUM::MACHINE_MODE mode);

    protected:

    private:

     /// we just need to decide good names for these things...
        liberaLLRFController* GUN_LLRF_Controller_Obj;// you can only have one of these
        liberaLLRFController* L01_LLRF_Controller_Obj;

        liberaLLRFController& getController(liberaLLRFController*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           llrfStructs::LLRF_TYPE llrfType,
                                           const HWC_ENUM::MACHINE_AREA myMachineArea);

        bool isaGUN(llrfStructs::LLRF_TYPE llrfType);

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        const std::string CLARA_LRRG_LLRF_CONFIG, VELA_HRRG_LLRF_CONFIG,
                          CLARA_HRRG_LLRF_CONFIG, VELA_LRRG_LLRF_CONFIG,
                          CLARA_L01_LLRF_CONFIG;

        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const liberaLLRFController*, std::pair<bool,bool>> messageStates;

        void updateMessageStates();
};
/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
/// I'm going to do it the function pointer way here...
/// The other alternative is to create lots of different function names in the c++ class
/// just to make the function pointer overloading neater, let's make some typedefs, generally i don't like doing this
//actually we should do this with using
typedef double doub;
typedef const double cdou;
typedef std::vector<double> vecd;
typedef std::vector<std::vector<double>> vvcd;
typedef const std::vector<double> cved;
typedef const size_t csiz;
typedef size_t size;
typedef std::vector<size_t> vsiz;
typedef std::string stri;
typedef const std::string cstr;
typedef std::vector<std::string> vecs;
typedef const std::vector<std::string> cves;
typedef std::vector<bool> vecb;
typedef llrfStructs::LLRF_PV_TYPE rfpv;
typedef const llrfStructs::LLRF_PV_TYPE crfpv;
typedef const bool cbol;
//


//void(liberaLLRFController::*startTraceMonitoring_1)() = &liberaLLRFController::startTraceMonitoring;
//bool(liberaLLRFController::*startTraceMonitoring_2)(crfpv pv) = &liberaLLRFController::startTraceMonitoring;
//bool(liberaLLRFController::*startTraceMonitoring_3)(cstr& name) = &liberaLLRFController::startTraceMonitoring;
//void(liberaLLRFController::*stopTraceMonitoring_1)() = &liberaLLRFController::stopTraceMonitoring;
//bool(liberaLLRFController::*stopTraceMonitoring_2)(crfpv pv) = &liberaLLRFController::stopTraceMonitoring;
//bool(liberaLLRFController::*stopTraceMonitoring_3)(cstr& name) = &liberaLLRFController::stopTraceMonitoring;

bool(liberaLLRFController::*isCheckingMask_1)(cstr& name) = &liberaLLRFController::isCheckingMask;
bool(liberaLLRFController::*isCheckingMask_2)(crfpv pv) = &liberaLLRFController::isCheckingMask;

bool(liberaLLRFController::*isNotCheckingMask_1)(cstr& name) = &liberaLLRFController::isNotCheckingMask;
bool(liberaLLRFController::*isNotCheckingMask_2)(crfpv pv ) = &liberaLLRFController::isNotCheckingMask;

//bool(liberaLLRFController::*isMonitoring_1)(cstr& name) = &liberaLLRFController::isMonitoring;
//bool(liberaLLRFController::*isMonitoring_2)(crfpv pv ) = &liberaLLRFController::isNotCheckingMask;
//
//bool(liberaLLRFController::*isNotMonitoring_1)(cstr& name) = &liberaLLRFController::isNotMonitoring;
//bool(liberaLLRFController::*isNotMonitoring_2)(crfpv pv ) = &liberaLLRFController::isNotMonitoring;


//bool(liberaLLRFController::*setNumRollingAverageTraces_1)(cstr& name, csiz value) = &liberaLLRFController::setNumRollingAverageTraces;
//void(liberaLLRFController::*setNumRollingAverageTraces_2)(csiz value) = &liberaLLRFController::setNumRollingAverageTraces;

bool(liberaLLRFController::*setShouldKeepRollingAverage_1)(cstr& name) = &liberaLLRFController::setShouldKeepRollingAverage;
void(liberaLLRFController::*setShouldKeepRollingAverage_2)() = &liberaLLRFController::setShouldKeepRollingAverage;

bool(liberaLLRFController::*setShouldNotKeepRollingAverage_1)(cstr& name) = &liberaLLRFController::setShouldNotKeepRollingAverage;
void(liberaLLRFController::*setShouldNotKeepRollingAverage_2)() = &liberaLLRFController::setShouldNotKeepRollingAverage;

bool(liberaLLRFController::*setHighMask_1)(cstr& name,const boost::python::list& value) = &liberaLLRFController::setHighMask;
bool(liberaLLRFController::*setHighMask_2)(cstr& name, cved& value) = &liberaLLRFController::setHighMask;

bool(liberaLLRFController::*setLowMask_1)(cstr& name,const boost::python::list& value) = &liberaLLRFController::setLowMask;
bool(liberaLLRFController::*setLowMask_2)(cstr& name, cved& value) = &liberaLLRFController::setLowMask;

bool(liberaLLRFController::*setKeepRollingAverageNoReset_1)(cstr& name, cbol value) = &liberaLLRFController::setKeepRollingAverageNoReset;
void(liberaLLRFController::*setKeepRollingAverageNoReset_2)(cbol value) = &liberaLLRFController::setKeepRollingAverageNoReset;


bool(liberaLLRFController::*setIndividualTraceBufferSize_1)(cstr& name, csiz value) = &liberaLLRFController::setIndividualTraceBufferSize;
bool(liberaLLRFController::*setIndividualTraceBufferSize_2)(csiz value) = &liberaLLRFController::setIndividualTraceBufferSize;


bool(liberaLLRFController::*setNumContinuousOutsideMaskCount_1)(cstr& name, size_t value) = &liberaLLRFController::setNumContinuousOutsideMaskCount;
bool(liberaLLRFController::*setNumContinuousOutsideMaskCount_2)(size_t value) = &liberaLLRFController::setNumContinuousOutsideMaskCount;

bool(liberaLLRFController::*setUnwrapPhaseTolerance_1)(cstr& name, cdou value) = &liberaLLRFController::setUnwrapPhaseTolerance;
bool(liberaLLRFController::*setUnwrapPhaseTolerance_2)(cdou value) = &liberaLLRFController::setUnwrapPhaseTolerance;


//______________________________________________________________________________
#include "VCllrf_docstrings.h"
using namespace boost::python;
using namespace llrfStructs; // !!!!
BOOST_PYTHON_MODULE(MODULE_NAME)
{
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();

    // Things that you want to use in python muct be exposed:
    // containers

    BOOST_PYTHON_INCLUDE::export_BaseObjects();

//    enum_<LLRF_PV_TYPE>("LLRF_PV_TYPE")
//        .value("LIB_LOCK",   LLRF_PV_TYPE::LIB_RF_OUTPUT)
//        .value("LIB_AMP_FF", LLRF_PV_TYPE::LIB_AMP_FF)
//        .value("LIB_AMP_SP", LLRF_PV_TYPE::LIB_AMP_SP)
//        .value("LIB_PHI_FF", LLRF_PV_TYPE::LIB_PHI_FF)
//        .value("LIB_PHI_SP", LLRF_PV_TYPE::LIB_PHI_SP)
//        .value("LIB_CH1_PWR_REM", LLRF_PV_TYPE::LIB_CH1_PWR_REM)
//        .value("LIB_CH2_PWR_REM", LLRF_PV_TYPE::LIB_CH2_PWR_REM)
//        .value("LIB_CH3_PWR_REM", LLRF_PV_TYPE::LIB_CH3_PWR_REM)
//        .value("LIB_CH4_PWR_REM", LLRF_PV_TYPE::LIB_CH4_PWR_REM)
//        .value("LIB_CH5_PWR_REM", LLRF_PV_TYPE::LIB_CH5_PWR_REM)
//        .value("LIB_CH6_PWR_REM", LLRF_PV_TYPE::LIB_CH6_PWR_REM)
//        .value("LIB_CH7_PWR_REM", LLRF_PV_TYPE::LIB_CH7_PWR_REM)
//        .value("LIB_CH8_PWR_REM", LLRF_PV_TYPE::LIB_CH8_PWR_REM)
//        .value("LIB_CH1_PHASE_REM", LLRF_PV_TYPE::LIB_CH1_PHASE_REM)
//        .value("LIB_CH2_PHASE_REM", LLRF_PV_TYPE::LIB_CH2_PHASE_REM)
//        .value("LIB_CH3_PHASE_REM", LLRF_PV_TYPE::LIB_CH3_PHASE_REM)
//        .value("LIB_CH4_PHASE_REM", LLRF_PV_TYPE::LIB_CH4_PHASE_REM)
//        .value("LIB_CH5_PHASE_REM", LLRF_PV_TYPE::LIB_CH5_PHASE_REM)
//        .value("LIB_CH6_PHASE_REM", LLRF_PV_TYPE::LIB_CH6_PHASE_REM)
//        .value("LIB_CH7_PHASE_REM", LLRF_PV_TYPE::LIB_CH7_PHASE_REM)
//        .value("LIB_CH8_PHASE_REM", LLRF_PV_TYPE::LIB_CH8_PHASE_REM)
//        .value("LIB_TIME_VECFTORS",  LLRF_PV_TYPE::LIB_TIME_VECTOR)
//        .value("LIB_PULSE_LENGTH", LLRF_PV_TYPE::LIB_PULSE_LENGTH)
//        .value("LIB_PULSE_OFFSET", LLRF_PV_TYPE::LIB_PULSE_OFFSET)
//        .value("TRIG_SOURCE", LLRF_PV_TYPE::TRIG_SOURCE)
//        .value("AMP_MVM", LLRF_PV_TYPE::AMP_MVM)
//        .value("PHI_DEG", LLRF_PV_TYPE::PHI_DEG)
//        .value("UNKNOWN", LLRF_PV_TYPE::UNKNOWN)
//        ;
//
    enum_<LLRF_TYPE>("LLRF_TYPE")
        .value("CLARA_HRRG",LLRF_TYPE::CLARA_HRRG)
        .value("CLARA_LRRG",LLRF_TYPE::CLARA_LRRG)
        .value("VELA_HRRG", LLRF_TYPE::VELA_HRRG)
        .value("VELA_LRRG", LLRF_TYPE::VELA_LRRG)
        .value("L01", LLRF_TYPE::L01)
        .value("UNKNOWN_TYPE", LLRF_TYPE::UNKNOWN_TYPE)
        ;
    enum_<TRIG>("TRIG")
        .value("OFF",TRIG::OFF)
        .value("EXTERNAL",TRIG::EXTERNAL)
        .value("INTERNAL", TRIG::INTERNAL)
        .value("UNKNOWN_TRIG", TRIG::UNKNOWN_TRIG)
        ;

    enum_<LLRF_SCAN>("LLRF_SCAN")
        .value("PASSIVE",LLRF_SCAN::PASSIVE)
        .value("EVENT",LLRF_SCAN::EVENT)
        .value("IO_INTR", LLRF_SCAN::IO_INTR)
        .value("TEN", LLRF_SCAN::TEN)
        .value("FIVE", LLRF_SCAN::FIVE)
        .value("TWO", LLRF_SCAN::TWO)
        .value("ZERO_POINT_TWO", LLRF_SCAN::ZERO_POINT_TWO)
        .value("ZERO_POINT_FIVE", LLRF_SCAN::ZERO_POINT_FIVE)
        .value("ZERO_POINT_ONE", LLRF_SCAN::ZERO_POINT_ONE)
        .value("UNKNOWN_SCAN", LLRF_SCAN::UNKNOWN_SCAN)
        ;

    class_<std::pair<std::string, double> >("string_double_pair")
        .def_readonly("first",  &std::pair<std::string, double>::first)
        .def_readonly("second", &std::pair<std::string, double>::second)
        ;
//______________________________________________________________________________
    const char* rf_trace_ds = "rf_trace object holds the data from a LLRF "
                              "power/phase trace also holds timestamps, Event "
                              "IDs and indices used to calclate means";
//    const char* mean_start_index_ds = "start index for mean calculation.";
//    const char* mean_stop_index_ds = "stop index for mean calculation.";
    const char* EVID_timeStr_ds = "Trace EVID time as a string.";
    const char* EVID_time_ds = "Trace EVID time in ns since the epoch.";
    const char* timeStr_ds = "Epics TimeStamp ns since epoch (string).";
    const char* value_ds = "trace values.";
    const char* time_ds = "Epics TimeStamp ns since epoch (double).";
    const char* EVID_ds = "Trace EVID (string).";
    const char* mean_ds = "mean value between mean_start_index and "
                          "mean_stop_index.";

    class_<rf_trace,boost::noncopyable>("rf_trace",rf_trace_ds, no_init)
//        .def_readonly("mean_start_index",&rf_trace::mean_start_index,
//                      mean_start_index_ds)
//        .def_readonly("mean_stop_index", &rf_trace::mean_stop_index,
//                      mean_stop_index_ds)
        .def_readonly("EVID_timeStr",    &rf_trace::EVID_timeStr,
                      EVID_timeStr_ds)
        .def_readonly("EVID_time",&rf_trace::EVID_time,
                      EVID_time_ds)
        .def_readonly("timeStr",  &rf_trace::timeStr,
                      timeStr_ds)
        .def_readonly("value",    &rf_trace::value,
                      value_ds)
        .def_readonly("time",     &rf_trace::time,
                      time_ds)
        .def_readonly("EVID",     &rf_trace::EVID,
                      EVID_ds)
        .def_readonly("mean",     &rf_trace::mean,
                      mean_ds)
        ;
//______________________________________________________________________________
        const char* name_ds      = "trace name";
        const char* check_mask_ds      = "should check mask" ;
        const char* hi_mask_set_ds     = "is hi mask set";
        const char* low_mask_set_ds    = "is low mask set";
        const char* keep_rolling_average_ds= "should keep rolling average";
        const char* keep_next_trace_ds = "should keep next trace";
        const char* has_average_ds     = "has the trace calcualted an an average yet?";
        const char* buffersize_ds      = "number of traces in buffer";
        const char* trace_size_ds      = "number of elements in a trace";
        const char* average_size_ds    = "number of traces to average";
        const char* high_mask_ds       = "high mask values";
        const char* low_mask_ds        = "low mask values";
        const char* rolling_average_ds = "rolling average values";
        const char* rolling_sum_ds     = "rolling sum values";
        const char* rolling_min_ds     = "rolling max values";
        const char* rolling_max_ds     = "rolling min values";
        const char* rolling_sd_ds      = "rolling standard deviation values";
        const char* traces_ds          = "all trace data in buffer of rf_trace objects (stored in c++ as std::vector<rf_trace> does this work?)";
//        const char* mean_start_index_ds= "start index for mean trace calculation.";
//        const char* mean_stop_index_ds = "stop index for mean trace calculation.";
//        const char* EVID_ds              = "Latest EVID for this trace.";
        const char* shot_ds     ="shot number, (currently number of traces since monitoring started, in future will be timing system shotnumber?)";
        //maybe have a ferernce to the latest trace??
        const char* latest_trace_index_ds = "index for trace last updated.";
        const char* current_trace_ds = "index for current trace.";
        const char* amp_drop_value_ds = "(when enabled) amp value to set on detecting outside mask trace.";
        const char* drop_amp_on_breakdown_ds = "If the amplitude should automatically be changed on detecting an outside mask trace.";
        const char* mask_floor_ds = "Mask floor level.";
        const char* num_continuous_outside_mask_count_ds = "number of continuous outside mask.";
        const char* outside_mask_index_ds = "index of elmenet that caused outside mask event.";
        const char* latest_max_ds = "maximum value of latest trace.";
        const char* scan_ds = "SCAN value.";
        const char* endInfiniteMask_Trace_Set_ds = "is endInfiniteMask_Trace_Set?.";
        const char* mask_end_by_power_index_ds ="value mask_end_by_power_index.";
        const char* endMaskTrace_Bound_ds ="value mask_end_by_power_index.";


    // rf_trace_data object, contains  struct to be exposed, used when returning a liberallrfObject reference
    class_<rf_trace_data,boost::noncopyable>
        ("rf_trace_data","rf_trace_data object Doc-String", no_init)
        .def_readonly("name",      &rf_trace_data::name,"tarce name")
        .def_readonly("check_mask",      &rf_trace_data::check_mask,"should check mask")
        .def_readonly("hi_mask_set",     &rf_trace_data::hi_mask_set,"is hi mask set")
        .def_readonly("lo_mask_set",     &rf_trace_data::lo_mask_set,"is low mask set")
        .def_readonly("keep_rolling_average",&rf_trace_data::keep_rolling_average,"should keep rolling average")
        .def_readonly("keep_next_trace",&rf_trace_data::keep_next_trace,"should keep next trace")
        .def_readonly("has_average",     &rf_trace_data::has_average,"has the trace calcualted an an average yet?")
        .def_readonly("buffersize",      &rf_trace_data::buffersize,"number of traces in buffer")
        .def_readonly("trace_size",      &rf_trace_data::trace_size,"number of elements in a trace")
        .def_readonly("rolling_average_size",    &rf_trace_data::rolling_average_size,"number of traces to averag in rolling_average")
        .def_readonly("hi_mask",        &rf_trace_data::hi_mask,"hi mask values")
        .def_readonly("lo_mask",        &rf_trace_data::lo_mask,"lo mask values")
        .def_readonly("rolling_average", &rf_trace_data::rolling_average,"rolling average values")
        .def_readonly("rolling_sum",     &rf_trace_data::rolling_sum,"rolling sum values")
        .def_readonly("rolling_min",     &rf_trace_data::rolling_min,"rolling max values")
        .def_readonly("rolling_max",     &rf_trace_data::rolling_max,"rolling min values")
        .def_readonly("rolling_sd",      &rf_trace_data::rolling_sd,"rolling standard deviation values")
        //.def_readonly("traces",          &rf_trace_data::traces,"all trace data in buffer of rf_trace objects (stored in c++ as std::vector<rf_trace> does this work?)")
        .def_readonly("mean_start_index",&rf_trace_data::mean_start_index,"start index for mean trace calculation.")
        .def_readonly("mean_stop_index", &rf_trace_data::mean_stop_index,"stop index for mean trace calculation.")
        .def_readonly("mean",            &rf_trace_data::mean,"Mean value between mean_start_index and mean_stop_index for latest trace.")
        //.def_readonly("EVID",              &rf_trace_data::EVID,"Latest EVID for this trace.")
        //.def_readonly("shot",     &rf_trace_data::shot   ,"shot number, (currently number of traces since monitoring started, in future will be timing system shotnumber?)")
        //maybe have a ferernce to the latest trace??
//        .def_readonly("latest_trace_index",&rf_trace_data::latest_trace_index,"index for trace last updated.")
//        .def_readonly("current_trace",&rf_trace_data::current_trace,"index for current trace.")
        .def_readonly("amp_drop_value", &rf_trace_data::amp_drop_value,"(when enabled) amp value to set on detecting outside mask trace.")
        .def_readonly("drop_amp_on_breakdown", &rf_trace_data::drop_amp_on_breakdown,"If the amplitude should automatically be changed on detecting an outside mask trace.")
        .def_readonly("mask_floor", &rf_trace_data::mask_floor,"Mask floor level.")
        .def_readonly("num_continuous_outside_mask_count", &rf_trace_data::num_continuous_outside_mask_count,"number of continuous outside mask.")
        .def_readonly("outside_mask_index", &rf_trace_data::outside_mask_index,"index of elmenet that caused outside mask event.")
        .def_readonly("latest_max", &rf_trace_data::latest_max,"maximum value of latest trace.")
        //.def_readonly("scan", &rf_trace_data::scan,"SCAN value.")
        .def_readonly("endInfiniteMask_Trace_Set", &rf_trace_data::endInfiniteMask_Trace_Set,"is endInfiniteMask_Trace_Set?.")
        .def_readonly("mask_end_by_power_index", &rf_trace_data::mask_end_by_power_index,"value mask_end_by_power_index.")
        .def_readonly("endMaskTrace_Bound", &rf_trace_data::endMaskTrace_Bound,"value mask_end_by_power_index.")
        ;

    // The map with all the TRACE data (keyed by trace name from conifg file
    class_< std::map<std::string, rf_trace_data> >("A map of all the rf_trace_data (keyed by trace name from config file)", no_init)
        .def(map_indexing_suite<std::map<std::string,rf_trace_data>>())
        ;


    // The map with all the TRACE data (keyed by trace name from conifg file
    class_< std::vector<rf_trace>>("A vector of rf_trace structs ", no_init)
        .def(vector_indexing_suite< std::vector<rf_trace> >())
        ;

    // liberallrfObject object struct to be exposed, used when returning a liberallrfObject reference
    class_<liberallrfObject,boost::noncopyable>
        ("liberallrfObject","liberallrfObject Doc String: LLRF virtual hardware object data members", no_init)
        .def_readonly("name",          &liberallrfObject::name,"LLRF Object Name")
        .def_readonly("pvRoot",        &liberallrfObject::pvRoot,"PV root")
        .def_readonly("phiCalibration",&liberallrfObject::phiCalibration,"Linear Phase in LLRF units to degrees")
        .def_readonly("ampCalibration",&liberallrfObject::ampCalibration,"Linear Amplitude LLRF units to MV/m")
        .def_readonly("phi_DEG",  &liberallrfObject::phi_DEG,"Current Phase in degrees relative to crestPhi.")
        .def_readonly("amp_MVM",  &liberallrfObject::amp_MVM,"Current Amplitude in MV/m.")
        .def_readonly("phi_sp",   &liberallrfObject::phi_sp,"Current Libera Phase SP.")
        .def_readonly("phi_ff",   &liberallrfObject::phi_ff,"Current Libera Phase FF.")
        .def_readonly("amp_sp",   &liberallrfObject::amp_sp,"Current Libera Amplitude SP.")
        .def_readonly("amp_ff",   &liberallrfObject::amp_ff,"Current Libera Amplitude FF.")
        .def_readonly("crestPhi", &liberallrfObject::crestPhi,"Crest Phase.")
        .def_readonly("maxAmp",   &liberallrfObject::maxAmp,"Maximum Amplitude Setting.")
        .def_readonly("type",     &liberallrfObject::type,"LLRF Controller Type.")
        .def_readonly("rf_output",&liberallrfObject::rf_output,"LLRF RF Output Status")
        .def_readonly("pulse_length",&liberallrfObject::pulse_length,"LLRF RF pulse length")
        .def_readonly("pulse_offset",&liberallrfObject::pulse_offset,"LLRF RF pulse offset")
        .def_readonly("ff_amp_lock_state",  &liberallrfObject::ff_amp_lock_state,"status of the the amplitide FF check box.")
        .def_readonly("ff_ph_lock_state",  &liberallrfObject::ff_ph_lock_state,"status of the the phase FF check box.")
        .def_readonly("interlock_state",&liberallrfObject::interlock_state,"Libera interlock  state.")
        .def_readonly("traceLength", &liberallrfObject::traceLength,"Number of elements in a trace.")
        .def_readonly("trace_data",  &liberallrfObject::trace_data,"Map of rf_trace_data objects, keyed by the Trace Name (defined in config file).")

        .add_property("time_vector", &liberallrfObject::getTimeVector, &liberallrfObject::setTimeVector,"List of values fo rthe time coordinate for LLRF trace data")

        //.def_readonly("time_vector", &liberallrfObject::time_vector,"The time vector, stored in a rf_trace_data object.")
        //.def_readonly("outside_mask_traces", &liberallrfObject::outside_mask_traces,"The saved outside_mask_traces, stored in a vector of outside_mask_trace objects.")
        .def_readonly("omed_count", &liberallrfObject::omed_count,"The number of omed_count.")
        .def_readonly("tracesToSaveOnBreakDown", &liberallrfObject::tracesToSaveOnBreakDown,"The names of the traces to save on break down event.")
        .def_readonly("pulse_latency", &liberallrfObject::pulse_latency,"The number of elements in a pulse trace before the RF pulse is active (approx.).")
        .def_readonly("timer_start", &liberallrfObject::timer_start,"ms since epoch since timer was started.")
        .def_readonly("breakdown_rate", &liberallrfObject::breakdown_rate,"estimate of number breakdowns per second.")
        .def_readonly("amp_drop_value", &liberallrfObject::amp_drop_value,"(when enabled) amp value to set on detecting outside mask trace.")
        .def_readonly("drop_amp_on_breakdown", &liberallrfObject::drop_amp_on_breakdown,"If the amplitude should automatically be changed on detecting an outside mask trace.")
        .def_readonly("active_pulse_count", &liberallrfObject::active_pulse_count,"(Total) Number of pulses with amp > 0 since connection.")
        .def_readonly("pulseCount", &liberallrfObject::previous_pulseCount,"EVID as number.")
        .def_readonly("pulseCountOffset", &liberallrfObject::pulseCountOffset,"offset to the pulseCountOffset.")
        .def_readonly("trig_source", &liberallrfObject::trig_source,"the trigger source.")
        .def_readonly("num_extra_traces", &liberallrfObject::num_extra_traces,"number of extra traces to save after outside mask event.")
        .def_readonly("active_pulse_kly_power_limit", &liberallrfObject::active_pulse_kly_power_limit,"Power limit above which active pulser counter is increased.")
        .def_readonly("kly_fwd_power_max", &liberallrfObject::kly_fwd_power_max,"Max value from latest Klystron Forward Power trace. (If monitored).")
        .def_readonly("check_mask",      &liberallrfObject::check_mask,"should check mask")
        .def_readonly("can_increase_active_pulses",      &liberallrfObject::can_increase_active_pulses,"can_increase_active_pulses value")
        ;

    using namespace UTL;
    class_<liberaLLRFController, bases<controller>, boost::noncopyable>
        ("liberaLLRFController","liberaLLRFController Doc String: Main functions for LLRF control",no_init )

        .def("setTORACQMEvent",    &liberaLLRFController::setTORACQMEvent)
        .def("setTORSCANToIOIntr", &liberaLLRFController::setTORSCANToIOIntr)
        .def("setTORSCANToPassive", &liberaLLRFController::setTORSCANToPassive)
        .def("resetTORSCANToIOIntr", &liberaLLRFController::resetTORSCANToIOIntr)


//--------------------------------------------------------------------------------------------------------------------------------------
/*                    __  ___         ___     __             __   ___  __
                 /\  /  `  |  | \  / |__     |__) |  | |    /__` |__  /__`
                /~~\ \__,  |  |  \/  |___    |    \__/ |___ .__/ |___ .__/
*/
        .def("setActivePulsePowerLimit",   &liberaLLRFController::setActivePulsePowerLimit,(VALUE_ARG), setActivePulsePowerLimit_ds)
        .def("getActivePulsePowerLimit",   &liberaLLRFController::getActivePulsePowerLimit, getActivePulsePowerLimit_ds)
        .def("getActivePulseCount",  &liberaLLRFController::getActivePulseCount, getActivePulseCount_ds)
        .def("setActivePulseCount",  &liberaLLRFController::setActivePulseCount,(VALUE_ARG),setActivePulseCount_ds)
        .def("addActivePulseCountOffset",  &liberaLLRFController::addActivePulseCountOffset,(VALUE_ARG), addActivePulseCountOffset_ds)
//--------------------------------------------------------------------------------------------------------------------------------------


        .def("setUnwrapPhaseTolerance",   setUnwrapPhaseTolerance_1,(NAME_ARG, VALUE_ARG), setUnwrapPhaseTolerance_1_ds)
        .def("setUnwrapPhaseTolerance", setUnwrapPhaseTolerance_2,(VALUE_ARG),           setUnwrapPhaseTolerance_2_ds)




//        .def("usePercentMask",  &liberaLLRFController::usePercentMask, (NAME_ARG), usePercentMask_ds  )
//        .def("useAbsoluteMask", &liberaLLRFController::useAbsoluteMask,(NAME_ARG), useAbsoluteMask_ds  )
//        .def("setMaskValue", &liberaLLRFController::setMaskValue,(NAME_ARG,VALUE_ARG), setMaskValue_ds  )



        .def("getOneTraceData",    &liberaLLRFController::getOneTraceData_Py, "Returns a nestedt list of time-stamp and onetrace data")

        .def("getILockStates",    &liberaLLRFController::getILockStates)

        .def("getType",    &liberaLLRFController::getType,"Return the Type of LLRF controller.")
        .def("getAmpFF",   &liberaLLRFController::getAmpFF,"Return Amplitude(FF) in LLRF Units")
        .def("getAmpSP",   &liberaLLRFController::getAmpSP,"Return Amplitude(SP) in LLRF Units")
        .def("getAmpLLRF", &liberaLLRFController::getAmpLLRF,"Return Amplitude in LLRF Units")
        .def("getAmpMVM",  &liberaLLRFController::getAmpMVM,"Return Amplitude in MV/m")
        .def("getPhiFF",   &liberaLLRFController::getPhiFF,"Return the Phase(FF) in LLRF Units")
        .def("getPhiSP",   &liberaLLRFController::getPhiSP,"Return the Phase(SP) in LLRF Units")
        .def("getPhiLLRF", &liberaLLRFController::getPhiLLRF,"Return the Phase in LLRF Units")
        .def("getBreakDownRate", &liberaLLRFController::getBreakDownRate,"Return estimate of breakdowns per second.")

        .def("getPhiCalibration", &liberaLLRFController::getPhiCalibration,"Return Linear Conversion of Phase from LLRF units to degrees")
        .def("getAmpCalibration", &liberaLLRFController::getAmpCalibration,"Return Linear Conversion of Amplitude from LLRF units to MV/m")
        .def("getCrestPhiLLRF",   &liberaLLRFController::getCrestPhiLLRF,"Return the Crest Phase in LLRF Units")


        .def("getLLRFObjConstRef",&liberaLLRFController::getLLRFObjConstRef,return_value_policy<reference_existing_object>(),"Return LLRF Object Reference")
        .def("getTraceDataConstRef",&liberaLLRFController::getTraceDataConstRef,return_value_policy<reference_existing_object>(),(NAME_ARG),"Return reference to LLRF Trace Object 'name'")

        .def("getIndex",&liberaLLRFController::getIndex,(boost::python::arg("time")),"Return index from trace at time 'time' (at least this time?")
        .def("getTime",&liberaLLRFController::getTime,(boost::python::arg("index")),"Return trace_time at this index")
        //.def("getIndex",&liberaLLRFController::getIndex,(boost::python::arg("time")),"Return index from trace at time 'time' (at lest thsi time?")



        .def("getPhiDEG",  &liberaLLRFController::getPhiDEG,"Get Current Phase relative to Crest [degrees] +ve sense?")
        .def("getPulseLength",  &liberaLLRFController::getPulseLength,"Return RF pulse length [micro-s]")
        .def("getPulseOffset",  &liberaLLRFController::getPulseOffset,"Return RF Pulse Offset [micro-s]")


        .def("getTraceLength",  &liberaLLRFController::getTraceLength,"Return Number of elements in a power trace")

//        .def("getChannelNames",&liberaLLRFController::getChannelNames_Py,"Return Channel names (defined in config file)")
        .def("getTraceNames",&liberaLLRFController::getTraceNames_Py,"Return Trace names (defined in config file)")




//        .def("getTraceData",   &liberaLLRFController::getTraceData,(NAME_ARG),"Return latest rf_trace object for Channel 'name'")



//        .def("getTraceRollingAverageData",   &liberaLLRFController::getTraceRollingAverageData_Py,(NAME_ARG),"Return the current traces being averaged and the average, for Trace 'name'")

        .def("getTraceValues", &liberaLLRFController::getTraceValues_Py,(NAME_ARG),"Return latest Trace Values for channel 'name'")
        //.def("getTraceBuffer", &liberaLLRFController::getTraceBuffer_Py,(NAME_ARG),"Return buffer of rf_trace objects for channel 'name'")
        //.def("dump_traces", &liberaLLRFController::dump_traces,"Dump trace history")



        .def("getOutsideMaskEventCount", &liberaLLRFController::getOutsideMaskEventCount, getOutsideMaskEventCount_ds )





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

//        .def("getProbePower",  &liberaLLRFController::getProbePower_Py,"Return cavity probe power")
//        .def("getProbePhase",  &liberaLLRFController::getProbePhase_Py,"Return cavity probe phase")

//        .def("getProbePowerAv",  &liberaLLRFController::getProbePowerAv_Py,"Return cavity probe power average ")
//        .def("getProbePhaseAv",  &liberaLLRFController::getProbePhaseAv_Py,"Return cavity probe phase average ")

        .def("setKeepKlyFwdPwrRS", &liberaLLRFController::setKeepKlyFwdPwrRS,(VALUE_ARG),"Set keep klystron forward power running stat to 'val'")
        .def("keepKlyFwdPwrRS",    &liberaLLRFController::keepKlyFwdPwrRS,    "Set should keep klystron forward power running stat")
        .def("dontKeepKlyFwdPwrRS",&liberaLLRFController::dontKeepKlyFwdPwrRS,"Set should NOT keep klystron forward power running stat")
        .def("isKeepingKlyFwdPwrRS",&liberaLLRFController::isKeepingKlyFwdPwrRS,"Returns value of keep_kly_fwd_pow_running_stat ")


        .def("getKlyFwdPwrRSState",&liberaLLRFController::getKlyFwdPwrRSState_Py,(VALUE_ARG),"Get Klystron Forward Poewr Running Stat State for Ampset 'value'")
        .def("setKlyFwdPwrRSState",&liberaLLRFController::setKlyFwdPwrRSState,
             (boost::python::arg("amp_sp"),boost::python::arg("n"),boost::python::arg("old_mean"),boost::python::arg("old_variance"))
             ,"Set klystron amp set dictionary for 'amp_sp', 'n' = numvalues, 'old_mean', 'old_variance' ")


        .def("setPhiLLRF", &liberaLLRFController::setPhiLLRF,(VALUE_ARG),"Set Phase in LLRF Units")
        .def("setAmpLLRF", &liberaLLRFController::setAmpLLRF,(VALUE_ARG),"Set Amplitude in LLRF units")
        .def("setPhiDEG",  &liberaLLRFController::setPhiDEG,(VALUE_ARG),"Set Cavity Phase Relative to Creset [degrees] (+ve sense?)")
        .def("setAmpMVM",  &liberaLLRFController::setAmpMVM,(VALUE_ARG),"Set Cavity Amplitude [MV/m]")
        .def("setPhiSP",  &liberaLLRFController::setPhiSP,(VALUE_ARG),"Set Phase(SP) in LLRF Units")
        .def("setPhiFF",  &liberaLLRFController::setPhiFF,(VALUE_ARG),"Set Phase(FF) in LLRF Units")
        .def("setAmpSP",  &liberaLLRFController::setAmpSP,(VALUE_ARG),"Set Amplitude(SP) in LLRF Units")
        .def("setAmpHP",  &liberaLLRFController::setAmpHP,(VALUE_ARG),"Set Amplitude(HP) in LLRF Units")
        .def("setAmpFF",  &liberaLLRFController::setAmpFF,(VALUE_ARG),"Set Amplitude(FF) in LLRF Units")





        //.def("resetToValue",  &liberaLLRFController::resetToValue,(VALUE_ARG), resetToValue_ds)


        .def("isFFLocked",  &liberaLLRFController::isFFLocked,"Return true if both amp and phase feed-forward are locked.")
        .def("isFFNotLocked",  &liberaLLRFController::isFFNotLocked,"Return true if both amp and phase feed-forward are NOT locked.")
        .def("RFOutput",  &liberaLLRFController::RFOutput,"Return true if RF output is enabled.")
        .def("isInterlockActive",  &liberaLLRFController::isInterlockActive,"Return true if interlock is Active.")
        .def("isInterlockNotActive",  &liberaLLRFController::isInterlockNotActive,"Return true if interlock is not Active.")
        .def("isAmpFFLocked",  &liberaLLRFController::isAmpFFLocked,"")
        .def("isAmpFFNotLocked",  &liberaLLRFController::isAmpFFNotLocked,"")
        .def("isPhaseFFLocked",  &liberaLLRFController::isPhaseFFLocked,"")
        .def("isPhaseFFNotLocked",  &liberaLLRFController::isPhaseFFNotLocked,"")
        .def("isAmpFFLocked",  &liberaLLRFController::isAmpFFLocked,"")

        .def("isRFOutput",  &liberaLLRFController::isRFOutput,"")
        .def("isNotRFOutput",  &liberaLLRFController::isNotRFOutput,"")
        .def("isInterlockActive",  &liberaLLRFController::isInterlockActive,"")
        .def("isInterlockNotActive",  &liberaLLRFController::isInterlockNotActive,"")



        .def("setInfiniteMaskEndByPower",&liberaLLRFController::setInfiniteMaskEndByPower,(boost::python::arg("power_trace"),boost::python::arg("phase_trace"),boost::python::arg("level")),
             "Automatically set mask end of phase_trace by index of power 'level' in power_trace")

        .def("disableInfiniteMaskEndByPower",  &liberaLLRFController::disableInfiniteMaskEndByPower,(boost::python::arg("phase_trace")),"Disable automatic setting of mask infinte end.")

        .def("trigOff",  &liberaLLRFController::trigOff,"Set the LLRF trigger to off.")
        .def("trigExt",  &liberaLLRFController::trigExt,"Set the LLRF trigger to external.")
        .def("trigInt",  &liberaLLRFController::trigInt,"Set the LLRF trigger to internal.")

        .def("getTrigSource",  &liberaLLRFController::getTrigSource,"Return current trigger source.")

        .def("setMeanStartEndTime",  &liberaLLRFController::setMeanStartEndTime,(boost::python::arg("start"),boost::python::arg("end"),boost::python::arg("name")),"Set trace 'name' start and end times for mean")
        .def("setMeanStartIndex",  &liberaLLRFController::setMeanStartIndex,(boost::python::arg("name"),VALUE_ARG),"Set trace 'name' start index for mean calculation")
        .def("setMeanStopIndex",  &liberaLLRFController::setMeanStopIndex,(boost::python::arg("name"),VALUE_ARG),"Set trace 'name' stop index for mean calculation")

        .def("setTraceSCAN",  &liberaLLRFController::setTraceSCAN,(boost::python::arg("name"),VALUE_ARG),"Set trace 'name' SCAN rate to 'value' (if monitoring)")
        .def("setAllTraceSCAN",  &liberaLLRFController::setAllTraceSCAN,(VALUE_ARG),"Set all monitoring traces SCAN rate to 'value'")
        .def("setAllSCANToPassive",  &liberaLLRFController::setAllSCANToPassive,"Set all SCAN to Passive")
        .def("getHiMask",  &liberaLLRFController::getHiMask_Py,(boost::python::arg("name")),"Get High mask for trace 'name'")
        .def("getLoMask",  &liberaLLRFController::getLoMask_Py,(boost::python::arg("name")),"Get Low mask for trace 'name'")

       // .def("resetAverageTraces",  &liberaLLRFController::resetAverageTraces,"Reset All Rolling Averages")

        .def("clearMask",  &liberaLLRFController::clearMask,(boost::python::arg("name")),"Clear the masks for trace 'name'")

        .def("fullLLRFTraceName",  &liberaLLRFController::fullLLRFTraceName,(NAME_ARG),"Returns full (i.e. longform) trace-name for 'name'")
        .def("shortLLRFTraceName",  &liberaLLRFController::shortLLRFTraceName,(NAME_ARG),"Returns shorthand trace-name for 'name'")

        .def("getMaskInfiniteEndByPowerIndex",  &liberaLLRFController::getMaskInfiniteEndByPowerIndex,(boost::python::arg("name")),"Returns end index of trace 'name' from auto-setting end by power.")
        .def("getMaskInfiniteEndByPowerTime",  &liberaLLRFController::getMaskInfiniteEndByPowerTime,(boost::python::arg("name")),"Returns end time of trace 'name' from auto-setting end by power.")

        .def("setPulseLength",  &liberaLLRFController::setPulseLength,(VALUE_ARG),"Set RF pulse length [micro-s]")
        .def("setPulseOffset",  &liberaLLRFController::setPulseOffset,(VALUE_ARG),"Set RF pulse offset [micro-sec]")
        .def("setPhiCalibration",  &liberaLLRFController::setPhiCalibration,(VALUE_ARG),"Set linear calibration of phase from LLRF units to degrees")
        .def("setAmpCalibration",  &liberaLLRFController::setAmpCalibration,(VALUE_ARG),"Set linear calibration of amplitude from LLRF units to MV/m")
        .def("setCrestPhiLLRF",  &liberaLLRFController::setCrestPhiLLRF,(VALUE_ARG),"Set the Crest Phi value in LLRF Units")
        .def("setInfiniteMasks",  &liberaLLRFController::setInfiniteMasks,(boost::python::arg("name")),"Set infitine hi/lo masks for for trace 'name'")
        .def("setHighMask",  setHighMask_1,(boost::python::arg("name"),VALUE_ARG),"Set the Hi mask for trace 'name'")
        .def("setHighMask",  setHighMask_2,(boost::python::arg("name"),VALUE_ARG),"Set the Hi mask for trace 'name'")
        .def("setLowMask",  setLowMask_1,(boost::python::arg("name"),VALUE_ARG),"Set the Lo mask for trace 'name'")
        .def("setLowMask",  setLowMask_2,(boost::python::arg("name"),VALUE_ARG),"Set the Lo mask for trace 'name'")
        .def("setCavRevPwrHiMask",  &liberaLLRFController::setCavRevPwrHiMask_Py,(VALUE_ARG),"Set the Hi mask for cavity reverse power (channel defined in config file)")
        .def("setCavRevPwrLoMask",  &liberaLLRFController::setCavRevPwrLoMask_Py,(VALUE_ARG),"Set the Lo mask for cavity reverse power (channel defined in config file)")
        .def("setPercentTimeMask",  &liberaLLRFController::setPercentTimeMask,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG,boost::python::arg("name")),"Set the mask for trace 'name' giving time co-oords for mask points")
        .def("setAbsoluteTimeMask",  &liberaLLRFController::setAbsoluteTimeMask,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG,boost::python::arg("name")),"Set the mask for trace 'name' giving time co-oords for mask points")
        .def("setPercentMask",  &liberaLLRFController::setPercentMask,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG,boost::python::arg("name")),"Set the mask for trace 'name' giving indixes for mask points")
        .def("setAbsoluteMask",  &liberaLLRFController::setAbsoluteMask,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG,boost::python::arg("name")),"Set the mask for trace 'name' giving indixes for mask points")




//        .def("setNumContinuousOutsideMaskCount",  &liberaLLRFController::setNumContinuousOutsideMaskCount,(boost::python::arg("name"),VALUE_ARG),"Set the number of continuous outside mask hits to trigger event, for trace 'name'")


                //bool setPercentMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name);
        //bool setAbsoluteMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name);


        .def("setCavRevPwrMaskPercent",  &liberaLLRFController::setCavRevPwrMaskPercent,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG),
            "set the Cavity Reverse Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average/n"
            "between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setCavRevPwrMaskAbsolute",  &liberaLLRFController::setCavRevPwrMaskAbsolute,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG),
            "set the Cavity Reverse Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value/n"
            "between element s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setCavFwdPwrMaskPercent",  &liberaLLRFController::setCavFwdPwrMaskPercent,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG),
            "set the Cavity Forward Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average/n"
            "between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setCavFwdPwrMaskAbsolute",  &liberaLLRFController::setCavFwdPwrMaskAbsolute,(boost::python::arg("s1"),boost::python::arg("s2"),boost::python::arg("s3"),boost::python::arg("s4"),VALUE_ARG),
            "set the Cavity Forward Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value/n"
            "between element s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")



        .def("setAllTraceBufferSize",        &liberaLLRFController::setAllTraceBufferSize,(VALUE_ARG),setAllTraceBufferSize_ds)
        .def("setIndividualTraceBufferSize", setIndividualTraceBufferSize_2,              (VALUE_ARG),setIndividualTraceBufferSize_1_ds)
        .def("setIndividualTraceBufferSize", setIndividualTraceBufferSize_1,              (NAME_ARG,VALUE_ARG),setIndividualTraceBufferSize_2_ds)

        .def("startTimer", &liberaLLRFController::startTimer,"Starts (or resets) a local timer")
        .def("elapsedTime",&liberaLLRFController::elapsedTime,"ms since the last startTimer call, (if negative then no call to startTimerhas been made)")
        .def("offsetTimer",&liberaLLRFController::offsetTimer,"offeswt the timer by an integer amount")

        .def("setKeepRollingAverage",  &liberaLLRFController::setKeepRollingAverage,(boost::python::arg("name"),VALUE_ARG),"Set whetrher to keep a rolling average of previous traces (pass 'name' and true or false)")

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*       __   __                    __                ___  __        __   ___  __
        |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
        |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/
*/
        .def("clearAllRollingAverage",  &liberaLLRFController::clearAllRollingAverage, clearAllRollingAverage_ds)
        .def("clearTraceRollingAverage",  &liberaLLRFController::clearTraceRollingAverage, (NAME_ARG), clearTraceRollingAverage_ds)
        .def("setShouldKeepRollingAverage",   setShouldKeepRollingAverage_1,(NAME_ARG), setShouldKeepRollingAverage_1_ds)
        .def("setShouldKeepRollingAverage",   setShouldKeepRollingAverage_2, setShouldKeepRollingAverage_2_ds)
        .def("setShouldNotKeepRollingAverage",setShouldNotKeepRollingAverage_1,(NAME_ARG),setShouldNotKeepRollingAverage_1_ds)
        .def("setShouldNotKeepRollingAverage",setShouldNotKeepRollingAverage_2,setShouldNotKeepRollingAverage_2_ds)
        .def("setKeepRollingAverage",     &liberaLLRFController::setKeepRollingAverage,(NAME_ARG, VALUE_ARG),setKeepRollingAverage_ds)
        .def("setTraceRollingAverageSize",&liberaLLRFController::setTraceRollingAverageSize,(NAME_ARG, VALUE_ARG),setTraceRollingAverageSize_ds)
        .def("setAllRollingAverageSize",&liberaLLRFController::setAllRollingAverageSize,(VALUE_ARG),setAllRollingAverageSize_ds)
        //.def("getTraceRollingAverage",&liberaLLRFController::getTraceRollingAverage_Py,(NAME_ARG),getTraceRollingAverage_Py_ds)
        .def("getTraceRollingAverageSize",&liberaLLRFController::getTraceRollingAverageSize,(NAME_ARG),getTraceRollingAverageSize_ds)
        .def("getTraceRollingAverageCount",&liberaLLRFController::getTraceRollingAverageCount,(NAME_ARG),getTraceRollingAverageCount_ds)
        .def("isKeepingRollingAverage",&liberaLLRFController::isKeepingRollingAverage,(NAME_ARG),isKeepingRollingAverage_ds)
        .def("hasRollingAverage",&liberaLLRFController::hasRollingAverage,(NAME_ARG),hasRollingAverage_ds)


        .def("setKeepRollingAverageNoReset", setKeepRollingAverageNoReset_1,(NAME_ARG,VALUE_ARG),"set tKeepRollingAverage flag, but don't chnage current average data")
        .def("setKeepRollingAverageNoReset", setKeepRollingAverageNoReset_2,(VALUE_ARG),"set tKeepRollingAverage flag, but don't chnage current average data")
        .def("getRollingAverage",   &liberaLLRFController::getRollingAverage_Py,(NAME_ARG),"Return latest average trace data for Channel 'name'")
        .def("getRollingAverageData",   &liberaLLRFController::getRollingAverageData_Py,(NAME_ARG),"Return latest average trace data for Channel 'name'")


//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/
*/
        .def("setShouldNotCheckMask",&liberaLLRFController::setShouldNotCheckMask,(NAME_ARG), setShouldNotCheckMask_ds)
        .def("setMaskWindowIndices", &liberaLLRFController::setMaskWindowIndices, (NAME_ARG, SVAL_ARG, EVAL_ARG),setMaskWindowIndices_ds)
        .def("setShouldCheckMask",   &liberaLLRFController::setShouldCheckMask,   (NAME_ARG), setShouldCheckMask_ds)
        .def("setMaskWindowTimes",   &liberaLLRFController::setMaskWindowTimes,   (NAME_ARG, SVAL_ARG, EVAL_ARG),setMaskWindowTimes_ds)
        .def("setUseAbsoluteMask",   &liberaLLRFController::setUseAbsoluteMask,   (NAME_ARG),setUseAbsoluteMask_ds)
        .def("setMaskAbsMinValue",   &liberaLLRFController::setMaskAbsMinValue,   (NAME_ARG, VALUE_ARG),setMaskAbsMinValue_ds)
        .def("setUsePercentMask",    &liberaLLRFController::setUsePercentMask,    (NAME_ARG),setUsePercentMask_ds)
        .def("setMaskStartIndex",    &liberaLLRFController::setMaskStartIndex,    (NAME_ARG, VALUE_ARG),setMaskStartIndex_ds)
        .def("setMaskStartTime",     &liberaLLRFController::setMaskStartTime,     (NAME_ARG, VALUE_ARG),setMaskStartTime_ds)
        .def("setMaskEndIndex",      &liberaLLRFController::setMaskEndIndex,      (NAME_ARG, VALUE_ARG),setMaskEndIndex_ds)
        .def("setMaskEndTime",       &liberaLLRFController::setMaskEndTime,       (NAME_ARG, VALUE_ARG),setMaskEndTime_ds)
        .def("setMaskFloor",         &liberaLLRFController::setMaskFloor,         (NAME_ARG, VALUE_ARG),setMaskFloor_ds)
        .def("setCheckMask",         &liberaLLRFController::setCheckMask,         (NAME_ARG, VALUE_ARG), setCheckMask_ds)
        .def("setMaskValue",         &liberaLLRFController::setMaskValue,         (VALUE_ARG),setMaskValue_ds)
        .def("clearMask",            &liberaLLRFController::clearMask,     (NAME_ARG, VALUE_ARG),clearMask_ds)
        .def("setHiMask",            &liberaLLRFController::setHiMask_Py,  (NAME_ARG, VALUE_ARG),setHiMask_ds)
        .def("setLoMask",            &liberaLLRFController::setLoMask_Py,  (NAME_ARG, VALUE_ARG),setLoMask_ds)

        .def("setMaskParamatersIndices", &liberaLLRFController::setMaskParamatersIndices,
            (NAME_ARG, boost::python::arg("isPercent"), boost::python::arg("mask_value"), boost::python::arg("mask_floor"), boost::python::arg("mask_abs_min"),
             boost::python::arg("start"), boost::python::arg("end"), boost::python::arg("window_start"), boost::python::arg("window_end")
            ),setMaskParamatersIndices_ds)
        .def("setMaskParamatersTimes", &liberaLLRFController::setMaskParamatersTimes,
            (NAME_ARG, boost::python::arg("isPercent"), boost::python::arg("mask_value"), boost::python::arg("mask_floor"), boost::python::arg("mask_abs_min"),
             boost::python::arg("start"),
             boost::python::arg("end"), boost::python::arg("window_start"), boost::python::arg("window_end")
            ),setMaskParamatersTimes_ds)

        .def("setNumContinuousOutsideMaskCount",  setNumContinuousOutsideMaskCount_1,  (NAME_ARG, VALUE_ARG),setNumContinuousOutsideMaskCount_1_ds)
        .def("setNumContinuousOutsideMaskCount",  setNumContinuousOutsideMaskCount_2,  (VALUE_ARG),          setNumContinuousOutsideMaskCount_2_ds)
        .def("setGlobalCheckMask",          &liberaLLRFController::setGlobalCheckMask,(VALUE_ARG), setGlobalCheckMask_ds)
        .def("setGlobalShouldCheckMask",    &liberaLLRFController::setGlobalShouldCheckMask,       setGlobalShouldCheckMask_ds)
        .def("setGlobalShouldNotCheckMask", &liberaLLRFController::setGlobalShouldNotCheckMask,    setGlobalShouldNotCheckMask_ds)

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        .def("getHiMask",            &liberaLLRFController::getHiMask_Py,  (NAME_ARG, VALUE_ARG),getHiMask_ds)
        .def("getLoMask",            &liberaLLRFController::getLoMask_Py,  (NAME_ARG, VALUE_ARG),getLoMask_ds)
        .def("getMaskValue",         &liberaLLRFController::getMaskValue,  (NAME_ARG),getMaskValue_ds)
        .def("getMaskStartIndex",    &liberaLLRFController::getMaskStartIndex,(NAME_ARG),getMaskStartIndex_ds)
        .def("getMaskEndIndex",      &liberaLLRFController::getMaskEndIndex,  (NAME_ARG),getMaskEndIndex_ds)
        .def("getMaskFloor",         &liberaLLRFController::getMaskFloor,     (NAME_ARG),getMaskFloor_ds)
        .def("getMaskWindowStartIndex",&liberaLLRFController::getMaskWindowStartIndex,(NAME_ARG),getMaskWindowStartIndex_ds)
        .def("getMaskWindowEndIndex",  &liberaLLRFController::getMaskWindowEndIndex,  (NAME_ARG),getMaskWindowEndIndex_ds)
        .def("getMaskStartTime",       &liberaLLRFController::getMaskStartTime,       (NAME_ARG),getMaskStartTime_ds)
        .def("getMaskEndTime",         &liberaLLRFController::getMaskEndTime,         (NAME_ARG),getMaskEndTime_ds)
        .def("getMaskWindowStartTime", &liberaLLRFController::getMaskWindowStartTime, (NAME_ARG),getMaskWindowStartTime_ds)
        .def("getMaskWindowEndTime", &liberaLLRFController::getMaskWindowEndTime, (NAME_ARG),getMaskWindowEndTime_ds)



        .def("setNumExtraTracesOnOutsideMaskEvent",  &liberaLLRFController::setNumExtraTracesOnOutsideMaskEvent,(VALUE_ARG),setNumExtraTracesOnOutsideMaskEvent_ds)
        .def("getNumExtraTracesOnOutsideMaskEvent",  &liberaLLRFController::getNumExtraTracesOnOutsideMaskEvent,getNumExtraTracesOnOutsideMaskEvent_ds)
        .def("setTracesToSaveOnOutsideMaskEvent",  &liberaLLRFController::setTracesToSaveOnOutsideMaskEvent_Py,(NAMES_ARG),setTracesToSaveOnOutsideMaskEvent_Py_ds)
        .def("setTracesToSaveOnOutsideMaskEvent",  &liberaLLRFController::setTracesToSaveOnOutsideMaskEvent_2,(NAME_ARG),setTracesToSaveOnOutsideMaskEvent_2_ds)
        .def("getTracesToSaveOnOutsideMaskEvent",  &liberaLLRFController::getTracesToSaveOnOutsideMaskEvent_Py, getTracesToSaveOnOutsideMaskEvent_Py_ds)
        .def("setDropAmpOnOutsideMaskEvent", &liberaLLRFController::setDropAmpOnOutsideMaskEvent,(NAME_ARG, BOOL_ARG, VALUE_ARG),setDropAmpOnOutsideMaskEvent_ds)
        .def("setDropAmpOnOutsideMaskEventValue", &liberaLLRFController::setDropAmpOnOutsideMaskEventValue,(NAME_ARG, VALUE_ARG),setDropAmpOnOutsideMaskEventValue_ds)

        .def("getOutsideMaskEventDataSize", &liberaLLRFController::getOutsideMaskEventDataSize, getOutsideMaskEventDataSize_ds)

        //.def("getOutsideMaskEventDataPart",&liberaLLRFController::getOutsideMaskEventDataPart_Py,(PART_ARG), getOutsideMaskEventDataPart_ds)
        .def("getOutsideMaskEventData",    &liberaLLRFController::getOutsideMaskEventData_Py, getOutsideMaskEventData_ds)

        .def("isOutsideMaskEventDataCollecting",&liberaLLRFController::isOutsideMaskEventDataCollecting,isOutsideMaskEventDataCollecting_ds)
        .def("canGetOutsideMaskEventData",      &liberaLLRFController::canGetOutsideMaskEventData,      canGetOutsideMaskEventData_ds)



//        .def("getNumRollingAverageTraces",  &liberaLLRFController::getNumRollingAverageTraces,(boost::python::arg("name")),"Get the number of traces to average for trace 'name'")
//        .def("clearRollingAverage",  &liberaLLRFController::clearRollingAverage,(boost::python::arg("name")),"Clear the Rolling Average data for trace 'name' This also rests other counters to zero, meaning any/all current data in the rolling averages will be lost. ")

//        .def("isTracePV",  &liberaLLRFController::isTracePV,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE a power trace?")
//        .def("isNotTracePV",  &liberaLLRFController::isNotTracePV,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT a power trace?")

//        .def("isMonitoring",  isMonitoring_1,(boost::python::arg("name")),"Is the trace 'name' being monitored?")
//        .def("isMonitoring",  isMonitoring_2,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE being monitored?")
//
//        .def("isNotMonitoring",  isNotMonitoring_1,(boost::python::arg("name")),"Is the trace 'name' NOT being monitored?")
//        .def("isNotMonitoring",  isNotMonitoring_2,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT being monitored?")

        .def("isCheckingMask", isCheckingMask_1,(boost::python::arg("name")),"Is trace 'name' checking against a mask?")
        .def("isCheckingMask", isCheckingMask_2,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE checking against a  mask?")

        .def("isNotCheckingMask", isNotCheckingMask_1,(boost::python::arg("name")),"Is trace 'name' NOT Checking against a mask?")
        .def("isNotCheckingMask", isNotCheckingMask_2,(boost::python::arg("LLRF_PV_TYPE")),"Is trace LLRF_PV_TYPE  NOT Checking against a mask?")


        .def("getPulseCount", &liberaLLRFController::getPulseCount,(boost::python::arg("name")),"Get Number of traces recieved by this controller for trace 'name'")


//        .def("startTraceMonitoring", startTraceMonitoring_1,"Start all LLRF Trace Monitors")
//        .def("startTraceMonitoring", startTraceMonitoring_2,(boost::python::arg("LLRF_PV_TYPE")),"Start Trace Monitoring for LLRF_PV_TYPE")
//        .def("startTraceMonitoring", startTraceMonitoring_3,(boost::python::arg("name")),"Start Trace Monitoring for 'name'")
//        .def("startCavFwdTraceMonitor", &liberaLLRFController::startCavFwdTraceMonitor,"Start Cavity Forward Power Trace Monitoring")
//        .def("startCavRevTraceMonitor", &liberaLLRFController::startCavRevTraceMonitor,"Start Cavity Reverse Power Trace Monitoring")
//        .def("startKlyFwdTraceMonitor", &liberaLLRFController::startKlyFwdTraceMonitor,"Start Klystron Forward Power Trace Monitoring")
//        .def("startKlyRevTraceMonitor", &liberaLLRFController::startKlyRevTraceMonitor,"Start Klystron Reverse Power Trace Monitoring")
//
//        .def("stopTraceMonitoring",stopTraceMonitoring_1,"Stop all LLRF Trace Monitors")
//        .def("stopTraceMonitoring",stopTraceMonitoring_2,(boost::python::arg("LLRF_PV_TYPE")),"Stop Trace Monitoring for LLRF_PV_TYPE")
//        .def("stopTraceMonitoring",stopTraceMonitoring_3,(boost::python::arg("name")),"Stop Trace Monitoring for trace 'name'")
//
//        .def("stopCavFwdTraceMonitor", &liberaLLRFController::stopCavFwdTraceMonitor,"Stop Cavity Forward Power Trace Monitoring")
//        .def("stopCavRevTraceMonitor", &liberaLLRFController::stopCavRevTraceMonitor,"Stop Cavity Reverse Power Trace Monitoring")
//        .def("stopKlyFwdTraceMonitor", &liberaLLRFController::stopKlyFwdTraceMonitor,"Stop Klystron Forward Power Trace Monitoring")
//        .def("stopKlyRevTraceMonitor", &liberaLLRFController::stopKlyRevTraceMonitor,"Stop Klystron Reverse Power Trace Monitoring")

        // general methods
        .def("get_CA_PEND_IO_TIMEOUT", &liberaLLRFController::get_CA_PEND_IO_TIMEOUT,"Time before Timeout when getting values")
        .def("set_CA_PEND_IO_TIMEOUT", &liberaLLRFController::set_CA_PEND_IO_TIMEOUT,"Time before Timeout when setting values")
        ;

    // The main class that creates all the controller objects
    class_<VCllrf,bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_CLARA_LRRG_LLRF_Controller", &VCllrf::virtual_CLARA_LRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_LRRG_LLRF_Controller", &VCllrf::physical_CLARA_LRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_LRRG_LLRF_Controller", &VCllrf::offline_CLARA_LRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_HRRG_LLRF_Controller", &VCllrf::virtual_VELA_HRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_HRRG_LLRF_Controller", &VCllrf::physical_VELA_HRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_HRRG_LLRF_Controller", &VCllrf::offline_VELA_HRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_LRRG_LLRF_Controller", &VCllrf::virtual_VELA_LRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_LRRG_LLRF_Controller", &VCllrf::physical_VELA_LRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_LRRG_LLRF_Controller", &VCllrf::offline_VELA_LRRG_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_L01_LLRF_Controller", &VCllrf::virtual_L01_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_L01_LLRF_Controller", &VCllrf::physical_L01_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_L01_LLRF_Controller", &VCllrf::offline_L01_LLRF_Controller,
             return_value_policy<reference_existing_object>())
        .def("getLLRFController", &VCllrf::getLLRFController,
             return_value_policy<reference_existing_object>())
        .def("killGun",          &VCllrf::killGun)
//        .def("setQuiet",         &VCllrf::setQuiet)
//        .def("setVerbose",       &VCllrf::setVerbose)
//        .def("setMessage",       &VCllrf::setMessage)
//        .def("setDebugMessage",  &VCllrf::setDebugMessage)
        ;
}

#endif // _VCllrf_H
