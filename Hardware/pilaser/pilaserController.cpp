/*
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
//
//  Author:      DJS
//  Last edit:   29-03-2018
//  FileName:    pilaserController.cpp
//  Description:
//
//
//*/

#include "pilaserController.h"
#include <fstream>
#include <iostream>
// stl
pilaserController::pilaserController(
    bool* show_messages,
    bool* show_debug_messages,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const std::string& name,
    const std::string& pilaserConf,
    const std::string& vcAnalysisConf,
    const std::string& piLaserMirrorConf,
    const std::string& piLaserShutterConf
    ):
controller(show_messages,show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::PI_LASER),
localMirrorName("LASER_MIRROR_CONTROLLER"),
localShutterName("LASER_SHUTTER_CONTROLLER"),
localVirtualCathodeName("VIRTUAL_CATHODE_ANALYSIS_CONTROLLER"),
localMirror(show_messages,
                        show_debug_messages,
                        startVirtualMachine,
                        shouldStartEPICs,
                        piLaserMirrorConf,
                        localMirrorName),
localShutter(show_messages,       show_debug_messages,
                  startVirtualMachine, shouldStartEPICs,
                  piLaserShutterConf,  localShutterName),
//localVirtualCathode(show_messages,show_debug_messages,
//                        startVirtualMachine,shouldStartEPICs,vcAnalysisConf,localVirtualCathodeName),
localInterface(show_messages,show_debug_messages,
                 startVirtualMachine,shouldStartEPICs,pilaserConf),
name(name)
{
//    initialise();
}
//______________________________________________________________________________
pilaserController::~pilaserController(){}    //dtor
//______________________________________________________________________________
//void pilaserController::initialise()
//{
//    if( localInterface.interfaceInitReport(shouldStartEPICs) )
//        message("pilaserController instantiation success.");
//}
////______________________________________________________________________________
//#ifdef BUILD_DLL
////____________________________________________________________________________________________
//double pilaserController::getHpos()
//{
//    return localInterface.getHpos();
//}
////____________________________________________________________________________________________
//double pilaserController::getVpos()
//{
//    return localInterface.getVpos();
//}
////____________________________________________________________________________________________
//double pilaserController::getIntensity()
//{
//    return localInterface.getIntensity();
//}
////____________________________________________________________________________________________
//bool pilaserController::setHpos(double value)
//{
//    return localInterface.setHpos( value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setHpos(int value)
//{
//    return localInterface.setHpos( value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setVpos(double value)
//{
//    return localInterface.setVpos( value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setVpos(int value)
//{
//    return localInterface.setVpos( value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setIntensity(double value)
//{
//    return localInterface.setIntensity( value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setIntensity(int value)
//{
//    return localInterface.setIntensity( value );
//}
////____________________________________________________________________________________________
//const pilaserStructs::pilaserObject &pilaserController::getPILObjConstRef()
//{
//    return localInterface.getPILObjConstRef();
//}
////____________________________________________________________________________________________
double pilaserController::get_CA_PEND_IO_TIMEOUT()const
{
    //return localInterface.get_CA_PEND_IO_TIMEOUT();
    return 1.0;
}
//_____________________________________________________________________________________________
void pilaserController::set_CA_PEND_IO_TIMEOUT(double val)
{
    //localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
////______________________________________________________________________________________________
//std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > pilaserController::getILockStates(const std::string& name)
//{
//    return localInterface.getILockStates(name);
//}
////______________________________________________________________________________________________
//std::map<HWC_ENUM::ILOCK_NUMBER,std::string> pilaserController::getILockStatesStr(const std::string& name)
//{
//    return localInterface.getILockStatesStr(name);
//}
////______________________________________________________________________________
////boost::python::dict pilaserController::getILockStatesStr_Py( std::string magName )
////{
////    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
////}
//////______________________________________________________________________________
////boost::python::dict pilaserController::getILockStates_Py( std::string magName )
////{
////    return enumMapToPythonDict( getILockStates( magName ) );
////}
//////______________________________________________________________________________
////boost::python::dict pilaserController::getMagPSUStateDefinition()
////{
////    std::map< HWC_ENUM::MAG_PSU_STATE,  std::string  > m;
////
////    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ); // ENUM_TO_STRING MACRO in velaStructs.h
////    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON ); // ENUM_TO_STRING MACRO in velaStructs.h
////    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ); // ENUM_TO_STRING MACRO in velaStructs.h
////    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ); // ENUM_TO_STRING MACRO in velaStructs.h
////    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ] = ENUM_TO_STRING( HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ); // ENUM_TO_STRING MACRO in velaStructs.h
////
////    return enumStringMapToPythonDict( m );
////}
////______________________________________________________________________________
//#endif // BUILD_DLL
//

