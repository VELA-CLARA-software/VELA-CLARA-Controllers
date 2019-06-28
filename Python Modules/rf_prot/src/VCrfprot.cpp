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
//  Author:      DJS
//  Last edit:   19-03-2018
//  FileName:    VCrfprot.cpp
//  Description:
//
//
//*/
#include "VCrfprot.h"
#include <iostream>
VCrfprot::VCrfprot():
VCbase("VCrfprot"),
virtual_Gun_Protection_Controller_Obj(nullptr),
physical_Gun_Protection_Controller_Obj(nullptr),
offline_Gun_Protection_Controller_Obj(nullptr),
allGunProtsConf(UTL::APCLARA1_CONFIG_PATH + UTL::RF_GUN_PROTECTION_CONFIG),
L01ProtsConf(UTL::APCLARA1_CONFIG_PATH + UTL::RF_L01_PROTECTION_CONFIG)
{
   std::cout << "Instantiated a VCrfprot " << std::endl;
    //ctor
}
//______________________________________________________________________________
VCrfprot::~VCrfprot()
{
    //dtor
    if(virtual_Gun_Protection_Controller_Obj)
    {
        delete virtual_Gun_Protection_Controller_Obj;
               virtual_Gun_Protection_Controller_Obj = nullptr;
    }
    if(physical_Gun_Protection_Controller_Obj)
    {
        delete physical_Gun_Protection_Controller_Obj;
               physical_Gun_Protection_Controller_Obj = nullptr;
    }
    if(offline_Gun_Protection_Controller_Obj)
    {
        delete offline_Gun_Protection_Controller_Obj;
               offline_Gun_Protection_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
gunProtController&
    VCrfprot::getProtectionController(const HWC_ENUM::MACHINE_MODE mode,
                                      const HWC_ENUM::MACHINE_AREA area)
{
    if(area == VELA_INJ)
    {
        getProtectionController(mode, RF_GUN);
    }
    else if(area == CLARA_INJ)
    {
        getProtectionController(mode, RF_GUN);
    }
    else if(area == CLARA_L01)
    {
        getProtectionController(mode, RF_L01);
    }
    else if(mode == VIRTUAL && area == RF_GUN)
    {
        return virtual_Gun_Protection_Controller();
    }
    else if(mode == PHYSICAL && area == RF_GUN)
    {
        return physical_Gun_Protection_Controller();
    }
    else if(mode == OFFLINE && area == RF_GUN)
    {
        return offline_Gun_Protection_Controller();
    }
    else if(mode == VIRTUAL && area == RF_L01)
    {
        //return offline_Gun_Protection_Controller();
    }
    else if(mode == PHYSICAL && area == RF_L01)
    {
        //return offline_Gun_Protection_Controller();
    }
    else if(mode == OFFLINE && area == RF_L01)
    {
        //return offline_Gun_Protection_Controller();
    }
    std::cout << "!!Error!! MACHINE_MODE / MACHINE_AREA "<< mode << " / "
              << area << " is a bad combination. Returning offline gun"
                 "protection controlller"
              << std::endl;

    return offline_Gun_Protection_Controller();
}
//______________________________________________________________________________
gunProtController& VCrfprot::virtual_Gun_Protection_Controller()
{
    std::string name  = "virtual_Gun_Protection_Controller_Obj";
    return getController(virtual_Gun_Protection_Controller_Obj,
                         allGunProtsConf,
                         name,
                         withVM,
                         withEPICS,
                         RF_GUN);
}
//______________________________________________________________________________
gunProtController& VCrfprot::physical_Gun_Protection_Controller()
{
    std::string name  = "physical_Gun_Protection_Controller_Obj";
    return getController(physical_Gun_Protection_Controller_Obj,
                         allGunProtsConf,
                         name,
                         withoutVM,
                         withEPICS,
                         RF_GUN);
}
//______________________________________________________________________________
gunProtController& VCrfprot::offline_Gun_Protection_Controller()
{
    std::string name  = "offline_Gun_Protection_Controller_Obj";
    return getController(offline_Gun_Protection_Controller_Obj,
                         allGunProtsConf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         RF_GUN);
}
//______________________________________________________________________________
gunProtController& VCrfprot::getController(gunProtController*& cont,
                                           const std::string& conf,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const HWC_ENUM::MACHINE_AREA myMachineArea)
{
    if(cont)
    {
        std::cout << name << " object already exists," <<std::endl;
    }
    else
    {
        std::cout << "Creating " << name << " object" <<std::endl;
        messageStates[cont].first  = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;
        cont = new gunProtController(messageStates.at(cont).first,
                                     messageStates.at(cont).second,
                                     conf, shouldVM, shouldEPICS,name);
    }
    return *cont;
}


//______________________________________________________________________________
L01ProtController& VCrfprot::virtual_L01_Protection_Controller()
{
    std::string name  = "virtual_L01_Protection_Controller_Obj";
    return getL01Controller(virtual_L01_Protection_Controller_Obj,
                         L01ProtsConf,
                         name,
                         withVM,
                         withEPICS,
                         RF_L01);
}
//______________________________________________________________________________
L01ProtController& VCrfprot::physical_L01_Protection_Controller()
{
    std::string name  = "physical_L01_Protection_Controller_Obj";
    return getL01Controller(physical_L01_Protection_Controller_Obj,
                         L01ProtsConf,
                         name,
                         withoutVM,
                         withEPICS,
                         RF_L01);
}
//______________________________________________________________________________
L01ProtController& VCrfprot::offline_L01_Protection_Controller()
{
    std::string name  = "offline_L01_Protection_Controller_Obj";
    return getL01Controller(offline_L01_Protection_Controller_Obj,
                         L01ProtsConf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         RF_L01);
}
//______________________________________________________________________________
L01ProtController& VCrfprot::getL01Controller(L01ProtController*& cont,
                                           const std::string& conf,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const HWC_ENUM::MACHINE_AREA myMachineArea)
{
    if(cont)
    {
        std::cout << name << " object already exists," <<std::endl;
    }
    else
    {
        std::cout << "Creating " << name << " object" <<std::endl;
        messageStatesL01[cont].first  = shouldShowMessage;
        messageStatesL01.at(cont).second = shouldShowDebugMessage;
        cont = new L01ProtController(messageStatesL01.at(cont).first,
                                     messageStatesL01.at(cont).second,
                                     conf, shouldVM, shouldEPICS,name);
    }
    return *cont;
}
//______________________________________________________________________________
void VCrfprot::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}














