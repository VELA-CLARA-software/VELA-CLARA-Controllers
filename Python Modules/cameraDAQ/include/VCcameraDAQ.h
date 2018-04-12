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
#ifndef VC_CAMERAS_DAQ_H
#define VC_CAMERAS_DAQ_H
//tp
#include "cameraDAQController.h"
#include "VCbase.h"
#include "VCHeader.h"
//boost
//#include <boost/python/detail/wrap_python.hpp>
//#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/return_value_policy.hpp>
//#include <boost/python/detail/wrap_python.hpp>
//#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/return_value_policy.hpp>
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

///Top Class///
class VCcameraDAQ : public VCbase
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
    protected:

    private:
        cameraDAQController * virtual_Camera_DAQ_Controller_Obj;
        cameraDAQController * offline_Camera_DAQ_Controller_Obj;
        cameraDAQController * physical_Camera_DAQ_Controller_Obj;
        cameraDAQController& getController(cameraDAQController * cont,
                                           const std::string& conf,
                                           const std::string & name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const HWC_ENUM::MACHINE_AREA myMachineArea );
        std::map<const cameraDAQController*, std::pair<bool,bool>> messageStates;

        void updateMessageStates();
};
///Expose Relevant Classes, Vectors and Enums to Python///
using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Camera_DAQ_Control )
{
    docstring_options doc_options(true);
    doc_options.disable_cpp_signatures();
    /* Main project objects and enums are defined in here */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();
/*
    ///Expose Vectors
    // if not used don't inlcude
    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>())
        ;
    class_<std::vector<double>>("std_vector_double")
        .def( vector_indexing_suite< std::vector<double>>())
        ;
*/
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
    /*
    class_<baseObject, boost::noncopyable>("baseObject", no_init)
        ;

    class_<controller, bases<baseObject>,boost::noncopyable>
        ("controller","controller Doc String", no_init) /// forces Python to not be able to construct (init) this object

        .def("get_CA_PEND_IO_TIMEOUT",
             pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT)          )
        .def("set_CA_PEND_IO_TIMEOUT",
             pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT)          )
        .def("getILockStatesStr",
             pure_virtual(&controller::getILockStatesStr)               )
        .def("getILockStates",
             pure_virtual(&controller::getILockStates)                  )
        ;
    */
    boost::python::class_<cameraStructs::cameraObject,boost::noncopyable>
        ("cameraObject_from_DAQ","cameraObject Doc String", boost::python::no_init)
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
        .def_readonly("streamingIPAddress",
                      &cameraStructs::cameraObject::streamingIPAddress,
                      "IP address for the camera live stream.")
        .def_readonly("DAQ",
                      &cameraStructs::cameraObject::DAQ,
                      "Object (cameraDAQObject) containing all the DAQ data.")
        .def_readonly("IA",
                      &cameraStructs::cameraObject::IA,
                      "Object (cameraIAObject) containing all the Image Anaylsis data.")
        ;
    class_<cameraStructs::cameraDAQObject,boost::noncopyable>
        ("cameraDAQObject","cameraDAQObject (read only values)", no_init)
        .def_readonly("captureState",
                      &cameraStructs::cameraDAQObject::captureState,
                      "The state indicating whether the Camera is collecting images.")
        .def_readonly("writeState",
                      &cameraStructs::cameraDAQObject::writeState,
                      "The state indicating whether the Camera is saving images")
        .def_readonly("writeCheck",
                      &cameraStructs::cameraDAQObject::writeState,
                      "A check to indicated the ability to save images (good/error).")
        .def_readonly("shotsTaken",
                      &cameraStructs::cameraDAQObject::shotsTaken,
                      "The number of shots taken and collected.")
        .def_readonly("maxShots",
                      &cameraStructs::cameraDAQObject::maxShots,
                      "The maximum number of shots that can be taken in one burst.")
        .def_readonly("numberOfShots",
                      &cameraStructs::cameraDAQObject::numberOfShots,
                      "The number of shots set to collect.")
        .def_readonly("frequency",
                      &cameraStructs::cameraDAQObject::frequency,
                      "Frequency of Camera's image acquisiton.")
        .def_readonly("sensorTemp",
                      &cameraStructs::cameraDAQObject::sensorTemp,
                      "Temperature of Sensor (CCD) in celcius.")
        .def_readonly("exposureTime",
                      &cameraStructs::cameraDAQObject::exposureTime,
                      "Time (seconds) of exposure for single camera image.")
        .def_readonly("acquisitionPeriod",
                      &cameraStructs::cameraDAQObject::acquisitionPeriod,
                      "Time (seconds) of full acquisition period, that includes expoture time.")
        .def_readonly("writeErrorMessage",
                      &cameraStructs::cameraDAQObject::writeErrorMessage,
                      "If there is an error with saving the images a message will be displayed here.")
        .def_readonly("latestDirectory",
                      &cameraStructs::cameraDAQObject::latestDirectory,
                      "Latest directory images were saved to.")
        ;

    class_<cameraDAQController, bases<controller>, boost::noncopyable>
        ("cameraDAQController","cameraDAQController", no_init)
        .def("get_CA_PEND_IO_TIMEOUT",
             &cameraDAQController::get_CA_PEND_IO_TIMEOUT)
        .def("set_CA_PEND_IO_TIMEOUT",
             &cameraDAQController::set_CA_PEND_IO_TIMEOUT)
        .def("collectAndSave",
             &cameraDAQController::collectAndSave,
             (arg("Number of Shots")),
             "Collects and saves images from selected camera in a thread")
        .def("killCollectAndSave",
             &cameraDAQController::killCollectAndSave,
             "Returns True if stopped the selected camera collectAndSave process")
       .def("collectAndSaveVC",
             &cameraDAQController::collectAndSaveVC,
             (arg("Number of Shots")),
             "Collects and saves images from VC camera in a thread")
        .def("killCollectAndSaveVC",
             &cameraDAQController::killCollectAndSaveVC,
             "Returns True if stopped the VC camera collectAndSave process")
        .def("getCamDAQObjConstRef",
             &cameraDAQController::getCamDAQObjConstRef,
             return_value_policy<reference_existing_object>(),
             (arg("name")),
             "Returns a reference to camera 'name'")
        .def("getSelectedDAQRef",
             &cameraDAQController::getSelectedDAQRef,
             return_value_policy<reference_existing_object>(),
             "Returns a reference to selected camera")
        .def("getVCDAQRef",
             &cameraDAQController::getVCDAQRef,
             return_value_policy<reference_existing_object>(),
             "Returns a reference to VC camera")
        .def("isON",
             &cameraDAQController::isON,
            (arg("name")),
             "Returns True if camera 'name' is ON")
        .def("isOFF",
             &cameraDAQController::isOFF,
             (arg("name")),
             "Returns True if camera 'name' is OFF")
        .def("isAcquiring",
             &cameraDAQController::isAcquiring,
             (arg("name")),
             "Returns True if camera 'name' is acquiring")
        .def("isNotAcquiring",
             &cameraDAQController::isNotAcquiring,
             (arg("name")),
             "Returns True if camera 'name' is not acquiring")
        .def("selectedCamera",
             &cameraDAQController::selectedCamera,
             "Returns the name (string) of the current selected camera")
        .def("setCamera",
             &cameraDAQController::setCamera,
             (arg("name")),
             "Returns True if camera 'name' is set as selected camera")
        .def("startAcquiring",
             &cameraDAQController::startAcquiring,
             "Sets Selected camera acquiring and returns True if successful")
        .def("stopAcquiring",
             &cameraDAQController::stopAcquiring,
             "Stops Selected camera acquiring and returns True if succesful")
        .def("startVCAcquiring",
             &cameraDAQController::startVCAcquiring,
             "Sets VC Camera acquiring and returns True if successful")
        .def("stopVCAcquiring",
             &cameraDAQController::stopVCAcquiring,
             "Stops VC Camera acquiring and returns True if successful")
         .def("getlatestDirectory",
             &cameraDAQController::getlatestDirectory,
             "Returns a string indicating directory selected camers last saved images to.")
        .def("getCameraNames",
             &cameraDAQController::getCameraNames,
             "Returns a list of all the camera names.")
        ;

    class_<VCcameraDAQ, bases<VCbase>, boost::noncopyable>("init")
        .def("virtual_VELA_Camera_DAQ_Controller",
             &VCcameraDAQ::virtual_VELA_Camera_DAQ_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_Camera_DAQ_Controller",
             &VCcameraDAQ::offline_VELA_Camera_DAQ_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_Camera_DAQ_Controller",
             &VCcameraDAQ::physical_VELA_Camera_DAQ_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_Camera_DAQ_Controller",
             &VCcameraDAQ::virtual_CLARA_Camera_DAQ_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_DAQ_Controller",
             &VCcameraDAQ::offline_CLARA_Camera_DAQ_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_DAQ_Controller",
             &VCcameraDAQ::physical_CLARA_Camera_DAQ_Controller,
             return_value_policy<reference_existing_object>())
        ;

}
#endif // VC_CAMERA_DAQ_H
