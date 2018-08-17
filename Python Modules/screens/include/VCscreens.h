/*
//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
//
//  Author:      ADB/DJS
//  Last edit:   07-08-2018
//  FileName:    VCscreens.h
//  Description:
//
//
//*/

#ifndef _VC_SCREENS_H_
#define _VC_SCREENS_H_
//stl
#include <string>
//
#include "screenController.h"
#include "VCheader.h"
#include "screenStructs.h"
#include "VCbase.h"
#include "configDefinitions.h"

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

class VCscreens : public VCbase
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

        screenController& virtual_C2B_Screen_Controller();
        screenController& offline_C2B_Screen_Controller();
        screenController& physical_C2B_Screen_Controller();

//        void setQuiet();
//        void setVerbose();
//        void setMessage();
//        void setDebugMessage();

        screenController& getScreenController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area );

    protected:

    private:

        std::map<screenController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        screenController& getController(screenController*& cont,
                                                     const std::string& conf,
                                                     const std::string& name,
                                                     const bool shouldVM,
                                                     const bool shouldEPICS,
                                                     const HWC_ENUM::MACHINE_AREA myMachineArea);

        screenController* virtual_VELA_INJ_Screen_Controller_Obj ;
        screenController* physical_VELA_INJ_Screen_Controller_Obj;
        screenController* offline_VELA_INJ_Screen_Controller_Obj ;

        screenController* virtual_VELA_BA1_Screen_Controller_Obj ;
        screenController* physical_VELA_BA1_Screen_Controller_Obj;
        screenController* offline_VELA_BA1_Screen_Controller_Obj ;

        screenController* virtual_VELA_BA2_Screen_Controller_Obj;
        screenController* offline_VELA_BA2_Screen_Controller_Obj;
        screenController* physical_VELA_BA2_Screen_Controller_Obj;

        screenController* virtual_CLARA_PH1_Screen_Controller_Obj;
        screenController* offline_CLARA_PH1_Screen_Controller_Obj;
        screenController* physical_CLARA_PH1_Screen_Controller_Obj;

        screenController* virtual_C2B_Screen_Controller_Obj;
        screenController* offline_C2B_Screen_Controller_Obj;
        screenController* physical_C2B_Screen_Controller_Obj;

        const HWC_ENUM::MACHINE_AREA  VELA_INJ, VELA_BA1, VELA_BA2, CLARA_PH1, UNKNOWN_AREA;
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
//        .value("SCREEN_IN",       screenStructs::SCREEN_STATE::SCREEN_IN  )
//        .value("SCREEN_OUT",      screenStructs::SCREEN_STATE::SCREEN_OUT )
//        .value("SCREEN_MOVING",   screenStructs::SCREEN_STATE::SCREEN_MOVING )
//        .value("SCREEN_ERROR",    screenStructs::SCREEN_STATE::SCREEN_ERROR  )
//        .value("SCREEN_UNKNOWN",  screenStructs::SCREEN_STATE::SCREEN_UNKNOWN)
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
    class_<std::pair<screenStructs::DRIVER_DIRECTION,screenStructs::SCREEN_STATE>>("std_pair_SCREEN_STATE_DRIVER_DIRECTION")
        .def(boost::python::init<  std::pair<screenStructs::DRIVER_DIRECTION,screenStructs::SCREEN_STATE> >())
        .def_readonly( "first",  &std::pair< screenStructs::DRIVER_DIRECTION,screenStructs::SCREEN_STATE >::first  )
        .def_readonly( "second", &std::pair< screenStructs::DRIVER_DIRECTION,screenStructs::SCREEN_STATE >::second );
        ;
//BOOST_PYTHON_MODULE(std_pair){
//    bp::class_< std::pair< int, int > >( "pair_less__int_comma__int__greater_", "documentation", bp::init< >("documentation") )
//        .def( bp::init< int const &, int const & >(( bp::arg("__a"),bp::arg("__b") ), "documentation") )
//        .def_readwrite( "first", &std::pair< int, int >::first,"documentation" )
//        .def_readwrite( "second", &std::pair< int, int >::second,"documentation" );
    char const* screenObjectString = "This struct contains the screen object - inc. type of screen (HV mover, pneumatic etc.), and horizontal and vertical state.\n"
                                     "It also gives the names of all the devices (positions) for a given screen.";
    boost::python::class_<screenStructs::screenObject,boost::noncopyable>
        ("screenObject",screenObjectString, boost::python::no_init)
        .def_readonly("name",             &screenStructs::screenObject::name            )
        .def_readonly("pvRoot",           &screenStructs::screenObject::pvRoot          )
        .def_readonly("screenType",       &screenStructs::screenObject::screenType      )
        .def_readonly("screenState",      &screenStructs::screenObject::screenState     )
        .def_readonly("screenSetState",   &screenStructs::screenObject::screenSetState  )
        .def_readonly("devCent",          &screenStructs::screenObject::devCent         )
        .def_readonly("actPOSH",          &screenStructs::screenObject::actPOSH         )
        .def_readonly("actPOSV",          &screenStructs::screenObject::actPOSV         )
        .def_readonly("actPOSP",          &screenStructs::screenObject::actPOSP         )
        .def_readonly("hDriverState",     &screenStructs::screenObject::hDriverState    )
        .def_readonly("vDriverState",     &screenStructs::screenObject::vDriverState    )
        .def_readonly("elementExists",    &screenStructs::screenObject::elementExists   )
        .def_readonly("elementPositions", &screenStructs::screenObject::elementPositions)
        .def_readonly("elementDirection", &screenStructs::screenObject::elementDirection)
        .def_readonly("numIlocks",        &screenStructs::screenObject::numIlocks       )
        .def_readonly("position",         &screenStructs::screenObject::position        )
        ;

    /// Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    /// we have to tell boost.python about pure virtual methods in abstract base classes
//    boost::python::class_<controller, boost::python::bases<baseObject>,  boost::noncopyable> ("controller", boost::python::no_init)
//        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
//        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
//        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
//        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
//		;

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
    char const* getScreenNamesString = "Returns all screen names defined in config file.";
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
    char const* getPosString = "Gets the position of the screen in the lattice.";

    char const* isClearForBeam_ds = "Are all elements (apart from, maybe,  an RF Cage) removed from the beam axis.";

    using namespace boost::python;
    class_<screenController, boost::python::bases<controller>, boost::noncopyable>
        ("screenController","screenController Doc String",boost::python::no_init)
        .def("getScreenObject",       &screenController::getScreenObject, (boost::python::arg("name")), getScreenObjectString, return_value_policy<reference_existing_object>() )
        .def("getILockStates",        &screenController::getILockStates                                                     )
        .def("isHOut",                &screenController::isHOut, (boost::python::arg("name")), isHOutString                                )
        .def("isVOut",                &screenController::isVOut, (boost::python::arg("name")), isVOutString                                )
        .def("is_HandV_OUT",          &screenController::is_HandV_OUT, (boost::python::arg("name")), isHandVOutString                      )
        .def("isHIn",                 &screenController::isHIn, (boost::python::arg("name")), isHOutString                                 )
        .def("isVIn",                 &screenController::isVIn, (boost::python::arg("name")), isVInString                                  )
        .def("isScreenIn",            &screenController::isScreenIn, (boost::python::arg("name")), isScreenInString                        )
//        .def("isScreenInVec",         &screenController::isScreenIn_Py, (boost::python::arg("name")), isScreenInString                     )
        .def("isScreenMoving",        &screenController::isScreenMoving, (boost::python::arg("name")), isScreenMovingString                )
        .def("moveScreenTo",          &screenController::moveScreenTo, (boost::python::arg("name"),boost::python::arg("state"),boost::python::arg("dir")), moveScreenToString       )
        .def("insertYAG",             &screenController::insertYAG, (boost::python::arg("name")), insertYAGString                          )
        .def("moveScreenOut",         &screenController::moveScreenOut, (boost::python::arg("name")), moveScreenOutString                  )
        .def("setScreenSDEV",         &screenController::setScreenSDEV, (boost::python::arg("name")), setScreenSDEVString                  )
        .def("setScreenTrigger",      &screenController::setScreenTrigger, (boost::python::arg("name")), setScreenTriggerString            )
        .def("getScreenState",        &screenController::getScreenState, (boost::python::arg("name")), getScreenStateString                )
        .def("getScreenStateStr",     &screenController::getScreenStateStr, (boost::python::arg("name")), getScreenStateString             )
        .def("getScreenType",         &screenController::getScreenType, (boost::python::arg("name")), getScreenTypeString                  )
        .def("isScreenInState",       &screenController::isScreenInState, (boost::python::arg("name"),boost::python::arg("state")), isScreenInStateString )
        .def("isMover",               &screenController::isMover, (boost::python::arg("name")), isYAGInString                              )
        .def("isPneumatic",           &screenController::isPneumatic, (boost::python::arg("name")), isMoverString                          )
        .def("isYAGIn",               &screenController::isYAGIn, (boost::python::arg("name")), isPneumaticString                          )
        .def("isHEnabled",            &screenController::isHEnabled, (boost::python::arg("name")), isHEnabledString                        )
        .def("isVEnabled",            &screenController::isVEnabled, (boost::python::arg("name")), isVEnabledString                        )
        .def("isHElement",            &screenController::isHElement, (boost::python::arg("name")), isHElementString                        )
        .def("isVElement",            &screenController::isVElement, (boost::python::arg("name")), isVElementString                        )
        .def("getACTPOS",             &screenController::getACTPOS, (boost::python::arg("name")), getACTPOSString                          )
        .def("getJDiff",              &screenController::getJDiff, (boost::python::arg("name")), getJDiffString                            )
        .def("getDevicePosition",     &screenController::getDevicePosition, (boost::python::arg("name"),boost::python::arg("state")), getDevicePosString  )
        .def("getPosition",           &screenController::getPosition, (boost::python::arg("name")), getPosString                           )
        .def("getScreenNames",        &screenController::getScreenNames_Py, getScreenNamesString                            )
        .def("getAvailableDevices",   &screenController::getAvailableDevices, (boost::python::arg("name")), getAvailableDevicesString      )
        .def("jogScreen",             &screenController::jogScreen, (boost::python::arg("name"),boost::python::arg("jog (mm)")), jogScreenString          )
        .def("resetPosition",         &screenController::resetPosition, (boost::python::arg("name")), resetPositionString                  )
        .def("setEX",                 &screenController::setEX, (boost::python::arg("name")), setEXString                                  )
        .def("setPosition",           &screenController::setPosition, (boost::python::arg("name"),boost::python::arg("pos (mm)")), setPositionString      )
        .def("isClearForBeam",        &screenController::isClearForBeam, (boost::python::arg("name")), isClearForBeam_ds )
        .def("debugMessagesOff",      &screenController::debugMessagesOff                         )
        .def("debugMessagesOn",       &screenController::debugMessagesOn                          )
        .def("messagesOff",           &screenController::messagesOff                              )
        .def("messagesOn",            &screenController::messagesOn                               )
        .def("silence",               &screenController::silence                                  )
        .def("verbose",               &screenController::verbose                                  )
    ;

    /// The main class that creates all the controller obejcts
        boost::python::class_<VCscreens,boost::python::bases<VCbase>,boost::noncopyable> ("init")
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

        .def("virtual_C2B_Screen_Controller",  &VCscreens::virtual_C2B_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_C2B_Screen_Controller",  &VCscreens::offline_C2B_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_C2B_Screen_Controller",  &VCscreens::physical_C2B_Screen_Controller,
             return_value_policy<reference_existing_object>())

        .def("getScreenController",  &VCscreens::getScreenController,
             return_value_policy<reference_existing_object>())
        ;
}

#endif // _VCSCREENS_H
