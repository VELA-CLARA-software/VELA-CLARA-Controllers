//djs
#include "screenConfigReader.h"
#include "screenStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//____________________________________________________________________________________________________
screenConfigReader::screenConfigReader( const std::string & configFileLocation1,
                                        const bool startVirtualMachine,
                                        const bool* show_messages_ptr,
                                        const bool* show_debug_messages_ptr ):
configReader( configFileLocation1,  show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine),
lastPVStruct(nullptr)
{
//    std::cout << configFileLocation1 << std::endl;
}
//_____________________________________________________________________________________________________
screenConfigReader::~screenConfigReader()
{
    if( lastPVStruct )
    {
        delete lastPVStruct;
        lastPVStruct = nullptr;
    }
}
//______________________________________________________________________________________________________
bool screenConfigReader::readConfig()
{
    // clear local copies of structs
    scrObjects.clear();
    pvScrComStructs.clear();
    pvScrMonStructs.clear();
    // call the main read config funciton, pass in function pointers to the funcs
    // that know how to read a object, command or monitor
    bool scrSuccess = readConfig(*this, configFile1, &screenConfigReader::addToScrObjectsV1,&screenConfigReader::addToPVStructMain);
    return scrSuccess;
}
//______________________________________________________________________________
void screenConfigReader::addToPVStructMain( const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        // there are different types of screen, here we are hardcoding in how to interpret the config file and where to put
        // each type of monitor.
        /// MONITORS
        if( keyVal[0] == UTL::PV_SUFFIX_H_MOVING )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_MOVING, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_READY )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_READY, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_GET_DEV )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_GET_DEV, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_DEV_STATE )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_DEV_STATE, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_MAX_POS )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_MAX_POS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_DEV_CENT )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_DEV_CENT, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_ACTPOS )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_ACTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_EN )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::H_EN, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        if( keyVal[0] == UTL::PV_SUFFIX_V_MOVING )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_MOVING, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_READY )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_READY, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_GET_DEV )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_GET_DEV, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_DEV_STATE )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_DEV_STATE, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_MAX_POS )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_MAX_POS, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_DEV_CENT )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_DEV_CENT, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_ACTPOS )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_ACTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_EN )
        {
            addToPVStruct( pvScrMonStructs, screenStructs::SCREEN_PV_TYPE::V_EN, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        /// COMMANDS
        else if( keyVal[0] == UTL::PV_SUFFIX_H_SDEV )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::H_SDEV, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_TRIGGER )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::H_TRIGGER, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_EX )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::H_EX, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_TGTPOS )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::H_TGTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_JOG )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::H_JOG, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_JDIFF )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::H_JDIFF, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_SDEV )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::V_SDEV, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_TRIGGER )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::V_TRIGGER, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_EX )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::V_EX, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_TGTPOS )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::V_TGTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_JOG )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::V_JOG, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_JDIFF )
        {
            addToPVStruct( pvScrComStructs, screenStructs::SCREEN_PV_TYPE::V_JDIFF, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
    }
    else // must be a mask, chtype config entry, if the config file follows the rules
    {
        addCOUNT_MASK_OR_CHTYPE( keyVal );
    }
}
//______________________________________________________________________________
void screenConfigReader::setH(std::vector< screenStructs::pvStruct > & pvs )
{
    pvs.back().dir = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
}
//______________________________________________________________________________
void screenConfigReader::setV(std::vector< screenStructs::pvStruct > & pvs )
{
    pvs.back().dir = screenStructs::DRIVER_DIRECTION::VERTICAL;
}
//______________________________________________________________________________
void screenConfigReader::addToPVStruct(std::vector< screenStructs::pvStruct > & pvs, const screenStructs::SCREEN_PV_TYPE pvtype,
                                       const std::string& pvSuffix, const screenStructs::DRIVER_DIRECTION direction)
{
    pvs.push_back( screenStructs::pvStruct() );
    pvs.back().pvType      = pvtype;
    pvs.back().pvSuffix    = pvSuffix;
    pvs.back().dir         = direction;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvs.back().pvType ), " in ", ENUM_TO_STRING( pvs.back().dir ), " direction" );
}
//______________________________________________________________________________
void screenConfigReader::addCOUNT_MASK_OR_CHTYPE(  const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        (*lastPVStruct).back().COUNT = getCOUNT( keyVal[ 1 ] ); // lastPVStruct is set in addToPVStruct,
    else if( keyVal[0] == UTL::PV_MASK )
        (*lastPVStruct).back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        (*lastPVStruct).back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
void screenConfigReader::addToScrObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        screenStructs::screenObject scr = screenStructs::screenObject();
        scr.name = keyVal [ 1 ];
        scr.numIlocks = (size_t)numIlocks;
        scrObjects.push_back( scr );
        debugMessage( "Added ", scrObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            scrObjects.back().pvRoot = UTL::VM_PREFIX +  keyVal[ 1 ];
        else
            scrObjects.back().pvRoot =  keyVal[ 1 ];
    }
    else if( keyVal[0] == UTL::SCREEN_TYPE )
    {
        scrObjects.back().screenType =  getScreenType(keyVal[ 1 ]);
    }
    else if( keyVal[0] == UTL::V_RETRACTED )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_RETRACTED ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_RETRACTED ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_RETRACTED ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::V_MAX )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_MAX ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_MAX ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_MAX ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::V_SLIT_1 )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_SLIT_1 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_SLIT_1 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_SLIT_1 ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::V_RF )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_RF ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_RF ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_RF ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::V_MIRROR )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_MIRROR ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_MIRROR ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_MIRROR ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::V_YAG )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_YAG ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_YAG ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_YAG ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::V_GRAT )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::V_GRAT ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::V_GRAT ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::V_GRAT ] = screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    else if( keyVal[0] == UTL::H_RETRACTED )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_RETRACTED ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_RETRACTED ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_RETRACTED ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    else if( keyVal[0] == UTL::H_SLIT_1 )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_SLIT_1 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_SLIT_1 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_SLIT_1 ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    else if( keyVal[0] == UTL::H_SLIT_2 )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_SLIT_2 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_SLIT_2 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_SLIT_2 ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    else if( keyVal[0] == UTL::H_SLIT_3)
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_SLIT_3 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_SLIT_3 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_SLIT_3 ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    else if( keyVal[0] == UTL::H_APT_1 )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_APT_1 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_APT_1 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_APT_1 ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    else if( keyVal[0] == UTL::H_APT_2 )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_APT_2 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_APT_2 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_APT_2 ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    else if( keyVal[0] == UTL::H_APT_3 )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::H_APT_3 ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::H_APT_3 ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::H_APT_3 ] = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
}
////______________________________________________________________________________________________________
//void screenConfigReader::setCassetteElementExists( screenStructs::screenCassette & cas, const std::string & element )
//{
//    cas.cassetteElements[ element ] = true;
//}
//______________________________________________________________________________________________________
screenStructs::SCREEN_TYPE screenConfigReader::getScreenType( const std::string & val )
{
    screenStructs::SCREEN_TYPE ret = screenStructs::SCREEN_TYPE::UNKNOWN_SCREEN_TYPE;
    if( val == UTL::VELA_HV_MOVER )
        ret = screenStructs::SCREEN_TYPE::VELA_HV_MOVER;
    else if( val == UTL::VELA_PNEUMATIC )
        ret = screenStructs::SCREEN_TYPE::VELA_PNEUMATIC;
    else if( val == UTL::CLARA_HV_MOVER )
        ret = screenStructs::SCREEN_TYPE::CLARA_HV_MOVER;
    else if( val == UTL::CLARA_V_MOVER )
        ret = screenStructs::SCREEN_TYPE::CLARA_V_MOVER;
    else if( val == UTL::CLARA_PNEUMATIC )
        ret = screenStructs::SCREEN_TYPE::CLARA_PNEUMATIC;
    return ret;
}
//______________________________________________________________________________________________________
bool screenConfigReader::readConfig( screenConfigReader & obj, const std::string & fn, aKeyValMemFn f1, aKeyValMemFn f2)
{
    debugMessage( "\n", "**** Attempting to Read ", fn, " ****" );
    if( usingVirtualMachine )
        debugMessage( "\n", "**** Using VIRTUAL Machine PVs ****" );
    else
        debugMessage(  "**** Using PHYSICAL Machine PVs  ****","\n" );

    std::string line, trimmedLine;
    bool success = false;

    std::ifstream inputFile;

    inputFile.open( fn, std::ios::in );
    if( inputFile )
    {
        std::cout << "found input file" << std::endl;

        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;
        while( std::getline( inputFile, line ) ) ///Go through line by line
        {
            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
            if (trimmedLine.size() > 0 )
            {
                if( stringIsSubString( line, UTL::END_OF_DATA ) )
                {
                    debugMessage( "found END_OF_DATA" );
                    readingData = false;
                    readingObjs = false;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    break;
                }
                // this is the main flag, you can change paramaters with this flag set
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
                                        CALL_MEMBER_FN(obj, f1)( keyVal );


                                    else if( readingCommandPVs )
                                    {
                                        CALL_MEMBER_FN(obj, f2)( keyVal );
                                    }


                                    else if( readingMonitorPVs )
                                        CALL_MEMBER_FN(obj, f2)( keyVal );
                                }
                                break;
                            default:
                                message("!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!" );
                        }
                    }
                }
                if( stringIsSubString( line, UTL::START_OF_DATA) )
                {
                    readingData = true;
                    debugMessage( "Found START_OF_DATA" );

                }
                if ( stringIsSubString( line, UTL::PV_COMMANDS_START ) )
                {
                    readingCommandPVs = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage( "FOUND PV_COMMANDS_START" );
                }
                if( stringIsSubString( line, UTL::PV_MONITORS_START ) )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage( "FOUND PV_MONITORS_START" );
                }
                if( stringIsSubString( line, UTL::OBJECTS_START ) )
                {
                    readingObjs = true;
                    readingCommandPVs  = false;
                    readingMonitorPVs  = false;
                    debugMessage( "FOUND OBJECTS_START" );
                }
            }
        }
        inputFile.close( );
        debugMessage( "File Closed" );
        if( numObjs == scrObjects.size() )
        {
            success = true;
            debugMessage( "Number of objects created, ",scrObjects.size()," == ",numObjs,", objects delared in Config" );
        }
    }


    return success;

}
//__________________________________________________________________________________________
bool screenConfigReader::getScreenObjects( std::map< std::string, screenStructs::screenObject > & mapToFill )
{
    mapToFill.clear();

    size_t nCount = 0;
    bool success = false;
    for( auto &&  it : scrObjects )
    {
        // add in magnetObj
        mapToFill[ it.name ] = it;
        switch( it.screenType )
        {
            case screenStructs::SCREEN_TYPE::VELA_PNEUMATIC:
                message( it.name, " is a VELA_PNEUMATIC screen");
                for( auto && it2 : pvScrMonStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrComStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                break;
            case screenStructs::SCREEN_TYPE::CLARA_PNEUMATIC:
                message( it.name, " is a CLARA_PNEUMATIC screen");
                for( auto && it2 : pvScrMonStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrComStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                break;

            case screenStructs::SCREEN_TYPE::VELA_HV_MOVER:
                message( it.name, " is a VELA_HV_MOVER screen");
                for( auto && it2 : pvScrComStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                break;

            case screenStructs::SCREEN_TYPE::CLARA_HV_MOVER:
                message( it.name, " is a CLARA_HV_MOVER screen");
                for( auto && it2 : pvScrComStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                break;
            case screenStructs::SCREEN_TYPE::CLARA_V_MOVER:
                message( it.name, " is a CLARA_V_MOVER screen");
                for( auto && it2 : pvScrComStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
        }
        nCount+=1;
    }
    if ( nCount == scrObjects.size() )
        success = true;
    return success;
}
////______________________________________________________________________________
//void screenConfigReader::addToScrObjectsV1( const std::vector<std::string> &keyVal )
//{
//    if( keyVal[0] == UTL::NAME )
//    {
//        screenStructs::screenObject scr = screenStructs::screenObject();
//        scr.name = keyVal [ 1 ];
//        scr.numIlocks = (size_t)numIlocks;
//        // each screen gets a driver and an H and V driver status
//        scr.driver.parentScreen            = scr.name;
//        scr.driver.hDriverSTA.parentScreen = scr.name;
//        scr.driver.vDriverSTA.parentScreen = scr.name;
//        scr.driver.hDriverSTA.dir = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
//        scr.driver.vDriverSTA.dir = screenStructs::DRIVER_DIRECTION::VERTICAL;
//
//        // each screen gets an H and V cassette elements
//        scr.driver.hCassette.parentScreen  = scr.name;
//        scr.driver.vCassette.parentScreen  = scr.name;
//        scr.driver.hCassette.dir  = screenStructs::DRIVER_DIRECTION::HORIZONTAL;
//        scr.driver.vCassette.dir  = screenStructs::DRIVER_DIRECTION::VERTICAL;
//        // set all elements in the cassette object's existanece map to false
//        for( auto && it : UTL::allScreenCassetteElements  )// allScreenCassetteElements is a list of all possible elements
//        {
//             scr.driver.hCassette.cassetteElements[ screenStructs::hCassetteElementMap.at(it) ] = false;
//             scr.driver.vCassette.cassetteElements[ screenStructs::vCassetteElementMap.at(it) ] = false;
//        }
//        scrObjects.push_back( scr );
//        debugMessage( "Added ", scrObjects.back().name );
//    }
//    else if( keyVal[0] == UTL::PV_ROOT )
//    {
//        if( usingVirtualMachine )
//            scrObjects.back().pvRoot = UTL::VM_PREFIX +  keyVal[ 1 ];
//        else
//            scrObjects.back().pvRoot =  keyVal[ 1 ];
//    }
//    else if( keyVal[0] == UTL::SCREEN_DRIVER_POS_TOLERANCE )
//    {
//        //message( "set ", scrObjects.back().name, " pos tolerance to ", getNumD(keyVal[1]));
//        scrObjects.back().driver.hCassette.posTolerance = getNumD(keyVal[1]);
//        scrObjects.back().driver.vCassette.posTolerance = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::SCREEN_TYPE )
//        scrObjects.back().screenType = getScreenType( keyVal[ 1 ] );
//    // the cassette  knows all the positions of screens, slits etc, plus current position
//    // if a cassette element exists we update the cassetteElements  map to refelct this (init to false above)
//    else if( keyVal[0] == UTL::H_MIRROR )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[ screenStructs::hCassetteElementMap.at(UTL::MIRROR) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::MIRROR)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_MIRROR )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::MIRROR) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::MIRROR) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_50_UM )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::SLIT_50_UM) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::SLIT_50_UM) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_SLIT_50_UM )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::SLIT_50_UM) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::SLIT_50_UM) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_25_UM )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::SLIT_25_UM) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::SLIT_25_UM)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_SLIT_25_UM )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[        screenStructs::vCassetteElementMap.at(UTL::SLIT_25_UM) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::SLIT_25_UM) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_HOLE_6p3_MM )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::HOLE_6p3_MM) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::HOLE_6p3_MM)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_HOLE_6p3_MM )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::HOLE_6p3_MM) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::HOLE_6p3_MM) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_HOLE_10_MM )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::HOLE_10_MM) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::HOLE_10_MM)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_HOLE_10_MM )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::HOLE_10_MM) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::HOLE_10_MM) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_YAG )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::YAG) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::YAG)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_YAG )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::YAG) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::YAG) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_OUT )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::OUT) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::OUT)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_OUT )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::OUT) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::OUT) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_RF )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::RF) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::RF)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_RF )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::RF) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::RF) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_1 )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::H_SLIT_1) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::H_SLIT_1)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_2 )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::H_SLIT_2) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::H_SLIT_2) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_3)
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::H_SLIT_3) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::H_SLIT_3)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_SLIT_1 )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::V_SLIT_1) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::V_SLIT_1) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_4MM_APERTURE)
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::H_4MM_APERTURE) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::H_4MM_APERTURE)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_6MM_APERTURE )
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::H_6MM_APERTURE) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::H_6MM_APERTURE) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_9MM_APERTURE)
//    {
//        scrObjects.back().driver.hCassette.cassetteElements[         screenStructs::hCassetteElementMap.at(UTL::H_9MM_APERTURE) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::H_9MM_APERTURE)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_GRATICULE )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::V_GRATICULE) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::V_GRATICULE) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_COLLIMATOR )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::V_COLLIMATOR) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::V_COLLIMATOR) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_MIRROR_POS )
//    {
//        // the cassettes have a map of whether the the element exists, and it's "offline" position, from the config file
//        //scrObjects.back().driver.hCassette.cassetteElementsPosition[ UTL::MIRROR_POS] = getNumD(keyVal[1]);
//        scrObjects.back().driver.hCassette.cassetteElements[ screenStructs::hCassetteElementMap.at(UTL::MIRROR_POS) ] = true;
//        scrObjects.back().driver.hCassette.cassetteElementsPosition[ screenStructs::hCassetteElementMap.at(UTL::MIRROR_POS)] = getNumD(keyVal[1]);
//        //setCassetteElementExists( scrObjects.back().driver.hCassette, UTL::MIRROR_POS );
//    }
//    else if( keyVal[0] == UTL::V_MIRROR_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElements[         screenStructs::vCassetteElementMap.at(UTL::MIRROR_POS) ] = true;
//        scrObjects.back().driver.vCassette.cassetteElementsPosition[ screenStructs::vCassetteElementMap.at(UTL::MIRROR_POS) ] = getNumD(keyVal[1]);
////        scrObjects.back().driver.vCassette.cassetteElementsPosition[ UTL::MIRROR_POS] = getNumD(keyVal[ 1 ]);
////        setCassetteElementExists( scrObjects.back().driver.vCassette, UTL::MIRROR_POS );
//    }
//    else if( keyVal[0] == UTL::H_SLIT_50_UM_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::SLIT_25_UM_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_SLIT_50_UM_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::SLIT_25_UM_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_25_UM_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::SLIT_25_UM_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_SLIT_25_UM_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::SLIT_25_UM_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_HOLE_6p3_MM_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::HOLE_6p3_MM_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_HOLE_6p3_MM_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::HOLE_6p3_MM_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_HOLE_10_MM_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::HOLE_10_MM_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_HOLE_10_MM_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::HOLE_10_MM_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_YAG_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::YAG_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_YAG_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::YAG_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_OUT_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::OUT_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_OUT_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::OUT_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_RF_POS)
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::RF_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_RF_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::RF_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_1_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::H_SLIT_1_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_2_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::H_SLIT_2_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_SLIT_3_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::H_SLIT_3_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_SLIT_1_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::V_SLIT_1_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_4MM_APERTURE_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::H_4MM_APERTURE_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_6MM_APERTURE_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::H_6MM_APERTURE_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::H_9MM_APERTURE_POS )
//    {
//        scrObjects.back().driver.hCassette.cassetteElementsPos[ screenStructs::hCassetteElementMap.at(UTL::H_9MM_APERTURE_POS)] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_GRATICULE_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::V_GRATICULE_POS) ] = getNumD(keyVal[1]);
//    }
//    else if( keyVal[0] == UTL::V_COLLIMATOR_POS )
//    {
//        scrObjects.back().driver.vCassette.cassetteElementsPos[ screenStructs::vCassetteElementMap.at(UTL::V_COLLIMATOR_POS) ] = getNumD(keyVal[1]);
//    }
//    // at the moment the H and V driver status are identical, so they are not distinguished in the config
//    else if( keyVal[0] == UTL::NUM_STA_BITS )
//    {
//        scrObjects.back().driver.hDriverSTA.numSTABits = getNum(keyVal[ 1 ]);
//        scrObjects.back().driver.vDriverSTA.numSTABits = getNum(keyVal[ 1 ]);
//    }
//    else if( keyVal[0] == UTL::STA_BIT )
//    {
//        /// ad this bit to the map and lable for H and V
//        scrObjects.back().driver.hDriverSTA.STA_bit_label.push_back( keyVal[1] );
//        //scrObjects.back().driver.hDriverSTA.STA_bit_label.push_back( keyVal[1] );
//        scrObjects.back().driver.hDriverSTA.STA_bit_map[ keyVal[1] ] = false;
//        //scrObjects.back().driver.hDriverSTA.STA_bit_map[ keyVal[1] ] = false;
//
//        //scrObjects.back().driver.vDriverSTA.STA_bit_label.push_back( keyVal[1] );
//        scrObjects.back().driver.vDriverSTA.STA_bit_label.push_back( keyVal[1] );
//        //scrObjects.back().driver.vDriverSTA.STA_bit_map[ keyVal[1] ] = false;
//        scrObjects.back().driver.vDriverSTA.STA_bit_map[ keyVal[1] ] = false;
//    }
//}
////______________________________________________________________________________________________________
//bool screenConfigReader::readConfig( screenConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
//{
//
//   debugMessage( "\n", "**** Attempting To Read ", fn, " ****");
//
//   std::string line, trimmedLine;
//   bool success = false;
//
//   std::ifstream inputFile;
//
//    inputFile.open( fn.c_str(), std::ios::in );
//    if( inputFile )
//    {
//        bool readingData       = false;
//        bool readingObjs       = false;
//        bool readingCommandPVs = false;
//        bool readingMonitorPVs = false;
//
//
//        debugMessage( "File opened from ", fn );
//        while( std::getline( inputFile, line ) ) ///Go through line by line
//        {
//            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
//            if (trimmedLine.size() > 0 )
//            {
//                if( stringIsSubString( line, UTL::END_OF_DATA ) )
//                {
//                    debugMessage( "found END_OF_DATA" );
//                    readingData = false;
//                    readingObjs = false;
//                    readingCommandPVs  = false;
//                    readingMonitorPVs  = false;
//                    break;
//                }
//                if( readingData )
//                {
//                    if( stringIsSubString( trimmedLine, UTL::VERSION ) )
//                        getVersion( trimmedLine );
//                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_OBJECTS ) )
//                        getNumObjs( trimmedLine );
//                    else if( stringIsSubString( trimmedLine, UTL::NUMBER_OF_ILOCKS ) )
//                        getNumIlocks( trimmedLine );
//                    else
//                    {
//                        switch( configVersion )
//                        {
//                            case 1:
//                                if( trimmedLine.find_first_of( UTL::EQUALS_SIGN ) != std::string::npos )
//                                {
//                                    std::vector<std::string> keyVal = getKeyVal( trimmedLine );
//
//
//                                    if( readingObjs )
//                                        CALL_MEMBER_FN(obj, f1)( keyVal );
//
//
//                                    else if( readingCommandPVs )
//                                        CALL_MEMBER_FN(obj, f2)( keyVal );
//
//
//                                    else if( readingMonitorPVs )
//                                        CALL_MEMBER_FN(obj, f3)( keyVal );
//                                }
//                                break;
//                            default:
//                                message("!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!" );
//                        }
//                    }
//                }
//                if( stringIsSubString( line, UTL::START_OF_DATA) )
//                {
//                    readingData = true;
//                    debugMessage( "Found START_OF_DATA" );
//
//                }
//                if ( stringIsSubString( line, UTL::PV_COMMANDS_START ) )
//                {
//                    readingCommandPVs = true;
//                    readingObjs = false;
//                    readingMonitorPVs = false;
//                    debugMessage( "FOUND PV_COMMANDS_START" );
//                }
//                if( stringIsSubString( line, UTL::PV_MONITORS_START ) )
//                {
//                    readingCommandPVs = false;
//                    readingObjs       = false;
//                    readingMonitorPVs = true;
//                    debugMessage( "FOUND PV_MONITORS_START" );
//                }
//                if( stringIsSubString( line, UTL::OBJECTS_START ) )
//                {
//                    readingObjs = true;
//                    readingCommandPVs  = false;
//                    readingMonitorPVs  = false;
//                    debugMessage( "FOUND OBJECTS_START" );
//                }
//            }
//        }
//
//        inputFile.close( );
//        debugMessage( "File Closed" );
//        success = true;
//    }
//    else
//    {
//        message("\t\tError Can't Open Screen Config File after searching for: " , fn , "!!!!" );
//    }
//    return success;
//}
//
//
//
////______________________________________________________________________________________________________
////bool screenConfigReader::readConfigFiles()
////{
////
////    bool success = true;
////    /// There are 2 objects for the screens, a YAG 01 object, a complex YAG object for screens 1/2/3 and
////    /// a simple YAG object for the remaining screens.
////    /// They are both defined in separate config files
////    /// They should still all end up in a screenObject
////
////    ///COMPLEX YAGS
////
////    complexYAGObjects.clear();
////    complexYAGMonStructs.clear();
////    bool complexYAGSuccess = readConfig( *this, configFile1, &screenConfigReader::addToComplexYAGObjectsV1,&screenConfigReader::addToComplexYAGComStructsV1, &screenConfigReader::addToComplexYAGMonStructsV1 );
////    if(!complexYAGSuccess )
////        success = false;
////    if( numObjs == complexYAGObjects.size() )
////        debugMessage("***Created ", numObjs, " COMPLEX YAG Objects, As Expected", "\n" );
////    else
////    {
////        debugMessage("***Created ", complexYAGObjects.size(), " COMPLEX YAG Objects, Expected ", numObjs, "ERROR ***", "\n" );
////        success = false;
////    }
////
////    ///SIMPLE YAGS
////
////    simpleYAGObjects.clear();
////    simpleYAGMonStructs.clear();
////    bool simpleYAGSuccess = readConfig( *this, configFile2, &screenConfigReader::addToSimpleYAGObjectsV1, &screenConfigReader::addToSimpleYAGComStructsV1, &screenConfigReader::addToSimpleYAGMonStructsV1 );
////    if( !simpleYAGSuccess )
////        success = false;
////    if( numObjs == simpleYAGObjects.size() )
////        debugMessage("***Created ", numObjs, " SIMPLE YAG Objects, As Expected", "\n" );
////    else
////    {
////        debugMessage( "***Created ", simpleYAGObjects.size(), " SIMPLE YAG Objects, Expected ", numObjs, " ERROR ***", "\n" );
////        success = false;
////    }
////
////    return success;
////
////}
//
//
////______________________________________________________________________________________________________________
//void screenConfigReader::addToComplexYAGMonStructsV1( const std::vector<std::string> &keyVal )
//{
//    addPVStruct( complexYAGMonStructs, keyVal );
//}
////______________________________________________________________________________________________________________
//void screenConfigReader::addToSimpleYAGMonStructsV1( const std::vector<std::string> &keyVal )
//{
//    addPVStruct( simpleYAGMonStructs, keyVal );
//}
////______________________________________________________________________________________________________________
//void screenConfigReader::addToComplexYAGComStructsV1( const std::vector<std::string> &keyVal )
//{
//    addPVStruct( complexYAGComStructs, keyVal );
//}
////_____________________________________________________________________________________________________________
//void screenConfigReader::addToSimpleYAGComStructsV1( const std::vector<std::string> &keyVal )
//{
//    addPVStruct( simpleYAGComStructs, keyVal );
//}
////______________________________________________________________________________________________________________
//double screenConfigReader::getPOS( const std::string & val )///Check whether this should be std::string & val like getCOUNT is, why does it not work when doing that?
//{
//    return std::stod( val.c_str() );
//}
////______________________________________________________________________________________________________________
//void screenConfigReader::addToComplexYAGObjectsV1( const std::vector<std::string> &keyVal )
//{
//    if( keyVal[0] == UTL::NAME )
//    {
//        screenStructs::COMPLEX_YAG_Object comp = screenStructs::COMPLEX_YAG_Object();
//        comp.name = keyVal [ 1 ];
//        comp.h_screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
//        comp.v_screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
//        comp.numIlocks = numIlocks;
//        complexYAGObjects.push_back( comp );
//        debugMessage( "Added ", complexYAGObjects.back().name );
//    }
//
//    else if( keyVal[0] == UTL::PV_ROOT )
//        complexYAGObjects.back().pvRoot = keyVal[ 1 ];
//
//    else if( keyVal[0] == UTL::H_MIRROR )
//        complexYAGObjects.back().H_MIRROR = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::H_50U_SLIT )
//        complexYAGObjects.back().H_50U_SLIT = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::H_25U_SLIT )
//        complexYAGObjects.back().H_25U_SLIT = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::H_63MM_HOLE )
//        complexYAGObjects.back().H_63MM_HOLE = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::H_10MM_HOLE )
//        complexYAGObjects.back().H_10MM_HOLE = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::V_YAG )
//        complexYAGObjects.back().V_YAG = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::V_SLIT )
//        complexYAGObjects.back().V_SLIT = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::H_SLIT )
//        complexYAGObjects.back().H_SLIT = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::H_OUT )
//        complexYAGObjects.back().H_OUT = getPOS(keyVal[ 1 ]);
//
//    else if( keyVal[0] == UTL::V_OUT )
//        complexYAGObjects.back().V_OUT = getPOS(keyVal[ 1 ]);
//}
////________________________________________________________________________________________________________________________
//void screenConfigReader::addToSimpleYAGObjectsV1( const std::vector<std::string> &keyVal )
//{
//    if( keyVal[0] == UTL::NAME )
//    {
//       /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
//        /// init structs 'correctly'
//        screenStructs::SIMPLE_YAG_Object simp = screenStructs::SIMPLE_YAG_Object();
//        simp.name = keyVal[ 1 ];
//        simp.screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
//        simp.numIlocks = numIlocks;
//        simpleYAGObjects.push_back( simp );
//        debugMessage("Added ", simpleYAGObjects.back().name );
//    }
//    else if( keyVal[0] == UTL::PV_ROOT )
//        simpleYAGObjects.back().pvRoot = keyVal[ 1 ];
//
//    else if ( keyVal[0] == UTL::IN )
//        simpleYAGObjects.back().IN = getPOS(keyVal[ 1 ]);
//
//    else if ( keyVal[0] == UTL::OUT )
//        simpleYAGObjects.back().OUT = getPOS(keyVal[ 1 ]);
//}
////_____________________________________________________________________________________________________________________________________
//void screenConfigReader::addPVStruct( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
//{
//    // http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
//    /// initstd::map< VELA_ENUM::ILOCK_NUMBER, std::string > structs 'correctly'
//   if( stringIsSubString( keyVal[0], "SUFFIX" ) )
//    {
//
//        pvStruct_v.push_back( screenStructs::pvStruct() );
//        pvStruct_v.back().pvSuffix = keyVal[1];
//
//        ///SIMPLE YAGs
//        if( keyVal[0] == UTL::PV_SUFFIX_ON )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::On;
//        else if( keyVal[0] == UTL::PV_SUFFIX_OFF )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::Off;
//        else if( keyVal[0] == UTL::PV_SUFFIX_STA )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::Sta;
//        ///COMPLEX YAGs
//        else if( keyVal[0] == UTL::PV_SUFFIX_H_MABS )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_MABS;
//        else if( keyVal[0] == UTL::PV_SUFFIX_V_MABS )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_MABS;
//        else if( keyVal[0] == UTL::PV_SUFFIX_STOP )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::STOP;
//        else if( keyVal[0] == UTL::PV_SUFFIX_H_RPOS )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_RPOS;
//        else if( keyVal[0] == UTL::PV_SUFFIX_V_RPOS )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_RPOS;
//        else if( keyVal[0] == UTL::PV_SUFFIX_H_PROT01 )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_PROT01;
//        else if( keyVal[0] == UTL::PV_SUFFIX_V_PROT01 )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_PROT01;
//        else if( keyVal[0] == UTL::PV_SUFFIX_PROT03 )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::PROT03;
//        else if( keyVal[0] == UTL::PV_SUFFIX_PROT05 )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::PROT05;
//        else if( keyVal[0] == UTL::PV_SUFFIX_H_RPWRLOSS )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_RPWRLOSS;
//        else if( keyVal[0] == UTL::PV_SUFFIX_V_RPWRLOSS )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_RPWRLOSS;
//
//
//       debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType ) );
//    }
//    else
//        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
//}
////______________________________________________________________________________________________________________________
//void screenConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
//{
//    if(keyVal[0] == UTL::PV_COUNT )
//        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_MASK )
//        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
//    else if( keyVal[0] == UTL::PV_CHTYPE )
//        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
//}




