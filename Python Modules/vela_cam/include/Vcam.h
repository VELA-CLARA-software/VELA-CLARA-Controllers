#ifndef VCAM_H
#define VCAM_H

#include <string>
// HWC
#include "velaCamController.h"
#include "structs.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
class Vcam
{
    public:
        Vcam();
        virtual ~Vcam();


        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        velaCamController& virtual_VELA_Camera_Controller();
        velaCamController& physical_VELA_Camera_Controller();
        velaCamController& offline_VELA_Camera_Controller();


    protected:

    private:
        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string velaCamConf;
};

using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_Camera_Control)
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();

    /// Things that you want to use in python muct be exposed:
    /// containers
    class_<std::vector<std::string>>("std_vector_string")
            .def(vector_indexing_suite< std::vector<std::string>>() )
            ;
    class_<std::vector<double> >("std_vector_double")
            .def(vector_indexing_suite< std::vector<double>>() )
            ;
    class_<std::vector<int> >("std_vector_int")
            .def(vector_indexing_suite< std::vector<int>>() )
            ;
    class_<std::vector<bool> >("std_vector_bool")
            .def(vector_indexing_suite< std::vector<bool>>() )
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


    enum_<velaCamStructs::CAM_SERVER_STATE>("CAM_SERVER_STATE","CAM_SERVER_STATE: a named integer giving the camera server status")
        .value("OFF",  velaCamStructs::CAM_SERVER_STATE::k_OFF  )
        .value("ON",  velaCamStructs::CAM_SERVER_STATE::k_ON  )
        .value("ERR", velaCamStructs::MACHINE_MODE::k_ERR )
        .value("DUMMY", velaCamStructs::MACHINE_MODE::DUMMY )
        ;

    boost::python::class_<velaCamStructs::camObject,boost::noncopyable>
        ("rfGunProtObject","rfGunProtObject member variables (read access only)", boost::python::no_init)
        .def_readonly("name",               &velaCamStructs::camObject::name,"object name")
        .def_readonly("serverPV",           &rfProtStructs::camObject::serverPV,"object server PV")
        .def_readonly("dataPV",             &rfProtStructs::camObject::dataPV,"object data PV")
        .def_readonly("serverNum",          &rfProtStructs::camObject::serverNum,"object server Number")
        .def_readonly("epicsCamServerState",&rfProtStructs::camObject::epicsCamServerState,"server state")
        .def_readonly("gunProtKeyBitValues",&rfProtStructs::camObject::gunProtKeyBitValues,"key bit values")
        .def_readonly("protType",           &rfProtStructs::camObject::protType,"rf gun protection type ")
        ;
    boost::python::class_<gunProtController, boost::python::bases<controller>, boost::noncopyable>
        ("gunProtController","DOCSTRING", boost::python::no_init)
        .def("resetRaw",         &gunProtController::resetRaw,"Docstring.")
        .def("getData",      &gunProtController::getData,"Docstring")
        .def("addData", &gunProtController::addData,"Docstring")
        ;

}


#endif // VCAM_H
