#ifndef vela_INJ_IMG_Controller_H
#define vela_INJ_IMG_Controller_H

// project
#include "invertedMagnetronGaugeController.h"
// stl
#include <string>
#include <vector>
#include <map>
#include <list>
//boost
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#define BOOST_LIB_DIAGNOSTIC
#include <boost/config.hpp>
#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/object/function.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/docstring_options.hpp>

class VCimg// : public invertedMagnetronGaugeController
{
    public:

        /// we have overloaded constructors to specify config-file location
//        velaINJBeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true, const std::string configFileLocation );
        VCimg();
        ~VCimg();

        ///VELA_INJ
        invertedMagnetronGaugeController & virtual_VELA_INJ_IMG_Controller();
        invertedMagnetronGaugeController & offline_VELA_INJ_IMG_Controller();
        invertedMagnetronGaugeController & physical_VELA_INJ_IMG_Controller();
        ///VELA_BA1
        invertedMagnetronGaugeController & virtual_VELA_BA1_IMG_Controller();
        invertedMagnetronGaugeController & offline_VELA_BA1_IMG_Controller();
        invertedMagnetronGaugeController & physical_VELA_BA1_IMG_Controller();
        ///VELA_BA2
        invertedMagnetronGaugeController & virtual_VELA_BA2_IMG_Controller();
        invertedMagnetronGaugeController & offline_VELA_BA2_IMG_Controller();
        invertedMagnetronGaugeController & physical_VELA_BA2_IMG_Controller();
        ///CLARA_S01
        invertedMagnetronGaugeController & virtual_CLARA_S01_IMG_Controller();
        invertedMagnetronGaugeController & offline_CLARA_S01_IMG_Controller();
        invertedMagnetronGaugeController & physical_CLARA_S01_IMG_Controller();
        ///CLARA_S02
        invertedMagnetronGaugeController & virtual_CLARA_S02_IMG_Controller();
        invertedMagnetronGaugeController & offline_CLARA_S02_IMG_Controller();
        invertedMagnetronGaugeController & physical_CLARA_S02_IMG_Controller();
        ///CLARA_PH1
        invertedMagnetronGaugeController & virtual_CLARA_PH1_IMG_Controller();
        invertedMagnetronGaugeController & offline_CLARA_PH1_IMG_Controller();
        invertedMagnetronGaugeController & physical_CLARA_PH1_IMG_Controller();
        ///C2V
        invertedMagnetronGaugeController & virtual_C2V_IMG_Controller();
        invertedMagnetronGaugeController & offline_C2V_IMG_Controller();
        invertedMagnetronGaugeController & physical_C2V_IMG_Controller();
        invertedMagnetronGaugeController & getIMGController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

    protected:
    private:

        bool withEPICS;
        bool withoutEPICS;
        bool withVM;
        bool withoutVM;
        bool showDebugMessages;
        bool showMessages;
        const VELA_ENUM::MACHINE_AREA VELA_INJ;
        const VELA_ENUM::MACHINE_AREA VELA_BA1;
        const VELA_ENUM::MACHINE_AREA VELA_BA2;
        const VELA_ENUM::MACHINE_AREA CLARA_INJ;
        const VELA_ENUM::MACHINE_AREA CLARA_S01;
        const VELA_ENUM::MACHINE_AREA CLARA_S02;
        const VELA_ENUM::MACHINE_AREA CLARA_PH1;
        const VELA_ENUM::MACHINE_AREA CLARA_2_VELA;
        const VELA_ENUM::MACHINE_AREA UNKNOWN_AREA;

        ///VELA_INJ
        invertedMagnetronGaugeController * virtual_VELA_INJ_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_VELA_INJ_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_VELA_INJ_IMG_Controller_Obj;
        ///VELA_BA1
        invertedMagnetronGaugeController * virtual_VELA_BA1_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_VELA_BA1_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_VELA_BA1_IMG_Controller_Obj;
        ///VELA_BA2
        invertedMagnetronGaugeController * virtual_VELA_BA2_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_VELA_BA2_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_VELA_BA2_IMG_Controller_Obj;
        ///CLARA_S01
        invertedMagnetronGaugeController * virtual_CLARA_S01_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_CLARA_S01_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_CLARA_S01_IMG_Controller_Obj;
        ///CLARA_S02
        invertedMagnetronGaugeController * virtual_CLARA_S02_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_CLARA_S02_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_CLARA_S02_IMG_Controller_Obj;
        ///CLARA_PH1
        invertedMagnetronGaugeController * virtual_CLARA_PH1_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_CLARA_PH1_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_CLARA_PH1_IMG_Controller_Obj;
        ///C2V
        invertedMagnetronGaugeController * virtual_C2V_IMG_Controller_Obj;
        invertedMagnetronGaugeController * offline_C2V_IMG_Controller_Obj;
        invertedMagnetronGaugeController * physical_C2V_IMG_Controller_Obj;
};

//#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!

#define BOOST_LIB_DIAGNOSTIC

/// FUNCTION OVERLOADING, if you have overloaded functions:
/// Create a load of different function pointers and use them in the bindings
/// For examples See magnets...

/// As another example we have overloaded constructors to enable / disable messages
/// and / or debug messages when instantiating. See here
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/exposing.html
/// and beware of: http://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor


using namespace boost::python;

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vvvc_overloads1, openAndWait_Py , 0, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vvvc_overloads2, closeAndWait_Py , 0, 1 );

BOOST_PYTHON_MODULE( VELA_CLARA_IMG_Control )
{
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();

    boost::python::type_info info = boost::python::type_id<std::vector<std::string>>();
    const boost::python::converter::registration* reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_<std::vector<std::string>>("std_vector_string")
            .def(vector_indexing_suite<std::vector<std::string>>())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_<std::vector<std::string>>("std_vector_string")
            .def(vector_indexing_suite<std::vector<std::string>>())
            ;
    }

    info = boost::python::type_id<std::vector<double> >();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_<std::vector<double> >("std_vector_double")
            .def(vector_indexing_suite< std::vector<double>>())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_<std::vector<double> >("std_vector_double")
            .def(vector_indexing_suite< std::vector<double>>())
            ;
    }
    info = boost::python::type_id<VELA_ENUM::MACHINE_MODE>();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
        ;
    } else if ((*reg).m_to_python == NULL) {
        enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
        ;
    }

    info = boost::python::type_id<VELA_ENUM::MACHINE_AREA>();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
    enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA","MACHINE_AREA Doc String")
        .value("VELA_INJ",     VELA_ENUM::MACHINE_AREA::VELA_INJ)
        .value("VELA_BA1",     VELA_ENUM::MACHINE_AREA::VELA_BA1)
        .value("VELA_BA2",     VELA_ENUM::MACHINE_AREA::VELA_BA2)
        .value("CLARA_INJ",    VELA_ENUM::MACHINE_AREA::CLARA_INJ)
        .value("CLARA_PH1",    VELA_ENUM::MACHINE_AREA::CLARA_PH1)
        .value("CLARA_2_VELA", VELA_ENUM::MACHINE_AREA::CLARA_2_VELA)
        .value("CLARA_S01",    VELA_ENUM::MACHINE_AREA::CLARA_S01)
        .value("CLARA_S02",    VELA_ENUM::MACHINE_AREA::CLARA_S02)
        .value("CLARA_L01",    VELA_ENUM::MACHINE_AREA::CLARA_L01)
        .value("USER",         VELA_ENUM::MACHINE_AREA::USER)
        .value("UNKNOWN_AREA", VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA)
        ;
    } else if ((*reg).m_to_python == NULL) {
    enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA","MACHINE_AREA Doc String")
        .value("VELA_INJ",     VELA_ENUM::MACHINE_AREA::VELA_INJ)
        .value("VELA_BA1",     VELA_ENUM::MACHINE_AREA::VELA_BA1)
        .value("VELA_BA2",     VELA_ENUM::MACHINE_AREA::VELA_BA2)
        .value("CLARA_INJ",    VELA_ENUM::MACHINE_AREA::CLARA_INJ)
        .value("CLARA_PH1",    VELA_ENUM::MACHINE_AREA::CLARA_PH1)
        .value("CLARA_2_VELA", VELA_ENUM::MACHINE_AREA::CLARA_2_VELA)
        .value("CLARA_S01",    VELA_ENUM::MACHINE_AREA::CLARA_S01)
        .value("CLARA_S02",    VELA_ENUM::MACHINE_AREA::CLARA_S02)
        .value("CLARA_L01",    VELA_ENUM::MACHINE_AREA::CLARA_L01)
        .value("USER",         VELA_ENUM::MACHINE_AREA::USER)
        .value("UNKNOWN_AREA", VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA)
        ;
    }

//    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
//            .value("OFFLINE",   VELA_ENUM::MACHINE_MODE::OFFLINE  )
//            .value("VIRTUAL",   VELA_ENUM::MACHINE_MODE::VIRTUAL  )
//            .value("PHYSICAL",  VELA_ENUM::MACHINE_MODE::PHYSICAL )
//            ;
//
//    enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA")
//            .value("VELA_INJ",  VELA_ENUM::MACHINE_AREA::VELA_INJ  )
//            .value("VELA_BA1",  VELA_ENUM::MACHINE_AREA::VELA_BA1  )
//            .value("VELA_BA2",  VELA_ENUM::MACHINE_AREA::VELA_BA2  )
//            .value("CLARA_INJ", VELA_ENUM::MACHINE_AREA::CLARA_INJ )
//            .value("CLARA_S01", VELA_ENUM::MACHINE_AREA::CLARA_S01 )
//            .value("CLARA_S02", VELA_ENUM::MACHINE_AREA::CLARA_S02 )
//            .value("CLARA_2_VELA",       VELA_ENUM::MACHINE_AREA::CLARA_2_VELA )
//            ;

    //typedef &llrfStructs::rf_trace rtr;
    //using namespace llrfStructs;
    // outside_mask_trace is a sturct that holds flagged traces...
    class_<invertedMagnetronGaugeStructs::vacImgObject>
        ("vacImgObject","vacImgObject Doc String", no_init)
        .def_readonly("name",          &invertedMagnetronGaugeStructs::vacImgObject::name   ,  "IMG name")
        .def_readonly("pvRoot",        &invertedMagnetronGaugeStructs::vacImgObject::pvRoot,"IMG pvRoot")
        .def_readonly("p",             &invertedMagnetronGaugeStructs::vacImgObject::p, "pressure")
        .def_readonly("vacImgState",   &invertedMagnetronGaugeStructs::vacImgObject::vacImgState,  "numIlocks")
        ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;

    /// we have to tell boost.python about pure virtual methods in abstract base classes
    char const* getVacValveStateString = "Returns (as a VELA_ENUM) the state of the vacuum valve - open, closed, moving, or error.\n";


    boost::python::class_<controller, boost::python::bases<baseObject>,  boost::noncopyable> ("controller", boost::python::no_init)
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
		;
    /// member functiosn to expose to python, remmeber to include enum deifntions as boost::python::dict <int, string>


	boost::python::class_<invertedMagnetronGaugeController, boost::python::bases<controller>, boost::noncopyable>
            ("invertedMagnetronGaugeController","This class contains all the functions in the vacuum valve controller for monitoring and controlling PVs",boost::python::no_init)
//            .def("getVacValveStateDefinition",      &invertedMagnetronGaugeController::getVacValveStateDefinition  )
            .def("getILockStatesDefinition",        &invertedMagnetronGaugeController::getILockStatesDefinition    )
            .def("getIMGObjConstRef",               &invertedMagnetronGaugeController::getIMGObjConstRef,return_value_policy<reference_existing_object>() )
            .def("getImgP",                         &invertedMagnetronGaugeController::getImgP            )
            .def("getImgState",                     &invertedMagnetronGaugeController::getImgState)
            .def("getImgStateStr",                  &invertedMagnetronGaugeController::getImgStateStr)
		;

    boost::python::class_<VCimg,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_IMG_Controller",   &VCimg::virtual_VELA_INJ_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_IMG_Controller",   &VCimg::offline_VELA_INJ_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_IMG_Controller",  &VCimg::physical_VELA_INJ_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_IMG_Controller",   &VCimg::virtual_VELA_BA1_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_IMG_Controller",   &VCimg::offline_VELA_BA1_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_IMG_Controller",  &VCimg::physical_VELA_BA1_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_IMG_Controller",   &VCimg::virtual_VELA_BA2_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_IMG_Controller",   &VCimg::offline_VELA_BA2_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_IMG_Controller",  &VCimg::physical_VELA_BA2_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_S01_IMG_Controller",  &VCimg::virtual_CLARA_S01_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_S01_IMG_Controller",  &VCimg::offline_CLARA_S01_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_S01_IMG_Controller", &VCimg::physical_CLARA_S01_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_S02_IMG_Controller",  &VCimg::virtual_CLARA_S02_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_S02_IMG_Controller",  &VCimg::offline_CLARA_S02_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_S02_IMG_Controller", &VCimg::physical_CLARA_S02_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_PH1_IMG_Controller",  &VCimg::virtual_CLARA_PH1_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_IMG_Controller",  &VCimg::offline_CLARA_PH1_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_IMG_Controller", &VCimg::physical_CLARA_PH1_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_C2V_IMG_Controller",        &VCimg::virtual_C2V_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("offline_C2V_IMG_Controller",        &VCimg::offline_C2V_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("physical_C2V_IMG_Controller",       &VCimg::offline_C2V_IMG_Controller, return_value_policy<reference_existing_object>())
        .def("getIMGController",                  &VCimg::getIMGController, return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VCimg::setQuiet )
        .def("setVerbose",       &VCimg::setVerbose )
        .def("setMessage",       &VCimg::setMessage )
        .def("setDebugMessage",  &VCimg::setDebugMessage )
        ;
}

//#endif // BUILD_DLL

#endif // velaChrageScopeController_H

