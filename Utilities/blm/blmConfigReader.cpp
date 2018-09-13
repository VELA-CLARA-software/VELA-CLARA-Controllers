//djs
#include "blmConfigReader.h"
#include "configDefinitions.h"
#include "blmStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

blmConfigReader::blmConfigReader( const std::string & blmConf1,
                                      const bool& show_messages_ptr,
                                      const bool& show_debug_messages_ptr,
                                      const bool startVirtualMachine ):
blmConf1( blmConf1 ),
startVirtualMachine( startVirtualMachine ),
configReader(blmConf1, show_messages_ptr, show_debug_messages_ptr )
{

}
//______________________________________________________________________________
blmConfigReader::~blmConfigReader(){}
//______________________________________________________________________________
blmStructs::blmObject blmConfigReader::getBLMObject()
{
    blmStructs::blmObject obj;

    obj.name = "VELA_SCOPE";

//    /// there are only 1 blmNumObject and 1 blmTraceDataObject
//
//    obj.numObject = blmNumObject;
//    for( auto && it : blmNumMonStructs )
//        obj.numObject.pvMonStructs[ it.pvType ] = it;
//

    for( auto && it : blmNumObjects )
        obj.numObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each num object ...

    for( auto && it : obj.numObjects )
    {
        for( auto it2 : blmNumMonStructs )
        {
            it.second.pvMonStructs[ it2.pvType ] = it2;
            it.second.isMonitoringMap[ it2.pvType ] = false;
            it.second.numData[ it2.pvType ] = nums;
            it.second.numTimeStamps[ it2.pvType ] = numtstamps;
            it.second.numStrTimeStamps[ it2.pvType ] = numstrtstamps;
            it.second.shotCounts[ it2.pvType ] = numshotcounts;
            it.second.numDataBuffer[ it2.pvType ] = numsbuffer;
        }
    }

    /// blmTraceDataObjects are in a map keyed by name ...

    for( auto && it : blmTraceDataObjects )
        obj.traceObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each num object ...

    for( auto && it : obj.traceObjects )
    {
        for( auto it2 : blmTraceDataMonStructs )
        {
            it.second.pvMonStructs[ it2.pvType ] = it2;
            it.second.isMonitoringMap[ it2.pvType ] = false;
            it.second.traceData[ it2.pvType ] = traces;
            it.second.timeStamps[ it2.pvType ] = tstamps;
            it.second.strTimeStamps[ it2.pvType ] = strtstamps;
            it.second.shotCounts[ it2.pvType ] = shotcounts;
            it.second.traceDataBuffer[ it2.pvType ] = tracesbuffer;
        }
    }

    return obj;
}
//______________________________________________________________________________
bool blmConfigReader::readConfigFiles()
{
    bool success = true;
    /// There are 2 types of objects in the blm, a trace monitor and a number monitor;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an blmObject
    //NUM
    blmNumObjects.clear();
    blmNumMonStructs.clear();
    bool numSuccess = readConfig( *this,  blmConf1, &blmConfigReader::addToBLMNumObjectsV1,nullptr, &blmConfigReader::addToBLMNumMonStructsV1 );
    if( !numSuccess )
        success = false;
//    if( numObjs == blmTraceDataObject.size() )
    if( numObjs == blmNumObjects.size() )
        debugMessage( "*** Created ", numObjs, " blm num Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", blmNumObjects.size() ," blm num Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }
    //TRACE
    blmTraceDataObjects.clear();
    blmTraceDataMonStructs.clear();
    bool traceSuccess = readConfig( *this,  blmConf2, &blmConfigReader::addToBLMTraceDataObjectsV1,nullptr, &blmConfigReader::addToBLMTraceDataMonStructsV1 );
    if( !traceSuccess )
        success = false;
//    if( numObjs == blmTraceDataObject.size() )
    if( numObjs == blmTraceDataObjects.size() )
        debugMessage( "*** Created ", numObjs, " blm trace Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", blmTraceDataObjects.size() ," blm trace Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }

    return success;
}
//______________________________________________________________________________
void blmConfigReader::addToBLMNumMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( blmNumMonStructs, keyVal );
}
//______________________________________________________________________________
void blmConfigReader::addToBLMTraceDataMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( blmTraceDataMonStructs, keyVal );
}
//______________________________________________________________________________
void blmConfigReader::addToBLMNumObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        blmStructs::blmNumObject sconumob = blmStructs::blmNumObject();
        sconumob.name = keyVal[ 1 ];
        blmNumObjects.push_back( sconumob );
        debugMessage("Added ", blmNumObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( startVirtualMachine )
            blmNumObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            blmNumObjects.back().pvRoot = keyVal[ 1 ];
    }
    else if( keyVal[0] == UTL::SCOPE_NAME )
    {
        blmNumObjects.back().blmName = getBLMName( keyVal[ 1 ] );
    }

}
//______________________________________________________________________________
void blmConfigReader::addToBLMTraceDataObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        blmStructs::blmTraceData scotrob = blmStructs::blmTraceData();
        scotrob.name = keyVal[ 1 ];
        blmTraceDataObjects.push_back( scotrob );
        debugMessage("Added ", blmTraceDataObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( startVirtualMachine )
            blmTraceDataObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            blmTraceDataObjects.back().pvRoot = keyVal[ 1 ];
    }
    else if( keyVal[0] == UTL::SCOPE_NAME )
    {
        blmTraceDataObjects.back().blmName = getBLMName( keyVal[ 1 ] );
    }
    else if( keyVal[0] == UTL::TIMEBASE )
    {
        blmTraceDataObjects.back().timebase = getNumD( keyVal[ 1 ] );
    }
}
//______________________________________________________________________________
void blmConfigReader::addToPVStruct( std::vector< blmStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( blmStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // NUMBER PVs
        if( keyVal[0] == UTL::PV_SUFFIX_P1  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::P1;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P2  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::P2;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P3  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::P3;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P4  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::P4;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::NUM;
        }
        // TRACE PVs
        else if( keyVal[0] == UTL::PV_SUFFIX_TR1  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::TR1;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR2  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::TR2;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR3  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::TR3;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR4  )
        {
            pvStruct_v.back().pvType    = blmStructs::SCOPE_PV_TYPE::TR4;
            pvStruct_v.back().blmType = blmStructs::SCOPE_TYPE::ARRAY;
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
void blmConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< blmStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
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
bool blmConfigReader::readConfig( blmConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
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
blmStructs::DIAG_TYPE blmConfigReader::getDiagType( const std::string & val )
{
    blmStructs::DIAG_TYPE r;

    if( val == UTL::WCM )
        r = blmStructs::DIAG_TYPE::WCM;
    else if( val == UTL::ICT1 )
        r = blmStructs::DIAG_TYPE::ICT1;
    else if( val == UTL::ICT2 )
        r = blmStructs::DIAG_TYPE::ICT2;
    else if( val == UTL::ED_FCUP )
        r = blmStructs::DIAG_TYPE::ED_FCUP;
    else if( val == UTL::FCUP )
        r = blmStructs::DIAG_TYPE::FCUP;

    return r;
}
//______________________________________________________________________________
blmStructs::SCOPE_NAME blmConfigReader::getBLMName( const std::string & val )
{
    blmStructs::SCOPE_NAME r;

    if( val == UTL::CLARASCOPE01 )
        r = blmStructs::SCOPE_NAME::CLARASCOPE01;
    else if( val == UTL::VELASCOPE02 )
        r = blmStructs::SCOPE_NAME::VELASCOPE02;
    else
        r = blmStructs::UNKNOWN_SCOPE_NAME;

    return r;
}

