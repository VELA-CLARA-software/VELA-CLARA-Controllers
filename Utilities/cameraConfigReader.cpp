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

cameraConfigReader::cameraConfigReader(const std::string& camConfig  ,const bool startVirtualMachine,
                                       const bool*show_messages_ptr,const bool* show_debug_messages_ptr ):
camConfig(camConfig),
configReader( show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{
}
//______________________________________________________________________________
cameraConfigReader::~cameraConfigReader(){}
//______________________________________________________________________________
bool cameraConfigReader::getCamData(  std::map< std::string, cameraStructs::cameraObject > & mapToFill )
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
        for( auto && it2 : pvCameraMonStructs )
            mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;

        for( auto && it2 : pvCameraComStructs )
            mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
    }
    return success;
}
bool cameraConfigReader::readConfig()
{
    if( usingVirtualMachine )
        debugMessage( "\n", "**** Using VIRTUAL Machine  ****" );
    else
        debugMessage(  "**** Using PHYSICAL Machine  ****","\n" );

    std::string line, trimmedLine;
    bool success = false;

    std::ifstream inputFile;

    inputFile.open( camConfig, std::ios::in );
    if( inputFile )
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage( "Opened ", camConfig );
        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpaceExceptBetweenDoubleQuotes( trimToDelimiter( line, UTL::END_OF_LINE ) );
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
                        switch( configVersion ) /// Actually this switch needs to come out of here and go in the beginning, we should look for version # first!
                        {
                            case 1:
                                if( trimmedLine.find_first_of( UTL::EQUALS_SIGN ) != std::string::npos )
                                {
                                    std::vector<std::string> keyVal = getKeyVal( trimmedLine );

                                    if(readingObjs)
                                        addToCameraObjects(keyVal);

                                    else if (readingCommandPVs)
                                        addToCameraCommandStructs(keyVal);

                                    else if (readingMonitorPVs)
                                        addToCameraMonitorStructs(keyVal);
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
        debugMessage( "config File Closed" );
        success = true;
    }
    else{
        message( "!!!! Error Can't Open Config File after searching for:  ", camConfig, " !!!!"  );
    }
    if(success)
        debugMessage("cameraConfigReader readconfig is returning TRUE");
    else
        debugMessage("cameraConfigReader readconfig is returning FALSE");

    return success;
}

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
        else if( keyVal[0] == UTL::PV_SUFFIX_DATA  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_DATA;
        else if( keyVal[0] == UTL::PV_SUFFIX_BKGRND_DATA  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_BKGRND_DATA;
        else if( keyVal[0] == UTL::PV_SUFFIX_FILE_PATH  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_FILE_PATH;
        else if( keyVal[0] == UTL::PV_SUFFIX_FILE_NAME  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_FILE_NAME;
        else if( keyVal[0] == UTL::PV_SUFFIX_FILE_TEMPLATE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_FILE_TEMPLATE;
        else if( keyVal[0] == UTL::PV_SUFFIX_FILE_WRITE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::PV_SUFFIX_FILE_WRITE;
        else if( keyVal[0] == UTL::PV_SUFFIX_CAM_STATE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_STATE;

        else if( keyVal[0] == UTL::PV_SUFFIX_X  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::X;
        else if( keyVal[0] == UTL::PV_SUFFIX_Y  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::Y;
        else if( keyVal[0] == UTL::PV_SUFFIX_SX  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_X;
        else if( keyVal[0] == UTL::PV_SUFFIX_SY  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_Y;
        else if( keyVal[0] == UTL::PV_SUFFIX_SXY  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_XY;
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void cameraConfigReader::addToCameraObjects(const std::vector<std::string> & keyVal )
{
    std::string value = keyVal[1];//MAGIC_NUMBER
    if( keyVal[0] == UTL::NAME )
    {
        camObjects.push_back(cameraStructs::cameraObject() );
        camObjects.back().name      = value;
        camObjects.back().numIlocks = (size_t)numIlocks;
        debugMessage("Added ", camObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            camObjects.back().pvRoot = UTL::VM_PREFIX + value;
        else
            camObjects.back().pvRoot = value;
    }
    else if( keyVal[0] == UTL::SCREEN_PV )
        camObjects.back().screenPV = value;
    else if( keyVal[0] == UTL::X_CENTER )
        camObjects.back().imageStruct.xCenterPix = getSize( value );
    else if( keyVal[0] == UTL::Y_CENTER )
        camObjects.back().imageStruct.yCenterPix = getSize( value );
    else if( keyVal[0] == UTL::X_RAD )
        camObjects.back().imageStruct.xRAD = getSize( value );
    else if( keyVal[0] == UTL::Y_RAD )
         camObjects.back().imageStruct.yRAD = getSize( value );
    else if( keyVal[0] == UTL::X_PIX_2_MM )
        camObjects.back().imageStruct.xPix2mm = getNumD( value );
    else if( keyVal[0] == UTL::Y_PIX_2_MM )
        camObjects.back().imageStruct.yPix2mm =  getNumD( value );
    else if( keyVal[0] == UTL::BIT_DEPTH )
        camObjects.back().imageStruct.bit_depth = getSize( value );
    else if( keyVal[0] == UTL::IMAGE_WIDTH )
        camObjects.back().imageStruct.image_width =  getSize( value );
    else if( keyVal[0] == UTL::IMAGE_HEIGHT )
        camObjects.back().imageStruct.image_height = getSize( value );
}
//______________________________________________________________________________
void cameraConfigReader::addToCameraMonitorStructs( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvCameraMonStructs, keyVal);
}
//______________________________________________________________________________
void cameraConfigReader::addToCameraCommandStructs( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvCameraComStructs, keyVal);
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
