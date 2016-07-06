//djs
#include "cameraConfigReader.h"
#include "configDefinitions.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

cameraConfigReader::cameraConfigReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
cameraConfigReader::cameraConfigReader( const std::string & configFile_Location, const std::string &configIOCFile_Location, const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
:  configReader( configFile_Location, configIOCFile_Location, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
cameraConfigReader::~cameraConfigReader(){}
//______________________________________________________________________________
bool cameraConfigReader::getCamData(  std::map< std::string, cameraStructs::camObject > & mapToFill )
{
    bool success = true;
    mapToFill.clear();

    size_t nCount = 0;
    size_t rCount = 0;

    for( auto && it : camObjects )
    {
        // add in magnetObj
        mapToFill[ it.name ] = it;
        // setMon and Com pvStructs
        for( auto && it2 : pvMonStructs )
            mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;

        for( auto && it2 : pvComStructs )
            mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
    }
    return success;
}
//______________________________________________________________________________
bool cameraConfigReader::getCamIOCData( cameraStructs::camIOCObject & c )
{
    c = camIOCObjects[ 0 ];

    for( auto && it2 : pvIOCMonStructs )
        c.pvMonStructs[ it2.pvType ] = it2;

    for( auto && it2 : pvIOCComStructs )
        c.pvComStructs[ it2.pvType ] = it2;

    return true;
}
//______________________________________________________________________________
bool cameraConfigReader::readConfig()
{
    bool ans1 = read_CAM_Config();///CONGIG
    bool ans2 = read_IOC_Config();
    if( ans1 && ans2 )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool cameraConfigReader::read_CAM_Config(  )
{
    debugMessage( "\n", "**** Attempting to Read ", configFile1," ****" );

    std::string line, trimmedLine;
    bool success = false;

    camObjects.clear();
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
                                        addToObjectsV1( keyVal );

                                    else if ( readingCommandPVs  )
                                        addToPVStruct( pvComStructs, keyVal );

                                    else if ( readingMonitorPVs )
                                        addToPVStruct( pvMonStructs, keyVal );
                                }
                                break;
                            default:
                                message( "!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
                        }
                    }
                }
                if( trimmedLine == UTL::START_OF_DATA  )
                {
                    readingData = true;
                    debugMessage( "Found START_OF_DATA" );
                }
                if( trimmedLine == UTL::PV_COMMANDS_START )
                {
                    readingCommandPVs  = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage( "Found PV_COMMANDS_START" );
                }
                if( trimmedLine == UTL::PV_MONITORS_START )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage( "Found PV_MONITORS_START" );
                }
                if( trimmedLine == UTL::OBJECTS_START )
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
//
//        if( numObjs == shutterObjects.size() )
//          //  debugMessage( "*** Created ", numObjs, " Shutter Objects, As Expected ***" );
//        else
//          //  debugMessage( "*** Created ", shutterObjects.size() ," Expected ", numObjs,  " ERROR ***"  );

        success = true;
    }
    else{
        message( "!!!! Error Can't Open after searching for:  ", configFile1, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
bool cameraConfigReader::read_IOC_Config(  )
{
    debugMessage( "\n", "**** Attempting to Read ",configFile2," ****" );

    std::string line, trimmedLine;
    bool success = false;

    camIOCObjects.clear();
    pvIOCMonStructs.clear();
    pvIOCMonStructs.clear();
    std::ifstream inputFile;

    inputFile.open( configFile2, std::ios::in );
    if( inputFile )
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage( "File Opened from ", configFile2 );
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
                                        addToCamIOCObjects( keyVal );

                                    else if ( readingCommandPVs  )
                                        addToPVStruct( pvIOCComStructs, keyVal );

                                    else if ( readingMonitorPVs )
                                        addToPVStruct( pvIOCMonStructs, keyVal );
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
//
//        if( numObjs == shutterObjects.size() )
//          //  debugMessage( "*** Created ", numObjs, " Shutter Objects, As Expected ***" );
//        else
//          //  debugMessage( "*** Created ", shutterObjects.size() ," Expected ", numObjs,  " ERROR ***"  );

        success = true;
    }
    else{
        message( "!!!! Error Can't Open after searching for:  ", configFile2, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________
void cameraConfigReader::addToPVStruct( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( cameraStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // NR-PSU PVs
        if( keyVal[0] == UTL::PV_SUFFIX_START  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_START;

        else if( keyVal[0] == UTL::PV_SUFFIX_STOP  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_STOP;

        else if( keyVal[0] == UTL::PV_SUFFIX_GAIN  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_GAIN;

        else if( keyVal[0] == UTL::PV_SUFFIX_BLACK  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_BLACK;

        else if( keyVal[0] == UTL::PV_SUFFIX_DATA  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_DATA;

        else if( keyVal[0] == UTL::PV_SUFFIX_BLACK_RBV  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_BLACK_RBV;

        else if( keyVal[0] == UTL::PV_SUFFIX_GAIN_RBV )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_GAIN_RBV;

        else if( keyVal[0] == UTL::PV_SUFFIX_CAM_COUNT  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_NUM_ACTIVE;

        else if( keyVal[0] == UTL::PV_SUFFIX_CAM_STATE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_STATE;

        else if( keyVal[0] == UTL::PV_SUFFIX_CAM_LIMIT  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_LIMIT;
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void cameraConfigReader::addToObjectsV1(const std::vector<std::string> & keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        cameraStructs::camObject cob = cameraStructs::camObject();
        cob.name = keyVal[ 1 ];
        cob.numIlocks = (size_t)numIlocks;
        camObjects.push_back( cob );
        debugMessage("Added ", camObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        camObjects.back().pvRoot = keyVal[ 1 ];

    else if( keyVal[0] == UTL::SCREEN )
        camObjects.back().screen = keyVal[ 1 ];

    else if( keyVal[0] == UTL::X0 )
        camObjects.back().imageStruct.x0 = getSize( keyVal[ 1 ] );

    else if( keyVal[0] == UTL::Y0 )
        camObjects.back().imageStruct.y0 = getSize( keyVal[ 1 ] );

    else if( keyVal[0] == UTL::XRAD )
        camObjects.back().imageStruct.xRad = getSize( keyVal[ 1 ] );

    else if( keyVal[0] == UTL::YRAD )
        camObjects.back().imageStruct.yRad =  getSize( keyVal[ 1 ] );

    else if( keyVal[0] == UTL::X_PIX_2_MM )
        camObjects.back().imageStruct.xPix2mm = getNumD( keyVal[ 1 ] );

    else if( keyVal[0] == UTL::Y_PIX_2_MM )
        camObjects.back().imageStruct.yPix2mm = getNumD( keyVal[ 1 ] );

}
//______________________________________________________________________________
void cameraConfigReader::addToCamIOCObjects( const std::vector<std::string> & keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        cameraStructs::camIOCObject cob = cameraStructs::camIOCObject();
        cob.name = keyVal[ 1 ];
        cob.numIlocks = (size_t)numIlocks;

        camIOCObjects.push_back( cob );
        debugMessage("Added ", camIOCObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        camIOCObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________
void cameraConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
