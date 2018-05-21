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
//  FileName:    fastCamStructs.h
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
#include <deque>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

#include "structs.h"
#include "configDefinitions.h"

class fastCamInterface;

namespace fastCamStructs
{
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(FAST_CAM_PV,(UNKNOWN_PV)
                                                    (START)
                                                    (STOP)
                                                    (GAIN)
                                                    (BLACK_LEVEL)
                                                    (DATA)
                                                    (GAIN_RBV)
                                                    (BLACK_LEVEL_RBV)
                                                    (STATE))

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

    struct fastCamObject
    {
        fastCamObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            screenName(UTL::UNKNOWN_STRING),
            streamingIPAddress(UTL::UNKNOWN_STRING),
            state(HWC_ENUM::UNKNOWN),
            gain(0),
            blacklevel(0),
            buffer_size(1),
            buffer_count(0),
            buffer_full(false),
            num_pix_x(UTL::ZERO_SIZET),
            num_pix_y(UTL::ZERO_SIZET),
            x_pix_to_mm(UTL::ZERO_DOUBLE),
            y_pix_to_mm(UTL::ZERO_DOUBLE),
            type(CAM_TYPE::NOT_KNOWN)
            {}
        std::string name, pvRoot, screenName, streamingIPAddress;
        HWC_ENUM::STATE state;
        size_t buffer_size,buffer_count,num_pix_x,num_pix_y;
        double x_pix_to_mm,y_pix_to_mm;
        bool buffer_full;
        long gain,blacklevel;
        std::map<FAST_CAM_PV, pvStruct> pvMonStructs;
        std::map<FAST_CAM_PV, pvStruct> pvComStructs;
        CAM_TYPE type;
        HWC_ENUM::MACHINE_AREA  machineArea;
        std::vector<double> background;
        std::vector<double> data;
        std::deque<std::vector<double>> data_buffer;
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

//    struct fastCamObject
//    {
//        fastCamObject():
//            name(UTL::UNKNOWN_NAME),
//            type(CAM_TYPE::NOT_KNOWN)
//            {}
//        std::string name, pvRoot;
//        CAM_TYPE type;
//        long gain,blacklevel;
//        STATE blacklevel;
//        std::map<FAST_CAM_PV, pvStruct> pvMonStructs;
//        std::map<FAST_CAM_PV, pvStruct> pvComStructs;
//    };

}
#endif // FAST_CAM_STRUCTS_H_
