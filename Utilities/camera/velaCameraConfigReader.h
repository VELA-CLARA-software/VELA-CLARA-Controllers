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
//  Last edit:   02-08-2018
//  FileName:    velaCameraConfigReader.h
//  Description: reads config for velacams, this is entirely
//               based on previous versions from 2015 (and earlier)
//
//
//*/
#ifndef _VELA_CAM_CONFIG_READER_H_
#define _VELA_CAM_CONFIG_READER_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "cameraStructs.h"


class velaCameraConfigReader : public configReader
{
    public:

        velaCameraConfigReader::velaCameraConfigReader(const std::string& config,
                                                 const bool& show_messages,
                                                 const bool& show_debug_messages,
                                                 const bool usingVM);
        ~velaCameraConfigReader();

        bool readConfig();
        bool getCamData(std::map<std::string, cameraStructs::cameraObject>& mapToFill, const bool no_vc);

    private:
        bool readCameraConfig();

        void addToPVStruct(std::vector<cameraStructs::pvStruct>& pvStruct_v,
                           const std::vector<std::string>&keyVal);

        void addToCameraObjects(const std::vector<std::string>& keyVal);
        void addToPVMonitorMapV1(const std::vector<std::string>& keyVal);
        void addToPVCommandMapV1(const std::vector<std::string>& keyVal);

        void addCOUNT_MASK_OR_CHTYPE(std::vector<cameraStructs::pvStruct>& pvStruct_v,
                                     const std::vector<std::string>& keyVal);

        void addPVStruct(std::vector<cameraStructs::pvStruct>& pvs,
                         cameraStructs::CAM_PV_TYPE pvType,
                         const std::string& pvSuffix);

        std::vector<cameraStructs::cameraObject> camObjects;
        std::vector<cameraStructs::pvStruct> pvMonStructs;
        std::vector<cameraStructs::pvStruct> pvComStructs;

        cameraStructs::CAM_TYPE getCameType(const std::string& value);
};
#endif // _VELA_CAM_CONFIG_READER_H_
