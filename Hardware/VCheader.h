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
//  Last edit:   19-03-2018
//  FileName:    VCheader.h
//  Description:
//
//
//*/
#ifndef VCHEADER_H_INCLUDED
#define VCHEADER_H_INCLUDED
// project includes
#include "VCbase.h"
#include "structs.h"
//______________________________________________________________________________
// boost.python includes
/// !!!IF YOU SIGNIFCANTLY CHANGE THIS FILE YOU MIGHT
/// RECOMPILE EVERY HARDWARE CONTROLLER!!!
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/docstring_options.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/manage_new_object.hpp>
/* some typdefs */
using doub = double ;
using cdou = const double;
using vecd = std::vector<double>;
using vvcd = std::vector<std::vector<double>> ;
using cved = const std::vector<double> ;
using csiz = const size_t ;
using size = size_t;
using vsiz = std::vector<size_t> ;
using stri = std::string ;
using cstr = const std::string ;
using vecs = std::vector<std::string> ;
using cves = const std::vector<std::string> ;
//______________________________________________________________________________
using namespace boost::python;
namespace BOOST_PYTHON_INCLUDE
{
    /* Things that you want to use in python must be exposed:
    */

    /*  these functions exposes objects that can be defined in mulitple HWC.
        Without this implementation you will get a Python warning that the
        objects have been defined multiple times
    */
    template<typename T>
    void reg_class(const char* py_name, const char* py_docs, class_<T> def)
    {
        /* info on the type we are exposing */
        boost::python::type_info info = type_id<T>();
        const converter::registration* reg;
        reg  = converter::registry::query(info);
        /*  if the class not been registered, register it */
        if(reg == nullptr)
        {
            def;
        }
        else if ((*reg).m_to_python == nullptr)
        {
            def;
        }
    }
    template<typename T>
    void reg_enum(const char* py_name, const char* py_docs, enum_<T> def)
    {
        /* info on the type we are exposing */
        boost::python::type_info info = type_id<T>();
        const converter::registration* reg;
        reg  = converter::registry::query(info);
        /*  if the class not been registered, register it */
        if(reg == nullptr)
        {
            def;
        }
        else if ((*reg).m_to_python == nullptr)
        {
            def;
        }
    }

    void export_BaseObjects()
    {
        /*  this function exposes objects that can be defined in mulitple HWC
            without this you will get a Python warning that the objects have
            been defined multiple time
        */

        /* the name of the object as it will appear in Python*/
        const char * py_name;
        /* the object docstring as it will appear in Python */
        const char * py_docs;
        //
        // expose std::vector<std::string>
        py_name = "std_vector_string";
        py_docs = "c++ standard vector of standard strings";
        /* the class definition, exposed to python, cf templated */
        class_<vecs> vecs_def =
            class_<vecs>(py_name, py_docs)
                .def(vector_indexing_suite<vecs>())
                ;
        reg_class<vecs>(py_name, py_docs, vecs_def);

        //
        // expose std::vector<double>
        py_name = "std_vector_double";
        py_docs = "c++ standard vector of doubles";
        class_<vecd> vecd_def =
            class_<vecd>(py_name, py_docs)
                .def(vector_indexing_suite<vecd>())
                ;
        reg_class<vecd>(py_name, py_docs, vecd_def);

        /* Expose VELA_ENUM enums */
        using namespace HWC_ENUM;

        // MACHINE_MODE
        py_name = "MACHINE_MODE";
        py_docs = "MACHINE_MODE: an enum for different machine modes, virtual, "
                  "physical or offline(no EPICS)";
        enum_<MACHINE_MODE> mm_def =
            enum_<MACHINE_MODE>(py_name,py_docs)
                .value("PHYSICAL", MACHINE_MODE::PHYSICAL)
                .value("OFFLINE",  MACHINE_MODE::OFFLINE )
                .value("VIRTUAL",  MACHINE_MODE::VIRTUAL )
                ;
        reg_enum<MACHINE_MODE>(py_name, py_docs, mm_def);

        // MACHINE_AREA
        py_name = "MACHINE_AREA";
        py_docs = "MACHINE_AREA: an enum for named areas in the machine";
        enum_<MACHINE_AREA> ma_def =
            enum_<MACHINE_AREA>(py_name,py_docs)
                .value("UNKNOWN_AREA", MACHINE_AREA::UNKNOWN_AREA)
                .value("CLARA_2_VELA", MACHINE_AREA::CLARA_2_VELA)
                .value("CLARA_INJ",    MACHINE_AREA::CLARA_INJ   )
                .value("CLARA_PH1",    MACHINE_AREA::CLARA_PH1   )
                .value("CLARA_S01",    MACHINE_AREA::CLARA_S01   )
                .value("CLARA_S02",    MACHINE_AREA::CLARA_S02   )
                .value("CLARA_L01",    MACHINE_AREA::CLARA_L01   )
                .value("VELA_INJ",     MACHINE_AREA::VELA_INJ    )
                .value("VELA_BA2",     MACHINE_AREA::VELA_BA2    )
                .value("VELA_BA1",     MACHINE_AREA::VELA_BA1    )
                .value("USER",         MACHINE_AREA::USER        )
                ;
        reg_enum<MACHINE_AREA>(py_name, py_docs, ma_def);

        // ILOCK_STATE
        py_name = "ILOCK_STATE";
        py_docs = "ILOCK_STATE: an enum for named interlock states in the machine";
        enum_<ILOCK_STATE> is_def =
            enum_<ILOCK_STATE>("ILOCK_STATE")
                .value("ILOCK_ERROR", ILOCK_STATE::ILOCK_ERROR)
                .value("ILOCK_GOOD",  ILOCK_STATE::ILOCK_GOOD )
                .value("ILOCK_BAD",   ILOCK_STATE::ILOCK_BAD  )
                ;
        reg_enum<ILOCK_STATE>(py_name, py_docs, is_def);

        // STATE
        py_name = "STATE";
        py_docs = "STATE: an enum for named states in the machine";
        enum_<STATE> s_def =
            enum_<STATE>("STATE")
                .value("UNKNOWN",STATE::UNKNOWN)
                .value("GOOD",   STATE::GOOD   )
                .value("BAD",    STATE::BAD    )
                .value("ERR",    STATE::ERR    )
                ;
        reg_enum<STATE>(py_name, py_docs, s_def);

        /*  Expose base classes,  these should never be multiply defined
            no_int  forces Python to not construct (init) this object
            pure virtual methods get their own function
        */
        using namespace boost;
        //
        // expose baseObject
        class_<baseObject, noncopyable>("baseObject", no_init)
            ;
        //
        // expose controller base,
        /* NB pure_virtual methods MUST be overloaded in derived classes
           document strings for VCbase functions */
        const char* isDebugMessageOn_doc ="Returns TRUE if debug message are enabled.";
        const char* isMessageOn_doc      ="Returns TRUE if message are enabled.";
        const char* isVerbose_doc        ="Returns TRUE if verbose mode is enabled.";
        const char* isSilent_doc         ="Returns TRUE if silent mode is enabled.";
        const char* getControllerType_doc="Returns controller type.";
        const char* debugMessagesOn_doc ="Sets debug messages ON for this controller";
        const char* debugMessagesOff_doc="Sets debug messages OFF for this controller";
        const char* messagesOn_doc      ="Sets messages ON for this controller";
        const char* messagesOff_doc     ="Sets messages OFF for this controller";
        const char* silence_doc         ="Sets silent mode for this controller";
        const char* verbose_doc         ="Sets verbose mode for this controller";
        const char* contName_doc        ="The name of the controller";
        const char* typeName_doc        ="The type of the controller";

        class_<controller, bases<baseObject>, noncopyable>("controller", no_init)
            .def("get_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
            .def("set_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
//            .def("getILockStatesStr",     pure_virtual(&controller::getILockStatesStr     ))
//            .def("getILockStates",        pure_virtual(&controller::getILockStates))

            .def_readonly("type",   &controller::controllerType, typeName_doc)
            .def_readonly("name",   &controller::name, contName_doc          )

            .def("getControllerType",&controller::getControllerType,getControllerType_doc)
            .def("isDebugMessageOn",&controller::isDebugMessageOn,isDebugMessageOn_doc   )
            .def("isMessageOn",     &controller::isMessageOn,isMessageOn_doc             )
            .def("isVerbose",       &controller::isVerbose,isVerbose_doc                 )
            .def("isSilent",        &controller::isSilent,isSilent_doc                   )
            .def("debugMessagesOn", &controller::debugMessagesOn,debugMessagesOn_doc     )
            .def("debugMessagesOff",&controller::debugMessagesOff,debugMessagesOff_doc   )
            .def("messagesOn",      &controller::messagesOn,messagesOn_doc               )
            .def("messagesOff",     &controller::messagesOff,messagesOff_doc             )
            .def("silence",         &controller::silence,silence_doc                     )
            .def("verbose",         &controller::verbose,verbose_doc                     )
            ;
        //
        // expose VCbase
        /* document strings for VCbase functions */
        const char* setQuiet_doc = "Set Quiet Mode: no messages or debug "
                                    "messages for all controllers in this "
                                    "module.";
        const char* setVerb__doc = "Set Verbose Mode: all messages and debug "
                                "messages for all controllers in this module.";
        const char* setMess__doc = "Set Message Mode: display only messages, no "
                                "debug messages,for all controllers in this module.";
        const char* setDebug_doc = "Set Debug  Mode: display only debug messages, "
                                "no messages, for all controllers in this module.";

        class_<VCbase, noncopyable>("VCbase", no_init)
            .def("setDebugMessage", &VCbase::setDebugMessage, setDebug_doc)
            .def("setVerbose",      &VCbase::setVerbose,setVerb__doc      )
            .def("setMessage",      &VCbase::setMessage,setMess__doc      )
            .def("setQuiet",        &VCbase::setQuiet,setQuiet_doc        )
            ;
    }
}
//______________________________________________________________________________
#endif // VCHEADER_H_INCLUDED
