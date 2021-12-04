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
//  FileName:    VCllrf.cpp
//  Description:
//
//
//*/
//______________________________________________________________________________
#include "VCllrf.h"
#include <iostream>

VCllrf::VCllrf():
VCbase("VCllrf"),
GUN_LLRF_Controller_Obj(nullptr),
L01_LLRF_Controller_Obj(nullptr),
CLARA_LRRG_LLRF_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::CLARA_LRRG_LLRF_CONFIG),
CLARA_HRRG_LLRF_CONFIG(UTL::CLARANET_CONFIG_PATH + UTL::CLARA_HRRG_LLRF_CONFIG),
CLARA_L01_LLRF_CONFIG (UTL::CLARANET_CONFIG_PATH + UTL::CLARA_L01_LLRF_CONFIG),
VELA_HRRG_LLRF_CONFIG (UTL::CLARANET_CONFIG_PATH + UTL::VELA_HRRG_LLRF_CONFIG),
VELA_LRRG_LLRF_CONFIG (UTL::CLARANET_CONFIG_PATH + UTL::VELA_LRRG_LLRF_CONFIG),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true)
{
    std::cout << "Instantiated a VCllrf in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
VCllrf::~VCllrf()
{    //dtor
    killGun();
    if(L01_LLRF_Controller_Obj)
    {
        delete L01_LLRF_Controller_Obj;
               L01_LLRF_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
void VCllrf::killGun()
{
    if(GUN_LLRF_Controller_Obj)
    {
        delete GUN_LLRF_Controller_Obj;
               GUN_LLRF_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_CLARA_LRRG_LLRF_Controller()
{
    std::string name  = "virtual_CLARA_LRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //CLARA_LRRG_LLRF_CONFIG,
                         name,
                         withVM,
                         withEPICS,
                         llrfStructs::CLARA_LRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_CLARA_LRRG_LLRF_Controller()
{
    std::string name  = "offline_CLARA_LRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //CLARA_LRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withoutEPICS,
                         llrfStructs::CLARA_LRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_CLARA_LRRG_LLRF_Controller()
{
    std::string name  = "physical_CLARA_LRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //CLARA_LRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withEPICS,
                         llrfStructs::CLARA_LRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_CLARA_HRRG_LLRF_Controller()
{
    std::string name  = "virtual_CLARA_HRRG_LLRF_Controamp_spller";
    return getController(GUN_LLRF_Controller_Obj,
                         //CLARA_HRRG_LLRF_CONFIG,
                         name,
                         withVM,
                         withEPICS,
                         llrfStructs::CLARA_HRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_CLARA_HRRG_LLRF_Controller()
{
    std::string name  = "offline_CLARA_HRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //CLARA_HRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withoutEPICS,
                         llrfStructs::CLARA_HRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_CLARA_HRRG_LLRF_Controller()
{
    std::string name  = "physical_CLARA_HRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //CLARA_HRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withEPICS,
                         llrfStructs::CLARA_HRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_VELA_LRRG_LLRF_Controller()
{
    std::string name  = "virtual_VELA_LRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //VELA_LRRG_LLRF_CONFIG,
                         name,
                         withVM,
                         withEPICS,
                         llrfStructs::VELA_LRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_VELA_LRRG_LLRF_Controller()
{
    std::string name  = "offline_VELA_LRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //VELA_LRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withoutEPICS,
                         llrfStructs::VELA_LRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_VELA_LRRG_LLRF_Controller()
{
    std::string name  = "physical_VELA_LRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //VELA_LRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withEPICS,
                         llrfStructs::VELA_LRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_VELA_HRRG_LLRF_Controller()
{
    std::string name  = "virtual_VELA_HRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //VELA_HRRG_LLRF_CONFIG,
                         name,
                         withVM,
                         withEPICS,
                         llrfStructs::VELA_HRRG,
                         HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_VELA_HRRG_LLRF_Controller()
{
    std::string name  = "offline_VELA_HRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //VELA_HRRG_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withoutEPICS,
                         llrfStructs::VELA_HRRG,HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_VELA_HRRG_LLRF_Controller()
{
    std::string name  = "physical_VELA_HRRG_LLRF_Controller";
    return getController(GUN_LLRF_Controller_Obj,
                         //VELA_HRRG_LLRF_CONFIG,
                         name,
                         withoutVM,withEPICS,llrfStructs::VELA_HRRG,HWC_ENUM::RF_GUN);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_L01_LLRF_Controller()
{
    std::string name  = "virtual_L01_LLRF_Controller";
    return getController(L01_LLRF_Controller_Obj,
                         //CLARA_L01_LLRF_CONFIG,
                         name,withVM,withEPICS,llrfStructs::L01,HWC_ENUM::RF_L01);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_L01_LLRF_Controller()
{
    std::string name  = "physical_L01_LLRF_Controller";
    return getController(L01_LLRF_Controller_Obj,
                         //CLARA_L01_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withEPICS,
                         llrfStructs::L01,
                         HWC_ENUM::RF_L01);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_L01_LLRF_Controller()
{
    std::string name  = "offline_L01_LLRF_Controller";
    return getController(L01_LLRF_Controller_Obj,
                         //CLARA_L01_LLRF_CONFIG,
                         name,
                         withoutVM,
                         withoutEPICS,llrfStructs::L01,HWC_ENUM::RF_L01);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::getController(liberaLLRFController*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           llrfStructs::LLRF_TYPE llrfType,
                                           const HWC_ENUM::MACHINE_AREA myMachineArea)
{
    if(cont)
    {
        std::cout << name << " object already exists," <<std::endl;
    }
    else
    {
        std::cout << "Creating a " << name << " object" <<std::endl;
        messageStates[cont].first  = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;

        /*


        */

        std::string conf = UTL::UNKNOWN_STRING;

        switch( llrfType )
        {
            case CLARA_HRRG:
                conf = CLARA_HRRG_LLRF_CONFIG;
                break;

            case CLARA_LRRG:
                conf = CLARA_LRRG_LLRF_CONFIG;
                break;
            case VELA_HRRG:
                conf = VELA_HRRG_LLRF_CONFIG;
                break;
            case VELA_LRRG:
                conf = VELA_LRRG_LLRF_CONFIG;
                break;
            case L01:
                conf = CLARA_L01_LLRF_CONFIG;
                break;
            case UNKNOWN_TYPE:

                break;

        }

        cont = new liberaLLRFController(messageStates.at(cont).first,
                                     messageStates.at(cont).second,
                                     conf,
                                     shouldVM,
                                     shouldEPICS,
                                     llrfType,
                                     HWC_ENUM::CONTROLLER_TYPE::LLRF,
                                     name);

    }
    std::cout << "getController FIN" <<std::endl;
    return *cont;
}
//______________________________________________________________________________
bool VCllrf::isaGUN(llrfStructs::LLRF_TYPE llrfType)
{
    bool r = true;
    switch(llrfType)
    {
        case llrfStructs::L01:
            r = false;
            break;
    }
    return r;
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::getLLRFController(HWC_ENUM::MACHINE_MODE mode,
                                                llrfStructs::LLRF_TYPE type)
{
    if(mode == HWC_ENUM::OFFLINE && type == llrfStructs::LLRF_TYPE::CLARA_HRRG)
        return offline_CLARA_HRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::VIRTUAL && type == llrfStructs::LLRF_TYPE::CLARA_HRRG)
        return virtual_CLARA_HRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::PHYSICAL && type == llrfStructs::LLRF_TYPE::CLARA_HRRG)
        return physical_CLARA_HRRG_LLRF_Controller();

    else if(mode == HWC_ENUM::OFFLINE && type == llrfStructs::LLRF_TYPE::CLARA_LRRG)
        return offline_CLARA_LRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::VIRTUAL && type ==llrfStructs::LLRF_TYPE::CLARA_LRRG)
        return virtual_CLARA_LRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::PHYSICAL && type == llrfStructs::LLRF_TYPE::CLARA_LRRG)
        return physical_CLARA_LRRG_LLRF_Controller();

    else if(mode == HWC_ENUM::OFFLINE && type ==  llrfStructs::LLRF_TYPE::VELA_HRRG)
        return offline_VELA_HRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::VIRTUAL && type == llrfStructs::LLRF_TYPE::VELA_HRRG)
        return virtual_VELA_HRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::PHYSICAL && type == llrfStructs::LLRF_TYPE::VELA_HRRG)
        return physical_VELA_HRRG_LLRF_Controller();

    else if(mode == HWC_ENUM::OFFLINE && type == llrfStructs::LLRF_TYPE::VELA_LRRG)
        return offline_VELA_LRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::VIRTUAL && type == llrfStructs::LLRF_TYPE::VELA_LRRG)
        return virtual_VELA_LRRG_LLRF_Controller();
    else if(mode == HWC_ENUM::PHYSICAL && type == llrfStructs::LLRF_TYPE::VELA_LRRG)
        return physical_VELA_LRRG_LLRF_Controller();

    else if(mode == HWC_ENUM::OFFLINE && type == llrfStructs::LLRF_TYPE::L01)
        return offline_L01_LLRF_Controller();
    else if(mode == HWC_ENUM::VIRTUAL && type == llrfStructs::LLRF_TYPE::L01)
        return virtual_L01_LLRF_Controller();
    else if(mode == HWC_ENUM::PHYSICAL && type == llrfStructs::LLRF_TYPE::L01)
        return physical_L01_LLRF_Controller();

}
//______________________________________________________________________________
void VCllrf::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}















