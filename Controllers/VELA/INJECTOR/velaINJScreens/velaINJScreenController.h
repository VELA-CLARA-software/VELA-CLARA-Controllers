
#ifndef velaINJScreen_CONTROLLER_H
#define velaINJScreen_CONTROLLER_H
//project

#include "screenController.h"

//stl
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



class velaINJScreenController : public screenController
{
    public:
    ///we have overloaded constructors to specify config-file location
        velaINJScreenController(const bool RealMachine = false, const bool show_messages = true , const bool show_debug_messages = true );
        velaINJScreenController( const std::string configFileLocation1,
                          const std::string configFileLocation2, const bool show_messages = true, const bool show_debug_messages = true );
        ~velaINJScreenController();

#ifdef BUILD_DLL
    ///Include a function to export enum definitions to a python dictionary
    boost::python::dict getScreenStateDefinition();

    /// python does not do pass-by-reference
    /// so we create some thin wrappers to those functions that boost.python can use
    /// (retain pass by reference versions for c++ applications)


        void Screen_Out_Py( const std::string name );
        void Screen_In_Py( const std::string name );
        void Screen_Move_Py( const std::string name, const std::string position );
        void Screen_Stop_Py( const std::string name );
        void All_Out_Py();

        void controller_move_to_position_Py( const std::string name, const std::string position );
        void controller_move_to_Py( const std::string name, const std::string V_H, const double position );

        bool IsOut_Py( const std::string name );
        bool IsIn_Py( const std::string name );
        bool horizontal_disabled_check_Py( const std::string name );
        bool vertical_disabled_check_Py( const std::string name );

        screenStructs::SCREEN_STATE getComplexHorizontalScreenState_Py( const std::string name );
        screenStructs::SCREEN_STATE getComplexVerticalScreenState_Py( const std::string name );
        screenStructs::SCREEN_STATE getSimpleScreenState_Py( const std::string name );
        screenStructs::SCREEN_STATE getScreenState_Py( const std::string name );
        screenStructs::SCREEN_STATE getScreenState_Py( const std::string name, const std::string V_H );

        double getComplexScreenHorizontalPosition_Py( const std::string name );
        double getComplexScreenVerticalPosition_Py( const std::string name );
        double getScreenPosition_Py( const std::string name, const std::string V_H );

        void get_info_Py( const std::string name );
        void get_config_values_Py( const std::string name );



        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string screenName );///This may be wrong, should be & name?
        boost::python::dict getILockStatesStr_Py( std::string screenName );

#endif //BUILD_DLL

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

//bool (screenController::*getScreenState_1)( const std::string ) = &screenController::getScreenState_Py;
//bool (screenController::*getScreenState_2)( const std::string, const std::string ) = &screenController::getScreenState_Py;
//
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(screen_overloads, getScreenState_Py,1,2);

/// As another example we have overloaded constructors to enable / disable messages
/// and / or debug messages when instantiating. See here
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/exposing.html
/// and beware of: http://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor

BOOST_PYTHON_MODULE( velaINJScreenControl )
{
 /// //http://www.cs.fsu.edu/~lacher/boost_1_32_0/libs/python/doc/tutorial/doc/html/python/exposing.html

/// expose any containers used in your class, e.g.
///  vector_indexing_suite seems to work, i haven't had much success with map_indexing_suite, in any case we convert maps to  boost::python::dict
///	class_< std::vector< double > >("std_vector_double")
///		.def(vector_indexing_suite< std::vector< double > >() )
///	;
///	class_<std::vector< std::string > >("std_vector_string")
///		.def(vector_indexing_suite< std::vector< std::string  > >() )
///	;


    /// Include ALL the enums you want to expose to Python
    boost::python::enum_<screenStructs::SCREEN_STATE>("SCREEN_STATE")
            .value("SCREEN_IN", screenStructs::SCREEN_STATE::SCREEN_IN )
            .value("SCREEN_OUT",   screenStructs::SCREEN_STATE::SCREEN_OUT   )
            .value("SCREEN_ERROR",  screenStructs::SCREEN_STATE::SCREEN_ERROR  )
            .value("SCREEN_UNKNOWN",  screenStructs::SCREEN_STATE::SCREEN_UNKNOWN  )
            .value("SCREEN_H_MIRROR",  screenStructs::SCREEN_STATE::SCREEN_H_MIRROR  )
            .value("SCREEN_H_50U_SLIT",  screenStructs::SCREEN_STATE::SCREEN_H_50U_SLIT  )
            .value("SCREEN_H_25U_SLIT",  screenStructs::SCREEN_STATE::SCREEN_H_25U_SLIT  )
            .value("SCREEN_H_63MM_HOLE",  screenStructs::SCREEN_STATE::SCREEN_H_63MM_HOLE  )
            .value("SCREEN_H_10MM_HOLE",  screenStructs::SCREEN_STATE::SCREEN_H_10MM_HOLE  )
            .value("SCREEN_H_YAG",  screenStructs::SCREEN_STATE::SCREEN_H_YAG  )
            .value("SCREEN_V_YAG",  screenStructs::SCREEN_STATE::SCREEN_V_YAG  )
            .value("SCREEN_V_SLIT",  screenStructs::SCREEN_STATE::SCREEN_V_SLIT  )
            .value("SCREEN_MOVING",  screenStructs::SCREEN_STATE::SCREEN_MOVING )
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


    boost::python::class_<velaINJScreenController, boost::python::bases<controller>>("velaINJScreenController")
                .def(boost::python::init<const std::string, const std::string, optional< const bool, const bool > >())
                .def(boost::python::init< optional<const bool, const bool, const bool> >())
                .def("getScreenStateDefinition",            &velaINJScreenController::getScreenStateDefinition )
                .def("detILockStatesDefinition",            &velaINJScreenController::getILockStatesDefinition )
                .def("get_CA_PEND_IO_TIMEOUT",              &velaINJScreenController::get_CA_PEND_IO_TIMEOUT    )
                .def("set_CA_PEND_IO_TIMEOUT",              &velaINJScreenController::set_CA_PEND_IO_TIMEOUT    )
                .def("getComplexHorizontalScreenState",     &velaINJScreenController::getComplexHorizontalScreenState_Py)
                .def("getComplexVerticalScreenState",       &velaINJScreenController::getComplexVerticalScreenState_Py)
                .def("getSimpleScreenState",                &velaINJScreenController::getSimpleScreenState_Py)
//                .def("getScreenState",                      &velaINJScreenController::getScreenState, getScreenState_1)
//                .def("getScreenState",                      &velaINJScreenController::getScreenState, getScreenState_2)
                //.def("getScreenState",                      &velaINJScreenController::getScreenState_Py, screen_overloads() )
                .def("getComplexScreenHorizontalPosition",  &velaINJScreenController::getComplexScreenHorizontalPosition_Py)
                .def("getComplexScreenVerticalPosition",    &velaINJScreenController::getComplexScreenVerticalPosition_Py)
                .def("getScreenPosition",                   &velaINJScreenController::getScreenPosition_Py)
                .def("get_info",                            &velaINJScreenController::get_info_Py)
                .def("get_config_values",                   &velaINJScreenController::get_config_values_Py)
                .def("getILockStates",                      &velaINJScreenController::getILockStates_Py)
                .def("Screen_Out",                          &velaINJScreenController::Screen_Out_Py )
                .def("Screen_In",                           &velaINJScreenController::Screen_In_Py )
                .def("Screen_Move",                         &velaINJScreenController::Screen_Move_Py )
                .def("Screen_Stop",                         &velaINJScreenController::Screen_Stop_Py )
                .def("controller_move_to_position",         &velaINJScreenController::controller_move_to_position_Py )
                .def("controller_move_to",                  &velaINJScreenController::controller_move_to_Py )
                .def("IsIn",                                &velaINJScreenController::IsIn_Py, boost::python::args("name") )
                .def("IsOut",                               &velaINJScreenController::IsOut_Py, boost::python::args("name") )///Should this be the same as IsIn?
                .def("horizontal_disabled_check",           &velaINJScreenController::horizontal_disabled_check_Py )
                .def("vertical_disabled_check",             &velaINJScreenController::vertical_disabled_check_Py )
                .def("debugMessagesOff",                    &velaINJScreenController::debugMessagesOff )
                .def("debugMessagesOn",                     &velaINJScreenController::debugMessagesOn )
                .def("messagesOff",                         &velaINJScreenController::messagesOff )
                .def("messagesOn",                          &velaINJScreenController::messagesOn )
                .def("silence",                             &velaINJScreenController::silence )
                .def("verbose",                             &velaINJScreenController::verbose )
           ;
};//BOOST_PYTHON_MODULE
#endif //BUILD_DLL

#endif //velaINJScreen_CONTROLLER_H




