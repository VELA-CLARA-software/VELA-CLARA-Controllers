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
    const std::string& vcAnalysisConf,
    const std::string& piLaserMirrorConf
    ):
controller(show_messages,show_debug_messages, HWC_ENUM::CONTROLLER_TYPE::PI_LASER),
//localMirrorName("LASER_MIRROR_CONTROLLER"),
//localVirtualCathodeName("VIRTUAL_CATHODE_ANALYSIS_CONTROLLER"),
localInterface(show_messages,
               show_debug_messages,
               startVirtualMachine,
               shouldStartEPICs,
               vcAnalysisConf,
               piLaserMirrorConf,
               pilaserConf),
name(name)
{
//    if(localInterface.interfaceInitReport(shouldStartEPICs))
//    {
//        if(localShutter.interfaceInitReport())
//        {
//            if(localMirror.interfaceInitReport())
//                message("pilaserController instantiation success.");
//        }
//    }
    message("pilaserController constructed");
}
//______________________________________________________________________________
pilaserController::~pilaserController()
{}    //dtor
//______________________________________________________________________________
void  pilaserController::set_CA_PEND_IO_TIMEOUT(double val)
{
//    localShutter.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________
//______________________________________________________________________________
double pilaserController::get_CA_PEND_IO_TIMEOUT()const
{
    //return localShutter.get_CA_PEND_IO_TIMEOUT();
    return 1.0;
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
bool pilaserController::isOn() const
{
    return localInterface.isOn();
}
//______________________________________________________________________________
bool pilaserController::isOff() const
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
double pilaserController::getX()const
{
    return localInterface.getX();
}
//______________________________________________________________________________
double pilaserController::getY()const
{
    return localInterface.getY();
}
//______________________________________________________________________________
double pilaserController::getSigX()const
{
    return localInterface.getSigX();
}
//______________________________________________________________________________
double pilaserController::getSigY()const
{
    return localInterface.getSigY();
}
//______________________________________________________________________________
double pilaserController::getSigXY()const
{
    return localInterface.getSigXY();
}
//______________________________________________________________________________
double pilaserController::getXPix()const
{
    return localInterface.getXPix();
}
//______________________________________________________________________________
double pilaserController::getYPix()const
{
    return localInterface.getYPix();
}
//______________________________________________________________________________
double pilaserController::getSigXPix()const
{
    return localInterface.getSigXPix();
}
//______________________________________________________________________________
double pilaserController::getSigYPix()const
{
    return localInterface.getSigYPix();
}
//______________________________________________________________________________
double pilaserController::getSigXYPix()const
{
    return localInterface.getSigXYPix();
}
//______________________________________________________________________________
double pilaserController::getQ()const
{
    return localInterface.getQ();
}
//______________________________________________________________________________
bool pilaserController::enableStabilisation()
{
    return localInterface.enableStabilisation();
}
//______________________________________________________________________________
void pilaserController::clearBuffer()
{
    localInterface.clearBuffer();
}
//______________________________________________________________________________
bool pilaserController::isBufferFull()
{
    return localInterface.isBufferFull();
}
//______________________________________________________________________________
bool pilaserController::isBufferNotFull()
{
    return localInterface.isBufferNotFull();
}
//______________________________________________________________________________
void pilaserController::setBufferSize(const size_t s)
{
    localInterface.setBufferSize(s);
}
//______________________________________________________________________________
size_t pilaserController::getBufferCount()
{
    return localInterface.getBufferCount();
}
//______________________________________________________________________________
size_t pilaserController::getBufferSize()
{
    return localInterface.getBufferSize();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getXBuffer() const
{
    return localInterface.getXBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getYBuffer()const
{
    return localInterface.getYBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getSigXBuffer()const
{
    return localInterface.getSigXBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getSigYBuffer()const
{
    return localInterface.getSigYBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getSigXYBuffer()const
{
    return localInterface.getSigXYBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getXPixBuffer()const
{
    return localInterface.getXPixBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getYPixBuffer()const
{
    return localInterface.getYPixBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getSigXPixBuffer()const
{
    return localInterface.getSigXPixBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getSigYPixBuffer()const
{
    return localInterface.getSigYPixBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getSigXYPixBuffer()const
{
    return localInterface.getSigXYPixBuffer();
}
//______________________________________________________________________________
std::deque<double> pilaserController::getQBuffer()const
{
    return localInterface.getQBuffer();
}
//______________________________________________________________________________
std::vector<double> pilaserController::getPixelValues()const
{
    return localInterface.getPixelValues();
}
//______________________________________________________________________________
std::deque<std::vector<double>> pilaserController::getPixelValuesBuffer()const
{
    return localInterface.getPixelValuesBuffer();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list pilaserController::getPixelValues_Py()const
{
    return toPythonList(getPixelValues());
}
//______________________________________________________________________________
boost::python::list pilaserController::getPixelValuesBuffer_Py()const
{
    return toPythonList(getPixelValuesBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getQBuffer_Py()const
{
    return toPythonList(getQBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getXBuffer_Py()const
{
    return toPythonList(getXBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getYBuffer_Py()const
{
    return toPythonList(getYBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getSigXBuffer_Py()const
{
    return toPythonList(getSigXBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getSigYBuffer_Py()const
{
    return toPythonList(getSigYBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getSigXYBuffer_Py()const
{
    return toPythonList(getSigXYBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getXPixBuffer_Py()const
{
    return toPythonList(getXPixBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getYPixBuffer_Py()const
{
    return toPythonList(getYPixBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getSigXPixBuffer_Py()const
{
    return toPythonList(getSigXPixBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getSigYPixBuffer_Py()const
{
    return toPythonList(getSigYPixBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getSigXYPixBuffer_Py()const
{
    return toPythonList(getSigXYPixBuffer());
}
//______________________________________________________________________________
#endif // BUILD_DLL








//
//   _____  .__
//  /     \ |__|_____________  ___________
// /  \ /  \|  \_  __ \_  __ \/  _ \_  __ \
///    Y    \  ||  | \/|  | \(  <_> )  | \/
//\____|__  /__||__|   |__|   \____/|__|
//        \/
//
//
//
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
//bool pilaserController::setHpos(int value)
//{
//    return localInterface.setHpos(value);
//}
//______________________________________________________________________________
bool pilaserController::setVpos(double value)
{
    return localInterface.setVpos(value);
}
//______________________________________________________________________________
//bool pilaserController::setVpos(int value)
//{
//    return localInterface.setVpos(value);
//}
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
const pilaserStructs::virtualCathodeDataObject& pilaserController::getVCDataObjConstRef() const
{
    return localInterface.getVCDataObjConstRef();
}
//______________________________________________________________________________
std::vector<double> pilaserController::getFastImage()
{
    return localInterface.getFastImage();
}
//______________________________________________________________________________
boost::python::list pilaserController::getFastImage_Py()
{
    return toPythonList(getFastImage());
}
//______________________________________________________________________________






////______________________________________________________________________________
//#ifdef BUILD_DLL
////____________________________________________________________________________________________
//double pilaserController::getHpos()
//{
//    return localShutter.getHpos();
//}
////____________________________________________________________________________________________
//double pilaserController::getVpos()
//{
//    return localInterface.getVpos();
//}
////____________________________________________________________________________________________
//double pilaserController::getIntensity()
//{
//    return localInterface.getIntensity();
//}
////____________________________________________________________________________________________
//bool pilaserController::setHpos(double value)
//{
//    return localInterface.setHpos(value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setHpos(int value)
//{
//    return localInterface.setHpos(value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setVpos(double value)
//{
//    return localInterface.setVpos(value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setVpos(int value)
//{
//    return localInterface.setVpos(value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setIntensity(double value)
//{
//    return localInterface.setIntensity(value );
//}
////____________________________________________________________________________________________
//bool pilaserController::setIntensity(int value)
//{
//    return localInterface.setIntensity(value );
//}
////____________________________________________________________________________________________
//const pilaserStructs::pilaserObject &pilaserController::getPILObjConstRef()
//{
//    return localInterface.getPILObjConstRef();
//}
////____________________________________________________________________________________________
//
////_____________________________________________________________________________________________
//void pilaserController::set_CA_PEND_IO_TIMEOUT(double val)
//{
//    //localInterface.set_CA_PEND_IO_TIMEOUT(val);
//}
////______________________________________________________________________________________________
//std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > pilaserController::getILockStates(const std::string& name)
//{
//    return localInterface.getILockStates(name);
//}
////______________________________________________________________________________________________
//std::map<HWC_ENUM::ILOCK_NUMBER,std::string> pilaserController::getILockStatesStr(const std::string& name)
//{
//    return localInterface.getILockStatesStr(name);
//}
//______________________________________________________________________________
//boost::python::dict pilaserController::getILockStatesStr_Py(std::string magName )
//{
//    return enumStringMapToPythonDict(getILockStatesStr(magName ) );
//}
////______________________________________________________________________________
//boost::python::dict pilaserController::getILockStates_Py(std::string magName )
//{
//    return enumMapToPythonDict(getILockStates(magName ) );
//}
////______________________________________________________________________________
//boost::python::dict pilaserController::getMagPSUStateDefinition()
//{
//    std::map< HWC_ENUM::MAG_PSU_STATE,  std::string  > m;
//
//    m[HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ); // ENUM_TO_STRING MACRO in velaStructs.h
//
//    return enumStringMapToPythonDict(m );
//}
//______________________________________________________________________________
//#endif // BUILD_DLL









//
//   _________.__            __    __
//  /   _____/|  |__  __ ___/  |__/  |_  ___________
//  \_____  \ |  |  \|  |  \   __\   __\/ __ \_  __ \
//  /        \|   Y  \  |  /|  |  |  | \  ___/|  | \/
// /_______  /|___|  /____/ |__|  |__|  \___  >__|
//         \/      \/                       \/
//
//
//
//
////______________________________________________________________________________
//void pilaserController::open(const std::string& name)
//{
//    localShutter.open(name);
//}
////______________________________________________________________________________
//void pilaserController::close(const std::string& name)
//{
//    localShutter.close(name);
//}
////______________________________________________________________________________
//bool pilaserController::isOpen(const std::string& name ) const
//{
//    return localShutter.isOpen(name );
//}
////______________________________________________________________________________
//bool pilaserController::isClosed(const std::string& name )const
//{
//    return localShutter.isClosed(name );
//}
////______________________________________________________________________________
//bool pilaserController::openAndWait(const std::string& name, const time_t waitTime )
//{
//    return localShutter.openAndWait(name, waitTime);
//}
////______________________________________________________________________________
//bool pilaserController::closeAndWait(const std::string & name, const time_t waitTime )
//{
//    return localShutter.closeAndWait(name,waitTime);
//}
////______________________________________________________________________________
//const shutterStructs::shutterObject&
//pilaserController::getShutterObjConstRef(const std::string& name )const
//{
//    return localShutter.getShutterObjConstRef(name);
//}
////______________________________________________________________________________
//shutterStructs::SHUTTER_STATE pilaserController::getShutterState(const std::string& name)
//{
//    return localShutter.getShutterState(name);
//}
////______________________________________________________________________________
//std::string pilaserController::getShutterStateStr(const std::string& name )
//{
//    return localShutter.getShutterStateStr(name);
//}
////______________________________________________________________________________
//std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
//    pilaserController::getILockStates(const std::string& name)const
//{
//    return localShutter.getILockStates(name);
//}
////______________________________________________________________________________
//std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
//    pilaserController::getILockStatesStr(const std::string& name)const
//{
//    return localShutter.getILockStatesStr(name);
//}
////______________________________________________________________________________
//bool pilaserController::areAllOpen()
//{
//    return localShutter.areAllOpen();
//}
////______________________________________________________________________________
//bool pilaserController::areAllClosed()
//{
//    return localShutter.areAllClosed();
//}

