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
//  Last edit:   29-03-2018
//  FileName:    virtualCathodeConfigReader.h
//  Description:
//
//
//*/
// project includes
#include "virtualCathodeConfigReader.h"
#include "pilaserStructs.h"
// stl includes
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//____________________________________________________________________________________________________
virtualCathodeConfigReader::virtualCathodeConfigReader(const std::string& configFile,
                           const bool& show_messages,
                           const bool& show_debug_messages,
                           const bool usingVM):
configReader(configFile,  show_messages, show_debug_messages,usingVM){}
//______________________________________________________________________________
virtualCathodeConfigReader::~virtualCathodeConfigReader(){}
//______________________________________________________________________________
bool virtualCathodeConfigReader::readConfig()
{
    debugMessage( "\n", "**** Attempting to Read virtualCathodeData Config File ****" );

    std::string line, trimmedLine;
    bool success = false;

    pvMonStructs.clear();
    std::ifstream inputFile;

    inputFile.open( configFile1, std::ios::in );
    if( inputFile )
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage( "Opened from ", configFile1 );
        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
            if( trimmedLine.size() > UTL::ZERO_SIZET )
            {
                if( stringIsSubString( line, UTL::END_OF_DATA ) )
                {
                    debugMessage( "Found END_OF_DATA" );
                    readingData = false;
                    readingObjs = false;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    break;
                }
                if( readingData )
                {
                    if( stringIsSubString( trimmedLine, UTL::VERSION ) )
                        getVersion( trimmedLine );
                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_OBJECTS ) )
                        getNumObjs( trimmedLine );
                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_ILOCKS ) )
                        getNumIlocks( trimmedLine );
                    else
                    {
                        switch( configVersion )
                        {
                            case 1:
                                if( trimmedLine.find_first_of( UTL::EQUALS_SIGN ) != std::string::npos )
                                {
                                    std::vector<std::string> keyVal = getKeyVal( trimmedLine );

                                    if( readingObjs )
                                        addToObjectsV1( keyVal );

//                                    else if ( readingCommandPVs  )
//                                        addToPVCommandMapV1( keyVal );

                                    else if ( readingMonitorPVs )
                                        addToPVMonitorMapV1( keyVal );
                                }
                                break;
                            default:
                                message( "!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
                        }
                    }
                }
                if( stringIsSubString( line, UTL::START_OF_DATA ) )
                {
                    readingData = true;
                    debugMessage( "Found START_OF_DATA" );
                }
                if( stringIsSubString( line, UTL::PV_COMMANDS_START ) )
                {
                    readingCommandPVs  = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage( "Found PV_COMMANDS_START" );
                }
                if( stringIsSubString( line, UTL::PV_MONITORS_START ) )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage( "Found PV_MONITORS_START" );
                }
                if( stringIsSubString( line, UTL::OBJECTS_START ) )
                {
                    readingObjs        = true;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    debugMessage( "Found OBJECTS_START" );
                }
            }
        }
        inputFile.close( );
        debugMessage( "File Closed" );

        success = true;
    }
    else{
        message( "!!!! Error Can't Open PILaser Config File after searching in:  ", configFile1, " !!!!"  );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToPVMonitorMapV1(const std::vector<std::string>&keyVal)
{
    using namespace UTL;
    if( stringIsSubString(keyVal[UTL::ZERO_SIZET], "SUFFIX"))
    {
        using namespace virtualCathodeStructs;
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_X)
        {
            addToPVStruct(pvMonStructs, PV_TYPE::X_RBV,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_Y)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::Y_RBV,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_SIGMA_X)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::SIGMA_X_RBV,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_SIGMA_Y)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::SIGMA_Y_RBV,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_X_PIX)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::X_PIX,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_Y_PIX)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::Y_PIX,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_SIGMA_X_PIX)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::SIGMA_X_PIX,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_SIGMA_Y_PIX)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::SIGMA_Y_PIX,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_COV_XY)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::COV_XY_RBV,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_COV_XY_PIX)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::COV_XY_PIX,keyVal[ONE_SIZET] );
        }
        if(keyVal[UTL::ZERO_SIZET] == UTL::PV_IA_SUFFIX_SUM_PIX_INTENSITY)
        {
            addToPVStruct(pvMonStructs,PV_TYPE::VC_INTENSITY,keyVal[ONE_SIZET] );
        }
    }
    else
    {
        if( keyVal[UTL::ZERO_SIZET] == UTL::PV_COUNT)
        {
            pvMonStructs.back().COUNT = getCOUNT( keyVal[ONE_SIZET] );
        }
        else if( keyVal[UTL::ZERO_SIZET] == UTL::PV_MASK )
        {
            pvMonStructs.back().MASK = getMASK( keyVal[ONE_SIZET] );
        }
        else if( keyVal[UTL::ZERO_SIZET] == UTL::PV_CHTYPE )
        {
            pvMonStructs.back().CHTYPE = getCHTYPE( keyVal[ONE_SIZET]);
        }
    }
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToPVStruct(std::vector<virtualCathodeStructs::pvStruct>& pvs,
                                               const virtualCathodeStructs::PV_TYPE pvtype,
                                               const std::string& pvSuffix)
{
    pvs.push_back( virtualCathodeStructs::pvStruct() );
    pvs.back().pvType      = pvtype;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix,
                 " suffix for ", ENUM_TO_STRING( pvs.back().pvType) );
}
//______________________________________________________________________________
void virtualCathodeConfigReader::addToObjectsV1(const std::vector<std::string>& keyVal)
{
    if( keyVal[UTL::ZERO_SIZET] == UTL::NAME )
    {
        object.name = keyVal[UTL::ONE_SIZET];
        //object.numIlocks = numIlocks;
        debugMessage("Added ", object.name );
    }
    else if(keyVal[UTL::ZERO_SIZET] == UTL::PV_ROOT)
    {
        if( useVM )
            object.pvRoot =  UTL::VM_PREFIX + keyVal[UTL::ONE_SIZET];
        else
            object.pvRoot = keyVal[UTL::ONE_SIZET];
    }
}
//______________________________________________________________________________
bool virtualCathodeConfigReader::getVirtualCathodeDataObject(
                virtualCathodeStructs::virtualCathodeDataObject& obj)
{
    obj = object;
    for(auto && it : pvMonStructs)
    {
        obj.pvMonStructs[it.pvType] = it;
    }
    return true;
}




