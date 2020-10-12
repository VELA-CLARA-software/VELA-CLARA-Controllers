#include "VCmagnets.h"
//stl
#include <iostream>
#include <string>
//
using namespace HWC_ENUM;
using cstr = const std::string;
//
VCmagnets::VCmagnets():
VCbase("VCmagnets"),
//virtual_VELA_INJ_Magnet_Controller_Obj(nullptr),
//virtual_VELA_BA1_Magnet_Controller_Obj(nullptr),
//virtual_VELA_BA2_Magnet_Controller_Obj(nullptr),
//virtual_CLARA_PH1_Magnet_Controller_Obj(nullptr),
//physical_VELA_INJ_Magnet_Controller_Obj(nullptr),
//physical_VELA_BA1_Magnet_Controller_Obj(nullptr),
//physical_VELA_BA2_Magnet_Controller_Obj(nullptr),
//physical_CLARA_PH1_Magnet_Controller_Obj(nullptr),
//offline_VELA_INJ_Magnet_Controller_Obj(nullptr),
virtual_VELA_INJ_Magnet_Controller_Obj(nullptr),
offline_VELA_INJ_Magnet_Controller_Obj(nullptr),
physical_VELA_INJ_Magnet_Controller_Obj(nullptr),
virtual_CLARA_PH1_Magnet_Controller_Obj(nullptr),
offline_CLARA_PH1_Magnet_Controller_Obj(nullptr),
physical_CLARA_PH1_Magnet_Controller_Obj(nullptr),
virtual_CLARA_BA1_Magnet_Controller_Obj(nullptr),
offline_CLARA_BA1_Magnet_Controller_Obj(nullptr),
physical_CLARA_BA1_Magnet_Controller_Obj(nullptr),
virtual_CB1_Magnet_Controller_Obj(nullptr),
offline_CB1_Magnet_Controller_Obj(nullptr),
physical_CB1_Magnet_Controller_Obj(nullptr),
virtual_CB2_Magnet_Controller_Obj(nullptr),
offline_CB2_Magnet_Controller_Obj(nullptr),
physical_CB2_Magnet_Controller_Obj(nullptr),

virtual_VELA_BA1_Magnet_Controller_Obj(nullptr),
offline_VELA_BA1_Magnet_Controller_Obj(nullptr),
physical_VELA_BA1_Magnet_Controller_Obj(nullptr),
virtual_VELA_BA2_Magnet_Controller_Obj(nullptr),
offline_VELA_BA2_Magnet_Controller_Obj(nullptr),
physical_VELA_BA2_Magnet_Controller_Obj(nullptr),


CLARA_PH1_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_MAG_CONFIG),
VELA_INJ_MAG_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_MAG_CONFIG),
VELA_BA1_MAG_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_MAG_CONFIG),
VELA_BA2_MAG_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_MAG_CONFIG),
CB1_MAG_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::CB1_MAG_CONFIG),
CB2_MAG_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::CB2_MAG_CONFIG),
C2B_MAG_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::C2B_MAG_CONFIG)
{
    std::cout <<"Instantiated a VCmagnets in Quiet Mode" <<std::endl;
    //ctor
}
//______________________________________________________________________________
VCmagnets::~VCmagnets()
{
    //dtor
    if(virtual_VELA_INJ_Magnet_Controller_Obj)
    {
        delete virtual_VELA_INJ_Magnet_Controller_Obj;
               virtual_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Magnet_Controller_Obj)
    {
        delete offline_VELA_INJ_Magnet_Controller_Obj;
               offline_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Magnet_Controller_Obj)
    {
        delete physical_VELA_INJ_Magnet_Controller_Obj;
               physical_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }


    if(virtual_VELA_BA1_Magnet_Controller_Obj)
    {
        delete virtual_VELA_BA1_Magnet_Controller_Obj;
               virtual_VELA_BA1_Magnet_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Magnet_Controller_Obj)
    {
        delete offline_VELA_BA2_Magnet_Controller_Obj;
               offline_VELA_BA2_Magnet_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Magnet_Controller_Obj)
    {
        delete physical_VELA_BA2_Magnet_Controller_Obj;
               physical_VELA_BA2_Magnet_Controller_Obj = nullptr;
    }

    if(virtual_CLARA_PH1_Magnet_Controller_Obj)
    {
        delete virtual_CLARA_PH1_Magnet_Controller_Obj;
               virtual_CLARA_PH1_Magnet_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_Magnet_Controller_Obj)
    {
        delete offline_CLARA_PH1_Magnet_Controller_Obj;
               offline_CLARA_PH1_Magnet_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_Magnet_Controller_Obj)
    {
        delete physical_CLARA_PH1_Magnet_Controller_Obj;
               physical_CLARA_PH1_Magnet_Controller_Obj = nullptr;
    }

    if(virtual_CB1_Magnet_Controller_Obj)
    {
        delete virtual_CB1_Magnet_Controller_Obj;
               virtual_CB1_Magnet_Controller_Obj = nullptr;
    }
    if(offline_CB1_Magnet_Controller_Obj)
    {
        delete offline_CB1_Magnet_Controller_Obj;
               offline_CB1_Magnet_Controller_Obj = nullptr;
    }
    if(physical_CB1_Magnet_Controller_Obj)
    {
        delete physical_CB1_Magnet_Controller_Obj;
               physical_CB1_Magnet_Controller_Obj = nullptr;
    }

    if(virtual_CB2_Magnet_Controller_Obj)
    {
        delete virtual_CB2_Magnet_Controller_Obj;
               virtual_CB2_Magnet_Controller_Obj = nullptr;
    }
    if(offline_CB2_Magnet_Controller_Obj)
    {
        delete offline_CB2_Magnet_Controller_Obj;
               offline_CB2_Magnet_Controller_Obj = nullptr;
    }
    if(physical_CB2_Magnet_Controller_Obj)
    {
        delete physical_CB2_Magnet_Controller_Obj;
               physical_CB2_Magnet_Controller_Obj = nullptr;
    }
}
// PH1
//______________________________________________________________________________
magnetController& VCmagnets::physical_CLARA_PH1_Magnet_Controller()
{
    std::string name  = "physical_CLARA_PH1_Magnet_Controller";
    return getController(physical_CLARA_PH1_Magnet_Controller_Obj,
                         CLARA_PH1_CONFIG,
                         name,withoutVM,withEPICS,CLARA_PH1);
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_CLARA_PH1_Magnet_Controller()
{
    std::string name  = "virtual_CLARA_PH1_Magnet_Controller";
    return getController(virtual_CLARA_PH1_Magnet_Controller_Obj,
                         CLARA_PH1_CONFIG,
                         name,withVM,withEPICS,CLARA_PH1);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_CLARA_PH1_Magnet_Controller()
{
    std::string name  = "offline_CLARA_PH1_Magnet_Controller";
    return getController(offline_CLARA_PH1_Magnet_Controller_Obj,
                         CLARA_PH1_CONFIG,
                         name,withoutVM,withoutEPICS,CLARA_PH1);
}
// VELA_INJ
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_INJ_Magnet_Controller()
{
    std::string name  = "virtual_VELA_INJ_Magnet_Controller_Obj";
    return getController(virtual_VELA_INJ_Magnet_Controller_Obj,
                         VELA_INJ_MAG_CONFIG,
                         name,withVM,withEPICS,VELA_INJ);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_VELA_INJ_Magnet_Controller()
{
    std::string name  = "offline_VELA_INJ_Magnet_Controller_Obj";
    return getController(offline_VELA_INJ_Magnet_Controller_Obj,
                         VELA_INJ_MAG_CONFIG,
                         name,withoutVM,withoutEPICS,VELA_INJ);
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_VELA_INJ_Magnet_Controller()
{
    std::string name  = "physical_VELA_INJ_Magnet_Controller_Obj";
    return getController(physical_VELA_INJ_Magnet_Controller_Obj,
                         VELA_INJ_MAG_CONFIG,name,withoutVM,withEPICS,VELA_INJ);
}
//
// BA1
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_BA1_Magnet_Controller()
{
    std::string name  = "virtual_VELA_INJ_Magnet_Controller_Obj";
    return getController(virtual_VELA_BA1_Magnet_Controller_Obj,
                         VELA_BA1_MAG_CONFIG,
                         name,withVM,withEPICS,VELA_BA1);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_VELA_BA1_Magnet_Controller()
{
    std::string name  = "offline_VELA_INJ_Magnet_Controller_Obj";
    return getController(offline_VELA_BA1_Magnet_Controller_Obj,
                         VELA_BA1_MAG_CONFIG,
                         name,withoutVM,withoutEPICS,VELA_BA1);
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_VELA_BA1_Magnet_Controller()
{
    std::string name  = "physical_VELA_INJ_Magnet_Controller_Obj";
    return getController(physical_VELA_BA1_Magnet_Controller_Obj,
                         VELA_BA1_MAG_CONFIG,
                         name,withoutVM,withEPICS,VELA_BA1);
}
// BA2
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_BA2_Magnet_Controller()
{
    std::string name  = "virtual_VELA_INJ_Magnet_Controller_Obj";
    return getController(virtual_VELA_BA2_Magnet_Controller_Obj,
                         VELA_BA2_MAG_CONFIG,
                         name,withVM,withEPICS,VELA_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_VELA_BA2_Magnet_Controller()
{
    std::string name  = "offline_VELA_INJ_Magnet_Controller_Obj";
    return getController(offline_VELA_BA2_Magnet_Controller_Obj,
                         VELA_BA2_MAG_CONFIG,
                         name,withoutVM,withoutEPICS,VELA_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_VELA_BA2_Magnet_Controller()
{
    std::string name  = "physical_VELA_INJ_Magnet_Controller_Obj";
    return getController(physical_VELA_BA2_Magnet_Controller_Obj,
                         VELA_BA2_MAG_CONFIG,
                         name,withoutVM,withEPICS,VELA_BA2);
}
// USER
//______________________________________________________________________________
magnetController& VCmagnets::virtual_USER_Magnet_Controller(cstr& config_path)
{
    std::string name  = "virtual_USER_Magnet_Controller_Obj";
    return getController(virtual_USER_Magnet_Controller_Obj,
                         config_path,
                         name,withVM,withEPICS,USER);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_USER_Magnet_Controller(cstr& config_path)
{
    std::string name  = "offline_USER_Magnet_Controller_Obj";
    return getController(offline_USER_Magnet_Controller_Obj,
                         config_path,
                         name,withoutVM,withoutEPICS,USER);
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_USER_Magnet_Controller(cstr& config_path)
{
    std::string name  = "physical_USER_Magnet_Controller_Obj";
    return getController(physical_USER_Magnet_Controller_Obj,
                         config_path,
                         name,withoutVM,withEPICS,USER);
}
// CB1
//______________________________________________________________________________
magnetController& VCmagnets::physical_CB1_Magnet_Controller()
{
    std::string name  = "physical_CB1_Magnet_Controller_Obj";
    return getController(physical_CB1_Magnet_Controller_Obj,
                         CB1_MAG_CONFIG,
                         name,withoutVM,withEPICS,CLARA_2_BA1);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_CB1_Magnet_Controller()
{
    std::string name  = "offline_CB1_Magnet_Controller_Obj";
    return getController(offline_CB1_Magnet_Controller_Obj,
                         CB1_MAG_CONFIG,
                         name,withoutVM,withoutEPICS,CLARA_2_BA1);
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_CB1_Magnet_Controller()
{
    std::string name  = "virtual_CB1_Magnet_Controller_Obj";
    return getController(virtual_CB1_Magnet_Controller_Obj,
                         CB1_MAG_CONFIG,
                         name,withVM,withEPICS,CLARA_2_BA1);
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_CB2_Magnet_Controller()
{
    std::string name  = "physical_CB2_Magnet_Controller_Obj";
    return getController(physical_CB2_Magnet_Controller_Obj,
                         CB2_MAG_CONFIG,
                         name,withoutVM,withEPICS,CLARA_2_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_CB2_Magnet_Controller()
{
    std::string name  = "offline_CB2_Magnet_Controller_Obj";
    return getController(offline_CB2_Magnet_Controller_Obj,
                         CB2_MAG_CONFIG,
                         name,withoutVM,withoutEPICS,CLARA_2_BA2);
}//______________________________________________________________________________
magnetController& VCmagnets::virtual_CB2_Magnet_Controller()
{
    std::string name  = "virtual_CB2_Magnet_Controller_Obj";
    return getController(virtual_CB2_Magnet_Controller_Obj,
                         CB2_MAG_CONFIG,
                         name,withVM,withEPICS,CLARA_2_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_C2B_Magnet_Controller()
{
    std::string name  = "virtual_C2B_Magnet_Controller";
    return getController(virtual_CB2_Magnet_Controller_Obj,
                         C2B_MAG_CONFIG,
                         name,withVM,withEPICS,CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_C2B_Magnet_Controller()
{
    std::string name  = "offline_C2B_Magnet_Controller";
    return getController(offline_CB2_Magnet_Controller_Obj,
                         C2B_MAG_CONFIG,
                         name,withoutVM,withoutEPICS,CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_C2B_Magnet_Controller()
{
    std::string name  = "physical_C2B_Magnet_Controller";
    return getController(physical_CB2_Magnet_Controller_Obj,
                         C2B_MAG_CONFIG,
                         name,withoutVM,withEPICS,CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
magnetController& VCmagnets::getMagnetController(MACHINE_MODE mode,MACHINE_AREA area)
{

    std::cout << ENUM_TO_STRING(area) << std::endl;
    std::cout << ENUM_TO_STRING(area) << std::endl;
    std::cout << ENUM_TO_STRING(area) << std::endl;

    if(mode == OFFLINE && area == VELA_INJ)
        return offline_VELA_INJ_Magnet_Controller();
    else if(mode == VIRTUAL && area == VELA_INJ)
        return virtual_VELA_INJ_Magnet_Controller();
    else if(mode == PHYSICAL && area == VELA_INJ)
        return physical_VELA_INJ_Magnet_Controller();

    else if(mode == OFFLINE && area == VELA_BA1)
        return offline_VELA_BA1_Magnet_Controller();
    else if(mode == VIRTUAL && area == VELA_BA1)
        return virtual_VELA_BA1_Magnet_Controller();
    else if(mode == PHYSICAL && area == VELA_BA1)
        return physical_VELA_BA1_Magnet_Controller();

    else if(mode == OFFLINE && area == VELA_BA2)
        return offline_VELA_BA2_Magnet_Controller();
    else if(mode == VIRTUAL && area == VELA_BA2)
        return virtual_VELA_BA2_Magnet_Controller();
    else if(mode == PHYSICAL && area == VELA_BA2)
        return physical_VELA_BA2_Magnet_Controller();

    else if(mode == OFFLINE && area == CLARA_PH1)
        return offline_CLARA_PH1_Magnet_Controller();
    else if(mode == VIRTUAL && area == CLARA_PH1)
        return virtual_CLARA_PH1_Magnet_Controller();
    else if(mode == PHYSICAL && area == CLARA_PH1)
        return physical_CLARA_PH1_Magnet_Controller();

    else if(mode == PHYSICAL && area == CLARA_2_BA1)
        return physical_CB1_Magnet_Controller();
    else if(mode == PHYSICAL && area == CLARA_2_BA2)
        return physical_CB1_Magnet_Controller();
    else if(mode == VIRTUAL && area == CLARA_2_BA1)
        return virtual_CB1_Magnet_Controller();

    else if(mode == VIRTUAL && area == CLARA_2_BA2)
        return virtual_CB2_Magnet_Controller();
    else if(mode == OFFLINE && area == CLARA_2_BA1)
        return offline_CB2_Magnet_Controller();
    else if(mode == OFFLINE && area == CLARA_2_BA2)
        return physical_CB2_Magnet_Controller();

    else if(mode == VIRTUAL && area == CLARA_2_BA1_BA2)
        return virtual_C2B_Magnet_Controller();
    else if(mode == OFFLINE && area == CLARA_2_BA1_BA2)
        return offline_C2B_Magnet_Controller();
    else if(mode == PHYSICAL && area == CLARA_2_BA1_BA2)
        return physical_C2B_Magnet_Controller();

}
//______________________________________________________________________________
magnetController& VCmagnets::getController(magnetController*& cont,
                                           const std::string& conf,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const MACHINE_AREA myMachineArea)
{
    if(cont)
    {
        std::cout << name  <<" object already exists," <<std::endl;
    }
    else
    {
        std::cout <<"Creating " <<name <<" object" <<std::endl;
        messageStates[cont].first     = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;
        cont = new magnetController(messageStates.at(cont).first,
                                    messageStates.at(cont).second,
                                    conf,
                                    shouldVM,
                                    shouldEPICS,
                                    myMachineArea,
                                    name);
    }
    return *cont;
}
//______________________________________________________________________________
void VCmagnets::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}

















