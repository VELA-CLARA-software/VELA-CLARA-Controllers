//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
#ifndef VC_CAMERAS_IA_H
#define VC_CAMERAS_IA_H
//tp
#include "cameraIAController.h"
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

using namespace cameraStructs;

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


using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Camera_IA_Control )
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

    ///Expose Enums
    enum_<CAM_STATE>("CAM_STATE","Enum to interpet the power state of camera.")
        .value("CAM_ON",            CAM_STATE::CAM_ON)
        .value("CAM_OFF",           CAM_STATE::CAM_OFF)
        .value("CAM_ERROR",         CAM_STATE::CAM_ERROR)
        ;
    enum_<ACQUIRE_STATE>("ACQUIRE_STATE","Enum to interpet the acquistion state of camera.")
        .value("NOT_ACQUIRING",     ACQUIRE_STATE::NOT_ACQUIRING)
        .value("ACQUIRING",         ACQUIRE_STATE::ACQUIRING)
        .value("ACQUIRING_ERROR",   ACQUIRE_STATE::ACQUIRING_ERROR)
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
        .def_readonly("acquireState",
                      &cameraStructs::cameraObject::acquireState,
                      "Horizontal position of beam's centroid in millemetres.")
        .def_readonly("IA",
                      &cameraStructs::cameraObject::IA,
                      "Object (cameraIAObject) containing all the Image Anaylsis data.")
        ;
    boost::python::class_<cameraStructs::cameraIAObject,boost::noncopyable>
        ("cameraIAObject","cameraIAObject Doc String", boost::python::no_init)
        .def_readonly("x",
                      &cameraStructs::cameraIAObject::x,
                      "Horizontal position of beam's centroid in millemetres.")
        .def_readonly("y",
                      &cameraStructs::cameraIAObject::y,
                      "Vertical position of beam's centroid in millemetres.")
        .def_readonly("sigmaX",
                      &cameraStructs::cameraIAObject::sigmaX,
                      "Horizontal sigma of beam in millemetres.")
        .def_readonly("sigmaY",
                      &cameraStructs::cameraIAObject::sigmaY,
                      "Vertical sigma of beam in millemetres.")
        .def_readonly("covXY",
                      &cameraStructs::cameraIAObject::covXY,
                      "Covariance of beam in millemetres squared.")
        .def_readonly("xPix",
                      &cameraStructs::cameraIAObject::xPix,
                      "Horizontal position of beam's centroid in pixels.")
        .def_readonly("yPix",
                      &cameraStructs::cameraIAObject::yPix,
                      "Vertical position of beam's centroid in pixels.")
        .def_readonly("xSigmaPix",
                      &cameraStructs::cameraIAObject::sigmaXPix,
                      "Horizontal sigma of beam in pixels.")
        .def_readonly("ySigmaPix",
                      &cameraStructs::cameraIAObject::sigmaYPix,
                      "Vertical sigma of beam in pixels.")
        .def_readonly("xyCovPix",
                      &cameraStructs::cameraIAObject::covXYPix,
                      "Covariance of beam in pixels squared.")
        .def_readonly("xCenterPix",
                      &cameraStructs::cameraIAObject::xCenterPix,
                      "Horizontal calibrated center of pipe (0 of ideal trajectory) in pixels.")
        .def_readonly("yCenterPix",
                      &cameraStructs::cameraIAObject::yCenterPix,
                      "Vertical calibrated center of pipe (0 of ideal trajectory) in pixels.")
        .def_readonly("xRad",
                      &cameraStructs::cameraIAObject::xRad,
                      "Horizontal radius of mask used in image analysis in pixels.")
        .def_readonly("yRad",
                      &cameraStructs::cameraIAObject::yRad,
                      "Vertical radius of mask used in image analysis in pixels.")
        .def_readonly("bitDepth",
                      &cameraStructs::cameraIAObject::bitDepth,
                      "Bit depth of image. ")
        .def_readonly("imageHeight",
                      &cameraStructs::cameraIAObject::imageHeight,
                      "Vertical length of full image in pixels.")
        .def_readonly("imageWidth",
                      &cameraStructs::cameraIAObject::imageWidth,
                      "Horizontal length of full image in pixels.")
        .def_readonly("pix2mm",
                      &cameraStructs::cameraIAObject::pix2mm,
                      "Conversion factor for convert pixel value to mm. (mm = pix2mm*pix)")
        ;
   class_<cameraIAController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraIAController","cameraIAController Doc String",no_init)
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
             "Returns True if selected camera copied the current Array Data to the background PV array data in EPICS.")
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
   class_<VCcameraIA,boost::noncopyable>("init")
        .def("virtual_VELA_Camera_IA_Controller",
             &VCcameraIA::virtual_VELA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_Camera_IA_Controller",
             &VCcameraIA::offline_VELA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_Camera_IA_Controller",
             &VCcameraIA::physical_VELA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_Camera_IA_Controller",
             &VCcameraIA::virtual_CLARA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_IA_Controller",
             &VCcameraIA::offline_CLARA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_IA_Controller",
             &VCcameraIA::physical_CLARA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",              &VCcameraIA::setQuiet   )
        .def("setVerbose",            &VCcameraIA::setVerbose )
        .def("setMessage",            &VCcameraIA::setMessage )
        .def("setDebugMessage",  &VCcameraIA::setDebugMessage )
        ;
}


#endif // VC_CAMERAS_IA_H
