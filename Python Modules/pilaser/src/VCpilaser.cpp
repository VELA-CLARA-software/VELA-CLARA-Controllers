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
//  FileName:    VCpilaser.cpp
//  Description:
//
//
//*/
#include "VCpilaser.h"
#include <iostream>

VCpilaser::VCpilaser():
VCbase("VCpilaser"),
virtual_pilaser_Controller_Obj(nullptr),
offline_pilaser_Controller_Obj(nullptr),
physical_pilaser_Controller_Obj(nullptr),
pilaserConf(UTL::APCLARA1_CONFIG_PATH + UTL::PI_LASER_CONFIG),
vcAnalysisConf(UTL::APCLARA1_CONFIG_PATH + UTL::VIRTUAL_CATHODE_ANALYSIS),
piLaserMirrorConf(UTL::APCLARA1_CONFIG_PATH + UTL::PIL_MIRROR_CONFIG),
piLaserShutterConf(UTL::APCLARA1_CONFIG_PATH + UTL::PIL_SHUTTER_CONFIG)
{
    std::cout << "Instantiated a VCpilaser in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
VCpilaser::~VCpilaser()
{
    //dtor
    if(virtual_pilaser_Controller_Obj)
    {
        delete virtual_pilaser_Controller_Obj;
               virtual_pilaser_Controller_Obj = nullptr;
    }
    if(offline_pilaser_Controller_Obj)
    {
        delete offline_pilaser_Controller_Obj;
               offline_pilaser_Controller_Obj = nullptr;
    }
    if(physical_pilaser_Controller_Obj)
    {
        delete physical_pilaser_Controller_Obj;
               physical_pilaser_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
pilaserController&  VCpilaser::virtual_PILaser_Controller()
{
    std::string name = "virtual_pilaser_Controller";
    std::cout << "virtual_pilaser_Controller" << std::endl;
    return getpilaserController(virtual_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                pilaserConf,
                                vcAnalysisConf,
                                piLaserMirrorConf,
                                piLaserShutterConf
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::offline_PILaser_Controller()
{
    std::string name = "offline_pilaser_Controller";
    std::cout << "offline_pilaser_Controller" << std::endl;
    return getpilaserController(offline_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                pilaserConf,
                                vcAnalysisConf,
                                piLaserMirrorConf,
                                piLaserShutterConf
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::physical_PILaser_Controller()
{
    std::string name = "physical_PILaser_Controller";
    std::cout << "physical_PILaser_Controller" << std::endl;
    return getpilaserController(physical_pilaser_Controller_Obj,
                                name,
                                withoutVM,
                                withEPICS,
                                pilaserConf,
                                vcAnalysisConf,
                                piLaserMirrorConf,
                                piLaserShutterConf
                                );
}
//______________________________________________________________________________
pilaserController& VCpilaser::getpilaserController(pilaserController*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const std::string& pilaserConf,
                                           const std::string& vcAnalysisConf,
                                           const std::string& piLaserMirrorConf,
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
        cont = new pilaserController(messageStates.at(cont).first,
                                     messageStates.at(cont).second,
                                     shouldVM,
                                     shouldEPICS,
                                     name,
                                     pilaserConf,
                                     vcAnalysisConf,
                                     piLaserMirrorConf,
                                     piLaserShutterConf
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
