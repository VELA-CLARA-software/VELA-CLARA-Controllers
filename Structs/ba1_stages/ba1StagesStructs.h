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
//  FileName:    VCba1Stages.cpp
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_BA1_STAGES_STRUCTS_H_
#define _VELA_CLARA_BA1_STAGES_STRUCTS_H_
// project includes
#include "structs.h"
// stl includes
#include <string>
#include <map>
// epics includes
#ifndef __CINT__
#include <cadef.h>
#endif

class ba1StagesInterface;
//______________________________________________________________________________
namespace ba1StagesStructs
{
    struct monitorStuct;
    struct ba1StagesObject;
    struct pvStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(BA1STAGE_PV_TYPE,(RPOS)
                                                        (MPOS)
                                                        (UNKNOWN)
                                        )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(BA1STAGE_STATE,(MOVING)
                                                      (NOT_MOVING)
                                                      (ERROR)
                                        )
    /*
        monType could be used to switch in the staticCallbackFunction
        For the ba1Stages this is basically redundant, there is only
        one monitor: "Sta" (apart from interlocks, these are handled
        in the base class) monType could be used to switch in the
        statisCallbackFunction
    */
    struct monitorStruct
    {
        monitorStruct():
            stageObj(nullptr),
            interface(nullptr),
            monType(UNKNOWN)
            {};
        ba1StagesObject*    stageObj;
        ba1StagesInterface* interface;
        BA1STAGE_PV_TYPE   monType;
        // default values for epics types?? probbaly com
        chtype              CHTYPE;
        evid                EVID;
    };

    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_STRING),
            COUNT(UTL::ZERO_UL),
            MASK(UTL::ZERO_UL),
            pvType(UNKNOWN)
            {};
        BA1STAGE_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
        evid            EVID;
    };

    struct ba1StagesObject
    {
        ba1StagesObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            r_position(UTL::DUMMY_DOUBLE),
            m_position(UTL::DUMMY_DOUBLE),
            yag_in_pos(UTL::DUMMY_DOUBLE),
            yag_out_pos(UTL::DUMMY_DOUBLE),
            has_yag(false),
            min_pos(UTL::ZERO_DOUBLE),
            max_pos(UTL::ZERO_DOUBLE),
            state(ERROR),
            stage_number(UTL::ZERO_SIZET),
            precision(UTL::ZERO_SIZET)
            {};
        std::string name, pvRoot;
        BA1STAGE_STATE state;
        bool has_yag;
        // defined in config file
        std::vector<std::string> devices;
        std::vector<double> positions;
        // from which we create this ...
        std::map<std::string, double> device_position_map;
        double r_position, m_position,yag_in_pos,yag_out_pos, min_pos, max_pos;
        std::map<BA1STAGE_PV_TYPE, pvStruct > pvMonStructs;
        std::map<BA1STAGE_PV_TYPE, pvStruct > pvComStructs;
        size_t stage_number,precision;
    };
}
//______________________________________________________________________________
#endif//_VELA_CLARA_SHUTTER_STRUCTS_H_
