#ifndef vela_QSCOPE_CONTROLLER_H
#define vela_QSCOPE_CONTROLLER_H

// project
#include "scopeController.h"
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

class VCscopes// : public beamPositionMonitorController
{
    public:

        /// we have overloaded constructors to specify config-file location
//        velaINJBeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true, const std::string configFileLocation );
        VCscopes();
        ~VCscopes();

        scopeController & virtual_VELA_INJ_Scope_Controller();
        scopeController & offline_VELA_INJ_Scope_Controller();
        scopeController & physical_VELA_INJ_Scope_Controller();
        scopeController & virtual_CLARA_INJ_Scope_Controller();
        scopeController & offline_CLARA_INJ_Scope_Controller();
        scopeController & physical_CLARA_INJ_Scope_Controller();
        scopeController & getScopeController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

#ifdef BUILD_DLL
#endif // BUILD_DLL

    protected:
    private:

        bool withEPICS;
        bool withoutEPICS;
        bool withVM;
        bool withoutVM;
        bool showDebugMessages;
        bool showMessages;
        VELA_ENUM::MACHINE_AREA VELA_INJ;
        VELA_ENUM::MACHINE_AREA VELA_BA1;
        VELA_ENUM::MACHINE_AREA VELA_BA2;
        VELA_ENUM::MACHINE_AREA CLARA_INJ;
        VELA_ENUM::MACHINE_AREA CLARA_2_VELA;
        VELA_ENUM::MACHINE_AREA UNKNOWN_AREA;

        scopeController * virtual_VELA_INJ_Scope_Controller_Obj;
        scopeController * offline_VELA_INJ_Scope_Controller_Obj;
        scopeController * physical_VELA_INJ_Scope_Controller_Obj;
        scopeController * virtual_CLARA_INJ_Scope_Controller_Obj;
        scopeController * offline_CLARA_INJ_Scope_Controller_Obj;
        scopeController * physical_CLARA_INJ_Scope_Controller_Obj;

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

BOOST_PYTHON_MODULE( VELA_CLARA_Scope_Control )
{
    /// Include ALL the enums you want to expose to Python

    class_< std::map< scopeStructs::SCOPE_PV_TYPE, std::vector< double > > > ("v2_map")
        .def( map_indexing_suite< std::map< scopeStructs::SCOPE_PV_TYPE, std::vector< double > > >());

    class_< std::map< scopeStructs::SCOPE_PV_TYPE, bool > >("bool_map")
        .def( map_indexing_suite< std::map< scopeStructs::SCOPE_PV_TYPE, bool > >());

    class_< std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string > >());

    class_< std::vector< double> > ("v_double")
        .def( vector_indexing_suite< std::vector< double > >());

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
            .value("C2V",       VELA_ENUM::MACHINE_AREA::CLARA_2_VELA )
            ;

    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
            ;

    enum_<scopeStructs::SCOPE_PV_TYPE>("SCOPE_PV_TYPE")
            .value("TR1", scopeStructs::SCOPE_PV_TYPE::TR1 )
            .value("TR2", scopeStructs::SCOPE_PV_TYPE::TR2 )
            .value("TR3", scopeStructs::SCOPE_PV_TYPE::TR3 )
            .value("TR4", scopeStructs::SCOPE_PV_TYPE::TR4 )
            .value("P1",  scopeStructs::SCOPE_PV_TYPE::P1  )
            .value("P2",  scopeStructs::SCOPE_PV_TYPE::P2  )
            .value("P3",  scopeStructs::SCOPE_PV_TYPE::P3  )
            .value("P4",  scopeStructs::SCOPE_PV_TYPE::P4  )
            ;

    enum_<VELA_ENUM::DIAG_TYPE>("DIAG_TYPE")
            .value("WCM",     VELA_ENUM::DIAG_TYPE::WCM       )
            .value("ICT1",    VELA_ENUM::DIAG_TYPE::ICT2      )
            .value("ICT2",    VELA_ENUM::DIAG_TYPE::ICT2      )
            .value("FCUP",    VELA_ENUM::DIAG_TYPE::FCUP      )
            .value("ED_FCUP", VELA_ENUM::DIAG_TYPE::ED_FCUP   )
            ;

    boost::python::class_<scopeStructs::scopeNumObject,boost::noncopyable>
        ("scopeNumObject", boost::python::no_init)
        .def_readonly("isMonitoringMap", &scopeStructs::scopeNumObject::isMonitoringMap)
        .def_readonly("name",            &scopeStructs::scopeNumObject::name           )
        .def_readonly("p1",              &scopeStructs::scopeNumObject::p1             )
        .def_readonly("p2",              &scopeStructs::scopeNumObject::p2             )
        .def_readonly("p3",              &scopeStructs::scopeNumObject::p3             )
        .def_readonly("p4",              &scopeStructs::scopeNumObject::p4             )
        .def_readonly("p1TimeStamp",     &scopeStructs::scopeNumObject::p1TimeStamp    )
        .def_readonly("p2TimeStamp",     &scopeStructs::scopeNumObject::p2TimeStamp    )
        .def_readonly("p3TimeStamp",     &scopeStructs::scopeNumObject::p3TimeStamp    )
        .def_readonly("p4TimeStamp",     &scopeStructs::scopeNumObject::p4TimeStamp    )
        .def_readonly("p1TimeStamps",    &scopeStructs::scopeNumObject::p1TimeStamps   )
        .def_readonly("p2TimeStamps",    &scopeStructs::scopeNumObject::p2TimeStamps   )
        .def_readonly("p3TimeStamps",    &scopeStructs::scopeNumObject::p3TimeStamps   )
        .def_readonly("p4TimeStamps",    &scopeStructs::scopeNumObject::p4TimeStamps   )
        .def_readonly("p1Vec",           &scopeStructs::scopeNumObject::p1Vec          )
        .def_readonly("p2Vec",           &scopeStructs::scopeNumObject::p2Vec          )
        .def_readonly("p3Vec",           &scopeStructs::scopeNumObject::p3Vec          )
        .def_readonly("p4Vec",           &scopeStructs::scopeNumObject::p4Vec          )
        .def_readonly("numData",         &scopeStructs::scopeNumObject::numData        )
        .def_readonly("numTimeStamps",   &scopeStructs::scopeNumObject::numTimeStamps  )
        .def_readonly("shotCounts",      &scopeStructs::scopeNumObject::shotCounts     )
        ;

    boost::python::class_<scopeStructs::scopeTraceData,boost::noncopyable>
        ("scopeTraceData", boost::python::no_init)
        .def_readonly("isMonitoringMap", &scopeStructs::scopeTraceData::isMonitoringMap)
        .def_readonly("name",            &scopeStructs::scopeTraceData::name           )
        .def_readonly("tr1TraceData",    &scopeStructs::scopeTraceData::tr1TraceData   )
        .def_readonly("tr2TraceData",    &scopeStructs::scopeTraceData::tr1TraceData   )
        .def_readonly("tr3TraceData",    &scopeStructs::scopeTraceData::tr1TraceData   )
        .def_readonly("tr4TraceData",    &scopeStructs::scopeTraceData::tr1TraceData   )
        .def_readonly("traceData",       &scopeStructs::scopeTraceData::traceData      )
        .def_readonly("tr1TimeStamps",   &scopeStructs::scopeTraceData::tr1TimeStamps  )
        .def_readonly("tr2TimeStamps",   &scopeStructs::scopeTraceData::tr2TimeStamps  )
        .def_readonly("tr3TimeStamps",   &scopeStructs::scopeTraceData::tr3TimeStamps  )
        .def_readonly("tr4TimeStamps",   &scopeStructs::scopeTraceData::tr4TimeStamps  )
        .def_readonly("shotCounts",      &scopeStructs::scopeTraceData::shotCounts     )
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


	boost::python::class_<scopeController, boost::python::bases<controller>, boost::noncopyable>
            ("scopeController","scopeController Doc String",boost::python::no_init)
//            .def(boost::python::init<const std::string, const std::string, optional<  const bool, const bool > >())
//            .def(boost::python::init< optional<const bool, const bool, const bool> >())
//            .def("getScopeStateDefinition",         &scopeController::getScopeStateDefinition                   )
//            .def("getILockStatesDefinition",        &scopeController::getILockStatesDefinition                  )
            .def("get_CA_PEND_IO_TIMEOUT",          &scopeController::get_CA_PEND_IO_TIMEOUT                    )
            .def("set_CA_PEND_IO_TIMEOUT",          &scopeController::set_CA_PEND_IO_TIMEOUT                    )
//            .def("getScopeState",                   &velaChargeScopeController::getScopeState_Py                          )
//            .def("getILockStates",                  &velaChargeScopeController::getILockStates_Py                         )
//            .def("hasNoTrig",                       &velaChargeScopeController::hasNoTrig_Py, boost::python::args("name") )
//            .def("hasTrig",                         &velaChargeScopeController::hasTrig_Py, boost::python::args("name")   )
            .def("getScopeTraceDataStruct",         &scopeController::getScopeTraceDataStruct, return_value_policy<reference_existing_object>())
            .def("getScopeNumDataStruct",           &scopeController::getScopeNumDataStruct, return_value_policy<reference_existing_object>()  )
            .def("isMonitoringScopeTrace",          &scopeController::isMonitoringScopeTrace                 )
            .def("isMonitoringScopeNum",            &scopeController::isMonitoringScopeNum                   )
            .def("isNotMonitoringScopeTrace",       &scopeController::isNotMonitoringScopeTrace              )
            .def("isNotMonitoringScopeNum",         &scopeController::isNotMonitoringScopeNum                )
            .def("getScopeNums",                    &scopeController::getScopeNums                           )
            .def("getScopeP1Vec",                   &scopeController::getScopeP1Vec                          )
            .def("getScopeP2Vec",                   &scopeController::getScopeP2Vec                          )
            .def("getScopeP3Vec",                   &scopeController::getScopeP3Vec                          )
            .def("getScopeP4Vec",                   &scopeController::getScopeP4Vec                          )
            .def("getScopeP1",                      &scopeController::getScopeP1                             )
            .def("getScopeP2",                      &scopeController::getScopeP2                             )
            .def("getScopeP3",                      &scopeController::getScopeP3                             )
            .def("getScopeP4",                      &scopeController::getScopeP4                             )
            .def("getWCMQ",                         &scopeController::getWCMQ                                )
            .def("getICT1Q",                        &scopeController::getICT1Q                               )
            .def("getICT2Q",                        &scopeController::getICT2Q                               )
            .def("getFCUPQ",                        &scopeController::getFCUPQ                               )
            .def("getEDFCUPQ",                      &scopeController::getEDFCUPQ                             )
            .def("getScopeTraces",                  &scopeController::getScopeTraces                         )
            .def("getMinOfTraces",                  &scopeController::getMinOfTraces                         )
            .def("getMaxOfTraces",                  &scopeController::getMaxOfTraces                         )
            .def("getAreaUnderTraces",              &scopeController::getAreaUnderTraces                     )
            .def("getTimeStamps",                   &scopeController::getTimeStamps                          )
            .def("getStrTimeStamps",                &scopeController::getStrTimeStamps                       )
            .def("monitorNumsForNShots",            &scopeController::monitorNumsForNShots                   )
            .def("monitorTracesForNShots",          &scopeController::monitorTracesForNShots                 )
            .def("getScopeNames",                   &scopeController::getScopeNames                          )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &scopeController::debugMessagesOff                       )
            .def("debugMessagesOn",                 &scopeController::debugMessagesOn                        )
            .def("messagesOff",                     &scopeController::messagesOff                            )
            .def("messagesOn",                      &scopeController::messagesOn                             )
            .def("silence",                         &scopeController::silence                                )
            .def("verbose",                         &scopeController::verbose                                )
		;

    boost::python::class_<VCscopes,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_Scope_Controller",   &VCscopes::virtual_VELA_INJ_Scope_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Scope_Controller",   &VCscopes::offline_VELA_INJ_Scope_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Scope_Controller",  &VCscopes::physical_VELA_INJ_Scope_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_INJ_Scope_Controller",  &VCscopes::virtual_CLARA_INJ_Scope_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_Scope_Controller",  &VCscopes::offline_CLARA_INJ_Scope_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_Scope_Controller", &VCscopes::physical_CLARA_INJ_Scope_Controller, return_value_policy<reference_existing_object>())
        .def("getScopeController",                  &VCscopes::getScopeController, return_value_policy<reference_existing_object>())
        ;

};

#endif // BUILD_DLL

#endif // velaChrageScopeController_H

