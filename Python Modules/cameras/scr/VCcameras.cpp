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
//  Last edit:   18-06-2018
//  FileName:    VCcameras.cpp
//  Description:
//
//
//*/
#include "VCcameras.h"
//stl
#include <iostream>
#include <string>

VCcameras::VCcameras():
VCbase("VCcameras"),
virtual_Camera_Controller_Obj(nullptr),
offline_Camera_Controller_Obj(nullptr),
physical_Camera_Controller_Obj(nullptr),
claraCamConfig(UTL::APCLARA1_CONFIG_PATH + UTL::CLARA_CAMERA_CONFIG)
{
    std::cout << "Instantiated a VCcameras in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
VCcameras::~VCcameras()
{
    //dtor
    if(virtual_Camera_Controller_Obj)
    {
        delete virtual_Camera_Controller_Obj;
               virtual_Camera_Controller_Obj = nullptr;
    }
    if(offline_Camera_Controller_Obj)
    {
        delete offline_Camera_Controller_Obj;
               offline_Camera_Controller_Obj = nullptr;
    }
    if(physical_Camera_Controller_Obj)
    {
        delete physical_Camera_Controller_Obj;
               physical_Camera_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
cameraControllerBase& VCcameras::physical_Camera_Controller()
{
    std::string name = "physical_Camera_Controller";
    std::cout <<"physical_Camera_Controller" <<std::endl;
    return getController(physical_Camera_Controller_Obj,
                         name,
                         withoutVM,
                         withEPICS);
}
//______________________________________________________________________________
cameraControllerBase& VCcameras::virtual_Camera_Controller()
{
    std::string name = "virtual_Camera_Controller";
    std::cout <<"virtual_Camera_Controller" <<std::endl;
    return getController(virtual_Camera_Controller_Obj,
                         name,
                         withVM,
                         withEPICS);
}
//______________________________________________________________________________
cameraControllerBase& VCcameras::offline_Camera_Controller()
{
    std::string name = "offline_CLARA_Camera_Controller";
    std::cout <<"offline_CLARA_Camera_Controller" <<std::endl;
    return getController(offline_Camera_Controller_Obj,
                         name,
                         withoutVM,
                         withoutEPICS);
}
//______________________________________________________________________________
cameraControllerBase& VCcameras::getController(cameraControllerBase*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS)
{
    if(cont)
    {
        std::cout <<name <<" object already exists," <<std::endl;
    }
    else
    {
        messageStates[cont].first     = shouldShowMessage;
        messageStates.at(cont).second = shouldShowDebugMessage;
        std::cout << "Creating " << name << " object" <<std::endl;
        cont = new cameraControllerBase(messageStates.at(cont).first,
                                        messageStates.at(cont).second,
                                        shouldVM,
                                        shouldEPICS,
                                        name,
                                        claraCamConfig,
                                        HWC_ENUM::CONTROLLER_TYPE::CAMERA,
                                        false);
    }
    return *cont;
}
//______________________________________________________________________________
void VCcameras::updateMessageStates()
{
    for(auto&& it:messageStates)
    {
        it.second.first  = shouldShowMessage;
        it.second.second = shouldShowDebugMessage;
    }
}
