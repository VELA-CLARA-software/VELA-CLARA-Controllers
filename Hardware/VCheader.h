/*
// This file is part of VELA-CLARA-Controllers.//
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or  //
//    (at your option) any later version.      //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of     //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      //
//    GNU General Public License for more details.          //
//      //
//    You should have received a copy of the GNU General Public License  //
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

#ifdef __INCLUDE_CAMERA_BASE__
#include "cameraControllerBase.h"
#define BOOST_LIB_NAME "boost_numpy"
#include <boost/config/auto_link.hpp>
#endif __INCLUDE_CAMERA_BASE__

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

/* some typdefs */
using doub = double;
using cdou = const double;
using vecd = std::vector<double>;
using cvus = const std::vector<unsigned short>;
using cvin = const std::vector<int>;
using vvcd = std::vector<std::vector<double>>;
using cved = const std::vector<double>;
using csiz = const size_t;
using size = size_t;
using vsiz = std::vector<size_t>;
using stri = std::string;
using cstr = const std::string;
using vecs = std::vector<std::string>;
using cves = const std::vector<std::string>;
using cus  = unsigned short;
using us   = unsigned short;
//______________________________________________________________________________
using namespace boost::python;
using namespace boost;
namespace BOOST_PYTHON_INCLUDE
{
    /*
        Things that you want to use in python must be exposed:
    */
    // Expose HWC_ENUM enums
    using namespace HWC_ENUM;
//______________________________________________________________________________
    void reg_MACHINE_MODE(const char* py_name, const char* py_docs)
    {
        enum_<MACHINE_MODE>(py_name,py_docs)
            .value("PHYSICAL", MACHINE_MODE::PHYSICAL)
            .value("OFFLINE",  MACHINE_MODE::OFFLINE )
            .value("VIRTUAL",  MACHINE_MODE::VIRTUAL )
            ;
    }
//______________________________________________________________________________
    void reg_MACHINE_AREA(const char* py_name, const char* py_docs)
    {
        enum_<MACHINE_AREA>(py_name,py_docs)
            .value("CLARA_2_BA1_BA2", MACHINE_AREA::CLARA_2_BA1_BA2 )
            .value("UNKNOWN_AREA",    MACHINE_AREA::UNKNOWN_AREA)
            .value("CLARA_2_VELA",    MACHINE_AREA::CLARA_2_VELA)
            .value("CLARA_2_BA1",     MACHINE_AREA::CLARA_2_BA1 )
            .value("CLARA_2_BA2",     MACHINE_AREA::CLARA_2_BA2 )
            .value("CLARA_INJ",       MACHINE_AREA::CLARA_INJ   )
            .value("CLARA_PH1",       MACHINE_AREA::CLARA_PH1   )
            .value("CLARA_S01",       MACHINE_AREA::CLARA_S01   )
            .value("CLARA_S02",       MACHINE_AREA::CLARA_S02   )
            .value("CLARA_L01",       MACHINE_AREA::CLARA_L01   )
            .value("VELA_INJ",        MACHINE_AREA::VELA_INJ    )
            .value("VELA_BA2",        MACHINE_AREA::VELA_BA2    )
            .value("VELA_BA1",        MACHINE_AREA::VELA_BA1    )
            .value("RF_GUN",          MACHINE_AREA::RF_GUN      )
            .value("RF_L01",          MACHINE_AREA::RF_L01      )
            .value("USER",            MACHINE_AREA::USER        )
            .value("PIL",             MACHINE_AREA::PIL         )
            ;
    }
//______________________________________________________________________________
    void reg_CONTROLLER_TYPE(const char* py_name, const char* py_docs)
    {
        enum_<CONTROLLER_TYPE>(py_name,py_docs)
            .value("UNKNOWN_CONTROLLER_TYPE",CONTROLLER_TYPE::UNKNOWN_CONTROLLER_TYPE)
            .value("GENERAL_MONITOR",        CONTROLLER_TYPE::GENERAL_MONITOR        )
            .value("CAMERA_DAQ",             CONTROLLER_TYPE::CAMERA_DAQ            )
            .value("VAC_VALVES",             CONTROLLER_TYPE::VAC_VALVES            )
            .value("CAMERA_IA",              CONTROLLER_TYPE::CAMERA_IA             )
            .value("FAST_CAM",               CONTROLLER_TYPE::FAST_CAM              )
            .value("PI_LASER",               CONTROLLER_TYPE::PI_LASER              )
            .value("SHUTTER",                CONTROLLER_TYPE::SHUTTER               )
            .value("RF_PROT",                CONTROLLER_TYPE::RF_PROT               )
            .value("CHARGE",                 CONTROLLER_TYPE::CHARGE                )
            .value("MAGNET",                 CONTROLLER_TYPE::MAGNET                )
            .value("CAMERA",                 CONTROLLER_TYPE::CAMERA                )
            .value("SCREEN",                 CONTROLLER_TYPE::SCREEN                )
            .value("RF_MOD",                 CONTROLLER_TYPE::RF_MOD                )
            .value("SCOPE",                  CONTROLLER_TYPE::SCOPE                 )
            .value("LLRF",                   CONTROLLER_TYPE::LLRF                  )
            .value("IMG",                    CONTROLLER_TYPE::IMG                   )
            .value("BPM",                    CONTROLLER_TYPE::BPM                   )
            ;
    }
//______________________________________________________________________________
    void reg_ILOCK_STATE_TYPE(const char* py_name, const char* py_docs)
    {
        enum_<ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_ERROR", ILOCK_STATE::ILOCK_ERROR)
            .value("ILOCK_GOOD",  ILOCK_STATE::ILOCK_GOOD )
            .value("ILOCK_BAD",   ILOCK_STATE::ILOCK_BAD  )
            ;
    }
//______________________________________________________________________________
    void reg_STATE_TYPE(const char* py_name, const char* py_docs)
    {
        enum_<STATE>("STATE")
            .value("UNKNOWN",STATE::UNKNOWN)
            .value("GOOD",   STATE::GOOD   )
            .value("BAD",    STATE::BAD    )
            .value("OFF",    STATE::OFF    )
            .value("ERR",    STATE::ERR    )
            .value("ON",     STATE::ON     )
            .value("TIMING", STATE::TIMING )
            .value("OPEN",   STATE::OPEN   )
            .value("CLOSED", STATE::CLOSED )
            ;
    }
#ifndef __VC_ENUM_ONLY__
//______________________________________________________________________________
    void reg_vec_string(const char* py_name, const char* py_docs)
    {
        class_<vecs>(py_name, py_docs,no_init)
            .def(vector_indexing_suite<vecs>())
            ;
    }
//______________________________________________________________________________
    void reg_vec_double(const char* py_name, const char* py_docs)
    {
        class_<vecd>(py_name, py_docs,no_init)
            .def(vector_indexing_suite<vecd>())
            ;
    }
//______________________________________________________________________________
    void reg_baseObject(const char* py_name, const char* py_docs)
    {
        class_<baseObject,noncopyable>(py_name, py_docs, no_init)
            ;
    }
//______________________________________________________________________________
    void reg_controller(const char* py_name, const char* py_docs)
    {
        const char* getControllerType_doc= "Returns controller type.";
        const char* debugMessagesOff_doc = "Sets debug messages OFF for this controller";
        const char* isDebugMessageOn_doc = "Returns TRUE if debug message are enabled.";
        const char* debugMessagesOn_doc  = "Sets debug messages ON for this controller";
        const char* messagesOff_doc      = "Sets messages OFF for this controller";
        const char* isMessageOn_doc      = "Returns TRUE if message are enabled.";
        const char* messagesOn_doc       = "Sets messages ON for this controller";
        const char* isVerbose_doc        = "Returns TRUE if verbose mode is enabled.";
        const char* contName_doc         = "The name of the controller";
        const char* typeName_doc         = "The type of the controller";
        const char* isSilent_doc         = "Returns TRUE if silent mode is enabled.";
        const char* silence_doc          = "Sets silent mode for this controller";
        const char* verbose_doc          = "Sets verbose mode for this controller";
        class_<controller, bases<baseObject>,noncopyable>(py_name,py_docs, no_init)
            .def("get_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
            .def("set_CA_PEND_IO_TIMEOUT",pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
            .def_readonly("type",    &controller::controllerType,    typeName_doc          )
            .def_readonly("name",    &controller::name,              contName_doc          )
            .def("getControllerType",&controller::getControllerType, getControllerType_doc )
            .def("debugMessagesOff", &controller::debugMessagesOff,  debugMessagesOff_doc  )
            .def("isDebugMessageOn", &controller::isDebugMessageOn,  isDebugMessageOn_doc  )
            .def("debugMessagesOn",  &controller::debugMessagesOn,   debugMessagesOn_doc   )
            .def("isMessageOn",      &controller::isMessageOn,       isMessageOn_doc       )
            .def("messagesOff",      &controller::messagesOff,       messagesOff_doc       )
            .def("messagesOn",       &controller::messagesOn,        messagesOn_doc        )
            .def("isVerbose",        &controller::isVerbose,         isVerbose_doc         )
            .def("isSilent",         &controller::isSilent,          isSilent_doc          )
            .def("silence",          &controller::silence,           silence_doc           )
            .def("verbose",          &controller::verbose,           verbose_doc           )
            ;
    }
//______________________________________________________________________________
    void reg_VCbase(const char* py_name, const char* py_docs)
    {
        /* document strings for VCbase functions */
        const char* setQuiet_doc =
        "Set Quiet Mode: no messages or debug messages for all controllers in this module.";
        const char* setVerb__doc =
        "Set Verbose Mode: all messages and debug messages for all controllers in this module.";
        const char* setMess__doc =
        "Set Message Mode: display only messages, no debug messages,for all controllers in this module.";
        const char* setDebug_doc =
        "Set Debug  Mode: display only debug messages, no messages, for all controllers in this module.";
        class_<VCbase, noncopyable>(py_name,py_docs, no_init)
           .def("setDebugMessage", &VCbase::setDebugMessage, setDebug_doc)
           .def("setVerbose",      &VCbase::setVerbose,setVerb__doc      )
           .def("setMessage",      &VCbase::setMessage,setMess__doc      )
           .def("setQuiet",        &VCbase::setQuiet,setQuiet_doc        )
           ;
    }
#endif
//______________________________________________________________________________
#ifdef __INCLUDE_CAMERA_BASE__
    void reg_analysis_mask(const char* py_name, const char* py_docs)
    {
        const char* mask_x_ds  = "";
        const char* name_ds  = "";
        const char* mask_y_ds  = "";
        const char* mask_x_rad_ds  = "";
        const char* mask_y_rad_ds  = "";
        const char* mask_x_def_ds  = "";
        const char* mask_y_def_ds  = "";
        const char* mask_x_rad_def_ds  = "";
        const char* mask_y_rad_def_ds  = "";

        using namespace cameraStructs;
        class_<analysis_mask,noncopyable>(py_name,py_docs, no_init)
//            .add_property("PSU",&magnetStructs::magnetObject::psuState,
//                                &magnetStructs::magnetObject::setPSU)
            .def_readonly("name",                &analysis_mask::name,name_ds )
            .def_readonly("mask_x",    &analysis_mask::mask_x,mask_x_ds)
            .def_readonly("mask_y",    &analysis_mask::mask_y,mask_y_ds)
            .def_readonly("mask_x_rad",    &analysis_mask::mask_x_rad,mask_x_rad_ds)
            .def_readonly("mask_y_rad",    &analysis_mask::mask_y_rad,mask_y_rad_ds)
            .def_readonly("mask_x_def",    &analysis_mask::mask_x_def,mask_x_def_ds)
            .def_readonly("mask_y_def",    &analysis_mask::mask_y_def,mask_y_def_ds)
            .def_readonly("mask_x_rad_def",    &analysis_mask::mask_x_rad_def,mask_x_rad_def_ds)
            .def_readonly("mask_y_rad_def",    &analysis_mask::mask_y_rad_def,mask_x_rad_def_ds)
            .add_property("MASKX",  &analysis_mask::mask_x,
                                    &analysis_mask::maskX,"Mask X setter/getter.")
            .add_property("MASKY",  &analysis_mask::mask_y,
                                    &analysis_mask::maskY,"Mask Y setter/getter.")

            .add_property("MASKXRAD", &analysis_mask::mask_x_rad,
                                      &analysis_mask::maskXrad,"Mask X radius setter/getter.")
            .add_property("MASKYRAD", &analysis_mask::mask_y_rad,
                                      &analysis_mask::maskYrad,"Mask Y radius setter/getter.")

            ;
    }
    void reg_analysis_data(const char* py_name,const char*  py_docs)
    {
        const char* name_ds  = "";
        const char* avg_pix_ds  = "";
        const char* sum_pix_ds  = "";
        const char* x_ds  = "";
        const char* y_ds  = "";
        const char* sig_x_ds  = "";
        const char* sig_y_ds  = "";
        const char* sig_xy_ds  = "";
        const char* x_pix_ds  = "";
        const char* y_pix_ds  = "";
        const char* sig_x_pix_ds  = "";
        const char* sig_y_pix_ds  = "";
        const char* sig_xy_pix_ds  = "";


        const char* x_mean_ds = "";
        const char* y_mean_ds = "";
        const char* sig_x_mean_ds = "";
        const char* sig_y_mean_ds = "";
        const char* sig_xy_mean_ds = "";
        const char* x_pix_mean_ds = "";
        const char* sig_x_pix_mean_ds = "";
        const char* sig_y_pix_mean_ds = "";
        const char* sig_xy_pix_mean = "";
        const char* sum_pix_mean_ds = "";
        const char* avg_pix_mean_ds = "";

        const char* x_sd_ds = "";
        const char* y_sd_ds = "";
        const char* sig_x_sd_ds = "";
        const char* sig_y_sd_ds = "";
        const char* sig_xy_sd_ds = "";
        const char* x_pix_sd_ds = "";
        const char* sig_x_pix_sd_ds = "";
        const char* sig_y_pix_sd_ds = "";
        const char* sig_xy_pix_sd = "";
        const char* sum_pix_sd_ds = "";
        const char* avg_pix_sd_ds = "";

        const char* x_clear_ds = "";
        const char* y_clear_ds = "";
        const char* sig_x_clear_ds = "";
        const char* sig_y_clear_ds = "";
        const char* sig_xy_clear_ds = "";
        const char* x_pix_clear_ds = "";
        const char* sig_x_pix_clear_ds = "";
        const char* sig_y_pix_clear_ds = "";
        const char* sig_xy_pix_clear_ds = "";
        const char* sum_pix_clear_ds = "";
        const char* avg_pix_clear_ds = "";

        const char* x_n_ds = "";
        const char* y_n_ds = "";
        const char* sig_x_n_ds = "";
        const char* sig_y_n_ds = "";
        const char* sig_xy_n_ds = "";
        const char* x_pix_n_ds = "";
        const char* sig_x_pix_n_ds = "";
        const char* sig_y_pix_n_ds = "";
        const char* sig_xy_pix_n_ds = "";
        const char* sum_pix_n_ds = "";
        const char* avg_pix_n_ds = "";

        const char* x_full_ds = "";
        const char* y_full_ds = "";
        const char* sig_x_full_ds = "";
        const char* sig_y_full_ds = "";
        const char* sig_xy_full_ds = "";
        const char* x_pix_full_ds = "";
        const char* sig_x_pix_full_ds = "";
        const char* sig_y_pix_full_ds = "";
        const char* sig_xy_pix_full_ds = "";
        const char* sum_pix_full_ds = "";
        const char* avg_pix_full_ds = "";

        const char* step_size_ds = "";

        using namespace cameraStructs;
        class_<analysis_data,noncopyable>(py_name,py_docs, no_init)
//            .add_property("PSU",&magnetStructs::magnetObject::psuState,
//                                &magnetStructs::magnetObject::setPSU)
            .def_readonly("name",                &analysis_data::name,name_ds )
            .def_readonly("avg_pix",              &analysis_data::avg_pix)
            .def_readonly("sum_pix",              &analysis_data::sum_pix)

            .def_readonly("x",              &analysis_data::x,x_ds)
            .def_readonly("y",              &analysis_data::y,y_ds)
            .def_readonly("sig_x",          &analysis_data::sig_x,sig_x_ds)
            .def_readonly("sig_y",          &analysis_data::sig_y,sig_y_ds)
            .def_readonly("sig_xy",         &analysis_data::sig_xy    ,sig_xy_ds    )

            .def_readonly("x_pix",          &analysis_data::x_pix     ,x_pix_ds     )
            .def_readonly("y_pix",          &analysis_data::y_pix     ,y_pix_ds     )
            .def_readonly("sig_x_pix",      &analysis_data::sig_x_pix ,sig_x_pix_ds )
            .def_readonly("sig_y_pix",      &analysis_data::sig_y_pix ,sig_y_pix_ds )
            .def_readonly("sig_xy_pix",     &analysis_data::sig_xy_pix,sig_xy_pix_ds)

            .def_readonly("x_mean",         &analysis_data::x_mean, x_mean_ds)
            .def_readonly("y_mean",         &analysis_data::y_mean, y_mean_ds)
            .def_readonly("sig_x_mean",     &analysis_data::sig_x_mean, sig_x_mean_ds)
            .def_readonly("sig_y_mean",     &analysis_data::sig_y_mean,sig_y_mean_ds)
            .def_readonly("sig_xy_mean",    &analysis_data::sig_xy_mean,sig_xy_mean_ds)
            .def_readonly("x_pix_mean",     &analysis_data::x_pix_mean,x_pix_mean_ds)
            .def_readonly("sig_x_pix_mean", &analysis_data::sig_x_pix_mean,sig_x_pix_mean_ds)
            .def_readonly("sig_y_pix_mean", &analysis_data::sig_y_pix_mean,sig_y_pix_mean_ds)
            .def_readonly("sig_xy_pix_mean",&analysis_data::sig_xy_pix_mean,sig_xy_pix_mean)
            .def_readonly("sum_pix_mean",   &analysis_data::sum_pix_mean,sum_pix_mean_ds)
            .def_readonly("avg_pix_mean",   &analysis_data::avg_pix_mean,avg_pix_mean_ds)

            .def_readonly("x_sd",         &analysis_data::x_sd, x_sd_ds)
            .def_readonly("y_sd",         &analysis_data::y_sd, y_sd_ds)
            .def_readonly("sig_x_sd",     &analysis_data::sig_x_sd, sig_x_sd_ds)
            .def_readonly("sig_y_sd",     &analysis_data::sig_y_sd,sig_y_sd_ds)
            .def_readonly("sig_xy_sd",    &analysis_data::sig_xy_sd,sig_xy_sd_ds)
            .def_readonly("x_pix_sd",     &analysis_data::x_pix_sd,x_pix_sd_ds)
            .def_readonly("sig_x_pix_sd", &analysis_data::sig_x_pix_sd,sig_x_pix_sd_ds)
            .def_readonly("sig_y_pix_sd", &analysis_data::sig_y_pix_sd,sig_y_pix_sd_ds)
            .def_readonly("sig_xy_pix_sd",&analysis_data::sig_xy_pix_sd,sig_xy_pix_sd)
            .def_readonly("sum_pix_sd",   &analysis_data::sum_pix_sd,sum_pix_sd_ds)
            .def_readonly("avg_pix_sd",   &analysis_data::avg_pix_sd,avg_pix_sd_ds)
            .def_readonly("x_sd",         &analysis_data::x_sd, x_sd_ds)

            .def_readonly("y_clear",         &analysis_data::y_clear, y_clear_ds)
            .def_readonly("sig_x_clear",     &analysis_data::sig_x_clear, sig_x_clear_ds)
            .def_readonly("sig_y_clear",     &analysis_data::sig_y_clear,sig_y_clear_ds)
            .def_readonly("sig_xy_clear",    &analysis_data::sig_xy_clear,sig_xy_clear_ds)
            .def_readonly("x_pix_clear",     &analysis_data::x_pix_clear,x_pix_clear_ds)
            .def_readonly("sig_x_pix_clear", &analysis_data::sig_x_pix_clear,sig_x_pix_clear_ds)
            .def_readonly("sig_y_pix_clear", &analysis_data::sig_y_pix_clear,sig_y_pix_clear_ds)
            .def_readonly("sig_xy_pix_clear",&analysis_data::sig_xy_pix_clear,sig_xy_pix_clear_ds)
            .def_readonly("sum_pix_clear",   &analysis_data::sum_pix_clear,sum_pix_clear_ds)
            .def_readonly("avg_pix_clear",   &analysis_data::avg_pix_clear,avg_pix_clear_ds)

            .def_readonly("y_n",         &analysis_data::y_n, y_n_ds)
            .def_readonly("sig_x_n",     &analysis_data::sig_x_n, sig_x_n_ds)
            .def_readonly("sig_y_n",     &analysis_data::sig_y_n,sig_y_n_ds)
            .def_readonly("sig_xy_n",    &analysis_data::sig_xy_n,sig_xy_n_ds)
            .def_readonly("x_pix_n",     &analysis_data::x_pix_n,x_pix_n_ds)
            .def_readonly("sig_x_pix_n", &analysis_data::sig_x_pix_n,sig_x_pix_n_ds)
            .def_readonly("sig_y_pix_n", &analysis_data::sig_y_pix_n,sig_y_pix_n_ds)
            .def_readonly("sig_xy_pix_n",&analysis_data::sig_xy_pix_n,sig_xy_pix_n_ds)
            .def_readonly("sum_pix_n",   &analysis_data::sum_pix_n,sum_pix_n_ds)
            .def_readonly("avg_pix_n",   &analysis_data::avg_pix_n,avg_pix_n_ds)


            .def_readonly("y_full",         &analysis_data::y_full, y_full_ds)
            .def_readonly("sig_x_full",     &analysis_data::sig_x_full, sig_x_full_ds)
            .def_readonly("sig_y_full",     &analysis_data::sig_y_full,sig_y_full_ds)
            .def_readonly("sig_xy_full",    &analysis_data::sig_xy_full,sig_xy_full_ds)
            .def_readonly("x_pix_full",     &analysis_data::x_pix_full,x_pix_full_ds)
            .def_readonly("sig_x_pix_full", &analysis_data::sig_x_pix_full,sig_x_pix_full_ds)
            .def_readonly("sig_y_pix_full", &analysis_data::sig_y_pix_full,sig_y_pix_full_ds)
            .def_readonly("sig_xy_pix_full",&analysis_data::sig_xy_pix_full,sig_xy_pix_full_ds)
            .def_readonly("sum_pix_full",   &analysis_data::sum_pix_full,sum_pix_full_ds)
            .def_readonly("avg_pix_full",   &analysis_data::avg_pix_full,avg_pix_full_ds)

            .def_readonly("step_size",   &analysis_data::step_size,step_size_ds)
            ;
    }
    void reg_fast_image(const char* py_name,const char*  py_docs)
    {

        const char*  num_pix_x_ds  = "";
        const char* num_pix_y_ds  = "";
        const char* bit_depth_ds  = "";
        const char* x_pix_to_mm_ds  = "";
        const char* y_pix_to_mm_ds  = "";
        const char* data_ds  = "";
        const char* data2D_ds  = "";
        const char* array_data_Py_ds  = "";
        const char* array_data2D_Py_ds  = "";
        const char* array_data_sum_ds  = "";

        const char* array_data_max_ds  = "";
        const char* array_data_min_ds  = "";

        const char* background_data_Py_ds  = "";
        const char* background_data_2D_Py_ds  = "";
        const char* background_data_sum_ds  = "";
        const char* name_ds  = "";
        const char* x_pix_scale_factor_ds  = "";
        const char* y_pix_scale_factor_ds  = "";

        using namespace cameraStructs;
        class_<fast_image,noncopyable>(py_name,py_docs, no_init)
//            .add_property("PSU",&magnetStructs::magnetObject::psuState,
//                                &magnetStructs::magnetObject::setPSU)
            .def_readonly("name",                &fast_image::name,name_ds )
            .def_readonly("num_pix_x",           &fast_image::num_pix_x,num_pix_x_ds)
            .def_readonly("num_pix_y",           &fast_image::num_pix_y,num_pix_x_ds)
            .def_readonly("bit_depth",           &fast_image::bit_depth,bit_depth_ds)
            .def_readonly("x_pix_to_mm",         &fast_image::x_pix_to_mm,x_pix_to_mm_ds)
            .def_readonly("y_pix_to_mm",         &fast_image::y_pix_to_mm,y_pix_to_mm_ds)

            .def_readonly("data",                &fast_image::data,data_ds)
            .def_readonly("data2D",              &fast_image::data2D,data2D_ds)
            .def_readonly("data_sum",            &fast_image::array_data_sum,array_data_sum_ds)
            .def_readonly("data_max",            &fast_image::array_data_max,array_data_max_ds)
            .def_readonly("data_min",            &fast_image::array_data_min,array_data_min_ds)
            .def_readonly("x_pix_scale_factor",  &fast_image::x_pix_scale_factor,x_pix_scale_factor_ds)
            .def_readonly("y_pix_scale_factor",  &fast_image::y_pix_scale_factor,y_pix_scale_factor_ds)
            ;
    }
    void reg_camera_state(const char* py_name,const char*  py_docs)
    {

        const char* use_background_ds =  "";
        const char* use_npoint_ds =  "";
        const char* power_ds = "";
        const char* acquire_ds = "";
        const char* analysing_ds = "";
        using namespace cameraStructs;
        class_<camera_state,noncopyable>(py_name,py_docs, no_init)
            .def_readonly("power",              &camera_state::power,          power_ds)
            .def_readonly("acquire",       &camera_state::acquire,   acquire_ds)
            .def_readonly("use_npoint",     &camera_state::use_npoint, use_npoint_ds)
            .def_readonly("analysing",     &camera_state::analysing, analysing_ds)
            .def_readonly("use_background",     &camera_state::use_background, use_background_ds)
            ;
    }
    void reg_clara_DAQ(const char* py_name,const char*  py_docs)
    {

        const char* name_ds =  "";
        const char* pvRoot_ds =  "";
        const char* screenName_ds =  "";
        const char* collectingState_ds =  "collecting state: indicates if collecting images to save or not";
        const char* writeState_ds =  "write state: indicates if writing or not";
        const char* writeCheck_ds =  "write check: whether the last write was succesful";
        const char* writeErrorMessage_ds =  "If error this string will get updated";
        const char* shotsTaken_ds =  "";
        const char* numberOfShots_ds =  "";
        const char* maxShots_ds =  "";
        const char* shotsTakenJPG_ds =  "";
        const char* numberOfShotsJPG_ds =  "";
        const char* frequency_ds =  "";
        const char* exposureTime_ds =  "";
        const char* acquisitionPeriod_ds =  "";
        const char* sensorTemp_ds =  "";
        const char* sensorMaxTemp_ds =  "";
        const char* sensorMinTemp_ds =  "";
        const char* collectingStateJPG_ds =  "";
        const char* writeStateJPG_ds =  "";
        const char* writeCheckJPG_ds =  "";
        const char* writeErrorMessageJPG_ds =  "";
        const char* latestFilename_ds =  "";
        const char* latestDirectory_ds =  "";


        using namespace cameraStructs;
        class_<clara_DAQ,noncopyable>(py_name,py_docs, no_init)
//            .add_property("PSU",&magnetStructs::magnetObject::psuState,
//                                &magnetStructs::magnetObject::setPSU)
            .def_readonly("name"                ,&clara_DAQ::name               ,name_ds)
            .def_readonly("pvRoot"              ,&clara_DAQ::pvRoot             ,pvRoot_ds)
            .def_readonly("screenName"          ,&clara_DAQ::screenName         ,screenName_ds)
            .def_readonly("collectingState"     ,&clara_DAQ::collectingState    ,collectingState_ds)
            .def_readonly("writeState"          ,&clara_DAQ::writeState         ,writeState_ds)
            .def_readonly("writeCheck"          ,&clara_DAQ::writeCheck         ,writeCheck_ds)
            .def_readonly("writeErrorMessage"   ,&clara_DAQ::writeErrorMessage  ,writeErrorMessage_ds)
            .def_readonly("shotsTaken"          ,&clara_DAQ::shotsTaken         ,shotsTaken_ds)
            .def_readonly("numberOfShots"       ,&clara_DAQ::numberOfShots      ,numberOfShots_ds)
            .def_readonly("maxShots"            ,&clara_DAQ::maxShots           ,maxShots_ds)
            .def_readonly("shotsTakenJPG"       ,&clara_DAQ::shotsTakenJPG      ,shotsTakenJPG_ds)
            .def_readonly("numberOfShotsJPG"    ,&clara_DAQ::numberOfShotsJPG   ,numberOfShotsJPG_ds)
            .def_readonly("frequency"           ,&clara_DAQ::frequency          ,frequency_ds)
            .def_readonly("exposureTime"        ,&clara_DAQ::exposureTime       ,exposureTime_ds)
            .def_readonly("acquisitionPeriod"   ,&clara_DAQ::acquisitionPeriod  ,acquisitionPeriod_ds)
            .def_readonly("sensorTemp"          ,&clara_DAQ::sensorTemp         ,sensorTemp_ds)
            .def_readonly("sensorMaxTemp"       ,&clara_DAQ::sensorMaxTemp      ,sensorMaxTemp_ds)
            .def_readonly("sensorMinTemp"       ,&clara_DAQ::sensorMinTemp      ,sensorMinTemp_ds)
            .def_readonly("collectingStateJPG"  ,&clara_DAQ::collectingStateJPG ,collectingStateJPG_ds)
            .def_readonly("writeStateJPG"       ,&clara_DAQ::writeStateJPG      ,writeStateJPG_ds)
            .def_readonly("writeCheckJPG"       ,&clara_DAQ::writeCheckJPG      ,writeCheckJPG_ds)
            .def_readonly("writeErrorMessageJPG",&clara_DAQ::writeErrorMessageJPG          ,writeErrorMessageJPG_ds)
            .def_readonly("latestFilename"      ,&clara_DAQ::latestFilename     ,latestFilename_ds)
            .def_readonly("latestDirectory"     ,&clara_DAQ::latestDirectory    ,latestDirectory_ds)
            ;
    }
    void reg_camera_image_data(const char* py_name,const char*  py_docs)
    {
        const char* image_ds = "";
        const char* analysis_ds = "";
        const char* mask_ds = "";

        using namespace cameraStructs;
        class_<camera_image_data,noncopyable>(py_name,py_docs, no_init)
            .def_readonly("name"              ,&camera_image_data::image    ,image_ds)
            .def_readonly("analysis"          ,&camera_image_data::analysis ,analysis_ds)
            .def_readonly("mask"              ,&camera_image_data::mask     ,mask_ds)
            ;

    }
    void reg_cameraObject(const char* py_name,const char*  py_docs)
    {
        const char* machineArea_ds = "";
        const char* name_ds = "";
        const char* pvRoot_ds = "";
        const char* screenName_ds = "";
        const char* streamingIPAddress_ds = "";
        const char* type_ds = "";
        const char* state_ds = "";

        const char* daq_ds = "";
        const char* data_ds = "";

        using namespace cameraStructs;

        class_<cameraObject,noncopyable>(py_name,py_docs, no_init)
            .def_readonly("machineArea",        &cameraObject::machineArea,        machineArea_ds)
            .def_readonly("name",               &cameraObject::name,               name_ds)
            .def_readonly("pvRoot",             &cameraObject::pvRoot,             pvRoot_ds)
            .def_readonly("screenName",         &cameraObject::screenName,         screenName_ds)
            .def_readonly("streamingIPAddress", &cameraObject::streamingIPAddress, streamingIPAddress_ds)
            .def_readonly("type",               &cameraObject::type,               type_ds)
            .def_readonly("state",              &cameraObject::state,             state_ds)
            .def_readonly("daq",                &cameraObject::daq,                daq_ds)
            .def_readonly("data",               &cameraObject::data,               data_ds)
            ;
    }
   void reg_cameraControllerBase(const char* py_name, const char* py_docs)
    {
        /*
            document strings for cameraControllerBase functions
            basically most thingss come in three flavours ...
        */
        const char* startAcquiring_VC_ds = "";
        const char* startAcquiring_ds_1 = "";
        const char* startAcquiring_ds_2 = "";
        const char* stopAcquiring_VC_ds = "";
        const char* stopAcquiring_ds1 = "";
        const char* stopAcquiring_ds2 = "";
        const char* stopAllAcquiring_ds = "";
        const char* stopAllAcquiringExceptVC_ds = "";
        const char* isAcquiring_VC_ds = "";
        const char* isAcquiring_ds_1 = "";
        const char* isAcquiring_ds_2 = "";
        const char* isNotAcquiring_VC_ds = "";
        const char* isNotAcquiring_ds_1 = "";
        const char* isNotAcquiring_ds_2 = "";
        const char* stopAcquireAndAnalysis_VC_ds = "";
        const char* stopAcquireAndAnalysis_ds1 = "";
        const char* stopAcquireAndAnalysis_ds2 = "";
        const char* startAcquireAndAnalysis_VC_ds = "";
        const char* startAcquireAndAnalysis_ds1 = "";
        const char* startAcquireAndAnalysis_ds2 = "";
        const char* latestCollectAndSaveSuccess_VC_ds = "";
        const char* latestCollectAndSaveSuccess_ds = "";
        const char* startAnalysing_VC_ds = "";
        const char* startAnalysing_ds_1 = "";
        const char* startAnalysing_ds_2 = "";
        const char* stopAnalysing_VC_ds = "";
        const char* stopAnalysing_ds1 = "";
        const char* stopAnalysing_ds2 = "";
        const char* stopAllAnalysing_ds = "";
        const char* stopAllAnalysingExceptVC_ds = "";
        const char* isAnalysing_VC_ds = "";
        const char* isAnalysing_ds_1 = "";
        const char* isAnalysing_ds_2 = "";
        const char* isNotAnalysing_VC_ds = "";
        const char* isNotAnalysing_ds_1 = "";
        const char* isNotAnalysing_ds_2 = "";
        const char* isNotCollectingOrSaving_VC_ds = "";
        const char* isNotCollectingOrSaving_ds1 = "";
        const char* isNotCollectingOrSaving_ds2 = "";
        const char* isUsingBackground_VC_ds     = "";
        const char* isUsingBackground_ds1       = "";
        const char* isUsingBackground_ds2       = "";
        const char* isNotUsingBackground_VC_ds= "";
        const char* isNotUsingBackground_ds1  = "";
        const char* isNotUsingBackground_ds2  = "";
        const char* isUsingNPoint_VC_ds     = "";
        const char* isUsingNPoint_ds1       = "";
        const char* isUsingNPoint_ds2       = "";
        const char* isNotUsingNPoint_VC_ds = "";
        const char* isNotUsingNPoint_ds1   = "";
        const char* isNotUsingNPoint_ds2   = "";
        const char* isBusy_VC_ds = "";
        const char*    isBusy_ds1 = "";
        const char*    isBusy_ds2 = "";
        const char* isNotBusy_VC_ds = "";
        const char* isNotBusy_ds1 = "";
        const char* isNotBusy_ds2 = "";
        const char* isON_VC_ds = "";
        const char* isON_ds_1 = "";
        const char* isON_ds_2 = "";
        const char* isOFF_VC_ds = "";
        const char* isOFF_ds_1 = "";
        const char* isOFF_ds_2 = "";

        const char* isVelaCam_VC_ds = "";
        const char* isVelaCam_ds1 = "";
        const char* isVelaCam_ds2 = "";
        const char* isClaraCam_VC_ds = "";
        const char* isClaraCam_ds1 = "";
        const char* isClaraCam_ds2 = "";

        const char* getMaskX_VC_ds = "";
        const char* getMaskX_ds_1 = "";
        const char* getMaskX_ds_2 = "";
        const char* getMaskY_VC_ds = "";
        const char* getMaskY_ds_1 = "";
        const char* getMaskY_ds_2 = "";
        const char* getMaskXrad_VC_ds = "";
        const char* getMaskXrad_ds_1 = "";
        const char* getMaskXrad_ds_2 = "";
        const char* getMaskYrad_VC_ds = "";
        const char* getMaskYrad_ds_1 = "";
        const char* getMaskYrad_ds_2 = "";
        const char* setMaskX_VC_ds = "";
        const char* setMaskX_ds_1 = "";
        const char* setMaskX_ds_2 = "";
        const char* setMaskY_VC_ds = "";
        const char* setMaskY_ds_1 = "";
        const char* setMaskY_ds_2 = "";
        const char* setMaskXrad_VC_ds = "";
        const char* setMaskXrad_ds_1 = "";
        const char* setMaskXrad_ds_2 = "";
        const char* setMaskYrad_VC_ds = "";
        const char* setMaskYrad_ds_1 = "";
        const char* setMaskYrad_ds_2 = "";
        const char* setMask_VC_ds1 = "";
        const char* setMask_VC_ds2 = "";
        const char* setMask_1_ds_1 = "";
        const char* setMask_2_ds_2 = "";
        const char* setMask_VC_Py_ds = "";
        const char* setMask_Py1_ds = "";
        const char* setMask_Py2_ds = "";
        const char* setBackground_VC_ds = "";
        const char* setBackground_ds_1 = "";
        const char* setBackground_ds_2 = "";
        const char* useBackground_VC_ds = "";
        const char* useBackground_ds_1 = "";
        const char* useBackground_ds_2 = "";
        const char* setStepSize_VC_ds = "";
        const char* setStepSize_ds_1 = "";
        const char* setStepSize_ds_2 = "";
        const char* setCenterXPixel_VC_ds = "";
        const char* setCenterXPixel_ds_1 = "";
        const char* setCenterXPixel_ds_2 = "";
        const char* setCenterYPixel_VC_ds = "";
        const char* setCenterYPixel_ds_1 = "";
        const char* setCenterYPixel_ds_2 = "";
        const char* setPixMM_VC_ds = "";
        const char* setPixMM_ds_1 = "";
        const char* setPixMM_ds_2 = "";
        const char* useNPoint_VC_ds = "";
        const char* useNPoint_ds_1 = "";
        const char* useNPoint_ds_2 = "";
        const char* startAnalysis_VC_ds = "";
        const char* startAnalysis_ds_1 = "";
        const char* startAnalysis_ds_2 = "";
        const char* stopAnalysis_VC_ds = "";
        const char* stopAnalysis_ds_1 = "";
        const char* stopAnalysis_ds_2 = "";
        const char* isCollecting_VC_ds = "";
        const char* isCollecting_ds_1 = "";
        const char* isCollecting_ds_2 = "";
        const char* isSaving_VC_ds = "";
        const char* isSaving_ds_1 = "";
        const char* isSaving_ds_2 = "";
        const char* isCollectingJPG_VC_ds = "";
        const char* isCollectingJPG_ds_1 = "";
        const char* isCollectingJPG_ds_2 = "";
        const char* isSavingJPG_VC_ds = "";
        const char* isSavingJPG_ds_1 = "";
        const char* isSavingJPG_ds_2 = "";
        const char* isCollectingOrSaving_VC_ds = "";
        const char* isCollectingOrSaving_ds_1 = "";
        const char* isCollectingOrSaving_ds_2 = "";
        const char* getXBuffer_VC_Py_ds  = "";
        const char* getXBuffer_Py1_ds1   = "";
        const char* getXBuffer_Py2_ds2   = "";
        const char* getYBuffer_VC_Py_ds = "";
        const char* getYBuffer_Py1_ds1  = "";
        const char* getYBuffer_Py2_ds2  = "";
        const char* getSigXBuffer_VC_Py_ds = "";
        const char* getSigXBuffer_Py1_ds1  = "";
        const char* getSigXBuffer_Py2_ds2  = "";
        const char* getSigYBuffer_VC_Py_ds = "";
        const char* getSigYBuffer_Py1_ds1  = "";
        const char* getSigYBuffer_Py2_ds2  = "";
        const char* getSigXYBuffer_VC_Py_ds = "";
        const char* getSigXYBuffer_Py1_ds1  = "";
        const char* getSigXYBuffer_Py2_ds2  = "";
        const char* getXPixBuffer_VC_Py_ds = "";
        const char* getXPixBuffer_Py1_ds1  = "";
        const char* getXPixBuffer_Py2_ds2  = "";
        const char* getYPixBuffer_VC_Py_ds = "";
        const char* getYPixBuffer_Py1_ds1  = "";
        const char* getYPixBuffer_Py2_ds2  = "";
        const char* getSigXPixBuffer_VC_Py_ds = "";
        const char* getSigXPixBuffer_Py1_ds1  = "";
        const char* getSigXPixBuffer_Py2_ds2  = "";
        const char* getSigYPixBuffer_VC_Py_ds = "";
        const char* getSigYPixBuffer_Py1_ds1  = "";
        const char* getSigYPixBuffer_Py2_ds2  = "";
        const char* getSigXYPixBuffer_VC_Py_ds = "";
        const char* getSigXYPixBuffer_Py1_ds1  = "";
        const char* getSigXYPixBuffer_Py2_ds2  = "";
        const char* getPixelValues_VC_Py_ds = "";
        const char* getPixelValues_Py1_ds1  = "";
        const char* getPixelValues_Py2_ds2  = "";
        const char* getPixelValuesBuffer_VC_Py_ds = "";
        const char* getPixelValuesBuffer_Py1_ds1  = "";
        const char* getPixelValuesBuffer_Py2_ds2  = "";
        const char* getFastImage_VC_Py_ds = "";
        const char* getFastImage_Py1_ds1  = "";
        const char* getFastImage_Py2_ds2  = "";
        const char* getFastImage2D_VC_Py_ds = "";
        const char* getFastImage2D_Py1_ds = "";
        const char* getFastImage2D_Py2_ds = "";
        const char* getSumIntensityBuffer_VC_Py_ds = "";
        const char* getSumIntensityBuffer_Py1_ds1  = "";
        const char* getSumIntensityBuffer_Py2_ds2  = "";
        const char* getAvgIntensityBuffer_VC_Py_ds = "";
        const char* getAvgIntensityBuffer_Py1_ds1  = "";
        const char* getAvgIntensityBuffer_Py2_ds2  = "";
        const char* getXBuffer_VC_NumPy_ds = "";
        const char* getXBuffer_NumPy1_ds1  = "";
        const char* getXBuffer_NumPy2_ds2  = "";
        const char* getYBuffer_VC_NumPy_ds = "";
        const char* getYBuffer_NumPy1_ds1  = "";
        const char* getYBuffer_NumPy2_ds2  = "";
        const char* getSigXBuffer_VC_NumPy_ds = "";
        const char* getSigXBuffer_NumPy1_ds1  = "";
        const char* getSigXBuffer_NumPy2_ds2  = "";
        const char* getSigYBuffer_VC_NumPy_ds = "";
        const char* getSigYBuffer_NumPy1_ds1  = "";
        const char* getSigYBuffer_NumPy2_ds2  = "";
        const char* getSigXYBuffer_VC_NumPy_ds = "";
        const char* getSigXYBuffer_NumPy1_ds1  = "";
        const char* getSigXYBuffer_NumPy2_ds2  = "";
        const char* getXPixBuffer_VC_NumPy_ds = "";
        const char* getXPixBuffer_NumPy1_ds1  = "";
        const char* getXPixBuffer_NumPy2_ds2  = "";
        const char* getYPixBuffer_VC_NumPy_ds = "";
        const char* getYPixBuffer_NumPy1_ds1  = "";
        const char* getYPixBuffer_NumPy2_ds2  = "";
        const char* getSigXPixBuffer_VC_NumPy_ds = "";
        const char* getSigXPixBuffer_NumPy1_ds1  = "";
        const char* getSigXPixBuffer_NumPy2_ds2  = "";
        const char* getSigYPixBuffer_VC_NumPy_ds = "";
        const char* getSigYPixBuffer_NumPy1_ds1  = "";
        const char* getSigYPixBuffer_NumPy2_ds2  = "";
        const char* getSigXYPixBuffer_VC_NumPy_ds = "";
        const char* getSigXYPixBuffer_NumPy1_ds1  = "";
        const char* getSigXYPixBuffer_NumPy2_ds2  = "";
        const char* getPixelValues_VC_NumPy_ds = "";
        const char* getPixelValues_NumPy1_ds1  = "";
        const char* getPixelValues_NumPy2_ds2  = "";
        const char* getPixelValuesBuffer_VC_NumPy_ds = "";
        const char* getPixelValuesBuffer_NumPy1_ds1  = "";
        const char* getPixelValuesBuffer_NumPy2_ds2  = "";
        const char* getFastImage_VC_NumPy_ds = "";
        const char* getFastImage_NumPy1_ds1  = "";
        const char* getFastImage_NumPy2_ds2  = "";
        const char* getSumIntensityBuffer_VC_NumPy_ds = "";
        const char* getSumIntensityBuffer_NumPy1_ds1  = "";
        const char* getSumIntensityBuffer_NumPy2_ds2  = "";
        const char* getAvgIntensityBuffer_VC_NumPy_ds = "";
        const char* getAvgIntensityBuffer_NumPy1_ds1  = "";
        const char* getAvgIntensityBuffer_NumPy2_ds2  = "";
        const char* clearRunningValues_VC_ds      = "";
        const char* clearRunningValues_ds1        = "";
        const char* clearRunningValues_ds2        = "";
        const char* isBufferFull_VC_ds = "";
        const char* isBufferFull_ds1   = "";
        const char* isBufferFull_ds2   = "";
        const char* isBufferNotFull_VC_ds = "";
        const char* isBufferNotFull_ds1   = "";
        const char* isBufferNotFull_ds2   = "";

        //const char* getSumPix


        const char* getX_VC_ds= "";
        const char* getX_ds1  = "";
        const char* getX_ds2  = "";
        const char* getY_VC_ds = "";
        const char* getY_ds1   = "";
        const char* getY_ds2   = "";
        const char* getSigX_VC_ds = "";
        const char* getSigX_ds1   = "";
        const char* getSigX_ds2   = "";
        const char* getSigY_VC_ds = "";
        const char* getSigY_ds1   = "";
        const char* getSigY_ds2   = "";
        const char* getSigXY_VC_ds = "";
        const char* getSigXY_ds1   = "";
        const char* getSigXY_ds2   = "";
        const char* getXPix_VC_ds = "";
        const char* getXPix_ds1   = "";
        const char* getXPix_ds2   = "";
        const char* getYPix_VC_ds = "";
        const char* getYPix_ds1   = "";
        const char* getYPix_ds2   = "";
        const char* getSigXPix_VC_ds = "";
        const char* getSigXPix_ds1   = "";
        const char* getSigXPix_ds2   = "";
        const char* getSigYPix_VC_ds = "";
        const char* getSigYPix_ds1   = "";
        const char* getSigYPix_ds2   = "";
        const char* getSigXYPix_VC_ds = "";
        const char* getSigXYPix_ds1   = "";
        const char* getSigXYPix_ds2   = "";
        const char* setBufferMaxCount_VC_ds = "";
        const char* setBufferMaxCount_ds1   = "";
        const char* setBufferMaxCount_ds2   = "";
        const char* clearBuffer_VC_ds = "";
        const char* clearBuffer_ds1   = "";
        const char* clearBuffer_ds2   = "";
        const char* getBufferCount_VC_ds = "";
        const char* getBufferCount_ds1   = "";
        const char* getBufferCount_ds2   = "";
        const char* getBufferMaxCount_VC_ds = "";
        const char* getBufferMaxCount_ds1   = "";
        const char* getBufferMaxCount_ds2   = "";
        const char* isNotCollecting_VC_ds = "";
        const char* isNotCollecting_ds1 = "";
        const char* isNotCollecting_ds2 = "";
        const char* isSelectedCamera_ds = "";
        const char* isNotSelectedCamera_ds = "";
        const char* collectAndSave_ds1    = "";
        const char* collectAndSave_ds2    = "";
        const char* collectAndSave_VC_ds = "";
        const char* getMaskObj_VC_ds = "";
        const char* getMaskObj_ds1 = "";
        const char* getMaskObj_ds2 = "";
        const char* getAnalysisObj_VC_ds = "";
        const char* getAnalysisObj_ds1 = "";
        const char* getAnalysisObj_ds2 = "";
        const char* getImageObj_VC_ds = "";
        const char* getImageObj_ds1 = "";
        const char* getImageObj_ds2 = "";
        const char* getImageDataObj_VC_ds = "";
        const char* getImageDataObj_ds1 = "";
        const char* getImageDataObj_ds2 = "";
        const char* getClaraDAQObj_VC_ds = "";
        const char* getClaraDAQObj_ds1 = "";
        const char* getClaraDAQObj_ds2 = "";
        const char* getCameraObj_VC_ds = "";
        const char* getCameraObj_ds1 = "";
        const char* getCameraObj_ds2 = "";
        const char* getStepSize_VC_ds = "";
        const char* getStepSize_ds1 = "";
        const char* getStepSize_ds2 = "";
        const char* takeFastImage_VC_ds = "";
        const char* takeFastImage_ds1 = "";
        const char* takeFastImage_ds2 = "";
        const char* getStateObj_VC_ds = "";
        const char* getStateObj_ds1 = "";
        const char* getStateObj_ds2 = "";

        const char* setGain_VC_ds = "";
        const char* setGain_ds1   = "";
        const char* setGain_ds2   = "";
        const char* setBlacklevel_VC_ds = "";
        const char* setBlacklevel_ds1   = "";
        const char* setBlacklevel_ds2   = "";
        const char* getGain_VC_ds = "";
        const char* getGain_ds1   = "";
        const char* getGain_ds2   = "";
        const char* getBlacklevel_VC_ds = "";
        const char* getBlacklevel_ds1   = "";
        const char* getBlacklevel_ds2   = "";

        const char* getCameraNames_ds = "";
        const char* getCameraScreenNames_ds = "";

        const char* takeAndGetFastImage2D_VC_ds = "";
        const char* takeAndGetFastImage2D_ds1   = "";
        const char* takeAndGetFastImage2D_ds2   = "";
        const char* takeAndGetFastImage_VC_ds  = "";
        const char* takeAndGetFastImage_ds1    = "";
        const char* takeAndGetFastImage_ds2    = "";


         const char* setMaskFeedBackOn_VC_ds = "";
         const char* setMaskFeedBackOn_ds1      = "";
         const char* setMaskFeedBackOn_ds2      = "";
         const char* setMaskFeedBackOff_VC_ds = "";
         const char* setMaskFeedBackOff_ds1     = "";
         const char* setMaskFeedBackOff_ds2     = "";
         const char* isMaskFeedbackOn_VC_ds = "";
         const char* isMaskFeedbackOn_ds1      = "";
         const char* isMaskFeedbackOn_ds2      = "";
         const char* isMaskFeedbackOff_VC_ds  = "";
         const char* isMaskFeedbackOff_ds1       = "";
         const char* isMaskFeedbackOff_ds2       = "";


        const char* hasBeam_VC_ds = "";
        const char* hasBeam_ds1   = "";
        const char* hasBeam_ds2  = "";

        const char* hasNoBeam_VC_ds = "";
        const char* hasNoBeam_ds1   = "";
        const char* hasNoBeam_ds2   = "";


         const char* isAnalysisUpdating_VC_ds = "";
         const char* isAnalysisUpdating_ds1    = "";
         const char* isAnalysisUpdating_ds2    = "";

         const char* isAnalysisNotUpdating_VC_ds = "";
         const char* isAnalysisNotUpdating_ds1   = "";
         const char* isAnalysisNotUpdating_ds2   = "";


        const char* getPix2mm_VC_ds = "";
        const char* getPix2mm_ds1    = "";
        const char* getPix2mm_ds2    = "";

        const char* getPix2mmDef_VC_ds = "";
        const char* getPix2mmDef_ds1   = "";
        const char* getPix2mmDef_ds2   = "";


        const char* claraLEDOn_ds = "";
        const char* claraLEDOff_ds = "";
        const char* isCLaraLEDOn_ds = "";
        const char* isCLaraLEDOff_ds = "";



        /*
            function pointers for overloads to expose to Python...
        */
        const cameraStructs::analysis_mask&(cameraControllerBase::*getMaskObj_1)(cstr&)const= &cameraControllerBase::getMaskObj;
        const cameraStructs::analysis_mask&(cameraControllerBase::*getMaskObj_2)()    const= &cameraControllerBase::getMaskObj;
        const cameraStructs::analysis_data&(cameraControllerBase::*getAnalysisObj_1)(cstr&)const= &cameraControllerBase::getAnalysisObj;
        const cameraStructs::analysis_data&(cameraControllerBase::*getAnalysisObj_2)()     const= &cameraControllerBase::getAnalysisObj;
        const cameraStructs::fast_image& (cameraControllerBase::*getImageObj_1)(cstr& name)const= &cameraControllerBase::getImageObj;
        const cameraStructs::fast_image& (cameraControllerBase::*getImageObj_2)()          const= &cameraControllerBase::getImageObj;
        const cameraStructs::camera_image_data&(cameraControllerBase::*getImageDataObj_1)(cstr&)const= &cameraControllerBase::getImageDataObj;
        const cameraStructs::camera_image_data&(cameraControllerBase::*getImageDataObj_2)()     const= &cameraControllerBase::getImageDataObj;
        const cameraStructs::clara_DAQ&(cameraControllerBase::*getClaraDAQObj_1)(cstr&)const= &cameraControllerBase::getClaraDAQObj;
        const cameraStructs::clara_DAQ&(cameraControllerBase::*getClaraDAQObj_2)()     const= &cameraControllerBase::getClaraDAQObj;
        const cameraStructs::cameraObject&(cameraControllerBase::*getCameraObj_1)(cstr&)const= &cameraControllerBase::getCameraObj;
        const cameraStructs::cameraObject&(cameraControllerBase::*getCameraObj_2)()     const= &cameraControllerBase::getCameraObj;
        const cameraStructs::camera_state&(cameraControllerBase::*getStateObj_1)(cstr&)const= &cameraControllerBase::getStateObj;
        const cameraStructs::camera_state&(cameraControllerBase::*getStateObj_2)()     const= &cameraControllerBase::getStateObj;

        bool(cameraControllerBase::*setMaskFeedBackOn_1)(cstr&)       = &cameraControllerBase::setMaskFeedBackOn;
        bool(cameraControllerBase::*setMaskFeedBackOn_2)()= &cameraControllerBase::setMaskFeedBackOn;
        bool(cameraControllerBase::*setMaskFeedBackOff_1)(cstr&)       = &cameraControllerBase::setMaskFeedBackOff;
        bool(cameraControllerBase::*setMaskFeedBackOff_2)()= &cameraControllerBase::setMaskFeedBackOff;
        bool(cameraControllerBase::*isMaskFeedbackOn_1)(cstr&)const       = &cameraControllerBase::isMaskFeedbackOn;
        bool(cameraControllerBase::*isMaskFeedbackOn_2)()const= &cameraControllerBase::isMaskFeedbackOn;
        bool(cameraControllerBase::*isMaskFeedbackOff_1)(cstr&)const       = &cameraControllerBase::isMaskFeedbackOff;
        bool(cameraControllerBase::*isMaskFeedbackOff_2)()const = &cameraControllerBase::isMaskFeedbackOff;



        bool(cameraControllerBase::*takeFastImage_1)(cstr&) = &cameraControllerBase::takeFastImage;
        bool(cameraControllerBase::*takeFastImage_2)()      = &cameraControllerBase::takeFastImage;


        bool(cameraControllerBase::*useNPoint_1)(bool,cstr&)       = &cameraControllerBase::useNPoint;
        bool(cameraControllerBase::*useNPoint_2)(bool)= &cameraControllerBase::useNPoint;
        bool(cameraControllerBase::*startAnalysis_1)(cstr&)     = &cameraControllerBase::startAnalysis;
        bool(cameraControllerBase::*startAnalysis_2)()          = &cameraControllerBase::startAnalysis;
        bool(cameraControllerBase::*stopAnalysis_1)(cstr&)      = &cameraControllerBase::stopAnalysis;
        bool(cameraControllerBase::*stopAnalysis_2)()           = &cameraControllerBase::stopAnalysis;
        bool(cameraControllerBase::*startAnalysing_1)(cstr&)          = &cameraControllerBase::startAnalysing;
        bool(cameraControllerBase::*startAnalysing_2)()  = &cameraControllerBase::startAnalysing;
        bool(cameraControllerBase::*stopAnalysing_1)(cstr&) = &cameraControllerBase::stopAnalysing;
        bool(cameraControllerBase::*stopAnalysing_2)()      = &cameraControllerBase::stopAnalysing;
        bool(cameraControllerBase::*startAcquiring_1)(cstr&)          = &cameraControllerBase::startAcquiring;
        bool(cameraControllerBase::*startAcquiring_2)()  = &cameraControllerBase::startAcquiring;
        bool(cameraControllerBase::*stopAcquiring_1)(cstr&) = &cameraControllerBase::stopAcquiring;
        bool(cameraControllerBase::*stopAcquiring_2)()      = &cameraControllerBase::stopAcquiring;
        bool(cameraControllerBase::*stopAcquireAndAnalysis_1)(cstr&)  = &cameraControllerBase::stopAcquireAndAnalysis;
        bool(cameraControllerBase::*stopAcquireAndAnalysis_2)()       = &cameraControllerBase::stopAcquireAndAnalysis;
        bool(cameraControllerBase::*startAcquireAndAnalysis_1)(cstr&) = &cameraControllerBase::startAcquireAndAnalysis;
        bool(cameraControllerBase::*startAcquireAndAnalysis_2)()      = &cameraControllerBase::startAcquireAndAnalysis;
        void(cameraControllerBase::*clearBuffer_1)(cstr&)                              = &cameraControllerBase::clearBuffer;
        void(cameraControllerBase::*clearBuffer_2)()                                   = &cameraControllerBase::clearBuffer;
        bool(cameraControllerBase::*collectAndSave_1)(cstr&,const int) = &cameraControllerBase::collectAndSave;
        bool(cameraControllerBase::*collectAndSave_2)(const int)      = &cameraControllerBase::collectAndSave;
        void(cameraControllerBase::*clearRunningValues_1)(cstr&)                            = &cameraControllerBase::clearRunningValues;
        void(cameraControllerBase::*clearRunningValues_2)()                            = &cameraControllerBase::clearRunningValues;


        bool(cameraControllerBase::*isAnalysisUpdating_1)(cstr&)const = &cameraControllerBase::isAnalysisUpdating;
        bool(cameraControllerBase::*isAnalysisUpdating_2)(     )const = &cameraControllerBase::isAnalysisUpdating;
        bool(cameraControllerBase::*isAnalysisNotUpdating_1)(cstr&)const = &cameraControllerBase::isAnalysisNotUpdating;
        bool(cameraControllerBase::*isAnalysisNotUpdating_2)(     )const = &cameraControllerBase::isAnalysisNotUpdating;

        bool(cameraControllerBase::*isBusy_1)(cstr&)const = &cameraControllerBase::isBusy;
        bool(cameraControllerBase::*isBusy_2)(     )const = &cameraControllerBase::isBusy;
        bool(cameraControllerBase::*isNotBusy_1)(cstr&)const = &cameraControllerBase::isNotBusy;
        bool(cameraControllerBase::*isNotBusy_2)(     )const = &cameraControllerBase::isNotBusy;

        bool(cameraControllerBase::*hasBeam_1)(cstr&)const = &cameraControllerBase::hasBeam;
        bool(cameraControllerBase::*hasBeam_2)(     )const = &cameraControllerBase::hasBeam;
        bool(cameraControllerBase::*hasNoBeam_1)(cstr&)const = &cameraControllerBase::hasNoBeam;
        bool(cameraControllerBase::*hasNoBeam_2)(     )const = &cameraControllerBase::hasNoBeam;

        bool(cameraControllerBase::*isUsingBackground_1)(cstr&)const = &cameraControllerBase::isUsingBackground;
        bool(cameraControllerBase::*isUsingBackground_2)(     )const = &cameraControllerBase::isUsingBackground;
        bool(cameraControllerBase::*isNotUsingBackground_1)(cstr&)const = &cameraControllerBase::isNotUsingBackground;
        bool(cameraControllerBase::*isNotUsingBackground_2)(     )const = &cameraControllerBase::isNotUsingBackground;
        bool(cameraControllerBase::*isUsingNPoint_1)(cstr&)const  = &cameraControllerBase::isUsingNPoint;
        bool(cameraControllerBase::*isUsingNPoint_2)(     )const  = &cameraControllerBase::isUsingNPoint;
        bool(cameraControllerBase::*isNotUsingNPoint_1)(cstr&)const = &cameraControllerBase::isNotUsingNPoint;
        bool(cameraControllerBase::*isNotUsingNPoint_2)(     )const = &cameraControllerBase::isNotUsingNPoint;
        bool(cameraControllerBase::*isNotCollectingOrSaving_1)(cstr&)const = &cameraControllerBase::isNotCollectingOrSaving;
        bool(cameraControllerBase::*isNotCollectingOrSaving_2)()const      = &cameraControllerBase::isNotCollectingOrSaving;
        bool(cameraControllerBase::*isAnalysing_1)(cstr&)const= &cameraControllerBase::isAnalysing;
        bool(cameraControllerBase::*isAnalysing_2)()     const= &cameraControllerBase::isAnalysing;
        bool(cameraControllerBase::*isNotAnalysing_1)(cstr&)          const= &cameraControllerBase::isNotAnalysing;
        bool(cameraControllerBase::*isNotAnalysing_2)()  const= &cameraControllerBase::isNotAnalysing;
        bool(cameraControllerBase::*isNotCollecting_1)(const std::string& cam)const = &cameraControllerBase::isNotCollecting;
        bool(cameraControllerBase::*isNotCollecting_2)()                      const = &cameraControllerBase::isNotCollecting;
        bool(cameraControllerBase::*isON_1)(cstr&)const       = &cameraControllerBase::isON;
        bool(cameraControllerBase::*isON_2)()const            = &cameraControllerBase::isON;
        bool(cameraControllerBase::*isOFF_1)(cstr&)const      = &cameraControllerBase::isOFF;
        bool(cameraControllerBase::*isOFF_2)()const           = &cameraControllerBase::isOFF;
        bool(cameraControllerBase::*isAcquiring_1)(cstr&)const= &cameraControllerBase::isAcquiring;
        bool(cameraControllerBase::*isAcquiring_2)()     const= &cameraControllerBase::isAcquiring;
        bool(cameraControllerBase::*isNotAcquiring_1)(cstr&) const         = &cameraControllerBase::isNotAcquiring;
        bool(cameraControllerBase::*isNotAcquiring_2)() const = &cameraControllerBase::isNotAcquiring;
        bool(cameraControllerBase::*isCollecting_1)(cstr&) const     = &cameraControllerBase::isCollecting;
        bool(cameraControllerBase::*isCollecting_2)()      const     = &cameraControllerBase::isCollecting;
        bool(cameraControllerBase::*isSaving_1)(cstr&)     const    = &cameraControllerBase::isSaving;
        bool(cameraControllerBase::*isSaving_2)() const= &cameraControllerBase::isSaving;
        bool(cameraControllerBase::*isCollectingJPG_1)(cstr&)const   = &cameraControllerBase::isCollectingJPG;
        bool(cameraControllerBase::*isCollectingJPG_2)()     const   = &cameraControllerBase::isCollectingJPG;
        bool(cameraControllerBase::*isSavingJPG_1)(cstr&)    const  = &cameraControllerBase::isSavingJPG;
        bool(cameraControllerBase::*isSavingJPG_2)()         const  = &cameraControllerBase::isSavingJPG;
        bool(cameraControllerBase::*isCollectingOrSaving_1)(cstr&)const          = &cameraControllerBase::isCollectingOrSaving;
        bool(cameraControllerBase::*isCollectingOrSaving_2)()  const= &cameraControllerBase::isCollectingOrSaving;
        bool(cameraControllerBase::*isBufferFull_1)(cstr&)const = &cameraControllerBase::isBufferFull;
        bool(cameraControllerBase::*isBufferFull_2)()     const = &cameraControllerBase::isBufferFull;


        bool(cameraControllerBase::*isBufferNotFull_1)(cstr&)const = &cameraControllerBase::isBufferNotFull;
        bool(cameraControllerBase::*isBufferNotFull_2)()     const = &cameraControllerBase::isBufferNotFull;

        bool(cameraControllerBase::*isClaraCam_1)(cstr&)const = &cameraControllerBase::isClaraCam;
        bool(cameraControllerBase::*isClaraCam_2)()     const = &cameraControllerBase::isClaraCam;

        bool(cameraControllerBase::*isVelaCam_1)(cstr&)const = &cameraControllerBase::isVelaCam;
        bool(cameraControllerBase::*isVelaCam_2)()     const = &cameraControllerBase::isVelaCam;


        bool(cameraControllerBase::*setMaskX_1)(unsigned short,cstr&)            = &cameraControllerBase::setMaskX;
        bool(cameraControllerBase::*setMaskX_2)(unsigned short)     = &cameraControllerBase::setMaskX;
        bool(cameraControllerBase::*setMaskY_1)(unsigned short,cstr&)            = &cameraControllerBase::setMaskY;
        bool(cameraControllerBase::*setMaskY_2)(unsigned short)     = &cameraControllerBase::setMaskY;
        bool(cameraControllerBase::*setMaskXrad_1)(unsigned short,cstr&)         = &cameraControllerBase::setMaskXrad;
        bool(cameraControllerBase::*setMaskXrad_2)(unsigned short)  = &cameraControllerBase::setMaskXrad;
        bool(cameraControllerBase::*setMaskYrad_1)(unsigned short,cstr&)         = &cameraControllerBase::setMaskYrad;
        bool(cameraControllerBase::*setMaskYrad_2)(unsigned short)  = &cameraControllerBase::setMaskYrad;
        bool(cameraControllerBase::*setMask_VC_1)(unsigned short,unsigned short,unsigned short,unsigned short) = &cameraControllerBase::setMask_VC;
        bool(cameraControllerBase::*setMask_VC_2)(cvus&) = &cameraControllerBase::setMask_VC;
        bool(cameraControllerBase::*setMask_1)(unsigned short,unsigned short,unsigned short,unsigned short,cstr&) = &cameraControllerBase::setMask;
        bool(cameraControllerBase::*setMask_2)(unsigned short,unsigned short,unsigned short,unsigned short) = &cameraControllerBase::setMask;
        bool(cameraControllerBase::*setBackground_1)(cstr&)     = &cameraControllerBase::setBackground;
        bool(cameraControllerBase::*setBackground_2)()          = &cameraControllerBase::setBackground;
        bool(cameraControllerBase::*useBackground_1)(bool ,cstr&)   = &cameraControllerBase::useBackground;
        bool(cameraControllerBase::*useBackground_2)(bool )         = &cameraControllerBase::useBackground;
        bool(cameraControllerBase::*setStepSize_1)(cus,cstr&)           = &cameraControllerBase::setStepSize;
        bool(cameraControllerBase::*setStepSize_2)(cus)    = &cameraControllerBase::setStepSize;
        bool(cameraControllerBase::*setCenterXPixel_1)(cus,cstr&)        = &cameraControllerBase::setCenterXPixel;
        bool(cameraControllerBase::*setCenterXPixel_2)(cus) = &cameraControllerBase::setCenterXPixel;
        bool(cameraControllerBase::*setCenterYPixel_1)(cus,cstr&)        = &cameraControllerBase::setCenterYPixel;
        bool(cameraControllerBase::*setCenterYPixel_2)(cus) = &cameraControllerBase::setCenterYPixel;
        bool(cameraControllerBase::*setPixMM_1)(const double,cstr&)      = &cameraControllerBase::setPixMM;
        bool(cameraControllerBase::*setPixMM_2)(const double)            = &cameraControllerBase::setPixMM;
        int (cameraControllerBase::*getStepSize_1)(cstr& )const  = &cameraControllerBase::getStepSize;
        int (cameraControllerBase::*getStepSize_2)()const        = &cameraControllerBase::getStepSize;
        unsigned short (cameraControllerBase::*getMaskX_1)(cstr& )  const= &cameraControllerBase::getMaskX;
        unsigned short (cameraControllerBase::*getMaskX_2)()        const= &cameraControllerBase::getMaskX;
        unsigned short (cameraControllerBase::*getMaskY_1)(cstr&)   const= &cameraControllerBase::getMaskY;
        unsigned short (cameraControllerBase::*getMaskY_2)()        const= &cameraControllerBase::getMaskY;
        unsigned short (cameraControllerBase::*getMaskXrad_1)(cstr& )            const= &cameraControllerBase::getMaskXrad;
        unsigned short (cameraControllerBase::*getMaskYrad_2)()     const= &cameraControllerBase::getMaskYrad;
        unsigned short (cameraControllerBase::*getMaskXrad_2)()     const= &cameraControllerBase::getMaskXrad;
        unsigned short (cameraControllerBase::*getMaskYrad_1)(cstr& )            const= &cameraControllerBase::getMaskYrad;
        double(cameraControllerBase::*getX_1)(cstr&)  const                                 = &cameraControllerBase::getX;
        double(cameraControllerBase::*getX_2)()      const                                  = &cameraControllerBase::getX;
        double(cameraControllerBase::*getY_1)(cstr&)  const                                 = &cameraControllerBase::getY;
        double(cameraControllerBase::*getY_2)()        const                                = &cameraControllerBase::getY;
        double(cameraControllerBase::*getSigX_1)(cstr&)  const                              = &cameraControllerBase::getSigX;
        double(cameraControllerBase::*getSigX_2)()     const                                = &cameraControllerBase::getSigX;
        double(cameraControllerBase::*getSigY_1)(cstr&) const                               = &cameraControllerBase::getSigY;
        double(cameraControllerBase::*getSigY_2)()       const                              = &cameraControllerBase::getSigY;
        double(cameraControllerBase::*getSigXY_1)(cstr&) const                              = &cameraControllerBase::getSigXY;
        double(cameraControllerBase::*getSigXY_2)()     const                               = &cameraControllerBase::getSigXY;
        double(cameraControllerBase::*getXPix_1)(cstr&)  const                              = &cameraControllerBase::getXPix;
        double(cameraControllerBase::*getXPix_2)()      const                               = &cameraControllerBase::getXPix;
        double(cameraControllerBase::*getYPix_1)(cstr&) const                               = &cameraControllerBase::getYPix;
        double(cameraControllerBase::*getYPix_2)()      const                               = &cameraControllerBase::getYPix;
        double(cameraControllerBase::*getSigXPix_1)(cstr&)const                             = &cameraControllerBase::getSigXPix;
        double(cameraControllerBase::*getSigXPix_2)()     const                             = &cameraControllerBase::getSigXPix;
        double(cameraControllerBase::*getSigYPix_1)(cstr&)const                             = &cameraControllerBase::getSigYPix;
        double(cameraControllerBase::*getSigYPix_2)()     const                             = &cameraControllerBase::getSigYPix;
        double(cameraControllerBase::*getSigXYPix_1)(cstr&)const                            = &cameraControllerBase::getSigXYPix;
        double(cameraControllerBase::*getSigXYPix_2)()     const                            = &cameraControllerBase::getSigXYPix;

        double(cameraControllerBase::*getPix2mm_1)(cstr&)const                            = &cameraControllerBase::getPix2mm;
        double(cameraControllerBase::*getPix2mm_2)()     const                            = &cameraControllerBase::getPix2mm;
        double(cameraControllerBase::*getPix2mmDef_1)(cstr&)const                         = &cameraControllerBase::getPix2mmDef;
        double(cameraControllerBase::*getPix2mmDef_2)()     const                         = &cameraControllerBase::getPix2mmDef;


        void(cameraControllerBase::*setBufferMaxCount_1)(const size_t s,cstr&)         = &cameraControllerBase::setBufferMaxCount;
        void(cameraControllerBase::*setBufferMaxCount_2)(const size_t s)               = &cameraControllerBase::setBufferMaxCount;

        size_t(cameraControllerBase::*getBufferCount_1)(cstr&)  const                       = &cameraControllerBase::getBufferCount;
        size_t(cameraControllerBase::*getBufferCount_2)()       const                       = &cameraControllerBase::getBufferCount;
        size_t(cameraControllerBase::*getBufferMaxCount_1)(cstr&)const                      = &cameraControllerBase::getBufferMaxCount;
        size_t(cameraControllerBase::*getBufferMaxCount_2)()     const                      = &cameraControllerBase::getBufferMaxCount;


        bool(cameraControllerBase::*setGain_1)(const long, cstr&) = &cameraControllerBase::setGain;
        bool(cameraControllerBase::*setGain_2)(const long)        = &cameraControllerBase::setGain;
        bool(cameraControllerBase::*setBlacklevel_1)(const long, cstr&) = &cameraControllerBase::setBlacklevel;
        bool(cameraControllerBase::*setBlacklevel_2)(const long)        = &cameraControllerBase::setBlacklevel;

        int(cameraControllerBase::*getGain_1)(cstr&)const = &cameraControllerBase::getGain;
        int(cameraControllerBase::*getGain_2)() const     = &cameraControllerBase::getGain;
        int(cameraControllerBase::*getBlacklevel_1)(cstr&)const = &cameraControllerBase::getBlacklevel;
        int(cameraControllerBase::*getBlacklevel_2)()const      = &cameraControllerBase::getBlacklevel;



        boost::python::list(cameraControllerBase::*takeAndGetFastImage2D_1)(cstr&)= &cameraControllerBase::takeAndGetFastImage2D;
        boost::python::list(cameraControllerBase::*takeAndGetFastImage2D_2)()     = &cameraControllerBase::takeAndGetFastImage2D;

        boost::python::list(cameraControllerBase::*takeAndGetFastImage_1)(cstr&)  = &cameraControllerBase::takeAndGetFastImage;
        boost::python::list(cameraControllerBase::*takeAndGetFastImage_2)()       = &cameraControllerBase::takeAndGetFastImage;







        //Py_Initialize();
        //boost::python::numpy::initialize();

    class_<cameraControllerBase, bases<controller>,noncopyable>(py_name,py_docs, no_init)

        .def("claraLEDOn",  &cameraControllerBase::claraLEDOn,claraLEDOn_ds  )
        .def("claraLEDOff",  &cameraControllerBase::claraLEDOff,claraLEDOff_ds  )
        .def("isCLaraLEDOn",  &cameraControllerBase::isCLaraLEDOn,isCLaraLEDOn_ds  )
        .def("isCLaraLEDOff",  &cameraControllerBase::isCLaraLEDOff,isCLaraLEDOff_ds  )

        .def("getMaskObj_VC",  &cameraControllerBase::getMaskObj_VC, return_value_policy<reference_existing_object>(),getMaskObj_VC_ds  )
        .def("getMaskObj",  getMaskObj_1, return_value_policy<reference_existing_object>(),getMaskObj_ds1    )
        .def("getMaskObj",  getMaskObj_2, return_value_policy<reference_existing_object>(),getMaskObj_ds2    )

        .def("getAnalysisObj_VC",  &cameraControllerBase::getAnalysisObj_VC  , return_value_policy<reference_existing_object>(),getAnalysisObj_VC_ds )
        .def("getAnalysisObj",  getAnalysisObj_1       , return_value_policy<reference_existing_object>(), getAnalysisObj_ds1   )
        .def("getAnalysisObj",  getAnalysisObj_2       , return_value_policy<reference_existing_object>(), getAnalysisObj_ds2   )

        .def("getImageObj_VC",  &cameraControllerBase::getImageObj_VC       , return_value_policy<reference_existing_object>(),getImageObj_VC_ds )
        .def("getImageObj",  getImageObj_1            , return_value_policy<reference_existing_object>(),getImageObj_ds1)
        .def("getImageObj",  getImageObj_2            , return_value_policy<reference_existing_object>(),getImageObj_ds2)

        .def("getImageDataObj_VC",  &cameraControllerBase::getImageDataObj_VC  , return_value_policy<reference_existing_object>(),getImageDataObj_VC_ds  )
        .def("getImageDataObj",  getImageDataObj_2      , return_value_policy<reference_existing_object>(), getImageDataObj_ds2    )
        .def("getImageDataObj",  getImageDataObj_1      , return_value_policy<reference_existing_object>(), getImageDataObj_ds1    )

        .def("getClaraDAQObj_VC",  &cameraControllerBase::getClaraDAQObj_VC, return_value_policy<reference_existing_object>(),        getClaraDAQObj_VC_ds   )
        .def("getClaraDAQObj",  getClaraDAQObj_1       , return_value_policy<reference_existing_object>(), getClaraDAQObj_ds1  )
        .def("getClaraDAQObj",  getClaraDAQObj_2     , return_value_policy<reference_existing_object>(),   getClaraDAQObj_ds2  )

        .def("getCameraObj_VC",  &cameraControllerBase::getCameraObj_VC  , return_value_policy<reference_existing_object>(),getCameraObj_VC_ds )
        .def("getCameraObj",  getCameraObj_1       , return_value_policy<reference_existing_object>(), getCameraObj_ds1 )
        .def("getCameraObj",  getCameraObj_2       , return_value_policy<reference_existing_object>(), getCameraObj_ds2 )

        .def("getStateObj_VC",  &cameraControllerBase::getStateObj_VC  , return_value_policy<reference_existing_object>(),getStateObj_VC_ds )
        .def("getStateObj",  getStateObj_1 , return_value_policy<reference_existing_object>(), getStateObj_ds1 )
        .def("getStateObj",  getStateObj_2 , return_value_policy<reference_existing_object>(), getStateObj_ds2)

        .def("getPix2mmDef_VC",  &cameraControllerBase::getPix2mm_VC, getPix2mm_VC_ds )
        .def("getPix2mmDef",  getPix2mm_1       ,  getPix2mm_ds1 )
        .def("getPix2mmDef",  getPix2mm_2       ,  getPix2mm_ds2 )

        .def("getPix2mm_VC",  &cameraControllerBase::getPix2mmDef_VC, getPix2mmDef_VC_ds )
        .def("getPix2mm",  getPix2mmDef_1       ,  getPix2mmDef_ds1 )
        .def("getPix2mm",  getPix2mmDef_2       ,  getPix2mmDef_ds2 )


        .def("isAnalysisUpdating_VC",  &cameraControllerBase::isAnalysisUpdating_VC ,isAnalysisUpdating_VC_ds )
        .def("isAnalysisUpdating",  isAnalysisUpdating_1       ,  isAnalysisUpdating_ds1 )
        .def("isAnalysisUpdating",  isAnalysisUpdating_2       ,  isAnalysisUpdating_ds2 )


        .def("isAnalysisNotUpdating_VC",  &cameraControllerBase::isAnalysisNotUpdating_VC ,isAnalysisNotUpdating_VC_ds )
        .def("isAnalysisNotUpdating",  isAnalysisNotUpdating_1       ,  isAnalysisNotUpdating_ds1 )
        .def("isAnalysisNotUpdating",  isAnalysisNotUpdating_2       ,  isAnalysisNotUpdating_ds2 )

        .def("isMaskFeedbackOn_VC",  &cameraControllerBase::isMaskFeedbackOn_VC ,isMaskFeedbackOn_VC_ds )
        .def("isMaskFeedbackOn",  isMaskFeedbackOn_1       ,  isMaskFeedbackOn_ds1 )
        .def("isMaskFeedbackOn",  isMaskFeedbackOn_2       ,  isMaskFeedbackOn_ds2 )

        .def("isMaskFeedbackOfVC",  &cameraControllerBase::isMaskFeedbackOff_VC ,isMaskFeedbackOff_VC_ds )
        .def("isMaskFeedbackOff",  isMaskFeedbackOff_1       ,  isMaskFeedbackOff_ds1 )
        .def("isMaskFeedbackOff",  isMaskFeedbackOff_2       ,  isMaskFeedbackOff_ds2 )


        .def("setMaskFeedBackOn_VC",  &cameraControllerBase::setMaskFeedBackOn_VC ,setMaskFeedBackOn_VC_ds )
        .def("setMaskFeedBackOn",  setMaskFeedBackOn_1       ,  setMaskFeedBackOn_ds1 )
        .def("setMaskFeedBackOn",  setMaskFeedBackOn_2       ,  setMaskFeedBackOn_ds2 )

        .def("setMaskFeedBackOff_VC",  &cameraControllerBase::setMaskFeedBackOff_VC ,setMaskFeedBackOff_VC_ds )
        .def("setMaskFeedBackOff",  setMaskFeedBackOff_1       ,  setMaskFeedBackOff_ds1 )
        .def("setMaskFeedBackOff",  setMaskFeedBackOff_2       ,  setMaskFeedBackOff_ds2 )


        .def("hasBeam_VC",  &cameraControllerBase::hasBeam_VC ,hasBeam_VC_ds )
        .def("hasBeam",  hasBeam_1       ,  hasBeam_ds1 )
        .def("hasBeam",  hasBeam_2       ,  hasBeam_ds2 )


        .def("hasNoBeam_VC",  &cameraControllerBase::hasNoBeam_VC ,hasNoBeam_VC_ds )
        .def("hasNoBeam",  hasNoBeam_1       ,  hasNoBeam_ds1 )
        .def("hasNoBeam",  hasNoBeam_2       ,  hasNoBeam_ds2 )



        .def("stopAnalysing_VC",  &cameraControllerBase::stopAnalysing_VC ,stopAnalysing_VC_ds )
        .def("stopAnalysing",  stopAnalysing_1       ,  stopAnalysing_ds1 )
        .def("stopAnalysing",  stopAnalysing_2       ,  stopAnalysing_ds2 )
        .def("stopAllAnalysing",          &cameraControllerBase::stopAllAnalysing         ,stopAllAnalysing_ds )
        .def("stopAllAnalysingExceptVC",  &cameraControllerBase::stopAllAnalysingExceptVC ,stopAllAnalysingExceptVC_ds )
        .def("startAnalysing_VC",              &cameraControllerBase::startAnalysing_VC  ,                        startAnalysing_VC_ds)
        .def("startAnalysing",                 startAnalysing_1                          , startAnalysing_ds_1)
        .def("startAnalysing",                 startAnalysing_2                          , startAnalysing_ds_2)

        .def("takeFastImage_VC", &cameraControllerBase::takeFastImage_VC, takeFastImage_VC_ds)
        .def("takeFastImage",    takeFastImage_1                        , takeFastImage_ds1)
        .def("takeFastImage",    takeFastImage_2                        , takeFastImage_ds2)

        .def("takeAndGetFastImage_VC", &cameraControllerBase::takeAndGetFastImage_VC, takeAndGetFastImage_VC_ds)
        .def("takeAndGetFastImage",    takeAndGetFastImage_1                        , takeAndGetFastImage_ds1)
        .def("takeAndGetFastImage",    takeAndGetFastImage_2                        , takeAndGetFastImage_ds2)

        .def("takeAndGetFastImage2D_VC", &cameraControllerBase::takeAndGetFastImage2D_VC, takeAndGetFastImage2D_VC_ds)
        .def("takeAndGetFastImage2D",    takeAndGetFastImage2D_1                        , takeAndGetFastImage2D_ds1)
        .def("takeAndGetFastImage2D",    takeAndGetFastImage2D_2                        , takeAndGetFastImage2D_ds2)


        .def("getCameraNames",       &cameraControllerBase::getCameraNames_Py                   , getCameraNames_ds)
        .def("getCameraScreenNames",    &cameraControllerBase::getCameraScreenNames_Py          , getCameraScreenNames_ds)


        .def("stopAcquireAndAnalysis_VC",  &cameraControllerBase::stopAcquireAndAnalysis_VC  , stopAcquireAndAnalysis_VC_ds)
        .def("stopAcquireAndAnalysis",     stopAcquireAndAnalysis_1                          , stopAcquireAndAnalysis_ds1)
        .def("stopAcquireAndAnalysis",     stopAcquireAndAnalysis_2                          , stopAcquireAndAnalysis_ds2)
        .def("startAcquireAndAnalysis_VC", &cameraControllerBase::startAcquireAndAnalysis_VC  ,startAcquireAndAnalysis_VC_ds)
        .def("startAcquireAndAnalysis",    startAcquireAndAnalysis_1 , startAcquireAndAnalysis_ds1)
        .def("startAcquireAndAnalysis",    startAcquireAndAnalysis_2 , startAcquireAndAnalysis_ds2)

        .def("isNotCollectingOrSaving_VC", &cameraControllerBase::isNotCollectingOrSaving_VC, isNotCollectingOrSaving_VC_ds)
        .def("isNotCollectingOrSaving",    isNotCollectingOrSaving_1, isNotCollectingOrSaving_ds1)
        .def("isNotCollectingOrSaving",    isNotCollectingOrSaving_2, isNotCollectingOrSaving_ds2)

        .def("isUsingBackground_VC",    &cameraControllerBase::isUsingBackground_VC     , isUsingBackground_VC_ds)
        .def("isUsingBackground",       isUsingBackground_1                             , isUsingBackground_ds1)
        .def("isUsingBackground",       isUsingBackground_2                             , isUsingBackground_ds2)
        .def("isNotUsingBackground_VC",  &cameraControllerBase::isNotUsingBackground_VC , isNotUsingBackground_VC_ds)
        .def("isNotUsingBackground",    isNotUsingBackground_1                          , isNotUsingBackground_ds1)
        .def("isNotUsingBackground",    isNotUsingBackground_2                          , isNotUsingBackground_ds2)
        .def("isUsingNPoint_VC",        &cameraControllerBase::isUsingNPoint_VC         , isUsingNPoint_VC_ds)
        .def("isUsingNPoint",           isUsingNPoint_1                                 , isUsingNPoint_ds1)
        .def("isUsingNPoint",           isUsingNPoint_2                                 , isUsingNPoint_ds2)
        .def("ivc_data2sNotUsingNPoint_VC",     &cameraControllerBase::isNotUsingNPoint_VC      , isNotUsingNPoint_VC_ds)
        .def("isNotUsingNPoint",        isNotUsingNPoint_1                              , isNotUsingNPoint_ds1)
        .def("isNotUsingNPoint",        isNotUsingNPoint_2                              , isNotUsingNPoint_ds2)

        .def("isBusy_VC",                 &cameraControllerBase::isBusy_VC     , isBusy_VC_ds)
        .def("isBusy",                      isBusy_1            ,                                  isBusy_ds1)
        .def("isBusy",                      isBusy_2            ,                                  isBusy_ds2)
        .def("isNotBusy_VC",              &cameraControllerBase::isNotBusy_VC , isNotBusy_VC_ds)
        .def("isNotBusy",                 isNotBusy_1                         , isNotBusy_ds1)
        .def("isNotBusy",                 isNotBusy_2                         , isNotBusy_ds2)

        .def("isClaraCam_VC",     &cameraControllerBase::isClaraCam_VC, isClaraCam_VC_ds)
        .def("isClaraCam",        isClaraCam_1            ,                                  isClaraCam_ds1)
        .def("isClaraCam",        isClaraCam_2            ,                                  isClaraCam_ds2)

        .def("isVelaCam_VC",     &cameraControllerBase::isVelaCam_VC, isVelaCam_VC_ds)
        .def("isVelaCam",        isVelaCam_1            ,                                  isVelaCam_ds1)
        .def("isVelaCam",        isVelaCam_2            ,                                  isVelaCam_ds2)



        .def("isAnalysing_VC",                 &cameraControllerBase::isAnalysing_VC     , isAnalysing_VC_ds)
        .def("isAnalysing",                      isAnalysing_1            ,                                  isAnalysing_ds_1)
        .def("isAnalysing",                      isAnalysing_2            ,                                  isAnalysing_ds_2)
        .def("isNotAnalysing_VC",              &cameraControllerBase::isNotAnalysing_VC , isNotAnalysing_VC_ds)
        .def("isNotAnalysing",                 isNotAnalysing_1                         , isNotAnalysing_ds_1)
        .def("isNotAnalysing",                 isNotAnalysing_2                         , isNotAnalysing_ds_2)


        .def("stopAcquiring_VC",  &cameraControllerBase::stopAcquiring_VC ,getCameraObj_VC_ds )
        .def("stopAcquiring",  stopAcquiring_1       ,  stopAcquiring_ds1 )
        .def("stopAcquiring",  stopAcquiring_2       ,  stopAcquiring_ds2 )
        .def("stopAllAcquiring",          &cameraControllerBase::stopAllAcquiring         ,stopAllAcquiring_ds )
        .def("stopAllAcquiringExceptVC",  &cameraControllerBase::stopAllAcquiringExceptVC ,stopAllAcquiringExceptVC_ds )
        .def("startAcquiring_VC",              &cameraControllerBase::startAcquiring_VC              ,                                               startAcquiring_VC_ds)
        .def("startAcquiring",                 startAcquiring_1                                      ,                                               startAcquiring_ds_1)
        .def("startAcquiring",                 startAcquiring_2                                      ,                                               startAcquiring_ds_2)
        .def("isAcquiring_VC",                 &cameraControllerBase::isAcquiring_VC                 ,                                               isAcquiring_VC_ds)
        .def("isAcquiring",                                                    isAcquiring_1                                   ,                                  isAcquiring_ds_1)
        .def("isAcquiring",                                                    isAcquiring_2                                   ,                                  isAcquiring_ds_2)
        .def("isNotAcquiring_VC",              &cameraControllerBase::isNotAcquiring_VC              ,                                               isNotAcquiring_VC_ds)
        .def("isNotAcquiring",                 isNotAcquiring_1                                      ,                                               isNotAcquiring_ds_1)
        .def("isNotAcquiring",                 isNotAcquiring_2                                      ,                                               isNotAcquiring_ds_2)


        .def("isON_VC",                        &cameraControllerBase::isON_VC                        ,                                               isON_VC_ds)
        .def("isON",                           isON_1                                                ,                                               isON_ds_1)
        .def("isON",                           isON_2                                                ,                                               isON_ds_2)
        .def("isOFF_VC",                       &cameraControllerBase::isOFF_VC                       ,                                               isOFF_VC_ds)
        .def("isOFF",                          isOFF_1                                               ,                                               isOFF_ds_1)
        .def("isOFF",                          isOFF_2                                               ,                                               isOFF_ds_2)
        .def("isAcquiring_VC",                 &cameraControllerBase::isAcquiring_VC                 ,                                               isAcquiring_VC_ds)
        .def("isAcquiring",                                                    isAcquiring_1                                   ,                                  isAcquiring_ds_1)
        .def("isAcquiring",                                                    isAcquiring_2                                   ,                                  isAcquiring_ds_2)
        .def("isNotAcquiring_VC",              &cameraControllerBase::isNotAcquiring_VC              ,                                               isNotAcquiring_VC_ds)
        .def("isNotAcquiring",                 isNotAcquiring_1                                      ,                                               isNotAcquiring_ds_1)
        .def("isNotAcquiring",                 isNotAcquiring_2                                      ,                                               isNotAcquiring_ds_2)
        .def("getMaskX_VC",                    &cameraControllerBase::getMaskX_VC                    ,                                               getMaskX_VC_ds)
        .def("getMaskX",                       getMaskX_1                                            ,                                               getMaskX_ds_1)
        .def("getMaskX",                       getMaskX_2                                            ,                                               getMaskX_ds_2)
        .def("getMaskY_VC",                    &cameraControllerBase::getMaskY_VC                    ,                                               getMaskY_VC_ds)
        .def("getMaskY",                       getMaskY_1                                            ,                                               getMaskY_ds_1)
        .def("getMaskY",                       getMaskY_2                                            ,                                               getMaskY_ds_2)
        .def("getMaskXrad_VC",                 &cameraControllerBase::getMaskXrad_VC                 ,                                               getMaskXrad_VC_ds)
        .def("getMaskXrad",                    getMaskXrad_1                                   ,                                  getMaskXrad_ds_1)
        .def("getMaskXrad",                    getMaskXrad_2                                   ,                                  getMaskXrad_ds_2)
        .def("getMaskYrad_VC",                 &cameraControllerBase::getMaskYrad_VC                 ,                                               getMaskYrad_VC_ds)
        .def("getMaskYrad",                    getMaskYrad_1                                         ,                                               getMaskYrad_ds_1)
        .def("getMaskYrad",                    getMaskYrad_2                                         ,                                               getMaskYrad_ds_2)
        .def("setMaskX_VC",                    &cameraControllerBase::setMaskX_VC                    ,                                               setMaskX_VC_ds)
        .def("setMaskX",                       setMaskX_1                                            ,                                               setMaskX_ds_1)
        .def("setMaskX",                       setMaskX_2                                            ,                                               setMaskX_ds_2)
        .def("setMaskY_VC",                    &cameraControllerBase::setMaskY_VC                    ,                                               setMaskY_VC_ds)
        .def("setMaskY",                       setMaskY_1                                            ,                                               setMaskY_ds_1)
        .def("setMaskY",                       setMaskY_2                                            ,                                               setMaskY_ds_2)
        .def("setMaskXrad_VC",                 &cameraControllerBase::setMaskXrad_VC                 ,                                               setMaskXrad_VC_ds)
        .def("setMaskXrad",                    setMaskXrad_1                                         ,                                               setMaskXrad_ds_1)
        .def("setMaskXrad",                    setMaskXrad_2                                         ,                                               setMaskXrad_ds_2)
        .def("setMaskYrad_VC",                 &cameraControllerBase::setMaskYrad_VC                 ,                                               setMaskYrad_VC_ds)
        .def("setMaskYrad",                    setMaskYrad_1                                         ,                                               setMaskYrad_ds_1)
        .def("setMaskYrad",                    setMaskYrad_2                                         ,                                               setMaskYrad_ds_2)
        //
        //.def("setMask_VC",                     setMask_VC_1                                          ,                                               setMask_VC_ds1)
        //.def("setMask_VC",                     setMask_VC_2                                          ,                                               setMask_VC_ds2)
        .def("setMask",                        setMask_1                                             ,                                               setMask_1_ds_1)
        .def("setMask",                        setMask_2                                             ,                                               setMask_2_ds_2)
        //
        .def("setMask_VC",                     &cameraControllerBase::setMask_VC_Py  , setMask_VC_Py_ds)
        .def("setMask",                        &cameraControllerBase::setMask_Py1,                   setMask_Py1_ds)
        .def("setMask",                        &cameraControllerBase::setMask_Py2,                   setMask_Py2_ds)
        //
        .def("setBackground_VC",               &cameraControllerBase::setBackground_VC               ,                                               setBackground_VC_ds)
        .def("setBackground",                  setBackground_1                                       ,                                               setBackground_ds_1)
        .def("setBackground",                  setBackground_2                                       ,                                               setBackground_ds_2)
        .def("useBackground_VC",               &cameraControllerBase::useBackground_VC               ,                                               useBackground_VC_ds)
        .def("useBackground",                  useBackground_1                                       ,                                               useBackground_ds_1)
        .def("useBackground",                  useBackground_2                                       ,                                               useBackground_ds_2)
        .def("setStepSize_VC",                 &cameraControllerBase::setStepSize_VC                 ,                                               setStepSize_VC_ds)
        .def("setStepSize",                    setStepSize_1                                         ,                                               setStepSize_ds_1)
        .def("setStepSize",                    setStepSize_2                                         ,                                               setStepSize_ds_2)
        .def("setCenterXPixel_VC",             &cameraControllerBase::setCenterXPixel_VC             ,                                               setCenterXPixel_VC_ds)
        .def("setCenterXPixel",                setCenterXPixel_1                                     ,                                               setCenterXPixel_ds_1)
        .def("setCenterXPixel",                setCenterXPixel_2                                     ,                                               setCenterXPixel_ds_2)
        .def("setCenterYPixel_VC",             &cameraControllerBase::setCenterYPixel_VC             ,                                               setCenterYPixel_VC_ds)
        .def("setCenterYPixel",                setCenterYPixel_1                                     ,                                               setCenterYPixel_ds_1)
        .def("setCenterYPixel",                setCenterYPixel_2                                     ,                                               setCenterYPixel_ds_2)
        .def("setPixMM_VC",                    &cameraControllerBase::setPixMM_VC                    ,                                               setPixMM_VC_ds)
        .def("setPixMM",                       setPixMM_1                                            ,                                               setPixMM_ds_1)
        .def("setPixMM",                       setPixMM_2                                            ,                                               setPixMM_ds_2)
        .def("useNPoint_VC",                   &cameraControllerBase::useNPoint_VC                   ,                                               useNPoint_VC_ds)
        .def("useNPoint",                      useNPoint_1,                                          useNPoint_ds_1)
        .def("useNPoint",                      useNPoint_2,                                          useNPoint_ds_2)
        .def("startAnalysis_VC",               &cameraControllerBase::startAnalysis_VC               ,                                               startAnalysis_VC_ds)
        .def("startAnalysis",                  startAnalysis_1                                       ,                                               startAnalysis_ds_1)
        .def("startAnalysis",                  startAnalysis_2                                       ,                                               startAnalysis_ds_2)
        .def("stopAnalysis_VC",                &cameraControllerBase::stopAnalysis_VC                ,                                               stopAnalysis_VC_ds)
        .def("stopAnalysis",                   stopAnalysis_1                                        ,                                               stopAnalysis_ds_1)
        .def("stopAnalysis",                   stopAnalysis_2                                        ,                                               stopAnalysis_ds_2)
        .def("isCollecting_VC",                &cameraControllerBase::isCollecting_VC                ,                                               isCollecting_VC_ds)
        .def("isCollecting",                   isCollecting_1                                        ,                                               isCollecting_ds_1)
        .def("isCollecting",                   isCollecting_2                                        ,                                               isCollecting_ds_2)
        .def("isSaving_VC",                   &cameraControllerBase::isSaving_VC                   ,                                               isSaving_VC_ds)
        .def("isSaving",                      isSaving_1,                                          isSaving_ds_1)
        .def("isSaving",                      isSaving_2,                                          isSaving_ds_2)
        .def("isCollectingJPG_VC",             &cameraControllerBase::isCollectingJPG_VC             ,                                               isCollectingJPG_VC_ds)
        .def("isCollectingJPG",                isCollectingJPG_1                                     ,                                               isCollectingJPG_ds_1)
        .def("isCollectingJPG",                isCollectingJPG_2                                     ,                                               isCollectingJPG_ds_2)
        .def("isSavingJPG_VC",                &cameraControllerBase::isSavingJPG_VC                ,                                               isSavingJPG_VC_ds)
        .def("isSavingJPG",                   isSavingJPG_1                                        ,                                               isSavingJPG_ds_1)
        .def("isSavingJPG",                   isSavingJPG_2                                        ,                                               isSavingJPG_ds_2)
        .def("isCollectingOrSaving_VC",       &cameraControllerBase::isCollectingOrSaving_VC       ,                                               isCollectingOrSaving_VC_ds)
        .def("isCollectingOrSaving",          isCollectingOrSaving_1                               ,                                               isCollectingOrSaving_ds_1)
        .def("isCollectingOrSaving",          isCollectingOrSaving_2                               ,                                               isCollectingOrSaving_ds_2)
        .def("getXBuffer_VC",               &cameraControllerBase::getXBuffer_VC_Py               ,                                               getXBuffer_VC_Py_ds)
        .def("getYBuffer_VC",               &cameraControllerBase::getYBuffer_VC_Py               ,                                               getYBuffer_VC_Py_ds)
        .def("getSigXBuffer_VC",            &cameraControllerBase::getSigXBuffer_VC_Py            ,                                               getSigXBuffer_VC_Py_ds)
        .def("getSigYBuffer_VC",            &cameraControllerBase::getSigYBuffer_VC_Py             ,                                              getSigYBuffer_VC_Py_ds)
        .def("getSigXYBuffer_VC",           &cameraControllerBase::getSigXYBuffer_VC_Py           ,                                               getSigXYBuffer_VC_Py_ds)
        .def("getXPixBuffer_VC",            &cameraControllerBase::getXPixBuffer_VC_Py            ,                                               getXPixBuffer_VC_Py_ds)
        .def("getYPixBuffer_VC",            &cameraControllerBase::getYPixBuffer_VC_Py            ,                                               getYPixBuffer_VC_Py_ds)
        .def("getSigXPixBuffer_VC",                                                    &cameraControllerBase::getSigXPixBuffer_VC_Py   ,                                  getSigXPixBuffer_VC_Py_ds)
        .def("getSigYPixBuffer_VC",                                                    &cameraControllerBase::getSigYPixBuffer_VC_Py   ,                                  getSigYPixBuffer_VC_Py_ds)
        .def("getSigXYPixBuffer_VC",        &cameraControllerBase::getSigXYPixBuffer_VC_Py,       getSigXYPixBuffer_VC_Py_ds)
        .def("getPixelValuesBuffer_VC",     &cameraControllerBase::getPixelValuesBuffer_VC_Py     ,                                               getPixelValuesBuffer_VC_Py_ds)
        .def("getPixelValues_VC",           &cameraControllerBase::getPixelValues_VC_Py           ,                                               getPixelValues_VC_Py_ds)
        .def("getFastImage_VC",             &cameraControllerBase::getFastImage_VC_Py             ,                                               getFastImage_VC_Py_ds)
        .def("getSumIntensityBuffer_VC",    &cameraControllerBase::getSumIntensityBuffer_VC_Py    ,                                               getSumIntensityBuffer_VC_Py_ds)
        .def("getAvgIntensityBuffer_VC",    &cameraControllerBase::getAvgIntensityBuffer_VC_Py    ,                                               getAvgIntensityBuffer_VC_Py_ds)

        .def("getFastImage2D_VC", &cameraControllerBase::getFastImage2D_VC_Py ,getFastImage2D_VC_Py_ds)
        .def("getFastImage2D",    &cameraControllerBase::getFastImage2D_Py1   ,getFastImage2D_Py1_ds)
        .def("getFastImage2D",    &cameraControllerBase::getFastImage2D_Py2   ,getFastImage2D_Py2_ds)


        .def("getStepSize_VC", &cameraControllerBase::getStepSize_VC ,getStepSize_VC_ds)
        .def("getStepSize",    getStepSize_1,getStepSize_ds1)
        .def("getStepSize",    getStepSize_2,getStepSize_ds2)

        .def("getGain_VC", &cameraControllerBase::getGain_VC ,getGain_VC_ds)
        .def("getGain",    getGain_1,getGain_ds1)
        .def("getGain",    getGain_2,getGain_ds2)

        .def("setGain_VC", &cameraControllerBase::setGain_VC ,setGain_VC_ds)
        .def("setGain",    setGain_1,setGain_ds1)
        .def("setGain",    setGain_2,setGain_ds2)

        .def("getBlacklevel_VC", &cameraControllerBase::getBlacklevel_VC ,getBlacklevel_VC_ds)
        .def("getBlacklevel",    getBlacklevel_1,getBlacklevel_ds1)
        .def("getBlacklevel",    getBlacklevel_2,getBlacklevel_ds2)

        .def("setBlacklevel_VC", &cameraControllerBase::setBlacklevel_VC ,setBlacklevel_VC_ds)
        .def("setBlacklevel",    setBlacklevel_1,setBlacklevel_ds1)
        .def("setBlacklevel",    setBlacklevel_2,setBlacklevel_ds2)



//        .def("getXBuffer_VC_NumPy",            &cameraControllerBase::getXBuffer_VC_NumPy            ,                                               getXBuffer_VC_NumPy_ds)
//        .def("getYBuffer_VC_NumPy",            &cameraControllerBase::getYBuffer_VC_NumPy            ,                                               getYBuffer_VC_NumPy_ds)
//        .def("getSigXBuffer_VC_NumPy",         &cameraControllerBase::getSigXBuffer_VC_NumPy         ,                                               getSigXBuffer_VC_NumPy_ds)
//        .def("getSigYBuffer_VC_NumPy",         &cameraControllerBase::getSigYBuffer_VC_NumPy         ,                                               getSigYBuffer_VC_NumPy_ds)
//        .def("getSigXYBuffer_VC_NumPy",        &cameraControllerBase::getSigXYBuffer_VC_NumPy,       getSigXYBuffer_VC_NumPy_ds)
//        .def("getXPixBuffer_VC_NumPy",         &cameraControllerBase::getXPixBuffer_VC_NumPy         ,                                 getXPixBuffer_VC_NumPy_ds)
//        .def("getYPixBuffer_VC_NumPy",         &cameraControllerBase::getYPixBuffer_VC_NumPy         ,                                 getYPixBuffer_VC_NumPy_ds)
//        .def("getSigXPixBuffer_VC_NumPy",      &cameraControllerBase::getSigXPixBuffer_VC_NumPy      ,                                 getSigXPixBuffer_VC_NumPy_ds)
//        .def("getSigYPixBuffer_VC_NumPy",      &cameraControllerBase::getSigYPixBuffer_VC_NumPy      ,                                 getSigYPixBuffer_VC_NumPy_ds)
//        .def("getSigXYPixBuffer_VC_NumPy",     &cameraControllerBase::getSigXYPixBuffer_VC_NumPy     ,                                 getSigXYPixBuffer_VC_NumPy_ds)
//        .def("getPixelValues_VC_NumPy",        &cameraControllerBase::getPixelValues_VC_NumPy,       getPixelValues_VC_NumPy_ds)
//
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//        //.def("getPixelValuesBuffer_VC_NumPy",  &cameraControllerBase::getPixelValuesBuffer_VC_NumPy, getPixelValuesBuffer_VC_NumPy_ds)
//        //.def("getPixelValuesBuffer_NumPy",  &cameraControllerBase::getPixelValuesBuffer_NumPy1 , getPixelValuesBuffer_NumPy1_ds1)
//        //.def("getPixelValuesBuffer_NumPy",  &cameraControllerBase::getPixelValuesBuffer_NumPy2 , getPixelValuesBuffer_NumPy2_ds2)
//
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//        .def("getFastImage_VC_NumPy",          &cameraControllerBase::getFastImage_VC_NumPy          ,                                 getFastImage_VC_NumPy_ds)
//        .def("getSumIntensityBuffer_VC_NumPy", &cameraControllerBase::getSumIntensityBuffer_VC_NumPy ,                                 getSumIntensityBuffer_VC_NumPy_ds)
//        .def("getAvgIntensityBuffer_VC_NumPy", &cameraControllerBase::getAvgIntensityBuffer_VC_NumPy ,                                 getAvgIntensityBuffer_VC_NumPy_ds)
//
//        .def("getXBuffer_NumPy",            &cameraControllerBase::getXBuffer_NumPy1           , getXBuffer_NumPy1_ds1)
//        .def("getXBuffer_NumPy",            &cameraControllerBase::getXBuffer_NumPy2           , getXBuffer_NumPy2_ds2)
//        .def("getYBuffer_NumPy",            &cameraControllerBase::getYBuffer_NumPy1           , getYBuffer_NumPy1_ds1)
//        .def("getYBuffer_NumPy",            &cameraControllerBase::getYBuffer_NumPy2           , getYBuffer_NumPy2_ds2)
//        .def("getSigXBuffer_NumPy",         &cameraControllerBase::getSigXBuffer_NumPy1        , getSigXBuffer_NumPy1_ds1)
//        .def("getSigXBuffer_NumPy",         &cameraControllerBase::getSigXBuffer_NumPy2        , getSigXBuffer_NumPy2_ds2)
//        .def("getSigYBuffer_NumPy",         &cameraControllerBase::getSigYBuffer_NumPy1        , getSigYBuffer_NumPy1_ds1)
//        .def("getSigYBuffer_NumPy",         &cameraControllerBase::getSigYBuffer_NumPy2        , getSigYBuffer_NumPy2_ds2)
//        .def("getSigXYBuffer_NumPy",        &cameraControllerBase::getSigXYBuffer_NumPy1       , getSigXYBuffer_NumPy1_ds1)
//        .def("getSigXYBuffer_NumPy",        &cameraControllerBase::getSigXYBuffer_NumPy2       , getSigXYBuffer_NumPy2_ds2)
//        .def("getXPixBuffer_NumPy",         &cameraControllerBase::getXPixBuffer_NumPy1        , getXPixBuffer_NumPy1_ds1)
//        .def("getXPixBuffer_NumPy",         &cameraControllerBase::getXPixBuffer_NumPy2        , getXPixBuffer_NumPy2_ds2)
//        .def("getYPixBuffer_NumPy",         &cameraControllerBase::getYPixBuffer_NumPy1        , getYPixBuffer_NumPy1_ds1)
//        .def("getYPixBuffer_NumPy",         &cameraControllerBase::getYPixBuffer_NumPy2        , getYPixBuffer_NumPy2_ds2)
//
//        .def("getSigXPixBuffer_NumPy",      &cameraControllerBase::getSigXPixBuffer_NumPy1     , getSigXPixBuffer_NumPy1_ds1)
//        .def("getSigXPixBuffer_NumPy",      &cameraControllerBase::getSigXPixBuffer_NumPy2     , getSigXPixBuffer_NumPy2_ds2)
//        .def("getSigYPixBuffer_NumPy",      &cameraControllerBase::getSigYPixBuffer_NumPy1     , getSigYPixBuffer_NumPy1_ds1)
//        .def("getSigYPixBuffer_NumPy",      &cameraControllerBase::getSigYPixBuffer_NumPy2     , getSigYPixBuffer_NumPy2_ds2)
//        .def("getSigXYPixBuffer_NumPy1",    &cameraControllerBase::getSigXYPixBuffer_NumPy1    , getSigXYPixBuffer_NumPy1_ds1)
//        .def("getSigXYPixBuffer_NumPy2",    &cameraControllerBase::getSigXYPixBuffer_NumPy2    , getSigXYPixBuffer_NumPy2_ds2)
//        .def("getPixelValues_NumPy",        &cameraControllerBase::getPixelValues_NumPy1       , getPixelValues_NumPy1_ds1)
//        .def("getPixelValues_NumPy",        &cameraControllerBase::getPixelValues_NumPy2       , getPixelValues_NumPy2_ds2)
//        .def("getFastImage_NumPy",          &cameraControllerBase::getFastImage_NumPy1         , getFastImage_NumPy1_ds1)
//        .def("getFastImage_NumPy",          &cameraControllerBase::getFastImage_NumPy2         , getFastImage_NumPy2_ds2)
//        .def("getSumIntensityBuffer_NumPy", &cameraControllerBase::getSumIntensityBuffer_NumPy1 , getSumIntensityBuffer_NumPy1_ds1)
//        .def("getSumIntensityBuffer_NumPy", &cameraControllerBase::getSumIntensityBuffer_NumPy2 , getSumIntensityBuffer_NumPy2_ds2)
//        .def("getAvgIntensityBuffer_NumPy", &cameraControllerBase::getAvgIntensityBuffer_NumPy1 , getAvgIntensityBuffer_NumPy1_ds1)
//        .def("getAvgIntensityBuffer_NumPy", &cameraControllerBase::getAvgIntensityBuffer_NumPy2 , getAvgIntensityBuffer_NumPy2_ds2)


        .def("clearRunningValues_VC", &cameraControllerBase::clearRunningValues_VC , clearRunningValues_VC_ds)
        .def("clearRunningValues",                           clearRunningValues_1  , clearRunningValues_ds1  )
        .def("clearRunningValues",                           clearRunningValues_2  , clearRunningValues_ds2  )
        .def("isBufferFull_VC",                &cameraControllerBase::isBufferFull_VC                ,                                 isBufferFull_VC_ds)
        .def("isBufferNotFull_VC",             &cameraControllerBase::isBufferNotFull_VC             ,                                 isBufferNotFull_VC_ds)
        .def("getX_VC",                        &cameraControllerBase::getX_VC                        ,                                 getX_VC_ds)
        .def("getY_VC",                        &cameraControllerBase::getY_VC                        ,                                 getY_VC_ds)
        .def("getSigX_VC",                     &cameraControllerBase::getSigX_VC,                    getSigX_VC_ds)
        .def("getSigY_VC",                     &cameraControllerBase::getSigY_VC,                    getSigY_VC_ds)
        .def("getSigXY_VC",                    &cameraControllerBase::getSigXY_VC,                                 getSigXY_VC_ds)
        .def("getXPix_VC",                     &cameraControllerBase::getXPix_VC,                    getXPix_VC_ds)
        .def("getYPix_VC",                     &cameraControllerBase::getYPix_VC,                    getYPix_VC_ds)
        .def("getSigXPix_VC",                  &cameraControllerBase::getSigXPix_VC                  ,                                 getSigXPix_VC_ds)
        .def("getSigYPix_VC",                  &cameraControllerBase::getSigYPix_VC                  ,                                 getSigYPix_VC_ds)
        .def("getSigXYPix_VC",                 &cameraControllerBase::getSigXYPix_VC                 ,                                 getSigXYPix_VC_ds)
        .def("setBufferMaxCount_VC",           &cameraControllerBase::setBufferMaxCount_VC           ,                                 setBufferMaxCount_VC_ds)
        .def("clearBuffer_VC",                 &cameraControllerBase::clearBuffer_VC                 ,                                 clearBuffer_VC_ds)
        .def("getBufferCount_VC",              &cameraControllerBase::getBufferCount_VC              ,                                 getBufferCount_VC_ds)
        .def("getBufferMaxCount_VC",           &cameraControllerBase::getBufferMaxCount_VC           ,                                 getBufferMaxCount_VC_ds)
        .def("getXBuffer",                  &cameraControllerBase::getXBuffer_Py1              , getXBuffer_Py1_ds1)
        .def("getXBuffer",                  &cameraControllerBase::getXBuffer_Py2              , getXBuffer_Py2_ds2)
        .def("getYBuffer",                  &cameraControllerBase::getYBuffer_Py1              , getYBuffer_Py1_ds1)
        .def("getYBuffer",                  &cameraControllerBase::getYBuffer_Py2              , getYBuffer_Py2_ds2)
        .def("getSigXBuffer",               &cameraControllerBase::getSigXBuffer_Py1           , getSigXBuffer_Py1_ds1)
        .def("getSigXBuffer",               &cameraControllerBase::getSigXBuffer_Py2           , getSigXBuffer_Py2_ds2)
        .def("getSigYBuffer",               &cameraControllerBase::getSigYBuffer_Py1           , getSigYBuffer_Py1_ds1)
        .def("getSigYBuffer",               &cameraControllerBase::getSigYBuffer_Py2           , getSigYBuffer_Py2_ds2)
        .def("getSigXYBuffer",              &cameraControllerBase::getSigXYBuffer_Py1          , getSigXYBuffer_Py1_ds1)
        .def("getSigXYBuffer",              &cameraControllerBase::getSigXYBuffer_Py2          , getSigXYBuffer_Py2_ds2)
        .def("getXPixBuffer",               &cameraControllerBase::getXPixBuffer_Py1           , getXPixBuffer_Py1_ds1)
        .def("getXPixBuffer",               &cameraControllerBase::getXPixBuffer_Py2           , getXPixBuffer_Py2_ds2)
        .def("getYPixBuffer",               &cameraControllerBase::getYPixBuffer_Py1           , getYPixBuffer_Py1_ds1)
        .def("getYPixBuffer",               &cameraControllerBase::getYPixBuffer_Py2           , getYPixBuffer_Py2_ds2)
        .def("getSigXPixBuffer",            &cameraControllerBase::getSigXPixBuffer_Py1        , getSigXPixBuffer_Py1_ds1)
        .def("getSigXPixBuffer",            &cameraControllerBase::getSigXPixBuffer_Py2        , getSigXPixBuffer_Py2_ds2)
        .def("getSigYPixBuffer",            &cameraControllerBase::getSigYPixBuffer_Py1        , getSigYPixBuffer_Py1_ds1)
        .def("getSigYPixBuffer",            &cameraControllerBase::getSigYPixBuffer_Py2        , getSigYPixBuffer_Py2_ds2)
        .def("getSigXYPixBuffer",           &cameraControllerBase::getSigXYPixBuffer_Py1       , getSigXYPixBuffer_Py1_ds1)
        .def("getSigXYPixBuffer",           &cameraControllerBase::getSigXYPixBuffer_Py2       , getSigXYPixBuffer_Py2_ds2)
        .def("getPixelValues",              &cameraControllerBase::getPixelValues_Py1          , getPixelValues_Py1_ds1)
        .def("getPixelValues",              &cameraControllerBase::getPixelValues_Py2          , getPixelValues_Py2_ds2)
        .def("getPixelValuesBuffer",        &cameraControllerBase::getPixelValuesBuffer_Py1    , getPixelValuesBuffer_Py1_ds1)
        .def("getPixelValuesBuffer",        &cameraControllerBase::getPixelValuesBuffer_Py2    , getPixelValuesBuffer_Py2_ds2)
        .def("getFastImage",                &cameraControllerBase::getFastImage_Py1            , getFastImage_Py1_ds1)
        .def("getFastImage",                &cameraControllerBase::getFastImage_Py2            , getFastImage_Py2_ds2)
        .def("getSumIntensityBuffer",       &cameraControllerBase::getSumIntensityBuffer_Py1   , getSumIntensityBuffer_Py1_ds1)
        .def("getSumIntensityBuffer",       &cameraControllerBase::getSumIntensityBuffer_Py2   , getSumIntensityBuffer_Py2_ds2)
        .def("getAvgIntensityBuffer",       &cameraControllerBase::getAvgIntensityBuffer_Py1   , getAvgIntensityBuffer_Py1_ds1)
        .def("getAvgIntensityBuffer",       &cameraControllerBase::getAvgIntensityBuffer_Py2   , getAvgIntensityBuffer_Py2_ds2)

        .def("isBufferFull",                       isBufferFull_1             ,    isBufferFull_ds1)
        .def("isBufferFull",                       isBufferFull_2             ,    isBufferFull_ds2)
        .def("isBufferNotFull",                    isBufferNotFull_1          ,    isBufferNotFull_ds1)
        .def("isBufferNotFull",                    isBufferNotFull_2          ,    isBufferNotFull_ds2)
        .def("getX",                               getX_1                     ,    getX_ds1)
        .def("getX",                               getX_2                     ,    getX_ds2)
        .def("getY",                               getY_1                     ,    getY_ds1)
        .def("getY",                               getY_2                     ,    getY_ds2)
        .def("getSigX",                            getSigX_1                  ,    getSigX_ds1)
        .def("getSigX",                            getSigX_2                  ,    getSigX_ds2)
        .def("getSigY",                            getSigY_1                  ,    getSigY_ds1)
        .def("getSigY",                            getSigY_2                  ,    getSigY_ds2)
        .def("getSigXY",                           getSigXY_1                 ,    getSigXY_ds1)
        .def("getSigXY",                           getSigXY_2                 ,    getSigXY_ds2)
        .def("getXPix",                            getXPix_1                  ,    getXPix_ds1)
        .def("getXPix",                            getXPix_2                  ,    getXPix_ds2)
        .def("getYPix",                            getYPix_1                  ,    getYPix_ds1)
        .def("getYPix",                            getYPix_2                  ,    getYPix_ds2)
        .def("getSigXPix",                         getSigXPix_1               ,    getSigXPix_ds1)
        .def("getSigXPix",                         getSigXPix_2               ,    getSigXPix_ds2)
        .def("getSigYPix",                         getSigYPix_1               ,    getSigYPix_ds1)
        .def("getSigYPix",                         getSigYPix_2               ,    getSigYPix_ds2)
        .def("getSigXYPix",                        getSigXYPix_1              ,    getSigXYPix_ds1)
        .def("getSigXYPix",                        getSigXYPix_2              ,    getSigXYPix_ds2)
        .def("setBufferMaxCount",                  setBufferMaxCount_1        ,    setBufferMaxCount_ds1)
        .def("setBufferMaxCount",                  setBufferMaxCount_2        ,    setBufferMaxCount_ds2)
        .def("clearBuffer",                        clearBuffer_1              ,    clearBuffer_ds1)
        .def("clearBuffer",                        clearBuffer_2              ,    clearBuffer_ds2)
        .def("getBufferCount",                     getBufferCount_1           ,    getBufferCount_ds1)
        .def("getBufferCount",                     getBufferCount_2           ,    getBufferCount_ds2)
        .def("getBufferMaxCount",                  getBufferMaxCount_1        ,    getBufferMaxCount_ds1)
        .def("getBufferMaxCount",                  getBufferMaxCount_2        ,    getBufferMaxCount_ds2)


        .def("isNotCollecting_VC",  &cameraControllerBase::isNotCollecting_VC  , isNotCollecting_VC_ds)
        .def("isNotCollecting",                            isNotCollecting_1   , isNotCollecting_ds1)
        .def("isNotCollecting",                            isNotCollecting_2   , isNotCollecting_ds2)
        .def("isSelectedCamera",    &cameraControllerBase::isSelectedCamera    , isSelectedCamera_ds)
        .def("isNotSelectedCamera", &cameraControllerBase::isNotSelectedCamera , isNotSelectedCamera_ds)
        .def("collectAndSave_VC",   &cameraControllerBase::collectAndSave_VC   , collectAndSave_VC_ds)
        .def("collectAndSave",      collectAndSave_1      , collectAndSave_ds1)
        .def("collectAndSave",      collectAndSave_2      , collectAndSave_ds2)

         .def("latestCollectAndSaveSuccess_VC",   &cameraControllerBase::latestCollectAndSaveSuccess_VC   , latestCollectAndSaveSuccess_VC_ds)
         .def("latestCollectAndSaveSuccess",   &cameraControllerBase::latestCollectAndSaveSuccess   , latestCollectAndSaveSuccess_ds)

        ;

    }

//______________________________________________________________________________
    void reg_COLLECTING_STATE_enum(const char* py_name, const char* py_docs)
    {
        enum_<cameraStructs::COLLECTING_STATE>("COLLECTING_STATE")
            .value("NOT_COLLECTING",     cameraStructs::COLLECTING_STATE::NOT_COLLECTING  )
            .value("COLLECTING",         cameraStructs::COLLECTING_STATE::COLLECTING      )
            .value("COLLECTING_ERROR",   cameraStructs::COLLECTING_STATE::COLLECTING_ERROR)
            ;
    }
    void reg_SAVE_STATE_enum(const char* py_name, const char* py_docs)
    {
        enum_<cameraStructs::SAVE_STATE>("SAVE_STATE")
            .value("NOT_SAVING",     cameraStructs::SAVE_STATE::NOT_SAVING  )
            .value("SAVING",         cameraStructs::SAVE_STATE::SAVING      )
            .value("SAVING_ERROR",   cameraStructs::SAVE_STATE::SAVING_ERROR)
            ;
    }
    void reg_WRITE_CHECK_enum(const char* py_name, const char* py_docs)
    {
        enum_<cameraStructs::WRITE_CHECK>("WRITE_CHECK")
            .value("WRITE_CHECK_OK",     cameraStructs::WRITE_CHECK::WRITE_CHECK_OK  )
            .value("WRITE_CHECK_ERROR",  cameraStructs::WRITE_CHECK::WRITE_CHECK_ERROR      )
            ;
    }

    void reg_CAM_STATE_enum(const char* py_name, const char* py_docs)
    {
        enum_<cameraStructs::CAM_STATE>("CAM_STATE")
            .value("CAM_OFF",     cameraStructs::CAM_STATE::CAM_OFF  )
            .value("CAM_ON",      cameraStructs::CAM_STATE::CAM_ON      )
            .value("CAM_ERROR",   cameraStructs::CAM_STATE::CAM_ERROR)
            ;
    }
    void reg_CAM_TYPE_enum(const char* py_name, const char* py_docs)
    {
        enum_<cameraStructs::CAM_TYPE>("CAM_TYPE")
            .value("VELA_CAM",     cameraStructs::CAM_TYPE::VELA_CAM  )
            .value("CLARA_CAM",    cameraStructs::CAM_TYPE::CLARA_CAM      )
            .value("UNKNOWN_CAM",  cameraStructs::CAM_TYPE::UNKNOWN_CAM)
            ;
    }
    void reg_ACQUIRE_STATE_enum(const char* py_name, const char* py_docs)
    {
        enum_<cameraStructs::ACQUIRE_STATE>("ACQUIRE_STATE")
            .value("NOT_ACQUIRING",     cameraStructs::ACQUIRE_STATE::NOT_ACQUIRING  )
            .value("ACQUIRING",         cameraStructs::ACQUIRE_STATE::ACQUIRING      )
            .value("ACQUIRING_ERROR",   cameraStructs::ACQUIRE_STATE::ACQUIRING_ERROR)
            ;
    }

#endif// __INCLUDE_CAMERA_BASE__
//______________________________________________________________________________
    void export_BaseObjects()
    {
        /*
            this function exposes objects that can be defined in mulitple HWC
            without this you will get a Python warning that the objects have
            been defined multiple times
        */
        /*
            the name of the object as it will appear in Python
        */
        const char * py_name;
        /*
            the object docstring as it will appear in Python
        */
        const char * py_docs;
        //
        py_name = "std_vector_string";
        py_docs = "c++ standard vector of standard strings";
        //
        // MACHINE_MODE
        py_name = "MACHINE_MODE";
        py_docs = "MACHINE_MODE: an enum for different machine modes, virtual, "
 "physical or offline(no EPICS)";
        //boost::python::type_info info1 = type_id<MACHINE_MODE>();
        const converter::registration* reg1  = converter::registry::query(type_id<MACHINE_MODE>());
        /*
            if the class has not been registered, register it
        */
        if(reg1 == nullptr)
        {
            reg_MACHINE_MODE(py_name,py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_MACHINE_MODE(py_name,py_docs);
        }
        //
        // MACHINE_AREA
        py_name = "MACHINE_AREA";
        py_docs = "MACHINE_AREA: an enum for named areas in the machine";
        reg1  = converter::registry::query(type_id<MACHINE_AREA>());
        if(reg1 == nullptr)
        {
            reg_MACHINE_AREA(py_name,py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_MACHINE_AREA(py_name,py_docs);
        }
        //
        // CONTROLLER_TYPE
        py_name = "CONTROLLER_TYPE";
        py_docs = "CONTROLLER_TYPE: an enum for named hardware controllers";
        reg1  = converter::registry::query(type_id<CONTROLLER_TYPE>());
        if(reg1 == nullptr)
        {
            reg_CONTROLLER_TYPE(py_name,py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_CONTROLLER_TYPE(py_name,py_docs);
        }
        //
        // ILOCK_STATE
        py_name = "ILOCK_STATE";
        py_docs = "ILOCK_STATE: an enum for named interlock states in the machine";
        reg1  = converter::registry::query(type_id<ILOCK_STATE>());
        if(reg1 == nullptr)
        {
            reg_ILOCK_STATE_TYPE(py_name,py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_ILOCK_STATE_TYPE(py_name,py_docs);
        }
        //
        // STATE
        py_name = "STATE";
        py_docs = "STATE: an enum for named states in the machine";
        reg1 = converter::registry::query(type_id<STATE>());
        if(reg1 == nullptr)
        {
            reg_STATE_TYPE(py_name,py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_STATE_TYPE(py_name,py_docs);
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
        /*
            the class definition, exposed to python, cf templated
        */
        reg1  = converter::registry::query(type_id<vecs>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_vec_string(py_name, py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_vec_string(py_name, py_docs);
        }
        //
        // expose std::vector<double>
        py_name = "std_vector_double";
        py_docs = "c++ standard vector of doubles";
        reg1  = converter::registry::query(type_id<vecd>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_vec_double(py_name, py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_vec_double(py_name, py_docs);
        }
        /*
            Expose base classes,  these should never be multiply defined
            no_int  forces Python to not construct (init) this object
            pure virtual methods get their own function
        */
        //
        // expose baseObject
        py_name = "baseObject";
        py_docs = "at the base of it all";
        reg1  = converter::registry::query(type_id<baseObject>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_baseObject(py_name, py_docs);
        }
        else if((*reg1).m_to_python == nullptr)
        {
            reg_baseObject(py_name, py_docs);
        }
        //
        // expose controller base,
        /*
           NB pure_virtual methods MUST be overloaded in derived classes
           document strings for VCbase functions
        */
        py_name = "controller object";
        py_docs = "base class fro all controllers";
        reg1  = converter::registry::query(type_id<controller>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_controller(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_controller(py_name, py_docs);
        }
        //
        // expose VCbase
        py_name = "VCbase object";
        py_docs = "base class for all VELA_CLARA python module init objects";
        reg1  = converter::registry::query(type_id<VCbase>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_VCbase(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_VCbase(py_name, py_docs);
        }
#endif //__VC_ENUM_ONLY__


#ifdef __INCLUDE_CAMERA_BASE__
        using namespace cameraStructs;

        py_name = "analysis_mask object";
        py_docs = "holds data for camera image analysis_mask";
        reg1  = converter::registry::query(type_id<analysis_mask>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_analysis_mask(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_analysis_mask(py_name, py_docs);
        }

        py_name = "analysis_data object";
        py_docs = "holds all data for camera analysis";
        reg1  = converter::registry::query(type_id<analysis_data>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_analysis_data(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_analysis_data(py_name, py_docs);
        }

        py_name = "fast_image object";
        py_docs = "fast_image holds data for camera pixel array";
        reg1  = converter::registry::query(type_id<fast_image>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_fast_image(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_fast_image(py_name, py_docs);
        }

        py_name = "clara_DAQ object";
        py_docs = "holds DAQ data for CLARA cam";
        reg1  = converter::registry::query(type_id<clara_DAQ>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_clara_DAQ(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_clara_DAQ(py_name, py_docs);
        }

        py_name = "camera_image_data object";
        py_docs = "camera_image_data holds the following objects (name): fast_image (image), mask_data (mask), analysis_data (data)";
        reg1  = converter::registry::query(type_id<camera_image_data>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_camera_image_data(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_camera_image_data(py_name, py_docs);
        }


        py_name = "camera_state object";
        py_docs = "camera_state holds the states of the camera, including analysis states";
        reg1  = converter::registry::query(type_id<camera_image_data>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_camera_state(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_camera_state(py_name, py_docs);
        }

        py_name = "cameraObject";
        py_docs = "VELA/CLARA virtual camera object";
        reg1  = converter::registry::query(type_id<cameraObject>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_cameraObject(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_cameraObject(py_name, py_docs);
        }

        //
        // expose cameraControllerBase
        py_name = "cameraControllerBase object";
        py_docs = "base class for all controllers that use cameras";
        reg1  = converter::registry::query(type_id<cameraControllerBase>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_cameraControllerBase(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_cameraControllerBase(py_name, py_docs);
        }



        py_name = "COLLECTING_STATE object";
        py_docs = "COLLECTING_STATE of camera";
        reg1  = converter::registry::query(type_id<COLLECTING_STATE>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_COLLECTING_STATE_enum(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_COLLECTING_STATE_enum(py_name, py_docs);
        }

        py_name = "SAVE_STATE object";
        py_docs = "SAVE_STATE of camera";
        reg1  = converter::registry::query(type_id<SAVE_STATE>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_SAVE_STATE_enum(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_SAVE_STATE_enum(py_name, py_docs);
        }


        py_name = "WRITE_CHECK object";
        py_docs = "WRITE_CHECK of camera";
        reg1  = converter::registry::query(type_id<WRITE_CHECK>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_WRITE_CHECK_enum(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_WRITE_CHECK_enum(py_name, py_docs);
        }


        py_name = "CAM_STATE object";
        py_docs = "CAM_STATE of camera";
        reg1  = converter::registry::query(type_id<CAM_STATE>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_CAM_STATE_enum(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_CAM_STATE_enum(py_name, py_docs);
        }



        py_name = "CAM_TYPE object";
        py_docs = "CAM_TYPE of camera";
        reg1  = converter::registry::query(type_id<CAM_TYPE>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_CAM_TYPE_enum(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_CAM_TYPE_enum(py_name, py_docs);
        }


        py_name = "ACQUIRE_STATE object";
        py_docs = "ACQUIRE_STATE of camera";
        reg1  = converter::registry::query(type_id<ACQUIRE_STATE>());
        /*  if the class not been registered, register it */
        if(reg1 == nullptr)
        {
            reg_ACQUIRE_STATE_enum(py_name, py_docs);
        }
        else if ((*reg1).m_to_python == nullptr)
        {
            reg_ACQUIRE_STATE_enum(py_name, py_docs);
        }





#endif //__INCLUDE_CAMERA_BASE__

    } // void export_BaseObjects()
}
//______________________________________________________________________________
#endif // VCHEADER_H_INCLUDED
