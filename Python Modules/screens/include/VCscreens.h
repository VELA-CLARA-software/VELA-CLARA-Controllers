#ifndef _VCSCREENS_H
#define _VCSCREENS_H
//stl
#include <string>
//
#include "screenController.h"

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

class VCscreens
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix

    public:
        VCscreens();
        ~VCscreens();
        screenController& virtual_VELA_INJ_Screen_Controller();
        screenController& offline_VELA_INJ_Screen_Controller();
        screenController& physical_VELA_INJ_Screen_Controller();

        screenController& virtual_VELA_BA1_Screen_Controller();
        screenController& offline_VELA_BA1_Screen_Controller();
        screenController& physical_VELA_BA1_Screen_Controller();

        screenController& virtual_VELA_BA2_Screen_Controller();
        screenController& offline_VELA_BA2_Screen_Controller();
        screenController& physical_VELA_BA2_Screen_Controller();

        screenController& virtual_CLARA_PH1_Screen_Controller();
        screenController& offline_CLARA_PH1_Screen_Controller();
        screenController& physical_CLARA_PH1_Screen_Controller();

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        screenController& getScreenController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

    protected:

    private:

     /// we just need to decide good names for these things...
        screenController * virtual_VELA_INJ_Screen_Controller_Obj ;
        screenController * physical_VELA_INJ_Screen_Controller_Obj;
        screenController * offline_VELA_INJ_Screen_Controller_Obj ;

        screenController * virtual_VELA_BA1_Screen_Controller_Obj ;
        screenController * physical_VELA_BA1_Screen_Controller_Obj;
        screenController * offline_VELA_BA1_Screen_Controller_Obj ;

        screenController * virtual_VELA_BA2_Screen_Controller_Obj;
        screenController * offline_VELA_BA2_Screen_Controller_Obj;
        screenController * physical_VELA_BA2_Screen_Controller_Obj;

        screenController * virtual_CLARA_PH1_Screen_Controller_Obj;
        screenController * offline_CLARA_PH1_Screen_Controller_Obj;
        screenController * physical_CLARA_PH1_Screen_Controller_Obj;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

        const VELA_ENUM::MACHINE_AREA  VELA_INJ, VELA_BA1, VELA_BA2, CLARA_PH1, UNKNOWN_AREA;
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
typedef const screenStructs::SCREEN_STATE csta;
typedef const std::vector< screenStructs::SCREEN_STATE > stav;

typedef std::vector<bool> vecb;
///
//bool(screenController::*screenIN_1)  (cstr& ) = &screenController::screenIN;
//bool(screenController::*screenIN_2)  (cves& ) = &screenController::screenIN;
//bool(screenController::*screenOUT_1) (cstr& ) = &screenController::screenOUT;
//bool(screenController::*screenOUT_2) (cves& ) = &screenController::screenOUT;
//
//bool(screenController::*screenMoveTo_1)(cves&, stav& ) = &screenController::screenMoveTo;
//bool(screenController::*screenMoveTo_2)(cstr&, csta& ) = &screenController::screenMoveTo;
//
//bool(screenController::*isScreenIN_1)(cstr& ) = &screenController::isScreenIN;
//vecb(screenController::*isScreenIN_2)(cves& ) = &screenController::isScreenIN;
//bool(screenController::*isScreenOUT_1)(cstr&) = &screenController::isScreenOUT;
//vecb(screenController::*isScreenOUT_2)(cves&) = &screenController::isScreenOUT;



using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Screen_Control )
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
    enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA")
        .value("VELA_INJ",     VELA_ENUM::MACHINE_AREA::VELA_INJ )
        .value("VELA_BA1",     VELA_ENUM::MACHINE_AREA::VELA_BA1 )
        .value("VELA_BA2",     VELA_ENUM::MACHINE_AREA::VELA_BA2 )
        .value("CLARA_INJ",    VELA_ENUM::MACHINE_AREA::CLARA_INJ)
        .value("CLARA_2_VELA", VELA_ENUM::MACHINE_AREA::CLARA_2_VELA)
        .value("UNKNOWN_AREA", VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA)
        ;
    // screen structs
    enum_<screenStructs::SCREEN_STATE>("SCREEN_STATE")
        .value("SCREEN_IN",       screenStructs::SCREEN_STATE::SCREEN_IN  )
        .value("SCREEN_OUT",      screenStructs::SCREEN_STATE::SCREEN_OUT )
        .value("SCREEN_MOVING",   screenStructs::SCREEN_STATE::SCREEN_MOVING )
        .value("SCREEN_ERROR",    screenStructs::SCREEN_STATE::SCREEN_ERROR  )
        .value("SCREEN_UNKNOWN",  screenStructs::SCREEN_STATE::SCREEN_UNKNOWN)
        .value("H_RETRACTED",     screenStructs::SCREEN_STATE::H_RETRACTED)
        .value("H_SLIT_1",        screenStructs::SCREEN_STATE::H_SLIT_1)
        .value("H_SLIT_2",        screenStructs::SCREEN_STATE::H_SLIT_2)
        .value("H_SLIT_3",        screenStructs::SCREEN_STATE::H_SLIT_3)
        .value("H_APT_1",         screenStructs::SCREEN_STATE::H_APT_1)
        .value("H_APT_2",         screenStructs::SCREEN_STATE::H_APT_2)
        .value("H_APT_3",         screenStructs::SCREEN_STATE::H_APT_3)
        .value("V_RETRACTED",     screenStructs::SCREEN_STATE::V_RETRACTED)
        .value("V_SLIT_1",        screenStructs::SCREEN_STATE::V_SLIT_1)
        .value("V_MAX",           screenStructs::SCREEN_STATE::V_MAX)
        .value("V_RF",            screenStructs::SCREEN_STATE::V_RF)
        .value("V_MIRROR",        screenStructs::SCREEN_STATE::V_MIRROR)
        .value("V_YAG",           screenStructs::SCREEN_STATE::V_YAG)
        .value("V_GRAT",          screenStructs::SCREEN_STATE::V_GRAT )
        .value("V_COL",           screenStructs::SCREEN_STATE::V_COL )
        .value("UNKNOWN_POSITION",screenStructs::SCREEN_STATE::UNKNOWN_POSITION )
        ;
    enum_<screenStructs::SCREEN_TYPE>("SCREEN_TYPE")
        .value("UNKNOWN_SCREEN_TYPE", screenStructs::SCREEN_TYPE::UNKNOWN_SCREEN_TYPE )
        .value("VELA_HV_MOVER",       screenStructs::SCREEN_TYPE::VELA_HV_MOVER       )
        .value("CLARA_HV_MOVER",      screenStructs::SCREEN_TYPE::CLARA_HV_MOVER      )
        .value("CLARA_V_MOVER",       screenStructs::SCREEN_TYPE::CLARA_V_MOVER       )
        .value("VELA_PNEUMATIC",      screenStructs::SCREEN_TYPE::VELA_PNEUMATIC      )
        .value("CLARA_PNEUMATIC",     screenStructs::SCREEN_TYPE::CLARA_PNEUMATIC     )
        ;
    enum_<screenStructs::DRIVER_STATE>("DRIVER_STATE")
        .value("H_DRIVER_MOVING",      screenStructs::DRIVER_STATE::H_DRIVER_MOVING      )
        .value("H_DRIVER_STATIONARY",  screenStructs::DRIVER_STATE::H_DRIVER_STATIONARY  )
        .value("H_DRIVER_DISABLED",    screenStructs::DRIVER_STATE::H_DRIVER_DISABLED    )
        .value("H_DRIVER_ENABLED",     screenStructs::DRIVER_STATE::H_DRIVER_ENABLED     )
        .value("H_DRIVER_ERROR",       screenStructs::DRIVER_STATE::H_DRIVER_ERROR       )
        .value("V_DRIVER_MOVING",      screenStructs::DRIVER_STATE::V_DRIVER_MOVING      )
        .value("V_DRIVER_STATIONARY",  screenStructs::DRIVER_STATE::V_DRIVER_STATIONARY  )
        .value("V_DRIVER_DISABLED",    screenStructs::DRIVER_STATE::V_DRIVER_DISABLED    )
        .value("V_DRIVER_ENABLED",     screenStructs::DRIVER_STATE::V_DRIVER_ENABLED     )
        .value("V_DRIVER_ERROR",       screenStructs::DRIVER_STATE::V_DRIVER_ERROR       )
        .value("UNKNOWN_DRIVER_STATE", screenStructs::DRIVER_STATE::UNKNOWN_DRIVER_STATE )
        ;
    enum_<screenStructs::DRIVER_DIRECTION>("DRIVER_DIRECTION")
        .value("HORIZONTAL",  screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        .value("VERTICAL",    screenStructs::DRIVER_DIRECTION::VERTICAL )
        .value("NONE",        screenStructs::DRIVER_DIRECTION::NONE )
        ;
    // maps foudn in objects

    class_<std::map<screenStructs::SCREEN_STATE,bool>>("std_map_SCREEN_STATE_bool")
        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,bool> >())
        ;
    class_<std::map<screenStructs::SCREEN_STATE,unsigned short>>("std_map_SCREEN_STATE_unsigned_short")
        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,unsigned short> >())
        ;
    class_<std::map<screenStructs::SCREEN_STATE,screenStructs::DRIVER_DIRECTION>>("std_map_SCREEN_STATE_DRIVER_DIRECTION")
        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,screenStructs::DRIVER_DIRECTION> >())
        ;
    class_<std::map<std::string,bool>>("std_map_string_bool")
        .def(map_indexing_suite< std::map<std::string, bool> >())
        ;

    boost::python::class_<screenStructs::screenObject,boost::noncopyable>
        ("screenObject", boost::python::no_init)
        .def_readonly("screenType",   &screenStructs::screenObject::screenType)
        .def_readonly("screenHState", &screenStructs::screenObject::screenHState)
        .def_readonly("screenVState", &screenStructs::screenObject::screenVState)
        .def_readonly("numIlocks",    &screenStructs::screenObject::numIlocks)
        ;


    /// Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    /// we have to tell boost.python about pure virtual methods in abstract base classes
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;

    boost::python::class_<screenController, boost::python::bases<controller>, boost::noncopyable>
        ("screenController","screenController Doc String",boost::python::no_init)
        .def("getILockStates",           &screenController::getILockStates        )
        .def("isHOut",                          &screenController::isHOut   )
        .def("isVOut",                          &screenController::isVOut   )
        .def("is_HandV_OUT",                    &screenController::is_HandV_OUT   )
        .def("isHIn",                           &screenController::isHIn   )
        .def("isVIn",                           &screenController::isVIn   )
        .def("isScreenIn",                      &screenController::isScreenIn   )
        .def("moveScreenTo",                    &screenController::moveScreenTo   )
        .def("setScreenSDEV",                   &screenController::setScreenSDEV   )
        .def("setScreenTrigger",                &screenController::setScreenTrigger   )
        .def("getScreenState",                  &screenController::getScreenState   )
        .def("isScreenInState",                 &screenController::isScreenInState   )
        .def("isYAGIn",                         &screenController::isYAGIn   )
        .def("isHEnabled",                      &screenController::isHEnabled   )
        .def("isVEnabled",                      &screenController::isVEnabled   )
        .def("isHElement",                      &screenController::isHElement   )
        .def("isVElement",                      &screenController::isVElement   )
        .def("getACTPOS",                       &screenController::getACTPOS   )
        .def("jogScreen",                       &screenController::jogScreen  )
        .def("resetPosition",                   &screenController::resetPosition   )
//        .def("isScreenInPosition",              &screenController::isScreenInPosition   )
//        .def("isScreenOUT",                     &screenController::isScreenOUT   )
//        .def("isScreenIN",                      &screenController::isScreenIN   )
//        .def("is_H_Element",                    &screenController::is_H_Element   )
//        .def("is_V_Element",                    &screenController::is_V_Element   )
//        .def("screenIN",                screenIN_1  )
//        .def("screenIN",                screenIN_2  )
//        .def("screenOUT",               screenOUT_1 )
//        .def("screenOUT",               screenOUT_2 )
//        .def("is_VELA_PNEUMATIC",       &screenController::is_VELA_PNEUMATIC )
//        .def("is_VELA_HV_MOVER",        &screenController::is_VELA_HV_MOVER  )
//        .def("get_VELA_PNEUMATIC_Screens", &screenController::get_VELA_PNEUMATIC_Screens )
//        .def("get_VELA_HV_MOVER_Screens",  &screenController::get_VELA_HV_MOVER_Screens )
//        .def("screenMoveTo",               screenMoveTo_1 )
//        .def("screenMoveTo",               screenMoveTo_2 )
//        .def("setPosition",                &screenController::setPosition )
//        .def("isMoving",                   &screenController::isMoving )
//        .def("isNotMoving",                &screenController::isNotMoving )
//        .def("is_VELA_PNEUMATIC",          &screenController::is_VELA_PNEUMATIC )
//        .def("is_VELA_HV_MOVER",           &screenController::is_VELA_HV_MOVER )
//        .def("isScreenOUT",                isScreenOUT_1 )
//        .def("isScreenOUT",                isScreenOUT_2 )
//        .def("isScreenIN",                 isScreenIN_1  )
//        .def("isScreenIN",                 isScreenIN_2  )
//        .def("exists_and_isLocked",        &screenController::exists_and_isLocked        )
//        .def("exists_and_isNotLocked",     &screenController::exists_and_isNotLocked     )
//        .def("get_VELA_PNEUMATIC_Screens", &screenController::get_VELA_PNEUMATIC_Screens )
//        .def("get_VELA_HV_MOVER_Screens",  &screenController::get_VELA_HV_MOVER_Screens  )
//        .def("getScreenState",             &screenController::getScreenState             )
//        .def("isScreenInState",            &screenController::isScreenInState            )
//        .def("get_SCREEN_STATE_Definition",&screenController::get_SCREEN_STATE_Definition )
//        .def("get_DRIVER_STATE_Definition",&screenController::get_DRIVER_STATE_Definition )
        ;

    /// The main class that creates all the controller obejcts
        boost::python::class_<VCscreens,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Screen_Controller",  &VCscreens::virtual_VELA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Screen_Controller",  &VCscreens::offline_VELA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Screen_Controller",  &VCscreens::physical_VELA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Screen_Controller",  &VCscreens::virtual_VELA_BA1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Screen_Controller",  &VCscreens::offline_VELA_BA1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Screen_Controller",  &VCscreens::physical_VELA_BA1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Screen_Controller",  &VCscreens::virtual_VELA_BA2_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Screen_Controller",  &VCscreens::offline_VELA_BA2_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Screen_Controller",  &VCscreens::physical_VELA_BA2_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_PH1_Screen_Controller",  &VCscreens::virtual_CLARA_PH1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_Screen_Controller",  &VCscreens::offline_CLARA_PH1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_Screen_Controller",  &VCscreens::physical_CLARA_PH1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("getScreenController",  &VCscreens::getScreenController,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VCscreens::setQuiet )
        .def("setVerbose",       &VCscreens::setVerbose )
        .def("setMessage",       &VCscreens::setMessage )
        .def("setDebugMessage",  &VCscreens::setDebugMessage )
        ;
}

#endif // _VCSCREENS_H
