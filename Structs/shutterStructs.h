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
//  FileName:    VCpilaser.cpp
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_SHUTTER_STRUCTS_H_
#define _VELA_CLARA_SHUTTER_STRUCTS_H_
// project includes
#include "structs.h"
// stl includes
#include <string>
#include <map>
// epics includes
#ifndef __CINT__
#include <cadef.h>
#endif

class shutterInterface;
//______________________________________________________________________________
namespace shutterStructs
{
    struct monitorStuct;
    struct shutterObject;
    struct pvStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SHUTTER_PV_TYPE,(Sta)
                                                        (On)
                                                        (Off)
                                                        (UNKNOWN)
                                        )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SHUTTER_STATE,(CLOSED)
                                                      (OPEN)
                                                      (TIMING)
                                                      (ERROR)
                                        )
    /*
        monType could be used to switch in the staticCallbackFunction
        For the shutter this is basically redundant, there is only
        one monitor: "Sta" (apart from interlocks, these are handled
        in the base class) monType could be used to switch in the
        statisCallbackFunction
    */
    struct monitorStuct
    {
        monitorStuct():
            shutObj(nullptr),
            interface(nullptr),
            monType(UNKNOWN)
            {};
        shutterObject*    shutObj;
        shutterInterface* interface;
        evid              ventID;
        SHUTTER_PV_TYPE   monType;

    };

    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_STRING),
            COUNT(UTL::ZERO_UL),
            MASK(UTL::ZERO_UL),
            pvType(UNKNOWN)
            {};
        SHUTTER_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };

    struct shutterObject
    {
        shutterObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            numIlocks(UTL::ONE_INT),
            shutterState(ERROR)
            {};
        std::string name, pvRoot;
        SHUTTER_STATE shutterState;
        int numIlocks;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > iLockStates;
        std::map<SHUTTER_PV_TYPE, pvStruct > pvMonStructs;
        std::map<SHUTTER_PV_TYPE, pvStruct > pvComStructs;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;
    };
}
//______________________________________________________________________________
#endif//_VELA_CLARA_SHUTTER_STRUCTS_H_
