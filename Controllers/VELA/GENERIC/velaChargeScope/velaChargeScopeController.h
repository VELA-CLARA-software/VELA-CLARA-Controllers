#ifndef vela_QSCOPE_CONTROLLER_H
#define vela_QSCOPE_CONTROLLER_H

// project
#include "scopeController.h"
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

class velaChargeScopeController : public scopeController
{
    public:

        /// we have overloaded constructors to specify config-file location

        velaChargeScopeController( const bool show_messages = true, const bool show_debug_messages = true );
        velaChargeScopeController( const std::string configFileLocation1,
                                   const std::string configFileLocation2, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaChargeScopeController();

#ifdef BUILD_DLL

        /// Include a function to export enum definitions to a python dictionary

        boost::python::dict getScopeStateDefinition();

        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

//        bool hasTrig_Py( const std::string & name );
//        bool hasNoTrig_Py( const std::string & name );

        bool isMonitoringScopeTraces_Py();
        bool isMonitoringScopeNums_Py();
        bool isNotMonitoringScopeTraces_Py();
        bool isNotMonitoringScopeNums_Py();

        double getScopeP1_Py( const std::string & name );
        double getScopeP2_Py( const std::string & name );
        double getScopeP3_Py( const std::string & name );
        double getScopeP4_Py( const std::string & name );
        double getWCMQ_Py()   ;
        double getICT1Q_Py()  ;
        double getICT2Q_Py()  ;
        double getFCUPQ_Py()  ;
        double getEDFCUPQ_Py();
        std::vector< std::vector< double > > getScopeTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getScopeNums_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getScopeP1Vec_Py( const std::string & name );
        std::vector< double > getScopeP2Vec_Py( const std::string & name );
        std::vector< double > getScopeP3Vec_Py( const std::string & name );
        std::vector< double > getScopeP4Vec_Py( const std::string & name );
        std::vector< double > getMinOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::string > getStrTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        void monitorTracesForNShots_Py( size_t N );
        void monitorNumsForNShots_Py( size_t N );
        std::vector< std::string > getScopeNames_Py();
//        VELA_ENUM::TRIG_STATE getScopeState_Py( const std::string & name );

        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string scopeName );

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

BOOST_PYTHON_MODULE( velaChargeScopeControl )
{
    /// Include ALL the enums you want to expose to Python

    class_< std::vector< std::vector< double > > > ("v2_double")
        .def( vector_indexing_suite< std::vector< std::vector< double > > >());

    class_< std::vector< double> > ("v_double")
        .def( vector_indexing_suite< std::vector< double > >());

    enum_<VELA_ENUM::TRIG_STATE>("TRIG_STATE")
            .value("NOTRIG",      VELA_ENUM::TRIG_STATE::NOTRIG      )
            .value("TRIG",        VELA_ENUM::TRIG_STATE::TRIG        )
            .value("TRIG_ERROR",  VELA_ENUM::TRIG_STATE::TRIG_ERROR  )
            ;

    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
            ;

    enum_<scopeStructs::SCOPE_PV_TYPE>("SCOPE_PV_TYPE")
            .value("TR1", scopeStructs::SCOPE_PV_TYPE::TR1 )
            .value("TR2", scopeStructs::SCOPE_PV_TYPE::TR2 )
            .value("TR3", scopeStructs::SCOPE_PV_TYPE::TR3 )
            .value("TR4", scopeStructs::SCOPE_PV_TYPE::TR4 )
            .value("P1",  scopeStructs::SCOPE_PV_TYPE::P1   )
            .value("P2",  scopeStructs::SCOPE_PV_TYPE::P2   )
            .value("P3",  scopeStructs::SCOPE_PV_TYPE::P3   )
            .value("P4",  scopeStructs::SCOPE_PV_TYPE::P4   )
            ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;

    /// we have to tell boost.python about pure virtual methods in abstract base classes

    boost::python::class_<controller, boost::python::bases<baseObject>,  boost::noncopyable> ("controller", boost::python::no_init)
//        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
//        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
		;
    /// member functiosn to expose to python, remmeber to include enum deifntions as boost::python::dict <int, string>


	boost::python::class_<velaChargeScopeController, boost::python::bases<controller>>("velaChargeScopeController")
            .def(boost::python::init<const std::string, const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional< const bool, const bool> >())
            .def("getScopeStateDefinition",         &velaChargeScopeController::getScopeStateDefinition                   )
            .def("getILockStatesDefinition",        &velaChargeScopeController::getILockStatesDefinition                  )
            .def("get_CA_PEND_IO_TIMEOUT",          &velaChargeScopeController::get_CA_PEND_IO_TIMEOUT                    )
            .def("set_CA_PEND_IO_TIMEOUT",          &velaChargeScopeController::set_CA_PEND_IO_TIMEOUT                    )
//            .def("getScopeState",                   &velaChargeScopeController::getScopeState_Py                          )
//            .def("getILockStates",                  &velaChargeScopeController::getILockStates_Py                         )
//            .def("hasNoTrig",                       &velaChargeScopeController::hasNoTrig_Py, boost::python::args("name") )
//            .def("hasTrig",                         &velaChargeScopeController::hasTrig_Py, boost::python::args("name")   )
            .def("isMonitoringScopeTraces",         &velaChargeScopeController::isMonitoringScopeTraces_Py                )
            .def("isMonitoringScopeNums",           &velaChargeScopeController::isMonitoringScopeNums_Py                  )
            .def("isNotMonitoringScopeTraces",      &velaChargeScopeController::isNotMonitoringScopeTraces_Py             )
            .def("isNotMonitoringScopeNums",        &velaChargeScopeController::isNotMonitoringScopeNums_Py               )
            .def("getScopeNums",                    &velaChargeScopeController::getScopeNums_Py                           )
            .def("getScopeP1Vec",                   &velaChargeScopeController::getScopeP1Vec_Py                          )
            .def("getScopeP2Vec",                   &velaChargeScopeController::getScopeP2Vec_Py                          )
            .def("getScopeP3Vec",                   &velaChargeScopeController::getScopeP3Vec_Py                          )
            .def("getScopeP4Vec",                   &velaChargeScopeController::getScopeP4Vec_Py                          )
            .def("getScopeP1",                      &velaChargeScopeController::getScopeP1_Py                             )
            .def("getScopeP2",                      &velaChargeScopeController::getScopeP2_Py                             )
            .def("getScopeP2",                      &velaChargeScopeController::getScopeP3_Py                             )
            .def("getScopeP2",                      &velaChargeScopeController::getScopeP4_Py                             )
            .def("getWCMQ",                         &velaChargeScopeController::getWCMQ_Py                                )
            .def("getICT1Q",                        &velaChargeScopeController::getICT1Q_Py                               )
            .def("getICT2Q",                        &velaChargeScopeController::getICT2Q_Py                               )
            .def("getFCUPQ",                        &velaChargeScopeController::getFCUPQ_Py                               )
            .def("getEDFCUPQ",                      &velaChargeScopeController::getEDFCUPQ_Py                             )
            .def("getScopeTraces",                  &velaChargeScopeController::getScopeTraces_Py                         )
            .def("getMinOfTraces",                  &velaChargeScopeController::getMinOfTraces_Py                         )
            .def("getMaxOfTraces",                  &velaChargeScopeController::getMaxOfTraces_Py                         )
            .def("getAreaUnderTraces",              &velaChargeScopeController::getAreaUnderTraces_Py                     )
            .def("getTimeStamps",                   &velaChargeScopeController::getTimeStamps_Py                          )
            .def("getStrTimeStamps",                &velaChargeScopeController::getStrTimeStamps_Py                       )
            .def("monitorNumsForNShots",            &velaChargeScopeController::monitorNumsForNShots_Py                   )
            .def("monitorTracesForNShots",          &velaChargeScopeController::monitorTracesForNShots_Py                 )
            .def("getScopeNames",                   &velaChargeScopeController::getScopeNames_Py                          )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &velaChargeScopeController::debugMessagesOff                          )
            .def("debugMessagesOn",                 &velaChargeScopeController::debugMessagesOn                           )
            .def("messagesOff",                     &velaChargeScopeController::messagesOff                               )
            .def("messagesOn",                      &velaChargeScopeController::messagesOn                                )
            .def("silence",                         &velaChargeScopeController::silence                                   )
            .def("verbose",                         &velaChargeScopeController::verbose                                   )
		;
};

#endif // BUILD_DLL

#endif // velaChrageScopeController_H

