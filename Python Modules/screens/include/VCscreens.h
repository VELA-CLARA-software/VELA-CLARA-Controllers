#ifndef _VCSCREENS_H
#define _VCSCREENS_H
//stl
#include <string>
//
#include "screenController.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


class VCscreens
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for expliocit functions asa quick fix

    public:
        VCscreens();
        ~VCscreens();
        screenController& virtual_VELA_INJ_Screen_Controller();
        screenController& offline_VELA_INJ_Screen_Controller();
        screenController& physical_VELA_INJ_Screen_Controller();

        screenController& virtual_VELA_BA1_Screen_Controller();
        screenController& offline_VELA_BA1_Screen_Controller();
        screenController& physical_VELA_BA1_Screen_Controller();

        screenController& virtual_VELA_BA2_Screen_Controller();
        screenController& offline_VELA_BA2_Screen_Controller();
        screenController& physical_VELA_BA2_Screen_Controller();

        screenController& virtual_CLARA_INJ_Screen_Controller();
        screenController& offline_CLARA_INJ_Screen_Controller();
        screenController& physical_CLARA_INJ_Screen_Controller();

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        screenController& getScreenController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

    protected:

    private:

     /// we just need to decide good names for these things...
        screenController * virtual_VELA_INJ_Screen_Controller_Obj ;
        screenController * physical_VELA_INJ_Screen_Controller_Obj;
        screenController * offline_VELA_INJ_Screen_Controller_Obj ;

        screenController * virtual_VELA_BA1_Screen_Controller_Obj ;
        screenController * physical_VELA_BA1_Screen_Controller_Obj;
        screenController * offline_VELA_BA1_Screen_Controller_Obj ;

        screenController * virtual_VELA_BA2_Screen_Controller_Obj;
        screenController * offline_VELA_BA2_Screen_Controller_Obj;
        screenController * physical_VELA_BA2_Screen_Controller_Obj;

        screenController * virtual_CLARA_INJ_Screen_Controller_Obj;
        screenController * offline_CLARA_INJ_Screen_Controller_Obj;
        screenController * physical_CLARA_INJ_Screen_Controller_Obj;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

        const VELA_ENUM::MACHINE_AREA  VELA_INJ,VELA_BA1,VELA_BA2,CLARA_INJ,UNKNOWN_AREA;
};
/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
/// I'm going to do it the function pointer way here...
/// The other alternative is to create lots of different function names in the c++ class
/// just to make the function pointer overloading neater, let's make some typedefs, generally i don't like doing this
typedef double doub;
typedef const double cdou;
typedef std::vector<double> vecd;
typedef std::vector<std::vector<double>> vvcd;
typedef const std::vector<double> cved;

typedef const size_t csiz;
typedef size_t size;
typedef std::vector<size_t> vsiz;

typedef std::string stri;
typedef const std::string cstr;
typedef std::vector<std::string> vecs;
typedef const std::vector<std::string> cves;
typedef const screenStructs::SCREEN_STATE csta;
typedef const std::vector< screenStructs::SCREEN_STATE > stav;

typedef std::vector<bool> vecb;
///
bool(screenController::*screenIN_1)  (cstr& ) = &screenController::screenIN;
bool(screenController::*screenIN_2)  (cves& ) = &screenController::screenIN;
bool(screenController::*screenOUT_1) (cstr& ) = &screenController::screenOUT;
bool(screenController::*screenOUT_2) (cves& ) = &screenController::screenOUT;

bool(screenController::*screenMoveTo_1)(cves&, stav& ) = &screenController::screenMoveTo;
bool(screenController::*screenMoveTo_2)(cstr&, csta& ) = &screenController::screenMoveTo;

bool(screenController::*isScreenIN_1)(cstr& ) = &screenController::isScreenIN;
vecb(screenController::*isScreenIN_2)(cves& ) = &screenController::isScreenIN;
bool(screenController::*isScreenOUT_1)(cstr&) = &screenController::isScreenOUT;
vecb(screenController::*isScreenOUT_2)(cves&) = &screenController::isScreenOUT;



using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_ScreenControl )
{
    /// Things that you want to use in python muct be exposed:
    /// containers
    class_<std::vector< std::string > >("std_vector_string")
            .def( vector_indexing_suite< std::vector< std::string >>() )
            ;
    class_<std::vector< double> >("std_vector_double")
            .def( vector_indexing_suite< std::vector< double>>() )
            ;
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
        .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
        .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
        .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
        ;
    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE  )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL  )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL )
        ;
    enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA")
        .value("VELA_INJ",     VELA_ENUM::MACHINE_AREA::VELA_INJ )
        .value("VELA_BA1",     VELA_ENUM::MACHINE_AREA::VELA_BA1 )
        .value("VELA_BA2",     VELA_ENUM::MACHINE_AREA::VELA_BA2 )
        .value("CLARA_INJ",    VELA_ENUM::MACHINE_AREA::CLARA_INJ)
        .value("CLARA_2_VELA", VELA_ENUM::MACHINE_AREA::CLARA_2_VELA)
        .value("UNKNOWN_AREA", VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA)
        ;
    // screen structs
    enum_<screenStructs::SCREEN_STATE>("SCREEN_STATE")
        .value("SCREEN_IN",       screenStructs::SCREEN_STATE::SCREEN_IN  )
        .value("SCREEN_OUT",      screenStructs::SCREEN_STATE::SCREEN_OUT )
        .value("SCREEN_MOVING",   screenStructs::SCREEN_STATE::SCREEN_MOVING )
        .value("SCREEN_ERROR",    screenStructs::SCREEN_STATE::SCREEN_ERROR  )
        .value("SCREEN_UNKNOWN",  screenStructs::SCREEN_STATE::SCREEN_UNKNOWN)
        .value("H_MIRROR",        screenStructs::SCREEN_STATE::H_MIRROR)
        .value("V_MIRROR",        screenStructs::SCREEN_STATE::V_MIRROR)
        .value("H_50U_SLIT",      screenStructs::SCREEN_STATE::H_50U_SLIT)
        .value("V_50U_SLIT",      screenStructs::SCREEN_STATE::V_50U_SLIT)
        .value("H_25U_SLIT",      screenStructs::SCREEN_STATE::H_25U_SLIT)
        .value("V_25U_SLIT",      screenStructs::SCREEN_STATE::V_25U_SLIT)
        .value("H_6p3MM_HOLE",    screenStructs::SCREEN_STATE::H_6p3MM_HOLE)
        .value("V_6p3MM_HOLE",    screenStructs::SCREEN_STATE::V_6p3MM_HOLE)
        .value("H_10MM_HOLE",     screenStructs::SCREEN_STATE::H_10MM_HOLE)
        .value("V_10MM_HOLE",     screenStructs::SCREEN_STATE::V_10MM_HOLE)
        .value("H_YAG",           screenStructs::SCREEN_STATE::H_YAG)
        .value("V_YAG",           screenStructs::SCREEN_STATE::V_YAG)
        .value("H_SLIT",          screenStructs::SCREEN_STATE::H_SLIT)
        .value("V_SLIT",          screenStructs::SCREEN_STATE::V_SLIT)
        .value("H_RF",            screenStructs::SCREEN_STATE::H_RF)
        .value("V_RF",            screenStructs::SCREEN_STATE::V_RF)
        .value("V_OUT",           screenStructs::SCREEN_STATE::V_OUT )
        .value("H_OUT",           screenStructs::SCREEN_STATE::H_OUT )
        .value("UNKNOWN_POSITION",screenStructs::SCREEN_STATE::UNKNOWN_POSITION )
        ;
    enum_<screenStructs::SCREEN_TYPE>("SCREEN_TYPE")
        .value("UNKNOWN_SCREEN_TYPE", screenStructs::SCREEN_TYPE::UNKNOWN_SCREEN_TYPE )
        .value("VELA_HV_MOVER",       screenStructs::SCREEN_TYPE::VELA_HV_MOVER  )
        .value("VELA_PNEUMATIC",      screenStructs::SCREEN_TYPE::VELA_PNEUMATIC )
        ;
    enum_<screenStructs::DRIVER_STATE>("DRIVER_STATE")
        .value("DRIVER_MOVING", screenStructs::DRIVER_STATE::DRIVER_MOVING )
        .value("DRIVER_STATIONARY",       screenStructs::DRIVER_STATE::DRIVER_STATIONARY )
        .value("DRIVER_DISABLED",      screenStructs::DRIVER_STATE::DRIVER_DISABLED )
        .value("DRIVER_ENABLED",      screenStructs::DRIVER_STATE::DRIVER_ENABLED )
        .value("DRIVER_ERROR",      screenStructs::DRIVER_STATE::DRIVER_ERROR )
        ;
    enum_<screenStructs::DRIVER_DIRECTION>("DRIVER_DIRECTION")
        .value("HORIZONTAL",  screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        .value("VERTICAL",    screenStructs::DRIVER_DIRECTION::VERTICAL )
        .value("NONE",        screenStructs::DRIVER_DIRECTION::NONE )
        ;
    // maps foudn in objects

    class_<std::map<screenStructs::SCREEN_STATE,bool>>("std_map_SCREEN_STATE_bool")
        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,bool> >())
        ;
    class_<std::map<screenStructs::SCREEN_STATE,double>>("std_map_SCREEN_STATE_double")
        .def(map_indexing_suite<  std::map<screenStructs::SCREEN_STATE,double> >())
        ;
    class_<std::map<std::string,bool>>("std_map_SCREEN_STATE_bool")
        .def(map_indexing_suite< std::map<std::string, bool> >())
        ;

    boost::python::class_<screenStructs::screenDriverStatus,boost::noncopyable>
        ("screenDriverStatus", boost::python::no_init)
        .def_readonly("parentScreen",   &screenStructs::screenDriverStatus::parentScreen)
        .def_readonly("position",       &screenStructs::screenDriverStatus::position)
        .def_readonly("STA",            &screenStructs::screenDriverStatus::STA)
        .def_readonly("STA_bit_label",  &screenStructs::screenDriverStatus::STA_bit_label)
        .def_readonly("STA_bit_map",    &screenStructs::screenDriverStatus::STA_bit_map)
        .def_readonly("dir",            &screenStructs::screenDriverStatus::dir)
        .def_readonly("state",          &screenStructs::screenDriverStatus::state)
        ;
    boost::python::class_<screenStructs::screenCassette,boost::noncopyable>
        ("screenCassette", boost::python::no_init)
        .def_readonly("parentScreen",               &screenStructs::screenCassette::parentScreen)
        .def_readonly("screenState",                &screenStructs::screenCassette::screenState)
        .def_readonly("posTolerance",               &screenStructs::screenCassette::posTolerance)
        .def_readonly("cassetteElements",           &screenStructs::screenCassette::cassetteElements)
        .def_readonly("cassetteElementsPosition",   &screenStructs::screenCassette::cassetteElementsPosition)
        .def_readonly("dir",                        &screenStructs::screenCassette::dir)
        ;
    boost::python::class_<screenStructs::screenDriver,boost::noncopyable>
        ("screenDriver", boost::python::no_init)
        .def_readonly("positionError",  &screenStructs::screenDriver::positionError)
        .def_readonly("homeError",      &screenStructs::screenDriver::homeError)
        .def_readonly("parentScreen",   &screenStructs::screenDriver::parentScreen)
        .def_readonly("hDriverSTA",     &screenStructs::screenDriver::hDriverSTA)
        .def_readonly("vDriverSTA",     &screenStructs::screenDriver::vDriverSTA)
        .def_readonly("hCassette",      &screenStructs::screenDriver::hCassette)
        .def_readonly("vCassette",      &screenStructs::screenDriver::vCassette)
        ;

    boost::python::class_<screenStructs::screenObject,boost::noncopyable>
        ("screenObject", boost::python::no_init)
        .def_readonly("screenType",  &screenStructs::screenObject::screenType)
        .def_readonly("driver",      &screenStructs::screenObject::driver)
        .def_readonly("screenState", &screenStructs::screenObject::screenState)
        .def_readonly("numIlocks",   &screenStructs::screenObject::numIlocks)
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

    boost::python::class_<screenController, boost::python::bases<controller>, boost::noncopyable>
        ("screenController","screenController Doc String",boost::python::no_init)
        .def("getILockStates",           &screenController::getILockStates        )
//        .def("getMagPSUStateDefinition", &magnetController::getMagPSUStateDefinition )
//        .def("getILockStatesDefinition", &magnetController::getILockStatesDefinition )
        .def("get_CA_PEND_IO_TIMEOUT",  &screenController::get_CA_PEND_IO_TIMEOUT   )
        .def("set_CA_PEND_IO_TIMEOUT",  &screenController::set_CA_PEND_IO_TIMEOUT   )
        .def("screenIN",                screenIN_1  )
        .def("screenIN",                screenIN_2  )
        .def("screenOUT",               screenOUT_1 )
        .def("screenOUT",               screenOUT_2 )
        .def("is_VELA_PNEUMATIC",       &screenController::is_VELA_PNEUMATIC )
        .def("is_VELA_HV_MOVER",        &screenController::is_VELA_HV_MOVER  )
        .def("get_VELA_PNEUMATIC_Screens", &screenController::get_VELA_PNEUMATIC_Screens )
        .def("get_VELA_HV_MOVER_Screens",  &screenController::get_VELA_HV_MOVER_Screens )
        .def("screenMoveTo",               screenMoveTo_1 )
        .def("screenMoveTo",               screenMoveTo_2 )
        .def("setPosition",                &screenController::setPosition )
        .def("isMoving",                   &screenController::isMoving )
        .def("isNotMoving",                &screenController::isNotMoving )
        .def("is_VELA_PNEUMATIC",          &screenController::is_VELA_PNEUMATIC )
        .def("is_VELA_HV_MOVER",           &screenController::is_VELA_HV_MOVER )
        .def("is_H_Element",               &screenController::is_H_Element )
        .def("is_V_Element",               &screenController::is_V_Element )
        .def("isScreenOUT",                isScreenOUT_1 )
        .def("isScreenOUT",                isScreenOUT_2 )
        .def("isScreenIN",                 isScreenIN_1  )
        .def("isScreenIN",                 isScreenIN_2  )
        .def("exists_and_isLocked",        &screenController::exists_and_isLocked        )
        .def("exists_and_isNotLocked",     &screenController::exists_and_isNotLocked     )
        .def("get_VELA_PNEUMATIC_Screens", &screenController::get_VELA_PNEUMATIC_Screens )
        .def("get_VELA_HV_MOVER_Screens",  &screenController::get_VELA_HV_MOVER_Screens  )
        .def("getScreenState",             &screenController::getScreenState             )
        .def("isScreenInState",            &screenController::isScreenInState            )
        .def("get_SCREEN_STATE_Definition",&screenController::get_SCREEN_STATE_Definition )
        .def("get_DRIVER_STATE_Definition",&screenController::get_DRIVER_STATE_Definition )
        ;

    /// The main class that creates all the controller obejcts
        boost::python::class_<VCscreens,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Screen_Controller",  &VCscreens::virtual_VELA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Screen_Controller",  &VCscreens::offline_VELA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Screen_Controller",  &VCscreens::physical_VELA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Screen_Controller",  &VCscreens::virtual_VELA_BA1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Screen_Controller",  &VCscreens::offline_VELA_BA1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Screen_Controller",  &VCscreens::physical_VELA_BA1_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Screen_Controller",  &VCscreens::virtual_VELA_BA2_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Screen_Controller",  &VCscreens::offline_VELA_BA2_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Screen_Controller",  &VCscreens::physical_VELA_BA2_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_INJ_Screen_Controller",  &VCscreens::virtual_CLARA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_Screen_Controller",  &VCscreens::offline_CLARA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_Screen_Controller",  &VCscreens::physical_CLARA_INJ_Screen_Controller,
             return_value_policy<reference_existing_object>())
        .def("getScreenController",  &VCscreens::getScreenController,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VCscreens::setQuiet )
        .def("setVerbose",       &VCscreens::setVerbose )
        .def("setMessage",       &VCscreens::setMessage )
        .def("setDebugMessage",  &VCscreens::setDebugMessage )
        ;
}

#endif // _VCSCREENS_H


//
///// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
///// Create a load of different function pointers and use them in the bindings
///// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
///// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
///// I'm going to do it the function pointer way here...
///// The other alternative is to create lots of different function names in the c++ class
///// just to make the function pointer overloading neater, lets make some typedefs, generally i don't like doing this
//typedef double doub;
//typedef const double cdou;
//typedef const size_t csiz;
//typedef const std::string cstr ;
//typedef std::vector<double> vecd;
//typedef const std::vector<double> cved;
//typedef std::vector<std::string> vecs;
//typedef const std::vector<std::string> cves;
//typedef magnetStructs::magnetStateStruct msst;
//
//doub(screenController::*getRI_1)(cstr&) = &screenController::getRI;
//vecd(screenController::*getRI_2)(cves&) = &screenController::getRI;
//
//vecd(screenController::*getSI_2)(cves&) = &screenController::getSI;
//doub(screenController::*getSI_1)(cstr&) = &screenController::getSI;
//
//bool(screenController::*setSI_1)(cstr&, cdou ) = &screenController::setSI;
//bool(screenController::*setSI_2)(cves&, cved&) = &screenController::setSI;
//bool(screenController::*setSI_3)(cstr&, cdou , cdou , csiz) = &screenController::setSI;
//vecs(screenController::*setSI_4)(cves&, cved&, cved&, csiz) = &screenController::setSI;
//
//bool(screenController::*switchONpsu_1 )(cstr&) = &screenController::switchONpsu;
//bool(screenController::*switchONpsu_2 )(cves&) = &screenController::switchONpsu;
//bool(screenController::*switchOFFpsu_1)(cstr&) = &screenController::switchOFFpsu;
//bool(screenController::*switchOFFpsu_2)(cves&) = &screenController::switchOFFpsu;
//
//size_t(screenController::*degauss_1)(cstr&, bool ) = &screenController::degauss;
//size_t(screenController::*degauss_2)(cves&, bool ) = &screenController::degauss;
//
//bool(screenController::*writeDBURT_1)( const msst&, cstr&, cstr&) = &screenController::writeDBURT;
//bool(screenController::*writeDBURT_2)(              cstr&, cstr&) = &screenController::writeDBURT;
//
//msst(screenController::*getCurrentMagnetState_1)( cves&) = &screenController::getCurrentMagnetState;
//msst(screenController::*getCurrentMagnetState_2)(      ) = &screenController::getCurrentMagnetState;
//
//using namespace boost::python;
//
//BOOST_PYTHON_MODULE( VELA_CLARA_MagnetControl )
//{
//        /// Things that you want to use in python muct be exposed:
//        /// containers
//        class_<std::vector< std::string > >("std_vector_string")
//                .def( vector_indexing_suite< std::vector< std::string >>() )
//                ;
//        class_<std::vector< double> >("std_vector_double")
//        .def( vector_indexing_suite< std::vector< double>>() )
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
////        nrPSUObject() : isGanged( false ), parentMagnet( "UNKNOWN" ), pvRoot( "UNKNOWN"),
////                        psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),
////                        numIlocks( 0 ) {} // proviude a default constructor
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
////        magnetObject() : magType (MAG_TYPE::UNKNOWN_MAGNET_TYPE), isGanged( false ), name("UNKNOWN"),pvRoot( "UNKNOWN"),
////                psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),canNRFlip( false ),samePSURoot( false ),
////                magRevType( MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE ),
////                si(-999.999), ri(-999.999), siWithPol(-999.999), riWithPol(-999.999), riTolerance(-999.999),
////                /// err... , an atomic_bool for isDegaussing( false ) does not work ... http://stackoverflow.com/questions/15750917/initializing-stdatomic-bool
////                /// ... which is probably evil && dangerous
////                numIlocks( 0 ),
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

