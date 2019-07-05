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
//  Last edit:   19-03-2018
//  FileName:    gunProtController.cpp
//  Description:
//
//
//*/
// project includes
#include "gunProtController.h"
using namespace HWC_ENUM;
using namespace rfProtStructs;
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
gunProtController::gunProtController(bool& show_messages,
                                     bool& show_debug_messages,
                                     const std::string& allGunProtsConf,
                                     const bool startVirtualMachine,
                                     const bool shouldStartEPICs,
                                     const std::string& name
                                     ):
shouldStartEPICs(shouldStartEPICs),
controller(show_messages,show_debug_messages,CONTROLLER_TYPE::RF_PROT,name),
localInterface(allGunProtsConf,
               startVirtualMachine,
               show_messages,
               show_debug_messages,
               shouldStartEPICs)
{
    if(shouldStartEPICs)
    {
        message("gunProtController shouldStartEPICs is true");
    }
    else
    {
        message("gunProtController shouldStartEPICs is false");
    }
    initialise();
}
//______________________________________________________________________________
gunProtController::~gunProtController(){}
//______________________________________________________________________________
void gunProtController::initialise()const
{
    if(localInterface.interfaceInitReport())
    {
        message("gunProtController instantiation success.");
    }
}
//______________________________________________________________________________
bool gunProtController::isGood(const std::string& name)const
{
    return localInterface.isGood(name);
}
//______________________________________________________________________________
bool gunProtController::isNotGood(const std::string& name)const
{
    return localInterface.isNotGood(name);
}
//______________________________________________________________________________
const rfProtObject& gunProtController::getRFProtObjConstRef(const std::string& name)const
{
    return localInterface.getRFProtObjConstRef(name);
}
//______________________________________________________________________________
const rfProtObject& gunProtController::getRFProtObjConstRef(const rfProtStructs::RF_PROT_TYPE type)const
{
    return localInterface.getRFProtObjConstRef(type);
}
//______________________________________________________________________________
bool gunProtController::isBad(const std::string& name)const
{
    return localInterface.isBad(name);
}
//____________________________________________________________________________________________
bool gunProtController::reset(const std::string& name)const
{
    return localInterface.reset(name);
}
//____________________________________________________________________________________________
bool gunProtController::reset(const std::vector<std::string>& names)const
{
    return localInterface.reset(names);
}
//____________________________________________________________________________________________
bool gunProtController::enable(const std::string& name)const
{
    return localInterface.enable(name);
}
//____________________________________________________________________________________________
bool gunProtController::enable(const std::vector<std::string>& names)const
{
    return localInterface.enable(names);
}
//____________________________________________________________________________________________
bool gunProtController::enable()const
{
    return localInterface.enable();
}
//____________________________________________________________________________________________
bool gunProtController::disable(const std::string& name)const
{
    return localInterface.disable(name);
}
//____________________________________________________________________________________________
bool gunProtController::disable(const std::vector<std::string>& names)const
{
    return localInterface.disable(names);
}
//____________________________________________________________________________________________
std::string gunProtController::getGeneralProtName()const
{
     return localInterface.getGeneralProtName();
}
//____________________________________________________________________________________________
std::string gunProtController::getEnableProtName() const
{
    return localInterface.getEnableProtName();
}
//____________________________________________________________________________________________
std::string gunProtController::getCurrentModeProtName() const
{
    return localInterface.getCurrentModeProtName();
}
//____________________________________________________________________________________________
std::vector<std::string> gunProtController::getProtNames()const
{
    return localInterface.getProtNames();
}

        /*  These are pure virtual methods in the controller base ,
            so need to have some implmentation in derived classes
        */
//______________________________________________________________________________________________
std::map<ILOCK_NUMBER,std::string>
    gunProtController::getILockStatesStr(const std::string& name)const
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________________________
std::map<ILOCK_NUMBER,ILOCK_STATE>
    gunProtController::getILockStates(const std::string& name)const
{
    return localInterface.getILockStates(name);
}
//____________________________________________________________________________________________
double gunProtController::get_CA_PEND_IO_TIMEOUT()const
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void gunProtController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________
#ifdef BUILD_DLL
// overloads to expose containers as python objetcs
//______________________________________________________________________________
boost::python::list gunProtController::getProtNames_Py() const
{
    return toPythonList(getProtNames());
}
//______________________________________________________________________________
boost::python::dict gunProtController::getILockStatesStr_Py(const std::string& name)
{
    return toPythonDict(getILockStatesStr(name));
}
//______________________________________________________________________________
boost::python::dict gunProtController::getILockStates_Py(const std::string& name)
{
    return toPythonDict(getILockStates(name));
}
//______________________________________________________________________________
#endif //BUILD_DLL


