#include "l01ModController.h"

l01ModController::l01ModController(
    bool& show_messages,
    bool& show_debug_messages,
    const std::string & l01ModConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(l01ModConf,startVirtualMachine,SHOW_MESSAGES,SHOW_DEBUG_MESSAGES,shouldStartEPICs)
//shouldStartEPICs(shouldStartEPICs)
{
    //ctor
}
//____________________________________________________________________________________________
l01ModController::~l01ModController()
{
    //dtor
}
//____________________________________________________________________________________________
const rfModStructs::l01ModObject& l01ModController::getObjConstRef()
{
    return localInterface.getObjConstRef();
}
//____________________________________________________________________________________________
bool l01ModController::reset()
{
    return localInterface.reset();
}
//____________________________________________________________________________________________
double l01ModController::get_CA_PEND_IO_TIMEOUT()const
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void l01ModController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> l01ModController::getILockStates(const std::string& name)
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,std::string> l01ModController::getILockStatesStr(const std::string& name)
{
    return localInterface.getILockStatesStr(name);
}
bool l01ModController::setOff()
{
    return localInterface.setOff();
}
