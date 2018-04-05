//djs
#include "magnetConfigReader.h"
#include "configDefinitions.h"
#include "RFGunStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>


magnetConfigReader::magnetConfigReader(const std::string& magConf,
                                       const bool startVirtualMachine,
                                       const bool& show_messages,
                                       const bool& show_debug_messages):
magConf(magConf),
configReader(magConf, show_messages, show_debug_messages, startVirtualMachine)
{
}
//______________________________________________________________________________
magnetConfigReader::~magnetConfigReader(){}
//______________________________________________________________________________
bool magnetConfigReader::getMagData(std::map<std::string, magnetStructs::magnetObject> & mapToFill)
{
    bool success = true;
    mapToFill.clear();

    size_t nCount = 0; // this may actually be broke??
    size_t rCount = 0;

    for(auto && it : magObjects)
    {
        // add in magnetObj
        mapToFill[ it.name ] = it;
        // setMon and Com pvStructs
        for(auto && it2 : pvMagMonStructs)
            mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;

        for(auto && it2 : pvMagComStructs)
            mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
    }
    // sanity checks
    if(rCount != nCount)
    {
        message("Error ", nCount, " N-PSUs and ", rCount, " R-PSUs");
        success = false;
    }
    return success;
}
//______________________________________________________________________________
magnetStructs::degaussValues magnetConfigReader::getDeguassStruct()
{
    return degstruct;
}
//______________________________________________________________________________
bool magnetConfigReader::readConfig()
{
    if(useVM)
        debugMessage("\n", "**** Using VIRTUAL Machine  ****");
    else
        debugMessage("**** Using PHYSICAL Machine  ****","\n");

    std::string line, trimmedLine;
    bool success = false;

    std::ifstream inputFile;

    inputFile.open(magConf, std::ios::in);
    if(inputFile)
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage("Opened ", magConf);
        while(std::getline(inputFile, line)) /// Go through, reading file line by line
        {
            //trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
            trimmedLine = trimAllWhiteSpaceExceptBetweenDoubleQuotes(trimToDelimiter(line, UTL::END_OF_LINE));
            if(trimmedLine.size()> 0)
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
                                        addToMagObjectsV1(keyVal);

                                    else if (readingCommandPVs)
                                        addToMagComStructsV1(keyVal);

                                    else if (readingMonitorPVs)
                                        addToMagMonStructsV1(keyVal);
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
        message("!!!! Error Can't Open Config File after searching for:  ", magConf, " !!!!");
    }
    if(success)
        debugMessage("magnetConfigReader readconfig is returning TRUE");
    else
        debugMessage("magnetConfigReader readconfig is returning FALSE");


    return success;
}
//______________________________________________________________________________
//______________________________________________________________________________
void magnetConfigReader::addToMagObjectsV1(const std::vector<std::string> &keyVal) // /V1 is a mechanism for having a new version of configs if needed
{
    std::string value = keyVal[1];//MAGIC_NUMBER
    if(keyVal[0] == UTL::NAME)
    {
        magObjects.push_back(magnetStructs::magnetObject());    /// Any way to avoid the ladders?
        magObjects.back().name      = value;
        magObjects.back().numIlocks = (size_t)numIlocks;
        debugMessage("Added ", magObjects.back().name);
    }
    else if(keyVal[0] == UTL::PV_ROOT)
    {
        if(useVM)
            magObjects.back().pvRoot = UTL::VM_PREFIX + value;
        else
            magObjects.back().pvRoot = value;
    }
    else if(keyVal[0] == UTL::PV_PSU_ROOT)
    {
        if(useVM)
            magObjects.back().psuRoot = UTL::VM_PREFIX + value;
        else
            magObjects.back().psuRoot = value;
    }
    else if(keyVal[0] == UTL::MAG_TYPE)
        addMagType(keyVal);
    else if(keyVal[0] == UTL::RI_TOLERANCE)
        magObjects.back().riTolerance = getNumD(value);
     else if(keyVal[0] == UTL::NUM_DEGAUSS_STEPS)
        magObjects.back().numDegaussSteps =getSize(value);
    else if(keyVal[0] == UTL::MAG_SET_MAX_WAIT_TIME)
        magObjects.back().maxWaitTime = getSize(value);
    else if(keyVal[0] == UTL::DEGAUSS_TOLERANCE)
        magObjects.back().degTolerance = getNumD(value);
    else if(keyVal[0] == UTL::DEGAUSS_VALUES)
         magObjects.back().degValues = getDoubleVector(value);
    else if(keyVal[0] == UTL::POSITION)
         magObjects.back().position = getNumD(value);
    else if(keyVal[0] == UTL::FIELD_INTEGRAL_COEFFICIENTS)
        magObjects.back().fieldIntegralCoefficients = getDoubleVector(value);
    else if(keyVal[0] == UTL::MANUFACTURER)
        magObjects.back().manufacturer = value ;
    else if(keyVal[0] == UTL::SERIAL_NUMBER)
        magObjects.back().serialNumber = value ;
    else if(keyVal[0] == UTL::MEASUREMENT_DATA_LOCATION)
        magObjects.back().measurementDataLocation = value;
    else if(keyVal[0] == UTL::MAGNETIC_LENGTH)
        magObjects.back().magneticLength =  getNumD(value);
    else if(keyVal[0] == UTL::MAGNET_BRANCH)
        magObjects.back().magnetBranch = value;
    else if(keyVal[0] == UTL::MAG_MAX_I)
        magObjects.back().maxI = getNumD(value);
    else if(keyVal[0] == UTL::MAG_MIN_I)
        magObjects.back().minI = getNumD(value);
}
//______________________________________________________________________________
void magnetConfigReader::addMagType(const std::vector<std::string> &keyVal)
{
    if(keyVal[1] == UTL::SOL)
        magObjects.back().magType = magnetStructs::MAG_TYPE::SOL;
    else if(keyVal[1] == UTL::BSOL)
        magObjects.back().magType = magnetStructs::MAG_TYPE::BSOL;
    else if(keyVal[1] == UTL::QUAD)
        magObjects.back().magType = magnetStructs::MAG_TYPE::QUAD;
    else if(keyVal[1] == UTL::DIP)
        magObjects.back().magType = magnetStructs::MAG_TYPE::DIP;
    else if(keyVal[1] == UTL::HCOR)
        magObjects.back().magType = magnetStructs::MAG_TYPE::HCOR;
    else if(keyVal[1] == UTL::VCOR)
        magObjects.back().magType = magnetStructs::MAG_TYPE::VCOR;
    else if(keyVal[1] == UTL::SEXT)
        magObjects.back().magType = magnetStructs::MAG_TYPE::SEXT;
}
//______________________________________________________________________________
void magnetConfigReader::addToMagMonStructsV1(const std::vector<std::string> &keyVal)
{
    addToPVStruct(pvMagMonStructs, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addToMagComStructsV1(const std::vector<std::string> &keyVal)
{
    addToPVStruct(pvMagComStructs, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addToPVStruct(std::vector<magnetStructs::pvStruct>  & pvStruct_v, const std::vector<std::string> &keyVal)
{
    if(stringIsSubString(keyVal[0], "SUFFIX"))
    {
        pvStruct_v.push_back(magnetStructs::pvStruct());    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        //GUN  Protection
        if(keyVal[0] == UTL::PV_SUFFIX_SPOWER)
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::SPOWER;

        else if(keyVal[0] == UTL::PV_SUFFIX_RPOWER)
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::RPOWER;

        else if(keyVal[0] == UTL::PV_SUFFIX_READI)
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::READI;

        else if(keyVal[0] == UTL::PV_SUFFIX_SETI)
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::SETI;

        else if(keyVal[0] == UTL::PV_SUFFIX_RILK)
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::RILK;

        else if(keyVal[0] == UTL::PV_SUFFIX_GETSETI)
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::GETSETI;

         debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING(pvStruct_v.back().pvType)) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvStruct_v, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<magnetStructs::pvStruct> & pvStruct_v, const std::vector<std::string> &keyVal)
{
    if(keyVal[0] == UTL::PV_COUNT)
        pvStruct_v.back().COUNT = getCOUNT(keyVal[ 1 ]);
    else if(keyVal[0] == UTL::PV_MASK)
        pvStruct_v.back().MASK = getMASK(keyVal[ 1 ]);
    else if(keyVal[0] == UTL::PV_CHTYPE)
        pvStruct_v.back().CHTYPE = getCHTYPE(keyVal[ 1 ]);
}





