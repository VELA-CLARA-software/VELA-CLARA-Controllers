#ifndef _VC_LLRF_CONTROLLER_H_
#define _VC_LLRF_CONTROLLER_H_
//stl
#include <string>
// vela-clara-controllers
#include "liberaLLRFController.h"
#include "llrfStructs.h"
//boost


#define MODULE_NAME VELA_CLARA_LLRF_Control

#include "VCheader.h"
//
//#include <boost/python/detail/wrap_python.hpp>
//#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/return_value_policy.hpp>
class VCllrf
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


        liberaLLRFController& getLLRFController(VELA_ENUM::MACHINE_MODE mode, llrfStructs::LLRF_TYPE type);

        // base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        //std::string CLARA_LRRG_LLRF_CONFIG,CLARA_L01_LLRF_CONFIG,VELA_HRRG_LLRF_CONFIG;

        void killGun();

        //liberaLLRFController& getliberaLLRFController(VELA_ENUM::MACHINE_MODE mode);

    protected:

    private:

     /// we just need to decide good names for these things...
        liberaLLRFController* GUN_LLRF_Controller_Obj;// you can only have one of these
        liberaLLRFController* L01_LLRF_Controller_Obj;

        liberaLLRFController& getController(bool shouldVM, bool shouldEPICS,llrfStructs::LLRF_TYPE llrfType);

        bool isaGUN(llrfStructs::LLRF_TYPE llrfType);

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string CLARA_LRRG_LLRF_CONFIG,CLARA_L01_LLRF_CONFIG,VELA_HRRG_LLRF_CONFIG;

};
/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
/// I'm going to do it the function pointer way here...
/// The other alternative is to create lots of different function names in the c++ class
/// just to make the function pointer overloading neater, let's make some typedefs, generally i don't like doing this
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
//


void(liberaLLRFController::*startTraceMonitoring_1)() = &liberaLLRFController::startTraceMonitoring;
bool(liberaLLRFController::*startTraceMonitoring_2)(crfpv pv) = &liberaLLRFController::startTraceMonitoring;
bool(liberaLLRFController::*startTraceMonitoring_3)(cstr& name) = &liberaLLRFController::startTraceMonitoring;
void(liberaLLRFController::*stopTraceMonitoring_1)() = &liberaLLRFController::stopTraceMonitoring;
bool(liberaLLRFController::*stopTraceMonitoring_2)(crfpv pv) = &liberaLLRFController::stopTraceMonitoring;
bool(liberaLLRFController::*stopTraceMonitoring_3)(cstr& name) = &liberaLLRFController::stopTraceMonitoring;

bool(liberaLLRFController::*isCheckingMask_1)(cstr& name) = &liberaLLRFController::isCheckingMask;
bool(liberaLLRFController::*isCheckingMask_2)(crfpv pv ) = &liberaLLRFController::isCheckingMask;

bool(liberaLLRFController::*isNotCheckingMask_1)(cstr& name) = &liberaLLRFController::isNotCheckingMask;
bool(liberaLLRFController::*isNotCheckingMask_2)(crfpv pv  ) = &liberaLLRFController::isNotCheckingMask;

bool(liberaLLRFController::*isMonitoring_1)(cstr& name) = &liberaLLRFController::isMonitoring;
bool(liberaLLRFController::*isMonitoring_2)(crfpv pv  ) = &liberaLLRFController::isNotCheckingMask;

bool(liberaLLRFController::*isNotMonitoring_1)(cstr& name) = &liberaLLRFController::isNotMonitoring;
bool(liberaLLRFController::*isNotMonitoring_2)(crfpv pv  ) = &liberaLLRFController::isNotMonitoring;


bool(liberaLLRFController::*setNumBufferTraces_1)(cstr& name, csiz value) = &liberaLLRFController::setNumBufferTraces;
void(liberaLLRFController::*setNumBufferTraces_2)(csiz value ) = &liberaLLRFController::setNumBufferTraces;


bool(liberaLLRFController::*setNumRollingAverageTraces_1)(cstr& name, csiz value) = &liberaLLRFController::setNumRollingAverageTraces;
void(liberaLLRFController::*setNumRollingAverageTraces_2)(csiz value ) = &liberaLLRFController::setNumRollingAverageTraces;


bool(liberaLLRFController::*setShouldKeepRollingAverage_1)(cstr& name) = &liberaLLRFController::setShouldKeepRollingAverage;
void(liberaLLRFController::*setShouldKeepRollingAverage_2)( ) = &liberaLLRFController::setShouldKeepRollingAverage;


bool(liberaLLRFController::*setShouldNotKeepRollingAverage_1)(cstr& name) = &liberaLLRFController::setShouldNotKeepRollingAverage;
void(liberaLLRFController::*setShouldNotKeepRollingAverage_2)() = &liberaLLRFController::setShouldNotKeepRollingAverage;


bool(liberaLLRFController::*setHighMask_1)(cstr& name,const boost::python::list& value) = &liberaLLRFController::setHighMask;
bool(liberaLLRFController::*setHighMask_2)(cstr& name, cved& value) = &liberaLLRFController::setHighMask;

bool(liberaLLRFController::*setLowMask_1)(cstr& name,const boost::python::list& value) = &liberaLLRFController::setLowMask;
bool(liberaLLRFController::*setLowMask_2)(cstr& name, cved& value) = &liberaLLRFController::setLowMask;

using namespace boost::python;
using namespace llrfStructs; // !!!!
BOOST_PYTHON_MODULE(MODULE_NAME)
{

    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();

    // Things that you want to use in python muct be exposed:
    // containers

    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    enum_<llrfStructs::LLRF_PV_TYPE>("LLRF_PV_TYPE")
        .value("LIB_LOCK",   LLRF_PV_TYPE::LIB_RF_OUTPUT )
        .value("LIB_AMP_FF", LLRF_PV_TYPE::LIB_AMP_FF )
        .value("LIB_AMP_SP", LLRF_PV_TYPE::LIB_AMP_SP)
        .value("LIB_PHI_FF", LLRF_PV_TYPE::LIB_PHI_FF)
        .value("LIB_PHI_SP", LLRF_PV_TYPE::LIB_PHI_SP)
        .value("LIB_CH1_PWR_REM", LLRF_PV_TYPE::LIB_CH1_PWR_REM)
        .value("LIB_CH2_PWR_REM", LLRF_PV_TYPE::LIB_CH2_PWR_REM)
        .value("LIB_CH3_PWR_REM", LLRF_PV_TYPE::LIB_CH3_PWR_REM)
        .value("LIB_CH4_PWR_REM", LLRF_PV_TYPE::LIB_CH4_PWR_REM)
        .value("LIB_CH5_PWR_REM", LLRF_PV_TYPE::LIB_CH5_PWR_REM)
        .value("LIB_CH6_PWR_REM", LLRF_PV_TYPE::LIB_CH6_PWR_REM)
        .value("LIB_CH7_PWR_REM", LLRF_PV_TYPE::LIB_CH7_PWR_REM)
        .value("LIB_CH8_PWR_REM", LLRF_PV_TYPE::LIB_CH8_PWR_REM)
        .value("LIB_CH1_PHASE_REM", LLRF_PV_TYPE::LIB_CH1_PHASE_REM)
        .value("LIB_CH2_PHASE_REM", LLRF_PV_TYPE::LIB_CH2_PHASE_REM)
        .value("LIB_CH3_PHASE_REM", LLRF_PV_TYPE::LIB_CH3_PHASE_REM)
        .value("LIB_CH4_PHASE_REM", LLRF_PV_TYPE::LIB_CH4_PHASE_REM)
        .value("LIB_CH5_PHASE_REM", LLRF_PV_TYPE::LIB_CH5_PHASE_REM)
        .value("LIB_CH6_PHASE_REM", LLRF_PV_TYPE::LIB_CH6_PHASE_REM)
        .value("LIB_CH7_PHASE_REM", LLRF_PV_TYPE::LIB_CH7_PHASE_REM)
        .value("LIB_CH8_PHASE_REM", LLRF_PV_TYPE::LIB_CH8_PHASE_REM)
        .value("LIB_TIME_VECTOR",  LLRF_PV_TYPE::LIB_TIME_VECTOR)
        .value("LIB_PULSE_LENGTH", LLRF_PV_TYPE::LIB_PULSE_LENGTH)
        .value("LIB_PULSE_OFFSET", LLRF_PV_TYPE::LIB_PULSE_OFFSET)
        .value("LIB_PULSE_OFFSET", LLRF_PV_TYPE::LIB_PULSE_OFFSET)
        .value("AMP_MVM", LLRF_PV_TYPE::AMP_MVM)
        .value("PHI_DEG", LLRF_PV_TYPE::PHI_DEG)
        .value("UNKNOWN", LLRF_PV_TYPE::UNKNOWN)
        ;
    enum_<llrfStructs::LLRF_TYPE>("LLRF_TYPE")
        .value("CLARA_HRRG",LLRF_TYPE::CLARA_HRRG )
        .value("CLARA_LRRG",LLRF_TYPE::CLARA_LRRG )
        .value("VELA_HRRG", LLRF_TYPE::VELA_HRRG)
        .value("VELA_LRRG", LLRF_TYPE::VELA_LRRG)
        .value("L01", LLRF_TYPE::L01)
        .value("UNKNOWN_TYPE", LLRF_TYPE::UNKNOWN_TYPE)
        ;

    enum_<llrfStructs::LLRF_SCAN>("LLRF_SCAN")
        .value("PASSIVE",LLRF_SCAN::PASSIVE )
        .value("EVENT",LLRF_SCAN::EVENT )
        .value("IO_INTR", LLRF_SCAN::IO_INTR)
        .value("TEN", LLRF_SCAN::TEN)
        .value("FIVE", LLRF_SCAN::FIVE)
        .value("TWO", LLRF_SCAN::TWO)
        .value("ZERO_POINT_TWO", LLRF_SCAN::ZERO_POINT_TWO)
        .value("ZERO_POINT_FIVE", LLRF_SCAN::ZERO_POINT_FIVE)
        .value("ZERO_POINT_ONE", LLRF_SCAN::ZERO_POINT_ONE)
        .value("UNKNOWN_SCAN", LLRF_SCAN::UNKNOWN_SCAN)
        ;
    class_<llrfStructs::rf_trace,boost::noncopyable>
        ("rf_trace","rf_trace Doc String", no_init)
        .def_readonly("value",    &rf_trace::value  ,"trace values")
        .def_readonly("time",     &rf_trace::time   ,"Epics TimeStamp ns since epoch (double)")
        .def_readonly("timeStr",  &rf_trace::timeStr,"Epics TimeStamp ns since epoch (string)")
        .def_readonly("EVID",     &rf_trace::EVID   ,"Trace EVID string")
        .def_readonly("EVID_time",&rf_trace::EVID_time   ,"Trace EVID time in ns since the epoch")
        .def_readonly("EVID_timeStr",&rf_trace::EVID_timeStr   ,"Trace EVID time as a string")
        .def_readonly("mean_start_index",&rf_trace::mean_start_index,"start index for mean trace calculation.")
        .def_readonly("mean_stop_index", &rf_trace::mean_stop_index,"stop index for mean trace calculation.")
        .def_readonly("mean", &rf_trace::mean,"mean pwoer between mean_start_index and mean_stop_index.")
        ;

    // rf_trace_data object, contains  struct to be exposed, used when returning a liberallrfObject reference
    class_<llrfStructs::rf_trace_data,boost::noncopyable>
        ("rf_trace_data","rf_trace_data object Doc-String", no_init)
        .def_readonly("check_mask",      &rf_trace_data::check_mask,"should check mask")
        .def_readonly("hi_mask_set",     &rf_trace_data::hi_mask_set,"is hi mask set")
        .def_readonly("low_mask_set",    &rf_trace_data::low_mask_set,"is low mask set")
        .def_readonly("keep_rolling_average",&rf_trace_data::keep_rolling_average,"should keep rolling average")
        .def_readonly("has_average",     &rf_trace_data::has_average,"has the trace calcualted an an average yet?")
        .def_readonly("buffersize",      &rf_trace_data::buffersize,"number of traces in buffer")
        .def_readonly("trace_size",      &rf_trace_data::trace_size,"number of elements in a trace")
        .def_readonly("average_size",    &rf_trace_data::average_size,"number of traces to average")
        .def_readonly("rolling_sum_counter",    &rf_trace_data::rolling_sum_counter,"Total number of traces that have been added to the rolling sum (NOT the number of traces IN the rolling sum)")
        .def_readonly("high_mask",       &rf_trace_data::high_mask,"high mask values")
        .def_readonly("low_mask",        &rf_trace_data::low_mask,"low mask values")
        .def_readonly("rolling_average", &rf_trace_data::rolling_average,"rolling average values")
        .def_readonly("rolling_sum",     &rf_trace_data::rolling_sum,"rolling sum values")
        .def_readonly("rolling_min",     &rf_trace_data::rolling_max,"rolling max values")
        .def_readonly("rolling_max",     &rf_trace_data::rolling_min,"rolling min values")
        .def_readonly("rolling_sd",     &rf_trace_data::rolling_sd,"rolling standard deviation values")
        .def_readonly("traces",          &rf_trace_data::traces,"all trace data in buffer of rf_trace objects ( stored in c++ as std::vector<llrfStructs::rf_trace> does this work?)")
        .def_readonly("mean_start_index",&rf_trace_data::mean_start_index,"start index for mean trace calculation.")
        .def_readonly("mean_stop_index", &rf_trace_data::mean_stop_index,"stop index for mean trace calculation.")
        .def_readonly("EVID",              &rf_trace_data::EVID,"Latest EVID for this trace.")
        .def_readonly("shot",     &rf_trace_data::shot   ,"shot number, (currently number of traces since monitoring started, in future will be timing system shotnumber?)")
        //maybe have a ferernce to the latest trace??
        .def_readonly("latest_trace_index",&rf_trace_data::latest_trace_index,"Latest EVID for this trace.")
        ;

    // The map with all the TRACE data (keyed by trace name from conifg file
    class_< std::map<std::string, llrfStructs::rf_trace_data> >("A map of all the rf_trace_data (keyed by trace name from config file)", no_init)
        .def(map_indexing_suite<std::map<std::string,rf_trace_data>>())
        ;


    // The map with all the TRACE data (keyed by trace name from conifg file
    class_< std::vector<llrfStructs::rf_trace>>("A vector of rf_trace structs ", no_init)
        .def(vector_indexing_suite< std::vector<llrfStructs::rf_trace> >())
        ;

    //typedef &llrfStructs::rf_trace rtr;
    //using namespace llrfStructs;
    // outside_mask_trace is a sturct that holds flagged traces...
    class_<outside_mask_trace>
        ("outside_mask_trace","outside_mask_trace Doc String", no_init)
        .def_readonly("rf_trace",   &outside_mask_trace::traces   ,  "rf_trace object")
        .def_readonly("trace_name", &outside_mask_trace::trace_name,"Channel name trace came from")
        .def_readonly("high_mask",  &outside_mask_trace::high_mask, "High mask values")
        .def_readonly("low_mask",   &outside_mask_trace::low_mask,  "Low mask values")
        .def_readonly("time",       &outside_mask_trace::low_mask,  "ms (approx) between timner start and trace flagged")
        ;

    class_<std::vector<outside_mask_trace>,boost::noncopyable>("std_vector_outside_mask_trace", no_init)
        .def(vector_indexing_suite< std::vector<outside_mask_trace>>())
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
        .def_readonly("ff_lock_state",  &liberallrfObject::ff_lock_state,"status of the the FF check box.")
        .def_readonly("interlock_state",&liberallrfObject::interlock_state,"Libera interlock  state.")
        .def_readonly("traceLength", &liberallrfObject::traceLength,"Number of elements in a trace.")
        .def_readonly("trace_data",  &liberallrfObject::trace_data,"Map of rf_trace_data objects, keyed by the Trace Name (defined in config file).")
        .def_readonly("time_vector", &liberallrfObject::time_vector,"The time vector, stored in a rf_trace_data object.")
        .def_readonly("outside_mask_traces", &liberallrfObject::outside_mask_traces,"The saved outside_mask_traces, stored in a vector of outside_mask_trace objects.")
        .def_readonly("num_outside_mask_traces", &liberallrfObject::num_outside_mask_traces,"The number of outside_mask_traces.")
        .def_readonly("tracesToSaveOnBreakDown", &liberallrfObject::tracesToSaveOnBreakDown,"The names of the traces to save on break down event.")
        .def_readonly("pulse_latency", &liberallrfObject::pulse_latency,"The number of elements in a pulse trace before the RF pulse is active (approx.).")
        .def_readonly("timer_start", &liberallrfObject::timer_start,"ms since epoch since timer was started.")
        .def_readonly("breakdown_rate", &liberallrfObject::breakdown_rate,"estimet of number breakdowns per second.")
        ;


    class_<liberaLLRFController, bases<controller>, boost::noncopyable>
        ("liberaLLRFController","liberaLLRFController Doc String: Main functions for LLRF control",no_init  )
        .def("getILockStates",    &liberaLLRFController::getILockStates)

        .def("getType",    &liberaLLRFController::getType,"Return the Type of LLRF controller.")
        .def("getAmpFF",   &liberaLLRFController::getAmpFF,"Return Amplitude(FF) in LLRF Units")
        .def("getAmpSP",   &liberaLLRFController::getAmpSP,"Return Amplitude(SP) in LLRF Units")
        .def("getAmpLLRF", &liberaLLRFController::getAmpLLRF,"Return Amplitude in LLRF Units")
        .def("getAmpMVM",  &liberaLLRFController::getAmpMVM,"Return Amplitude in MV/m")
        .def("getPhiFF",   &liberaLLRFController::getPhiFF,"Return the Phase(FF) in LLRF Units")
        .def("getPhiSP",   &liberaLLRFController::getPhiSP,"Return the Phase(SP) in LLRF Units")
        .def("getPhiLLRF", &liberaLLRFController::getPhiLLRF,"Return the Phase in LLRF Units")
        .def("getNumOutsideMaskTraces", &liberaLLRFController::getNumOutsideMaskTraces,"Return the Number of elements in outside_mask_traces")
        .def("getBreakDownRate", &liberaLLRFController::getBreakDownRate,"Return estimate of breakdowns per second.")

        .def("getPhiCalibration", &liberaLLRFController::getPhiCalibration,"Return Linear Conversion of Phase from LLRF units to degrees")
        .def("getAmpCalibration", &liberaLLRFController::getAmpCalibration,"Return Linear Conversion of Amplitude from LLRF units to MV/m")
        .def("getCrestPhiLLRF",   &liberaLLRFController::getCrestPhiLLRF,"Return the Crest Phase in LLRF Units")
        .def("getLLRFObjConstRef",&liberaLLRFController::getLLRFObjConstRef,return_value_policy<reference_existing_object>(),(arg("name")),"Return LLRF Object Reference")
        .def("getPhiDEG",  &liberaLLRFController::getPhiDEG,"Get Current Phase relative to Crest [degrees] +ve sense?")
        .def("getPulseLength",  &liberaLLRFController::getPulseLength,"Return RF pulse length [micro-s]")
        .def("getPulseOffset",  &liberaLLRFController::getPulseOffset,"Return RF Pulse Offset [micro-s]")

        .def("isFFLocked",  &liberaLLRFController::isFFLocked,"Return true if FF check box is checked.")
        .def("isFFNotLocked",  &liberaLLRFController::isFFNotLocked,"Return true if FF check box is not checked.")
        .def("RFOutput",  &liberaLLRFController::RFOutput,"Return true if RF output is enabled.")

        .def("interlockActive",  &liberaLLRFController::interlockActive,"Return true if interlock is Active.")
        .def("interlockNotActive",  &liberaLLRFController::interlockNotActive,"Return true if interlock is not Active.")

        .def("getTraceLength",  &liberaLLRFController::getTraceLength,"Return Number of elements in a power trace")

        .def("getChannelNames",&liberaLLRFController::getChannelNames_Py,"Return Channel names (defined in config file)" )
        .def("getTraceNames",&liberaLLRFController::getTraceNames_Py,"Return Trace names (defined in config file)" )

        .def("getOutsideMaskData",&liberaLLRFController::getOutsideMaskData_Py,"Return Saved Data of traces outside masks" )
        .def("getOutsideMaskData",&liberaLLRFController::getOutsideMaskData2_Py,(arg("parte")),"Return index [part] from saved data of traces outside masks" )

        .def("getTraceData",   &liberaLLRFController::getTraceData,(arg("name")),"Return latest rf_trace object for Channel 'name'")
        .def("getAverageTraceData",   &liberaLLRFController::getAverageTraceData_Py,(arg("name")),"Return latest average trace data for Channel 'name'")
        .def("getTraceValues", &liberaLLRFController::getTraceValues_Py,(arg("name")),"Return latest Trace Values for channel 'name'")
        .def("getTraceBuffer", &liberaLLRFController::getTraceBuffer_Py,(arg("name")),"Return buffer of rf_trace objects for channel 'name'")

        .def("getCavRevPowerData",&liberaLLRFController::getCavRevPowerData,"Return latest cavity reverse power rf_trace object")
        .def("getCavFwdPowerData",&liberaLLRFController::getCavFwdPowerData,"Return latest cavity forward power rf_trace object")
        .def("getKlyRevPowerData",&liberaLLRFController::getKlyRevPowerData,"Return latest klystron reverse power rf_trace object")
        .def("getKlyFwdPowerData",&liberaLLRFController::getKlyFwdPowerData,"Return latest klystron forward power rf_trace object")
        .def("getCavRevPhaseData",&liberaLLRFController::getCavRevPhaseData,"Return latest cavity reverse phase rf_trace object")
        .def("getCavFwdPhaseData",&liberaLLRFController::getCavFwdPhaseData,"Return latest cavity forward phase rf_trace object")
        .def("getKlyRevPhaseData",&liberaLLRFController::getKlyRevPhaseData,"Return latest klystron reverse phase rf_trace object")
        .def("getKlyFwdPhaseData",&liberaLLRFController::getKlyFwdPhaseData,"Return latest klystron forward phase rf_trace object")

        .def("getCavRevPowerBuffer",&liberaLLRFController::getCavRevPowerBuffer_Py,"Return buffer of cavity reverse power rf_trace objects")
        .def("getCavFwdPowerBuffer",&liberaLLRFController::getCavFwdPowerBuffer_Py,"Return buffer of cavity forward power rf_trace objects")
        .def("getKlyRevPowerBuffer",&liberaLLRFController::getKlyRevPowerBuffer_Py,"Return buffer of klystron reverse power rf_trace objects")
        .def("getKlyFwdPowerBuffer",&liberaLLRFController::getKlyFwdPowerBuffer_Py,"Return buffer of klystron forward power rf_trace objects")
        .def("getCavRevPhaseBuffer",&liberaLLRFController::getCavRevPhaseBuffer_Py,"Return buffer of cavity reverse phase rf_trace objects")
        .def("getCavFwdPhaseBuffer",&liberaLLRFController::getCavFwdPhaseBuffer_Py,"Return buffer of cavity forward phase rf_trace objects")
        .def("getKlyRevPhaseBuffer",&liberaLLRFController::getKlyRevPhaseBuffer_Py,"Return buffer of klystron reverse phase rf_trace objects")
        .def("getKlyFwdPhaseBuffer",&liberaLLRFController::getKlyFwdPhaseBuffer_Py,"Return buffer of klystron forward phase rf_trace objects")


        .def("getCavRevPower",  &liberaLLRFController::getCavRevPower_Py,"Return latest cavity reverse power trace")
        .def("getCavFwdPower",  &liberaLLRFController::getCavFwdPower_Py,"Return latest cavity forward power trace")
        .def("getKlyRevPower",  &liberaLLRFController::getKlyRevPower_Py,"Return latest klystron reverse power trace")
        .def("getKlyFwdPower",  &liberaLLRFController::getKlyFwdPower_Py,"Return latest klystron forward power trace")
        .def("getCavRevPhase",  &liberaLLRFController::getCavRevPhase_Py,"Return latest cavity reverse phase trace")
        .def("getCavFwdPhase",  &liberaLLRFController::getCavFwdPhase_Py,"Return latest cavity forward phase trace")
        .def("getKlyRevPhase",  &liberaLLRFController::getKlyRevPhase_Py,"Return latest klystron reverse phase trace")
        .def("getKlyFwdPhase",  &liberaLLRFController::getKlyFwdPhase_Py,"Return latest klystron forward phase trace")

        .def("getCavRevPowerAv",  &liberaLLRFController::getCavRevPowerAv_Py,"Return latest cavity reverse power average trace")
        .def("getCavFwdPowerAv",  &liberaLLRFController::getCavFwdPowerAv_Py,"Return latest cavity forward power average trace")
        .def("getKlyRevPowerAv",  &liberaLLRFController::getKlyRevPowerAv_Py,"Return latest klystron reverse power average trace")
        .def("getKlyFwdPowerAv",  &liberaLLRFController::getKlyFwdPowerAv_Py,"Return latest klystron forward power average trace")
        .def("getCavRevPhaseAv",  &liberaLLRFController::getCavRevPhaseAv_Py,"Return latest cavity reverse phase average trace")
        .def("getCavFwdPhaseAv",  &liberaLLRFController::getCavFwdPhaseAv_Py,"Return latest cavity forward phase average trace")
        .def("getKlyRevPhaseAv",  &liberaLLRFController::getKlyRevPhaseAv_Py,"Return latest klystron reverse phase average trace")

        .def("getProbePower",  &liberaLLRFController::getProbePower_Py,"Return cavity probe power")
        .def("getProbePhase",  &liberaLLRFController::getProbePhase_Py,"Return cavity probe phase")

        .def("getProbePowerAv",  &liberaLLRFController::getProbePowerAv_Py,"Return cavity probe power average ")
        .def("getProbePhaseAv",  &liberaLLRFController::getProbePhaseAv_Py,"Return cavity probe phase average ")

        //.def("setTracesToSaveOnBreakDown",  &liberaLLRFController::setTracesToSaveOnBreakDown,"Set the Names of Traces to Save when a break down occurs")
        .def("setTracesToSaveOnBreakDown",  &liberaLLRFController::setTracesToSaveOnBreakDown_Py,"Set the Names of Traces to Save when a break down occurs")
        .def("getTracesToSaveOnBreakDown",  &liberaLLRFController::getTracesToSaveOnBreakDown_Py,"Get the Names of Traces to Save when a break down occurs")

        .def("setPhiLLRF", &liberaLLRFController::setPhiLLRF,(arg("value")),"Set Phase in LLRF Units")
        .def("setAmpLLRF",&liberaLLRFController::setAmpLLRF,(arg("value")),"Set Amplitude in LLRF units")
        .def("setPhiDEG",  &liberaLLRFController::setPhiDEG,(arg("value")),"Set Cavity Phase Relative to Creset [degrees] (+ve sense?)")
        .def("setAmpMVM",  &liberaLLRFController::setAmpMVM,(arg("value")),"Set Cavity Amplitude [MV/m]")
        .def("setPhiSP",  &liberaLLRFController::setPhiSP,(arg("value")),"Set Phase(SP) in LLRF Units")
        .def("setPhiFF",  &liberaLLRFController::setPhiFF,(arg("value")),"Set Phase(FF) in LLRF Units")
        .def("setAmpSP",  &liberaLLRFController::setAmpSP,(arg("value")),"Set Amplitude(SP) in LLRF Units")
        .def("setAmpFF",  &liberaLLRFController::setAmpFF,(arg("value")),"Set Amplitude(FF) in LLRF Units")

        .def("setMeanStartIndex",  &liberaLLRFController::setMeanStartIndex,(arg("name"),arg("value")),"Set trace 'name' start index for mean calculation")
        .def("setMeanStopIndex",  &liberaLLRFController::setMeanStopIndex,(arg("name"),arg("value")),"Set trace 'name' stop index for mean calculation")


        .def("setTraceSCAN",  &liberaLLRFController::setTraceSCAN,(arg("name"),arg("value")),"Set trace 'name' SCAN rate to 'value' (if monitoring)")
        .def("setAllTraceSCAN",  &liberaLLRFController::setAllTraceSCAN,(arg("value")),"Set all monitoring traces SCAN rate to 'value'")
        .def("getHiMask",  &liberaLLRFController::getHiMask_Py,(arg("name")),"Get High mask for trace 'name'")
        .def("getLoMask",  &liberaLLRFController::getLoMask_Py,(arg("name")),"Get Low mask for trace 'name'")


        .def("resetAverageTraces",  &liberaLLRFController::resetAverageTraces,"Reset All Rolling Averages")

        .def("clearMask",  &liberaLLRFController::clearMask,(arg("name")),"Cleark the masks for trace 'name'")

        .def("setPulseLength",  &liberaLLRFController::setPulseLength,(arg("value")),"Set RF pulse length [micro-s]")
        .def("setPulseOffset",  &liberaLLRFController::setPulseOffset,(arg("value")),"Set RF pulse offset [micro-sec]")
        .def("setPhiCalibration",  &liberaLLRFController::setPhiCalibration,(arg("value")),"Set linear calibration of phase from LLRF units to degrees")
        .def("setAmpCalibration",  &liberaLLRFController::setAmpCalibration,(arg("value")),"Set linear calibration of amplitude from LLRF units to MV/m")
        .def("setCrestPhiLLRF",  &liberaLLRFController::setCrestPhiLLRF,(arg("value")),"Set the Crest Phi value in LLRF Units")

        .def("setHighMask",  setHighMask_1,(arg("name"),arg("value")),"Set the Hi mask for trace 'name'")
        .def("setHighMask",  setHighMask_2,(arg("name"),arg("value")),"Set the Hi mask for trace 'name'")
        .def("setLowMask",  setLowMask_1,(arg("name"),arg("value")),"Set the Lo mask for trace 'name'")
        .def("setLowMask",  setLowMask_2,(arg("name"),arg("value")),"Set the Lo mask for trace 'name'")
        .def("setCavRevPwrHiMask",  &liberaLLRFController::setCavRevPwrHiMask_Py,(arg("value")),"Set the Hi mask for cavity reverse power (channel defined in config file)")
        .def("setCavRevPwrLoMask",  &liberaLLRFController::setCavRevPwrLoMask_Py,(arg("value")),"Set the Lo mask for cavity reverse power (channel defined in config file)")


        .def("setCavRevPwrMaskPercent",  &liberaLLRFController::setCavRevPwrMaskPercent,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
            "set the Cavity Reverse Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average/n"
            "between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setCavRevPwrMaskAbsolute",  &liberaLLRFController::setCavRevPwrMaskAbsolute,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
            "set the Cavity Reverse Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value/n"
            "between element s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setCavFwdPwrMaskPercent",  &liberaLLRFController::setCavFwdPwrMaskPercent,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
            "set the Cavity Forward Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average/n"
            "between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setCavFwdPwrMaskAbsolute",  &liberaLLRFController::setCavFwdPwrMaskAbsolute,(arg("s1"),arg("s2"),arg("s3"),arg("s4"),arg("value")),
            "set the Cavity Forward Power Mask based on the rolling_average for cavity_rev_power trace/n"
            "between element 0    and s1 will be set to default hi/lo (+/-infinity)/n"
            "between element s1+1 and s2 will be set by rolling_average +/- value/n"
            "between element s2+1 and s3 will be set very default hi/lo (+/-infinity)/n"
            "between element s3+1 and s4 will be set by rolling_average +/- value/n"
            "between element s3+1 and s4 will be set very default hi/lo (+/-infinity)/n")

        .def("setNumBufferTraces",  setNumBufferTraces_1,(arg("name"),arg("value")),"Set the number of buffer traces to keep for trace 'name' to 'value'")
        .def("setNumBufferTraces",  setNumBufferTraces_2,(arg("value")),"Set the number of buffer traces for all traces to 'value'")

        .def("startTimer", &liberaLLRFController::startTimer,"Starts (or resets) a local timer")
        .def("elapsedTime",&liberaLLRFController::elapsedTime,"ms since the last startTimer call, (if negative then no call to startTimerhas been made)")
        .def("offsetTimer",&liberaLLRFController::offsetTimer,"offeswt the timer by an integer amount")


        .def("setCheckMask",  &liberaLLRFController::setCheckMask,(arg("name"),arg("value")),"Set whether to check (or not check) new traces against the mask (pass 'name' and true or false)")
        .def("setShouldCheckMask",  &liberaLLRFController::setShouldCheckMask,(arg("name")),"Set check mask to true for trace 'name'")
        .def("setShouldNotCheckMask",  &liberaLLRFController::setShouldNotCheckMask,(arg("name")),"Set check mask to false for trace 'name'")

        .def("setGlobalCheckMask",  &liberaLLRFController::setGlobalCheckMask,(arg("value")),"Set Global check mask flag to  'value'")
        .def("setGlobalShouldCheckMask",  &liberaLLRFController::setGlobalShouldCheckMask,"Set Global check mask flag to True")
        .def("setGlobalShouldNotCheckMask",  &liberaLLRFController::setGlobalShouldNotCheckMask,"Set Global check mask flag to False")
        .def("setKeepRollingAverage",  &liberaLLRFController::setKeepRollingAverage,(arg("name"),arg("value")),"Set whetrher to keep a rolling average of previous traces (pass 'name' and true or false)")
        .def("setNumRollingAverageTraces",  setNumRollingAverageTraces_1,(arg("name"),arg("value")),"Set the number of traces used for the rolling average")
        .def("setNumRollingAverageTraces",  setNumRollingAverageTraces_2,(arg("name")),"Set the number of traces used for the rolling average")


        .def("setShouldKeepRollingAverage",   setShouldKeepRollingAverage_1,(arg("name")),"Set keep rolling average to true for trace 'name'")
        .def("setShouldKeepRollingAverage",   setShouldKeepRollingAverage_2,"Set keep rolling average to true for all traces  ")

        .def("setShouldNotKeepRollingAverage",setShouldNotKeepRollingAverage_1,(arg("name")),"Set keep rolling average to false for trace 'name'")
        .def("setShouldNotKeepRollingAverage",setShouldNotKeepRollingAverage_2,"Set keep rolling average to false for all traces")

        .def("getNumRollingAverageTraces",  &liberaLLRFController::getNumRollingAverageTraces,(arg("name")),"Get the number of traces to average for trace 'name'")
        .def("clearRollingAverage",  &liberaLLRFController::clearRollingAverage,(arg("name")),"Clear the Rolling Average data for trace 'name' This also rests other counters to zero, meaning any/all current data in the rolling averages will be lost. ")

        .def("isTracePV",  &liberaLLRFController::isTracePV,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE a power trace?")
        .def("isNotTracePV",  &liberaLLRFController::isNotTracePV,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT a power trace?")

        .def("isMonitoring",  isMonitoring_1,(arg("name")),"Is the trace 'name' being monitored?")
        .def("isMonitoring",  isMonitoring_2,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE being monitored?")

        .def("isNotMonitoring",  isNotMonitoring_1,(arg("name")),"Is the trace 'name' NOT being monitored?")
        .def("isNotMonitoring",  isNotMonitoring_2,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT being monitored?")

        .def("isCheckingMask", isCheckingMask_1,(arg("name")),"Is trace 'name' checking against a mask?")
        .def("isCheckingMask", isCheckingMask_2,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE checking against a  mask?")

        .def("isNotCheckingMask", isNotCheckingMask_1,(arg("name")),"Is trace 'name' NOT Checking against a mask?")
        .def("isNotCheckingMask", isNotCheckingMask_2,(arg("LLRF_PV_TYPE")),"Is trace LLRF_PV_TYPE  NOT Checking against a mask?")


        .def("getShotCount", &liberaLLRFController::getShotCount,(arg("name")),"Get Number of traces recieved by this controller for trace 'name'")


        .def("startTraceMonitoring", startTraceMonitoring_1,"Start all LLRF Trace Monitors")
        .def("startTraceMonitoring", startTraceMonitoring_2,(arg("LLRF_PV_TYPE")),"Start Trace Monitoring for LLRF_PV_TYPE")
        .def("startTraceMonitoring", startTraceMonitoring_3,(arg("name")),"Start Trace Monitoring for 'name'")
        .def("startCavFwdTraceMonitor", &liberaLLRFController::startCavFwdTraceMonitor,"Start Cavity Forward Power Trace Monitoring")
        .def("startCavRevTraceMonitor", &liberaLLRFController::startCavRevTraceMonitor,"Start Cavity Reverse Power Trace Monitoring")
        .def("startKlyFwdTraceMonitor", &liberaLLRFController::startKlyFwdTraceMonitor,"Start Klystron Forward Power Trace Monitoring")
        .def("startKlyRevTraceMonitor", &liberaLLRFController::startKlyRevTraceMonitor,"Start Klystron Reverse Power Trace Monitoring")

        .def("stopTraceMonitoring",stopTraceMonitoring_1,"Stop all LLRF Trace Monitors")
        .def("stopTraceMonitoring",stopTraceMonitoring_2,(arg("LLRF_PV_TYPE")),"Stop Trace Monitoring for LLRF_PV_TYPE")
        .def("stopTraceMonitoring",stopTraceMonitoring_3,(arg("name")),"Stop Trace Monitoring for trace 'name'")

        .def("stopCavFwdTraceMonitor", &liberaLLRFController::stopCavFwdTraceMonitor,"Stop Cavity Forward Power Trace Monitoring")
        .def("stopCavRevTraceMonitor", &liberaLLRFController::stopCavRevTraceMonitor,"Stop Cavity Reverse Power Trace Monitoring")
        .def("stopKlyFwdTraceMonitor", &liberaLLRFController::stopKlyFwdTraceMonitor,"Stop Klystron Forward Power Trace Monitoring")
        .def("stopKlyRevTraceMonitor", &liberaLLRFController::stopKlyRevTraceMonitor,"Stop Klystron Reverse Power Trace Monitoring")

        // general methods
        .def("get_CA_PEND_IO_TIMEOUT", &liberaLLRFController::get_CA_PEND_IO_TIMEOUT,"Time before Timeout when getting values")
        .def("set_CA_PEND_IO_TIMEOUT", &liberaLLRFController::set_CA_PEND_IO_TIMEOUT,"Time before Timeout when setting values")
        ;

    // The main class that creates all the controller objects
    class_<VCllrf,boost::noncopyable> ("init")
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
        .def("setQuiet",         &VCllrf::setQuiet)
        .def("setVerbose",       &VCllrf::setVerbose)
        .def("setMessage",       &VCllrf::setMessage)
        .def("setDebugMessage",  &VCllrf::setDebugMessage)

        //.def_readwrite("CLARA_LRRG_LLRF_CONFIG", &VCllrf::CLARA_LRRG_LLRF_CONFIG)
        //.def_readwrite("CLARA_L01_LLRF_CONFIG",  &VCllrf::CLARA_L01_LLRF_CONFIG)
        //.def_readwrite("VELA_HRRG_LLRF_CONFIG",  &VCllrf::VELA_HRRG_LLRF_CONFIG)
        ;
}

#endif // _VCllrf_H
