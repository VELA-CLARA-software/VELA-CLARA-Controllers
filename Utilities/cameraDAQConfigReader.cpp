//tp
#include "cameraDAQConfigReader.h"
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

cameraDAQConfigReader::cameraDAQConfigReader(const std::string& camConfig,
                                             const bool startVirtualMachine,
                                             const bool*show_messages_ptr,
                                             const bool* show_debug_messages_ptr ):
camConfig(camConfig), configReader( show_messages_ptr, show_debug_messages_ptr ), usingVirtualMachine(startVirtualMachine)
{
}
cameraDAQConfigReader::~cameraDAQConfigReader(){}
bool cameraDAQConfigReader::getCamDAQData(  std::map< std::string, cameraStructs::cameraDAQObject > & mapToFill )
{
    bool success = true;
    mapToFill.clear();

    size_t nCount = 0;
    size_t rCount = 0;

    for( auto && it : camDAQObject )
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
bool cameraDAQConfigReader::readConfig()
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
        debugMessage("cameraDAQConfigReader readconfig is returning TRUE");
    else
        debugMessage("cameraDAQConfigReader readconfig is returning FALSE");

    return success;
}
void cameraDAQConfigReader::addToPVStruct( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( cameraStructs::pvStruct() );
        pvStruct_v.back().pvSuffix = keyVal[1];
        if( keyVal[0] == UTL::PV_SUFFIX_START  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE;
        else if( keyVal[0] == UTL::PV_SUFFIX_ACQUIRE_RBV  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE_RBV;
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
        else if( keyVal[0] == UTL::PV_SUFFIX_WRITE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE;
        else if( keyVal[0] == UTL::PV_SUFFIX_WRITE_STATUS  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_STATUS;
        else if( keyVal[0] == UTL::PV_SUFFIX_WRITE_MESSAGE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_FILE_WRITE_MESSAGE;
        else if( keyVal[0] == UTL::PV_SUFFIX_CAM_STATE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_STATUS;
        else if( keyVal[0] == UTL::PV_SUFFIX_CAPTURE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_CAPTURE;
        else if( keyVal[0] == UTL::PV_SUFFIX_NUM_CAPTURE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURE;
        else if( keyVal[0] == UTL::PV_SUFFIX_NUM_CAPTURED  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_NUM_CAPTURED;
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
void cameraDAQConfigReader::addToCameraObjects(const std::vector<std::string> & keyVal )
{
    std::string value = keyVal[1];//MAGIC_NUMBER
    if( keyVal[0] == UTL::NAME )
    {
        camDAQObject.push_back(cameraStructs::cameraDAQObject() );
        camDAQObject.back().name      = value;

       // camDAQObject.back().numIlocks = (size_t)numIlocks;
        debugMessage("Added camera ", camDAQObject.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            camDAQObject.back().pvRoot = UTL::VM_PREFIX + value;
        else
            camDAQObject.back().pvRoot = value;
    }
    else if( keyVal[0] == UTL::SCREEN_PV )
    {
        if( usingVirtualMachine )
            camDAQObject.back().screenPV = UTL::VM_PREFIX + value;
        else
            camDAQObject.back().screenPV = value;
    }
}
void cameraDAQConfigReader::addToCameraMonitorStructs( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvCameraMonStructs, keyVal);
}
void cameraDAQConfigReader::addToCameraCommandStructs( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvCameraComStructs, keyVal);
}
void cameraDAQConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
