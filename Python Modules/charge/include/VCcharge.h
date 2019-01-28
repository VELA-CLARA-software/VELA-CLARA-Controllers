#ifndef vela_Qcharge_CONTROLLER_H
#define vela_Qcharge_CONTROLLER_H

// project
#include "chargeController.h"
#include "chargeStructs.h"
#include "VCheader.h"
#include "VCbase.h"
#include "configDefinitions.h"
// stl
#include <string>
#include <vector>
#include <map>
#include <utility>
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
//______________________________________________________________________________
class VCcharge : public VCbase
{
    public:
        VCcharge();
        ~VCcharge();

        chargeController & virtual_VELA_INJ_Charge_Controller();
        chargeController & offline_VELA_INJ_Charge_Controller();
        chargeController & physical_VELA_INJ_Charge_Controller();
        chargeController & virtual_VELA_BA1_Charge_Controller();
        chargeController & offline_VELA_BA1_Charge_Controller();
        chargeController & physical_VELA_BA1_Charge_Controller();
        chargeController & virtual_VELA_BA2_Charge_Controller();
        chargeController & offline_VELA_BA2_Charge_Controller();
        chargeController & physical_VELA_BA2_Charge_Controller();
        chargeController & virtual_CLARA_PH1_Charge_Controller();
        chargeController & offline_CLARA_PH1_Charge_Controller();
        chargeController & physical_CLARA_PH1_Charge_Controller();
        chargeController & virtual_C2B_Charge_Controller();
        chargeController & offline_C2B_Charge_Controller();
        chargeController & physical_C2B_Charge_Controller();
        chargeController & virtual_Charge_Controller();
        chargeController & offline_Charge_Controller();
        chargeController & physical_Charge_Controller();
        chargeController & getChargeController( HWC_ENUM::MACHINE_MODE mode, HWC_ENUM::MACHINE_AREA area );

    protected:
    private:

        std::map<chargeController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        chargeController& getController(chargeController*& cont,
                                       const std::string& conf1,
                                       const std::string& name,
                                       const bool shouldEPICS,
                                       const bool shouldVM,
                                       const HWC_ENUM::MACHINE_AREA myMachineArea);

        chargeController * virtual_VELA_INJ_Charge_Controller_Obj;
        chargeController * offline_VELA_INJ_Charge_Controller_Obj;
        chargeController * physical_VELA_INJ_Charge_Controller_Obj;
        chargeController * virtual_VELA_BA1_Charge_Controller_Obj;
        chargeController * offline_VELA_BA1_Charge_Controller_Obj;
        chargeController * physical_VELA_BA1_Charge_Controller_Obj;
        chargeController * virtual_VELA_BA2_Charge_Controller_Obj;
        chargeController * offline_VELA_BA2_Charge_Controller_Obj;
        chargeController * physical_VELA_BA2_Charge_Controller_Obj;
        chargeController * virtual_CLARA_PH1_Charge_Controller_Obj;
        chargeController * offline_CLARA_PH1_Charge_Controller_Obj;
        chargeController * physical_CLARA_PH1_Charge_Controller_Obj;
        chargeController * virtual_C2B_Charge_Controller_Obj;
        chargeController * offline_C2B_Charge_Controller_Obj;
        chargeController * physical_C2B_Charge_Controller_Obj;
        chargeController * virtual_Charge_Controller_Obj;
        chargeController * offline_Charge_Controller_Obj;
        chargeController * physical_Charge_Controller_Obj;
};

#ifdef BUILD_DLL
#define BOOST_LIB_DIAGNOSTIC

using namespace boost::python;

BOOST_PYTHON_MODULE( VELA_CLARA_Charge_Control )
{
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    /// Include ALL the enums you want to expose to Python

    enum_<chargeStructs::CHARGE_DIAG_TYPE>("CHARGE_DIAG_TYPE")
            .value("WCM",                   chargeStructs::CHARGE_DIAG_TYPE::WCM                   )
            .value("VELA_ICT1",             chargeStructs::CHARGE_DIAG_TYPE::VELA_ICT1             )
            .value("ED_FCUP",               chargeStructs::CHARGE_DIAG_TYPE::ED_FCUP               )
            .value("VELA_ICT2",             chargeStructs::CHARGE_DIAG_TYPE::VELA_ICT2             )
            .value("S02_FCUP",              chargeStructs::CHARGE_DIAG_TYPE::S02_FCUP              )
            .value("SP1_FCUP",              chargeStructs::CHARGE_DIAG_TYPE::SP1_FCUP              )
            .value("UNKNOWN_CHARGE_DIAG",   chargeStructs::CHARGE_DIAG_TYPE::UNKNOWN_CHARGE_DIAG   )
            ;

    boost::python::class_< boost::circular_buffer< double > >("Circular buffer definition for python ", boost::python::no_init)
        .def(vector_indexing_suite< boost::circular_buffer< double > >())
        ;

    char const* dataObjectStructString = "This struct contains data a given charge data object.";
    boost::python::class_<chargeStructs::dataObject,boost::noncopyable>
        ("dataObject",dataObjectStructString,boost::python::no_init)
        .def_readonly("pvRoot",        &chargeStructs::dataObject::pvRoot        )
        .def_readonly("name",          &chargeStructs::dataObject::name          )
        .def_readonly("diagType",      &chargeStructs::dataObject::diagType      )
        .def_readonly("isMonitoring",  &chargeStructs::dataObject::isMonitoring  )
        .def_readonly("charge",        &chargeStructs::dataObject::charge        )
        .def_readonly("voltage",       &chargeStructs::dataObject::voltage       )
        .def_readonly("timestamp",     &chargeStructs::dataObject::timeStamp     )
//        .def_readonly("chargeVec",     &chargeStructs::dataObject::chargeVec     )
//        .def_readonly("voltageVec",    &chargeStructs::dataObject::voltageVec    )
        .def_readonly("chargeBuffer",  &chargeStructs::dataObject::chargeBuffer  )
        .def_readonly("voltageBuffer", &chargeStructs::dataObject::voltageBuffer )
        .def_readonly("timeStamps",    &chargeStructs::dataObject::timeStamps    )
        .def_readonly("strTimeStamps", &chargeStructs::dataObject::strTimeStamps )
        ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    char const* getChargeDataStructDocString = "Returns the charge data struct for str(chargeName). See documentation on the dataObject struct for what this contains.";
    char const* setBufferSizeDocString = "Set size of buffer for continuous monitor.";
    char const* restartContinuousMonitoringDocString = "Restarts continuous monitoring of charge parameters. !!!!!!!WILL RESET ALL VALUES!!!!!!!.";
    char const* cancelDataMonitorsDocString = "Cancels monitoring of charge parameters.";
    char const* monitorForNShotsDocString = "Monitors str(name) for N shots.";
    char const* getBufferSizeDocString = "Returns the size of the buffer.";
    char const* getDiagTypeDocString = "Returns the diagnostic type as a CHARGE_PV_TYPE for str(name).";
    char const* getDiagTypeStrDocString = "Returns the diagnostic type as a string for str(name).";
    char const* getChargeVectorDocString = "Returns a vector of doubles for the charge of str(chargeName), after using monitorForNShots.";
    char const* getVoltageVectorDocString = "Returns a vector of doubles for the voltage of str(chargeName), after using monitorForNShots.";
    char const* getWCMChargeVectorDocString = "Returns a vector of doubles for WCM charge, after using monitorForNShots.";
    char const* getWCMVoltageVectorDocString = "Returns a vector of doubles for WCM voltage, after using monitorForNShots.";
    char const* getS02FCUPChargeVectorDocString = "Returns a vector of doubles for S02-FCUP charge, after using monitorForNShots.";
    char const* getS02FCUPVoltageVectorDocString = "Returns a vector of doubles for S02-FCUP voltage, after using monitorForNShots.";
    char const* getChargeBufferDocString = "Returns a vector containing the last (buffersize) charges for str(name).";
    char const* getVoltageBufferDocString = "Returns a vector containing the last (buffersize) voltages for str(name).";
    char const* getWCMChargeBufferDocString = "Returns a vector containing the last (buffersize) charges for WCM.";
    char const* getWCMVoltageBufferDocString = "Returns a vector containing the last (buffersize) voltages for WCM.";
    char const* getS02FCUPChargeBufferDocString = "Returns a vector containing the last (buffersize) charges for S02-FCUP.";
    char const* getS02FCUPVoltageBufferDocString = "Returns a vector containing the last (buffersize) voltages for S02-FCUP.";
    char const* getTimeStampsDocString = "Returns a vector containing the timestamps as doubles for str(name) - these are defined in the config file.\n";
    char const* getStrTimeStampsDocString = "Returns a vector containing the timestamps as strings (if that's your thing) for str(charge) - these are defined in the config file.\n";
    char const* getChargeDiagnosticNamesDocString = "Returns all charge diagnostic names defined in config file";
    char const* getChargePVsDocString = "Returns all charge PVs defined in config file";
    char const* getChargeDocString = "Returns current charge of str(name)";
    char const* getVoltageDocString = "Returns current voltage of str(name)";
    char const* getWCMChargeDocString = "Returns current value of WCM charge";
    char const* getWCMVoltageDocString = "Returns current voltage of WCM";
    char const* getS02FCUPChargeDocString = "Returns current value of S02-FCUP charge";
    char const* getS02FCUPVoltageDocString = "Returns current voltage of S02-FCUP";
    char const* isChargeBufferFullDocString = "Returns true if the charge buffer for str(name) is full";
    char const* isVoltageBufferFullDocString = "Returns true if the voltage buffer for str(name) is full";
    char const* isChargeBufferNotFullDocString = "Returns true if the charge buffer for str(name) is not full";
    char const* isVoltageBufferNotFullDocString = "Returns true if the voltage buffer for str(name) is not full";
    char const* isWCMBufferFullDocString = "Returns true if the WCM buffer is full";
    char const* isWCMBufferNotFullDocString = "Returns true if the WCM buffer is not full";
    char const* isS02FCUPBufferFullDocString = "Returns true if the S02-FCUP buffer is full";
    char const* isS02FCUPBufferNotFullDocString = "Returns true if the S02-FCUP buffer is not full";
    char const* isSP1FCUPBufferFullDocString = "Returns true if the SP1-FCUP buffer is full";
    char const* isSP1FCUPBufferNotFullDocString = "Returns true if the SP1-FCUP buffer is not full";
    char const* isNotMonitoringChargeDocString = "Returns true if str(name) is not being monitored";
    char const* isMonitoringWCMDocString = "Returns true if WCM is being monitored";
    char const* isNotMonitoringWCMDocString = "Returns true if WCM is not being monitored";
    char const* isMonitoringS02FCUPDocString = "Returns true if S02-FCUP is being monitored";
    char const* isNotMonitoringS02FCUPDocString = "Returns true if S02-FCUP is not being monitored";

	boost::python::class_<chargeController, boost::python::bases<controller>, boost::noncopyable>
            ("chargeController","chargeController Doc String",boost::python::no_init)
            .def("getChargeDataStruct",         &chargeController::getChargeDataStruct, (boost::python::arg("name")), getChargeDataStructDocString, return_value_policy<reference_existing_object>() )
            .def("get_CA_PEND_IO_TIMEOUT",      &chargeController::get_CA_PEND_IO_TIMEOUT                                                   )
            .def("set_CA_PEND_IO_TIMEOUT",      &chargeController::set_CA_PEND_IO_TIMEOUT                                                   )
            .def("setBufferSize",               &chargeController::setBufferSize, (boost::python::arg("size_t")), setBufferSizeDocString                   )
//            .def("restartContinuousMonitoring", &chargeController::restartContinuousMonitoring, restartContinuousMonitoringDocString        )
//            .def("monitorForNShots",            &chargeController::monitorForNShots, (arg("name"),arg("size_t")), monitorForNShotsDocString )
//            .def("cancelDataMonitors",          &chargeController::cancelDataMonitors, cancelDataMonitorsDocString                          )
            .def("getBufferSize",               &chargeController::getBufferSize, (boost::python::arg("name")), getBufferSizeDocString                     )
            .def("getDiagType",                 &chargeController::getDiagType, (boost::python::arg("name")), getDiagTypeDocString                         )
            .def("getDiagTypeStr",              &chargeController::getDiagTypeStr, (boost::python::arg("name")), getDiagTypeStrDocString                   )
            .def("getChargeBuffer",             &chargeController::getChargeBuffer_Py, (boost::python::arg("name")), getChargeBufferDocString              )
            .def("getVoltageBuffer",            &chargeController::getVoltageBuffer_Py, (boost::python::arg("name")), getVoltageBufferDocString            )
            .def("getWCMChargeBuffer",          &chargeController::getWCMChargeBuffer_Py, getWCMChargeBufferDocString                                      )
            .def("getWCMVoltageBuffer",         &chargeController::getWCMVoltageBuffer_Py, getWCMVoltageBufferDocString                                    )
            .def("getS02FCUPChargeBuffer",      &chargeController::getS02FCUPChargeBuffer_Py, getS02FCUPChargeBufferDocString                              )
            .def("getSP1FCUPChargeBuffer",      &chargeController::getSP1FCUPChargeBuffer_Py, getS02FCUPChargeBufferDocString                              )
            .def("getS02FCUPVoltageBuffer",     &chargeController::getS02FCUPVoltageBuffer_Py, getS02FCUPVoltageBufferDocString                            )
            .def("getSP1FCUPVoltageBuffer",     &chargeController::getSP1FCUPVoltageBuffer_Py, getS02FCUPVoltageBufferDocString                            )
            .def("getTimeStamps",               &chargeController::getTimeStamps_Py, (boost::python::arg("name")), getTimeStampsDocString                  )
            .def("getStrTimeStamps",            &chargeController::getStrTimeStamps_Py, (boost::python::arg("name")), getStrTimeStampsDocString            )
            .def("getChargeDiagnosticNames",    &chargeController::getChargeDiagnosticNames_Py, getChargeDiagnosticNamesDocString                          )
            .def("getChargePVs",                &chargeController::getChargePVs_Py, getChargePVsDocString                                                  )
            .def("getCharge",                   &chargeController::getCharge, (boost::python::arg("name")), getChargeDocString                             )
            .def("getVoltage",                  &chargeController::getVoltage, (boost::python::arg("name")), getVoltageDocString                           )
            .def("getWCMCharge",                &chargeController::getWCMCharge, getWCMChargeDocString                                                     )
            .def("getWCMVoltage",               &chargeController::getWCMVoltage, getWCMVoltageDocString                                                   )
            .def("getS02FCUPCharge",            &chargeController::getS02FCUPCharge, getS02FCUPChargeDocString                                             )
            .def("getSP1FCUPCharge",            &chargeController::getSP1FCUPCharge, getS02FCUPChargeDocString                                             )
            .def("getS02FCUPVoltage",           &chargeController::getS02FCUPVoltage, getS02FCUPVoltageDocString                                           )
            .def("getSP1FCUPVoltage",           &chargeController::getSP1FCUPVoltage, getS02FCUPVoltageDocString                                           )
            .def("isChargeBufferFull",          &chargeController::isChargeBufferFull, (boost::python::arg("name")), isChargeBufferFullDocString           )
            .def("isVoltageBufferFull",         &chargeController::isVoltageBufferFull, (boost::python::arg("name")), isVoltageBufferFullDocString         )
            .def("isChargeBufferNotFull",       &chargeController::isChargeBufferNotFull, (boost::python::arg("name")), isChargeBufferNotFullDocString     )
            .def("isVoltageBufferNotFull",      &chargeController::isVoltageBufferNotFull, (boost::python::arg("name")), isVoltageBufferNotFullDocString   )
            .def("isWCMBufferFull",             &chargeController::isWCMBufferFull, isWCMBufferFullDocString                                               )
            .def("isWCMBufferNotFull",          &chargeController::isWCMBufferNotFull, isWCMBufferNotFullDocString                                         )
            .def("isS02FCUPBufferFull",         &chargeController::isS02FCUPBufferFull, isS02FCUPBufferFullDocString                                       )
            .def("isSP1FCUPBufferFull",         &chargeController::isSP1FCUPBufferFull, isSP1FCUPBufferFullDocString                                       )
            .def("isS02FCUPBufferNotFull",      &chargeController::isS02FCUPBufferNotFull, isS02FCUPBufferNotFullDocString                                 )
            .def("isSP1FCUPBufferNotFull",      &chargeController::isSP1FCUPBufferNotFull, isSP1FCUPBufferNotFullDocString                                 )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",            &chargeController::debugMessagesOff                       )
            .def("debugMessagesOn",             &chargeController::debugMessagesOn                        )
            .def("messagesOff",                 &chargeController::messagesOff                            )
            .def("messagesOn",                  &chargeController::messagesOn                             )
            .def("silence",                     &chargeController::silence                                )
            .def("verbose",                     &chargeController::verbose                                )
		;

    boost::python::class_<VCcharge,boost::python::bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Charge_Controller",   &VCcharge::virtual_VELA_INJ_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Charge_Controller",   &VCcharge::offline_VELA_INJ_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Charge_Controller",  &VCcharge::physical_VELA_INJ_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Charge_Controller",   &VCcharge::virtual_VELA_BA1_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Charge_Controller",   &VCcharge::offline_VELA_BA1_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Charge_Controller",  &VCcharge::physical_VELA_BA1_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Charge_Controller",   &VCcharge::virtual_VELA_BA2_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Charge_Controller",   &VCcharge::offline_VELA_BA2_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Charge_Controller",  &VCcharge::physical_VELA_BA2_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_PH1_Charge_Controller",  &VCcharge::virtual_CLARA_PH1_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_Charge_Controller",  &VCcharge::offline_CLARA_PH1_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_Charge_Controller", &VCcharge::physical_CLARA_PH1_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_C2B_Charge_Controller",        &VCcharge::virtual_C2B_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_C2B_Charge_Controller",        &VCcharge::offline_C2B_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_C2B_Charge_Controller",       &VCcharge::physical_C2B_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_Charge_Controller",            &VCcharge::virtual_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_Charge_Controller",            &VCcharge::offline_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_Charge_Controller",           &VCcharge::physical_Charge_Controller, return_value_policy<reference_existing_object>())
        .def("getChargeController",                  &VCcharge::getChargeController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL
#endif // velaChrageScopeController_H

