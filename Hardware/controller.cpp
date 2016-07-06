#include "controller.h"
// stl
#include <iostream>
#include <sstream>

controller::controller() : SHOW_DEBUG_MESSAGES( true ) , SHOW_MESSAGES( true ), baseObject( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES ){}
controller::controller(bool show_messages, bool show_debug_messages):SHOW_DEBUG_MESSAGES(show_debug_messages),
SHOW_MESSAGES(show_messages), baseObject( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES ){}
//______________________________________________________________________________
controller::~controller(){}
//______________________________________________________________________________
void controller::debugMessagesOn()
{
    SHOW_DEBUG_MESSAGES = true;
}
//______________________________________________________________________________
void controller::debugMessagesOff()
{
    SHOW_DEBUG_MESSAGES = false;
}
//______________________________________________________________________________
void controller::messagesOn()
{
    SHOW_MESSAGES = true;
}
//______________________________________________________________________________
void controller::messagesOff()
{
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
    if( !SHOW_DEBUG_MESSAGES && !SHOW_MESSAGES )
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
    if( SHOW_MESSAGES )
        return true;
    else
        return false;
}
bool controller::isDebugMessageOn()
{
    if( SHOW_DEBUG_MESSAGES )
        return true;
    else
        return false;
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict controller::getILockStatesDefinition()
{
    std::map< VELA_ENUM::ILOCK_STATE,  std::string  > m;
    m[ VELA_ENUM::ILOCK_STATE::ILOCK_BAD   ] = ENUM_TO_STRING( VELA_ENUM::ILOCK_STATE::ILOCK_BAD   );
    m[ VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  ] = ENUM_TO_STRING( VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  );
    m[ VELA_ENUM::ILOCK_STATE::ILOCK_ERROR ] = ENUM_TO_STRING( VELA_ENUM::ILOCK_STATE::ILOCK_ERROR );
    return enumStringMapToPythonDict( m );
}
#endif //BUILD_DLL
