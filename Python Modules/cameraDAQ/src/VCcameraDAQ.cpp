#include "VCcameraDAQ.h"
//stl
#include <iostream>
#include <string>

VCcameraDAQ::VCcameraDAQ():
virtual_Camera_DAQ_Controller_Obj(nullptr),
offline_Camera_DAQ_Controller_Obj(nullptr),
physical_Camera_DAQ_Controller_Obj(nullptr),
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
    std::cout << "Instantiated a VCcameraDAQ in Quiet Mode" << std::endl;
    //ctor
}
VCcameraDAQ::~VCcameraDAQ()
{
}
void VCcameraDAQ::setQuiet()
{
    std::cout << "VCcameraDAQ Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
void VCcameraDAQ::setVerbose()
{
    std::cout << "VCcameraDAQ Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
void VCcameraDAQ::setMessage()
{
    std::cout << "VCcameraDAQ Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
void VCcameraDAQ::setDebugMessage()
{
    std::cout << "VCcameraDAQ DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
cameraDAQController& VCcameraDAQ::physical_CLARA_Camera_DAQ_Controller()
{
    std::string cconf = UTL::CONFIG_PATH + UTL::DAQ_CAMERA_CONFIG;
    std::string name  = "physical_CLARA_Camera_Controller";
    return getController(physical_Camera_DAQ_Controller_Obj,cconf,name,withoutVM,withEPICS,CLARA_PH1);
}
cameraDAQController& VCcameraDAQ::virtual_CLARA_Camera_DAQ_Controller()
{
    std::string mconf = UTL::CONFIG_PATH + UTL::DAQ_CAMERA_CONFIG;
    std::string name  = "virtual_CLARA_Camera_Controller";
    return getController(virtual_Camera_DAQ_Controller_Obj,mconf,name,withVM,withEPICS,CLARA_PH1);
}
cameraDAQController& VCcameraDAQ::offline_CLARA_Camera_DAQ_Controller()
{
    std::string mconf = UTL::CONFIG_PATH + UTL::DAQ_CAMERA_CONFIG;
    std::string name  = "offline_CLARA_Camera_Controller";
    return getController(offline_Camera_DAQ_Controller_Obj,mconf,name,withoutVM,withoutEPICS,CLARA_PH1);
}
cameraDAQController& VCcameraDAQ::physical_VELA_Camera_DAQ_Controller()
{
    std::string cconf = UTL::CONFIG_PATH + UTL::DAQ_CAMERA_CONFIG;
    std::string name  = "physical_VELA_Camera_Controller";
    return getController(physical_Camera_DAQ_Controller_Obj,cconf,name,withoutVM,withEPICS,VELA_INJ);
}
cameraDAQController& VCcameraDAQ::virtual_VELA_Camera_DAQ_Controller()
{
    std::string mconf = UTL::CONFIG_PATH + UTL::DAQ_CAMERA_CONFIG;
    std::string name  = "virtual_VELA_Camera_Controller";
    return getController(virtual_Camera_DAQ_Controller_Obj,mconf,name,withVM,withEPICS,VELA_INJ);
}
cameraDAQController& VCcameraDAQ::offline_VELA_Camera_DAQ_Controller()
{
    std::string mconf = UTL::CONFIG_PATH + UTL::DAQ_CAMERA_CONFIG;
    std::string name  = "offline_VELA_Camera_Controller";
    return getController(offline_Camera_DAQ_Controller_Obj,mconf,name,withoutVM,withoutEPICS,VELA_INJ);
}
cameraDAQController& VCcameraDAQ::getController(cameraDAQController * cont,const std::string& conf,const std::string & name, const bool shouldVM, const bool shouldEPICS, const VELA_ENUM::MACHINE_AREA myMachineArea )
{
    if(cont)
    {
        std::cout << name  << " object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating " << name << " object" << std::endl;
        cont = new cameraDAQController(shouldShowMessage, shouldShowDebugMessage, conf, shouldVM, shouldEPICS, myMachineArea);
    }
    return *cont;
}

