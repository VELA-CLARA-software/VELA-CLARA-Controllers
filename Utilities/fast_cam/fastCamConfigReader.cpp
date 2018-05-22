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
//  Last edit:   17-05-2018
//  FileName:    fastCamConfigReader.h
//  Description:
//
//
//
//*/
// project includes
#include "fastCamConfigReader.h"
// stl includes
#include <fstream>

//______________________________________________________________________________
fastCamConfigReader::fastCamConfigReader(const std::string& velaConfig,
                                         const std::string& claraConfig,
                                         const bool startVirtualMachine,
                                         const bool& show_messages,
                                         const bool& show_debug_messages):
configReader(velaConfig, claraConfig, show_messages, show_debug_messages, startVirtualMachine),
readingVela(false),
readingClara(false)
{
    //ctor
}
//______________________________________________________________________________
fastCamConfigReader::~fastCamConfigReader()
{
    //dtor
}
//______________________________________________________________________________
bool fastCamConfigReader::readConfig()
{
    if(useVM)
        debugMessage("\n", "**** Using VIRTUAL Machine  ****");
    else
        debugMessage("**** Using PHYSICAL Machine  ****","\n");

    readingVela  = true;
    readingClara = false;

    bool readVELA = false;
    bool shouldReadVELA = true;

    if(configFile1 !=  UTL::UNKNOWN_STRING )
    {
        debugMessage("**** Attempting to read VELA fast Cams ****","\n");
        readVELA = readConfig(configFile1);
    }
    else
    {
        shouldReadVELA = false;
    }

    readingVela  = false;
    readingClara = true;
    bool readCLARA = false;
    bool shouldReadCLARA = true;
    if(configFile2 !=  UTL::UNKNOWN_STRING )
    {
        debugMessage("**** Attempting to read CLARA fast Cams ****","\n");
        readCLARA = readConfig(configFile2);
    }
    else
    {
        shouldReadCLARA = false;
    }
    /*
        What to return?
    */
    if(shouldReadVELA && shouldReadCLARA )
    {
        if(readVELA && readCLARA )
            return true;
        else
            return false;
    }
    if(shouldReadVELA && !shouldReadCLARA )
    {
        if(readVELA)
            return true;
        else
            return false;
    }
    if(!shouldReadVELA && shouldReadCLARA )
    {
        if(readCLARA)
            return true;
        else
            return false;
    }
    // something unexpected
    return false;
}
//______________________________________________________________________________
bool fastCamConfigReader::readConfig(const std::string& file)
{
    std::string line, trimmedLine;
    bool success = false;

    std::ifstream inputFile;

    inputFile.open(file, std::ios::in);
    if(inputFile)
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage("Opened ", file);
        while(std::getline(inputFile, line)) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
            //trimmedLine = trimAllWhiteSpaceExceptBetweenDoubleQuotes(trimToDelimiter(line, UTL::END_OF_LINE));
            if(trimmedLine.size()> UTL::ZERO_SIZET)
            {

                //message("trimmedLine = ", trimmedLine);

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
                        switch(configVersion) /// Actually this switch needs to come out of here and go in the beginning, we should look for version # first!
                        {
                            case 1:
                                if(trimmedLine.find_first_of(UTL::EQUALS_SIGN) != std::string::npos)
                                {
                                    std::vector<std::string> keyVal = getKeyVal(trimmedLine);

                                    if(readingObjs)
                                    {
                                        addToObjectsV1(keyVal);
                                    }
                                    else if(readingCommandPVs)
                                    {
                                        if( readingVela )
                                        {
                                            addToPVStruct(pvFCamComStructs_VELA,keyVal);
                                        }
                                        else if(readingClara)
                                        {
                                            addToPVStruct(pvFCamComStructs_CLARA,keyVal);
                                        }
                                    }
                                    else if(readingMonitorPVs)
                                    {
                                        if( readingVela )
                                        {
                                            addToPVStruct(pvFCamMonStructs_VELA,keyVal);
                                        }
                                        else if(readingClara)
                                        {
                                            addToPVStruct(pvFCamMonStructs_CLARA,keyVal);
                                        }
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
        debugMessage("config File Closed");

//        if(numObjs == magObjects.size()) // MAY HVE BROKE THINGS HERE!
//        {
            success = true;
//            message("config file reader returning true");
//        }
//        else
//             message("config file reader returning false");
    }
    else{
        message("!!!! Error Can't Open Config File after searching for:  ", file, " !!!!");
    }
    if(success)
        debugMessage("fastCamConfigReader readconfig is returning TRUE");
    else
        debugMessage("fastCamConfigReader readconfig is returning FALSE");


    return success;
}
//______________________________________________________________________________
bool fastCamConfigReader::getCamData(std::map<std::string, fastCamStructs::fastCamObject>& mapToFill)
{
    for(auto&&it:fCamObjects)
    {
        mapToFill[it.name] = it;
    }
    for(auto&&it:mapToFill)
    {
        if(it.second.type == fastCamStructs::CAM_TYPE::CLARA)
        {
            for(auto&&it2:pvFCamMonStructs_CLARA)
            {
                message(ENUM_TO_STRING(it2.pvType));
                it.second.pvMonStructs[it2.pvType] = it2;
            }
            for(auto&&it2:pvFCamComStructs_CLARA)
            {
                message(ENUM_TO_STRING(it2.pvType));
                it.second.pvComStructs[it2.pvType] = it2;
            }
        }
        else if(it.second.type == fastCamStructs::CAM_TYPE::VELA)
        {
            for(auto&&it2:pvFCamMonStructs_VELA)
            {
                message(ENUM_TO_STRING(it2.pvType));
                it.second.pvMonStructs[it2.pvType] = it2;
            }
            for(auto&&it2:pvFCamComStructs_VELA)
            {
                message(ENUM_TO_STRING(it2.pvType));
                it.second.pvComStructs[it2.pvType] = it2;
            }
        }
        else
            message("ERROR expected camera type of VELA or CLARA for ",it.second.name);
    }
    return true;
}

//______________________________________________________________________________
void fastCamConfigReader::addToPVStruct(std::vector<fastCamStructs::pvStruct>& pvStruct_v,
                                        const std::vector<std::string> &keyVal)
{
    if(stringIsSubString(keyVal[UTL::ZERO_SIZET], "SUFFIX"))
    {
        pvStruct_v.push_back(fastCamStructs::pvStruct());    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[UTL::ONE_SIZET];

        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_START)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::START;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_STOP)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::STOP;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_GAIN)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::GAIN;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BLACK)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::BLACK_LEVEL;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_DATA)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::DATA;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_GAIN_RBV)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::GAIN_RBV;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_BLACK_RBV)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::BLACK_LEVEL_RBV;
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_CAM_STATE)
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::STATE;

        else if( keyVal[0] == UTL::PV_IA_SUFFIX_X  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::X;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_Y  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::Y;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_SIGMA_X  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::SIGMA_X;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_SIGMA_Y  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::SIGMA_Y;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_COV_XY  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::COV_XY;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_X_PIX  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::X_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_Y_PIX  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::Y_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_SIGMA_X_PIX  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::SIGMA_X_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_SIGMA_Y_PIX  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::SIGMA_Y_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_COV_XY_PIX  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::COV_XY_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_AV_PIX_INTENSITY  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::AVG_PIX_INETSITY;

        else if( keyVal[0] == UTL::PV_IA_SUFFIX_START_IA  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::START_IA;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CENTER_X  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::X_CENTER;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CENTER_Y  )
        {
            message("Added Y_CENTER in config");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::Y_CENTER;
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CENTER_X_RBV  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::X_CENTER_RBV;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CENTER_Y_RBV  )
        {
            message("Added Y_CENTER in config");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::Y_CENTER_RBV;
        }

        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_X_RBV  )
        {
            message("Added MASK_X_RBV in config");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_X_RBV;
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_Y_RBV  )
        {
            message("Added MASK_Y_RBV in config");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_Y_RBV;
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_X_RAD_RBV  )
        {
            message("Added MASK_X_RAD_RBV in config");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_X_RAD_RBV;
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_Y_RAD_RBV  )
        {
            message("Added MASK_Y_RAD_RBV in config");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_Y_RAD_RBV;
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_X  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_X;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_Y  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_Y;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_X_RAD  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_X_RAD;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_MASK_Y_RAD  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::MASK_Y_RAD;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_PIX_MM  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::PIX_MM;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_STEP_SIZE  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::STEP_SIZE;

        else if( keyVal[0] == UTL::PV_IA_SUFFIX_SET_B  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::SET_BKGRND;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_B  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::USE_BKGRND;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_NPOINT  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::USE_NPOINT;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_START_ACQUIRE  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::CAM_START_ACQUIRE;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_STOP_ACQUIRE  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::CAM_STOP_ACQUIRE;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_ACQUIRE_RBV  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::CAM_ACQUIRE_RBV;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_STATE  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::CAM_STATUS;

        else if( keyVal[0] == UTL::PV_IA_SUFFIX_START_IA_RBV  ){
            //message("HELLO");
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::START_IA_RBV;
        }

        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_B_RBV  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::USE_BKGRND_RBV;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_NPOINT_RBV  )
            pvStruct_v.back().pvType = fastCamStructs::FAST_CAM_PV::USE_NPOINT_RBV;


        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvStruct_v.back().pvType)) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvStruct_v, keyVal);
}
//______________________________________________________________________________
void fastCamConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<fastCamStructs::pvStruct>& pvStruct_v,
                                                  const std::vector<std::string>&keyVal)
{
    if(keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        pvStruct_v.back().COUNT = getCOUNT(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK)
        pvStruct_v.back().MASK = getMASK(keyVal[UTL::ONE_SIZET]);
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE)
        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[UTL::ONE_SIZET]);
}
//______________________________________________________________________________
void fastCamConfigReader::addToObjectsV1(const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    if(keyVal[ZERO_SIZET] == NAME )
    {
        fCamObjects.push_back(fastCamStructs::fastCamObject());    /// Any way to avoid the ladders?
        fCamObjects.back().name      = keyVal[ONE_SIZET];
        debugMessage("Added ", fCamObjects.back().name);
    }
    else if(keyVal[ZERO_SIZET] == PV_ROOT)
    {
        if(useVM )
            fCamObjects.back().pvRoot =  VM_PREFIX + keyVal[ONE_SIZET];
        else
            fCamObjects.back().pvRoot = keyVal[ONE_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == CAM_TYPE)
    {
        fCamObjects.back().type = getCamType(keyVal[ONE_SIZET]);
    }
    else if(keyVal[ZERO_SIZET] == SCREEN)
    {
        fCamObjects.back().screen = keyVal[ONE_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == NUMBER_OF_PIXELS_X)
    {
        fCamObjects.back().num_pix_x = getSize(keyVal[ONE_SIZET]);
    }
    else if(keyVal[ZERO_SIZET] == NUMBER_OF_PIXELS_Y)
    {
        fCamObjects.back().num_pix_y = getSize(keyVal[ONE_SIZET]);
    }
    else if(keyVal[ZERO_SIZET] == X_PIX_2_MM)
    {
        fCamObjects.back().x_pix_to_mm = getNumD(keyVal[ONE_SIZET]);
    }
    else if(keyVal[ZERO_SIZET] == Y_PIX_2_MM)
    {
        fCamObjects.back().y_pix_to_mm = getNumD(keyVal[ONE_SIZET]);
    }
}
//______________________________________________________________________________
fastCamStructs::CAM_TYPE fastCamConfigReader::getCamType(const std::string& cam)
{
    if(cam == ENUM_TO_STRING(fastCamStructs::CAM_TYPE::VELA))
    {
        return fastCamStructs::CAM_TYPE::VELA;
    }
    else if(cam == ENUM_TO_STRING(fastCamStructs::CAM_TYPE::CLARA))
    {
        return fastCamStructs::CAM_TYPE::CLARA;
    }
    return fastCamStructs::CAM_TYPE::NOT_KNOWN;
}




