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
//  Last edit:   29-03-2018
//  FileName:    VCba1Stages.cpp
//  Description:
//
//
//*/
#include "VCba1Stages.h"
#include <iostream>

VCba1Stages::VCba1Stages():
VCbase("VCba1Stages"),
virtual_BA1_Stages_Controller_Obj(nullptr),
physical_BA1_Stages_Controller_Obj(nullptr),
offline_BA1_Stages_Controller_Obj(nullptr),
ba1StageConf(UTL::CLARANET_CONFIG_PATH + UTL::BA1_STAGES_CONFIG)
{
    std::cout << "Instantiated a VCba1Stages in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
VCba1Stages::~VCba1Stages()
{
    //dtor
    if(virtual_BA1_Stages_Controller_Obj)
    {
        delete virtual_BA1_Stages_Controller_Obj;
               virtual_BA1_Stages_Controller_Obj = nullptr;
    }
    if(physical_BA1_Stages_Controller_Obj)
    {
        delete physical_BA1_Stages_Controller_Obj;
               physical_BA1_Stages_Controller_Obj = nullptr;
    }
    if(offline_BA1_Stages_Controller_Obj)
    {
        delete offline_BA1_Stages_Controller_Obj;
               offline_BA1_Stages_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
ba1StagesController&  VCba1Stages::virtual_BA1_Stages_Controller()
{
    std::string name = "virtual_BA1_Stages_Controller";
    return getController(virtual_BA1_Stages_Controller_Obj,
                                name,
                                withVM,
                                withEPICS,
                                ba1StageConf
                                );
}
//______________________________________________________________________________
ba1StagesController& VCba1Stages::offline_BA1_Stages_Controller()
{
    std::string name = "offline_BA1_Stages_Controller";
    return getController(offline_BA1_Stages_Controller_Obj,
                                name,
                                withoutVM,
                                withoutEPICS,
                                ba1StageConf
                                );
}
//______________________________________________________________________________
ba1StagesController& VCba1Stages::physical_BA1_Stages_Controller()
{
    std::string name = "physical_BA1_Stages_Controller";
    return getController(offline_BA1_Stages_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                ba1StageConf
                                );
}
//______________________________________________________________________________
ba1StagesController& VCba1Stages::getController(ba1StagesController*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const std::string& config
                                        )
{
    if(cont)
    {
        std::cout << name << " object already exists," <<std::endl;
    }
    else
    {
        messageStates[cont].first     = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;
        std::cout << "Creating " << name << " object" <<std::endl;
        cont = new ba1StagesController(messageStates.at(cont).first,
                                     messageStates.at(cont).second,
                                     shouldVM,
                                     shouldEPICS,
                                     config,
                                     name
                                    );

    }
    return *cont;
}
//______________________________________________________________________________
void VCba1Stages::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}
