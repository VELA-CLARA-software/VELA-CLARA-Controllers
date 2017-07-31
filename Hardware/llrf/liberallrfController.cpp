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
size_t liberaLLRFController::getPowerTraceLength()
{
    return localInterface.getPowerTraceLength();
}
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
const llrfStructs::liberallrfObject& liberaLLRFController::getLLRFObjConstRef()
{
    return localInterface.getLLRFObjConstRef();
}
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
bool liberaLLRFController::Is_TracePV(llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.Is_TracePV(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::IsNot_TracePV(llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.IsNot_TracePV(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotMonitoring(pv);
}
//______________________________________________________________________________
void liberaLLRFController::startTraceMonitoring()
{
    localInterface.startTraceMonitoring();
}
//______________________________________________________________________________
bool liberaLLRFController::startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.Is_TracePV(pv);
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
bool liberaLLRFController::stopTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.stopTraceMonitoring(pv);
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


