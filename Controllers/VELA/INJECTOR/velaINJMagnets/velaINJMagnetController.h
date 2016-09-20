#ifndef VELA_ING_MAG_CONTROLLER_H_
#define VELA_INJ_MAG_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "magnetController.h"


#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
//#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
//#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class velaINJMagnetController : public magnetController
{
    public:
        /// we have overloaded constructors to specify config-file location
        velaINJMagnetController(const bool RealMachine = false, const bool show_messages = true , const bool show_debug_messages = true);
        velaINJMagnetController( const std::string configFileLocation1,  const std::string configFileLocation2, const std::string configFileLocation3,
                                 const  bool show_messages = true, const bool show_debug_messages = true );
        ~velaINJMagnetController( );

#ifdef BUILD_DLL
        /// Include a function to export enum definitions to a python dictionary
        boost::python::dict getMagPSUStateDefinition();



        magnetStructs::magnetStateStruct getDBURT_Py( const std::string fileName );
        magnetStructs::magnetStateStruct getDBURTCorOnly_Py( const std::string fileName );
        magnetStructs::magnetStateStruct getDBURTQuadOnly_Py( const std::string fileName );

        void applyDBURT_Py( const std::string fileName );
        void applyDBURTCorOnly_Py( const std::string fileName );
        void applyDBURTQuadOnly_Py( const std::string fileName );

        bool writeDBURT_Py( magnetStructs::magnetStateStruct ms, const std::string fileName = "", const std::string comments = "" );
        bool writeDBURT_Py( const std::string fileName = "", const std::string comments = "" );


        /// python does not do pass-by-reference
        /// so we create some thin wrappers to those functions that boost.python can use
        /// (retain pass by reference versions for c++ applications)

        bool isAQuad_Py( const std::string magName );
        bool isABSol_Py( const std::string magName );
        bool isAHCor_Py( const std::string magName );
        bool isAVCor_Py( const std::string magName );
        bool isADip_Py ( const std::string magName );
        bool isASol_Py ( const std::string magName );
        bool isACor_Py ( const std::string magName );
        bool isNR_Py( const std::string  magName );
        bool isBipolar_Py( const std::string  magName );
        bool isNRGanged_Py( const std::string  magName );
        bool isNRorNRGanged_Py( const std::string  magName );
        bool isON_psuN_Py( const std::string  magName );
        bool isON_psuR_Py( const std::string  magName );
        bool isON_Py ( const std::string  magName );
        bool isOFF_psuN_Py( const std::string  magName );
        bool isOFF_psuR_Py( const std::string  magName );
        bool isOFF_Py ( const std::string  magName );

        bool isDegaussing_Py( const std::string magName );
        bool isNotDegaussing_Py( const std::string magName );

        void setRITolerance_Py( const std::string magName, const double val);

        double getSI_Py1( const std::string magName );
        double getRI_Py1( const std::string magName );

        std::vector< double > getSI_Py2( const std::vector< std::string > magNames );
        std::vector< double > getRI_Py2( const std::vector< std::string > magNames );

        bool setSI_Py1( const std::string magName, const double value);
        bool setSI_Py2( const  std::vector< std::string > magNames, const std::vector< double > values);
        bool setSI_Py4( const std::string magNames, const double values, const double tolerances, const size_t timeOUT );
        std::vector< std::string >  setSI_Py3( const std::vector< std::string > magNames, const std::vector< double > values, const std::vector< double > tolerances, const size_t timeOUT );

        /// These functions return wether the commands were sent to EPICS correctly, not if the oiperation was succesful

        bool switchONpsu_Py1 ( const std::string magName  );
        bool switchOFFpsu_Py1( const std::string magName  );
        bool switchONpsu_Py2 ( const std::vector< std::string > magNames );
        bool switchOFFpsu_Py2( const std::vector< std::string > magNames );

        size_t degauss_Py1( const std::string mag );
        size_t degauss_Py2( const std::vector< std::string > mag );

        /// any functions that return a map need a wrapper to convert to a python dictionary
        /// (we need the functions that return std::map types when building c++ applications)
        /// NB getILockStates exists in the base class

        boost::python::dict getILockStates_Py( std::string magName );
        boost::python::dict getILockStatesStr_Py( std::string magName );
#endif // BUILD_DLL

    protected:
    private:
};
#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!


using namespace boost::python;

/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// For examples See magnets...
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( vpsc_overloads1, getSI_Py1 , 1, 1 );minimum of 1 and a maximum of 3 arguments
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads1,  getSI_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads2,  getRI_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads3,  getSI_Py2, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads4,  getRI_Py2, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads5,  setSI_Py1, 2, 2 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads6,  setSI_Py1, 2, 2 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads7,  setSI_Py3, 4, 4 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads8,  switchONpsu_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads9,  switchOFFpsu_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads10, switchONpsu_Py2, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads11, switchOFFpsu_Py2, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads12, degauss_Py1, 1, 1 );
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( mag_overloads13, degauss_Py2, 1, 1 );


BOOST_PYTHON_MODULE( velaINJMagnetControl )
{
    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>() )
        ;

    class_<std::vector< double> >("std_vector_double")
        .def( vector_indexing_suite< std::vector< double>>() )
        ;

    boost::python::enum_<VELA_ENUM::MAG_PSU_STATE>("MAG_PSU_STATE")
            .value("MAG_PSU_OFF",   VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF   )
            .value("MAG_PSU_ON",    VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON    )
            .value("MAG_PSU_TIMING",VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING)
            .value("MAG_PSU_ERROR", VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR )
            .value("MAG_PSU_NONE",  VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE  )
            ;

    class_<magnetStructs::magnetStateStruct>("magnetStateStruct")
        .def_readwrite("numMags", &magnetStructs::magnetStateStruct::numMags)
        .def_readwrite("magNames", &magnetStructs::magnetStateStruct::magNames)
        .def_readwrite("psuStates", &magnetStructs::magnetStateStruct::psuStates)
        .def_readwrite("siValues", &magnetStructs::magnetStateStruct::siValues)
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

//  boost::python::class_<controller,       boost::python::bases<baseObject    >, boost::noncopyable> ("controller", boost::python::no_init)
	boost::python::class_<velaINJMagnetController, boost::python::bases<controller>, boost::noncopyable> ("velaINJMagnetController")
	        .def(boost::python::init<const std::string, const std::string, const std::string, optional<  const bool, const bool > >())
            .def(boost::python::init< optional<const bool, const bool, const bool>>())
            .def("getILockStates", &velaINJMagnetController::getILockStates_Py )
            .def("getMagPSUStateDefinition", &velaINJMagnetController::getMagPSUStateDefinition )
            .def("getILockStatesDefinition", &velaINJMagnetController::getILockStatesDefinition  )
            .def("get_CA_PEND_IO_TIMEOUT",   &velaINJMagnetController::get_CA_PEND_IO_TIMEOUT    )
            .def("set_CA_PEND_IO_TIMEOUT",   &velaINJMagnetController::set_CA_PEND_IO_TIMEOUT    )
            .def("isAQuad",                  &velaINJMagnetController::isAQuad_Py    )
            .def("isABSol",                  &velaINJMagnetController::isABSol_Py    )
            .def("isAHCor",                  &velaINJMagnetController::isAHCor_Py    )
            .def("isAVCor",                  &velaINJMagnetController::isAVCor_Py    )
            .def("isADip",                   &velaINJMagnetController::isADip_Py    )
            .def("isASol",                   &velaINJMagnetController::isASol_Py    )
            .def("isACor",                   &velaINJMagnetController::isACor_Py    )
            .def("isNR",                     &velaINJMagnetController::isNR_Py         )
            .def("isBipolar",                &velaINJMagnetController::isBipolar_Py     )
            .def("isNRGanged",               &velaINJMagnetController::isNRGanged_Py    )
            .def("isNRorNRGanged",           &velaINJMagnetController::isNRorNRGanged_Py)
            .def("isON_psuN",                &velaINJMagnetController::isON_psuN_Py     )
            .def("isON_psuR",                &velaINJMagnetController::isON_psuR_Py     )
            .def("isON",                     &velaINJMagnetController::isON_Py          )
            .def("isOFF_psuN",               &velaINJMagnetController::isOFF_psuN_Py    )
            .def("isOFF_psuR",               &velaINJMagnetController::isOFF_psuR_Py    )
            .def("isOFF",                    &velaINJMagnetController::isOFF_Py         )
            .def("isDegaussing",             &velaINJMagnetController::isDegaussing_Py    )
            .def("isNotDegaussing",          &velaINJMagnetController::isNotDegaussing_Py )
            .def("getSI",                    &velaINJMagnetController::getSI_Py1          )
            .def("getRI",                    &velaINJMagnetController::getRI_Py1          )
            .def("getSI",                    &velaINJMagnetController::getSI_Py2          )
            .def("getRI",                    &velaINJMagnetController::getRI_Py2          )
            .def("setSI",                    &velaINJMagnetController::setSI_Py1          )
            .def("setSI",                    &velaINJMagnetController::setSI_Py2          )
            .def("setSI",                    &velaINJMagnetController::setSI_Py3          )
            .def("setSI",                    &velaINJMagnetController::setSI_Py4          )
            .def("switchONpsu",              &velaINJMagnetController::switchONpsu_Py1    )
            .def("switchOFFpsu",             &velaINJMagnetController::switchOFFpsu_Py1   )
            .def("switchONpsu",              &velaINJMagnetController::switchONpsu_Py2    )
            .def("switchOFFpsu",             &velaINJMagnetController::switchOFFpsu_Py2   )
            .def("degauss",                  &velaINJMagnetController::degauss_Py1        )
            .def("degauss",                  &velaINJMagnetController::degauss_Py2        )
            .def("getMagnetNames",           &velaINJMagnetController::getMagnetNames     )
            .def("getQuadNames",             &velaINJMagnetController::getQuadNames       )
            .def("getHCorNames",             &velaINJMagnetController::getHCorNames       )
            .def("getVCorNames",             &velaINJMagnetController::getVCorNames       )
            .def("getDipNames",              &velaINJMagnetController::getDipNames        )
            .def("getSolNames",              &velaINJMagnetController::getSolNames        )
            .def("applyDBURT",               &velaINJMagnetController::applyDBURT         )
            .def("applyDBURTCorOnly",        &velaINJMagnetController::applyDBURTCorOnly  )
            .def("applyDBURTQuadOnly",       &velaINJMagnetController::applyDBURTQuadOnly )
            .def("getDBURT",                 &velaINJMagnetController::getDBURT_Py        )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",         &velaINJMagnetController::debugMessagesOff )
            .def("debugMessagesOn",          &velaINJMagnetController::debugMessagesOn )
            .def("messagesOff",              &velaINJMagnetController::messagesOff )
            .def("messagesOn",               &velaINJMagnetController::messagesOn )
            .def("silence",                  &velaINJMagnetController::silence )
            .def("verbose",                  &velaINJMagnetController::verbose )
            .def("isSilent",                 &velaINJMagnetController::isSilent )
            .def("isVerbose",                &velaINJMagnetController::isVerbose )
            .def("isMessageOn",              &velaINJMagnetController::isMessageOn )
            .def("isDebugMessageOn",         &velaINJMagnetController::isDebugMessageOn )
            ;
}
#endif // BUILD_DLL


#endif // VELA_INJ_MAG_CONTROLLER_H_


