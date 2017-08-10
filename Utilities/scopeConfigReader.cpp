//djs
#include "scopeConfigReader.h"
#include "configDefinitions.h"
#include "scopeStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

scopeConfigReader::scopeConfigReader( const std::string & scopeConf1, const std::string & scopeConf2,
                                      const bool *show_messages_ptr, const bool *show_debug_messages_ptr,
                                      const bool startVirtualMachine ):
scopeConf1(scopeConf1), scopeConf2(scopeConf2),
configReader( show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{

}
//______________________________________________________________________________
scopeConfigReader::~scopeConfigReader(){}
//______________________________________________________________________________
scopeStructs::scopeObject scopeConfigReader::getScopeObject()
{
    scopeStructs::scopeObject obj;

    obj.name = "VELA_SCOPE";

//    /// there are only 1 scopeNumObject and 1 scopeTraceDataObject
//
//    obj.numObject = scopeNumObject;
//    for( auto && it : scopeNumMonStructs )
//        obj.numObject.pvMonStructs[ it.pvType ] = it;
//

    for( auto && it : scopeNumObjects )
        obj.numObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each num object ...

    for( auto && it : obj.numObjects )
    {
        for( auto it2 : scopeNumMonStructs )
        {
            it.second.pvMonStructs[ it2.pvType ] = it2;
            it.second.isMonitoringMap[ it2.pvType ] = false;
            it.second.numData[ it2.pvType ] = nums;
            it.second.numTimeStamps[ it2.pvType ] = numtstamps;
            it.second.numStrTimeStamps[ it2.pvType ] = numstrtstamps;
            it.second.shotCounts[ it2.pvType ] = numshotcounts;
        }
    }

    /// scopeTraceDataObjects are in a map keyed by name ...

    for( auto && it : scopeTraceDataObjects )
        obj.traceObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each num object ...

    for( auto && it : obj.traceObjects )
    {
        for( auto it2 : scopeTraceDataMonStructs )
        {
            it.second.pvMonStructs[ it2.pvType ] = it2;
            it.second.isMonitoringMap[ it2.pvType ] = false;
            it.second.traceData[ it2.pvType ] = traces;
            it.second.timeStamps[ it2.pvType ] = tstamps;
            it.second.strTimeStamps[ it2.pvType ] = strtstamps;
            it.second.shotCounts[ it2.pvType ] = shotcounts;
        }
    }

    return obj;
}
//______________________________________________________________________________
bool scopeConfigReader::readConfigFiles()
{
    bool success = true;
    /// There are 2 types of objects in the scope, a trace monitor and a number monitor;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an scopeObject
    //NUM
    scopeNumObjects.clear();
    scopeNumMonStructs.clear();
    bool numSuccess = readConfig( *this,  scopeConf1, &scopeConfigReader::addToScopeNumObjectsV1,nullptr, &scopeConfigReader::addToScopeNumMonStructsV1 );
    if( !numSuccess )
        success = false;
//    if( numObjs == scopeTraceDataObject.size() )
    if( numObjs == scopeNumObjects.size() )
        debugMessage( "*** Created ", numObjs, " scope num Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", scopeNumObjects.size() ," scope num Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }
    //TRACE
    scopeTraceDataObjects.clear();
    scopeTraceDataMonStructs.clear();
    bool traceSuccess = readConfig( *this,  scopeConf2, &scopeConfigReader::addToScopeTraceDataObjectsV1,nullptr, &scopeConfigReader::addToScopeTraceDataMonStructsV1 );
    if( !traceSuccess )
        success = false;
//    if( numObjs == scopeTraceDataObject.size() )
    if( numObjs == scopeTraceDataObjects.size() )
        debugMessage( "*** Created ", numObjs, " scope trace Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", scopeTraceDataObjects.size() ," scope trace Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }

    return success;
}
//______________________________________________________________________________
void scopeConfigReader::addToScopeNumMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( scopeNumMonStructs, keyVal );
}
//______________________________________________________________________________
void scopeConfigReader::addToScopeTraceDataMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( scopeTraceDataMonStructs, keyVal );
}
//______________________________________________________________________________
void scopeConfigReader::addToScopeNumObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        scopeStructs::scopeNumObject sconumob = scopeStructs::scopeNumObject();
        sconumob.name = keyVal[ 1 ];
        scopeNumObjects.push_back( sconumob );
        debugMessage("Added ", scopeNumObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            scopeNumObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            scopeNumObjects.back().pvRoot = keyVal[ 1 ];
    }

//    else if( keyVal[0] == UTL::TIMEBASE )
//    {
//        scopeNumObjects.back().timebase = getNumD( keyVal[ 1 ] );
//    }

}
//______________________________________________________________________________
void scopeConfigReader::addToScopeTraceDataObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        scopeStructs::scopeTraceData scotrob = scopeStructs::scopeTraceData();
        scotrob.name = keyVal[ 1 ];
        scopeTraceDataObjects.push_back( scotrob );
        debugMessage("Added ", scopeTraceDataObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            scopeTraceDataObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            scopeTraceDataObjects.back().pvRoot = keyVal[ 1 ];
    }

    else if( keyVal[0] == UTL::TIMEBASE )
    {
        scopeTraceDataObjects.back().timebase = getNumD( keyVal[ 1 ] );
    }
}
//______________________________________________________________________________
void scopeConfigReader::addToPVStruct( std::vector< scopeStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( scopeStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // NUMBER PVs
        if( keyVal[0] == UTL::PV_SUFFIX_P1  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::P1;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P2  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::P2;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P3  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::P3;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P4  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::P4;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::NUM;
        }
        // TRACE PVs
        else if( keyVal[0] == UTL::PV_SUFFIX_TR1  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::TR1;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR2  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::TR2;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR3  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::TR3;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR4  )
        {
            pvStruct_v.back().pvType    = scopeStructs::SCOPE_PV_TYPE::TR4;
            pvStruct_v.back().scopeType = scopeStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::DIAG_TYPE )
        {
            pvStruct_v.back().diagType = getDiagType( keyVal[ 1 ] );
        }
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void scopeConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< scopeStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::DIAG_TYPE )
        pvStruct_v.back().diagType = getDiagType( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
bool scopeConfigReader::readConfig( scopeConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
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
VELA_ENUM::DIAG_TYPE scopeConfigReader::getDiagType( const std::string & val )
{
    VELA_ENUM::DIAG_TYPE r;

    if( val == UTL::WCM )
        r = VELA_ENUM::WCM;
    else if( val == UTL::ICT1 )
        r = VELA_ENUM::ICT1;
    else if( val == UTL::ICT2 )
        r = VELA_ENUM::ICT2;
    else if( val == UTL::ED_FCUP )
        r = VELA_ENUM::ED_FCUP;
    else if( val == UTL::FCUP )
        r = VELA_ENUM::FCUP;

    return r;
}

