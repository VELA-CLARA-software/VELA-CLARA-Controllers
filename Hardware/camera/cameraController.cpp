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

#include "cameraController.h"
// stl
cameraController::cameraController( const std::string configFileLocation, const std::string configIOCFileLocation, const  bool show_messages, const bool show_debug_messages )
: controller( show_messages, show_debug_messages ), localInterface( configFileLocation, configIOCFileLocation, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
cameraController::cameraController( const  bool show_messages, const bool show_debug_messages  )
: controller( show_messages, show_debug_messages ), localInterface( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
cameraController::~cameraController(){}    //dtor
//______________________________________________________________________________
void cameraController::initialise()
{
    if( localInterface.interfaceInitReport() )
        message("cameraController instantiation success.");
}
//______________________________________________________________________________
bool cameraController::isON ( const std::string & cam )
{
    return localInterface.isON( cam );
}
//______________________________________________________________________________
bool cameraController::isOFF( const std::string & cam )
{
    return localInterface.isOFF( cam );
}
//______________________________________________________________________________
bool cameraController::start(const std::string & cam )
{
    return localInterface.start( cam );
}
//______________________________________________________________________________
bool cameraController::start( const std::vector< std::string  > & cam )
{
    return localInterface.start( cam );
}
//______________________________________________________________________________
bool cameraController::startAndWait(const std::string & cam, size_t timeout )
{
    return localInterface.startAndWait( cam, timeout );
}
//______________________________________________________________________________
std::vector< std::string > cameraController::startAndWait( const std::vector< std::string  > & cam, size_t timeout )
{
    return localInterface.startAndWait( cam, timeout );
}
//______________________________________________________________________________
bool cameraController::stop(const std::string & cam )
{
    return localInterface.stop( cam );
}
//______________________________________________________________________________
bool cameraController::stop( const std::vector< std::string  >  & cam )
{
    return localInterface.stop( cam );
}
//______________________________________________________________________________
bool cameraController::stopAll( )
{
    return localInterface.stopAll( );
}
//______________________________________________________________________________
bool cameraController::startCamDataMonitor( const std::string & cam, size_t N )
{
//    ScopedGILRelease scoped;
    return localInterface.startCamDataMonitor( cam, N );
}
//______________________________________________________________________________
std::vector< std::vector< cameraStructs::camDataType >> cameraController::getRawData(const std::string & name  )
{
    return localInterface.getRawData( name );
}
//______________________________________________________________________________
std::vector< double > cameraController::getRawDataStamp(const std::string & name  )
{
    return localInterface.getRawDataStamp( name );
}
//______________________________________________________________________________
std::vector< std::string > cameraController::getRawDataStampStr( const std::string & name )
{
    return localInterface.getRawDataStampStr( name );
}
//______________________________________________________________________________
bool cameraController::isMonitoring( const std::string & cam )
{
    return localInterface.isMonitoring( cam );
}
//______________________________________________________________________________
bool cameraController::isNotMonitoring( const std::string & cam )
{
    return localInterface.isNotMonitoring( cam );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE  >  cameraController::getILockStates( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  cameraController::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string > r;
    return r;
}
//______________________________________________________________________________
double cameraController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT();
};
//______________________________________________________________________________
void cameraController::set_CA_PEND_IO_TIMEOUT( double val )
{
    return localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
