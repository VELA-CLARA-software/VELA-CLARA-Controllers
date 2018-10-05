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

        blmController & virtual_CLARA_PH1_BLM_Controller();
        blmController & offline_CLARA_PH1_BLM_Controller();
        blmController & physical_CLARA_PH1_BLM_Controller();
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

        blmController * virtual_CLARA_PH1_BLM_Controller_Obj;
        blmController * offline_CLARA_PH1_BLM_Controller_Obj;
        blmController * physical_CLARA_PH1_BLM_Controller_Obj;
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

    boost::python::type_info info = boost::python::type_id< std::map< blmStructs::BLM_PV_TYPE, bool > > ();
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

    info = boost::python::type_id< std::map< blmStructs::BLM_PV_TYPE, bool > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >("std_scope_map_bool")
            .def( map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, bool > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >("std_scope_map_bool")
            .def( map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, bool > >())
            ;
    }

    info = boost::python::type_id< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >("std_scope_map_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >("std_scope_map_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, std::vector< double > > >())
            ;
    }

    info = boost::python::type_id< std::map< blmStructs::BLM_PV_TYPE, std::vector< std::vector< double > > > > ();
    reg = boost::python::converter::registry::query(info);
    if (reg == NULL)  {
        class_< std::map< blmStructs::BLM_PV_TYPE, std::vector< std::vector< double > > > >("std_scope_map_vector_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, std::vector< std::vector< double > > > >())
            ;
    } else if ((*reg).m_to_python == NULL) {
        class_< std::map< blmStructs::BLM_PV_TYPE, std::vector< std::vector< double > > > >("std_scope_map_vector_vector_double")
            .def( map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, std::vector< std::vector< double > > > >())
            ;
    }

    enum_<blmStructs::BLM_PV_TYPE>("BLM_PV_TYPE")
            .value("CH1WAVE", blmStructs::BLM_PV_TYPE::CH1WAVE )
            .value("CH2WAVE", blmStructs::BLM_PV_TYPE::CH2WAVE )
            .value("CH3WAVE", blmStructs::BLM_PV_TYPE::CH3WAVE )
            .value("CH4WAVE", blmStructs::BLM_PV_TYPE::CH4WAVE )
            .value("CH1TIME",  blmStructs::BLM_PV_TYPE::CH1TIME  )
            .value("CH2TIME",  blmStructs::BLM_PV_TYPE::CH2TIME  )
            .value("CH3TIME",  blmStructs::BLM_PV_TYPE::CH3TIME  )
            .value("CH4TIME",  blmStructs::BLM_PV_TYPE::CH4TIME  )
            .value("UNKNOWN",  blmStructs::BLM_PV_TYPE::UNKNOWN  )
            ;

    char const* blmTraceDataStructString = "This struct contains trace data for the four channels on the scope - e.g. tr1TraceData contains a vector of the EPICS PV for channel 1 on the scope.\n"
                                             "This will only contain real-time data if scope trace data is being submitted to EPICS - you will need to check this on the scope.\n";
                                             "traceData contains a vector of vectors, with data from all four channels on the scope.\n"
                                             "Timestamps can also be accessed.";
//    boost::python::class_<blmStructs::blmTraceData,boost::noncopyable>
//        ("blmTraceData",blmTraceDataStructString,boost::python::no_init)
//        .def_readonly("isMonitoringMap", &blmStructs::blmTraceData::isMonitoringMap)
//        .def_readonly("name",            &blmStructs::blmTraceData::name           )
//        .def_readonly("pvRoot",          &blmStructs::blmTraceData::pvRoot         )
//        .def_readonly("tr1TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
//        .def_readonly("tr2TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
//        .def_readonly("tr3TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
//        .def_readonly("tr4TraceData",    &blmStructs::blmTraceData::tr1TraceData   )
//        .def_readonly("traceData",       &blmStructs::blmTraceData::traceData      )
//        .def_readonly("tr1TimeStamps",   &blmStructs::blmTraceData::tr1TimeStamps  )
//        .def_readonly("tr2TimeStamps",   &blmStructs::blmTraceData::tr2TimeStamps  )
//        .def_readonly("tr3TimeStamps",   &blmStructs::blmTraceData::tr3TimeStamps  )
//        .def_readonly("tr4TimeStamps",   &blmStructs::blmTraceData::tr4TimeStamps  )
//        .def_readonly("shotCounts",      &blmStructs::blmTraceData::shotCounts     )
//        .def_readonly("timebase",        &blmStructs::blmTraceData::timebase       )
//        .def_readonly("timeStamps",      &blmStructs::blmTraceData::timeStamps     )
//        ;

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
    char const* getBLMNumsDocString = "Returns a vector of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), after using monitorNumsForNShots.";
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
    char const* getBLMTracesDocString = "Returns a vector of vectors of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), after using monitorTracesForNShots.";
    char const* getPartOfTraceDocString = "Returns a vector of vectors of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), of a user-specified portion of the trace (between part1 and part2), after using monitorTracesForNShots.";
    char const* getAreaUnderTracesDocString = "Returns a vector of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), containing the area under each trace.\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getAreaUnderPartOfTraceDocString = "Returns a vector of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), containing the area under a user-specified portion of the trace (between part1 and part2).\n"
                                              "The user should specify part1 and part2 to be regions of the trace where there is no signal (it may be good to check the array size first).";
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getAvgNoiseDocString = "Returns a vector of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), containing the average noise in a user-specified portion of the trace (between part1 and part2).\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getMaxOfTracesDocString = "Returns a vector of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), containing the maximum value of each trace\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getMinOfTracesDocString = "Returns a vector of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), containing the minimum value of each trace\n"
                                              "This function should only be used after using monitorTracesForNShots.";
    char const* getTimeStampsDocString = "Returns a vector containing the timestamps as doubles for str(scope), for the channel BLM_PV_TYPE(pvType) - these are defined in the config file.\n"
                                         "To be used in conjunction with functions monitorNumsForNShots or monitorTracesForNShots.";
    char const* getStrTimeStampsDocString = "Returns a vector containing the timestamps as strings (if that's your thing) for str(scope), for the channel BLM_PV_TYPE(pvType) - these are defined in the config file.\n"
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
            .def("isMonitoringBLMTrace",          &blmController::isMonitoringBLMTrace, isMonitoringTraceDocString           )
            .def("isNotMonitoringBLMTrace",       &blmController::isNotMonitoringBLMTrace, isNotMonitoringNumDocString       )
            .def("setBufferSize",                   &blmController::setBufferSize, setBufferSizeDocString                        )
            .def("restartContinuousMonitoring",     &blmController::restartContinuousMonitoring, restartMonitoringDocString      )
            .def("setTimebase",                     &blmController::setTimebase, setTimebaseDocString                            )
            .def("getTimebase",                     &blmController::getTimebase, getTimebaseDocString                            )
            .def("getBufferSize",                   &blmController::getBufferSize, getBufferSizeDocString                        )
            .def("getBLMTrace",             &blmController::getBLMTraceBuffer_Py, getBLMTraceBufferDocString         )
            .def("getBLMTraces",             &blmController::getBLMTraceBuffer_Py, getBLMTraceBufferDocString         )
            .def("getBLMTraceBuffer",             &blmController::getBLMTraceBuffer_Py, getBLMTraceBufferDocString         )
            .def("getBLMCH1WaveformBuffer",               &blmController::getBLMCH1WaveformBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH2WaveformBuffer",               &blmController::getBLMCH2WaveformBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH3WaveformBuffer",               &blmController::getBLMCH3WaveformBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH4WaveformBuffer",               &blmController::getBLMCH4WaveformBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH1TimeBuffer",               &blmController::getBLMCH1TimeBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH2TimeBuffer",               &blmController::getBLMCH2TimeBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH3TimeBuffer",               &blmController::getBLMCH3TimeBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH4TimeBuffer",               &blmController::getBLMCH4TimeBuffer_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH1Waveform",               &blmController::getBLMCH1Waveform_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH2Waveform",               &blmController::getBLMCH2Waveform_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH3Waveform",               &blmController::getBLMCH3Waveform_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH4Waveform",               &blmController::getBLMCH4Waveform_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH1Time",               &blmController::getBLMCH1Time_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH2Time",               &blmController::getBLMCH2Time_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH3Time",               &blmController::getBLMCH3Time_Py, getBLMTraceBufferDocString           )
            .def("getBLMCH4Time",               &blmController::getBLMCH4Time_Py, getBLMTraceBufferDocString           )
            .def("getBLMTraces",                  &blmController::getBLMTraces, getBLMTracesDocString                      )
            .def("getMinOfTraces",                  &blmController::getMinOfTraces_Py, getMinOfTracesDocString                   )
            .def("getMaxOfTraces",                  &blmController::getMaxOfTraces_Py, getMaxOfTracesDocString                   )
            .def("getAreaUnderTraces",              &blmController::getAreaUnderTraces_Py, getAreaUnderTracesDocString           )
            .def("getAreaUnderPartOfTrace",         &blmController::getAreaUnderPartOfTrace_Py, getAreaUnderPartOfTraceDocString )
            .def("getAvgNoise",                     &blmController::getAvgNoise_Py, getAvgNoiseDocString                         )
            .def("getPartOfTrace",                  &blmController::getPartOfTrace, getPartOfTraceDocString                      )
            .def("getTimeStamps",                   &blmController::getTimeStamps_Py, getTimeStampsDocString                     )
            .def("getStrTimeStamps",                &blmController::getStrTimeStamps_Py, getStrTimeStampsDocString               )
            .def("monitorTracesForNShots",          &blmController::monitorTracesForNShots, monitorTracesDocString               )
            .def("monitorATraceForNShots",          &blmController::monitorATraceForNShots, monitorATraceDocString               )
            .def("getBLMNames",                   &blmController::getBLMNames_Py                                             )
            .def("getBLMPVs",                     &blmController::getBLMPVs_Py                                               )
            .def("getBLMTracePVs",                &blmController::getBLMTracePVs_Py                                          )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &blmController::debugMessagesOff                       )
            .def("debugMessagesOn",                 &blmController::debugMessagesOn                        )
            .def("messagesOff",                     &blmController::messagesOff                            )
            .def("messagesOn",                      &blmController::messagesOn                             )
            .def("silence",                         &blmController::silence                                )
            .def("verbose",                         &blmController::verbose                                )
		;

    boost::python::class_<VCblm,boost::python::bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_CLARA_PH1_BLM_Controller",  &VCblm::virtual_CLARA_PH1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_BLM_Controller",  &VCblm::offline_CLARA_PH1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_BLM_Controller", &VCblm::physical_CLARA_PH1_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("getBLMController",                  &VCblm::getBLMController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL
#endif // velaChrageBLMController_H

