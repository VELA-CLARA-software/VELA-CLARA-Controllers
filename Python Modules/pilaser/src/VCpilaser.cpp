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
//  Last edit:   07-08-2018
//  FileName:    VCpilaser.cpp
//  Description:
//
//
//*/
#include "VCpilaser.h"
#include <iostream>

VCpilaser::VCpilaser():
VCbase("VCpilaser"),
virtual_VELA_pilaser_Controller_Obj(nullptr),
offline_VELA_pilaser_Controller_Obj(nullptr),
physical_VELA_pilaser_Controller_Obj(nullptr),
virtual_CLARA_pilaser_Controller_Obj(nullptr),
offline_CLARA_pilaser_Controller_Obj(nullptr),
physical_CLARA_pilaser_Controller_Obj(nullptr),
//pilaserConf(UTL::APCLARA1_CONFIG_PATH + UTL::PI_LASER_CONFIG),
pilaserConf(UTL::CLARANET_CONFIG_PATH + UTL::PI_LASER_CONFIG),
//velaCamConfig(UTL::APCLARA1_CONFIG_PATH + UTL::VELA_CAMERA_CONFIG),
velaCamConfig(UTL::CLARANET_CONFIG_PATH + UTL::VELA_CAMERA_CONFIG),
//piLaserMirrorConf(UTL::APCLARA1_CONFIG_PATH + UTL::PIL_MIRROR_CONFIG),
piLaserMirrorConf(UTL::CLARANET_CONFIG_PATH + UTL::PIL_MIRROR_CONFIG),
//claraCamConfig(UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_CAMERA_CONFIG)
claraCamConfig(UTL::CLARANET_CONFIG_PATH + UTL::CLARA_CAMERA_CONFIG)
{
    std::cout <<"Instantiated a VCpilaser in Quiet Mode" <<std::endl;
    //ctor
}
//______________________________________________________________________________
VCpilaser::~VCpilaser()
{
    std::cout <<"starting desctruction" <<std::endl;
    //dtor
    if(virtual_VELA_pilaser_Controller_Obj)
    {
        delete virtual_VELA_pilaser_Controller_Obj;
               virtual_VELA_pilaser_Controller_Obj = nullptr;
    }
    if(offline_VELA_pilaser_Controller_Obj)
    {
        delete offline_VELA_pilaser_Controller_Obj;
               offline_VELA_pilaser_Controller_Obj = nullptr;
    }
    if(physical_VELA_pilaser_Controller_Obj)
    {
        delete physical_VELA_pilaser_Controller_Obj;
               physical_VELA_pilaser_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_pilaser_Controller_Obj)
    {
        delete virtual_CLARA_pilaser_Controller_Obj;
               virtual_CLARA_pilaser_Controller_Obj = nullptr;
    }
    if(offline_CLARA_pilaser_Controller_Obj)
    {
        delete offline_CLARA_pilaser_Controller_Obj;
               offline_CLARA_pilaser_Controller_Obj = nullptr;
    }
    if(physical_CLARA_pilaser_Controller_Obj)
    {
        delete physical_CLARA_pilaser_Controller_Obj;
               physical_CLARA_pilaser_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
pilaserController&  VCpilaser::virtual_PILaser_Controller()
{
    std::string name = "virtual_CLARA_pilaser_Controller";
    std::cout <<"virtual_pilaser_Controller" <<std::endl;
    return getpilaserController(virtual_CLARA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::CLARA
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::offline_PILaser_Controller()
{
    std::string name = "offline_CLARA_pilaser_Controller";
    std::cout <<"offline_pilaser_Controller" <<std::endl;
    return getpilaserController(offline_CLARA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::CLARA
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::physical_PILaser_Controller()
{
    std::string name = "physical_CLARA_PILaser_Controller";
    std::cout <<"physical_PILaser_Controller" <<std::endl;
    return getpilaserController(physical_CLARA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::CLARA
                                );
}
//______________________________________________________________________________
pilaserController&  VCpilaser::virtual_CLARA_PILaser_Controller()
{
    std::string name = "virtual_CLARA_pilaser_Controller";
    std::cout <<"virtual_pilaser_Controller" <<std::endl;
    return getpilaserController(virtual_CLARA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::CLARA
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::offline_CLARA_PILaser_Controller()
{
    std::string name = "offline_CLARA_pilaser_Controller";
    std::cout <<"offline_pilaser_Controller" <<std::endl;
    return getpilaserController(offline_CLARA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::CLARA
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::physical_CLARA_PILaser_Controller()
{
    std::string name = "physical_VELA_PILaser_Controller";
    std::cout <<"physical_PILaser_Controller" <<std::endl;
    return getpilaserController(physical_CLARA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::CLARA
                                );
}

//______________________________________________________________________________
pilaserController&  VCpilaser::virtual_VELA_PILaser_Controller()
{
    std::string name = "virtual_VELA_pilaser_Controller";
    std::cout <<"virtual_pilaser_Controller" <<std::endl;
    return getpilaserController(virtual_VELA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::VELA
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::offline_VELA_PILaser_Controller()
{
    std::string name = "offline_VELA_pilaser_Controller";
    std::cout <<"offline_VELA_pilaser_Controller" <<std::endl;
    return getpilaserController(offline_VELA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::VELA
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::physical_VELA_PILaser_Controller()
{
    std::string name = "physical_VELA_PILaser_Controller";
    std::cout <<"physical_VELA_PILaser_Controller" <<std::endl;
    return getpilaserController(physical_VELA_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                HWC_ENUM::MACHINE_AREA::VELA
                                );
}



//______________________________________________________________________________
pilaserController& VCpilaser::getpilaserController(pilaserController*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const HWC_ENUM::MACHINE_AREA area
//                                           const std::string& pilaserConf,
//                                           const std::string& vcAnalysisConf,
//                                           const std::string& piLaserMirrorConf
                                        )
{
    if(cont)
    {
        std::cout <<name <<" object already exists," <<std::endl;
    }
    else
    {
        messageStates[cont].first     = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;
        std::cout <<"Creating " <<name <<" object" <<std::endl;
        cont = new pilaserController(messageStates.at(cont).first,
                                     messageStates.at(cont).second,
                                     shouldVM,
                                     shouldEPICS,
                                     name,
                                     pilaserConf,
                                     claraCamConfig,
                                     piLaserMirrorConf,
                                     velaCamConfig,
                                     area
                                    );

    }
    return *cont;
}
//______________________________________________________________________________
void VCpilaser::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}
