
#include "velaINJMagnetController.h"
#include <fstream>
#include <iostream>

// stl
velaINJMagnetController::velaINJMagnetController( const std::string configFileLocation1,
                                    const std::string configFileLocation2,
                                    const std::string configFileLocation3, const bool show_messages, const bool show_debug_messages )
: magnetController( configFileLocation1, configFileLocation2, configFileLocation3, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaINJMagnetController::velaINJMagnetController( const  bool show_messages, const bool show_debug_messages , const std::string Direc )
: magnetController( Direc+"\\Config\\velaINJMagnetsNRpsu.config",
                    Direc+"\\Config\\velaINJMagnets.config",
                    Direc+"\\Config\\velaINJMagnetsDegauss.config",
                    show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaINJMagnetController::~velaINJMagnetController(){}    //dtor
//______________________________________________________________________________

#ifdef BUILD_DLL
boost::python::dict velaINJMagnetController::getMagPSUStateDefinition()
{
    std::map< VELA_ENUM::MAG_PSU_STATE,  std::string  > m;

    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ); // ENUM_TO_STRING MACRO in velaStructs.h
    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON ); // ENUM_TO_STRING MACRO in velaStructs.h
    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ); // ENUM_TO_STRING MACRO in velaStructs.h
    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ); // ENUM_TO_STRING MACRO in velaStructs.h
    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ); // ENUM_TO_STRING MACRO in velaStructs.h

    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict velaINJMagnetController::getILockStatesStr_Py( std::string magName )
{
    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
}
//______________________________________________________________________________
boost::python::dict velaINJMagnetController::getILockStates_Py( std::string magName )
{
    return enumMapToPythonDict( getILockStates( magName ) );
}

//______________________________________________________________________________
bool velaINJMagnetController::isAQuad_Py( const std::string magName )
{
    return isAQuad( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isABSol_Py( const std::string magName )
{
    return isABSol( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isAHCor_Py( const std::string magName )
{
    return isAHCor( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isAVCor_Py( const std::string magName )
{
    return isAVCor( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isADip_Py ( const std::string magName )
{
    return isADip( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isASol_Py ( const std::string magName )
{
    return isASol( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isACor_Py ( const std::string magName )
{
    return isACor( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isNR_Py( const std::string  magName )
{
    return isNR( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isBipolar_Py( const std::string  magName )
{
    return isBipolar( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isNRGanged_Py( const std::string  magName )
{
    return isNRGanged( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isNRorNRGanged_Py( const std::string  magName )
{
    return isNRorNRGanged( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isON_psuN_Py( const std::string  magName )
{
    return isON_psuN( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isON_psuR_Py( const std::string  magName )
{
    return isON_psuR( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isON_Py ( const std::string  magName )
{
    return isON( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isOFF_psuN_Py( const std::string  magName )
{
    return isOFF_psuN( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isOFF_psuR_Py( const std::string  magName )
{
    return isOFF_psuR( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isOFF_Py ( const std::string  magName )
{
    return isOFF( magName );
}
bool velaINJMagnetController::isDegaussing_Py( const std::string magName )
{
    return isDegaussing( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::isNotDegaussing_Py( const std::string magName )
{
    return isNotDegaussing( magName );
}
//______________________________________________________________________________
void velaINJMagnetController::setRITolerance_Py( const std::string magName, const double val)
{
    return setRITolerance( magName, val );
}
//______________________________________________________________________________
double velaINJMagnetController::getSI_Py1( const std::string magName )
{
    return getSI( magName );
}
//______________________________________________________________________________
double velaINJMagnetController::getRI_Py1( const std::string magName )
{
    return getRI( magName );
}
//______________________________________________________________________________
std::vector< double > velaINJMagnetController::getSI_Py2( const std::vector< std::string > magNames )
{
    return getSI( magNames );
}
//______________________________________________________________________________
std::vector< double > velaINJMagnetController::getRI_Py2( const std::vector< std::string > magNames )
{
    return getRI( magNames );
}
//______________________________________________________________________________
bool velaINJMagnetController::setSI_Py1( const std::string magName, const double value)
{
    return setSI( magName, value );
}
//______________________________________________________________________________
bool velaINJMagnetController::setSI_Py2( const  std::vector< std::string > magNames, const std::vector< double > values)
{
    return setSI( magNames, values );
}
//______________________________________________________________________________
std::vector< std::string >  velaINJMagnetController::setSI_Py3( const std::vector< std::string > magNames, const std::vector< double > values, const std::vector< double > tolerances, const size_t timeOUT )
{
    return setSI( magNames, values, tolerances, timeOUT );
}
//______________________________________________________________________________
bool velaINJMagnetController::setSI_Py4( const std::string magName, const double value, const double tolerance, const size_t timeOUT )
{
    return setSI( magName, value, tolerance, timeOUT );
}
//______________________________________________________________________________
bool velaINJMagnetController::switchONpsu_Py1( const std::string magName  )
{
    return switchONpsu( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::switchOFFpsu_Py1( const std::string magName  )
{
    return switchOFFpsu( magName );
}
//______________________________________________________________________________
bool velaINJMagnetController::switchONpsu_Py2( const std::vector< std::string > magNames )
{
    return switchONpsu( magNames );
}
//______________________________________________________________________________
bool velaINJMagnetController::switchOFFpsu_Py2( const std::vector< std::string > magNames )
{
    return switchOFFpsu( magNames );
}
//______________________________________________________________________________
size_t velaINJMagnetController::degauss_Py1( const std::string mag )
{
    //ScopedGILRelease scoped;
    return degauss( mag );
}
//______________________________________________________________________________
size_t velaINJMagnetController::degauss_Py2( const std::vector< std::string > mag )
{
    //ScopedGILRelease scoped;
    return degauss( mag );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct velaINJMagnetController::getDBURT_Py( const std::string fileName )
{
     return getDBURT( fileName );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct velaINJMagnetController::getDBURTCorOnly_Py( const std::string fileName )
{
     return getDBURTCorOnly( fileName );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct velaINJMagnetController::getDBURTQuadOnly_Py( const std::string fileName )
{
     return getDBURTQuadOnly( fileName );
}
//______________________________________________________________________________
void velaINJMagnetController::applyDBURT_Py( const std::string fileName )
{
     return applyDBURT( fileName );
}
//______________________________________________________________________________
void velaINJMagnetController::applyDBURTCorOnly_Py( const std::string fileName )
{
     return applyDBURTCorOnly( fileName );
}
//______________________________________________________________________________
void velaINJMagnetController::applyDBURTQuadOnly_Py( const std::string fileName )
{
     return applyDBURTQuadOnly( fileName );
}
//______________________________________________________________________________
bool velaINJMagnetController::writeDBURT_Py( const magnetStructs::magnetStateStruct ms, const std::string fileName, const std::string comments )
{
     return writeDBURT( ms, fileName, comments );
}
//______________________________________________________________________________
bool velaINJMagnetController::writeDBURT_Py( const std::string fileName, const std::string comments )
{
     return writeDBURT( fileName, comments );
}

#endif //BUILD_DLL
