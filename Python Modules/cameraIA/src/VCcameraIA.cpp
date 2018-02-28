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
#include "VCcameraIA.h"
//stl
#include <iostream>
#include <string>

VCcameraIA::VCcameraIA():
virtual_Camera_IA_Controller_Obj(nullptr),
offline_Camera_IA_Controller_Obj(nullptr),
physical_Camera_IA_Controller_Obj(nullptr),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
VELA_INJ ( VELA_ENUM::MACHINE_AREA::VELA_INJ ),
VELA_BA1 ( VELA_ENUM::MACHINE_AREA::VELA_BA1 ),
VELA_BA2 ( VELA_ENUM::MACHINE_AREA::VELA_BA2 ),
CLARA_PH1( VELA_ENUM::MACHINE_AREA::CLARA_PH1),
UNKNOWN_AREA(VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is not show messages!!
{
    std::cout << "Instantiated a VCcameraIA in Quiet Mode" << std::endl;
    //ctor
}
VCcameraIA::~VCcameraIA()
{
}
void VCcameraIA::setQuiet()
{
    std::cout << "VCcameraIA Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
void VCcameraIA::setVerbose()
{
    std::cout << "VCcameraIA Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
void VCcameraIA::setMessage()
{
    std::cout << "VCcameraIA Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
void VCcameraIA::setDebugMessage()
{
    std::cout << "VCcameraIA DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
cameraIAController& VCcameraIA::physical_CLARA_Camera_IA_Controller()
{
    std::string cconf = UTL::APCLARA1_CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "physical_CLARA_Camera_Controller";
    return getController(physical_Camera_IA_Controller_Obj,
                         cconf,
                         name,
                         withoutVM,
                         withEPICS,
                         CLARA_PH1);
}
cameraIAController& VCcameraIA::virtual_CLARA_Camera_IA_Controller()
{
    std::string mconf = UTL::APCLARA1_CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "virtual_CLARA_Camera_Controller";
    return getController(virtual_Camera_IA_Controller_Obj,
                         mconf,
                         name,
                         withVM,
                         withEPICS,
                         CLARA_PH1);
}
cameraIAController& VCcameraIA::offline_CLARA_Camera_IA_Controller()
{
    std::string mconf = UTL::APCLARA1_CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "offline_CLARA_Camera_Controller";
    return getController(offline_Camera_IA_Controller_Obj,
                         mconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         CLARA_PH1);
}
cameraIAController& VCcameraIA::physical_VELA_Camera_IA_Controller()
{
    std::string cconf = UTL::APCLARA1_CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "physical_VELA_Camera_Controller";
    return getController(physical_Camera_IA_Controller_Obj,
                         cconf,
                         name,
                         withoutVM,
                         withEPICS,
                         VELA_INJ);
}
cameraIAController& VCcameraIA::virtual_VELA_Camera_IA_Controller()
{
    std::string mconf = UTL::APCLARA1_CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "virtual_VELA_Camera_Controller";
    return getController(virtual_Camera_IA_Controller_Obj,
                         mconf,
                         name,
                         withVM,
                         withEPICS,
                         VELA_INJ);
}
cameraIAController& VCcameraIA::offline_VELA_Camera_IA_Controller()
{
    std::string mconf = UTL::APCLARA1_CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "offline_VELA_Camera_Controller";
    return getController(offline_Camera_IA_Controller_Obj,
                         mconf,
                         name,
                         withoutVM,
                         withoutEPICS,
                         VELA_INJ);
}
cameraIAController& VCcameraIA::getController(cameraIAController *cont,
                                            const std::string &conf,
                                            const std::string &name,
                                            const bool shouldVM,
                                            const bool shouldEPICS,
                                            const VELA_ENUM::MACHINE_AREA myMachineArea)
{
    if(cont)
    {
        std::cout << name  << " object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating " << name << " object" << std::endl;
        cont = new cameraIAController(shouldShowMessage,
                                       shouldShowDebugMessage,
                                       VELA_ENUM::CONTROLLER_TYPE::CAMERA_IA,
                                       conf,
                                       shouldVM,
                                       shouldEPICS,
                                       myMachineArea);
    }
    return *cont;
}

