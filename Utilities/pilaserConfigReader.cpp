//djs
#include "pilaserConfigReader.h"
#include "pilaserStructs.h"
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
pilaserConfigReader::pilaserConfigReader( const std::string & configFileLocation1,
                                        const bool startVirtualMachine,
                                        const bool* show_messages_ptr,
                                        const bool* show_debug_messages_ptr ):
configReader( configFileLocation1, show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{
//    std::cout << configFileLocation1 << std::endl;
}
//______________________________________________________________________________
pilaserConfigReader::~pilaserConfigReader(){}
//______________________________________________________________________________
bool pilaserConfigReader::readConfig()
{
    debugMessage( "\n", "**** Attempting to Read PILaser Config File ****" );

    std::string line, trimmedLine;
    bool success = false;

    pvComStructs.clear();
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
            if( trimmedLine.size() > 0 )
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
                                        addTopilaserObjectsV1( keyVal );

                                    else if ( readingCommandPVs  )
                                        addToPVCommandMapV1( keyVal );

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
void pilaserConfigReader::addToPVMonitorMapV1( const  std::vector<std::string> &keyVal  )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        // there are different types of screen, here we are hardcoding in how to interpret the config file and where to put
        // each type of monitor.
        if( keyVal[0] == UTL::PV_SUFFIX_LASER_V_POS  )
        {
            addToPVStruct( pvMonStructs, pilaserStructs::PILASER_PV_TYPE::V_POS,keyVal[1] );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_LASER_H_POS  )
        {
            addToPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::H_POS,keyVal[1] );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_LASER_INTENSITY  )
        {
            addToPVStruct(pvMonStructs, pilaserStructs::PILASER_PV_TYPE::INTENSITY,keyVal[1] );
        }
    }
    else
    {
        if( keyVal[0] == UTL::PV_COUNT )
            pvMonStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
        else if( keyVal[0] == UTL::PV_MASK )
            pvMonStructs.back().MASK = getMASK( keyVal[ 1 ] );
        else if( keyVal[0] == UTL::PV_CHTYPE )
            pvMonStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
    }
}
//______________________________________________________________________________
void pilaserConfigReader::addToPVStruct(std::vector< pilaserStructs::pvStruct > & pvs, const pilaserStructs::PILASER_PV_TYPE pvtype, const std::string& pvSuffix)
{
    pvs.push_back( pilaserStructs::pvStruct() );
    pvs.back().pvType      = pvtype;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    //lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvs.back().pvType) );
}
//______________________________________________________________________________
void pilaserConfigReader::addToPVCommandMapV1( const  std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_SUFFIX_ON )
        addPVStruct( pvComStructs, keyVal );
    else if( keyVal[0] == UTL::PV_SUFFIX_OFF )
        addPVStruct( pvComStructs, keyVal );
    else if( keyVal[0] == UTL::PV_COUNT )
        pvComStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvComStructs.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvComStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
void pilaserConfigReader::addTopilaserObjectsV1( const std::vector<std::string> &keyVal   )
{
    if( keyVal[0] == UTL::NAME )
    {
        pilaserObject.name = keyVal[ 1 ];
        pilaserObject.numIlocks = numIlocks;
        debugMessage("Added ", pilaserObject.name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            pilaserObject.pvRoot =  UTL::VM_PREFIX + keyVal[ 1 ];
        else
            pilaserObject.pvRoot = keyVal[ 1 ];
    }
}
//______________________________________________________________________________
bool pilaserConfigReader::getpilaserObject( pilaserStructs::pilaserObject & obj )
{
    obj = pilaserObject;
    for(auto && it : pvMonStructs)
    {
        obj.pvMonStructs[it.pvType] = it;
    }
    for(auto && it : pvComStructs)
    {
        obj.pvComStructs[it.pvType] = it;
    }
    return true;
}



























