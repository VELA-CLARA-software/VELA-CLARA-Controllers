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
//  FileName:    VCshutter.cpp
//  Description:
//
//
//*/
#include "VCshutter.h"
#include <iostream>

VCshutter::VCshutter():
VCbase("VCshutter"),
virtual_PIL_shutter_Controller_Obj(nullptr),
physical_PIL_shutter_Controller_Obj(nullptr),
offline_PIL_shutter_Controller_Obj(nullptr),
//piLaserShutterConf(UTL::APCLARA1_CONFIG_PATH + UTL::PIL_SHUTTER_CONFIG)
piLaserShutterConf(UTL::CLARANET_CONFIG_PATH + UTL::PIL_SHUTTER_CONFIG)
{
    std::cout << "Instantiated a VCshutter in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
VCshutter::~VCshutter()
{
    //dtor
    if(virtual_PIL_shutter_Controller_Obj)
    {
        delete virtual_PIL_shutter_Controller_Obj;
               virtual_PIL_shutter_Controller_Obj = nullptr;
    }
    if(physical_PIL_shutter_Controller_Obj)
    {
        delete physical_PIL_shutter_Controller_Obj;
               physical_PIL_shutter_Controller_Obj = nullptr;
    }
    if(offline_PIL_shutter_Controller_Obj)
    {
        delete offline_PIL_shutter_Controller_Obj;
               offline_PIL_shutter_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
shutterController&  VCshutter::virtual_PIL_Shutter_Controller()
{
    std::string name = "virtual_PIL_Shutter_Controller";
    return getController(virtual_PIL_shutter_Controller_Obj,
                                name,
                                withVM,
                                withEPICS,
                                piLaserShutterConf
                                );
}
//______________________________________________________________________________
shutterController& VCshutter::offline_PIL_Shutter_Controller()
{
    std::string name = "offline_pilaser_Controller";
    return getController(offline_PIL_shutter_Controller_Obj,
                                name,
                                withoutVM,
                                withoutEPICS,
                                piLaserShutterConf
                                );
}
//______________________________________________________________________________
shutterController& VCshutter::physical_PIL_Shutter_Controller()
{
    std::string name = "physical_PIL_Shutter_Controller";
    return getController(physical_PIL_shutter_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                piLaserShutterConf
                                );
}
//______________________________________________________________________________
shutterController& VCshutter::getController(shutterController*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const std::string& piLaserShutterConf
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
        cont = new shutterController(messageStates.at(cont).first,
                                     messageStates.at(cont).second,
                                     shouldVM,
                                     shouldEPICS,
                                     piLaserShutterConf,
                                     name
                                    );

    }
    return *cont;
}
//______________________________________________________________________________
void VCshutter::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}
