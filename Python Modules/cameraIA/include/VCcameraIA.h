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
#include "offlineImageAnalyser.h"
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
    //class_<std::vector<cameraObject>>("std_vector_cameraObject")
     //   .def( vector_indexing_suite< std::vector<cameraObject>>())
      //  ;
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
    enum_<CAPTURE_STATE>("CAPTURE_STATE","Enum to interpet the capturing state of camera.")
        .value("NOT_CAPTURING",     CAPTURE_STATE::NOT_CAPTURING)
        .value("CAPTURING",         CAPTURE_STATE::CAPTURING)
        .value("CAPTURING_ERROR",   CAPTURE_STATE::CAPTURING_ERROR)
        ;
    enum_<WRITE_STATE>("WRITE_STATE","Enum to interpet the saving state of camera.")
        .value("NOT_WRITING",     WRITE_STATE::NOT_WRITING)
        .value("WRITING",         WRITE_STATE::WRITING)
        .value("WRITING_ERROR",   WRITE_STATE::WRITING_ERROR)
        ;
    enum_<WRITE_CHECK>("WRITE_CHECK","Enum to interpet the saving errors of camera.")
        .value("WRITE_OK",          WRITE_CHECK::WRITE_CHECK_OK)
        .value("WRITE_CHECK_ERROR", WRITE_CHECK::WRITE_CHECK_ERROR)
        ;
    ///Expose Classes
    class_<baseObject, boost::noncopyable>("baseObject", no_init)
        ;

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
        .def_readonly("maskXRad",
                      &cameraStructs::cameraIAObject::maskXRad,
                      "Horizontal radius of mask used in image analysis in pixels.")
        .def_readonly("maskYRad",
                      &cameraStructs::cameraIAObject::maskYRad,
                      "Vertical radius of mask used in image analysis in pixels.")
        .def_readonly("bitDepth",
                      &cameraStructs::cameraIAObject::bitDepth,
                      "Bit depth of image. ")
        /*.def_readonly("imageHeight",
                      &cameraStructs::cameraIAObject::imageHeight,
                      "Vertical length of full image in pixels.")
        .def_readonly("imageWidth",
                      &cameraStructs::cameraIAObject::imageWidth,
                      "Horizontal length of full image in pixels.")*/
        .def_readonly("summedIntensity",
                      &cameraStructs::cameraIAObject::summedIntensity,
                      "Summed pixel values of image. ")
        .def_readonly("pix2mm",
                      &cameraStructs::cameraIAObject::pix2mm,
                      "Conversion factor for convert pixel value to mm. (mm = pix2mm*pix)")
        ;

    boost::python::class_<cameraStructs::cameraOfflineIAObject,boost::noncopyable>
        ("cameraOfflineIAObject","cameraOfflineIAObject Doc String", boost::python::no_init)
        .def_readonly("rawData",
        &cameraStructs::cameraOfflineIAObject::rawData,
        "1")
        .def_readonly("xProjection",
        &cameraStructs::cameraOfflineIAObject::xProjection,
        "1")
        .def_readonly("imageName",
        &cameraStructs::cameraOfflineIAObject::imageName,
        "1")
        .def_readonly("bkgrndName",
        &cameraStructs::cameraOfflineIAObject::bkgrndName,
        "1")
        .def_readonly("yProjection",
        &cameraStructs::cameraOfflineIAObject::yProjection,
        "2")
        .def_readonly("maskXProjection",
        &cameraStructs::cameraOfflineIAObject::maskXProjection,
        "3")
        .def_readonly("maskYProjection",
        &cameraStructs::cameraOfflineIAObject::maskYProjection,
        "4")
        .def_readonly("dataSize",
        &cameraStructs::cameraOfflineIAObject::dataSize,
        "5")
        .def_readonly("imageHeight",
        &cameraStructs::cameraOfflineIAObject::imageHeight,
        "6")
        .def_readonly("imageWidth",
        &cameraStructs::cameraOfflineIAObject::imageWidth,
        "7")
        .def_readonly("rotation",
        &cameraStructs::cameraOfflineIAObject::rotation,
        "8")
        .def_readonly("x0",
        &cameraStructs::cameraOfflineIAObject::x0,
        "9")
        .def_readonly("y0",
        &cameraStructs::cameraOfflineIAObject::y0,
        "1")
        .def_readonly("xRad",
        &cameraStructs::cameraOfflineIAObject::xRad,
        "2")
        .def_readonly("yRad",
        &cameraStructs::cameraOfflineIAObject::yRad,
        "4")
        .def_readonly("xBVN",
        &cameraStructs::cameraOfflineIAObject::xBVN,
        "5")
        .def_readonly("yBVN",
        &cameraStructs::cameraOfflineIAObject::yBVN,
        "6")
        .def_readonly("sxBVN",
        &cameraStructs::cameraOfflineIAObject::sxBVN,
        "7")
        .def_readonly("syBVN",
        &cameraStructs::cameraOfflineIAObject::syBVN,
        "7")
        .def_readonly("cxyBVN",
        &cameraStructs::cameraOfflineIAObject::cxyBVN,
        "8")
        .def_readonly("xMLE",
        &cameraStructs::cameraOfflineIAObject::xMLE,
        "9")
        .def_readonly("yMLE",
        &cameraStructs::cameraOfflineIAObject::yMLE,
        "2")
        .def_readonly("sxMLE",
        &cameraStructs::cameraOfflineIAObject::sxMLE,
        "1")
        .def_readonly("syMLE",
        &cameraStructs::cameraOfflineIAObject::syMLE,
        "1")
        .def_readonly("cxyMLE",
        &cameraStructs::cameraOfflineIAObject::cxyMLE,
        "1")
        .def_readonly("xBVNerr",
        &cameraStructs::cameraOfflineIAObject::xBVNerr,
        "5")
        .def_readonly("yBVNerr",
        &cameraStructs::cameraOfflineIAObject::yBVNerr,
        "6")
        .def_readonly("sxBVNerr",
        &cameraStructs::cameraOfflineIAObject::sxBVNerr,
        "7")
        .def_readonly("syBVNerr",
        &cameraStructs::cameraOfflineIAObject::syBVNerr,
        "7")
        .def_readonly("cxyBVNerr",
        &cameraStructs::cameraOfflineIAObject::cxyBVNerr,
        "8")
        .def_readonly("xMLEerr",
        &cameraStructs::cameraOfflineIAObject::xMLEerr,
        "9")
        .def_readonly("yMLEerr",
        &cameraStructs::cameraOfflineIAObject::yMLEerr,
        "2")
        .def_readonly("sxMLEerr",
        &cameraStructs::cameraOfflineIAObject::sxMLEerr,
        "1")
        .def_readonly("syMLEerr",
        &cameraStructs::cameraOfflineIAObject::syMLEerr,
        "1")
        .def_readonly("cxyMLEerr",
        &cameraStructs::cameraOfflineIAObject::cxyMLEerr,
        "1")
        .def_readonly("totalPixelIntensity",
        &cameraStructs::cameraOfflineIAObject::totalPixelIntensity,
        "1")
        .def_readonly("useBkgrnd",
        &cameraStructs::cameraOfflineIAObject::useBkgrnd,
        "1")
        .def_readonly("useManualCrop",
        &cameraStructs::cameraOfflineIAObject::useManualCrop,
        "1")
        .def_readonly("useBVN",
        &cameraStructs::cameraOfflineIAObject::useBVN,
        "1")
        .def_readonly("useMaskFromES",
        &cameraStructs::cameraOfflineIAObject::useMaskFromES,
        "1")
        .def_readonly("maskXES",
        &cameraStructs::cameraOfflineIAObject::maskXES,
        "1")
        .def_readonly("maskYES",
        &cameraStructs::cameraOfflineIAObject::maskYES,
        "1")
        .def_readonly("maskRXES",
        &cameraStructs::cameraOfflineIAObject::maskRXES,
        "1")
        .def_readonly("maskRYES",
        &cameraStructs::cameraOfflineIAObject::maskRYES,
        "1")
        .def_readonly("usePixToMmFromES",
        &cameraStructs::cameraOfflineIAObject::usePixToMmFromES,
        "1")
        .def_readonly("pixToMmES",
        &cameraStructs::cameraOfflineIAObject::pixToMmES,
        "1")
        .def_readonly("useRRThresholdFromES",
        &cameraStructs::cameraOfflineIAObject::useRRThresholdFromES,
        "1")
        .def_readonly("RRThresholdES",
        &cameraStructs::cameraOfflineIAObject::RRThresholdES,
        "1")
        .def_readonly("useSigmaCutFromES",
        &cameraStructs::cameraOfflineIAObject::useSigmaCutFromES,
        "1")
        .def_readonly("sigmaCutES",
        &cameraStructs::cameraOfflineIAObject::sigmaCutES,
        "1")
        .def_readonly("useFilterFromES",
        &cameraStructs::cameraOfflineIAObject::useFilterFromES,
        "1")
        .def_readonly("filterES",
        &cameraStructs::cameraOfflineIAObject::filterES,
        "1")
        .def_readonly("useDirectCutLevelFromES",
        &cameraStructs::cameraOfflineIAObject::useDirectCutLevelFromES,
        "1")
        .def_readonly("DirectCutLevelES",
        &cameraStructs::cameraOfflineIAObject::DirectCutLevelES,
        "1")
        .def_readonly("useRRThresholdFromES",
        &cameraStructs::cameraOfflineIAObject::useRRThresholdFromES,
        "1")
        .def_readonly("manualCropX",
        &cameraStructs::cameraOfflineIAObject::manualCropX,
        "1")
        .def_readonly("manualCropY",
        &cameraStructs::cameraOfflineIAObject::manualCropY,
        "1")
        .def_readonly("manualCropW",
        &cameraStructs::cameraOfflineIAObject::manualCropW,
        "1")
        .def_readonly("manualCropH",
        &cameraStructs::cameraOfflineIAObject::manualCropH,
        "1")
        .def_readonly("savedCroppedX",
        &cameraStructs::cameraOfflineIAObject::savedCroppedX,
        "1")
        .def_readonly("savedCroppedY",
        &cameraStructs::cameraOfflineIAObject::savedCroppedY,
        "1")
        .def_readonly("pixToMM",
        &cameraStructs::cameraOfflineIAObject::pixToMM,
        "1")
        ;
      boost::python::class_<offlineImageAnalyser,boost::noncopyable>
        ("offlineImageAnalyser","offlineImageAnalyser Doc String", boost::python::no_init)
        .def("loadImage",
                       &offlineImageAnalyser::loadImage,
                      (arg("data"),arg("height"),arg("width")),
                      "Takes in 1D array, raw image data.")
        .def("loadBackgroundImage",
                      &offlineImageAnalyser::loadBackgroundImage,
                      (arg("data")),
                      "Takes in 1D array, raw background image data.")
        .def("analyse",
                      &offlineImageAnalyser::analyse,
                      "Based of your set up this will analyse your image and output the resulst to to object CoIA ")
        .def("isAnalysing",
                      &offlineImageAnalyser::isAnalysing,
                      "return true if analysis if running in a thread. ")
        .def("writeData",
                      &offlineImageAnalyser::writeData,
                      "Writes data into a csv file.")
        .def_readonly("CoIA",
                      &offlineImageAnalyser::CoIA,
                      "Object (cameraOffline IMage Object) containing all the offline Image Anaylsis data.")
        .def("useBackground",
                      &offlineImageAnalyser::useBackground,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use background.")
        .def("useManualCrop",
                      &offlineImageAnalyser::useManualCrop,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use Manual Crop.")
        .def("useESMask",
                      &offlineImageAnalyser::useESMask,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use  Expert Setting Mask.")
         .def("useESPixToMm",
                      &offlineImageAnalyser::useESPixToMm,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use Expert Setting pixel to mm ratio.")
        .def("useESRRThreshold",
                      &offlineImageAnalyser::useESRRThreshold,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use Expert Setting R-squared threshold.")
        .def("useESSigmaCut",
                      &offlineImageAnalyser::useESSigmaCut,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use Expert Setting Sigma Cut.")
        .def("useESFilter",
                      &offlineImageAnalyser::useESFilter,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use Expert Setting Filter.")
        .def("useESDirectCut",
                      &offlineImageAnalyser::useESDirectCut,
                      (arg("bool")),
                      "Set a bool to determine whether or not to use Expert Setting Direct Cut.")

        .def("setManualCrop",
                      &offlineImageAnalyser::setManualCrop,
                      (arg("x"),arg("y"),arg("w"),arg("h")),
                      "Manual crop sett parameters")
        .def("setESMask",
                    &offlineImageAnalyser::setESMask,
                    (arg("x"),arg("y"),arg("rx"),arg("ry")),
                    "Set Mask parameterss")
         .def("setESPixToMm",
                      &offlineImageAnalyser::setESPixToMm,
                      (arg("double")),
                      "Set pixel to mm ratio")
        .def("setESRRThreshold",
                      &offlineImageAnalyser::setESRRThreshold,
                      (arg("double")),
                      "Set R-squared threshold.")
        .def("setESSigmaCut",
                      &offlineImageAnalyser::setESSigmaCut,
                      (arg("double")),
                      "Set Sigma Cut.")
        .def("setESFilter",
                      &offlineImageAnalyser::setESFilter,
                      (arg("int")),
                      "Set Filter.")
        .def("setESDirectCut",
                      &offlineImageAnalyser::setESDirectCut,
                      (arg("double")),
                      "Set Direct Cut.")
        ;
   class_<cameraIAController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraIAController","cameraIAController Doc String",no_init)
        .def("get_CA_PEND_IO_TIMEOUT",   &cameraIAController::get_CA_PEND_IO_TIMEOUT  )
        .def("set_CA_PEND_IO_TIMEOUT",   &cameraIAController::set_CA_PEND_IO_TIMEOUT  )
        .def("getOfflineIARef",
             &cameraIAController::getOfflineIARef,
             return_value_policy<reference_existing_object>(),
             "This is a ref to the class that holds the functions for offline image analysis.")
       // .def_readwrite("hi",
       //      &cameraIAController::offlineIA)
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
        .def("startAnalysis",
             &cameraIAController::startAnalysis,
             "Starts analysis on selected camera.")
        .def("stopAnalysis",
             &cameraIAController::stopAnalysis,
             "Stops analysis on selected camera.")
        .def("setBackground",
             &cameraIAController::setBackground,
             "Returns True if selected camera copied the current Array Data to the background PV array data in EPICS.")
        .def("useBackground",
             &cameraIAController::useBackground,
             (arg("True/False")),
             "Returns True if using background that has been previous set using the function 'setBackground'.")
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

        .def_readonly("controllerType",
            &cameraIAController::controllerType,
            "This is an enum that returns the controller type")
        ;
   class_<VCcameraIA,boost::noncopyable>("init")
        .def("virtual_VELA_Camera_IA_Controller",
             &VCcameraIA::virtual_VELA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>(),
             "Function that returns a controller connected to a virtual EPICS system")
        .def("offline_VELA_Camera_IA_Controller",
             &VCcameraIA::offline_VELA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>(),
             "Function that returns a controller connected to no EPICS system")
        .def("physical_VELA_Camera_IA_Controller",
             &VCcameraIA::physical_VELA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>(),
             "Function that returns a controller connected to REAL EPICS system")
        .def("virtual_CLARA_Camera_IA_Controller",
             &VCcameraIA::virtual_CLARA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_IA_Controller",
             &VCcameraIA::offline_CLARA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_IA_Controller",
             &VCcameraIA::physical_CLARA_Camera_IA_Controller,
             return_value_policy<reference_existing_object>())
        .def("setQuiet",              &VCcameraIA::setQuiet,
            "This is the setQuiet function that sops the controller displaying all messages")
        .def("setVerbose",            &VCcameraIA::setVerbose,
             "This is the setVerbose function that sets the controller to display all messages")
        .def("setMessage",            &VCcameraIA::setMessage,
            "This is the setMessage function that sets the controller to only display messages")
        .def("setDebugMessage",  &VCcameraIA::setDebugMessage,
            "This is the setDebugMessage function that sets the controller to only display debug messages")
        ;
}


#endif // VC_CAMERAS_IA_H
