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
/*
    vela_clara_enums is a .pyd that contains all the
    enums that are shared amongst many HWC. In Python programmes
    that use many HWC it can(should) be imported first
    in every file so that only the vela_clara_enums namespace
    has the defintions. (making accesing this enums consistant
    throughout the python programme.
*/
// project includes
#include "structs.h"
#ifndef __VC_ENUM_ONLY__
#include "VCbase.h"
#include "baseObject.h"
#include "controller.h"
#include "interface.h"
#include "structs.h"
#endif // __VC_ENUM_ONLY__
// stl includes
#include <string>
#include <vector>
//______________________________________________________________________________
// boost.python includes
/// !!!IF YOU SIGNIFCANTLY CHANGE THIS FILE YOU MIGHT
/// RECOMPILE EVERY HARDWARE CONTROLLER!!!
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#define BOOST_PYTHON_STATIC_LIB

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/docstring_options.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/manage_new_object.hpp>
#ifndef __VC_ENUM_ONLY__
#endif // __VC_ENUM_ONLY__
/* some typdefs */
using doub = double;
using cdou = const double;
using vecd = std::vector<double>;
using vvcd = std::vector<std::vector<double>>;
using cved = const std::vector<double>;
using csiz = const size_t;
using size = size_t;
using vsiz = std::vector<size_t>;
using stri = std::string;
using cstr = const std::string;
using vecs = std::vector<std::string>;
using cves = const std::vector<std::string>;
//______________________________________________________________________________
using namespace boost::python;
namespace BOOST_PYTHON_INCLUDE
{
    /*
        Things that you want to use in python must be exposed:
    */
    /*  these functions expose objects that can be defined in mulitple HWC.
        Without this implementation you will get a Python warning that the
        objects have been defined multiple times
    */
//    template<typename T>
//    void reg_class(const char* py_name, const char* py_docs, class_<T>& def,
//                   boost::python::type_info& info)
//    {
//        /* info on the type we are exposing */
////        boost::python::type_info info = type_id<T>();
//        const converter::registration* reg  = converter::registry::query(info);
//        /*  if the class not been registered, register it */
//        if(reg == nullptr)
//        {
//            def;
//        }
//        else if ((*reg).m_to_python == nullptr)
//        {
//            def;
//        }
//    }
    template<typename T>
    void reg_enum(const char* py_name, const char* py_docs, enum_<T>& def,
                  boost::python::type_info& info)
    {
        /* info on the type we are exposing */
//        boost::python::type_info info = type_id<T>();
        const converter::registration* reg = converter::registry::query(info);
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
            been defined multiple times
        */
        /* the name of the object as it will appear in Python*/
        const char * py_name;
        /* the object docstring as it will appear in Python */
        const char * py_docs;
        //
        // expose std::vector<std::string>
        py_name = "std_vector_string";
        py_docs = "c++ standard vector of standard strings";
        //
        // Expose HWC_ENUM enums
        using namespace HWC_ENUM;

        // MACHINE_MODE
        py_name = "MACHINE_MODE";
        py_docs = "MACHINE_MODE: an enum for different machine modes, virtual, "
                  "physical or offline(no EPICS)";
//        enum_<MACHINE_MODE> mm_def =
//            enum_<MACHINE_MODE>(py_name,py_docs)
//                .value("PHYSICAL", MACHINE_MODE::PHYSICAL)
//                .value("OFFLINE",  MACHINE_MODE::OFFLINE )
//                .value("VIRTUAL",  MACHINE_MODE::VIRTUAL )
//                ;
        boost::python::type_info info1 = type_id<MACHINE_MODE>();
        const converter::registration* reg1  = converter::registry::query(info1);
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            enum_<MACHINE_MODE>(py_name,py_docs)
                .value("PHYSICAL", MACHINE_MODE::PHYSICAL)
                .value("OFFLINE",  MACHINE_MODE::OFFLINE )
                .value("VIRTUAL",  MACHINE_MODE::VIRTUAL )
                ;
        }
        else if((*reg1).m_to_python == nullptr)
        {
            enum_<MACHINE_MODE>(py_name,py_docs)
                .value("PHYSICAL", MACHINE_MODE::PHYSICAL)
                .value("OFFLINE",  MACHINE_MODE::OFFLINE )
                .value("VIRTUAL",  MACHINE_MODE::VIRTUAL )
                ;
        }

        //reg_enum<MACHINE_MODE>(py_name, py_docs, mm_def,info1);

        // MACHINE_AREA
        py_name = "MACHINE_AREA";
        py_docs = "MACHINE_AREA: an enum for named areas in the machine";
//        enum_<MACHINE_AREA> ma_def =
//            enum_<MACHINE_AREA>(py_name,py_docs)
//                .value("UNKNOWN_AREA", MACHINE_AREA::UNKNOWN_AREA)
//                .value("CLARA_2_VELA", MACHINE_AREA::CLARA_2_VELA)
//                .value("CLARA_INJ",    MACHINE_AREA::CLARA_INJ   )
//                .value("CLARA_PH1",    MACHINE_AREA::CLARA_PH1   )
//                .value("CLARA_S01",    MACHINE_AREA::CLARA_S01   )
//                .value("CLARA_S02",    MACHINE_AREA::CLARA_S02   )
//                .value("CLARA_L01",    MACHINE_AREA::CLARA_L01   )
//                .value("VELA_INJ",     MACHINE_AREA::VELA_INJ    )
//                .value("VELA_BA2",     MACHINE_AREA::VELA_BA2    )
//                .value("VELA_BA1",     MACHINE_AREA::VELA_BA1    )
//                .value("RF_GUN",       MACHINE_AREA::RF_GUN      )
//                .value("RF_L01",       MACHINE_AREA::RF_L01      )
//                .value("USER",         MACHINE_AREA::USER        )
//                ;
        boost::python::type_info info2 = type_id<MACHINE_AREA>();
        const converter::registration* reg2  = converter::registry::query(info2);
        if(reg2 == nullptr)
        {
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
                .value("RF_GUN",       MACHINE_AREA::RF_GUN      )
                .value("RF_L01",       MACHINE_AREA::RF_L01      )
                .value("USER",         MACHINE_AREA::USER        )
                ;
        }
        else if((*reg2).m_to_python == nullptr)
        {
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
                .value("RF_GUN",       MACHINE_AREA::RF_GUN      )
                .value("RF_L01",       MACHINE_AREA::RF_L01      )
                .value("USER",         MACHINE_AREA::USER        )
                ;
        }

        // CONTROLLER_TYPE
        py_name = "CONTROLLER_TYPE";
        py_docs = "CONTROLLER_TYPE: an enum for named hardware controllers";
//        enum_<CONTROLLER_TYPE> ct_def =
//            enum_<CONTROLLER_TYPE>(py_name,py_docs)
//                .value("UNKNOWN_CONTROLLER_TYPE",CONTROLLER_TYPE::UNKNOWN_CONTROLLER_TYPE)
//                .value("GENERAL_MONITOR",        CONTROLLER_TYPE::GENERAL_MONITOR)
//                .value("CAMERA_DAQ",             CONTROLLER_TYPE::CAMERA_DAQ)
//                .value("VAC_VALVES",             CONTROLLER_TYPE::VAC_VALVES)
//                .value("CAMERA_IA",              CONTROLLER_TYPE::CAMERA_IA)
//                .value("PI_LASER",               CONTROLLER_TYPE::PI_LASER)
//                .value("SHUTTER",                CONTROLLER_TYPE::SHUTTER)
//                .value("RF_PROT",                CONTROLLER_TYPE::RF_PROT)
//                .value("MAGNET",                 CONTROLLER_TYPE::MAGNET)
//                .value("SCREEN",                 CONTROLLER_TYPE::SCREEN)
//                .value("RF_MOD",                 CONTROLLER_TYPE::RF_MOD)
//                .value("SCOPE",                  CONTROLLER_TYPE::SCOPE)
//                .value("LLRF",                   CONTROLLER_TYPE::LLRF)
//                .value("IMG",                    CONTROLLER_TYPE::IMG)
//                .value("BPM",                    CONTROLLER_TYPE::BPM)
//                ;
        boost::python::type_info info3 = type_id<CONTROLLER_TYPE>();
        const converter::registration* reg3  = converter::registry::query(info3);
        if(reg3 == nullptr)
        {
            enum_<CONTROLLER_TYPE>(py_name,py_docs)
                .value("UNKNOWN_CONTROLLER_TYPE",CONTROLLER_TYPE::UNKNOWN_CONTROLLER_TYPE)
                .value("GENERAL_MONITOR",        CONTROLLER_TYPE::GENERAL_MONITOR)
                .value("CAMERA_DAQ",             CONTROLLER_TYPE::CAMERA_DAQ)
                .value("VAC_VALVES",             CONTROLLER_TYPE::VAC_VALVES)
                .value("CAMERA_IA",              CONTROLLER_TYPE::CAMERA_IA)
                .value("PI_LASER",               CONTROLLER_TYPE::PI_LASER)
                .value("SHUTTER",                CONTROLLER_TYPE::SHUTTER)
                .value("RF_PROT",                CONTROLLER_TYPE::RF_PROT)
                .value("MAGNET",                 CONTROLLER_TYPE::MAGNET)
                .value("SCREEN",                 CONTROLLER_TYPE::SCREEN)
                .value("RF_MOD",                 CONTROLLER_TYPE::RF_MOD)
                .value("SCOPE",                  CONTROLLER_TYPE::SCOPE)
                .value("LLRF",                   CONTROLLER_TYPE::LLRF)
                .value("IMG",                    CONTROLLER_TYPE::IMG)
                .value("BPM",                    CONTROLLER_TYPE::BPM)
                .value("CHARGE",                 CONTROLLER_TYPE::CHARGE)
                ;
        }
        else if((*reg3).m_to_python == nullptr)
        {
            enum_<CONTROLLER_TYPE>(py_name,py_docs)
                .value("UNKNOWN_CONTROLLER_TYPE",CONTROLLER_TYPE::UNKNOWN_CONTROLLER_TYPE)
                .value("GENERAL_MONITOR",        CONTROLLER_TYPE::GENERAL_MONITOR)
                .value("CAMERA_DAQ",             CONTROLLER_TYPE::CAMERA_DAQ)
                .value("VAC_VALVES",             CONTROLLER_TYPE::VAC_VALVES)
                .value("CAMERA_IA",              CONTROLLER_TYPE::CAMERA_IA)
                .value("PI_LASER",               CONTROLLER_TYPE::PI_LASER)
                .value("SHUTTER",                CONTROLLER_TYPE::SHUTTER)
                .value("RF_PROT",                CONTROLLER_TYPE::RF_PROT)
                .value("MAGNET",                 CONTROLLER_TYPE::MAGNET)
                .value("SCREEN",                 CONTROLLER_TYPE::SCREEN)
                .value("RF_MOD",                 CONTROLLER_TYPE::RF_MOD)
                .value("SCOPE",                  CONTROLLER_TYPE::SCOPE)
                .value("LLRF",                   CONTROLLER_TYPE::LLRF)
                .value("IMG",                    CONTROLLER_TYPE::IMG)
                .value("BPM",                    CONTROLLER_TYPE::BPM)
                .value("CHARGE",                 CONTROLLER_TYPE::CHARGE)
                ;
        }


        // ILOCK_STATE
        py_name = "ILOCK_STATE";
        py_docs = "ILOCK_STATE: an enum for named interlock states in the machine";
//        enum_<ILOCK_STATE> is_def =
//            enum_<ILOCK_STATE>("ILOCK_STATE")
//                .value("ILOCK_ERROR", ILOCK_STATE::ILOCK_ERROR)
//                .value("ILOCK_GOOD",  ILOCK_STATE::ILOCK_GOOD )
//                .value("ILOCK_BAD",   ILOCK_STATE::ILOCK_BAD  )
//                ;
        boost::python::type_info info4 = type_id<ILOCK_STATE>();
        const converter::registration* reg4  = converter::registry::query(info4);
        if(reg4 == nullptr)
        {
            enum_<ILOCK_STATE>("ILOCK_STATE")
                .value("ILOCK_ERROR", ILOCK_STATE::ILOCK_ERROR)
                .value("ILOCK_GOOD",  ILOCK_STATE::ILOCK_GOOD )
                .value("ILOCK_BAD",   ILOCK_STATE::ILOCK_BAD  )
                ;
        }
        else if((*reg4).m_to_python == nullptr)
        {
            enum_<ILOCK_STATE>("ILOCK_STATE")
                .value("ILOCK_ERROR", ILOCK_STATE::ILOCK_ERROR)
                .value("ILOCK_GOOD",  ILOCK_STATE::ILOCK_GOOD )
                .value("ILOCK_BAD",   ILOCK_STATE::ILOCK_BAD  )
                ;
        }


        // STATE
        py_name = "STATE";
        py_docs = "STATE: an enum for named states in the machine";
//        enum_<STATE> s_def =
//            enum_<STATE>("STATE")
//                .value("UNKNOWN",STATE::UNKNOWN)
//                .value("GOOD",   STATE::GOOD   )
//                .value("BAD",    STATE::BAD    )
//                .value("OFF",    STATE::OFF    )
//                .value("ERR",    STATE::ERR    )
//                .value("ON",    STATE::ON    )
//                ;
        boost::python::type_info info5 = type_id<STATE>();
        const converter::registration* reg5  = converter::registry::query(info5);
        if(reg5 == nullptr)
        {
            enum_<STATE>("STATE")
                .value("UNKNOWN",STATE::UNKNOWN)
                .value("GOOD",   STATE::GOOD   )
                .value("BAD",    STATE::BAD    )
                .value("OFF",    STATE::OFF    )
                .value("ERR",    STATE::ERR    )
                .value("ON",    STATE::ON    )
                .value("TIMING",    STATE::TIMING    )
                .value("OPEN",    STATE::OPEN    )
                .value("CLOSED",    STATE::CLOSED    )
                ;
        }
        else if((*reg5).m_to_python == nullptr)
        {
            enum_<STATE>("STATE")
                .value("UNKNOWN",STATE::UNKNOWN)
                .value("GOOD",   STATE::GOOD   )
                .value("BAD",    STATE::BAD    )
                .value("OFF",    STATE::OFF    )
                .value("ERR",    STATE::ERR    )
                .value("ON",    STATE::ON    )
                .value("TIMING",    STATE::TIMING    )
                .value("OPEN",    STATE::OPEN    )
                .value("CLOSED",    STATE::CLOSED    )
                ;
        }
/*
    vela_clara_enums is a .pyd that contains all the
    enums that are shared amongst many HWC. In Python programmes
    that use many HWC it can(should) be imported first
    in every file so that only the vela_clara_enums namespace
    has the defintions. (making accesing this enums consistant
    throughout the python programme.
*/
#ifndef __VC_ENUM_ONLY__
        //
        // expose std::vector<std::string>
        py_name = "std_vector_string";
        py_docs = "c++ standard vector of standard strings";
        /* the class definition, exposed to python, cf templated */
//        class_<vecs> vecs_def =
//            class_<vecs>(py_name, py_docs,no_init)
//                .def(vector_indexing_suite<vecs>())
//                ;
        boost::python::type_info info6 = type_id<vecs>();
        const converter::registration* reg6  = converter::registry::query(info6);
        /*  if the class not been registered, register it */
        if(reg6 == nullptr)
        {
            class_<vecs>(py_name, py_docs,no_init)
                .def(vector_indexing_suite<vecs>())
                ;
        }
        else if((*reg6).m_to_python == nullptr)
        {
            class_<vecs>(py_name, py_docs,no_init)
                .def(vector_indexing_suite<vecs>())
                ;
        }

        //
        // expose std::vector<double>
        py_name = "std_vector_double";
        py_docs = "c++ standard vector of doubles";
//        class_<vecd> vecd_def =
//            class_<vecd>(py_name, py_docs,no_init)
//                .def(vector_indexing_suite<vecd>())
//                ;
        boost::python::type_info info7 = type_id<vecd>();
        const converter::registration* reg7  = converter::registry::query(info7);
        /*  if the class not been registered, register it */
        if(reg7 == nullptr)
        {
            class_<vecd>(py_name, py_docs,no_init)
                .def(vector_indexing_suite<vecd>())
                ;
        }
        else if((*reg7).m_to_python == nullptr)
        {
            class_<vecd>(py_name, py_docs,no_init)
                .def(vector_indexing_suite<vecd>())
                ;
        }
        /*
            Expose base classes,  these should never be multiply defined
            no_int  forces Python to not construct (init) this object
            pure virtual methods get their own function
        */
        using namespace boost;
        //
        // expose baseObject
        py_name = "baseObject";
        py_docs = "at the base of it all";
        class_<baseObject,noncopyable> bobj_def =
            class_<baseObject,noncopyable>(py_name, py_docs, no_init)
                ;
        boost::python::type_info info8 = type_id<baseObject>();
        const converter::registration* reg8  = converter::registry::query(info8);
        /*  if the class not been registered, register it */
        if(reg8 == nullptr)
        {
            bobj_def;
        }
        else if((*reg8).m_to_python == nullptr)
        {
            bobj_def;
        }

        //
        // expose controller base,
        /*
           NB pure_virtual methods MUST be overloaded in derived classes
           document strings for VCbase functions
        */
        const char* isDebugMessageOn_doc ="Returns TRUE if debug message are enabled.";
        const char* isMessageOn_doc      ="Returns TRUE if message are enabled.";
        const char* isVerbose_doc        ="Returns TRUE if verbose mode is enabled.";
        const char* isSilent_doc         ="Returns TRUE if silent mode is enabled.";
        const char* getControllerType_doc="Returns controller type.";
        const char* debugMessagesOff_doc="Sets debug messages OFF for this controller";
        const char* debugMessagesOn_doc ="Sets debug messages ON for this controller";
        const char* messagesOn_doc      ="Sets messages ON for this controller";
        const char* messagesOff_doc     ="Sets messages OFF for this controller";
        const char* silence_doc         ="Sets silent mode for this controller";
        const char* verbose_doc         ="Sets verbose mode for this controller";
        const char* contName_doc        ="The name of the controller";
        const char* typeName_doc        ="The type of the controller";

        const char* controller_name = "controller";
        const char* controller_docs = "controller base class";
//        class_<controller,bases<baseObject>,noncopyable> cobj_def =
//            class_<controller, bases<baseObject>,noncopyable>(py_name,py_docs, no_init)
//                .def("get_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
//                .def("set_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
//                .def_readonly("type",   &controller::controllerType, typeName_doc)
//                .def_readonly("name",   &controller::name, contName_doc          )
//                .def("getControllerType",&controller::getControllerType,getControllerType_doc)
//                .def("isDebugMessageOn",&controller::isDebugMessageOn,isDebugMessageOn_doc   )
//                .def("isMessageOn",     &controller::isMessageOn,isMessageOn_doc             )
//                .def("isVerbose",       &controller::isVerbose,isVerbose_doc                 )
//                .def("isSilent",        &controller::isSilent,isSilent_doc                   )
//                .def("debugMessagesOn", &controller::debugMessagesOn,debugMessagesOn_doc     )
//                .def("debugMessagesOff",&controller::debugMessagesOff,debugMessagesOff_doc   )
//                .def("messagesOn",      &controller::messagesOn,messagesOn_doc               )
//                .def("messagesOff",     &controller::messagesOff,messagesOff_doc             )
//                .def("silence",         &controller::silence,silence_doc                     )
//                .def("verbose",         &controller::verbose,verbose_doc                     )
//                ;
        boost::python::type_info info9 = type_id<controller>();
        const converter::registration* reg9  = converter::registry::query(info9);
        /*  if the class not been registered, register it */
        if(reg9 == nullptr)
        {
            class_<controller, bases<baseObject>,noncopyable>(controller_name,controller_docs, no_init)
                .def("get_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
                .def("set_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
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
        }
        else if ((*reg9).m_to_python == nullptr)
        {
            class_<controller, bases<baseObject>,noncopyable>(controller_name,controller_docs, no_init)
                .def("get_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
                .def("set_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
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
        }

        //reg_class<controller>(py_name, py_docs, cobj_def,info9);
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

//        class_<VCbase,noncopyable> vcbobj_def =
//            class_<VCbase, noncopyable>("VCbase", no_init)
//                .def("setDebugMessage", &VCbase::setDebugMessage, setDebug_doc)
//                .def("setVerbose",      &VCbase::setVerbose,setVerb__doc      )
//                .def("setMessage",      &VCbase::setMessage,setMess__doc      )
//                .def("setQuiet",        &VCbase::setQuiet,setQuiet_doc        )
//                ;
        boost::python::type_info info10 = type_id<VCbase>();
        const converter::registration* reg10  = converter::registry::query(info10);
        /*  if the class not been registered, register it */
        if(reg10 == nullptr)
        {
            class_<VCbase, noncopyable>("VCbase", no_init)
                .def("setDebugMessage", &VCbase::setDebugMessage, setDebug_doc)
                .def("setVerbose",      &VCbase::setVerbose,setVerb__doc      )
                .def("setMessage",      &VCbase::setMessage,setMess__doc      )
                .def("setQuiet",        &VCbase::setQuiet,setQuiet_doc        )
                ;
        }
        else if ((*reg10).m_to_python == nullptr)
        {
            class_<VCbase, noncopyable>("VCbase", no_init)
                .def("setDebugMessage", &VCbase::setDebugMessage, setDebug_doc)
                .def("setVerbose",      &VCbase::setVerbose,setVerb__doc      )
                .def("setMessage",      &VCbase::setMessage,setMess__doc      )
                .def("setQuiet",        &VCbase::setQuiet,setQuiet_doc        )
                ;
        }
#endif //__VC_ENUM_ONLY__
    }
}
//______________________________________________________________________________
#endif // VCHEADER_H_INCLUDED
