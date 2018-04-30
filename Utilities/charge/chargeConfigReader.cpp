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
                                      const std::string & chargeConf2,
                                      const bool& show_messages_ptr,
                                      const bool& show_debug_messages_ptr,
                                      const bool startVirtualMachine ):
chargeConf1( chargeConf1 ),
chargeConf2( chargeConf2 ),
startVirtualMachine( startVirtualMachine ),
configReader(chargeConf1, chargeConf2, show_messages_ptr, show_debug_messages_ptr )
{

}
//______________________________________________________________________________
chargeConfigReader::~chargeConfigReader(){}
//______________________________________________________________________________
chargeStructs::chargeObject chargeConfigReader::getchargeObject()
{
    chargeStructs::chargeObject obj;

    obj.name = "VELA_charge";

//    /// there are only 1 chargeNumObject and 1 chargeTraceDataObject
//
//    obj.numObject = chargeNumObject;
//    for( auto && it : chargeNumMonStructs )
//        obj.numObject.pvMonStructs[ it.pvType ] = it;
//

    for( auto && it : chargeNumObjects )
        obj.numObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each num object ...

    for( auto && it : obj.numObjects )
    {
        for( auto it2 : chargeNumMonStructs )
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

    /// chargeTraceDataObjects are in a map keyed by name ...

    for( auto && it : chargeTraceDataObjects )
        obj.traceObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each num object ...

    for( auto && it : obj.traceObjects )
    {
        for( auto it2 : chargeTraceDataMonStructs )
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
bool chargeConfigReader::readConfigFiles()
{
    bool success = true;
    /// There are 2 types of objects in the charge, a trace monitor and a number monitor;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an chargeObject
    //NUM
    chargeNumObjects.clear();
    chargeNumMonStructs.clear();
    bool numSuccess = readConfig( *this,  chargeConf1, &chargeConfigReader::addTochargeNumObjectsV1,nullptr, &chargeConfigReader::addTochargeNumMonStructsV1 );
    if( !numSuccess )
        success = false;
//    if( numObjs == chargeTraceDataObject.size() )
    if( numObjs == chargeNumObjects.size() )
        debugMessage( "*** Created ", numObjs, " charge num Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", chargeNumObjects.size() ," charge num Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }
    //TRACE
    chargeTraceDataObjects.clear();
    chargeTraceDataMonStructs.clear();
    bool traceSuccess = readConfig( *this,  chargeConf2, &chargeConfigReader::addTochargeTraceDataObjectsV1,nullptr, &chargeConfigReader::addTochargeTraceDataMonStructsV1 );
    if( !traceSuccess )
        success = false;
//    if( numObjs == chargeTraceDataObject.size() )
    if( numObjs == chargeTraceDataObjects.size() )
        debugMessage( "*** Created ", numObjs, " charge trace Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", chargeTraceDataObjects.size() ," charge trace Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }

    return success;
}
//______________________________________________________________________________
void chargeConfigReader::addTochargeNumMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( chargeNumMonStructs, keyVal );
}
//______________________________________________________________________________
void chargeConfigReader::addTochargeTraceDataMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( chargeTraceDataMonStructs, keyVal );
}
//______________________________________________________________________________
void chargeConfigReader::addTochargeNumObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        chargeStructs::chargeNumObject sconumob = chargeStructs::chargeNumObject();
        sconumob.name = keyVal[ 1 ];
        chargeNumObjects.push_back( sconumob );
        debugMessage("Added ", chargeNumObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( startVirtualMachine )
            chargeNumObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            chargeNumObjects.back().pvRoot = keyVal[ 1 ];
    }
    else if( keyVal[0] == UTL::charge_NAME )
    {
        chargeNumObjects.back().chargeName = getchargeName( keyVal[ 1 ] );
    }

}
//______________________________________________________________________________
void chargeConfigReader::addTochargeTraceDataObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        chargeStructs::chargeTraceData scotrob = chargeStructs::chargeTraceData();
        scotrob.name = keyVal[ 1 ];
        chargeTraceDataObjects.push_back( scotrob );
        debugMessage("Added ", chargeTraceDataObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( startVirtualMachine )
            chargeTraceDataObjects.back().pvRoot = UTL::VM_PREFIX + keyVal[ 1 ];
        else
            chargeTraceDataObjects.back().pvRoot = keyVal[ 1 ];
    }
    else if( keyVal[0] == UTL::charge_NAME )
    {
        chargeTraceDataObjects.back().chargeName = getchargeName( keyVal[ 1 ] );
    }
    else if( keyVal[0] == UTL::TIMEBASE )
    {
        chargeTraceDataObjects.back().timebase = getNumD( keyVal[ 1 ] );
    }
}
//______________________________________________________________________________
void chargeConfigReader::addToPVStruct( std::vector< chargeStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( chargeStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // NUMBER PVs
        if( keyVal[0] == UTL::PV_SUFFIX_P1  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::P1;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P2  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::P2;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P3  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::P3;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::NUM;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_P4  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::P4;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::NUM;
        }
        // TRACE PVs
        else if( keyVal[0] == UTL::PV_SUFFIX_TR1  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::TR1;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR2  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::TR2;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR3  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::TR3;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::ARRAY;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TR4  )
        {
            pvStruct_v.back().pvType    = chargeStructs::charge_PV_TYPE::TR4;
            pvStruct_v.back().chargeType = chargeStructs::charge_TYPE::ARRAY;
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
void chargeConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< chargeStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
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
chargeStructs::DIAG_TYPE chargeConfigReader::getDiagType( const std::string & val )
{
    chargeStructs::DIAG_TYPE r;

    if( val == UTL::WCM )
        r = chargeStructs::DIAG_TYPE::WCM;
    else if( val == UTL::ICT1 )
        r = chargeStructs::DIAG_TYPE::ICT1;
    else if( val == UTL::ICT2 )
        r = chargeStructs::DIAG_TYPE::ICT2;
    else if( val == UTL::ED_FCUP )
        r = chargeStructs::DIAG_TYPE::ED_FCUP;
    else if( val == UTL::FCUP )
        r = chargeStructs::DIAG_TYPE::FCUP;

    return r;
}
//______________________________________________________________________________
chargeStructs::charge_NAME chargeConfigReader::getchargeName( const std::string & val )
{
    chargeStructs::charge_NAME r;

    if( val == UTL::CLARAcharge01 )
        r = chargeStructs::charge_NAME::CLARAcharge01;
    else if( val == UTL::VELAcharge02 )
        r = chargeStructs::charge_NAME::VELAcharge02;
    else
        r = chargeStructs::UNKNOWN_charge_NAME;

    return r;
}

