#ifndef VCMAGNETS_H
#define VCMAGNETS_H
//stl
#include <string>
//
#include "magnetController.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


/// we just need to decide good names for these things...
class VCmagnets
{
    public:
        VCmagnets();
        ~VCmagnets();

        magnetController& virtual_VELA_INJ_Magnet_Controller();
        magnetController& offline_VELA_INJ_Magnet_Controller();
        magnetController& physical_VELA_INJ_Magnet_Controller();

        magnetController& virtual_VELA_BA1_Magnet_Controller();
        magnetController& offline_VELA_BA1_Magnet_Controller();
        magnetController& physical_VELA_BA1_Magnet_Controller();

        magnetController& virtual_VELA_BA2_Magnet_Controller();
        magnetController& offline_VELA_BA2_Magnet_Controller();
        magnetController& physical_VELA_BA2_Magnet_Controller();

        magnetController& virtual_CLARA_INJ_Magnet_Controller();
        magnetController& offline_CLARA_INJ_Magnet_Controller();
        magnetController& physical_CLARA_INJ_Magnet_Controller();

    protected:

    private:

     /// we just need to decide good names for these things...
        magnetController * virtual_VELA_INJ_Magnet_Controller_Obj;
        magnetController * offline_VELA_INJ_Magnet_Controller_Obj;
        magnetController * physical_VELA_INJ_Magnet_Controller_Obj;

        magnetController * virtual_VELA_BA1_Magnet_Controller_Obj;
        magnetController * offline_VELA_BA1_Magnet_Controller_Obj;
        magnetController * physical_VELA_BA1_Magnet_Controller_Obj;

        magnetController * virtual_VELA_BA2_Magnet_Controller_Obj;
        magnetController * offline_VELA_BA2_Magnet_Controller_Obj;
        magnetController * physical_VELA_BA2_Magnet_Controller_Obj;

        magnetController * virtual_CLARA_INJ_Magnet_Controller_Obj;
        magnetController * offline_CLARA_INJ_Magnet_Controller_Obj;
        magnetController * physical_CLARA_INJ_Magnet_Controller_Obj;

//        bool virtual_VELA_INJ_Magnet_Controller_Obj_Exists;
//        bool offline_VELA_INJ_Magnet_Controller_Obj_Exists;
//        bool physical_VELA_INJ_Magnet_Controller_Obj_Exists;
//
//        bool virtual_VELA_BA1_Magnet_Controller_Obj_Exists;
//        bool offline_VELA_BA1_Magnet_Controller_Obj_Exists;
//        bool physical_VELA_BA1_Magnet_Controller_Obj_Exists;
//
//        bool virtual_VELA_BA2_Magnet_Controller_Obj_Exists;
//        bool offline_VELA_BA2_Magnet_Controller_Obj_Exists;
//        bool physical_VELA_BA2_Magnet_Controller_Obj_Exists;
//
//        bool virtual_CLARA_INJ_Magnet_Controller_Obj_Exists;
//        bool offline_CLARA_INJ_Magnet_Controller_Obj_Exists;
//        bool physical_CLARA_INJ_Magnet_Controller_Obj_Exists;

};

/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
/// I'm going to do it the function pointer way here...
/// The other alternative is to create lots of different function names in the c++ class
/// just to make the function pointer overloading neater, lets make some typedefs, generally i don't like doing this
typedef double doub;
typedef const double cdou;
typedef const size_t csiz;
typedef const std::string cstr ;
typedef std::vector<double> vecd;
typedef const std::vector<double> cved;
typedef std::vector<std::string> vecs;
typedef const std::vector<std::string> cves;
typedef magnetStructs::magnetStateStruct msst;

doub(magnetController::*getRI_1)(cstr&) = &magnetController::getRI;
vecd(magnetController::*getRI_2)(cves&) = &magnetController::getRI;

vecd(magnetController::*getSI_2)(cves&) = &magnetController::getSI;
doub(magnetController::*getSI_1)(cstr&) = &magnetController::getSI;

bool(magnetController::*setSI_1)(cstr&, cdou ) = &magnetController::setSI;
bool(magnetController::*setSI_2)(cves&, cved&) = &magnetController::setSI;
bool(magnetController::*setSI_3)(cstr&, cdou , cdou , csiz) = &magnetController::setSI;
vecs(magnetController::*setSI_4)(cves&, cved&, cved&, csiz) = &magnetController::setSI;

bool(magnetController::*switchONpsu_1 )(cstr&) = &magnetController::switchONpsu;
bool(magnetController::*switchONpsu_2 )(cves&) = &magnetController::switchONpsu;
bool(magnetController::*switchOFFpsu_1)(cstr&) = &magnetController::switchOFFpsu;
bool(magnetController::*switchOFFpsu_2)(cves&) = &magnetController::switchOFFpsu;

size_t(magnetController::*degauss_1)(cstr&, bool ) = &magnetController::degauss;
size_t(magnetController::*degauss_2)(cves&, bool ) = &magnetController::degauss;

bool(magnetController::*writeDBURT_1)( const msst&, cstr&, cstr&) = &magnetController::writeDBURT;
bool(magnetController::*writeDBURT_2)(              cstr&, cstr&) = &magnetController::writeDBURT;

msst(magnetController::*getCurrentMagnetState_1)( cves&) = &magnetController::getCurrentMagnetState;
msst(magnetController::*getCurrentMagnetState_2)(      ) = &magnetController::getCurrentMagnetState;

using namespace boost::python;

BOOST_PYTHON_MODULE( VELA_CLARA_MagnetControl )
{
        /// Things that you want to use in python muct be exposed:
        /// containers
        class_<std::vector< std::string > >("std_vector_string")
                .def( vector_indexing_suite< std::vector< std::string >>() )
                ;
        class_<std::vector< double> >("std_vector_double")
        .def( vector_indexing_suite< std::vector< double>>() )
        ;
        /// and enums, remember we have a enum to string python dictionary macro too!
        enum_<VELA_ENUM::MAG_PSU_STATE>("MAG_PSU_STATE")
                .value("MAG_PSU_OFF",   VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF   )
                .value("MAG_PSU_ON",    VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON    )
                .value("MAG_PSU_TIMING",VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING)
                .value("MAG_PSU_ERROR", VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR )
                .value("MAG_PSU_NONE",  VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE  )
                ;
        boost::python::enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
                .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
                .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
                .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
                ;
        /// and enums, remember we have a enum to string python dictionary macro too!
        enum_<magnetStructs::MAG_TYPE>("MAG_TYPE")
                .value("QUAD", magnetStructs::MAG_TYPE::QUAD )
                .value("DIP" , magnetStructs::MAG_TYPE::DIP  )
                .value("HCOR", magnetStructs::MAG_TYPE::HCOR )
                .value("VCOR", magnetStructs::MAG_TYPE::VCOR )
                .value("BSOL", magnetStructs::MAG_TYPE::BSOL )
                .value("SOL" , magnetStructs::MAG_TYPE::SOL  )
                .value("SEXT", magnetStructs::MAG_TYPE::SEXT )
                .value("UNKNOWN_MAGNET_TYPE",  magnetStructs::MAG_TYPE::UNKNOWN_MAGNET_TYPE  )
                ;
        /// and enums, remember we have a enum to string python dictionary macro too!
        enum_<magnetStructs::MAG_REV_TYPE>("MAG_REV_TYPE")
                .value("NR"       , magnetStructs::MAG_REV_TYPE::NR )
                .value("BIPOLAR"  , magnetStructs::MAG_REV_TYPE::BIPOLAR  )
                .value("NR_GANGED", magnetStructs::MAG_REV_TYPE::NR_GANGED )/// hopefully never again....
                .value("POS"      , magnetStructs::MAG_REV_TYPE::POS )
                .value("UNKNOWN_MAG_REV_TYPE", magnetStructs::MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE )
                ;


    /// structs (this one is a one-stop shop for comomn parameters)
    boost::python::class_<magnetStructs::magnetStateStruct>
        ("magnetStateStruct")
        .add_property("numMags",   &magnetStructs::magnetStateStruct::numMags)
        .add_property("magNames",  &magnetStructs::magnetStateStruct::magNames)
        .add_property("psuStates", &magnetStructs::magnetStateStruct::psuStates)
        .add_property("siValues",  &magnetStructs::magnetStateStruct::siValues)
        .add_property("riValues",  &magnetStructs::magnetStateStruct::riValues)
        ;

    /// Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    /// we have to tell boost.python about pure virtual methods in abstract base classes
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;

    boost::python::class_<magnetStructs::nrPSUObject,boost::noncopyable>
        ("nrPSUObject", boost::python::no_init)
        .def_readonly("psuState", &magnetStructs::nrPSUObject::psuState)
        ;

//    struct  nrPSUObject
//    {   // proviude a default constructor
//        nrPSUObject() : isGanged( false ), parentMagnet( "UNKNOWN" ), pvRoot( "UNKNOWN"),
//                        psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),
//                        numIlocks( 0 ) {} // proviude a default constructor
//        std::string  parentMagnet, pvRoot;
//        bool isGanged;//, canFlip;/// canflip? probably refactor as function...
//        VELA_ENUM::MAG_PSU_STATE psuState;
//        size_t numIlocks;
//        std::vector< std::string > gangMembers;
//        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
//    #ifndef __CINT__
//        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
//        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
//        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
//    #endif
//    };

    boost::python::class_<magnetStructs::magnetObject,boost::noncopyable>
        ("magnetObject", boost::python::no_init)
        .def_readonly("magType",    &magnetStructs::magnetObject::magType)
        .def_readonly("psuState",   &magnetStructs::magnetObject::psuState)
        .def_readonly("magRevType", &magnetStructs::magnetObject::magRevType)
        .def_readonly("siWithPol",  &magnetStructs::magnetObject::siWithPol)
        .def_readonly("riWithPol",  &magnetStructs::magnetObject::riWithPol)
        .def_readonly("name",       &magnetStructs::magnetObject::name)
        .def_readonly("nPSU",       &magnetStructs::magnetObject::nPSU)
        .def_readonly("rPSU",       &magnetStructs::magnetObject::rPSU)
        ;

//     struct  magnetObject
//    {   // proviude a default constructor
//        magnetObject() : magType (MAG_TYPE::UNKNOWN_MAGNET_TYPE), isGanged( false ), name("UNKNOWN"),pvRoot( "UNKNOWN"),
//                psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),canNRFlip( false ),samePSURoot( false ),
//                magRevType( MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE ),
//                si(-999.999), ri(-999.999), siWithPol(-999.999), riWithPol(-999.999), riTolerance(-999.999),
//                /// err... , an atomic_bool for isDegaussing( false ) does not work ... http://stackoverflow.com/questions/15750917/initializing-stdatomic-bool
//                /// ... which is probably evil && dangerous
//                numIlocks( 0 ),
//                //added deguassing initialisers here
//                numDegaussSteps(0), maxWaitTime(0), numDegaussElements(0) {} // proviude a default constructor
//        MAG_TYPE magType;           /// dipole, quad etc.
//        MAG_REV_TYPE  magRevType;   /// reverse type, NR, bipolar etc.
//        VELA_ENUM::MAG_PSU_STATE psuState;
//        size_t numIlocks;
//        nrPSUObject nPSU, rPSU;
//        std::string  name, pvRoot, psuRoot;
//        bool isGanged, canNRFlip, samePSURoot;
//        std::vector< std::string > gangMembers;
//        double si, ri, siWithPol, riWithPol, riTolerance;
//
//        //DEGUASSING: added here by Tim Price
//
//        size_t numDegaussSteps, maxWaitTime, numDegaussElements;
//        std::vector< double > degValues;
//        double  degTolerance;
//
////        std::atomic< bool > isDegaussing;/// NO thread safe copy constructor malarkey...  http://stackoverflow.com/questions/29332897/error-c2280-attempting-to-reference-a-deleted-function-atomicint
//        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
//    #ifndef __CINT__
//        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
//        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
//        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
//    #endif
//    };

    /// and the class member functions to expose to python,
    /// remmeber to include enum definitions as boost::python::dict <int, string>
    /// as well as boost::python::dict <int, int>
    boost::python::class_<magnetController, boost::python::bases<controller>, boost::noncopyable>
        ("magnetController","magnetController Doc String",boost::python::no_init)
        .def("getILockStates",           &magnetController::getILockStates        )
        .def("getMagPSUStateDefinition", &magnetController::getMagPSUStateDefinition )
        .def("getILockStatesDefinition", &magnetController::getILockStatesDefinition )
        .def("get_CA_PEND_IO_TIMEOUT",   &magnetController::get_CA_PEND_IO_TIMEOUT   )
        .def("set_CA_PEND_IO_TIMEOUT",   &magnetController::set_CA_PEND_IO_TIMEOUT   )
        .def("getSI", getSI_1)
        .def("getSI", getSI_2)
        .def("getSI", getRI_2)
        .def("getRI", getRI_1)
        .def("setSI", setSI_1)
        .def("setSI", setSI_2)
        .def("setSI", setSI_3)
        .def("setSI",  setSI_4)
        .def("switchONpsu", switchONpsu_1)
        .def("switchONpsu", switchONpsu_2)
        .def("switchOFFpsu",switchOFFpsu_1)
        .def("switchOFFpsu",switchOFFpsu_2)
        .def("degauss",   degauss_1)
        .def("degauss",   degauss_2)
        .def("writeDBURT",     writeDBURT_1)
        .def("writeDBURT",    writeDBURT_2)
        .def("getCurrentMagnetState",    getCurrentMagnetState_1)
        .def("getCurrentMagnetState",    getCurrentMagnetState_2)
        .def("getMagObjConstRef",        &magnetController::getMagObjConstRef,return_value_policy<reference_existing_object>())
        .def("isAQuad",                  &magnetController::isAQuad    )
        .def("isABSol",                  &magnetController::isABSol    )
        .def("isAHCor",                  &magnetController::isAHCor    )
        .def("isAVCor",                  &magnetController::isAVCor    )
        .def("isADip",                   &magnetController::isADip    )
        .def("isASol",                   &magnetController::isASol    )
        .def("isACor",                   &magnetController::isACor    )
        .def("isNR",                     &magnetController::isNR         )
        .def("isBipolar",                &magnetController::isBipolar     )
        .def("isNRGanged",               &magnetController::isNRGanged    )
        .def("isNRorNRGanged",           &magnetController::isNRorNRGanged)
        .def("isON_psuN",                &magnetController::isON_psuN     )
        .def("isON_psuR",                &magnetController::isON_psuR     )
        .def("isON",                     &magnetController::isON          )
        .def("isOFF_psuN",               &magnetController::isOFF_psuN    )
        .def("isOFF_psuR",               &magnetController::isOFF_psuR    )
        .def("isOFF",                    &magnetController::isOFF         )
        .def("isDegaussing",             &magnetController::isDegaussing    )
        .def("isNotDegaussing",          &magnetController::isNotDegaussing )
        .def("getMagnetNames",           &magnetController::getMagnetNames     )
        .def("getQuadNames",             &magnetController::getQuadNames       )
        .def("getHCorNames",             &magnetController::getHCorNames       )
        .def("getVCorNames",             &magnetController::getVCorNames       )
        .def("getDipNames",              &magnetController::getDipNames        )
        .def("getSolNames",              &magnetController::getSolNames        )
        .def("applyDBURT",               &magnetController::applyDBURT         )
        .def("applyDBURTCorOnly",        &magnetController::applyDBURTCorOnly  )
        .def("applyDBURTQuadOnly",       &magnetController::applyDBURTQuadOnly )
        .def("getDBURT",                 &magnetController::getDBURT        )
        /// Don't forget functions in the base class we want to expose....
        .def("debugMessagesOff",         &magnetController::debugMessagesOff )
        .def("debugMessagesOn",          &magnetController::debugMessagesOn )
        .def("messagesOff",              &magnetController::messagesOff )
        .def("messagesOn",               &magnetController::messagesOn )
        .def("silence",                  &magnetController::silence )
        .def("verbose",                  &magnetController::verbose )
        .def("isSilent",                 &magnetController::isSilent )
        .def("isVerbose",                &magnetController::isVerbose )
        .def("isMessageOn",              &magnetController::isMessageOn )
        .def("isDebugMessageOn",         &magnetController::isDebugMessageOn )
        ;

    /// Finally the main class that creates all the obejcts
    boost::python::class_<VCmagnets,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Magnet_Controller",  &VCmagnets::virtual_VELA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Magnet_Controller",  &VCmagnets::offline_VELA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Magnet_Controller",  &VCmagnets::physical_VELA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Magnet_Controller",  &VCmagnets::virtual_VELA_BA1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Magnet_Controller",  &VCmagnets::offline_VELA_BA1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Magnet_Controller",  &VCmagnets::physical_VELA_BA1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Magnet_Controller",  &VCmagnets::virtual_VELA_BA2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Magnet_Controller",  &VCmagnets::offline_VELA_BA2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Magnet_Controller",  &VCmagnets::physical_VELA_BA2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_INJ_Magnet_Controller",  &VCmagnets::virtual_CLARA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_Magnet_Controller",  &VCmagnets::offline_CLARA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_Magnet_Controller",  &VCmagnets::physical_CLARA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
                ;




}

#endif // VCMAGNETS_H
