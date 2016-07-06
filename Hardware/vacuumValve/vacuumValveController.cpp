#include "vacuumValveController.h"
// stl
#include <iostream>
// djs
#include "vacuumValveInterface.h"

//______________________________________________________________________________
vacuumValveController::vacuumValveController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: controller( show_messages, show_debug_messages ), localInterface( configFileLocation, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
vacuumValveController::vacuumValveController( const  bool show_messages, const bool show_debug_messages  )
: controller( show_messages, show_debug_messages ), localInterface( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
void vacuumValveController::initialise()
{
    /// Keep a local copy of all the existing shutter names for openShutter1(), openShutter2(), etc...

    localInterface.findVacValveNames( vacValveNames );

    message("The vacuumValveController succesfully instantiated a vacuumValveInterface.");
}
//______________________________________________________________________________
vacuumValveController::~vacuumValveController(){}    //dtor
//______________________________________________________________________________
void vacuumValveController::openVacValve( const std::string & name )
{
    localInterface.openVacValve( name );
}
//______________________________________________________________________________
void vacuumValveController::closeVacValve( const std::string & name )
{
    localInterface.closeVacValve( name );
}
//______________________________________________________________________________
void vacuumValveController::openValve1( )
{
    if( vacValveNames.size() >= 1 )
        openVacValve( vacValveNames[0] );
}
//______________________________________________________________________________
void vacuumValveController::openValve2( )
{
    if( vacValveNames.size() >= 2 )
        openVacValve( vacValveNames[1] );
}
//______________________________________________________________________________
void vacuumValveController::openValve3( )
{
    if( vacValveNames.size() >= 3 )
        openVacValve( vacValveNames[2] );
}
//______________________________________________________________________________
void vacuumValveController::openValve4( )
{
    if( vacValveNames.size() >= 4 )
        openVacValve( vacValveNames[3] );
}
//______________________________________________________________________________
void vacuumValveController::openValve5( )
{
    if( vacValveNames.size() >= 5 )
        openVacValve( vacValveNames[4] );
}
//______________________________________________________________________________
void vacuumValveController::openValve6( )
{
    if( vacValveNames.size() >= 6 )
        openVacValve( vacValveNames[5] );
}
//______________________________________________________________________________
void vacuumValveController::openValve7( )
{
    if( vacValveNames.size() >= 7 )
        openVacValve( vacValveNames[6] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve1( )
{
    if( vacValveNames.size() >= 1 )
        closeVacValve( vacValveNames[0] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve2( )
{
    if( vacValveNames.size() >= 2 )
        closeVacValve( vacValveNames[1] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve3( )
{
    if( vacValveNames.size() >= 3 )
        closeVacValve( vacValveNames[2] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve4( )
{
    if( vacValveNames.size() >= 4 )
        closeVacValve( vacValveNames[3] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve5( )
{
    if( vacValveNames.size() >= 5 )
        closeVacValve( vacValveNames[4] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve6( )
{
    if( vacValveNames.size() >= 6 )
        closeVacValve( vacValveNames[5] );
}
//______________________________________________________________________________
void vacuumValveController::closeValve7( )
{
    if( vacValveNames.size() >= 7 )
        closeVacValve( vacValveNames[6] );
}
//______________________________________________________________________________
bool vacuumValveController::isOpen( const std::string & name )
{
    return localInterface.isOpen( name );
}
//______________________________________________________________________________
bool vacuumValveController::isClosed( const std::string & name )
{
    return localInterface.isClosed( name );
}
//______________________________________________________________________________
bool vacuumValveController::openAndWait( const std::string & name, const time_t waitTime )
{
    return localInterface.openAndWait( name, waitTime );
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWait( const std::string & name, const time_t waitTime )
{
    return localInterface.closeAndWait( name, waitTime );
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve1( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 1 )
        ret = openAndWait( vacValveNames[0], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve2( time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 2 )
        ret = openAndWait( vacValveNames[1], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve3( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 3 )
        ret = openAndWait( vacValveNames[2], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve4( time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 4 )
        ret = openAndWait( vacValveNames[3], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve5( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 5 )
        ret = openAndWait( vacValveNames[4], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve6( time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 6 )
        ret = openAndWait( vacValveNames[5], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::openAndWaitValve7( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 7 )
        ret = openAndWait( vacValveNames[6], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve1( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 1 )
        ret = closeAndWait( vacValveNames[0], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve2( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 2 )
        ret = closeAndWait( vacValveNames[1], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve3( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 3 )
        ret = closeAndWait( vacValveNames[2], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve4( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 4 )
        ret = closeAndWait( vacValveNames[3], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve5( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 5 )
        ret = closeAndWait( vacValveNames[4], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve6( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 6 )
        ret = closeAndWait( vacValveNames[5], waitTime );
    return ret;
}
//______________________________________________________________________________
bool vacuumValveController::closeAndWaitValve7( const time_t waitTime )
{
    bool ret = false;
    if( vacValveNames.size() >= 7 )
        ret = closeAndWait( vacValveNames[6], waitTime );
    return ret;
}
//______________________________________________________________________________
std::vector< std::string > vacuumValveController::getVacValveNames()
{
    return localInterface.getVacValveNames();
}
//______________________________________________________________________________
double vacuumValveController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void vacuumValveController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
VELA_ENUM::VALVE_STATE  vacuumValveController::getVacValveState( const std::string & name )
{
    return localInterface.getVacValveState( name );
}
//______________________________________________________________________________
std::string vacuumValveController::getVacValveStateStr( const std::string & name )
{
    return ENUM_TO_STRING(localInterface.getVacValveState( name ));
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > vacuumValveController::getILockStates( const std::string & objName )
{
    return localInterface.getILockStates( objName );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > vacuumValveController::getILockStatesStr( const std::string & objName )
{
    return localInterface.getILockStatesStr( objName );
}
//______________________________________________________________________________
