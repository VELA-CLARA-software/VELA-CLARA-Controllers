//djs
#include "gunModConfigReader.h"
#include "configDefinitions.h"
#include "rfModStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

gunModConfigReader::gunModConfigReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
gunModConfigReader::gunModConfigReader(const std::string& configFileLocation1,
                                       const bool startVirtualMachine,
                                       const bool* show_messages_ptr,
                                       const bool * show_debug_messages_ptr  ):
configReader( configFileLocation1, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
gunModConfigReader::~gunModConfigReader(){}
////______________________________________________________________________________
bool gunModConfigReader::getGunModObject(rfModStructs::gunModObject & obj )
{
//    rfModStructs::rfObject obj;

//    obj.name = "VELA_RF_GUN"; /// MAGIC NAME!
//
//    obj.mod = RFModObject;

    obj = RFModObject;



    /// In the final object the pvStructs are in a map keyed by PV Type...

    for(auto && it : pvMonStructs )
        obj.pvMonStructs[ it.pvType ] = it;

    for(auto && it : pvComStructs )
        obj.pvComStructs[ it.pvType ] = it;



//    obj.mod.badModErrorReadStr = RFModObject.badModErrorReadStr;
//    obj.mod.goodModErrorReadStr = RFModObject.goodModErrorReadStr;

    /// rfPowerObjects are in a map keyed by name ...

//    for(auto && it : rfPowerObjects )
//        obj.powerObjects[ it.name ] = it;

//    /// Then we add in the monitor structs, for each power object ...
//
//    for(auto && it : obj.powerObjects )
//        for( auto it2 : pvPWRMonStructs )
//            it.second.pvMonStructs[ it2.pvType ] = it2;
//    /// LLRF
//    obj.llrf = RFLLRFObject;
//    for(auto && it : pvLLRFMonStructs )
//        obj.llrf.pvMonStructs[ it.pvType ] = it;

    return true;
}
////______________________________________________________________________________
bool gunModConfigReader::readConfig()
{
    debugMessage( "\n", "**** Attempting to Read Gun Modulator Config File ****" );

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
                                        addToModObjectsV1( keyVal );

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
        message( "!!!! Error Can't Open  Gun Modulator Config File after searching in:  ", configFile1, " !!!!"  );
    }
    return success;
    return false;
}
//______________________________________________________________________________
void gunModConfigReader::addToPVMonitorMapV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvMonStructs, keyVal);
}
//______________________________________________________________________________
void gunModConfigReader::addToPVCommandMapV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvComStructs, keyVal);
}
//______________________________________________________________________________
void gunModConfigReader::addToModObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        //rfModStructs::rfModObject rfob = rfModStructs::rfModObject();
        RFModObject.name = keyVal[ 1 ];
        debugMessage("Added ", RFModObject.name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        RFModObject.pvRoot = keyVal[ 1 ];

//    else if( keyVal[0] == UTL::GOOD_MOD_ERR )
//    {
//        RFModObject.goodModErrorReadStr.push_back(  keyVal[ 1 ] );
//        debugMessage("Added ", RFModObject.goodModErrorReadStr.back(), " to good ErrorRead Codes." );
//    }
//
//    else if( keyVal[0] == UTL::BAD_MOD_ERR )
//    {
//        RFModObject.badModErrorReadStr.push_back(  keyVal[ 1 ] );
//        debugMessage("Added ", RFModObject.badModErrorReadStr.back(), " to bad ErrorRead Codes." );
//    }
}
////______________________________________________________________________________
void gunModConfigReader::addToPVStruct( std::vector< rfModStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( rfModStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        //GUN  Modulator

        if( keyVal[0] == UTL::PV_SUFFIX_GUN_MOD_RESET  )
            pvStruct_v.back().pvType = rfModStructs::GUN_MOD_PV_TYPE::MOD_RESET;

        else if(keyVal[0] == UTL::PV_SUFFIX_GUN_MOD_STATESET  )
            pvStruct_v.back().pvType = rfModStructs::GUN_MOD_PV_TYPE::MOD_STATE_SET;

        else if( keyVal[0] == UTL::PV_SUFFIX_GUN_MOD_STATEREAD  )
            pvStruct_v.back().pvType = rfModStructs::GUN_MOD_PV_TYPE::MOD_STATE_READ;

        else if( keyVal[0] == UTL::PV_SUFFIX_GUN_MOD_EXILOCK1  )
            pvStruct_v.back().pvType = rfModStructs::GUN_MOD_PV_TYPE::MOD_ERROR_READ;

        else if( keyVal[0] == UTL::PV_SUFFIX_GUN_MOD_EXILOCK2 )
            pvStruct_v.back().pvType = rfModStructs::GUN_MOD_PV_TYPE::MOD_ERROR_READ_STR;

        else if( keyVal[0] == UTL::PV_SUFFIX_GUN_MOD_WARMUPT )
            pvStruct_v.back().pvType = rfModStructs::GUN_MOD_PV_TYPE::MOD_WARMUP_TIME;
         debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void gunModConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< rfModStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
////______________________________________________________________________________
//bool gunModConfigReader::readConfig( gunModConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
//{
//    debugMessage( "\n", "**** Attempting to Read ", fn, " ****" );
//
//    std::string line, trimmedLine;
//    bool success = false;
//
//    std::ifstream inputFile;
//
//    inputFile.open( fn, std::ios::in );
//    if( inputFile )
//    {
//        bool readingData       = false;
//        bool readingObjs       = false;
//        bool readingCommandPVs = false;
//        bool readingMonitorPVs = false;
//
//        debugMessage( "File Opened from ", fn );
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
//                                        CALL_MEMBER_FN(obj, f1)( keyVal ) ;
//
//                                    else if ( readingCommandPVs  )
//                                        CALL_MEMBER_FN(obj, f2)( keyVal ) ;
//
//                                    else if ( readingMonitorPVs )
//                                        CALL_MEMBER_FN(obj, f3)( keyVal ) ;
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
//        success = true;
//    }
//    else{
//        message( "!!!! Error Can't Open File after searching for:  ", fn, " !!!!"  );
//    }
//    return success;
//}
////______________________________________________________________________________
//
//
//
