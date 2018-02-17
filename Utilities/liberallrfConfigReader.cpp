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
liberallrfConfigReader::liberallrfConfigReader(const std::string & configFileLocation1,
                                        const bool startVirtualMachine,
                                        const bool* show_messages_ptr,
                                        const bool* show_debug_messages_ptr):
configReader(configFileLocation1, show_messages_ptr, show_debug_messages_ptr),
usingVirtualMachine(startVirtualMachine),
lastPVstruct(nullptr),
type(llrfStructs::LLRF_TYPE::UNKNOWN_TYPE ),
//channel(0),
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
    debugMessage("\n", "**** Attempting to Read The RF Config File ****");

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
                                        addToliberallrfObjectsV1(keyVal);

                                    else if (readingCommandPVs )
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

//        if(numObjs == laserObjects.size())
//            debugMessage("*** Created ", numObjs, " Shutter Objects, As Expected ***");
//        else
//            debugMessage("*** Created ", laserObjects.size() ," Expected ", numObjs,  " ERROR ***" );

        success = true;
    }
    else{
        message("!!!! Error Can't Open Config File after searching in:  ", configFile1, " !!!!" );
    }
    if(success);
    {
        success = readTraceConfig();
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
    addToPVMapV1(keyVal);
}
//______________________________________________________________________________
void liberallrfConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal )
{
    addToPVMapV1(keyVal);
}
//______________________________________________________________________________
void liberallrfConfigReader::addToPVMapV1(const std::vector<std::string>& keyVal)
{// here i've hardcoded in the monitor or command PVs
    if(stringIsSubString(keyVal[0],"SUFFIX"))
    {
        // there are different types of screen, here we are hardcoding in how to interpret the config file and where to put
        // each type of monitor.
        if(keyVal[0] == UTL::PV_SUFFIX_LIB_AMP_FF)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_AMP_SP)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_PHI_FF)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_PHI_FF,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_PHI_SP)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_PHI_SP,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_PULSE_LENGTH)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_PULSE_LENGTH,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_PULSE_OFFSET)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_PULSE_OFFSET,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_ILOCK_STATE)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_ILOCK_STATE,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_FF_AMP_LOCK_STATE)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_FF_PHASE_LOCK_STATE)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_TIME_VECTOR)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_TIME_VECTOR,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_LIB_RF_OUTPUT)
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::LIB_RF_OUTPUT,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_TRIG_SOURCE )
        {
            addToPVStruct(pvMonStructs, llrfStructs::LLRF_PV_TYPE::TRIG_SOURCE,keyVal[1]);
        }

    }
    else
    {   // we assume the order in the config file is correct
        if(keyVal[0] == UTL::PV_COUNT)
            //lastPVstruct.back().COUNT = getCOUNT(keyVal[ 1 ]);
            lastPVstruct->COUNT = getCOUNT(keyVal[ 1 ]);
        else if(keyVal[0] == UTL::PV_MASK)
            lastPVstruct->MASK = getMASK(keyVal[ 1 ]);
        else if(keyVal[0] == UTL::PV_CHTYPE)
            lastPVstruct->CHTYPE = getCHTYPE(keyVal[ 1 ]);
        else if(keyVal[0] == UTL::PV_NAME)
        {
            lastPVstruct->name = keyVal[ 1 ];
            debugMessage("Added name =  ", lastPVstruct->name );
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
void liberallrfConfigReader::addToliberallrfObjectsV1(const std::vector<std::string> &keyVal  )
{
    if(keyVal[0] == UTL::NAME)
    {
        /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
        /// init structs 'correctly'
        llrfObj.name = keyVal[ 1 ];
        llrfObj.numIlocks = numIlocks;
        debugMessage("Added ", llrfObj.name);
    }
    else if(keyVal[0] == UTL::PV_ROOT)
    {
        if(usingVirtualMachine)
            llrfObj.pvRoot =  UTL::VM_PREFIX + keyVal[ 1 ];
        else
            llrfObj.pvRoot = keyVal[ 1 ];
        debugMessage("LLRF pvroot = ", llrfObj.pvRoot);
    }
    else if(keyVal[0] == UTL::LLRF_PHI_CALIBRATION)
    {
        llrfObj.phiCalibration = getNumD(keyVal[ 1 ]);
        debugMessage("LLRF phiCalibration = ", llrfObj.phiCalibration);
    }
    else if(keyVal[0] == UTL::LLRF_AMP_CALIBRATION)
    {
        llrfObj.ampCalibration = getNumD(keyVal[ 1 ]);
        debugMessage("LLRF ampCalibration = ", llrfObj.ampCalibration);
    }
    else if(keyVal[0] == UTL::LLRF_CREST_PHI)
    {
        llrfObj.crestPhi = getNumL(keyVal[ 1 ]);
    }
    else if(keyVal[0] == UTL::LLRF_PULSE_LATENCY)
    {
        llrfObj.pulse_latency = getSize(keyVal[1]);
    }
    // potentially we hardcode this value into the binary, and have it *NOT* configurable
    else if(keyVal[0] == UTL::LLRF_MAX_AMPLITUDE)
    {
        llrfObj.maxAmp = getNumL(keyVal[ 1 ]);

    }
}
//______________________________________________________________________________
bool liberallrfConfigReader::getliberallrfObject(llrfStructs::liberallrfObject& obj)
{
    obj = llrfObj;
    for(auto && it : pvMonStructs)
    {
        //message("getliberallrfObject, ",ENUM_TO_STRING(it.pvType));
        obj.pvMonStructs[it.pvType] = it;
    }
    for(auto && it : pvComStructs)
    {
        obj.pvComStructs[it.pvType] = it;
    }
    message("liberallrfConfigReader::getliberallrfObject FIN ");
    return true;
}

//______________________________________________________________________________
bool liberallrfConfigReader::readTraceConfig()
{
    switch(type)
    {
        case llrfStructs::LLRF_TYPE::CLARA_HRRG:
            readConfig(UTL::APCLARA1_CONFIG_PATH + UTL::GUN_LLRF_TRACES);
            break;
        case llrfStructs::LLRF_TYPE::CLARA_LRRG:
            readConfig(UTL::APCLARA1_CONFIG_PATH + UTL::GUN_LLRF_TRACES);
            break;
        case llrfStructs::LLRF_TYPE::VELA_HRRG:
            readConfig(UTL::APCLARA1_CONFIG_PATH + UTL::GUN_LLRF_TRACES);
            break;
        case llrfStructs::LLRF_TYPE::VELA_LRRG:
            readConfig(UTL::APCLARA1_CONFIG_PATH + UTL::GUN_LLRF_TRACES);
            break;
        case llrfStructs::LLRF_TYPE::L01:
            readConfig(UTL::APCLARA1_CONFIG_PATH + UTL::L01_LLRF_TRACES);
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
bool liberallrfConfigReader::readConfig(const std::string & configFile1 )
{
    debugMessage("\n", "**** Attempting to Read The LLRF TRACES Config File ****");

    std::string line, trimmedLine;
    bool success = false;

    pvPowerTraceStructs.clear();
    pvPhaseTraceStructs.clear();
    std::ifstream inputFile;
    std::vector<llrfStructs::pvStruct> pvMonStructsLocal;
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
                                        addToTraceObjectsV1(keyVal);

                                    else if (readingCommandPVs )
                                        addToPVCommandMapV1(keyVal);

                                    else if (readingMonitorPVs)
                                        addToTracePVMap(keyVal);
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

//        if(numObjs == laserObjects.size())
//            debugMessage("*** Created ", numObjs, " Shutter Objects, As Expected ***");
//        else
//            debugMessage("*** Created ", laserObjects.size() ," Expected ", numObjs,  " ERROR ***" );

        success = true;
    }
    else{
        message("!!!! Error Can't Open Config File after searching in:  ", configFile1, " !!!!" );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void liberallrfConfigReader::addToTracePVMap(const std::vector<std::string>& keyVal)
{// here i've hardcoded in the monitor or command PVs
    if(stringIsSubString(keyVal[0],"SUFFIX"))//MAGIC_STRING
    {
        /// TRACE SPECIFIC PVs
        if(keyVal[0] == UTL::PV_SUFFIX_PWR_REM_EVID)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_PWR_REM_SCAN)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_PHASE_REM_EVID)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_PHASE_REM_SCAN)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN,keyVal[1]);
        }
        else if(keyVal[0] == UTL::PV_SUFFIX_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM,keyVal[1]);
        }

        else if(keyVal[0] == UTL::PV_SUFFIX_AMP_DER_SCAN)
        {
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN,keyVal[1]);
        }

        else if(keyVal[0] == UTL::PV_SUFFIX_PHASE_DER_SCAN)
        {
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN, keyVal[1]);
        }

        else if(keyVal[0] == UTL::PV_SUFFIX_PWR_LOC_SCAN)
        {
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN,keyVal[1]);
        }

    }
    else
    {   // we assume the order in the config file is correct
        if(keyVal[0] == UTL::PV_COUNT)
            //lastPVstruct.back().COUNT = getCOUNT(keyVal[ 1 ]);
            lastPVstruct->COUNT = getCOUNT(keyVal[ 1 ]);
        else if(keyVal[0] == UTL::PV_MASK)
            lastPVstruct->MASK = getMASK(keyVal[ 1 ]);
        else if(keyVal[0] == UTL::PV_CHTYPE)
            lastPVstruct->CHTYPE = getCHTYPE(keyVal[ 1 ]);
        else if(keyVal[0] == UTL::PV_NAME)
        {
            lastPVstruct->name = keyVal[ 1 ];
            debugMessage("Added name =  ", lastPVstruct->name );
        }
    }
}
//______________________________________________________________________________
void liberallrfConfigReader::addToTraceObjectsV1(const std::vector<std::string> &keyVal)
{
    if(keyVal[0] == UTL::NAME)
    {
        temp_name = keyVal[1];
        debugMessage("Added temp_name =  ", temp_name );
    }
    else if(keyVal[0] == UTL::PV_ROOT)
    {
            temp_pvroot = keyVal[1];
            debugMessage("Added temp_pvroot =  ", temp_pvroot );
    }
    else if(keyVal[0] == UTL::LLRF_CHANNEL)
    {
         if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH1_PHASE_REM)
         {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM);
         }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH2_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH3_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH4_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH5_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH6_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM);

        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH7_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH8_PHASE_REM)
        {
            addToPVStruct(pvPhaseTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM);
            addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH1_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH2_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH3_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH4_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH5_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH6_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH7_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM);
        }
        else if(keyVal[1] == UTL::PV_SUFFIX_LIB_CH8_PWR_REM)
        {
            addToPVStruct(pvPowerTraceStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM);
            //addToPVStruct(pvLocalDerStructs, llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM);
        }
    }
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
llrfStructs::LLRF_PV_TYPE liberallrfConfigReader::getPVType( const llrfStructs::LLRF_PV_TYPE main, const llrfStructs::LLRF_PV_TYPE sub)
{
    if( sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM)
        return main;
    else if(sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM)
        return main;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_SCAN;

    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_EVID;


    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_SCAN;

    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_EVID;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_REM_EVID)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_EVID;
    // these next ones are JUST for SCAN parameters
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_LOCAL_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_LOCAL_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_LOCAL_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_LOCAL_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_LOCAL_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_LOCAL_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_LOCAL_SCAN;

    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_AMP_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_AMP_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_AMP_DER_SCAN;

    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_DER_SCAN;
    if(main == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM && sub == llrfStructs::LLRF_PV_TYPE::LIB_PHASE_DER_SCAN)
        return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_DER_SCAN;

}
