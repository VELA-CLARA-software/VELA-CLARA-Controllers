#ifndef _VCSCREENS_H
#define _VCSCREENS_H
//stl
#include <string>
//
#include "velaINJscreenController.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


OK i should think about a compelte change in approach here

maybe we should have a screenController base-class that is returned,

but there are different falvours of it?



class VCscreens
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix

    public:
        VCscreens();
        ~VCscreens();
        velaINJscreenController& virtual_VELA_INJ_Screen_Controller();
        velaINJscreenController& offline_VELA_INJ_Screen_Controller();
        velaINJscreenController& physical_VELA_INJ_Screen_Controller();

        velaINJscreenController& virtual_VELA_BA1_Screen_Controller();
        velaINJscreenController& offline_VELA_BA1_Screen_Controller();
        velaINJscreenController& physical_VELA_BA1_Screen_Controller();

        velaINJscreenController& virtual_VELA_BA2_Screen_Controller();
        velaINJscreenController& offline_VELA_BA2_Screen_Controller();
        velaINJscreenController& physical_VELA_BA2_Screen_Controller();

        velaINJscreenController& virtual_CLARA_INJ_Screen_Controller();
        velaINJscreenController& offline_CLARA_INJ_Screen_Controller();
        velaINJscreenController& physical_CLARA_INJ_Screen_Controller();

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        velaINJscreenController& getScreenController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

    protected:

    private:

     /// we just need to decide good names for these things...
        velaINJscreenController * virtual_VELA_INJ_Screen_Controller_Obj ;
        velaINJscreenController * physical_VELA_INJ_Screen_Controller_Obj;
        velaINJscreenController * offline_VELA_INJ_Screen_Controller_Obj ;

        velaINJscreenController * virtual_VELA_BA1_Screen_Controller_Obj ;
        velaINJscreenController * physical_VELA_BA1_Screen_Controller_Obj;
        velaINJscreenController * offline_VELA_BA1_Screen_Controller_Obj ;

        velaINJscreenController * virtual_VELA_BA2_Screen_Controller_Obj;
        velaINJscreenController * offline_VELA_BA2_Screen_Controller_Obj;
        velaINJscreenController * physical_VELA_BA2_Screen_Controller_Obj;

        velaINJscreenController * virtual_CLARA_INJ_Screen_Controller_Obj;
        velaINJscreenController * offline_CLARA_INJ_Screen_Controller_Obj;
        velaINJscreenController * physical_CLARA_INJ_Screen_Controller_Obj;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

        const VELA_ENUM::MACHINE_AREA  VELA_INJ,VELA_BA1,VELA_BA2,CLARA_INJ,UNKNOWN_AREA;
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
bool(velaINJscreenController::*screenIN_1)  (cstr& ) = &velaINJscreenController::screenIN;
bool(velaINJscreenController::*screenIN_2)  (cves& ) = &velaINJscreenController::screenIN;
bool(velaINJscreenController::*screenOUT_1) (cstr& ) = &velaINJscreenController::screenOUT;
bool(velaINJscreenController::*screenOUT_2) (cves& ) = &velaINJscreenController::screenOUT;

bool(velaINJscreenController::*screenMoveTo_1)(cves&, stav& ) = &velaINJscreenController::screenMoveTo;
bool(velaINJscreenController::*screenMoveTo_2)(cstr&, csta& ) = &velaINJscreenController::screenMoveTo;

bool(velaINJscreenController::*isScreenIN_1)(cstr& ) = &velaINJscreenController::isScreenIN;
vecb(velaINJscreenController::*isScreenIN_2)(cves& ) = &velaINJscreenController::isScreenIN;
bool(velaINJscreenController::*isScreenOUT_1)(cstr&) = &velaINJscreenController::isScreenOUT;
vecb(velaINJscreenController::*isScreenOUT_2)(cves&) = &velaINJscreenController::isScreenOUT;



using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_ScreenControl )
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
        .value("H_MIRROR",        screenStructs::SCREEN_STATE::H_MIRROR)
        .value("V_MIRROR",        screenStructs::SCREEN_STATE::V_MIRROR)
        .value("H_50U_SLIT",      screenStructs::SCREEN_STATE::H_50U_SLIT)
        .value("V_50U_SLIT",      screenStructs::SCREEN_STATE::V_50U_SLIT)
        .value("H_25U_SLIT",      screenStructs::SCREEN_STATE::H_25U_SLIT)
        .value("V_25U_SLIT",      screenStructs::SCREEN_STATE::V_25U_SLIT)
        .value("H_6p3MM_HOLE",    screenStructs::SCREEN_STATE::H_6p3MM_HOLE)
        .value("V_6p3MM_HOLE",    screenStructs::SCREEN_STATE::V_6p3MM_HOLE)
        .value("H_10MM_HOLE",     screenStructs::SCREEN_STATE::H_10MM_HOLE)
        .value("V_10MM_HOLE",     screenStructs::SCREEN_STATE::V_10MM_HOLE)
        .value("H_YAG",           screenStructs::SCREEN_STATE::H_YAG)
        .value("V_YAG",           screenStructs::SCREEN_STATE::V_YAG)
        .value("H_SLIT",          screenStructs::SCREEN_STATE::H_SLIT)
        .value("V_SLIT",          screenStructs::SCREEN_STATE::V_SLIT)
        .value("H_RF",            screenStructs::SCREEN_STATE::H_RF)
        .value("V_RF",            screenStructs::SCREEN_STATE::V_RF)
        .value("V_OUT",           screenStructs::SCREEN_STATE::V_OUT )
        .value("H_OUT",           screenStructs::SCREEN_STATE::H_OUT )
        .value("UNKNOWN_POSITION",screenStructs::SCREEN_STATE::UNKNOWN_POSITION )
        ;
    enum_<screenStructs::SCREEN_TYPE>("SCREEN_TYPE")
        .value("UNKNOWN_SCREEN_TYPE", screenStructs::SCREEN_TYPE::UNKNOWN_SCREEN_TYPE )
        .value("VELA_HV_MOVER",       screenStructs::SCREEN_TYPE::VELA_HV_MOVER  )
        .value("VELA_PNEUMATIC",      screenStructs::SCREEN_TYPE::VELA_PNEUMATIC )
        ;
    enum_<screenStructs::DRIVER_STATE>("DRIVER_STATE")
        .value("DRIVER_MOVING", screenStructs::DRIVER_STATE::DRIVER_MOVING )
        .value("DRIVER_STATIONARY",       screenStructs::DRIVER_STATE::DRIVER_STATIONARY )
        .value("DRIVER_DISABLED",      screenStructs::DRIVER_STATE::DRIVER_DISABLED )
        .value("DRIVER_ENABLED",      screenStructs::DRIVER_STATE::DRIVER_ENABLED )
        .value("DRIVER_ERROR",      screenStructs::DRIVER_STATE::DRIVER_ERROR )
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
    class_<std::map<screenStructs::SCREEN_STATE,double>>("std_map_SCREEN_STATE_double")
        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,double> >())
        ;
    class_<std::map<std::string,bool>>("std_map_SCREEN_STATE_bool")
        .def(map_indexing_suite< std::map<std::string, bool> >())
        ;

    boost::python::class_<screenStructs::screenDriverStatus,boost::noncopyable>
        ("screenDriverStatus", boost::python::no_init)
        .def_readonly("parentScreen",   &screenStructs::screenDriverStatus::parentScreen)
        .def_readonly("position",       &screenStructs::screenDriverStatus::position)
        .def_readonly("STA",            &screenStructs::screenDriverStatus::STA)
        .def_readonly("STA_bit_label",  &screenStructs::screenDriverStatus::STA_bit_label)
        .def_readonly("STA_bit_map",    &screenStructs::screenDriverStatus::STA_bit_map)
        .def_readonly("dir",            &screenStructs::screenDriverStatus::dir)
        .def_readonly("state",          &screenStructs::screenDriverStatus::state)
        ;
    boost::python::class_<screenStructs::screenCassette,boost::noncopyable>
        ("screenCassette", boost::python::no_init)
        .def_readonly("parentScreen",               &screenStructs::screenCassette::parentScreen)
        .def_readonly("screenState",                &screenStructs::screenCassette::screenState)
        .def_readonly("posTolerance",               &screenStructs::screenCassette::posTolerance)
        .def_readonly("cassetteElements",           &screenStructs::screenCassette::cassetteElements)
        .def_readonly("cassetteElementsPosition",   &screenStructs::screenCassette::cassetteElementsPosition)
        .def_readonly("dir",                        &screenStructs::screenCassette::dir)
        ;
    boost::python::class_<screenStructs::screenDriver,boost::noncopyable>
        ("screenDriver", boost::python::no_init)
        .def_readonly("positionError",  &screenStructs::screenDriver::positionError)
        .def_readonly("homeError",      &screenStructs::screenDriver::homeError)
        .def_readonly("parentScreen",   &screenStructs::screenDriver::parentScreen)
        .def_readonly("hDriverSTA",     &screenStructs::screenDriver::hDriverSTA)
        .def_readonly("vDriverSTA",     &screenStructs::screenDriver::vDriverSTA)
        .def_readonly("hCassette",      &screenStructs::screenDriver::hCassette)
        .def_readonly("vCassette",      &screenStructs::screenDriver::vCassette)
        ;

    boost::python::class_<screenStructs::velaINJscreenObject,boost::noncopyable>
        ("velaINJscreenObject", boost::python::no_init)
        .def_readonly("screenType",  &screenStructs::velaINJscreenObject::screenType)
        .def_readonly("driver",      &screenStructs::velaINJscreenObject::driver)
        .def_readonly("screenState", &screenStructs::velaINJscreenObject::screenState)
        .def_readonly("numIlocks",   &screenStructs::velaINJscreenObject::numIlocks)
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

    boost::python::class_<velaINJscreenController, boost::python::bases<controller>, boost::noncopyable>
        ("velaINJscreenController","velaINJscreenController Doc String",boost::python::no_init)
        .def("getILockStates",           &velaINJscreenController::getILockStates        )
//        .def("getMagPSUStateDefinition", &magnetController::getMagPSUStateDefinition )
//        .def("getILockStatesDefinition", &magnetController::getILockStatesDefinition )
        .def("get_CA_PEND_IO_TIMEOUT",  &velaINJscreenController::get_CA_PEND_IO_TIMEOUT   )
        .def("set_CA_PEND_IO_TIMEOUT",  &velaINJscreenController::set_CA_PEND_IO_TIMEOUT   )
        .def("screenIN",                screenIN_1  )
        .def("screenIN",                screenIN_2  )
        .def("screenOUT",               screenOUT_1 )
        .def("screenOUT",               screenOUT_2 )
        .def("is_VELA_PNEUMATIC",       &velaINJscreenController::is_VELA_PNEUMATIC )
        .def("is_VELA_HV_MOVER",        &velaINJscreenController::is_VELA_HV_MOVER  )
        .def("get_VELA_PNEUMATIC_Screens", &velaINJscreenController::get_VELA_PNEUMATIC_Screens )
        .def("get_VELA_HV_MOVER_Screens",  &velaINJscreenController::get_VELA_HV_MOVER_Screens )
        .def("screenMoveTo",               screenMoveTo_1 )
        .def("screenMoveTo",               screenMoveTo_2 )
        .def("setPosition",                &velaINJscreenController::setPosition )
        .def("isMoving",                   &velaINJscreenController::isMoving )
        .def("isNotMoving",                &velaINJscreenController::isNotMoving )
        .def("is_VELA_PNEUMATIC",          &velaINJscreenController::is_VELA_PNEUMATIC )
        .def("is_VELA_HV_MOVER",           &velaINJscreenController::is_VELA_HV_MOVER )
        .def("is_H_Element",               &velaINJscreenController::is_H_Element )
        .def("is_V_Element",               &velaINJscreenController::is_V_Element )
        .def("isScreenOUT",                isScreenOUT_1 )
        .def("isScreenOUT",                isScreenOUT_2 )
        .def("isScreenIN",                 isScreenIN_1  )
        .def("isScreenIN",                 isScreenIN_2  )
        .def("exists_and_isLocked",        &velaINJscreenController::exists_and_isLocked        )
        .def("exists_and_isNotLocked",     &velaINJscreenController::exists_and_isNotLocked     )
        .def("get_VELA_PNEUMATIC_Screens", &velaINJscreenController::get_VELA_PNEUMATIC_Screens )
        .def("get_VELA_HV_MOVER_Screens",  &velaINJscreenController::get_VELA_HV_MOVER_Screens  )
        .def("getScreenState",             &velaINJscreenController::getScreenState             )
        .def("isScreenInState",            &velaINJscreenController::isScreenInState            )
        .def("get_SCREEN_STATE_Definition",&velaINJscreenController::get_SCREEN_STATE_Definition )
        .def("get_DRIVER_STATE_Definition",&velaINJscreenController::get_DRIVER_STATE_Definition )
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
        .def("virtual_CLARA_INJ_Screen_Controller",  &VCscreens::virtual_CLARA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_Screen_Controller",  &VCscreens::offline_CLARA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_Screen_Controller",  &VCscreens::physical_CLARA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("getvelaINJscreenController",  &VCscreens::getvelaINJscreenController,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VCscreens::setQuiet )
        .def("setVerbose",       &VCscreens::setVerbose )
        .def("setMessage",       &VCscreens::setMessage )
        .def("setDebugMessage",  &VCscreens::setDebugMessage )
        ;
}

#endif // _VCSCREENS_H
