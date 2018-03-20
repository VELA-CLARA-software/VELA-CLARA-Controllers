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
//  Description:
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

namespace rfProtStructs
{
//    struct pvStruct;
//    struct rfPowerObject;
//    struct rfPowerMonitorStruct;
//    struct monitorStuct;
//    struct rfLLRFObject;
//    struct rfModObject;
//    struct rfObject;


    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_PV_TYPE, (RESET)(STATUS)(ON)(OFF)(CMI))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_TYPE, (CLARA_HRRG)(VELA_LRRG)(VELA_HRRG)(CLARA_LRRG)(TEST)(NOT_KNOWN)(GENERAL)(ENABLE)(NO_MODE))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_STATUS, (GOOD)(BAD)(ERROR)(UNKNOWN))

    /// These can't go in HWC_ENUM as they need a pvType.
    struct pvStruct
    {
        pvStruct() : pvSuffix("UNKNOWN" ), objName("UNKNOWN"),COUNT(0),MASK(0){}
        RF_GUN_PROT_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
    };

    struct rfGunProtObject
    {
        rfGunProtObject() : status(UNKNOWN),protType(NOT_KNOWN),name("unknown"),numIlocks(0){}
        std::string         name,pvRoot;
        RF_GUN_PROT_STATUS  status;
        unsigned long       cmi;
        std::vector<int>    gunProtKeyBits;// the bit position of the key bits in the prot cmi
        std::vector<bool>   gunProtKeyBitValues;// calues of the key bits
        size_t              numIlocks;
        RF_GUN_PROT_TYPE        protType;
        std::map< HWC_ENUM::ILOCK_NUMBER , HWC_ENUM::ILOCK_STATE > iLockStates;
        std::map< RF_GUN_PROT_PV_TYPE, pvStruct > pvMonStructs;
        std::map< RF_GUN_PROT_PV_TYPE, pvStruct > pvComStructs;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;
    };

    struct monitorStruct
    {
        monitorStruct() : rfProtObject(nullptr),interface(nullptr){}
        RF_GUN_PROT_PV_TYPE   monType;
        rfGunProtObject*  rfProtObject;
        chtype            CHTYPE;
        //void *           val; /// most of the rfObject Values appear to be doubles, which is handy... recast in staticEntry function...
        gunProtInterface *interface;
        evid              EVID;
    };
}
#endif//_RF_PROT_STRUCTS_H_
