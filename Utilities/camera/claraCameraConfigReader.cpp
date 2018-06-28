//djs
#include "claraCameraConfigReader.h"
#include "configDefinitions.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>


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
//  Last edit:   ZERO_SIZET5-06-2018
//  FileName:    claraCameraConfigReader.cpp
//  Description: This class reads in all the analysis PVs for analysing
//               camera image data
//
//
//*/
//djs
#include "claraCameraConfigReader.h"
#include "configDefinitions.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

claraCameraConfigReader::claraCameraConfigReader(const std::string& configFile,
                                                       const bool& show_messages,
                                                       const bool& show_debug_messages,
                                                       const bool usingVM):
configReader(configFile,  show_messages, show_debug_messages,usingVM)
{
}
//______________________________________________________________________________
claraCameraConfigReader::~claraCameraConfigReader(){}
//______________________________________________________________________________
bool claraCameraConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read CLARA camera Config File ****" );

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
                    if( numObjs == camObjects.size() )
                        message("Found ", camObjects.size() , " objects, expetced ",numObjs);
                    else
                        message("!!ERROR!! Found ", camObjects.size() , " objects, expetced ",numObjs);

                    debugMessage("\nFound END_OF_DATA\n" );
                    debugMessage("Number of Monitors = ",pvMonStructs.size() );
                    debugMessage("Number of Commands = ",pvComStructs.size() );
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
                                        addToCameraImageDataObjects(keyVal);

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
                    debugMessage("\nFound PV_COMMANDS_START\n" );
                }
                if(stringIsSubString(line, UTL::PV_MONITORS_START))
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage("\nFound PV_MONITORS_START\n" );
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
void claraCameraConfigReader::addToPVMonitorMapV1(const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX"))
    {
        using namespace cameraStructs;
        if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_X_RBV)//1
        {
            //message("addToPVMonitorMapV1 ",keyVal[ZERO_SIZET] );
            addPVStruct(pvMonStructs, CAM_PV_TYPE::X_RBV, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_Y_RBV)//2
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::Y_RBV, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_X_RBV)//3
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SIGMA_X_RBV, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_Y_RBV)//4
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SIGMA_Y_RBV, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_X_PIX_RBV)//5
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::X_PIX_RBV, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_Y_PIX_RBV)//6
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::Y_PIX_RBV, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_X_PIX_RBV)//7
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SIGMA_X_PIX_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_Y_PIX_RBV)//8
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SIGMA_Y_PIX_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_COV_XY_RBV)//9
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::COV_XY_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_COV_XY_PIX_RBV)//10
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::COV_XY_PIX_RBV,keyVal[ONE_SIZET]);
        }
        // intensity
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_AV_PIX_INTENSITY_RBV)//11
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::AVG_PIX_INTENSITY_RBV,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SUM_PIX_INTENSITY_RBV)//12
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SUM_PIX_INTENSITY_RBV,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIXEL_RESULTS_RBV)//13
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::PIXEL_RESULTS_RBV,keyVal[ONE_SIZET]);
        }
        // mask parameters
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X_RBV)//14
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_X_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y_RBV)//15
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_Y_RBV,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X_RAD_RBV)//16
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_X_RAD_RBV,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y_RAD_RBV)//17
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_Y_RAD_RBV,keyVal[ONE_SIZET]);
        }
        // image centre and pix 2 mm
        //
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIX_MM_RBV)//18
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::PIX_MM_RBV,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_X_RBV)//19
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::X_CENTER_RBV,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_Y_RBV)//20
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::Y_CENTER_RBV,keyVal[ONE_SIZET]);
        }
        else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_STEP_SIZE_RBV)//21
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::STEP_SIZE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_START_IA_RBV  )//22
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::START_IA_RBV,keyVal[ONE_SIZET]);
        }
        // image analysis States
        else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_USE_B_RBV  )//23
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::USE_BKGRND_RBV,keyVal[ONE_SIZET]);
        }
        else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_USE_NPOINT_RBV  )//24
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::USE_NPOINT_RBV,keyVal[ONE_SIZET]);
        }
        //NOT NEEDED else if( keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CAM_STATE  )
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_ACQUIRE_RBV  )//25
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_ACQUIRE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_CAPTURE_RBV  )//26
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_CAPTURE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_NUM_CAPTURED_RBV  )//27
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_NUM_CAPTURED_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_NUM_CAPTURE_RBV  )//28
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_NUM_CAPTURE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_STATUS  )//29
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_FILE_WRITE_STATUS,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_FILE_RBV  )//3ZERO_SIZET
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_WRITE_FILE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_MESSAGE  )//31
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_FILE_WRITE_ERROR_MESSAGE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_CAPTURE_RBV_J  )//32
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::JPG_CAPTURE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_NUM_CAPTURED_J  )//33
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::JPG_NUM_CAPTURED,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_NUM_CAPTURE_RBV_J )//34
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::JPG_NUM_CAPTURE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_CHECK_J )//35
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::JPG_FILE_WRITE_CHECK,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_RBV_J  )//36
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::JPG_FILE_WRITE_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_MESSAGE_J  )//37
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::JPG_FILE_WRITE_MESSAGE,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_EXPOSURE_RBV  )//38
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_EXPOSURE_TIME_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_ACQ_PERIOD_RBV  )//39
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_ACQUIRE_PERIOD_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FREQ_RBV  )//4ZERO_SIZET
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_FREQ_RBV,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_SENSOR_TEMP_RBV)//41
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::CAM_SENSOR_TEMP_RBV,keyVal[ONE_SIZET] );
        }
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvMonStructs, keyVal);
}
//______________________________________________________________________________
void claraCameraConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<cameraStructs::pvStruct>& pvStruct_v,
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
void claraCameraConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal  )
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX" ) )
    {
        using namespace cameraStructs;
        if( keyVal[ZERO_SIZET] == UTL::PV_IA_SUFFIX_START_IA  )//1
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::START_IA, keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_IA_SUFFIX_SET_B  )//2
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::SET_BKGRND,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_IA_SUFFIX_USE_B  )//3
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::USE_BKGRND,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_IA_SUFFIX_USE_NPOINT  )//4
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::USE_NPOINT,keyVal[ONE_SIZET] );
        }
        // image centre and pix 2 mm
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_X)//5
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::X_CENTER,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_Y)//6
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::Y_CENTER,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIX_MM)//7
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::PIX_MM,keyVal[ONE_SIZET]);
        }
        // how the image is decimated before anlaysis ?????????
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_STEP_SIZE)//8
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::STEP_SIZE,keyVal[ONE_SIZET]);
        }
        // actual cam image data
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_DATA)//9
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::ARRAY_DATA,keyVal[ONE_SIZET]);
        }
        // mask setters
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X)//1ZERO_SIZET
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_X,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y)//11
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_Y,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X_RAD)//12
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_X_RAD,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y_RAD)//13
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_Y_RAD,keyVal[ONE_SIZET]);
        }
        //
        // first 15 are in cameraImageDataConfigReader
        if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_START_ACQUIRE)//14
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_START_ACQUIRE, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_STOP_ACQUIRE)//15
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_STOP_ACQUIRE, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_PATH)//16
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_FILE_PATH, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_NAME)//17
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_FILE_NAME, keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_CAPTURE  )//18
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::CAM_CAPTURE,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_NUMBER)//19
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_FILE_NUMBER, keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_WRITE  )//20
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::CAM_FILE_WRITE,keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_NUM_CAPTURE  )//21
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::CAM_NUM_CAPTURE,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_PATH_J)//22
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::JPG_FILE_PATH, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_NAME_J)//23
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::JPG_FILE_NAME, keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_CAPTURE_J  )//24
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::JPG_CAPTURE,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_NUMBER_J)//25
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::JPG_FILE_NUMBER, keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_WRITE_J)//26
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::JPG_FILE_WRITE, keyVal[ONE_SIZET] );
        }
        else if( keyVal[ZERO_SIZET] == UTL::PV_DAQ_SUFFIX_NUM_CAPTURE_J  )//27
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::JPG_NUM_CAPTURE,keyVal[ONE_SIZET] );
        }
        // this is not used, what is it ??????????????????????????????
        else if(keyVal[ZERO_SIZET] == PV_DAQ_SUFFIX_FILE_TEMPLATE)//30????????????????
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_FILE_TEMPLATE, keyVal[ONE_SIZET] );
        }
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvComStructs, keyVal );
}
//______________________________________________________________________________
void claraCameraConfigReader::addPVStruct(std::vector<cameraStructs::pvStruct>& pvs,
                                      cameraStructs::CAM_PV_TYPE pvType,
                                      const std::string& pvSuffix)
{
    pvs.push_back(cameraStructs::pvStruct());
    pvs.back().pvType      = pvType;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    debugMessage(pvs.size(),": Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
}

//______________________________________________________________________________
bool claraCameraConfigReader::getCamData(  std::map<std::string, cameraStructs::cameraObject> & mapToFill )
{
    bool success = true;
    mapToFill.clear();
    for(auto&& it : camObjects)
    {
        mapToFill[ it.name ] = it;
        for(auto&& it2 : pvMonStructs)
            mapToFill.at(it.name).pvMonStructs[ it2.pvType ] = it2;

        for(auto&& it2 : pvComStructs)
            mapToFill.at(it.name).pvComStructs[ it2.pvType ] = it2;
    }
    return success;
}
//______________________________________________________________________________
void claraCameraConfigReader::addToCameraImageDataObjects(const std::vector<std::string> & keyVal )
{
    using namespace cameraStructs;
    using namespace UTL;
    std::string value = keyVal[ONE_SIZET];//MAGIC_NUMBER
    if( keyVal[ZERO_SIZET] == NAME )
    {
        camObjects.push_back(cameraObject() );
        camObjects.back().name      = value;

       // camDAQObject.back().numIlocks = (size_t)numIlocks;
        debugMessage("Added camera ", camObjects.back().name );
    }
    else if( keyVal[ZERO_SIZET] == PV_ROOT )
    {
        if( useVM )
            camObjects.back().pvRoot = VM_PREFIX + value;
        else
            camObjects.back().pvRoot = value;
    }
    else if( keyVal[ZERO_SIZET] == SCREEN_NAME )
    {
        if( useVM )
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
        camObjects.back().data.analysis.x_centre_def  = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == Y_CENTER_DEF)
    {
        camObjects.back().data.analysis.x_centre_def  = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == PIX_2_MM_RATIO_DEF)
    {
        camObjects.back().data.analysis.pix_2_mm_def  = getNumD(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_NUM_PIX_X)
    {
        camObjects.back().data.image.num_pix_x  = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_NUM_PIX_Y)
    {
        camObjects.back().data.image.num_pix_y = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_X_PIX_2_MM)
    {
        camObjects.back().data.image.x_pix_to_mm  = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == ARRAY_DATA_Y_PIX_2_MM)
    {
        camObjects.back().data.image.y_pix_to_mm = getNumD(value);
    }
    else if(keyVal[ZERO_SIZET] == IP_ADDRESS_STREAM)
    {
        camObjects.back().streamingIPAddress = value;
    }
    else if(keyVal[ZERO_SIZET] == MAX_SHOTS_NUMBER)
    {
        camObjects.back().daq.maxShots = getNum(value);
    }
    else if(keyVal[ZERO_SIZET] == SENSOR_MIN_TEMP)
    {
        camObjects.back().daq.sensorMinTemp = getNumD(value);
    }
    else if(keyVal[ZERO_SIZET] == SENSOR_MAX_TEMP)
    {
        camObjects.back().daq.sensorMaxTemp = getNumD(value);
    }
    else if(keyVal[ZERO_SIZET] == X)
    {
        camObjects.back().data.analysis.pixel_values_pos[getNum(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == Y)
    {
        camObjects.back().data.analysis.pixel_values_pos[getNum(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == X_SIGMA)
    {
        camObjects.back().data.analysis.pixel_values_pos[getNum(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == Y_SIGMA)
    {
        camObjects.back().data.analysis.pixel_values_pos[getNum(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == XY_SIGMA)
    {
        camObjects.back().data.analysis.pixel_values_pos[getNum(value)] = keyVal[ZERO_SIZET];
    }
    else if(keyVal[ZERO_SIZET] == UTL::CAM_TYPE)
    {
        camObjects.back().type = getCameType(keyVal[ZERO_SIZET]);
    }
}
//______________________________________________________________________________
cameraStructs::CAM_TYPE claraCameraConfigReader::getCameType(const std::string& value)
{
    if(value == ENUM_TO_STRING(cameraStructs::CAM_TYPE::VELA_CAM))
       return cameraStructs::CAM_TYPE::VELA_CAM;
    if(value == ENUM_TO_STRING(cameraStructs::CAM_TYPE::CLARA_CAM))
        return cameraStructs::CAM_TYPE::CLARA_CAM;
    return cameraStructs::CAM_TYPE::UNKNOWN_CAM;
}

