#include "VCvacValves.h"
// stl

#include <iostream>

VCvacValves::VCvacValves():
VCbase("VCvacValves"),
virtual_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
virtual_VELA_BA1_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_BA1_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_BA1_Vac_Valve_Controller_Obj(nullptr),
virtual_VELA_BA2_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_BA2_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_BA2_Vac_Valve_Controller_Obj(nullptr),
virtual_CLARA_PH1_Vac_Valve_Controller_Obj(nullptr),
offline_CLARA_PH1_Vac_Valve_Controller_Obj(nullptr),
physical_CLARA_PH1_Vac_Valve_Controller_Obj(nullptr),
virtual_Vac_Valve_Controller_Obj(nullptr),
offline_Vac_Valve_Controller_Obj(nullptr),
physical_Vac_Valve_Controller_Obj(nullptr)
{
    //ctor
}
//______________________________________________________________________________
VCvacValves::~VCvacValves()
{
/// VELA_INJ
    if(virtual_VELA_INJ_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for virtual_VELA_INJ_Vac_Valve_Controller_Obj called" << std::endl;
        delete virtual_VELA_INJ_Vac_Valve_Controller_Obj;
               virtual_VELA_INJ_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for offline_VELA_INJ_Vac_Valve_Controller_Obj called" << std::endl;
        delete offline_VELA_INJ_Vac_Valve_Controller_Obj;
               offline_VELA_INJ_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for physical_VELA_INJ_Vac_Valve_Controller_Obj called" << std::endl;
        delete physical_VELA_INJ_Vac_Valve_Controller_Obj;
               physical_VELA_INJ_Vac_Valve_Controller_Obj = nullptr;
    }

/// VELA_BA1
    if(virtual_VELA_BA1_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for virtual_VELA_BA1_Vac_Valve_Controller_Obj called" << std::endl;
        delete virtual_VELA_BA1_Vac_Valve_Controller_Obj;
               virtual_VELA_BA1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for offline_VELA_BA1_Vac_Valve_Controller_Obj called" << std::endl;
        delete offline_VELA_BA1_Vac_Valve_Controller_Obj;
               offline_VELA_BA1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for physical_VELA_BA1_Vac_Valve_Controller_Obj called" << std::endl;
        delete physical_VELA_BA1_Vac_Valve_Controller_Obj;
               physical_VELA_BA1_Vac_Valve_Controller_Obj = nullptr;
    }

/// VELA_BA2
    if(virtual_VELA_BA2_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for virtual_VELA_BA2_Vac_Valve_Controller_Obj called" << std::endl;
        delete virtual_VELA_BA2_Vac_Valve_Controller_Obj;
               virtual_VELA_BA2_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for offline_VELA_BA2_Vac_Valve_Controller_Obj called" << std::endl;
        delete offline_VELA_BA2_Vac_Valve_Controller_Obj;
               offline_VELA_BA2_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for physical_VELA_BA2_Vac_Valve_Controller_Obj called" << std::endl;
        delete physical_VELA_BA2_Vac_Valve_Controller_Obj;
               physical_VELA_BA2_Vac_Valve_Controller_Obj = nullptr;
    }

/// CLARA_S01
    if(virtual_CLARA_PH1_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for physical_CLARA_PH1_Vac_Valve_Controller_Obj called" << std::endl;
        delete virtual_CLARA_PH1_Vac_Valve_Controller_Obj;
               virtual_CLARA_PH1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for offline_CLARA_PH1_Vac_Valve_Controller_Obj called" << std::endl;
        delete offline_CLARA_PH1_Vac_Valve_Controller_Obj;
               offline_CLARA_PH1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_Vac_Valve_Controller_Obj)
    {
//        THIS IS THE CULPRIT - ASK DUNCAN ABOUT DESTRUCTORS
        std::cout << "Destructor for physical_CLARA_PH1_Vac_Valve_Controller_Obj called" << std::endl;
        delete physical_CLARA_PH1_Vac_Valve_Controller_Obj;
               physical_CLARA_PH1_Vac_Valve_Controller_Obj = nullptr;
    }

/// ALL_VELA_CLARA
    if(virtual_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for physical_Vac_Valve_Controller_Obj called" << std::endl;
        delete virtual_Vac_Valve_Controller_Obj;
               virtual_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_Vac_Valve_Controller_Obj)
    {
        std::cout << "Destructor for offline_Vac_Valve_Controller_Obj called" << std::endl;
        delete offline_Vac_Valve_Controller_Obj;
               offline_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_Vac_Valve_Controller_Obj)
    {
//        THIS IS THE CULPRIT - ASK DUNCAN ABOUT DESTRUCTORS
        std::cout << "Destructor for physical_Vac_Valve_Controller_Obj called" << std::endl;
        delete physical_Vac_Valve_Controller_Obj;
               physical_Vac_Valve_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
vacuumValveController& VCvacValves::getController(vacuumValveController*& cont,
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
            case HWC_ENUM::MACHINE_AREA::VELA_INJ:
                cont = new vacuumValveController(conf1,
                                                 messageStates.at(cont).first,
                                                 messageStates.at(cont).second,
                                                 shouldEPICS,
                                                 shouldVM,
                                                 myMachineArea);
                 std::cout << " CONTROLLER CONSTRUCTED FOR VELA INJ" << std::endl;
                 break;
            case HWC_ENUM::MACHINE_AREA::CLARA_PH1:
                cont = new vacuumValveController(conf1,
                                                 messageStates.at(cont).first,
                                                 messageStates.at(cont).second,
                                                 shouldEPICS,
                                                 shouldVM,
                                                 myMachineArea);
                break;
            case HWC_ENUM::MACHINE_AREA::ALL_VELA_CLARA:
                cont = new vacuumValveController(conf1,
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
vacuumValveController& VCvacValves::virtual_VELA_INJ_Vac_Valve_Controller()
{
    std::string name  = "virtual_VELA_INJ_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;
    return getController(virtual_VELA_INJ_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::offline_VELA_INJ_Vac_Valve_Controller()
{
    std::string name  = "offline_VELA_INJ_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;
    return getController(offline_VELA_INJ_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::physical_VELA_INJ_Vac_Valve_Controller()
{
    std::string name  = "physical_VELA_INJ_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;
    std::cout << conf1 << std::endl;
    return getController(physical_VELA_INJ_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::virtual_VELA_BA1_Vac_Valve_Controller()
{
    std::string name  = "virtual_VELA_BA1_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;
    return getController(virtual_VELA_BA1_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::offline_VELA_BA1_Vac_Valve_Controller()
{
    std::string name  = "offline_VELA_BA1_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;
    return getController(offline_VELA_BA1_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::physical_VELA_BA1_Vac_Valve_Controller()
{
    std::string name  = "physical_VELA_BA1_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;
    return getController(physical_VELA_INJ_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::virtual_VELA_BA2_Vac_Valve_Controller()
{
    std::string name  = "virtual_VELA_BA2_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;
    return getController(virtual_VELA_BA2_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::offline_VELA_BA2_Vac_Valve_Controller()
{
    std::string name  = "offline_VELA_BA2_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;
    return getController(offline_VELA_BA2_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::physical_VELA_BA2_Vac_Valve_Controller()
{
    std::string name  = "physical_VELA_BA2_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;
    return getController(physical_VELA_BA2_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::virtual_CLARA_PH1_Vac_Valve_Controller()
{
    std::string name  = "virtual_CLARA_PH1_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_PH1_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_VALVE_CONFIG;
    return getController(virtual_CLARA_PH1_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::offline_CLARA_PH1_Vac_Valve_Controller()
{
    std::string name  = "offline_CLARA_PH1_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_PH1_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_VALVE_CONFIG;
    return getController(offline_CLARA_PH1_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::physical_CLARA_PH1_Vac_Valve_Controller()
{
    std::string name  = "physical_CLARA_PH1_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_PH1_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_VALVE_CONFIG;
    return getController(physical_CLARA_PH1_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
//______________________________________________________________________________
vacuumValveController& VCvacValves::virtual_Vac_Valve_Controller()
{
    std::string name  = "virtual_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::ALL_VAC_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::ALL_VAC_VALVE_CONFIG;
    return getController(virtual_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::ALL_VELA_CLARA);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::offline_Vac_Valve_Controller()
{
    std::string name  = "offline_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::ALL_VAC_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::ALL_VAC_VALVE_CONFIG;
    return getController(offline_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withoutVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::ALL_VELA_CLARA);
}
//______________________________________________________________________________
vacuumValveController& VCvacValves::physical_Vac_Valve_Controller()
{
    std::string name  = "physical_Vac_Valve_Controller";
    //const std::string conf1 = UTL::APCLARA1_CONFIG_PATH + UTL::ALL_VAC_VALVE_CONFIG;
    const std::string conf1 = UTL::CLARANET_CONFIG_PATH + UTL::ALL_VAC_VALVE_CONFIG;
    return getController(physical_Vac_Valve_Controller_Obj,
                         conf1,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::ALL_VELA_CLARA);
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::getVacValveController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area )
{
    if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_INJ )
        return offline_VELA_INJ_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_INJ )
        return physical_VELA_INJ_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA1 )
        return offline_VELA_BA1_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA1 )
        return physical_VELA_BA1_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA2 )
        return offline_VELA_BA2_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA2 )
        return physical_VELA_BA2_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_Vac_Valve_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_Vac_Valve_Controller();
}
//______________________________________________________________________________
void VCvacValves::updateMessageStates()
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



