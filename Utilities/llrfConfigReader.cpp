//djs
#include "llrfConfigReader.h"
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
llrfConfigReader::llrfConfigReader( const std::string & configFileLocation1,
                                        const bool startVirtualMachine,
                                        const bool* show_messages_ptr,
                                        const bool* show_debug_messages_ptr ):
configReader( configFileLocation1, show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{
//    std::cout << configFileLocation1 << std::endl;
}
//______________________________________________________________________________
llrfConfigReader::~llrfConfigReader(){}
//______________________________________________________________________________
bool llrfConfigReader::readConfig()
{
//    debugMessage( "\n", "**** Attempting to Read The Shutter Config File ****" );
//
//    std::string line, trimmedLine;
//    bool success = false;
//
//    pvComStructs.clear();
//    pvMonStructs.clear();
//    std::ifstream inputFile;
//
//    inputFile.open( configFile1, std::ios::in );
//    if( inputFile )
//    {
//        bool readingData       = false;
//        bool readingObjs       = false;
//        bool readingCommandPVs = false;
//        bool readingMonitorPVs = false;
//
//        debugMessage( "Opened from ", configFile1 );
//        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
//        {
//            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
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
//                        switch( configVersion )
//                        {
//                            case 1:
//                                if( trimmedLine.find_first_of( UTL::EQUALS_SIGN ) != std::string::npos )
//                                {
//                                    std::vector<std::string> keyVal = getKeyVal( trimmedLine );
//
//                                    if( readingObjs )
//                                        addTolaserObjectsV1( keyVal );
//
//                                    else if ( readingCommandPVs  )
//                                        addToPVCommandMapV1( keyVal );
//
//                                    else if ( readingMonitorPVs )
//                                        addToPVMonitorMapV1( keyVal );
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
//        debugMessage( "File Closed" );
//
//        if( numObjs == laserObjects.size() )
//            debugMessage( "*** Created ", numObjs, " Shutter Objects, As Expected ***" );
//        else
//            debugMessage( "*** Created ", laserObjects.size() ," Expected ", numObjs,  " ERROR ***"  );
//
//        success = true;
//    }
//    else{
//        message( "!!!! Error Can't Open Shutter Config File after searching in:  ", configFile1, " !!!!"  );
//    }
//    return success;
    return false;
}
//______________________________________________________________________________
void llrfConfigReader::addToPVMonitorMapV1( const  std::vector<std::string> &keyVal  )
{
//    if( keyVal[ 0 ] == UTL::PV_SUFFIX_STA )
//       addPVStruct( pvMonStructs, keyVal );
//    else if( keyVal[0] == UTL::PV_COUNT )
//        pvMonStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_MASK )
//        pvMonStructs.back().MASK = getMASK( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_CHTYPE )
//        pvMonStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
void llrfConfigReader::addToPVCommandMapV1( const  std::vector<std::string> &keyVal  )
{
//    if( keyVal[0] == UTL::PV_SUFFIX_ON )
//        addPVStruct( pvComStructs, keyVal );
//    else if( keyVal[0] == UTL::PV_SUFFIX_OFF )
//        addPVStruct( pvComStructs, keyVal );
//    else if( keyVal[0] == UTL::PV_COUNT )
//        pvComStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_MASK )
//        pvComStructs.back().MASK = getMASK( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_CHTYPE )
//        pvComStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
void llrfConfigReader::addPVStruct( std::vector< llrfStructs::pvStruct > & pvStruct_v,  const  std::vector<std::string> &keyVal )
{
    /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
    /// init structs 'correctly'
//    pvStruct_v.push_back( llrfStructs::pvStruct() );
//    pvStruct_v.back().pvSuffix = keyVal[1];
//    if( keyVal[0] == UTL::PV_SUFFIX_ON  )
//        pvStruct_v.back().pvType = llrfStructs::PIL_SHUTTER_PV_TYPE::On;
//    else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
//        pvStruct_v.back().pvType = llrfStructs::PIL_SHUTTER_PV_TYPE::Off;
//    else if( keyVal[0] == UTL::PV_SUFFIX_STA  )
//        pvStruct_v.back().pvType = llrfStructs::PIL_SHUTTER_PV_TYPE::Sta;
//
//    debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix" );
}
//______________________________________________________________________________
void llrfConfigReader::addTolaserObjectsV1( const std::vector<std::string> &keyVal   )
{
//    if( keyVal[0] == UTL::NAME )
//    {
//        /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
//        /// init structs 'correctly'
//        llrfStructs::laserObject shob = llrfStructs::laserObject();
//        shob.name = keyVal[ 1 ];
//        shob.shutterState = VELA_ENUM::SHUTTER_STATE::SHUTTER_ERROR;
//        shob.numIlocks = numIlocks;
//        laserObjects.push_back( shob );
//        debugMessage("Added ", laserObjects.back().name );
//    }
//    else if( keyVal[0] == UTL::PV_ROOT )
//        laserObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________



