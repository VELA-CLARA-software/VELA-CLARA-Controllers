#include "velaVacuumValveController.h"
// stl
#include <iostream>


//______________________________________________________________________________
velaVacuumValveController::velaVacuumValveController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: vacuumValveController(configFileLocation, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaVacuumValveController::velaVacuumValveController(const bool RealMachine, const  bool show_messages, const bool show_debug_messages  )
:  vacuumValveController((RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_VALVE_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_VALVE_CONFIG,
                         show_messages, show_debug_messages )
{}
velaVacuumValveController::~velaVacuumValveController()
{}
//______________________________________________________________________________

//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict velaVacuumValveController::getVacValveStateDefinition()
{
    std::map< VELA_ENUM::VALVE_STATE,  std::string  > m;

    m[ VELA_ENUM::VALVE_STATE::VALVE_CLOSED ] = ENUM_TO_STRING( VELA_ENUM::VALVE_STATE::VALVE_CLOSED  ); // ENUM_TO_STRING MACRO in structs.h
    m[ VELA_ENUM::VALVE_STATE::VALVE_OPEN   ] = ENUM_TO_STRING( VELA_ENUM::VALVE_STATE::VALVE_OPEN    );
    m[ VELA_ENUM::VALVE_STATE::VALVE_TIMING ] = ENUM_TO_STRING( VELA_ENUM::VALVE_STATE::VALVE_TIMING  );
    m[ VELA_ENUM::VALVE_STATE::VALVE_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::VALVE_STATE::VALVE_ERROR   );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict velaVacuumValveController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve_Py( const std::string & name )
{
    return openVacValve( name );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve1_Py( )
{
    return openValve1( );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve2_Py( )
{
    return openValve2( );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve3_Py( )
{
    return openValve3( );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve4_Py( )
{
    return openValve4( );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve5_Py( )
{
    return openValve5( );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve6_Py( )
{
    return openValve6( );
}
//______________________________________________________________________________
void velaVacuumValveController::openValve7_Py( )
{
    return openValve7( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve_Py( const std::string & name )
{
    return closeVacValve( name );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve1_Py( )
{
    return closeValve1( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve2_Py( )
{
    return closeValve2( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve3_Py( )
{
    return closeValve3( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve4_Py( )
{
    return closeValve4( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve5_Py( )
{
    return closeValve5( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve6_Py( )
{
    return closeValve6( );
}
//______________________________________________________________________________
void velaVacuumValveController::closeValve7_Py( )
{
    return closeValve7( );
}
//______________________________________________________________________________
bool velaVacuumValveController::isOpen_Py( const std::string & name )
{
    return isOpen( name );
}
//______________________________________________________________________________
bool velaVacuumValveController::isClosed_Py( const std::string & name )
{
    return isClosed( name );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve_Py( const std::string & name, const time_t waitTime )
{
    return openAndWait( name, waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve_Py( const std::string & name, const time_t waitTime  )
{
    return closeAndWait( name, waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve1_Py( const time_t waitTime )
{
    return openAndWaitValve1( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve2_Py( const time_t waitTime )
{
    return openAndWaitValve2( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve3_Py( const time_t waitTime )
{
    return openAndWaitValve3( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve4_Py( const time_t waitTime )
{
    return openAndWaitValve4( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve5_Py( const time_t waitTime )
{
    return openAndWaitValve5( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve6_Py( const time_t waitTime )
{
    return openAndWaitValve6( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::openAndWaitValve7_Py( const time_t waitTime )
{
    return openAndWaitValve7( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve1_Py( const time_t waitTime  )
{
    return closeAndWaitValve1( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve2_Py( const time_t waitTime  )
{
    return closeAndWaitValve2( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve3_Py( const time_t waitTime  )
{
    return closeAndWaitValve3( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve4_Py( const time_t waitTime  )
{
    return closeAndWaitValve4( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve5_Py( const time_t waitTime  )
{
    return closeAndWaitValve5( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve6_Py( const time_t waitTime  )
{
    return closeAndWaitValve6( waitTime );
}
//______________________________________________________________________________
bool velaVacuumValveController::closeAndWaitValve7_Py( const time_t waitTime  )
{
    return closeAndWaitValve7( waitTime );
}
//______________________________________________________________________________
std::vector< std::string > velaVacuumValveController::getVacValveNames_Py()
{
    return getVacValveNames();
}
//______________________________________________________________________________
VELA_ENUM::VALVE_STATE velaVacuumValveController::getVacValveState_Py( const std::string & name )
{
    return getVacValveState( name );
}
#endif //BUILD_DLL


