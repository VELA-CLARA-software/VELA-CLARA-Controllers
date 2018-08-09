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
//  Last edit:   19-03-2018
//  FileName:    controller.cpp
//  Description:
//
//
//*/
#include "screenController.h"
//stl
#include <iostream>
#include <time.h>
screenController::screenController( const std::string &configFileLocation,
                                    bool& show_messages,
                                    bool& show_debug_messages,
                                    const bool shouldStartEPICs,
                                    const bool startVirtualMachine,
                                    const HWC_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::SCREEN ),
localInterface( configFileLocation, show_messages, show_debug_messages, shouldStartEPICs, startVirtualMachine, myMachineArea ),
shouldStartEPICs( shouldStartEPICs ),
machineArea( myMachineArea )
{
    initialise();
}
//_____________________________________________________________________________
screenController::~screenController(){}
//dtor
//_______________________________________________________________________________
void screenController::initialise()
{
//  localInterface.getScreenNames( ScreenNames );

//  if( localInterface.interfaceInitReport() )
        message("screenController instantation success.");
}
////________________________________________________________________________________
//bool screenController::screenIN (const std::string& name )
//{
//    return localInterface.screenIN( name );
//}
////________________________________________________________________________________
//bool screenController::screenIN (const std::vector< std::string> & name )
//{
//    return localInterface.screenIN( name );
//}
////________________________________________________________________________________
//bool screenController::screenOUT(const std::string & name )
//{
//    return localInterface.screenOUT( name );
//}
////________________________________________________________________________________
//bool screenController::screenOUT(const std::vector<std::string> & name )
//{
//    return localInterface.screenOUT( name );
//}
//________________________________________________________________________________
bool screenController::isHOut(const std::string & name)
{
    return localInterface.isHOut(name);
}
//________________________________________________________________________________
bool screenController::isVOut(const std::string & name)
{
    return localInterface.isVOut(name);
}
//________________________________________________________________________________
bool screenController::is_HandV_OUT(const std::string & name)
{
    return localInterface.is_HandV_OUT(name);
}
//________________________________________________________________________________
bool screenController::isHIn(const std::string & name)
{
    return localInterface.isHIn(name);
}
//________________________________________________________________________________
bool screenController::isVIn(const std::string & name)
{
    return localInterface.isVIn(name);
}
//________________________________________________________________________________
bool screenController::isScreenIn(const std::string & name)
{
    return localInterface.isScreenIn(name);
}
//________________________________________________________________________________

bool screenController::isClearForBeam(const std::string & name)
{
    return localInterface.isClearForBeam(name);
}

//________________________________________________________________________________
bool screenController::isMover(const std::string & name)
{
    return localInterface.isMover(name);
}
//________________________________________________________________________________
bool screenController::isPneumatic(const std::string & name)
{
    return localInterface.isPneumatic(name);
}
//________________________________________________________________________________
bool screenController::isHMoving(const std::string & name)
{
    return localInterface.isHMoving(name);
}
//________________________________________________________________________________
bool screenController::isVMoving(const std::string & name)
{
    return localInterface.isVMoving(name);
}
//________________________________________________________________________________
bool screenController::isScreenMoving(const std::string & name)
{
    return localInterface.isScreenMoving(name);
}
//________________________________________________________________________________
void screenController::moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    localInterface.moveScreenTo(name, state);
}
//________________________________________________________________________________
void screenController::insertYAG( const std::string & name )
{
    localInterface.insertYAG(name);
}
//________________________________________________________________________________
void screenController::moveScreenOut( const std::string & name )
{
    localInterface.moveScreenOut(name);
}
//________________________________________________________________________________
void screenController::resetPosition( const std::string & name )
{
    localInterface.resetPosition(name);
}
//________________________________________________________________________________
void screenController::jogScreen( const std::string & name, const double jog )
{
    localInterface.jogScreen(name, jog);
}
//________________________________________________________________________________
void screenController::setPosition( const std::string & name, const double setPos )
{
    localInterface.setPosition(name, setPos);
}
//________________________________________________________________________________
bool screenController::setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    return localInterface.setScreenSDEV(name, state);
}
//________________________________________________________________________________
bool screenController::setScreenTrigger(const std::string & name )
{
    return localInterface.setScreenTrigger(name);
}
//________________________________________________________________________________
bool screenController::setEX(const std::string & name )
{
    return localInterface.setEX(name);
}
////________________________________________________________________________________
//screenStructs::SCREEN_STATE screenController::getScreenState( const std::string & name, const bool weKnowEntryExists )
//{
//    return localInterface.getScreenState(name, weKnowEntryExists);
//}
//________________________________________________________________________________
const std::string screenController::getScreenState( const std::string & name )
{
    return localInterface.getScreenState(name);
}
//________________________________________________________________________________
const std::string screenController::getScreenType( const std::string & name )
{
    return localInterface.getScreenTypeStr(name);
}
//________________________________________________________________________________
const screenStructs::screenObject & screenController::getScreenObject( const std::string & name )
{
    return localInterface.getScreenObject(name);
}
//________________________________________________________________________________
bool screenController::isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    return localInterface.isScreenInState(name, sta);
}
//________________________________________________________________________________
bool screenController::isYAGIn(const std::string & name)
{
    return localInterface.isYAGIn(name);
}
//________________________________________________________________________________
bool screenController::isHEnabled(const std::string & name)
{
    return localInterface.isHEnabled(name);
}
//________________________________________________________________________________
bool screenController::isVEnabled(const std::string & name)
{
    return localInterface.isVEnabled(name);
}
//________________________________________________________________________________
bool screenController::isHElement(const std::string & name, const screenStructs::SCREEN_STATE e)
{
    return localInterface.isHElement(name, e);
}
//________________________________________________________________________________
bool screenController::isVElement(const std::string & name, const screenStructs::SCREEN_STATE e)
{
    return localInterface.isVElement(name, e);
}
//________________________________________________________________________________
double screenController::getACTPOS(const std::string & name)
{
    return localInterface.getACTPOS(name);
}
//________________________________________________________________________________
double screenController::getJDiff(const std::string & name)
{
    return localInterface.getJDiff(name);
}
//________________________________________________________________________________
double screenController::getDevicePosition(const std::string & name, const screenStructs::SCREEN_STATE state)
{
    return localInterface.getDevicePosition(name, state);
}
//________________________________________________________________________________
double screenController::getPosition(const std::string & name)
{
    return localInterface.getPosition(name);
}
//________________________________________________________________________________
std::vector< screenStructs::SCREEN_STATE > screenController::getAvailableDevices(const std::string & name)
{
    return localInterface.getAvailableDevices(name);
}
//___________________________________________________________________________________________________________
std::vector< std::string > screenController::getScreenNames()
{
    return localInterface.getScreenNames();
}
////________________________________________________________________________________
//bool screenController::isScreenInPosition(const std::string & name, screenStructs::SCREEN_STATE sta)
//{
//    return localInterface.isScreenInPosition(name, sta);
//}
////________________________________________________________________________________
//bool screenController::isScreenOUT(const std::string & name, const bool weKnowEntryExists )
//{
//    return localInterface.isScreenOUT(name, weKnowEntryExists);
//}
//________________________________________________________________________________
bool screenController::isScreenIN(const std::string & name, const bool weKnowEntryExists )
{
    return localInterface.isScreenIN(name, weKnowEntryExists);
}
//________________________________________________________________________________
#ifdef BUILD_DLL
//______________________________________________________________________________
boost::python::dict screenController::getILockStatesStr_Py( std::string& name )
{
    return enumStringMapToPythonDict( getILockStatesStr( name ) );
}
//______________________________________________________________________________
boost::python::dict screenController::getILockStates_Py( std::string& name )
{
    return enumMapToPythonDict(getILockStates(name) );
}
//______________________________________________________________________________
boost::python::list screenController::getScreenNames_Py()
{
    return toPythonList(getScreenNames());
}
//______________________________________________________________________________
boost::python::dict screenController::get_SCREEN_STATE_Definition()
{
    std::map< screenStructs::SCREEN_STATE,  std::string  > m;
    m[screenStructs::SCREEN_STATE::SCREEN_IN        ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_IN        );
    m[screenStructs::SCREEN_STATE::SCREEN_OUT       ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_OUT       );
    m[screenStructs::SCREEN_STATE::SCREEN_MOVING    ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_MOVING    );
    m[screenStructs::SCREEN_STATE::SCREEN_ERROR     ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_ERROR     );
    m[screenStructs::SCREEN_STATE::SCREEN_UNKNOWN   ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_UNKNOWN   );
    m[screenStructs::SCREEN_STATE::V_RETRACTED      ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_RETRACTED      );
    m[screenStructs::SCREEN_STATE::V_MAX            ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_MAX            );
    m[screenStructs::SCREEN_STATE::V_MIRROR         ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_MIRROR         );
    m[screenStructs::SCREEN_STATE::V_YAG            ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_YAG            );
    m[screenStructs::SCREEN_STATE::V_GRAT           ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_GRAT           );
    m[screenStructs::SCREEN_STATE::V_SLIT_1         ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_SLIT_1         );
    m[screenStructs::SCREEN_STATE::V_RF             ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_RF             );
    m[screenStructs::SCREEN_STATE::V_COL            ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::V_COL            );
    m[screenStructs::SCREEN_STATE::H_RETRACTED      ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_RETRACTED      );
    m[screenStructs::SCREEN_STATE::H_RF             ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_RF             );
    m[screenStructs::SCREEN_STATE::H_SLIT_1         ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_SLIT_1         );
    m[screenStructs::SCREEN_STATE::H_SLIT_2         ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_SLIT_2         );
    m[screenStructs::SCREEN_STATE::H_SLIT_3         ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_SLIT_3         );
    m[screenStructs::SCREEN_STATE::H_APT_1          ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_APT_1          );
    m[screenStructs::SCREEN_STATE::H_APT_2          ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_APT_2          );
    m[screenStructs::SCREEN_STATE::H_APT_3          ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::H_APT_3          );
    m[screenStructs::SCREEN_STATE::UNKNOWN_POSITION ] = ENUM_TO_STRING(screenStructs::SCREEN_STATE::UNKNOWN_POSITION );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict screenController::get_DRIVER_STATE_Definition()
{
    std::map< screenStructs::DRIVER_STATE,  std::string  > m;
    m[screenStructs::DRIVER_STATE::H_DRIVER_MOVING      ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::H_DRIVER_MOVING      );
    m[screenStructs::DRIVER_STATE::H_DRIVER_STATIONARY  ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::H_DRIVER_STATIONARY  );
    m[screenStructs::DRIVER_STATE::H_DRIVER_DISABLED    ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::H_DRIVER_DISABLED    );
    m[screenStructs::DRIVER_STATE::H_DRIVER_ENABLED     ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::H_DRIVER_ENABLED     );
    m[screenStructs::DRIVER_STATE::H_DRIVER_ERROR       ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::H_DRIVER_ERROR       );
    m[screenStructs::DRIVER_STATE::V_DRIVER_MOVING      ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::V_DRIVER_MOVING      );
    m[screenStructs::DRIVER_STATE::V_DRIVER_STATIONARY  ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::V_DRIVER_STATIONARY  );
    m[screenStructs::DRIVER_STATE::V_DRIVER_DISABLED    ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::V_DRIVER_DISABLED    );
    m[screenStructs::DRIVER_STATE::V_DRIVER_ENABLED     ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::V_DRIVER_ENABLED     );
    m[screenStructs::DRIVER_STATE::V_DRIVER_ERROR       ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::V_DRIVER_ERROR       );
    m[screenStructs::DRIVER_STATE::UNKNOWN_DRIVER_STATE ] = ENUM_TO_STRING(screenStructs::DRIVER_STATE::UNKNOWN_DRIVER_STATE );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
#endif // BUILD_DLL
//____________________________________________________________________________________________
double screenController::get_CA_PEND_IO_TIMEOUT()const
{
  return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//_____________________________________________________________________________________________
void screenController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > screenController::getILockStates( const std::string & name )const
{
    return localInterface.getILockStates( name );
}
//______________________________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string > screenController::getILockStatesStr( const std::string & name )const
{
    return localInterface.getILockStatesStr( name );
}
//______________________________________________________________________________________________
