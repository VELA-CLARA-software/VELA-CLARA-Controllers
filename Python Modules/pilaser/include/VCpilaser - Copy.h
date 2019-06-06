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
#include <map>
//
#include "pilaserController.h"
#include "VCheader.h"
#include "VCbase.h"
#include "expose_shutter.h"
//______________________________________________________________________________
class VCpilaser : public VCbase
{
   public:
        VCpilaser();
        ~VCpilaser();

        pilaserController& virtual_PILaser_Controller();
        pilaserController& physical_PILaser_Controller();
        pilaserController& offline_PILaser_Controller();

        pilaserController& virtual_CLARA_PILaser_Controller();
        pilaserController& physical_CLARA_PILaser_Controller();
        pilaserController& offline_CLARA_PILaser_Controller();

        pilaserController& virtual_VELA_PILaser_Controller();
        pilaserController& physical_VELA_PILaser_Controller();
        pilaserController& offline_VELA_PILaser_Controller();

    protected:
    private:
        /* object names */
        pilaserController* virtual_VELA_pilaser_Controller_Obj ;
        pilaserController* physical_VELA_pilaser_Controller_Obj;
        pilaserController* offline_VELA_pilaser_Controller_Obj ;

        pilaserController* virtual_CLARA_pilaser_Controller_Obj ;
        pilaserController* physical_CLARA_pilaser_Controller_Obj;
        pilaserController* offline_CLARA_pilaser_Controller_Obj ;


        const std::string pilaserConf, claraCamConfig,
                          piLaserMirrorConf,velaCamConfig;

        pilaserController& getpilaserController(pilaserController*& cont,
                                                const std::string & name,
                                                const bool shouldVM,
                                                const bool shouldEPICS,
                                                const HWC_ENUM::MACHINE_AREA area
                                                );
        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const pilaserController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();
};

BOOST_PYTHON_MODULE(VELA_CLARA_PILaser_Control)
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();

    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    BOOST_PYTHON_INCLUDE::expose_shutter();
    /*
        // IF WE BUILD INDIVIDUAL CONTRLLERS FOR EACH SUB-HARDWARE SYSTEM
        // THEN THE DEFINITIONS NEED TO MOVE TO THOSE CLASSES
        // I.E A VClasermirror.pyd
        // we can then use pre-processor commands to include thwe desired defintions
        // similar to   BOOST_PYTHON_INCLUDE::export_BaseObjects();
    */
    using namespace UTL;
    using namespace boost::python;
    using namespace boost;
    using namespace pilaserStructs;


    const char* VC_SET_POS_STATE_ds= "ENUM defining states when setting VC Position";


    enum_<VC_SET_POS_STATE>("VC_SET_POS_STATE",VC_SET_POS_STATE_ds)
            .value("SHUTTER_CLOSED", VC_SET_POS_STATE::SHUTTER_CLOSED)
            .value("LASER_NOT_IN_IMAGE",  VC_SET_POS_STATE::LASER_NOT_IN_IMAGE )
            .value("H_HI",  VC_SET_POS_STATE::H_HI )
            .value("V_LO",  VC_SET_POS_STATE::V_LO )
            .value("TIME_OUT",  VC_SET_POS_STATE::TIME_OUT )
            .value("MAX_ITERATIONS",  VC_SET_POS_STATE::MAX_ITERATIONS )
            .value("UNKNOWN_STATE",  VC_SET_POS_STATE::UNKNOWN_STATE )
            .value("MOVING",  VC_SET_POS_STATE::MOVING )
            .value("STARTUP",  VC_SET_POS_STATE::STARTUP )
            .value("SUCCESS",  VC_SET_POS_STATE::SUCCESS )
            .value("FAIL",  VC_SET_POS_STATE::FAIL )
            .value("RUNNING",  VC_SET_POS_STATE::RUNNING )
            ;


    const char* hPos_ds = "";
    const char* vPos_ds = "";
    const char* hStep_ds = "";
    const char* vStep_ds = "";
    const char* STEP_MAX_ds = "";

    class_<pilMirrorObject,noncopyable>
        ("pilMirrorObject","pilMirrorObject member variables (read access only)", no_init)
        .def_readonly("hPos",    &pilMirrorObject::hPos,hPos_ds)
        .def_readonly("vPos",    &pilMirrorObject::vPos,vPos_ds)
        .def_readonly("hStep",   &pilMirrorObject::hStep,hStep_ds)
        .def_readonly("vStep",   &pilMirrorObject::vStep,vStep_ds)
        .def_readonly("STEP_MAX",&pilMirrorObject::STEP_MAX,STEP_MAX_ds)
        ;

    const char* Q_ds= "";
    const char* Q_mean_ds= "";
    const char* Q_n_ds= "";
    const char* Q_sd_ds= "";
    const char* Q_clear_ds= "";

    const char* energy_ds= "";
    const char* energy_mean_ds= "";
    const char* energy_n_ds= "";
    const char* energy_sd_ds= "";
    const char* energy_clear_ds= "";
    const char* status_ds= "";
    const char* HWP_ds= "";
    const char* setCharge_ds= "";
    const char* setVCPosState_ds= "";

    class_<pilaserObject,noncopyable>
        ("pilaserObject","pilaserObject member variables (read access only)", no_init)
        .def_readonly("HWP",     &pilaserObject::HWP,    HWP_ds)
        .def_readonly("Q",       &pilaserObject::Q, Q_ds)
        .def_readonly("Q_mean",  &pilaserObject::Q_mean, Q_mean_ds)
        .def_readonly("Q_n",     &pilaserObject::Q_n,    Q_n_ds)
        .def_readonly("Q_sd",    &pilaserObject::Q_sd,   Q_sd_ds)
        .def_readonly("Q_clear", &pilaserObject::Q_clear, Q_clear_ds)
        .def_readonly("energy",  &pilaserObject::energy,    energy_ds)
        .def_readonly("energy_mean",  &pilaserObject::energy_mean,    energy_mean_ds)
        .def_readonly("energy_n",    &pilaserObject::energy_n,        energy_n_ds)
        .def_readonly("energy_sd",   &pilaserObject::energy_sd,       energy_sd_ds)
        .def_readonly("energy_clear",  &pilaserObject::energy_clear,  energy_clear_ds)
        .def_readonly("setCharge",  &pilaserObject::setCharge,          setCharge_ds)
        .def_readonly("setVCPosState", &pilaserObject::setVCPosState, setVCPosState_ds)
        ;

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
    const char* getEnergy_doc = "getIntensity_doc.";
    const char* getEnergyBuffer_doc = "getEnergyBuffer_doc.";
    const char* setCharge_doc = "setCharge_doc.";
    const char* setEnergy_doc = "getIntensity_doc.";
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

    const char* getHpos_doc
        = "horizontal Mirror Position (not available as of April 2018)";
    const char* getVpos_doc
        = "vertical Mirror Position (not available as of April 2018)";

    const char* setHpos_doc
        = "horizontal Mirror Position (not available as of April 2018)";
    const char* setVpos_doc        = "vertical Mirror Position (not available as of April 2018)";

    const char* getHstep_doc = "horizontal Mirror step size ";
    const char* getVstep_doc = "vertical   Mirror step size ";

    const char* setHstep_doc = "set horizontal Mirror step size ";
    const char* setVstep_doc = "set vertical   Mirror step size ";

    const char* moveUp_doc   = "Move Laser Beam UP, by step 'step' (if too close to edge will not work)";
    const char* moveDown_doc = "Move Laser Beam DOWN, by step 'step' (if too close to edge will not work)";
    const char* moveLeft_doc   = "Move Laser Beam LEFT, by step 'step' (if too close to edge will not work)";
    const char* moveRight_doc = "Move Laser Beam RIGHT, by step 'step' (if too close to edge will not work)";


    const char* stabilisation_status_doc
        = "Status (ON/OFF) of laser transport system stabilisation system.";
    const char* energy_doc
        = "Laser energy set-point (AU) add more detail when we have it.";
    const char* status_doc  = "Status (ON/OFF) of laser system.";
    const char* setCharge_doc2 = "Charge (pc) set point.";

    const char* getQ_doc        = "getQ_doc.";
    const char* getQBuffer_doc  = "getQBuffer_doc.";
    const char* clearLaserRunningValues_doc  = "getQBuffer_doc.";

    const char* setVCPosition_doc  = "setVCPosition_doc, to position 'horizonal' and 'vertical' in mm.";
    const char* getSetVCPosState_doc  = "getSetVCPosState_doc, gets current status of SetVCPosState.";

    const char* isSettingPosition_doc  = "returns true if the feedback is currently trying to set a position.";

    /*
        some function pointers for exposing overloads to python
    */

    bool(pilaserController::*setVCPos_1)(double, double)         = &pilaserController::setVCPos;
    bool(pilaserController::*setVCPos_2)(double, double, double, double, double, double,size_t, size_t, size_t, size_t) = &pilaserController::setVCPos;

    const char* setVCPos_doc1  = "setVCPos using the default parameters.";
    const char* setVCPos_doc2  = "setVCPos with full control of all parameters: "
                                  "xpos (to move to), ypos (to move to), "
                                  "x_prec (tolerance for x), y_prec (tolerance for y), "
                                  "mirror_step_x_0 (initial mirror stepsize in horizontal), "
                                  "mirror_step_y_0 (initial mirror stepsize in vertical ), "
                                  "num_points_x (number of points to take position average), "
                                  "num_points_y (number of points to take position average), "
                                  "max_it (maximum numbe of iterations before stopping), "
                                  "time_out( macimum time (seconds) before quitting)";



    class_<pilaserController, bases<cameraControllerBase,shutterController>, noncopyable>
    //class_<pilaserController, bases<cameraControllerBase>, noncopyable>
        ("pilaserController","pilaserController member functions", no_init )
        .def("getHWP",&pilaserController::getHWP,getHWP_doc)
        .def("setHWP",&pilaserController::setHWP,(VALUE_ARG),setHWP_doc)
        .def("setCharge",&pilaserController::setCharge,(VALUE_ARG), setCharge_doc)

        .def("setEnergy",&pilaserController::setEnergy, setEnergy_doc)

//        .def("getEnergy",&pilaserController::getEnergy, getEnergy_doc)
//        .def("getEnergyBuffer",&pilaserController::getEBuffer_Py, getEnergy_doc)

//
//        .def("getStatus",&pilaserController::getStatus, getStatus_doc)
//        .def("getStabilisationStatus",&pilaserController::getStabilisationStatus,getStabilisationStatus_doc)
//        .def("isOn",&pilaserController::isOn,isOn_doc)
//        .def("isOff",&pilaserController::isOff,isOff_doc)
//        .def("isStabilisationOff",&pilaserController::isStabilisationOff,isStabilisationOff_doc)
//        .def("isStabilisationOn",&pilaserController::isStabilisationOn,isStabilisationOn_doc)
//        .def("disableStabilisation",&pilaserController::disableStabilisation,disableStabilisation_doc)
//        .def("enableStabilisation",&pilaserController::enableStabilisation,enableStabilisation_doc)
//        .def("getHpos", &pilaserController::getHpos, getHpos_doc )
//        .def("getVpos", &pilaserController::getVpos, getVpos_doc )
//        .def("setHpos", &pilaserController::setHpos,(VALUE_ARG), setHpos_doc )
//        .def("setVpos", &pilaserController::setVpos,(VALUE_ARG), setVpos_doc )
//        .def("getHstep",&pilaserController::getHstep,getHstep_doc)
//        .def("getVstep",&pilaserController::getVstep,getVstep_doc)
//        .def("setHstep",&pilaserController::setHstep,(VALUE_ARG),setVstep_doc)
//        .def("setVstep",&pilaserController::setVstep,(VALUE_ARG),setHstep_doc)
//
//        .def("getSetVCPosState",&pilaserController::getSetVCPosState, getSetVCPosState_doc)
//
//        .def("moveLeft",  &pilaserController::moveLeft,(VALUE_ARG),   moveLeft_doc   )
//        .def("moveRight", &pilaserController::moveRight,(VALUE_ARG), moveRight_doc   )
//
//        .def("moveUp",  &pilaserController::moveUp, (VALUE_ARG),  moveUp_doc   )
//        .def("moveDown", &pilaserController::moveDown,(VALUE_ARG), moveDown_doc   )
//
//        .def("clearRunningValues",   &pilaserController::clearRunningValues,   clearLaserRunningValues_doc )
//
//        .def("setVCPos",   setVCPos_1,   setVCPos_doc1 )
//        .def("setVCPos",   setVCPos_2, ( boost::python::arg("xpos"),
//                                         boost::python::arg("x_prec"),
//                                         boost::python::arg("y_prec"),
//                                         boost::python::arg("mirror_step_x_0"),
//                                         boost::python::arg("mirror_step_y_0"),
//                                         boost::python::arg("num_points_x"),
//                                         boost::python::arg("num_points_y"),
//                                         boost::python::arg("max_it"),
//                                         boost::python::arg("time_out")), setVCPos_doc2 )
//
//
//        .def("isSettingPosition",   &pilaserController::isSettingPosition,   isSettingPosition_doc )
//
//        .def("getQ",        &pilaserController::getQ,          getQ_doc)
//        .def("getQBuffer",  &pilaserController::getQBuffer_Py, getQBuffer_doc)
//
//
//        .def("getVCDataObjConstRef",     &cameraControllerBase::getAnalysisObj_VC, "",return_value_policy<reference_existing_object>())
//        .def("getpilMirrorObjConstRef",  &pilaserController::getpilMirrorObjConstRef, "",return_value_policy<reference_existing_object>())
//        .def("getPILObjConstRef",  &pilaserController::getPILObjConstRef, "",return_value_policy<reference_existing_object>())
        ;
//
    // The main class that creates all the controller obejcts
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


//////////////////////////////////    // pilaser object struct to be exposed, used when returning a pilaser reference
//////////////////////////////////    boost::python::class_<pilaserStructs::pilaserObject,boost::noncopyable>
//////////////////////////////////        ("pilaserObject","pilaserObject Doc String", boost::python::no_init)
//////////////////////////////////        .def_readonly("name",      &pilaserStructs::pilaserObject::name,
//////////////////////////////////                      "The name of the PI laser object")
//////////////////////////////////        .def_readonly("pvRoot",    &pilaserStructs::pilaserObject::pvRoot,
//////////////////////////////////                      "The PV root name, prefixed to all PVs")
//////////////////////////////////        .def_readonly("hPos",      &pilaserStructs::pilaserObject::hPos,
//////////////////////////////////                      "The horizontal position of the laser on the cathode [mm hopefully]")
//////////////////////////////////        .def_readonly("vPos",      &pilaserStructs::pilaserObject::vPos,
//////////////////////////////////                      "The vertical position of the laser on the cathode [mm hopefully]"
//////////////////////////////////        .def_readonly("energy", &pilaserStructs::pilaserObject::energy,
//////////////////////////////////                      "The energy of the laser on the cathode [mJ ??]")
////////////////////////////////////        .def_readonly("numIlocks", &pilaserStructs::pilaserObject::numIlocks,
////////////////////////////////////                      "The number of standard interlocks on the laser system")
//////////////////////////////////        ;
////////////////
//////////////////    boost::python::class_<pilaserController, boost::python::bases<controller>, boost::noncopyable>
//////////////////        ("pilaserController",
//////////////////
////////////////// "<=================================================>\n"
////////////////// " _____ _____   _                                   \n"
////////////////// "|  __ \\_   _| | |                                 \n"
////////////////// "| |__) || |   | |     __ _ ___  ___ _ __           \n"
////////////////// "|  ___/ | |   | |    / _` / __|/ _ \\ '__|         \n"
////////////////// "| |    _| |_  | |___| (_| \\__ \\  __/ |           \n"
////////////////// "_|___|_____| |______\\__,_|___/\\___|_|            \n"
////////////////// " / ____|          | |           | | |              \n"
////////////////// "| |     ___  _ __ | |_ _ __ ___ | | | ___ _ __     \n"
////////////////// "| |    / _ \\| '_ \\| __| '__/ _ \\| | |/ _ \\ '__|\n"
////////////////// "| |___| (_) | | | | |_| | | (_) | | |  __/ |       \n"
////////////////// " \\_____\\___/|_| |_|\\__|_|  \\___/|_|_|\\___|_|  \n"
////////////////// "| |  | |    | |                                    \n"
////////////////// "| |__| | ___| |_ __        DOCUMENTATION           \n"
////////////////// "|  __  |/ _ \\ | '_ \\           &                 \n"
////////////////// "| |  | |  __/ | |_) |          HINTS               \n"
////////////////// "|_|  |_|\\___|_| .__/                              \n"
////////////////// "              | |      Duncan Scott March 2017     \n"
////////////////// "              |_|                                  \n"
////////////////// "\n"
////////////////// "=> The object to control the PhotoInjector Laser   \n"
////////////////// "\n"
////////////////// "=> Use the set and get methods for values and getPILObjConstRef to return a reference to the PILaser data Object"
////////////////// "\n"
////////////////// "<=================================================>\n"
//////////////////
//////////////////         , boost::python::no_init)
//////////////////        .def("getILockStates",    &pilaserController::getILockStates,
//////////////////                                  "Return the state of interlocks as an integer. There Currently NO epics ilocks for the PI laser.")
//////////////////        .def("getILockStatesStr", &pilaserController::getILockStatesStr,
//////////////////                                  "Return state of interlocks as a stringr. There Currently NO epics ilocks for the PI laser.")
//////////////////        .def("get_CA_PEND_IO_TIMEOUT",  &pilaserController::get_CA_PEND_IO_TIMEOUT,
//////////////////                                        "Return the current waiting time [seconds] when sending commands to EPICS.")
//////////////////        .def("set_CA_PEND_IO_TIMEOUT",  &pilaserController::set_CA_PEND_IO_TIMEOUT,
//////////////////                                        (boost::python::arg("time"),
//////////////////                                        "Set a new waiting time [seconds] when sending commands to EPICS"))
//////////////////        .def("getHpos", &pilaserController::getHpos,
//////////////////                            "returns the horizontal position of the laser on the cathode [units etc?]."
//////////////////        .def("getVpos", &pilaserController::getVpos,
//////////////////                            "returns the vertical position of the laser on the cathode [units etc?]."
//////////////////        .def("getIntensity", &pilaserController::getIntensity,
//////////////////                            "returns the energy the laser [units etc?]."
//////////////////        .def("getPILObjConstRef",  &pilaserController::getPILObjConstRef,
//////////////////                                   return_value_policy<reference_existing_object>(),
//////////////////                            "returns a reference to a PI laser object, (identified by its name)."
//////////////////        .def("setHpos",  setHpos_1,
//////////////////                            "set the horizontal position of the laser beam on the cathode."
//////////////////        .def("setHpos",  setHpos_2,
//////////////////             "set the horizontal position of the laser beam on the cathode."
//////////////////        .def("setVpos",  setVpos_1,"set the vertical position of the laser beam on the cathode."
//////////////////        .def("setVpos",  setVpos_2,"set the vertical position of the laser beam on the cathode."
//////////////////        .def("setIntensity",  setIntensity_1,"set the energy of the laser beam on the cathode.")
//////////////////        .def("setIntensity",  setIntensity_2,"set the energy of the laser beam on the cathode.")
//////////////////        ;

//https://wiki.python.org/moin/boost.python/FAQ
//#include /libs/python/test/module_tail.cpp
}

#endif // _VC_PILASER_H_
