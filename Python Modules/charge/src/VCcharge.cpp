#include "VCcharge.h"
// stl
#include <iostream>

VCcharge::VCcharge():
VCbase("VCcharge"),
virtual_VELA_INJ_Charge_Controller_Obj(nullptr),
offline_VELA_INJ_Charge_Controller_Obj(nullptr),
physical_VELA_INJ_Charge_Controller_Obj(nullptr),
virtual_VELA_BA1_Charge_Controller_Obj(nullptr),
offline_VELA_BA1_Charge_Controller_Obj(nullptr),
physical_VELA_BA1_Charge_Controller_Obj(nullptr),
virtual_VELA_BA2_Charge_Controller_Obj(nullptr),
offline_VELA_BA2_Charge_Controller_Obj(nullptr),
physical_VELA_BA2_Charge_Controller_Obj(nullptr),
virtual_CLARA_PH1_Charge_Controller_Obj(nullptr),
offline_CLARA_PH1_Charge_Controller_Obj(nullptr),
physical_CLARA_PH1_Charge_Controller_Obj(nullptr)
{
    //ctor
}
//______________________________________________________________________________
VCcharges::~VCcharges()
{
    if(virtual_VELA_INJ_Charge_Controller_Obj)
    {
        delete virtual_VELA_INJ_Charge_Controller_Obj;
               virtual_VELA_INJ_Charge_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Charge_Controller_Obj)
    {
        delete offline_VELA_INJ_Charge_Controller_Obj;
               offline_VELA_INJ_Charge_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Charge_Controller_Obj)
    {
        delete physical_VELA_INJ_Charge_Controller_Obj;
               physical_VELA_INJ_Charge_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_Charge_Controller_Obj)
    {
        delete virtual_VELA_BA1_Charge_Controller_Obj;
               virtual_VELA_BA1_Charge_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_Charge_Controller_Obj)
    {
        delete offline_VELA_BA1_Charge_Controller_Obj;
               offline_VELA_BA1_Charge_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_Charge_Controller_Obj)
    {
        delete physical_VELA_BA1_Charge_Controller_Obj;
               physical_VELA_BA1_Charge_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_Charge_Controller_Obj)
    {
        delete virtual_VELA_BA2_Charge_Controller_Obj;
               virtual_VELA_BA2_Charge_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Charge_Controller_Obj)
    {
        delete offline_VELA_BA2_Charge_Controller_Obj;
               offline_VELA_BA2_Charge_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Charge_Controller_Obj)
    {
        delete physical_VELA_BA2_Charge_Controller_Obj;
               physical_VELA_BA2_Charge_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_PH1_Charge_Controller_Obj)
    {
        delete virtual_CLARA_PH1_Charge_Controller_Obj;
               virtual_CLARA_PH1_Charge_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_Charge_Controller_Obj)
    {
        delete offline_CLARA_PH1_Charge_Controller_Obj;
               offline_CLARA_PH1_Charge_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_Charge_Controller_Obj)
    {
        delete physical_CLARA_PH1_Charge_Controller_Obj;
               physical_CLARA_PH1_Charge_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
chargeController& VCcharges::getController(chargeController*& cont,
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
                cont = new chargeController(conf1,
                                           conf2,
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
chargeController& VCcharges::virtual_VELA_INJ_Charge_Controller()
{
    std::string name  = "virtual_VELA_INJ_Charge_Controller";
    return getController(virtual_VELA_INJ_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
chargeController & VCcharges::offline_VELA_INJ_Charge_Controller()
{
    std::string name  = "offline_VELA_INJ_Charge_Controller";
    return getController(offline_VELA_INJ_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
chargeController & VCcharges::physical_VELA_INJ_Charge_Controller()
{
    std::string name  = "physical_VELA_INJ_Charge_Controller";
    return getController(physical_VELA_INJ_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
chargeController & VCcharges::virtual_VELA_BA1_Charge_Controller()
{
    std::string name  = "virtual_VELA_BA1_Charge_Controller";
    return getController(virtual_VELA_BA1_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
chargeController & VCcharges::offline_VELA_BA1_Charge_Controller()
{
    std::string name  = "offline_VELA_BA1_Charge_Controller";
    return getController(offline_VELA_BA1_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
chargeController & VCcharges::physical_VELA_BA1_Charge_Controller()
{
    std::string name  = "physical_VELA_BA1_Charge_Controller";
    return getController(physical_VELA_BA1_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
chargeController & VCcharges::virtual_VELA_BA2_Charge_Controller()
{
    std::string name  = "virtual_VELA_BA2_Charge_Controller";
    return getController(virtual_VELA_BA2_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
chargeController & VCcharges::offline_VELA_BA2_Charge_Controller()
{
    std::string name  = "offline_VELA_BA2_Charge_Controller";
    return getController(offline_VELA_BA2_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
chargeController & VCcharges::physical_VELA_BA2_Charge_Controller()
{
    std::string name  = "physical_VELA_BA2_Charge_Controller";
    return getController(physical_VELA_BA2_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
chargeController & VCcharges::virtual_CLARA_PH1_Charge_Controller()
{
    std::string name  = "virtual_CLARA_PH1_Charge_Controller";
    return getController(virtual_CLARA_PH1_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
chargeController & VCcharges::offline_CLARA_PH1_Charge_Controller()
{
    std::string name  = "offline_CLARA_PH1_Charge_Controller";
    const std::string chargeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_CHARGE_CONFIG;
    const std::string chargeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_CHARGE_TRACE_CONFIG;
    return getController(offline_CLARA_PH1_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
chargeController & VCcharges::physical_CLARA_PH1_Charge_Controller()
{
    std::string name  = "physical_CLARA_PH1_Charge_Controller";
    const std::string chargeconf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_CHARGE_CONFIG;
    const std::string chargeconf2 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_CHARGE_TRACE_CONFIG;
    return getController(physical_CLARA_PH1_Charge_Controller_Obj,
                         chargeconf1,
                         chargeconf2,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
chargeController & VCcharges::getchargeController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area )
{

    if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_INJ )
        return offline_VELA_INJ_Charge_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Charge_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_INJ )
        return physical_VELA_INJ_Charge_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA1 )
        return offline_VELA_BA1_Charge_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_Charge_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA1 )
        return physical_VELA_BA1_Charge_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA2 )
        return offline_VELA_BA2_Charge_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_Charge_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA2 )
        return physical_VELA_BA2_Charge_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_Charge_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_Charge_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_Charge_Controller();
}
//______________________________________________________________________________
void VCcharges::updateMessageStates()
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



