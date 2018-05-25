/*
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
//
//  Author:      DJS
//  Last edit:   29-03-2018
//  FileName:    VCpilaser.h
//  Description:
//
//
//*/
#ifndef _VC_PILASER_H_
#define _VC_PILASER_H_
//stl
#include <string>
//
#include "pilaserController.h"
#include "VCheader.h"
#include "VCbase.h"
//______________________________________________________________________________
class VCpilaser : public VCbase
{
   public:
        VCpilaser();
        ~VCpilaser();

        pilaserController& virtual_PILaser_Controller();
        pilaserController& physical_PILaser_Controller();
        pilaserController& offline_PILaser_Controller();


    protected:
    private:
        /* object names */
        pilaserController* virtual_pilaser_Controller_Obj ;
        pilaserController* physical_pilaser_Controller_Obj;
        pilaserController* offline_pilaser_Controller_Obj ;

        const std::string pilaserConf, vcAnalysisConf,
                          piLaserMirrorConf;

        pilaserController& getpilaserController(pilaserController*& cont,
                                                const std::string & name,
                                                const bool shouldVM,
                                                const bool shouldEPICS,
                                                const std::string& pilaserConf,
                                                const std::string& vcAnalysisConf,
                                                const std::string& piLaserMirrorConf
                                                );

        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const pilaserController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();
};
///// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
///// Create a load of different function pointers and use them in the bindings
///// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
///// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
///// I'm going to do it the function pointer way here...
///// The other alternative is to create lots of different function names in the c++ class
///// just to make the function pointer overloading neater, let's make some typedefs, generally i don't like doing this
//typedef double doub;
//typedef const double cdou;
//typedef std::vector<double> vecd;
//typedef std::vector<std::vector<double>> vvcd;
//typedef const std::vector<double> cved;
//typedef const size_t csiz;
//typedef size_t size;
//typedef std::vector<size_t> vsiz;
//typedef std::string stri;
//typedef const std::string cstr;
//typedef std::vector<std::string> vecs;
//typedef const std::vector<std::string> cves;
//typedef std::vector<bool> vecb;
/////
////bool(pilaserController::*screenIN_1)  (cstr& = &pilaserController::screenIN;
//
//bool(pilaserController::*setHpos_1)(doub) = &pilaserController::setHpos;
//bool(pilaserController::*setHpos_2)(int = &pilaserController::setHpos;
//bool(pilaserController::*setVpos_1)(doub) = &pilaserController::setVpos;
//bool(pilaserController::*setVpos_2)(int = &pilaserController::setVpos;
//bool(pilaserController::*setIntensity_1)(doub) = &pilaserController::setIntensity;
//bool(pilaserController::*setIntensity_2)(int) = &pilaserController::setIntensity;
//
//______________________________________________________________________________

////using namespace virtualCathodeStructs;
////using namespace pilaserMirrorStructs;
////using namespace pilaserStructs;
////using namespace shutterStructs;
//using namespace UTL;
BOOST_PYTHON_MODULE(VELA_CLARA_PILaser_Control)
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();
    /*
        Things that you want to use in python muct be exposed:
        containers
    */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    /*
        // IF WE BUILD INDIVIDUAL CONTRLLER S FOR EACH SUB-HARDWRAE SYSTEM
        // THEN THE DEFINITIONS NEED TO MOVE TO THOSE CLASSES
        // I.E A VClasermirror.pyd
        // we can then use pre-processor commands to include thwe desired defintions
        // similar to   BOOST_PYTHON_INCLUDE::export_BaseObjects();
    */
    using namespace UTL;
    using namespace boost::python;
    using namespace boost;

//    const char* getShutterNames_Py_doc = "getShutterNames_Py_doc.";
//    const char* closeAndWait_doc = "closeAndWait_doc.";
//    const char* openAndWait_doc = "openAndWait_doc.";
//    const char* isClosed_doc = "isClosed_doc.";
//    const char* isOpen_doc = "isOpen_doc.";
//    const char* close_doc = "close_doc.";
//    const char* open_doc = "open_doc.";
//    const char* name_doc = "name_doc.";
//    const char* getState_doc = "getState_doc.";
//
    const char* getIntensity_doc = "getIntensity_doc.";
    const char* setCharge_doc = "setCharge_doc.";
    const char* setIntensity_doc = "getIntensity_doc.";
    const char* getStatus_doc = "getIntensity_doc.";
    const char* getStabilisationStatus_doc = "getIntensity_doc.";
    const char* isOn_doc = "isOn_doc.";
    const char* isOff_doc = "isOff_doc.";
    const char* isStabilisationOn_doc = "isStabilisationOn_doc.";
    const char* isStabilisationOff_doc = "isStabilisationOff_doc.";
    const char* disableStabilisation_doc = "disableStabilisation_doc.";
    const char* enableStabilisation_doc = "isStabilisationOff_doc.";
    const char* getHWP_doc = "getHWP_doc.";
    const char* setHWP_doc = "setHWP_doc.";
    const char* areAllOpen_doc = "areAllOpen_doc.";
    const char* areAllClosed_doc = "areAllClosed_doc.";
    const char* getShutterNames_doc = "getShutterNames_doc.";



    const char* getHpos_doc  = "getHpos_doc.";
    const char* getVpos_doc  = "getVpos_doc.";
    const char* setHpos_doc  = "setHpos_doc.";
    const char* setVpos_doc  = "setVpos_doc.";
    const char* getHstep_doc = "getHstep_doc.";
    const char* getVstep_doc = "getVstep_doc.";
    const char* setVstep_doc = "setVstep_doc.";
    const char* setHstep_doc = "setHstep_doc.";
    const char* moveH_doc    = "moveH_doc.";
    const char* moveV_doc    = "moveV_doc.";
    const char* getXBuffer_doc    = "getXBuffer_doc.";
    const char* getYBuffer_doc    = "getYBuffer_doc.";
    const char* getSigXBuffer_doc = "getSigXBuffer_doc.";
    const char* getSigYBuffer_doc = "getSigYBuffer_doc.";
    const char* getSigXYBuffer_doc= "getSigXYBuffer_doc.";
    const char* getXPixBuffer_doc = "getXPixBuffer_doc.";
    const char* getYPixBuffer_doc = "getYPixBuffer_doc.";
    const char* getSigXPixBuffer_doc  = "getSigXPixBuffer_doc.";
    const char* getSigYPixBuffer_doc  = "getSigYPixBuffer_doc.";
    const char* getSigXYPixBuffer_doc = "getSigXYPixBuffer_doc.";
    const char* getQBuffer_doc        = "getQBuffer_doc.";

    const char* getX_doc    = "getX_doc.";
    const char* getY_doc    = "getY_doc.";
    const char* getSigX_doc = "getSigX_doc.";
    const char* getSigY_doc = "getSigY_doc.";
    const char* getSigXY_doc= "getSigXY_doc.";
    const char* getXPix_doc = "getXPix_doc.";
    const char* getYPix_doc = "getYPix_doc.";
    const char* getSigXPix_doc  = "getSigXPix_doc.";
    const char* getSigYPix_doc  = "getSigYPix_doc.";
    const char* getSigXYPix_doc = "getSigXYP_doc.";
    const char* getQ_doc        = "getQ_doc.";

    const char* clearBuffer_doc = "clearBuffer_doc.";
    const char* isBufferFull_doc = "isBufferFull_doc.";
    const char* isBufferNotFull_doc = "isBufferNotFull_doc.";
    const char* setBufferSize_doc = "setBufferSize_doc.";
    const char* getBufferSize_doc = "getBufferSize_doc";
    const char* getPixelValues_doc = "getPixelValues_doc";
    const char* getPixelValuesBuffer_doc = "getPixelValuesBuffer_doc";
    const char* getBufferCount_doc = "getBufferCount_doc";


    const char* getFastImage_doc    = "getFastImage_doc   ";


    class_<pilaserController, bases<controller>, noncopyable>
        ("pilaserController","pilaserController member functions", no_init )
        .def("getHWP",&pilaserController::getHWP,getHWP_doc)
        .def("setHWP",&pilaserController::setHWP,(VALUE_ARG),setHWP_doc)
        .def("setCharge",&pilaserController::setCharge,setCharge_doc)
        .def("setIntensity",&pilaserController::setIntensity,setIntensity_doc)
        .def("getIntensity",&pilaserController::getIntensity,getIntensity_doc)
        .def("getStatus",&pilaserController::getStatus,getStatus_doc)
        .def("getStabilisationStatus",&pilaserController::getStabilisationStatus,getStabilisationStatus_doc)
        .def("isOn",&pilaserController::isOn,isOn_doc)
        .def("isOff",&pilaserController::isOff,isOff_doc)
        .def("isStabilisationOff",&pilaserController::isStabilisationOff,isStabilisationOff_doc)
        .def("isStabilisationOn",&pilaserController::isStabilisationOn,isStabilisationOn_doc)
        .def("disableStabilisation",&pilaserController::disableStabilisation,disableStabilisation_doc)
        .def("enableStabilisation",&pilaserController::enableStabilisation,enableStabilisation_doc)
        .def("getHpos", &pilaserController::getHpos, getHpos_doc )
        .def("getVpos", &pilaserController::getVpos, getVpos_doc )
        .def("setHpos", &pilaserController::setHpos, setHpos_doc )
        .def("setVpos", &pilaserController::setVpos, setVpos_doc )
        .def("getHstep",&pilaserController::getHstep,getHstep_doc)
        .def("getVstep",&pilaserController::getVstep,getVstep_doc)
        .def("setHstep",&pilaserController::setHstep,setVstep_doc)
        .def("setVstep",&pilaserController::setVstep,setHstep_doc)
        .def("moveH",   &pilaserController::moveH,   moveH_doc   )
        .def("moveV",   &pilaserController::moveV,   moveV_doc   )


        .def("getFastImage",   &pilaserController::getFastImage_Py,   getFastImage_doc   )

        .def("getX",        &pilaserController::getX,    getX_doc    )
        .def("getY",        &pilaserController::getY,    getY_doc    )
        .def("getSigX",     &pilaserController::getSigX, getSigX_doc )
        .def("getSigY",     &pilaserController::getSigY, getSigY_doc )
        .def("getSigXY",    &pilaserController::getSigXY,getSigXY_doc )
        .def("getXPix",     &pilaserController::getXPix, getXPix_doc )
        .def("getYPix",     &pilaserController::getYPix, getYPix_doc )
        .def("getSigXPix",  &pilaserController::getSigXPix,  getSigXPix_doc)
        .def("getSigYPix",  &pilaserController::getSigYPix,  getSigYPix_doc)
        .def("getSigXYPix", &pilaserController::getSigXYPix, getSigXYPix_doc)
        .def("getQ",        &pilaserController::getQ,        getQ_doc)

        .def("getBufferCount",     &pilaserController::getBufferCount,           getBufferCount_doc)
        .def("clearBuffer",        &pilaserController::clearBuffer,              clearBuffer_doc)
        .def("isBufferFull",       &pilaserController::isBufferFull,             isBufferFull_doc)
        .def("isBufferNotFull",    &pilaserController::isBufferNotFull,          isBufferNotFull_doc)
        .def("setBufferSize",      &pilaserController::setBufferSize,            setBufferSize_doc)
        .def("getBufferSize",      &pilaserController::getBufferSize,            getBufferSize_doc)
        .def("getPixelValues",     &pilaserController::getPixelValues_Py,        getPixelValues_doc)
        .def("getPixelValuesBuffer",&pilaserController::getPixelValuesBuffer_Py, getPixelValuesBuffer_doc)

        .def("getXBuffer",        &pilaserController::getXBuffer_Py,    getXBuffer_doc    )
        .def("getYBuffer",        &pilaserController::getYBuffer_Py,    getYBuffer_doc    )
        .def("getSigXBuffer",     &pilaserController::getSigXBuffer_Py, getSigXBuffer_doc )
        .def("getSigYBuffer",     &pilaserController::getSigYBuffer_Py, getSigYBuffer_doc )
        .def("getSigXYBuffer",    &pilaserController::getSigXYBuffer_Py,getSigXYBuffer_doc )
        .def("getXPixBuffer",     &pilaserController::getXPixBuffer_Py, getXPixBuffer_doc )
        .def("getYPixBuffer",     &pilaserController::getYPixBuffer_Py, getYPixBuffer_doc )
        .def("getSigXPixBuffer",  &pilaserController::getSigXPixBuffer_Py,  getSigXPixBuffer_doc)
        .def("getSigYPixBuffer",  &pilaserController::getSigYPixBuffer_Py,  getSigYPixBuffer_doc)
        .def("getSigXYPixBuffer", &pilaserController::getSigXYPixBuffer_Py, getSigXYPixBuffer_doc)
        .def("getQBuffer",        &pilaserController::getQBuffer_Py,        getQBuffer_doc)
        ;

//
//    const char* hPos_doc
//        = "horizontal Mirror Position (not available as of April 2018)";
//    const char* vPos_doc
//        = "vertical Mirror Position (not available as of April 2018)";
//    const char* hStep_doc = "horizontal Mirror step size ";
//    const char* vStep_doc = "vertical   Mirror step size ";
//                name_doc  = "object name";


//    class_<pilaserMirrorStructs::pilMirrorObject,noncopyable>
//        ("pilMirrorObject","pilMirrorObject member variables (read access only)",no_init)
//        .def_readonly("hStep",&pilaserMirrorStructs::pilMirrorObject::hStep,hStep_doc)
////        .def_readonly("vStep",&pilMirrorObject::vStep,vStep_doc)
////        .def_readonly("hPos", &pilMirrorObject::hPos,hPos_doc)
////        .def_readonly("vPos", &pilMirrorObject::vPos,vPos_doc)
////        .def_readonly("name", &pilMirrorObject::name,name_doc)
//        ;

//    const char* stabilisation_status_doc
//        = "Status (ON/OFF) of laser transport system stabilisation system.";
//    const char* intensity_doc
//        = "Laser intensity set-point (AU) add more detail when we have it.";
//    const char* status_doc  = "Status (ON/OFF) of laser system.";
//    const char* setCharge_doc2 = "Charge (pc) set point.";

//    class_<pilaserStructs::pilaserObject,noncopyable>
//        ("pilaserObject","pilaserObject member variables (read access only)", no_init)
//        .def_readonly("status",&pilaserStructs::pilaserObject::status,status_doc)
////        .def_readonly("stabilisation_status",&pilaserObject::stabilisation_status,stabilisation_status_doc)
////        .def_readonly("intensity", &pilaserObject::intensity,intensity_doc)
////        .def_readonly("setCharge", &pilaserObject::setCharge,setCharge_doc)
////        .def_readonly("name", &pilaserObject::name,name_doc)
//        ;

//    class_<virtualCathodeStructs::virtualCathodeDataObject,noncopyable>
//        ("virtualCathodeDataObject",
//            "virtualCathodeDataObject member variables (read access only) "
//            "we are waiting to see what this will look like"
//            , no_init)
//        .def("buffer_size",&virtualCathodeStructs::virtualCathodeDataObject::buffer_size)
//        ;

//    const char* state_doc = "State (OPEN/CLOSED) of Shutter.";

//    class_<shutterStructs::shutterObject,noncopyable>
//        ("shutterObject","shutterObject member variables (read access only)", no_init)
//        .def_readonly("state",&shutterStructs::shutterObject::state,state_doc)
//        ;


    /// The main class that creates all the controller obejcts
    class_<VCpilaser,bases<VCbase>,boost::noncopyable> ("init","Doc string")
        .def("virtual_PILaser_Controller",  &VCpilaser::virtual_PILaser_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual PI laser object.")
        .def("physical_PILaser_Controller",  &VCpilaser::physical_PILaser_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical PI laser object.")
        .def("offline_PILaser_Controller",  &VCpilaser::offline_PILaser_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline PI laser object.")
        ;

//
//    // pilaser object struct to be exposed, used when returning a pilaser reference
//    boost::python::class_<pilaserStructs::pilaserObject,boost::noncopyable>
//        ("pilaserObject","pilaserObject Doc String", boost::python::no_init)
//        .def_readonly("name",      &pilaserStructs::pilaserObject::name,
//                      "The name of the PI laser object")
//        .def_readonly("pvRoot",    &pilaserStructs::pilaserObject::pvRoot,
//                      "The PV root name, prefixed to all PVs")
//        .def_readonly("hPos",      &pilaserStructs::pilaserObject::hPos,
//                      "The horizontal position of the laser on the cathode [mm hopefully]")
//        .def_readonly("vPos",      &pilaserStructs::pilaserObject::vPos,
//                      "The vertical position of the laser on the cathode [mm hopefully]"
//        .def_readonly("intensity", &pilaserStructs::pilaserObject::intensity,
//                      "The intensity of the laser on the cathode [mJ ??]")
////        .def_readonly("numIlocks", &pilaserStructs::pilaserObject::numIlocks,
////                      "The number of standard interlocks on the laser system")
//        ;
//
//    boost::python::class_<pilaserController, boost::python::bases<controller>, boost::noncopyable>
//        ("pilaserController",
//
// "<=================================================>\n"
// " _____ _____   _                                   \n"
// "|  __ \\_   _| | |                                 \n"
// "| |__) || |   | |     __ _ ___  ___ _ __           \n"
// "|  ___/ | |   | |    / _` / __|/ _ \\ '__|         \n"
// "| |    _| |_  | |___| (_| \\__ \\  __/ |           \n"
// "_|___|_____| |______\\__,_|___/\\___|_|            \n"
// " / ____|          | |           | | |              \n"
// "| |     ___  _ __ | |_ _ __ ___ | | | ___ _ __     \n"
// "| |    / _ \\| '_ \\| __| '__/ _ \\| | |/ _ \\ '__|\n"
// "| |___| (_) | | | | |_| | | (_) | | |  __/ |       \n"
// " \\_____\\___/|_| |_|\\__|_|  \\___/|_|_|\\___|_|  \n"
// "| |  | |    | |                                    \n"
// "| |__| | ___| |_ __        DOCUMENTATION           \n"
// "|  __  |/ _ \\ | '_ \\           &                 \n"
// "| |  | |  __/ | |_) |          HINTS               \n"
// "|_|  |_|\\___|_| .__/                              \n"
// "              | |      Duncan Scott March 2017     \n"
// "              |_|                                  \n"
// "\n"
// "=> The object to control the PhotoInjector Laser   \n"
// "\n"
// "=> Use the set and get methods for values and getPILObjConstRef to return a reference to the PILaser data Object"
// "\n"
// "<=================================================>\n"
//
//         , boost::python::no_init)
//        .def("getILockStates",    &pilaserController::getILockStates,
//                                  "Return the state of interlocks as an integer. There Currently NO epics ilocks for the PI laser.")
//        .def("getILockStatesStr", &pilaserController::getILockStatesStr,
//                                  "Return state of interlocks as a stringr. There Currently NO epics ilocks for the PI laser.")
//        .def("get_CA_PEND_IO_TIMEOUT",  &pilaserController::get_CA_PEND_IO_TIMEOUT,
//                                        "Return the current waiting time [seconds] when sending commands to EPICS.")
//        .def("set_CA_PEND_IO_TIMEOUT",  &pilaserController::set_CA_PEND_IO_TIMEOUT,
//                                        (boost::python::arg("time"),
//                                        "Set a new waiting time [seconds] when sending commands to EPICS"))
//        .def("getHpos", &pilaserController::getHpos,
//                            "returns the horizontal position of the laser on the cathode [units etc?]."
//        .def("getVpos", &pilaserController::getVpos,
//                            "returns the vertical position of the laser on the cathode [units etc?]."
//        .def("getIntensity", &pilaserController::getIntensity,
//                            "returns the intensity the laser [units etc?]."
//        .def("getPILObjConstRef",  &pilaserController::getPILObjConstRef,
//                                   return_value_policy<reference_existing_object>(),
//                            "returns a reference to a PI laser object, (identified by its name)."
//        .def("setHpos",  setHpos_1,
//                            "set the horizontal position of the laser beam on the cathode."
//        .def("setHpos",  setHpos_2,
//             "set the horizontal position of the laser beam on the cathode."
//        .def("setVpos",  setVpos_1,"set the vertical position of the laser beam on the cathode."
//        .def("setVpos",  setVpos_2,"set the vertical position of the laser beam on the cathode."
//        .def("setIntensity",  setIntensity_1,"set the intensity of the laser beam on the cathode.")
//        .def("setIntensity",  setIntensity_2,"set the intensity of the laser beam on the cathode.")
//        ;

//https://wiki.python.org/moin/boost.python/FAQ
//#include /libs/python/test/module_tail.cpp
}

#endif // _VC_PILASER_H_
