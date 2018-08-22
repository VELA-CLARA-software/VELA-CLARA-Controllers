#include "VCBPMs.h"
// stl

#include <iostream>

VCBPMs::VCBPMs():
VCbase("VCBPMs"),
virtual_VELA_INJ_BPM_Controller_Obj(nullptr),
offline_VELA_INJ_BPM_Controller_Obj(nullptr),
physical_VELA_INJ_BPM_Controller_Obj(nullptr),
virtual_VELA_BA1_BPM_Controller_Obj(nullptr),
offline_VELA_BA1_BPM_Controller_Obj(nullptr),
physical_VELA_BA1_BPM_Controller_Obj(nullptr),
virtual_VELA_BA2_BPM_Controller_Obj(nullptr),
offline_VELA_BA2_BPM_Controller_Obj(nullptr),
physical_VELA_BA2_BPM_Controller_Obj(nullptr),
virtual_CLARA_PH1_BPM_Controller_Obj(nullptr),
offline_CLARA_PH1_BPM_Controller_Obj(nullptr),
physical_CLARA_PH1_BPM_Controller_Obj(nullptr),
virtual_C2B_BPM_Controller_Obj(nullptr),
offline_C2B_BPM_Controller_Obj(nullptr),
physical_C2B_BPM_Controller_Obj(nullptr)
{
    //ctor
}
//______________________________________________________________________________
VCBPMs::~VCBPMs()
{
    if(virtual_VELA_INJ_BPM_Controller_Obj)
    {
        delete virtual_VELA_INJ_BPM_Controller_Obj;
               virtual_VELA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_BPM_Controller_Obj)
    {
        delete offline_VELA_INJ_BPM_Controller_Obj;
               offline_VELA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_BPM_Controller_Obj)
    {
        delete physical_VELA_INJ_BPM_Controller_Obj;
               physical_VELA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_BPM_Controller_Obj)
    {
        delete virtual_VELA_BA1_BPM_Controller_Obj;
               virtual_VELA_BA1_BPM_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_BPM_Controller_Obj)
    {
        delete offline_VELA_BA1_BPM_Controller_Obj;
               offline_VELA_BA1_BPM_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_BPM_Controller_Obj)
    {
        delete physical_VELA_BA1_BPM_Controller_Obj;
               physical_VELA_BA1_BPM_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_BPM_Controller_Obj)
    {
        delete virtual_VELA_BA2_BPM_Controller_Obj;
               virtual_VELA_BA2_BPM_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_BPM_Controller_Obj)
    {
        delete offline_VELA_BA2_BPM_Controller_Obj;
               offline_VELA_BA2_BPM_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_BPM_Controller_Obj)
    {
        delete physical_VELA_BA2_BPM_Controller_Obj;
               physical_VELA_BA2_BPM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_PH1_BPM_Controller_Obj)
    {
        delete virtual_CLARA_PH1_BPM_Controller_Obj;
               virtual_CLARA_PH1_BPM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_BPM_Controller_Obj)
    {
        delete offline_CLARA_PH1_BPM_Controller_Obj;
               offline_CLARA_PH1_BPM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_BPM_Controller_Obj)
    {
        delete physical_CLARA_PH1_BPM_Controller_Obj;
               physical_CLARA_PH1_BPM_Controller_Obj = nullptr;
    }
    if(virtual_C2B_BPM_Controller_Obj)
    {
        delete virtual_C2B_BPM_Controller_Obj;
               virtual_C2B_BPM_Controller_Obj = nullptr;
    }
    if(offline_C2B_BPM_Controller_Obj)
    {
        delete offline_C2B_BPM_Controller_Obj;
               offline_C2B_BPM_Controller_Obj = nullptr;
    }
    if(physical_C2B_BPM_Controller_Obj)
    {
        delete physical_C2B_BPM_Controller_Obj;
               physical_C2B_BPM_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::getController(beamPositionMonitorController*& cont,
                                         const std::string& conf,
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
        cont = new beamPositionMonitorController(conf,
                                                 messageStates.at(cont).first,
                                                 messageStates.at(cont).second,
                                                 shouldEPICS,
                                                 shouldVM,
                                                 myMachineArea);
    }
    return *cont;
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::virtual_VELA_INJ_BPM_Controller()
{
    std::string name  = "virtual_VELA_INJ_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_INJ_BPM_CONFIG;
    return getController(virtual_VELA_INJ_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::offline_VELA_INJ_BPM_Controller()
{
    std::string name  = "offline_VELA_INJ_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_INJ_BPM_CONFIG;
    return getController(offline_VELA_INJ_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::physical_VELA_INJ_BPM_Controller()
{
    std::string name  = "physical_VELA_INJ_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_INJ_BPM_CONFIG;
    return getController(physical_VELA_INJ_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::virtual_VELA_BA1_BPM_Controller()
{
    std::string name  = "virtual_VELA_BA1_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA1_BPM_CONFIG;
    return getController(virtual_VELA_BA1_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::offline_VELA_BA1_BPM_Controller()
{
    std::string name  = "offline_VELA_BA1_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA1_BPM_CONFIG;
    return getController(offline_VELA_BA1_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::physical_VELA_BA1_BPM_Controller()
{
    std::string name  = "physical_VELA_BA1_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA1_BPM_CONFIG;
    return getController(physical_VELA_BA1_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::virtual_VELA_BA2_BPM_Controller()
{
    std::string name  = "virtual_VELA_BA2_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA2_BPM_CONFIG;
    return getController(virtual_VELA_BA2_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::offline_VELA_BA2_BPM_Controller()
{
    std::string name  = "offline_VELA_BA2_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA2_BPM_CONFIG;
    return getController(offline_VELA_BA2_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::physical_VELA_BA2_BPM_Controller()
{
    std::string name  = "physical_VELA_BA2_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA2_BPM_CONFIG;
    return getController(physical_VELA_BA2_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::virtual_CLARA_PH1_BPM_Controller()
{
    std::string name  = "virtual_CLARA_PH1_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_PH1_BPM_CONFIG;
    return getController(virtual_CLARA_PH1_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::offline_CLARA_PH1_BPM_Controller()
{
    std::string name  = "offline_CLARA_PH1_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_PH1_BPM_CONFIG;
    return getController(offline_CLARA_PH1_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::physical_CLARA_PH1_BPM_Controller()
{
    std::string name  = "physical_CLARA_PH1_BPM_Controller";
    std::cout << name << std::endl;
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_PH1_BPM_CONFIG;
    return getController(physical_CLARA_PH1_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::virtual_C2B_BPM_Controller()
{
    std::string name  = "virtual_C2B_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::C2B_BPM_CONFIG;
    return getController(virtual_C2B_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::offline_C2B_BPM_Controller()
{
    std::string name  = "offline_C2B_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::C2B_BPM_CONFIG;
    return getController(offline_C2B_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
beamPositionMonitorController& VCBPMs::physical_C2B_BPM_Controller()
{
    std::string name  = "physical_C2B_BPM_Controller";
    const std::string bpmconf = UTL::APCLARA1_CONFIG_PATH + UTL::C2B_BPM_CONFIG;
    return getController(physical_C2B_BPM_Controller_Obj,
                         bpmconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::getBPMController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area )
{
    if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_INJ )
        return offline_VELA_INJ_BPM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_INJ )
        return virtual_VELA_INJ_BPM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_INJ )
        return physical_VELA_INJ_BPM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA1 )
        return offline_VELA_BA1_BPM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_BPM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA1 )
        return physical_VELA_BA1_BPM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA2 )
        return offline_VELA_BA2_BPM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_BPM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA2 )
        return physical_VELA_BA2_BPM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_BPM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_BPM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_BPM_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return offline_C2B_BPM_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return virtual_C2B_BPM_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return physical_C2B_BPM_Controller();
}
//______________________________________________________________________________
void VCBPMs::updateMessageStates()
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
