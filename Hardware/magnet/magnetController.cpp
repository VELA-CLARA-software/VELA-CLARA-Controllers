//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //

#include "magnetController.h"
#include <fstream>
#include <iostream>
// stl
magnetController::magnetController(
    const bool show_messages, const bool show_debug_messages,
    const std::string & magConf,  const std::string & NRConf, const bool startVirtualMachine, const bool shouldStartEPICs
):
shouldStartEPICs(shouldStartEPICs),
controller( show_messages, show_debug_messages ),
localInterface( magConf, NRConf, startVirtualMachine, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES, shouldStartEPICs )
{
//    if( shouldStartEPICs )
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
magnetController::~magnetController(){}    //dtor
//______________________________________________________________________________
void magnetController::initialise()
{
    if( localInterface.interfaceInitReport(shouldStartEPICs) )
        message("magnetController instantiation success.");
}
//______________________________________________________________________________
bool magnetController::setSI( const std::vector< std::string > & magNames, const std::vector< double > & values)
{
    return localInterface.setSI( magNames, values );
}
//______________________________________________________________________________
bool magnetController::setSI( const std::string & magNames, const double values)
{
    return localInterface.setSI( magNames, values );
}
//______________________________________________________________________________
bool magnetController::setSI( const std::string & magName, const double value, const double tolerance, const size_t timeOUT )
{
    return localInterface.setSI( magName, value, tolerance, timeOUT );
}
//______________________________________________________________________________
std::vector< std::string >  magnetController::setSI( const std::vector< std::string > & magNames, const std::vector< double > & values, const std::vector< double > & tolerances, const size_t timeOUT )
{
    return localInterface.setSI( magNames, values, tolerances, timeOUT );
}
//______________________________________________________________________________
bool magnetController::switchONpsu ( const std::vector< std::string > & magNames)
{
    return localInterface.switchONpsu( magNames );
}
//______________________________________________________________________________
bool magnetController::switchONpsu ( const std::string & magName )
{
    return localInterface.switchONpsu( magName );
}
//______________________________________________________________________________
bool magnetController::switchOFFpsu( const std::vector< std::string > & magNames )
{
    return localInterface.switchOFFpsu( magNames );
}
//______________________________________________________________________________
bool magnetController::switchOFFpsu( const std::string & magName )
{
    return localInterface.switchOFFpsu( magName );
}
//______________________________________________________________________________
double magnetController::getSI( const std::string & magName )
{
    return localInterface.getSI( magName );
}
//______________________________________________________________________________
std::vector< double > magnetController::getSI( const std::vector< std::string > & magNames )
{
    return localInterface.getSI( magNames );
}
//______________________________________________________________________________
size_t magnetController::degauss( const std::vector< std::string > & magToDeg, bool resetToZero  )
{
    return localInterface.deGauss( magToDeg, resetToZero );
}
//______________________________________________________________________________
size_t magnetController::degauss( const std::string & magToDeg, bool resetToZero  )
{
    return localInterface.deGauss( magToDeg, resetToZero );
}
//______________________________________________________________________________
size_t magnetController::degaussAll( bool resetToZero  )
{
    return localInterface.degaussAll( resetToZero );
}
//______________________________________________________________________________
bool magnetController::isAQuad( const std::string & magName )
{
    return localInterface.isAQuad( magName );
}
//______________________________________________________________________________
bool magnetController::isABSol( const std::string & magName )
{
    return localInterface.isABSol( magName );
}
//______________________________________________________________________________
bool magnetController::isAHCor( const std::string & magName )
{
    return localInterface.isAHCor( magName );
}
//______________________________________________________________________________
bool magnetController::isAVCor( const std::string & magName )
{
    return localInterface.isAVCor( magName );
}
//______________________________________________________________________________
bool magnetController::isADip ( const std::string & magName )
{
    return localInterface.isADip( magName );
}
//______________________________________________________________________________
bool magnetController::isASol ( const std::string & magName )
{
    return localInterface.isASol( magName );
}
//______________________________________________________________________________
bool magnetController::isACor ( const std::string & magName )
{
    return localInterface.isACor( magName );
}
//______________________________________________________________________________
bool magnetController::isNR( const std::string & magName )
{
    return localInterface.isNR( magName );
}
//______________________________________________________________________________
bool magnetController::isBipolar( const std::string & magName )
{
    return localInterface.isBipolar( magName );
}
//______________________________________________________________________________
bool magnetController::isNRGanged( const std::string & magName )
{
    return localInterface.isNRGanged( magName );
}
//______________________________________________________________________________
bool magnetController::isNRorNRGanged( const std::string & magName )
{
    return localInterface.isNRorNRGanged( magName );
}
//______________________________________________________________________________
bool magnetController::isON_psuN( const std::string & magName )
{
    return localInterface.isON_psuN( magName );
}
//______________________________________________________________________________
bool magnetController::isON_psuR( const std::string & magName )
{
    return localInterface.isON_psuR( magName );
}
//______________________________________________________________________________
bool magnetController::isON ( const std::string & magName )
{
    return localInterface.isON( magName );
}
//______________________________________________________________________________
bool magnetController::isOFF_psuN( const std::string & magName )
{
    return localInterface.isOFF_psuN( magName );
}
//______________________________________________________________________________
bool magnetController::isOFF_psuR( const std::string & magName )
{
    return localInterface.isOFF_psuR( magName );
}
//______________________________________________________________________________
bool magnetController::isOFF ( const std::string & magName )
{
    return localInterface.isOFF( magName );
}
//______________________________________________________________________________
bool magnetController::isDegaussing( const std::string & magName )
{
    return localInterface.isDegaussing( magName );
}
//______________________________________________________________________________
bool magnetController::isNotDegaussing( const std::string & magName )
{
    return localInterface.isNotDegaussing( magName );
}
//______________________________________________________________________________
void magnetController::setRITolerance( const std::string & magName, const double val)
{
    return localInterface.setRITolerance( magName, val );
}
//______________________________________________________________________________
void magnetController::showMagRevType()
{
    localInterface.showMagRevType();
}
//______________________________________________________________________________
double magnetController::getRI( const std::string & magName )
{
    return localInterface.getRI( magName );
}
//______________________________________________________________________________
std::vector< double > magnetController::getRI( const std::vector< std::string > & magNames )
{
    return localInterface.getRI( magNames );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetController::getCurrentMagnetState( const std::vector< std::string > & s )
{
    return localInterface.getCurrentMagnetState( s );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetController::getCurrentMagnetState()
{
    return localInterface.getCurrentMagnetState( );
}
//______________________________________________________________________________
void magnetController::applyMagnetStateStruct( const magnetStructs::magnetStateStruct & ms  )
{
    return localInterface.applyMagnetStateStruct( ms );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetController::getDBURT( const std::string & fileName )
{
    return localInterface.getDBURT( fileName );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetController::getDBURTCorOnly( const std::string & fileName )
{
    return localInterface.getDBURTCorOnly( fileName );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetController::getDBURTQuadOnly( const std::string & fileName )
{
    return localInterface.getDBURTQuadOnly( fileName );
}
//______________________________________________________________________________
void magnetController::applyDBURT( const std::string & fileName )
{
    return localInterface.applyDBURT( fileName );
}
//______________________________________________________________________________
void magnetController::applyDBURTCorOnly( const std::string & fileName )
{
    return localInterface.applyDBURTCorOnly( fileName );
}
//______________________________________________________________________________
void magnetController::applyDBURTQuadOnly( const std::string & fileName )
{
    return localInterface.applyDBURTQuadOnly( fileName );
}
//______________________________________________________________________________
bool magnetController::writeDBURT( const magnetStructs::magnetStateStruct & ms, const std::string &fileName, const std::string &comments)
{
    return localInterface.writeDBURT( ms, fileName, comments );
}
//______________________________________________________________________________
bool magnetController::writeDBURT( const std::string & fileName, const std::string & comments )
{
    return localInterface.writeDBURT( fileName, comments );
}
//______________________________________________________________________________
const magnetStructs::magnetObject& magnetController::getMagObjConstRef( const std::string & magName  )
{
    return localInterface.getMagObjConstRef( magName );
}
//______________________________________________________________________________
const magnetStructs::magnetObject* magnetController::getMagObjConstPtr( const std::string & magName  )
{
    return localInterface.getMagObjConstPtr( magName );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  magnetController::getILockStates( const std::string & name )
{
    return localInterface.getILockStates( name );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > magnetController::getILockStatesStr( const std::string & name )
{
    return localInterface.getILockStatesStr( name );
}
//______________________________________________________________________________
double magnetController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void   magnetController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
 std::vector< std::string > magnetController::getMagnetNames()
{
    return localInterface.getMagnetNames( );
}
//______________________________________________________________________________
std::vector< std::string > magnetController::getQuadNames()
{
    return localInterface.getQuadNames( );
}
//______________________________________________________________________________
std::vector< std::string > magnetController::getHCorNames()
{
    return localInterface.getHCorNames( );
}
//______________________________________________________________________________
std::vector< std::string > magnetController::getVCorNames()
{
    return localInterface.getVCorNames( );
}
//______________________________________________________________________________
std::vector< std::string > magnetController::getDipNames()
{
    return localInterface.getDipNames( );
}
//______________________________________________________________________________
std::vector< std::string > magnetController::getSolNames()
{
    return localInterface.getSolNames( );
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________


magnetStructs::MAG_REV_TYPE magnetController::getMagRevType( const std::string & magName )
{
    return localInterface.getMagRevType( magName );
}
//______________________________________________________________________________
std::vector<  magnetStructs::MAG_REV_TYPE >  magnetController::getMagRevType( const std::vector< std::string > & magNames )
{
    return localInterface.getMagRevType( magNames );
}
//______________________________________________________________________________
magnetStructs::MAG_TYPE magnetController::getMagType( const std::string & magName )
{
    return localInterface.getMagType( magName );
}
//______________________________________________________________________________
std::vector<  magnetStructs::MAG_TYPE >  magnetController::getMagType( const std::vector< std::string > & magNames )
{
    return localInterface.getMagType( magNames );
}
//______________________________________________________________________________
VELA_ENUM::MAG_PSU_STATE magnetController::getMagPSUState( const std::string & magName )
{
    return localInterface.getMagPSUState( magName );
}
//______________________________________________________________________________
std::vector<  VELA_ENUM::MAG_PSU_STATE > magnetController::getMagPSUState( const std::vector< std::string > & magNames )
{
    return localInterface.getMagPSUState( magNames );
}
//______________________________________________________________________________
double magnetController::getPosition( const std::string & magName )
{
    return localInterface.getPosition( magName );
}
//______________________________________________________________________________
std::vector< double > magnetController::getPosition( const std::vector< std::string > & magNames )
{
    return localInterface.getPosition( magNames );
}
//______________________________________________________________________________
double magnetController::getSlope( const std::string & magName )
{
    return localInterface.getSlope( magName );
}
//______________________________________________________________________________
std::vector< double > magnetController::getSlope( const std::vector< std::string > & magNames )
{
    return localInterface.getSlope( magNames );
}
//______________________________________________________________________________
double magnetController::getIntercept( const std::string & magName )
{
    return localInterface.getIntercept( magName );
}
//______________________________________________________________________________
std::vector< double > magnetController::getIntercept( const std::vector< std::string > & magNames )
{
    return localInterface.getIntercept( magNames );
}
//______________________________________________________________________________
      ///
std::vector< double > magnetController::getDegValues( const std::string & magName )
{
    return localInterface.getDegValues( magName );
}
//______________________________________________________________________________
std::vector< std::vector< double > > magnetController::getDegValues( const std::vector< std::string >  & magNames )
{
    return localInterface.getDegValues( magNames );
}
//______________________________________________________________________________
#ifdef BUILD_DLL
//______________________________________________________________________________
boost::python::dict magnetController::getILockStatesStr_Py( std::string magName )
{
    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
}
//______________________________________________________________________________
boost::python::dict magnetController::getILockStates_Py( std::string magName )
{
    return enumMapToPythonDict( getILockStates( magName ) );
}
//______________________________________________________________________________
boost::python::dict magnetController::getMagPSUStateDefinition()
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
#endif // BUILD_DLL


