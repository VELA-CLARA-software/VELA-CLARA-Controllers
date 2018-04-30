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
                                  const std::string &configFileLocation2,
                                  bool& show_messages,
                                  bool& show_debug_messages,
                                  const bool shouldStartEPICs,
                                  const bool startVirtualMachine,
                                  HWC_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::charge ),
localInterface( configFileLocation1, configFileLocation2, show_messages, show_debug_messages, shouldStartEPICs, startVirtualMachine, myMachineArea ),
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
void chargeController::monitorTracesForNShots( size_t N )
{
    localInterface.monitorTracesForNShots( N );
}
//______________________________________________________________________________
void chargeController::monitorATraceForNShots( const std::string trace, chargeStructs::charge_PV_TYPE channel, size_t N )
{
    localInterface.monitorATraceForNShots( trace, channel, N );
}
//______________________________________________________________________________
void chargeController::monitorNumsForNShots( size_t N )
{
    localInterface.monitorNumsForNShots( N );
}
//______________________________________________________________________________
void chargeController::monitorANumForNShots( const std::string num, chargeStructs::charge_PV_TYPE channel, size_t N )
{
    localInterface.monitorANumForNShots( num, channel, N );
}
//______________________________________________________________________________
bool chargeController::isMonitoringchargeTrace( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.isMonitoringchargeTrace( chargeName, pvType );
}
//______________________________________________________________________________
bool chargeController::isNotMonitoringchargeTrace( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringchargeTrace( chargeName, pvType );
}
//______________________________________________________________________________
bool chargeController::isMonitoringchargeNum( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.isMonitoringchargeNum( chargeName, pvType );
}
//______________________________________________________________________________
bool chargeController::isNotMonitoringchargeNum( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringchargeNum( chargeName, pvType );
}
//______________________________________________________________________________
const chargeStructs::chargeTraceData & chargeController::getchargeTraceDataStruct( const std::string & chargeName )
{
    return localInterface.getchargeTraceDataStruct( chargeName );
}
//______________________________________________________________________________
const chargeStructs::chargeNumObject & chargeController::getchargeNumDataStruct( const std::string & chargeName )
{
    return localInterface.getchargeNumDataStruct( chargeName );
}
//______________________________________________________________________________
std::vector< std::vector< double > > chargeController::getchargeTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getchargeTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > chargeController::getchargeNums( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getchargeNums( name, pvType );
}
//______________________________________________________________________________
std::vector< std::vector< double > > chargeController::getPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > chargeController::getAreaUnderPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAreaUnderPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > chargeController::getchargeP1Vec( const std::string & name )
{
    return localInterface.getchargeP1Vec( name );
}
//______________________________________________________________________________
std::vector< double > chargeController::getchargeP2Vec( const std::string & name )
{
    return localInterface.getchargeP2Vec( name );
}
//______________________________________________________________________________
std::vector< double > chargeController::getchargeP3Vec( const std::string & name )
{
    return localInterface.getchargeP3Vec( name );
}
//______________________________________________________________________________
std::vector< double > chargeController::getchargeP4Vec( const std::string & name )
{
    return localInterface.getchargeP4Vec( name );
}
//______________________________________________________________________________
void chargeController::setBufferSize( size_t bufferSize )
{
    localInterface.setBufferSize( bufferSize );
}
//______________________________________________________________________________
void chargeController::setNumBufferSize( size_t bufferSize )
{
    localInterface.setNumBufferSize( bufferSize );
}
//______________________________________________________________________________
void chargeController::setTraceBufferSize( size_t bufferSize )
{
    localInterface.setTraceBufferSize( bufferSize );
}
//______________________________________________________________________________
void chargeController::restartContinuousMonitoring()
{
    localInterface.restartContinuousMonitoring();
}
//______________________________________________________________________________
void chargeController::setTimebase( const std::string & name, const double timebase )
{
    localInterface.setTimebase( name, timebase );
}
//______________________________________________________________________________
double chargeController::getTimebase( const std::string & name )
{
    return localInterface.getTimebase( name );
}
//______________________________________________________________________________
size_t chargeController::getBufferSize( const std::string & name )
{
    return localInterface.getBufferSize( name );
}
//______________________________________________________________________________
const chargeStructs::DIAG_TYPE chargeController::getDiagType( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getDiagType( chargeName, pvType );
}
//______________________________________________________________________________
const std::string chargeController::getDiagTypeStr( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getDiagTypeStr( chargeName, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeController::getchargeNumBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getchargeNumBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeController::getchargeP1Buffer( const std::string & name )
{
    return localInterface.getchargeP1Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeController::getchargeP2Buffer( const std::string & name )
{
    return localInterface.getchargeP2Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeController::getchargeP3Buffer( const std::string & name )
{
    return localInterface.getchargeP3Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeController::getchargeP4Buffer( const std::string & name )
{
    return localInterface.getchargeP4Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > chargeController::getchargeTraceBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getchargeTraceBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > chargeController::getchargeTR1Buffer( const std::string & name )
{
    return localInterface.getchargeTR1Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > chargeController::getchargeTR2Buffer( const std::string & name )
{
    return localInterface.getchargeTR2Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > chargeController::getchargeTR3Buffer( const std::string & name )
{
    return localInterface.getchargeTR3Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > chargeController::getchargeTR4Buffer( const std::string & name )
{
    return localInterface.getchargeTR4Buffer( name );
}
//______________________________________________________________________________
std::vector< double > chargeController::getMinOfTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getMinOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > chargeController::getMaxOfTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getMaxOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > chargeController::getAreaUnderTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getAreaUnderTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > chargeController::getTimeStamps( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< std::string > chargeController::getStrTimeStamps( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return localInterface.getStrTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< double > chargeController::getAvgNoise( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAvgNoise( name, pvType, part1, part2 );
}
//______________________________________________________________________________
double chargeController::getchargeP1( const std::string & name )
{
    return localInterface.getchargeP1( name );
}
//______________________________________________________________________________
double chargeController::getchargeP2( const std::string & name )
{
    return localInterface.getchargeP2( name );
}
//______________________________________________________________________________
double chargeController::getchargeP3( const std::string & name )
{
    return localInterface.getchargeP3( name );
}
//______________________________________________________________________________
double chargeController::getchargeP4( const std::string & name )
{
    return localInterface.getchargeP4( name );
}
//______________________________________________________________________________
double chargeController::getWCMQ()
{
    return localInterface.getWCMQ();
}
//______________________________________________________________________________
double chargeController::getICT1Q( const int part1, const int part2 )
{
    return localInterface.getICT1Q( part1, part2 );
}
//______________________________________________________________________________
double chargeController::getICT2Q( const int part1, const int part2 )
{
    return localInterface.getICT2Q( part1, part2 );
}
//______________________________________________________________________________
double chargeController::getFCUPQ()
{
    return localInterface.getFCUPQ();
}
//______________________________________________________________________________
double chargeController::getEDFCUPQ()
{
    return localInterface.getEDFCUPQ();
}
//______________________________________________________________________________
std::vector< std::string > chargeController::getchargeNames()
{
    return localInterface.getchargeNames();
}
//______________________________________________________________________________
std::vector< std::string > chargeController::getchargePVs()
{
    return localInterface.getchargePVs();
}
//______________________________________________________________________________
std::vector< std::string > chargeController::getchargeTracePVs()
{
    return localInterface.getchargeTracePVs();
}
//______________________________________________________________________________
std::vector< std::string > chargeController::getchargeNumPVs()
{
    return localInterface.getchargeNumPVs();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list chargeController::getchargeP1Vec_Py( const std::string & name )
{
    return toPythonList(getchargeP1Vec( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP2Vec_Py( const std::string & name )
{
    return toPythonList(getchargeP2Vec( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP3Vec_Py( const std::string & name )
{
    return toPythonList(getchargeP3Vec( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP4Vec_Py( const std::string & name )
{
    return toPythonList(getchargeP4Vec( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getMinOfTraces_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getMinOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getMaxOfTraces_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getMaxOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getAreaUnderTraces_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getAreaUnderTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getTimeStamps_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getStrTimeStamps_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getStrTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getAvgNoise_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAvgNoise( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeNumBuffer_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getchargeNumBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP1Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeP1Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP2Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeP2Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP3Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeP3Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeP4Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeP4Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeTraceBuffer_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getchargeTraceBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeTR1Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeTR1Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeTR2Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeTR2Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeTR3Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeTR3Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeTR4Buffer_Py( const std::string & name )
{
    return toPythonList(getchargeTR4Buffer( name ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeNums_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    return toPythonList(getchargeNums( name, pvType ));
}
//______________________________________________________________________________
boost::python::list chargeController::getAreaUnderPartOfTrace_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAreaUnderPartOfTrace( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeNames_Py()
{
    return toPythonList(getchargeNames());
}
//______________________________________________________________________________
boost::python::list chargeController::getchargePVs_Py()
{
    return toPythonList(getchargePVs());
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeTracePVs_Py()
{
    return toPythonList(getchargeTracePVs());
}
//______________________________________________________________________________
boost::python::list chargeController::getchargeNumPVs_Py()
{
    return toPythonList(getchargeNumPVs());
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
