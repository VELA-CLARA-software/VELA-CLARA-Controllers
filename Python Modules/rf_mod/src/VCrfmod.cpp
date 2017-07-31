#include "VCrfmod.h"
#include <iostream>

VCrfmod::VCrfmod():
virtual_GUN_MOD_Controller_Obj(nullptr),
physical_GUN_MOD_Controller_Obj(nullptr),
virtual_L01_MOD_Controller_Obj(nullptr),
physical_L01_MOD_Controller_Obj(nullptr),
offline_L01_MOD_Controller_Obj(nullptr),
gunModConf(UTL::CONFIG_PATH + UTL::RF_GUN_MOD_CONFIG),
l01ModConf(UTL::CONFIG_PATH + UTL::L01_MOD_CONFIG),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is quiet mode
{
   std::cout << "Instantiated a VCrfmod " << std::endl;
    //ctor
}
//______________________________________________________________________________
VCrfmod::~VCrfmod()
{
    //dtor
    if(virtual_GUN_MOD_Controller_Obj)
    {
        delete virtual_GUN_MOD_Controller_Obj;
               virtual_GUN_MOD_Controller_Obj = nullptr;
    }
    if(physical_GUN_MOD_Controller_Obj)
    {
        delete physical_GUN_MOD_Controller_Obj;
               physical_GUN_MOD_Controller_Obj = nullptr;
    }
    if(offline_GUN_MOD_Controller_Obj)
    {
        delete offline_GUN_MOD_Controller_Obj;
               offline_GUN_MOD_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
void VCrfmod::setQuiet()
{
    std::cout << "VCrfmod Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCrfmod::setVerbose()
{
    std::cout << "VCrfmod Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCrfmod::setMessage()
{
    std::cout << "VCrfmod Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCrfmod::setDebugMessage()
{
    std::cout << "VCrfmod DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
gunModController& VCrfmod::virtual_GUN_MOD_Controller()
{
    if( virtual_GUN_MOD_Controller_Obj )
    {
        std::cout << "virtual_GUN_MOD_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_GUN_MOD_Controller object" << std::endl;
        virtual_GUN_MOD_Controller_Obj =
            new gunModController(shouldShowMessage,shouldShowDebugMessage,gunModConf, withVM,withEPICS);
    }
    return *virtual_GUN_MOD_Controller_Obj;
}
//______________________________________________________________________________
gunModController& VCrfmod::physical_GUN_MOD_Controller()
{
    if( physical_GUN_MOD_Controller_Obj )
    {
        std::cout << "physical_GUN_MOD_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_GUN_MOD_Controller object" << std::endl;
        physical_GUN_MOD_Controller_Obj =
            new gunModController(shouldShowMessage,shouldShowDebugMessage,gunModConf, withoutVM,withEPICS);
    }
    return *physical_GUN_MOD_Controller_Obj;
}
//______________________________________________________________________________
gunModController& VCrfmod::offline_GUN_MOD_Controller()
{
    if( offline_GUN_MOD_Controller_Obj )
    {
        std::cout << "offline_GUN_MOD_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_GUN_MOD_Controller object" << std::endl;
        offline_GUN_MOD_Controller_Obj =
            new gunModController(shouldShowMessage,shouldShowDebugMessage,gunModConf,withoutVM,withoutEPICS);
    }
    return *offline_GUN_MOD_Controller_Obj;
}
//______________________________________________________________________________
l01ModController& VCrfmod::virtual_L01_MOD_Controller()
{
    if( virtual_L01_MOD_Controller_Obj )
    {
        std::cout << "virtual_L01_MOD_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_L01_MOD_Controller object" << std::endl;
        virtual_L01_MOD_Controller_Obj =
            new l01ModController(shouldShowMessage,shouldShowDebugMessage,l01ModConf,withVM,withEPICS);
    }
    return *virtual_L01_MOD_Controller_Obj;
}
//______________________________________________________________________________
l01ModController& VCrfmod::physical_L01_MOD_Controller()
{
    if(physical_L01_MOD_Controller_Obj)
    {
        std::cout << "physical_L01_MOD_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_L01_MOD_Controller_Obj object" << std::endl;
        physical_L01_MOD_Controller_Obj =
            new l01ModController(shouldShowMessage,shouldShowDebugMessage,l01ModConf,withoutVM,withEPICS);
    }
    return *physical_L01_MOD_Controller_Obj;
}
//______________________________________________________________________________
l01ModController& VCrfmod::offline_L01_MOD_Controller()
{
    if( offline_L01_MOD_Controller_Obj )
    {
        std::cout << "offline_L01_MOD_Controller_Obj object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_L01_MOD_Controller_Obj object" << std::endl;
        offline_L01_MOD_Controller_Obj =
            new l01ModController(shouldShowMessage,shouldShowDebugMessage,gunModConf,withoutVM,withoutEPICS);
    }
    return *offline_L01_MOD_Controller_Obj;
}





