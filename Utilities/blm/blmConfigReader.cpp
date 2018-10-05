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

    //TRACE
    blmTraceDataObjects.clear();
    blmTraceDataMonStructs.clear();
    bool traceSuccess = readConfig( *this,  blmConf1, &blmConfigReader::addToBLMTraceDataObjectsV1,nullptr, &blmConfigReader::addToBLMTraceDataMonStructsV1 );
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
void blmConfigReader::addToBLMTraceDataMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( blmTraceDataMonStructs, keyVal );
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
}
//______________________________________________________________________________
void blmConfigReader::addToPVStruct( std::vector< blmStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( blmStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // NUMBER PVs
        if( keyVal[0] == UTL::PV_SUFFIX_CH1_WAVE  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH1WAVE;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH2_WAVE  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH2WAVE;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH3_WAVE  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH3WAVE;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH4_WAVE  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH4WAVE;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH1_TIME  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH1TIME;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH2_TIME  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH2TIME;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH3_TIME  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH3TIME;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_CH4_TIME  )
        {
            pvStruct_v.back().pvType      = blmStructs::BLM_PV_TYPE::CH4TIME;
            pvStruct_v.back().blmDataType = blmStructs::BLM_DATA_TYPE::WAVE;
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
