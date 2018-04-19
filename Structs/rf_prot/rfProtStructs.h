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
//  FileName:    rfProtStruts.h
//  Description: enums and structs for rf protection
//
//
//*/
#ifndef _RF_PROT_STRUCTS_H_
#define _RF_PROT_STRUCTS_H_
//
#include "structs.h"
//stl
#include <vector>
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class gunProtInterface;
//______________________________________________________________________________
namespace rfProtStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_PV_TYPE, (RESET)
                                                             (STATUS)
                                                             (ON)
                                                             (OFF)
                                                             (CMI)
                                                             (UNKNOWN_PV)
                                        )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_PROT_TYPE, (CLARA_HRRG)
                                                      (VELA_LRRG)
                                                      (VELA_HRRG)
                                                      (CLARA_LRRG)
                                                      (TEST)
                                                      (NOT_KNOWN)
                                                      (GENERAL)
                                                      (ENABLE)
                                                      (NO_MODE)
                                        )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_STATUS, (GOOD)
                                                            (BAD)
                                                            (ERROR)
                                                            (UNKNOWN)
                                        )

    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_STRING),
            objName(UTL::UNKNOWN_NAME),
            COUNT(UTL::ZERO_UL),
            MASK(UTL::ZERO_UL)
            {}
        std::string     pvSuffix, objName;
        RF_GUN_PROT_PV_TYPE pvType;
        unsigned long   COUNT, MASK;
        // defaults for chtype and chid ???
        chtype          CHTYPE;
        chid            CHID;
    };

    struct rfGunProtObject
    {
        rfGunProtObject():
            status(UNKNOWN),
            protType(NOT_KNOWN),
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            numIlocks(UTL::ZERO_SIZET),
            cmi(UTL::ZERO_UL)
            {}
        std::string         name,pvRoot;
        RF_GUN_PROT_STATUS  status;
        long       cmi;
        //unsigned long       cmi;
        // the bit position of the key bits in the prot cmi
        std::vector<int>    gunProtKeyBits;
        // values of the key bits
        std::vector<bool>   gunProtKeyBitValues;
        size_t              numIlocks;
        RF_PROT_TYPE        protType;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> iLockStates;
        std::map<RF_GUN_PROT_PV_TYPE, pvStruct> pvMonStructs;
        std::map<RF_GUN_PROT_PV_TYPE, pvStruct> pvComStructs;
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct> iLockPVStructs;
    };

    struct monitorStruct
    {
        monitorStruct():
            rfProtObject(nullptr),
            interface(nullptr),
            monType(UNKNOWN_PV)
            {}
        RF_GUN_PROT_PV_TYPE monType;
        rfGunProtObject*    rfProtObject;
        gunProtInterface*   interface;
        // default values for epics types??
        chtype              CHTYPE;
        evid                EVID;
    };
}
//______________________________________________________________________________
#endif//_RF_PROT_STRUCTS_H_
