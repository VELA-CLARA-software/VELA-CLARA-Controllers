//djs
#include "dburt.h"
#include "dburtDefinitions.h"
#include "magnetStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

dburt::dburt(const bool& show_messages,
             const bool& show_debug_messages,
             const HWC_ENUM::MACHINE_AREA myMachineArea ):
configReader(UTL::DBURT_PATH, show_messages, show_debug_messages),
myMachineArea(myMachineArea)
{
}
//______________________________________________________________________________
//dburt::dburt(const std::string & configFile_Location, const bool* show_messages, const bool * show_debug_messages ):
//configReader(configFile_Location, show_messages, show_debug_messages)
//{
//}
//______________________________________________________________________________
dburt::~dburt(){}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURT(const std::string& filePath,const std::string& fileName)
{
    return actuallyReadTheDBURT(filePath,fileName);
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURT(const std::string& fileName)
{
    return readDBURT(UTL::DBURT_PATH, fileName);
}
//______________________________________________________________________________
std::string dburt::getFilePathFromINputs(const std::string& filePath,
                                         const std::string& fileName)const
{
    std::string pathandfile;
    if(filePath == "")
    {
        pathandfile = UTL::DBURT_PATH + fileName;
    }
    else if(filePath.back() == UTL::SLASH_SLASH_C)
    {
        pathandfile = filePath + fileName;
    }
    else if(filePath.back() != UTL::SLASH_SLASH_C)
    {
        pathandfile = filePath + UTL::SLASH_SLASH + pathandfile;
    }
    return pathandfile;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::actuallyReadTheDBURT(const std::string& filePath,const std::string& fileName)
{
    std::string pathandfile = getFilePathFromINputs(filePath,fileName);

    message("\n", "**** Attempting to Read ", pathandfile, " ****");

    std::string line, trimmedLine;

    std::ifstream inputFile;

    configVersion = -1;

    inputFile.open(pathandfile, std::ios::in);
    if(inputFile)
    {
        message("File Opened from ",  pathandfile);
        while(std::getline(inputFile, line)) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));

            message(line);

            if(stringIsSubString(line, UTL::VELA_MAGNET_SAVE_FILE_v1))
            {
                configVersion = 1;
                break;
            }
            else if(stringIsSubString(line, UTL::DBURT_HEADER_V3))
            {
                configVersion = 3;// version 2 got lost around October 2015
                break;
            }
            else if(stringIsSubString(line, UTL::DBURT_HEADER_V4))
            {
                configVersion = 4;// version 2 got lost around October 2015
                break;
            }
            else if(stringIsSubString(line, UTL::VERSION))
            {
                if(stringIsSubString(trimmedLine, UTL::VERSION))
                    getVersion(trimmedLine);
            }
            else if(stringIsSubString(line, UTL::VELA_CLARA_DBURT_ALIAS_V1))
            {
                message("stringIsSubString(line, UTL::VELA_CLARA_DBURT_ALIAS_V1)");
                std::getline(inputFile, line);
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
                std::vector<std::string> keyvalue = getKeyVal(trimmedLine, UTL::EQUALS_SIGN_C);

                message(keyvalue[0]);
                message(keyvalue[1]);
                pathandfile = getFilePathFromINputs(trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[0]),
                                                    trimAllWhiteSpaceExceptBetweenDoubleQuotes(keyvalue[1]));
                message(pathandfile);
                configVersion = 4;
            }
        }
    }
    debugMessage("Finished preprocessing file");
    inputFile.close();
    magnetStructs::magnetStateStruct magState;
    switch(configVersion)
    {
        case -1:
            debugMessage("NO DBURT VERSION FOUND EXIT");
            break;

        case 1:
            debugMessage("VERSION 1 DBURT FOUND");
            //magState = readDBURTv1(filePath);
            break;
        case 2:
            //debugMessage("VERSION 2 DBURT FOUND");
            break;
        case 3:
            debugMessage("VERSION 3 DBURT FOUND");
            //magState = readDBURTv3(filePath);
            break;
        case 4:
            debugMessage("VERSION 4 DBURT FOUND");
            magState = readDBURTv4(pathandfile);
            break;
        default:
            debugMessage("UNEXPECTED DBURT VERSION, ", configVersion, ", FOUND");

    }
    return magState;
}


//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURTv4(const std::string & pathandfile)
{
//    std::string pathToDBURT;
//    if(path == "")
//        pathToDBURT =  UTL::DBURT_PATH;
//    else
//        pathToDBURT =  path;

    magnetStructs::magnetStateStruct magState;

    std::string line, trimmedLine;
    std::ifstream inputFile;

    std::vector<std::string> keyvalue;

    inputFile.open(pathandfile, std::ios::in);
    if(inputFile)
    {
        bool readingParameters = false;
        int  linenumber        = 0;

        while(std::getline(inputFile, line)) /// Go through line by line
        {
            std::stringstream iss(line); /// make a stream of the line and then do some tests
            ++linenumber;
            if(stringIsSubString(iss.str(), UTL::DBURT_EOF_V3))
            {
                message("FOUND END OF FILE ");
                readingParameters = false;
                break;
            }
            if(readingParameters)
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));

                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                if(keyvalue.size() == 4)
                {

                    debugMessage("FOUND ",keyvalue[0],
                                 ", psu state = ", keyvalue[1],
                                 ", SI = ",getNumD(keyvalue[2]),
                                 ", RI = ",getNumD(keyvalue[3])
                                 );

                    magState.magNames.push_back(keyvalue[0]);

                    if(keyvalue[1] == UTL::ON)
                    {
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::ON);
                    }
                    else if(keyvalue[1] == UTL::OFF)
                    {
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::OFF);
                    }
                    else
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::ERROR);

                    magState.siValues.push_back(getNumD(keyvalue[2]));
                }
            } // if(readingParameters)_END

            if(stringIsSubString(iss.str(), UTL::DBURT_NUM_MAGNETS_V4) )
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                magState.numMags = getSize(keyvalue[1]);
                message("FOUND NUM MAGNETS = ",  magState.numMags);
                message("FOUND NUM MAGNETS = ",  getSize(keyvalue[1]), "   ", keyvalue[1]);
                readingParameters = true;
            }
            if(stringIsSubString(iss.str(), UTL::DBURT_PARAMETERS_V4) )
            {
                message("FOUND START OF DATA");
            }
            if(stringIsSubString(iss.str(), UTL::DBURT_HEADER_AREA) )
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::VELA_INJ))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_INJ;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::VELA_BA1))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_BA1;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::VELA_BA2))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_BA2;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_INJ))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_INJ;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_PH1))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_PH1;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_2_BA1))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_2_BA1;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_2_BA2))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_2_BA2;
                }
                else if(keyvalue[UTL::ONE_SIZET] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2))
                {
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2;
                }
            }

        } // while
    } // main if
    else
        message("ERROR: In importDBURT: failed to open ",  pathandfile);

    return magState;
}
//______________________________________________________________________________
bool dburt::writeDBURT(const magnetStructs::magnetStateStruct& magState,
                       const std::string& fileName,
                       const std::string& comments,
                       const std::string& keywords)
{
    //std::cout << "writeDBURT CALLED!" << std::endl;

    bool success = false;

    std::string fn;


    //std::cout << "getting CDT " << std::endl;
    std::string cdt = currentDateTime();
    //std::cout << "got CDT " << std::endl;

    //message("cdt = ", cdt);

    if(fileName == "")
        fn = UTL::DBURT_PATH + UTL::SLASH_SLASH + cdt + UTL::dotDBURT;
    else
        fn = fileName;


    //std::cout << "fileName FN = " << fn <<  std::endl;

    std::ofstream outputFile;

    outputFile.open(fn, std::ios::out);

    if(outputFile)
    {
        //std::cout << "outputFile file open " <<  std::endl;

        outputFile << UTL::DBURT_HEADER_V4   <<std::endl;
        outputFile << std::endl;
        outputFile << UTL::DBURT_HEADER_DT   <<cdt <<UTL::END_OF_LINE <<std::endl;
        outputFile << std::endl;
        outputFile << UTL::DBURT_HEADER_KEYW <<keywords <<UTL::END_OF_LINE <<std::endl;
        outputFile << std::endl;
        outputFile << UTL::DBURT_HEADER_COM  <<comments <<UTL::END_OF_LINE <<std::endl;
        outputFile << std::endl;
        outputFile << UTL::DBURT_HEADER_AREA <<ENUM_TO_STRING(myMachineArea) << UTL::END_OF_LINE << std::endl;
        outputFile << std::endl;
        outputFile << UTL::START_OF_DATA << UTL::END_OF_LINE << std::endl;

        outputFile <<UTL::NUMBER_OF_OBJECTS << UTL::COLON_C << magState.numMags << UTL::END_OF_LINE << std::endl;
        for(size_t i = UTL::ZERO_SIZET; i <magState.numMags; ++i)//MAGIC_NUMBER
        {
            outputFile <<magState.magNames[i] <<UTL::COLON_C;
            switch(magState.psuStates[i])
            {
                case magnetStructs::MAG_PSU_STATE::OFF:
                    outputFile <<UTL::OFF <<UTL::COLON_C;
                    break;

                case magnetStructs::MAG_PSU_STATE::ON:
                    outputFile <<UTL::ON <<UTL::COLON_C;
                    break;

                case magnetStructs::MAG_PSU_STATE::TIMING:
                    outputFile <<UTL::TIMING<<UTL::COLON_C;
                    break;

                case magnetStructs::MAG_PSU_STATE::ERROR:
                    outputFile <<UTL::ERROR<<UTL::COLON_C;
                    break;

                case magnetStructs::MAG_PSU_STATE::NONE:
                     outputFile <<UTL::NONE<<UTL::COLON_C;
                    break;

                default:
                    outputFile <<UTL::ERROR<<UTL::COLON_C;
            }
            outputFile << magState.siValues[i];
            outputFile << UTL::COLON_C;
            outputFile << magState.riValues[i];
            outputFile << UTL::END_OF_LINE << std::endl;
        }
        outputFile << UTL::END_OF_DATA << UTL::END_OF_LINE << std::endl;
        outputFile.close();
        success = true;
    }
    else{

        std::cout <<"Can't create output file..." <<std::endl;
    }
    return success;
}
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY
/// LEGACY



//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURTv3(const char* fileName, const std::string & path)
{
    std::string pathToDBURT;
    if(path == "")
        pathToDBURT =  UTL::DBURT_PATH;
    else
        pathToDBURT =  UTL::DBURT_PATH;

    magnetStructs::magnetStateStruct magState;


    std::string line, trimmedLine;
    std::ifstream inputFile;

    std::vector<std::string> keyvalue;

    inputFile.open(pathToDBURT+fileName, std::ios::in);
    if(inputFile)
    {
        bool readingParameters = false;
        int  linenumber        = 0;

        while(std::getline(inputFile, line)) /// Go through line by line
        {
            std::stringstream iss(line); /// make a stream of the line and then do some tests
            ++linenumber;


            if(stringIsSubString(iss.str(), UTL::DBURT_EOF_V3))
            {
                message("FOUND END OF FILE ");
                readingParameters = false;
                break;
            }
            if(readingParameters)
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));

                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                if(keyvalue.size() == 3)
                {

                    debugMessage("FOUND ",keyvalue[0],", psu state = ", keyvalue[1],", SI = ",getNumD(keyvalue[2]));

                    magState.magNames.push_back(keyvalue[0]);

                    if(keyvalue[1] == UTL::ON)
                    {
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::ON);
                    }
                    else if(keyvalue[1] == UTL::OFF)
                    {
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::OFF);
                    }
                    else
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::ERROR);

                    magState.siValues.push_back(getNumD(keyvalue[2]));
                }
            } // if(readingParameters)_END
            else if(stringIsSubString(iss.str(), UTL::DBURT_NUM_MAGNETS_V3) )
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                magState.numMags = getSize(keyvalue[1]);
                message("FOUND NUM MAGNETS = ",  magState.numMags);
                readingParameters = true;
            }
            else if(stringIsSubString(iss.str(), UTL::DBURT_PARAMETERS_V1) )
            {
                message("FOUND START OF DATA");
            }
            else if(stringIsSubString(iss.str(), UTL::DBURT_HEADER_AREA) )
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                message(keyvalue[0],",",keyvalue[1]);

                if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::VELA_INJ))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_INJ;
                else if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::VELA_BA1))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_BA1;
                else if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::VELA_BA2))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_BA2;
                else if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_INJ))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_INJ;
                else if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_PH1))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_PH1;
                else if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_2_BA1))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_2_BA1;
                else if(keyvalue[1] == ENUM_TO_STRING(HWC_ENUM::MACHINE_AREA::CLARA_2_BA2))
                    magState.machineArea = HWC_ENUM::MACHINE_AREA::CLARA_2_BA2;
            }
            message("Line  =  ",iss.str());

        } // while
    } // main if
    else
        message("ERROR: In importDBURT: failed to open ",  pathToDBURT, fileName);

    return magState;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURTv1(const char* fileName, const std::string & path)
{
    std::string pathToDBURT;
    if(path == "")
        pathToDBURT =  UTL::DBURT_PATH;
    else
        pathToDBURT =  UTL::DBURT_PATH;


    magnetStructs::magnetStateStruct magState;
    magState.machineArea = HWC_ENUM::MACHINE_AREA::VELA_INJ;

    std::string line, trimmedLine;
    std::ifstream inputFile;

    std::vector<std::string> keyvalue;

    inputFile.open(pathToDBURT+fileName, std::ios::in);
    if(inputFile)
    {
        bool readingParameters = false;
        int  linenumber        = 0;

        while(std::getline(inputFile, line)) /// Go through line by line
        {
            std::stringstream iss(line); /// make a stream of the line and then do some tests
            ++linenumber;
            if(stringIsSubString(iss.str(), UTL::DBURT_EOF_V1))
            {
                message("FOUND END OF FILE ");
                readingParameters = false;
                break;
            }
            if(readingParameters)
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));

                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);

                if(keyvalue.size() == 3)
                {
                    magState.magNames.push_back(keyvalue[0]);

                    if(keyvalue[1] == UTL::ON)
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::ON);
                    else if(keyvalue[1] == UTL::OFF)
                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::OFF);
//                    else
//                        magState.psuStates.push_back(magnetStructs::MAG_PSU_STATE::ON);

                    magState.siValues.push_back(getNumD(keyvalue[2]));
                }
            } // if(readingParameters)_END

            if(stringIsSubString(iss.str(), UTL::DBURT_NUM_MAGNETS_V1) )
            {
                trimmedLine = trimAllWhiteSpace(trimToDelimiter(line, UTL::END_OF_LINE));
                keyvalue = getKeyVal(trimmedLine, UTL::COLON_C);


                magState.numMags = getSize(keyvalue[1]);
                message("FOUND NUM MAGNETS = ",  getSize(keyvalue[1]), "   ", keyvalue[1]);
                message("FOUND NUM MAGNETS = ",  magState.numMags);
                readingParameters = true;
            }
            if(stringIsSubString(iss.str(), UTL::DBURT_PARAMETERS_V1) )
            {
                message("FOUND START OF DATA");
            }
        } // while
    } // main if
    else
        message("ERROR: In importDBURT: failed to open ",  pathToDBURT, fileName);

    return magState;
}




