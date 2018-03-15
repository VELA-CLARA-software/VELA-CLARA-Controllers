#ifndef _VCSCREENS_H
#define _VCSCREENS_H
//stl
#include <string>
//
#include "screenController.h"
#include "VCheader.h"

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
        .value("RETRACTED",       screenStructs::SCREEN_STATE::YAG )
        .value("YAG",             screenStructs::SCREEN_STATE::RETRACTED )
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
        .value("P_DRIVER_MOVING",      screenStructs::DRIVER_STATE::P_DRIVER_MOVING      )
        .value("P_DRIVER_STATIONARY",  screenStructs::DRIVER_STATE::P_DRIVER_STATIONARY  )
        .value("P_DRIVER_DISABLED",    screenStructs::DRIVER_STATE::P_DRIVER_DISABLED    )
        .value("P_DRIVER_ENABLED",     screenStructs::DRIVER_STATE::P_DRIVER_ENABLED     )
        .value("P_DRIVER_ERROR",       screenStructs::DRIVER_STATE::P_DRIVER_ERROR       )
        .value("UNKNOWN_DRIVER_STATE", screenStructs::DRIVER_STATE::UNKNOWN_DRIVER_STATE )
        ;
    enum_<screenStructs::DRIVER_DIRECTION>("DRIVER_DIRECTION")
        .value("HORIZONTAL",  screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        .value("VERTICAL",    screenStructs::DRIVER_DIRECTION::VERTICAL   )
        .value("PNEUMATIC",   screenStructs::DRIVER_DIRECTION::PNEUMATIC  )
        .value("NONE",        screenStructs::DRIVER_DIRECTION::NONE       )
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
    char const* screenObjectString = "This struct contains the screen object - inc. type of screen (HV mover, pneumatic etc.), and horizontal and vertical state.\n"
                                     "It also gives the names of all the devices (positions) for a given screen.";
    boost::python::class_<screenStructs::screenObject,boost::noncopyable>
        ("screenObject",screenObjectString, boost::python::no_init)
        .def_readonly("screenType",       &screenStructs::screenObject::screenType      )
        .def_readonly("screenHState",     &screenStructs::screenObject::screenHState    )
        .def_readonly("screenVState",     &screenStructs::screenObject::screenVState    )
        .def_readonly("screenVState",     &screenStructs::screenObject::screenPState    )
        .def_readonly("screenSetHState",  &screenStructs::screenObject::screenSetHState )
        .def_readonly("screenSetVState",  &screenStructs::screenObject::screenSetVState )
        .def_readonly("screenSetVState",  &screenStructs::screenObject::screenSetPState )
        .def_readonly("devCentH",         &screenStructs::screenObject::devCentH        )
        .def_readonly("devCentV",         &screenStructs::screenObject::devCentV        )
        .def_readonly("devCentV",         &screenStructs::screenObject::devCentP        )
        .def_readonly("actPOSH",          &screenStructs::screenObject::actPOSH         )
        .def_readonly("actPOSV",          &screenStructs::screenObject::actPOSV         )
        .def_readonly("actPOSV",          &screenStructs::screenObject::actPOSP         )
        .def_readonly("hDriverState",     &screenStructs::screenObject::hDriverState    )
        .def_readonly("vDriverState",     &screenStructs::screenObject::vDriverState    )
        .def_readonly("elementExists",    &screenStructs::screenObject::elementExists   )
        .def_readonly("elementPositions", &screenStructs::screenObject::elementPositions)
        .def_readonly("elementDirection", &screenStructs::screenObject::elementDirection)
        .def_readonly("numIlocks",        &screenStructs::screenObject::numIlocks       )
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

    char const* getScreenObjectString = "Returns the screen object (name).";
    char const* isHOutString = "Returns true if the horizontal stage is out.";
    char const* isHInString = "Returns true if the horizontal stage is in.";
    char const* isVOutString = "Returns true if the vertical stage is out.";
    char const* isVInString = "Returns true if the vertical stage is in.";
    char const* isHandVOutString = "Returns true if both stages are out.";
    char const* isScreenInString = "Returns true if any of the screen devices are inserted.";
    char const* isHMovingString = "Returns true if the horizontal stage is moving.";
    char const* isVMovingString = "Returns true if the vertical stage is moving.";
    char const* isScreenMovingString = "Returns true if the screen stage is moving.";
    char const* moveScreenToString = "Will move the screen (name) to SCREEN_STATE state if it exists (as defined in the config file).";
    char const* insertYAGString = "Inserts the YAG screen.";
    char const* moveScreenOutString = "Will remove both horizontal and vertical devices.";
    char const* setScreenSDEVString = "Set screen device - the first part of moving a screen. Set the device to SCREEN_STATE state.";
    char const* setScreenTriggerString = "Set screen trigger == 1 - the second part of moving a screen. This will set the stage in motion.";
    char const* getScreenStateString = "Returns the current SCREEN_STATE.";
    char const* getScreenTypeString = "Returns the SCREEN_TYPE.";
    char const* getAvailableDevicesString = "Returns the devices available on the screen.";
    char const* isScreenInStateString = "Returns true if screen is currently in SCREEN_STATE state.";
    char const* isYAGInString = "Returns true if the YAG screen is in.";
    char const* isMoverString = "Returns true if the screen is an H and/or V mover.";
    char const* isPneumaticString = "Returns true if the screen is a pneumatic device.";
    char const* isHEnabledString = "Returns true if the horizontal stage is enabled.";
    char const* isVEnabledString = "Returns true if the vertical stage is enabled.";
    char const* isHElementString = "Returns true if the element SCREEN_STATE state is a horizontal device.";
    char const* isVElementString = "Returns true if the element SCREEN_STATE state is a vertical device.";
    char const* getACTPOSString = "Returns the current actuator position.";
    char const* getJDiffString = "Returns the difference between the current actuator position and the device centre.";
    char const* jogScreenString = "Jogs the screen from its current position by jog (mm) - for expert use and calibration.";
    char const* resetPositionString = "Resets the device to the position defined by SDEV.";
    char const* setEXString = "Sets the screen in motion after setPosition is called.";
    char const* setPositionString = "Sets the target position of the screen (mm) - for expert use and calibration.";
    char const* getDevicePosString = "Gets the position of a device SCREEN_STATE state.";
    boost::python::class_<screenController, boost::python::bases<controller>, boost::noncopyable>
        ("screenController","screenController Doc String",boost::python::no_init)
        .def("getScreenObject",       &screenController::getScreenObject, (arg("name")), getScreenObjectString, return_value_policy<reference_existing_object>() )
        .def("getILockStates",        &screenController::getILockStates                                                     )
        .def("isHOut",                &screenController::isHOut, (arg("name")), isHOutString                                )
        .def("isVOut",                &screenController::isVOut, (arg("name")), isVOutString                                )
        .def("is_HandV_OUT",          &screenController::is_HandV_OUT, (arg("name")), isHandVOutString                      )
        .def("isHIn",                 &screenController::isHIn, (arg("name")), isHOutString                                 )
        .def("isVIn",                 &screenController::isVIn, (arg("name")), isVInString                                  )
        .def("isScreenIn",            &screenController::isScreenIn, (arg("name")), isScreenInString                        )
        .def("isHMoving",             &screenController::isHMoving, (arg("name")), isHMovingString                          )
        .def("isVMoving",             &screenController::isVMoving, (arg("name")), isVMovingString                          )
        .def("isScreenMoving",        &screenController::isScreenMoving, (arg("name")), isScreenMovingString                )
        .def("moveScreenTo",          &screenController::moveScreenTo, (arg("name"),arg("state")), moveScreenToString       )
        .def("insertYAG",             &screenController::insertYAG, (arg("name")), insertYAGString                          )
        .def("moveScreenOut",         &screenController::moveScreenOut, (arg("name")), moveScreenOutString                  )
        .def("setScreenSDEV",         &screenController::setScreenSDEV, (arg("name")), setScreenSDEVString                  )
        .def("setScreenTrigger",      &screenController::setScreenTrigger, (arg("name")), setScreenTriggerString            )
        .def("getScreenState",        &screenController::getScreenState, (arg("name")), getScreenStateString                )
        .def("getScreenType",         &screenController::getScreenType, (arg("name")), getScreenTypeString                  )
        .def("isScreenInState",       &screenController::isScreenInState, (arg("name"),arg("state")), isScreenInStateString )
        .def("isMover",               &screenController::isMover, (arg("name")), isYAGInString                              )
        .def("isPneumatic",           &screenController::isPneumatic, (arg("name")), isMoverString                          )
        .def("isYAGIn",               &screenController::isYAGIn, (arg("name")), isPneumaticString                          )
        .def("isHEnabled",            &screenController::isHEnabled, (arg("name")), isHEnabledString                        )
        .def("isVEnabled",            &screenController::isVEnabled, (arg("name")), isVEnabledString                        )
        .def("isHElement",            &screenController::isHElement, (arg("name")), isHElementString                        )
        .def("isVElement",            &screenController::isVElement, (arg("name")), isVElementString                        )
        .def("getACTPOS",             &screenController::getACTPOS, (arg("name")), getACTPOSString                          )
        .def("getJDiff",              &screenController::getJDiff, (arg("name")), getJDiffString                            )
        .def("getDevicePosition",     &screenController::getDevicePosition, (arg("name"),arg("state")), getDevicePosString  )
        .def("getAvailableDevices",   &screenController::getAvailableDevices, (arg("name")), getAvailableDevicesString      )
        .def("jogScreen",             &screenController::jogScreen, (arg("name"),arg("jog (mm)")), jogScreenString          )
        .def("resetPosition",         &screenController::resetPosition, (arg("name")), resetPositionString                  )
        .def("setEX",                 &screenController::setEX, (arg("name")), setEXString                                  )
        .def("setPosition",           &screenController::setPosition, (arg("name"),arg("pos (mm)")), setPositionString      )
        .def("debugMessagesOff",      &screenController::debugMessagesOff                         )
        .def("debugMessagesOn",       &screenController::debugMessagesOn                          )
        .def("messagesOff",           &screenController::messagesOff                              )
        .def("messagesOn",            &screenController::messagesOn                               )
        .def("silence",               &screenController::silence                                  )
        .def("verbose",               &screenController::verbose                                  )
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
