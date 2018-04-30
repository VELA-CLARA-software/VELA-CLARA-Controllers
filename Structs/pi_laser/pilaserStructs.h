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
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
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
                                                        (HALF_WAVE_PLATE)
                                                        (UNKNOWN_PV)
                                        )
    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN_PV),
            interface(nullptr),
            EVID(nullptr),
            pilaserObj(nullptr)
            {}
        PILASER_PV_TYPE   monType;
        pilaserInterface* interface;
        chtype            CHTYPE;
        evid              EVID;
        pilaserObject*    pilaserObj;
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
            pvType(UNKNOWN_PV)
            {};
        PILASER_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };
    // The main hardware object holds ...
    struct pilaserObject
    {
        pilaserObject():
            status(HWC_ENUM::STATE::UNKNOWN),
            stabilisation_status(HWC_ENUM::STATE::UNKNOWN),
            intensity(UTL::DUMMY_DOUBLE),
            setCharge(UTL::DUMMY_DOUBLE),
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT)
            {};
        std::string name, pvRoot;
        double intensity,setCharge;
        HWC_ENUM::STATE status, stabilisation_status;
        std::map<PILASER_PV_TYPE, pvStruct> pvMonStructs;
        std::map<PILASER_PV_TYPE, pvStruct> pvComStructs;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct> iLockPVStructs;
    };
}
#endif
