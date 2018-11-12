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
class VCblm : public VCbase
{
    public:
        VCblm();
        ~VCblm();

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

        blmController * virtual_CLARA_PH1_BLM_Controller_Obj;
        blmController * offline_CLARA_PH1_BLM_Controller_Obj;
        blmController * physical_CLARA_PH1_BLM_Controller_Obj;
        blmController * virtual_CLARA_2_BA1_BA2_BLM_Controller_Obj;
        blmController * offline_CLARA_2_BA1_BA2_BLM_Controller_Obj;
        blmController * physical_CLARA_2_BA1_BA2_BLM_Controller_Obj;
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

    boost::python::class_< boost::circular_buffer< double > >("Circular buffer definition for python ", boost::python::no_init)
        .def(vector_indexing_suite< boost::circular_buffer< double > >())
        ;

    boost::python::class_< std::map< blmStructs::BLM_PV_TYPE, boost::circular_buffer< double > > >("map of BLM_PV_TYPE of circular buffer of doubles ", boost::python::no_init)
        .def(map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, boost::circular_buffer< double > > >())
        ;

    boost::python::class_< boost::circular_buffer< std::vector< double > > >("Circular buffer of vector of doubles definition for python ", boost::python::no_init)
        .def(vector_indexing_suite< boost::circular_buffer< std::vector< double > > >())
        ;

    boost::python::class_< std::map< blmStructs::BLM_PV_TYPE, boost::circular_buffer< std::vector< double > > > >("map of BLM_PV_TYPE of circular buffer of doubles ", boost::python::no_init)
        .def(map_indexing_suite< std::map< blmStructs::BLM_PV_TYPE, boost::circular_buffer< std::vector< double > > > >())
        ;

    enum_<blmStructs::BLM_PV_TYPE>("BLM_PV_TYPE")
            .value("CH1WAVE", blmStructs::BLM_PV_TYPE::CH1WAVE )
            .value("CH2WAVE", blmStructs::BLM_PV_TYPE::CH2WAVE )
            .value("CH3WAVE", blmStructs::BLM_PV_TYPE::CH3WAVE )
            .value("CH4WAVE", blmStructs::BLM_PV_TYPE::CH4WAVE )
            .value("CH1TIME", blmStructs::BLM_PV_TYPE::CH1TIME  )
            .value("CH2TIME", blmStructs::BLM_PV_TYPE::CH2TIME  )
            .value("CH3TIME", blmStructs::BLM_PV_TYPE::CH3TIME  )
            .value("CH4TIME", blmStructs::BLM_PV_TYPE::CH4TIME  )
            .value("UNKNOWN", blmStructs::BLM_PV_TYPE::UNKNOWN  )
            ;

    char const* blmTraceDataStructString = "This struct contains trace data for the four channels on the scope - e.g. tr1TraceData contains a vector of the EPICS PV for channel 1 on the scope.\n"
                                             "This will only contain real-time data if scope trace data is being submitted to EPICS - you will need to check this on the scope.\n";
                                             "traceData contains a vector of vectors, with data from all four channels on the scope.\n"
                                             "Timestamps can also be accessed.";
    boost::python::class_<blmStructs::blmTraceData,boost::noncopyable>
        ("blmTraceData",blmTraceDataStructString,boost::python::no_init)
        .def_readonly("isMonitoringMap",     &blmStructs::blmTraceData::isMonitoringMap)
        .def_readonly("name",                &blmStructs::blmTraceData::name           )
        .def_readonly("pvRoot",              &blmStructs::blmTraceData::pvRoot         )
        .def_readonly("traceData",           &blmStructs::blmTraceData::traceData   )
        .def_readonly("traceDataBuffer",     &blmStructs::blmTraceData::traceDataBuffer   )
        .def_readonly("timeStampsBuffer",    &blmStructs::blmTraceData::timeStampsBuffer   )
        .def_readonly("strTimeStampsBuffer", &blmStructs::blmTraceData::strTimeStampsBuffer   )
        .def_readonly("buffer",              &blmStructs::blmTraceData::buffer   )
        ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    char const* getBLMTraceDataStructString = "Returns the scope trace data struct for str(scopeName). See documentation on the blmTraceData struct for what this contains.";
    char const* isMonitoringTraceDocString = "Returns true if str(scopeName) traces are being monitored - these are defined in the config file.";
    char const* isNotMonitoringTraceDocString = "Returns true if str(scopeName) traces are not being monitored - these are defined in the config file.";
    char const* setBufferSizeDocString = "Set size of buffer for continuous monitor.";
    char const* setNumBufferSizeDocString = "Set size of buffer for continuous P values monitor.";
    char const* setTraceBufferSizeDocString = "Set size of buffer for continuous trace monitor.";
    char const* restartMonitoringDocString = "Restarts continuous monitoring of scope parameters. !!!!!!!WILL RESET ALL VALUES!!!!!!!.";
    char const* getTimebaseDocString = "Returns the timebase (number of points in trace / time window of trace).";
    char const* getBufferSizeDocString = "Returns the size of the buffer.";
    char const* getBLMCH1WaveformBufferDocString = "Returns a circular buffer of vectors of doubles for the voltage profile of channel 1 for str(blmName).";
    char const* getBLMCH2WaveformBufferDocString = "Returns a circular buffer of vectors of doubles for the voltage profile of channel 2 for str(blmName).";
    char const* getBLMCH3WaveformBufferDocString = "Returns a circular buffer of vectors of doubles for the voltage profile of channel 3 for str(blmName).";
    char const* getBLMCH4WaveformBufferDocString = "Returns a circular buffer of vectors of doubles for the voltage profile of channel 4 for str(blmName).";
    char const* getBLMCH1WaveformDocString = "Returns a vector of doubles for the voltage profile of channel 1 for str(blmName).";
    char const* getBLMCH2WaveformDocString = "Returns a vector of doubles for the voltage profile of channel 2 for str(blmName).";
    char const* getBLMCH3WaveformDocString = "Returns a vector of doubles for the voltage profile of channel 3 for str(blmName).";
    char const* getBLMCH4WaveformDocString = "Returns a vector of doubles for the voltage profile of channel 4 for str(blmName).";
    char const* getBLMCH1TimeBufferDocString = "Returns a circular buffer of vectors of doubles for the time profile of channel 1 for str(blmName).";
    char const* getBLMCH2TimeBufferDocString = "Returns a circular buffer of vectors of doubles for the time profile of channel 2 for str(blmName).";
    char const* getBLMCH3TimeBufferDocString = "Returns a circular buffer of vectors of doubles for the time profile of channel 3 for str(blmName).";
    char const* getBLMCH4TimeBufferDocString = "Returns a circular buffer of vectors of doubles for the time profile of channel 4 for str(blmName).";
    char const* getBLMCH1TimeDocString = "Returns a vector of doubles for the time profile of channel 1 for str(blmName).";
    char const* getBLMCH2TimeDocString = "Returns a vector of doubles for the time profile of channel 2 for str(blmName).";
    char const* getBLMCH3TimeDocString = "Returns a vector of doubles for the time profile of channel 3 for str(blmName).";
    char const* getBLMCH4TimeDocString = "Returns a vector of doubles for the time profile of channel 4 for str(blmName).";
    char const* getBLMTraceDocString = "Returns a vector of doubles containing the traces for a given BLM_PV_TYPE for str(blmName).";
    char const* getBLMTraceBufferDocString = "Returns a circular_buffer of vectors containing the last (buffersize) traces for a given BLM_PV_TYPE for str(blmName).";
    char const* getBLMTracesDocString = "Returns a vector of vectors of doubles for str(blmName), for the channel BLM_PV_TYPE(pvType), after using monitorTracesForNShots.";
    char const* getPartOfTraceDocString = "Returns a vector of vectors of doubles for str(scopeName), for the channel BLM_PV_TYPE(pvType), of a user-specified portion of the trace (between part1 and part2), after using monitorTracesForNShots.";
    char const* getBLMNamesDocString = "Returns a list of strings for all BLM devices.";
    char const* getBLMPVsDocString = "Returns a list of PVs (as string) for all BLMs defined in the config file.";
    char const* getBLMPVStringsDocString = "Returns a list of PVs (as BLM_PV_TYPE) for all BLMs defined in the config file.";
    char const* getBLMWaveformPVsDocString = "Returns a list of PVs (as BLM_PV_TYPE) for all waveform BLMs defined in the config file.";
    char const* getBLMTimePVsDocString = "Returns a list of PVs (as BLM_PV_TYPE) for all time BLMs defined in the config file.";
    char const* getBLMWaveformPVStringsDocString = "Returns a list of PVs (as string) for all BLMs defined in the config file.";
    char const* getBLMTimePVStringsDocString = "Returns a list of PVs (as string) for all BLMs defined in the config file.";
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
    char const* monitorTracesDocString = "Monitors traces (see scope) for str(scopeName) - these should be defined in the config file. This will fill four vectors of vectors of doubles with scope trace data.\n"
                                      "Data can be accessed using getBLMTraces - see documentation.\n";
    char const* monitorATraceDocString = "Monitors a specific trace (see scope) for channel (pvType) of str(scopeName) - these should be defined in the config file. This will fill a vectors of vectors of doubles with scope trace data.\n"
                                      "Data can be accessed using getBLMTraces - see documentation.\n";

	boost::python::class_<blmController, boost::python::bases<controller>, boost::noncopyable>
            ("blmController","blmController Doc String",boost::python::no_init)
            .def("get_CA_PEND_IO_TIMEOUT",          &blmController::get_CA_PEND_IO_TIMEOUT                    )
            .def("set_CA_PEND_IO_TIMEOUT",          &blmController::set_CA_PEND_IO_TIMEOUT                    )
            .def("getBLMTraceDataStruct",           &blmController::getBLMTraceDataStruct, getBLMTraceDataStructString, return_value_policy<reference_existing_object>())
            .def("isMonitoringBLMTrace",            &blmController::isMonitoringBLMTrace, isMonitoringTraceDocString                )
            .def("isNotMonitoringBLMTrace",         &blmController::isNotMonitoringBLMTrace, isNotMonitoringTraceDocString          )
            .def("setBufferSize",                   &blmController::setBufferSize, setBufferSizeDocString                           )
            .def("restartContinuousMonitoring",     &blmController::restartContinuousMonitoring, restartMonitoringDocString         )
            .def("getTimebase",                     &blmController::getTimebase, getTimebaseDocString                               )
            .def("getBufferSize",                   &blmController::getBufferSize, getBufferSizeDocString                           )
            .def("getBLMTrace",                     &blmController::getBLMTrace_Py, getBLMTraceDocString                            )
            .def("getBLMTraces",                    &blmController::getBLMTraces_Py, getBLMTracesDocString                          )
            .def("getBLMTraceBuffer",               &blmController::getBLMTraceBuffer_Py, getBLMTraceBufferDocString                )
            .def("getBLMCH1WaveformBuffer",         &blmController::getBLMCH1WaveformBuffer_Py, getBLMCH1WaveformBufferDocString    )
            .def("getBLMCH2WaveformBuffer",         &blmController::getBLMCH2WaveformBuffer_Py, getBLMCH2WaveformBufferDocString    )
            .def("getBLMCH3WaveformBuffer",         &blmController::getBLMCH3WaveformBuffer_Py, getBLMCH3WaveformBufferDocString    )
            .def("getBLMCH4WaveformBuffer",         &blmController::getBLMCH4WaveformBuffer_Py, getBLMCH4WaveformBufferDocString    )
            .def("getBLMCH1TimeBuffer",             &blmController::getBLMCH1TimeBuffer_Py, getBLMCH1TimeBufferDocString            )
            .def("getBLMCH2TimeBuffer",             &blmController::getBLMCH2TimeBuffer_Py, getBLMCH2TimeBufferDocString            )
            .def("getBLMCH3TimeBuffer",             &blmController::getBLMCH3TimeBuffer_Py, getBLMCH3TimeBufferDocString            )
            .def("getBLMCH4TimeBuffer",             &blmController::getBLMCH4TimeBuffer_Py, getBLMCH4TimeBufferDocString            )
            .def("getBLMCH1Waveform",               &blmController::getBLMCH1Waveform_Py, getBLMCH1WaveformDocString                )
            .def("getBLMCH2Waveform",               &blmController::getBLMCH2Waveform_Py, getBLMCH2WaveformDocString                )
            .def("getBLMCH3Waveform",               &blmController::getBLMCH3Waveform_Py, getBLMCH3WaveformDocString                )
            .def("getBLMCH4Waveform",               &blmController::getBLMCH4Waveform_Py, getBLMCH4WaveformDocString                )
            .def("getBLMCH1Time",                   &blmController::getBLMCH1Time_Py, getBLMCH1TimeDocString                        )
            .def("getBLMCH2Time",                   &blmController::getBLMCH2Time_Py, getBLMCH2TimeDocString                        )
            .def("getBLMCH3Time",                   &blmController::getBLMCH3Time_Py, getBLMCH3TimeDocString                        )
            .def("getBLMCH4Time",                   &blmController::getBLMCH4Time_Py, getBLMCH4TimeDocString                        )
            .def("getMinOfTraces",                  &blmController::getMinOfTraces_Py, getMinOfTracesDocString                      )
            .def("getMaxOfTraces",                  &blmController::getMaxOfTraces_Py, getMaxOfTracesDocString                      )
            .def("getAreaUnderTraces",              &blmController::getAreaUnderTraces_Py, getAreaUnderTracesDocString              )
            .def("getAreaUnderPartOfTrace",         &blmController::getAreaUnderPartOfTrace_Py, getAreaUnderPartOfTraceDocString    )
            .def("getAvgNoise",                     &blmController::getAvgNoise_Py, getAvgNoiseDocString                            )
            .def("getPartOfTrace",                  &blmController::getPartOfTrace, getPartOfTraceDocString                         )
            .def("getTimeStamps",                   &blmController::getTimeStamps_Py, getTimeStampsDocString                        )
            .def("getTimeStampsBuffer",             &blmController::getTimeStampsBuffer_Py, getTimeStampsDocString                  )
            .def("getStrTimeStamps",                &blmController::getStrTimeStamps_Py, getStrTimeStampsDocString                  )
            .def("getStrTimeStampsBuffer",          &blmController::getStrTimeStampsBuffer_Py, getStrTimeStampsDocString            )
            .def("monitorTracesForNShots",          &blmController::monitorTracesForNShots, monitorTracesDocString                  )
            .def("monitorATraceForNShots",          &blmController::monitorATraceForNShots, monitorATraceDocString                  )
            .def("getBLMNames",                     &blmController::getBLMNames_Py, getBLMNamesDocString                            )
            .def("getBLMPVs",                       &blmController::getBLMPVs_Py, getBLMPVsDocString                                )
            .def("getBLMTimePVs",                   &blmController::getBLMTimePVs_Py, getBLMTimePVsDocString                        )
            .def("getBLMWaveformPVs",               &blmController::getBLMWaveformPVs_Py, getBLMWaveformPVsDocString                )
            .def("getBLMPVStrings",                 &blmController::getBLMPVStrings_Py, getBLMPVStringsDocString                    )
            .def("getBLMTimePVStrings",             &blmController::getBLMTimePVStrings_Py, getBLMTimePVStringsDocString            )
            .def("getBLMWaveformPVStrings",         &blmController::getBLMWaveformPVStrings_Py, getBLMWaveformPVStringsDocString    )
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
        .def("virtual_C2B_BLM_Controller",  &VCblm::virtual_C2B_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_C2B_BLM_Controller",  &VCblm::offline_C2B_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_C2B_BLM_Controller", &VCblm::physical_C2B_BLM_Controller, return_value_policy<reference_existing_object>())
        .def("getBLMController",                  &VCblm::getBLMController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL
#endif // velaChrageBLMController_H

