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

#include "blmController.h"
//______________________________________________________________________________
blmController::blmController( const std::string &configFileLocation1,
                                  bool& show_messages,
                                  bool& show_debug_messages,
                                  const bool shouldStartEPICs,
                                  const bool startVirtualMachine,
                                  HWC_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::SCOPE ),
localInterface( configFileLocation1, show_messages, show_debug_messages, shouldStartEPICs, startVirtualMachine, myMachineArea ),
shouldStartEPICs( shouldStartEPICs ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
void blmController::initialise()
{
    message("blmController instantiation success.");
//    if( localInterface.interfaceInitReport( shouldStartEPICs ) )
//        message("blmController instantiation success.");

}
//______________________________________________________________________________
blmController::~blmController(){}    //dtor
//______________________________________________________________________________
//std::vector< std::vector< double > > blmController::monitorForNCounts( const std::string & blmName, int numcounts )
//{
//    return localInterface.monitorForNCounts( blmName, numcounts );
//}
//______________________________________________________________________________
void blmController::monitorTracesForNShots( size_t N )
{
    localInterface.monitorTracesForNShots( N );
}
//______________________________________________________________________________
void blmController::monitorATraceForNShots( const std::string trace, blmStructs::SCOPE_PV_TYPE channel, size_t N )
{
    localInterface.monitorATraceForNShots( trace, channel, N );
}
//______________________________________________________________________________
void blmController::monitorNumsForNShots( size_t N )
{
    localInterface.monitorNumsForNShots( N );
}
//______________________________________________________________________________
void blmController::monitorANumForNShots( const std::string num, blmStructs::SCOPE_PV_TYPE channel, size_t N )
{
    localInterface.monitorANumForNShots( num, channel, N );
}
//______________________________________________________________________________
bool blmController::isMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isMonitoringBLMTrace( blmName, pvType );
}
//______________________________________________________________________________
bool blmController::isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringBLMTrace( blmName, pvType );
}
//______________________________________________________________________________
bool blmController::isMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isMonitoringBLMNum( blmName, pvType );
}
//______________________________________________________________________________
bool blmController::isNotMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringBLMNum( blmName, pvType );
}
//______________________________________________________________________________
const blmStructs::blmTraceData & blmController::getBLMTraceDataStruct( const std::string & blmName )
{
    return localInterface.getBLMTraceDataStruct( blmName );
}
//______________________________________________________________________________
const blmStructs::blmNumObject & blmController::getBLMNumDataStruct( const std::string & blmName )
{
    return localInterface.getBLMNumDataStruct( blmName );
}
//______________________________________________________________________________
std::vector< std::vector< double > > blmController::getBLMTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getBLMTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMNums( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getBLMNums( name, pvType );
}
//______________________________________________________________________________
std::vector< std::vector< double > > blmController::getPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > blmController::getAreaUnderPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAreaUnderPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMP1Vec( const std::string & name )
{
    return localInterface.getBLMP1Vec( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMP2Vec( const std::string & name )
{
    return localInterface.getBLMP2Vec( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMP3Vec( const std::string & name )
{
    return localInterface.getBLMP3Vec( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMP4Vec( const std::string & name )
{
    return localInterface.getBLMP4Vec( name );
}
//______________________________________________________________________________
void blmController::setBufferSize( size_t bufferSize )
{
    localInterface.setBufferSize( bufferSize );
}
//______________________________________________________________________________
void blmController::setNumBufferSize( size_t bufferSize )
{
    localInterface.setNumBufferSize( bufferSize );
}
//______________________________________________________________________________
void blmController::setTraceBufferSize( size_t bufferSize )
{
    localInterface.setTraceBufferSize( bufferSize );
}
//______________________________________________________________________________
void blmController::restartContinuousMonitoring()
{
    localInterface.restartContinuousMonitoring();
}
//______________________________________________________________________________
void blmController::setTimebase( const std::string & name, const double timebase )
{
    localInterface.setTimebase( name, timebase );
}
//______________________________________________________________________________
double blmController::getTimebase( const std::string & name )
{
    return localInterface.getTimebase( name );
}
//______________________________________________________________________________
size_t blmController::getBufferSize( const std::string & name )
{
    return localInterface.getBufferSize( name );
}
//______________________________________________________________________________
const blmStructs::DIAG_TYPE blmController::getDiagType( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getDiagType( blmName, pvType );
}
//______________________________________________________________________________
const std::string blmController::getDiagTypeStr( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getDiagTypeStr( blmName, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > blmController::getBLMNumBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getBLMNumBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > blmController::getBLMP1Buffer( const std::string & name )
{
    return localInterface.getBLMP1Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > blmController::getBLMP2Buffer( const std::string & name )
{
    return localInterface.getBLMP2Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > blmController::getBLMP3Buffer( const std::string & name )
{
    return localInterface.getBLMP3Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< double > blmController::getBLMP4Buffer( const std::string & name )
{
    return localInterface.getBLMP4Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMTraceBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getBLMTraceBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMTR1Buffer( const std::string & name )
{
    return localInterface.getBLMTR1Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMTR2Buffer( const std::string & name )
{
    return localInterface.getBLMTR2Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMTR3Buffer( const std::string & name )
{
    return localInterface.getBLMTR3Buffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMTR4Buffer( const std::string & name )
{
    return localInterface.getBLMTR4Buffer( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getMinOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getMinOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getMaxOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getMaxOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getAreaUnderTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getAreaUnderTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< std::string > blmController::getStrTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return localInterface.getStrTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getAvgNoise( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAvgNoise( name, pvType, part1, part2 );
}
//______________________________________________________________________________
double blmController::getBLMP1( const std::string & name )
{
    return localInterface.getBLMP1( name );
}
//______________________________________________________________________________
double blmController::getBLMP2( const std::string & name )
{
    return localInterface.getBLMP2( name );
}
//______________________________________________________________________________
double blmController::getBLMP3( const std::string & name )
{
    return localInterface.getBLMP3( name );
}
//______________________________________________________________________________
double blmController::getBLMP4( const std::string & name )
{
    return localInterface.getBLMP4( name );
}
//______________________________________________________________________________
double blmController::getWCMQ()
{
    return localInterface.getWCMQ();
}
//______________________________________________________________________________
double blmController::getICT1Q( const int part1, const int part2 )
{
    return localInterface.getICT1Q( part1, part2 );
}
//______________________________________________________________________________
double blmController::getICT2Q( const int part1, const int part2 )
{
    return localInterface.getICT2Q( part1, part2 );
}
//______________________________________________________________________________
double blmController::getFCUPQ()
{
    return localInterface.getFCUPQ();
}
//______________________________________________________________________________
double blmController::getEDFCUPQ()
{
    return localInterface.getEDFCUPQ();
}
//______________________________________________________________________________
std::vector< std::string > blmController::getBLMNames()
{
    return localInterface.getBLMNames();
}
//______________________________________________________________________________
std::vector< std::string > blmController::getBLMPVs()
{
    return localInterface.getBLMPVs();
}
//______________________________________________________________________________
std::vector< std::string > blmController::getBLMTracePVs()
{
    return localInterface.getBLMTracePVs();
}
//______________________________________________________________________________
std::vector< std::string > blmController::getBLMNumPVs()
{
    return localInterface.getBLMNumPVs();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list blmController::getBLMP1Vec_Py( const std::string & name )
{
    return toPythonList(getBLMP1Vec( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP2Vec_Py( const std::string & name )
{
    return toPythonList(getBLMP2Vec( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP3Vec_Py( const std::string & name )
{
    return toPythonList(getBLMP3Vec( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP4Vec_Py( const std::string & name )
{
    return toPythonList(getBLMP4Vec( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getMinOfTraces_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getMinOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getMaxOfTraces_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getMaxOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getAreaUnderTraces_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getAreaUnderTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getTimeStamps_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getStrTimeStamps_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getStrTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getAvgNoise_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAvgNoise( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMNumBuffer_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getBLMNumBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP1Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMP1Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP2Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMP2Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP3Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMP3Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMP4Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMP4Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTraceBuffer_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getBLMTraceBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTR1Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMTR1Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTR2Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMTR2Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTR3Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMTR3Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTR4Buffer_Py( const std::string & name )
{
    return toPythonList(getBLMTR4Buffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMNums_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return toPythonList(getBLMNums( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getAreaUnderPartOfTrace_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAreaUnderPartOfTrace( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMNames_Py()
{
    return toPythonList(getBLMNames());
}
//______________________________________________________________________________
boost::python::list blmController::getBLMPVs_Py()
{
    return toPythonList(getBLMPVs());
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTracePVs_Py()
{
    return toPythonList(getBLMTracePVs());
}
//______________________________________________________________________________
boost::python::list blmController::getBLMNumPVs_Py()
{
    return toPythonList(getBLMNumPVs());
}
#endif
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,std::string> blmController::getILockStatesStr(const std::string& name)const
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> blmController::getILockStates(const std::string& name)const
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________
double blmController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void blmController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
