#ifndef _VC_LLRF_CONTROLLER_H_
#define _VC_LLRF_CONTROLLER_H_
//stl
#include <string>
// vela-clara-controllers
#include "llrfController.h"
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
        llrfController& virtual_CLARA_LRRG_LLRF_Controller();
        llrfController& physical_CLARA_LRRG_LLRF_Controller();
        llrfController& offline_CLARA_LRRG_LLRF_Controller();

        llrfController& virtual_CLARA_HRRG_LLRF_Controller();
        llrfController& physical_CLARA_HRRG_LLRF_Controller();
        llrfController& offline_CLARA_HRRG_LLRF_Controller();

        llrfController& virtual_VELA_HRRG_LLRF_Controller();
        llrfController& physical_VELA_HRRG_LLRF_Controller();
        llrfController& offline_VELA_HRRG_LLRF_Controller();

        llrfController& virtual_VELA_LRRG_LLRF_Controller();
        llrfController& physical_VELA_LRRG_LLRF_Controller();
        llrfController& offline_VELA_LRRG_LLRF_Controller();

        llrfController& virtual_L01_LLRF_Controller();
        llrfController& physical_L01_LLRF_Controller();
        llrfController& offline_L01_LLRF_Controller();

        // base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();


        void killGun();


        //llrfController& getllrfController( VELA_ENUM::MACHINE_MODE mode );

    protected:

    private:

     /// we just need to decide good names for these things...
        llrfController* GUN_LLRF_Controller_Obj;// you can only have one of these
        llrfController* L01_LLRF_Controller_Obj;

        llrfController& getController(bool shouldVM, bool shouldEPICS,llrfStructs::LLRF_TYPE llrfType);

        bool isaGUN(llrfStructs::LLRF_TYPE llrfType);

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string CLARA_LRRG_LLRF_GUN_CONFIG,CLARA_HRRG_LLRF_GUN_CONFIG,
                          VELA_LRRG_LLRF_GUN_CONFIG,VELA_HRRG_LLRF_GUN_CONFIG,
                          CLARA_L01_LLRF_CONFIG;

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
//bool(llrfController::*screenIN_1)  (cstr& ) = &llrfController::screenIN;



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
        .value("AMP_R",   llrfStructs::LLRF_PV_TYPE::AMP_R   )
        .value("AMP_W",  llrfStructs::LLRF_PV_TYPE::AMP_W  )
        .value("PHI", llrfStructs::LLRF_PV_TYPE::PHI )
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
    // llrfObject object struct to be exposed, used when returning a llrfObject reference
    boost::python::class_<llrfStructs::llrfObject,boost::noncopyable>
        ("pilaserObject","pilaserObject Doc String", boost::python::no_init)
        .def_readonly("name",      &llrfStructs::llrfObject::name)
        .def_readonly("pvRoot",    &llrfStructs::llrfObject::pvRoot)
        .def_readonly("phiCalibration",      &llrfStructs::llrfObject::phiCalibration)
        .def_readonly("ampCalibration",      &llrfStructs::llrfObject::ampCalibration)
        .def_readonly("phi_DEG", &llrfStructs::llrfObject::phi_DEG)
        .def_readonly("amp_MVM", &llrfStructs::llrfObject::amp_MVM)
        .def_readonly("ampR", &llrfStructs::llrfObject::ampR)
        .def_readonly("ampW", &llrfStructs::llrfObject::ampW)
        .def_readonly("phiLLRF", &llrfStructs::llrfObject::phiLLRF)
        .def_readonly("crestPhi", &llrfStructs::llrfObject::crestPhi)
        .def_readonly("maxAmp", &llrfStructs::llrfObject::maxAmp)
        .def_readonly("type", &llrfStructs::llrfObject::type)
        ;
    boost::python::class_<llrfController, boost::python::bases<controller>, boost::noncopyable>
        ("llrfController","llrfController Doc String",boost::python::no_init   )
        .def("getILockStates",      &llrfController::getILockStates )
        .def("getType",             &llrfController::getType      )
        .def("getAmpRead",          &llrfController::getAmpRead   )
        .def("getAmpWrite",         &llrfController::getAmpWrite  )
        .def("getPhiLLRF",          &llrfController::getPhiLLRF   )
        .def("getPhi",              &llrfController::getPhi       )
        .def("getAmp",              &llrfController::getAmp       )
        .def("setPhiLLRF",          &llrfController::setPhiLLRF   )
        .def("setAmpLLLRF",         &llrfController::setAmpLLLRF  )
        .def("setPhi",              &llrfController::setPhi       )
        .def("setAmp",              &llrfController::setAmp       )
        .def("getPhiCalibration",   &llrfController::getPhiCalibration  )
        .def("getAmpCalibration",   &llrfController::getAmpCalibration  )
        .def("getCrestPhiLLRF",     &llrfController::getCrestPhiLLRF    )
        .def("getLLRFObjConstRef",  &llrfController::getLLRFObjConstRef,
                                    return_value_policy<reference_existing_object>())
        .def("get_CA_PEND_IO_TIMEOUT",  &llrfController::get_CA_PEND_IO_TIMEOUT)
        .def("set_CA_PEND_IO_TIMEOUT",  &llrfController::set_CA_PEND_IO_TIMEOUT)
        ;
    // The main class that creates all the controller obejcts
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
