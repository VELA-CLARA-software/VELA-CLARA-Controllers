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
//  Last edit:   31-08-2018
//  FileName:    pilaserStructs.cpp
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_PIL_STRUCTS_H_
#define _VELA_CLARA_PIL_STRUCTS_H_
// project
#include "structs.h"
#include "configDefinitions.h"
#include "cameraStructs.h"
#include "running_stat.h"
//stl
#include <string>
#include <map>
#include <vector>
#include <chrono>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif
// forward declare classes/structs to avoid circular dependency
//https://stackoverflow.com/questions/2059665/why-cant-i-forward-declare-a-class-in-a-namespace-using-double-colons
class pilaserInterface;
//
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
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(PILASER_PV_TYPE,(ENERGY)
                                                        (STABILISATION)
                                                        (STATUS)
                                                        (HALF_WAVE_PLATE_SET)
                                                        (HALF_WAVE_PLATE_READ)
                                                        (VC_INTENSITY)
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
                                                        (WCM_Q)
                                                        (UNKNOWN_PILASER_PV_TYPE)
                                                        (ENERGY_RANGE_RB)
                                                        (ENERGY_RANGE_SP)
                                                        (ENERGY_RUN_RB)
                                                        (ENERGY_RUN_SP)
                                                        (ENERGY_OVERRANGE_RB)
                                                        )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(VC_SET_POS_STATE,(SHUTTER_CLOSED)
                                                         (CAMERA_ANALYSIS_NOT_WORKING)
                                                         (LASER_NOT_IN_IMAGE)
                                                         (H_HI)
                                                         (V_HI)
                                                         (H_LO)
                                                         (V_LO)
                                                         (TIME_OUT)
                                                         (MAX_ITERATIONS)
                                                         (UNKNOWN_STATE)
                                                         (MOVING)
                                                         (STARTUP)
                                                         (SUCCESS)
                                                         (FAIL)
                                                         (RUNNING)
                                                         (NO_MASK_FEEDBACK)
                                                         )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(EM_RANGE,(TWO_HUNDRED_MICRO_J)
                                                 (TWENTY_MICRO_J)
                                                 (TWO_MICRO_J)
                                                 (TWO_HUNDRED_NANO_J)
                                                 (UNKNOWN_RANGE)
                                                         )

    struct monitorStruct
    {
        monitorStruct():
            monType(PILASER_PV_TYPE::UNKNOWN_PILASER_PV_TYPE),
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
            pvType(PILASER_PV_TYPE::UNKNOWN_PILASER_PV_TYPE)
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
            hStep(UTL::TEN_DOUBLE),
            vStep(UTL::TEN_DOUBLE),
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            STEP_MAX(UTL::ZERO_DOUBLE),
            left_sense(UTL::ZERO_DOUBLE),
            right_sense(UTL::ZERO_DOUBLE),
            up_sense(UTL::ZERO_DOUBLE),
            down_sense(UTL::ZERO_DOUBLE)
            {};
        std::string name, pvRoot;
        double hPos, vPos, hStep, vStep,STEP_MAX;
        double left_sense, right_sense, up_sense, down_sense;
    };

    // The main hardware object holds ...
    struct pilaserObject
    {
        pilaserObject():
            status(HWC_ENUM::STATE::UNKNOWN),
            stabilisation_status(HWC_ENUM::STATE::UNKNOWN),
            energy(UTL::DUMMY_DOUBLE),
            setCharge(UTL::DUMMY_DOUBLE),
            HWP(UTL::DUMMY_DOUBLE),
            Q(UTL::DUMMY_DOUBLE),
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            max_buffer_count(UTL::TEN_SIZET),
            buffer_count(UTL::ZERO_SIZET),
            buffer_full(false),
            pvRootQ(UTL::UNKNOWN_STRING),
            pvRootE(UTL::UNKNOWN_STRING),
            setVCPosState(UNKNOWN_STATE),
            em_over_range(false),
            em_is_running(false),
            em_range(UNKNOWN_RANGE)
            {};
        /*
            buffers for pixel values
        */
        size_t max_buffer_count,buffer_count;
        bool buffer_full;
        std::string name, pvRoot, pvRootQ, pvRootE;
        //cameraStructs::cameraObject vcCam;
        double energy, setCharge, HWP, Q;
        std::vector<double> Q_buf;
        std::vector<double> E_buf;
        runningStat Q_rs, energy_rs;
        pilMirrorObject mirror;
        HWC_ENUM::STATE status, stabilisation_status;
        std::map<PILASER_PV_TYPE, pvStruct> pvMonStructs;
        std::map<PILASER_PV_TYPE, pvStruct> pvComStructs;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct> iLockPVStructs;
        VC_SET_POS_STATE setVCPosState;
        bool em_over_range, em_is_running;
        EM_RANGE em_range;
        void   Q_clear();
        void   energy_clear();
        void   Q_rs_buffer_size(size_t new_size);
        void   energy_rs_buffer_size(size_t new_size);
        bool   Q_full();
        bool   energy_full();
        double Q_mean();
        double energy_mean();
        double Q_sd();
        double energy_sd();
        size_t Q_n();
        size_t energy_n();

    };

    struct set_vc_position
    {
        set_vc_position():
            x_pos(UTL::DUMMY_DOUBLE),
            y_pos(UTL::DUMMY_DOUBLE),
            x_precision(UTL::DUMMY_DOUBLE),
            y_precision(UTL::DUMMY_DOUBLE),
            mirror_step_x(UTL::ZERO_DOUBLE),
            mirror_step_y(UTL::ZERO_DOUBLE),
            num_points_x(UTL::ZERO_SIZET),
            num_points_y(UTL::ZERO_SIZET),
            max_iterations(UTL::ZERO_SIZET),
            state(UNKNOWN_STATE),
            time_start(UTL::TIMET_0),
            time_out(UTL::TIMET_0),
            thread(nullptr)
            {};
        double x_pos, y_pos,x_precision,y_precision, mirror_step_x, mirror_step_y;
        size_t  max_iterations, num_points_x, num_points_y;
        time_t time_out;
        time_t time_start;
        pilaserInterface* interface;
        VC_SET_POS_STATE state;
        std::thread*   thread;
    };


}
#endif
