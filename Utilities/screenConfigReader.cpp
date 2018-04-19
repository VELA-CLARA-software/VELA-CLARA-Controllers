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
screenConfigReader::screenConfigReader( const std::string & configFile1,
                                        const bool& show_messages_ptr,
                                        const bool& show_debug_messages_ptr,
                                        const bool startVirtualMachine ):
configReader( configFile1, show_messages_ptr, show_debug_messages_ptr ),
configFile1( configFile1 ),
usingVirtualMachine(startVirtualMachine)
{
}
//_____________________________________________________________________________________________________
screenConfigReader::~screenConfigReader(){}
//______________________________________________________________________________________________________
bool screenConfigReader::readConfig()
{
    // clear local copies of structs
    scrObjects.clear();
    pvScrComStructs.clear();
    pvScrComPneumaticStructs.clear();
    pvScrComHStructs.clear();
    pvScrComVStructs.clear();
    pvScrMonStructs.clear();
    pvScrMonPneumaticStructs.clear();
    pvScrMonHStructs.clear();
    pvScrMonVStructs.clear();
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
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_MOVING, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_READY )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_READY, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_GET_DEV )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_GET_DEV, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_DEV_STATE )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_DEV_STATE, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_MAX_POS )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_MAX_POS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_DEV_CENT )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_DEV_CENT, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_ACTPOS )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_ACTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_EN )
        {
            addToPVStruct( pvScrMonHStructs, screenStructs::SCREEN_PV_TYPE::H_EN, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_MOVING )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_MOVING, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_READY )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_READY, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_GET_DEV )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_GET_DEV, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_DEV_STATE )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_DEV_STATE, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_MAX_POS )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_MAX_POS, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_DEV_CENT )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_DEV_CENT, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_ACTPOS )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_ACTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_EN )
        {
            addToPVStruct( pvScrMonVStructs, screenStructs::SCREEN_PV_TYPE::V_EN, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        /// COMMANDS
        else if( keyVal[0] == UTL::PV_SUFFIX_H_SDEV )
        {
            addToPVStruct( pvScrComHStructs, screenStructs::SCREEN_PV_TYPE::H_SDEV, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_TRIGGER )
        {
            addToPVStruct( pvScrComHStructs, screenStructs::SCREEN_PV_TYPE::H_TRIGGER, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_EX )
        {
            addToPVStruct( pvScrComHStructs, screenStructs::SCREEN_PV_TYPE::H_EX, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_TGTPOS )
        {
            addToPVStruct( pvScrComHStructs, screenStructs::SCREEN_PV_TYPE::H_TGTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_JOG )
        {
            addToPVStruct( pvScrComHStructs, screenStructs::SCREEN_PV_TYPE::H_JOG, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_H_JDIFF )
        {
            addToPVStruct( pvScrComHStructs, screenStructs::SCREEN_PV_TYPE::H_JDIFF, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_SDEV )
        {
            addToPVStruct( pvScrComVStructs, screenStructs::SCREEN_PV_TYPE::V_SDEV, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_TRIGGER )
        {
            addToPVStruct( pvScrComVStructs, screenStructs::SCREEN_PV_TYPE::V_TRIGGER, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_EX )
        {
            addToPVStruct( pvScrComVStructs, screenStructs::SCREEN_PV_TYPE::V_EX, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_TGTPOS )
        {
            addToPVStruct( pvScrComVStructs, screenStructs::SCREEN_PV_TYPE::V_TGTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_JOG )
        {
            addToPVStruct( pvScrComVStructs, screenStructs::SCREEN_PV_TYPE::V_JOG, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_V_JDIFF )
        {
            addToPVStruct( pvScrComVStructs, screenStructs::SCREEN_PV_TYPE::V_JDIFF, keyVal[1], screenStructs::DRIVER_DIRECTION::VERTICAL );
        }
        /// PNEUMATIC
        if( keyVal[0] == UTL::PV_SUFFIX_MOVING )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::MOVING, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_READY )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::READY, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_GET_DEV )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::GET_DEV, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_DEV_STATE )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::DEV_STATE, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_MAX_POS )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::MAX_POS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_DEV_CENT )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::DEV_CENT, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_ACTPOS )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::ACTPOS, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_EN )
        {
            addToPVStruct( pvScrMonPneumaticStructs, screenStructs::SCREEN_PV_TYPE::EN, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_SDEV )
        {
            addToPVStruct( pvScrComPneumaticStructs, screenStructs::SCREEN_PV_TYPE::SDEV, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
        }
        else if( keyVal[0] == UTL::PV_SUFFIX_TRIGGER )
        {
            addToPVStruct( pvScrComPneumaticStructs, screenStructs::SCREEN_PV_TYPE::TRIGGER, keyVal[1], screenStructs::DRIVER_DIRECTION::HORIZONTAL );
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
    else if( keyVal[0] == UTL::YAG )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::YAG ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::YAG ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::YAG ] = screenStructs::DRIVER_DIRECTION::PNEUMATIC;
    }
    else if( keyVal[0] == UTL::RETRACTED )
    {
        scrObjects.back().elementExists[    screenStructs::SCREEN_STATE::RETRACTED ] = true;
        scrObjects.back().elementPositions[ screenStructs::SCREEN_STATE::RETRACTED ] = getNumUS(keyVal[1]);
        scrObjects.back().elementDirection[ screenStructs::SCREEN_STATE::RETRACTED ] = screenStructs::DRIVER_DIRECTION::PNEUMATIC;
    }
    else if( keyVal[0] == UTL::V_RETRACTED_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_RETRACTED ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::V_MAX_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_MAX ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::V_SLIT_1_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_SLIT_1 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::V_RF_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_RF ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::V_MIRROR_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_MIRROR ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::V_YAG_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_YAG ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::V_GRAT_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::V_GRAT ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_RETRACTED_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_RETRACTED ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_SLIT_1_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_SLIT_1 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_SLIT_2_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_SLIT_2 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_SLIT_3_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_SLIT_3 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_APT_1_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_APT_1 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_APT_2_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_APT_2 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::H_APT_3_POS )
    {
        scrObjects.back().devicePositions[ screenStructs::SCREEN_STATE::H_APT_3 ] = getNumD(keyVal[1]);
    }
    else if( keyVal[0] == UTL::POSITION )
    {
        scrObjects.back().position = getNumD(keyVal[1]);
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
        message(numObjs);
        message(scrObjects.size());
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
                for( auto && it2 : pvScrMonPneumaticStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrComPneumaticStructs )
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
                for( auto && it2 : pvScrComHStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonHStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrComVStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonVStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                break;

            case screenStructs::SCREEN_TYPE::CLARA_HV_MOVER:
                message( it.name, " is a CLARA_HV_MOVER screen");
                for( auto && it2 : pvScrComHStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonHStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrComVStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonVStructs )
                {
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                }
                break;
            case screenStructs::SCREEN_TYPE::CLARA_V_MOVER:
                message( it.name, " is a CLARA_V_MOVER screen");
                for( auto && it2 : pvScrComVStructs )
                {
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                }
                for( auto && it2 : pvScrMonVStructs )
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
//__________________________________________________________________________________________
