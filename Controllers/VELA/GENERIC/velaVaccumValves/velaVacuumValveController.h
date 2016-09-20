#ifndef vela_VAC_VALVE_CONTROLLER_H
#define vela_VAC_VALVE_CONTROLLER_H

// project

#include "vacuumValveController.h"
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

class velaVacuumValveController : public vacuumValveController
{
    public:

        /// we have overloaded constructors to specify config-file location

        velaVacuumValveController(const bool RealMachine = false, const bool show_messages = true, const bool show_debug_messages = true );
        velaVacuumValveController( const std::string configFileLocation, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaVacuumValveController();

#ifdef BUILD_DLL

        /// Include a function to export enum definitions to a python dictionary

        boost::python::dict getVacValveStateDefinition();

        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

        void openValve_Py( const std::string & name );
        void closeValve_Py( const std::string & name );
        void openValve1_Py( );
        void openValve2_Py( );
        void openValve3_Py( );
        void openValve4_Py( );
        void openValve5_Py( );
        void openValve6_Py( );
        void openValve7_Py( );
        void closeValve1_Py( );
        void closeValve2_Py( );
        void closeValve3_Py( );
        void closeValve4_Py( );
        void closeValve5_Py( );
        void closeValve6_Py( );
        void closeValve7_Py( );
        std::vector< std::string > getVacValveNames_Py();
        bool isOpen_Py( const std::string & name );
        bool isClosed_Py( const std::string & name );
        bool openAndWaitValve_Py( const std::string & name, const time_t waitTime = 2 );
        bool closeAndWaitValve_Py ( const std::string & name, const time_t waitTime = 2 );
        bool openAndWaitValve1_Py( const time_t waitTime = 2 );
        bool openAndWaitValve2_Py( const time_t waitTime = 2 );
        bool openAndWaitValve3_Py( const time_t waitTime = 2 );
        bool openAndWaitValve4_Py( const time_t waitTime = 2 );
        bool openAndWaitValve5_Py( const time_t waitTime = 2 );
        bool openAndWaitValve6_Py( const time_t waitTime = 2 );
        bool openAndWaitValve7_Py( const time_t waitTime = 2 );
        bool closeAndWaitValve1_Py ( const time_t waitTime = 2 );
        bool closeAndWaitValve2_Py ( const time_t waitTime = 2 );
        bool closeAndWaitValve3_Py ( const time_t waitTime = 2 );
        bool closeAndWaitValve4_Py ( const time_t waitTime = 2 );
        bool closeAndWaitValve5_Py ( const time_t waitTime = 2 );
        bool closeAndWaitValve6_Py ( const time_t waitTime = 2 );
        bool closeAndWaitValve7_Py ( const time_t waitTime = 2 );
        VELA_ENUM::VALVE_STATE getVacValveState_Py( const std::string & name );

        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string vacValveName );

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

BOOST_PYTHON_MODULE( velaVacuumValveControl )
{
    /// Include ALL the enums you want to expose to Python
    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>() )
        ;

    enum_<VELA_ENUM::VALVE_STATE>("STATE")
            .value("VALVE_CLOSED", VELA_ENUM::VALVE_STATE::VALVE_CLOSED )
            .value("VALVE_OPEN",   VELA_ENUM::VALVE_STATE::VALVE_OPEN   )
            .value("VALVE_TIMING", VELA_ENUM::VALVE_STATE::VALVE_TIMING )
            .value("VALVE_ERROR",  VELA_ENUM::VALVE_STATE::VALVE_ERROR  )
            ;
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
            ;

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


	boost::python::class_<velaVacuumValveController, boost::python::bases<controller>>("velaVacuumValveController")
            .def(boost::python::init<const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional<const bool, const bool, const bool> >())
            .def("getVacValveStateDefinition",      &velaVacuumValveController::getVacValveStateDefinition               )
            .def("getILockStatesDefinition",        &velaVacuumValveController::getILockStatesDefinition                 )
            .def("get_CA_PEND_IO_TIMEOUT",          &velaVacuumValveController::get_CA_PEND_IO_TIMEOUT                   )
            .def("set_CA_PEND_IO_TIMEOUT",          &velaVacuumValveController::set_CA_PEND_IO_TIMEOUT                   )
            .def("getVacValveState",                &velaVacuumValveController::getVacValveState_Py                      )
            .def("getILockStates",                  &velaVacuumValveController::getILockStates_Py                        )
            .def("closeValve",                      &velaVacuumValveController::closeValve_Py                            )
            .def("closeValve1",                     &velaVacuumValveController::closeValve1_Py                           )
            .def("closeValve2",                     &velaVacuumValveController::closeValve2_Py                           )
            .def("closeValve3",                     &velaVacuumValveController::closeValve3_Py                           )
            .def("closeValve4",                     &velaVacuumValveController::closeValve4_Py                           )
            .def("closeValve5",                     &velaVacuumValveController::closeValve5_Py                           )
            .def("closeValve6",                     &velaVacuumValveController::closeValve6_Py                           )
            .def("closeValve7",                     &velaVacuumValveController::closeValve7_Py                           )
            .def("openValve",                       &velaVacuumValveController::openValve_Py                             )
            .def("openValve1",                      &velaVacuumValveController::openValve1_Py                            )
            .def("openValve2",                      &velaVacuumValveController::openValve2_Py                            )
            .def("openValve3",                      &velaVacuumValveController::openValve3_Py                            )
            .def("openValve4",                      &velaVacuumValveController::openValve4_Py                            )
            .def("openValve5",                      &velaVacuumValveController::openValve5_Py                            )
            .def("openValve6",                      &velaVacuumValveController::openValve6_Py                            )
            .def("openValve7",                      &velaVacuumValveController::openValve7_Py                            )
            .def("closeAndWaitValve",               &velaVacuumValveController::closeAndWaitValve_Py                     )
            .def("closeAndWaitValve1",              &velaVacuumValveController::closeAndWaitValve1_Py                    )
            .def("closeAndWaitValve2",              &velaVacuumValveController::closeAndWaitValve2_Py                    )
            .def("closeAndWaitValve3",              &velaVacuumValveController::closeAndWaitValve3_Py                    )
            .def("closeAndWaitValve4",              &velaVacuumValveController::closeAndWaitValve4_Py                    )
            .def("closeAndWaitValve5",              &velaVacuumValveController::closeAndWaitValve5_Py                    )
            .def("closeAndWaitValve6",              &velaVacuumValveController::closeAndWaitValve6_Py                    )
            .def("closeAndWaitValve7",              &velaVacuumValveController::closeAndWaitValve7_Py                    )
            .def("openAndWaitValve",               &velaVacuumValveController::openAndWaitValve_Py                       )
            .def("openAndWaitValve1",              &velaVacuumValveController::openAndWaitValve1_Py                      )
            .def("openAndWaitValve2",              &velaVacuumValveController::openAndWaitValve2_Py                      )
            .def("openAndWaitValve3",              &velaVacuumValveController::openAndWaitValve3_Py                      )
            .def("openAndWaitValve4",              &velaVacuumValveController::openAndWaitValve4_Py                      )
            .def("openAndWaitValve5",              &velaVacuumValveController::openAndWaitValve5_Py                      )
            .def("openAndWaitValve6",              &velaVacuumValveController::openAndWaitValve6_Py                      )
            .def("openAndWaitValve7",              &velaVacuumValveController::openAndWaitValve7_Py                      )
            .def("getVacValveNames",               &velaVacuumValveController::getVacValveNames_Py                       )
//            .def("closeAndWait",                    &velaVacuumValveController::closeAndWait_Py , vvvc_overloads1()      )
//            .def("openAndWait",                     &velaVacuumValveController::openAndWait_Py  , vvvc_overloads2()      )
            .def("isClosed",                        &velaVacuumValveController::isClosed_Py, boost::python::args("name") )
            .def("isOpen",                          &velaVacuumValveController::isOpen_Py                                )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &velaVacuumValveController::debugMessagesOff                         )
            .def("debugMessagesOn",                 &velaVacuumValveController::debugMessagesOn                          )
            .def("messagesOff",                     &velaVacuumValveController::messagesOff                              )
            .def("messagesOn",                      &velaVacuumValveController::messagesOn                               )
            .def("silence",                         &velaVacuumValveController::silence                                  )
            .def("verbose",                         &velaVacuumValveController::verbose                                  )
		;
};

#endif // BUILD_DLL

#endif // vela_VAC_VALVE_CONTROLLER_H

