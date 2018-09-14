#include "VCblm.h"
// stl
#include <iostream>

VCblm::VCblm():
VCbase("VCblm"),
virtual_VELA_INJ_BLM_Controller_Obj(nullptr),
offline_VELA_INJ_BLM_Controller_Obj(nullptr),
physical_VELA_INJ_BLM_Controller_Obj(nullptr),
virtual_VELA_BA1_BLM_Controller_Obj(nullptr),
offline_VELA_BA1_BLM_Controller_Obj(nullptr),
physical_VELA_BA1_BLM_Controller_Obj(nullptr),
virtual_VELA_BA2_BLM_Controller_Obj(nullptr),
offline_VELA_BA2_BLM_Controller_Obj(nullptr),
physical_VELA_BA2_BLM_Controller_Obj(nullptr),
virtual_CLARA_PH1_BLM_Controller_Obj(nullptr),
offline_CLARA_PH1_BLM_Controller_Obj(nullptr),
physical_CLARA_PH1_BLM_Controller_Obj(nullptr),
virtual_C2B_BLM_Controller_Obj(nullptr),
offline_C2B_BLM_Controller_Obj(nullptr),
physical_C2B_BLM_Controller_Obj(nullptr)
{
    //ctor
}
//______________________________________________________________________________
VCblm::~VCblm()
{
    if(virtual_VELA_INJ_BLM_Controller_Obj)
    {
        delete virtual_VELA_INJ_BLM_Controller_Obj;
               virtual_VELA_INJ_BLM_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_BLM_Controller_Obj)
    {
        delete offline_VELA_INJ_BLM_Controller_Obj;
               offline_VELA_INJ_BLM_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_BLM_Controller_Obj)
    {
        delete physical_VELA_INJ_BLM_Controller_Obj;
               physical_VELA_INJ_BLM_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_BLM_Controller_Obj)
    {
        delete virtual_VELA_BA1_BLM_Controller_Obj;
               virtual_VELA_BA1_BLM_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_BLM_Controller_Obj)
    {
        delete offline_VELA_BA1_BLM_Controller_Obj;
               offline_VELA_BA1_BLM_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_BLM_Controller_Obj)
    {
        delete physical_VELA_BA1_BLM_Controller_Obj;
               physical_VELA_BA1_BLM_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_BLM_Controller_Obj)
    {
        delete virtual_VELA_BA2_BLM_Controller_Obj;
               virtual_VELA_BA2_BLM_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_BLM_Controller_Obj)
    {
        delete offline_VELA_BA2_BLM_Controller_Obj;
               offline_VELA_BA2_BLM_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_BLM_Controller_Obj)
    {
        delete physical_VELA_BA2_BLM_Controller_Obj;
               physical_VELA_BA2_BLM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_PH1_BLM_Controller_Obj)
    {
        delete virtual_CLARA_PH1_BLM_Controller_Obj;
               virtual_CLARA_PH1_BLM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_BLM_Controller_Obj)
    {
        delete offline_CLARA_PH1_BLM_Controller_Obj;
               offline_CLARA_PH1_BLM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_BLM_Controller_Obj)
    {
        delete physical_CLARA_PH1_BLM_Controller_Obj;
               physical_CLARA_PH1_BLM_Controller_Obj = nullptr;
    }
    if(virtual_C2B_BLM_Controller_Obj)
    {
        delete virtual_C2B_BLM_Controller_Obj;
               virtual_C2B_BLM_Controller_Obj = nullptr;
    }
    if(offline_C2B_BLM_Controller_Obj)
    {
        delete offline_C2B_BLM_Controller_Obj;
               offline_C2B_BLM_Controller_Obj = nullptr;
    }
    if(physical_C2B_BLM_Controller_Obj)
    {
        delete physical_C2B_BLM_Controller_Obj;
               physical_C2B_BLM_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
blmController& VCblm::getController(blmController*& cont,
                                         const std::string& conf1,
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
            case HWC_ENUM::MACHINE_AREA::CLARA_PH1:
                cont = new blmController(conf1,
                                           messageStates.at(cont).first,
                                           messageStates.at(cont).second,
                                           shouldEPICS,
                                           shouldVM,
                                           myMachineArea);
                break;
            case HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2:
                cont = new blmController(conf1,
                                           messageStates.at(cont).first,
                                           messageStates.at(cont).second,
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
blmController& VCblm::virtual_VELA_INJ_BLM_Controller()
{
    std::string name  = "virtual_VELA_INJ_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_INJ_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
blmController & VCblm::offline_VELA_INJ_BLM_Controller()
{
    std::string name  = "offline_VELA_INJ_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(offline_VELA_INJ_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
blmController & VCblm::physical_VELA_INJ_BLM_Controller()
{
    std::string name  = "physical_VELA_INJ_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(physical_VELA_INJ_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
blmController & VCblm::virtual_VELA_BA1_BLM_Controller()
{
    std::string name  = "virtual_VELA_BA1_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_BA1_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
blmController & VCblm::offline_VELA_BA1_BLM_Controller()
{
    std::string name  = "offline_VELA_BA1_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(offline_VELA_BA1_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
blmController & VCblm::physical_VELA_BA1_BLM_Controller()
{
    std::string name  = "physical_VELA_BA1_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(physical_VELA_BA1_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
blmController & VCblm::virtual_VELA_BA2_BLM_Controller()
{
    std::string name  = "virtual_VELA_BA2_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_VELA_BA2_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
blmController & VCblm::offline_VELA_BA2_BLM_Controller()
{
    std::string name  = "offline_VELA_BA2_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(offline_VELA_BA2_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
blmController & VCblm::physical_VELA_BA2_BLM_Controller()
{
    std::string name  = "physical_VELA_BA2_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(physical_VELA_BA2_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
blmController & VCblm::virtual_CLARA_PH1_BLM_Controller()
{
    std::string name  = "virtual_CLARA_PH1_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_CLARA_PH1_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
blmController & VCblm::offline_CLARA_PH1_BLM_Controller()
{
    std::string name  = "offline_CLARA_PH1_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(offline_CLARA_PH1_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
blmController & VCblm::physical_CLARA_PH1_BLM_Controller()
{
    std::string name  = "physical_CLARA_PH1_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(physical_CLARA_PH1_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
blmController & VCblm::virtual_C2B_BLM_Controller()
{
    std::string name  = "virtual_C2B_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(virtual_C2B_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
blmController & VCblm::offline_C2B_BLM_Controller()
{
    std::string name  = "offline_C2B_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(offline_C2B_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
blmController & VCblm::physical_C2B_BLM_Controller()
{
    std::string name  = "physical_C2B_BLM_Controller";
    const std::string blmconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;
    return getController(physical_C2B_BLM_Controller_Obj,
                         blmconf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
blmController & VCblm::getBLMController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area )
{

    if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_INJ )
        return offline_VELA_INJ_BLM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_INJ )
        return virtual_VELA_INJ_BLM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_INJ )
        return physical_VELA_INJ_BLM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA1 )
        return offline_VELA_BA1_BLM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_BLM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA1 )
        return physical_VELA_BA1_BLM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA2 )
        return offline_VELA_BA2_BLM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_BLM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA2 )
        return physical_VELA_BA2_BLM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_BLM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_BLM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_BLM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return offline_C2B_BLM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return virtual_C2B_BLM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return physical_C2B_BLM_Controller();
}
//______________________________________________________________________________
void VCblm::updateMessageStates()
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



