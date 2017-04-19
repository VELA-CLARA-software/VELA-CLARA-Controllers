#ifndef VCrfprot_H_
#define VCrfprot_H_
// STL
#include <string>
// HWC
#include "gunProtController.h"
#include "rfProtStructs.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


/// we just need to decide good names for these things...
class VCrfprot
{
    public:
        VCrfprot();
        ~VCrfprot();

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

    // The gun is one type of RF Modulator, we're also going to have TDC, linac, etc. ones, all callable from here
        gunProtController& virtual_Gun_Protection_Controller();
        gunProtController& physical_Gun_Protection_Controller();
        gunProtController& offline_Gun_Protection_Controller();

    protected:

    private:

     /// we just need to decide good names for these things...
        gunProtController * virtual_Gun_Protection_Controller_Obj ;
        gunProtController * physical_Gun_Protection_Controller_Obj;
        gunProtController * offline_Gun_Protection_Controller_Obj ;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string allGunProtsConf;


};

using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_CLARA_RF_Protection_Control )
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();

    /// Things that you want to use in python muct be exposed:
    /// containers
    class_<std::vector< std::string > >("std_vector_string")
            .def(vector_indexing_suite< std::vector< std::string >>() )
            ;
    class_<std::vector< double> >("std_vector_double")
            .def(vector_indexing_suite< std::vector< double>>() )
            ;
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
        .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
        .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
        .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
        ;
    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE","MACHINE_MODE: a named integer giving the")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE  )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL  )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL )
        ;

    // Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    // we have to tell boost.python about pure virtual methods in abstract base classes
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller", boost::python::no_init) /// force Python to not construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;


 boost::python::class_<gunProtController, boost::python::bases<controller>, boost::noncopyable>
        ("gunProtController","DOCSTRING", boost::python::no_init)
        .def("getILockStates",    &gunProtController::getILockStates,
                                  "Return the state of interlocks as an integer. There Currently NO epics ilocks for the PI laser.")
        .def("getILockStatesStr", &gunProtController::getILockStatesStr,
                                  "Return state of interlocks as a stringr. There Currently NO epics ilocks for the PI laser.")
        .def("get_CA_PEND_IO_TIMEOUT",  &gunProtController::get_CA_PEND_IO_TIMEOUT,
                                        "Return the current waiting time [seconds] when sending commands to EPICS.")
        .def("set_CA_PEND_IO_TIMEOUT",  &gunProtController::set_CA_PEND_IO_TIMEOUT,
                                        (boost::python::arg("time"),
                                        "Set a new waiting time [seconds] when sending commands to EPICS"))
        .def("isGood", &gunProtController::isGood, (boost::python::arg("name"),"returns True if interlock status is good"))
        .def("isNotGood", &gunProtController::isNotGood, (boost::python::arg("name"),"returns True if interlock status is not good"))
        .def("isBad", &gunProtController::isBad, (boost::python::arg("name"),"returns True if interlock status is bad"))
        ;



    /// The main class that creates all the controller obejcts
        boost::python::class_<VCrfprot,boost::noncopyable> ("init")
        .def("virtual_Gun_Protection_Controller",  &VCrfprot::virtual_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual gun modulator object.")
        .def("physical_Gun_Protection_Controller",  &VCrfprot::physical_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical gun modulator object.")
        .def("offline_Gun_Protection_Controller",  &VCrfprot::offline_Gun_Protection_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline gun modulator object.")
        .def("setQuiet",         &VCrfprot::setQuiet,
             "set Quiet Mode (no messages, no debug messages) for all PI laser objects." )
        .def("setVerbose",       &VCrfprot::setVerbose,
             "set Verbose Mode (all messages, all debug messages) for all PI laser objects.")
        .def("setMessage",       &VCrfprot::setMessage,
             "set Message Mode (all  messages, no debug messages) for all PI laser objects.")
        .def("setDebugMessage",  &VCrfprot::setDebugMessage,
             "set Debug Mode (no messages, all debug messages) for all PI laser objects." )
        ;


}


//BOOST_PYTHON_MODULE(VELA_CLARA_MagnetControl )
//{
//        /// Things that you want to use in python muct be exposed:
//        /// containers
//        class_<std::vector< std::string > >("std_vector_string")
//                .def(vector_indexing_suite< std::vector< std::string >>() )
//                ;
//        class_<std::vector< double> >("std_vector_double")
//        .def(vector_indexing_suite< std::vector< double>>() )
//        ;
//        /// and enums, remember we have a enum to string python dictionary macro too!
//        enum_<VELA_ENUM::MAG_PSU_STATE>("MAG_PSU_STATE")
//                .value("MAG_PSU_OFF",   VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF   )
//                .value("MAG_PSU_ON",    VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON    )
//                .value("MAG_PSU_TIMING",VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING)
//                .value("MAG_PSU_ERROR", VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR )
//                .value("MAG_PSU_NONE",  VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE  )
//                ;
//        boost::python::enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
//                .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
//                .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
//                .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
//                ;
//        /// and enums, remember we have a enum to string python dictionary macro too!
//        enum_<magnetStructs::MAG_TYPE>("MAG_TYPE")
//                .value("QUAD", magnetStructs::MAG_TYPE::QUAD )
//                .value("DIP" , magnetStructs::MAG_TYPE::DIP  )
//                .value("HCOR", magnetStructs::MAG_TYPE::HCOR )
//                .value("VCOR", magnetStructs::MAG_TYPE::VCOR )
//                .value("BSOL", magnetStructs::MAG_TYPE::BSOL )
//                .value("SOL" , magnetStructs::MAG_TYPE::SOL  )
//                .value("SEXT", magnetStructs::MAG_TYPE::SEXT )
//                .value("UNKNOWN_MAGNET_TYPE",  magnetStructs::MAG_TYPE::UNKNOWN_MAGNET_TYPE  )
//                ;
//        /// and enums, remember we have a enum to string python dictionary macro too!
//        enum_<magnetStructs::MAG_REV_TYPE>("MAG_REV_TYPE")
//                .value("NR"       , magnetStructs::MAG_REV_TYPE::NR )
//                .value("BIPOLAR"  , magnetStructs::MAG_REV_TYPE::BIPOLAR  )
//                .value("NR_GANGED", magnetStructs::MAG_REV_TYPE::NR_GANGED )/// hopefully never again....
//                .value("POS"      , magnetStructs::MAG_REV_TYPE::POS )
//                .value("UNKNOWN_MAG_REV_TYPE", magnetStructs::MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE )
//                ;
//
//
//    /// structs (this one is a one-stop shop for comomn parameters)
//    boost::python::class_<magnetStructs::magnetStateStruct>
//        ("magnetStateStruct")
//        .add_property("numMags",   &magnetStructs::magnetStateStruct::numMags)
//        .add_property("magNames",  &magnetStructs::magnetStateStruct::magNames)
//        .add_property("psuStates", &magnetStructs::magnetStateStruct::psuStates)
//        .add_property("siValues",  &magnetStructs::magnetStateStruct::siValues)
//        .add_property("riValues",  &magnetStructs::magnetStateStruct::riValues)
//        ;
//
//    /// Expose base classes
//    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
//        ;
//    /// we have to tell boost.python about pure virtual methods in abstract base classes
//    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
//        ("controller", boost::python::no_init) /// forces Python to not be able to construct (init) this object
//        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
//        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
//        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
//        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
//        ;
//
//    boost::python::class_<magnetStructs::nrPSUObject,boost::noncopyable>
//        ("nrPSUObject", boost::python::no_init)
//        .def_readonly("psuState", &magnetStructs::nrPSUObject::psuState)
//        ;
//
////    struct  nrPSUObject
////    {   // proviude a default constructor
////        nrPSUObject() : isGanged(false ), parentMagnet("UNKNOWN" ), pvRoot("UNKNOWN"),
////                        psuState(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),
////                        numIlocks(0 ) {} // proviude a default constructor
////        std::string  parentMagnet, pvRoot;
////        bool isGanged;//, canFlip;/// canflip? probably refactor as function...
////        VELA_ENUM::MAG_PSU_STATE psuState;
////        size_t numIlocks;
////        std::vector< std::string > gangMembers;
////        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
////    #ifndef __CINT__
////        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
////        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
////        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
////    #endif
////    };
//
//    boost::python::class_<magnetStructs::magnetObject,boost::noncopyable>
//        ("magnetObject", boost::python::no_init)
//        .def_readonly("magType",    &magnetStructs::magnetObject::magType)
//        .def_readonly("psuState",   &magnetStructs::magnetObject::psuState)
//        .def_readonly("magRevType", &magnetStructs::magnetObject::magRevType)
//        .def_readonly("siWithPol",  &magnetStructs::magnetObject::siWithPol)
//        .def_readonly("riWithPol",  &magnetStructs::magnetObject::riWithPol)
//        .def_readonly("name",       &magnetStructs::magnetObject::name)
//        .def_readonly("nPSU",       &magnetStructs::magnetObject::nPSU)
//        .def_readonly("rPSU",       &magnetStructs::magnetObject::rPSU)
//        .def_readonly("degValues",  &magnetStructs::magnetObject::degValues)
//        ;
//
////     struct  magnetObject
////    {   // proviude a default constructor
////        magnetObject() : magType (MAG_TYPE::UNKNOWN_MAGNET_TYPE), isGanged(false ), name("UNKNOWN"),pvRoot("UNKNOWN"),
////                psuState(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),canNRFlip(false ),samePSURoot(false ),
////                magRevType(MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE ),
////                si(-999.999), ri(-999.999), siWithPol(-999.999), riWithPol(-999.999), riTolerance(-999.999),
////                /// err... , an atomic_bool for isDegaussing(false ) does not work ... http://stackoverflow.com/questions/15750917/initializing-stdatomic-bool
////                /// ... which is probably evil && dangerous
////                numIlocks(0 ),
////                //added deguassing initialisers here
////                numDegaussSteps(0), maxWaitTime(0), numDegaussElements(0) {} // proviude a default constructor
////        MAG_TYPE magType;           /// dipole, quad etc.
////        MAG_REV_TYPE  magRevType;   /// reverse type, NR, bipolar etc.
////        VELA_ENUM::MAG_PSU_STATE psuState;
////        size_t numIlocks;
////        nrPSUObject nPSU, rPSU;
////        std::string  name, pvRoot, psuRoot;
////        bool isGanged, canNRFlip, samePSURoot;
////        std::vector< std::string > gangMembers;
////        double si, ri, siWithPol, riWithPol, riTolerance;
////
////        //DEGUASSING: added here by Tim Price
////
////        size_t numDegaussSteps, maxWaitTime, numDegaussElements;
////        std::vector< double > degValues;
////        double  degTolerance;
////
//////        std::atomic< bool > isDegaussing;/// NO thread safe copy constructor malarkey...  http://stackoverflow.com/questions/29332897/error-c2280-attempting-to-reference-a-deleted-function-atomicint
////        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
////    #ifndef __CINT__
////        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
////        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
////        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
////    #endif
////    };
//
//    /// and the class member functions to expose to python,
//    /// remmeber to include enum definitions as boost::python::dict <int, string>
//    /// as well as boost::python::dict <int, int>
//    boost::python::class_<screenController, boost::python::bases<controller>, boost::noncopyable>
//        ("screenController","screenController Doc String",boost::python::no_init)
//        .def("getILockStates",           &screenController::getILockStates        )
//        .def("getMagPSUStateDefinition", &screenController::getMagPSUStateDefinition )
//        .def("getILockStatesDefinition", &screenController::getILockStatesDefinition )
//        .def("get_CA_PEND_IO_TIMEOUT",   &screenController::get_CA_PEND_IO_TIMEOUT   )
//        .def("set_CA_PEND_IO_TIMEOUT",   &screenController::set_CA_PEND_IO_TIMEOUT   )
//        .def("getSI", getSI_1)
//        .def("getSI", getSI_2)
//        .def("getSI", getRI_2)
//        .def("getRI", getRI_1)
//        .def("setSI", setSI_1)
//        .def("setSI", setSI_2)
//        .def("setSI", setSI_3)
//        .def("setSI",  setSI_4)
//        .def("switchONpsu", switchONpsu_1)
//        .def("switchONpsu", switchONpsu_2)
//        .def("switchOFFpsu",switchOFFpsu_1)
//        .def("switchOFFpsu",switchOFFpsu_2)
//        .def("degauss",   degauss_1)
//        .def("degauss",   degauss_2)
//        .def("writeDBURT",     writeDBURT_1)
//        .def("writeDBURT",    writeDBURT_2)
//        .def("getCurrentMagnetState",    getCurrentMagnetState_1)
//        .def("getCurrentMagnetState",    getCurrentMagnetState_2)
//        .def("getMagObjConstRef",        &screenController::getMagObjConstRef,return_value_policy<reference_existing_object>())
//        .def("isAQuad",                  &screenController::isAQuad    )
//        .def("isABSol",                  &screenController::isABSol    )
//        .def("isAHCor",                  &screenController::isAHCor    )
//        .def("isAVCor",                  &screenController::isAVCor    )
//        .def("isADip",                   &screenController::isADip    )
//        .def("isASol",                   &screenController::isASol    )
//        .def("isACor",                   &screenController::isACor    )
//        .def("isNR",                     &screenController::isNR         )
//        .def("isBipolar",                &screenController::isBipolar     )
//        .def("isNRGanged",               &screenController::isNRGanged    )
//        .def("isNRorNRGanged",           &screenController::isNRorNRGanged)
//        .def("isON_psuN",                &screenController::isON_psuN     )
//        .def("isON_psuR",                &screenController::isON_psuR     )
//        .def("isON",                     &screenController::isON          )
//        .def("isOFF_psuN",               &screenController::isOFF_psuN    )
//        .def("isOFF_psuR",               &screenController::isOFF_psuR    )
//        .def("isOFF",                    &screenController::isOFF         )
//        .def("isDegaussing",             &screenController::isDegaussing    )
//        .def("isNotDegaussing",          &screenController::isNotDegaussing )
//        .def("getMagnetNames",           &screenController::getMagnetNames     )
//        .def("getQuadNames",             &screenController::getQuadNames       )
//        .def("getHCorNames",             &screenController::getHCorNames       )
//        .def("getVCorNames",             &screenController::getVCorNames       )
//        .def("getDipNames",              &screenController::getDipNames        )
//        .def("getSolNames",              &screenController::getSolNames        )
//        .def("applyDBURT",               &screenController::applyDBURT         )
//        .def("applyDBURTCorOnly",        &screenController::applyDBURTCorOnly  )
//        .def("applyDBURTQuadOnly",       &screenController::applyDBURTQuadOnly )
//        .def("getDBURT",                 &screenController::getDBURT        )
//        /// Don't forget functions in the base class we want to expose....
//        .def("debugMessagesOff",         &screenController::debugMessagesOff )
//        .def("debugMessagesOn",          &screenController::debugMessagesOn )
//        .def("messagesOff",              &screenController::messagesOff )
//        .def("messagesOn",               &screenController::messagesOn )
//        .def("silence",                  &screenController::silence )
//        .def("verbose",                  &screenController::verbose )
//        .def("isSilent",                 &screenController::isSilent )
//        .def("isVerbose",                &screenController::isVerbose )
//        .def("isMessageOn",              &screenController::isMessageOn )
//        .def("isDebugMessageOn",         &screenController::isDebugMessageOn )
//        ;
//
//    /// Finally the main class that creates all the obejcts
//    boost::python::class_<VCscreens,boost::noncopyable> ("init")
//        .def("virtual_VELA_INJ_Magnet_Controller",  &VCscreens::virtual_VELA_INJ_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_VELA_INJ_Magnet_Controller",  &VCscreens::offline_VELA_INJ_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_VELA_INJ_Magnet_Controller",  &VCscreens::physical_VELA_INJ_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("virtual_VELA_BA1_Magnet_Controller",  &VCscreens::virtual_VELA_BA1_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_VELA_BA1_Magnet_Controller",  &VCscreens::offline_VELA_BA1_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_VELA_BA1_Magnet_Controller",  &VCscreens::physical_VELA_BA1_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("virtual_VELA_BA2_Magnet_Controller",  &VCscreens::virtual_VELA_BA2_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_VELA_BA2_Magnet_Controller",  &VCscreens::offline_VELA_BA2_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_VELA_BA2_Magnet_Controller",  &VCscreens::physical_VELA_BA2_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("virtual_CLARA_INJ_Magnet_Controller",  &VCscreens::virtual_CLARA_INJ_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("offline_CLARA_INJ_Magnet_Controller",  &VCscreens::offline_CLARA_INJ_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//        .def("physical_CLARA_INJ_Magnet_Controller",  &VCscreens::physical_CLARA_INJ_Magnet_Controller,
//             return_value_policy<reference_existing_object>())
//                ;
//
//
//
//
//}

#endif // VCrfprot_H_
