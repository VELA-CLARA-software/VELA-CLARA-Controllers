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

#include "controller.h"
// stl
#include <iostream>
#include <sstream>

controller::controller(bool show_messages, bool show_debug_messages):
SHOW_DEBUG_MESSAGES(show_debug_messages),
SHOW_MESSAGES(show_messages),
baseObject(&SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES)
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
controller::~controller(){}
//______________________________________________________________________________
void controller::debugMessagesOn()
{
    std::cout << "debugMessages On" << std::endl;
    SHOW_DEBUG_MESSAGES = true;
}
//______________________________________________________________________________
void controller::debugMessagesOff()
{
    std::cout << "debugMessages Off" << std::endl;
    SHOW_DEBUG_MESSAGES = false;
}
//______________________________________________________________________________
void controller::messagesOn()
{
    std::cout << "messages On" << std::endl;
    SHOW_MESSAGES = true;
}
//______________________________________________________________________________
void controller::messagesOff()
{
    std::cout << "messages Off" << std::endl;
    SHOW_MESSAGES = false;
}
//______________________________________________________________________________
void controller::silence()
{
    SHOW_DEBUG_MESSAGES = false;
    SHOW_MESSAGES = false;
}
//______________________________________________________________________________
void controller::verbose()
{
    SHOW_DEBUG_MESSAGES = true;
    SHOW_MESSAGES = true;
}

bool controller::isSilent()
{
    if(!SHOW_DEBUG_MESSAGES && !SHOW_MESSAGES )
        return true;
    else
        return false;
}
bool controller::isVerbose()
{
    return !isSilent();
}
bool controller::isMessageOn()
{
    if(SHOW_MESSAGES )
        return true;
    else
        return false;
}
bool controller::isDebugMessageOn()
{
    if(SHOW_DEBUG_MESSAGES )
        return true;
    else
        return false;
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict controller::getILockStatesDefinition()
{
    std::map< VELA_ENUM::ILOCK_STATE,  std::string  > m;
    m[ VELA_ENUM::ILOCK_STATE::ILOCK_BAD   ] = ENUM_TO_STRING(VELA_ENUM::ILOCK_STATE::ILOCK_BAD   );
    m[ VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  ] = ENUM_TO_STRING(VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  );
    m[ VELA_ENUM::ILOCK_STATE::ILOCK_ERROR ] = ENUM_TO_STRING(VELA_ENUM::ILOCK_STATE::ILOCK_ERROR );
    return enumStringMapToPythonDict(m );
}
#endif //BUILD_DLL
