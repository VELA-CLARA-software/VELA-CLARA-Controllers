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

#include "llrfController.h"
#include <fstream>
#include <iostream>
// stl
llrfController::llrfController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & laserConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(laserConf,startVirtualMachine,&SHOW_MESSAGES,&SHOW_DEBUG_MESSAGES,shouldStartEPICs),
shouldStartEPICs(shouldStartEPICs)
{
//    if( shouldStartEPICs )
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
llrfController::~llrfController(){}    //dtor
//______________________________________________________________________________
void llrfController::initialise()
{
    if( localInterface.interfaceInitReport(shouldStartEPICs) )
        message("llrfController instantiation success.");
}
//______________________________________________________________________________
#ifdef BUILD_DLL
//______________________________________________________________________________
//____________________________________________________________________________________________
double llrfController::get_CA_PEND_IO_TIMEOUT()
{
  return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//_____________________________________________________________________________________________
void llrfController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > llrfController::getILockStates( const std::string & name )
{
    return localInterface.getILockStates( name );
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > llrfController::getILockStatesStr( const std::string & name )
{
    return localInterface.getILockStatesStr( name );
}
//______________________________________________________________________________
//boost::python::dict llrfController::getILockStatesStr_Py( std::string magName )
//{
//    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict llrfController::getILockStates_Py( std::string magName )
//{
//    return enumMapToPythonDict( getILockStates( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict llrfController::getMagPSUStateDefinition()
//{
//    std::map< VELA_ENUM::MAG_PSU_STATE,  std::string  > m;
//
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ] = ENUM_TO_STRING( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ); // ENUM_TO_STRING MACRO in velaStructs.h
//
//    return enumStringMapToPythonDict( m );
//}
//______________________________________________________________________________
#endif // BUILD_DLL


