#ifndef EXPOSE_SHUTTER_H_INCLUDED
#define EXPOSE_SHUTTER_H_INCLUDED

#include "VCheader.h"
#include "ba1StagesStructs.h"

using namespace boost::python;
using namespace boost;
using namespace UTL;
using namespace ba1StagesStructs;
namespace BOOST_PYTHON_INCLUDE
{

    void expose_ba1Stages()
    {
        /*
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        NO - NEVER put a docstrings option in a function like this
        Having multiple docstring_options causes a crash when importing to PYTHON!!!
        So we have to have some rule-of-thumb
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//        docstring_options doc_options(true, false, false);
//        doc_options.disable_cpp_signatures();
        */

        /*
            Things that you want to use in python must be exposed:
            containers
        */
        export_BaseObjects();


//        enum_<SHUTTER_STATE>("SHUTTER_STATE","State of ba1Stagess")
//            .value("CLOSED", SHUTTER_STATE::CLOSED)
//            .value("TIMING", SHUTTER_STATE::TIMING)
//            .value("ERROR" , SHUTTER_STATE::ERROR )
//            .value("OPEN"  , SHUTTER_STATE::OPEN  )
//            ;

    //    const char* getShutterNames_Py_doc = "getShutterNames_Py_doc.";
    //    const char* closeAndWait_doc = "closeAndWait_doc.";
    //    const char* openAndWait_doc = "openAndWait_doc.";
    //    const char* isClosed_doc = "isClosed_doc.";
    //    const char* isOpen_doc = "isOpen_doc.";
    //    const char* close_doc = "close_doc.";
    //    const char* open_doc = "open_doc.";
    //    const char* getState_doc = "getState_doc.";
    //    const char* areAllOpen_doc = "areAllOpen_doc.";
    //    const char* areAllClosed_doc = "areAllClosed_doc.";
    //    const char* getShutterNames_doc = "getShutterNames_doc.";

//        const char* shut_obj_doc = "ba1StagesObject member variables (currently read access only)";
//        const char* state_doc = "State (OPEN/CLOSED/TIMING/ERROR) of Shutter.";
//        const char* name_doc  = "Shutter name.";

//        class_<ba1StagesObject,noncopyable>
//            ("ba1StagesObject", "ba1StagesObject member variables (read access only)",no_init)
//            .def_readonly("state", &ba1StagesObject::state, state_doc)
//            .def_readonly("name" , &ba1StagesObject::name , name_doc )
//            ;

        class_<ba1StagesController, bases<controller>, noncopyable>
            ("ba1StagesController","ba1StagesController Doc String",no_init)


            .def("screenIn",   &ba1StagesController::screenIn,(UTL::NAME_ARG),"Moves stage to YAG IN position (if available)")
            .def("screenOut",   &ba1StagesController::screenOut,(UTL::NAME_ARG),"Moves stage to YAG OUTposition (if available)")
            .def("isReadPosEqualSetPos",   &ba1StagesController::isReadPosEqualSetPos,(UTL::NAME_ARG,UTL::VALUE_ARG),
                 "returns True if stage 'name' read position = the set position to within a tolerance of 'value'")
            .def("getStageNames",   &ba1StagesController::getStageNames_Py,"Returns names of stages")
            .def("getMinPos",   &ba1StagesController::getMinPos,(UTL::NAME_ARG),"Returns min position of stage 'name'")
            .def("getMaxPos",   &ba1StagesController::getMaxPos,(UTL::NAME_ARG),"Returns max position of stage 'name'")
            .def("getStageNames",   &ba1StagesController::getStageNames_Py,"Returns names of stages")
            .def("getStagePosition",   &ba1StagesController::getStagePosition,(UTL::NAME_ARG),"Returns read position for stage 'name'")
            .def("getStageSetPosition",   &ba1StagesController::getStageSetPosition,(UTL::NAME_ARG),"Returns set position for stage 'name'")
            .def("setStagePosition",   &ba1StagesController::setStagePosition,(UTL::NAME_ARG,UTL::DEVICE_ARG),"Sets stage 'name' to position for 'device'")
            .def("setDevice",   &ba1StagesController::setDevice,(UTL::NAME_ARG,UTL::VALUE_ARG),"Sets stage 'name' to position 'value'")
            .def("getDevices",   &ba1StagesController::getDevices_Py,(UTL::NAME_ARG),"Returns devices on stage 'name'")
            .def("getStageNumber",   &ba1StagesController::getStageNumber,(UTL::NAME_ARG),"Returns stage number for stage = 'name'")
            .def("getDevicePos",   &ba1StagesController::getDevicePos,(UTL::NAME_ARG,UTL::DEVICE_ARG),"Returns device position for stage = 'name' and device = device")
            .def("getStagePrecision",   &ba1StagesController::getStagePrecision,(UTL::NAME_ARG),"Returns stage precision (number of decimal places on set/read) for stage = 'name'")
            .def("getStageDeviceAndPositionMap",   &ba1StagesController::getStageDeviceAndPositionMap_Py,(UTL::NAME_ARG,UTL::VALUE_ARG),"returns dictionary of devices and positions for stage 'name'")
            ;
    }
}

#endif // EXPOSE_SHUTTER_H_INCLUDED
