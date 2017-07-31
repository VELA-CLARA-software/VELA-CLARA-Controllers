#ifndef VC_CAMERAS_DAQ_H
#define VC_CAMERAS_DAQ_H
//
#include "cameraDAQController.h"
//
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


class VCcameraDAQ
{
    public:
        VCcameraDAQ();
        ~VCcameraDAQ();
        cameraDAQController& virtual_VELA_Camera_DAQ_Controller();
        cameraDAQController& offline_VELA_Camera_DAQ_Controller();
        cameraDAQController& physical_VELA_Camera_DAQ_Controller();
        cameraDAQController& virtual_CLARA_Camera_DAQ_Controller();
        cameraDAQController& offline_CLARA_Camera_DAQ_Controller();
        cameraDAQController& physical_CLARA_Camera_DAQ_Controller();
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();
    protected:

    private:
        cameraDAQController * virtual_Camera_DAQ_Controller_Obj;
        cameraDAQController * offline_Camera_DAQ_Controller_Obj;
        cameraDAQController * physical_Camera_DAQ_Controller_Obj;
        cameraDAQController& getController(cameraDAQController * cont,const std::string& conf,
                                        const std::string & name,
                                        const bool shouldVM,
                                        const bool shouldEPICS,
                                        const VELA_ENUM::MACHINE_AREA myMachineArea );
        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const VELA_ENUM::MACHINE_AREA VELA_INJ,VELA_BA1,VELA_BA2,CLARA_PH1,UNKNOWN_AREA;
};
typedef int inter;
typedef const int cinter;
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
typedef boost::python::list & cbpl;


bool(cameraDAQController::*collectAndSave_1)(cinter&) = &cameraDAQController::collectAndSave;
bool(cameraDAQController::*collectAndSave_2)(cinter&, cstr&) = &cameraDAQController::collectAndSave;
//cbpl (cameraDAQController::*getSelectedDAQRef_1)() = &cameraDAQController::getSelectedDAQRef();

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Camera_DAQ_Control )
{
    docstring_options doc_options(false);
    doc_options.disable_cpp_signatures();

    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>())
        ;
    class_<std::vector<double>>("std_vector_double")
        .def( vector_indexing_suite< std::vector<double>>())
        ;
    /// and enums, remember we have a enum to string python dictionary macro too!
    enum_<cameraStructs::CAM_STATE>("CAM_STATE","CAM_STATE Doc String")
        .value("CAM_ON",   cameraStructs::CAM_STATE::CAM_ON   )
        .value("CAM_OFF",    cameraStructs::CAM_STATE::CAM_OFF    )
        .value("CAM_ACQUIRING",cameraStructs::CAM_STATE::CAM_ACQUIRING)
        .value("CAM_NOT_ACQUIRING",cameraStructs::CAM_STATE::CAM_NOT_ACQUIRING)
        .value("CAM_ERROR", cameraStructs::CAM_STATE::CAM_ERROR )
        ;

    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller","controller Doc String", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;
    boost::python::class_<cameraStructs::cameraDAQObject,boost::noncopyable>
        ("cameraDAQObject","cameraDAQObject Doc String", boost::python::no_init)
        .def_readonly("name",               &cameraStructs::cameraDAQObject::name)
        .def_readonly("pvRoot",             &cameraStructs::cameraDAQObject::pvRoot)
        .def_readonly("screenPV",           &cameraStructs::cameraDAQObject::screenPV)
        .def_readonly("state",              &cameraStructs::cameraDAQObject::state)
        .def_readonly("shotsTaken",         &cameraStructs::cameraDAQObject::shotsTaken)
        .def_readonly("numberOfShots",      &cameraStructs::cameraDAQObject::numberOfShots)
        .def_readonly("frequency",          &cameraStructs::cameraDAQObject::frequency)
        .def_readonly("exposureTime",       &cameraStructs::cameraDAQObject::exposureTime)
        //.def_readonly("machineArea",        &cameraStructs::cameraDAQObject::machineArea)
        // More stuff to be added here?
        ;
    boost::python::class_<cameraDAQController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraDAQController","cameraDAQController Doc String",boost::python::no_init)
        //.def("getILockStates",           &cameraDAQController::getILockStates          )
        //.def("getILockStatesDefinition", &cameraDAQController::getILockStatesDefinition)
        .def("get_CA_PEND_IO_TIMEOUT",      &cameraDAQController::get_CA_PEND_IO_TIMEOUT  )
        .def("set_CA_PEND_IO_TIMEOUT",      &cameraDAQController::set_CA_PEND_IO_TIMEOUT  )
        .def("collectAndSave",              collectAndSave_1  )
        .def("collectAndSave",              collectAndSave_2  )
        .def("killCollectAndSave",          &cameraDAQController::killCollectAndSave  )
        .def("getCamDAQObjConstRef",        &cameraDAQController::getCamDAQObjConstRef,return_value_policy<reference_existing_object>())
        .def("getSelectedDAQRef",           &cameraDAQController::getSelectedDAQRef,return_value_policy<reference_existing_object>())
        .def("getSelectedDAQPtr",           &cameraDAQController::getSelectedDAQPtr,return_internal_reference<>())//return_value_policy<reference_existing_object>())
        .def("getVCDAQRef",                 &cameraDAQController::getVCDAQRef,return_value_policy<reference_existing_object>())
        .def("isON",                        &cameraDAQController::isON  )
        .def("isOFF",                       &cameraDAQController::isOFF  )
        .def("isAquiring",                  &cameraDAQController::isAquiring  )
        .def("isNotAquiring",               &cameraDAQController::isNotAquiring  )
        .def("selectedCamera",              &cameraDAQController::selectedCamera  )
        .def("setCamera",                   &cameraDAQController::setCamera  )
        .def("startAquiring",               &cameraDAQController::startAquiring  )
        .def("stopAquiring",                &cameraDAQController::stopAquiring  )
        .def("startVCAquiring",             &cameraDAQController::startVCAquiring  )
        .def("stopVCAquiring",              &cameraDAQController::stopVCAquiring  )

        ;
    boost::python::class_<VCcameraDAQ,boost::noncopyable>("init")
        .def("virtual_VELA_Camera_DAQ_Controller",     &VCcameraDAQ::virtual_VELA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_Camera_DAQ_Controller",     &VCcameraDAQ::offline_VELA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_Camera_DAQ_Controller",   &VCcameraDAQ::physical_VELA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_Camera_DAQ_Controller",   &VCcameraDAQ::virtual_CLARA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_DAQ_Controller",   &VCcameraDAQ::offline_CLARA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_DAQ_Controller", &VCcameraDAQ::physical_CLARA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("setQuiet",              &VCcameraDAQ::setQuiet   )
        .def("setVerbose",            &VCcameraDAQ::setVerbose )
        .def("setMessage",            &VCcameraDAQ::setMessage )
        .def("setDebugMessage",  &VCcameraDAQ::setDebugMessage )
        ;
}


#endif // VC_CAMERA_DAQ_H
