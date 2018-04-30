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
//  FileName:    pilaserConfigReader.cpp
//  Description:
//
//
//*/
#ifndef _PI_LASER_CONFIG_READER_H_
#define _PI_LASER_CONFIG_READER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project includes
#include "configReader.h"
#include "pilaserStructs.h"
//____________________________________________________________________________________________________
class pilaserConfigReader:public configReader
{
    public:
        pilaserConfigReader(const std::string& configFile,
                            const bool& show_messages,
                            const bool& show_debug_messages,
                            const bool usingVM);
        ~pilaserConfigReader();

        bool readConfig();
        bool getpilaserObject(pilaserStructs::pilaserObject& obj);

    private:
        pilaserStructs::pilaserObject pilaserObject;
        std::vector<pilaserStructs::pvStruct> pvMonStructs;
        std::vector<pilaserStructs::pvStruct> pvComStructs;
        void addCOUNT_MASK_OR_CHTYPE(std::vector<pilaserStructs::pvStruct>& pvStruct_v,
                                     const std::vector<std::string>& keyVal);
        void addPVStruct(std::vector< pilaserStructs::pvStruct>& pvs,
                         const std::vector<std::string>& keyVal);
        void addTopilaserObjectsV1(const std::vector<std::string>& keyVal);
        void addToPVCommandMapV1  (const std::vector<std::string>& keyVal);
        void addToPVMonitorMapV1  (const std::vector<std::string>& keyVal);
};
//____________________________________________________________________________________________________
#endif //_PI_LASER_CONFIG_READER_H_
