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
                                        const std::string & configFileLocation2,
                                        const bool startVirtualMachine,
                                        const bool* show_messages_ptr,
                                        const bool* show_debug_messages_ptr ):
configReader( configFileLocation1, configFileLocation2, show_messages_ptr, show_debug_messages_ptr ),
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
    pvScrComStructs_VELA_HV_MOVER.clear();
    pvScrComStructs_VELA_PNEUMATIC.clear();
    pvScrMonStructs_VELA_HV_MOVER.clear();
    pvScrMonStructs_VELA_PNEUMATIC.clear();
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
//        pvStruct_v.push_back( screenStructs::pvStruct() );    /// Any way to avoid the ladders?
//        pvStruct_v.back().pvSuffix = keyVal[1];
        if( keyVal[0] == UTL::PV_SUFFIX_STA  )
            addToPVStruct(pvScrMonStructs_VELA_PNEUMATIC, screenStructs::SCREEN_PV_TYPE::Sta,
                screenStructs::SCREEN_TYPE::VELA_PNEUMATIC, keyVal[1] );
        else if( keyVal[0] == UTL::PV_SUFFIX_ON  )
            addToPVStruct(pvScrMonStructs_VELA_PNEUMATIC, screenStructs::SCREEN_PV_TYPE::On,
                          screenStructs::SCREEN_TYPE::VELA_PNEUMATIC, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_OFF )
            addToPVStruct(pvScrMonStructs_VELA_PNEUMATIC, screenStructs::SCREEN_PV_TYPE::Off,
                          screenStructs::SCREEN_TYPE::VELA_PNEUMATIC, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_H_PROT01 )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::H_PROT01,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_V_PROT01 )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::V_PROT01,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_H_RPWRLOSS )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::H_RPWRLOSS,
              screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_V_RPWRLOSS )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::V_RPWRLOSS,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_V_RPOS )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::V_RPOS,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_H_RPOS )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::H_RPOS,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_PROT05 )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::PROT05,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1]);

        else if( keyVal[0] == UTL::PV_SUFFIX_PROT03 )
            addToPVStruct(pvScrMonStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::PROT03,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_H_MABS )
            addToPVStruct(pvScrComStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::H_MABS,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_V_MABS )
            addToPVStruct(pvScrComStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::V_MABS,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );

        else if( keyVal[0] == UTL::PV_SUFFIX_STOP )
            addToPVStruct(pvScrComStructs_VELA_HV_MOVER, screenStructs::SCREEN_PV_TYPE::STOP,
                          screenStructs::SCREEN_TYPE::VELA_HV_MOVER, keyVal[1] );
    }
    else // must be a mask, chtype config entry, if the config file follows the rules
    {
        addCOUNT_MASK_OR_CHTYPE( keyVal );
    }

}
//______________________________________________________________________________
void screenConfigReader::addToPVStruct(std::vector< screenStructs::pvStruct > & pvs,
                                       const screenStructs::SCREEN_PV_TYPE pvtype,
                                       screenStructs::SCREEN_TYPE scrType, const std::string& pvSuffix)
{
    pvs.push_back( screenStructs::pvStruct() );
    pvs.back().pvType      = pvtype;
    pvs.back().pvSuffix    = pvSuffix;
    // we know the PV_CHTYPE, PV_MASK, etc must come after the suffix,
    // so store a ref to which vector to update with that info. (this does make sense)
    lastPVStruct = &pvs;
    debugMessage("Added ", pvs.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvs.back().pvType) );
}
//______________________________________________________________________________
void screenConfigReader::addCOUNT_MASK_OR_CHTYPE(  const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        (*lastPVStruct).back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        (*lastPVStruct).back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        (*lastPVStruct).back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}

//______________________________________________________________________________
//void screenConfigReader::setLasttoVELA_HV_MOVER()
//{
//    lastPVStructScreenType = screenStructs::SCREEN_TYPE::VELA_HV_MOVER;
//}
////______________________________________________________________________________
//void screenConfigReader::setLasttoVELA_PNEUMATIC()
//{
//    lastPVStructScreenType = screenStructs::SCREEN_TYPE::VELA_PNEUMATIC;
//}
//______________________________________________________________________________
//void screenConfigReader::addToScComStructsV1( const std::vector<std::string> &keyVal )
//{
//    PV_SUFFIX_H_MABS
//    PV_SUFFIX_V_MABS
//    PV_SUFFIX_H_RPOS
//    PV_SUFFIX_V_RPOS
//    PV_SUFFIX_STOP
//
//        else if( keyVal[0] == UTL::PV_SUFFIX_ON  )
//        {
//            pvScrComStructs_VELA_PNEUMATIC.push_back( screenStructs::pvStruct() );
//            pvScrComStructs_VELA_PNEUMATIC.back().pvType = screenStructs::SCREEN_PV_TYPE::On;
//            setLasttoVELA_PNEUMATIC();
//        }
//        else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
//        {
//            pvScrComStructs_VELA_PNEUMATIC.push_back( screenStructs::pvStruct() );
//            pvScrComStructs_VELA_PNEUMATIC.back().pvType = screenStructs::SCREEN_PV_TYPE::Off;
//            setLasttoVELA_PNEUMATIC();
//        }
//        else if( keyVal[0] == UTL::PV_SUFFIX_ON  )
//        {
//            pvScrComStructs_VELA_PNEUMATIC.push_back( screenStructs::pvStruct() );
//            pvScrComStructs_VELA_PNEUMATIC.back().pvType = screenStructs::SCREEN_PV_TYPE::On;
//            setLasttoVELA_PNEUMATIC();
//        }
//        else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
//        {
//            pvScrComStructs_VELA_PNEUMATIC.push_back( screenStructs::pvStruct() );
//            pvScrComStructs_VELA_PNEUMATIC.back().pvType = screenStructs::SCREEN_PV_TYPE::Off;
//            setLasttoVELA_PNEUMATIC();
//        }
//
//
//
//    addToPVStruct( pvScrComStructs, keyVal);
//}
//______________________________________________________________________________
void screenConfigReader::addToScrObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::NAME )
    {
        screenStructs::screenObjectDEV scr = screenStructs::screenObjectDEV();
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
        scrObjects.back().screenType = getScreenType( keyVal[ 1 ] );
    // the mover knows all the positions of screens, slits etc, plus current position
    else if( keyVal[0] == UTL::H_MIRROR_POS )
        scrObjects.back().mover.H_MIRROR_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_MIRROR_POS )
        scrObjects.back().mover.H_MIRROR_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_50U_SLIT_POS )
        scrObjects.back().mover.H_50U_SLIT_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_25U_SLIT_POS )
        scrObjects.back().mover.H_25U_SLIT_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_63MM_HOLE_POS )
        scrObjects.back().mover.H_63MM_HOLE_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_10MM_HOLE_POS )
        scrObjects.back().mover.H_10MM_HOLE_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::V_YAG_POS )
        scrObjects.back().mover.V_YAG_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::V_SLIT_POS )
        scrObjects.back().mover.V_SLIT_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_SLIT_POS )
        scrObjects.back().mover.H_SLIT_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::H_OUT_POS )
        scrObjects.back().mover.H_OUT_POS = getNumD(keyVal[ 1 ]);
    else if( keyVal[0] == UTL::V_OUT_POS )
        scrObjects.back().mover.V_OUT_POS = getNumD(keyVal[ 1 ]);
}
//______________________________________________________________________________________________________
screenStructs::SCREEN_TYPE screenConfigReader::getScreenType( const std::string & val )
{
    screenStructs::SCREEN_TYPE ret = screenStructs::SCREEN_TYPE::UNKNOWN_SCREEN_TYPE;
    if( val == UTL::VELA_HV_MOVER_SCREEN )
        ret = screenStructs::SCREEN_TYPE::VELA_HV_MOVER;
    else if( val == UTL::VELA_PNEUMATIC_SCREEN )
        ret = screenStructs::SCREEN_TYPE::VELA_PNEUMATIC;
    return ret;
}
//______________________________________________________________________________________________________
//void screenConfigReader::addToPVStruct( std::vector<screenStructs::pvStruct> & pvStruct_v, const std::vector<std::string> &keyVal )
//{
//
//    //if a command is used on more than one type of scren it will probably have to get a double entry (... ?)
//
//    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
//    {
//        pvStruct_v.push_back( screenStructs::pvStruct() );    /// Any way to avoid the ladders?
//        pvStruct_v.back().pvSuffix = keyVal[1];
//
//        if( keyVal[0] == UTL::PV_SUFFIX_STA  )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::Sta;
//
//        else if( keyVal[0] == UTL::PV_SUFFIX_ON  )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::On;
//
//        else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::Off;
//
//        else if( keyVal[0] == UTL::PV_SUFFIX_RI  )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::H_MABS;
//
//        else if( keyVal[0] == UTL::PV_SUFFIX_SI  )
//            pvStruct_v.back().pvType = screenStructs::SCREEN_PV_TYPE::V_MABS;
//
//        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
//    }
//    else
//        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
//}
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
                                        CALL_MEMBER_FN(obj, f2)( keyVal );


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
bool screenConfigReader::getScreenObjects( std::map< std::string, screenStructs::screenObjectDEV > & mapToFill )
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
                for( auto && it2 : pvScrMonStructs_VELA_PNEUMATIC )
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                for( auto && it2 : pvScrComStructs_VELA_PNEUMATIC)
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                break;
            case screenStructs::SCREEN_TYPE::VELA_HV_MOVER:
                message( it.name, " is a VELA_HV_MOVER screen");
                for( auto && it2 : pvScrMonStructs_VELA_HV_MOVER )
                    mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;
                for( auto && it2 : pvScrComStructs_VELA_HV_MOVER )
                    mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
                break;
        }
        nCount+=1;
    }
    if ( nCount == scrObjects.size() )
        success = true;
    return success;
}










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




