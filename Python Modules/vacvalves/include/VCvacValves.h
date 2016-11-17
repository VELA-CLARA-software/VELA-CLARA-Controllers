#ifndef vela_INJ_VAC_VALVE_CONTROLLER_H
#define vela_INJ_VAC_VALVE_CONTROLLER_H

// project
#include "vacuumValveController.h"
// stl
#include <string>
#include <vector>
#include <map>
#include <list>
//boost
#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class VCvacValves// : public vacuumValveController
{
    public:

        /// we have overloaded constructors to specify config-file location
//        velaINJBeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true, const std::string configFileLocation );
        VCvacValves();
        ~VCvacValves();

        vacuumValveController & virtual_VELA_INJ_Vac_Valve_Controller();
        vacuumValveController & offline_VELA_INJ_Vac_Valve_Controller();
        vacuumValveController & physical_VELA_INJ_Vac_Valve_Controller();
        vacuumValveController & virtual_VELA_BA1_Vac_Valve_Controller();
        vacuumValveController & offline_VELA_BA1_Vac_Valve_Controller();
        vacuumValveController & physical_VELA_BA1_Vac_Valve_Controller();
        vacuumValveController & virtual_VELA_BA2_Vac_Valve_Controller();
        vacuumValveController & offline_VELA_BA2_Vac_Valve_Controller();
        vacuumValveController & physical_VELA_BA2_Vac_Valve_Controller();
        vacuumValveController & virtual_CLARA_INJ_Vac_Valve_Controller();
        vacuumValveController & offline_CLARA_INJ_Vac_Valve_Controller();
        vacuumValveController & physical_CLARA_INJ_Vac_Valve_Controller();
        vacuumValveController & virtual_C2V_Vac_Valve_Controller();
        vacuumValveController & offline_C2V_Vac_Valve_Controller();
        vacuumValveController & physical_C2V_Vac_Valve_Controller();
        vacuumValveController & getVacValveController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

#ifdef BUILD_DLL
#endif // BUILD_DLL

    protected:
    private:

        vacuumValveController * virtual_VELA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_VELA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_VELA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * virtual_VELA_BA1_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_VELA_BA1_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_VELA_BA1_Vac_Valve_Controller_Obj;
        vacuumValveController * virtual_VELA_BA2_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_VELA_BA2_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_VELA_BA2_Vac_Valve_Controller_Obj;
        vacuumValveController * virtual_CLARA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_CLARA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_CLARA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * virtual_C2V_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_C2V_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_C2V_Vac_Valve_Controller_Obj;

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

BOOST_PYTHON_MODULE( VELA_CLARA_Vac_Valve_Control )
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

    enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
            .value("OFFLINE",   VELA_ENUM::MACHINE_MODE::OFFLINE  )
            .value("VIRTUAL",   VELA_ENUM::MACHINE_MODE::VIRTUAL  )
            .value("PHYSICAL",  VELA_ENUM::MACHINE_MODE::PHYSICAL )
            ;

    enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA")
            .value("VELA_INJ",  VELA_ENUM::MACHINE_AREA::VELA_INJ  )
            .value("VELA_BA1",  VELA_ENUM::MACHINE_AREA::VELA_BA1  )
            .value("VELA_BA2",  VELA_ENUM::MACHINE_AREA::VELA_BA2  )
            .value("CLARA_INJ", VELA_ENUM::MACHINE_AREA::CLARA_INJ )
            .value("C2V",       VELA_ENUM::MACHINE_AREA::C2V       )
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


	boost::python::class_<vacuumValveController, boost::python::bases<controller>, boost::noncopyable>
            ("vacuumValveController","vacuumValve Doc String",boost::python::no_init)
//            .def("getVacValveStateDefinition",      &vacuumValveController::getVacValveStateDefinition  )
            .def("getILockStatesDefinition",        &vacuumValveController::getILockStatesDefinition    )
            .def("get_CA_PEND_IO_TIMEOUT",          &vacuumValveController::get_CA_PEND_IO_TIMEOUT      )
            .def("set_CA_PEND_IO_TIMEOUT",          &vacuumValveController::set_CA_PEND_IO_TIMEOUT      )
            .def("getVacValveState",                &vacuumValveController::getVacValveState            )
            .def("getILockStates",                  &vacuumValveController::getILockStates              )
            .def("closeVacValve",                      &vacuumValveController::closeVacValve            )
            .def("closeValve1",                     &vacuumValveController::closeValve1                 )
            .def("closeValve2",                     &vacuumValveController::closeValve2                 )
            .def("closeValve3",                     &vacuumValveController::closeValve3                 )
            .def("closeValve4",                     &vacuumValveController::closeValve4                 )
            .def("closeValve5",                     &vacuumValveController::closeValve5                 )
            .def("closeValve6",                     &vacuumValveController::closeValve6                 )
            .def("closeValve7",                     &vacuumValveController::closeValve7                 )
            .def("openVacValve",                    &vacuumValveController::openVacValve                )
            .def("openValve1",                      &vacuumValveController::openValve1                  )
            .def("openValve2",                      &vacuumValveController::openValve2                  )
            .def("openValve3",                      &vacuumValveController::openValve3                  )
            .def("openValve4",                      &vacuumValveController::openValve4                  )
            .def("openValve5",                      &vacuumValveController::openValve5                  )
            .def("openValve6",                      &vacuumValveController::openValve6                  )
            .def("openValve7",                      &vacuumValveController::openValve7                  )
            .def("closeAndWait",                    &vacuumValveController::closeAndWait                )
            .def("closeAndWaitValve1",              &vacuumValveController::closeAndWaitValve1          )
            .def("closeAndWaitValve2",              &vacuumValveController::closeAndWaitValve2          )
            .def("closeAndWaitValve3",              &vacuumValveController::closeAndWaitValve3          )
            .def("closeAndWaitValve4",              &vacuumValveController::closeAndWaitValve4          )
            .def("closeAndWaitValve5",              &vacuumValveController::closeAndWaitValve5          )
            .def("closeAndWaitValve6",              &vacuumValveController::closeAndWaitValve6          )
            .def("closeAndWaitValve7",              &vacuumValveController::closeAndWaitValve7          )
            .def("openAndWait",                     &vacuumValveController::openAndWait                 )
            .def("openAndWaitValve1",               &vacuumValveController::openAndWaitValve1           )
            .def("openAndWaitValve2",               &vacuumValveController::openAndWaitValve2           )
            .def("openAndWaitValve3",               &vacuumValveController::openAndWaitValve3           )
            .def("openAndWaitValve4",               &vacuumValveController::openAndWaitValve4           )
            .def("openAndWaitValve5",               &vacuumValveController::openAndWaitValve5           )
            .def("openAndWaitValve6",               &vacuumValveController::openAndWaitValve6           )
            .def("openAndWaitValve7",               &vacuumValveController::openAndWaitValve7           )
            .def("getVacValveNames",                &vacuumValveController::getVacValveNames            )
//            .def("closeAndWait",                    &velaVacuumValveController::closeAndWait_Py , vvvc_overloads1()      )
//            .def("openAndWait",                     &velaVacuumValveController::openAndWait_Py  , vvvc_overloads2()      )
            .def("isClosed",                        &vacuumValveController::isClosed                    )//, boost::python::args("name") )
            .def("isOpen",                          &vacuumValveController::isOpen                      )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &vacuumValveController::debugMessagesOff            )
            .def("debugMessagesOn",                 &vacuumValveController::debugMessagesOn             )
            .def("messagesOff",                     &vacuumValveController::messagesOff                 )
            .def("messagesOn",                      &vacuumValveController::messagesOn                  )
            .def("silence",                         &vacuumValveController::silence                     )
            .def("verbose",                         &vacuumValveController::verbose                     )
		;

    boost::python::class_<VCvacValves,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Vac_Valve_Controller",   &VCvacValves::virtual_VELA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Vac_Valve_Controller",   &VCvacValves::offline_VELA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Vac_Valve_Controller",  &VCvacValves::offline_VELA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Vac_Valve_Controller",   &VCvacValves::virtual_VELA_BA1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Vac_Valve_Controller",   &VCvacValves::offline_VELA_BA1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Vac_Valve_Controller",  &VCvacValves::offline_VELA_BA1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Vac_Valve_Controller",   &VCvacValves::virtual_VELA_BA2_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Vac_Valve_Controller",   &VCvacValves::offline_VELA_BA2_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Vac_Valve_Controller",  &VCvacValves::offline_VELA_BA2_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_INJ_Vac_Valve_Controller",  &VCvacValves::virtual_CLARA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_Vac_Valve_Controller",  &VCvacValves::offline_CLARA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_Vac_Valve_Controller", &VCvacValves::offline_CLARA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_C2V_Vac_Valve_Controller",        &VCvacValves::virtual_C2V_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_C2V_Vac_Valve_Controller",        &VCvacValves::offline_C2V_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_C2V_Vac_Valve_Controller",       &VCvacValves::offline_C2V_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("getVacValveController",                   &VCvacValves::getVacValveController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL

#endif // velaChrageScopeController_H

