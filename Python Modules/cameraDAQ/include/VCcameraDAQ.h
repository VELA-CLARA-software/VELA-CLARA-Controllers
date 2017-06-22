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
typedef const boost::python::list & cbpl;

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_CameraDAQ_Control )
{
    docstring_options doc_options(false);
    doc_options.disable_cpp_signatures();

    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>())
        ;
    class_<std::vector<double>>("std_vector_double")
        .def( vector_indexing_suite< std::vector<double>>())
        ;
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init);
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller","controller Doc String", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;
    boost::python::class_<cameraStructs::cameraDAQObject,boost::noncopyable>
        ("cameraDAQObject","cameraDAQObject Doc String", boost::python::no_init)
        .def_readonly("pvRoot",       &cameraStructs::cameraDAQObject::pvRoot)
        // More stuff to be added here...
        ;
    boost::python::class_<cameraController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraController","cameraController Doc String",boost::python::no_init)
        .def("getILockStates",           &cameraController::getILockStates          )
        .def("getILockStatesDefinition", &cameraController::getILockStatesDefinition)
        .def("get_CA_PEND_IO_TIMEOUT",   &cameraController::get_CA_PEND_IO_TIMEOUT  )
        .def("set_CA_PEND_IO_TIMEOUT",   &cameraController::set_CA_PEND_IO_TIMEOUT  )
        .def("cameraName",   &cameraController::cameraName  )
        .def("setCamera",   &cameraController::setCamera  )
        .def("getCamObjConstRef",        &cameraController::getCamObjConstRef,return_value_policy<reference_existing_object>())
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
