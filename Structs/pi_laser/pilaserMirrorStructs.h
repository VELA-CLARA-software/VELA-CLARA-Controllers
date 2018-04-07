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
//  Last edit:   05-04-2018
//  FileName:    pilaserMirrorStructs.h
//  Description:
//
//
//
//
//*/
#ifndef _VELA_CLARA_PIL_MIRROR_STRUCTS_H_
#define _VELA_CLARA_PIL_MIRROR_STRUCTS_H_
// project includes
#include "structs.h"
#include "configDefinitions.h"
//stlincludes
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif
// Forward declare class
class pilaserMirrorInterface;
//______________________________________________________________________________
namespace pilaserMirrorStructs
{
    struct monitorStuct;
    struct pilMirrorObject;
    struct pvStruct;
    /*
        Use this MACRO to define enums.
        NUMS that are more 'global' in structs.h
    */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(PIL_MIRROR_PV_TYPE,(H_POS)
                                                           (V_POS)
                                                           (H_STEP)
                                                           (V_STEP)
                                                           (H_MOVE)
                                                           (V_MOVE)
                                                           (UNKNOWN)
                                        )

    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN),
            pilMirrorObj(nullptr),
            interface(nullptr),
            EVID(nullptr)
            {};
        PIL_MIRROR_PV_TYPE monType;
        pilaserMirrorInterface*  interface;
        chtype             CHTYPE;
        evid               EVID;
        pilMirrorObject*   pilMirrorObj;
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
            {};
        PIL_MIRROR_PV_TYPE pvType;
        chid               CHID;
        std::string        pvSuffix;
        unsigned long      COUNT, MASK;
        chtype             CHTYPE;
        evid               EVID;
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
        std::map<PIL_MIRROR_PV_TYPE, pvStruct> pvMonStructs;
        std::map<PIL_MIRROR_PV_TYPE, pvStruct> pvComStructs;
    };
}
//______________________________________________________________________________
#endif//_VELA_CLARA_PIL_MIRROR_STRUCTS_H_
