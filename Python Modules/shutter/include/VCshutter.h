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
//  FileName:    VCshutter.h
//  Description:
//
//
//*/
#ifndef _VC_PILASER_H_
#define _VC_PILASER_H_
//stl
#include <string>
//
#include "shutterController.h"
#include "expose_shutter.h"

#include "VCbase.h"
//______________________________________________________________________________
class VCshutter : public VCbase
{
   public:
        VCshutter();
        ~VCshutter();

        shutterController& virtual_PIL_Shutter_Controller();
        shutterController& physical_PIL_Shutter_Controller();
        shutterController& offline_PIL_Shutter_Controller();

    protected:
    private:
        /* object names */
        shutterController* virtual_PIL_shutter_Controller_Obj ;
        shutterController* physical_PIL_shutter_Controller_Obj;
        shutterController* offline_PIL_shutter_Controller_Obj ;

        const std::string pilaserConf, vcAnalysisConf,
                          piLaserMirrorConf,piLaserShutterConf;

        shutterController& getController(shutterController*& cont,
                                         const std::string & name,
                                         const bool shouldVM,
                                         const bool shouldEPICS,
                                         const std::string& piLaserShutterConf);

        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const shutterController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();
};
//______________________________________________________________________________
BOOST_PYTHON_MODULE(VELA_CLARA_Shutter_Control)
{

    BOOST_PYTHON_INCLUDE::expose_shutter();

    /// The main class that creates all the controller obejcts
    class_<VCshutter,bases<VCbase>,noncopyable> ("init","Doc string")
        .def("virtual_PIL_Shutter_Controller",  &VCshutter::virtual_PIL_Shutter_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual PI laser object.")
        .def("physical_PIL_Shutter_Controller",  &VCshutter::physical_PIL_Shutter_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical PI laser object.")
        .def("offline_PIL_Shutter_Controller",  &VCshutter::offline_PIL_Shutter_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline PI laser object.")
        ;

}
//    //using namespace boost::python;
//    docstring_options doc_options(true, false, false);
//    doc_options.disable_cpp_signatures();
//    /*
//        Things that you want to use in python muct be exposed:
//        containers
//    */
//    BOOST_PYTHON_INCLUDE::export_BaseObjects();
//
//    using namespace UTL;
//    using namespace boost::python;
//    using namespace boost;
//    using namespace shutterStructs;
//
//    enum_<SHUTTER_STATE>("SHUTTER_STATE","State of shutters")
//        .value("OPEN", SHUTTER_STATE::OPEN)
//        .value("CLOSED",  SHUTTER_STATE::CLOSED )
//        .value("TIMING",  SHUTTER_STATE::TIMING )
//        .value("ERROR",  SHUTTER_STATE::ERROR )
//        ;
//
////    const char* getShutterNames_Py_doc = "getShutterNames_Py_doc.";
////    const char* closeAndWait_doc = "closeAndWait_doc.";
////    const char* openAndWait_doc = "openAndWait_doc.";
////    const char* isClosed_doc = "isClosed_doc.";
////    const char* isOpen_doc = "isOpen_doc.";
////    const char* close_doc = "close_doc.";
////    const char* open_doc = "open_doc.";
////    const char* getState_doc = "getState_doc.";
//
////    const char* areAllOpen_doc = "areAllOpen_doc.";
////    const char* areAllClosed_doc = "areAllClosed_doc.";
////    const char* getShutterNames_doc = "getShutterNames_doc.";
//    const char* state_doc = "State (OPEN/CLOSED/TIMING etc.) of Shutter.";
//    const char* name_doc = "Shutter name.";
//
//    class_<shutterObject,noncopyable>
//        ("shutterObject","shutterObject member variables (read access only)", no_init)
//        .def_readonly("state",&shutterObject::state,state_doc)
//        .def_readonly("name",&shutterObject::name,name_doc)
//        ;
//
//    class_<shutterController, bases<controller>, noncopyable>
//        ("shutterController","shutterController Doc String",no_init)
//        .def("closeAndWait",  &shutterController::closeAndWait         )
//        .def("openAndWait", &shutterController::openAndWait)
//        .def("isClosed",   &shutterController::isClosed )
//        .def("isOpen",   &shutterController::isOpen )
//        .def("areAllOpen",   &shutterController::areAllOpen )
//        .def("areAllClosed",   &shutterController::areAllClosed )
//        .def("close",   &shutterController::close )
//        .def("open",   &shutterController::open )
//        .def("getShutterNames",   &shutterController::getShutterNames_Py )
//        .def("getShutterObjConstRef",   &shutterController::getShutterObjConstRef,return_value_policy<reference_existing_object>())
//        ;
//
//    /// The main class that creates all the controller obejcts
//    class_<VCshutter,bases<VCbase>,noncopyable> ("init","Doc string")
//        .def("virtual_PIL_Shutter_Controller",  &VCshutter::virtual_PIL_Shutter_Controller,
//             return_value_policy<reference_existing_object>(),
//             "returns a reference to the virtual PI laser object.")
//        .def("physical_PIL_Shutter_Controller",  &VCshutter::physical_PIL_Shutter_Controller,
//             return_value_policy<reference_existing_object>(),
//            "returns a reference to the physical PI laser object.")
//        .def("offline_PIL_Shutter_Controller",  &VCshutter::offline_PIL_Shutter_Controller,
//             return_value_policy<reference_existing_object>(),
//             "returns a reference to the offline PI laser object.")
//        ;
//
////https://wiki.python.org/moin/boost.python/FAQ
////#include /libs/python/test/module_tail.cpp
//}

#endif // _VC_PILASER_H_
