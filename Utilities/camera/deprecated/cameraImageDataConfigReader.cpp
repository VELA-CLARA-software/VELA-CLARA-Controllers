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
//  Last edit:   05-06-2018
//  FileName:    cameraImageDataConfigReader.cpp
//  Description: This class reads in all the analysis PVs for analysing
//               camera image data
//
//
//*/
//djs
#include "cameraImageDataConfigReader.h"
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

cameraImageDataConfigReader::cameraImageDataConfigReader(const std::string& configFile,
                                                       const bool& show_messages,
                                                       const bool& show_debug_messages,
                                                       const bool usingVM):
configReader(configFile,  show_messages, show_debug_messages,usingVM)
{
}
//______________________________________________________________________________
cameraImageDataConfigReader::~cameraImageDataConfigReader(){}
//______________________________________________________________________________
bool cameraImageDataConfigReader::readConfig()
{
    debugMessage("\n", "**** Attempting to Read cameraImageData  Config File ****" );

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
                    debugMessage("Found PV_COMMANDS_START" );
                }
                if(stringIsSubString(line, UTL::PV_MONITORS_START))
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage("Found PV_MONITORS_START" );
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
void cameraImageDataConfigReader::addToPVMonitorMapV1(const std::vector<std::string>& keyVal)
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX"))
    {
        using namespace cameraStructs;
        if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_X_RBV)//1
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::X_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_Y_RBV)//2
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::Y_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_X_RBV)//3
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SIGMA_X_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_SIGMA_Y_RBV)//4
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::SIGMA_Y_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_X_PIX_RBV)//5
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::X_PIX_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_Y_PIX_RBV)//6
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::Y_PIX_RBV,keyVal[ONE_SIZET] );
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
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIXEL_RESULTS_RBV)//12
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::PIX_INTENSITY_RBV,keyVal[ONE_SIZET]);
        }
        // mask parameters
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X_RBV)//13
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_X_RBV,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y_RBV)//14
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_Y_RBV,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X_RAD_RBV)//15
        {
            addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_X_RAD_RBV,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y_RAD_RBV)//16
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::MASK_Y_RAD_RBV,keyVal[ONE_SIZET]);
        }
        // image centre and pix 2 mm
        //
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIX_MM_RBV)//17
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::PIX_MM_RBV,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_X_RBV)//18
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::X_CENTER_RBV,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_Y_RBV)//19
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::Y_CENTER_RBV,keyVal[ONE_SIZET]);
        }
        //
        // image analysis States
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_ACQUIRE_RBV  )//20
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::CAM_ACQUIRE_RBV,keyVal[ONE_SIZET]);
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_B_RBV  )//21
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::USE_BKGRND_RBV,keyVal[ONE_SIZET]);
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_NPOINT_RBV  )//22
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::USE_NPOINT_RBV,keyVal[ONE_SIZET]);
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_START_IA_RBV  )//23
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::START_IA_RBV,keyVal[ONE_SIZET]);
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_STATE  )//24
        {
             addPVStruct(pvMonStructs,CAM_PV_TYPE::CAM_STATUS,keyVal[ONE_SIZET]);
        }
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvMonStructs, keyVal);
}
//______________________________________________________________________________
void cameraImageDataConfigReader::addCOUNT_MASK_OR_CHTYPE(std::vector<cameraStructs::pvStruct>& pvStruct_v,
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
void cameraImageDataConfigReader::addToPVCommandMapV1(const  std::vector<std::string> &keyVal  )
{
    using namespace UTL;
    if(stringIsSubString(keyVal[ZERO_SIZET], "SUFFIX" ) )
    {
        using namespace cameraStructs;
        if( keyVal[0] == UTL::PV_IA_SUFFIX_START_ACQUIRE  )//1
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_START_ACQUIRE, keyVal[ONE_SIZET] );
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_STOP_ACQUIRE  )//2
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::CAM_START_ACQUIRE, keyVal[ONE_SIZET] );
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_START_IA  )//3
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::START_IA, keyVal[ONE_SIZET] );
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_SET_B  )//4
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::SET_BKGRND,keyVal[ONE_SIZET] );
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_B  )//5
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::USE_BKGRND,keyVal[ONE_SIZET] );
        }
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_USE_NPOINT  )//6
        {
            addPVStruct(pvMonStructs, CAM_PV_TYPE::USE_NPOINT,keyVal[ONE_SIZET] );
        }
        // image centre and pix 2 mm
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_X)//7
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::X_CENTER,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_CENTER_Y)//8
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::Y_CENTER,keyVal[ONE_SIZET]);
        }
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_PIX_MM)//9
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::PIX_MM,keyVal[ONE_SIZET]);
        }
        // how the image is decimated before anlaysis ?????????
        else  if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_STEP_SIZE)//10
        {
             addPVStruct(pvComStructs,CAM_PV_TYPE::STEP_SIZE,keyVal[ONE_SIZET]);
        }
        // actual cam image data
        else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_SUFFIX_DATA)//11
        {
            addPVStruct(pvComStructs, CAM_PV_TYPE::ARRAY_DATA,keyVal[ONE_SIZET]);
        }
        // mask setters
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X)//12
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_X,keyVal[ONE_SIZET] );
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y)//13
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_Y,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_X_RAD)//14
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_X_RAD,keyVal[ONE_SIZET]);
        }
        else if(keyVal[ZERO_SIZET] == PV_IA_SUFFIX_MASK_Y_RAD)//15
        {
            addPVStruct(pvComStructs,CAM_PV_TYPE::MASK_Y_RAD,keyVal[ONE_SIZET]);
        }
    }
    else
        addCOUNT_MASK_OR_CHTYPE(pvComStructs, keyVal );
}
//______________________________________________________________________________
void cameraImageDataConfigReader::addPVStruct(std::vector< cameraStructs::pvStruct>& pvs,
                                      cameraStructs::CAM_PV_TYPE pvType,
                                      const std::string& pvSuffix)
{
    pvs.push_back(cameraStructs::pvStruct());
    pvs.back().pvType      = pvType;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING(pvs.back().pvType));
}



//______________________________________________________________________________
bool cameraImageDataConfigReader::getCamData(  std::map< std::string, cameraStructs::cameraObject > & mapToFill )
{
    bool success = true;
    mapToFill.clear();

    size_t nCount = 0;
    size_t rCount = 0;

    for( auto && it : camObjects )
    {
        // add in magnetObj
        mapToFill[ it.name ] = it;
        // setMon and Com pvStructs
        for( auto && it2 : pvMonStructs )
            mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;

        for( auto && it2 : pvComStructs )
            mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
    }
    return success;
}

void cameraImageDataConfigReader::addToCameraImageDataObjects(const std::vector<std::string> & keyVal )
{

}












//
//bool cameraImageDataConfigReader::readConfig()
//{
//    if( usingVirtualMachine )
//        debugMessage( "\n", "**** Using VIRTUAL Machine  ****" );
//    else
//        debugMessage(  "**** Using PHYSICAL Machine  ****","\n" );
//
//    std::string line, trimmedLine;
//    bool success = false;
//
//    std::ifstream inputFile;
//
//    inputFile.open( camConfig, std::ios::in );
//    if( inputFile )
//    {
//        bool readingData       = false;
//        bool readingObjs       = false;
//        bool readingCommandPVs = false;
//        bool readingMonitorPVs = false;
//
//        debugMessage( "Opened ", camConfig );
//        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
//        {
//            trimmedLine = trimAllWhiteSpaceExceptBetweenDoubleQuotes( trimToDelimiter( line, UTL::END_OF_LINE ) );
//            if( trimmedLine.size() > 0 )
//            {
//                if( stringIsSubString( line, UTL::END_OF_DATA ) )
//                {
//                    debugMessage( "Found END_OF_DATA" );
//                    readingData = false;
//                    readingObjs = false;
//                    readingCommandPVs  = false;
//                    readingMonitorPVs  = false;
//                    break;
//                }
//                if( readingData )
//                {
//                    if( stringIsSubString( trimmedLine, UTL::VERSION ) )
//                        getVersion( trimmedLine );
//                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_OBJECTS ) )
//                        getNumObjs( trimmedLine );
//                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_ILOCKS ) )
//                        getNumIlocks( trimmedLine );
//                    else
//                    {
//                        switch( configVersion ) /// Actually this switch needs to come out of here and go in the beginning, we should look for version # first!
//                        {
//                            case 1:
//                                if( trimmedLine.find_first_of( UTL::EQUALS_SIGN ) != std::string::npos )
//                                {
//                                    std::vector<std::string> keyVal = getKeyVal( trimmedLine );
//
//                                    if(readingObjs)
//                                        addToCameraObjects(keyVal);
//
//                                    else if (readingCommandPVs)
//                                        addToCameraCommandStructs(keyVal);
//
//                                    else if (readingMonitorPVs)
//                                        addToCameraMonitorStructs(keyVal);
//                                }
//                                break;
//                            default:
//                                message( "!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
//                        }
//                    }
//                }
//                if( stringIsSubString( line, UTL::START_OF_DATA ) )
//                {
//                    readingData = true;
//                    debugMessage( "Found START_OF_DATA" );
//                }
//                if( stringIsSubString( line, UTL::PV_COMMANDS_START ) )
//                {
//                    readingCommandPVs  = true;
//                    readingObjs = false;
//                    readingMonitorPVs = false;
//                    debugMessage( "Found PV_COMMANDS_START" );
//                }
//                if( stringIsSubString( line, UTL::PV_MONITORS_START ) )
//                {
//                    readingCommandPVs = false;
//                    readingObjs       = false;
//                    readingMonitorPVs = true;
//                    debugMessage( "Found PV_MONITORS_START" );
//                }
//                if( stringIsSubString( line, UTL::OBJECTS_START ) )
//                {
//                    readingObjs        = true;
//                    readingCommandPVs  = false;
//                    readingMonitorPVs  = false;
//                    debugMessage( "Found OBJECTS_START" );
//                }
//            }
//        }
//        inputFile.close( );
//        debugMessage( "config File Closed" );
//        success = true;
//    }
//    else{
//        message( "!!!! Error Can't Open Config File after searching for:  ", camConfig, " !!!!"  );
//    }
//    if(success)
//        debugMessage("cameraImageDataConfigReader readconfig is returning TRUE");
//    else
//        debugMessage("cameraImageDataConfigReader readconfig is returning FALSE");
//
//    return success;
//}
//
//void cameraImageDataConfigReader::addPVStruct( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
//{
//    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
//    {
//
//
//        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
//    }
//    else
//        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
//}
////______________________________________________________________________________

////______________________________________________________________________________
//void cameraImageDataConfigReader::addToCameraMonitorStructs( const std::vector<std::string> &keyVal )
//{
//    addPVStruct( pvCameraMonStructs, keyVal);
//}
////______________________________________________________________________________
//void cameraImageDataConfigReader::addToCameraCommandStructs( const std::vector<std::string> &keyVal )
//{
//    addPVStruct( pvCameraComStructs, keyVal);
//}
////______________________________________________________________________________
//void cameraImageDataConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
//{
//
//}
