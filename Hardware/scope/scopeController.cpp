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
//______________________________________________________________________________
scopeController::scopeController( const std::string &configFileLocation1,
                                  const std::string &configFileLocation2,
                                  bool& show_messages,
                                  bool& show_debug_messages,
                                  const bool shouldStartEPICs,
                                  const bool startVirtualMachine,
                                  HWC_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::SCOPE ),
localInterface( configFileLocation1, configFileLocation2, show_messages, show_debug_messages, shouldStartEPICs, startVirtualMachine, myMachineArea ),
shouldStartEPICs( shouldStartEPICs ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
void scopeController::initialise()
{
    if( localInterface.interfaceInitReport( shouldStartEPICs ) )
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
void scopeController::monitorANumForNShots( const std::string num, scopeStructs::SCOPE_PV_TYPE channel, size_t N )
{
    localInterface.monitorANumForNShots( num, channel, N );
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
void scopeController::setBufferSize( size_t bufferSize )
{
    localInterface.setBufferSize( bufferSize );
}
//______________________________________________________________________________
void scopeController::setNumBufferSize( size_t bufferSize )
{
    localInterface.setNumBufferSize( bufferSize );
}
//______________________________________________________________________________
void scopeController::setTraceBufferSize( size_t bufferSize )
{
    localInterface.setTraceBufferSize( bufferSize );
}
//______________________________________________________________________________
void scopeController::restartContinuousMonitoring()
{
    localInterface.restartContinuousMonitoring();
}
//______________________________________________________________________________
void scopeController::setTimebase( const std::string & name, const double timebase )
{
    localInterface.setTimebase( name, timebase );
}
//______________________________________________________________________________
double scopeController::getTimebase( const std::string & name )
{
    return localInterface.getTimebase( name );
}
//______________________________________________________________________________
size_t scopeController::getBufferSize( const std::string & name )
{
    return localInterface.getBufferSize( name );
}
//______________________________________________________________________________
const scopeStructs::DIAG_TYPE scopeController::getDiagType( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getDiagType( scopeName, pvType );
}
//______________________________________________________________________________
const std::string scopeController::getDiagTypeStr( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getDiagTypeStr( scopeName, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > scopeController::getScopeNumBuffer( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getScopeNumBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > scopeController::getScopeP1Buffer( const std::string & name )
{
    return localInterface.getScopeP1Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > scopeController::getScopeP2Buffer( const std::string & name )
{
    return localInterface.getScopeP2Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > scopeController::getScopeP3Buffer( const std::string & name )
{
    return localInterface.getScopeP3Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > scopeController::getScopeP4Buffer( const std::string & name )
{
    return localInterface.getScopeP4Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > scopeController::getScopeTraceBuffer( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getScopeTraceBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > scopeController::getScopeTR1Buffer( const std::string & name )
{
    return localInterface.getScopeTR1Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > scopeController::getScopeTR2Buffer( const std::string & name )
{
    return localInterface.getScopeTR2Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > scopeController::getScopeTR3Buffer( const std::string & name )
{
    return localInterface.getScopeTR3Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > scopeController::getScopeTR4Buffer( const std::string & name )
{
    return localInterface.getScopeTR4Buffer( name );
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
#ifdef BUILD_DLL
boost::python::list scopeController::getScopeP1Vec_Py( const std::string & name )
{
    return toPythonList(getScopeP1Vec( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP2Vec_Py( const std::string & name )
{
    return toPythonList(getScopeP2Vec( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP3Vec_Py( const std::string & name )
{
    return toPythonList(getScopeP3Vec( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP4Vec_Py( const std::string & name )
{
    return toPythonList(getScopeP4Vec( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getMinOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getMinOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getMaxOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getMaxOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getAreaUnderTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getAreaUnderTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getStrTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getStrTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getAvgNoise_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAvgNoise( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeNumBuffer_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getScopeNumBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP1Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeP1Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP2Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeP2Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP3Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeP3Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeP4Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeP4Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeTraceBuffer_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getScopeTraceBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeTR1Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeTR1Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeTR2Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeTR2Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeTR3Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeTR3Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeTR4Buffer_Py( const std::string & name )
{
    return toPythonList(getScopeTR4Buffer( name ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeNums_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getScopeNums( name, pvType ));
}
//______________________________________________________________________________
boost::python::list scopeController::getAreaUnderPartOfTrace_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAreaUnderPartOfTrace( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeNames_Py()
{
    return toPythonList(getScopeNames());
}
//______________________________________________________________________________
boost::python::list scopeController::getScopePVs_Py()
{
    return toPythonList(getScopePVs());
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeTracePVs_Py()
{
    return toPythonList(getScopeTracePVs());
}
//______________________________________________________________________________
boost::python::list scopeController::getScopeNumPVs_Py()
{
    return toPythonList(getScopeNumPVs());
}
#endif
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,std::string> scopeController::getILockStatesStr(const std::string& name)const
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> scopeController::getILockStates(const std::string& name)const
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________
double scopeController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void scopeController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
