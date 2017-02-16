#ifndef _VClaser_H
#define _VClaser_H
//stl
#include <string>
//
#include "laserController.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


class VClaser
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix

    public:
        VClaser();
        ~VClaser();
        laserController& virtual_Laser_Controller();
        laserController& physical_Laser_Controller();
        laserController& offline_Laser_Controller();

        // base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        laserController& getlaserController( VELA_ENUM::MACHINE_MODE mode );

    protected:

    private:

     /// we just need to decide good names for these things...
        laserController * virtual_Laser_Controller_Obj ;
        laserController * physical_Laser_Controller_Obj;
        laserController * offline_Laser_Controller_Obj ;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string laserConf;

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
//bool(laserController::*screenIN_1)  (cstr& ) = &laserController::screenIN;



using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_LaserControl )
{
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
    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE  )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL  )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL )
        ;
// map example
//    class_<std::map<screenStructs::SCREEN_STATE,bool>>("std_map_SCREEN_STATE_bool")
//        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,bool> >())
//        ;
    /// Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    /// we have to tell boost.python about pure virtual methods in abstract base classes
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller", boost::python::no_init) /// force Python to not construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;

    boost::python::class_<laserController, boost::python::bases<controller>, boost::noncopyable>
        ("laserController","laserController Doc String",boost::python::no_init)
        .def("getILockStates",           &laserController::getILockStates        )
        .def("get_CA_PEND_IO_TIMEOUT",  &laserController::get_CA_PEND_IO_TIMEOUT   )
        .def("set_CA_PEND_IO_TIMEOUT",  &laserController::set_CA_PEND_IO_TIMEOUT   )
        ;

    /// The main class that creates all the controller obejcts
        boost::python::class_<VClaser,boost::noncopyable> ("init")
        .def("virtual_Laser_Controller",  &VClaser::virtual_Laser_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_Laser_Controller",  &VClaser::physical_Laser_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_Laser_Controller",  &VClaser::offline_Laser_Controller,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VClaser::setQuiet )
        .def("setVerbose",       &VClaser::setVerbose )
        .def("setMessage",       &VClaser::setMessage )
        .def("setDebugMessage",  &VClaser::setDebugMessage )
        ;
}

#endif // _VClaser_H
