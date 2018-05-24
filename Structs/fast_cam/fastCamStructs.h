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
                                                    (STATE)
                                                    (X)
                                                    (Y)
                                                    (SIGMA_X)
                                                    (SIGMA_Y)
                                                    (COV_XY)
                                                    (X_PIX)
                                                    (Y_PIX)
                                                    (SIGMA_X_PIX)
                                                    (SIGMA_Y_PIX)
                                                    (COV_XY_PIX)
                                                    (AVG_PIX_INETSITY)
                                                    (START_IA)
                                                    (X_CENTER)
                                                    (Y_CENTER)
                                                    (X_CENTER_RBV)
                                                    (Y_CENTER_RBV)
                                                    (MASK_X_RBV)
                                                    (MASK_Y_RBV)
                                                    (MASK_X_RAD_RBV)
                                                    (MASK_Y_RAD_RBV)
                                                    (MASK_X)
                                                    (MASK_Y)
                                                    (MASK_X_RAD)
                                                    (MASK_Y_RAD)
                                                    (PIX_MM)
                                                    (STEP_SIZE)
                                                    (SET_BKGRND)
                                                    (USE_BKGRND)
                                                    (USE_NPOINT)
                                                    (CAM_START_ACQUIRE)
                                                    (CAM_STOP_ACQUIRE)
                                                    (CAM_ACQUIRE_RBV)
                                                    (CAM_STATUS)
                                                    (START_IA_RBV)
                                                    (USE_BKGRND_RBV)
                                                    (USE_NPOINT_RBV)
                                                    (CAM_DATA))

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

    struct mask_struct
    {
        mask_struct():
            x(UTL::DUMMY_INT),
            y(UTL::DUMMY_INT),
            x_rad(UTL::DUMMY_INT),
            y_rad(UTL::DUMMY_INT)
            {}
        int x,y,x_rad,y_rad;
    };

    struct fastCamObject
    {
        fastCamObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            screen(UTL::UNKNOWN_STRING),
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
        std::string name, pvRoot, screen, streamingIPAddress;
        HWC_ENUM::STATE state;
        size_t buffer_size,buffer_count,num_pix_x,num_pix_y;
        double x_pix_to_mm,y_pix_to_mm;
        bool buffer_full;
        long gain,blacklevel;
        std::map<FAST_CAM_PV, pvStruct> pvMonStructs;
        std::map<FAST_CAM_PV, pvStruct> pvComStructs;
        CAM_TYPE type;
        HWC_ENUM::MACHINE_AREA  machineArea;
        mask_struct mask;
        fastCamInterface* interface;
//        bool setMASK(const double x,
//                     const double y,
//                     const double x_rad,
//                     const double y_rad,
//                     );
//        bool setMASKX(const double v);
//        bool setMASKY(const double v);
//        bool setMASKX_RAD(const double v);
//        bool setMASKY_RAD(const double v);

        std::vector<double> background;
        std::vector<double> data;
        std::string data_time;
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
