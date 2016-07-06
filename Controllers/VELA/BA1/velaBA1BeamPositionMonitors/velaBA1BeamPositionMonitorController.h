#ifndef vela_INJ_BPM_CONTROLLER_H
#define vela_INJ_BPM_CONTROLLER_H

// project

#include "beamPositionMonitorController.h"
// stl
#include <string>
#include <vector>
//boost
#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class velaBA1BeamPositionMonitorController : public beamPositionMonitorController
{
    public:

        /// we have overloaded constructors to specify config-file location

        velaBA1BeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true );
        velaBA1BeamPositionMonitorController( const std::string configFileLocation, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaBA1BeamPositionMonitorController();

#ifdef BUILD_DLL

        /// Include a function to export enum definitions to a python dictionary

        boost::python::dict getBPMStateDefinition();

        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

        bool hasTrig_Py( const std::string & name );
        bool hasNoTrig_Py( const std::string & name );
        double getX_Py( const std::string & bpm );
        double getY_Py( const std::string & bpm  );
        double getQ_Py( const std::string & bpm  );
        double getXFromPV_Py( const std::string & bpm  );
        double getYFromPV_Py( const std::string & bpm  );
//        beamPositionMonitorStructs::rawDataStruct getAllBPMData( const std::string & name, size_t N );
        std::vector< std::vector< double > > getBPMRawData_Py( const std::string & bpmName );
        std::vector< double > getBPMXVec_Py( const std::string & bpmName );
        std::vector< double > getBPMYVec_Py( const std::string & bpmName );
        std::vector< double > getBPMQVec_Py( const std::string & bpmName );
        std::vector< double > getTimeStamps_Py( const std::string & bpmName );
        std::vector< std::string > getStrTimeStamps_Py( const std::string & bpmName );
        long getRA1_Py( const std::string & bpmName );
        long getRA2_Py( const std::string & bpmName );
        long getRD1_Py( const std::string & bpmName );
        long getRD2_Py( const std::string & bpmName );
        void setSA1_Py( const std::string & bpmName, long sa1 );
        void setSA2_Py( const std::string & bpmName, long sa2 );
        void setSD1_Py( const std::string & bpmName, long sd1 );
        void setSD2_Py( const std::string & bpmName, long sd2 );
        beamPositionMonitorStructs::rawDataStruct getAllBPMData_Py( const std::string & name, size_t N );
        void monitorDataForNShots_Py( size_t N, const std::string & bpmNames );
        std::vector< std::string > getBPMNames_Py();

//        VELA_ENUM::TRIG_STATE getBPMState_Py( const std::string & name );

        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string bpmName );

#endif // BUILD_DLL

    protected:
    private:

};

#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!

#define BOOST_LIB_DIAGNOSTIC

/// FUNCTION OVERLOADING, if you have overloaded functions:
/// Create a load of different function pointers and use them in the bindings
/// For examples See magnets...

/// As another example we have overloaded constructors to enable / disable messages
/// and / or debug messages when instantiating. See here
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/exposing.html
/// and beware of: http://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor


using namespace boost::python;


//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vvvc_overloads1, openAndWait_Py , 0, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vvvc_overloads2, closeAndWait_Py , 0, 1 );

BOOST_PYTHON_MODULE( velaBA1BeamPositionMonitorControl )
{
    /// Include ALL the enums you want to expose to Python

    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>() )
        ;

    class_< std::vector< std::vector< double > > > ("v2_double")
        .def( vector_indexing_suite< std::vector< std::vector< double > > >());

    class_< std::vector< double> > ("v_double")
        .def( vector_indexing_suite< std::vector< double > >());

//    enum_<VELA_ENUM::TRIG_STATE>("TRIG_STATE")
//            .value("NOTRIG",      VELA_ENUM::TRIG_STATE::NOTRIG )
//            .value("TRIG",        VELA_ENUM::TRIG_STATE::TRIG   )
//            .value("TRIG_ERROR",  VELA_ENUM::TRIG_STATE::TRIG_ERROR  )
//            ;
//    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
//            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
//            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
//            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
//            ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;

    /// we have to tell boost.python about pure virtual methods in abstract base classes

    boost::python::class_<controller, boost::python::bases<baseObject>,  boost::noncopyable> ("controller", boost::python::no_init)
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
		;
    /// member functiosn to expose to python, remmeber to include enum deifntions as boost::python::dict <int, string>


	boost::python::class_<velaBA1BeamPositionMonitorController, boost::python::bases<controller>>("velaBA1BeamPositionMonitorController")
            .def(boost::python::init<const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional< const bool, const bool> >())
            .def("getBPMStateDefinition",           &velaBA1BeamPositionMonitorController::getBPMStateDefinition                     )
            .def("getILockStatesDefinition",        &velaBA1BeamPositionMonitorController::getILockStatesDefinition                  )
            .def("get_CA_PEND_IO_TIMEOUT",          &velaBA1BeamPositionMonitorController::get_CA_PEND_IO_TIMEOUT                    )
            .def("set_CA_PEND_IO_TIMEOUT",          &velaBA1BeamPositionMonitorController::set_CA_PEND_IO_TIMEOUT                    )
//            .def("getBPMState",                     &velaBA1BeamPositionMonitorController::getBPMState_Py                            )
            .def("getILockStates",                  &velaBA1BeamPositionMonitorController::getILockStates_Py                         )
//            .def("hasNoTrig",                       &velaBA1BeamPositionMonitorController::hasNoTrig_Py, boost::python::args("name") )
//            .def("hasTrig",                         &velaBA1BeamPositionMonitorController::hasTrig_Py, boost::python::args("name")   )
            .def("getX",                            &velaBA1BeamPositionMonitorController::getX_Py                                   )
            .def("getY",                            &velaBA1BeamPositionMonitorController::getY_Py                                   )
            .def("getQ",                            &velaBA1BeamPositionMonitorController::getQ_Py                                   )
            .def("getXFromPV",                      &velaBA1BeamPositionMonitorController::getXFromPV_Py                             )
            .def("getYFromPV",                      &velaBA1BeamPositionMonitorController::getYFromPV_Py                             )
            .def("getBPMXVec",                      &velaBA1BeamPositionMonitorController::getBPMXVec_Py                             )
            .def("getBPMYVec",                      &velaBA1BeamPositionMonitorController::getBPMYVec_Py                             )
            .def("getBPMQVec",                      &velaBA1BeamPositionMonitorController::getBPMQVec_Py                             )
            .def("getAllBPMData",                   &velaBA1BeamPositionMonitorController::getAllBPMData_Py                          )
            .def("getTimeStamps",                   &velaBA1BeamPositionMonitorController::getTimeStamps_Py                          )
            .def("getStrTimeStamps",                &velaBA1BeamPositionMonitorController::getStrTimeStamps_Py                       )
//            .def("getBPMRawData",                   &velaBA1BeamPositionMonitorController::getBPMRawData_Py                          )
            .def("getRA1",                          &velaBA1BeamPositionMonitorController::getRA1_Py                                 )
            .def("getRA2",                          &velaBA1BeamPositionMonitorController::getRA2_Py                                 )
            .def("getRD1",                          &velaBA1BeamPositionMonitorController::getRD1_Py                                 )
            .def("getRD2",                          &velaBA1BeamPositionMonitorController::getRD2_Py                                 )
            .def("setSA1",                          &velaBA1BeamPositionMonitorController::setSA1_Py                                 )
            .def("setSA2",                          &velaBA1BeamPositionMonitorController::setSA2_Py                                 )
            .def("setSD1",                          &velaBA1BeamPositionMonitorController::setSD1_Py                                 )
            .def("setSD2",                          &velaBA1BeamPositionMonitorController::setSD2_Py                                 )
            .def("monitorDataForNShots",            &velaBA1BeamPositionMonitorController::monitorDataForNShots_Py                   )
            .def("getBPMNames",                     &velaBA1BeamPositionMonitorController::getBPMNames_Py                            )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &velaBA1BeamPositionMonitorController::debugMessagesOff                          )
            .def("debugMessagesOn",                 &velaBA1BeamPositionMonitorController::debugMessagesOn                           )
            .def("messagesOff",                     &velaBA1BeamPositionMonitorController::messagesOff                               )
            .def("messagesOn",                      &velaBA1BeamPositionMonitorController::messagesOn                                )
            .def("silence",                         &velaBA1BeamPositionMonitorController::silence                                   )
            .def("verbose",                         &velaBA1BeamPositionMonitorController::verbose                                   )
		;
};

#endif // BUILD_DLL

#endif // vela_INJ_BPM_CONTROLLER_H

