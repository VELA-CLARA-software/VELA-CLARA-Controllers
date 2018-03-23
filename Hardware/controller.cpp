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
//  FileName:    controller.cpp
//  Description:
//
//
//*/
// project includes
#include "controller.h"
// stl includes
#include <iostream>
#include <sstream>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
controller::controller(bool* show_messages,
                       bool* show_debug_messages):
controller(show_messages,
           show_debug_messages,
           HWC_ENUM::CONTROLLER_TYPE::UNKNOWN_CONTROLLER_TYPE)
{}
//______________________________________________________________________________
controller::controller(bool* show_messages,
                       bool* show_debug_messages,
                       const HWC_ENUM::CONTROLLER_TYPE type):
controller(show_messages,show_debug_messages,type,UTL::UNKNOWN_NAME)
{}
//______________________________________________________________________________
controller::controller(bool* show_messages,
           bool* show_debug_messages,
           const HWC_ENUM::CONTROLLER_TYPE type,
           const std::string& name):
SHOW_DEBUG_MESSAGES(show_debug_messages),
SHOW_MESSAGES(show_messages),
controllerType(type),
baseObject(show_messages, show_debug_messages),
name(name)
{
    if(show_messages)
    {
        messagesOn();
    }
    else
    {
        messagesOff();
    }
    if(show_debug_messages)
    {
        debugMessagesOn();
    }
    else
    {
        debugMessagesOff();
    }
}
//______________________________________________________________________________

//______________________________________________________________________________
controller::~controller(){}
//______________________________________________________________________________
void controller::debugMessagesOn()
{
    std::cout << "debugMessages On" <<std::endl;
    *SHOW_DEBUG_MESSAGES = true;
}
//______________________________________________________________________________
void controller::debugMessagesOff()
{
    std::cout << "debugMessages Off" <<std::endl;
    *SHOW_DEBUG_MESSAGES = false;
}
//______________________________________________________________________________
void controller::messagesOn()
{
    std::cout << "messages On" <<std::endl;
    *SHOW_MESSAGES = true;
}
//______________________________________________________________________________
void controller::messagesOff()
{
    std::cout << "messages Off" <<std::endl;
    *SHOW_MESSAGES = false;
}
//______________________________________________________________________________
void controller::silence()
{
    std::cout << "silence" <<std::endl;
    *SHOW_DEBUG_MESSAGES = false;
    *SHOW_MESSAGES = false;
}
//______________________________________________________________________________
void controller::verbose()
{
    std::cout <<"verbose" <<std::endl;
    *SHOW_DEBUG_MESSAGES = true;
    *SHOW_MESSAGES = true;
}
//______________________________________________________________________________
bool controller::isSilent() const
{
    if(!*SHOW_DEBUG_MESSAGES && !*SHOW_MESSAGES)
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool controller::isVerbose() const
{
    return !isSilent();
}
//______________________________________________________________________________
bool controller::isMessageOn() const
{
    if(*SHOW_MESSAGES)
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool controller::isDebugMessageOn() const
{
    if(*SHOW_DEBUG_MESSAGES)
        return true;
    else
        return false;
}
//______________________________________________________________________________
HWC_ENUM::CONTROLLER_TYPE controller::getControllerType() const
{
    return controllerType;
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict controller::getILockStatesDefinition() const
{
    std::map<HWC_ENUM::ILOCK_STATE, std::string> m;
    using namespace HWC_ENUM;
    m[ILOCK_STATE::ILOCK_GOOD ] = ENUM_TO_STRING(ILOCK_STATE::ILOCK_GOOD);
    m[ILOCK_STATE::ILOCK_BAD  ] = ENUM_TO_STRING(ILOCK_STATE::ILOCK_BAD);
    m[ILOCK_STATE::ILOCK_ERROR] = ENUM_TO_STRING(ILOCK_STATE::ILOCK_ERROR);
    return enumStringMapToPythonDict(m);
}
#endif //BUILD_DLL
