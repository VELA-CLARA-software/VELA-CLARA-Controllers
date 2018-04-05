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
//  FileName:    pilaserMirrorConfigReader.cpp
//  Description:
//
//
//*/
// project  inlcudes
#include "pilaserMirrorConfigReader.h"
#include "pilaserStructs.h"
// stl includes
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//____________________________________________________________________________________________________
pilaserMirrorConfigReader::pilaserMirrorConfigReader(const std::string& configFile,
                                                     const bool& show_messages,
                                                     const bool& show_debug_messages,
                                                     const bool usingVM):
configReader(configFile, show_messages, show_debug_messages, usingVM)
{}
//______________________________________________________________________________
pilaserMirrorConfigReader::~pilaserMirrorConfigReader(){}
////______________________________________________________________________________
bool pilaserMirrorConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read PILaser Mirror Config File ****");

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
                                        addTopilaserObjectsV1(keyVal);

                                    else if (readingCommandPVs)
                                        addToPVCommandMapV1(keyVal);
//
//                                    else if (readingMonitorPVs)
//                                        addToPVMonitorMapV1(keyVal);
                                }
                                break;
                            default:
                                message("!!!!!WARNING DID NOT FIND CONFIG "
                                        "FILE VERSION NUMBER!!!!!!" );
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

        success = true;
    }
    else{
        message("!!!! Error Can't Open PILaser Mirror  Config File "
                "after searching in: ",configFile1, " !!!!" );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void pilaserMirrorConfigReader::addPVStruct(
                std::vector<pilaserMirrorStructs::pvStruct>& pvs,
                const std::vector<std::string>& keyVal)
{
    pvs.push_back(pilaserMirrorStructs::pvStruct());
    pvs.back().pvSuffix = keyVal[UTL::ONE_SIZET];

    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_STEP)
        pvs.back().pvType = pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::H_STEP;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_STEP)
        pvs.back().pvType = pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_STEP;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_POS)
        pvs.back().pvType = pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::H_POS;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_POS)
        pvs.back().pvType = pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_POS;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_MOVE)
        pvs.back().pvType = pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::H_MOVE;
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_MOVE)
        pvs.back().pvType = pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_MOVE;
    /*
        we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
        so you can store a ref to which vector to update with that info.
        (this does make sense)
        //lastPVStruct = &pvs;
        we can actually get EPICS to fill in these values for us
    */
    debugMessage("Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
}
//______________________________________________________________________________
void pilaserMirrorConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal )
{
    /* switch to set different PVs for the VM and physical Machine */
    if(useVM)
    {
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_POS)
            addPVStruct(pvComStructs, keyVal);
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_POS)
            addPVStruct(pvComStructs, keyVal);
    }
    else
    {
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_STEP)
            addPVStruct(pvComStructs, keyVal);
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_STEP)
            addPVStruct(pvComStructs, keyVal);
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_MOVE)
            addPVStruct(pvComStructs, keyVal);
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_MOVE)
            addPVStruct(pvComStructs, keyVal);
    }

    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        pvComStructs.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
        pvComStructs.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
        pvComStructs.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
}
//______________________________________________________________________________
void pilaserMirrorConfigReader::addTopilaserObjectsV1(const std::vector<std::string>& keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::NAME)
    {
        pilMirrorObject.name = keyVal[UTL::ONE_SIZET];
        //pilMirrorObject.numIlocks = numIlocks;
        debugMessage("Added ", pilMirrorObject.name);
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_ROOT)
    {
        if(useVM)
            pilMirrorObject.pvRoot =  UTL::VM_PREFIX + keyVal[UTL::ONE_SIZET];
        else
            pilMirrorObject.pvRoot = keyVal[UTL::ONE_SIZET];
    }
}
//______________________________________________________________________________
bool pilaserMirrorConfigReader::getpilMirrorObject(pilaserMirrorStructs::pilMirrorObject & obj)
{
    obj = pilMirrorObject;
    for(auto&& it:pvMonStructs)
    {
        obj.pvMonStructs[it.pvType] = it;
    }
    for(auto&& it:pvComStructs)
    {
        obj.pvComStructs[it.pvType] = it;
    }
    return true;
}



