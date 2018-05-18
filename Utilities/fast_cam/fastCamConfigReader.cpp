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
configReader(velaConfig, claraConfig, show_messages, show_debug_messages, startVirtualMachine)
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

    bool readingVela  = true;
    bool readingClara = false;
    debugMessage("**** Attempting to read VELA fast Cams ****","\n");
    bool readVELA = readConfig(configFile1);

    readingVela  = false;
    readingClara = true;
    debugMessage("**** Attempting to read CLARA fast Cams ****","\n");
    bool readCLARA = readConfig(configFile2);

    if(readVELA && readCLARA)
        return true;
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
                                    else if (readingCommandPVs)
                                    {
                                        addToPVStruct(pvFCamComStructs,keyVal);
                                    }

                                    else if (readingMonitorPVs)
                                    {
                                        addToPVStruct(pvFCamMonStructs, keyVal);
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
bool fastCamConfigReader::getCamData(std::map<std::string, fastCamStructs::fastCamObject> & mapToFill)
{

    return false;
}

//______________________________________________________________________________
void fastCamConfigReader::addToPVStruct(std::vector<fastCamStructs::pvStruct>& pvStruct_v,
                                        const std::vector<std::string> &keyVal)
{
//    if(stringIsSubString(keyVal[0], "SUFFIX"))
//    {
//        pvStruct_v.push_back(magnetStructs::pvStruct());    /// Any way to avoid the ladders?
//        pvStruct_v.back().pvSuffix = keyVal[1];
//        //GUN  Protection
//        if(keyVal[0] == UTL::PV_SUFFIX_SPOWER)
//            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::SPOWER;
//
//        else if(keyVal[0] == UTL::PV_SUFFIX_RPOWER)
//            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::RPOWER;
//
//        else if(keyVal[0] == UTL::PV_SUFFIX_READI)
//            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::READI;
//
//        else if(keyVal[0] == UTL::PV_SUFFIX_SETI)
//            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::SETI;
//
//        else if(keyVal[0] == UTL::PV_SUFFIX_RILK)
//            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::RILK;
//
//        else if(keyVal[0] == UTL::PV_SUFFIX_GETSETI)
//            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::GETSETI;
//
//         debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvStruct_v.back().pvType)) ;
//    }
//    else
//        addCOUNT_MASK_OR_CHTYPE(pvStruct_v, keyVal);
}
//______________________________________________________________________________
void fastCamConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<fastCamStructs::pvStruct>& pvStruct_v,
                                                  const std::vector<std::string>&keyVal)
{
//    if(keyVal[0] == UTL::PV_COUNT)
//        pvStruct_v.back().COUNT = getCOUNT(keyVal[ 1 ]);
//    else if(keyVal[0] == UTL::PV_MASK)
//        pvStruct_v.back().MASK = getMASK(keyVal[ 1 ]);
//    else if(keyVal[0] == UTL::PV_CHTYPE)
//        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[ 1 ]);
}
//______________________________________________________________________________
void fastCamConfigReader::addToObjectsV1(const std::vector<std::string> &keyVal)
{

}



