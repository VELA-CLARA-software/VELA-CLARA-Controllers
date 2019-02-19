/*
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
//
//  Author:      DJS
//  Last edit:   06-04-2018
//  FileName:    ba1StagesController.cpp
//  Description:
//
//
//*/
// project includes
#include "ba1StagesController.h"
// stl includes
#include <iostream>
//______________________________________________________________________________
ba1StagesController::ba1StagesController(bool& show_messages,
                                     bool& show_debug_messages,
                                     const bool startVirtualMachine,
                                     const bool shouldStartEPICs,
                                     const std::string& configFile,
                                     const std::string& name
                          ):
controller(show_messages, show_debug_messages,name, HWC_ENUM::CONTROLLER_TYPE::SHUTTER),
localInterface(show_messages,
               show_debug_messages,
               startVirtualMachine,
               shouldStartEPICs,
               configFile)
{
    if(localInterface.interfaceInitReport() )
        message("ba1StagesController instantiation success.");
}
//______________________________________________________________________________
bool ba1StagesController::interfaceInitReport()
{
    return localInterface.interfaceInitReport();
}
//______________________________________________________________________________
ba1StagesController::~ba1StagesController(){}    //dtor
//--------------------------------------------------------------------------------
std::vector<std::string> ba1StagesController::getStageNames()const
{
    return localInterface.getStageNames();
}

//--------------------------------------------------------------------------------
double ba1StagesController::getStagePosition(const std::string& stage)const
{
    return localInterface.getStagePosition(stage);
}
//--------------------------------------------------------------------------------
bool ba1StagesController::setStagePosition(const std::string& stage, double val)
{
    return localInterface.setStagePosition(stage, val);
}
//--------------------------------------------------------------------------------
bool ba1StagesController::screenIn(const std::string& stage)
{
    return localInterface.screenIn(stage);
}
//--------------------------------------------------------------------------------
bool ba1StagesController::screenOut(const std::string& stage)
{
    return localInterface.screenOut(stage);
}
//--------------------------------------------------------------------------------
bool ba1StagesController::isReadPosEqualSetPos(const std::string& stage, double tolerance)
{
    return localInterface.isReadPosEqualSetPos(stage,tolerance);
}
//--------------------------------------------------------------------------------
double ba1StagesController::getMinPos(const std::string& stage)const
{
    return localInterface.getMinPos(stage);
}
//--------------------------------------------------------------------------------
double ba1StagesController::getMaxPos(const std::string& stage)const
{
    return localInterface.getMaxPos(stage);
}
//--------------------------------------------------------------------------------
bool ba1StagesController::setDevice(const std::string& stage, const std::string& device)
{
    return localInterface.setDevice(stage, device);
}
//--------------------------------------------------------------------------------
double ba1StagesController::getStageSetPosition(const std::string& stage)const
{
    return localInterface.getStageSetPosition(stage);
}
//--------------------------------------------------------------------------------
std::vector<std::string> ba1StagesController::getDevices(const std::string& stage)const
{
    return localInterface.getDevices(stage);
}
//--------------------------------------------------------------------------------
std::map<std::string, double>  ba1StagesController::getStageDeviceAndPositionMap(const std::string& stage)const
{
   return localInterface.getStageDeviceAndPositionMap(stage);
}
//--------------------------------------------------------------------------------
#ifdef BUILD_DLL
boost::python::list ba1StagesController::getStageNames_Py()const
{
    return toPythonList(getStageNames());
}
//--------------------------------------------------------------------------------
boost::python::list ba1StagesController::getDevices_Py(const std::string& stage)const
{
    return toPythonList(getDevices(stage));
}
//--------------------------------------------------------------------------------
boost::python::dict ba1StagesController::getStageDeviceAndPositionMap_Py(const std::string& stage)
{
    return toPythonDict(getStageDeviceAndPositionMap(stage));
}
#endif










//______________________________________________________________________________
using namespace HWC_ENUM;
std::map<ILOCK_NUMBER, ILOCK_STATE>
    ba1StagesController::getILockStates(const std::string& name)const
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________
std::map<ILOCK_NUMBER, std::string>
    ba1StagesController::getILockStatesStr(const std::string& name)const
{
    return localInterface.getILockStatesStr(name);
}

//______________________________________________________________________________
double ba1StagesController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//______________________________________________________________________________
void ba1StagesController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}

