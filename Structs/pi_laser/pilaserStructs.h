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
//  Last edit:   29-06-2018
//  FileName:    pilaserStructs.cpp
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_PIL_STRUCTS_H_
#define _VELA_CLARA_PIL_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
#include "cameraStructs.h"
#include "running_stat.h"
//stl
#include <string>
#include <map>
#include <vector>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

#ifdef BUILD_DLL
#include <boost/python.hpp>
#endif


// forward declare classes/structs to avoid circular dependency
class pilaserInterface;
//https://stackoverflow.com/questions/2059665/why-cant-i-forward-declare-a-class-in-a-namespace-using-double-colons

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
            STEP_MAX(UTL::ZERO_DOUBLE)
            {};
        std::string name, pvRoot;
        double hPos, vPos, hStep, vStep,STEP_MAX;
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
            pvRoot(UTL::UNKNOWN_PVROOT),
            max_buffer_count(UTL::TEN_SIZET),
            buffer_count(UTL::ZERO_SIZET),
            buffer_full(false)
            {};
        /*
            buffers for pixel values
        */
        size_t max_buffer_count,buffer_count;
        bool buffer_full;
        std::string name, pvRoot,pvRootQ;
        cameraStructs::cameraObject vcCam;
        double intensity,setCharge,HWP,Q;
        std::vector<double> Q_buf;
        runningStat Q_rs,intensity_rs;
        pilMirrorObject             mirror;
        HWC_ENUM::STATE status, stabilisation_status;
        std::map<PILASER_PV_TYPE, pvStruct> pvMonStructs;
        std::map<PILASER_PV_TYPE, pvStruct> pvComStructs;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct> iLockPVStructs;


        void Q_clear();
        void intensity_clear();
        double Q_mean();
        double intensity_mean();
        double Q_sd();
        double intensity_sd();
        size_t Q_n();
        size_t intensity_n();
    };
}
#endif
