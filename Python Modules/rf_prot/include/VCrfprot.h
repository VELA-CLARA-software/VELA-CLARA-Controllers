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
// STL includes
#include <string>
#include <map>
#include <utility>
// Project includes
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

        gunProtController& virtual_Gun_Protection_Controller();
        gunProtController& physical_Gun_Protection_Controller();
        gunProtController& offline_Gun_Protection_Controller();

        gunProtController& getProtectionController(const HWC_ENUM::MACHINE_MODE mode,
                                                   const HWC_ENUM::MACHINE_AREA area);

    protected:

    private:
        gunProtController* virtual_Gun_Protection_Controller_Obj ;
        gunProtController* physical_Gun_Protection_Controller_Obj;
        gunProtController* offline_Gun_Protection_Controller_Obj ;

        /* all get controller functions route here */
        gunProtController& getController(gunProtController*& cont,
                                         const std::string& conf,
                                         const std::string& name,
                                         const bool shouldVM,
                                         const bool shouldEPICS,
                                         const HWC_ENUM::MACHINE_AREA myMachineArea);
        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const gunProtController*, std::pair<bool,bool>> messageStates;

        void updateMessageStates();

        const std::string allGunProtsConf;
};
/* yay, function pointers for boost.python overloads */
using namespace boost::python;
bool(gunProtController::*reset_1)(const std::string&) const=
                                                &gunProtController::reset;
bool(gunProtController::*reset_2)(const std::vector<std::string>&) const=
                                                &gunProtController::reset;
bool(gunProtController::*enable_1)(const std::string&) const=
                                                &gunProtController::enable;
bool(gunProtController::*enable_2)(const std::vector<std::string>&) const=
                                                &gunProtController::enable;
bool(gunProtController::*enable_3)() const= &gunProtController::enable;
bool(gunProtController::*disable_1)(const std::string&) const=
                                                &gunProtController::disable;
bool(gunProtController::*disable_2)(const std::vector<std::string>&) const=
                                                &gunProtController::disable;
//______________________________________________________________________________
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
    const char * RF_GUN_PROT_STATUS_doc = "RF_GUN_PROT_STATUS: a named integer"
                                " giving the state of an RF protection object";
    enum_<RF_GUN_PROT_STATUS>("RF_GUN_PROT_STATUS",RF_GUN_PROT_STATUS_doc)
        .value("UNKNOWN",RF_GUN_PROT_STATUS::UNKNOWN)
        .value("ERROR",  RF_GUN_PROT_STATUS::ERROR)
        .value("GOOD",   RF_GUN_PROT_STATUS::GOOD)
        .value("BAD",    RF_GUN_PROT_STATUS::BAD)
        ;
    const char* RF_GUN_PROT_TYPE_doc = "RF_PROT_TYPE: a named integer"
                    " giving the type of RF protection object";
    enum_<rfProtStructs::RF_PROT_TYPE>("RF_PROT_TYPE",RF_GUN_PROT_TYPE_doc)
        .value("CLARA_HRRG",RF_PROT_TYPE::CLARA_HRRG)
        .value("VELA_LRRG",  RF_PROT_TYPE::VELA_LRRG)
        .value("VELA_HRRG",   RF_PROT_TYPE::VELA_HRRG)
        .value("CLARA_LRRG",    RF_PROT_TYPE::CLARA_LRRG)
        .value("TEST",    RF_PROT_TYPE::TEST)
        .value("NOT_KNOWN",    RF_PROT_TYPE::NOT_KNOWN)
        .value("GENERAL",    RF_PROT_TYPE::GENERAL)
        .value("ENABLE",    RF_PROT_TYPE::ENABLE)
        .value("NO_MODE",    RF_PROT_TYPE::NO_MODE)
        ;


    const char* gunProtKeyBitValues_doc = "key bit values";
    const char* gunProtKeyBits_doc      = "which bits in cmi refer to physcial keys";
    const char* protType_doc            = "RF gun protection type";
    const char* pvRoot_doc              = "object PV Root";
    const char* status_doc              = "object status";
    const char* name_doc                = "object name";
    const char* cmi_doc                 = "cmi value";

    using namespace boost;
    class_<rfGunProtObject,noncopyable>
        ("rfGunProtObject","rfGunProtObject member variables (read access only)", no_init)
        .def_readonly("gunProtKeyBitValues",
                      &rfGunProtObject::gunProtKeyBitValues,gunProtKeyBitValues_doc)
        .def_readonly("gunProtKeyBits",
                      &rfGunProtObject::gunProtKeyBits,gunProtKeyBits_doc)
        .def_readonly("protType",
                      &rfGunProtObject::protType,protType_doc)
        .def_readonly("pvRoot",
                      &rfGunProtObject::pvRoot, pvRoot_doc)
        .def_readonly("status",
                      &rfGunProtObject::status, status_doc)
        .def_readonly("name",
                      &rfGunProtObject::name,name_doc)
        .def_readonly("cmi",
                      &rfGunProtObject::cmi, cmi_doc)
        ;


    const char* gunProtController_doc = "Monitors and controls the Gun protections";
    const char* getILockStates_doc = "Return the state of interlocks as an integer."
        "There are currently NO epics ilocks for the gun protections (in a sense the"
        " protections ARE the interlocks).";
    const char* getILockStatesStr_doc = "Return state of interlocks as a stringr. "
        "There are currently NO epics ilocks for the gun protections (in a sense the "
        "protections ARE the interlocks)";
    const char* get_CA_PEND_IO_TIMEOUT_doc = "Return the current waiting time (seconds)"
        " when sending commands to EPICS.";
    const char* set_CA_PEND_IO_TIMEOUT_doc = "Set the waiting time to 'time'(seconds) "
        "when sending commands to EPICS";
    const char* getRFProtObjConstRef_doc = "Return RF protection object 'name'";
    const char* getCurrentModeProtName_doc = "returns the object name for the 'current"
        " mode' protection. Current mode depends on which physical keys are active.";
    const char* getGeneralProtName_doc= "returns the object name for the 'general protection'";
    const char* getEnableProtName_doc= "returns the object name for the enable protection";
    const char* isNotGood_doc= "returns True if protection status is not good";
    const char* getProtNames_doc= "Return names of protection objects in controller";
    const char* isGood_doc   = "returns True if protection status is good";
    const char* isBad_doc= "returns True if protection status is bad";

    const char* reset1_doc  = "reset protection 'name'";
    const char* reset2_doc  = "reset multiple protections given by 'names'";
    const char* enable1_doc = "enable protection 'name'";
    const char* enable2_doc = "enable multiple protections given by 'names'";
    const char* enable3_doc = "enable general, current mode and enable, "
                              "protections, in that order";
    const char* disable1_doc= "disable protection 'name'";
    const char* disable2_doc= "disable multiple protections given by 'names'";

    class_<gunProtController, bases<controller>, noncopyable>
        ("gunProtController",gunProtController_doc, no_init)
        .def("getILockStates",
             &gunProtController::getILockStates_Py,getILockStates_doc)
        .def("getILockStatesStr",
             &gunProtController::getILockStatesStr_Py,getILockStatesStr_doc)
        .def("get_CA_PEND_IO_TIMEOUT",
             &gunProtController::get_CA_PEND_IO_TIMEOUT,get_CA_PEND_IO_TIMEOUT_doc)
        .def("set_CA_PEND_IO_TIMEOUT",
             &gunProtController::set_CA_PEND_IO_TIMEOUT,
             (TIME_ARG,set_CA_PEND_IO_TIMEOUT_doc))
        .def("getRFProtObjConstRef",
             &gunProtController::getRFProtObjConstRef,
             return_value_policy<reference_existing_object>(),
             (NAME_ARG,getRFProtObjConstRef_doc))
        .def("isGood",
             &gunProtController::isGood,    (NAME_ARG,isGood_doc))
        .def("isNotGood",
             &gunProtController::isNotGood, (NAME_ARG,isNotGood_doc))
        .def("isBad",
             &gunProtController::isBad,(NAME_ARG,isBad_doc))
        .def("reset",       reset_1,   (NAME_ARG,reset1_doc))
        .def("reset",       reset_2,   (NAMES_ARG,reset2_doc))
        .def("enable",      enable_1,  (NAME_ARG,enable1_doc))
        .def("enable",      enable_2,  (NAMES_ARG,enable2_doc))
        .def("enable",      enable_3,   enable3_doc)
        .def("disable",     disable_1, (NAME_ARG,disable1_doc))
        .def("disable",     disable_2, (NAMES_ARG,disable2_doc))
        .def("getGeneralProtName",
             &gunProtController::getGeneralProtName,getGeneralProtName_doc)
        .def("getEnableProtName",
             &gunProtController::getEnableProtName,getEnableProtName_doc)
        .def("getCurrentModeProtName",
             &gunProtController::getCurrentModeProtName,getCurrentModeProtName_doc)
        .def("getProtNames",
             &gunProtController::getProtNames_Py,getProtNames_doc)
        ;

    const char* GPC_doc = "Returns a reference to the protection object given "
                          "by 'mode' and 'area'.";
    const char* vGPC_doc = "Returns a reference to the virtual gun protection object.";
    const char* pGPC_doc = "Returns a reference to the physical gun protection object.";
    const char* oGPC_doc = "Returns a reference to the offline gun protection object.";

    class_<VCrfprot, bases<VCbase>, noncopyable>("init")
        .def("getProtectionController",
             &VCrfprot::getProtectionController,
             return_value_policy<reference_existing_object>(),(MODE_ARG,AREA_ARG,GPC_doc))
        .def("virtual_Gun_Protection_Controller",
             &VCrfprot::virtual_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),vGPC_doc)
        .def("physical_Gun_Protection_Controller",
             &VCrfprot::physical_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),pGPC_doc)
        .def("offline_Gun_Protection_Controller",
             &VCrfprot::offline_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),oGPC_doc)
//        .def("setQuiet",         &VCbase::setQuiet)
//        .def("setVerbose",       &VCbase::setVerbose)
//        .def("setMessage",       &VCbase::setMessage)
//        .def("setDebugMessage",  &VCbase::setDebugMessage)
        ;
}
//______________________________________________________________________________
#endif // VC_RFPROTECTION_H_
