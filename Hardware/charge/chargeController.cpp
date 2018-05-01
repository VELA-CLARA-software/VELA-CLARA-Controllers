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

#include "chargeController.h"
//______________________________________________________________________________
chargeController::chargeController( const std::string &configFileLocation1,
                                    bool& show_messages,
                                    bool& show_debug_messages,
                                    const bool shouldStartEPICs,
                                    const bool startVirtualMachine,
                                    HWC_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::CHARGE ),
localInterface( configFileLocation1, show_messages, show_debug_messages, shouldStartEPICs, startVirtualMachine, myMachineArea ),
shouldStartEPICs( shouldStartEPICs ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
void chargeController::initialise()
{
    message("chargeController instantiation success.");
//    if( localInterface.interfaceInitReport( shouldStartEPICs ) )
//        message("chargeController instantiation success.");

}
//______________________________________________________________________________
chargeController::~chargeController(){}    //dtor
//______________________________________________________________________________
//std::vector< std::vector< double > > chargeController::monitorForNCounts( const std::string & chargeName, int numcounts )
//{
//    return localInterface.monitorForNCounts( chargeName, numcounts );
//}
//______________________________________________________________________________
void chargeController::monitorForNShots_Vec( const std::vector< std::string > charges, size_t N )
{
    localInterface.monitorForNShots( charges, N );
}
//______________________________________________________________________________
void chargeController::monitorForNShots( const std::string charge, size_t N )
{
    localInterface.monitorForNShots( charge, N );
}
//______________________________________________________________________________
void chargeController::setBufferSize( size_t N )
{
    localInterface.setBufferSize( N );
}
//______________________________________________________________________________
void chargeController::restartContinuousMonitoring()
{
    localInterface.restartContinuousMonitoring();
}
//______________________________________________________________________________
void chargeController::cancelDataMonitors()
{
    return localInterface.cancelDataMonitors();
}
//______________________________________________________________________________
size_t chargeController::getBufferSize( const std::string & name )
{
    return localInterface.getBufferSize( name );
}
//______________________________________________________________________________
const chargeStructs::CHARGE_DIAG_TYPE chargeController::getDiagType( const std::string & chargeName )
{
    return localInterface.getDiagType( chargeName );
}
//______________________________________________________________________________
const std::string chargeController::getDiagTypeStr( const std::string & chargeName )
{
    return localInterface.getDiagTypeStr( chargeName );
}
//______________________________________________________________________________
const chargeStructs::dataObject & chargeController::getChargeDataStruct( const std::string & chargeName )
{
    return localInterface.getChargeDataStruct( chargeName );
}
//______________________________________________________________________________
const std::vector< double > chargeController::getChargeVector( const std::string & name )
{
    return localInterface.getChargeVector( name );
}
//______________________________________________________________________________
const std::vector< double > chargeController::getVoltageVector( const std::string & name )
{
    return localInterface.getVoltageVector( name );
}
//______________________________________________________________________________
const std::vector< double > chargeController::getWCMChargeVector()
{
    return localInterface.getWCMChargeVector();
}
//______________________________________________________________________________
const std::vector< double > chargeController::getS02FCUPChargeVector()
{
    return localInterface.getS02FCUPChargeVector();
}
//______________________________________________________________________________
const std::vector< double > chargeController::getWCMVoltageVector()
{
    return localInterface.getWCMVoltageVector();
}
//______________________________________________________________________________
const std::vector< double > chargeController::getS02FCUPVoltageVector()
{
    return localInterface.getS02FCUPVoltageVector();
}
//______________________________________________________________________________
const boost::circular_buffer< double > chargeController::getChargeBuffer( const std::string & name )
{
    return localInterface.getChargeBuffer( name );
}
//______________________________________________________________________________
const boost::circular_buffer< double > chargeController::getVoltageBuffer( const std::string & name )
{
    return localInterface.getVoltageBuffer( name );
}
//______________________________________________________________________________
const boost::circular_buffer< double > chargeController::getWCMChargeBuffer()
{
    return localInterface.getWCMChargeBuffer();
}
//______________________________________________________________________________
const boost::circular_buffer< double > chargeController::getS02FCUPChargeBuffer()
{
    return localInterface.getS02FCUPChargeBuffer();
}
//______________________________________________________________________________
const std::vector< double > chargeController::getTimeStamps( const std::string & name )
{
    return localInterface.getTimeStamps( name );
}
//______________________________________________________________________________
const std::vector< std::string > chargeController::getStrTimeStamps( const std::string & name )
{
    return localInterface.getStrTimeStamps( name );
}
//______________________________________________________________________________
const std::vector< std::string > chargeController::getChargeDiagnosticNames()
{
    return localInterface.getChargeDiagnosticNames();
}
//______________________________________________________________________________
const std::vector< std::string > chargeController::getChargePVs()
{
    return localInterface.getChargePVs();
}
//______________________________________________________________________________
double chargeController::getCharge( const std::string & name )
{
    return localInterface.getCharge( name );
}
//______________________________________________________________________________
double chargeController::getVoltage( const std::string & name )
{
    return localInterface.getVoltage( name );
}
//______________________________________________________________________________
double chargeController::getWCMCharge()
{
    return localInterface.getWCMCharge();
}
//______________________________________________________________________________
double chargeController::getS02FCUPCharge()
{
    return localInterface.getS02FCUPCharge();
}
//______________________________________________________________________________
double chargeController::getWCMVoltage()
{
    return localInterface.getWCMVoltage();
}
//______________________________________________________________________________
double chargeController::getS02FCUPVoltage()
{
    return localInterface.getS02FCUPVoltage();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list chargeController::getChargeVector_Py( const std::string & name )
{
    return toPythonList(getChargeVector( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getVoltageVector_Py( const std::string & name )
{
    return toPythonList(getVoltageVector( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getWCMChargeVector_Py()
{
    return toPythonList(getWCMChargeVector());
}
//______________________________________________________________________________
boost::python::list chargeController::getS02FCUPChargeVector_Py()
{
    return toPythonList(getS02FCUPChargeVector());
}
//______________________________________________________________________________
boost::python::list chargeController::getWCMVoltageVector_Py()
{
    return toPythonList(getWCMVoltageVector());
}
//______________________________________________________________________________
boost::python::list chargeController::getS02FCUPVoltageVector_Py()
{
    return toPythonList(getS02FCUPVoltageVector());
}
//______________________________________________________________________________
boost::python::list chargeController::getChargeBuffer_Py( const std::string & name )
{
    return toPythonList(getChargeBuffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getWCMChargeBuffer_Py()
{
    return toPythonList(getWCMChargeBuffer());
}
//______________________________________________________________________________
boost::python::list chargeController::getS02FCUPChargeBuffer_Py()
{
    return toPythonList(getS02FCUPChargeBuffer());
}
//______________________________________________________________________________
boost::python::list chargeController::getVoltageBuffer_Py( const std::string & name )
{
    return toPythonList(getVoltageBuffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getWCMVoltageBuffer_Py()
{
    return toPythonList(getWCMVoltageBuffer());
}
//______________________________________________________________________________
boost::python::list chargeController::getS02FCUPVoltageBuffer_Py()
{
    return toPythonList(getS02FCUPVoltageBuffer());
}
//______________________________________________________________________________
boost::python::list chargeController::getTimeStamps_Py( const std::string & name )
{
    return toPythonList(getTimeStamps( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getStrTimeStamps_Py( const std::string & name )
{
    return toPythonList(getStrTimeStamps( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getChargeDiagnosticNames_Py()
{
    return toPythonList(getChargeDiagnosticNames());
}
//______________________________________________________________________________
boost::python::list chargeController::getChargePVs_Py()
{
    return toPythonList(getChargePVs());
}
#endif
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,std::string> chargeController::getILockStatesStr(const std::string& name)const
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> chargeController::getILockStates(const std::string& name)const
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________
double chargeController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void chargeController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
