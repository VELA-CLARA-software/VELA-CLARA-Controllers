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

#include "gunModController.h"
//#include <fstream>
//#include <iostream>
// stl
gunModController::gunModController(
    bool& show_messages,
    bool& show_debug_messages,
    const std::string & gunModConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(gunModConf,startVirtualMachine,&SHOW_MESSAGES,&SHOW_DEBUG_MESSAGES,shouldStartEPICs),
shouldStartEPICs(shouldStartEPICs)
{
//    if(shouldStartEPICs)
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
//    initialise();
}
//______________________________________________________________________________
gunModController::~gunModController(){}    //dtor
//______________________________________________________________________________
const rfModStructs::gunModObject& gunModController::getGunObjConstRef()
{
    return localInterface.getGunObjConstRef();
}
//______________________________________________________________________________
bool gunModController::isErrorStateGood() const
{
    return localInterface.isErrorStateGood();
}
//______________________________________________________________________________
bool gunModController::isWarmedUp() const
{
    return localInterface.isWarmedUp();
}
//______________________________________________________________________________
bool gunModController::isNotWarmedUp() const
{
    return localInterface.isNotWarmedUp();
}
//______________________________________________________________________________
bool gunModController::isInRFOn() const
{
    return localInterface.isInRFOn();
}
//______________________________________________________________________________
bool gunModController::isInHVOn() const
{
    return localInterface.isInHVOn();
}
//______________________________________________________________________________
bool gunModController::isInStandby() const
{
    return localInterface.isInStandby();
}
//______________________________________________________________________________
bool gunModController::isInOff() const
{
    return localInterface.isInOff();
}
//______________________________________________________________________________
void gunModController::reset()
{
    return localInterface.reset();
}
//______________________________________________________________________________
bool gunModController::resetAndWait(const size_t waitTime)
{
    return localInterface.resetAndWait(waitTime);
}
//______________________________________________________________________________
rfModStructs::GUN_MOD_STATE gunModController::getMainState() const
{
    return localInterface.getMainState();
}
//______________________________________________________________________________
rfModStructs::GUN_MOD_ERR_STATE gunModController::getErrorState() const
{
    return localInterface.getErrorState();
}
//______________________________________________________________________________
bool gunModController::waitForModState(rfModStructs::GUN_MOD_STATE state, const time_t waitTime)
{
    return localInterface.waitForModState(state,waitTime);
}
//____________________________________________________________________________________________
double gunModController::get_CA_PEND_IO_TIMEOUT()const
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void gunModController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > gunModController::getILockStates(const std::string& name)
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,std::string> gunModController::getILockStatesStr(const std::string& name)
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________________________
bool gunModController::setOff() const
{
    return localInterface.setOff();
}
//______________________________________________________________________________________________
bool gunModController::setStandby() const
{
    return localInterface.setStandby();
}
//______________________________________________________________________________________________
bool gunModController::setHVOn() const
{
    return localInterface.setHVOn();
}
//______________________________________________________________________________________________
bool gunModController::setRFOn() const
{
    return localInterface.setRFOn();
}

