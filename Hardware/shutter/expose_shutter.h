#ifndef EXPOSE_SHUTTER_H_INCLUDED
#define EXPOSE_SHUTTER_H_INCLUDED

#include "VCheader.h"
#include "shutterStructs.h"

using namespace boost::python;
using namespace boost;
using namespace UTL;
using namespace shutterStructs;
namespace BOOST_PYTHON_INCLUDE
{

    void expose_shutter()
    {
        docstring_options doc_options(true, false, false);
        doc_options.disable_cpp_signatures();
        /*
            Things that you want to use in python must be exposed:
            containers
        */
        export_BaseObjects();


        enum_<SHUTTER_STATE>("SHUTTER_STATE","State of shutters")
            .value("CLOSED", SHUTTER_STATE::CLOSED)
            .value("TIMING", SHUTTER_STATE::TIMING)
            .value("ERROR" , SHUTTER_STATE::ERROR )
            .value("OPEN"  , SHUTTER_STATE::OPEN  )
            ;

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

        const char* shut_obj_doc = "shutterObject member variables (currently read access only)";
        const char* state_doc = "State (OPEN/CLOSED/TIMING/ERROR) of Shutter.";
        const char* name_doc  = "Shutter name.";

        class_<shutterObject,noncopyable>
            ("shutterObject", no_init)
            .def_readonly("state""shutterObject member variables (read access only)", &shutterObject::state, state_doc)
            .def_readonly("name" , &shutterObject::name , name_doc )
            ;

        class_<shutterController, bases<controller>, noncopyable>
            ("shutterController","shutterController Doc String",no_init)
            .def("closeAndWait",  &shutterController::closeAndWait         )
            .def("openAndWait", &shutterController::openAndWait)
            .def("isClosed",   &shutterController::isClosed )
            .def("isOpen",   &shutterController::isOpen )
            .def("areAllOpen",   &shutterController::areAllOpen )
            .def("areAllClosed",   &shutterController::areAllClosed )
            .def("close",   &shutterController::close )
            .def("open",   &shutterController::open )
            .def("getShutterNames",   &shutterController::getShutterNames_Py )
            .def("getShutterObjConstRef",   &shutterController::getShutterObjConstRef,
                 return_value_policy<reference_existing_object>())
            ;


    }
}

#endif // EXPOSE_SHUTTER_H_INCLUDED
