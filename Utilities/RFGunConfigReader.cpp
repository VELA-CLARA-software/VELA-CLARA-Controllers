//djs
#include "RFGunConfigReader.h"
#include "configDefinitions.h"
#include "RFGunStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>
//BJAS
#include <regex>

RFGunConfigReader::RFGunConfigReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
RFGunConfigReader::RFGunConfigReader( const std::string configFileLocation1,
                                      const std::string configFileLocation2,
                                      const std::string configFileLocation3, const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
:  configReader( configFileLocation1, configFileLocation2, configFileLocation3, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
RFGunConfigReader::~RFGunConfigReader(){}
//______________________________________________________________________________
RFGunStructs::rfObject RFGunConfigReader::getRFObject()
{
    RFGunStructs::rfObject obj;

    obj.name = "VELA_RF_GUN"; /// MAGIC NAME!

    obj.mod = RFModObject;

    /// In the final object the pvStructs are in a map keyed by PV Type...

    for(auto && it : pvModMonStructs )
        obj.mod.pvMonStructs[ it.pvType ] = it;

    for(auto && it : pvModComStructs )
        obj.mod.pvComStructs[ it.pvType ] = it;


    obj.mod.badModErrorReadStr = RFModObject.badModErrorReadStr;
    obj.mod.goodModErrorReadStr = RFModObject.goodModErrorReadStr;

    /// rfPowerObjects are in a map keyed by name ...

    for(auto && it : rfPowerObjects )
        obj.powerObjects[ it.name ] = it;

    /// Then we add in the monitor structs, for each power object ...

    for(auto && it : obj.powerObjects )
        for( auto it2 : pvPWRMonStructs )
            it.second.pvMonStructs[ it2.pvType ] = it2;
    /// LLRF
    obj.llrf = RFLLRFObject;
    for(auto && it : pvLLRFMonStructs )
        obj.llrf.pvMonStructs[ it.pvType ] = it;

    return obj;
}
//______________________________________________________________________________
bool RFGunConfigReader::readConfigFiles()
{
    bool success = true;
    /// There are 3 types of objects in the RF gun, a modulator, power (x2) and LLRF;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an rfObject
    //Power
    rfPowerObjects.clear();
    pvPWRMonStructs.clear();
    bool pwrSuccess = readConfig( *this,   configFile1, &RFGunConfigReader::addToRFPowObjectsV1, nullptr, &RFGunConfigReader::addToRFPowPVMonStructsV1 );
    if( !pwrSuccess )
        success = false;

    if( numObjs == rfPowerObjects.size() )
        debugMessage( "*** Created ", numObjs, " RF Power Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", rfPowerObjects.size() ," RF Power Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }
    //LLRF
    pvLLRFMonStructs.clear();
    bool llrfSuccess = readConfig( *this,   configFile2, &RFGunConfigReader::addToLLRFObjectsV1,nullptr, &RFGunConfigReader::addToLLRFPVMonStructsV1 );
    if( !llrfSuccess )
        success = false;
    if( numObjs == 1 ) /// MAGIC NUMBER
        debugMessage( "*** Created ", numObjs, " LLRF Objects, As Expected ***" );
    else
    {
        debugMessage( "*** Created ", 1 ," LLRF Objects, Expected ", numObjs,  " ERROR ***"  ); /// MAGIC NUMBER
        success = false;
    }
    //Modulator
    bool modSuccess = readConfig( *this,  configFile3, &RFGunConfigReader::addToModObjectsV1, &RFGunConfigReader::addToModPVComStructsV1, &RFGunConfigReader::addToModPVMonStructsV1 );
    if( !modSuccess )
        success = false;
    if( numObjs == 1 ) /// MAGIC NUMBER
        debugMessage( "*** Created ", numObjs, " RF Modulator Objects, As Expected ***" );
    else
    {
        debugMessage( "*** Created ", 1 ," RF Modulator, Expected ", numObjs,  " ERROR ***"  ); /// MAGIC NUMBER
        success = false;
    }

    return success;
}
//______________________________________________________________________________
void RFGunConfigReader::addToModPVMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvModMonStructs, keyVal);
}
//______________________________________________________________________________
void RFGunConfigReader::addToModPVComStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvModComStructs, keyVal);
}
//______________________________________________________________________________
void RFGunConfigReader::addToRFPowPVMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvPWRMonStructs, keyVal );
}
//______________________________________________________________________________
void RFGunConfigReader::addToLLRFPVMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvLLRFMonStructs, keyVal );
}
//______________________________________________________________________________
void RFGunConfigReader::addToLLRFObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        RFLLRFObject.name = keyVal[ 1 ];
//        RFGunStructs::rfLLRFObject rfob = RFGunStructs::rfLLRFObject();
//        rfob.name = keyVal[ 1 ];
//        rfLLRFObjects.push_back( rfob );
        debugMessage("Added ", RFLLRFObject.name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        RFLLRFObject.pvRoot = keyVal[ 1 ];
        //rfLLRFObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________
void RFGunConfigReader::addToRFPowObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        RFGunStructs::rfPowerObject rfob = RFGunStructs::rfPowerObject();
        rfob.name = keyVal[ 1 ];
        rfPowerObjects.push_back( rfob );
        debugMessage("Added ", rfPowerObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        rfPowerObjects.back().pvRoot = keyVal[ 1 ];
}
//______________________________________________________________________________
void RFGunConfigReader::addToModObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        //RFGunStructs::rfModObject rfob = RFGunStructs::rfModObject();
        RFModObject.name = keyVal[ 1 ];
        debugMessage("Added ", RFModObject.name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        RFModObject.pvRoot = keyVal[ 1 ];


    else if( keyVal[0] == UTL::GOOD_MOD_ERR )
    {
        RFModObject.goodModErrorReadStr.push_back(  keyVal[ 1 ] );
                debugMessage("Added ", RFModObject.goodModErrorReadStr.back(), " to good ErrorRead Codes." );
    }

    else if( keyVal[0] == UTL::BAD_MOD_ERR )
    {
        RFModObject.badModErrorReadStr.push_back(  keyVal[ 1 ] );
        debugMessage("Added ", RFModObject.badModErrorReadStr.back(), " to bad ErrorRead Codes." );
    }


}
//______________________________________________________________________________
void RFGunConfigReader::addToPVStruct( std::vector< RFGunStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( RFGunStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // POWER PVs
        if( keyVal[0] == UTL::PV_SUFFIX_REV  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_REV;
        else if( keyVal[0] == UTL::PV_SUFFIX_FWD  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_FWD;
        else if( keyVal[0] == UTL::PV_SUFFIX_REVT  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_REVT;
        else if( keyVal[0] == UTL::PV_SUFFIX_FWDT  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_FWDT;
        else if( keyVal[0] == UTL::PV_SUFFIX_REVTM  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_REVTM;
        else if( keyVal[0] == UTL::PV_SUFFIX_FWDTM  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_FWDTM;
        else if( keyVal[0] == UTL::PV_SUFFIX_RATIO  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PWR_RATIO;
        // LLRF PVs
        else if( keyVal[0] == UTL::PV_SUFFIX_AMPR  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_AMP_READ;
        else if( keyVal[0] == UTL::PV_SUFFIX_PHI  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_PHI;
        else if( keyVal[0] == UTL::PV_SUFFIX_AMPW  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::RF_AMP_WRITE;
        // Modulator

        else if( keyVal[0] == UTL::PV_SUFFIX_RESET  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::MOD_RESET;

        else if( keyVal[0] == UTL::PV_SUFFIX_STATESET  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::MOD_STATE_SET;

        else if( keyVal[0] == UTL::PV_SUFFIX_STATEREAD  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::MOD_STATE_READ;

        else if( keyVal[0] == UTL::PV_SUFFIX_EXILOCK1  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::MOD_ERROR_READ;

        else if( keyVal[0] == UTL::PV_SUFFIX_EXILOCK2  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::MOD_ERROR_READ_STR;

        else if( keyVal[0] == UTL::PV_SUFFIX_WARMUPT  )
            pvStruct_v.back().pvType = RFGunStructs::RF_PV_TYPE::MOD_WARMUP_TIME;
        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void RFGunConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< RFGunStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
bool RFGunConfigReader::readConfig( RFGunConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
{
  	std::regex config_line("([A-Z_]+?)(?:\\s*=\\s*(.*))?;");
 	std::string parameter;
 	std::string value;
	std::smatch matches;
    std::vector<std::string> keyVal;

    debugMessage( "\n", "**** Attempting to Read ", fn, " ****" );

    std::string line, trimmedLine;
    bool success = false;

    std::ifstream inputFile;

    inputFile.open( fn, std::ios::in );
    if( inputFile )
    {
        bool readingData       = false;
        bool readingObjs       = false;
        bool readingCommandPVs = false;
        bool readingMonitorPVs = false;

        debugMessage( "File Opened from ", fn );
        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
        {
//            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
            //BJAS: use regex rather than parsing manually, it's more hygienic
            if( regex_search(line, matches, config_line) )
            {
                parameter = matches[1];
                value = matches[2];
                keyVal.clear();
                keyVal.push_back(parameter);
                keyVal.push_back(value);
                if( stringIsSubString( parameter, UTL::END_OF_DATA ) )
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
                    if( stringIsSubString( parameter, UTL::VERSION ) )
                        getVersion( value );
                    else if( stringIsSubString( parameter, UTL::NUMBER_OF_OBJECTS ) )
                        getNumObjs( value );
                    else if( stringIsSubString( parameter, UTL::NUMBER_OF_ILOCKS ) )
                        getNumIlocks( value );
                    else
                    {
                        switch( configVersion )
                        {
                            case 1:
                                if( trimmedLine.find_first_of( UTL::EQUALS_SIGN ) != std::string::npos )
                                {
                                    std::vector<std::string> keyVal = getKeyVal( trimmedLine );

                                if( readingObjs )
                                    CALL_MEMBER_FN(obj, f1)( keyVal ) ;

                                else if ( readingCommandPVs  )
                                    CALL_MEMBER_FN(obj, f2)( keyVal ) ;

                                else if ( readingMonitorPVs )
                                    CALL_MEMBER_FN(obj, f3)( keyVal ) ;
                                break;
                            default:
                                message( "!!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!!"  );
                        }
                    }
                }
                if( stringIsSubString( parameter, UTL::START_OF_DATA ) )
                {
                    readingData = true;
                    debugMessage( "Found START_OF_DATA" );
                }
                if( stringIsSubString( parameter, UTL::PV_COMMANDS_START ) )
                {
                    readingCommandPVs  = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage( "Found PV_COMMANDS_START" );
                }
                if( stringIsSubString( parameter, UTL::PV_MONITORS_START ) )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage( "Found PV_MONITORS_START" );
                }
                if( stringIsSubString( parameter, UTL::OBJECTS_START ) )
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
        message( "!!!! Error Can't Open File after searching for:  ", fn, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________



