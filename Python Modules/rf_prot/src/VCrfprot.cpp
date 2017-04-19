#include "VCrfprot.h"
#include <iostream>

VCrfprot::VCrfprot():
virtual_Gun_Protection_Controller_Obj(nullptr),
physical_Gun_Protection_Controller_Obj(nullptr),
offline_Gun_Protection_Controller_Obj(nullptr),
allGunProtsConf(UTL::CONFIG_PATH + UTL::RF_GUN_PROTECTION_CONFIG),
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
//______________________________________________________________________________
VCrfprot::~VCrfprot()
{
    //dtor
    if(virtual_Gun_Protection_Controller_Obj)
    {
        delete virtual_Gun_Protection_Controller_Obj;
               virtual_Gun_Protection_Controller_Obj = nullptr;
    }
    if(physical_Gun_Protection_Controller_Obj)
    {
        delete physical_Gun_Protection_Controller_Obj;
               physical_Gun_Protection_Controller_Obj = nullptr;
    }
    if(offline_Gun_Protection_Controller_Obj)
    {
        delete offline_Gun_Protection_Controller_Obj;
               offline_Gun_Protection_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
void VCrfprot::setQuiet()
{
    std::cout << "VCrfprot Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCrfprot::setVerbose()
{
    std::cout << "VCrfprot Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCrfprot::setMessage()
{
    std::cout << "VCrfprot Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCrfprot::setDebugMessage()
{
    std::cout << "VCrfprot DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
gunProtController& VCrfprot::virtual_Gun_Protection_Controller()
{
    if(virtual_Gun_Protection_Controller_Obj)
    {
        std::cout << "virtual_Gun_Protection_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_Gun_Protection_Controller object" << std::endl;
        virtual_Gun_Protection_Controller_Obj =
            new gunProtController(shouldShowMessage,shouldShowDebugMessage,allGunProtsConf, withVM,withEPICS);
    }
    return *virtual_Gun_Protection_Controller_Obj;
}
//______________________________________________________________________________
gunProtController& VCrfprot::physical_Gun_Protection_Controller()
{
    if(physical_Gun_Protection_Controller_Obj)
    {
        std::cout << "physical_Gun_Protection_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_Gun_Protection_Controller object" << std::endl;
        physical_Gun_Protection_Controller_Obj =
            new gunProtController(shouldShowMessage,shouldShowDebugMessage,allGunProtsConf, withoutVM,withEPICS);
    }
    return *physical_Gun_Protection_Controller_Obj;
}
//______________________________________________________________________________
gunProtController& VCrfprot::offline_Gun_Protection_Controller()
{
    if(offline_Gun_Protection_Controller_Obj)
    {
        std::cout << "offline_Gun_ProtectionController object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_Gun_ProtectionController object" << std::endl;
        offline_Gun_Protection_Controller_Obj =
            new gunProtController(shouldShowMessage,shouldShowDebugMessage,allGunProtsConf,withoutVM,withoutEPICS);
    }
    return *offline_Gun_Protection_Controller_Obj;
}















