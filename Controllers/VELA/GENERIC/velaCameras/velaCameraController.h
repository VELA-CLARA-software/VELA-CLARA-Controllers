#ifndef VELA_CAM_CONTROLLER_H
#define VELA_CAM_CONTROLLER_H
// stl
#include <string>
#include <vector>
#include <map>
// djs

#include "cameraController.h"

#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class velaCameraController  : public cameraController
{
    public:
        /// we have overloaded constructors to specify config-file location
        velaCameraController(const bool RealMachine = false, const bool show_messages = true , const bool show_debug_messages = true );
        velaCameraController( const std::string configFileLocation,  const std::string configIOCFileLocation, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaCameraController( );

#ifdef BUILD_DLL
//        boost::python::dict  getILockStates_Py( const std::string name );
//        boost::python::dict  getILockStatesStr_Py( const std::string name );

        bool isON_Py ( const std::string cam );
        bool isOFF_Py( const std::string cam );

        bool isMonitoring_Py( const std::string cam );
        bool isNotMonitoring_Py( const std::string cam );

        bool start_Py1(const std::string cam );
        bool start_Py2( const std::vector< std::string  > cam );
        bool stop_Py1(const std::string cam );
        bool stop_Py2( const std::vector< std::string  >  cam );


        bool startAndWait_Py1( const std::string cam, size_t timeout );
        std::vector< std::string > startAndWait_Py2( const std::vector< std::string  > cam, size_t timeout );

        bool startCamDataMonitor_Py( const std::string cam, size_t N = -1 );

        std::vector< std::vector< cameraStructs::camDataType >> getRawData_Py(const std::string name );
        std::vector< double > getRawDataStamp_Py(const std::string name );
        std::vector< std::string > getRawDataStampStr_Py( const std::string name );
#endif

    protected:
    private:


};
#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!

#define BOOST_LIB_DIAGNOSTIC

/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// For examples See magnets...
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads1, getSI_Py1 , 1, 1 );minimum of 1 and a maximum of 3 arguments
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( cam_overloads1,  start_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( cam_overloads2,  start_Py2, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( cam_overloads3,  stop_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( cam_overloads4,  stop_Py2, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( cam_overloads4,  stop_Py2, 1, 1 );

using namespace boost::python;

BOOST_PYTHON_MODULE( velaCameraControl ) /// This name MUST match the dll file name http://stackoverflow.com/questions/8400244/boost-python-import-error-module-does-not-define-init-function
{

    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>() )
        ;

    class_<std::vector< double> >("std_vector_double")
        .def( vector_indexing_suite< std::vector< double>>() )
        ;
    class_<std::vector< cameraStructs::camDataType> >("std_vector_camDataType")
        .def( vector_indexing_suite< std::vector< cameraStructs::camDataType>>() )
        ;
    class_<std::vector< std::vector< cameraStructs::camDataType >> >("std_vector_vector_camDataType")
        .def( vector_indexing_suite< std::vector< std::vector<cameraStructs::camDataType>> >() )
        ;

//    enum_<velaRFStructs::MOD_EXILOCK1>("MOD_EXILOCK1")
//            .value("GOOD", velaRFStructs::MOD_EXILOCK1::GOOD)
//            .value("BAD", velaRFStructs::MOD_EXILOCK1::BAD)
//            .value("ERROR", velaRFStructs::MOD_EXILOCK1::ERROR)
//            .value("UNKNOWN", velaRFStructs::MOD_EXILOCK1::UNKNOWN)
//            ;

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
    boost::python::class_<velaCameraController, boost::python::bases<controller>, boost::noncopyable >("velaCameraController")
            .def(boost::python::init<const std::string, const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional<const bool, const bool, const bool> >())
            .def("isON",                     &velaCameraController::isON_Py            )
            .def("isOFF",                    &velaCameraController::isOFF_Py           )
            .def("isMonitoring",             &velaCameraController::isMonitoring_Py    )
            .def("isNotMonitoring",          &velaCameraController::isNotMonitoring_Py )
            .def("start",                    &velaCameraController::start_Py1          )
            .def("start",                    &velaCameraController::start_Py2          )
            .def("stop",                     &velaCameraController::stop_Py1           )
            .def("stop",                     &velaCameraController::stop_Py2           )
            .def("startCamDataMonitor",      &velaCameraController::startCamDataMonitor_Py )
            .def("getRawData",               &velaCameraController::getRawData_Py          )
            .def("getRawDataStamp",          &velaCameraController::getRawDataStamp_Py     )
            .def("getRawDataStampStr",       &velaCameraController::getRawDataStampStr_Py  )
            .def("stopAll",                  &velaCameraController::stopAll                )
            .def("startAndWait",             &velaCameraController::startAndWait_Py1       )
            .def("startAndWait",             &velaCameraController::startAndWait_Py2       )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",         &velaCameraController::debugMessagesOff )
            .def("debugMessagesOn",          &velaCameraController::debugMessagesOn  )
            .def("messagesOff",              &velaCameraController::messagesOff      )
            .def("messagesOn",               &velaCameraController::messagesOn       )
            .def("silence",                  &velaCameraController::silence          )
            .def("verbose",                  &velaCameraController::verbose          )
            ;
}
#endif // BUILD_DLL
#endif // velaCameraController_H

