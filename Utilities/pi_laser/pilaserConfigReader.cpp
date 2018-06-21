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
//djs
#include "pilaserConfigReader.h"
#include "pilaserStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//____________________________________________________________________________________________________
pilaserConfigReader::pilaserConfigReader(const std::string& configFile,
                           const bool& show_messages,
                           const bool& show_debug_messages,
                           const bool usingVM):
configReader(configFile,  show_messages, show_debug_messages,usingVM)
{
    message("creating a pilaserConfigReader" );
}
//______________________________________________________________________________
pilaserConfigReader::~pilaserConfigReader(){}
//______________________________________________________________________________
bool pilaserConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read PILaser Config File ****" );

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

        debugMessage("Opened from ", configFile1 );
        while(std::getline(inputFile, line)) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
            if(trimmedLine.size()> UTL::ZERO_SIZET )
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
                                if(trimmedLine.find_first_of(UTL::EQUALS_SIGN) != std::string::npos)
                                {
                                    std::vector<std::string> keyVal = getKeyVal(trimmedLine);

                                    if(readingObjs)
                                        addTopilaserObjectsV1(keyVal);

                                    else if (readingCommandPVs)
                                        addToPVCommandMapV1(keyVal);

                                    else if (readingMonitorPVs)
                                        addToPVMonitorMapV1(keyVal);
                                }
                                break;
                            default:
                                message("!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
                        }
                    }
                }
                if(stringIsSubString(line, UTL::START_OF_DATA))
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
                if(stringIsSubString(line, UTL::PV_MONITORS_START))
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage("Found PV_MONITORS_START" );
                }
                if(stringIsSubString(line, UTL::OBJECTS_START))
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
        message("!!!! Error Can't Open PILaser Config File after searching in:  ", configFile1, " !!!!"  );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void pilaserConfigReader::addToPVMonitorMapV1(const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX" ) )
    {
        if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_STATUS  )
        {
            addPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::STATUS, keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_HALF_WAVE_PLATE_READ)
        {
            addPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_READ, keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_SUFFIX_WCM_CHARGE)
        {
            addPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::WCM_Q, keyVal[ONE_SIZET]);
        }
//        else if( keyVal[0] == PV_IA_SUFFIX_ACQUIRE_RBV  )
//        {
//            addPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::CAM_ACQUIRE_RBV, keyVal[ONE_SIZET]);
//        }
//        else if( keyVal[0] == PV_IA_SUFFIX_CAM_STATE  )
//        {
//            addPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::CAM_STATUS, keyVal[ONE_SIZET]);
//        }
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvMonStructs, keyVal);
}
//______________________________________________________________________________
void pilaserConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<pilaserStructs::pvStruct>& pvStruct_v,
                                                  const std::vector<std::string>& keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        pvStruct_v.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
        pvStruct_v.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
}
//______________________________________________________________________________
void pilaserConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal  )
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX" ) )
    {
        if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_STABILISATION)
        {
            addPVStruct(pvComStructs, pilaserStructs::PILASER_PV_TYPE::STABILISATION, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_INTENSITY)
        {
            addPVStruct(pvComStructs, pilaserStructs::PILASER_PV_TYPE::INTENSITY, keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_HALF_WAVE_PLATE_SET)
        {
            addPVStruct(pvComStructs, pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_SET, keyVal[ONE_SIZET]);
        }
//        else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_START_ACQUIRE  )
//        {
//            addPVStruct(pvComStructs, pilaserStructs::PILASER_PV_TYPE::CAM_START_ACQUIRE,keyVal[ONE_SIZET]);
//        }
//        else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_STOP_ACQUIRE  )
//        {
//            addPVStruct(pvComStructs, pilaserStructs::PILASER_PV_TYPE::CAM_STOP_ACQUIRE,keyVal[ONE_SIZET]);
//        }
//        debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvComStructs.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvComStructs, keyVal );
}
//______________________________________________________________________________
void pilaserConfigReader::addPVStruct(std::vector<pilaserStructs::pvStruct>& pvs,
                                      pilaserStructs::PILASER_PV_TYPE pvType,
                                      const std::string& pvSuffix)
{
    pvs.push_back(pilaserStructs::pvStruct());
    pvs.back().pvType      = pvType;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
}
//______________________________________________________________________________
//void pilaserConfigReader::addPVStruct(std::vector<pilaserStructs::pvStruct>& pvs,
//                                      const std::vector<std::string>& keyVal)
//{
//    pvs.push_back(pilaserStructs::pvStruct() );
//    pvs.back().pvSuffix = keyVal[UTL::ONE_SIZET];
//    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_INTENSITY)
//    {
//        pvs.back().pvType = pilaserStructs::PILASER_PV_TYPE::INTENSITY;
//    }
//    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_STABILISATION)
//    {
//        pvs.back().pvType = pilaserStructs::PILASER_PV_TYPE::STABILISATION;
//    }
//    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_STATUS)
//    {
//        pvs.back().pvType = pilaserStructs::PILASER_PV_TYPE::STATUS;
//    }
//    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_HALF_WAVE_PLATE_SET)
//    {
//        pvs.back().pvType = pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_SET;
//    }
//    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_HALF_WAVE_PLATE_READ)
//    {
//        pvs.back().pvType = pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_READ;
//    }
//    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_WCM_CHARGE)
//    {
//        pvs.back().pvType = pilaserStructs::PILASER_PV_TYPE::WCM_Q;
//    }
//    /*
//        we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
//        so you can store a ref to which vector to update with that info.
//        (this does make sense)
//        //lastPVStruct = &pvs;
//        we can actually get EPICS to fill in these values for us
//    */
//    debugMessage("Added ", pvs.back().pvSuffix,
//                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
//}
//______________________________________________________________________________
void pilaserConfigReader::addTopilaserObjectsV1(const std::vector<std::string>& keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::NAME )
    {
        pilaserObject.name = keyVal[UTL::ONE_SIZET];
        //pilaserObject.numIlocks = numIlocks;
        debugMessage("Added ", pilaserObject.name );
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_ROOT)
    {
        if(useVM)
            pilaserObject.pvRoot =  UTL::VM_PREFIX + keyVal[UTL::ONE_SIZET];
        else
            pilaserObject.pvRoot = keyVal[UTL::ONE_SIZET];
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_ROOT_Q)
    {
        if(useVM)
            pilaserObject.pvRootQ =  UTL::VM_PREFIX + keyVal[UTL::ONE_SIZET];
        else
            pilaserObject.pvRootQ = keyVal[UTL::ONE_SIZET];

    }
}
//______________________________________________________________________________
bool pilaserConfigReader::getpilaserObject(pilaserStructs::pilaserObject& obj)
{
    obj = pilaserObject;
    for(auto && it : pvMonStructs)
    {
        obj.pvMonStructs[it.pvType] = it;
    }
    for(auto && it : pvComStructs)
    {
        obj.pvComStructs[it.pvType] = it;
    }
    return true;
}
















