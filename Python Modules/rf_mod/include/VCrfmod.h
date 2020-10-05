#ifndef VCRFMOD_H_
#define VCRFMOD_H_
// STL
#include <string>
// HWC
#include "gunModController.h"
#include "l01ModController.h"
#include "rfModStructs.h"
#include "rfModStructs.h"

#define MODULE_NAME VELA_CLARA_RF_Modulator_Control

#include "VCheader.h"
#include "VCbase.h"

class VCrfmod : public VCbase
{
    public:
        VCrfmod();
        ~VCrfmod();

        gunModController& virtual_GUN_MOD_Controller();
        gunModController& physical_GUN_MOD_Controller();
        gunModController& offline_GUN_MOD_Controller();

        l01ModController& virtual_L01_MOD_Controller();
        l01ModController& physical_L01_MOD_Controller();
        l01ModController& offline_L01_MOD_Controller();

    protected:

    private:

        gunModController* virtual_GUN_MOD_Controller_Obj ;
        gunModController* physical_GUN_MOD_Controller_Obj;
        gunModController* offline_GUN_MOD_Controller_Obj ;

        l01ModController* virtual_L01_MOD_Controller_Obj ;
        l01ModController* physical_L01_MOD_Controller_Obj;
        l01ModController* offline_L01_MOD_Controller_Obj ;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;

        const std::string gunModConf;
        const std::string l01ModConf;
        l01ModController& getL01Controller(l01ModController*& cont,
                           const std::string& conf,
                           const std::string& name,
                           const bool shouldVM,
                           const bool shouldEPICS,
                           const HWC_ENUM::MACHINE_AREA myMachineArea);
        gunModController& getGunController(gunModController*& cont,
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
        std::map<const controller*, std::pair<bool,bool>> messageStates;

        void VCrfmod::updateMessageStates();

};
//
using namespace boost::python;
BOOST_PYTHON_MODULE(MODULE_NAME)
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();
    /*
        Things that you want to use in python muct be exposed:
        containers
    */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    enum_<rfModStructs::L01_MOD_FAULT>("L01_MOD_FAULT","L01_MOD_FAULT: a named integer giving the fault status of the L01 Modulator")
        .value("FAULT_UNKNOWN",  rfModStructs::L01_MOD_FAULT::FAULT_UNKNOWN)
        .value("NO_FAULT",       rfModStructs::L01_MOD_FAULT::NO_FAULT)
        ;

    enum_<rfModStructs::L01_MOD_STATE>("L01_MOD_STATE",
                                       "L01_MOD_STATE: a named integer giving the state of the L01 Modulator")
        .value("STATE_UNKNOWN", rfModStructs::L01_MOD_STATE::STATE_UNKNOWN)
        .value("L01_OFF",       rfModStructs::L01_MOD_STATE::L01_OFF)
        .value("L01_STANDBY",   rfModStructs::L01_MOD_STATE::L01_STANDBY)
        .value("L01_HV_ON",     rfModStructs::L01_MOD_STATE::L01_HV_ON)
        .value("L01_RF_ON",     rfModStructs::L01_MOD_STATE::L01_RF_ON)
        ;

    /// The main class that creates all the controller obejcts
        class_<VCrfmod,bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_GUN_MOD_Controller",  &VCrfmod::virtual_GUN_MOD_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual gun modulator object.")
        .def("physical_GUN_MOD_Controller",  &VCrfmod::physical_GUN_MOD_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical gun modulator object.")
        .def("offline_GUN_MOD_Controller",  &VCrfmod::offline_GUN_MOD_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline gun modulator object.")
        .def("virtual_L01_MOD_Controller",  &VCrfmod::virtual_L01_MOD_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual L01 modulator object.")
        .def("physical_L01_MOD_Controller",  &VCrfmod::physical_L01_MOD_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical L01 modulator object.")
        .def("offline_L01_MOD_Controller",  &VCrfmod::offline_L01_MOD_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline L01 modulator object.")
//        .def("setQuiet",         &VCrfmod::setQuiet,
//             "set Quiet Mode (no messages, no debug messages) for all PI laser objects.")
//        .def("setVerbose",       &VCrfmod::setVerbose,
//             "set Verbose Mode (all messages, all debug messages) for all PI laser objects.")
//        .def("setMessage",       &VCrfmod::setMessage,
//             "set Message Mode (all  messages, no debug messages) for all PI laser objects.")
//        .def("setDebugMessage",  &VCrfmod::setDebugMessage,
//             "set Debug Mode (no messages, all debug messages) for all PI laser objects.")
        ;

    class_<l01ModController, bases<controller>, boost::noncopyable>
        ("l01ModController","l01ModController Doc String",boost::python::no_init )
        .def("getModObjConstRef",&l01ModController::getObjConstRef,return_value_policy<reference_existing_object>(),
             "Return L01 Modulator Object Reference")
        .def("reset",     &l01ModController::reset,"")
        .def("setOff",    &l01ModController::setOff,"")
        ;

    class_<gunModController, bases<controller>, boost::noncopyable>
        ("gunModController","gunModController Doc String",boost::python::no_init )
        .def("getGunObjConstRef",&gunModController::getGunObjConstRef,return_value_policy<reference_existing_object>(),
             "Return Gun Modulator Object Reference")
        .def("getModObjConstRef",&gunModController::getGunObjConstRef,return_value_policy<reference_existing_object>(),
             "Return Gun Modulator Object Reference")
        .def("isErrorStateGood",&gunModController::isErrorStateGood,"Is the error state in GOOD")
        .def("isWarmedUp",&gunModController::isWarmedUp,"")
        .def("isNotWarmedUp",&gunModController::isNotWarmedUp,"")
        .def("waitForModState",&gunModController::waitForModState,"")
        .def("isErrorStateGood",&gunModController::isErrorStateGood,"")
        .def("isInRFOn",&gunModController::isInRFOn,"")
        .def("isInHVOn",&gunModController::isInHVOn,"")
        .def("isInStandby",&gunModController::isInStandby,"")
        .def("isInOff",&gunModController::isInOff,"")
        .def("reset",&gunModController::reset,"")
        .def("resetAndWait",&gunModController::resetAndWait,"")
        .def("getMainState",&gunModController::getMainState,"")
        .def("getErrorState",&gunModController::getErrorState,"")
        .def("setOff",&gunModController::setOff,"")
        .def("setStandby",&gunModController::setStandby,"")
        .def("setHVOn",&gunModController::setHVOn,"")
        .def("setRFOn",&gunModController::setRFOn,"")
        ;

    enum_<rfModStructs::GUN_MOD_STATE>("GUN_MOD_STATE","GUN_MOD_STATE: a named integer giving the state of the GUN Modulator")
        .value("NOT_CONNECTED",      rfModStructs::GUN_MOD_STATE::NOT_CONNECTED)
        .value("STANDYBY_INTERLOCK", rfModStructs::GUN_MOD_STATE::STANDYBY_INTERLOCK)
        .value("OFF",                rfModStructs::GUN_MOD_STATE::OFF)
        .value("OFF_REQUEST",        rfModStructs::GUN_MOD_STATE::OFF_REQUEST)
        .value("HV_INTERLOCK",       rfModStructs::GUN_MOD_STATE::HV_INTERLOCK)
        .value("STANDBY_REQUEST",    rfModStructs::GUN_MOD_STATE::STANDBY_REQUEST)
        .value("STANDBY",            rfModStructs::GUN_MOD_STATE::STANDBY)
        .value("HV_OFF_REQUEST",     rfModStructs::GUN_MOD_STATE::HV_OFF_REQUEST)
        .value("RF_ON_INTERLOCK",    rfModStructs::GUN_MOD_STATE::RF_ON_INTERLOCK)
        .value("HV_REQUEST",         rfModStructs::GUN_MOD_STATE::HV_REQUEST)
        .value("HV_ON",              rfModStructs::GUN_MOD_STATE::HV_ON)
        .value("RF_OFF_REQUEST",     rfModStructs::GUN_MOD_STATE::RF_OFF_REQUEST)
        .value("RF_ON_REQUEST",      rfModStructs::GUN_MOD_STATE::RF_ON_REQUEST)
        .value("RF_ON",               rfModStructs::GUN_MOD_STATE::RF_ON)
        .value("UNKNOWN_STATE",      rfModStructs::GUN_MOD_STATE::UNKNOWN_STATE)
        ;

    enum_<rfModStructs::GUN_MOD_ERR_STATE>("GUN_MOD_ERR_STATE",
                "GUN_MOD_ERR_STATE: a named integer giving the error state of the GUN Modulator")
        .value("NOT_CONNECTED",      rfModStructs::GUN_MOD_ERR_STATE::BAD)
        .value("STANDYBY_INTERLOCK", rfModStructs::GUN_MOD_ERR_STATE::GOOD)
        .value("OFF",                rfModStructs::GUN_MOD_ERR_STATE::UNKNOWN)
        ;

    enum_<rfModStructs::HOLD_RF_ON_STATE>("HOLD_RF_ON_STATE",
                "HOLD_RF_ON_STATE: a named integer giving the error state of the GUN Modulator")
        .value("MANUAL_RF",  rfModStructs::HOLD_RF_ON_STATE::MANUAL_RF)
        .value("HOLD_RF_ON", rfModStructs::HOLD_RF_ON_STATE::HOLD_RF_ON)
        .value("HOLD_RF_ON_CON", rfModStructs::HOLD_RF_ON_STATE::HOLD_RF_ON_CON)
        .value("UNKNOWN_HOLD_RF_ON_STATE", rfModStructs::HOLD_RF_ON_STATE::UNKNOWN_HOLD_RF_ON_STATE)
        ;

    boost::python::class_<rfModStructs::gunModObject,boost::noncopyable>
        ("gunModObject","gunModObject Doc String", boost::python::no_init)
        .def_readonly("name",          &rfModStructs::gunModObject::name,"Modulator Object Name")
        .def_readonly("pvRoot",        &rfModStructs::gunModObject::pvRoot,"PV root")
        .def_readonly("MagPs1CurrRead",&rfModStructs::gunModObject::MagPs1CurrRead,"")
        .def_readonly("MagPs2CurrRead",&rfModStructs::gunModObject::MagPs2CurrRead,"")
        .def_readonly("MagPs3CurrRead",&rfModStructs::gunModObject::MagPs3CurrRead,"")
        .def_readonly("MagPs4CurrRead",&rfModStructs::gunModObject::MagPs4CurrRead,"")
        .def_readonly("MagPs1VoltRead",&rfModStructs::gunModObject::MagPs1VoltRead,"")
        .def_readonly("MagPs2VoltRead",&rfModStructs::gunModObject::MagPs2VoltRead,"")
        .def_readonly("MagPs3VoltRead",&rfModStructs::gunModObject::MagPs3VoltRead,"")
        .def_readonly("MagPs4VoltRead",&rfModStructs::gunModObject::MagPs4VoltRead,"")
        .def_readonly("HvPs1VoltRead", &rfModStructs::gunModObject::HvPs1VoltRead,"")
        .def_readonly("HvPs2VoltRead", &rfModStructs::gunModObject::HvPs2VoltRead,"")
        .def_readonly("HvPs3VoltRead", &rfModStructs::gunModObject::HvPs3VoltRead,"")
        .def_readonly("HvPs1CurrRead", &rfModStructs::gunModObject::HvPs1CurrRead,"")
        .def_readonly("HvPs2CurrRead", &rfModStructs::gunModObject::HvPs2CurrRead,"")
        .def_readonly("HvPs3CurrRead", &rfModStructs::gunModObject::HvPs3CurrRead,"")
        .def_readonly("PrfSet",        &rfModStructs::gunModObject::PrfSet,"")
        .def_readonly("PrfRead",       &rfModStructs::gunModObject::PrfRead,"")
        .def_readonly("CtRead",        &rfModStructs::gunModObject::CtRead,"")
        .def_readonly("CvdRead",       &rfModStructs::gunModObject::CvdRead,"")
        .def_readonly("PlswthRead",    &rfModStructs::gunModObject::PlswthRead,"")
        .def_readonly("PlswthFwhmRead",&rfModStructs::gunModObject::PlswthFwhmRead,"")
        .def_readonly("ionp",          &rfModStructs::gunModObject::ionp,"")
        .def_readonly("main_state",    &rfModStructs::gunModObject::main_state,"")
        .def_readonly("ilock1",        &rfModStructs::gunModObject::ilock1,"")
        .def_readonly("ilock2",        &rfModStructs::gunModObject::ilock2,"")
        .def_readonly("ilock3",        &rfModStructs::gunModObject::ilock3,"")
        .def_readonly("ilock4",        &rfModStructs::gunModObject::ilock4,"")
        .def_readonly("ilock5",        &rfModStructs::gunModObject::ilock5,"")
        .def_readonly("hex_state_str", &rfModStructs::gunModObject::hex_state_str,"")
        .def_readonly("hex_state_message",&rfModStructs::gunModObject::hex_state_message,"")
        .def_readonly("error_state",      &rfModStructs::gunModObject::error_state,"")
        .def_readonly("hold_rf_on_state",&rfModStructs::gunModObject::hold_rf_on_state,"")
        ;

    boost::python::class_<rfModStructs::l01ModObject,boost::noncopyable>
        ("l01ModObject","l01ModObject Doc String", boost::python::no_init)
        .def_readonly("name",          &rfModStructs::l01ModObject::name,"Modulator Object Name")
        .def_readonly("pvRoot",        &rfModStructs::l01ModObject::pvRoot,"PV root")
        .def_readonly("heater_voltage",&rfModStructs::l01ModObject::heater_voltage,"")
        .def_readonly("heater_current",&rfModStructs::l01ModObject::heater_current,"")
        .def_readonly("reset_voltage",&rfModStructs::l01ModObject::reset_voltage,"")
        .def_readonly("reset_current",&rfModStructs::l01ModObject::reset_current,"")
        .def_readonly("hvps_current_read",&rfModStructs::l01ModObject::hvps_current_read,"")
        .def_readonly("ion_pump_voltage",&rfModStructs::l01ModObject::ion_pump_voltage,"")
        .def_readonly("ion_pump_current",&rfModStructs::l01ModObject::ion_pump_current,"")
        .def_readonly("supply_pressure",&rfModStructs::l01ModObject::supply_pressure,"")
        .def_readonly("return_temp",&rfModStructs::l01ModObject::return_temp,"")
        .def_readonly("return_pressure",&rfModStructs::l01ModObject::return_pressure,"")
        .def_readonly("body_flow_rate",&rfModStructs::l01ModObject::body_flow_rate,"")
        .def_readonly("collector_flow_rate",&rfModStructs::l01ModObject::collector_flow_rate,"")
        .def_readonly("solenoid_flow_rate",&rfModStructs::l01ModObject::solenoid_flow_rate,"")
        .def_readonly("tank_flow_rate",&rfModStructs::l01ModObject::tank_flow_rate,"")
        .def_readonly("collector_return_rate",&rfModStructs::l01ModObject::collector_return_rate,"")
        .def_readonly("body_return_temperature",&rfModStructs::l01ModObject::body_return_temperature,"")
        .def_readonly("hvps_voltage_read",&rfModStructs::l01ModObject::hvps_voltage_read,"")
        .def_readonly("solenoid_1_voltage",&rfModStructs::l01ModObject::solenoid_1_voltage,"")
        .def_readonly("solenoid_2_voltage",&rfModStructs::l01ModObject::solenoid_2_voltage,"")
        .def_readonly("solenoid_3_voltage",&rfModStructs::l01ModObject::solenoid_3_voltage,"")
        .def_readonly("solenoid_1_current",&rfModStructs::l01ModObject::solenoid_1_current,"")
        .def_readonly("solenoid_2_current",&rfModStructs::l01ModObject::solenoid_2_current,"")
        .def_readonly("solenoid_3_current",&rfModStructs::l01ModObject::solenoid_3_current,"")
        .def_readonly("main_state",&rfModStructs::l01ModObject::main_state,"")
        .def_readonly("state",&rfModStructs::l01ModObject::main_state,"")
        .def_readonly("hvps_voltage_set_read",&rfModStructs::l01ModObject::hvps_voltage_set_read,"")
        .def_readonly("hvps_voltage_low_alarm_set_read",&rfModStructs::l01ModObject::hvps_voltage_low_alarm_set_read,"")
        .def_readonly("hvps_voltage_hi_alarm_set_read",&rfModStructs::l01ModObject::hvps_voltage_hi_alarm_set_read,"")
        .def_readonly("l01_fault",&rfModStructs::l01ModObject::l01_fault,"")
        .def_readonly("modErrorWords",&rfModStructs::l01ModObject::modErrorWordsL,"")
        .def_readonly("modErrorDescs",&rfModStructs::l01ModObject::modErrorDescsL,"")
        .def_readonly("hold_rf_on_state",&rfModStructs::l01ModObject::hold_rf_on_state,"")
        ;

}
#endif // VCRFMOD_H_
