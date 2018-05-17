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

#include "beamPositionMonitorController.h"
// stl
#include <iostream>
//______________________________________________________________________________
beamPositionMonitorController::beamPositionMonitorController( const std::string &configFileLocation,
                                                              bool& show_messages,
                                                              bool& show_debug_messages,
                                                              const bool shouldStartEPICs,
                                                              const bool startVirtualMachine,
                                                              const HWC_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::BPM ),
localInterface( configFileLocation, show_messages, show_debug_messages, shouldStartEPICs, startVirtualMachine, myMachineArea ),
shouldStartEPICs( shouldStartEPICs ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
//beamPositionMonitorController::beamPositionMonitorController( const  bool show_messages, const bool show_debug_messages  )
//: controller( show_messages, show_debug_messages ), localInterface( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
//{
//    initialise();
//}
//______________________________________________________________________________
beamPositionMonitorController::~beamPositionMonitorController(){}    //dtor
//______________________________________________________________________________
void beamPositionMonitorController::initialise()
{
    message("beamPositionMonitorController instantiation success.");
//    if( localInterface.interfaceInitReport( shouldStartEPICs ) )
//        message("beamPositionMonitorController instantiation success.");
}
//______________________________________________________________________________
const beamPositionMonitorStructs::bpmDataObject & beamPositionMonitorController::getBPMDataObject( const std::string & name )
{
    return localInterface.getBPMDataObject( name );
}
//______________________________________________________________________________
std::vector< std::vector< double > > beamPositionMonitorController::getBPMRawData( const std::string & name )
{
    return localInterface.getBPMRawData( name );
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorController::getBPMXVec( const std::string & name )
{
    return localInterface.getBPMXVec( name );
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorController::getBPMYVec( const std::string & name )
{
    return localInterface.getBPMYVec( name );
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorController::getBPMQVec( const std::string & name )
{
    return localInterface.getBPMQVec( name );
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorController::getTimeStamps( const std::string & name )
{
    return localInterface.getTimeStamps( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > beamPositionMonitorController::getBPMRawDataBuffer( const std::string & name )
{
    return localInterface.getBPMRawDataBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorController::getBPMXPVBuffer( const std::string & name )
{
    return localInterface.getBPMXPVBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorController::getBPMYPVBuffer( const std::string & name )
{
    return localInterface.getBPMYPVBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorController::getBPMXBuffer( const std::string & name )
{
    return localInterface.getBPMXBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorController::getBPMYBuffer( const std::string & name )
{
    return localInterface.getBPMYBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorController::getBPMQBuffer( const std::string & name )
{
    return localInterface.getBPMQBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorController::getTimeStampsBuffer( const std::string & name )
{
    return localInterface.getTimeStampsBuffer( name );
}
//______________________________________________________________________________
std::vector< std::string > beamPositionMonitorController::getStrTimeStamps( const std::string & name )
{
    return localInterface.getStrTimeStamps( name );
}
//______________________________________________________________________________
std::vector< std::string > beamPositionMonitorController::getBPMNames()
{
    return localInterface.getBPMNames();
}
//______________________________________________________________________________
const bool beamPositionMonitorController::isDataBufferFull( const std::string & name )
{
    return localInterface.isDataBufferFull( name );
}
//______________________________________________________________________________
const bool beamPositionMonitorController::isDataBufferNotFull( const std::string & name )
{
    return localInterface.isDataBufferNotFull( name );
}
//______________________________________________________________________________
const bool beamPositionMonitorController::isXBufferFull( const std::string & name )
{
    return localInterface.isXBufferFull( name );
}
//______________________________________________________________________________
const bool beamPositionMonitorController::isYBufferFull( const std::string & name )
{
    return localInterface.isYBufferFull( name );
}
//______________________________________________________________________________
const bool beamPositionMonitorController::isXBufferNotFull( const std::string & name )
{
    return localInterface.isXBufferNotFull( name );
}
//______________________________________________________________________________
const bool beamPositionMonitorController::isYBufferNotFull( const std::string & name )
{
    return localInterface.isYBufferNotFull( name );
}
//______________________________________________________________________________
void beamPositionMonitorController::monitorDataForNShots( size_t N, const std::string & name )
{
    localInterface.monitorDataForNShots( N, name );
}
//______________________________________________________________________________
void beamPositionMonitorController::monitorDataForNShots( size_t N, const std::vector< std::string > & names )
{
    localInterface.monitorDataForNShots( N, names );
}
//______________________________________________________________________________
void beamPositionMonitorController::reCalAttenuation( const std::string & name, double qScope )
{
    localInterface.reCalAttenuation( name, qScope );
}
//______________________________________________________________________________
double beamPositionMonitorController::getX( const std::string & name )
{
    return localInterface.getX( name );
}
//______________________________________________________________________________
double beamPositionMonitorController::getY( const std::string & name )
{
    return localInterface.getY( name );
}
//______________________________________________________________________________
double beamPositionMonitorController::getQ( const std::string & name )
{
    return localInterface.getQ( name );
}
//______________________________________________________________________________
double beamPositionMonitorController::getPosition( const std::string & name )
{
    return localInterface.getPosition( name );
}
//______________________________________________________________________________
double beamPositionMonitorController::getXFromPV( const std::string & name )
{
    return localInterface.getXFromPV( name );
}
//______________________________________________________________________________
double beamPositionMonitorController::getYFromPV( const std::string & name )
{
    return localInterface.getYFromPV( name );
}
//______________________________________________________________________________
double beamPositionMonitorController::getBPMResolution( const std::string & name )
{
    return localInterface.getBPMResolution( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getRA1( const std::string & name )
{
    return localInterface.getRA1( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getRA2( const std::string & name )
{
    return localInterface.getRA2( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getRD1( const std::string & name )
{
    return localInterface.getRD1( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getRD2( const std::string & name )
{
    return localInterface.getRD2( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getSA1( const std::string & name )
{
    return localInterface.getSA1( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getSA2( const std::string & name )
{
    return localInterface.getSA2( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getSD1( const std::string & name )
{
    return localInterface.getSD1( name );
}
//______________________________________________________________________________
long beamPositionMonitorController::getSD2( const std::string & name )
{
    return localInterface.getSD2( name );
}
//______________________________________________________________________________
void beamPositionMonitorController::setSA1( const std::string & bpmName, long sa1 )
{
    localInterface.setSA1( bpmName, sa1 );
}
//______________________________________________________________________________
void beamPositionMonitorController::setSA2( const std::string & bpmName, long sa2 )
{
    localInterface.setSA2( bpmName, sa2 );
}
//______________________________________________________________________________
void beamPositionMonitorController::setSD1( const std::string & bpmName, long sd1 )
{
    localInterface.setSD1( bpmName, sd1 );
}
//______________________________________________________________________________
void beamPositionMonitorController::setSD2( const std::string & bpmName, long sd2 )
{
    localInterface.setSD2( bpmName, sd2 );
}
//______________________________________________________________________________
void beamPositionMonitorController::setX( const std::string & bpmName, double val )
{
    localInterface.setX( bpmName, val );
}
//______________________________________________________________________________
void beamPositionMonitorController::setBufferSize( size_t buffer )
{
    localInterface.setBufferSize( buffer );
}
//______________________________________________________________________________
void beamPositionMonitorController::clearBuffers()
{
    localInterface.clearBuffers();
}
//______________________________________________________________________________
void beamPositionMonitorController::setY( const std::string & bpmName, double val )
{
    localInterface.setY( bpmName, val );
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list beamPositionMonitorController::getBPMNames_Py()
{
    return toPythonList(getBPMNames());
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMRawData_Py( const std::string & name )
{
    return toPythonList(getBPMRawData( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMXVec_Py( const std::string & name )
{
    return toPythonList(getBPMXVec( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMYVec_Py( const std::string & name )
{
    return toPythonList(getBPMYVec( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMQVec_Py( const std::string & name )
{
    return toPythonList(getBPMQVec( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getTimeStamps_Py( const std::string & name )
{
    return toPythonList(getTimeStamps( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getStrTimeStamps_Py( const std::string & name )
{
    return toPythonList(getStrTimeStamps( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMRawDataBuffer_Py( const std::string & name )
{
    return toPythonList(getBPMRawDataBuffer( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMXBuffer_Py( const std::string & name )
{
    return toPythonList(getBPMXBuffer( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMYBuffer_Py( const std::string & name )
{
    return toPythonList(getBPMYBuffer( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMXPVBuffer_Py( const std::string & name )
{
    return toPythonList(getBPMXPVBuffer( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMYPVBuffer_Py( const std::string & name )
{
    return toPythonList(getBPMYPVBuffer( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getBPMQBuffer_Py( const std::string & name )
{
    return toPythonList(getBPMQBuffer( name ));
}
//______________________________________________________________________________
boost::python::list beamPositionMonitorController::getTimeStampsBuffer_Py( const std::string & name )
{
    return toPythonList(getTimeStampsBuffer( name ));
}
#endif
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > beamPositionMonitorController::getILockStates( const std::string & objName )const
{
    return localInterface.getILockStates( objName );
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string > beamPositionMonitorController::getILockStatesStr( const std::string & objName )const
{
    return localInterface.getILockStatesStr( objName );
}
//______________________________________________________________________________
double beamPositionMonitorController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void beamPositionMonitorController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
