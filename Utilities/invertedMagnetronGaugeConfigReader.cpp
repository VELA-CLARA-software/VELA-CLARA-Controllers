//djs
#include "invertedMagnetronGaugeConfigReader.h"
#include "invertedMagnetronGaugeStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//______________________________________________________________________________
invertedMagnetronGaugeConfigReader::invertedMagnetronGaugeConfigReader( const std::string & configFile1,
                                                                        const bool* show_messages_ptr,
                                                                        const bool * show_debug_messages_ptr,
                                                                        const bool startVirtualMachine  ):
configFile1( configFile1 ),
configReader( show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{
}
//______________________________________________________________________________
invertedMagnetronGaugeConfigReader::~invertedMagnetronGaugeConfigReader(){}
//______________________________________________________________________________
bool invertedMagnetronGaugeConfigReader::readVacImgConfig()
{
    debugMessage( "\n", "**** Attempting to Read The Img Config File ****" );

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

        debugMessage( "File Opened from ", configFile1 );
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
                                        addToVacImgObjectsV1( keyVal );

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

        if( numObjs == vacImgObjects.size() )
            debugMessage( "*** Created ", numObjs, " VacImg Objects, As Expected ***" );
        else
            debugMessage( "*** Created ", vacImgObjects.size() ," Expected ", numObjs,  " ERROR ***"  );

        success = true;
    }
    else{
        message( "!!!! Error Can't Open IMG Config File after searching for:  ", configFile1, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
void invertedMagnetronGaugeConfigReader::addToPVMonitorMapV1( std::vector<std::string> &keyVal  )
{
    if( keyVal[ 0 ] == UTL::PV_SUFFIX_STA )
        addPVStruct( pvMonStructs, keyVal );
    else if( keyVal[0] == UTL::PV_VAC_PRESSURE )
        addPVStruct( pvMonStructs, keyVal );
    else if( keyVal[0] == UTL::PV_COUNT )
        pvMonStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvMonStructs.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvMonStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );

}
//______________________________________________________________________________
void invertedMagnetronGaugeConfigReader::addToPVCommandMapV1( std::vector<std::string> &keyVal  )
{
    message("");
//    if( keyVal[0] == UTL::PV_VAC_PRESSURE )
//        addPVStruct( pvComStructs, keyVal );
//    else if( keyVal[0] == UTL::PV_COUNT )
//        pvComStructs.back().COUNT = getCOUNT( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_MASK )
//        pvComStructs.back().MASK = getMASK( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_CHTYPE )
//        pvComStructs.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
void invertedMagnetronGaugeConfigReader::addPVStruct( std::vector< invertedMagnetronGaugeStructs::pvStruct > & pvStruct_v, std::vector<std::string> &keyVal )
{
    /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
    /// init structs 'correctly'
    pvStruct_v.push_back( invertedMagnetronGaugeStructs::pvStruct() );
    pvStruct_v.back().pvSuffix = keyVal[1];
    if( keyVal[0] == UTL::PV_SUFFIX_STA  )
    {
            message("here");
        pvStruct_v.back().pvType = invertedMagnetronGaugeStructs::IMG_PV_TYPE::STA;
        message("here");
        debugMessage("Added ", pvMonStructs.back().pvSuffix, " suffix" );
        message("here");
    }
    else if( keyVal[0] == UTL::PV_VAC_PRESSURE  )
    {
            message("here");
        pvStruct_v.back().pvType = invertedMagnetronGaugeStructs::IMG_PV_TYPE::P;
        debugMessage("Added ", pvMonStructs.back().pvSuffix, " suffix" );
    }

//    debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix" );
}
//______________________________________________________________________________
void invertedMagnetronGaugeConfigReader::addToVacImgObjectsV1( std::vector<std::string> &keyVal   )
{
    if( keyVal[0] == UTL::NAME )
    {
        /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
        /// init structs 'correctly'

        invertedMagnetronGaugeStructs::vacImgObject imgob = invertedMagnetronGaugeStructs::vacImgObject();
        imgob.name = keyVal[ 1 ];
        imgob.vacImgState = invertedMagnetronGaugeStructs::IMG_STATE::IMG_ERROR;
        imgob.numIlocks = numIlocks;

        vacImgObjects.push_back( imgob );

        debugMessage("Added ", vacImgObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        vacImgObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________
const std::vector< invertedMagnetronGaugeStructs::vacImgObject > invertedMagnetronGaugeConfigReader::getVacImgObjects()
{
    /// turn the pv vectors into maps, you can probably figure a way of doing this in
    /// addPVStruct, but i like using vector.back()

    std::map< invertedMagnetronGaugeStructs::IMG_PV_TYPE, invertedMagnetronGaugeStructs::pvStruct > pvMonStructs_m;
    std::map< invertedMagnetronGaugeStructs::IMG_PV_TYPE, invertedMagnetronGaugeStructs::pvStruct > pvComStructs_m;

    for( auto && it : pvMonStructs )
        pvMonStructs_m[ it.pvType ] = it;

    for( auto && it : pvComStructs )
        pvComStructs_m[ it.pvType ] = it;

    for( auto && it : vacImgObjects )
    {
        it.pvComStructs = pvComStructs_m;
        it.pvMonStructs = pvMonStructs_m;
    }
    return vacImgObjects;
}
//______________________________________________________________________________
