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
//  Author:      ADB/DJS
//  Last edit:   07-08-2018
//  FileName:    VCscreens.cpp
//  Description:
//
//
//*/
#include "VCscreens.h"
#include <iostream>

VCscreens::VCscreens():
VCbase("VCscreens"),
virtual_VELA_INJ_Screen_Controller_Obj(nullptr),
offline_VELA_INJ_Screen_Controller_Obj(nullptr),
physical_VELA_INJ_Screen_Controller_Obj(nullptr),
virtual_VELA_BA1_Screen_Controller_Obj(nullptr),
offline_VELA_BA1_Screen_Controller_Obj(nullptr),
physical_VELA_BA1_Screen_Controller_Obj(nullptr),
virtual_VELA_BA2_Screen_Controller_Obj(nullptr),
offline_VELA_BA2_Screen_Controller_Obj(nullptr),
physical_VELA_BA2_Screen_Controller_Obj(nullptr),
virtual_CLARA_PH1_Screen_Controller_Obj(nullptr),
offline_CLARA_PH1_Screen_Controller_Obj(nullptr),
physical_CLARA_PH1_Screen_Controller_Obj(nullptr),
virtual_C2B_Screen_Controller_Obj(nullptr),
offline_C2B_Screen_Controller_Obj(nullptr),
physical_C2B_Screen_Controller_Obj(nullptr),
virtual_Screen_Controller_Obj(nullptr),
offline_Screen_Controller_Obj(nullptr),
physical_Screen_Controller_Obj(nullptr),

VELA_INJ ( HWC_ENUM::MACHINE_AREA::VELA_INJ ),
VELA_BA1 ( HWC_ENUM::MACHINE_AREA::VELA_BA1 ),
VELA_BA2 ( HWC_ENUM::MACHINE_AREA::VELA_BA2 ),
CLARA_PH1( HWC_ENUM::MACHINE_AREA::CLARA_PH1 ),
ALL_VELA_CLARA( HWC_ENUM::MACHINE_AREA::ALL_VELA_CLARA ),
UNKNOWN_AREA(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA)
{
    //ctor
}
//______________________________________________________________________________
VCscreens::~VCscreens()
{
    //dtor
    if(virtual_VELA_INJ_Screen_Controller_Obj)
    {
        delete virtual_VELA_INJ_Screen_Controller_Obj;
               virtual_VELA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Screen_Controller_Obj)
    {
        delete physical_VELA_INJ_Screen_Controller_Obj;
               physical_VELA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Screen_Controller_Obj)
    {
        delete offline_VELA_INJ_Screen_Controller_Obj;
               offline_VELA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_Screen_Controller_Obj)
    {
        delete virtual_VELA_BA1_Screen_Controller_Obj;
               virtual_VELA_BA1_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_Screen_Controller_Obj)
    {
        delete physical_VELA_BA1_Screen_Controller_Obj;
               physical_VELA_BA1_Screen_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_Screen_Controller_Obj)
    {
        delete virtual_VELA_BA2_Screen_Controller_Obj;
               virtual_VELA_BA2_Screen_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Screen_Controller_Obj)
    {
        delete offline_VELA_BA2_Screen_Controller_Obj;
               offline_VELA_BA2_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Screen_Controller_Obj)
    {
        delete physical_VELA_BA2_Screen_Controller_Obj;
               physical_VELA_BA2_Screen_Controller_Obj = nullptr;
    }

    if(virtual_CLARA_PH1_Screen_Controller_Obj)
    {
        delete virtual_CLARA_PH1_Screen_Controller_Obj;
               virtual_CLARA_PH1_Screen_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_Screen_Controller_Obj)
    {
        delete offline_CLARA_PH1_Screen_Controller_Obj;
               offline_CLARA_PH1_Screen_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_Screen_Controller_Obj)
    {
        delete physical_CLARA_PH1_Screen_Controller_Obj;
               physical_CLARA_PH1_Screen_Controller_Obj = nullptr;
    }

    if(virtual_C2B_Screen_Controller_Obj)
    {
        delete virtual_C2B_Screen_Controller_Obj;
               virtual_C2B_Screen_Controller_Obj = nullptr;
    }
    if(offline_C2B_Screen_Controller_Obj)
    {
        delete offline_C2B_Screen_Controller_Obj;
               offline_C2B_Screen_Controller_Obj = nullptr;
    }
    if(physical_C2B_Screen_Controller_Obj)
    {
        delete physical_C2B_Screen_Controller_Obj;
               physical_C2B_Screen_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
screenController & VCscreens::getController(screenController*& cont,
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
        cont = new screenController(conf,
                                    messageStates.at(cont).first,
                                    messageStates.at(cont).second,
                                    shouldEPICS,
                                    shouldVM,
                                    myMachineArea);
    }
    return *cont;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_INJ_Screen_Controller()
{
    std::string name  = "virtual_VELA_INJ_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    return getController(virtual_VELA_INJ_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_INJ_Screen_Controller()
{
    std::string name  = "offline_VELA_INJ_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    return getController(offline_VELA_INJ_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_INJ_Screen_Controller()
{
    std::string name  = "physical_VELA_INJ_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    return getController(physical_VELA_INJ_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_INJ);
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_BA1_Screen_Controller()
{
    std::string name  = "virtual_VELA_BA1_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_SCREENS_CONFIG;
    return getController(virtual_VELA_BA1_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_BA1_Screen_Controller()
{
    std::string name  = "offline_VELA_BA1_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_SCREENS_CONFIG;
    return getController(offline_VELA_BA1_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_BA1_Screen_Controller()
{
    std::string name  = "physical_VELA_BA1_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA1_SCREENS_CONFIG;
    return getController(physical_VELA_BA1_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA1);
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_BA2_Screen_Controller()
{
    std::string name  = "virtual_VELA_BA2_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_SCREENS_CONFIG;
    return getController(virtual_VELA_BA2_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_BA2_Screen_Controller()
{
    std::string name  = "offline_VELA_BA2_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_SCREENS_CONFIG;
    return getController(offline_VELA_BA2_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_BA2_Screen_Controller()
{
    std::string name  = "physical_VELA_BA2_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::VELA_BA2_SCREENS_CONFIG;
    return getController(physical_VELA_BA2_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::VELA_BA2);
}
//______________________________________________________________________________
screenController& VCscreens::virtual_CLARA_PH1_Screen_Controller()
{
    std::string name  = "virtual_CLARA_PH1_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_SCREENS_CONFIG;
    return getController(virtual_CLARA_PH1_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
screenController& VCscreens::offline_CLARA_PH1_Screen_Controller()
{
    std::string name  = "offline_CLARA_PH1_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_SCREENS_CONFIG;
    return getController(offline_CLARA_PH1_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
screenController& VCscreens::physical_CLARA_PH1_Screen_Controller()
{
    std::string name  = "physical_CLARA_PH1_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::CLARA_PH1_SCREENS_CONFIG;
    return getController(physical_CLARA_PH1_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_PH1);
}
//______________________________________________________________________________
screenController& VCscreens::virtual_C2B_Screen_Controller()
{
    std::string name  = "virtual_C2B_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::C2B_SCREENS_CONFIG;
    return getController(virtual_C2B_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
screenController& VCscreens::offline_C2B_Screen_Controller()
{
    std::string name  = "offline_C2B_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::C2B_SCREENS_CONFIG;
    return getController(offline_C2B_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//______________________________________________________________________________
screenController& VCscreens::physical_C2B_Screen_Controller()
{
    std::string name  = "physical_C2B_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::C2B_SCREENS_CONFIG;
    return getController(physical_C2B_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withEPICS,
                         HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2);
}
//------------------------------------------------------------------------------------
screenController& VCscreens::offline_Screen_Controller()
{
    std::string name  = "offline_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::ALL_VELA_CLARA_SCREENS_CONFIG;
    return getController(virtual_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         ALL_VELA_CLARA);
}
//------------------------------------------------------------------------------------
screenController& VCscreens::virtual_Screen_Controller()
{
    std::string name  = "virtual_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::ALL_VELA_CLARA_SCREENS_CONFIG;
    return getController(virtual_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withVM,
                         withEPICS,
                         ALL_VELA_CLARA);
}
//------------------------------------------------------------------------------------
screenController& VCscreens::physical_Screen_Controller()
{
    std::string name  = "physical_Screen_Controller";
    const std::string screenconf = UTL::CLARANET_CONFIG_PATH + UTL::ALL_VELA_CLARA_SCREENS_CONFIG;
    return getController(physical_Screen_Controller_Obj,
                         screenconf,
                         name,
                         withoutVM,
                         withEPICS,
                         ALL_VELA_CLARA);
}
//------------------------------------------------------------------------------------
screenController& VCscreens::getScreenController( const HWC_ENUM::MACHINE_MODE mode, const HWC_ENUM::MACHINE_AREA area )
{
    if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_INJ )
        return offline_VELA_INJ_Screen_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Screen_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_INJ )
        return physical_VELA_INJ_Screen_Controller();

    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA1 )
        return offline_VELA_BA1_Screen_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_Screen_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA1 )
        return physical_VELA_BA1_Screen_Controller();

    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::VELA_BA2 )
        return offline_VELA_BA2_Screen_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_Screen_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::VELA_BA2 )
        return physical_VELA_BA2_Screen_Controller();

    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_Screen_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_Screen_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_Screen_Controller();

    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return offline_C2B_Screen_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return virtual_C2B_Screen_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::CLARA_2_BA1_BA2 )
        return physical_C2B_Screen_Controller();

    else if( mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::ALL_VELA_CLARA)
        return offline_Screen_Controller();
    else if( mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::ALL_VELA_CLARA )
        return virtual_Screen_Controller();
    else if( mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::ALL_VELA_CLARA )
        return physical_Screen_Controller();


}
//______________________________________________________________________________
void VCscreens::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}
//______________________________________________________________________________
