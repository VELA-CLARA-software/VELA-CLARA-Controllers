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
#include "L01ProtController.h"
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

        L01ProtController& virtual_L01_Protection_Controller();
        L01ProtController& physical_L01_Protection_Controller();
        L01ProtController& offline_L01_Protection_Controller();

        gunProtController& getProtectionController(const HWC_ENUM::MACHINE_MODE mode,
                                                   const HWC_ENUM::MACHINE_AREA area);

    protected:

    private:
        gunProtController* virtual_Gun_Protection_Controller_Obj ;
        gunProtController* physical_Gun_Protection_Controller_Obj;
        gunProtController* offline_Gun_Protection_Controller_Obj ;

        L01ProtController* virtual_L01_Protection_Controller_Obj ;
        L01ProtController* physical_L01_Protection_Controller_Obj;
        L01ProtController* offline_L01_Protection_Controller_Obj ;

        /* all get controller functions route here */
        L01ProtController& getL01Controller(L01ProtController*& cont,
                                         const std::string& conf,
                                         const std::string& name,
                                         const bool shouldVM,
                                         const bool shouldEPICS,
                                         const HWC_ENUM::MACHINE_AREA myMachineArea);


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
        std::map<const L01ProtController*, std::pair<bool,bool>> messageStatesL01;

        void updateMessageStates();

        const std::string allGunProtsConf, L01ProtsConf;
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

const rfProtStructs::rfProtObject&(gunProtController::*getRFProtObjConstRef_1)(const std::string&) const=
                                                &gunProtController::getRFProtObjConstRef;


const rfProtStructs::rfProtObject&(gunProtController::*getRFProtObjConstRef_2)(const rfProtStructs::RF_PROT_TYPE type) const=
                                                &gunProtController::getRFProtObjConstRef;


    // There is a gun controller and a linac controller
    // This is clearly nuts - for CATAP 2.0 make better !

bool(L01ProtController::*reset_1L01)(const std::string&) const=
                                                &L01ProtController::reset;
bool(L01ProtController::*reset_2L01)(const std::vector<std::string>&) const=
                                                &L01ProtController::reset;
bool(L01ProtController::*enable_1L01)(const std::string&) const=
                                                &L01ProtController::enable;
bool(L01ProtController::*enable_2L01)(const std::vector<std::string>&) const=
                                                &L01ProtController::enable;
bool(L01ProtController::*enable_3L01)() const= &L01ProtController::enable;
bool(L01ProtController::*disable_1L01)(const std::string&) const=
                                                &L01ProtController::disable;
bool(L01ProtController::*disable_2L01)(const std::vector<std::string>&) const=
                                                &L01ProtController::disable;

const rfProtStructs::rfProtObject&(L01ProtController::*getRFProtObjConstRef_1L01)(const std::string&) const=
&L01ProtController::getRFProtObjConstRef;

const rfProtStructs::rfProtObject&(L01ProtController::*getRFProtObjConstRef_2L01)(const rfProtStructs::RF_PROT_TYPE type) const=
                                                &L01ProtController::getRFProtObjConstRef;



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
    const char * RF_PROT_STATUS_doc = "RF_PROT_STATUS: a named integer"
                                " giving the state of an RF protection object";
    enum_<RF_PROT_STATUS>("RF_PROT_STATUS",RF_PROT_STATUS_doc)
        .value("UNKNOWN",RF_PROT_STATUS::UNKNOWN)
        .value("ERROR",  RF_PROT_STATUS::ERROR)
        .value("GOOD",   RF_PROT_STATUS::GOOD)
        .value("BAD",    RF_PROT_STATUS::BAD)
        ;
    const char* RF_PROT_TYPE_doc = "RF_PROT_TYPE: a named integer"
                    " giving the type of RF protection object";
    enum_<rfProtStructs::RF_PROT_TYPE>("RF_PROT_TYPE",RF_PROT_TYPE_doc)
        .value("CLARA_HRRG",RF_PROT_TYPE::CLARA_HRRG)
        .value("VELA_LRRG",  RF_PROT_TYPE::VELA_LRRG)
        .value("VELA_HRRG",   RF_PROT_TYPE::VELA_HRRG)
        .value("CLARA_LRRG",    RF_PROT_TYPE::CLARA_LRRG)
        .value("TEST",    RF_PROT_TYPE::TEST)
        .value("L01",    RF_PROT_TYPE::L01)
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
    class_<rfProtObject,noncopyable>
        ("rfProtObject","rfProtObject member variables (read access only)", no_init)
        .def_readonly("gunProtKeyBitValues",
                      &rfProtObject::gunProtKeyBitValues,gunProtKeyBitValues_doc)
        .def_readonly("gunProtKeyBits",
                      &rfProtObject::gunProtKeyBits,gunProtKeyBits_doc)
        .def_readonly("protType",
                      &rfProtObject::protType,protType_doc)
        .def_readonly("pvRoot",
                      &rfProtObject::pvRoot, pvRoot_doc)
        .def_readonly("status",
                      &rfProtObject::status, status_doc)
        .def_readonly("name",
                      &rfProtObject::name,name_doc)
        .def_readonly("cmi",
                      &rfProtObject::cmi, cmi_doc)
        ;

    // There is a gun controller and a linac controller
    // This is clearly nuts - for CATAP 2.0 make better !

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
    const char* getRFProtObjConstRef_1_doc = "Return RF protection object 'name' where name is a string";
    const char* getRFProtObjConstRef_2_doc = "Return RF protection object 'name' where name is a RF_PROT_TYPE";
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
             getRFProtObjConstRef_1,
             return_value_policy<reference_existing_object>(),
             (NAME_ARG,getRFProtObjConstRef_1_doc))
        .def("getRFProtObjConstRef",
             getRFProtObjConstRef_2,
             return_value_policy<reference_existing_object>(),
             (NAME_ARG,getRFProtObjConstRef_2))
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




    const char* L01ProtController_doc = "Monitors and controls the Gun protections";


    class_<L01ProtController, bases<controller>, noncopyable>
        ("L01ProtController",L01ProtController_doc, no_init)
        .def("getILockStates",
             &L01ProtController::getILockStates_Py,getILockStates_doc)
        .def("getILockStatesStr",
             &L01ProtController::getILockStatesStr_Py,getILockStatesStr_doc)
        .def("get_CA_PEND_IO_TIMEOUT",
             &L01ProtController::get_CA_PEND_IO_TIMEOUT,get_CA_PEND_IO_TIMEOUT_doc)
        .def("set_CA_PEND_IO_TIMEOUT",
             &L01ProtController::set_CA_PEND_IO_TIMEOUT,
             (TIME_ARG,set_CA_PEND_IO_TIMEOUT_doc))
        .def("getRFProtObjConstRef",
             getRFProtObjConstRef_1L01,
             return_value_policy<reference_existing_object>(),
             (NAME_ARG,getRFProtObjConstRef_1_doc))
        .def("getRFProtObjConstRef",
             getRFProtObjConstRef_2L01,
             return_value_policy<reference_existing_object>(),
             (NAME_ARG,getRFProtObjConstRef_2_doc))
        .def("isGood",
             &L01ProtController::isGood,    (NAME_ARG,isGood_doc))
        .def("isNotGood",
             &L01ProtController::isNotGood, (NAME_ARG,isNotGood_doc))
        .def("isBad",
             &L01ProtController::isBad,(NAME_ARG,isBad_doc))
        .def("reset",       reset_1L01,   (NAME_ARG,reset1_doc))
        .def("reset",       reset_2L01,   (NAMES_ARG,reset2_doc))
        .def("enable",      enable_1L01,  (NAME_ARG,enable1_doc))
        .def("enable",      enable_2L01,  (NAMES_ARG,enable2_doc))
        .def("enable",      enable_3L01,   enable3_doc)
        .def("disable",     disable_1L01, (NAME_ARG,disable1_doc))
        .def("disable",     disable_2L01, (NAMES_ARG,disable2_doc))
        .def("getGeneralProtName",
             &L01ProtController::getGeneralProtName,getGeneralProtName_doc)
        .def("getEnableProtName",
             &L01ProtController::getEnableProtName,getEnableProtName_doc)
        .def("getCurrentModeProtName",
             &L01ProtController::getCurrentModeProtName,getCurrentModeProtName_doc)
        .def("getProtNames",
             &L01ProtController::getProtNames_Py,getProtNames_doc)
        ;




    const char* GPC_doc = "Returns a reference to the protection object given "
                          "by 'mode' and 'area'.";
    const char* vGPC_doc = "Returns a reference to the virtual gun RF protection object.";
    const char* pGPC_doc = "Returns a reference to the physical gun RF protection object.";
    const char* oGPC_doc = "Returns a reference to the offline gun RF protection object.";
    const char* vL01PC_doc = "Returns a reference to the virtual L01 RF protection object.";
    const char* pL01PC_doc = "Returns a reference to the physical L01 RF protection object.";
    const char* oL01PC_doc= "Returns a reference to the offline L01 RF protection object.";

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

        .def("virtual_L01_Protection_Controller",
             &VCrfprot::virtual_L01_Protection_Controller,
             return_value_policy<reference_existing_object>(),vL01PC_doc)
        .def("physical_L01_Protection_Controller",
             &VCrfprot::physical_L01_Protection_Controller,
             return_value_policy<reference_existing_object>(),pL01PC_doc)
        .def("offline_L01_Protection_Controller",
             &VCrfprot::offline_L01_Protection_Controller,
             return_value_policy<reference_existing_object>(),oL01PC_doc)
//        .def("setQuiet",         &VCbase::setQuiet)
//        .def("setVerbose",       &VCbase::setVerbose)
//        .def("setMessage",       &VCbase::setMessage)
//        .def("setDebugMessage",  &VCbase::setDebugMessage)
        ;
}
//______________________________________________________________________________
#endif // VC_RFPROTECTION_H_
