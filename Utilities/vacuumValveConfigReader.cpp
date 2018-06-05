//djs
#include "vacuumValveConfigReader.h"
#include "vacuumValveStructs.h"
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
vacuumValveConfigReader::vacuumValveConfigReader( const std::string & vacValveConf,
                                                  const bool& show_messages_ptr,
                                                  const bool& show_debug_messages_ptr,
                                                  const bool startVirtualMachine ):
vacValveConf( vacValveConf ),
configReader( vacValveConf, show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{

}
////______________________________________________________________________________
//vacuumValveConfigReader::vacuumValveConfigReader( const std::string & configFile_Location,
//                                                  const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
//:  configReader( configFile_Location, show_messages_ptr, show_debug_messages_ptr )
//{
//}
//______________________________________________________________________________
vacuumValveConfigReader::~vacuumValveConfigReader(){}
//______________________________________________________________________________
bool vacuumValveConfigReader::readConfigFiles()
{
    bool success = true;
    pvMonStructs.clear();
    pvComStructs.clear();
    vacValveObjects.clear();
    bool vacValveSuccess = readConfig( *this, vacValveConf, &vacuumValveConfigReader::addToVacValveObjectsV1, &vacuumValveConfigReader::addToPVCommandMapV1, &vacuumValveConfigReader::addToPVMonitorMapV1 );
    if( !vacValveSuccess )
        success = false;
    if( numObjs == vacValveObjects.size() )
        debugMessage( "*** Created ", numObjs, " Vac valve Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", vacValveObjects.size() ," Vac valve Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }

    return success;
}
//______________________________________________________________________________
bool vacuumValveConfigReader::readConfig( vacuumValveConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
{
    debugMessage( "\n", "**** Attempting to Read ", fn, " ****" );

    std::string line, trimmedLine;
    bool success = false;

    std::ifstream inputFile;

    inputFile.open( fn.c_str(), std::ios::in );
    if( inputFile )
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage( "File Opened from ", fn );
        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
            if( trimmedLine.size() > 0 )
            {
                //message("trimmedLine = ", trimmedLine);
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
                                    {
                                        CALL_MEMBER_FN(obj, f1)( keyVal ) ;
                                    }

                                    else if ( readingCommandPVs  )
                                    {
                                        CALL_MEMBER_FN(obj, f2)( keyVal ) ;
                                    }

                                    else if ( readingMonitorPVs )
                                    {
                                        CALL_MEMBER_FN(obj, f3)( keyVal ) ;
                                    }
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
        message( "!!!! Error Can't Open Shutter Config File after searching in:  ", fn, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
void vacuumValveConfigReader::addToPVMonitorMapV1( const std::vector<std::string> &keyVal  )
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
void vacuumValveConfigReader::addToPVCommandMapV1( const std::vector<std::string> &keyVal  )
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
void vacuumValveConfigReader::addPVStruct( std::vector< vacuumValveStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
{
    /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
    /// init structs 'correctly'
    pvStruct_v.push_back( vacuumValveStructs::pvStruct() );
    pvStruct_v.back().pvSuffix = keyVal[1];
    if( keyVal[0] == UTL::PV_SUFFIX_ON  )
        pvStruct_v.back().pvType = vacuumValveStructs::VAC_VALVE_PV_TYPE::On;
    else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
        pvStruct_v.back().pvType = vacuumValveStructs::VAC_VALVE_PV_TYPE::Off;
    else if( keyVal[0] == UTL::PV_SUFFIX_STA  )
        pvStruct_v.back().pvType = vacuumValveStructs::VAC_VALVE_PV_TYPE::Sta;

    debugMessage("Added ", pvComStructs.back().pvSuffix, " suffix" );
}
//______________________________________________________________________________
void vacuumValveConfigReader::addToVacValveObjectsV1( const std::vector<std::string> &keyVal   )
{
    //std::cout << "addToVacValveObjectsV1 called " <<std::endl;
    if( keyVal[0] == UTL::NAME )
    {
        /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
        /// init structs 'correctly'

        vacuumValveStructs::vacValveObject valob = vacuumValveStructs::vacValveObject();
        valob.name = keyVal[ 1 ];
        valob.vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_ERROR;
        valob.numIlocks = numIlocks;

        vacValveObjects.push_back( valob );

        debugMessage("Added ", vacValveObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        vacValveObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________
const std::vector< vacuumValveStructs::vacValveObject > vacuumValveConfigReader::getVacValveObjects()
{
    /// turn the pv vectors into maps, you can probably figure a way of doing this in
    /// addPVStruct, but i like using vector.back()

    std::map< vacuumValveStructs::VAC_VALVE_PV_TYPE, vacuumValveStructs::pvStruct > pvMonStructs_m;
    std::map< vacuumValveStructs::VAC_VALVE_PV_TYPE, vacuumValveStructs::pvStruct > pvComStructs_m;

    for( auto && it : pvMonStructs )
        pvMonStructs_m[ it.pvType ] = it;

    for( auto && it : pvComStructs )
        pvComStructs_m[ it.pvType ] = it;

    for( auto && it : vacValveObjects )
    {
        it.pvComStructs = pvComStructs_m;
        it.pvMonStructs = pvMonStructs_m;
    }
    return vacValveObjects;
}
//______________________________________________________________________________
//bool vacuumValveConfigReader::readVacValveConfig()
//{
//    debugMessage( "\n", "**** Attempting to Read The VacValve Config File ****" );
//
//    std::string line, trimmedLine;
//
//    bool success = false;
//
//    pvComStructs.clear();
//    pvMonStructs.clear();
//
//    std::ifstream inputFile;
//    inputFile.open( configFile1, std::ios::in );
//    if( inputFile )
//    {
//        bool readingData       = false;
//        bool readingObjs       = false;
//        bool readingCommandPVs = false;
//        bool readingMonitorPVs = false;
//
//        debugMessage( "File Opened from ", configFile1 );
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
//
//                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_OBJECTS ) )
//                        getNumObjs( trimmedLine );
//
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
//                                        addToVacValveObjectsV1( keyVal );
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
//        if( numObjs == vacValveObjects.size() )
//            debugMessage( "*** Created ", numObjs, " VacValve Objects, As Expected ***" );
//        else
//            debugMessage( "*** Created ", vacValveObjects.size() ," Expected ", numObjs,  " ERROR ***"  );
//
//        success = true;
//    }
//    else{
//        message( "!!!! Error Can't Open File after searching for:  ", configFile1, " !!!!"  );
//    }
//    return success;
//}
