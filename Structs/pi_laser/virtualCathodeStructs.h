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
//  FileName:    virtualCathodeStructs.h
//  Description:
//
//
//*/
#ifndef _VELA_CLARA_PIL_VIRTUAL_CATHODE_STRUCTS_H_
#define _VELA_CLARA_PIL_VIRTUAL_CATHODE_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
#include <deque>
//epics
#include <cadef.h>
//______________________________________________________________________________
class virtualCathodeInterface;
//______________________________________________________________________________
namespace virtualCathodeStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;
    struct virtualCathodeDataObject;
    struct pvStruct;
    // Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(PV_TYPE,(X_RBV)
                                                 (Y_RBV)
                                                 (SIGMA_X_RBV)
                                                 (SIGMA_Y_RBV)
                                                 (COV_XY_RBV)
                                                 (X_PIX)
                                                 (Y_PIX)
                                                 (SIGMA_X_PIX)
                                                 (SIGMA_Y_PIX)
                                                 (COV_XY_PIX)
                                                 (VC_INTENSITY)
                                                 (UNKNOWN_PV)
                                        )
    struct monitorStruct
    {
        monitorStruct():
            monType(UNKNOWN_PV),
            interface(nullptr),
            EVID(nullptr),
            object(nullptr)
            {}
        PV_TYPE   monType;
        virtualCathodeInterface*  interface;
        chtype                    CHTYPE;
        evid                      EVID;
        virtualCathodeDataObject* object;
    };

    struct pvStruct
    {
        pvStruct():
            pvSuffix(UTL::UNKNOWN_STRING),
            COUNT(UTL::ZERO_UL),
            MASK(UTL::ZERO_UL)
            {}
        PV_TYPE       pvType;
        chid          CHID;
        std::string   pvSuffix;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };
    // the data from each PV will go, timestamped, in a data struct
    struct data
    {
        data():
            value(UTL::DUMMY_DOUBLE)
            {}
        double                   value;
        HWC_ENUM::epics_timestamp time;
    };
//     all the image data from a single frame goes in a image_data struct
//    struct image_data
//    {
//        std::map<PV_TYPE, data> dataStructs;
//    };
    // we have a blank const map ready to update
    // image_data_buffer with as new entries come in
    struct static_blank_image_data
    {
        static std::map<PV_TYPE, data> create_blank_image_data()
        {
            std::map<PV_TYPE, data> m;
            m[PV_TYPE::X_RBV] = data();
            m[PV_TYPE::Y_RBV] = data();
            m[PV_TYPE::SIGMA_X_RBV] = data();
            m[PV_TYPE::SIGMA_Y_RBV] = data();
            m[PV_TYPE::COV_XY_RBV] = data();
            m[PV_TYPE::X_PIX] = data();
            m[PV_TYPE::Y_PIX] = data();
            m[PV_TYPE::SIGMA_X_PIX] = data();
            m[PV_TYPE::SIGMA_Y_PIX] = data();
            m[PV_TYPE::COV_XY_PIX] = data();
            m[PV_TYPE::VC_INTENSITY] = data();
            return m;
        }
        static const std::map<PV_TYPE, data>  blank_image_data;
    };
    // The main hardware object holds ...
    struct virtualCathodeDataObject
    {
        virtualCathodeDataObject():
            name(UTL::UNKNOWN_NAME),
            pvRoot(UTL::UNKNOWN_PVROOT),
            buffer_size(UTL::BUFFER_60000)
            {}
        std::string name, pvRoot;
        size_t buffer_size;
        // the raw data
        std::deque<std::map<PV_TYPE, data> > image_data_buffer;
        // pointer of where the next data entry will go, so we can try and keep
        // anlysis of each image in the same image_data struct
        //std::map< PV_TYPE, std::dequestd::map<PV_TYPE, data>::iterator > image_data_buffer_next_update;
        std::map<PV_TYPE, std::map<PV_TYPE, data>*> image_data_buffer_next_update;
        std::map<PV_TYPE, pvStruct> pvMonStructs;
    };
}
//______________________________________________________________________________
#endif//_VELA_CLARA_PIL_VIRTUAL_CATHODE_STRUCTS_H_
