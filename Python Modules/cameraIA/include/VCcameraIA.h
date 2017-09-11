#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>

class VCcameraIA
{
    public:
        VCcameraIA();
        ~VCcameraIA();
        cameraIAController& virtual_VELA_Camera_IA_Controller();
        cameraIAController& offline_VELA_Camera_IA_Controller();
        cameraIAController& physical_VELA_Camera_IA_Controller();
        cameraIAController& virtual_CLARA_Camera_IA_Controller();
        cameraIAController& offline_CLARA_Camera_IA_Controller();
        cameraIAController& physical_CLARA_Camera_IA_Controller();
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();
    protected:

    private:
        cameraIAController * virtual_Camera_IA_Controller_Obj;
        cameraIAController * offline_Camera_IA_Controller_Obj;
        cameraIAController * physical_Camera_IA_Controller_Obj;
        cameraIAController& getController(cameraIAController * cont,const std::string& conf,
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

using namespace cameraStructs;

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_CameraIA_Control )
{
    docstring_options doc_options(true);
    doc_options.disable_cpp_signatures();
    //doc_options.disable_py_signatures();

    ///Expose Vectors
    // if not used don't inlcude
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
    boost::python::class_<cameraStructs::cameraObject,boost::noncopyable>
        ("cameraObject","cameraObject Doc String", boost::python::no_init)
        .def_readonly("name",
                      &cameraStructs::cameraObject::name,
                      "Name of the camera (defined in the config file).")
        .def_readonly("pvRoot",
                      &cameraStructs::cameraObject::pvRoot,
                      "Camera's PV preffix (defined in the config file).")
        .def_readonly("screenName",
                      &cameraStructs::cameraObject::screenName,
                      "Name of screen associated Camera (defined in config file).")
        .def_readonly("state",
                      &cameraStructs::cameraObject::state,
                      "The state indicating whether Camera is 'reachable', i.e. power is on. ")
        .def_readonly("x",
                      &cameraStructs::cameraObject::IA::x,
                      "Horizontal position of beam's centroid in millemetres.")
        .def_readonly("y",
                      &cameraStructs::cameraObject::IA::y,
                      "Vertical position of beam's centroid in millemetres.")
        .def_readonly("sigmaX",
                      &cameraStructs::cameraObject::IA::sigmaX,
                      "Horizontal sigma of beam in millemetres.")
        .def_readonly("sigmaY",
                      &cameraStructs::cameraObject::IA::sigmaY,
                      "Vertical sigma of beam in millemetres.")
        .def_readonly("covXY",
                      &cameraStructs::cameraObject::IA::covXY,
                      "Covariance of beam in millemetres squared.")
        .def_readonly("xPix",
                      &cameraStructs::cameraObject::IA::xPix,
                      "Horizontal position of beam's centroid in pixels.")
        .def_readonly("yPix",
                      &cameraStructs::cameraObject::IA::yPix,
                      "Vertical position of beam's centroid in pixels.")
        .def_readonly("xSigmaPix",
                      &cameraStructs::cameraObject::IA::xSigmaPix,
                      "Horizontal sigma of beam in pixels.")
        .def_readonly("ySigmaPix",
                      &cameraStructs::cameraObject::IA::ySigmaPix,
                      "Vertical sigma of beam in pixels.")
        .def_readonly("xyCovPix",
                      &cameraStructs::cameraObject::IA::xyCovPix,
                      "Covariance of beam in pixels squared.")
        .def_readonly("xCenterPix",
                      &cameraStructs::cameraObject::IA::xCenterPix,
                      "Horizontal calibrated center of pipe (0 of ideal trajectory) in pixels.")
        .def_readonly("yCenterPix",
                      &cameraStructs::cameraObject::IA::yCenterPix,
                      "Vertical calibrated center of pipe (0 of ideal trajectory) in pixels.")
        .def_readonly("xRad",
                      &cameraStructs::cameraObject::IA::xRad,
                      "Horizontal radius of mask used in image analysis in pixels.")
        .def_readonly("yRad",
                      &cameraStructs::cameraObject::IA::yRad,
                      "Vertical radius of mask used in image analysis in pixels.")
        .def_readonly("bitDepth",
                      &cameraStructs::cameraObject::IA::bitDepth,
                      "Bit depth of image. ")
        .def_readonly("imageHeight",
                      &cameraStructs::cameraObject::IA::imageHeight,
                      "Vertical length of full image in pixels.")
        .def_readonly("imageWidth",
                      &cameraStructs::cameraObject::IA::imageWidth,
                      "Horizontal length of full image in pixels.")
        .def_readonly("pix2mm",
                      &cameraStructs::cameraObject::IA::pix2mm,
                      "Conversion factor for convert pixel value to mm. (mm = pix2mm*pix)")
        ;
    boost::python::class_<cameraIAController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraIAController","cameraIAController Doc String",boost::python::no_init)
        .def("getILockStates",           &cameraIAController::getILockStates          )
        .def("getILockStatesDefinition", &cameraIAController::getILockStatesDefinition)
        .def("get_CA_PEND_IO_TIMEOUT",   &cameraIAController::get_CA_PEND_IO_TIMEOUT  )
        .def("set_CA_PEND_IO_TIMEOUT",   &cameraIAController::set_CA_PEND_IO_TIMEOUT  )

        .def("isON",
             &cameraIAController::isON,
            (arg("name")),
             "Returns True if camera 'name' is ON.")
        .def("isOFF",
             &cameraIAController::isOFF,
             (arg("name")),
             "Returns True if camera 'name' is OFF.")
        .def("isAcquiring",
             &cameraIAController::isAcquiring,
             (arg("name")),
             "Returns True if camera 'name' is acquiring.")
        .def("isNotAcquiring",
             &cameraIAController::isNotAcquiring,
             (arg("name")),
             "Returns True if camera 'name' is not acquiring.")
        .def("selectedCamera",
             &cameraIAController::selectedCamera,
             "Returns the name (string) of the current selected camera.")
        .def("setCamera",
             &cameraIAController::setCamera,
             (arg("name")),
             "Returns True if camera 'name' is set as selected camera.")
        .def("startAcquiring",
             &cameraIAController::startAcquiring,
             "Sets Selected camera acquiring and returns True if successful.")
        .def("stopAcquiring",
             &cameraIAController::stopAcquiring,
             "Stops Selected camera acquiring and returns True if succesful.")
        .def("startVCAcquiring",
             &cameraIAController::startVCAcquiring,
             "Sets VC Camera acquiring and returns True if successful.")
        .def("stopVCAcquiring",
             &cameraIAController::stopVCAcquiring,
             "Stops VC Camera acquiring and returns True if successful.")

        //Functions yet to make
        .def("setBackground",
             &cameraIAController::setBackground,
             "Returns True if selected camera copied the current Array Data "+
             "to the background PV array data in EPICS.")
        .def("getCamIAObjConstRef",
             &cameraIAController::getCamIAObjConstRef,
             return_value_policy<reference_existing_object>(),
             (arg("name")),
             "Returns a reference to camera 'name'.")
        .def("getSelectedIARef",
             &cameraIAController::getSelectedIARef,
             return_value_policy<reference_existing_object>(),
             "Returns a reference to selected camera.")
        .def("getVCIARef",
             &cameraIAController::getVCIARef,
             return_value_policy<reference_existing_object>(),
             "Returns a reference to VC camera.")
        ;
    boost::python::class_<VCcameraIA,boost::noncopyable>("init")
        .def("virtual_VELA_Camera_IA_Controller",     &VCcameraIA::virtual_VELA_Camera_IA_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_Camera_IA_Controller",     &VCcameraIA::offline_VELA_Camera_IA_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_Camera_IA_Controller",   &VCcameraIA::physical_VELA_Camera_IA_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_Camera_IA_Controller",   &VCcameraIA::virtual_CLARA_Camera_IA_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_IA_Controller",   &VCcameraIA::offline_CLARA_Camera_IA_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_IA_Controller", &VCcameraIA::physical_CLARA_Camera_IA_Controller, return_value_policy<reference_existing_object>())
        .def("setQuiet",              &VCcameraIA::setQuiet   )
        .def("setVerbose",            &VCcameraIA::setVerbose )
        .def("setMessage",            &VCcameraIA::setMessage )
        .def("setDebugMessage",  &VCcameraIA::setDebugMessage )
        ;
}


#endif // VC_CAMERA_DAQ_H
