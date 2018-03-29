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

#include "virtualCathodeController.h"
#include <fstream>
#include <iostream>
// stl
virtualCathodeController::virtualCathodeController(
    bool* show_messages,
    bool* show_debug_messages,
    const std::string & pilaserConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(pilaserConf,startVirtualMachine,SHOW_MESSAGES,SHOW_DEBUG_MESSAGES,shouldStartEPICs)
//shouldStartEPICs(shouldStartEPICs)
{
//    if( shouldStartEPICs )
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
virtualCathodeController::~virtualCathodeController(){}    //dtor
//______________________________________________________________________________
void virtualCathodeController::initialise()
{
//    if( localInterface.interfaceInitReport(shouldStartEPICs))
//        message("virtualCathodeController instantiation success.");
}
//______________________________________________________________________________
//#ifdef BUILD_DLL
////____________________________________________________________________________________________
//double virtualCathodeController::getHpos()
//{
//    return localInterface.getHpos();
//}
////____________________________________________________________________________________________
//double virtualCathodeController::getVpos()
//{
//    return localInterface.getVpos();
//}
////____________________________________________________________________________________________
//double virtualCathodeController::getIntensity()
//{
//    return localInterface.getIntensity();
//}
////____________________________________________________________________________________________
//bool virtualCathodeController::setHpos(double value)
//{
//    return localInterface.setHpos( value );
//}
////____________________________________________________________________________________________
//bool virtualCathodeController::setHpos(int value)
//{
//    return localInterface.setHpos( value );
//}
////____________________________________________________________________________________________
//bool virtualCathodeController::setVpos(double value)
//{
//    return localInterface.setVpos( value );
//}
////____________________________________________________________________________________________
//bool virtualCathodeController::setVpos(int value)
//{
//    return localInterface.setVpos( value );
//}
////____________________________________________________________________________________________
//bool virtualCathodeController::setIntensity(double value)
//{
//    return localInterface.setIntensity( value );
//}
////____________________________________________________________________________________________
//bool virtualCathodeController::setIntensity(int value)
//{
//    return localInterface.setIntensity( value );
//}
////____________________________________________________________________________________________
//const pilaserStructs::pilaserObject &virtualCathodeController::getPILObjConstRef()
//{
//    return localInterface.getPILObjConstRef();
//}
////____________________________________________________________________________________________
double virtualCathodeController::get_CA_PEND_IO_TIMEOUT()
{
  //return localInterface.get_CA_PEND_IO_TIMEOUT();
  return 1.0;
}
//_____________________________________________________________________________________________
void virtualCathodeController::set_CA_PEND_IO_TIMEOUT(double val)
{
    //localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________________________
//std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > virtualCathodeController::getILockStates(const std::string& name)
//{
//    return localInterface.getILockStates(name);
//}
////______________________________________________________________________________________________
//std::map<HWC_ENUM::ILOCK_NUMBER,std::string> virtualCathodeController::getILockStatesStr(const std::string& name)
//{
//    return localInterface.getILockStatesStr(name);
//}
//______________________________________________________________________________
//boost::python::dict virtualCathodeController::getILockStatesStr_Py( std::string magName )
//{
//    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict virtualCathodeController::getILockStates_Py( std::string magName )
//{
//    return enumMapToPythonDict( getILockStates( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict virtualCathodeController::getMagPSUStateDefinition()
//{
//    std::map< HWC_ENUM::MAG_PSU_STATE,  std::string  > m;
//
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ); // ENUM_TO_STRING MACRO in velaStructs.h
//
//    return enumStringMapToPythonDict( m );
//}
//______________________________________________________________________________
//#endif // BUILD_DLL


