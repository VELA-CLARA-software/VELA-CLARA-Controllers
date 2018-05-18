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
//  Author:      TP, then DJS
//  Last edit:   16-05-2018
//  FileName:    cameraStructs.h
//  Description:
//
//
//
//
//*/
#ifndef FAST_CAM_STRUCTS_H_
#define FAST_CAM_STRUCTS_H_
//stl
#include <vector>
#include <string>
#include <thread>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

#include "structs.h"
#include "configDefinitions.h"

class fastCamInterface;

namespace fastCamStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(FAST_CAM_PV,(UNKNOWN_PV))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_STATE,(CAM_ERROR))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CAM_TYPE,(VELA)(CLARA)(NOT_KNOWN))

    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_PVSUFFIX),
            COUNT(UTL::ZERO_INT),
            MASK(UTL::ZERO_INT),
            pvType(UNKNOWN_PV)
            {}
        FAST_CAM_PV     pvType;
        chid            CHID;
        std::string     pvSuffix;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };

    struct fastCameraObject
    {
        fastCameraObject():name(UTL::UNKNOWN_NAME),
                           pvRoot(UTL::UNKNOWN_PVROOT),
                           screenName(UTL::UNKNOWN_STRING),
                           streamingIPAddress(UTL::UNKNOWN_STRING),
                           state(CAM_ERROR)
                           {}
        std::string name, pvRoot, screenName, streamingIPAddress;
        CAM_STATE state;
        //ACQUIRE_STATE acquireState;
        std::map<FAST_CAM_PV, pvStruct> pvMonStructs;
        std::map<FAST_CAM_PV, pvStruct> pvComStructs;
        HWC_ENUM::MACHINE_AREA  machineArea;
        std::vector<double> background;
        std::vector<double> data;
    };

    struct monitorStruct
    {   // proviude a default constructor
        monitorStruct():monType(UNKNOWN_PV),
                        objName("UNKNOWN"),
                        interface(nullptr)
                        {}
        fastCamInterface* interface;
        FAST_CAM_PV       monType;
        std::string       objName;
        chtype            CHTYPE;
        evid              EVID;
    };

    struct fastCamObject
    {
        fastCamObject():
            name(UTL::UNKNOWN_NAME),
            type(CAM_TYPE::NOT_KNOWN)
            {}
        std::string name, pvRoot;
        CAM_TYPE type;
        std::map<FAST_CAM_PV, pvStruct> pvMonStructs;
        std::map<FAST_CAM_PV, pvStruct> pvComStructs;
    };

}
#endif // FAST_CAM_STRUCTS_H_
