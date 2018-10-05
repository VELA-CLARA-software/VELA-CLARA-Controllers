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
void blmController::monitorATraceForNShots( const std::string trace, blmStructs::BLM_PV_TYPE channel, size_t N )
{
    localInterface.monitorATraceForNShots( trace, channel, N );
}
//______________________________________________________________________________
bool blmController::isMonitoringBLMTrace( const std::string & blmName, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.isMonitoringBLMTrace( blmName, pvType );
}
//______________________________________________________________________________
bool blmController::isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.isNotMonitoringBLMTrace( blmName, pvType );
}
//______________________________________________________________________________
const blmStructs::blmTraceData & blmController::getBLMTraceDataStruct( const std::string & blmName )
{
    return localInterface.getBLMTraceDataStruct( blmName );
}
//______________________________________________________________________________
std::vector< std::vector< double > > blmController::getBLMTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getBLMTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getBLMTrace( name, pvType );
}
//______________________________________________________________________________
std::vector< std::vector< double > > blmController::getPartOfTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
std::vector< double > blmController::getAreaUnderPartOfTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAreaUnderPartOfTrace( name, pvType, part1, part2 );
}
//______________________________________________________________________________
void blmController::setBufferSize( size_t bufferSize )
{
    localInterface.setBufferSize( bufferSize );
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
std::vector< double > blmController::getBLMCH1Waveform( const std::string & name )
{
    return localInterface.getBLMCH1Waveform( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH2Waveform( const std::string & name )
{
    return localInterface.getBLMCH2Waveform( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH3Waveform( const std::string & name )
{
    return localInterface.getBLMCH3Waveform( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH4Waveform( const std::string & name )
{
    return localInterface.getBLMCH4Waveform( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH1Time( const std::string & name )
{
    return localInterface.getBLMCH1Time( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH2Time( const std::string & name )
{
    return localInterface.getBLMCH2Time( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH3Time( const std::string & name )
{
    return localInterface.getBLMCH3Time( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getBLMCH4Time( const std::string & name )
{
    return localInterface.getBLMCH4Time( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMTraceBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getBLMTraceBuffer( name, pvType );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH1WaveformBuffer( const std::string & name )
{
    return localInterface.getBLMCH1WaveformBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH2WaveformBuffer( const std::string & name )
{
    return localInterface.getBLMCH2WaveformBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH3WaveformBuffer( const std::string & name )
{
    return localInterface.getBLMCH3WaveformBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH4WaveformBuffer( const std::string & name )
{
    return localInterface.getBLMCH4WaveformBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH1TimeBuffer( const std::string & name )
{
    return localInterface.getBLMCH1TimeBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH2TimeBuffer( const std::string & name )
{
    return localInterface.getBLMCH2TimeBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH3TimeBuffer( const std::string & name )
{
    return localInterface.getBLMCH3TimeBuffer( name );
}
//______________________________________________________________________________
boost::circular_buffer< std::vector < double > > blmController::getBLMCH4TimeBuffer( const std::string & name )
{
    return localInterface.getBLMCH4TimeBuffer( name );
}
//______________________________________________________________________________
std::vector< double > blmController::getMinOfTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getMinOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getMaxOfTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getMaxOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getAreaUnderTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getAreaUnderTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getTimeStamps( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< std::string > blmController::getStrTimeStamps( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return localInterface.getStrTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< double > blmController::getAvgNoise( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 )
{
    return localInterface.getAvgNoise( name, pvType, part1, part2 );
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
#ifdef BUILD_DLL
boost::python::list blmController::getBLMCH1Waveform_Py( const std::string & name )
{
    return toPythonList(getBLMCH1Waveform( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH2Waveform_Py( const std::string & name )
{
    return toPythonList(getBLMCH2Waveform( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH3Waveform_Py( const std::string & name )
{
    return toPythonList(getBLMCH3Waveform( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH4Waveform_Py( const std::string & name )
{
    return toPythonList(getBLMCH4Waveform( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH1Time_Py( const std::string & name )
{
    return toPythonList(getBLMCH1Time( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH2Time_Py( const std::string & name )
{
    return toPythonList(getBLMCH2Time( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH3Time_Py( const std::string & name )
{
    return toPythonList(getBLMCH3Time( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH4Time_Py( const std::string & name )
{
    return toPythonList(getBLMCH4Time( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getBLMTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTrace_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getBLMTrace( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH1WaveformBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH1WaveformBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH2WaveformBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH2WaveformBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH3WaveformBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH3WaveformBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH4WaveformBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH4WaveformBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH1TimeBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH1TimeBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH2TimeBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH2TimeBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH3TimeBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH3TimeBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMCH4TimeBuffer_Py( const std::string & name )
{
    return toPythonList(getBLMCH4TimeBuffer( name ));
}
//______________________________________________________________________________
boost::python::list blmController::getBLMTraceBuffer_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getBLMTraceBuffer( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getMinOfTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getMinOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getMaxOfTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getMaxOfTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getAreaUnderTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getAreaUnderTraces( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getTimeStamps_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getStrTimeStamps_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return toPythonList(getStrTimeStamps( name, pvType ));
}
//______________________________________________________________________________
boost::python::list blmController::getAvgNoise_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 )
{
    return toPythonList(getAvgNoise( name, pvType, part1, part2 ));
}
//______________________________________________________________________________
boost::python::list blmController::getAreaUnderPartOfTrace_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 )
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
