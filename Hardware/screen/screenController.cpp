#include "screenController.h"
//stl
#include <iostream>
#include <time.h>
//djs
#include "screenInterface.h"
#include "structs.h"

///Initialsing
///Moving
///Checks
///Get Stuff


screenController::screenController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & conf1,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const VELA_ENUM::MACHINE_AREA myMachineArea):
controller( show_messages, show_debug_messages ),
localInterface( conf1, startVirtualMachine, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES, shouldStartEPICs, myMachineArea ),
shouldStartEPICs(shouldStartEPICs),
myMachineArea(myMachineArea)
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

  if( localInterface.interfaceInitReport() )
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
void screenController::moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    localInterface.moveScreenTo(name, state);
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
bool screenController::setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    return localInterface.setScreenSDEV(name, state);
}
//________________________________________________________________________________
bool screenController::setScreenTrigger(const std::string & name )
{
    return localInterface.setScreenTrigger(name);
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
////________________________________________________________________________________
//const std::vector<std::string> screenController::get_VELA_PNEUMATIC_Screens(const std::vector<std::string>& names)
//{
//    return localInterface.get_VELA_PNEUMATIC_Screens(names);
//}
////________________________________________________________________________________
//const std::vector<std::string> screenController::get_VELA_HV_MOVER_Screens(const std::vector<std::string>& names)
//{
//    return localInterface.get_VELA_HV_MOVER_Screens(names);
//}
////________________________________________________________________________________
//const std::vector<std::string> screenController::get_CLARA_PNEUMATIC_Screens(const std::vector<std::string>& names)
//{
//    return localInterface.get_CLARA_PNEUMATIC_Screens(names);
//}
////________________________________________________________________________________
//const std::vector<std::string> screenController::get_CLARA_HV_MOVER_Screens(const std::vector<std::string>& names)
//{
//    return localInterface.get_CLARA_HV_MOVER_Screens(names);
//}
////________________________________________________________________________________
//const std::vector<std::string> screenController::get_CLARA_V_MOVER_Screens(const std::vector<std::string>& names)
//{
//    return localInterface.get_CLARA_V_MOVER_Screens(names);
//}
////________________________________________________________________________________
//bool screenController::screenMoveTo(const std::vector<std::string>& names,const std::vector<screenStructs::SCREEN_STATE>& states)
//{
//    return localInterface.screenMoveTo(names,states);
//}
////________________________________________________________________________________
//bool screenController::screenMoveTo(const std::string & name, const screenStructs::SCREEN_STATE & states)
//{
//    return localInterface.screenMoveTo(name,states);
//}
////________________________________________________________________________________
//bool screenController::setPosition(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const double value )
//{
//    return localInterface.setPosition(name, dir, value);
//}
////________________________________________________________________________________
//bool screenController::isMoving(const std::string& name)
//{
//    return localInterface.isMoving(name,false);
//}
////________________________________________________________________________________
//bool screenController::isNotMoving(const std::string& name)
//{
//    return localInterface.isMoving(name,false);
//}
////________________________________________________________________________________
//bool screenController::is_VELA_PNEUMATIC(const std::string & name)
//{
//    return localInterface.is_VELA_PNEUMATIC(name);
//}
////________________________________________________________________________________
//bool screenController::is_VELA_HV_MOVER(const std::string & name)
//{
//    return localInterface.is_VELA_HV_MOVER(name);
//}
////________________________________________________________________________________
//bool screenController::is_CLARA_PNEUMATIC(const std::string & name)
//{
//    return localInterface.is_CLARA_PNEUMATIC(name);
//}
////________________________________________________________________________________
//bool screenController::is_CLARA_HV_MOVER(const std::string & name)
//{
//    return localInterface.is_CLARA_HV_MOVER(name);
//}
////________________________________________________________________________________
//bool screenController::is_CLARA_V_MOVER(const std::string & name)
//{
//    return localInterface.is_CLARA_V_MOVER(name);
//}
////________________________________________________________________________________
//bool screenController::is_H_Element(const std::string & name, const screenStructs::SCREEN_STATE e)
//{
//    return localInterface.is_H_Element(name, e);
//}
////________________________________________________________________________________
//bool screenController::is_V_Element(const std::string & name, const screenStructs::SCREEN_STATE e)
//{
//    return localInterface.is_V_Element(name, e);
//}
////________________________________________________________________________________
//double screenController::get_H_ACTPOS(const std::string & name)
//{
//    return localInterface.get_H_ACTPOS(name);
//}
////________________________________________________________________________________
//double screenController::get_V_ACTPOS(const std::string & name)
//{
//    return localInterface.get_V_ACTPOS(name);
//}
////________________________________________________________________________________
//bool screenController::isScreenOUT(const std::string & name )
//{
//    return localInterface.isScreenOUT(name, false);
//}
////________________________________________________________________________________
//bool screenController::isScreenIN(const std::string & name)
//{
//    return localInterface.isScreenIN(name, false);
//}
////________________________________________________________________________________
//std::vector<bool> screenController::isScreenIN( const std::vector<std::string> & name )
//{
//    return localInterface.isScreenIN(name);
//}
////________________________________________________________________________________
//std::vector<bool> screenController::isScreenOUT( const std::vector<std::string> & name )
//{
//    return localInterface.isScreenOUT(name);
//}
////________________________________________________________________________________
//std::vector<bool> screenController::exists_and_isLocked(const std::string& name)
//{
//    return localInterface.exists_and_isLocked(name);
//}
////________________________________________________________________________________
//std::vector<bool> screenController::exists_and_isNotLocked(const std::string& name)
//{
//    return localInterface.exists_and_isNotLocked(name);
//}
////________________________________________________________________________________nt
//screenStructs::SCREEN_STATE screenController::getScreenState( const std::string & name )
//{
//    return localInterface.getScreenState(name);
//}
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
double screenController::get_CA_PEND_IO_TIMEOUT()
{
  return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//_____________________________________________________________________________________________
void screenController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > screenController::getILockStates( const std::string & name )
{
    return localInterface.getILockStates( name );
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > screenController::getILockStatesStr( const std::string & name )
{
    return localInterface.getILockStatesStr( name );
}
//______________________________________________________________________________________________
