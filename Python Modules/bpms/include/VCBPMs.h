#ifndef vela_INJ_BPM_CONTROLLER_H
#define vela_INJ_BPM_CONTROLLER_H

// project
#include "beamPositionMonitorController.h"
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

class VCBPMs// : public beamPositionMonitorController
{
    public:

        /// we have overloaded constructors to specify config-file location
//        velaINJBeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true, const std::string configFileLocation );
        VCBPMs();
        ~VCBPMs();

        beamPositionMonitorController & virtual_VELA_INJ_BPM_Controller();
        beamPositionMonitorController & offline_VELA_INJ_BPM_Controller();
        beamPositionMonitorController & physical_VELA_INJ_BPM_Controller();
        beamPositionMonitorController & virtual_VELA_BA1_BPM_Controller();
        beamPositionMonitorController & offline_VELA_BA1_BPM_Controller();
        beamPositionMonitorController & physical_VELA_BA1_BPM_Controller();
        beamPositionMonitorController & virtual_VELA_BA2_BPM_Controller();
        beamPositionMonitorController & offline_VELA_BA2_BPM_Controller();
        beamPositionMonitorController & physical_VELA_BA2_BPM_Controller();
        beamPositionMonitorController & virtual_CLARA_INJ_BPM_Controller();
        beamPositionMonitorController & offline_CLARA_INJ_BPM_Controller();
        beamPositionMonitorController & physical_CLARA_INJ_BPM_Controller();
        beamPositionMonitorController & virtual_CLARA_2_VELA_BPM_Controller();
        beamPositionMonitorController & offline_CLARA_2_VELA_BPM_Controller();
        beamPositionMonitorController & physical_CLARA_2_VELA_BPM_Controller();
        beamPositionMonitorController & getBPMController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

#ifdef BUILD_DLL

#endif // BUILD_DLL

    protected:
    private:

        bool withEPICS;
        bool withoutEPICS;
        bool withVM;
        bool withoutVM;
        bool showDebugMessages;
        bool dontShowMessages;
        bool showMessages;
        bool dontShowDebugMessages;
        VELA_ENUM::MACHINE_AREA VELA_INJ;
        VELA_ENUM::MACHINE_AREA VELA_BA1;
        VELA_ENUM::MACHINE_AREA VELA_BA2;
        VELA_ENUM::MACHINE_AREA CLARA_INJ;
        VELA_ENUM::MACHINE_AREA CLARA_2_VELA;
        VELA_ENUM::MACHINE_AREA UNKNOWN_AREA;

        beamPositionMonitorController * virtual_VELA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * offline_VELA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * physical_VELA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_VELA_BA1_BPM_Controller_Obj;
        beamPositionMonitorController * offline_VELA_BA1_BPM_Controller_Obj;
        beamPositionMonitorController * physical_VELA_BA1_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_VELA_BA2_BPM_Controller_Obj;
        beamPositionMonitorController * offline_VELA_BA2_BPM_Controller_Obj;
        beamPositionMonitorController * physical_VELA_BA2_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_CLARA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * offline_CLARA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * physical_CLARA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_CLARA_2_VELA_BPM_Controller_Obj;
        beamPositionMonitorController * offline_CLARA_2_VELA_BPM_Controller_Obj;
        beamPositionMonitorController * physical_CLARA_2_VELA_BPM_Controller_Obj;

};

typedef std::vector<std::string> vecString;

class getVecString
{
    public:

    vecString returnVecString()
    {
     return vecString();
    }
    void setVecString(vecString & lis, std::string & name)
    {
        lis.push_back(name);
    }
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

BOOST_PYTHON_MODULE( VELA_CLARA_BPM_Control )
{
    /// Include ALL the enums you want to expose to Python

    class_< std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string > >() )
        ;

    class_< std::vector< std::vector< double > > > ("v2_double")
        .def( vector_indexing_suite< std::vector< std::vector< double > > >())
        ;

    class_< std::vector< double > > ("v_double")
        .def( vector_indexing_suite< std::vector< double > >())
        ;

//    class_< vecString >("vecString")
//        .def(vector_indexing_suite< vecString >() )
//        ;
//
//    class_< getVecString >("getVecString")
//        .def("returnVecString", &getVecString::returnVecString)
//        .def("setVecString",    &getVecString::setVecString)
//        ;

//    enum_<VELA_ENUM::TRIG_STATE>("TRIG_STATE")
//            .value("NOTRIG",      VELA_ENUM::TRIG_STATE::NOTRIG )
//            .value("TRIG",        VELA_ENUM::TRIG_STATE::TRIG   )
//            .value("TRIG_ERROR",  VELA_ENUM::TRIG_STATE::TRIG_ERROR  )
//            ;
//    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
//            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
//            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
//            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
//            ;

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

    boost::python::class_<beamPositionMonitorStructs::bpmDataObject, boost::shared_ptr<beamPositionMonitorStructs::bpmDataObject>, boost::noncopyable>
        ("bpmDataObject", boost::python::no_init)
        .def_readonly("pvRoot",         &beamPositionMonitorStructs::bpmDataObject::pvRoot       )
        .def_readonly("appendingData",  &beamPositionMonitorStructs::bpmDataObject::appendingData)
        .def_readonly("xPV",            &beamPositionMonitorStructs::bpmDataObject::xPV          )
        .def_readonly("yPV",            &beamPositionMonitorStructs::bpmDataObject::yPV          )
        .def_readonly("sa1",            &beamPositionMonitorStructs::bpmDataObject::sa1          )
        .def_readonly("sa2",            &beamPositionMonitorStructs::bpmDataObject::sa2          )
        .def_readonly("ra1",            &beamPositionMonitorStructs::bpmDataObject::ra1          )
        .def_readonly("ra2",            &beamPositionMonitorStructs::bpmDataObject::ra2          )
        .def_readonly("sd1",            &beamPositionMonitorStructs::bpmDataObject::sd1          )
        .def_readonly("sd2",            &beamPositionMonitorStructs::bpmDataObject::sd2          )
        .def_readonly("rd1",            &beamPositionMonitorStructs::bpmDataObject::rd1          )
        .def_readonly("rd2",            &beamPositionMonitorStructs::bpmDataObject::rd2          )
        .def_readonly("q",              &beamPositionMonitorStructs::bpmDataObject::q            )
        ;

    boost::python::class_<beamPositionMonitorStructs::rawDataStruct, boost::shared_ptr<beamPositionMonitorStructs::rawDataStruct>, boost::noncopyable>
        ("rawDataStruct", boost::python::no_init)
        .def_readonly("name",           &beamPositionMonitorStructs::rawDataStruct::name         )
        .def_readonly("appendingData",  &beamPositionMonitorStructs::rawDataStruct::appendingData)
        .def_readonly("p1",             &beamPositionMonitorStructs::rawDataStruct::p1           )
        .def_readonly("p2",             &beamPositionMonitorStructs::rawDataStruct::p2           )
        .def_readonly("pu1",            &beamPositionMonitorStructs::rawDataStruct::pu1          )
        .def_readonly("pu2",            &beamPositionMonitorStructs::rawDataStruct::pu2          )
        .def_readonly("pu3",            &beamPositionMonitorStructs::rawDataStruct::pu3          )
        .def_readonly("pu4",            &beamPositionMonitorStructs::rawDataStruct::pu4          )
        .def_readonly("c1",             &beamPositionMonitorStructs::rawDataStruct::c1           )
        .def_readonly("c2",             &beamPositionMonitorStructs::rawDataStruct::c2           )
        .def_readonly("x",              &beamPositionMonitorStructs::rawDataStruct::x            )
        .def_readonly("y",              &beamPositionMonitorStructs::rawDataStruct::y            )
        .def_readonly("q",              &beamPositionMonitorStructs::rawDataStruct::q            )
        .def_readonly("timeStamps",     &beamPositionMonitorStructs::rawDataStruct::timeStamps   )
        .def_readonly("strTimeStamps",  &beamPositionMonitorStructs::rawDataStruct::strTimeStamps)
        .def_readonly("rawBPMData",     &beamPositionMonitorStructs::rawDataStruct::rawBPMData   )
        ;


	boost::python::class_<beamPositionMonitorController, boost::python::bases<controller>, boost::noncopyable>
            ("beamPositionMonitorController","beamPositionMonitor Doc String",boost::python::no_init)
//            .def(boost::python::init<const std::string, optional<const bool, const bool > >())
//            .def(boost::python::init< optional<const bool, const bool, const bool >>())
            .def("getAllBPMData",                   &beamPositionMonitorController::getAllBPMData, return_value_policy<reference_existing_object>())
            .def("getBPMDataObject",                &beamPositionMonitorController::getBPMDataObject, return_value_policy<reference_existing_object>()            )
//            .def("getBPMStateDefinition",           &beamPositionMonitorController::getBPMStateDefinition                     )
            .def("getILockStatesDefinition",        &beamPositionMonitorController::getILockStatesDefinition    )
            .def("get_CA_PEND_IO_TIMEOUT",          &beamPositionMonitorController::get_CA_PEND_IO_TIMEOUT      )
            .def("set_CA_PEND_IO_TIMEOUT",          &beamPositionMonitorController::set_CA_PEND_IO_TIMEOUT      )
//            .def("getBPMState",                     &velaINJBeamPositionMonitorController::getBPMState_Py                            )
            .def("getILockStates",                  &beamPositionMonitorController::getILockStates              )
//            .def("hasNoTrig",                       &velaINJBeamPositionMonitorController::hasNoTrig_Py, boost::python::args("name") )
//            .def("hasTrig",                         &velaINJBeamPositionMonitorController::hasTrig_Py, boost::python::args("name")   )
            .def("isMonitoringBPMData",             &beamPositionMonitorController::isMonitoringBPMData         )
            .def("isNotMonitoringBPMData",          &beamPositionMonitorController::isNotMonitoringBPMData      )
            .def("getX",                            &beamPositionMonitorController::getX                        )
            .def("getY",                            &beamPositionMonitorController::getY                        )
            .def("getQ",                            &beamPositionMonitorController::getQ                        )
            .def("reCalAttenuation",                &beamPositionMonitorController::reCalAttenuation            )
            .def("getXFromPV",                      &beamPositionMonitorController::getXFromPV                  )
            .def("getYFromPV",                      &beamPositionMonitorController::getYFromPV                  )
            .def("getBPMResolution",                &beamPositionMonitorController::getBPMResolution            )
            .def("getBPMXVec",                      &beamPositionMonitorController::getBPMXVec                  )
            .def("getBPMYVec",                      &beamPositionMonitorController::getBPMYVec                  )
            .def("getBPMQVec",                      &beamPositionMonitorController::getBPMQVec                  )
            .def("getTimeStamps",                   &beamPositionMonitorController::getTimeStamps               )
            .def("getStrTimeStamps",                &beamPositionMonitorController::getStrTimeStamps            )
            .def("getBPMRawData",                   &beamPositionMonitorController::getBPMRawData               )
            .def("getRA1",                          &beamPositionMonitorController::getRA1                      )
            .def("getRA2",                          &beamPositionMonitorController::getRA2                      )
            .def("getRD1",                          &beamPositionMonitorController::getRD1                      )
            .def("getRD2",                          &beamPositionMonitorController::getRD2                      )
            .def("setSA1",                          &beamPositionMonitorController::setSA1                      )
            .def("setSA2",                          &beamPositionMonitorController::setSA2                      )
            .def("setSD1",                          &beamPositionMonitorController::setSD1                      )
            .def("setSD2",                          &beamPositionMonitorController::setSD2                      )
            .def("monitorDataForNShots",            &beamPositionMonitorController::monitorDataForNShots        )
            .def("monitorMultipleDataForNShots",    &beamPositionMonitorController::monitorMultipleDataForNShots)
            .def("getBPMNames",                     &beamPositionMonitorController::getBPMNames                 )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &beamPositionMonitorController::debugMessagesOff            )
            .def("debugMessagesOn",                 &beamPositionMonitorController::debugMessagesOn             )
            .def("messagesOff",                     &beamPositionMonitorController::messagesOff                 )
            .def("messagesOn",                      &beamPositionMonitorController::messagesOn                  )
            .def("silence",                         &beamPositionMonitorController::silence                     )
            .def("verbose",                         &beamPositionMonitorController::verbose                     )
		;

    boost::python::class_<VCBPMs,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_BPM_Controller",  &VCBPMs::virtual_VELA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_BPM_Controller",  &VCBPMs::offline_VELA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_BPM_Controller", &VCBPMs::offline_VELA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_BPM_Controller",  &VCBPMs::virtual_VELA_BA1_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_BPM_Controller",  &VCBPMs::offline_VELA_BA1_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_BPM_Controller", &VCBPMs::offline_VELA_BA1_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_BPM_Controller",  &VCBPMs::virtual_VELA_BA2_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_BPM_Controller",  &VCBPMs::offline_VELA_BA2_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_BPM_Controller", &VCBPMs::offline_VELA_BA2_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_INJ_BPM_Controller",  &VCBPMs::virtual_CLARA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_BPM_Controller",  &VCBPMs::offline_CLARA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_BPM_Controller", &VCBPMs::offline_CLARA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_2_VELA_BPM_Controller",  &VCBPMs::virtual_CLARA_2_VELA_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_2_VELA_BPM_Controller",  &VCBPMs::offline_CLARA_2_VELA_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_2_VELA_BPM_Controller", &VCBPMs::offline_CLARA_2_VELA_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("getBPMController", &VCBPMs::getBPMController, return_value_policy<reference_existing_object>())
        ;
};

#endif // BUILD_DLL

#endif // vela_INJ_BPM_CONTROLLER_H
