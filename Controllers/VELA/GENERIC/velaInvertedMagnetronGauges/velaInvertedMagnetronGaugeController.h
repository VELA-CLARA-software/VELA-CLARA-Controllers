#ifndef VELA_IMG_CONTROLLER_H
#define VELA_IMG_CONTROLLER_H

// project

#include "invertedMagnetronGaugeController.h"
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

class velaInvertedMagnetronGaugeController : public invertedMagnetronGaugeController
{
    public:

        /// we have overloaded constructors to specify config-file location

        velaInvertedMagnetronGaugeController( const bool show_messages = true, const bool show_debug_messages = true );
        velaInvertedMagnetronGaugeController( const std::string configFileLocation, const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaInvertedMagnetronGaugeController();



#ifdef BUILD_DLL

        /// Include a function to export enum definitions to a python dictionary

        boost::python::dict getImgStateDefinition();

        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

        double getImgP_Py( const std::string & name );
        std::vector< std::vector< double > > monitorForNCounts_Py( const std::string & name, int numcounts );
        VELA_ENUM::IMG_STATE getImgState_Py( const std::string & name );

        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string vacImgName );

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

BOOST_PYTHON_MODULE( velaInvertedMagnetronGaugeControl )
{
    /// Include ALL the enums you want to expose to Python

    class_< std::vector< std::vector< double > > > ("v2_double")
        .def( vector_indexing_suite< std::vector< std::vector< double > > >());

    class_< std::vector< double> > ("v_double")
        .def( vector_indexing_suite< std::vector< double > >());

    enum_<VELA_ENUM::IMG_STATE>("IMG_STATE")
            .value("IMG_BAD",    VELA_ENUM::IMG_STATE::IMG_BAD    )
            .value("IMG_GOOD",   VELA_ENUM::IMG_STATE::IMG_GOOD   )
            .value("IMG_ERROR",  VELA_ENUM::IMG_STATE::IMG_ERROR  )
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
//        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
//        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
		;
    /// member functiosn to expose to python, remmeber to include enum deifntions as boost::python::dict <int, string>


	boost::python::class_<velaInvertedMagnetronGaugeController, boost::python::bases<controller>>("velaInvertedMagnetronGaugeController")
            .def(boost::python::init<const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional< const bool, const bool> >())
            .def("getImgStateDefinition",        &velaInvertedMagnetronGaugeController::getImgStateDefinition                 )
            .def("getILockStatesDefinition",        &velaInvertedMagnetronGaugeController::getILockStatesDefinition                 )
            .def("get_CA_PEND_IO_TIMEOUT",          &velaInvertedMagnetronGaugeController::get_CA_PEND_IO_TIMEOUT                   )
            .def("set_CA_PEND_IO_TIMEOUT",          &velaInvertedMagnetronGaugeController::set_CA_PEND_IO_TIMEOUT                   )
            .def("getImgState",                  &velaInvertedMagnetronGaugeController::getImgState_Py                        )
            .def("getILockStates",                  &velaInvertedMagnetronGaugeController::getILockStates_Py                        )
            .def("getImgP",                      &velaInvertedMagnetronGaugeController::getImgP_Py                            )
            .def("monitorForNCounts",               &velaInvertedMagnetronGaugeController::monitorForNCounts_Py                     )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &velaInvertedMagnetronGaugeController::debugMessagesOff                         )
            .def("debugMessagesOn",                 &velaInvertedMagnetronGaugeController::debugMessagesOn                          )
            .def("messagesOff",                     &velaInvertedMagnetronGaugeController::messagesOff                              )
            .def("messagesOn",                      &velaInvertedMagnetronGaugeController::messagesOn                               )
            .def("silence",                         &velaInvertedMagnetronGaugeController::silence                                  )
            .def("verbose",                         &velaInvertedMagnetronGaugeController::verbose                                  )
		;
};

#endif // BUILD_DLL

#endif // velaInvertedMagnetronGaugeController_H

