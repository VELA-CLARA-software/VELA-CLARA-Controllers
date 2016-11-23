#include "VCmagnets.h"
#include <iostream>

VCmagnets::VCmagnets():
virtual_VELA_INJ_Magnet_Controller_Obj(nullptr),
virtual_VELA_BA1_Magnet_Controller_Obj(nullptr),
virtual_VELA_BA2_Magnet_Controller_Obj(nullptr),
virtual_CLARA_INJ_Magnet_Controller_Obj(nullptr),
physical_VELA_INJ_Magnet_Controller_Obj(nullptr),
physical_VELA_BA1_Magnet_Controller_Obj(nullptr),
physical_VELA_BA2_Magnet_Controller_Obj(nullptr),
physical_CLARA_INJ_Magnet_Controller_Obj(nullptr),
offline_VELA_INJ_Magnet_Controller_Obj(nullptr),
offline_VELA_BA1_Magnet_Controller_Obj(nullptr),
offline_VELA_BA2_Magnet_Controller_Obj(nullptr),
offline_CLARA_INJ_Magnet_Controller_Obj(nullptr),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
showdebugMessage(true),
dontShowMessage(false),
showDebugMessage(true),
dontShowDebugMessage(false)
{
   std::cout << "Instantiated a VCmagnets " << std::endl;
    //ctor

}
//______________________________________________________________________________
VCmagnets::~VCmagnets()
{
    //dtor
    if(virtual_VELA_INJ_Magnet_Controller_Obj)
    {
        delete virtual_VELA_INJ_Magnet_Controller_Obj;
               virtual_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Magnet_Controller_Obj)
    {
        delete offline_VELA_INJ_Magnet_Controller_Obj;
               offline_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Magnet_Controller_Obj)
    {
        delete physical_VELA_INJ_Magnet_Controller_Obj;
               physical_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }


    if(virtual_VELA_BA1_Magnet_Controller_Obj)
    {
        delete virtual_VELA_BA1_Magnet_Controller_Obj;
               virtual_VELA_BA1_Magnet_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Magnet_Controller_Obj)
    {
        delete offline_VELA_BA2_Magnet_Controller_Obj;
               offline_VELA_BA2_Magnet_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Magnet_Controller_Obj)
    {
        delete physical_VELA_BA2_Magnet_Controller_Obj;
               physical_VELA_BA2_Magnet_Controller_Obj = nullptr;
    }

    if(virtual_CLARA_INJ_Magnet_Controller_Obj)
    {
        delete virtual_CLARA_INJ_Magnet_Controller_Obj;
               virtual_CLARA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(offline_CLARA_INJ_Magnet_Controller_Obj)
    {
        delete offline_CLARA_INJ_Magnet_Controller_Obj;
               offline_CLARA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(physical_CLARA_INJ_Magnet_Controller_Obj)
    {
        delete physical_CLARA_INJ_Magnet_Controller_Obj;
               physical_CLARA_INJ_Magnet_Controller_Obj = nullptr;
    }

        if(virtual_VELA_INJ_Magnet_Controller_Obj)
    {
        delete virtual_VELA_INJ_Magnet_Controller_Obj;
               virtual_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Magnet_Controller_Obj)
    {
        delete offline_VELA_INJ_Magnet_Controller_Obj;
               offline_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Magnet_Controller_Obj)
    {
        delete physical_VELA_INJ_Magnet_Controller_Obj;
               physical_VELA_INJ_Magnet_Controller_Obj = nullptr;
    }

}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_INJ_Magnet_Controller()
//void VCmagnets::virtual_VELA_INJ_Magnet_Controller()
{
    std::cout << "creating virtual_VELA_INJ_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;

    if( virtual_VELA_INJ_Magnet_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_INJ_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withVM, withEPICS );
    }
    return *virtual_VELA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
//magnetController VCmagnets::offline_VELA_INJ_Magnet_Controller()
magnetController& VCmagnets::physical_VELA_INJ_Magnet_Controller()
{
    std::cout << "creating physical_VELA_INJ_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;
    if( physical_VELA_INJ_Magnet_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_INJ_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withoutVM, withEPICS );
    }
    return *physical_VELA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_VELA_INJ_Magnet_Controller()
{
    std::cout << "creating offline_VELA_INJ_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;
    if( offline_VELA_INJ_Magnet_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_INJ_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withVM, withoutEPICS );
    }
    return *offline_VELA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_BA1_Magnet_Controller()
{
    std::cout << "creating virtual_VELA_BA1_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_BA1_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_BA1_MAG_NR_PSU_CONFIG;

    if( virtual_VELA_BA1_Magnet_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA1_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withVM, withEPICS );
    }
    return *virtual_VELA_BA1_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_VELA_BA1_Magnet_Controller()
{
    std::cout << "creating offline_VELA_BA1_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_BA1_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_BA1_MAG_NR_PSU_CONFIG;
    if( offline_VELA_BA1_Magnet_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA1_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withVM, withoutEPICS );
    }
    return *offline_VELA_BA1_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_VELA_BA1_Magnet_Controller()
{
    std::cout << "creating physical_VELA_BA1_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;
    if( physical_VELA_BA1_Magnet_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA1_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withoutVM, withEPICS );
    }
    return *physical_VELA_BA1_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_BA2_Magnet_Controller()
{
    std::cout << "creating virtual_VELA_BA2_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_BA2_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_BA2_MAG_NR_PSU_CONFIG;

    if( virtual_VELA_BA2_Magnet_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA2_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withVM, withEPICS );
    }
    return *virtual_VELA_BA2_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_VELA_BA2_Magnet_Controller()
{
    std::cout << "creating offline_VELA_BA2_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_BA2_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_BA2_MAG_NR_PSU_CONFIG;
    if( offline_VELA_BA2_Magnet_Controller_Obj )
    {
    }
    else
    {
        offline_VELA_BA2_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withVM, withoutEPICS );
    }
    return *offline_VELA_BA2_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_VELA_BA2_Magnet_Controller()
{
    std::cout << "creating physical_VELA_BA2_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;
    if( physical_VELA_BA1_Magnet_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA1_Magnet_Controller_Obj = new magnetController( showdebugMessage, showDebugMessage, mconf, nconf, withoutVM, withEPICS );
    }
    return *physical_VELA_BA1_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_CLARA_INJ_Magnet_Controller()
{
    std::cout << "creating virtual_CLARA_INJ_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_MAG_NR_PSU_CONFIG;

    if( virtual_CLARA_INJ_Magnet_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_INJ_Magnet_Controller_Obj = new magnetController( true, true, mconf, nconf, withVM, withEPICS );
    }
    return *virtual_CLARA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::offline_CLARA_INJ_Magnet_Controller()
{
    std::cout << "creating offline_CLARA_INJ_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_MAG_NR_PSU_CONFIG;
    if( offline_CLARA_INJ_Magnet_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_INJ_Magnet_Controller_Obj = new magnetController( true, true, mconf, nconf, withVM, withoutEPICS );
    }
    return *offline_CLARA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::physical_CLARA_INJ_Magnet_Controller()
{
    std::cout << "creating physical_CLARA_INJ_Magnet_Controller object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_MAG_NR_PSU_CONFIG;
    if( physical_CLARA_INJ_Magnet_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_INJ_Magnet_Controller_Obj = new magnetController( true, true, mconf, nconf, withoutVM, withEPICS );
    }
    return *physical_CLARA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
magnetController& VCmagnets::getMagnetController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area )
{
    if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_INJ )
        return offline_VELA_INJ_Magnet_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Magnet_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_INJ )
        return physical_VELA_INJ_Magnet_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA1 )
        return offline_VELA_BA1_Magnet_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_Magnet_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA1 )
        return physical_VELA_BA1_Magnet_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA2 )
        return offline_VELA_BA2_Magnet_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_Magnet_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA2 )
        return physical_VELA_BA2_Magnet_Controller();

}

















