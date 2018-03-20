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
//  Author:      DJS
//  Last edit:   19-03-2018
//  FileName:    VCrfprot.h
//  Description:
//
//
//*/
#ifndef VC_RFPROTECTION_H_
#define VC_RFPROTECTION_H_
// STL
#include <string>
// HWC
#include "gunProtController.h"
#include "rfProtStructs.h"
#include "VCbase.h"
#include "VCHeader.h"
#include "configdEfinitions.h"
//______________________________________________________________________________
class VCrfprot : public VCbase
{
    public:
        VCrfprot();
        ~VCrfprot();

//        void setQuiet();
//        void setVerbose();
//        void setMessage();
//        void setDebugMessage();

        gunProtController& virtual_Gun_Protection_Controller();
        gunProtController& physical_Gun_Protection_Controller();
        gunProtController& offline_Gun_Protection_Controller();

    protected:

    private:
        gunProtController * virtual_Gun_Protection_Controller_Obj ;
        gunProtController * physical_Gun_Protection_Controller_Obj;
        gunProtController * offline_Gun_Protection_Controller_Obj ;

//        const bool withEPICS, withoutEPICS, withoutVM, withVM;
//        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string allGunProtsConf;


};
/* yay, function pointers for boost.python overloads */
using namespace boost::python;
bool(gunProtController::*reset_1)(const std::string&) =
                                                &gunProtController::reset;
bool(gunProtController::*reset_2)(const std::vector<std::string>&) =
                                                &gunProtController::reset;
bool(gunProtController::*enable_1 )(const std::string&) =
                                                &gunProtController::enable;
bool(gunProtController::*enable_2 )(const std::vector<std::string>&) =
                                                &gunProtController::enable;
bool(gunProtController::*enable_3 )() = &gunProtController::enable;
bool(gunProtController::*disable_1)(const std::string&) =
                                                &gunProtController::disable;
bool(gunProtController::*disable_2)(const std::vector<std::string>&) =
                                                &gunProtController::disable;

using namespace boost::python;
using namespace rfProtStructs;
using namespace UTL;
BOOST_PYTHON_MODULE(VELA_CLARA_RF_Protection_Control)
{
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();

    /* Main project objects and enums are defined in here */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();
        /* docstrings will be defined like this  */
    const char * RF_GUN_PROT_STATUS_str = "RF_GUN_PROT_STATUS: a named integer giving the state of an RF protection object";


    enum_<RF_GUN_PROT_STATUS>("RF_GUN_PROT_STATUS",RF_GUN_PROT_STATUS_str)
        .value("UNKNOWN",RF_GUN_PROT_STATUS::UNKNOWN)
        .value("ERROR",  RF_GUN_PROT_STATUS::ERROR)
        .value("GOOD",   RF_GUN_PROT_STATUS::GOOD)
        .value("BAD",    RF_GUN_PROT_STATUS::BAD)
        ;

    boost::python::class_<rfGunProtObject,boost::noncopyable>
        ("rfGunProtObject","rfGunProtObject member variables (read access only)", boost::python::no_init)
        .def_readonly("name",               &rfGunProtObject::name,"object name")
        .def_readonly("pvRoot",             &rfGunProtObject::pvRoot,"object pvRoot")
        .def_readonly("status",             &rfGunProtObject::status,"object status")
        .def_readonly("cmi",                &rfGunProtObject::cmi,"object cmi")
        .def_readonly("gunProtKeyBits",     &rfGunProtObject::gunProtKeyBits,"which bits in cmi refer to physcial keys")
        .def_readonly("gunProtKeyBitValues",&rfGunProtObject::gunProtKeyBitValues,"key bit values")
        .def_readonly("protType",           &rfGunProtObject::protType,"rf gun protection type ")
        ;

 boost::python::class_<gunProtController, boost::python::bases<controller>, boost::noncopyable>
        ("gunProtController","DOCSTRING", boost::python::no_init)
        .def("getILockStates",         &gunProtController::getILockStates,"Return the state of interlocks as an integer. There are currently NO epics ilocks for the gun protections (in a sense the protections ARE the interlocks).")
        .def("getILockStatesStr",      &gunProtController::getILockStatesStr,"Return state of interlocks as a stringr. There are currently NO epics ilocks for the gun protections (in a sense the protections ARE the interlocks)")
        .def("get_CA_PEND_IO_TIMEOUT", &gunProtController::get_CA_PEND_IO_TIMEOUT,"Return the current waiting time [seconds] when sending commands to EPICS.")
        .def("set_CA_PEND_IO_TIMEOUT",  &gunProtController::set_CA_PEND_IO_TIMEOUT, (boost::python::arg("time"), "Set a new waiting time [seconds] when sending commands to EPICS"))
        .def("getRFProtObjConstRef",&gunProtController::getRFProtObjConstRef,return_value_policy<reference_existing_object>(),(arg("name")),"Return RF protection object 'name'")
        .def("isGood",      &gunProtController::isGood,    (boost::python::arg("name"),"returns True if protection status is good"))
        .def("isNotGood",   &gunProtController::isNotGood, (boost::python::arg("name"),"returns True if protection status is not good"))
        .def("isBad",       &gunProtController::isBad,     (boost::python::arg("name"),"returns True if protection status is bad"))
        .def("reset",       reset_1,   (NAME_ARG,"reset protection by name"))
        .def("reset",       reset_2,   (boost::python::arg("names"),"reset multiple protections by names"))
        .def("enable",      enable_1,  (boost::python::arg("name"),"enable protection by name"))
        .def("enable",      enable_2,  (boost::python::arg("names"),"enable multiple protections by names"))
        .def("enable",      enable_3,  "enable general, current mode and enable, protections, in that order")
        .def("disable",     disable_1, (boost::python::arg("name"),"disable protection by name"))
        .def("disable",     disable_2, (boost::python::arg("names"),"disable multiple protections by names"))
        .def("getGeneralProtName",     &gunProtController::getGeneralProtName, "returns the object name for the general protection")
        .def("getEnableProtName",     &gunProtController::getEnableProtName, "returns the object name for the enable protection")
        .def("getCurrentModeProtName",     &gunProtController::getCurrentModeProtName, "returns the object name for the current mode protection. Current mode depnds on which physical keys are active.")
        //.def_readonly("currentMode",               &gunProtController::currentMode,"Varibale defining the current mode of the gun system. Current mode depnds on which physical keys are active and could be, VELA line, LRRG, CLARA line HRRG, Test mode, etc.")
        ;

        using namespace boost;
    /// The main class that creates all the controller obejcts
    class_<VCrfprot, bases<VCbase>, noncopyable>("init")
        .def("virtual_Gun_Protection_Controller",  &VCrfprot::virtual_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual gun protection object.")
        .def("physical_Gun_Protection_Controller",  &VCrfprot::physical_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical gun protection object.")
        .def("offline_Gun_Protection_Controller",  &VCrfprot::offline_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline gun protection object.")
//        .def("setQuiet",         &VCrfprot::setQuiet,
//             "set Quiet Mode (no messages, no debug messages) for all PI laser objects." )
//        .def("setVerbose",       &VCrfprot::setVerbose,"set Verbose Mode (all messages, all debug messages) for all gun protection objects.")
//        .def("setMessage",       &VCrfprot::setMessage, "set Message Mode (all  messages, no debug messages) for all gun protection objects.")
//        .def("setDebugMessage",  &VCrfprot::setDebugMessage, "set Debug Mode (no messages, all debug messages) for all gun protection objects." )
        ;
}

#endif // VCrfprot_H_
