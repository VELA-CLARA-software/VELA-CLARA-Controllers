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
//  FileName:    velaCameraConfigReader.cpp
//  Description: reads config for velacams, this is entirely
//               based on previous versions from 2015 (and earlier)
//
//
//*/
// project
#include "velaCameraConfigReader.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <fstream>
#include <sstream>
//##include <algorithm>
//#include <ctype.h>

velaCameraConfigReader::velaCameraConfigReader(const std::string& configFile,
                                         const bool& show_messages,
                                         const bool& show_debug_messages,
                                         const bool usingVM):
configReader(configFile,  show_messages, show_debug_messages,usingVM)
{
    //ctor
}
//---------------------------------------------------------------------------------
velaCameraConfigReader::~velaCameraConfigReader()
{
    //dtor
}
//---------------------------------------------------------------------------------
bool velaCameraConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read VELA camera Config File ****");

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
            if(trimmedLine.size()> UTL::ZERO_SIZET)
            {
                if(stringIsSubString(line, UTL::END_OF_DATA))
                {
                    if(numObjs == camObjects.size())
                        message("Found ", camObjects.size() , " objects, expetced ",numObjs);
                    else
                        message("!!ERROR!! Found ", camObjects.size() , " objects, expetced ",numObjs);

                    debugMessage("\nFound END_OF_DATA\n");
                    debugMessage("Number of Monitors = ",pvMonStructs.size());
                    debugMessage("Number of Commands = ",pvComStructs.size());
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
                                        addToCameraObjects(keyVal);

                                    else if (readingCommandPVs)
                                        addToPVCommandMapV1(keyVal);

                                    else if (readingMonitorPVs)
                                        addToPVMonitorMapV1(keyVal);
                                }
                                break;
                            default:
                                message("!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!" );
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
                    debugMessage("\nFound PV_COMMANDS_START\n");
                }
                if(stringIsSubString(line, UTL::PV_MONITORS_START))
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage("\nFound PV_MONITORS_START\n");
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
        message("!!!! Error Can't Open PILaser Config File after searching in:  ", configFile1, " !!!!" );
    }
    return success;
}
//---------------------------------------------------------------------------------------
void velaCameraConfigReader::addToPVMonitorMapV1(const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX"))
    {
        using namespace cameraStructs;
        if(keyVal[ZERO_SIZET] == PV_SUFFIX_GAIN_RBV)//1
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::GAINRAW_RBV, keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_SUFFIX_BLACK_LEVEL_RBV)//1
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::BLACKLEVEL_RBV, keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_ACQUIRE_RBV)//1
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_ACQUIRE_RBV, keyVal[ONE_SIZET]);
        }
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_LED_STA)//9
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::VELA_LED_STA,keyVal[ONE_SIZET]);
        }

    }
    else
    {
        addCOUNT_MASK_OR_CHTYPE(pvMonStructs, keyVal);
    }
}
//---------------------------------------------------------------------------------------
void velaCameraConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<cameraStructs::pvStruct>& pvStruct_v,
                                                  const std::vector<std::string>& keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        pvStruct_v.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
        pvStruct_v.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
}
//---------------------------------------------------------------------------------------
void velaCameraConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal )
{
    using namespace UTL;
    //message("addToPVCommandMapV1, ", keyVal[0]);
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX"))
    {
        using namespace cameraStructs;
        if(keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_START_ACQUIRE )//1
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_START_ACQUIRE, keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_STOP_ACQUIRE )//1
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_STOP_ACQUIRE, keyVal[ONE_SIZET]);
        }
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_DATA)//9
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::ARRAY_DATA, keyVal[ONE_SIZET]);
        }
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BLACK_LEVEL)//9
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::Blacklevel, keyVal[ONE_SIZET]);
        }
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_GAIN)//9
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::GAINRAW, keyVal[ONE_SIZET]);
        }
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_LED_OFF)//9
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::VELA_LED_OFF, keyVal[ONE_SIZET]);
        }
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_LED_ON)//9
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::VELA_LED_ON, keyVal[ONE_SIZET]);
        }

    }
    else
    {
        addCOUNT_MASK_OR_CHTYPE(pvComStructs, keyVal);
    }
}
//______________________________________________________________________________
void velaCameraConfigReader::addPVStruct(std::vector<cameraStructs::pvStruct>& pvs,
                                      cameraStructs::CAM_PV_TYPE pvType,
                                      const std::string& pvSuffix)
{
    pvs.push_back(cameraStructs::pvStruct());
    pvs.back().pvType      = pvType;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    message(pvs.size(),": Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
}
//---------------------------------------------------------------------------------------
bool velaCameraConfigReader::getCamData(std::map<std::string, cameraStructs::cameraObject>& mapToFill,
                                        const bool no_vc)
{
    bool success = true;
    bool addcam;
    //mapToFill.clear();
    for(auto&& it:camObjects)
    {
        /*
            no_vc is a flag to remove or keep the VIRTUAL_CATHODE
            there are two physical virtual cathodes, ( clara and vela lines)
            however you can only choose one for a camera controller
        */
        addcam = true;
        if(no_vc)
        {
            if(it.screenName == UTL::VIRTUAL_CATHODE)
            {
                addcam = false;
            }
            if(it.name == UTL::VIRTUAL_CATHODE)
            {
                addcam = false;
            }
        }
        if(addcam)
        {
            mapToFill[it.name] = it;
            for(auto&& it2 : pvMonStructs)
                mapToFill.at(it.name).pvMonStructs[ it2.pvType ] = it2;

            for(auto&& it2 : pvComStructs)
                mapToFill.at(it.name).pvComStructs[ it2.pvType ] = it2;
        }
    }


    return success;
}
//---------------------------------------------------------------------------------------
void velaCameraConfigReader::addToCameraObjects(const std::vector<std::string> & keyVal)
{
    using namespace cameraStructs;
    using namespace UTL;
    std::string value = keyVal[ONE_SIZET];//MAGIC_NUMBER
    if(keyVal[ZERO_SIZET] == NAME)
    {
        camObjects.push_back(cameraObject());
        camObjects.back().name = value;
       // camDAQObject.back().numIlocks = (size_t)numIlocks;
        debugMessage("Added camera ", camObjects.back().name);
    }
    else if(keyVal[ZERO_SIZET] == UTL::CAM_TYPE)
    {
        camObjects.back().type = getCameType(keyVal[ONE_SIZET]);
        //message(camObjects.back().name, " is a ", ENUM_TO_STRING(camObjects.back().type));
    }
    else if(keyVal[ZERO_SIZET] == PV_ROOT)
    {
        if(useVM)
            camObjects.back().pvRoot = VM_PREFIX + value;
        else
            camObjects.back().pvRoot = value;
    }
    else if(keyVal[ZERO_SIZET] == SCREEN_NAME)
    {
        if(useVM)
            camObjects.back().screenName = VM_PREFIX + value;
        else
            camObjects.back().screenName = value;
    }
    else if(keyVal[ZERO_SIZET] == X_MASK_RAD_DEF)
    {
        camObjects.back().data.mask.mask_x_rad_def = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == Y_MASK_RAD_DEF)
    {
        camObjects.back().data.mask.mask_y_rad_def = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == X_MASK_DEF)
    {
        camObjects.back().data.mask.mask_x_def   = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == Y_MASK_DEF)
    {
        camObjects.back().data.mask.mask_y_def = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == X_CENTER_DEF)
    {
        camObjects.back().data.analysis.x_centre_def  = getSize(value);
    }
    else if(keyVal[ZERO_SIZET] == Y_CENTER_DEF)
    {
        camObjects.back().data.analysis.x_centre_def  = getSize(value);
    }
    else if(keyVal[ZERO_SIZET] == PIX_2_MM_RATIO_DEF)
    {
        camObjects.back().data.analysis.pix_2_mm_def  = getNumD(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_NUM_PIX_X)
    {
        camObjects.back().data.image.num_pix_x  = getSize(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_NUM_PIX_Y)
    {
        camObjects.back().data.image.num_pix_y = getSize(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_X_PIX_2_MM)
    {
        camObjects.back().data.image.x_pix_to_mm  = getSize(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_Y_PIX_2_MM)
    {
        camObjects.back().data.image.y_pix_to_mm = getSize(value);
    }
    else if(keyVal[ZERO_SIZET] == IP_ADDRESS_STREAM)
    {
        camObjects.back().streamingIPAddress = value;
    }
    else if(keyVal[ZERO_SIZET] == MAX_SHOTS_NUMBER)
    {
        camObjects.back().daq.maxShots = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == X)
    {
        camObjects.back().data.analysis.pixel_values_pos[getSize(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == Y)
    {
        camObjects.back().data.analysis.pixel_values_pos[getSize(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == X_SIGMA)
    {
        camObjects.back().data.analysis.pixel_values_pos[getSize(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == Y_SIGMA)
    {
        camObjects.back().data.analysis.pixel_values_pos[getSize(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == XY_SIGMA)
    {
        camObjects.back().data.analysis.pixel_values_pos[getSize(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == UTL::X_PIX_SCALE_FACTOR)
    {
        camObjects.back().data.image.x_pix_scale_factor = getSize(keyVal[ZERO_SIZET]);
    }
    else if(keyVal[ZERO_SIZET] == UTL::Y_PIX_SCALE_FACTOR)
    {
        camObjects.back().data.image.y_pix_scale_factor = getSize(keyVal[ZERO_SIZET]);
    }
}
//______________________________________________________________________________
cameraStructs::CAM_TYPE velaCameraConfigReader::getCameType(const std::string& value)
{
    if(value == ENUM_TO_STRING(cameraStructs::CAM_TYPE::VELA_CAM))
       return cameraStructs::CAM_TYPE::VELA_CAM;
    if(value == ENUM_TO_STRING(cameraStructs::CAM_TYPE::CLARA_CAM))
        return cameraStructs::CAM_TYPE::CLARA_CAM;
    return cameraStructs::CAM_TYPE::UNKNOWN_CAM;
}
