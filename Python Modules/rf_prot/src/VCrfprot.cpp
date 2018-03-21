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
VCbase(),
virtual_Gun_Protection_Controller_Obj(nullptr),
physical_Gun_Protection_Controller_Obj(nullptr),
offline_Gun_Protection_Controller_Obj(nullptr),
allGunProtsConf(UTL::APCLARA1_CONFIG_PATH + UTL::RF_GUN_PROTECTION_CONFIG)
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
    if(area == HWC_ENUM::VELA_INJ)
    {
        getProtectionController(mode, HWC_ENUM::RF_GUN);
    }
    else if(area == HWC_ENUM::CLARA_INJ)
    {
        getProtectionController(mode, HWC_ENUM::RF_GUN);
    }
    else if(area == HWC_ENUM::CLARA_L01)
    {
        getProtectionController(mode, HWC_ENUM::RF_L01);
    }
    else if(mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::RF_GUN)
    {
        return virtual_Gun_Protection_Controller();
    }
    else if(mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::RF_GUN)
    {
        return physical_Gun_Protection_Controller();
    }
    else if(mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::RF_GUN)
    {
        return offline_Gun_Protection_Controller();
    }
    else if(mode == HWC_ENUM::VIRTUAL && area == HWC_ENUM::RF_L01)
    {
        //return offline_Gun_Protection_Controller();
    }
    else if(mode == HWC_ENUM::PHYSICAL && area == HWC_ENUM::RF_L01)
    {
        //return offline_Gun_Protection_Controller();
    }
    else if(mode == HWC_ENUM::OFFLINE && area == HWC_ENUM::RF_L01)
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
    if(virtual_Gun_Protection_Controller_Obj)
    {
        std::cout << "virtual_Gun_Protection_Controller object already exists,"
                  << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_Gun_Protection_Controller object"
                  << std::endl;
        virtual_Gun_Protection_Controller_Obj =
            new gunProtController(shouldShowMessage,
                                  shouldShowDebugMessage,
                                  allGunProtsConf,
                                  withVM,
                                  withEPICS);
    }
    return *virtual_Gun_Protection_Controller_Obj;
}
//______________________________________________________________________________
gunProtController& VCrfprot::physical_Gun_Protection_Controller()
{
    if(physical_Gun_Protection_Controller_Obj)
    {
        std::cout << "physical_Gun_Protection_Controller object already exists,"
                  << std::endl;
    }
    else
    {
        std::cout << "Creating physical_Gun_Protection_Controller object"
                  << std::endl;
        physical_Gun_Protection_Controller_Obj =
            new gunProtController(shouldShowMessage,
                                  shouldShowDebugMessage,
                                  allGunProtsConf,
                                  withoutVM,
                                  withEPICS);
    }
    return *physical_Gun_Protection_Controller_Obj;
}
//______________________________________________________________________________
gunProtController& VCrfprot::offline_Gun_Protection_Controller()
{
    if(offline_Gun_Protection_Controller_Obj)
    {
        std::cout << "offline_Gun_ProtectionController object already exists,"
                  << std::endl;
    }
    else
    {
        std::cout << "Creating offline_Gun_ProtectionController object"
                  << std::endl;
        offline_Gun_Protection_Controller_Obj =
            new gunProtController(shouldShowMessage,
                                  shouldShowDebugMessage,
                                  allGunProtsConf,
                                  withoutVM,
                                  withoutEPICS);
    }
    return *offline_Gun_Protection_Controller_Obj;
}















