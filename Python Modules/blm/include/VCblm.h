#ifndef vela_QSCOPE_CONTROLLER_H
#define vela_QSCOPE_CONTROLLER_H

// project
#include "blmController.h"
#include "blmStructs.h"
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
class VCblm : public VCbase
{
    public:
        VCblm();
        ~VCblm();

        blmController & virtual_VELA_INJ_BLM_Controller();
        blmController & offline_VELA_INJ_BLM_Controller();
        blmController & physical_VELA_INJ_BLM_Controller();
        blmController & virtual_VELA_BA1_BLM_Controller();
        blmController & offline_VELA_BA1_BLM_Controller();
        blmController & physical_VELA_BA1_BLM_Controller();
        blmController & virtual_VELA_BA2_BLM_Controller();
        blmController & offline_VELA_BA2_BLM_Controller();
        blmController & physical_VELA_BA2_BLM_Controller();
        blmController & virtual_CLARA_PH1_BLM_Controller();
        blmController & offline_CLARA_PH1_BLM_Controller();
        blmController & physical_CLARA_PH1_BLM_Controller();
        blmController & virtual_C2B_BLM_Controller();
        blmController & offline_C2B_BLM_Controller();
        blmController & physical_C2B_BLM_Controller();
        blmController & getBLMController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area );

    protected:
    private:

        std::map<blmController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        blmController& getController(blmController*& cont,
                                       const std::string& conf1,
                                       const std::string& name,
                                       const bool shouldVM,
                                       const bool shouldEPICS,
                                       const HWC_ENUM::MACHINE_AREA myMachineArea);

        blmController * virtual_VELA_INJ_BLM_Controller_Obj;
        blmController * offline_VELA_INJ_BLM_Controller_Obj;
        blmController * physical_VELA_INJ_BLM_Controller_Obj;
        blmController * virtual_VELA_BA1_BLM_Controller_Obj;
        blmController * offline_VELA_BA1_BLM_Controller_Obj;
        blmController * physical_VELA_BA1_BLM_Controller_Obj;
        blmController * virtual_VELA_BA2_BLM_Controller_Obj;
        blmController * offline_VELA_BA2_BLM_Controller_Obj;
        blmController * physical_VELA_BA2_BLM_Controller_Obj;
        blmController * virtual_CLARA_PH1_BLM_Controller_Obj;
        blmController * offline_CLARA_PH1_BLM_Controller_Obj;
        blmController * physical_CLARA_PH1_BLM_Controller_Obj;
        blmController * virtual_C2B_BLM_Controller_Obj;
        blmController * offline_C2B_BLM_Controller_Obj;
        blmController * physical_C2B_BLM_Controller_Obj;
};

#ifdef BUILD_DLL
#define BOOST_LIB_DIAGNOSTIC

using namespace boost::python;

BOOST_PYTHON_MODULE( VELA_CLARA_BLM_Control )
{
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    /// Include ALL the enums you want to expose to Python

    boost::python::type_info info = boost::python::type_id< std::map< blmStructs::SCOPE_PV_TYPE, bool > > ();
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

    info = boost::python::type_id< std::map< blmStructs::SCOPE_PV_TYPE, bool > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >("std_scope_map_bool")
            .def( map_indexing_suite< std::map< blmStructs::SCOPE_PV_TYPE, bool > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >("std_scope_map_bool")
            .def( map_indexing_suite< std::map< blmStructs::SCOPE_PV_TYPE, bool > >())
            ;
    }

    info = boost::python::type_id< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >("std_scope_map_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >("std_scope_map_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< double > > >())
            ;
    }

    info = boost::python::type_id< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< std::vector< double > > > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< std::vector< double > > > >("std_scope_map_vector_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< std::vector< double > > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< std::vector< double > > > >("std_scope_map_vector_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::SCOPE_PV_TYPE, std::vector< std::vector< double > > > >())
            ;
    }

    enum_<blmStructs::SCOPE_PV_TYPE>("SCOPE_PV_TYPE")
            .value("TR1", blmStructs::SCOPE_PV_TYPE::TR1 )
            .value("TR2", blmStructs::SCOPE_PV_TYPE::TR2 )
            .value("TR3", blmStructs::SCOPE_PV_TYPE::TR3 )
            .value("TR4", blmStructs::SCOPE_PV_TYPE::TR4 )
            .value("P1",  blmStructs::SCOPE_PV_TYPE::P1  )
            .value("P2",  blmStructs::SCOPE_PV_TYPE::P2  )
            .value("P3",  blmStructs::SCOPE_PV_TYPE::P3  )
            .value("P4",  blmStructs::SCOPE_PV_TYPE::P4  )
            .value("UNKNOWN",  blmStructs::SCOPE_PV_TYPE::UNKNOWN  )
            ;

    enum_<blmStructs::SCOPE_NAME>("SCOPE_NAME")
            .value("CLARASCOPE01",       blmStructs::SCOPE_NAME::CLARASCOPE01       )
            .value("VELASCOPE02",        blmStructs::SCOPE_NAME::VELASCOPE02        )
            .value("UNKNOWN_SCOPE_NAME", blmStructs::SCOPE_NAME::UNKNOWN_SCOPE_NAME )
            ;

    enum_<blmStructs::DIAG_TYPE>("DIAG_TYPE")
            .value("WCM",     blmStructs::DIAG_TYPE::WCM       )
            .value("ICT1",    blmStructs::DIAG_TYPE::ICT2      )
            .value("ICT2",    blmStructs::DIAG_TYPE::ICT2      )
            .value("FCUP",    blmStructs::DIAG_TYPE::FCUP      )
            .value("ED_FCUP", blmStructs::DIAG_TYPE::ED_FCUP   )
            ;
    char const* blmNumObjectStructString = "This struct contains data for the four channels on the scope - e.g. p1 contains the P1 value from the scope.\n"
                                       "This will only contain real-time data if scope number data is being submitted to EPICS - you will need to check this on the scope.\n"
                                       "p1Vec contains a vector of values after monitorNumsForNShots, and numData contains a vector of vectors, with data from all four channels on the scope.\n"
                                       "Timestamps can also be accessed.";
    boost::python::class_<blmStructs::blmNumObject,boost::noncopyable>
        ("blmNumObject",blmNumObjectStructString,boost::python::no_init)
        .def_readonly("isMonitoringMap", &blmStructs::blmNumObject::isMonitoringMap)
        .def_readonly("name",            &blmStructs::blmNumObject::name           )
        .def_readonly("p1",              &blmStructs::blmNumObject::p1             )
        .def_readonly("p2",              &blmStructs::blmNumObject::p2             )
        .def_readonly("p3",              &blmStructs::blmNumObject::p3             )
        .def_readonly("p4",              &blmStructs::blmNumObject::p4             )
        .def_readonly("p1TimeStamp",     &blmStructs::blmNumObject::p1TimeStamp    )
        .def_readonly("p2TimeStamp",     &blmStructs::blmNumObject::p2TimeStamp    )
        .def_readonly("p3TimeStamp",     &blmStructs::blmNumObject::p3TimeStamp    )
        .def_readonly("p4TimeStamp",     &blmStructs::blmNumObject::p4TimeStamp    )
        .def_readonly("p1TimeStamps",    &blmStructs::blmNumObject::p1TimeStamps   )
        .def_readonly("p2TimeStamps",    &blmStructs::blmNumObject::p2TimeStamps   )
        .def_readonly("p3TimeStamps",    &blmStructs::blmNumObject::p3TimeStamps   )
        .def_readonly("p4TimeStamps",    &blmStructs::blmNumObject::p4TimeStamps   )
        .def_readonly("p1Vec",           &blmStructs::blmNumObject::p1Vec          )
        .def_readonly("p2Vec",           &blmStructs::blmNumObject::p2Vec          )
        .def_readonly("p3Vec",           &blmStructs::blmNumObject::p3Vec          )
        .def_readonly("p4Vec",           &blmStructs::blmNumObject::p4Vec          )
        .def_readonly("numData",         &blmStructs::blmNumObject::numData        )
        .def_readonly("numTimeStamps",   &blmStructs::blmNumObject::numTimeStamps  )
        .def_readonly("shotCounts",      &blmStructs::blmNumObject::shotCounts     )
        .def_readonly("buffer",          &blmStructs::blmNumObject::buffer         )
        ;

    char const* blmTraceDataStructString = "This struct contains trace data for the four channels on the scope - e.g. tr1TraceData contains a vector of the EPICS PV for channel 1 on the scope.\n"
                                             "This will only contain real-time data if scope trace data is being submitted to EPICS - you will need to check this on the scope.\n";
                                             "traceData contains a vector of vectors, with data from all four channels on the scope.\n"
                                             "Timestamps can also be accessed.";
    boost::python::class_<blmStructs::blmTraceData,boost::noncopyable>
        ("blmTraceData",blmTraceDataStructString,boost::python::no_init)
        .def_readonly("isMonitoringMap", &blmStructs::blmTraceData::isMonitoringMap)
        .def_readonly("name",            &blmStructs::blmTraceData::name           )
        .def_readonly("pvRoot",          &blmStructs::blmTraceData::pvRoot         )
        .def_readonly("tr1TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
        .def_readonly("tr2TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
        .def_readonly("tr3TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
        .def_readonly("tr4TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
        .def_readonly("traceData",       &blmStructs::blmTraceData::traceData      )
        .def_readonly("tr1TimeStamps",   &blmStructs::blmTraceData::tr1TimeStamps  )
        .def_readonly("tr2TimeStamps",   &blmStructs::blmTraceData::tr2TimeStamps  )
        .def_readonly("tr3TimeStamps",   &blmStructs::blmTraceData::tr3TimeStamps  )
        .def_readonly("tr4TimeStamps",   &blmStructs::blmTraceData::tr4TimeStamps  )
        .def_readonly("shotCounts",      &blmStructs::blmTraceData::shotCounts     )
        .def_readonly("timebase",        &blmStructs::blmTraceData::timebase       )
        .def_readonly("timeStamps",      &blmStructs::blmTraceData::timeStamps     )
        ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    char const* getBLMTraceDataStructString = "Returns the scope trace data struct for str(scopeName). See documentation on the blmTraceData struct for what this contains.";
    char const* getBLMNumDataStructString = "Returns the scope number data struct for str(scopeName). See documentation on the scopeNumData struct for what this contains.";
    char const* isMonitoringTraceDocString = "Returns true if str(scopeName) traces are being monitored - these are defined in the config file.";
    char const* isMonitoringNumDocString = "Returns true if str(scopeName) P values are being monitored - these are defined in the config file.";
    char const* isNotMonitoringTraceDocString = "Returns true if str(scopeName) traces are not being monitored - these are defined in the config file.";
    char const* isNotMonitoringNumDocString = "Returns true if str(scopeName) P values are not being monitored - these are defined in the config file.";
    char const* setBufferSizeDocString = "Set size of buffer for continuous monitor.";
    char const* setNumBufferSizeDocString = "Set size of buffer for continuous P values monitor.";
    char const* setTraceBufferSizeDocString = "Set size of buffer for continuous trace monitor.";
    char const* restartMonitoringDocString = "Restarts continuous monitoring of scope parameters. !!!!!!!WILL RESET ALL VALUES!!!!!!!.";
    char const* setTimebaseDocString = "Sets the timebase (number of points in trace / time window of trace).";
    char const* getTimebaseDocString = "Returns the timebase (number of points in trace / time window of trace).";
    char const* getBufferSizeDocString = "Returns the size of the buffer.";
    char const* getDiagTypeDocString = "Returns the diagnostic type for a given channel on the scope (Trace or number).";
    char const* getDiagTypeStrDocString = "Returns the diagnostic type for a given channel on the scope (Trace or number).";
    char const* getBLMNumsDocString = "Returns a vector of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), after using monitorNumsForNShots.";
    char const* getBLMP1VecDocString = "Returns a vector of doubles for str(scopeName), for channel P1, after using monitorNumsForNShots.";
    char const* getBLMP2VecDocString = "Returns a vector of doubles for str(scopeName), for channel P2, after using monitorNumsForNShots.";
    char const* getBLMP3VecDocString = "Returns a vector of doubles for str(scopeName), for channel P3, after using monitorNumsForNShots.";
    char const* getBLMP4VecDocString = "Returns a vector of doubles for str(scopeName), for channel P4, after using monitorNumsForNShots.";
    char const* getBLMP1DocString = "Returns a double containing the value for channel P1 for str(scopeName).";
    char const* getBLMP2DocString = "Returns a double containing the value for channel P2 for str(scopeName).";
    char const* getBLMP3DocString = "Returns a double containing the value for channel P3 for str(scopeName).";
    char const* getBLMP4DocString = "Returns a double containing the value for channel P4 for str(scopeName).";
    char const* getBLMNumBufferDocString = "Returns a vector containing the last (buffersize) values for a given channel for str(scopeName).";
    char const* getBLMTraceBufferDocString = "Returns a vector of vectors containing the last (buffersize) traces for a given channel for str(scopeName).";
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
    char const* getBLMTracesDocString = "Returns a vector of vectors of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), after using monitorTracesForNShots.";
    char const* getPartOfTraceDocString = "Returns a vector of vectors of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), of a user-specified portion of the trace (between part1 and part2), after using monitorTracesForNShots.";
    char const* getAreaUnderTracesDocString = "Returns a vector of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), containing the area under each trace.\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getAreaUnderPartOfTraceDocString = "Returns a vector of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), containing the area under a user-specified portion of the trace (between part1 and part2).\n"
                                              "The user should specify part1 and part2 to be regions of the trace where there is no signal (it may be good to check the array size first).";
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getAvgNoiseDocString = "Returns a vector of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), containing the average noise in a user-specified portion of the trace (between part1 and part2).\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getMaxOfTracesDocString = "Returns a vector of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), containing the maximum value of each trace\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getMinOfTracesDocString = "Returns a vector of doubles for str(scopeName), for the channel SCOPE_PV_TYPE(pvType), containing the minimum value of each trace\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getTimeStampsDocString = "Returns a vector containing the timestamps as doubles for str(scope), for the channel SCOPE_PV_TYPE(pvType) - these are defined in the config file.\n"
                                         "To be used in conjunction with functions monitorNumsForNShots or monitorTracesForNShots.";
    char const* getStrTimeStampsDocString = "Returns a vector containing the timestamps as strings (if that's your thing) for str(scope), for the channel SCOPE_PV_TYPE(pvType) - these are defined in the config file.\n"
                                         "To be used in conjunction with functions monitorNumsForNShots or monitorTracesForNShots.";
    char const* monitorNumsDocString = "Monitors P values (see scope) for str(scopeName) - these should be defined in the config file. This will fill four vectors of doubles with scope data.\n"
                                      "Data can be accessed using getBLMNums, or getBLMP(1/2/3/4)Vec.\n";
    char const* monitorTracesDocString = "Monitors traces (see scope) for str(scopeName) - these should be defined in the config file. This will fill four vectors of vectors of doubles with scope trace data.\n"
                                      "Data can be accessed using getBLMTraces - see documentation.\n";
    char const* monitorATraceDocString = "Monitors a specific trace (see scope) for channel (pvType) of str(scopeName) - these should be defined in the config file. This will fill a vectors of vectors of doubles with scope trace data.\n"
                                      "Data can be accessed using getBLMTraces - see documentation.\n";
    char const* monitorANumDocString = "Monitors a specific P value (see scope) for channel (pvType) of str(scopeName) - these should be defined in the config file. This will fill a vector of doubles with scope data.\n"
                                      "Data can be accessed using getBLMTraces - see documentation.\n";

	boost::python::class_<blmController, boost::python::bases<controller>, boost::noncopyable>
            ("blmController","blmController Doc String",boost::python::no_init)
            .def("get_CA_PEND_IO_TIMEOUT",          &blmController::get_CA_PEND_IO_TIMEOUT                    )
            .def("set_CA_PEND_IO_TIMEOUT",          &blmController::set_CA_PEND_IO_TIMEOUT                    )
            .def("getBLMTraceDataStruct",         &blmController::getBLMTraceDataStruct, getBLMTraceDataStructString, return_value_policy<reference_existing_object>())
            .def("getBLMNumDataStruct",           &blmController::getBLMNumDataStruct, getBLMNumDataStructString, return_value_policy<reference_existing_object>()  )
            .def("isMonitoringBLMTrace",          &blmController::isMonitoringBLMTrace, isMonitoringTraceDocString           )
            .def("isMonitoringBLMNum",            &blmController::isMonitoringBLMNum, isMonitoringNumDocString               )
            .def("isNotMonitoringBLMTrace",       &blmController::isNotMonitoringBLMTrace, isNotMonitoringNumDocString       )
            .def("isNotMonitoringBLMNum",         &blmController::isNotMonitoringBLMNum, isNotMonitoringNumDocString         )
            .def("setBufferSize",                   &blmController::setBufferSize, setBufferSizeDocString                        )
            .def("setNumBufferSize",                &blmController::setBufferSize, setNumBufferSizeDocString                     )
            .def("setTraceBufferSize",              &blmController::setBufferSize, setTraceBufferSizeDocString                   )
            .def("restartContinuousMonitoring",     &blmController::restartContinuousMonitoring, restartMonitoringDocString      )
            .def("setTimebase",                     &blmController::setTimebase, setTimebaseDocString                            )
            .def("getTimebase",                     &blmController::getTimebase, getTimebaseDocString                            )
            .def("getBufferSize",                   &blmController::getBufferSize, getBufferSizeDocString                        )
            .def("getDiagType",                     &blmController::getDiagType, getDiagTypeDocString                            )
            .def("getDiagTypeStr",                  &blmController::getDiagTypeStr, getDiagTypeStrDocString                      )
            .def("getBLMNums",                    &blmController::getBLMNums_Py, getBLMNumsDocString                       )
            .def("getBLMP1Vec",                   &blmController::getBLMP1Vec_Py, getBLMP1VecDocString                     )
            .def("getBLMP2Vec",                   &blmController::getBLMP2Vec_Py, getBLMP2VecDocString                     )
            .def("getBLMP3Vec",                   &blmController::getBLMP3Vec_Py, getBLMP3VecDocString                     )
            .def("getBLMP4Vec",                   &blmController::getBLMP4Vec_Py, getBLMP4VecDocString                     )
            .def("getBLMNumBuffer",               &blmController::getBLMNumBuffer_Py, getBLMNumBufferDocString             )
            .def("getBLMP1Buffer",                &blmController::getBLMP1Buffer_Py, getBLMNumBufferDocString              )
            .def("getBLMP2Buffer",                &blmController::getBLMP2Buffer_Py, getBLMNumBufferDocString              )
            .def("getBLMP3Buffer",                &blmController::getBLMP3Buffer_Py, getBLMNumBufferDocString              )
            .def("getBLMP4Buffer",                &blmController::getBLMP4Buffer_Py, getBLMNumBufferDocString              )
            .def("getBLMTraceBuffer",             &blmController::getBLMTraceBuffer_Py, getBLMTraceBufferDocString         )
            .def("getBLMTR1Buffer",               &blmController::getBLMTR1Buffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMTR2Buffer",               &blmController::getBLMTR2Buffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMTR3Buffer",               &blmController::getBLMTR3Buffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMTR4Buffer",               &blmController::getBLMTR4Buffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMP1",                      &blmController::getBLMP1, getBLMP1DocString                              )
            .def("getBLMP2",                      &blmController::getBLMP2, getBLMP2DocString                              )
            .def("getBLMP3",                      &blmController::getBLMP3, getBLMP3DocString                              )
            .def("getBLMP4",                      &blmController::getBLMP4, getBLMP4DocString                              )
            .def("getWCMQ",                         &blmController::getWCMQ, getWCMQDocString                                    )
            .def("getICT1Q",                        &blmController::getICT1Q, getICT1QDocString                                  )
            .def("getICT2Q",                        &blmController::getICT2Q, getICT2QDocString                                  )
            .def("getFCUPQ",                        &blmController::getFCUPQ, getFCUPQDocString                                  )
            .def("getEDFCUPQ",                      &blmController::getEDFCUPQ, getEDFCUPQDocString                              )
            .def("getBLMTraces",                  &blmController::getBLMTraces, getBLMTracesDocString                      )
            .def("getMinOfTraces",                  &blmController::getMinOfTraces_Py, getMinOfTracesDocString                   )
            .def("getMaxOfTraces",                  &blmController::getMaxOfTraces_Py, getMaxOfTracesDocString                   )
            .def("getAreaUnderTraces",              &blmController::getAreaUnderTraces_Py, getAreaUnderTracesDocString           )
            .def("getAreaUnderPartOfTrace",         &blmController::getAreaUnderPartOfTrace_Py, getAreaUnderPartOfTraceDocString )
            .def("getAvgNoise",                     &blmController::getAvgNoise_Py, getAvgNoiseDocString                         )
            .def("getPartOfTrace",                  &blmController::getPartOfTrace, getPartOfTraceDocString                      )
            .def("getTimeStamps",                   &blmController::getTimeStamps_Py, getTimeStampsDocString                     )
            .def("getStrTimeStamps",                &blmController::getStrTimeStamps_Py, getStrTimeStampsDocString               )
            .def("monitorNumsForNShots",            &blmController::monitorNumsForNShots, monitorNumsDocString                   )
            .def("monitorANumForNShots",            &blmController::monitorANumForNShots, monitorANumDocString                   )
            .def("monitorTracesForNShots",          &blmController::monitorTracesForNShots, monitorTracesDocString               )
            .def("monitorATraceForNShots",          &blmController::monitorATraceForNShots, monitorATraceDocString               )
            .def("getBLMNames",                   &blmController::getBLMNames_Py                                             )
            .def("getBLMPVs",                     &blmController::getBLMPVs_Py                                               )
            .def("getBLMTracePVs",                &blmController::getBLMTracePVs_Py                                          )
            .def("getBLMNumPVs",                  &blmController::getBLMNumPVs_Py                                            )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &blmController::debugMessagesOff                       )
            .def("debugMessagesOn",                 &blmController::debugMessagesOn                        )
            .def("messagesOff",                     &blmController::messagesOff                            )
            .def("messagesOn",                      &blmController::messagesOn                             )
            .def("silence",                         &blmController::silence                                )
            .def("verbose",                         &blmController::verbose                                )
		;

    boost::python::class_<VCblm,boost::python::bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_BLM_Controller",   &VCblm::virtual_VELA_INJ_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_BLM_Controller",   &VCblm::offline_VELA_INJ_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_BLM_Controller",  &VCblm::physical_VELA_INJ_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_BLM_Controller",   &VCblm::virtual_VELA_BA1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_BLM_Controller",   &VCblm::offline_VELA_BA1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_BLM_Controller",  &VCblm::physical_VELA_BA1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_BLM_Controller",   &VCblm::virtual_VELA_BA2_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_BLM_Controller",   &VCblm::offline_VELA_BA2_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_BLM_Controller",  &VCblm::physical_VELA_BA2_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_PH1_BLM_Controller",  &VCblm::virtual_CLARA_PH1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_BLM_Controller",  &VCblm::offline_CLARA_PH1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_BLM_Controller", &VCblm::physical_CLARA_PH1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_C2B_BLM_Controller",  &VCblm::virtual_C2B_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_C2B_BLM_Controller",  &VCblm::offline_C2B_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_C2B_BLM_Controller", &VCblm::physical_C2B_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("getBLMController",                  &VCblm::getBLMController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL
#endif // velaChrageBLMController_H

