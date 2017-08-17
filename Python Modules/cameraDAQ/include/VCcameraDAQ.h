#ifndef VC_CAMERAS_DAQ_H
#define VC_CAMERAS_DAQ_H
//tp
#include "cameraDAQController.h"
//boost
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

///Top Class///
class VCcameraDAQ
{
    public:
        VCcameraDAQ();
        ~VCcameraDAQ();
        // no need atm VELA system is completely different
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
///Expose Relevant Classes, Vectors and Enums to Python///
using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Camera_DAQ_Control )
{
    docstring_options doc_options(false);
    doc_options.disable_cpp_signatures();

    ///Expose Vectors
    // if not used don't inlcude
    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>())
        ;
    class_<std::vector<double>>("std_vector_double")
        .def( vector_indexing_suite< std::vector<double>>())
        ;
    ///Expose Enums
    enum_<cameraStructs::CAM_STATE>("CAM_STATE","CAM_STATE Doc String")
        .value("CAM_ON",cameraStructs::CAM_STATE::CAM_ON)
        .value("CAM_OFF",cameraStructs::CAM_STATE::CAM_OFF)
        .value("CAM_ERROR",cameraStructs::CAM_STATE::CAM_ERROR)
        ;
    enum_<cameraStructs::AQUIRE_STATE>("AQUIRE_STATE","AQUIRE_STATE Doc String")
        .value("NOT_ACQUIRING", cameraStructs::AQUIRE_STATE::NOT_ACQUIRING)
        .value("ACQUIRING",cameraStructs::AQUIRE_STATE::ACQUIRING)
        .value("AQUIRING_ERROR",cameraStructs::AQUIRE_STATE::AQUIRING_ERROR)
        ;
    enum_<cameraStructs::WRITE_CHECK>("WRITE_CHECK","WRITE_CHECK Doc String")
        .value("WRITE_OK",      cameraStructs::WRITE_CHECK::WRITE_OK)
        .value("WRITE_CHECK_ERROR",   cameraStructs::WRITE_CHECK::WRITE_CHECK_ERROR)
        ;
    ///Expose Classes
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
        .def_readonly("aquireState",        &cameraStructs::cameraDAQObject::aquireState)
        .def_readonly("writeState",         &cameraStructs::cameraDAQObject::writeState)
        .def_readonly("shotsTaken",         &cameraStructs::cameraDAQObject::shotsTaken)
        .def_readonly("numberOfShots",      &cameraStructs::cameraDAQObject::numberOfShots)
        .def_readonly("frequency",          &cameraStructs::cameraDAQObject::frequency)
        .def_readonly("exposureTime",       &cameraStructs::cameraDAQObject::exposureTime)
        ;
    boost::python::class_<cameraDAQController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraDAQController","cameraDAQController Doc String",boost::python::no_init)
        .def("get_CA_PEND_IO_TIMEOUT",      &cameraDAQController::get_CA_PEND_IO_TIMEOUT  )
        .def("set_CA_PEND_IO_TIMEOUT",      &cameraDAQController::set_CA_PEND_IO_TIMEOUT  )
        .def("collect",                     &cameraDAQController::collect)
        .def("save",                        &cameraDAQController::save)
        .def("collectAndSave",              &cameraDAQController::collectAndSave)
        .def("killCollectAndSave",          &cameraDAQController::killCollectAndSave)
        .def("getCamDAQObjConstRef",        &cameraDAQController::getCamDAQObjConstRef, return_value_policy<reference_existing_object>())
        .def("getSelectedDAQRef",           &cameraDAQController::getSelectedDAQRef,    return_value_policy<reference_existing_object>())
        .def("getVCDAQRef",                 &cameraDAQController::getVCDAQRef,          return_value_policy<reference_existing_object>())
        .def("isON",                        &cameraDAQController::isON,(arg("name")),"Returns True if Camera 'name' is ON")
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
        .def("virtual_VELA_Camera_DAQ_Controller",      &VCcameraDAQ::virtual_VELA_Camera_DAQ_Controller,   return_value_policy<reference_existing_object>())
        .def("offline_VELA_Camera_DAQ_Controller",      &VCcameraDAQ::offline_VELA_Camera_DAQ_Controller,   return_value_policy<reference_existing_object>())
        .def("physical_VELA_Camera_DAQ_Controller",     &VCcameraDAQ::physical_VELA_Camera_DAQ_Controller,  return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_Camera_DAQ_Controller",     &VCcameraDAQ::virtual_CLARA_Camera_DAQ_Controller,  return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_DAQ_Controller",     &VCcameraDAQ::offline_CLARA_Camera_DAQ_Controller,  return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_DAQ_Controller",    &VCcameraDAQ::physical_CLARA_Camera_DAQ_Controller, return_value_policy<reference_existing_object>())
        .def("setQuiet",                                &VCcameraDAQ::setQuiet   )
        .def("setVerbose",                              &VCcameraDAQ::setVerbose )
        .def("setMessage",                              &VCcameraDAQ::setMessage )
        .def("setDebugMessage",                         &VCcameraDAQ::setDebugMessage )
        ;
}
#endif // VC_CAMERA_DAQ_H
