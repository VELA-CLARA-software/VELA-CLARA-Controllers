#include "VCscopes.h"
// stl
#include <iostream>

VCscopes::VCscopes():
VCbase("VCscopes"),
virtual_VELA_INJ_Scope_Controller_Obj(nullptr),
offline_VELA_INJ_Scope_Controller_Obj(nullptr),
physical_VELA_INJ_Scope_Controller_Obj(nullptr),
virtual_VELA_BA1_Scope_Controller_Obj(nullptr),
offline_VELA_BA1_Scope_Controller_Obj(nullptr),
physical_VELA_BA1_Scope_Controller_Obj(nullptr),
virtual_VELA_BA2_Scope_Controller_Obj(nullptr),
offline_VELA_BA2_Scope_Controller_Obj(nullptr),
physical_VELA_BA2_Scope_Controller_Obj(nullptr),
virtual_CLARA_PH1_Scope_Controller_Obj(nullptr),
offline_CLARA_PH1_Scope_Controller_Obj(nullptr),
physical_CLARA_PH1_Scope_Controller_Obj(nullptr)
{
    //ctor
}
//______________________________________________________________________________
VCscopes::~VCscopes()
{
    if(virtual_VELA_INJ_Scope_Controller_Obj)
    {
        delete virtual_VELA_INJ_Scope_Controller_Obj;
               virtual_VELA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Scope_Controller_Obj)
    {
        delete offline_VELA_INJ_Scope_Controller_Obj;
               offline_VELA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Scope_Controller_Obj)
    {
        delete physical_VELA_INJ_Scope_Controller_Obj;
               physical_VELA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_Scope_Controller_Obj)
    {
        delete virtual_VELA_BA1_Scope_Controller_Obj;
               virtual_VELA_BA1_Scope_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_Scope_Controller_Obj)
    {
        delete offline_VELA_BA1_Scope_Controller_Obj;
               offline_VELA_BA1_Scope_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_Scope_Controller_Obj)
    {
        delete physical_VELA_BA1_Scope_Controller_Obj;
               physical_VELA_BA1_Scope_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_Scope_Controller_Obj)
    {
        delete virtual_VELA_BA2_Scope_Controller_Obj;
               virtual_VELA_BA2_Scope_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Scope_Controller_Obj)
    {
        delete offline_VELA_BA2_Scope_Controller_Obj;
               offline_VELA_BA2_Scope_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Scope_Controller_Obj)
    {
        delete physical_VELA_BA2_Scope_Controller_Obj;
               physical_VELA_BA2_Scope_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_PH1_Scope_Controller_Obj)
    {
        delete virtual_CLARA_PH1_Scope_Controller_Obj;
               virtual_CLARA_PH1_Scope_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_Scope_Controller_Obj)
    {
        delete offline_CLARA_PH1_Scope_Controller_Obj;
               offline_CLARA_PH1_Scope_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_Scope_Controller_Obj)
    {
        delete physical_CLARA_PH1_Scope_Controller_Obj;
               physical_CLARA_PH1_Scope_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
scopeController& VCscopes::getController(scopeController*& cont,
                                         const std::string& conf1,
                                         const std::string& conf2,
                                         const std::string & name,
                                         const bool shouldVM,
                                         const bool shouldEPICS,
                                         const HWC_ENUM::MACHINE_AREA myMachineArea)
{
    if(cont)
    {
        std::cout << name  <<" object already exists," <<std::endl;
    }
    else
    {
        std::cout <<"Creating " <<name <<" object" <<std::endl;
        messageStates[cont].first  = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;
        switch(myMachineArea)
        {
            case HWC_ENUM::MACHINE_AREA::VELA_INJ:
                cont = new scopeController(conf1,
                                           conf2,
                                           &messageStates.at(cont).first,
                                           &messageStates.at(cont).second,
                                           shouldEPICS,
                                           shouldVM,
                                           myMachineArea);
                break;
            default:
                std::cout << name  <<" can't be created just yet," <<std::endl;
        }
    }
    return *cont;
}
//______________________________________________________________________________
scopeController& VCscopes::virtual_VELA_INJ_Scope_Controller()
{
    std::string name  = "virtual_VELA_INJ_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_INJ_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_INJ_Scope_Controller()
{
    std::string name  = "virtual_VELA_INJ_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_INJ_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_INJ_Scope_Controller()
{
    std::string name  = "virtual_VELA_INJ_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_INJ_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_VELA_BA1_Scope_Controller()
{
    std::string name  = "virtual_VELA_INJ_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_INJ_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_BA1_Scope_Controller()
{
    std::string name  = "virtual_VELA_BA1_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_INJ_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_BA1_Scope_Controller()
{
    std::string name  = "virtual_VELA_BA1_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_BA1_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_VELA_BA2_Scope_Controller()
{
    std::string name  = "virtual_VELA_BA2_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_BA2_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_BA2_Scope_Controller()
{
    std::string name  = "virtual_VELA_BA2_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_BA2_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_BA2_Scope_Controller()
{
    std::string name  = "virtual_VELA_BA2_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_BA2_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_CLARA_PH1_Scope_Controller()
{
    std::string name  = "virtual_CLARA_PH1_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_CLARA_PH1_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
scopeController & VCscopes::offline_CLARA_PH1_Scope_Controller()
{
    std::string name  = "offline_CLARA_PH1_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(offline_CLARA_PH1_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
scopeController & VCscopes::physical_CLARA_PH1_Scope_Controller()
{
    std::string name  = "physical_CLARA_PH1_Scope_Controller";
    const std::string scopeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(physical_CLARA_PH1_Scope_Controller_Obj,
                         scopeconf1,
                         scopeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
scopeController & VCscopes::getScopeController( HWC_ENUM::MACHINE_MODE mode, HWC_ENUM::MACHINE_AREA area )
{

    if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_INJ )
        return offline_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_INJ )
        return physical_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA1 )
        return offline_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA1 )
        return virtual_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA1 )
        return physical_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA2 )
        return offline_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA2 )
        return virtual_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA2 )
        return physical_VELA_INJ_Scope_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_Scope_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_Scope_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_Scope_Controller();
}
//______________________________________________________________________________
void VCscopes::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}
//______________________________________________________________________________
#ifdef BUILD_DLL
#endif //BUILD_DLL



