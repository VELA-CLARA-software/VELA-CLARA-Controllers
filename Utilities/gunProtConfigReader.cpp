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
//  Last edit:   19-03-2018
//  FileName:    gunProtConfigReader.cpp
//  Description:
//
//
//*/
//djs
#include "gunProtConfigReader.h"
#include "configDefinitions.h"
#include "rfProtStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

gunProtConfigReader::gunProtConfigReader(const bool* show_messages_ptr,
                                         const bool* show_debug_messages_ptr):
configReader(UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr)
{}
//______________________________________________________________________________
gunProtConfigReader::gunProtConfigReader(const std::string& configFileLocation1,
                                         const bool startVirtualMachine,
                                         const bool* show_messages_ptr,
                                         const bool* show_debug_messages_ptr):
configReader(configFileLocation1, show_messages_ptr, show_debug_messages_ptr)
{}
//______________________________________________________________________________
gunProtConfigReader::~gunProtConfigReader(){}
//______________________________________________________________________________
bool gunProtConfigReader::getrfGunProtObjects(std::map<std::string, rfProtStructs::rfGunProtObject>& mapToFill )
{
    mapToFill.clear();
    for(auto && it : gunProtObjects)
    {
        mapToFill[it.name] = it;

        for(auto && it_m : pvMonStructs )
            mapToFill[it.name].pvMonStructs[ it_m.pvType ] = it_m;

        for(auto && it_c : pvComStructs)
            mapToFill[it.name].pvComStructs[ it_c.pvType ] = it_c;

        message("Added ", it.name, " To interface map.");
    }
    bool r = false;
    if( mapToFill.size() == numObjs )
    {
        r = true;
        message("!!!SUCCESS!!!! Found ", mapToFill.size(), " objects and expecetd ",numObjs );
    }
    else
    {
        message("!!!gunProtConfigReader ERROR!!!! Found ", mapToFill.size(), " objects and expecetd ",numObjs );
    }
    return r;
}
//______________________________________________________________________________
bool gunProtConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read Gun Modulator Config File ****" );

    std::string line, trimmedLine;
    bool success = false;

    pvComStructs.clear();
    pvMonStructs.clear();
    std::ifstream inputFile;

    inputFile.open(configFile1, std::ios::in );
    if(inputFile )
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage("Opened from ", configFile1 );
        while(std::getline(inputFile, line ) ) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE ) );
            if(trimmedLine.size() > 0 )
            {
                if(stringIsSubString(line, UTL::END_OF_DATA ) )
                {
                    debugMessage("Found END_OF_DATA" );
                    readingData = false;
                    readingObjs = false;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    break;
                }
                if(readingData )
                {
                    if(stringIsSubString(trimmedLine, UTL::VERSION ) )
                        getVersion(trimmedLine );
                    else if(stringIsSubString(trimmedLine, UTL::NUMBER_OF_OBJECTS ) )
                        getNumObjs(trimmedLine );
                    else if(stringIsSubString(trimmedLine, UTL::NUMBER_OF_ILOCKS ) )
                        getNumIlocks(trimmedLine );
                    else
                    {
                        switch(configVersion )
                        {
                            case 1:
                                if(trimmedLine.find_first_of(UTL::EQUALS_SIGN ) != std::string::npos )
                                {
                                    std::vector<std::string> keyVal = getKeyVal(trimmedLine );

                                    if(readingObjs )
                                        addToProtObjectsV1(keyVal );

                                    else if (readingCommandPVs  )
                                        addToPVCommandMapV1(keyVal );

                                    else if (readingMonitorPVs )
                                        addToPVMonitorMapV1(keyVal );
                                }
                                break;
                            default:
                                message("!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
                        }
                    }
                }
                if(stringIsSubString(line, UTL::START_OF_DATA ) )
                {
                    readingData = true;
                    debugMessage("Found START_OF_DATA" );
                }
                if(stringIsSubString(line, UTL::PV_COMMANDS_START ) )
                {
                    readingCommandPVs  = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage("Found PV_COMMANDS_START" );
                }
                if(stringIsSubString(line, UTL::PV_MONITORS_START ) )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage("Found PV_MONITORS_START" );
                }
                if(stringIsSubString(line, UTL::OBJECTS_START ) )
                {
                    readingObjs        = true;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    debugMessage("Found OBJECTS_START" );
                }
            }
        }
        inputFile.close();
        debugMessage("File Closed" );

        success = true;
    }
    else{
        message("!!!! Error Can't Open  Gun Modulator Config File after searching in:  ", configFile1, " !!!!"  );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void gunProtConfigReader::addToPVMonitorMapV1(const std::vector<std::string> &keyVal )
{
    addToPVStruct(pvMonStructs, keyVal);
}
//______________________________________________________________________________
void gunProtConfigReader::addToPVCommandMapV1(const std::vector<std::string> &keyVal )
{
    addToPVStruct(pvComStructs, keyVal);
}
//______________________________________________________________________________
void gunProtConfigReader::addToProtObjectsV1(const std::vector<std::string> &keyVal )
{
    if(keyVal[0] == UTL::NAME)
    {
        rfProtStructs::rfGunProtObject prot = rfProtStructs::rfGunProtObject();
        prot.name = keyVal[1];
        prot.numIlocks = (size_t)numIlocks;
        gunProtObjects.push_back(prot);
        debugMessage("Added ", gunProtObjects.back().name," ", gunProtObjects.size(), " objects");
    }
    else if(keyVal[0] == UTL::PV_ROOT)
        gunProtObjects.back().pvRoot = keyVal[1];
    else if(keyVal[0] == UTL::GUN_PROT_KEY_BITS)
    {
        gunProtObjects.back().gunProtKeyBits = getIntVector( keyVal[1] );

        for(auto && it : gunProtObjects.back().gunProtKeyBits )
        {
            gunProtObjects.back().gunProtKeyBitValues.push_back(false);
        }
    }
    else if(keyVal[0] == UTL::RF_PROT_TYPE)
    {
        if( keyVal[1] == UTL::ENABLE)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::ENABLE;
        else if( keyVal[1] == UTL::GENERAL)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::GENERAL;
        else if( keyVal[1] == UTL::CLARA_HRRG)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::CLARA_HRRG;
        else if( keyVal[1] == UTL::VELA_LRRG)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::VELA_LRRG;
        else if( keyVal[1] == UTL::CLARA_LRRG)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::CLARA_LRRG;
        else if( keyVal[1] == UTL::VELA_HRRG)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::VELA_HRRG;
        else if( keyVal[1] == UTL::TEST)
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::TEST;
        else
            gunProtObjects.back().protType = rfProtStructs::RF_PROT_TYPE::NOT_KNOWN;

        message(gunProtObjects.back().name," gunProtObjects.back().protType = ", ENUM_TO_STRING(gunProtObjects.back().protType));
    }
}
//______________________________________________________________________________
void gunProtConfigReader::addToPVStruct(std::vector< rfProtStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if(stringIsSubString(keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back(rfProtStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        //GUN  Protection
        if(keyVal[0] == UTL::PV_SUFFIX_ON  )
            pvStruct_v.back().pvType = rfProtStructs::RF_GUN_PROT_PV_TYPE::ON;

        else if(keyVal[0] == UTL::PV_SUFFIX_OFF  )
            pvStruct_v.back().pvType = rfProtStructs::RF_GUN_PROT_PV_TYPE::OFF;

        else if(keyVal[0] == UTL::PV_SUFFIX_STA  )
            pvStruct_v.back().pvType = rfProtStructs::RF_GUN_PROT_PV_TYPE::STATUS;

        else if(keyVal[0] == UTL::PV_SUFFIX_CMI  )
            pvStruct_v.back().pvType = rfProtStructs::RF_GUN_PROT_PV_TYPE::CMI;

        else if(keyVal[0] == UTL::PV_SUFFIX_RST  )
            pvStruct_v.back().pvType = rfProtStructs::RF_GUN_PROT_PV_TYPE::RESET;

         debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvStruct_v, keyVal );
}
//______________________________________________________________________________
void gunProtConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector< rfProtStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if(keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT(keyVal[ 1 ] );
    else if(keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK(keyVal[ 1 ] );
    else if(keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[ 1 ] );
}
////______________________________________________________________________________
//bool gunProtConfigReader::readConfig(gunProtConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
//{
//    debugMessage("\n", "**** Attempting to Read ", fn, " ****" );
//
//    std::string line, trimmedLine;
//    bool success = false;
//
//    std::ifstream inputFile;
//
//    inputFile.open(fn, std::ios::in );
//    if(inputFile )
//    {
//        bool readingData       = false;
//        bool readingObjs       = false;
//        bool readingCommandPVs = false;
//        bool readingMonitorPVs = false;
//
//        debugMessage("File Opened from ", fn );
//        while(std::getline(inputFile, line ) ) /// Go through, reading file line by line
//        {
//            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE ) );
//            if(trimmedLine.size() > 0 )
//            {
//                if(stringIsSubString(line, UTL::END_OF_DATA ) )
//                {
//                    debugMessage("Found END_OF_DATA" );
//                    readingData = false;
//                    readingObjs = false;
//                    readingCommandPVs  = false;
//                    readingMonitorPVs  = false;
//                    break;
//                }
//                if(readingData )
//                {
//                    if(stringIsSubString(trimmedLine, UTL::VERSION ) )
//                        getVersion(trimmedLine );
//                    else if(stringIsSubString(trimmedLine, UTL::NUMBER_OF_OBJECTS ) )
//                        getNumObjs(trimmedLine );
//                    else if(stringIsSubString(trimmedLine, UTL::NUMBER_OF_ILOCKS ) )
//                        getNumIlocks(trimmedLine );
//                    else
//                    {
//                        switch(configVersion )
//                        {
//                            case 1:
//                                if(trimmedLine.find_first_of(UTL::EQUALS_SIGN ) != std::string::npos )
//                                {
//                                    std::vector<std::string> keyVal = getKeyVal(trimmedLine );
//
//                                    if(readingObjs )
//                                        CALL_MEMBER_FN(obj, f1)(keyVal ) ;
//
//                                    else if (readingCommandPVs  )
//                                        CALL_MEMBER_FN(obj, f2)(keyVal ) ;
//
//                                    else if (readingMonitorPVs )
//                                        CALL_MEMBER_FN(obj, f3)(keyVal ) ;
//                                }
//                                break;
//                            default:
//                                message("!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
//                        }
//                    }
//                }
//                if(stringIsSubString(line, UTL::START_OF_DATA ) )
//                {
//                    readingData = true;
//                    debugMessage("Found START_OF_DATA" );
//                }
//                if(stringIsSubString(line, UTL::PV_COMMANDS_START ) )
//                {
//                    readingCommandPVs  = true;
//                    readingObjs = false;
//                    readingMonitorPVs = false;
//                    debugMessage("Found PV_COMMANDS_START" );
//                }
//                if(stringIsSubString(line, UTL::PV_MONITORS_START ) )
//                {
//                    readingCommandPVs = false;
//                    readingObjs       = false;
//                    readingMonitorPVs = true;
//                    debugMessage("Found PV_MONITORS_START" );
//                }
//                if(stringIsSubString(line, UTL::OBJECTS_START ) )
//                {
//                    readingObjs        = true;
//                    readingCommandPVs  = false;
//                    readingMonitorPVs  = false;
//                    debugMessage("Found OBJECTS_START" );
//                }
//            }
//        }
//        inputFile.close();
//        debugMessage("File Closed" );
//        success = true;
//    }
//    else{
//        message("!!!! Error Can't Open File after searching for:  ", fn, " !!!!"  );
//    }
//    return success;
//}
////______________________________________________________________________________
//
//
//
