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
//  Last edit:   29-03-2018
//  FileName:    pilaserController.cpp
//  Description:
//
//
//*/
// project includes
#include "pilaserController.h"
// stl includes
//#include <fstream>
//#include <iostream>
//______________________________________________________________________________
pilaserController::pilaserController(
    bool& show_messages,
    bool& show_debug_messages,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const std::string& name,
    const std::string& pilaserConf,
    const std::string& claraCamConfig,
    const std::string& piLaserMirrorConf,
    const std::string& velaCamConfig,
    const HWC_ENUM::MACHINE_AREA area
    ):
localInterface(show_messages,
               show_debug_messages,
               startVirtualMachine,
               shouldStartEPICs,
               pilaserConf,
               claraCamConfig,
               piLaserMirrorConf,
               velaCamConfig,
               area),
name(name),
cameraControllerBase(show_messages,show_debug_messages, name, HWC_ENUM::CONTROLLER_TYPE::PI_LASER, &localInterface)
{
    if(localInterface.interfaceInitReport(shouldStartEPICs))
    {
        camBase = &localInterface;
        message("pilaserController instantiation success.");
    }
}
//______________________________________________________________________________
pilaserController::~pilaserController()
{}    //dtor
//______________________________________________________________________________
void  pilaserController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________
double pilaserController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//______________________________________________________________________________
int  pilaserController::setHWP(const double value)
{
    return localInterface.setHWP(value);
}
//______________________________________________________________________________
double pilaserController::getHWP()
{
    return localInterface.getHWP();
}
//______________________________________________________________________________
bool pilaserController::setCharge(const double value)
{
    return localInterface.setCharge(value);
}
//______________________________________________________________________________
bool pilaserController::setIntensity(const double value)
{
    return localInterface.setIntensity(value);
}
//______________________________________________________________________________
double pilaserController::getIntensity() const
{
    return localInterface.getIntensity();
}
//______________________________________________________________________________
std::string pilaserController::getName() const
{
    return localInterface.getName();
}
//______________________________________________________________________________
HWC_ENUM::STATE pilaserController::getStatus() const
{
    return localInterface.getStatus();
}
//______________________________________________________________________________
double pilaserController::getStabilisationStatus() const
{
    return localInterface.getStabilisationStatus();
}
//______________________________________________________________________________
bool pilaserController::isOn()
{
    return localInterface.isOn();
}
//______________________________________________________________________________
bool pilaserController::isOff()
{
    return localInterface.isOff();
}
//______________________________________________________________________________
bool pilaserController::isStabilisationOff() const
{
    return localInterface.isStabilisationOff();
}
//______________________________________________________________________________
bool pilaserController::isStabilisationOn() const
{
    return localInterface.isStabilisationOn();
}
//______________________________________________________________________________
bool pilaserController::disableStabilisation()
{
    return localInterface.disableStabilisation();
}
//______________________________________________________________________________
bool pilaserController::enableStabilisation()
{
    return localInterface.enableStabilisation();
}
//______________________________________________________________________________
double pilaserController::getQ()const
{
    return localInterface.getQ();
}
//______________________________________________________________________________
std::vector<double> pilaserController::getQBuffer()const
{
    return localInterface.getQBuffer();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list pilaserController::getQBuffer_Py()const
{
    return toPythonList(getQBuffer());
}
#endif // BUILD_DLL
//______________________________________________________________________________
//
//   _____  .__
//  /     \ |__|_____________  ___________
// /  \ /  \|  \_  __ \_  __ \/  _ \_  __ \
///    Y    \  ||  | \/|  | \(  <_> )  | \/
//\____|__  /__||__|   |__|   \____/|__|
//        \/
//
//______________________________________________________________________________
double pilaserController::getHpos() const
{
    return localInterface.getHpos();
}
//______________________________________________________________________________
double pilaserController::getVpos() const
{
    return localInterface.getVpos();
}
//______________________________________________________________________________
bool pilaserController::setHpos(double value)
{
     return localInterface.setHpos(value);
}
//______________________________________________________________________________
bool pilaserController::setVpos(double value)
{
    return localInterface.setVpos(value);
}
//______________________________________________________________________________
double pilaserController::getHstep() const
{
    return localInterface.getHstep();
}
//______________________________________________________________________________
double pilaserController::getVstep() const
{
    return localInterface.getVstep();
}
//______________________________________________________________________________
bool pilaserController::setHstep(double value)
{
    return localInterface.setHstep(value);
}
//______________________________________________________________________________
bool pilaserController::setVstep(double value)
{
    return localInterface.setVstep(value);
}
//______________________________________________________________________________
bool pilaserController::moveH()
{
    return localInterface.moveH();
}
//______________________________________________________________________________
bool pilaserController::moveV()
{
    return localInterface.moveV();
}
//______________________________________________________________________________
const pilaserStructs::pilMirrorObject& pilaserController::getpilMirrorObjConstRef() const
{
    return localInterface.getpilMirrorObjConstRef();
}
//______________________________________________________________________________
const pilaserStructs::pilaserObject& pilaserController::getPILObjConstRef() const
{
    return localInterface.getPILObjConstRef();
}
//______________________________________________________________________________
void pilaserController::clearRunningValues()
{
    localInterface.clearRunningValues();
    cameraControllerBase::clearRunningValues();
}
//______________________________________________________________________________
//const pilaserStructs::virtualCathodeDataObject& pilaserController::getVCDataObjConstRef() const
//{
//    return localInterface.getVCDataObjConstRef();
//}

