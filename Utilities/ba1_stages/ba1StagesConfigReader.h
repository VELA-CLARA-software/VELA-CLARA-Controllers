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
//  FileName:    ba1StagesConfigReader.h
//  Description:
//
//
//*/
#ifndef CONFIG_READER_BA1_STAGES_H
#define CONFIG_READER_BA1_STAGES_H
// stl
#include <string>
#include <vector>
#include <map>
// me
#include "configReader.h"
#include "ba1StagesStructs.h"


class ba1StagesConfigReader : public configReader
{
    public:
        ba1StagesConfigReader(const std::string& configFile,
                           const bool& show_messages,
                           const bool& show_debug_messages,
                           const bool usingVM);
        ~ba1StagesConfigReader();
//
        bool readConfig();
        const std::vector<ba1StagesStructs::ba1StagesObject> getBA1StageObjects();

    private:
        std::vector<ba1StagesStructs::ba1StagesObject> ba1StagesObjects;
        std::vector<ba1StagesStructs::pvStruct     > pvMonStructs;
        std::vector<ba1StagesStructs::pvStruct     > pvComStructs;

        void addPVStruct(std::vector<ba1StagesStructs::pvStruct>& pvStruct_v,
                         const std::vector<std::string>& keyVal);

        void addToBA1StageObjectsV1(const std::vector<std::string>& keyVal);
        void addToPVCommandMapV1  (const std::vector<std::string>& keyVal);
        void addToPVMonitorMapV1  (const std::vector<std::string>& keyVal);

};
#endif //UTL_FILE_IO_H
