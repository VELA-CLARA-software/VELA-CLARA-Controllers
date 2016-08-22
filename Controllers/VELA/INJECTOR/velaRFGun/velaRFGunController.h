#ifndef VELA_RFGUN_CONTROLLER_H_
#define VELA_RFGUN_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>

#include "RFGunController.h"

#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class velaRFGunController : public RFGunController
{
    public:
        /// we have overloaded constructors to specify config-file location
        velaRFGunController( const bool show_messages = true , const bool show_debug_messages = true, const std::string pydDirec="" );
        velaRFGunController( const std::string configFileLocation1,
                         const std::string configFileLocation2,
                         const std::string configFileLocation3, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaRFGunController( );

    #ifdef BUILD_DLL
        /// Include a function to export enum definitions to a python dictionary
        boost::python::dict getModStateDefintions( );
        boost::python::dict getModEXILOCK1Defintions( );

        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

        std::vector< std::vector< double >> getRevT_Py( const std::string name );
        std::vector< std::vector< double >> getFwdT_Py(const std::string name  );
        std::vector< double > getRevTStamp_Py(const std::string name  );
        std::vector< double > getFwdTStamp_Py(const std::string name  );
        std::vector< std::string > getRevTStampStr_Py( const std::string name );
        std::vector< std::string > getFwdTStampStr_Py( const std::string name  );

        double getFwd_Py( std::string name );
        double getRev_Py( std::string name );
        double getRatio_Py( std::string name );

        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

#endif // BUILD_DLL

    protected:
    private:

};
#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!

#define BOOST_LIB_DIAGNOSTIC
//#define BOOST_PYTHON_STATIC_LIB

//#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE( velaRFGunControl ) /// This name MUST match the dll file name http://stackoverflow.com/questions/8400244/boost-python-import-error-module-does-not-define-init-function
{

    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>() )
        ;

    class_<std::vector< double> >("std_vector_double")
        .def( vector_indexing_suite< std::vector< double>>() )
        ;

    class_< std::vector< std::vector< double >>  >("std_vector_vector_double")
        .def( vector_indexing_suite< std::vector< std::vector< double >> >() )
        ;

    enum_<RFGunStructs::MOD_EXILOCK1>("MOD_EXILOCK1")
            .value("GOOD", RFGunStructs::MOD_EXILOCK1::GOOD)
            .value("BAD", RFGunStructs::MOD_EXILOCK1::BAD)
            .value("ERROR", RFGunStructs::MOD_EXILOCK1::ERROR)
            .value("UNKNOWN", RFGunStructs::MOD_EXILOCK1::UNKNOWN)
            ;

    enum_<RFGunStructs::MOD_STATE>("MOD_STATE")
            .value("ERROR1",            RFGunStructs::MOD_STATE::ERROR1)
            .value("OFF",               RFGunStructs::MOD_STATE::OFF)
            .value("off_Request",       RFGunStructs::MOD_STATE::off_Request)
            .value("HV_Intrlock",       RFGunStructs::MOD_STATE::HV_Intrlock)
            .value("Standby_Request",   RFGunStructs::MOD_STATE::Standby_Request)
            .value("HV_Off_Requ",       RFGunStructs::MOD_STATE::HV_Off_Requ)
            .value("Trigger_Interl",    RFGunStructs::MOD_STATE::Trigger_Interl)
            .value("HV_On",             RFGunStructs::MOD_STATE::HV_On)
            .value("Trig_Off_Req",      RFGunStructs::MOD_STATE::Trig_Off_Req)
            .value("Trig_Request",      RFGunStructs::MOD_STATE::Trig_Request)
            .value("Trig",              RFGunStructs::MOD_STATE::Trig)
            ;

    /// Expose base classes

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;

    /// we have to tell boost.python about pure virtual methods in abstract base classes

    boost::python::class_<controller, boost::python::bases<baseObject>,  boost::noncopyable> ("controller", boost::python::no_init)
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
		;

    /// member functions to expose to python, remmeber to include enum definitions as boost::python::dict <int, string>
    /// as well as boost::python::dict <int, int>
    boost::python::class_<velaRFGunController, boost::python::bases<controller>>("velaRFGunController")
            .def(boost::python::init<const std::string, const std::string, const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional< const bool, const bool,const std::string> >())

            .def("monitorTracesForNShots",       &velaRFGunController::monitorTracesForNShots    )
            .def("isModILockStateGood",       &velaRFGunController::isModILockStateGood    )
            .def("isModILockStateNotGood",       &velaRFGunController::isModILockStateNotGood    )
            .def("isMonitoringTraces",       &velaRFGunController::isMonitoringTraces    )
            .def("isNotMonitoringTraces",       &velaRFGunController::isNotMonitoringTraces    )
            .def("isModWarmedUp",               &velaRFGunController::isModWarmedUp    )
            .def("isModNotWarmedUp",            &velaRFGunController::isModNotWarmedUp    )
            .def("isModInHVOn",                 &velaRFGunController::isModInHVOn    )
            .def("isModInTrig",                  &velaRFGunController::isModInTrig    )
            .def("isModInOff",                   &velaRFGunController::isModInOff    )
            .def("isModInStandby",               &velaRFGunController::isModInStandby    )
            .def("killTraceMonitors",            &velaRFGunController::killTraceMonitors    )
            .def("isMonitoringTraces",           &velaRFGunController::isMonitoringTraces    )
            .def("isNotMonitoringTraces",        &velaRFGunController::isNotMonitoringTraces    )
            .def("getModState",                  &velaRFGunController::isNotMonitoringTraces    )
            .def("getModiLock",                  &velaRFGunController::getModState    )
            .def("getModStateStr",               &velaRFGunController::getModStateStr    )
            .def("getModiLockStr",               &velaRFGunController::getModiLockStr    )
            .def("get_CA_PEND_IO_TIMEOUT",       &velaRFGunController::get_CA_PEND_IO_TIMEOUT    )
            .def("set_CA_PEND_IO_TIMEOUT",       &velaRFGunController::set_CA_PEND_IO_TIMEOUT    )
            .def("setModToOff",                  &velaRFGunController::setModToOff    )
            .def("setModToTrig",                 &velaRFGunController::setModToTrig    )
            .def("setModToStandby",              &velaRFGunController::setModToStandby    )
            .def("setModToHVOn",                 &velaRFGunController::setModToHVOn    )
            .def("setPhi",                       &velaRFGunController::setPhi    )
            .def("setAmp",                       &velaRFGunController::setAmp    )
            .def("modReset",                     &velaRFGunController::modReset    )
            .def("getRevT",                      &velaRFGunController::getRevT_Py    )
            .def("getFwdT",                      &velaRFGunController::getFwdT_Py    )
            .def("getRevTStamp",                 &velaRFGunController::getRevTStamp_Py    )
            .def("getFwdTStamp",                 &velaRFGunController::getFwdTStamp_Py    )
            .def("getRevTStampStr",              &velaRFGunController::getRevTStampStr_Py    )
            .def("getFwdTStampStr",              &velaRFGunController::getFwdTStampStr_Py    )
            .def("getFwd",                       &velaRFGunController::getFwd_Py    )
            .def("getRev",                       &velaRFGunController::getRev_Py    )
            .def("getAmpR",                       &velaRFGunController::getAmpR    )
            .def("getAmpW",                       &velaRFGunController::getAmpW    )
            .def("getPhi",                       &velaRFGunController::getPhi    )
            .def("getRatio",                     &velaRFGunController::getRatio_Py    )
            .def("getModStateDefintions",        &velaRFGunController::getModStateDefintions    )
            .def("getModEXILOCK1Defintions",     &velaRFGunController::getModEXILOCK1Defintions    )
            .def("getRFPowerNames",              &velaRFGunController::getRFPowerNames    )
            .def("setModToOff",                  &velaRFGunController::setModToOff    )
            .def("setModToStandby",              &velaRFGunController::setModToStandby    )
            .def("switchOnRF",                   &velaRFGunController::switchOnRF    )
            .def("switchOffRF",                  &velaRFGunController::switchOffRF    )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",             &velaRFGunController::debugMessagesOff )
            .def("debugMessagesOn",              &velaRFGunController::debugMessagesOn )
            .def("messagesOff",                  &velaRFGunController::messagesOff )
            .def("messagesOn",                   &velaRFGunController::messagesOn )
            .def("silence",                      &velaRFGunController::silence )
            .def("verbose",                      &velaRFGunController::verbose )
            .def("isSilent",                     &velaRFGunController::isSilent )
            .def("isVerbose",                    &velaRFGunController::isVerbose )
            .def("isMessageOn",                  &velaRFGunController::isMessageOn )
            .def("isDebugMessageOn",             &velaRFGunController::isDebugMessageOn )
            ;
}
#endif // BUILD_DLL
#endif // VELA_RFGUN_CONTROLLER_H_


