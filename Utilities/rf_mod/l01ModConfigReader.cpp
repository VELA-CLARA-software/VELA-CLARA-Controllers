
//djs
#include "l01ModConfigReader.h"
#include "configDefinitions.h"
#include "rfModStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

l01ModConfigReader::l01ModConfigReader(const bool& show_messages_ptr, const  bool& show_debug_messages_ptr )
: configReader(UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr)
{
}
//______________________________________________________________________________
l01ModConfigReader::l01ModConfigReader(const std::string& configFileLocation1,
                                       const bool startVirtualMachine,
                                       const bool& show_messages_ptr,
                                       const bool& show_debug_messages_ptr ):
configReader(configFileLocation1, show_messages_ptr, show_debug_messages_ptr)
{
}
//______________________________________________________________________________
l01ModConfigReader::~l01ModConfigReader(){}
////______________________________________________________________________________
bool l01ModConfigReader::getL01ModObject(rfModStructs::l01ModObject & obj)
{
//    rfModStructs::rfObject obj;
//    obj.name = "VELA_RF_GUN"; /// MAGIC NAME!
//
//    obj.mod = RFModObject;

    obj = RFModObject;

    /// In the final object the pvStructs are in a map keyed by PV Type...

    for(auto && it : pvMonStructs)
        obj.pvMonStructs[ it.pvType ] = it;

    for(auto && it : pvComStructs)
        obj.pvComStructs[ it.pvType ] = it;
#ifdef BUILD_DLL
    for(auto i : obj.modErrorWords)
        obj.modErrorWordsL.append("");

    for(auto i : obj.modErrorDescs)
        obj.modErrorDescsL.append("");
#endif
    return true;
}
////______________________________________________________________________________
bool l01ModConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read L01  Modulator Config File ****");

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
            if(trimmedLine.size() > 0)
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
                                        addToModObjectsV1(keyVal);

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
        message("!!!! Error Can't Open  Gun Modulator Config File after searching in:  ",
                configFile1, " !!!!");
    }
    return success;
    return false;
}
//______________________________________________________________________________
void l01ModConfigReader::addToPVMonitorMapV1(const std::vector<std::string> &keyVal)
{
    addToPVStruct(pvMonStructs, keyVal);
}
//______________________________________________________________________________
void l01ModConfigReader::addToPVCommandMapV1(const std::vector<std::string> &keyVal)
{
    addToPVStruct(pvComStructs, keyVal);
}
//______________________________________________________________________________
void l01ModConfigReader::addToModObjectsV1(const std::vector<std::string> &keyVal)
{
    if(keyVal[0] == UTL::NAME)
    {
        //rfModStructs::rfModObject rfob = rfModStructs::rfModObject();
        RFModObject.name = keyVal[ 1 ];
        debugMessage("Added ", RFModObject.name);
    }
    else if(keyVal[0] == UTL::PV_ROOT)
        RFModObject.pvRoot = keyVal[ 1 ];
}
////______________________________________________________________________________
void l01ModConfigReader::addToPVStruct(std::vector<rfModStructs::l01_pvStruct>& pvStruct_v, const std::vector<std::string> &keyVal)
{
    if(stringIsSubString(keyVal[0], "PV_SUFFIX"))
    {
        pvStruct_v.push_back(rfModStructs::l01_pvStruct());    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        //L01  Modulator
        if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_1 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_1;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_2 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_2;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_3 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_3;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_4 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_4;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_5 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_5;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_6 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_6;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_7 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_7;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_8 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_8;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_9 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_9;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_10 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_10;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_11 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_11;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_12 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_12;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_13 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_13;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_14 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_14;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_15 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_15;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_16 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_16;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_17 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_17;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_18 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_18;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_19 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_19;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_WORD_20 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_20;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_1 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_1;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_2 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_2;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_3 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_3;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_4 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_4;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_5 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_5;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_6 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_6;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_7 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_7;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_8 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_8;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_9 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_9;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_10 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_10;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_11 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_11;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_12 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_12;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_13 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_13;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_14 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_14;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_15 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_15;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_16 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_16;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_17 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_17;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_18 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_18;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_19 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_19;
        else if(keyVal[0] == UTL::PV_SUFFIX_ERROR_DESC_20 )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_20;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_READ )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_READ;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_CURRENT_READ )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_CURRENT_READ;
        else if(keyVal[0] == UTL::PV_SUFFIX_HEATER_VOLTAGE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HEATER_VOLTAGE;
        else if(keyVal[0] == UTL::PV_SUFFIX_HEATER_CURRENT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HEATER_CURRENT;
        else if(keyVal[0] == UTL::PV_SUFFIX_RESET_VOLTAGE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::RESET_VOLTAGE;
        else if(keyVal[0] == UTL::PV_SUFFIX_RESET_CURRENT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::RESET_CURRENT;
        else if(keyVal[0] == UTL::PV_SUFFIX_ION_PUMP_VOLTAGE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ION_PUMP_VOLTAGE;
        else if(keyVal[0] == UTL::PV_SUFFIX_ION_PUMP_CURRENT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::ION_PUMP_CURRENT;
        else if(keyVal[0] == UTL::PV_SUFFIX_SUPPLY_TEMP )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SUPPLY_TEMP;
        else if(keyVal[0] == UTL::PV_SUFFIX_SUPPLY_PRESSURE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SUPPLY_PRESSURE;
        else if(keyVal[0] == UTL::PV_SUFFIX_RETURN_TEMP )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::RETURN_TEMP;
        else if(keyVal[0] == UTL::PV_SUFFIX_RETURN_PRESSURE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::RETURN_PRESSURE;
        else if(keyVal[0] == UTL::PV_SUFFIX_BODY_FLOW_RATE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::BODY_FLOW_RATE;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_FLOW_RATE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_FLOW_RATE;
        else if(keyVal[0] == UTL::PV_SUFFIX_TANK_FLOW_RATE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::TANK_FLOW_RATE;
        else if(keyVal[0] == UTL::PV_SUFFIX_COLLECTOR_RETURN_RATE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::COLLECTOR_RETURN_RATE;
        else if(keyVal[0] == UTL::PV_SUFFIX_BODY_RETURN_TEMPERATURE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::BODY_RETURN_TEMPERATURE;
        else if(keyVal[0] == UTL::PV_SUFFIX_COLLECTOR_FLOW_RATE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::COLLECTOR_FLOW_RATE;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_1_VOLTAGE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_1_VOLTAGE;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_2_VOLTAGE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_2_VOLTAGE;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_3_VOLTAGE )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_3_VOLTAGE;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_1_CURRENT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_1_CURRENT;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_2_CURRENT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_2_CURRENT;
        else if(keyVal[0] == UTL::PV_SUFFIX_SOLENOID_3_CURRENT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SOLENOID_3_CURRENT;
        else if(keyVal[0] == UTL::PV_SUFFIX_SYSTEM_STATE_READ )
        {
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ;
            debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvStruct_v.back().pvType));
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_SET_READ )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_SET_READ;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_LOW_ALARM_SET_READ )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_LOW_ALARM_SET_READ;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_HI_ALARM_SET_READ )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_HI_ALARM_SET_READ;
        else if(keyVal[0] == UTL::PV_SUFFIX_L01_FAULT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::L01_FAULT;
        else if(keyVal[0] == UTL::PV_SUFFIX_SYSTEM_STATE_PUT )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_PUT;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_SET )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_SET;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_LOW_ALARM_SET )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_LOW_ALARM_SET;
        else if(keyVal[0] == UTL::PV_SUFFIX_HVPS_VOLTAGE_HI_ALARM_SET )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_HI_ALARM_SET;
        else if(keyVal[0] == UTL::PV_SUFFIX_L01_RESET )
            pvStruct_v.back().pvType = rfModStructs::L01_MOD_PV_TYPE::L01_RESET;
        else
        {
            message("CONFIG FILE ERROR ", keyVal[0],
                     " is not recognised BE WARY, PROCEED WITH CAUTION. FIX source of config file.");
        }
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvStruct_v, keyVal);

    debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ",
                 ENUM_TO_STRING(pvStruct_v.back().pvType)) ;
}
//______________________________________________________________________________
void l01ModConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<rfModStructs::l01_pvStruct>& pvStruct_v,
                                                 const std::vector<std::string> &keyVal)
{
    if(keyVal[0] == UTL::PV_COUNT)
        pvStruct_v.back().COUNT = getCOUNT(keyVal[1]);
    else if(keyVal[0] == UTL::PV_MASK)
        pvStruct_v.back().MASK = getMASK(keyVal[1]);
    else if(keyVal[0] == UTL::PV_CHTYPE)
        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[1]);
}
//______________________________________________________________________________

