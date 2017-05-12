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

#include "gunProtController.h"
//#include <fstream>
//#include <iostream>
// stl
gunProtController::gunProtController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & allGunProtsConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(allGunProtsConf,startVirtualMachine,&SHOW_MESSAGES,&SHOW_DEBUG_MESSAGES,shouldStartEPICs),
shouldStartEPICs(shouldStartEPICs)
{
    if(shouldStartEPICs )
        message("gunProtController shouldStartEPICs is true");
    else
        message("gunProtController shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
gunProtController::~gunProtController(){}    //dtor
//______________________________________________________________________________
void gunProtController::initialise()
{
    if(localInterface.interfaceInitReport(shouldStartEPICs) )
        message("gunProtController instantiation success.");
}
//______________________________________________________________________________
bool gunProtController::isGood(const std::string & name)
{
    return localInterface.isGood(name);
}
//______________________________________________________________________________
bool gunProtController::isNotGood(const std::string & name)
{
    return localInterface.isNotGood(name);
}
//______________________________________________________________________________
bool gunProtController::isBad(const std::string & name)
{
    return localInterface.isBad(name);
}
//____________________________________________________________________________________________
double gunProtController::get_CA_PEND_IO_TIMEOUT()
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void gunProtController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > gunProtController::getILockStates(const std::string& name)
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________________________
std::map<VELA_ENUM::ILOCK_NUMBER,std::string> gunProtController::getILockStatesStr(const std::string& name)
{
    return localInterface.getILockStatesStr(name);
}
//____________________________________________________________________________________________
bool gunProtController::reset(const std::string& name)
{
    return localInterface.reset(name);
}
//____________________________________________________________________________________________
bool gunProtController::reset(const std::vector<std::string>& names)
{
    return localInterface.reset(names);
}
//____________________________________________________________________________________________
bool gunProtController::enable(const std::string& name)
{
    return localInterface.enable(name);
}
//____________________________________________________________________________________________
bool gunProtController::enable(const std::vector<std::string>& names)
{
    return localInterface.enable(names);
}
//____________________________________________________________________________________________
bool gunProtController::enable()
{
    return localInterface.enable();
}
//____________________________________________________________________________________________
bool gunProtController::disable(const std::string& name)
{
    return localInterface.disable(name);
}
//____________________________________________________________________________________________
bool gunProtController::disable(const std::vector<std::string>& names)
{
    return localInterface.disable(names);
}
//____________________________________________________________________________________________
std::string gunProtController::getGeneralProtName()
{
     return localInterface.getGeneralProtName();
}
//____________________________________________________________________________________________
std::string gunProtController::getEnableProtName()
{
    return localInterface.getEnableProtName();
}
//____________________________________________________________________________________________
std::string gunProtController::getCurrentModeProtName()
{
    return localInterface.getCurrentModeProtName();
}
//____________________________________________________________________________________________

