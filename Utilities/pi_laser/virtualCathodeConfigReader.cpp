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
//  FileName:    virtualCathodeConfigReader.h
//  Description:
//
//
//*/
// project includes
#include "virtualCathodeConfigReader.h"
#include "pilaserStructs.h"
// stl includes
#include <iostream>
#include <vector>
#include <string>

#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//____________________________________________________________________________________________________
virtualCathodeConfigReader::virtualCathodeConfigReader(const std::string& vcMirrorConfig,
                                                       const std::string& vcDataConfig,
                                                       const bool& show_messages,
                                                       const bool& show_debug_messages,
                                                       const bool usingVM):
configReader(vcMirrorConfig,
             vcDataConfig,
             show_messages,
             show_debug_messages,
             usingVM),
readingMrror(false),
readingData(false)
{}
//______________________________________________________________________________
virtualCathodeConfigReader::~virtualCathodeConfigReader(){}
//______________________________________________________________________________
bool virtualCathodeConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read vc_Mirror Config Files ****" );

    pvMonStructs.clear();
    pvComStructs.clear();
    std::ifstream inputFile;

    inputFile.open(configFile1, std::ios::in );
    readingMrror = true;
    readingData  = false;
    bool mirror_read = readConfig(inputFile);

    readingMrror = false;
    readingData  = true;

    debugMessage("\n", "**** Attempting to Read vc_AnalysisData Config Files ****" );

    inputFile.open(configFile2, std::ios::in );
    bool data_read   = readConfig(inputFile);
    return true;
}
//______________________________________________________________________________
bool virtualCathodeConfigReader::readConfig(std::ifstream& inputFile)
{
    std::string line, trimmedLine;
    bool success = false;
    if(inputFile)
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage("Opened from ", configFile1 );
        while(std::getline(inputFile, line ) ) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE ) );
            if(trimmedLine.size() > UTL::ZERO_SIZET )
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
                                        addToObjectsV1(keyVal );

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
                if(stringIsSubString(line, UTL::START_OF_DATA))
                {
                    readingData = true;
                    debugMessage("Found START_OF_DATA" );
                }
                if(stringIsSubString(line, UTL::PV_COMMANDS_START))
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
        message("!!!! Error Can't Open PILaser Config File after searching in:  ", configFile1, " !!!!"  );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToPVMonitorMapV1(const std::vector<std::string>&keyVal)
{
    using namespace UTL;
    /*
        switch to set different PVs for the VM and physical Machine
    */
    if(useVM)
    {

    }
    else// not using vm
    {
        if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX"))
        {
            using namespace virtualCathodeStructs;
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_X)
            {
                addToPVStruct(pvMonStructs, VC_PV_TYPE::X_RBV,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_Y)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::Y_RBV,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_X)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::SIGMA_X_RBV,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_Y)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::SIGMA_Y_RBV,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_X_PIX)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::X_PIX,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_Y_PIX)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::Y_PIX,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_X_PIX)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::SIGMA_X_PIX,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_Y_PIX)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::SIGMA_Y_PIX,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_COV_XY)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::COV_XY_RBV,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_COV_XY_PIX)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::COV_XY_PIX,keyVal[ONE_SIZET] );
            }
            if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_AV_PIX_INTENSITY)
            {
                addToPVStruct(pvMonStructs,VC_PV_TYPE::VC_INTENSITY,keyVal[ONE_SIZET] );
            }
            else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIXEL_RESULTS)
            {
                 addToPVStruct(pvMonStructs,VC_PV_TYPE::PIXEL_RESULTS,keyVal[ONE_SIZET] );
            }
//            if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_H_STEP)
//            {
//                addPVStruct(pvComStructs, keyVal);
//            }
//            else if(keyVal[ZERO_SIZET] == PV_SUFFIX_PIL_V_STEP)
//            {
//                addPVStruct(pvComStructs, keyVal);
//            }
        }
        /*
            we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
            so you can store a ref to which vector to update with that info.
            (this does make sense)
            //lastPVStruct = &pvs;
            we can actually get EPICS to fill in these values for us
        */
        else
            addCOUNT_MASK_OR_CHTYPE(pvMonStructs, keyVal);
    }// close not using vm
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToPVCommandMapV1(const std::vector<std::string>&keyVal)
{
    using namespace UTL;
    /*
        switch to set different PVs for the VM and physical Machine
    */
    if(useVM)
    {

    }
    else// not using vm
    {
        if(stringIsSubString(keyVal[UTL::ZERO_SIZET], "SUFFIX" ) )
        {
            using namespace virtualCathodeStructs;
            if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_STEP)
            {
                addToPVStruct(pvComStructs, VC_PV_TYPE::H_STEP,keyVal[ONE_SIZET]);
            }
            else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_STEP)
            {
                addToPVStruct(pvComStructs, VC_PV_TYPE::V_STEP,keyVal[ONE_SIZET]);
            }
            else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_H_MOVE)
            {
                addToPVStruct(pvComStructs, VC_PV_TYPE::H_MOVE,keyVal[ONE_SIZET]);
            }
            else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_PIL_V_MOVE)
            {
                addToPVStruct(pvComStructs, VC_PV_TYPE::V_MOVE,keyVal[ONE_SIZET]);
            }
            //debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvComStructs.back().pvType) ) ;
        }
        /*
            we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
            so you can store a ref to which vector to update with that info.
            (this does make sense)
            //lastPVStruct = &pvs;
            we can actually get EPICS to fill in these values for us
        */
        else
            addCOUNT_MASK_OR_CHTYPE(pvComStructs, keyVal );
    }// close not using vm
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<virtualCathodeStructs::pvStruct>& pvStruct_v,
                                                  const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    /*
        switch to set different PVs for the VM and physical Machine
    */
    if(useVM)
    {

    }
    else
    {
        if(keyVal[ZERO_SIZET] == PV_COUNT)
        {
            pvStruct_v.back().COUNT = getCOUNT(keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == UTL::PV_MASK)
        {
            pvStruct_v.back().MASK = getMASK(keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_CHTYPE)
        {
            pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[ONE_SIZET]);
        }
    }// close not using vm
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToObjectsV1(const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    if(readingData)
    {
        if(keyVal[ZERO_SIZET] == NAME )
        {
            vcObject.name = keyVal[ONE_SIZET];
            //object.numIlocks = numIlocks;
            debugMessage("Added ", vcObject.name );
        }
        else if(keyVal[ZERO_SIZET] == PV_ROOT)
        {
            if(useVM )
                vcObject.pvRoot =  VM_PREFIX + keyVal[ONE_SIZET];
            else
                vcObject.pvRoot = keyVal[ONE_SIZET];
        }
    }
    if(readingMrror)
    {
        if(keyVal[ZERO_SIZET] == NAME )
        {
            vcObject.mirror.name = keyVal[ONE_SIZET];
            debugMessage("Added ", vcObject.mirror.name);
        }
        else if(keyVal[ZERO_SIZET] == PV_ROOT)
        {
            if(useVM)
                vcObject.mirror.pvRoot =  VM_PREFIX + keyVal[ONE_SIZET];
            else
                vcObject.mirror.pvRoot = keyVal[ONE_SIZET];
        }
        else if(keyVal[ZERO_SIZET] == X_POS)
        {
            vcObject.x_pos = getSize(keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == Y_POS)
        {
            vcObject.y_pos = getSize(keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == X_SIGMA_POS)
        {
            vcObject.x_sigma_pos = getSize(keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == COV_POS)
        {
            vcObject.cov_pos = getSize(keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == X_NAME)
        {
            vcObject.x_name = keyVal[ONE_SIZET];
        }
        else if(keyVal[ZERO_SIZET] == Y_NAME)
        {
            vcObject.y_name = keyVal[ONE_SIZET];
        }
        else if(keyVal[ZERO_SIZET] == X_SIGMA_NAME)
        {
            vcObject.x_sigma_name = keyVal[ONE_SIZET];
        }
        else if(keyVal[ZERO_SIZET] == Y_SIGMA_NAME)
        {
            vcObject.y_sigma_name = keyVal[ONE_SIZET];
        }
        else if(keyVal[ZERO_SIZET] == COV_NAME)
        {
            vcObject.cov_name = keyVal[ONE_SIZET];
        }
    }
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToPVStruct(std::vector<virtualCathodeStructs::pvStruct>& pvs,
                                               const virtualCathodeStructs::VC_PV_TYPE pvtype,
                                               const std::string& pvSuffix)
{
    pvs.push_back(virtualCathodeStructs::pvStruct());
    pvs.back().pvType      = pvtype;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
}

//______________________________________________________________________________
bool virtualCathodeConfigReader::getVirtualCathodeObject(
                virtualCathodeStructs::virtualCathodeObject& obj)
{
    obj = vcObject;
    for(auto&&it:pvMonStructs)
    {
        obj.pvMonStructs[it.pvType] = it;
    }
    for(auto&&it:pvComStructs)
    {
        obj.pvComStructs[it.pvType] = it;
    }
    return true;
}




