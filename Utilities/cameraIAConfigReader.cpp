//tp
#include "cameraIAConfigReader.h"
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

cameraIAConfigReader::cameraIAConfigReader(const std::string& camConfig,
                                             const bool startVirtualMachine,
                                             const bool*show_messages_ptr,
                                             const bool* show_debug_messages_ptr ):
camConfig(camConfig), configReader( show_messages_ptr, show_debug_messages_ptr ), usingVirtualMachine(startVirtualMachine)
{
}
cameraIAConfigReader::~cameraIAConfigReader(){}
bool cameraIAConfigReader::getCamData(  std::map< std::string, cameraStructs::cameraObject > & mapToFill )
{
    bool success = true;
    mapToFill.clear();

    size_t nCount = 0;
    size_t rCount = 0;

    for( auto && it : camObject )
    {
        // add in magnetObj
        mapToFill[ it.name ].IA = it.IA;
        mapToFill[ it.name ].name = it.name;
        mapToFill[ it.name ].screenName = it.screenName;
        mapToFill[ it.name ].pvRoot = it.pvRoot;
        // setMon and Com pvStructs
        for( auto && it2 : pvCameraMonStructs )
            mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;

        for( auto && it2 : pvCameraComStructs )
            mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
    }
    return success;
}
bool cameraIAConfigReader::readConfig()
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
        debugMessage("cameraIAConfigReader readconfig is returning TRUE");
    else
        debugMessage("cameraIAConfigReader readconfig is returning FALSE");

    return success;
}
void cameraIAConfigReader::addToPVStruct( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) && stringIsSubString( keyVal[0], "IA" ))
    {
        pvStruct_v.push_back( cameraStructs::pvStruct() );
        pvStruct_v.back().pvSuffix = keyVal[1];
        if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_X  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::X;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_Y  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::Y;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_SIGMA_X  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_X;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_SIGMA_Y  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_Y;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_COV_XY  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::COV_XY;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_X_PIX  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::X_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_Y_PIX  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::Y_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_SIGMA_X_PIX  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_X_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_SIGMA_Y_PIX  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::SIGMA_Y_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_COV_XY_PIX  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::COV_XY_PIX;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_BIT_DEPTH  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::BIT_DEPTH;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_IMAGE_HEIGHT  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::IMAGE_HEIGHT;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_IMAGE_WIDTH  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::IMAGE_WIDTH;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_ACQUIRE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_ACQUIRE_RBV  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_ACQUIRE_RBV;
        else if( keyVal[0] == UTL::PV_IA_SUFFIX_CAM_STATE  )
            pvStruct_v.back().pvType = cameraStructs::CAM_PV_TYPE::CAM_STATUS;
        else
            message("The next line is false.");
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    //else
       // addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
void cameraIAConfigReader::addToCameraObjects(const std::vector<std::string> & keyVal )
{
    std::string value = keyVal[1];//MAGIC_NUMBER
    if( keyVal[0] == UTL::NAME )
    {
        camObject.push_back(cameraStructs::cameraObject() );
        camObject.back().name      = value;

       // camDAQObject.back().numIlocks = (size_t)numIlocks;
        debugMessage("Added camera ", camObject.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            camObject.back().pvRoot = UTL::VM_PREFIX + value;
        else
            camObject.back().pvRoot = value;
    }
    else if( keyVal[0] == UTL::SCREEN_NAME )
    {
        if( usingVirtualMachine )
            camObject.back().screenName = UTL::VM_PREFIX + value;
        else
            camObject.back().screenName = value;
    }
    else if(keyVal[0] == UTL::X_MASK_RAD)
    {
        camObject.back().IA.xRad  = getNum(value);
    }
    else if(keyVal[0] == UTL::Y_MASK_RAD)
    {
        camObject.back().IA.yRad  = getNum(value);
    }
    else if(keyVal[0] == UTL::X_MASK_CENTER)
    {
        camObject.back().IA.xCenterPix  = getNum(value);
    }
    else if(keyVal[0] == UTL::Y_MASK_CENTER)
    {
        camObject.back().IA.yCenterPix  = getNum(value);
    }
    else if(keyVal[0] == UTL::PIX_2_MM_RATIO)
    {
        camObject.back().IA.pix2mm  = getNumD(value);
    }
}
void cameraIAConfigReader::addToCameraMonitorStructs( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvCameraMonStructs, keyVal);
}
void cameraIAConfigReader::addToCameraCommandStructs( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvCameraComStructs, keyVal);
}
void cameraIAConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    message("HI");
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}

