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

#include "scopeController.h"
// stl
#include <iostream>
// djs
#include "scopeInterface.h"

//______________________________________________________________________________
scopeController::scopeController( const std::string &configFileLocation1,
                                  const std::string &configFileLocation2,
                                  const bool show_messages,
                                  const bool show_debug_messages,
                                  const bool shouldStartEPICS,
                                  const bool startVirtualMachine,
                                  const VELA_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages ),
localInterface( configFileLocation1, configFileLocation2, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES, shouldStartEPICS, startVirtualMachine, myMachineArea ),
shouldStartEPICS( shouldStartEPICS ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
void scopeController::initialise()
{
    if( localInterface.interfaceInitReport( shouldStartEPICS ) )
        message("scopeController instantiation success.");
}
//______________________________________________________________________________
scopeController::~scopeController(){}    //dtor
//______________________________________________________________________________
//std::vector< std::vector< double > > scopeController::monitorForNCounts( const std::string & scopeName, int numcounts )
//{
//    return localInterface.monitorForNCounts( scopeName, numcounts );
//}
//______________________________________________________________________________
void scopeController::monitorTracesForNShots( size_t N )
{
    localInterface.monitorTracesForNShots( N );
}
//______________________________________________________________________________
void scopeController::monitorATraceForNShots( const std::string trace, scopeStructs::SCOPE_PV_TYPE channel, size_t N )
{
    localInterface.monitorATraceForNShots( trace, channel, N );
}
//______________________________________________________________________________
void scopeController::monitorNumsForNShots( size_t N )
{
    localInterface.monitorNumsForNShots( N );
}
//______________________________________________________________________________
bool scopeController::isMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isMonitoringScopeTrace( scopeName, pvType );
}
//______________________________________________________________________________
bool scopeController::isNotMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringScopeTrace( scopeName, pvType );
}
//______________________________________________________________________________
bool scopeController::isMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isMonitoringScopeNum( scopeName, pvType );
}
//______________________________________________________________________________
bool scopeController::isNotMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringScopeNum( scopeName, pvType );
}
//______________________________________________________________________________
const scopeStructs::scopeTraceData & scopeController::getScopeTraceDataStruct( const std::string & scopeName )
{
    return localInterface.getScopeTraceDataStruct( scopeName );
}
//______________________________________________________________________________
const scopeStructs::scopeNumObject & scopeController::getScopeNumDataStruct( const std::string & scopeName )
{
    return localInterface.getScopeNumDataStruct( scopeName );
}
//______________________________________________________________________________
std::vector< std::vector< double > > scopeController::getScopeTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getScopeTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > scopeController::getScopeNums( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getScopeNums( name, pvType );
}
//______________________________________________________________________________
std::vector< std::vector< double > > scopeController::getPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > scopeController::getAreaUnderPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAreaUnderPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > scopeController::getScopeP1Vec( const std::string & name )
{
    return localInterface.getScopeP1Vec( name );
}
//______________________________________________________________________________
std::vector< double > scopeController::getScopeP2Vec( const std::string & name )
{
    return localInterface.getScopeP2Vec( name );
}
//______________________________________________________________________________
std::vector< double > scopeController::getScopeP3Vec( const std::string & name )
{
    return localInterface.getScopeP3Vec( name );
}
//______________________________________________________________________________
std::vector< double > scopeController::getScopeP4Vec( const std::string & name )
{
    return localInterface.getScopeP4Vec( name );
}
//______________________________________________________________________________
std::vector< double > scopeController::getMinOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getMinOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > scopeController::getMaxOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getMaxOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > scopeController::getAreaUnderTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getAreaUnderTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > scopeController::getTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< std::string > scopeController::getStrTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getStrTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< double > scopeController::getAvgNoise( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAvgNoise( name, pvType, part1, part2 );
}
//______________________________________________________________________________
double scopeController::getScopeP1( const std::string & name )
{
    return localInterface.getScopeP1( name );
}
//______________________________________________________________________________
double scopeController::getScopeP2( const std::string & name )
{
    return localInterface.getScopeP2( name );
}
//______________________________________________________________________________
double scopeController::getScopeP3( const std::string & name )
{
    return localInterface.getScopeP3( name );
}
//______________________________________________________________________________
double scopeController::getScopeP4( const std::string & name )
{
    return localInterface.getScopeP4( name );
}
//______________________________________________________________________________
double scopeController::getWCMQ()
{
    return localInterface.getWCMQ();
}
//______________________________________________________________________________
double scopeController::getICT1Q( const int part1, const int part2 )
{
    return localInterface.getICT1Q( part1, part2 );
}
//______________________________________________________________________________
double scopeController::getICT2Q( const int part1, const int part2 )
{
    return localInterface.getICT2Q( part1, part2 );
}
//______________________________________________________________________________
double scopeController::getFCUPQ()
{
    return localInterface.getFCUPQ();
}
//______________________________________________________________________________
double scopeController::getEDFCUPQ()
{
    return localInterface.getEDFCUPQ();
}
//______________________________________________________________________________
std::vector< std::string > scopeController::getScopeNames()
{
    return localInterface.getScopeNames();
}
//______________________________________________________________________________
std::vector< std::string > scopeController::getScopePVs()
{
    return localInterface.getScopePVs();
}
//______________________________________________________________________________
std::vector< std::string > scopeController::getScopeTracePVs()
{
    return localInterface.getScopeTracePVs();
}
//______________________________________________________________________________
std::vector< std::string > scopeController::getScopeNumPVs()
{
    return localInterface.getScopeNumPVs();
}
//______________________________________________________________________________
//bool scopeController::hasTrig( const std::string & name )
//{
//    return localInterface.hasTrig( name );
//}
////______________________________________________________________________________
//bool scopeController::hasNoTrig( const std::string & name )
//{
//    return localInterface.hasNoTrig( name );
//}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE  scopeController::getScopeState( const std::string & name )
//{
//    return localInterface.getScopeState( name );
//}
////______________________________________________________________________________
//std::string scopeController::getScopeStateStr( const std::string & name )
//{
//    return ENUM_TO_STRING(localInterface.getScopeState( name ));
//}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > scopeController::getILockStates( const std::string & objName )
{
    return localInterface.getILockStates( objName );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > scopeController::getILockStatesStr( const std::string & objName )
{
    return localInterface.getILockStatesStr( objName );
}
//______________________________________________________________________________
double scopeController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void scopeController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
