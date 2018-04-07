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
class VCpilaser : VCbase
{
   public:
        VCpilaser();
        ~VCpilaser();

        pilaserController& virtual_PILaser_Controller();
        pilaserController& physical_PILaser_Controller();
        pilaserController& offline_PILaser_Controller();


        pilaserController& getpilaserController(pilaserController*& cont,
                                                const std::string & name,
                                                const bool shouldVM,
                                                const bool shouldEPICS,
                                                const std::string& pilaserConf,
                                                const std::string& vcAnalysisConf,
                                                const std::string& piLaserMirrorConf,
                                                const std::string& piLaserShutterConf);
    protected:
    private:
        /* object names */
        pilaserController* virtual_pilaser_Controller_Obj ;
        pilaserController* physical_pilaser_Controller_Obj;
        pilaserController* offline_pilaser_Controller_Obj ;

        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;
        const std::string pilaserConf, vcAnalysisConf,
                          piLaserMirrorConf,piLaserShutterConf;
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
//using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_CLARA_PILaserControl)
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();
    /*
        Things that you want to use in python muct be exposed:
        containers
    */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();
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
//    /// The main class that creates all the controller obejcts
//        boost::python::class_<VCpilaser,boost::noncopyable> ("init")
//        .def("virtual_PILaser_Controller",  &VCpilaser::virtual_PILaser_Controller,
//             return_value_policy<reference_existing_object>(),
//             "returns a reference to the virtual PI laser object.")
//        .def("physical_PILaser_Controller",  &VCpilaser::physical_PILaser_Controller,
//             return_value_policy<reference_existing_object>(),
//            "returns a reference to the physical PI laser object.")
//        .def("offline_PILaser_Controller",  &VCpilaser::offline_PILaser_Controller,
//             return_value_policy<reference_existing_object>(),
//             "returns a reference to the offline PI laser object.")
//        .def("setQuiet",         &VCpilaser::setQuiet,
//             "set Quiet Mode (no messages, no debug messages) for all PI laser objects."
//        .def("setVerbose",       &VCpilaser::setVerbose,
//             "set Verbose Mode (all messages, all debug messages) for all PI laser objects.")
//        .def("setMessage",       &VCpilaser::setMessage,
//             "set Message Mode (all  messages, no debug messages) for all PI laser objects.")
//        .def("setDebugMessage",  &VCpilaser::setDebugMessage,
//             "set Debug Mode (no messages, all debug messages) for all PI laser objects."
//        ;
}

#endif // _VC_PILASER_H_
