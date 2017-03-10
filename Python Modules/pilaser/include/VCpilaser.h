#ifndef _VCpilaser_H
#define _VCpilaser_H
//stl
#include <string>
//
#include "pilaserController.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/docstring_options.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/manage_new_object.hpp>

class VCpilaser
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix

    public:
        VCpilaser();
        ~VCpilaser();
        pilaserController& virtual_PILaser_Controller();
        pilaserController& physical_PILaser_Controller();
        pilaserController& offline_PILaser_Controller();

        // base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        pilaserController& getpilaserController( VELA_ENUM::MACHINE_MODE mode );

    protected:

    private:

     /// we just need to decide good names for these things...
        pilaserController* virtual_pilaser_Controller_Obj ;
        pilaserController* physical_pilaser_Controller_Obj;
        pilaserController* offline_pilaser_Controller_Obj ;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string pilaserConf;

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
///
//bool(pilaserController::*screenIN_1)  (cstr& ) = &pilaserController::screenIN;

bool(pilaserController::*setHpos_1)(doub) = &pilaserController::setHpos;
bool(pilaserController::*setHpos_2)(int ) = &pilaserController::setHpos;
bool(pilaserController::*setVpos_1)(doub) = &pilaserController::setVpos;
bool(pilaserController::*setVpos_2)(int ) = &pilaserController::setVpos;
bool(pilaserController::*setIntensity_1)(doub) = &pilaserController::setIntensity;
bool(pilaserController::*setIntensity_2)(int) = &pilaserController::setIntensity;

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_PILaserControl )
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();

    /// Things that you want to use in python muct be exposed:
    /// containers
    class_<std::vector< std::string > >("std_vector_string")
            .def( vector_indexing_suite< std::vector< std::string >>() )
            ;
    class_<std::vector< double> >("std_vector_double")
            .def( vector_indexing_suite< std::vector< double>>() )
            ;
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
        .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
        .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
        .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
        ;
    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE","MACHINE_MODE: a named integer giving the")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE  )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL  )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL )
        ;
    // pilaser object struct to be exposed, used when returning a pilaser reference
    boost::python::class_<pilaserStructs::pilaserObject,boost::noncopyable>
        ("pilaserObject","pilaserObject Doc String", boost::python::no_init)
        .def_readonly("name",      &pilaserStructs::pilaserObject::name)
        .def_readonly("pvRoot",    &pilaserStructs::pilaserObject::pvRoot)
        .def_readonly("hPos",      &pilaserStructs::pilaserObject::hPos)
        .def_readonly("vPos",      &pilaserStructs::pilaserObject::vPos)
        .def_readonly("intensity", &pilaserStructs::pilaserObject::intensity)
        .def_readonly("numIlocks", &pilaserStructs::pilaserObject::numIlocks)
        ;
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

    boost::python::class_<pilaserController, boost::python::bases<controller>, boost::noncopyable>
        ("pilaserController","pilaserController Doc String",boost::python::no_init)
        .def("getILockStates",    &pilaserController::getILockStates,
                                  "Return the state of interlocks as an integer. There Currently NO epics ilocks for the PI laser.")
        .def("getILockStatesStr", &pilaserController::getILockStatesStr,
                                  "Return state of interlocks as a stringr. There Currently NO epics ilocks for the PI laser.")
        .def("get_CA_PEND_IO_TIMEOUT",  &pilaserController::get_CA_PEND_IO_TIMEOUT,
                                        "Return the current waiting time [seconds] when sending commands to EPICS.")
        .def("set_CA_PEND_IO_TIMEOUT",  &pilaserController::set_CA_PEND_IO_TIMEOUT,
                                        (boost::python::arg("time"),
                                        "Set a new waiting time [seconds] when sending commands to EPICS"))
        .def("getHpos", &pilaserController::getHpos,
                        "returns the horizontal position of the laser on the cathode [units etc?]." )
        .def("getVpos", &pilaserController::getVpos,
                        "returns the vertical position of the laser on the cathode [units etc?]."   )
        .def("getIntensity", &pilaserController::getIntensity,
                             "returns the intensity the laser [units etc?]."                   )
        .def("getPILObjConstRef",  &pilaserController::getPILObjConstRef,
                                   return_value_policy<reference_existing_object>())
        .def("setHpos",  setHpos_1 )
        .def("setHpos",  setHpos_2 )
        .def("setVpos",  setVpos_1 )
        .def("setVpos",  setVpos_2 )
        .def("setIntensity",  setIntensity_1 )
        .def("setIntensity",  setIntensity_2 )
        ;

    /// The main class that creates all the controller obejcts
        boost::python::class_<VCpilaser,boost::noncopyable> ("init")
        .def("virtual_PILaser_Controller",  &VCpilaser::virtual_PILaser_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_PILaser_Controller",  &VCpilaser::physical_PILaser_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_PILaser_Controller",  &VCpilaser::offline_PILaser_Controller,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VCpilaser::setQuiet )
        .def("setVerbose",       &VCpilaser::setVerbose )
        .def("setMessage",       &VCpilaser::setMessage )
        .def("setDebugMessage",  &VCpilaser::setDebugMessage )
        ;
}

#endif // _VCpilaser_H
