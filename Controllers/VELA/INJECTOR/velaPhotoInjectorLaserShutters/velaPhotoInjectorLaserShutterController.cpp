#include "velaPhotoInjectorLaserShutterController.h"
// stl
#include <iostream>

velaPhotoInjectorLaserShutterController::velaPhotoInjectorLaserShutterController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
:shutterController( configFileLocation, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaPhotoInjectorLaserShutterController::velaPhotoInjectorLaserShutterController(const bool RealMachine, const  bool show_messages, const bool show_debug_messages  )
:  shutterController((RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_PIL_SHUTTER_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_PIL_SHUTTER_CONFIG, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaPhotoInjectorLaserShutterController::~velaPhotoInjectorLaserShutterController(){}    //dtor
//______________________________________________________________________________

//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict velaPhotoInjectorLaserShutterController::getShutterStateDefinition()
{
    std::map< VELA_ENUM::SHUTTER_STATE,  std::string  > m;

    m[ VELA_ENUM::SHUTTER_STATE::SHUTTER_CLOSED ] = ENUM_TO_STRING( VELA_ENUM::SHUTTER_STATE::SHUTTER_CLOSED ); // ENUM_TO_STRING MACRO in structs.h
    m[ VELA_ENUM::SHUTTER_STATE::SHUTTER_OPEN   ] = ENUM_TO_STRING( VELA_ENUM::SHUTTER_STATE::SHUTTER_OPEN   );
    m[ VELA_ENUM::SHUTTER_STATE::SHUTTER_TIMING ] = ENUM_TO_STRING( VELA_ENUM::SHUTTER_STATE::SHUTTER_TIMING );
    m[ VELA_ENUM::SHUTTER_STATE::SHUTTER_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::SHUTTER_STATE::SHUTTER_ERROR  );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict velaPhotoInjectorLaserShutterController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
//______________________________________________________________________________
boost::python::dict velaPhotoInjectorLaserShutterController::getILockStatesStr_Py( std::string name )
{
    return enumStringMapToPythonDict( getILockStatesStr( name ) );
}
//______________________________________________________________________________
void velaPhotoInjectorLaserShutterController::open_Py( const std::string name )
{
    return open( name );
}
//______________________________________________________________________________
void velaPhotoInjectorLaserShutterController::close_Py( const std::string name )
{
    return close( name );
}
//______________________________________________________________________________
bool velaPhotoInjectorLaserShutterController::isOpen_Py( const std::string name )
{
    return isOpen( name );
}
//______________________________________________________________________________
bool velaPhotoInjectorLaserShutterController::isClosed_Py( const std::string name )
{
    return isClosed( name );
}
//______________________________________________________________________________
bool velaPhotoInjectorLaserShutterController::closeAndWait_Py( const std::string name, const time_t waitTime )
{
    return closeAndWait( name, waitTime );
}
//______________________________________________________________________________
bool velaPhotoInjectorLaserShutterController::openAndWait_Py( const std::string name, const time_t waitTime  )
{
    return openAndWait( name, waitTime );
}
//______________________________________________________________________________
VELA_ENUM::SHUTTER_STATE velaPhotoInjectorLaserShutterController::getShutterState_Py( const std::string name )
{
    return getShutterState( name);
}
//______________________________________________________________________________
std::string velaPhotoInjectorLaserShutterController::getShutterStateStr_Py( const std::string name )
{
    return getShutterStateStr( name);
}
#endif //BUILD_DLL


