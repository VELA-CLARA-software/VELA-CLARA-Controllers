//djs
#include "beamPositionMonitorConfigReader.h"
#include "configDefinitions.h"
#include "beamPositionMonitorStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

beamPositionMonitorConfigReader::beamPositionMonitorConfigReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
beamPositionMonitorConfigReader::beamPositionMonitorConfigReader( const std::string & configFileLocation, const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
:  configReader( configFileLocation, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
beamPositionMonitorConfigReader::~beamPositionMonitorConfigReader(){}
//______________________________________________________________________________
beamPositionMonitorStructs::bpmObject beamPositionMonitorConfigReader::getBPMObject()
{
    beamPositionMonitorStructs::bpmObject obj;

    obj.name = "VELA_BPM";

    for( auto && it : bpmDataObjects )
        obj.dataObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each bpm object ...

    for( auto && it : obj.dataObjects )
        for( auto it2 : bpmDataMonStructs )
            it.second.pvMonStructs[ it2.pvType ] = it2;

    for( auto && it : obj.dataObjects )
        for( auto it2 : bpmDataMonStructs )
            it.second.pvComStructs[ it2.pvType ] = it2;


    return obj;
}
//______________________________________________________________________________
bool beamPositionMonitorConfigReader::readConfigFiles()
{
    bool success = true;
    /// There are 2 types of objects in the bpm, a trace monitor and a number monitor;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an bpmObject
    //NUM
    bpmDataMonStructs.clear();
    bpmDataObjects.clear();
    bool bpmSuccess = readConfig( *this, configFile1, &beamPositionMonitorConfigReader::addToBPMDataObjectsV1, &beamPositionMonitorConfigReader::addToBPMPVComStructsV1, &beamPositionMonitorConfigReader::addToBPMPVMonStructsV1 );
    if( !bpmSuccess )
        success = false;
    if( numObjs == bpmDataObjects.size() )
        debugMessage( "*** Created ", numObjs, " BPM Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", bpmDataObjects.size() ," BPM Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }

    return success;
}
//______________________________________________________________________________
void beamPositionMonitorConfigReader::addToBPMPVMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( bpmDataMonStructs, keyVal );
}
//______________________________________________________________________________
void beamPositionMonitorConfigReader::addToBPMPVComStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( bpmDataMonStructs, keyVal );
}
//______________________________________________________________________________
void beamPositionMonitorConfigReader::addToBPMDataObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        beamPositionMonitorStructs::bpmDataObject bpmob = beamPositionMonitorStructs::bpmDataObject();
        bpmob.name = keyVal[ 1 ];
        bpmob.bpmRawData.name = keyVal[ 1 ];
        bpmDataObjects.push_back( bpmob );
        debugMessage("Added ", bpmDataObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        bpmDataObjects.back().pvRoot = keyVal[ 1 ];
    }

    else if( keyVal[0] == UTL::ATT1CAL )
    {
        bpmDataObjects.back().att1cal = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::ATT2CAL )
    {
        bpmDataObjects.back().att2cal = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::V1CAL )
    {
        bpmDataObjects.back().v1cal = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::V2CAL )
    {
        bpmDataObjects.back().v2cal = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::QCAL )
    {
        bpmDataObjects.back().qcal = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::MN )
    {
        bpmDataObjects.back().mn = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::XN )
    {
        bpmDataObjects.back().xn = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }

    else if( keyVal[0] == UTL::YN )
    {
        bpmDataObjects.back().yn = getNumD( keyVal[ 1 ] ); /// djs edit stringToDouble for getNumD 10.11.15
    }
}
//______________________________________________________________________________
void beamPositionMonitorConfigReader::addToPVStruct( std::vector< beamPositionMonitorStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( beamPositionMonitorStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // Command PVs
        if( keyVal[0] == UTL::PV_SUFFIX_BPM_SA1  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::SA1;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_SA2  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::SA2;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_SD1  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::SD1;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_SD2  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::SD2;
        // Monitor PVs
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_RA1  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::RA1;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_RA2  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::RA2;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_RD1  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::RD1;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_RD2  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::RD2;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_X  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::X;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_Y  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::Y;
        else if( keyVal[0] == UTL::PV_SUFFIX_BPM_DATA  )
            pvStruct_v.back().pvType = beamPositionMonitorStructs::BPM_PV_TYPE::DATA;
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void beamPositionMonitorConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< beamPositionMonitorStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
bool beamPositionMonitorConfigReader::readConfig( beamPositionMonitorConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
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
        message( "!!!! Error Can't Open Shutter Config File after searching in:  ", fn, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
