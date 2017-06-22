#include "VCcameras.h"
//stl
#include <iostream>
#include <string>

VCcameras::VCcameras():
virtual_Camera_Controller_Obj(nullptr),
offline_Camera_Controller_Obj(nullptr),
physical_Camera_Controller_Obj(nullptr),
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
shouldShowMessage(false)//default is show messages!!
{
    std::cout << "Instantiated a VCcameras in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
VCcameras::~VCcameras()
{
}
//______________________________________________________________________________
void VCcameras::setQuiet()
{
    std::cout << "VCcameras Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCcameras::setVerbose()
{
    std::cout << "VCcameras Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCcameras::setMessage()
{
    std::cout << "VCcameras Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCcameras::setDebugMessage()
{
    std::cout << "VCcameras DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
cameraController& VCcameras::physical_CLARA_Camera_Controller()
{
    std::string cconf = UTL::CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "physical_CLARA_Camera_Controller";
    return getController(physical_Camera_Controller_Obj,cconf,name,withoutVM,withEPICS,CLARA_PH1);
}
//______________________________________________________________________________
cameraController& VCcameras::virtual_CLARA_Camera_Controller()
{
    std::string mconf = UTL::CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "virtual_CLARA_Camera_Controller";
    return getController(virtual_Camera_Controller_Obj,mconf,name,withVM,withEPICS,CLARA_PH1);
}
//______________________________________________________________________________
cameraController& VCcameras::offline_CLARA_Camera_Controller()
{
    std::string mconf = UTL::CONFIG_PATH + UTL::CAMERA_CONFIG;
    std::string name  = "offline_CLARA_Camera_Controller";
    return getController(offline_Camera_Controller_Obj,mconf,name,withoutVM,withoutEPICS,CLARA_PH1);
}
//______________________________________________________________________________
cameraController& VCcameras::getController(cameraController * cont,const std::string& conf,const std::string & name, const bool shouldVM, const bool shouldEPICS, const VELA_ENUM::MACHINE_AREA myMachineArea )
{
    if(cont)
    {
        std::cout << name  << " object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating " << name << " object" << std::endl;
        cont = new cameraController(shouldShowMessage, shouldShowDebugMessage, conf, shouldVM, shouldEPICS, myMachineArea);
    }
    return *cont;
}
