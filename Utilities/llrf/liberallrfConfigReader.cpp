//djs
#include "liberallrfConfigReader.h"
#include "llrfStructs.h"
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
liberallrfConfigReader::liberallrfConfigReader(const std::string& configFileLocation1,
                                        const bool startVirtualMachine,
                                        const bool& show_messages,
                                        const bool& show_debug_messages_ptr):
configReader(configFileLocation1, show_messages, show_debug_messages_ptr),
usingVirtualMachine(startVirtualMachine),
lastPVstruct(nullptr),
type(llrfStructs::LLRF_TYPE::UNKNOWN_TYPE),
readingMainRF(false),
readingOneTrace(false),
readingTraceSCAN(false),
temp_name("temp_name"),
temp_pvroot("temp_pvroot")
{
//    std::cout << configFileLocation1 << std::endl;
}
//______________________________________________________________________________
liberallrfConfigReader::~liberallrfConfigReader(){}
//______________________________________________________________________________
void liberallrfConfigReader::setType(const llrfStructs::LLRF_TYPE typein)
{
    type = typein;
}
//______________________________________________________________________________
bool liberallrfConfigReader::readConfig()
{
    /*
        This is the main function that reads in the config files,
        There are three, and which file depends on which RF structure i sbeing used
    */
    bool success = false;

    pvComStructs.clear();
    pvMonStructs.clear();
    pvOneTraceMonStructs.clear();
    pvOneTraceComStructs.clear();
    pvSCANMonStructs.clear();
    pvSCANComStructs.clear();


    debugMessage("\n", "**** Attempting to Read The RF Config File ****");
    readingMainRF = true;
    readingOneTrace = false;
    readingTraceSCAN = false;
    if( readConfig(configFile1) );
    {
        readingMainRF = false;
        readingOneTrace = true;
        readingTraceSCAN = false;

        if( getOneTraceConfig() )
        {
            readingMainRF = false;
            readingOneTrace = false;
            readingTraceSCAN = true;

            if( getSCANConfig() )
            {
                success = true;
            }
        }
    }
    return success;
}
//______________________________________________________________________________
bool liberallrfConfigReader::getOneTraceConfig()
{
    debugMessage("\n", "**** Attempting to Read The LLRF One-Trace Config File ****");
    switch(type)
    {
        case llrfStructs::LLRF_TYPE::CLARA_HRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_ONE_RECORD);
            break;
        case llrfStructs::LLRF_TYPE::CLARA_LRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_ONE_RECORD);
            break;
        case llrfStructs::LLRF_TYPE::VELA_HRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_ONE_RECORD);
            break;
        case llrfStructs::LLRF_TYPE::VELA_LRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_ONE_RECORD);
            break;
        case llrfStructs::LLRF_TYPE::L01:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::L01_LLRF_TRACES_ONE_RECORD);
            break;
        case llrfStructs::LLRF_TYPE::UNKNOWN_TYPE:
            return false;
            break;
        default:
            return false;
    }
    return true;
}
//______________________________________________________________________________
bool liberallrfConfigReader::getSCANConfig()
{
    debugMessage("\n", "**** Attempting to Read The LLRF SCAN Config File ****");
    switch(type)
    {
        case llrfStructs::LLRF_TYPE::CLARA_HRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_SCAN);
            break;
        case llrfStructs::LLRF_TYPE::CLARA_LRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_SCAN);
            break;
        case llrfStructs::LLRF_TYPE::VELA_HRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_SCAN);
            break;
        case llrfStructs::LLRF_TYPE::VELA_LRRG:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::GUN_LLRF_TRACES_SCAN);
            break;
        case llrfStructs::LLRF_TYPE::L01:
            readConfig(UTL::CLARANET_CONFIG_PATH + UTL::L01_LLRF_TRACES_SCAN);
            break;
        case llrfStructs::LLRF_TYPE::UNKNOWN_TYPE:
            return false;
            break;
        default:
            return false;
    }
    return true;
}
//______________________________________________________________________________
bool liberallrfConfigReader::readConfig(const std::string & configFile1)
{

    std::string line, trimmedLine;
    bool success = false;

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
                                        addToliberallrfObjectsV1(keyVal);

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
        success = true;
    }
    else{
        message("!!!! Error Can't Open Config File after searching in:  ", configFile1, " !!!!");
    }
    if(success)
        debugMessage("readConfig returning true");
    else
        debugMessage("readConfig returning false");
    return success;
}
//______________________________________________________________________________
void liberallrfConfigReader::addToPVMonitorMapV1(const std::vector<std::string>& keyVal)
{
    if(readingMainRF)
    {
        addToPVMapV1(pvMonStructs,keyVal);
    }
    else if(readingOneTrace)
    {
        addToPVMapV1(pvOneTraceMonStructs,keyVal);
    }
    else if(readingTraceSCAN)
    {
        addToPVMapV1(pvSCANMonStructs,keyVal);
    }
}
//______________________________________________________________________________
void liberallrfConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal)
{
    if(readingMainRF)
    {
        addToPVMapV1(pvComStructs,keyVal);
    }
    else if(readingOneTrace)
    {
        addToPVMapV1(pvOneTraceComStructs,keyVal);
    }
    else if(readingTraceSCAN)
    {
        addToPVMapV1(pvSCANComStructs,keyVal);
    }
}
//______________________________________________________________________________


void liberallrfConfigReader::addToPVMapV1(std::vector<llrfStructs::pvStruct>& pvs, const std::vector<std::string>& keyVal)
{// here i've hardcoded in the monitor or command PVs

    using namespace UTL;
    using namespace llrfStructs;
    std::string key = keyVal[ZERO_SIZET];
    std::string val = keyVal[ONE_SIZET];

    message("addToPVMapV1 passed Key = ", key, ", value = ", val);


    if(stringIsSubString(key,"SUFFIX"))
    {
        message("stringIsSubString = true", key);

        if(key == PV_SUFFIX_LIB_AMP_FF)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_AMP_FF, val);
        }
        else if(key == PV_SUFFIX_LIB_AMP_SP)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_AMP_SP,val);
        }
        else if(key == PV_SUFFIX_LIB_PHI_FF)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PHI_FF,val);
        }
        else if(key == PV_SUFFIX_LIB_PHI_SP)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PHI_SP,val);
        }
        else if(key == PV_SUFFIX_LIB_PULSE_LENGTH)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PULSE_LENGTH,val);
        }

        else if(key == PV_SUFFIX_LIB_PULSE_SHAPE)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::PULSE_SHAPE,val);
        }
        else if(key == PV_SUFFIX_LIB_PULSE_SHAPE_APPLY)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::PULSE_SHAPE_APPLY,val);
        }

        else if(key == PV_SUFFIX_LIB_MAX_AMP_SP)
        {
            message("PV_SUFFIX_LIB_MAX_AMP_SP");
            addToPVStruct(pvs, LLRF_PV_TYPE::LLRF_MAX_AMP_SP,val);
        }
        else if(key == PV_SUFFIX_LIB_KEEP_ALIVE)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::KEEP_ALIVE,val);
        }
        else if(key == PV_SUFFIX_LIB_PULSE_OFFSET)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PULSE_OFFSET,val);
        }
        else if(key == PV_SUFFIX_FF_AMP_LOCK_STATE)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE,val);
        }
        else if(key == PV_SUFFIX_FF_PHASE_LOCK_STATE)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE,val);
        }
        else if(key == PV_SUFFIX_LIB_TIME_VECTOR)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_TIME_VECTOR, val);
        }
        else if(key == PV_SUFFIX_LIB_RF_OUTPUT)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_RF_OUTPUT, val);
        }
        else if(key == PV_SUFFIX_TRIG_SOURCE)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::TRIG_SOURCE, val);
        }
//        else if(key == PV_SUFFIX_TIMING)
//        {
//            addToPVStruct(pvs, LLRF_PV_TYPE::TIMING_TRIGGER, val);
//        }
        // individual trace scans (needed so we can turn updating off)
        else if(key == PV_SUFFIX_PWR_REM_SCAN)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PWR_REM_SCAN, val);
        }
        else if(key == PV_SUFFIX_PHASE_REM_SCAN)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PHASE_REM_SCAN, val);
        }
        else if(key == PV_SUFFIX_AMP_DER_SCAN)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_AMP_DER_SCAN, val);
        }
        else if(key == PV_SUFFIX_PHASE_DER_SCAN)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PHASE_DER_SCAN, val);
        }
        else if(key == PV_SUFFIX_PWR_LOC_SCAN)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN, val);
        }
        // all traces in one record PVs
        else if(key == PV_SUFFIX_LLRF_TRACES_SCAN)
        {
            message(PV_SUFFIX_LLRF_TRACES_SCAN, " =|", val,"|");
            addToPVStruct(pvs, LLRF_PV_TYPE::ALL_TRACES_SCAN, val);
        }
//        else if(key == UTL::PV_SUFFIX_LLRF_TRACES_EVID)
//        {
//            addToPVStruct(pvs, LLRF_PV_TYPE::ALL_TRACES_EVID, val);
//        }
        else if(key == PV_SUFFIX_LLRF_TRACES_ACQM)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::ALL_TRACES_ACQM, val);
        }
        else if(key == PV_SUFFIX_LLRF_TRACES)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::ALL_TRACES, val);
        }
        else if(key == PV_SUFFIX_INTERLOCK)
        {
            addToPVStruct(pvs, LLRF_PV_TYPE::INTERLOCK, val);
        }
        else if(key == PV_SUFFIX_CH1_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH1_INTERLOCK_STATUS , val); }
        else if(key == PV_SUFFIX_CH1_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH1_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH1_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH1_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH1_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH1_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH1_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH1_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH2_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH2_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH2_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH2_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH2_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH2_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH2_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH2_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH2_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH2_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH3_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH3_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH3_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH3_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH3_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH3_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH3_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH3_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH3_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH3_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH4_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH4_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH4_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH4_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH4_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH4_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH4_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH4_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH4_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH4_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH5_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH5_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH5_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH5_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH5_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH5_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH5_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH5_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH5_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH5_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH6_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH6_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH6_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH6_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH6_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH6_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH6_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH6_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH6_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH6_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH7_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH7_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH7_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH7_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH7_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH7_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH7_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH7_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH7_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH7_INTERLOCK_PDBM  , val); }
        else if(key == PV_SUFFIX_CH8_INTERLOCK_STATUS){ addToPVStruct(pvs, LLRF_PV_TYPE::CH8_INTERLOCK_STATUS, val); }
        else if(key == PV_SUFFIX_CH8_INTERLOCK_ENABLE){ addToPVStruct(pvs, LLRF_PV_TYPE::CH8_INTERLOCK_ENABLE, val); }
        else if(key == PV_SUFFIX_CH8_INTERLOCK_U     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH8_INTERLOCK_U     , val); }
        else if(key == PV_SUFFIX_CH8_INTERLOCK_P     ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH8_INTERLOCK_P     , val); }
        else if(key == PV_SUFFIX_CH8_INTERLOCK_PDBM  ){ addToPVStruct(pvs, LLRF_PV_TYPE::CH8_INTERLOCK_PDBM  , val); }

    }
    else
    {   // we assume the order in the config file is correct
        if(key == PV_COUNT)
        {
            lastPVstruct->COUNT = getCOUNT(val);
        }
        else if(key == PV_MASK)
        {
            lastPVstruct->MASK = getMASK(val);
        }
        else if(key == PV_CHTYPE)
        {
            lastPVstruct->CHTYPE = getCHTYPE(val);

            if(lastPVstruct->pvType == LLRF_PV_TYPE::LIB_PHI_SP)
            {
                message("lastPVstruct->CHTYPE, val = ", val);
                message("lastPVstruct->CHTYPE, = ", lastPVstruct->CHTYPE);
            }
        }
        else if(key == PV_NAME)
        {
            lastPVstruct->name = val;
            debugMessage("Added name =  ", lastPVstruct->name);
        }
    }
}
//______________________________________________________________________________
void liberallrfConfigReader::addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtype,const std::string& pvSuffix)
{
    pvs.push_back(llrfStructs::pvStruct());
    pvs.back().pvType      = pvtype;
    pvs.back().pvSuffix    = pvSuffix;
    debugMessage("Added ", pvs.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
    lastPVstruct = &pvs.back();
}
//______________________________________________________________________________
void liberallrfConfigReader::addToliberallrfObjectsV1(const std::vector<std::string> &keyVal )
{
    if(readingMainRF)
    {
        addToMainObj(keyVal);
    }
    else if(readingOneTrace)
    {
        addToOneTrace(keyVal);
    }
    else if(readingTraceSCAN)
    {
        addToSCAN(keyVal);
    }
}
//______________________________________________________________________________
void liberallrfConfigReader::addToMainObj(const std::vector<std::string>&keyVal)
{
    using namespace UTL;
    std::string key = keyVal[ZERO_SIZET];
    std::string val = keyVal[ONE_SIZET];
    if(key == NAME)
    {
        /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
        /// init structs 'correctly'
        llrfObj.name = val;
        llrfObj.numIlocks = numIlocks;
        debugMessage("Added ", llrfObj.name);
    }
    else if(key == PV_ROOT)
    {
        if(usingVirtualMachine)
            llrfObj.pvRoot =  VM_PREFIX + val;
        else
            llrfObj.pvRoot = val;
        debugMessage("LLRF pvroot = ", llrfObj.pvRoot);
    }
    else if(key == LLRF_PHI_CALIBRATION)
    {
        llrfObj.phiCalibration = getNumD(val);
        debugMessage("LLRF phiCalibration = ", llrfObj.phiCalibration);
    }
    else if(key == LLRF_AMP_CALIBRATION)
    {
        llrfObj.ampCalibration = getNumD(val);
        debugMessage("LLRF ampCalibration = ", llrfObj.ampCalibration);
    }
    else if(key == LLRF_CREST_PHI)
    {
        llrfObj.crestPhi = getNumL(val);
    }
    else if(key == LLRF_PULSE_LATENCY)
    {
        llrfObj.pulse_latency = getSize(val);
    }
    // potentially we hardcode this value into the binary, and have it *NOT* configurable
    else if(key == LLRF_MAX_AMPLITUDE)
    {
        llrfObj.maxAmp = getNumL(val);
    }
}
//------------------------------------------------------------------------------------------
void liberallrfConfigReader::addToOneTrace(const std::vector<std::string>&keyVal)
{
    using namespace UTL;
    std::string key = keyVal[ZERO_SIZET];
    std::string val = keyVal[ONE_SIZET];
    if(key == PV_ROOT)
    {
        llrfObj.pvRoot_oneTrace = val;
    }
    else if(key == TRACE_NUM_ELEMENTS_TOTAL)
    {
        llrfObj.all_traces.num_elements_per_trace = getSize(val);
    }
    else if(key == TRACE_NUM_ELEMENTS_USED)
    {
        llrfObj.all_traces.num_elements_per_trace_used = getSize(val);
    }
    else if(key == TRACE_NUM_OF_START_ZEROS)
    {
        llrfObj.all_traces.num_start_zeroes_per_trace = getSize(val);
    }
    else if(key == NUM_TRACES)
    {
        llrfObj.all_traces.num_traces = getSize(val);
    }

    /*
        The below values are speicif to individual traces,
        not every llrfObject needs all possible traces (HRRG adn LRRG difference)
        so below is more complicated ...
    */
    else if(key == NAME)
    {
        /*
            if using HRRG or LRRG we don't need all the traces from the libera
            (only 1 gun can run at a time) so only add traces that match the LLRF_TYPE
        */
        if(traceMatches_LLRF_TYPE(val))
        {
            llrfObj.all_traces.trace_info[val];
            llrfObj.all_traces.trace_info.at(val).name = val;
            lastOneTraceInfoStruct = &llrfObj.all_traces.trace_info.at(val);
            add_trace_type = true;
            message("Adding ", val, " as a relvant LLRF trace for this LLRF_TYPE (", ENUM_TO_STRING(type) );
        }
        else
        {
            add_trace_type = false;
        }
    }
    /*
        only add if previous trace "NAME" was added
    */
    if(add_trace_type)
    {
        if(key == TRACE_TYPE)
        {
            if(val == POWER)
            {
                lastOneTraceInfoStruct->type = llrfStructs::TRACE_TYPE::POWER;
            }
            else if(val == PHASE)
            {
                lastOneTraceInfoStruct->type = llrfStructs::TRACE_TYPE::PHASE;
            }
        }
        else if(key == TRACE_NUM)
        {
            lastOneTraceInfoStruct->position = getSize(val);
        }
    }
}
//------------------------------------------------------------------------------------------
void liberallrfConfigReader::addToSCAN(const std::vector<std::string>&keyVal)
{
    using namespace UTL;
    std::string key = keyVal[ZERO_SIZET];
    std::string val = keyVal[ONE_SIZET];
    if(key == NAME)
    {
        llrfObj.trace_scans[val];
        llrfObj.trace_scans.at(val).name = val;
        //debugMessage("Added SCAN for ", llrfObj.trace_scans.at(val).name);
        lastScanStruct = &llrfObj.trace_scans.at(val);
    }
    else if( key == PV_ROOT)
    {
        lastScanStruct->pvroot = val;
    }
}
//______________________________________________________________________________
bool liberallrfConfigReader::getliberallrfObject(llrfStructs::liberallrfObject& obj)
{
    obj = llrfObj;
    for(auto&& it:pvMonStructs)
    {
        //message("getliberallrfObject, ",ENUM_TO_STRING(it.pvType));
        obj.pvMonStructs[it.pvType] = it;
    }
    for(auto&& it:pvComStructs)
    {
        obj.pvComStructs[it.pvType] = it;
    }

    for(auto&& it:pvOneTraceMonStructs)
    {
        //message("getliberallrfObject, ",ENUM_TO_STRING(it.pvType));
        obj.pvOneTraceMonStructs[it.pvType] = it;
    }
    for(auto&& it:pvOneTraceComStructs)
    {
        obj.pvOneTraceComStructs[it.pvType] = it;
    }

    for( auto&&obj_it: obj.trace_scans )
    {
        for(auto&&it:pvSCANMonStructs)
        {
            //message("getliberallrfObject, ",ENUM_TO_STRING(it.pvType));
            obj_it.second.pvSCANStructs[it.pvType] = it;
        }
        for(auto&&it:pvSCANComStructs)
        {
            obj_it.second.pvSCANStructs[it.pvType] = it;
        }
        /*
            set initial values for each objects SCAN
        */
        for( auto&&it:obj_it.second.pvSCANStructs)
        {
            obj_it.second.value[it.first] = llrfStructs::LLRF_SCAN::UNKNOWN_SCAN;

            //message(obj_it.first, " added scan for ", ENUM_TO_STRING(it.first)  );

        }
    }

    message("liberallrfConfigReader::getliberallrfObject FIN ");
    return true;
}

bool liberallrfConfigReader::traceMatches_LLRF_TYPE(const std::string& val) const
{
    if(stringIsSubString(val, "LRRG"))//MAGIC_STRING
    {
        if(type == llrfStructs::LLRF_TYPE::VELA_LRRG)
        {
            return true;
        }
        if(type == llrfStructs::LLRF_TYPE::CLARA_LRRG)
        {
            return true;
        }
        return false;
    }
    else if(stringIsSubString(val, "HRRG"))//MAGIC_STRING
    {
        if(type == llrfStructs::LLRF_TYPE::VELA_HRRG)
        {
            return true;
        }
        if(type == llrfStructs::LLRF_TYPE::CLARA_HRRG)
        {
            return true;
        }
        return false;
    }
    else if(stringIsSubString(val, "PROBE"))//MAGIC_STRING
    {
        if(type == llrfStructs::LLRF_TYPE::VELA_HRRG)
        {
            return true;
        }
        if(type == llrfStructs::LLRF_TYPE::CLARA_HRRG)
        {
            return true;
        }
        if(type == llrfStructs::LLRF_TYPE::L01)
        {
            return true;
        }
        return false;
    }
    else if(stringIsSubString(val, "CALIBRATION"))//MAGIC_STRING
    {
        return false;
    }
    /*
        if HRRG or LRRG are not in val (the trace name) then it must be OK
    */
    return true;
}



//______________________________________________________________________________
void liberallrfConfigReader::addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtypein)
{
    for(auto && it: pvs)
    {
        pvMonStructs.push_back(llrfStructs::pvStruct());
        pvMonStructs.back().pvType   = getPVType(pvtypein, it.pvType);
        pvMonStructs.back().pvSuffix = temp_pvroot + it.pvSuffix;
        pvMonStructs.back().name     = temp_name;
        pvMonStructs.back().COUNT    = it.COUNT;
        pvMonStructs.back().MASK    = it.MASK;
        pvMonStructs.back().CHTYPE    = it.CHTYPE;
        debugMessage("Added ", pvMonStructs.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvMonStructs.back().pvType));
    }
}
//______________________________________________________________________________



//______________________________________________________________________________
llrfStructs::LLRF_PV_TYPE liberallrfConfigReader::getPVType(const llrfStructs::LLRF_PV_TYPE main, const llrfStructs::LLRF_PV_TYPE sub)
{
//    if(sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM)
//        return main;
//    else if(sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM)
//        return main;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_SCAN;
//
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_EVID;
//
//
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_SCAN;
//
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_EVID;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_EVID;
//    // these next ones are JUST for SCAN parameters
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_LOCAL_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_LOCAL_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_LOCAL_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_LOCAL_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_LOCAL_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_LOCAL_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_LOCAL_SCAN;
//
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_AMP_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_AMP_DER_SCAN;
//
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_DER_SCAN;
//    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
//
    return llrfStructs::LLRF_PV_TYPE::UNKNOWN;

}
