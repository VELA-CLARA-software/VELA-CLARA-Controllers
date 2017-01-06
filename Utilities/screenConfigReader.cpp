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
//BJAS
#include <regex>

screenConfigReader::screenConfigReader( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//____________________________________________________________________________________________________
screenConfigReader::screenConfigReader( const std::string & configFileLocation1,
                                        const std::string & configFileLocation2, const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
: configReader( configFileLocation1, configFileLocation2, show_messages_ptr, show_debug_messages_ptr )
{
}
//_____________________________________________________________________________________________________
screenConfigReader::~screenConfigReader(){}
//______________________________________________________________________________________________________
bool screenConfigReader::readConfigFiles()
{

    bool success = true;
    /// There are 2 objects for the screens, a YAG 01 object, a complex YAG object for screens 1/2/3 and
    /// a simple YAG object for the remaining screens.
    /// They are both defined in separate config files
    /// They should still all end up in a screenObject

    ///COMPLEX YAGS

    complexYAGObjects.clear();
    complexYAGMonStructs.clear();
    bool complexYAGSuccess = readConfig( *this, configFile1, &screenConfigReader::addToComplexYAGObjectsV1,&screenConfigReader::addToComplexYAGComStructsV1, &screenConfigReader::addToComplexYAGMonStructsV1 );
    if(!complexYAGSuccess )
        success = false;
    if( numObjs == complexYAGObjects.size() )
        debugMessage("***Created ", numObjs, " COMPLEX YAG Objects, As Expected", "\n" );
    else
    {
        debugMessage("***Created ", complexYAGObjects.size(), " COMPLEX YAG Objects, Expected ", numObjs, "ERROR ***", "\n" );
        success = false;
    }

    ///SIMPLE YAGS

    simpleYAGObjects.clear();
    simpleYAGMonStructs.clear();
    bool simpleYAGSuccess = readConfig( *this, configFile2, &screenConfigReader::addToSimpleYAGObjectsV1, &screenConfigReader::addToSimpleYAGComStructsV1, &screenConfigReader::addToSimpleYAGMonStructsV1 );
    if( !simpleYAGSuccess )
        success = false;
    if( numObjs == simpleYAGObjects.size() )
        debugMessage("***Created ", numObjs, " SIMPLE YAG Objects, As Expected", "\n" );
    else
    {
        debugMessage( "***Created ", simpleYAGObjects.size(), " SIMPLE YAG Objects, Expected ", numObjs, " ERROR ***", "\n" );
        success = false;
    }

    return success;

}

//______________________________________________________________________________________________________
bool screenConfigReader::readConfig( screenConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
{
  	std::regex config_line("([A-Z_]+?)(?:\\s*=\\s*(.*))?;");
 	std::string parameter;
 	std::string value;
	std::smatch matches;
    std::vector<std::string> keyVal;

   debugMessage( "\n", "**** Attempting To Read ", fn, " ****");

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


        debugMessage( "File opened from ", fn );
        while( std::getline( inputFile, line ) ) ///Go through line by line
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
                    debugMessage( "found END_OF_DATA" );
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
                                if( readingObjs )
                                    CALL_MEMBER_FN(obj, f1)( keyVal ) ;

                                else if ( readingCommandPVs  )
                                    CALL_MEMBER_FN(obj, f2)( keyVal ) ;

                                else if ( readingMonitorPVs )
                                    CALL_MEMBER_FN(obj, f3)( keyVal ) ;
                                break;
                            default:
                                message("!!!!WARNING DID NOT FIND CONFIG FILE VERSION NUMBER!!!!!" );
                        }
                    }
                }
                if( stringIsSubString( parameter, UTL::START_OF_DATA) )
                {
                    readingData = true;
                    debugMessage( "Found START_OF_DATA" );

                }
                if ( stringIsSubString( parameter, UTL::PV_COMMANDS_START ) )
                {
                    readingCommandPVs = true;
                    readingObjs = false;
                    readingMonitorPVs = false;
                    debugMessage( "FOUND PV_COMMANDS_START" );
                }
                if( stringIsSubString( parameter, UTL::PV_MONITORS_START ) )
                {
                    readingCommandPVs = false;
                    readingObjs       = false;
                    readingMonitorPVs = true;
                    debugMessage( "FOUND PV_MONITORS_START" );
                }
                if( stringIsSubString( parameter, UTL::OBJECTS_START ) )
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
        success = true;
    }
    else
    {
        message("\t\tError Can't Open Screen Config File after searching for: " , fn , "!!!!" );
    }
    return success;
}
//______________________________________________________________________________________________________________
void screenConfigReader::addToComplexYAGMonStructsV1( const std::vector<std::string> &keyVal )
{
    addPVStruct( complexYAGMonStructs, keyVal );
}
//______________________________________________________________________________________________________________
void screenConfigReader::addToSimpleYAGMonStructsV1( const std::vector<std::string> &keyVal )
{
    addPVStruct( simpleYAGMonStructs, keyVal );
}
//______________________________________________________________________________________________________________
void screenConfigReader::addToComplexYAGComStructsV1( const std::vector<std::string> &keyVal )
{
    addPVStruct( complexYAGComStructs, keyVal );
}
//_____________________________________________________________________________________________________________
void screenConfigReader::addToSimpleYAGComStructsV1( const std::vector<std::string> &keyVal )
{
    addPVStruct( simpleYAGComStructs, keyVal );
}
//______________________________________________________________________________________________________________
double screenConfigReader::getPOS( const std::string & val )///Check whether this should be std::string & val like getCOUNT is, why does it not work when doing that?
{
    return std::stod( val.c_str() );
}
//______________________________________________________________________________________________________________
void screenConfigReader::addToComplexYAGObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        screenStructs::COMPLEX_YAG_Object comp = screenStructs::COMPLEX_YAG_Object();
        comp.name = keyVal [ 1 ];
        comp.h_screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
        comp.v_screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
        comp.numIlocks = numIlocks;
        complexYAGObjects.push_back( comp );
        debugMessage( "Added ", complexYAGObjects.back().name );
    }

    else if( keyVal[0] == UTL::PV_ROOT )
        complexYAGObjects.back().pvRoot = keyVal[ 1 ];

    else if( keyVal[0] == UTL::H_MIRROR )
        complexYAGObjects.back().H_MIRROR = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::H_50U_SLIT )
        complexYAGObjects.back().H_50U_SLIT = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::H_25U_SLIT )
        complexYAGObjects.back().H_25U_SLIT = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::H_63MM_HOLE )
        complexYAGObjects.back().H_63MM_HOLE = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::H_10MM_HOLE )
        complexYAGObjects.back().H_10MM_HOLE = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::V_YAG )
        complexYAGObjects.back().V_YAG = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::V_SLIT )
        complexYAGObjects.back().V_SLIT = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::H_SLIT )
        complexYAGObjects.back().H_SLIT = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::H_OUT )
        complexYAGObjects.back().H_OUT = getPOS(keyVal[ 1 ]);

    else if( keyVal[0] == UTL::V_OUT )
        complexYAGObjects.back().V_OUT = getPOS(keyVal[ 1 ]);
}
//________________________________________________________________________________________________________________________
void screenConfigReader::addToSimpleYAGObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
       /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
        /// init structs 'correctly'
        screenStructs::SIMPLE_YAG_Object simp = screenStructs::SIMPLE_YAG_Object();
        simp.name = keyVal[ 1 ];
        simp.screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
        simp.numIlocks = numIlocks;
        simpleYAGObjects.push_back( simp );
        debugMessage("Added ", simpleYAGObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
        simpleYAGObjects.back().pvRoot = keyVal[ 1 ];

    else if ( keyVal[0] == UTL::IN )
        simpleYAGObjects.back().IN = getPOS(keyVal[ 1 ]);

    else if ( keyVal[0] == UTL::OUT )
        simpleYAGObjects.back().OUT = getPOS(keyVal[ 1 ]);
}
//_____________________________________________________________________________________________________________________________________
void screenConfigReader::addPVStruct( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
{
    // http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
    /// initstd::map< VELA_ENUM::ILOCK_NUMBER, std::string > structs 'correctly'
   if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {

        pvStruct_v.push_back( screenStructs::pvStruct() );
        pvStruct_v.back().pvSuffix = keyVal[1];

        ///SIMPLE YAGs
        if( keyVal[0] == UTL::PV_SUFFIX_ON )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::On;
        else if( keyVal[0] == UTL::PV_SUFFIX_OFF )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::Off;
        else if( keyVal[0] == UTL::PV_SUFFIX_STA )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::Sta;
        ///COMPLEX YAGs
        else if( keyVal[0] == UTL::PV_SUFFIX_H_MABS )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_MABS;
        else if( keyVal[0] == UTL::PV_SUFFIX_V_MABS )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_MABS;
        else if( keyVal[0] == UTL::PV_SUFFIX_STOP )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::STOP;
        else if( keyVal[0] == UTL::PV_SUFFIX_H_RPOS )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_RPOS;
        else if( keyVal[0] == UTL::PV_SUFFIX_V_RPOS )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_RPOS;
        else if( keyVal[0] == UTL::PV_SUFFIX_H_PROT01 )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_PROT01;
        else if( keyVal[0] == UTL::PV_SUFFIX_V_PROT01 )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_PROT01;
        else if( keyVal[0] == UTL::PV_SUFFIX_PROT03 )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::PROT03;
        else if( keyVal[0] == UTL::PV_SUFFIX_PROT05 )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::PROT05;
        else if( keyVal[0] == UTL::PV_SUFFIX_H_RPWRLOSS )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_RPWRLOSS;
        else if( keyVal[0] == UTL::PV_SUFFIX_V_RPWRLOSS )
            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_RPWRLOSS;


       debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType ) );
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________________________________________________
void screenConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if(keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//________________________________________________________________________________________________________________________
screenStructs::screenObject screenConfigReader::getScreenObjects()
{
   screenStructs::screenObject obj;

   obj.name = "VELA_SCREEN";

   ///COMPLEX YAG MAP

   for( auto && it : complexYAGObjects )
    obj.complexObjects[ it.name ] = it;

    ///add in the monitor structs, for each complex screen object

   for( auto && it : obj.complexObjects )
        for( auto it2 : complexYAGMonStructs )
            it.second.pvMonStructs[ it2.pvType ] = it2;

    ///add in the command structs, for each complex screen object

   for( auto && it : obj.complexObjects )
        for( auto it2 : complexYAGComStructs )
            it.second.pvComStructs[ it2.pvType ] = it2;

   ///SIMPLE YAG MAP

   for( auto && it : simpleYAGObjects )
    obj.simpleObjects[ it.name ] = it;

    ///add in the monitor struct, for each simple screen object

   for( auto && it : obj.simpleObjects )
        for( auto it2 : simpleYAGMonStructs )
            it.second.pvMonStructs[ it2.pvType ] = it2;

    ///add in the command struct, for each simple screen object

   for( auto && it : obj.simpleObjects )
        for( auto it2 : simpleYAGComStructs )
            it.second.pvComStructs[ it2.pvType ] = it2;



 return obj;
}



