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
//  FileName:    ba1StagesConfigReader.cpp
//  Description:
//
//
//*/
//djs
#include "ba1StagesConfigReader.h"
#include "ba1StagesStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//______________________________________________________________________________
ba1StagesConfigReader::ba1StagesConfigReader(const std::string& configFile,
                                         const bool& show_messages,
                                         const bool& show_debug_messages,
                                         const bool usingVM):
configReader(configFile, show_messages, show_debug_messages, usingVM)
{}
//______________________________________________________________________________
ba1StagesConfigReader::~ba1StagesConfigReader(){}
//______________________________________________________________________________
bool ba1StagesConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read The BA1 Stages Config File ****");

    std::string line, trimmedLine;
    bool success = false;

    pvComStructs.clear();
    pvMonStructs.clear();
    std::ifstream inputFile;

    inputFile.open(configFile1, std::ios::in);
    if(inputFile)
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage("Opened from ", configFile1);
        while(std::getline(inputFile, line)) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
            if(trimmedLine.size() > UTL::ZERO_SIZET)
            {
                if(stringIsSubString(line, UTL::END_OF_DATA))
                {
                    debugMessage("Found END_OF_DATA");
                    readingData = false;
                    readingObjs = false;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    break;
                }
                if(readingData)
                {
                    if(stringIsSubString(trimmedLine, UTL::VERSION))
                        getVersion(trimmedLine);
                    else if(stringIsSubString(trimmedLine, UTL::NUMBER_OF_OBJECTS))
                        getNumObjs(trimmedLine);
                    else if(stringIsSubString(trimmedLine, UTL::NUMBER_OF_ILOCKS))
                        getNumIlocks(trimmedLine);
                    else
                    {
                        switch(configVersion)
                        {
                            case 1:
                                if(trimmedLine.find_first_of(UTL::EQUALS_SIGN) != std::string::npos)
                                {
                                    std::vector<std::string> keyVal = getKeyVal(trimmedLine);

                                    if(readingObjs)
                                    {
                                        addToBA1StageObjectsV1(keyVal);
                                    }

                                    else if (readingCommandPVs)
                                    {
                                        addToPVCommandMapV1(keyVal);
                                    }
                                    else if (readingMonitorPVs)
                                    {
                                        addToPVMonitorMapV1(keyVal);
                                    }
                                }
                                break;
                            default:
                                message("!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!");
                        }
                    }
                }
                if(stringIsSubString(line, UTL::START_OF_DATA))
                {
                    readingData = true;
                    debugMessage("Found START_OF_DATA");
                }
                if(stringIsSubString(line, UTL::PV_COMMANDS_START))
                {
                    readingCommandPVs  = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage("Found PV_COMMANDS_START");
                }
                if(stringIsSubString(line, UTL::PV_MONITORS_START))
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage("Found PV_MONITORS_START");
                }
                if(stringIsSubString(line, UTL::OBJECTS_START))
                {
                    readingObjs        = true;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    debugMessage("Found OBJECTS_START");
                }
            }
        }
        inputFile.close();
        debugMessage("File Closed");

        if(numObjs == ba1StagesObjects.size())
            debugMessage("*** Created ", numObjs, " BA1 Stage Objects, As Expected ***");
        else
            debugMessage("*** Created ", ba1StagesObjects.size() ," Expected ", numObjs,  " ERROR ***");

        success = true;
    }
    else{
        message("!!!! Error Can't Open BA1 Stages Config File after searching in:  ", configFile1, " !!!!");
    }
    return success;
}
//______________________________________________________________________________
void ba1StagesConfigReader::addToPVMonitorMapV1(const  std::vector<std::string> &keyVal)
{
    message("Monitor ", keyVal[UTL::ZERO_SIZET],"  ", keyVal[UTL::ONE_SIZET]);
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BA1_RPOS)
    {
        addPVStruct(pvMonStructs, keyVal);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BA1_MPOS)
    {
        addPVStruct(pvMonStructs, keyVal);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
    {
        pvMonStructs.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
    {
        pvMonStructs.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
    {
        pvMonStructs.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
    }
}
//______________________________________________________________________________
void ba1StagesConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal)
{
    message("Command ", keyVal[UTL::ZERO_SIZET],"  ", keyVal[UTL::ONE_SIZET]);

    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BA1_MPOS)
    {
        addPVStruct(pvComStructs, keyVal);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
    {
        pvComStructs.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
    {
        pvComStructs.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
    {
        pvComStructs.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
    }
}
//______________________________________________________________________________
void ba1StagesConfigReader::addPVStruct(std::vector<ba1StagesStructs::pvStruct>& pvStruct_v,
                                      const std::vector<std::string>& keyVal)
{
    pvStruct_v.push_back(ba1StagesStructs::pvStruct());
    pvStruct_v.back().pvSuffix = keyVal[UTL::ONE_SIZET];
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BA1_MPOS)
    {
        pvStruct_v.back().pvType = ba1StagesStructs::BA1STAGE_PV_TYPE::MPOS;
        debugMessage("Added ", ENUM_TO_STRING(ba1StagesStructs::BA1STAGE_PV_TYPE::MPOS));
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BA1_RPOS)
    {
        pvStruct_v.back().pvType = ba1StagesStructs::BA1STAGE_PV_TYPE::RPOS;
        debugMessage("Added ", ENUM_TO_STRING(ba1StagesStructs::BA1STAGE_PV_TYPE::RPOS));
    }
    debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix");
}
//______________________________________________________________________________
void ba1StagesConfigReader::addToBA1StageObjectsV1(const std::vector<std::string>& keyVal )
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::NAME)
    {
        ba1StagesStructs::ba1StagesObject shob = ba1StagesStructs::ba1StagesObject();
        shob.name = keyVal[UTL::ONE_SIZET];
        ba1StagesObjects.push_back(shob);
        debugMessage("Added ", ba1StagesObjects.back().name);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_ROOT)
    {
        ba1StagesObjects.back().pvRoot = keyVal[UTL::ONE_SIZET];
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::MAX_POS)
    {
        ba1StagesObjects.back().max_pos = getNumD(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::MIN_POS)
    {
        ba1StagesObjects.back().min_pos = getNumD(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::HAS_YAG)
    {
        ba1StagesObjects.back().has_yag = getBool(keyVal[UTL::ONE_SIZET]);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::DEVICES)
    {
        ba1StagesObjects.back().devices = stringSplitComma(keyVal[UTL::ONE_SIZET]);

        for(auto i : ba1StagesObjects.back().devices)
        {
            message("Added device = ", i );
        }
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::DEVICE_POSITIONS)
    {
        ba1StagesObjects.back().positions = getNumVector<double>(keyVal[UTL::ONE_SIZET]);
    }
}
//______________________________________________________________________________
const std::vector< ba1StagesStructs::ba1StagesObject > ba1StagesConfigReader::getBA1StageObjects()
{
    /// turn the pv vectors into maps, you can probably figure a way of doing this in
    /// addPVStruct, but i like using vector.back()

    std::map<ba1StagesStructs::BA1STAGE_PV_TYPE, ba1StagesStructs::pvStruct > pvMonStructs_m;
    std::map<ba1StagesStructs::BA1STAGE_PV_TYPE, ba1StagesStructs::pvStruct > pvComStructs_m;
    for(auto&& it : pvMonStructs)
    {
        pvMonStructs_m[it.pvType] = it;
    }

    for(auto&& it : pvComStructs)
    {
        pvComStructs_m[it.pvType] = it;
    }

    for(auto&& it : ba1StagesObjects)
    {
        it.pvComStructs = pvComStructs_m;
        it.pvMonStructs = pvMonStructs_m;

        // sanity check devices and positions
        if(it.devices.size() != it.positions.size())
        {
            message("Config Error for ",it.name,", num devices =  ",it.devices.size(), ", num device positions = ",it.positions.size());
        }
        else
        {
            // meh
            for(auto i = UTL::ZERO_SIZET; i < it.devices.size(); ++i)
            {
                it.device_position_map[it.devices[i]] = it.positions[i];
            }
        }
    }
    return ba1StagesObjects;
}



