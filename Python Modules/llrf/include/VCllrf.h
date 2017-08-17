#ifndef _VC_LLRF_CONTROLLER_H_
#define _VC_LLRF_CONTROLLER_H_
//stl
#include <string>
// vela-clara-controllers
#include "liberaLLRFController.h"
#include "llrfStructs.h"
//boost
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
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

        // base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();


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
//



void(liberaLLRFController::*startTraceMonitoring_1)() = &liberaLLRFController::startTraceMonitoring;
bool(liberaLLRFController::*startTraceMonitoring_2)(llrfStructs::LLRF_PV_TYPE pv) = &liberaLLRFController::startTraceMonitoring;
void(liberaLLRFController::*stopTraceMonitoring_1)() = &liberaLLRFController::stopTraceMonitoring;
bool(liberaLLRFController::*stopTraceMonitoring_2)(llrfStructs::LLRF_PV_TYPE pv) = &liberaLLRFController::stopTraceMonitoring;



using namespace boost::python;
using namespace llrfStructs; // !!!!
BOOST_PYTHON_MODULE(VELA_CLARA_LLRF_Control)
{
    // Things that you want to use in python muct be exposed:
    // containers
    class_<std::vector< std::string > >("std_vector_string")
            .def(vector_indexing_suite< std::vector< std::string >>())
            ;
// can't get deque to work in a simple way
//    class_<std::deque< llrfStructs::rf_trace > >("std_deque_rf_trace")
//            .def(vector_indexing_suite< std::deque< llrfStructs::rf_trace >>())
//            ;

    class_<std::vector<double> >("std_vector_double")
            .def(vector_indexing_suite< std::vector<double>>())
            ;
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
        .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD  )
        .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD )
        .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR)
        ;
    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
        ;

    enum_<llrfStructs::LLRF_PV_TYPE>("LLRF_PV_TYPE")
        .value("LIB_LOCK",   LLRF_PV_TYPE::LIB_LOCK  )
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

    // Expose base classes
    class_<baseObject, boost::noncopyable>("baseObject", no_init)
        ;
    // we have to tell boost.python about pure virtual methods in abstract base classes
    class_<controller,bases<baseObject>,boost::noncopyable>
        ("controller", no_init) /// force Python to not construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
        .def("set_CA_PEND_IO_TIMEOUT", pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
        .def("getILockStatesStr",      pure_virtual(&controller::getILockStatesStr)     )
        .def("getILockStates",         pure_virtual(&controller::getILockStates)        )
        ;

    class_<llrfStructs::rf_trace,boost::noncopyable>
        ("rf_trace","rf_trace Doc String", no_init)
        .def_readonly("value",    &rf_trace::value  ,"trace values")
        .def_readonly("time",     &rf_trace::time   ,"Epics TimeStamp ns since epoch (double)")
        .def_readonly("timeStr",  &rf_trace::timeStr,"Epics TimeStamp ns since epoch (string)")
        .def_readonly("shot",     &rf_trace::shot   ,"shot number, (currently number of traces since monitoring started, in future will be timing system shotnumber)")
        ;

    // rf_trace_data object, contains  struct to be exposed, used when returning a liberallrfObject reference
    class_<llrfStructs::rf_trace_data,boost::noncopyable>
        ("rf_trace_data","rf_trace_data object Doc-String", no_init)
        .def_readonly("check_mask",      &rf_trace_data::check_mask,"should check mask")
        .def_readonly("hi_mask_set",     &rf_trace_data::hi_mask_set,"is hi mask set")
        .def_readonly("low_mask_set",    &rf_trace_data::low_mask_set,"is low mask set")
        .def_readonly("keep_rolling_average",&rf_trace_data::keep_rolling_average,"should keep rolling average")
        .def_readonly("buffersize",      &rf_trace_data::buffersize,"number of traces in buffer")
        .def_readonly("trace_size",      &rf_trace_data::trace_size,"number of elements in a trace")
        .def_readonly("average_size",    &rf_trace_data::average_size,"number of traces to average")
        .def_readonly("high_mask",       &rf_trace_data::high_mask,"high mask values")
        .def_readonly("low_mask",        &rf_trace_data::low_mask,"low mask values")
        .def_readonly("rolling_average", &rf_trace_data::rolling_average,"rolling average values")
        .def_readonly("traces",          &rf_trace_data::traces,"all trace data in buffer of rf_trace objects ( stored in c++ as std::deque<llrfStructs::rf_trace> does this work?)")
        ;

    // The map with all the TRACE data (keyed by trace name from conifg file
    class_< std::map<std::string, llrfStructs::rf_trace_data> >("A map of all the rf_trace_data (keyed by trace name from config file)", no_init)
        .def(map_indexing_suite<std::map<std::string,rf_trace_data>>())
        ;

    //typedef &llrfStructs::rf_trace rtr;
    //using namespace llrfStructs;
    // outside_mask_trace is a sturct that holds flagged traces...
    class_<outside_mask_trace>
        ("rf_trace","rf_trace Doc String", no_init)
        .def_readonly("value",     &outside_mask_trace::value,     "trace values")
        .def_readonly("trace_name",&outside_mask_trace::trace_name,"Name of channel trace came from")
        .def_readonly("high_mask", &outside_mask_trace::high_mask, "high_mask values")
        .def_readonly("low_mask",  &outside_mask_trace::low_mask,  "low_mask values")
        .def_readonly("time",      &outside_mask_trace::time,   "Epics TimeStamp: ns since epoch (double)")
        .def_readonly("timeStr",   &outside_mask_trace::timeStr,"Epics TimeStamp: ns since epoch (string)")
        .def_readonly("shot",      &outside_mask_trace::shot,   "shot number, (currently number of traces since monitoring started, in future will be timing system shotnumber)")
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
        .def_readonly("phi_DEG", &liberallrfObject::phi_DEG,"Current Phase in degrees relative to crestPhi.")
        .def_readonly("amp_MVM", &liberallrfObject::amp_MVM,"Current Amplitude in MV/m.")
        .def_readonly("phi_sp",  &liberallrfObject::phi_sp,"Current Libera Phase SP.")
        .def_readonly("phi_ff",  &liberallrfObject::phi_ff,"Current Libera Phase FF.")
        .def_readonly("amp_sp",  &liberallrfObject::amp_sp,"Current Libera Amplitude SP.")
        .def_readonly("amp_ff",  &liberallrfObject::amp_ff,"Current Libera Amplitude FF.")
        .def_readonly("crestPhi",&liberallrfObject::crestPhi,"Crest Phase.")
        .def_readonly("maxAmp",  &liberallrfObject::maxAmp,"Maximum Amplitude Setting.")
        .def_readonly("type",    &liberallrfObject::type,"LLRF Controller Type.")
        .def_readonly("islocked",&liberallrfObject::islocked,"LLRF Amplitude &Phase Lock.")
        .def_readonly("powerTraceLength", &liberallrfObject::powerTraceLength,"Number of elements in a power trace.")
        .def_readonly("trace_data", &liberallrfObject::trace_data,"Map of rf_trace_data objects, keyed by the Trace Name (defined in config file).")
        .def_readonly("time_vector", &liberallrfObject::time_vector,"The time vector, stored in a rf_trace_data object.")
        .def_readonly("outside_mask_traces", &liberallrfObject::outside_mask_traces,"The time vector, stored in a rf_trace_data object.")
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

        .def("getPhiCalibration", &liberaLLRFController::getPhiCalibration,"Return Linear Conversion of Phase from LLRF units to degrees")
        .def("getAmpCalibration", &liberaLLRFController::getAmpCalibration,"Return Linear Conversion of Amplitude from LLRF units to MV/m")
        .def("getCrestPhiLLRF",   &liberaLLRFController::getCrestPhiLLRF,"Return the Crest Phase in LLRF Units")
        .def("getLLRFObjConstRef",&liberaLLRFController::getLLRFObjConstRef,return_value_policy<reference_existing_object>(),(arg("name")),"Return LLRF Object Reference")
        .def("getPhiDEG",  &liberaLLRFController::getPhiDEG,"Get Current Phase relative to Crest [degrees] +ve sense?")
        .def("getPulseLength",  &liberaLLRFController::getPulseLength,"Return RF pulse length [micro-s]")
        .def("getPulseOffset",  &liberaLLRFController::getPulseOffset,"Return RF Pulse Offset [micro-s]")
        .def("isLocked",  &liberaLLRFController::isLocked,"Return true if LLRf Locked")
        .def("getPowerTraceLength",  &liberaLLRFController::getPowerTraceLength,"Return Number of elements in a power trace")

        .def("getChannelNames",&liberaLLRFController::getChannelNames_Py,"Return Channel names (defined in config file)" )
        .def("getTraceData",   &liberaLLRFController::getTraceData,(arg("name")),"Return latest rf_trace object for Channel 'name'")
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



        .def("setPhiLLRF",        &liberaLLRFController::setPhiLLRF,(arg("value")),"Set Phase in LLRF Units")
        .def("setAmpLLLRF",       &liberaLLRFController::setAmpLLRF,(arg("value")),"Set Amplitude in LLRF units")
        .def("setPhiDEG",         &liberaLLRFController::setPhiDEG,(arg("value")),"Set Cavity Phase Relative to Creset [degrees] (+ve sense?)")
        .def("setAmpMVM",         &liberaLLRFController::setAmpMVM,(arg("value")),"Set Cavity Amplitude [MV/m]")
        .def("setPhiSP",  &liberaLLRFController::setPhiSP,(arg("value")),"Set Phase(SP) in LLRF Units")
        .def("setPhiFF",  &liberaLLRFController::setPhiFF,(arg("value")),"Set Phase(FF) in LLRF Units")
        .def("setAmpSP",  &liberaLLRFController::setAmpSP,(arg("value")),"Set Amplitude(SP) in LLRF Units")
        .def("setAmpFF",  &liberaLLRFController::setAmpFF,(arg("value")),"Set Amplitude(FF) in LLRF Units")
        .def("setPulseLength",  &liberaLLRFController::setPulseLength,(arg("value")),"Set RF pulse length [micro-s]")
        .def("setPulseOffset",  &liberaLLRFController::setPulseOffset,(arg("value")),"Set RF pulse offset [micro-sec]")
        .def("setPhiCalibration",  &liberaLLRFController::setPhiCalibration,(arg("value")),"Set linear calibration of phase from LLRF units to degrees")
        .def("setAmpCalibration",  &liberaLLRFController::setAmpCalibration,(arg("value")),"Set linear calibration of amplitude from LLRF units to MV/m")
        .def("setCrestPhiLLRF",  &liberaLLRFController::setCrestPhiLLRF,(arg("value")),"Set the Crest Phi value in LLRF Units")
        .def("setHighMask",  &liberaLLRFController::setHighMask,(arg("name"),arg("value")),"Set the Hi mask for trace 'name'")
        .def("setLowMask",  &liberaLLRFController::setLowMask,(arg("name"),arg("value")),"Set the Lo mask for trace 'name'")
        .def("setNumBufferTraces",  &liberaLLRFController::setNumBufferTraces,(arg("name"),arg("value")),"Set the number of buffer traces to keep for trace 'name'")
        .def("setCheckMask",  &liberaLLRFController::setCheckMask,(arg("name"),arg("value")),"Set whether to check (or not check) new traces against the mask (pass 'name' and true or false)")
        .def("setKeepRollingAverage",  &liberaLLRFController::setKeepRollingAverage,(arg("name"),arg("value")),"Set whetrher to keep a rolling average of previous traces (pass 'name' and true or false)")
        .def("setNumRollingAverageTraces",  &liberaLLRFController::setNumRollingAverageTraces,(arg("name"),arg("value")),"Set the number of traces used for the rolling average")

        .def("Is_TracePV",  &liberaLLRFController::Is_TracePV,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE a power trace?")
        .def("IsNot_TracePV",  &liberaLLRFController::IsNot_TracePV,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT a power trace?")
        .def("isMonitoring",  &liberaLLRFController::isMonitoring,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE being monitored?")
        .def("isNotMonitoring", &liberaLLRFController::isNotMonitoring,(arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT being monitored?")

        .def("startTraceMonitoring", startTraceMonitoring_1,"Start all LLRF Trace Monitors")
        .def("startTraceMonitoring", startTraceMonitoring_2,(arg("LLRF_PV_TYPE")),"Start Trace Monitoring for LLRF_PV_TYPE")
        .def("startCavFwdTraceMonitor", &liberaLLRFController::startCavFwdTraceMonitor,"Start Cavity Forward Power Trace Monitoring")
        .def("startCavRevTraceMonitor", &liberaLLRFController::startCavRevTraceMonitor,"Start Cavity Reverse Power Trace Monitoring")
        .def("startKlyFwdTraceMonitor", &liberaLLRFController::startKlyFwdTraceMonitor,"Start Klystron Forward Power Trace Monitoring")
        .def("startKlyRevTraceMonitor", &liberaLLRFController::startKlyRevTraceMonitor,"Start Klystron Reverse Power Trace Monitoring")
        .def("stopTraceMonitoring", stopTraceMonitoring_1,"Stop all LLRF Trace Monitors")
        .def("stopTraceMonitoring",stopTraceMonitoring_2,(arg("LLRF_PV_TYPE")),"Stop Trace Monitoring for LLRF_PV_TYPE")
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
        .def("killGun",          &VCllrf::killGun)
        .def("setQuiet",         &VCllrf::setQuiet)
        .def("setVerbose",       &VCllrf::setVerbose)
        .def("setMessage",       &VCllrf::setMessage)
        .def("setDebugMessage",  &VCllrf::setDebugMessage)
        ;
}

#endif // _VCllrf_H
