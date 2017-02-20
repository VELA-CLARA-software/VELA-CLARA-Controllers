#include "VCpilaser.h"
#include <iostream>

VCpilaser::VCpilaser():
virtual_pilaser_Controller_Obj(nullptr),
offline_pilaser_Controller_Obj(nullptr),
physical_pilaser_Controller_Obj(nullptr),
pilaserConf(UTL::CONFIG_PATH + UTL::PILASER_CONFIG),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is quiet mode
{
    std::cout << "Instantiated a VCpilaser in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
void VCpilaser::setQuiet()
{
    std::cout << "VCpilaser Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCpilaser::setVerbose()
{
    std::cout << "VCpilaser Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCpilaser::setMessage()
{
    std::cout << "VCpilaser Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCpilaser::setDebugMessage()
{
    std::cout << "VCpilaser DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
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
pilaserController& VCpilaser::virtual_PILaser_Controller()
{
    if( virtual_pilaser_Controller_Obj )
    {
        std::cout << "virtual_PILaser_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_PILaser_Controller object" << std::endl;
        virtual_pilaser_Controller_Obj =
            new pilaserController(shouldShowMessage,shouldShowDebugMessage,pilaserConf, withVM,withEPICS);
    }
    return *virtual_pilaser_Controller_Obj;
}
//______________________________________________________________________________
pilaserController& VCpilaser::offline_PILaser_Controller()
{
    if( offline_pilaser_Controller_Obj )
    {
        std::cout << "offline_PILaser_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_PILaser_Controller object" << std::endl;
        offline_pilaser_Controller_Obj =
            new pilaserController(shouldShowMessage,shouldShowDebugMessage,pilaserConf,withoutVM,withoutEPICS);
    }
    return *offline_pilaser_Controller_Obj;
}
//______________________________________________________________________________
pilaserController& VCpilaser::physical_PILaser_Controller()
{
    if( physical_pilaser_Controller_Obj )
    {
        std::cout << "physical_PILaser_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_PILaser_Controller object" << std::endl;
        physical_pilaser_Controller_Obj =
            new pilaserController(shouldShowMessage,shouldShowDebugMessage,pilaserConf,withoutVM,withEPICS);
    }
    return *physical_pilaser_Controller_Obj;
}
//______________________________________________________________________________
pilaserController& VCpilaser::getpilaserController( VELA_ENUM::MACHINE_MODE mode )
{
    switch( mode )
    {
        case VELA_ENUM::OFFLINE:
            return virtual_PILaser_Controller();
            break;
        case VELA_ENUM::PHYSICAL:
            return physical_PILaser_Controller();
            break;
        case VELA_ENUM::VIRTUAL:
            return virtual_PILaser_Controller();
            break;
    }
}















