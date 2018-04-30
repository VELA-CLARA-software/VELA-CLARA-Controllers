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
//#include <boost/python/detail/wrap_python.hpp>
//#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
//#define BOOST_LIB_DIAGNOSTIC
//#include <boost/config.hpp>
//#include <boost/python.hpp>
//#include <boost/python/class.hpp>
//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python/object/function.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/docstring_options.hpp>
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
        chargeController & getChargeController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area );

    protected:
    private:

        std::map<chargeController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        chargeController& getController(chargeController*& cont,
                                       const std::string& conf1,
                                       const std::string& conf2,
                                       const std::string& name,
                                       const bool shouldVM,
                                       const bool shouldEPICS,
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

    boost::python::type_info info = boost::python::type_id< std::map< chargeStructs::charge_PV_TYPE, bool > > ();
    const boost::python::converter::registration* reg = boost::python::converter::registry::query(info);

    info = boost::python::type_id< std::vector< std::vector< double > > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::vector< std::vector< double > > >("std_vector_double")
            .def( vector_indexing_suite< std::vector< std::vector< double > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::vector< std::vector< double > > >("std_vector_double")
            .def( vector_indexing_suite< std::vector< std::vector< double > > >())
            ;
    }

    info = boost::python::type_id< std::map< chargeStructs::charge_PV_TYPE, bool > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >("std_charge_map_bool")
            .def( map_indexing_suite< std::map< chargeStructs::charge_PV_TYPE, bool > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >("std_charge_map_bool")
            .def( map_indexing_suite< std::map< chargeStructs::charge_PV_TYPE, bool > >())
            ;
    }

    info = boost::python::type_id< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >("std_charge_map_vector_double")
            .def( map_indexing_suite< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >("std_charge_map_vector_double")
            .def( map_indexing_suite< std::map< chargeStructs::charge_PV_TYPE, std::vector< double > > >())
            ;
    }

    info = boost::python::type_id< std::map< chargeStructs::charge_PV_TYPE, std::vector< std::vector< double > > > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< chargeStructs::charge_PV_TYPE, std::vector< std::vector< double > > > >("std_charge_map_vector_vector_double")
            .def( map_indexing_suite< std::map< chargeStructs::charge_PV_TYPE, std::vector< std::vector< double > > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< chargeStructs::charge_PV_TYPE, std::vector< std::vector< double > > > >("std_charge_map_vector_vector_double")
            .def( map_indexing_suite< std::map< chargeStructs::charge_PV_TYPE, std::vector< std::vector< double > > > >())
            ;
    }

    enum_<chargeStructs::charge_PV_TYPE>("charge_PV_TYPE")
            .value("TR1", chargeStructs::charge_PV_TYPE::TR1 )
            .value("TR2", chargeStructs::charge_PV_TYPE::TR2 )
            .value("TR3", chargeStructs::charge_PV_TYPE::TR3 )
            .value("TR4", chargeStructs::charge_PV_TYPE::TR4 )
            .value("P1",  chargeStructs::charge_PV_TYPE::P1  )
            .value("P2",  chargeStructs::charge_PV_TYPE::P2  )
            .value("P3",  chargeStructs::charge_PV_TYPE::P3  )
            .value("P4",  chargeStructs::charge_PV_TYPE::P4  )
            .value("UNKNOWN",  chargeStructs::charge_PV_TYPE::UNKNOWN  )
            ;

    enum_<chargeStructs::charge_NAME>("charge_NAME")
            .value("CLARAcharge01",       chargeStructs::charge_NAME::CLARAcharge01       )
            .value("VELAcharge02",        chargeStructs::charge_NAME::VELAcharge02        )
            .value("UNKNOWN_charge_NAME", chargeStructs::charge_NAME::UNKNOWN_charge_NAME )
            ;

    enum_<chargeStructs::DIAG_TYPE>("DIAG_TYPE")
            .value("WCM",     chargeStructs::DIAG_TYPE::WCM       )
            .value("ICT1",    chargeStructs::DIAG_TYPE::ICT2      )
            .value("ICT2",    chargeStructs::DIAG_TYPE::ICT2      )
            .value("FCUP",    chargeStructs::DIAG_TYPE::FCUP      )
            .value("ED_FCUP", chargeStructs::DIAG_TYPE::ED_FCUP   )
            ;
    char const* chargeNumObjectStructString = "This struct contains data for the four channels on the charge - e.g. p1 contains the P1 value from the charge.\n"
                                       "This will only contain real-time data if charge number data is being submitted to EPICS - you will need to check this on the charge.\n"
                                       "p1Vec contains a vector of values after monitorNumsForNShots, and numData contains a vector of vectors, with data from all four channels on the charge.\n"
                                       "Timestamps can also be accessed.";
    boost::python::class_<chargeStructs::chargeNumObject,boost::noncopyable>
        ("chargeNumObject",chargeNumObjectStructString,boost::python::no_init)
        .def_readonly("isMonitoringMap", &chargeStructs::chargeNumObject::isMonitoringMap)
        .def_readonly("name",            &chargeStructs::chargeNumObject::name           )
        .def_readonly("p1",              &chargeStructs::chargeNumObject::p1             )
        .def_readonly("p2",              &chargeStructs::chargeNumObject::p2             )
        .def_readonly("p3",              &chargeStructs::chargeNumObject::p3             )
        .def_readonly("p4",              &chargeStructs::chargeNumObject::p4             )
        .def_readonly("p1TimeStamp",     &chargeStructs::chargeNumObject::p1TimeStamp    )
        .def_readonly("p2TimeStamp",     &chargeStructs::chargeNumObject::p2TimeStamp    )
        .def_readonly("p3TimeStamp",     &chargeStructs::chargeNumObject::p3TimeStamp    )
        .def_readonly("p4TimeStamp",     &chargeStructs::chargeNumObject::p4TimeStamp    )
        .def_readonly("p1TimeStamps",    &chargeStructs::chargeNumObject::p1TimeStamps   )
        .def_readonly("p2TimeStamps",    &chargeStructs::chargeNumObject::p2TimeStamps   )
        .def_readonly("p3TimeStamps",    &chargeStructs::chargeNumObject::p3TimeStamps   )
        .def_readonly("p4TimeStamps",    &chargeStructs::chargeNumObject::p4TimeStamps   )
        .def_readonly("p1Vec",           &chargeStructs::chargeNumObject::p1Vec          )
        .def_readonly("p2Vec",           &chargeStructs::chargeNumObject::p2Vec          )
        .def_readonly("p3Vec",           &chargeStructs::chargeNumObject::p3Vec          )
        .def_readonly("p4Vec",           &chargeStructs::chargeNumObject::p4Vec          )
        .def_readonly("numData",         &chargeStructs::chargeNumObject::numData        )
        .def_readonly("numTimeStamps",   &chargeStructs::chargeNumObject::numTimeStamps  )
        .def_readonly("shotCounts",      &chargeStructs::chargeNumObject::shotCounts     )
        .def_readonly("buffer",          &chargeStructs::chargeNumObject::buffer         )
        ;

    char const* chargeTraceDataStructString = "This struct contains trace data for the four channels on the charge - e.g. tr1TraceData contains a vector of the EPICS PV for channel 1 on the charge.\n"
                                             "This will only contain real-time data if charge trace data is being submitted to EPICS - you will need to check this on the charge.\n";
                                             "traceData contains a vector of vectors, with data from all four channels on the charge.\n"
                                             "Timestamps can also be accessed.";
    boost::python::class_<chargeStructs::chargeTraceData,boost::noncopyable>
        ("chargeTraceData",chargeTraceDataStructString,boost::python::no_init)
        .def_readonly("isMonitoringMap", &chargeStructs::chargeTraceData::isMonitoringMap)
        .def_readonly("name",            &chargeStructs::chargeTraceData::name           )
        .def_readonly("pvRoot",          &chargeStructs::chargeTraceData::pvRoot         )
        .def_readonly("tr1TraceData",    &chargeStructs::chargeTraceData::tr1TraceData   )
        .def_readonly("tr2TraceData",    &chargeStructs::chargeTraceData::tr1TraceData   )
        .def_readonly("tr3TraceData",    &chargeStructs::chargeTraceData::tr1TraceData   )
        .def_readonly("tr4TraceData",    &chargeStructs::chargeTraceData::tr1TraceData   )
        .def_readonly("traceData",       &chargeStructs::chargeTraceData::traceData      )
        .def_readonly("tr1TimeStamps",   &chargeStructs::chargeTraceData::tr1TimeStamps  )
        .def_readonly("tr2TimeStamps",   &chargeStructs::chargeTraceData::tr2TimeStamps  )
        .def_readonly("tr3TimeStamps",   &chargeStructs::chargeTraceData::tr3TimeStamps  )
        .def_readonly("tr4TimeStamps",   &chargeStructs::chargeTraceData::tr4TimeStamps  )
        .def_readonly("shotCounts",      &chargeStructs::chargeTraceData::shotCounts     )
        .def_readonly("timebase",        &chargeStructs::chargeTraceData::timebase       )
        .def_readonly("timeStamps",      &chargeStructs::chargeTraceData::timeStamps     )
        ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    char const* getchargeTraceDataStructString = "Returns the charge trace data struct for str(chargeName). See documentation on the chargeTraceData struct for what this contains.";
    char const* getchargeNumDataStructString = "Returns the charge number data struct for str(chargeName). See documentation on the chargeNumData struct for what this contains.";
    char const* isMonitoringTraceDocString = "Returns true if str(chargeName) traces are being monitored - these are defined in the config file.";
    char const* isMonitoringNumDocString = "Returns true if str(chargeName) P values are being monitored - these are defined in the config file.";
    char const* isNotMonitoringTraceDocString = "Returns true if str(chargeName) traces are not being monitored - these are defined in the config file.";
    char const* isNotMonitoringNumDocString = "Returns true if str(chargeName) P values are not being monitored - these are defined in the config file.";
    char const* setBufferSizeDocString = "Set size of buffer for continuous monitor.";
    char const* setNumBufferSizeDocString = "Set size of buffer for continuous P values monitor.";
    char const* setTraceBufferSizeDocString = "Set size of buffer for continuous trace monitor.";
    char const* restartMonitoringDocString = "Restarts continuous monitoring of charge parameters. !!!!!!!WILL RESET ALL VALUES!!!!!!!.";
    char const* setTimebaseDocString = "Sets the timebase (number of points in trace / time window of trace).";
    char const* getTimebaseDocString = "Returns the timebase (number of points in trace / time window of trace).";
    char const* getBufferSizeDocString = "Returns the size of the buffer.";
    char const* getDiagTypeDocString = "Returns the diagnostic type for a given channel on the charge (Trace or number).";
    char const* getDiagTypeStrDocString = "Returns the diagnostic type for a given channel on the charge (Trace or number).";
    char const* getchargeNumsDocString = "Returns a vector of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), after using monitorNumsForNShots.";
    char const* getchargeP1VecDocString = "Returns a vector of doubles for str(chargeName), for channel P1, after using monitorNumsForNShots.";
    char const* getchargeP2VecDocString = "Returns a vector of doubles for str(chargeName), for channel P2, after using monitorNumsForNShots.";
    char const* getchargeP3VecDocString = "Returns a vector of doubles for str(chargeName), for channel P3, after using monitorNumsForNShots.";
    char const* getchargeP4VecDocString = "Returns a vector of doubles for str(chargeName), for channel P4, after using monitorNumsForNShots.";
    char const* getchargeP1DocString = "Returns a double containing the value for channel P1 for str(chargeName).";
    char const* getchargeP2DocString = "Returns a double containing the value for channel P2 for str(chargeName).";
    char const* getchargeP3DocString = "Returns a double containing the value for channel P3 for str(chargeName).";
    char const* getchargeP4DocString = "Returns a double containing the value for channel P4 for str(chargeName).";
    char const* getchargeNumBufferDocString = "Returns a vector containing the last (buffersize) values for a given channel for str(chargeName).";
    char const* getchargeTraceBufferDocString = "Returns a vector of vectors containing the last (buffersize) traces for a given channel for str(chargeName).";
    char const* getWCMQDocString = "Returns a double containing the current value for the WCM, provided that the WCM channel is defined in the config file.\n"
                          "This should work regardless of whether traces or P values are being submitted to EPICS (not for dark current measurements).";
    char const* getICT1QDocString = "Returns a double containing the current value for the ICT1, provided that the ICT1 channel is defined in the config file.\n"
                          "This should work regardless of whether traces or P values are being submitted to EPICS.";
    char const* getICT2QDocString = "Returns a double containing the current value for the ICT2, provided that the ICT2 channel is defined in the config file.\n"
                          "This should work regardless of whether traces or P values are being submitted to EPICS.";
    char const* getFCUPQDocString = "Returns a double containing the current value for the FCUP, provided that the FCUP channel is defined in the config file.\n"
                          "This should work regardless of whether traces or P values are being submitted to EPICS.";
    char const* getEDFCUPQDocString = "Returns a double containing the current value for the ED-FCUP, provided that the ED-FCUP channel is defined in the config file.\n"
                          "This should work regardless of whether traces or P values are being submitted to EPICS.";
    char const* getchargeTracesDocString = "Returns a vector of vectors of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), after using monitorTracesForNShots.";
    char const* getPartOfTraceDocString = "Returns a vector of vectors of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), of a user-specified portion of the trace (between part1 and part2), after using monitorTracesForNShots.";
    char const* getAreaUnderTracesDocString = "Returns a vector of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), containing the area under each trace.\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getAreaUnderPartOfTraceDocString = "Returns a vector of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), containing the area under a user-specified portion of the trace (between part1 and part2).\n"
                                              "The user should specify part1 and part2 to be regions of the trace where there is no signal (it may be good to check the array size first).";
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getAvgNoiseDocString = "Returns a vector of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), containing the average noise in a user-specified portion of the trace (between part1 and part2).\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getMaxOfTracesDocString = "Returns a vector of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), containing the maximum value of each trace\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getMinOfTracesDocString = "Returns a vector of doubles for str(chargeName), for the channel charge_PV_TYPE(pvType), containing the minimum value of each trace\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getTimeStampsDocString = "Returns a vector containing the timestamps as doubles for str(charge), for the channel charge_PV_TYPE(pvType) - these are defined in the config file.\n"
                                         "To be used in conjunction with functions monitorNumsForNShots or monitorTracesForNShots.";
    char const* getStrTimeStampsDocString = "Returns a vector containing the timestamps as strings (if that's your thing) for str(charge), for the channel charge_PV_TYPE(pvType) - these are defined in the config file.\n"
                                         "To be used in conjunction with functions monitorNumsForNShots or monitorTracesForNShots.";
    char const* monitorNumsDocString = "Monitors P values (see charge) for str(chargeName) - these should be defined in the config file. This will fill four vectors of doubles with charge data.\n"
                                      "Data can be accessed using getchargeNums, or getchargeP(1/2/3/4)Vec.\n";
    char const* monitorTracesDocString = "Monitors traces (see charge) for str(chargeName) - these should be defined in the config file. This will fill four vectors of vectors of doubles with charge trace data.\n"
                                      "Data can be accessed using getchargeTraces - see documentation.\n";
    char const* monitorATraceDocString = "Monitors a specific trace (see charge) for channel (pvType) of str(chargeName) - these should be defined in the config file. This will fill a vectors of vectors of doubles with charge trace data.\n"
                                      "Data can be accessed using getchargeTraces - see documentation.\n";
    char const* monitorANumDocString = "Monitors a specific P value (see charge) for channel (pvType) of str(chargeName) - these should be defined in the config file. This will fill a vector of doubles with charge data.\n"
                                      "Data can be accessed using getchargeTraces - see documentation.\n";

	boost::python::class_<chargeController, boost::python::bases<controller>, boost::noncopyable>
            ("chargeController","chargeController Doc String",boost::python::no_init)
            .def("get_CA_PEND_IO_TIMEOUT",          &chargeController::get_CA_PEND_IO_TIMEOUT                    )
            .def("set_CA_PEND_IO_TIMEOUT",          &chargeController::set_CA_PEND_IO_TIMEOUT                    )
            .def("getchargeTraceDataStruct",         &chargeController::getchargeTraceDataStruct, getchargeTraceDataStructString, return_value_policy<reference_existing_object>())
            .def("getchargeNumDataStruct",           &chargeController::getchargeNumDataStruct, getchargeNumDataStructString, return_value_policy<reference_existing_object>()  )
            .def("isMonitoringchargeTrace",          &chargeController::isMonitoringchargeTrace, isMonitoringTraceDocString           )
            .def("isMonitoringchargeNum",            &chargeController::isMonitoringchargeNum, isMonitoringNumDocString               )
            .def("isNotMonitoringchargeTrace",       &chargeController::isNotMonitoringchargeTrace, isNotMonitoringNumDocString       )
            .def("isNotMonitoringchargeNum",         &chargeController::isNotMonitoringchargeNum, isNotMonitoringNumDocString         )
            .def("setBufferSize",                   &chargeController::setBufferSize, setBufferSizeDocString                        )
            .def("setNumBufferSize",                &chargeController::setBufferSize, setNumBufferSizeDocString                     )
            .def("setTraceBufferSize",              &chargeController::setBufferSize, setTraceBufferSizeDocString                   )
            .def("restartContinuousMonitoring",     &chargeController::restartContinuousMonitoring, restartMonitoringDocString      )
            .def("setTimebase",                     &chargeController::setTimebase, setTimebaseDocString                            )
            .def("getTimebase",                     &chargeController::getTimebase, getTimebaseDocString                            )
            .def("getBufferSize",                   &chargeController::getBufferSize, getBufferSizeDocString                        )
            .def("getDiagType",                     &chargeController::getDiagType, getDiagTypeDocString                            )
            .def("getDiagTypeStr",                  &chargeController::getDiagTypeStr, getDiagTypeStrDocString                      )
            .def("getchargeNums",                    &chargeController::getchargeNums_Py, getchargeNumsDocString                       )
            .def("getchargeP1Vec",                   &chargeController::getchargeP1Vec_Py, getchargeP1VecDocString                     )
            .def("getchargeP2Vec",                   &chargeController::getchargeP2Vec_Py, getchargeP2VecDocString                     )
            .def("getchargeP3Vec",                   &chargeController::getchargeP3Vec_Py, getchargeP3VecDocString                     )
            .def("getchargeP4Vec",                   &chargeController::getchargeP4Vec_Py, getchargeP4VecDocString                     )
            .def("getchargeNumBuffer",               &chargeController::getchargeNumBuffer_Py, getchargeNumBufferDocString             )
            .def("getchargeP1Buffer",                &chargeController::getchargeP1Buffer_Py, getchargeNumBufferDocString              )
            .def("getchargeP2Buffer",                &chargeController::getchargeP2Buffer_Py, getchargeNumBufferDocString              )
            .def("getchargeP3Buffer",                &chargeController::getchargeP3Buffer_Py, getchargeNumBufferDocString              )
            .def("getchargeP4Buffer",                &chargeController::getchargeP4Buffer_Py, getchargeNumBufferDocString              )
            .def("getchargeTraceBuffer",             &chargeController::getchargeTraceBuffer_Py, getchargeTraceBufferDocString         )
            .def("getchargeTR1Buffer",               &chargeController::getchargeTR1Buffer_Py, getchargeTraceBufferDocString           )
            .def("getchargeTR2Buffer",               &chargeController::getchargeTR2Buffer_Py, getchargeTraceBufferDocString           )
            .def("getchargeTR3Buffer",               &chargeController::getchargeTR3Buffer_Py, getchargeTraceBufferDocString           )
            .def("getchargeTR4Buffer",               &chargeController::getchargeTR4Buffer_Py, getchargeTraceBufferDocString           )
            .def("getchargeP1",                      &chargeController::getchargeP1, getchargeP1DocString                              )
            .def("getchargeP2",                      &chargeController::getchargeP2, getchargeP2DocString                              )
            .def("getchargeP3",                      &chargeController::getchargeP3, getchargeP3DocString                              )
            .def("getchargeP4",                      &chargeController::getchargeP4, getchargeP4DocString                              )
            .def("getWCMQ",                         &chargeController::getWCMQ, getWCMQDocString                                    )
            .def("getICT1Q",                        &chargeController::getICT1Q, getICT1QDocString                                  )
            .def("getICT2Q",                        &chargeController::getICT2Q, getICT2QDocString                                  )
            .def("getFCUPQ",                        &chargeController::getFCUPQ, getFCUPQDocString                                  )
            .def("getEDFCUPQ",                      &chargeController::getEDFCUPQ, getEDFCUPQDocString                              )
            .def("getchargeTraces",                  &chargeController::getchargeTraces, getchargeTracesDocString                      )
            .def("getMinOfTraces",                  &chargeController::getMinOfTraces_Py, getMinOfTracesDocString                   )
            .def("getMaxOfTraces",                  &chargeController::getMaxOfTraces_Py, getMaxOfTracesDocString                   )
            .def("getAreaUnderTraces",              &chargeController::getAreaUnderTraces_Py, getAreaUnderTracesDocString           )
            .def("getAreaUnderPartOfTrace",         &chargeController::getAreaUnderPartOfTrace_Py, getAreaUnderPartOfTraceDocString )
            .def("getAvgNoise",                     &chargeController::getAvgNoise_Py, getAvgNoiseDocString                         )
            .def("getPartOfTrace",                  &chargeController::getPartOfTrace, getPartOfTraceDocString                      )
            .def("getTimeStamps",                   &chargeController::getTimeStamps_Py, getTimeStampsDocString                     )
            .def("getStrTimeStamps",                &chargeController::getStrTimeStamps_Py, getStrTimeStampsDocString               )
            .def("monitorNumsForNShots",            &chargeController::monitorNumsForNShots, monitorNumsDocString                   )
            .def("monitorANumForNShots",            &chargeController::monitorANumForNShots, monitorANumDocString                   )
            .def("monitorTracesForNShots",          &chargeController::monitorTracesForNShots, monitorTracesDocString               )
            .def("monitorATraceForNShots",          &chargeController::monitorATraceForNShots, monitorATraceDocString               )
            .def("getchargeNames",                   &chargeController::getchargeNames_Py                                             )
            .def("getchargePVs",                     &chargeController::getchargePVs_Py                                               )
            .def("getchargeTracePVs",                &chargeController::getchargeTracePVs_Py                                          )
            .def("getchargeNumPVs",                  &chargeController::getchargeNumPVs_Py                                            )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &chargeController::debugMessagesOff                       )
            .def("debugMessagesOn",                 &chargeController::debugMessagesOn                        )
            .def("messagesOff",                     &chargeController::messagesOff                            )
            .def("messagesOn",                      &chargeController::messagesOn                             )
            .def("silence",                         &chargeController::silence                                )
            .def("verbose",                         &chargeController::verbose                                )
		;

    boost::python::class_<VCcharges,boost::python::bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_charge_Controller",   &VCcharges::virtual_VELA_INJ_charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_charge_Controller",   &VCcharges::offline_VELA_INJ_charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_charge_Controller",  &VCcharges::physical_VELA_INJ_charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_charge_Controller",   &VCcharges::virtual_VELA_BA1_charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_charge_Controller",   &VCcharges::offline_VELA_BA1_charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_charge_Controller",  &VCcharges::physical_VELA_BA1_charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_charge_Controller",   &VCcharges::virtual_VELA_BA2_charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_charge_Controller",   &VCcharges::offline_VELA_BA2_charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_charge_Controller",  &VCcharges::physical_VELA_BA2_charge_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_PH1_charge_Controller",  &VCcharges::virtual_CLARA_PH1_charge_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_charge_Controller",  &VCcharges::offline_CLARA_PH1_charge_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_charge_Controller", &VCcharges::physical_CLARA_PH1_charge_Controller, return_value_policy<reference_existing_object>())
        .def("getchargeController",                  &VCscopes::getScopeController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL
#endif // velaChrageScopeController_H

