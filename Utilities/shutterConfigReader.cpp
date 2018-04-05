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
//  FileName:    shutterConfigReader.cpp
//  Description:
//
//
//*/
//djs
#include "shutterConfigReader.h"
#include "shutterStructs.h"
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
shutterConfigReader::shutterConfigReader(const std::string& configFile,
                                         const bool& show_messages,
                                         const bool& show_debug_messages,
                                         const bool usingVM):
configReader(configFile, show_messages, show_debug_messages, usingVM)
{}
//______________________________________________________________________________
shutterConfigReader::~shutterConfigReader(){}
//______________________________________________________________________________
bool shutterConfigReader::readShutterConfig()
{
    debugMessage("\n", "**** Attempting to Read The Shutter Config File ****");

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
                                        addToShutterObjectsV1(keyVal);

                                    else if (readingCommandPVs)
                                        addToPVCommandMapV1(keyVal);

                                    else if (readingMonitorPVs)
                                        addToPVMonitorMapV1(keyVal);
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

        if(numObjs == shutterObjects.size())
            debugMessage("*** Created ", numObjs, " Shutter Objects, As Expected ***");
        else
            debugMessage("*** Created ", shutterObjects.size() ," Expected ", numObjs,  " ERROR ***");

        success = true;
    }
    else{
        message("!!!! Error Can't Open Shutter Config File after searching in:  ", configFile1, " !!!!");
    }
    return success;
}
//______________________________________________________________________________
void shutterConfigReader::addToPVMonitorMapV1(const  std::vector<std::string> &keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_STA)
       addPVStruct(pvMonStructs, keyVal);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        pvMonStructs.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
        pvMonStructs.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
        pvMonStructs.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
}
//______________________________________________________________________________
void shutterConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_ON)
        addPVStruct(pvComStructs, keyVal);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_OFF)
        addPVStruct(pvComStructs, keyVal);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        pvComStructs.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
        pvComStructs.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
        pvComStructs.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
}
//______________________________________________________________________________
void shutterConfigReader::addPVStruct(std::vector<shutterStructs::pvStruct>& pvStruct_v,
                                      const std::vector<std::string>& keyVal)
{
    pvStruct_v.push_back(shutterStructs::pvStruct());
    pvStruct_v.back().pvSuffix = keyVal[UTL::ONE_SIZET];
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_ON)
        pvStruct_v.back().pvType = shutterStructs::SHUTTER_PV_TYPE::On;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_OFF)
        pvStruct_v.back().pvType = shutterStructs::SHUTTER_PV_TYPE::Off;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_STA)
        pvStruct_v.back().pvType = shutterStructs::SHUTTER_PV_TYPE::Sta;

    debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix");
}
//______________________________________________________________________________
void shutterConfigReader::addToShutterObjectsV1(const std::vector<std::string>& keyVal )
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::NAME)
    {
        shutterStructs::shutterObject shob = shutterStructs::shutterObject();
        shob.name = keyVal[UTL::ONE_SIZET];
        shob.shutterState = shutterStructs::SHUTTER_STATE::ERROR;
        shob.numIlocks = numIlocks;
        shutterObjects.push_back(shob);
        debugMessage("Added ", shutterObjects.back().name);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_ROOT)
        shutterObjects.back().pvRoot = keyVal[UTL::ONE_SIZET];
}
//______________________________________________________________________________
const std::vector< shutterStructs::shutterObject > shutterConfigReader::getShutterObjects()
{
    /// turn the pv vectors into maps, you can probably figure a way of doing this in
    /// addPVStruct, but i like using vector.back()

    std::map<shutterStructs::SHUTTER_PV_TYPE, shutterStructs::pvStruct > pvMonStructs_m;
    std::map<shutterStructs::SHUTTER_PV_TYPE, shutterStructs::pvStruct > pvComStructs_m;
    for(auto&& it : pvMonStructs)
        pvMonStructs_m[it.pvType] = it;

    for(auto&& it : pvComStructs)
        pvComStructs_m[it.pvType] = it;

    for(auto&& it : shutterObjects)
    {
        it.pvComStructs = pvComStructs_m;
        it.pvMonStructs = pvMonStructs_m;
    }
    return shutterObjects;
}



