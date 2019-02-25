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
//  FileName:    shutterController.cpp
//  Description:
//
//
//*/
// project includes
#include "shutterController.h"
// stl includes
#include <iostream>
//______________________________________________________________________________
shutterController::shutterController(bool& show_messages,
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
    {
        message("shutterController instantiation success.");
    }
}
//______________________________________________________________________________
bool shutterController::interfaceInitReport()
{
    return localInterface.interfaceInitReport();
}
//______________________________________________________________________________
shutterController::~shutterController()
{
    message("shutterController destructor complete.");
}    //dtor
//______________________________________________________________________________
void shutterController::open(const std::string& name)
{
    localInterface.open(name);
}
//______________________________________________________________________________
void shutterController::close(const std::string& name)
{
    localInterface.close(name);
}
//______________________________________________________________________________
bool shutterController::isOpen(const std::string& name ) const
{
    return localInterface.isOpen(name);
}
//______________________________________________________________________________
bool shutterController::isClosed(const std::string& name )const
{
    return localInterface.isClosed(name);
}
//______________________________________________________________________________
bool shutterController::areAllOpen()
{
    return localInterface.areAllOpen();
}
//______________________________________________________________________________
bool shutterController::areAllClosed()
{
    return localInterface.areAllClosed();
}
//______________________________________________________________________________
bool shutterController::openAndWait(const std::string& name, const time_t waitTime)
{
    return localInterface.openAndWait(name, waitTime);
}
//______________________________________________________________________________
bool shutterController::closeAndWait(const std::string & name, const time_t waitTime)
{
    return localInterface.closeAndWait(name, waitTime );
}
//______________________________________________________________________________
double shutterController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//______________________________________________________________________________
void shutterController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________
using namespace shutterStructs;
SHUTTER_STATE shutterController::getShutterState(const std::string& name)
{
    return localInterface.getShutterState(name);
}
//______________________________________________________________________________
std::string shutterController::getShutterStateStr(const std::string & name )
{
    return ENUM_TO_STRING(localInterface.getShutterState(name ));
}
//______________________________________________________________________________
const shutterObject&
    shutterController::getShutterObjConstRef(const std::string& name )const
{
    return localInterface.getShutterObjConstRef(name);
}
//______________________________________________________________________________
using namespace HWC_ENUM;
std::map<ILOCK_NUMBER, ILOCK_STATE>
    shutterController::getILockStates(const std::string& name)const
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________
std::map<ILOCK_NUMBER, std::string>
    shutterController::getILockStatesStr(const std::string& name)const
{
    return localInterface.getILockStatesStr(name);
}
//std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > shutterController::getILockStates(const std::string & name )
//{
//    return localInterface.getILockStates(name );
//}
////______________________________________________________________________________
//std::map< HWC_ENUM::ILOCK_NUMBER, std::string > shutterController::getILockStatesStr(const std::string & name )
//{
//    return localInterface.getILockStatesStr(name );
//}
//______________________________________________________________________________
std::vector<std::string> shutterController::getShutterNames()const
{
    return localInterface.getShutterNames();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list shutterController::getShutterNames_Py()
{
    return toPythonList(localInterface.getShutterNames());
}
#endif

//______________________________________________________________________________
//void shutterController::openShutter1()
//{
//    if(pilShutterNames.size() >= 1 )
//        open(pilShutterNames[0] );
//}
////______________________________________________________________________________
//void shutterController::closeShutter1()
//{
//    if(pilShutterNames.size() >= 1 )
//        close(pilShutterNames[0] );
//}
////______________________________________________________________________________
//void shutterController::openShutter2()
//{
//    if(pilShutterNames.size() >= 2 )
//        open(pilShutterNames[1] );
//}
////______________________________________________________________________________
//void shutterController::closeShutter2()
//{
//    if(pilShutterNames.size() >= 2 )
//        close(pilShutterNames[1] );
//}
////______________________________________________________________________________
//bool shutterController::openAndWaitShutter1(const time_t waitTime )
//{
//    bool ret = false;
//    if(pilShutterNames.size() >= 1 )
//        ret = openAndWait(pilShutterNames[0], waitTime );
//    return ret;
//}
////______________________________________________________________________________
//bool shutterController::openAndWaitShutter2(time_t waitTime )
//{
//    bool ret = false;
//    if(pilShutterNames.size() >= 2 )
//        ret = openAndWait(pilShutterNames[1], waitTime );
//    return ret;
//}
////______________________________________________________________________________
//bool shutterController::closeAndWaitShutter1(const time_t waitTime )
//{
//    bool ret = false;
//    if(pilShutterNames.size() >= 1 )
//        ret = closeAndWait(pilShutterNames[0], waitTime );
//    return ret;
//}
////______________________________________________________________________________
//bool shutterController::closeAndWaitShutter2(const time_t waitTime )
//{
//    bool ret = false;
//    if(pilShutterNames.size() >= 2 )
//        ret = closeAndWait(pilShutterNames[1], waitTime );
//    return ret;
//}
