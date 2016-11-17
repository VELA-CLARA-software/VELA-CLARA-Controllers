//djs
#include "magnetConfigReader.h"
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

magnetConfigReader::magnetConfigReader( const std::string&magConf,const std::string&NRConf,
                                        const bool startVirtualMachine,
              const bool*show_messages_ptr,const bool*show_debug_messages_ptr ):
magConf(magConf),NRConf(NRConf),
configReader( show_messages_ptr, show_debug_messages_ptr ),
usingVirtualMachine(startVirtualMachine)
{
}
//magnetConfigReader::magnetConfigReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
//: configReader( UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
//{
//}
////______________________________________________________________________________
//magnetConfigReader::magnetConfigReader( const std::string configFileLocation1,
//                                        const std::string configFileLocation2,
//                                        const std::string configFileLocation3, const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
//:  configReader(configFileLocation1, configFileLocation2, configFileLocation3, show_messages_ptr, show_debug_messages_ptr )
//{
//}
//______________________________________________________________________________
magnetConfigReader::~magnetConfigReader(){}
//______________________________________________________________________________
bool magnetConfigReader::getInjMagData( std::map< std::string, magnetStructs::magnetObject > & mapToFill )
{
    bool success = true;
    mapToFill.clear();

    size_t nCount = 0;
    size_t rCount = 0;

    for( auto && it : magObjects )
    {
        // add in magnetObj
        mapToFill[ it.name ] = it;
        // setMon and Com pvStructs
        for( auto && it2 : pvMagMonStructs )
            mapToFill[ it.name ].pvMonStructs[ it2.pvType ] = it2;

        for( auto && it2 : pvMagComStructs )
            mapToFill[ it.name ].pvComStructs[ it2.pvType ] = it2;
        // addin NR-PSU if appropraite
        if( it.magRevType == magnetStructs::MAG_REV_TYPE::NR || it.magRevType == magnetStructs::MAG_REV_TYPE::NR_GANGED )
        {
            for( auto && it2 : magPSUObjects_N)
                if( it.name == it2.parentMagnet )
                {
                    mapToFill[ it.name ].nPSU = it2;
                    for( auto && it3 : pvPSUMonStructs )
                        mapToFill[ it.name ].nPSU.pvMonStructs[ it3.pvType ] = it3;
                    for( auto && it3 : pvPSUComStructs )
                        mapToFill[ it.name ].nPSU.pvComStructs[ it3.pvType ] = it3;

                    ++nCount;
                    break;
                }
            for( auto && it2 : magPSUObjects_R)
                if( it.name == it2.parentMagnet )
                {
                    mapToFill[ it.name ].rPSU = it2;
                    for( auto && it3 : pvPSUMonStructs )
                        mapToFill[ it.name ].rPSU.pvMonStructs[ it3.pvType ] = it3;
                    for( auto && it3 : pvPSUComStructs )
                        mapToFill[ it.name ].rPSU.pvComStructs[ it3.pvType ] = it3;
                    ++rCount;
                    break;
                }
        }
    }
    // sanity checks
    if( nCount != magPSUObjects_N.size() )
    {
        message( "Error in Number of N-Type PSUs, nCount = ", nCount, " magPSUObjects_N.size() = ", magPSUObjects_N.size() );
        success = false;
    }

    if( rCount != magPSUObjects_R.size() )
    {
        message( "Error in Number of R-Type PSUs, nCount = ", nCount, " magPSUObjects_R.size() = ", magPSUObjects_R.size() );
        success = false;
    }

    if( rCount != nCount )
    {
        message( "Error ", nCount, " N-PSUs and ", rCount, " R-PSUs");
        success = false;
    }
    return success;
}
//______________________________________________________________________________
magnetStructs::degaussValues magnetConfigReader::getDeguassStruct()
{
    return degstruct;
}
//______________________________________________________________________________
bool magnetConfigReader::readConfig()
{
    bool success = true;
    /// There are 2 types of objects for an inj_mag, the magnet and (maybe) and N-R switched PSU;
    /// They are defined in seperate config files to seperate the data more clearly
    /// they still all end up in an a magObj
    //PSU
    magPSUObjects_N.clear();
    magPSUObjects_R.clear();
    pvPSUMonStructs.clear();
    pvPSUComStructs.clear();
    bool psuSuccess = readConfig(*this, NRConf, &magnetConfigReader::addToMagPSUObjectsV1, &magnetConfigReader::addToMagPSUComStructsV1,&magnetConfigReader::addToMagPSUMonStructsV1 );

    if( !psuSuccess )
        success = false;

    if( magPSUObjects_N.size() != magPSUObjects_R.size() )
    {
        debugMessage( "*** Number of N-PSU Objects does not equal number of R-PSU objects ERROR ***" );
        success = false;
    }
    else if( numObjs == magPSUObjects_N.size() && numObjs == magPSUObjects_R.size() )
        debugMessage( "*** Created ", numObjs, " NR-PSU Objects, As Expected ***", "\n" );
    else
    {
        debugMessage( "*** Created ", magPSUObjects_N.size() ," MAG PSU Objects, Expected ", numObjs,  " ERROR ***", "\n"  );
        success = false;
    }
    //Magnets
    magObjects.clear();
    pvMagMonStructs.clear();
    pvMagComStructs.clear();

    bool magSuccess = readConfig( *this, magConf, &magnetConfigReader::addToMagObjectsV1,&magnetConfigReader::addToMagComStructsV1, &magnetConfigReader::addToMagMonStructsV1 );
    if( !magSuccess )
        success = false;
    if( numObjs == magObjects.size() )
        debugMessage( "*** Created ", numObjs, " Magnet Objects, As Expected ***" );
    else
    {
        debugMessage( "*** Created ", magObjects.size() ," Magnet Objects, Expected ", numObjs,  " ERROR ***"  ); /// MAGIC_NUMBER
        success = false;
    }


///DO NOT READ DEGUASS FILE ANYMORE
   // bool degSuccess = readConfig( *this, configFile3, &magnetConfigReader::addToDegaussObj, nullptr, nullptr );
    //if( !degSuccess )
     //   success = false;

    // sanity check number of elements to degauss

//    if( degstruct.numDegaussElements == degstruct.degValues.size() )
//        debugMessage( "*** Found ", degstruct.numDegaussElements, " elements that can be degaussed as expected ***" );
//    else
//    {
//        debugMessage( "*** Found ", degstruct.degValues.size(), " degauss steps, expected ", degstruct.numDegaussElements, " ***ERROR***" );
//        success = false;
//    }

    // sanity check number of elements to degauss
//
//    size_t degStepsErrors = 0;
//    for( auto && it : degstruct.degValues )
//        if( it.second.size() != degstruct.numDegaussSteps  )
//        {
//            debugMessage("Error ", it.first, " has incorrect number of degauss steps.");
//            success = false;
//            ++degStepsErrors;
//        }
//    if( degStepsErrors == 0 )
//        debugMessage( "*** Set ", degstruct.numDegaussSteps, " degauss steps as expected ***" );


    return success;
}
//______________________________________________________________________________
//void magnetConfigReader::addToDegaussObj( const std::vector<std::string> &keyVal )
//{
//    std::string temp = keyVal[1];
//    if( keyVal[0] == UTL::BSOL_DEGAUSS_VALUES )
//        degstruct.degValues[ keyVal[0] ] = getDoubleVector( temp );
//    else if( keyVal[0] == UTL::DIP_DEGAUSS_VALUES )
//        degstruct.degValues[ keyVal[0] ] = getDoubleVector( temp );
//    else if( keyVal[0] == UTL::SOL_DEGAUSS_VALUES )
//        degstruct.degValues[ keyVal[0] ] = getDoubleVector( temp );
//    else if( keyVal[0] == UTL::QUAD_DEGAUSS_VALUES )
//        degstruct.degValues[ keyVal[0] ] = getDoubleVector( temp );
//
//    else if( keyVal[0] == UTL::COR_DEGAUSS_VALUES )
//        degstruct.degValues[ keyVal[0] ] = getDoubleVector( temp );
//
//    else if( keyVal[0] == UTL::QUAD_DEGAUSS_TOLERANCE )
//        degstruct.degTolerance[ keyVal[0] ] = getNumD( temp );
//    else if( keyVal[0] == UTL::BSOL_DEGAUSS_TOLERANCE )
//        degstruct.degTolerance[ keyVal[0] ] = getNumD( temp );
//    else if( keyVal[0] == UTL::DIP_DEGAUSS_TOLERANCE )
//        degstruct.degTolerance[ keyVal[0] ] = getNumD( temp );
//    else if( keyVal[0] == UTL::SOL_DEGAUSS_TOLERANCE )
//        degstruct.degTolerance[ keyVal[0] ] = getNumD( temp );
//
//    else if( keyVal[0] == UTL::COR_DEGAUSS_TOLERANCE )
//        degstruct.degTolerance[ keyVal[0] ] = getNumD( temp );
//
//    else if( keyVal[0] == UTL::NUM_DEGAUSS_STEPS )
//        degstruct.numDegaussSteps =getSize( temp );
//
//    else if( keyVal[0] == UTL::NUM_DEGAUSS_ELEMENTS )
//        degstruct.numDegaussElements = getSize( temp );
//
//
//    else if( keyVal[0] == UTL::MAG_SET_MAX_WAIT_TIME )
//        degstruct.maxWaitTime = getSize( temp );
//}
//______________________________________________________________________________
void magnetConfigReader::addToMagPSUMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvPSUMonStructs, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addToMagPSUComStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvPSUComStructs, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addToMagPSUObjectsV1( const std::vector<std::string> &keyVal )
{
    if( keyVal[0] == UTL::PARENT_MAGNET )
    {
        addToMagPSUObj_v( magPSUObjects_N, keyVal[ 1 ] );
        debugMessage("Added ", magPSUObjects_N.back().parentMagnet, " N PSU" );
        addToMagPSUObj_v( magPSUObjects_R, keyVal[ 1 ] );
        debugMessage("Added ", magPSUObjects_R.back().parentMagnet, " R PSU" );
    }
    else if( keyVal[0] == UTL::PV_ROOT_N )
        magPSUObjects_N.back().pvRoot = keyVal[ 1 ];
    else if( keyVal[0] == UTL::PV_ROOT_R )
        magPSUObjects_R.back().pvRoot = keyVal[ 1 ];
    else if( keyVal[0] == UTL::MAG_GANG_MEMBER )
    {
        magPSUObjects_N.back().gangMembers.push_back( keyVal[ 1 ] );
        magPSUObjects_R.back().gangMembers.push_back( keyVal[ 1 ] );
        magPSUObjects_N.back().isGanged = true ;
        magPSUObjects_R.back().isGanged = true ;
        debugMessage("Added ", magPSUObjects_R.back().parentMagnet, " NR-PSU Gang Member ", magPSUObjects_N.back().gangMembers.back() );
    }
}
//______________________________________________________________________________
void magnetConfigReader::addToMagPSUObj_v( std::vector< magnetStructs::nrPSUObject > & vec, const std::string & parentMagnet )
{
        magnetStructs::nrPSUObject psuob = magnetStructs::nrPSUObject();
        psuob.parentMagnet = parentMagnet;
        psuob.numIlocks    = (size_t)numIlocks;
        psuob.psuState     = VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR;

        vec.push_back( psuob );
}
//______________________________________________________________________________
void magnetConfigReader::addToMagMonStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvMagMonStructs, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addToMagComStructsV1( const std::vector<std::string> &keyVal )
{
    addToPVStruct( pvMagComStructs, keyVal);
}
//______________________________________________________________________________
void magnetConfigReader::addToMagObjectsV1( const std::vector<std::string> &keyVal ) // /V1 is a mechanism for having a new version of configs if needed
{
    std::string temp = keyVal[1];
    if( keyVal[0] == UTL::NAME )
    {
        magnetStructs::magnetObject mob = magnetStructs::magnetObject();
        mob.name = keyVal[ 1 ];
        mob.numIlocks = (size_t)numIlocks;
        magObjects.push_back( mob );
        debugMessage("Added ", magObjects.back().name );
    }
    else if( keyVal[0] == UTL::PV_ROOT )
    {
        if( usingVirtualMachine )
            magObjects.back().pvRoot = UTL::VM_PREFIX + temp;
        else
            magObjects.back().pvRoot = temp;
    }
    else if( keyVal[0] == UTL::PV_PSU_ROOT )
    {
        if( usingVirtualMachine )
            magObjects.back().psuRoot = UTL::VM_PREFIX + temp;
        else
            magObjects.back().psuRoot = temp;
    }
    else if( keyVal[0] == UTL::MAG_TYPE )
        addMagType( keyVal );
    else if( keyVal[0] == UTL::MAG_REV_TYPE )
        addRevType( keyVal );
    else if( keyVal[0] == UTL::RI_TOLERANCE )
    {
        std::string temp =  keyVal[ 1 ];
        magObjects.back().riTolerance = getNumD( temp );
    }
    //added by tim price (deguassing)
     else if( keyVal[0] == UTL::NUM_DEGAUSS_STEPS )
        magObjects.back().numDegaussSteps =getSize( temp );
    else if( keyVal[0] == UTL::MAG_SET_MAX_WAIT_TIME )
        magObjects.back().maxWaitTime = getSize( temp );
    else if( keyVal[0] == UTL::DEGAUSS_TOLERANCE )
        magObjects.back().degTolerance = getNumD( temp );
    else if( keyVal[0] == UTL::DEGAUSS_VALUES )
         magObjects.back().degValues = getDoubleVector( temp );
    /// BJAS requests for magnet data  16/11/16
    else if( keyVal[0] == UTL::POSITION )
    {
        std::cout << "FOUND POSITION " << temp << std::endl;
         magObjects.back().position = getNumD( temp );

    }
    else if( keyVal[0] == UTL::SLOPE )
    {
        std::cout << "FOUND SLOPE " << temp << std::endl;
         magObjects.back().slope = getNumD( temp );

    }
    else if( keyVal[0] == UTL::INTERCEPT )
    {
        std::cout << "FOUND INTERCEPT  " << temp <<  std::endl;
        magObjects.back().intercept = getNumD( temp );

    }

}
//______________________________________________________________________________
void magnetConfigReader::addMagType( const std::vector<std::string> &keyVal )
{
    if( keyVal[1] == UTL::SOL )
        magObjects.back().magType = magnetStructs::MAG_TYPE::SOL;
    else if( keyVal[1] == UTL::BSOL )
        magObjects.back().magType = magnetStructs::MAG_TYPE::BSOL;
    else if( keyVal[1] == UTL::QUAD )
        magObjects.back().magType = magnetStructs::MAG_TYPE::QUAD;
    else if( keyVal[1] == UTL::DIP )
        magObjects.back().magType = magnetStructs::MAG_TYPE::DIP;
    else if( keyVal[1] == UTL::HCOR )
        magObjects.back().magType = magnetStructs::MAG_TYPE::HCOR;
    else if( keyVal[1] == UTL::VCOR )
        magObjects.back().magType = magnetStructs::MAG_TYPE::VCOR;
}
//______________________________________________________________________________
void magnetConfigReader::addRevType( const std::vector<std::string> &keyVal )
{
    if( keyVal[1] == UTL::NR )
        magObjects.back().magRevType = magnetStructs::MAG_REV_TYPE::NR;
    else if( keyVal[1] == UTL::BIPOLAR )
        magObjects.back().magRevType = magnetStructs::MAG_REV_TYPE::BIPOLAR;
    else if( keyVal[1] == UTL::NR_GANGED )
        magObjects.back().magRevType = magnetStructs::MAG_REV_TYPE::NR_GANGED;
    else if( keyVal[1] == UTL::POS )
        magObjects.back().magRevType = magnetStructs::MAG_REV_TYPE::POS;
}
//______________________________________________________________________________
void magnetConfigReader::addToPVStruct( std::vector< magnetStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal )
{
    if( stringIsSubString( keyVal[0], "SUFFIX" ) )
    {
        pvStruct_v.push_back( magnetStructs::pvStruct() );    /// Any way to avoid the ladders?
        pvStruct_v.back().pvSuffix = keyVal[1];
        // NR-PSU PVs
        if( keyVal[0] == UTL::PV_SUFFIX_STA  )
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::Sta;

        else if( keyVal[0] == UTL::PV_SUFFIX_ON  )
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::On;

        else if( keyVal[0] == UTL::PV_SUFFIX_OFF  )
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::Off;

        else if( keyVal[0] == UTL::PV_SUFFIX_RI  )
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::RI;

        else if( keyVal[0] == UTL::PV_SUFFIX_SI  )
            pvStruct_v.back().pvType = magnetStructs::MAG_PV_TYPE::SI;

        debugMessage("Added ", pvStruct_v.back().pvSuffix, " suffix for ", ENUM_TO_STRING( pvStruct_v.back().pvType) ) ;
    }
    else
        addCOUNT_MASK_OR_CHTYPE( pvStruct_v, keyVal );
}
//______________________________________________________________________________
void magnetConfigReader::addCOUNT_MASK_OR_CHTYPE( std::vector< magnetStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  )
{
    if( keyVal[0] == UTL::PV_COUNT )
        pvStruct_v.back().COUNT = getCOUNT( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_MASK )
        pvStruct_v.back().MASK = getMASK( keyVal[ 1 ] );
    else if( keyVal[0] == UTL::PV_CHTYPE )
        pvStruct_v.back().CHTYPE = getCHTYPE( keyVal[ 1 ] );
}
//______________________________________________________________________________
bool magnetConfigReader::readConfig( magnetConfigReader & obj, const std::string & fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 )
{
    debugMessage( "\n", "**** Attempting to Read ", fn, " ****" );
    if( usingVirtualMachine )
        debugMessage( "\n", "**** Using VIRTUAL Machine  ****" );
    else
        debugMessage(  "**** Using PHYSICAL Machine  ****","\n" );

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
                        switch( configVersion ) /// Actually this switch needs to come out of here and go in the beginning, we should look for version # first!
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
        message( "!!!! Error Can't Open Mag Config File after searching for:  ", fn, " !!!!"  );
    }
    return success;
}
//______________________________________________________________________________





