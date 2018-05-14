//djs
#include "chargeConfigReader.h"
#include "configDefinitions.h"
#include "chargeStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

chargeConfigReader::chargeConfigReader( const std::string & chargeConf1,
                                       const bool& show_messages_ptr,
                                       const bool& show_debug_messages_ptr,
                                       const bool startVirtualMachine ):
chargeConf1( chargeConf1 ),
startVirtualMachine( startVirtualMachine ),
configReader(chargeConf1, show_messages_ptr, show_debug_messages_ptr )
{

}
//______________________________________________________________________________
chargeConfigReader::~chargeConfigReader(){}
//______________________________________________________________________________
chargeStructs::chargeObject chargeConfigReader::getChargeObject()
{
    chargeStructs::chargeObject obj;

    obj.name = "CHARGE";

    for( auto && it : chargeObjects )
        obj.dataObjects[ it.name ] = it;

    for( auto && it : obj.dataObjects )
    {
        for( auto it2 : chargeMonStructs )
        {
            it.second.pvMonStructs[ it2.pvType ] = it2;
        }
    }

    for( auto && it : obj.dataObjects )
    {
        for( auto it2 : chargeComStructs )
        {
            it.second.pvComStructs[ it2.pvType ] = it2;
        }
    }

    return obj;
}
//______________________________________________________________________________
bool chargeConfigReader::readConfigFiles()
{
    bool success = true;
    /// There are 2 types of objects in the charge, a trace monitor and a number monitor;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an chargeObject
    //NUM
    chargeObjects.clear();
    chargeMonStructs.clear();
    chargeComStructs.clear();
    bool numSuccess = readConfig( *this,  chargeConf1, &chargeConfigReader::addToChargeObjectsV1, &chargeConfigReader::addToChargeComStructsV1, &chargeConfigReader::addToChargeMonStructsV1 );
    if( !numSuccess )
        success = false;
//    if( numObjs == chargeTraceDataObject.size() )
    if( numObjs == chargeObjects.size() )
        debugMessage( "*** Created ", numObjs, " charge Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", chargeObjects.size() ," charge Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }

    return success;
}
//______________________________________________________________________________
void chargeConfigReader::addToChargeMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( chargeMonStructs, keyVal );
}
//______________________________________________________________________________
void chargeConfigReader::addToChargeComStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( chargeComStructs, keyVal );
}
//______________________________________________________________________________
void chargeConfigReader::addToChargeObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        chargeStructs::dataObject chob = chargeStructs::dataObject();
        chob.name = keyVal[ 1 ];
        chargeObjects.push_back( chob );
        debugMessage("Added ", chargeObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( startVirtualMachine )
            chargeObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            chargeObjects.back().pvRoot = keyVal[ 1 ];
    }
    else if( keyVal[0] == UTL::DIAG_TYPE )
    {
        chargeObjects.back().diagType = getDiagType( keyVal[ 1 ] );
    }
}
//______________________________________________________________________________
void chargeConfigReader::addToPVStruct( std::vector< chargeStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( chargeStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        if( keyVal[0] == UTL::PV_SUFFIX_CHARGE_Q  )
        {
            pvStruct_v.back().pvType   = chargeStructs::CHARGE_PV_TYPE::Q;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CHARGE_V  )
        {
            pvStruct_v.back().pvType   = chargeStructs::CHARGE_PV_TYPE::V;
        }
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void chargeConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< chargeStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
bool chargeConfigReader::readConfig( chargeConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
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
        message( "!!!! Error Can't Open Shutter Config File after searching for:  ", fn, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
chargeStructs::CHARGE_DIAG_TYPE chargeConfigReader::getDiagType( const std::string & val )
{
    chargeStructs::CHARGE_DIAG_TYPE r;

    if( val == UTL::WCM )
        r = chargeStructs::CHARGE_DIAG_TYPE::WCM;
    else if( val == UTL::ICT1 )
        r = chargeStructs::CHARGE_DIAG_TYPE::VELA_ICT1;
    else if( val == UTL::ICT2 )
        r = chargeStructs::CHARGE_DIAG_TYPE::VELA_ICT2;
    else if( val == UTL::ED_FCUP )
        r = chargeStructs::CHARGE_DIAG_TYPE::ED_FCUP;
    else if( val == UTL::FCUP )
        r = chargeStructs::CHARGE_DIAG_TYPE::S02_FCUP;
    else if( val == UTL::FCUP )
        r = chargeStructs::CHARGE_DIAG_TYPE::SP1_FCUP;

    return r;
}
//______________________________________________________________________________
