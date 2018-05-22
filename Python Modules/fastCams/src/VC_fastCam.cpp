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
//  FileName:    VC_fastCam.cpp
//  Description:
//
//
//
//
//*/
// projetc includes
#include "VC_fastCam.h"
//______________________________________________________________________________
using namespace HWC_ENUM;
using namespace UTL;
//______________________________________________________________________________
VC_fastCam::VC_fastCam():
VCbase("VC_fastCam"),
virtual_VELA_FastCam_Controller_Obj(nullptr),
offline_VELA_FastCam_Controller_Obj(nullptr),
physical_VELA_FastCam_Controller_Obj(nullptr),
offline_CLARA_FastCam_Controller_Obj(nullptr),
physical_CLARA_FastCam_Controller_Obj(nullptr),
virtual_CLARA_FastCam_Controller_Obj(nullptr),
virtual_CB1_FastCam_Controller_Obj(nullptr),
offline_CB1_FastCam_Controller_Obj(nullptr),
physical_CB1_FastCam_Controller_Obj(nullptr),
VELA_CONFIG(APCLARA1_CONFIG_PATH + VELA_FAST_CAM_CONFIG),
CLARA_CONFIG(APCLARA1_CONFIG_PATH + CLARA_FAST_CAM_CONFIG)
{
    //ctor
}
//______________________________________________________________________________
VC_fastCam::~VC_fastCam()
{
    //dtor
    if(virtual_VELA_FastCam_Controller_Obj)
    {
        delete virtual_VELA_FastCam_Controller_Obj;
               virtual_VELA_FastCam_Controller_Obj = nullptr;
    }
    if(offline_VELA_FastCam_Controller_Obj)
    {
        delete offline_VELA_FastCam_Controller_Obj;
               offline_VELA_FastCam_Controller_Obj = nullptr;
    }
    if(physical_VELA_FastCam_Controller_Obj)
    {
        delete physical_VELA_FastCam_Controller_Obj;
               physical_VELA_FastCam_Controller_Obj = nullptr;
    }
    if(offline_CLARA_FastCam_Controller_Obj)
    {
        delete offline_CLARA_FastCam_Controller_Obj;
               offline_CLARA_FastCam_Controller_Obj = nullptr;
    }
    if(physical_CLARA_FastCam_Controller_Obj)
    {
        delete physical_CLARA_FastCam_Controller_Obj;
               physical_CLARA_FastCam_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_FastCam_Controller_Obj)
    {
        delete virtual_CLARA_FastCam_Controller_Obj;
               virtual_CLARA_FastCam_Controller_Obj = nullptr;
    }
    if(virtual_CB1_FastCam_Controller_Obj)
    {
        delete virtual_CB1_FastCam_Controller_Obj;
               virtual_CB1_FastCam_Controller_Obj = nullptr;
    }
    if(offline_CB1_FastCam_Controller_Obj)
    {
        delete offline_CB1_FastCam_Controller_Obj;
               offline_CB1_FastCam_Controller_Obj = nullptr;
    }
    if(physical_CB1_FastCam_Controller_Obj)
    {
        delete physical_CB1_FastCam_Controller_Obj;
               physical_CB1_FastCam_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
fastCamController& VC_fastCam::virtual_CLARA_FastCam_Controller()
{
    std::string name  = "virtual_CLARA_FastCam_Controller";
    return getController(virtual_CLARA_FastCam_Controller_Obj,
                         name,withVM,withEPICS,CLARA);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::offline_CLARA_FastCam_Controller()
{
    std::string name  = "offline_CLARA_FastCam_Controller";
    return getController(offline_CLARA_FastCam_Controller_Obj,
                         name,withoutVM,withoutEPICS,CLARA);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::physical_CLARA_FastCam_Controller()
{
    std::string name  = "physical_CLARA_FastCam_Controller_Obj";
    return getController(physical_CLARA_FastCam_Controller_Obj,
                         name,withoutVM,withEPICS,CLARA);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::virtual_VELA_FastCam_Controller()
{
    std::string name  = "virtual_VELA_FastCam_Controller";
    return getController(virtual_VELA_FastCam_Controller_Obj,
                         name,withVM,withEPICS,CLARA);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::offline_VELA_FastCam_Controller()
{
    std::string name  = "offline_VELA_FastCam_Controller";
    return getController(offline_VELA_FastCam_Controller_Obj,
                         name,withoutVM,withoutEPICS,VELA);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::physical_VELA_FastCam_Controller()
{
    std::string name  = "physical_VELA_FastCam_Controller";
    return getController(physical_VELA_FastCam_Controller_Obj,
                         name,withoutVM,withEPICS,VELA);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::virtual_CB1_FastCam_Controller()
{
    std::string name  = "virtual_CB1_FastCam_Controller";
    return getController(virtual_CB1_FastCam_Controller_Obj,
                         name,withVM,withEPICS,CLARA_2_BA1);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::offline_CB1_FastCam_Controller()
{
    std::string name  = "offline_CB1_FastCam_Controller";
    return getController(offline_CB1_FastCam_Controller_Obj,
                         name,withoutVM,withoutEPICS,CLARA_2_BA1);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::physical_CB1_FastCam_Controller()
{
    std::string name  = "physical_CB1_FastCam_Controller";
    return getController(physical_CB1_FastCam_Controller_Obj,
                         name,withoutVM,withEPICS,CLARA_2_BA1);
}
//______________________________________________________________________________
fastCamController& VC_fastCam::getController(MACHINE_MODE mode,MACHINE_AREA area)
{
    if(mode == OFFLINE && area == VELA)
        return offline_VELA_FastCam_Controller();
    else if(mode == VIRTUAL && area == VELA)
        return virtual_VELA_FastCam_Controller();
    else if(mode == PHYSICAL && area == VELA)
        return physical_VELA_FastCam_Controller();

    else if(mode == OFFLINE && area == CLARA)
        return offline_CLARA_FastCam_Controller();
    else if(mode == VIRTUAL && area == CLARA)
        return virtual_CLARA_FastCam_Controller();
    else if(mode == PHYSICAL && area == CLARA)
        return physical_CLARA_FastCam_Controller();

    else if(mode == VIRTUAL && area == CLARA_2_BA1)
        return virtual_CB1_FastCam_Controller();
    else if(mode == PHYSICAL && area == CLARA_2_BA2)
        return physical_CB1_FastCam_Controller();
    else if(mode == OFFLINE && area == CLARA_2_BA1)
        return offline_CB1_FastCam_Controller();

}
//______________________________________________________________________________
fastCamController& VC_fastCam::getController(fastCamController*& cont,
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

        if(myMachineArea == CLARA_2_BA1)
        {
            cont = new fastCamController(messageStates.at(cont).first,
                                    messageStates.at(cont).second,
                                    VELA_CONFIG,
                                    CLARA_CONFIG,
                                    shouldVM,
                                    shouldEPICS,
                                    myMachineArea,
                                    name);
        }
        else if(myMachineArea == CLARA)
        {
            cont = new fastCamController(messageStates.at(cont).first,
                                    messageStates.at(cont).second,
                                    UTL::UNKNOWN_STRING,
                                    CLARA_CONFIG,
                                    shouldVM,
                                    shouldEPICS,
                                    myMachineArea,
                                    name);
        }
        else if(myMachineArea == VELA)
        {
            cont = new fastCamController(messageStates.at(cont).first,
                                    messageStates.at(cont).second,
                                    VELA_CONFIG,
                                    UTL::UNKNOWN_STRING,
                                    shouldVM,
                                    shouldEPICS,
                                    myMachineArea,
                                    name);
        }

    }
    return *cont;
}
//______________________________________________________________________________
void VC_fastCam::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}









