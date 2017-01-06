//djs
#include "shutterConfigReader.h"
#include "shutterStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//BJAS
#include <regex>
//______________________________________________________________________________
shutterConfigReader::shutterConfigReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
shutterConfigReader::shutterConfigReader( const std::string & configFileLocation,
                                                 const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
:  configReader( configFileLocation, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
shutterConfigReader::~shutterConfigReader(){}
//______________________________________________________________________________
bool shutterConfigReader::readPILShutterConfig()
{
  	std::regex config_line("([A-Z_]+?)(?:\\s*=\\s*(.*))?;");
 	std::string parameter;
 	std::string value;
	std::smatch matches;
    std::vector<std::string> keyVal;

    debugMessage( "\n", "**** Attempting to Read The Shutter Config File ****" );

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
//            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
            //BJAS: use regex rather than parsing manually, it's more hygienic
            if( regex_search(line, matches, config_line) )
            {
                parameter = matches[1];
                value = matches[2];
                keyVal.clear();
                keyVal.push_back(parameter);
                keyVal.push_back(value);
                if( stringIsSubString( parameter, UTL::END_OF_DATA ) )
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
                    if( stringIsSubString( parameter, UTL::VERSION ) )
                        getVersion( value );
                    else if( stringIsSubString( parameter, UTL::NUMBER_OF_OBJECTS ) )
                        getNumObjs( value );
                    else if( stringIsSubString( parameter, UTL::NUMBER_OF_ILOCKS ) )
                        getNumIlocks( value );
                    else
                    {
                        switch( configVersion )
                        {
                            case 1:
                                if( readingObjs )
                                    addToShutterObjectsV1( keyVal );

                                else if ( readingCommandPVs  )
                                    addToPVCommandMapV1( keyVal );

                                else if ( readingMonitorPVs )
                                    addToPVMonitorMapV1( keyVal );
                                break;
                            default:
                                message( "!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
                        }
                    }
                }
                if( stringIsSubString( parameter, UTL::START_OF_DATA ) )
                {
                    readingData = true;
                    debugMessage( "Found START_OF_DATA" );
                }
                if( stringIsSubString( parameter, UTL::PV_COMMANDS_START ) )
                {
                    readingCommandPVs  = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage( "Found PV_COMMANDS_START" );
                }
                if( stringIsSubString( parameter, UTL::PV_MONITORS_START ) )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage( "Found PV_MONITORS_START" );
                }
                if( stringIsSubString( parameter, UTL::OBJECTS_START ) )
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

        if( numObjs == shutterObjects.size() )
            debugMessage( "*** Created ", numObjs, " Shutter Objects, As Expected ***" );
        else
            debugMessage( "*** Created ", shutterObjects.size() ," Expected ", numObjs,  " ERROR ***"  );

        success = true;
    }
    else{
        message( "!!!! Error Can't Open Shutter Config File after searching in:  ", configFile1, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
void shutterConfigReader::addToPVMonitorMapV1( const  std::vector<std::string> &keyVal  )
{
    if( keyVal[ 0 ] == UTL::PV_SUFFIX_STA )
       addPVStruct( pvMonStructs, keyVal );
    else if( keyVal[0] == UTL::PV_COUNT )
        pvMonStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvMonStructs.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvMonStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
void shutterConfigReader::addToPVCommandMapV1( const  std::vector<std::string> &keyVal  )
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
void shutterConfigReader::addPVStruct( std::vector< shutterStructs::pvStruct > & pvStruct_v,  const  std::vector<std::string> &keyVal )
{
    /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
    /// init structs 'correctly'
    pvStruct_v.push_back( shutterStructs::pvStruct() );
    pvStruct_v.back().pvSuffix = keyVal[1];
    if( keyVal[0] == UTL::PV_SUFFIX_ON  )
        pvStruct_v.back().pvType = shutterStructs::PIL_SHUTTER_PV_TYPE::On;
    else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
        pvStruct_v.back().pvType = shutterStructs::PIL_SHUTTER_PV_TYPE::Off;
    else if( keyVal[0] == UTL::PV_SUFFIX_STA  )
        pvStruct_v.back().pvType = shutterStructs::PIL_SHUTTER_PV_TYPE::Sta;

    debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix" );
}
//______________________________________________________________________________
void shutterConfigReader::addToShutterObjectsV1( const std::vector<std::string> &keyVal   )
{
    if( keyVal[0] == UTL::NAME )
    {
        /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
        /// init structs 'correctly'
        shutterStructs::shutterObject shob = shutterStructs::shutterObject();
        shob.name = keyVal[ 1 ];
        shob.shutterState = VELA_ENUM::SHUTTER_STATE::SHUTTER_ERROR;
        shob.numIlocks = numIlocks;
        shutterObjects.push_back( shob );
        debugMessage("Added ", shutterObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        shutterObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________
const std::vector< shutterStructs::shutterObject > shutterConfigReader::getShutterObjects()
{
    /// turn the pv vectors into maps, you can probably figure a way of doing this in
    /// addPVStruct, but i like using vector.back()

    std::map< shutterStructs::PIL_SHUTTER_PV_TYPE, shutterStructs::pvStruct > pvMonStructs_m;
    std::map< shutterStructs::PIL_SHUTTER_PV_TYPE, shutterStructs::pvStruct > pvComStructs_m;
    for( auto && it : pvMonStructs )
        pvMonStructs_m[ it.pvType ] = it;

    for( auto && it : pvComStructs )
        pvComStructs_m[ it.pvType ] = it;

    for( auto && it : shutterObjects )
    {
        it.pvComStructs = pvComStructs_m;
        it.pvMonStructs = pvMonStructs_m;
    }
    return shutterObjects;
}



