#include "Vcam.h"
#include <iostream>

Vcam::Vcam():
virtual_Gun_Protection_Controller_Obj(nullptr),
physical_Gun_Protection_Controller_Obj(nullptr),
offline_Gun_Protection_Controller_Obj(nullptr),
velaCamConf(UTL::CONFIG_PATH + UTL::RF_GUN_PROTECTION_CONFIG),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is quiet mode
{
   std::cout << "Instantiated a VCrfprot " << std::endl;
    //ctor
}
Vcam::Vcam()
{
    //ctor
}
//______________________________________________________________________________
Vcam::~Vcam()
{
    //dtor
}
//______________________________________________________________________________
void Vcam::setQuiet()
{
    std::cout << "Vcam Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void Vcam::setVerbose()
{
    std::cout << "Vcam Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void Vcam::setMessage()
{
    std::cout << "Vcam Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void Vcam::setDebugMessage()
{
    std::cout << "Vcam DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
velaCamController& Vcam::virtual_VELA_Camera_Controller()
{
    if(virtual_VELA_Camera_Controller_Obj)
    {
        std::cout << "virtual_Gun_Protection_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_Gun_Protection_Controller object" << std::endl;
        virtual_VELA_Camera_Controller_Obj =
            new gunProtController(shouldShowMessage,shouldShowDebugMessage,velaCamConf, withVM,withEPICS);
    }
    return *virtual_VELA_Camera_Controller_Obj;
}
//______________________________________________________________________________
velaCamController& Vcam::physical_VELA_Camera_Controller()
{
    if(physical_VELA_Camera_Controller_Obj)
    {
        std::cout << "physical_VELA_Camera_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_Camera_Controller object" << std::endl;
        physical_VELA_Camera_Controller_Obj =
            new gunProtController(shouldShowMessage,shouldShowDebugMessage,velaCamConf, withoutVM,withEPICS);
    }
    return *physical_VELA_Camera_Controller_Obj;
}
//______________________________________________________________________________
velaCamController& Vcam::offline_VELA_Camera_Controller()
{
    if(offline_VELA_Camera_Controller_Obj)
    {
        std::cout << "offline_VELA_Camera_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_Camera_Controller object" << std::endl;
        offline_VELA_Camera_Controller_Obj =
            new gunProtController(shouldShowMessage,shouldShowDebugMessage,velaCamConf,withoutVM,withoutEPICS);
    }
    return *offline_VELA_Camera_Controller_Obj;
}


