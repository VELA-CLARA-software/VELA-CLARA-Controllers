#ifndef velaPILShutter_CONTROLLER_H
#define velaPILShutter_CONTROLLER_H

// project
#include "shutterController.h"

// stl
#include <string>
#include <vector>
#include <map>
//boost

#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif


class velaPhotoInjectorLaserShutterController : public shutterController
{
    public:

        /// we have overloaded constructors to specify config-file location
        velaPhotoInjectorLaserShutterController( const bool show_messages = true , const bool show_debug_messages = true );
        velaPhotoInjectorLaserShutterController( const std::string configFileLocation, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaPhotoInjectorLaserShutterController( );

#ifdef BUILD_DLL
        /// Include a function to export enum definitions to a python dictionary
        boost::python::dict getShutterStateDefinition();

        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

        void open_Py( const std::string name );
        void close_Py( const std::string name );
        bool isOpen_Py( const std::string name );
        bool isClosed_Py( const std::string name );
        bool closeAndWait_Py( const std::string name, const time_t waitTime = 2 );
        bool openAndWait_Py ( const std::string name, const time_t waitTime = 2 );
        VELA_ENUM::SHUTTER_STATE getShutterState_Py( const std::string name );
        std::string getShutterStateStr_Py( const std::string name );


        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string shutterName );
        boost::python::dict getILockStatesStr_Py( std::string shutterName );
#endif // BUILD_DLL
    private:
};

#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!

#define BOOST_LIB_DIAGNOSTIC

using namespace boost::python;

/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// For examples See magnets...
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads1, openAndWaitShutter1 , 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads2, openAndWaitShutter2 , 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads3, closeAndWaitShutter1, 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads4, closeAndWaitShutter2, 0, 1 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads5, closeAndWait_Py     , 1, 2 );
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads6, openAndWait_Py      , 1, 2 );

/// As another example we have overloaded constructors to enable / disable messages
/// and / or debug messages when instantiating. See here
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/exposing.html
/// and beware of: http://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor

BOOST_PYTHON_MODULE( velaPhotoInjectorLaserShutterControl )
{
/// //http://www.cs.fsu.edu/~lacher/boost_1_32_0/libs/python/doc/tutorial/doc/html/python/exposing.html
/// expose any containers used in your class, e.g.
///  vector_indexing_suite seems to work, i haven't had much success with map_indexing_suite, in any case we convert maps to  boost::python::dict
//	class_< std::vector< double > >("std_vector_double")
//		.def(vector_indexing_suite< std::vector< double > >() )
//	;
	class_<std::vector< std::string > >("std_vector_string")
		.def( vector_indexing_suite< std::vector< std::string >>() )
		;

    /// Include ALL the enums you want to expose to Python
    boost::python::enum_<VELA_ENUM::SHUTTER_STATE>("SHUTTER_STATE")
            .value("SHUTTER_CLOSED", VELA_ENUM::SHUTTER_STATE::SHUTTER_CLOSED )
            .value("SHUTTER_OPEN",   VELA_ENUM::SHUTTER_STATE::SHUTTER_OPEN   )
            .value("SHUTTER_TIMING", VELA_ENUM::SHUTTER_STATE::SHUTTER_TIMING )
            .value("SHUTTER_ERROR",  VELA_ENUM::SHUTTER_STATE::SHUTTER_ERROR  )
            ;
    boost::python::enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
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
	boost::python::class_<velaPhotoInjectorLaserShutterController, boost::python::bases<controller>>("velaPhotoInjectorLaserShutterController")
            .def(boost::python::init<const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional< const bool, const bool> >())
            .def("getShutterStateDefinition",       &velaPhotoInjectorLaserShutterController::getShutterStateDefinition )
            .def("getILockStatesDefinition",        &velaPhotoInjectorLaserShutterController::getILockStatesDefinition  )
            .def("get_CA_PEND_IO_TIMEOUT",          &velaPhotoInjectorLaserShutterController::get_CA_PEND_IO_TIMEOUT    )
            .def("set_CA_PEND_IO_TIMEOUT",          &velaPhotoInjectorLaserShutterController::set_CA_PEND_IO_TIMEOUT    )
            .def("closeAndWaitShutter1",            &velaPhotoInjectorLaserShutterController::closeAndWaitShutter1, vpsc_overloads1() )
            .def("closeAndWaitShutter2",            &velaPhotoInjectorLaserShutterController::closeAndWaitShutter2, vpsc_overloads2() )
            .def("openAndWaitShutter1",             &velaPhotoInjectorLaserShutterController::openAndWaitShutter1 , vpsc_overloads3() )
            .def("openAndWaitShutter2",             &velaPhotoInjectorLaserShutterController::openAndWaitShutter2 , vpsc_overloads4() )
            .def("getShutterState",                 &velaPhotoInjectorLaserShutterController::getShutterState_Py        )
            .def("getILockStates",                  &velaPhotoInjectorLaserShutterController::getILockStates_Py         )
            .def("getShutterNames",                 &velaPhotoInjectorLaserShutterController::getShutterNames      )
            .def("getILockStatesStr",               &velaPhotoInjectorLaserShutterController::getILockStatesStr_Py      )
            .def("closeShutter1",                   &velaPhotoInjectorLaserShutterController::closeShutter1            )
            .def("closeShutter2",                   &velaPhotoInjectorLaserShutterController::closeShutter2            )
            .def("openShutter1",                    &velaPhotoInjectorLaserShutterController::openShutter1             )
            .def("openShutter2",                    &velaPhotoInjectorLaserShutterController::openShutter2             )
            .def("closeAndWait",                    &velaPhotoInjectorLaserShutterController::closeAndWait_Py , vpsc_overloads5() )
            .def("openAndWait",                     &velaPhotoInjectorLaserShutterController::openAndWait_Py  , vpsc_overloads6() )
            .def("isClosed",                        &velaPhotoInjectorLaserShutterController::isClosed_Py, boost::python::args("name") )
            .def("isOpen",                          &velaPhotoInjectorLaserShutterController::isOpen_Py )
            .def("open",                            &velaPhotoInjectorLaserShutterController::open_Py )
            .def("close",                           &velaPhotoInjectorLaserShutterController::close_Py )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &velaPhotoInjectorLaserShutterController::debugMessagesOff )
            .def("debugMessagesOn",                 &velaPhotoInjectorLaserShutterController::debugMessagesOn )
            .def("messagesOff",                     &velaPhotoInjectorLaserShutterController::messagesOff )
            .def("messagesOn",                      &velaPhotoInjectorLaserShutterController::messagesOn )
            .def("silence",                         &velaPhotoInjectorLaserShutterController::silence )
            .def("verbose",                         &velaPhotoInjectorLaserShutterController::verbose )
            .def("isSilent",                        &velaPhotoInjectorLaserShutterController::isSilent )
            .def("isVerbose",                       &velaPhotoInjectorLaserShutterController::isVerbose )
            .def("isMessageOn",                     &velaPhotoInjectorLaserShutterController::isMessageOn )
            .def("isDebugMessageOn",                &velaPhotoInjectorLaserShutterController::isDebugMessageOn )
            ;















};//BOOST_PYTHON_MODULE

#endif // BUILD_DLL

#endif // velaPILShutter_CONTROLLER_H

