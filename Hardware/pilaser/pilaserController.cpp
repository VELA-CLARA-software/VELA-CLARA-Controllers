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

#include "pilaserController.h"
#include <fstream>
#include <iostream>
// stl
pilaserController::pilaserController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & pilaserConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(pilaserConf,startVirtualMachine,&SHOW_MESSAGES,&SHOW_DEBUG_MESSAGES,shouldStartEPICs),
shouldStartEPICs(shouldStartEPICs)
{
//    if( shouldStartEPICs )
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
pilaserController::~pilaserController(){}    //dtor
//______________________________________________________________________________
void pilaserController::initialise()
{
    if( localInterface.interfaceInitReport(shouldStartEPICs) )
        message("pilaserController instantiation success.");
}
//______________________________________________________________________________
#ifdef BUILD_DLL
//____________________________________________________________________________________________
double pilaserController::getHpos()
{
    return localInterface.getHpos();
}
//____________________________________________________________________________________________
double pilaserController::getVpos()
{
    return localInterface.getVpos();
}
//____________________________________________________________________________________________
double pilaserController::getIntensity()
{
    return localInterface.getIntensity();
}
//____________________________________________________________________________________________
bool pilaserController::setHpos(double value)
{
    return localInterface.setHpos( value );
}
//____________________________________________________________________________________________
bool pilaserController::setHpos(int value)
{
    return localInterface.setHpos( value );
}
//____________________________________________________________________________________________
bool pilaserController::setVpos(double value)
{
    return localInterface.setVpos( value );
}
//____________________________________________________________________________________________
bool pilaserController::setVpos(int value)
{
    return localInterface.setVpos( value );
}
//____________________________________________________________________________________________
bool pilaserController::setIntensity(double value)
{
    return localInterface.setIntensity( value );
}
//____________________________________________________________________________________________
bool pilaserController::setIntensity(int value)
{
    return localInterface.setIntensity( value );
}
//____________________________________________________________________________________________
const pilaserStructs::pilaserObject &pilaserController::getPILObjConstRef()
{
    return localInterface.getPILObjConstRef();
}
//____________________________________________________________________________________________
double pilaserController::get_CA_PEND_IO_TIMEOUT()
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void pilaserController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > pilaserController::getILockStates(const std::string& name)
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________________________
std::map<VELA_ENUM::ILOCK_NUMBER,std::string> pilaserController::getILockStatesStr(const std::string& name)
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________
//boost::python::dict pilaserController::getILockStatesStr_Py( std::string magName )
//{
//    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict pilaserController::getILockStates_Py( std::string magName )
//{
//    return enumMapToPythonDict( getILockStates( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict pilaserController::getMagPSUStateDefinition()
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


