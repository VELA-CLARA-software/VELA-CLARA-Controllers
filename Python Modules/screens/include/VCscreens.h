#ifndef _VCSCREENS_H
#define _VCSCREENS_H
//stl
#include <string>
//
#include "screenController.h"
// stl
#include <string>
#include <vector>
#include <map>
#include <list>
//boost
#define MODULE_NAME VELA_CLARA_Screen_Control

#include "VCheader.h"

//OK i should think about a compelte change in approach here
//
//maybe we should have a screenController base-class that is returned,
//
//but there are different falvours of it?

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

        screenController& virtual_CLARA_INJ_Screen_Controller();
        screenController& offline_CLARA_INJ_Screen_Controller();
        screenController& physical_CLARA_INJ_Screen_Controller();

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

        screenController * virtual_CLARA_INJ_Screen_Controller_Obj;
        screenController * offline_CLARA_INJ_Screen_Controller_Obj;
        screenController * physical_CLARA_INJ_Screen_Controller_Obj;

        screenController * virtual_CLARA_PH1_Screen_Controller_Obj;
        screenController * offline_CLARA_PH1_Screen_Controller_Obj;
        screenController * physical_CLARA_PH1_Screen_Controller_Obj;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

        const VELA_ENUM::MACHINE_AREA  VELA_INJ, VELA_BA1, VELA_BA2, CLARA_INJ, CLARA_PH1, UNKNOWN_AREA;
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
bool(screenController::*screenIN_1)  (cstr& ) = &screenController::screenIN;
bool(screenController::*screenIN_2)  (cves& ) = &screenController::screenIN;
bool(screenController::*screenOUT_1) (cstr& ) = &screenController::screenOUT;
bool(screenController::*screenOUT_2) (cves& ) = &screenController::screenOUT;

bool(screenController::*screenMoveTo_1)(cves&, stav& ) = &screenController::screenMoveTo;
bool(screenController::*screenMoveTo_2)(cstr&, csta& ) = &screenController::screenMoveTo;

bool(screenController::*isScreenIN_1)(cstr& ) = &screenController::isScreenIN;
vecb(screenController::*isScreenIN_2)(cves& ) = &screenController::isScreenIN;
bool(screenController::*isScreenOUT_1)(cstr&) = &screenController::isScreenOUT;
vecb(screenController::*isScreenOUT_2)(cves&) = &screenController::isScreenOUT;



using namespace boost::python;
BOOST_PYTHON_MODULE( MODULE_NAME )
{
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();

    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    /// Things that you want to use in python muct be exposed:
    /// containers
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
        .value("H_YAG",           screenStructs::SCREEN_STATE::H_YAG)
        .value("V_YAG",           screenStructs::SCREEN_STATE::V_YAG)
        .value("H_SLIT",          screenStructs::SCREEN_STATE::H_SLIT)
        .value("V_SLIT",          screenStructs::SCREEN_STATE::V_SLIT)
        .value("H_RF",            screenStructs::SCREEN_STATE::H_RF)
        .value("V_RF",            screenStructs::SCREEN_STATE::V_RF)
        .value("V_OUT",           screenStructs::SCREEN_STATE::V_OUT )
        .value("V_OUT_POS",       screenStructs::SCREEN_STATE::V_OUT_POS )
        .value("H_OUT",           screenStructs::SCREEN_STATE::H_OUT )
        .value("H_SLIT_1",        screenStructs::SCREEN_STATE::H_SLIT_1)
        .value("H_SLIT_2",        screenStructs::SCREEN_STATE::H_SLIT_2)
        .value("H_SLIT_3",        screenStructs::SCREEN_STATE::H_SLIT_3)
        .value("V_SLIT_1",        screenStructs::SCREEN_STATE::V_SLIT_1)
        .value("V_GRATICULE",     screenStructs::SCREEN_STATE::V_GRATICULE)
        .value("V_COLLIMATOR",    screenStructs::SCREEN_STATE::V_COLLIMATOR)
        .value("H_4MM_APERTURE",  screenStructs::SCREEN_STATE::H_4MM_APERTURE)
        .value("H_6MM_APERTURE",  screenStructs::SCREEN_STATE::H_6MM_APERTURE)
        .value("H_9MM_APERTURE",  screenStructs::SCREEN_STATE::H_9MM_APERTURE)
        .value("H_OUT",           screenStructs::SCREEN_STATE::H_OUT )
        .value("UNKNOWN_POSITION",screenStructs::SCREEN_STATE::UNKNOWN_POSITION )
        ;
    enum_<screenStructs::SCREEN_TYPE>("SCREEN_TYPE")
        .value("UNKNOWN_SCREEN_TYPE", screenStructs::SCREEN_TYPE::UNKNOWN_SCREEN_TYPE )
        .value("VELA_HV_MOVER",       screenStructs::SCREEN_TYPE::VELA_HV_MOVER   )
        .value("CLARA_HV_MOVER",      screenStructs::SCREEN_TYPE::CLARA_HV_MOVER  )
        .value("CLARA_V_MOVER",       screenStructs::SCREEN_TYPE::CLARA_V_MOVER   )
        .value("VELA_PNEUMATIC",      screenStructs::SCREEN_TYPE::VELA_PNEUMATIC  )
        .value("CLARA_PNEUMATIC",     screenStructs::SCREEN_TYPE::CLARA_PNEUMATIC )
        ;
    enum_<screenStructs::DRIVER_STATE>("DRIVER_STATE")
        .value("DRIVER_MOVING",        screenStructs::DRIVER_STATE::DRIVER_MOVING )
        .value("DRIVER_STATIONARY",    screenStructs::DRIVER_STATE::DRIVER_STATIONARY )
        .value("DRIVER_DISABLED",      screenStructs::DRIVER_STATE::DRIVER_DISABLED )
        .value("DRIVER_ENABLED",       screenStructs::DRIVER_STATE::DRIVER_ENABLED )
        .value("DRIVER_ERROR",         screenStructs::DRIVER_STATE::DRIVER_ERROR )
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
        .def_readonly("cassetteElementsPos",        &screenStructs::screenCassette::cassetteElementsPos)
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

    boost::python::class_<screenStructs::screenObject,boost::noncopyable>
        ("screenObject", boost::python::no_init)
        .def_readonly("screenType",  &screenStructs::screenObject::screenType)
        .def_readonly("driver",      &screenStructs::screenObject::driver)
        .def_readonly("screenState", &screenStructs::screenObject::screenState)
        .def_readonly("numIlocks",   &screenStructs::screenObject::numIlocks)
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
        .def("getILockStates",             &screenController::getILockStates        )
//        .def("getMagPSUStateDefinition", &magnetController::getMagPSUStateDefinition )
//        .def("getILockStatesDefinition", &magnetController::getILockStatesDefinition )
        .def("get_CA_PEND_IO_TIMEOUT",     &screenController::get_CA_PEND_IO_TIMEOUT   )
        .def("set_CA_PEND_IO_TIMEOUT",     &screenController::set_CA_PEND_IO_TIMEOUT   )
        .def("screenIN",                   screenIN_1  )
        .def("screenIN",                   screenIN_2  )
        .def("screenOUT",                  screenOUT_1 )
        .def("screenOUT",                  screenOUT_2 )
        .def("is_VELA_PNEUMATIC",          &screenController::is_VELA_PNEUMATIC )
        .def("is_VELA_HV_MOVER",           &screenController::is_VELA_HV_MOVER  )
        .def("is_CLARA_PNEUMATIC",         &screenController::is_CLARA_PNEUMATIC )
        .def("is_CLARA_HV_MOVER",          &screenController::is_CLARA_HV_MOVER )
        .def("is_CLARA_V_MOVER",           &screenController::is_CLARA_V_MOVER )
        .def("get_VELA_PNEUMATIC_Screens", &screenController::get_VELA_PNEUMATIC_Screens )
        .def("get_VELA_HV_MOVER_Screens",  &screenController::get_VELA_HV_MOVER_Screens )
        .def("get_CLARA_PNEUMATIC_Screens",&screenController::get_CLARA_PNEUMATIC_Screens )
        .def("get_CLARA_HV_MOVER_Screens", &screenController::get_CLARA_HV_MOVER_Screens )
        .def("get_CLARA_V_MOVER_Screens",  &screenController::get_CLARA_V_MOVER_Screens )
        .def("screenMoveTo",               screenMoveTo_1 )
        .def("screenMoveTo",               screenMoveTo_2 )
        .def("setPosition",                &screenController::setPosition )
        .def("get_H_ACTPOS",               &screenController::get_H_ACTPOS )
        .def("get_V_ACTPOS",               &screenController::get_V_ACTPOS )
        .def("isMoving",                   &screenController::isMoving )
        .def("isNotMoving",                &screenController::isNotMoving )
        .def("is_H_Element",               &screenController::is_H_Element )
        .def("is_V_Element",               &screenController::is_V_Element )
        .def("isScreenOUT",                isScreenOUT_1 )
        .def("isScreenOUT",                isScreenOUT_2 )
        .def("isScreenIN",                 isScreenIN_1  )
        .def("isScreenIN",                 isScreenIN_2  )
        .def("exists_and_isLocked",        &screenController::exists_and_isLocked        )
        .def("exists_and_isNotLocked",     &screenController::exists_and_isNotLocked     )
        .def("getScreenState",             &screenController::getScreenState             )
        .def("isScreenInState",            &screenController::isScreenInState            )
        .def("get_SCREEN_STATE_Definition",&screenController::get_SCREEN_STATE_Definition )
        .def("get_DRIVER_STATE_Definition",&screenController::get_DRIVER_STATE_Definition )
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
