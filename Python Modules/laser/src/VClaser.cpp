#include "VClaser.h"
#include <iostream>

VClaser::VClaser():
virtual_Laser_Controller_Obj(nullptr),
offline_Laser_Controller_Obj(nullptr),
physical_Laser_Controller_Obj(nullptr),
laserConf(UTL::CONFIG_PATH + UTL::LASER_CONFIG),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is quiet mode
{
    std::cout << "Instantiated a VClaser in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
void VClaser::setQuiet()
{
    std::cout << "VClaser Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VClaser::setVerbose()
{
    std::cout << "VClaser Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VClaser::setMessage()
{
    std::cout << "VClaser Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VClaser::setDebugMessage()
{
    std::cout << "VClaser DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
VClaser::~VClaser()
{
    //dtor
    if(virtual_Laser_Controller_Obj)
    {
        delete virtual_Laser_Controller_Obj;
               virtual_Laser_Controller_Obj = nullptr;
    }
    if(offline_Laser_Controller_Obj)
    {
        delete offline_Laser_Controller_Obj;
               offline_Laser_Controller_Obj = nullptr;
    }
    if(physical_Laser_Controller_Obj)
    {
        delete physical_Laser_Controller_Obj;
               physical_Laser_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
laserController& VClaser::virtual_Laser_Controller()
{
    if( virtual_Laser_Controller_Obj )
    {
        std::cout << "virtual_Laser_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_Laser_Controller object" << std::endl;
        virtual_Laser_Controller_Obj =
            new laserController(shouldShowMessage,shouldShowDebugMessage,laserConf, withVM,withEPICS);
    }
    return *virtual_Laser_Controller_Obj;
}
//______________________________________________________________________________
laserController& VClaser::offline_Laser_Controller()
{
    if( offline_Laser_Controller_Obj )
    {
        std::cout << "offline_Laser_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_Laser_Controller object" << std::endl;
        offline_Laser_Controller_Obj =
            new laserController(shouldShowMessage,shouldShowDebugMessage,laserConf,withoutVM,withoutEPICS);
    }
    return *offline_Laser_Controller_Obj;
}
//______________________________________________________________________________
laserController& VClaser::physical_Laser_Controller()
{
    if( physical_Laser_Controller_Obj )
    {
        std::cout << "physical_Laser_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_Laser_Controller object" << std::endl;
        physical_Laser_Controller_Obj =
            new laserController(shouldShowMessage,shouldShowDebugMessage,laserConf,withoutVM,withEPICS);
    }
    return *physical_Laser_Controller_Obj;
}
//______________________________________________________________________________
laserController& VClaser::getlaserController( VELA_ENUM::MACHINE_MODE mode )
{
    switch( mode )
    {
        case VELA_ENUM::OFFLINE:
            return virtual_Laser_Controller();
            break;
        case VELA_ENUM::PHYSICAL:
            return physical_Laser_Controller();
            break;
        case VELA_ENUM::VIRTUAL:
            return virtual_Laser_Controller();
            break;
    }
}















