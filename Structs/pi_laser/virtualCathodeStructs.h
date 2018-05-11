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
//  FileName:    virtualCathodeStructs.h
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_PIL_VIRTUAL_CATHODE_STRUCTS_H_
#define _VELA_CLARA_PIL_VIRTUAL_CATHODE_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
#include <deque>
//epics
#include <cadef.h>
//______________________________________________________________________________
class virtualCathodeInterface;
//______________________________________________________________________________
namespace virtualCathodeStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;
    struct virtualCathodeObject;
    struct pvStruct;
    // Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(VC_PV_TYPE,

                                                (X_RBV)
                                                 (Y_RBV)
                                                 (SIGMA_X_RBV)
                                                 (SIGMA_Y_RBV)
                                                 (COV_XY_RBV)
                                                 (X_PIX)
                                                 (Y_PIX)
                                                 (SIGMA_X_PIX)
                                                 (SIGMA_Y_PIX)
                                                 (COV_XY_PIX)
                                                 (VC_INTENSITY)
                                                 (PIXEL_RESULTS)
                                                 (H_POS)
                                                 (V_POS)
                                                 (H_MREL)
                                                 (V_MREL)
                                                 (H_STEP)
                                                 (V_STEP)
                                                 (H_MOVE)
                                                 (V_MOVE)
                                                 (H_STOP)
                                                 (V_STOP)
                                                 (POS_UPDATE)
                                                 (UNKNOWN)
                                        )
    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN),
            interface(nullptr),
            EVID(nullptr),
            object(nullptr)
            {}
        VC_PV_TYPE   monType;
        virtualCathodeInterface*  interface;
        chtype                    CHTYPE;
        evid                      EVID;
        virtualCathodeObject* object;
    };

    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_STRING),
            COUNT(UTL::ZERO_UL),
            MASK(UTL::ZERO_UL),
            pvType(UNKNOWN)
            {}
        VC_PV_TYPE    pvType;
        chid          CHID;
        std::string   pvSuffix;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };
    // the data from each PV will go, timestamped, in a data struct
    struct data
    {
        data():
            value(UTL::DUMMY_DOUBLE)
            {}
        double                   value;
        HWC_ENUM::epics_timestamp time;
    };
//     all the image data from a single frame goes in a image_data struct
//    struct image_data
//    {
//        std::map<PV_TYPE, data> dataStructs;
//    };
    // we have a blank const map ready to update
    // image_data_buffer with as new entries come in
    struct static_blank_image_data
    {
        static std::map<VC_PV_TYPE, data> create_blank_image_data()
        {
            std::map<VC_PV_TYPE, data> m;
            m[VC_PV_TYPE::X_RBV] = data();
            m[VC_PV_TYPE::Y_RBV] = data();
            m[VC_PV_TYPE::SIGMA_X_RBV] = data();
            m[VC_PV_TYPE::SIGMA_Y_RBV] = data();
            m[VC_PV_TYPE::COV_XY_RBV] = data();
            m[VC_PV_TYPE::X_PIX] = data();
            m[VC_PV_TYPE::Y_PIX] = data();
            m[VC_PV_TYPE::SIGMA_X_PIX] = data();
            m[VC_PV_TYPE::SIGMA_Y_PIX] = data();
            m[VC_PV_TYPE::COV_XY_PIX] = data();
            m[VC_PV_TYPE::VC_INTENSITY] = data();
            return m;
        }
        static const std::map<VC_PV_TYPE, data>  blank_image_data;
    };
    // The main hardware object holds ...
    struct pilMirrorObject
    {
        pilMirrorObject():
            hPos(UTL::DUMMY_DOUBLE),
            vPos(UTL::DUMMY_DOUBLE),
            hStep(UTL::DUMMY_DOUBLE),
            vStep(UTL::DUMMY_DOUBLE),
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT)
            {};
        std::string name, pvRoot;
        double hPos, vPos, hStep, vStep;
    };

    // The main hardware object holds ...
    struct virtualCathodeObject
    {
        virtualCathodeObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            buffer_size(UTL::BUFFER_60000),
            x_pos(UTL::DUMMY_SIZET),
            y_pos(UTL::DUMMY_SIZET),
            x_sigma_pos(UTL::DUMMY_SIZET),
            y_sigma_pos(UTL::DUMMY_SIZET),
            cov_pos(UTL::DUMMY_SIZET),
            x_name(UTL::UNKNOWN_NAME),
            y_name(UTL::UNKNOWN_NAME),
            x_sigma_name(UTL::UNKNOWN_NAME),
            y_sigma_name(UTL::UNKNOWN_NAME),
            cov_name(UTL::UNKNOWN_NAME),
            buffer_full(false)
            {};
        double hPos, vPos, hStep, vStep;
        std::string name, pvRoot, x_name, y_name, x_sigma_name, y_sigma_name, cov_name;
        size_t buffer_size, x_pos, y_pos, x_sigma_pos, y_sigma_pos, cov_pos,results_count;
        bool buffer_full;
        /*
            time stamped pixel array readback
        */
        std::vector<double> pix_values;
        std::deque<std::vector<double>> pix_values_buffer;
        std::string pix_values_time;
        /*
            this map is defined in the config file
            and tells us which element is which
            analysis data for the pixel array RBV
        */
        std::map<size_t, std::string> pixel_values_pos;
        std::map<std::string, double> pixel_values;
#ifdef BUILD_DLL
        boost::python::dict pixel_values_dict;
#endif
        pilMirrorObject mirror;
        std::map<VC_PV_TYPE, pvStruct> pvMonStructs;
        std::map<VC_PV_TYPE, pvStruct> pvComStructs;
    };
}
//______________________________________________________________________________
#endif//_VELA_CLARA_PIL_VIRTUAL_CATHODE_STRUCTS_H_
