#ifndef vela_INJ_VAC_VALVE_CONTROLLER_H
#define vela_INJ_VAC_VALVE_CONTROLLER_H

// project
#include "vacuumValveController.h"
#include "vacuumValveStructs.h"
#include "VCheader.h"
#include "VCbase.h"
#include "configDefinitions.h"
// stl
#include <string>
#include <vector>
#include <map>
#include <list>
//boost
//#include <boost/python/detail/wrap_python.hpp>
//#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
//#define BOOST_LIB_DIAGNOSTIC
//#include <boost/config.hpp>
//#include <boost/python.hpp>
//#include <boost/python/class.hpp>
//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python/object/function.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/docstring_options.hpp>

class VCvacValves : public VCbase
{
    public:

        /// we have overloaded constructors to specify config-file location
//        velaINJBeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true, const std::string configFileLocation );
        VCvacValves();
        ~VCvacValves();

        ///VELA_INJ
        vacuumValveController & virtual_VELA_INJ_Vac_Valve_Controller();
        vacuumValveController & offline_VELA_INJ_Vac_Valve_Controller();
        vacuumValveController & physical_VELA_INJ_Vac_Valve_Controller();
        ///VELA_BA1
        vacuumValveController & virtual_VELA_BA1_Vac_Valve_Controller();
        vacuumValveController & offline_VELA_BA1_Vac_Valve_Controller();
        vacuumValveController & physical_VELA_BA1_Vac_Valve_Controller();
        ///VELA_BA2
        vacuumValveController & virtual_VELA_BA2_Vac_Valve_Controller();
        vacuumValveController & offline_VELA_BA2_Vac_Valve_Controller();
        vacuumValveController & physical_VELA_BA2_Vac_Valve_Controller();
        ///CLARA_PH1
        vacuumValveController & virtual_CLARA_PH1_Vac_Valve_Controller();
        vacuumValveController & offline_CLARA_PH1_Vac_Valve_Controller();
        vacuumValveController & physical_CLARA_PH1_Vac_Valve_Controller();


        ///ALL_VELA_CLARA
        vacuumValveController & virtual_Vac_Valve_Controller();
        vacuumValveController & offline_Vac_Valve_Controller();
        vacuumValveController & physical_Vac_Valve_Controller();


        vacuumValveController & getVacValveController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area );

    protected:
    private:

        std::map<vacuumValveController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        vacuumValveController& getController(vacuumValveController*& cont,
                                             const std::string& conf1,
                                             const std::string& name,
                                             const bool shouldVM,
                                             const bool shouldEPICS,
                                             const HWC_ENUM::MACHINE_AREA myMachineArea);

        ///VELA_INJ
        vacuumValveController * virtual_VELA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_VELA_INJ_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_VELA_INJ_Vac_Valve_Controller_Obj;
        ///VELA_BA1
        vacuumValveController * virtual_VELA_BA1_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_VELA_BA1_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_VELA_BA1_Vac_Valve_Controller_Obj;
        ///VELA_BA2
        vacuumValveController * virtual_VELA_BA2_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_VELA_BA2_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_VELA_BA2_Vac_Valve_Controller_Obj;
        ///CLARA_PH1
        vacuumValveController * virtual_CLARA_PH1_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_CLARA_PH1_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_CLARA_PH1_Vac_Valve_Controller_Obj;
        ///ALL_VELA_CLARA
        vacuumValveController * virtual_Vac_Valve_Controller_Obj;
        vacuumValveController * offline_Vac_Valve_Controller_Obj;
        vacuumValveController * physical_Vac_Valve_Controller_Obj;

};

//#ifdef BUILD_DLL

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
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    enum_<vacuumValveStructs::VALVE_STATE>("VALVE_STATE")
            .value("VALVE_CLOSED", vacuumValveStructs::VALVE_STATE::VALVE_CLOSED )
            .value("VALVE_OPEN",   vacuumValveStructs::VALVE_STATE::VALVE_OPEN   )
            .value("VALVE_TIMING", vacuumValveStructs::VALVE_STATE::VALVE_TIMING )
            .value("VALVE_ERROR",  vacuumValveStructs::VALVE_STATE::VALVE_ERROR  )
            ;

    class_<vacuumValveStructs::vacValveObject>
        ("vacValveObject","vacValveObject Doc String", no_init)
        .def_readonly("name",   &vacuumValveStructs::vacValveObject::name   ,  "valve name")
        .def_readonly("pvRoot", &vacuumValveStructs::vacValveObject::pvRoot,"valve pvRoot")
        .def_readonly("vacValveState",  &vacuumValveStructs::vacValveObject::vacValveState, "valve state")
        .def_readonly("state",  &vacuumValveStructs::vacValveObject::vacValveState, "valve state")
        .def_readonly("numIlocks",   &vacuumValveStructs::vacValveObject::numIlocks,  "numIlocks")
        ;

    /// we have to tell boost.python about pure virtual methods in abstract base classes
    char const* getVacValveStateString = "Returns (as a HWC_ENUM) the state of the vacuum valve - open, closed, moving, or error.\n";
    char const* getVacValveObjectString = "Returns the vacuum valve struct, containing name, PV root, current state and interlock states.\n";
    char const* closeAndWaitString = "Closes the specified valve (arg1 (string)) and waits for the given time (arg2 (time_t)).\n";
    char const* openAndWaitString = "Opens the specified valve (arg1 (string)) and waits for the given time (arg2 (time_t)).\n";
    char const* isOpenString = "Checks if the given valve (arg1 (string)) is open, returns 1 if yes, 0 if no.\n";
    char const* isClosedString = "Checks if the given valve (arg1 (string)) is closed, returns 1 if yes, 0 if no.\n";
    char const* closeValveString = "Closes the specified valve (arg1 (string)).\n";
    char const* openValveString = "Opens the specified valve (arg1 (string)).\n";
    char const* getVacValveNamesString = "Returns, as a vector of strings, the available vacuum valve names given in the config file.\n";
    char const* getILockStates = "Returns, a map of valve interlock states, keyed by interlock number.\n.";

    /// member functiosn to expose to python, remmeber to include enum deifntions as boost::python::dict <int, string>

	boost::python::class_<vacuumValveController, boost::python::bases<controller>, boost::noncopyable>
            ("vacuumValveController","This class contains all the functions in the vacuum valve controller for monitoring and controlling PVs",boost::python::no_init)
//            .def("getVacValveStateDefinition",      &vacuumValveController::getVacValveStateDefinition  )
            .def("getILockStatesDefinition",        &vacuumValveController::getILockStatesDefinition                            )
            .def("getVacValveObjConstRef",          &vacuumValveController::getVacValveObjConstRef,return_value_policy<reference_existing_object>() )
            .def("get_CA_PEND_IO_TIMEOUT",          &vacuumValveController::get_CA_PEND_IO_TIMEOUT                              )
            .def("set_CA_PEND_IO_TIMEOUT",          &vacuumValveController::set_CA_PEND_IO_TIMEOUT                              )
            .def("getVacValveState",                &vacuumValveController::getVacValveState, getVacValveStateString            )
            .def("getVacValveStateStr",             &vacuumValveController::getVacValveStateStr, getVacValveStateString            )
            .def("getILockStates",                  &vacuumValveController::getILockStates, getILockStates              )
            .def("closeVacValve",                   &vacuumValveController::closeVacValve, closeValveString               )
            .def("close",                           &vacuumValveController::closeVacValve, closeValveString               )
            .def("closeValve1",                     &vacuumValveController::closeValve1                 )
            .def("closeValve2",                     &vacuumValveController::closeValve2                 )
            .def("closeValve3",                     &vacuumValveController::closeValve3                 )
            .def("closeValve4",                     &vacuumValveController::closeValve4                 )
            .def("closeValve5",                     &vacuumValveController::closeValve5                 )
            .def("closeValve6",                     &vacuumValveController::closeValve6                 )
            .def("closeValve7",                     &vacuumValveController::closeValve7                 )
            .def("openVacValve",                    &vacuumValveController::openVacValve, openValveString                )
            .def("open",                            &vacuumValveController::openVacValve, openValveString                )
            .def("openValve1",                      &vacuumValveController::openValve1                  )
            .def("openValve2",                      &vacuumValveController::openValve2                  )
            .def("openValve3",                      &vacuumValveController::openValve3                  )
            .def("openValve4",                      &vacuumValveController::openValve4                  )
            .def("openValve5",                      &vacuumValveController::openValve5                  )
            .def("openValve6",                      &vacuumValveController::openValve6                  )
            .def("openValve7",                      &vacuumValveController::openValve7                  )
            .def("closeAndWait",                    &vacuumValveController::closeAndWait, closeAndWaitString                )
            .def("closeAndWaitValve1",              &vacuumValveController::closeAndWaitValve1          )
            .def("closeAndWaitValve2",              &vacuumValveController::closeAndWaitValve2          )
            .def("closeAndWaitValve3",              &vacuumValveController::closeAndWaitValve3          )
            .def("closeAndWaitValve4",              &vacuumValveController::closeAndWaitValve4          )
            .def("closeAndWaitValve5",              &vacuumValveController::closeAndWaitValve5          )
            .def("closeAndWaitValve6",              &vacuumValveController::closeAndWaitValve6          )
            .def("closeAndWaitValve7",              &vacuumValveController::closeAndWaitValve7          )
            .def("openAndWait",                     &vacuumValveController::openAndWait, openAndWaitString                 )
            .def("openAndWaitValve1",               &vacuumValveController::openAndWaitValve1           )
            .def("openAndWaitValve2",               &vacuumValveController::openAndWaitValve2           )
            .def("openAndWaitValve3",               &vacuumValveController::openAndWaitValve3           )
            .def("openAndWaitValve4",               &vacuumValveController::openAndWaitValve4           )
            .def("openAndWaitValve5",               &vacuumValveController::openAndWaitValve5           )
            .def("openAndWaitValve6",               &vacuumValveController::openAndWaitValve6           )
            .def("openAndWaitValve7",               &vacuumValveController::openAndWaitValve7           )
            .def("getVacValveNames",                &vacuumValveController::getVacValveNames_Py, getVacValveNamesString            )
            .def("getNames",                        &vacuumValveController::getVacValveNames_Py, getVacValveNamesString            )
//            .def("closeAndWait",                    &velaVacuumValveController::closeAndWait_Py , vvvc_overloads1()      )
//            .def("openAndWait",                     &velaVacuumValveController::openAndWait_Py  , vvvc_overloads2()      )
            .def("isClosed",                        &vacuumValveController::isClosed, isClosedString                    )//, boost::python::args("name") )
            .def("isOpen",                          &vacuumValveController::isOpen, isOpenString                      )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &vacuumValveController::debugMessagesOff            )
            .def("debugMessagesOn",                 &vacuumValveController::debugMessagesOn             )
            .def("messagesOff",                     &vacuumValveController::messagesOff                 )
            .def("messagesOn",                      &vacuumValveController::messagesOn                  )
            .def("silence",                         &vacuumValveController::silence                     )
            .def("verbose",                         &vacuumValveController::verbose                     )
		;

    boost::python::class_<VCvacValves,boost::python::bases<VCbase>,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Vac_Valve_Controller",   &VCvacValves::virtual_VELA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Vac_Valve_Controller",   &VCvacValves::offline_VELA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Vac_Valve_Controller",  &VCvacValves::physical_VELA_INJ_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Vac_Valve_Controller",   &VCvacValves::virtual_VELA_BA1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Vac_Valve_Controller",   &VCvacValves::offline_VELA_BA1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Vac_Valve_Controller",  &VCvacValves::physical_VELA_BA1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Vac_Valve_Controller",   &VCvacValves::virtual_VELA_BA2_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Vac_Valve_Controller",   &VCvacValves::offline_VELA_BA2_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Vac_Valve_Controller",  &VCvacValves::physical_VELA_BA2_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_PH1_Vac_Valve_Controller",  &VCvacValves::virtual_CLARA_PH1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_Vac_Valve_Controller",  &VCvacValves::offline_CLARA_PH1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_Vac_Valve_Controller", &VCvacValves::physical_CLARA_PH1_Vac_Valve_Controller, return_value_policy<reference_existing_object>())


        .def("virtual_Vac_Valve_Controller", &VCvacValves::virtual_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("offline_Vac_Valve_Controller", &VCvacValves::offline_Vac_Valve_Controller, return_value_policy<reference_existing_object>())
        .def("physical_Vac_Valve_Controller", &VCvacValves::physical_Vac_Valve_Controller, return_value_policy<reference_existing_object>())


        .def("getVacValveController",                   &VCvacValves::getVacValveController, return_value_policy<reference_existing_object>())
        ;
};

//#endif // BUILD_DLL

#endif // velaChrageScopeController_H

