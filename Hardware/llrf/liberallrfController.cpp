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

#include "liberaLLRFController.h"
#include <fstream>
#include <iostream>
// stl
liberaLLRFController::liberaLLRFController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & laserConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const llrfStructs::LLRF_TYPE type):
controller(show_messages,show_debug_messages),
localInterface(laserConf,startVirtualMachine,&SHOW_MESSAGES,&SHOW_DEBUG_MESSAGES,shouldStartEPICs,type),
shouldStartEPICs(shouldStartEPICs)
{
//    if(shouldStartEPICs )
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
//  __   ___ ___ ___  ___  __   __
// / _` |__   |   |  |__  |__) /__`
// \__> |___  |   |  |___ |  \ .__/
//
//______________________________________________________________________________
liberaLLRFController::~liberaLLRFController(){}    //dtor
//______________________________________________________________________________
void liberaLLRFController::initialise()
{
    if(localInterface.interfaceInitReport(shouldStartEPICs) )
        message("liberaLLRFController instantiation success.");
}
//______________________________________________________________________________
llrfStructs::LLRF_TYPE liberaLLRFController::getType()
{
    return localInterface.getType();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpFF()
{
    return localInterface.getAmpFF();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpSP()
{
    return localInterface.getAmpSP();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiFF()
{
    return localInterface.getPhiFF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiSP()
{
    return localInterface.getPhiSP();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpLLRF()
{
    return localInterface.getAmpLLRF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiLLRF()
{
    return localInterface.getPhiLLRF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiDEG()
{
    return localInterface.getPhiDEG();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpMVM()
{
    return localInterface.getAmpMVM();
}
//______________________________________________________________________________
double liberaLLRFController::getPulseLength()
{
    return localInterface.getPulseLength();
}
//______________________________________________________________________________
double liberaLLRFController::getPulseOffset()
{
    return localInterface.getPulseOffset();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiCalibration()
{
    return localInterface.getPhiCalibration();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpCalibration()
{
    return localInterface.getAmpCalibration();
}
//______________________________________________________________________________
double liberaLLRFController::getCrestPhiLLRF()
{
    return localInterface.getCrestPhiLLRF();
}
//______________________________________________________________________________
bool   liberaLLRFController::isLocked()
{
    return localInterface.isLocked();
}
//______________________________________________________________________________
size_t liberaLLRFController::getTraceLength()
{
    return localInterface.getTraceLength();
}
//______________________________________________________________________________
std::vector<std::string> liberaLLRFController::getChannelNames()
{
    return localInterface.getChannelNames();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getTraceValues(const std::string& name)
{
    return localInterface.getTraceValues(name);
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getTraceData(const std::string& name)
{
    return localInterface.getTraceData(name);
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getTraceBuffer(const std::string& name)
{
    return localInterface.getTraceBuffer(name);
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getChannelNames_Py()
{
    return toPythonList(getChannelNames());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTraceValues_Py(const std::string& name)
{
    return toPythonList(getTraceValues(name));
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTraceBuffer_Py(const std::string& name)
{
    return toPythonList(getTraceBuffer(name));
}
#endif
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavRevPower()
{
    return localInterface.getCavRevPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavFwdPower()
{
    return localInterface.getCavFwdPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyRevPower()
{
    return localInterface.getKlyRevPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyFwdPower()
{
    return localInterface.getKlyFwdPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavRevPhase()
{
    return localInterface.getCavRevPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavFwdPhase()
{
    return localInterface.getCavFwdPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyRevPhase()
{
    return localInterface.getKlyRevPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyFwdPhase()
{
    return localInterface.getKlyFwdPhase();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPower_Py()
{
    return toPythonList(getCavRevPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPower_Py()
{
    return toPythonList(getKlyFwdPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPower_Py()
{
    return toPythonList(getCavFwdPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPower_Py()
{
    return toPythonList(getKlyRevPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPhase_Py()
{
    return toPythonList(getCavRevPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPhase_Py()
{
    return toPythonList(getKlyFwdPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPhase_Py()
{
    return toPythonList(getCavFwdPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPhase_Py()
{
    return toPythonList(getKlyRevPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
bool liberaLLRFController::setHighMask_Py(const std::string& name, boost::python::list& value)
{
    return setHighMask(name,to_std_vector<double>(value));
}
//______________________________________________________________________________
bool liberaLLRFController::setLowMask_Py(const std::string& name, boost::python::list& value)
{
    return setLowMask(name,to_std_vector<double>(value));
}
//______________________________________________________________________________
#endif
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavRevPowerData()
{
    return localInterface.getCavRevPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavFwdPowerData()
{
    return localInterface.getCavFwdPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyRevPowerData()
{
    return localInterface.getKlyRevPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyFwdPowerData()
{
    return localInterface.getKlyFwdPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavRevPhaseData()
{
    return localInterface.getCavRevPhaseData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavFwdPhaseData()
{
    return localInterface.getCavFwdPhaseData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyRevPhaseData()
{
    return localInterface.getKlyRevPhaseData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyFwdPhaseData()
{
    return localInterface.getKlyFwdPhaseData();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavRevPowerBuffer()
{
    return localInterface.getCavRevPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavFwdPowerBuffer()
{
    return localInterface.getCavFwdPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyRevPowerBuffer()
{
    return localInterface.getKlyRevPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyFwdPowerBuffer()
{
    return localInterface.getKlyFwdPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavRevPhaseBuffer()
{
    return localInterface.getCavRevPhaseBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavFwdPhaseBuffer()
{
    return localInterface.getCavFwdPhaseBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyRevPhaseBuffer()
{
    return localInterface.getKlyRevPhaseBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyFwdPhaseBuffer()
{
    return localInterface.getKlyFwdPhaseBuffer();
}
//______________________________________________________________________________
//______________________________________________________________________________

//______________________________________________________________________________
const llrfStructs::liberallrfObject& liberaLLRFController::getLLRFObjConstRef()
{
    return localInterface.getLLRFObjConstRef();
}
//______________________________________________________________________________
//  __   ___ ___ ___  ___  __   __
// /__` |__   |   |  |__  |__) /__`
// .__/ |___  |   |  |___ |  \ .__/
//______________________________________________________________________________
bool liberaLLRFController::setPhiSP(double value)
{
    return localInterface.setPhiSP(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiFF(double value)
{
    return localInterface.setPhiFF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpSP(double value)
{
    return localInterface.setAmpSP(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpFF(double value)
{
    return localInterface.setAmpFF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpLLRF(double value)
{
    return localInterface.setAmpLLRF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiLLRF(double value)
{
    return localInterface.setPhiLLRF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpMVM(double value)
{
    return localInterface.setAmpMVM(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiDEG(double value)
{
    return localInterface.setPhiDEG(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPulseLength(double value)
{
    return localInterface.setPulseLength(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPulseOffset(double value)
{
    return localInterface.setPulseOffset(value);
}
//______________________________________________________________________________
void liberaLLRFController::setPhiCalibration(double value)
{
    localInterface.setPhiCalibration(value);
}
//______________________________________________________________________________
void liberaLLRFController::setAmpCalibration(double value)
{
    localInterface.setAmpCalibration(value);
}
//______________________________________________________________________________
void liberaLLRFController::setCrestPhiLLRF(double value)
{
    localInterface.setCrestPhiLLRF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setHighMask(const std::string&name, std::vector<double>& value)
{
    return localInterface.setHighMask(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setLowMask(const std::string&name, std::vector<double>& value)
{
    return localInterface.setLowMask(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::clearMask(const std::string&name)
{
    return localInterface.clearMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::setNumBufferTraces(const std::string&name, size_t value)
{
    return localInterface.setNumBufferTraces(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setCheckMask(const std::string&name, bool value)
{
    return localInterface.setCheckMask(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setShouldCheckMask(const std::string&name)
{
    return localInterface.setShouldCheckMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::setShouldNotCheckMask(const std::string&name)
{
    return localInterface.setShouldNotCheckMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::setKeepRollingAverage(const std::string&name, bool value)
{
    return localInterface.setKeepRollingAverage(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setShouldKeepRollingAverage(const std::string&name)
{
    return localInterface.setShouldKeepRollingAverage(name);
}
//______________________________________________________________________________
bool liberaLLRFController::setShouldNotKeepRollingAverage(const std::string&name)
{
    return localInterface.setShouldNotKeepRollingAverage(name);
}
//______________________________________________________________________________
bool liberaLLRFController::setNumRollingAverageTraces(const std::string&name, size_t value )
{
    return localInterface.setNumRollingAverageTraces(name, value);
}
//______________________________________________________________________________
size_t liberaLLRFController::getNumRollingAverageTraces(const std::string&name)
{
    return localInterface.getNumRollingAverageTraces(name);
}
//______________________________________________________________________________
bool liberaLLRFController::clearRollingAverage(const std::string&name)
{
    return localInterface.clearRollingAverage(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isTracePV(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.Is_TracePV(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotTracePV(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.IsNot_TracePV(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isMonitoring(const std::string& name)
{
    return localInterface.isMonitoring(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotMonitoring(const std::string& name)
{
    return localInterface.isNotMonitoring(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isCheckingMask(const std::string& name)
{
    return localInterface.isCheckingMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotCheckingMask(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotCheckingMask(const std::string& name)
{
    return localInterface.isCheckingMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotCheckingMask(pv);
}
//______________________________________________________________________________
void liberaLLRFController::startTraceMonitoring()
{
    localInterface.startTraceMonitoring();
}
//______________________________________________________________________________
bool liberaLLRFController::startTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.startTraceMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::startTraceMonitoring(const std::string& name)
{
    return localInterface.startTraceMonitoring(name);
}
//______________________________________________________________________________
bool liberaLLRFController::startCavFwdTraceMonitor()
{
    return localInterface.startCavFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::startCavRevTraceMonitor()
{
    return localInterface.startCavRevTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::startKlyFwdTraceMonitor()
{
    return localInterface.startKlyFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::startKlyRevTraceMonitor()
{
    return localInterface.startKlyRevTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.stopTraceMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::stopTraceMonitoring(const std::string& name)
{
    return localInterface.stopTraceMonitoring(name);
}
//______________________________________________________________________________
void liberaLLRFController::stopTraceMonitoring()
{
    return localInterface.stopTraceMonitoring();
}
//______________________________________________________________________________
bool liberaLLRFController::stopCavFwdTraceMonitor()
{
    return localInterface.stopCavFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopCavRevTraceMonitor()
{
    return localInterface.stopCavRevTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopKlyFwdTraceMonitor()
{
    return localInterface.stopKlyFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopKlyRevTraceMonitor()
{
    return localInterface.stopKlyRevTraceMonitor();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
//______________________________________________________________________________
//____________________________________________________________________________________________
double liberaLLRFController::get_CA_PEND_IO_TIMEOUT()
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void liberaLLRFController::set_CA_PEND_IO_TIMEOUT(double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val );
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > liberaLLRFController::getILockStates( const std::string & name )
{
    return localInterface.getILockStates(name );
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > liberaLLRFController::getILockStatesStr(const std::string & name )
{
    return localInterface.getILockStatesStr(name );
}
//______________________________________________________________________________
//______________________________________________________________________________
//boost::python::dict liberaLLRFController::getILockStatesStr_Py(std::string magName )
//{
//    return enumStringMapToPythonDict(getILockStatesStr(magName ) );
//}
////______________________________________________________________________________
//boost::python::dict liberaLLRFController::getILockStates_Py(std::string magName )
//{
//    return enumMapToPythonDict(getILockStates(magName ) );
//}
////______________________________________________________________________________
//boost::python::dict liberaLLRFController::getMagPSUStateDefinition()
//{
//    std::map< VELA_ENUM::MAG_PSU_STATE,  std::string  > m;
//
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ] = ENUM_TO_STRING(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON ] = ENUM_TO_STRING(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ] = ENUM_TO_STRING(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ] = ENUM_TO_STRING(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ] = ENUM_TO_STRING(VELA_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ); // ENUM_TO_STRING MACRO in velaStructs.h
//
//    return enumStringMapToPythonDict(m );
//}
//______________________________________________________________________________
#endif // BUILD_DLL


