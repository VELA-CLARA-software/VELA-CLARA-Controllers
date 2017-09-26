#include "VCscreens.h"
#include <iostream>

VCscreens::VCscreens():
virtual_VELA_INJ_Screen_Controller_Obj(nullptr),
offline_VELA_INJ_Screen_Controller_Obj (nullptr),
physical_VELA_INJ_Screen_Controller_Obj(nullptr),

virtual_VELA_BA1_Screen_Controller_Obj (nullptr),
offline_VELA_BA1_Screen_Controller_Obj(nullptr),
physical_VELA_BA1_Screen_Controller_Obj(nullptr),

virtual_VELA_BA2_Screen_Controller_Obj(nullptr),
offline_VELA_BA2_Screen_Controller_Obj(nullptr),
physical_VELA_BA2_Screen_Controller_Obj(nullptr),

virtual_CLARA_INJ_Screen_Controller_Obj(nullptr),
offline_CLARA_INJ_Screen_Controller_Obj(nullptr),
physical_CLARA_INJ_Screen_Controller_Obj(nullptr),

virtual_CLARA_PH1_Screen_Controller_Obj(nullptr),
offline_CLARA_PH1_Screen_Controller_Obj(nullptr),
physical_CLARA_PH1_Screen_Controller_Obj(nullptr),

withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
VELA_INJ ( VELA_ENUM::MACHINE_AREA::VELA_INJ ),
VELA_BA1 ( VELA_ENUM::MACHINE_AREA::VELA_BA1 ),
VELA_BA2 ( VELA_ENUM::MACHINE_AREA::VELA_BA2 ),
CLARA_INJ( VELA_ENUM::MACHINE_AREA::CLARA_INJ),
CLARA_PH1( VELA_ENUM::MACHINE_AREA::CLARA_PH1),
UNKNOWN_AREA(VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is quiet mode
{
    std::cout << "Instantiated a VCscreens in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
void VCscreens::setQuiet()
{
    std::cout << "VCscreens Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCscreens::setVerbose()
{
    std::cout << "VCscreens Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCscreens::setMessage()
{
    std::cout << "VCscreens Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCscreens::setDebugMessage()
{
    std::cout << "VCscreens DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
VCscreens::~VCscreens()
{
    //dtor
    if(virtual_VELA_INJ_Screen_Controller_Obj)
    {
        delete virtual_VELA_INJ_Screen_Controller_Obj;
               virtual_VELA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Screen_Controller_Obj)
    {
        delete physical_VELA_INJ_Screen_Controller_Obj;
               physical_VELA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Screen_Controller_Obj)
    {
        delete offline_VELA_INJ_Screen_Controller_Obj;
               offline_VELA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_Screen_Controller_Obj)
    {
        delete virtual_VELA_BA1_Screen_Controller_Obj;
               virtual_VELA_BA1_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_Screen_Controller_Obj)
    {
        delete physical_VELA_BA1_Screen_Controller_Obj;
               physical_VELA_BA1_Screen_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_Screen_Controller_Obj)
    {
        delete virtual_VELA_BA2_Screen_Controller_Obj;
               virtual_VELA_BA2_Screen_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Screen_Controller_Obj)
    {
        delete offline_VELA_BA2_Screen_Controller_Obj;
               offline_VELA_BA2_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Screen_Controller_Obj)
    {
        delete physical_VELA_BA2_Screen_Controller_Obj;
               physical_VELA_BA2_Screen_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_INJ_Screen_Controller_Obj)
    {
        delete virtual_CLARA_INJ_Screen_Controller_Obj;
               virtual_CLARA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(offline_CLARA_INJ_Screen_Controller_Obj)
    {
        delete offline_CLARA_INJ_Screen_Controller_Obj;
               offline_CLARA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(physical_CLARA_INJ_Screen_Controller_Obj)
    {
        delete physical_CLARA_INJ_Screen_Controller_Obj;
               physical_CLARA_INJ_Screen_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_PH1_Screen_Controller_Obj)
    {
        delete virtual_CLARA_PH1_Screen_Controller_Obj;
               virtual_CLARA_PH1_Screen_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_Screen_Controller_Obj)
    {
        delete offline_CLARA_PH1_Screen_Controller_Obj;
               offline_CLARA_PH1_Screen_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_Screen_Controller_Obj)
    {
        delete physical_CLARA_PH1_Screen_Controller_Obj;
               physical_CLARA_PH1_Screen_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_INJ_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;

    if( virtual_VELA_INJ_Screen_Controller_Obj )
    {
        std::cout << "virtual_VELA_INJ_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_VELA_INJ_Screen_Controller object" << std::endl;
        virtual_VELA_INJ_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withEPICS,VELA_INJ);
    }
    return *virtual_VELA_INJ_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_INJ_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( offline_VELA_INJ_Screen_Controller_Obj )
    {
        std::cout << "offline_VELA_INJ_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_INJ_Screen_Controller object" << std::endl;
        offline_VELA_INJ_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withoutEPICS,VELA_INJ);
    }
    return *offline_VELA_INJ_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_INJ_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( physical_VELA_INJ_Screen_Controller_Obj )
    {
        std::cout << "physical_VELA_INJ_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_INJ_Screen_Controller object" << std::endl;
        physical_VELA_INJ_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withEPICS,VELA_INJ);
    }
    return *physical_VELA_INJ_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_BA1_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( virtual_VELA_BA1_Screen_Controller_Obj )
    {
        std::cout << "virtual_VELA_BA1_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_VELA_BA1_Screen_Controller object" << std::endl;
        virtual_VELA_BA1_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withEPICS,VELA_BA1);
    }
    return *virtual_VELA_BA1_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_BA1_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( offline_VELA_BA1_Screen_Controller_Obj )
    {
        std::cout << "offline_VELA_BA1_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_BA1_Screen_Controller object" << std::endl;
        offline_VELA_BA1_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withoutEPICS,VELA_BA1);
    }
    return *offline_VELA_BA1_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_BA1_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( physical_VELA_BA1_Screen_Controller_Obj )
    {
        std::cout << "physical_VELA_BA1_Screen_Controlle object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_BA1_Screen_Controlle object" << std::endl;
        physical_VELA_BA1_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withEPICS,VELA_BA1);
    }
    return *physical_VELA_BA1_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_BA2_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( virtual_VELA_BA2_Screen_Controller_Obj )
    {
        std::cout << "virtual_VELA_BA2_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_VELA_BA2_Screen_Controller object" << std::endl;
        virtual_VELA_BA2_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withEPICS,VELA_BA2);
    }
    return *virtual_VELA_BA2_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_BA2_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( offline_VELA_BA2_Screen_Controller_Obj )
    {
        std::cout << "offline_VELA_BA2_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_BA2_Screen_Controller object" << std::endl;
        offline_VELA_BA2_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withoutEPICS,VELA_BA2);
    }
    return *offline_VELA_BA2_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_BA2_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( physical_VELA_BA2_Screen_Controller_Obj )
    {
        std::cout << "physical_VELA_BA2_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_BA2_Screen_Controller object" << std::endl;
        physical_VELA_BA2_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withEPICS,VELA_BA2);
    }
    return *physical_VELA_BA2_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_CLARA_INJ_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( virtual_CLARA_INJ_Screen_Controller_Obj )
    {
        std::cout << "virtual_CLARA_INJ_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_CLARA_INJ_Screen_Controller object" << std::endl;
        virtual_CLARA_INJ_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withEPICS,CLARA_INJ);
    }
    return *virtual_CLARA_INJ_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_CLARA_INJ_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( offline_CLARA_INJ_Screen_Controller_Obj )
    {
        std::cout << "offline_CLARA_INJ_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_CLARA_INJ_Screen_Controller object" << std::endl;
        offline_CLARA_INJ_Screen_Controller_Obj =
        virtual_CLARA_INJ_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withoutEPICS,CLARA_INJ);
    }
    return *offline_CLARA_INJ_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_CLARA_INJ_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::VELA_INJ_SCREENS_CONFIG;
    if( physical_CLARA_INJ_Screen_Controller_Obj )
    {
        std::cout << "physical_CLARA_INJ_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_CLARA_INJ_Screen_Controller object" << std::endl;
        physical_CLARA_INJ_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withEPICS,CLARA_INJ);
    }
    return *physical_CLARA_INJ_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_CLARA_PH1_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::CLARA_PH1_SCREENS_CONFIG;
    if( virtual_CLARA_PH1_Screen_Controller_Obj )
    {
        std::cout << "virtual_CLARA_PH1_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_CLARA_PH1_Screen_Controller object" << std::endl;
        virtual_CLARA_PH1_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withVM,withEPICS,CLARA_PH1);
    }
    return *virtual_CLARA_PH1_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_CLARA_PH1_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::CLARA_PH1_SCREENS_CONFIG;
    if( offline_CLARA_PH1_Screen_Controller_Obj )
    {
        std::cout << "offline_CLARA_PH1_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_CLARA_PH1_Screen_Controller object" << std::endl;
        offline_CLARA_PH1_Screen_Controller_Obj =
        virtual_CLARA_PH1_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withoutEPICS,CLARA_PH1);
    }
    return *offline_CLARA_PH1_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_CLARA_PH1_Screen_Controller()
{
    std::string configfile  = UTL::CONFIG_PATH + UTL::CLARA_PH1_SCREENS_CONFIG;
    if( physical_CLARA_PH1_Screen_Controller_Obj )
    {
        std::cout << "physical_CLARA_PH1_Screen_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_CLARA_PH1_Screen_Controller object" << std::endl;
        physical_CLARA_PH1_Screen_Controller_Obj =
            new screenController(shouldShowMessage,shouldShowDebugMessage,configfile,withoutVM,withEPICS,CLARA_PH1);
    }
    return *physical_CLARA_PH1_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::getScreenController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area )
{
    if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_INJ )
        return offline_VELA_INJ_Screen_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Screen_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_INJ )
        return physical_VELA_INJ_Screen_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA1 )
        return offline_VELA_BA1_Screen_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_Screen_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA1 )
        return physical_VELA_BA1_Screen_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA2 )
        return offline_VELA_BA2_Screen_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_Screen_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA2 )
        return physical_VELA_BA2_Screen_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_Screen_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_Screen_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_Screen_Controller();
}















