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
//  FileName:    shutterConfigReader.cpp
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_PIL_STRUCTS_H_
#define _VELA_CLARA_PIL_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
#include "running_stat.h"
//stl
#include <string>
#include <map>
#include <deque>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

#ifdef BUILD_DLL
#include <boost/python.hpp>
//#include <boost/python/class.hpp>
//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python/scope.hpp>
#endif

class pilaserInterface;

namespace pilaserStructs
{
    /* Forward declare structs, gcc seems to like this...*/
    struct monitorStuct;
    struct pilaserObject;
    struct pvStruct;
    /*
        Use this MACRO to define enums.
        Consider putting ENUMS that are more 'global' in structs.h
    */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(PILASER_PV_TYPE,(INTENSITY)
                                                        (STABILISATION)
                                                        (STATUS)
                                                        (HALF_WAVE_PLATE_SET)
                                                        (HALF_WAVE_PLATE_READ)
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
                                                        (WCM_Q)
                                                        (ARRAY_DATA)
                                        )
    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN),
            interface(nullptr),
            EVID(nullptr),
            object(nullptr)
            {}
        PILASER_PV_TYPE   monType;
        pilaserInterface*  interface;
        chtype                    CHTYPE;
        evid                      EVID;
        pilaserObject* object;
    };
    /*
        The hardware object holds a map keyed by PV type,
        with pvStruct values, some values come from the config
        The rest are paramaters passed to EPICS, ca_create_channel,
        ca_create_subscription etc..
    */
    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_STRING),
            COUNT(UTL::ZERO_UL),
            MASK(UTL::ZERO_UL),
            pvType(UNKNOWN)
            {}
        PILASER_PV_TYPE    pvType;
        chid          CHID;
        std::string   pvSuffix;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
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
            pvRoot(UTL::UNKNOWN_PVROOT),
            STEP_MAX(UTL::ZERO_DOUBLE)
            {};
        std::string name, pvRoot;
        double hPos, vPos, hStep, vStep,STEP_MAX;
    };

    // The main hardware object holds ...
    struct virtualCathodeDataObject
    {
        virtualCathodeDataObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            buffer_size(UTL::BUFFER_HUNDRED),
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
            buffer_full(false),
            x(UTL::DUMMY_DOUBLE),
            y(UTL::DUMMY_DOUBLE),
            sig_x(UTL::DUMMY_DOUBLE),
            sig_y(UTL::DUMMY_DOUBLE),
            sig_xy(UTL::DUMMY_DOUBLE),
            x_pix(UTL::DUMMY_DOUBLE),
            y_pix(UTL::DUMMY_DOUBLE),
            sig_x_pix(UTL::DUMMY_DOUBLE),
            sig_y_pix(UTL::DUMMY_DOUBLE),
            sig_xy_pix(UTL::DUMMY_DOUBLE)
            {};
        double hPos, vPos, hStep, vStep;
        std::string name, pvRoot, x_name, y_name, x_sigma_name, y_sigma_name, cov_name;
        size_t buffer_size, buffer_count, x_pos, y_pos, x_sigma_pos, y_sigma_pos, cov_pos,results_count;
        double x,y,sig_x,sig_y,sig_xy,x_pix,y_pix,sig_x_pix,sig_y_pix,sig_xy_pix;
        std::deque<double> x_buf,y_buf,sig_x_buf,sig_y_buf,sig_xy_buf,x_pix_buf,y_pix_buf,sig_x_pix_buf,sig_y_pix_buf,sig_xy_pix_buf;
        bool buffer_full;
        /*
            time stamped pixel array readback
        */
        std::vector<double> pix_values;
        std::deque<std::vector<double>> pix_values_buffer;
        std::string pix_values_time;

        std::vector<int> array_data;
        //std::deque<std::vector<double>> pix_values_buffer;
        runningStat x_rs,y_rs,sig_x_rs,sig_y_rs,sig_xy_rs,x_pix_rs,y_pix_rs,sig_x_pix_rs,sig_y_pix_rs,sig_xy_pix_rs;

        double x_mean();
        double y_mean();
        double sig_x_mean();
        double sig_y_mean();
        double sig_xy_mean();

        double x_pix_mean();
        double y_pix_mean();
        double sig_x_pix_mean();
        double sig_y_pix_mean();
        double sig_xy_pix_mean();

        double x_sd();
        double y_sd();
        double sig_x_sd();
        double sig_y_sd();
        double sig_xy_sd();

        double x_pix_sd();
        double y_pix_sd();
        double sig_x_pix_sd();
        double sig_y_pix_sd();
        double sig_xy_pix_sd();



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
        std::map<PILASER_PV_TYPE, pvStruct> pvMonStructs;
        std::map<PILASER_PV_TYPE, pvStruct> pvComStructs;
    };

    // The main hardware object holds ...
    struct pilaserObject
    {
        pilaserObject():
            status(HWC_ENUM::STATE::UNKNOWN),
            stabilisation_status(HWC_ENUM::STATE::UNKNOWN),
            intensity(UTL::DUMMY_DOUBLE),
            setCharge(UTL::DUMMY_DOUBLE),
            HWP(UTL::DUMMY_DOUBLE),
            Q(UTL::DUMMY_DOUBLE),
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT)
            {};
        std::string name, pvRoot,pvRootQ;
        double intensity,setCharge,HWP,Q;
        std::deque<double> Q_buf;
        virtualCathodeDataObject vcData;
        HWC_ENUM::STATE status, stabilisation_status;
        std::map<PILASER_PV_TYPE, pvStruct> pvMonStructs;
        std::map<PILASER_PV_TYPE, pvStruct> pvComStructs;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct> iLockPVStructs;
    };
}
#endif
