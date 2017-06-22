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

        //liberaLLRFController& getliberaLLRFController( VELA_ENUM::MACHINE_MODE mode );

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
BOOST_PYTHON_MODULE( VELA_CLARA_LLRFControl )
{
    // Things that you want to use in python muct be exposed:
    // containers
    class_<std::vector< std::string > >("std_vector_string")
            .def(vector_indexing_suite< std::vector< std::string >>() )
            ;
    class_<std::vector<double> >("std_vector_double")
            .def(vector_indexing_suite< std::vector<double>>() )
            ;
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
        .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
        .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
        .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
        ;
    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE  )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL  )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL )
        ;
    enum_<llrfStructs::LLRF_PV_TYPE>("LLRF_PV_TYPE")
        .value("LIB_LOCK",   llrfStructs::LLRF_PV_TYPE::LIB_LOCK   )
        .value("LIB_AMP_FF",  llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF  )
        .value("LIB_AMP_SP", llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP )
        .value("LIB_PHI_FF", llrfStructs::LLRF_PV_TYPE::LIB_PHI_FF )
        .value("LIB_PHI_SP", llrfStructs::LLRF_PV_TYPE::LIB_PHI_SP )
        .value("LIB_CAV_FWD", llrfStructs::LLRF_PV_TYPE::LIB_CAV_FWD )
        .value("LIB_CAV_REV", llrfStructs::LLRF_PV_TYPE::LIB_CAV_REV )
        .value("LIB_KLY_FWD", llrfStructs::LLRF_PV_TYPE::LIB_KLY_FWD )
        .value("LIB_KLY_REV", llrfStructs::LLRF_PV_TYPE::LIB_KLY_REV )
        .value("LIB_TIME_VECTOR", llrfStructs::LLRF_PV_TYPE::LIB_TIME_VECTOR )
        .value("LIB_PULSE_LENGTH", llrfStructs::LLRF_PV_TYPE::LIB_PULSE_LENGTH )
        .value("LIB_PULSE_OFFSET", llrfStructs::LLRF_PV_TYPE::LIB_PULSE_OFFSET )
        .value("LIB_PULSE_OFFSET", llrfStructs::LLRF_PV_TYPE::LIB_PULSE_OFFSET )
        .value("AMP_MVM", llrfStructs::LLRF_PV_TYPE::AMP_MVM )
        .value("PHI_DEG", llrfStructs::LLRF_PV_TYPE::PHI_DEG )
        .value("UNKNOWN", llrfStructs::LLRF_PV_TYPE::UNKNOWN )
        ;

    enum_<llrfStructs::LLRF_TYPE>("LLRF_TYPE")
        .value("CLARA_HRRG",  llrfStructs::LLRF_TYPE::CLARA_HRRG  )
        .value("CLARA_LRRG",  llrfStructs::LLRF_TYPE::CLARA_LRRG  )
        .value("VELA_HRRG", llrfStructs::LLRF_TYPE::VELA_HRRG )
        .value("VELA_LRRG", llrfStructs::LLRF_TYPE::VELA_LRRG )
        .value("L01", llrfStructs::LLRF_TYPE::L01 )
        .value("UNKNOWN_TYPE", llrfStructs::LLRF_TYPE::UNKNOWN_TYPE )
        ;
// map example
//    class_<std::map<screenStructs::SCREEN_STATE,bool>>("std_map_SCREEN_STATE_bool")
//        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,bool> >())
//        ;
    // Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    // we have to tell boost.python about pure virtual methods in abstract base classes
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller", boost::python::no_init) /// force Python to not construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;
    // liberallrfObject object struct to be exposed, used when returning a liberallrfObject reference
    boost::python::class_<llrfStructs::liberallrfObject,boost::noncopyable>
        ("pilaserObject","pilaserObject Doc String", boost::python::no_init)
        .def_readonly("name",          &llrfStructs::liberallrfObject::name,"LLRF Object Name")
        .def_readonly("pvRoot",        &llrfStructs::liberallrfObject::pvRoot,"PV root")
        .def_readonly("phiCalibration",&llrfStructs::liberallrfObject::phiCalibration,"Linear Phase in LLRF units to degrees")
        .def_readonly("ampCalibration",&llrfStructs::liberallrfObject::ampCalibration,"Linear Amplitude LLRF units to MV/m")
        .def_readonly("phi_DEG", &llrfStructs::liberallrfObject::phi_DEG,"Current Phase in degrees relative to crestPhi.")
        .def_readonly("amp_MVM", &llrfStructs::liberallrfObject::amp_MVM,"Current Amplitude in MV/m.")
        .def_readonly("phi_sp", &llrfStructs::liberallrfObject::phi_sp,"Current Libera Phase SP.")
        .def_readonly("phi_ff", &llrfStructs::liberallrfObject::phi_ff,"Current Libera Phase FF.")
        .def_readonly("amp_sp", &llrfStructs::liberallrfObject::amp_sp,"Current Libera Amplitude SP.")
        .def_readonly("amp_ff", &llrfStructs::liberallrfObject::amp_ff,"Current Libera Amplitude FF.")
        .def_readonly("crestPhi", &llrfStructs::liberallrfObject::crestPhi,"Crest Phase.")
        .def_readonly("maxAmp", &llrfStructs::liberallrfObject::maxAmp,"Maximum Amplitude Setting.")
        .def_readonly("type", &llrfStructs::liberallrfObject::type,"LLRF Controller Type.")
        .def_readonly("islocked", &llrfStructs::liberallrfObject::islocked,"LLRF Amplitude &Phase Lock.")
        .def_readonly("powerTraceLength", &llrfStructs::liberallrfObject::powerTraceLength,"Number of elements in a power trace.")
        .def_readonly("cav_r_power", &llrfStructs::liberallrfObject::cav_r_power,"Cavity Reverse Power trace.")
        .def_readonly("cav_f_power", &llrfStructs::liberallrfObject::cav_f_power,"Cavity Forward Power trace.")
        .def_readonly("kly_f_power", &llrfStructs::liberallrfObject::kly_f_power,"Klystron Forward Power trace.")
        .def_readonly("kly_r_power", &llrfStructs::liberallrfObject::kly_r_power,"Klystron Reverse Power trace.")
        .def_readonly("time_vector", &llrfStructs::liberallrfObject::time_vector,"Power trace time values.")
        ;

boost::python::class_<liberaLLRFController, boost::python::bases<controller>, boost::noncopyable>
    ("liberaLLRFController","liberaLLRFController Doc String",boost::python::no_init   )
    .def("getILockStates",    &liberaLLRFController::getILockStates )
    .def("getType",           &liberaLLRFController::getType,"Return the Type of LLRF controller.")
    .def("getAmpFF",          &liberaLLRFController::getAmpFF,"Return Amplitude(FF) in LLRF Units")
    .def("getAmpSP",          &liberaLLRFController::getAmpSP,"Return Amplitude(SP) in LLRF Units")
    .def("getAmpLLRF",        &liberaLLRFController::getAmpLLRF,"Return Amplitude in LLRF Units")
    .def("getAmpMVM",         &liberaLLRFController::getAmpMVM,"Return Amplitude in MV/m")
    .def("getPhiFF",          &liberaLLRFController::getPhiFF,"Return the Phase(FF) in LLRF Units")
    .def("getPhiSP",          &liberaLLRFController::getPhiSP,"Return the Phase(SP) in LLRF Units")
    .def("getPhiLLRF",        &liberaLLRFController::getPhiLLRF,"Return the Phase in LLRF Units")
    .def("setPhiLLRF",        &liberaLLRFController::setPhiLLRF,(boost::python::arg("value")),"Set Phase in LLRF Units")
    .def("setAmpLLLRF",       &liberaLLRFController::setAmpLLRF,(boost::python::arg("value")),"Set Amplitude in LLRF units")
    .def("setPhiDEG",         &liberaLLRFController::setPhiDEG,(boost::python::arg("value")),"Set Cavity Phase Relative to Creset [degrees] (+ve sense?)")
    .def("setAmpMVM",         &liberaLLRFController::setAmpMVM,(boost::python::arg("value")),"Set Cavity Amplitude [MV/m]")
    .def("getPhiCalibration", &liberaLLRFController::getPhiCalibration,"Return Linear Conversion of Phase from LLRF units to degrees")
    .def("getAmpCalibration", &liberaLLRFController::getAmpCalibration,"Return Linear Conversion of Amplitude from LLRF units to MV/m")
    .def("getCrestPhiLLRF",   &liberaLLRFController::getCrestPhiLLRF,"Return the Crest Phase in LLRF Units")
    .def("getLLRFObjConstRef",&liberaLLRFController::getLLRFObjConstRef,return_value_policy<reference_existing_object>(),(boost::python::arg("name")),"Return LLRF Object Reference")
    .def("getPhiDEG",  &liberaLLRFController::getPhiDEG,"Get Current Phase relative to Crest [degrees] +ve sense?")
    .def("getPulseLength",  &liberaLLRFController::getPulseLength,"Return RF pulse length [micro-s]")
    .def("getPulseOffset",  &liberaLLRFController::getPulseOffset,"Return RF Pulse Offset [micro-s]")
    .def("isLocked",  &liberaLLRFController::isLocked,"Return true if LLRf Locked")
    .def("getPowerTraceLength",  &liberaLLRFController::getPowerTraceLength,"Return Number of elements in a power trace")
    .def("getCavRevPower",  &liberaLLRFController::getCavRevPower,"Return latest cavity reverse power trace")
    .def("getCavFwdPower",  &liberaLLRFController::getCavFwdPower,"Return latest cavity forward power trace")
    .def("getKlyRevPower",  &liberaLLRFController::getKlyRevPower,"Return latest klystron reverse power trace")
    .def("getKlyFwdPower",  &liberaLLRFController::getKlyFwdPower,"Return latest klystron forward power trace")
    .def("setPhiSP",  &liberaLLRFController::setPhiSP,(boost::python::arg("value")),"Set Phase(SP) in LLRF Units")
    .def("setPhiFF",  &liberaLLRFController::setPhiFF,(boost::python::arg("value")),"Set Phase(FF) in LLRF Units")
    .def("setAmpSP",  &liberaLLRFController::setAmpSP,(boost::python::arg("value")),"Set Amplitude(SP) in LLRF Units")
    .def("setAmpFF",  &liberaLLRFController::setAmpFF,(boost::python::arg("value")),"Set Amplitude(FF) in LLRF Units")
    .def("setPulseLength",  &liberaLLRFController::setPulseLength,(boost::python::arg("value")),"Set RF pulse length [micro-s]")
    .def("setPulseOffset",  &liberaLLRFController::setPulseOffset,(boost::python::arg("value")),"Set RF pulse offset [micro-sec]")
    .def("setPhiCalibration",  &liberaLLRFController::setPhiCalibration,(boost::python::arg("value")),"Set linear calibration of phase from LLRF units to degrees")
    .def("setAmpCalibration",  &liberaLLRFController::setAmpCalibration,(boost::python::arg("value")),"Set linear calibration of ampliutude from LLRF units to MV/m")
    .def("setCrestPhiLLRF",  &liberaLLRFController::setCrestPhiLLRF,(boost::python::arg("value")),"Set the Crest Phi value in LLRF Units")


    .def("Is_TracePV",  &liberaLLRFController::Is_TracePV,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE a power trace?")
    .def("IsNot_TracePV",  &liberaLLRFController::IsNot_TracePV,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT a power trace?")
    .def("isMonitoring",  &liberaLLRFController::isMonitoring,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE being monitored?")
    .def("isNotMonitoring", &liberaLLRFController::isNotMonitoring,(boost::python::arg("LLRF_PV_TYPE")),"Is this LLRF_PV_TYPE NOT being monitored?")
    .def("startTraceMonitoring", startTraceMonitoring_1,"Start all LLRF Trace Monitors")
    .def("startTraceMonitoring", startTraceMonitoring_2,(boost::python::arg("LLRF_PV_TYPE")),"Start Trace Monitoring for LLRF_PV_TYPE")
    .def("startCavFwdTraceMonitor", &liberaLLRFController::startCavFwdTraceMonitor,"Start Cavity Forward Power Trace Monitoring")
    .def("startCavRevTraceMonitor", &liberaLLRFController::startCavRevTraceMonitor,"Start Cavity Reverse Power Trace Monitoring")
    .def("startKlyFwdTraceMonitor", &liberaLLRFController::startKlyFwdTraceMonitor,"Start Klystron Forward Power Trace Monitoring")
    .def("startKlyRevTraceMonitor", &liberaLLRFController::startKlyRevTraceMonitor,"Start Klystron Reverse Power Trace Monitoring")
    .def("startKlyRevTraceMonitor", &liberaLLRFController::startKlyRevTraceMonitor,"Start Klystron Reverse Power Trace Monitoring")

    .def("stopTraceMonitoring", stopTraceMonitoring_1,"Stop all LLRF Trace Monitors")
    .def("startTraceMonitoring",stopTraceMonitoring_2,(boost::python::arg("LLRF_PV_TYPE")),"Start Trace Monitoring for LLRF_PV_TYPE")
    .def("startCavFwdTraceMonitor", &liberaLLRFController::stopCavFwdTraceMonitor,"Stop Cavity Forward Power Trace Monitoring")
    .def("startCavRevTraceMonitor", &liberaLLRFController::stopCavRevTraceMonitor,"Stop Cavity Reverse Power Trace Monitoring")
    .def("startKlyFwdTraceMonitor", &liberaLLRFController::stopKlyFwdTraceMonitor,"Stop Klystron Forward Power Trace Monitoring")
    .def("startKlyRevTraceMonitor", &liberaLLRFController::stopKlyRevTraceMonitor,"Stop Klystron Reverse Power Trace Monitoring")
    .def("startKlyRevTraceMonitor", &liberaLLRFController::stopKlyRevTraceMonitor,"Stop Klystron Reverse Power Trace Monitoring")
    .def("get_CA_PEND_IO_TIMEOUT",  &liberaLLRFController::get_CA_PEND_IO_TIMEOUT,"Time before Timeout when getting values")
    .def("set_CA_PEND_IO_TIMEOUT",  &liberaLLRFController::set_CA_PEND_IO_TIMEOUT,"Time before Timeout when setting values")
    ;

    // The main class that creates all the controller objects
    boost::python::class_<VCllrf,boost::noncopyable> ("init")
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
        .def("killGun",          &VCllrf::killGun )
        .def("setQuiet",         &VCllrf::setQuiet )
        .def("setVerbose",       &VCllrf::setVerbose )
        .def("setMessage",       &VCllrf::setMessage )
        .def("setDebugMessage",  &VCllrf::setDebugMessage )
        ;
}

#endif // _VCllrf_H
