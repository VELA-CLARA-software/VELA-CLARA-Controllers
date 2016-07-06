#include "shutterController.h"
// stl
#include <iostream>
// djs
#include "shutterInterface.h"

shutterController::shutterController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: controller( show_messages, show_debug_messages ), localInterface( configFileLocation, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
shutterController::shutterController( const  bool show_messages, const bool show_debug_messages  )
: controller( show_messages, show_debug_messages ), localInterface( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
shutterController::~shutterController(){}    //dtor
//______________________________________________________________________________
void shutterController::initialise()
{
    /// Keep a local copy of all the existing shutter names for openShutter1(), openShutter2(), etc...
    localInterface.getShutterNames( pilShutterNames );

    if( localInterface.interfaceInitReport() )
        message("shutterController instantiation success.");
}
//______________________________________________________________________________
void shutterController::open( const std::string & name )
{
    localInterface.open( name );
}
//______________________________________________________________________________
void shutterController::close( const std::string & name )
{
    localInterface.close( name );
}
//______________________________________________________________________________
void shutterController::openShutter1( )
{
    if( pilShutterNames.size() >= 1 )
        open( pilShutterNames[0] );
}
//______________________________________________________________________________
void shutterController::closeShutter1( )
{
    if( pilShutterNames.size() >= 1 )
        close( pilShutterNames[0] );
}
//______________________________________________________________________________
void shutterController::openShutter2( )
{
    if( pilShutterNames.size() >= 2 )
        open( pilShutterNames[ 1 ] );
}
//______________________________________________________________________________
void shutterController::closeShutter2( )
{
    if( pilShutterNames.size() >= 2 )
        close( pilShutterNames[ 1 ] );
}
//______________________________________________________________________________
bool shutterController::isOpen( const std::string & name )
{
    return localInterface.isOpen( name );
}
//______________________________________________________________________________
bool shutterController::isClosed( const std::string & name )
{
    return localInterface.isClosed( name );
}
//______________________________________________________________________________
bool shutterController::openAndWaitShutter1( const time_t waitTime )
{
    bool ret = false;
    if( pilShutterNames.size() >= 1 )
        ret = openAndWait( pilShutterNames[0], waitTime );
    return ret;
}
//______________________________________________________________________________
bool shutterController::openAndWaitShutter2( time_t waitTime )
{
    bool ret = false;
    if( pilShutterNames.size() >= 2 )
        ret = openAndWait( pilShutterNames[1], waitTime );
    return ret;
}
//______________________________________________________________________________
bool shutterController::closeAndWaitShutter1( const time_t waitTime )
{
    bool ret = false;
    if( pilShutterNames.size() >= 1 )
        ret = closeAndWait( pilShutterNames[0], waitTime );
    return ret;
}
//______________________________________________________________________________
bool shutterController::closeAndWaitShutter2( const time_t waitTime )
{
    bool ret = false;
    if( pilShutterNames.size() >= 2 )
        ret = closeAndWait( pilShutterNames[1], waitTime );
    return ret;
}
//______________________________________________________________________________
bool shutterController::openAndWait( const std::string & name, const time_t waitTime )
{
    return localInterface.openAndWait( name, waitTime );
}
//______________________________________________________________________________
bool shutterController::closeAndWait( const std::string & name, const time_t waitTime )
{
    return localInterface.closeAndWait( name, waitTime );
}
//______________________________________________________________________________
double shutterController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void shutterController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
VELA_ENUM::SHUTTER_STATE  shutterController::getShutterState( const std::string & name )
{
    return localInterface.getShutterState( name );
}
//______________________________________________________________________________
std::string shutterController::getShutterStateStr( const std::string & name )
{
    return ENUM_TO_STRING(localInterface.getShutterState( name ));
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > shutterController::getILockStates( const std::string & name )
{
    return localInterface.getILockStates( name );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > shutterController::getILockStatesStr( const std::string & name )
{
    return localInterface.getILockStatesStr( name );
}
//______________________________________________________________________________
std::vector< std::string > shutterController::getShutterNames()
{
    return pilShutterNames;
}

