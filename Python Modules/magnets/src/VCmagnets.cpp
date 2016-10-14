#include "VCmagnets.h"
#include <iostream>

VCmagnets::VCmagnets():
virtual_VELA_INJ_Magnet_Controller_Obj_Exists(false),
virtual_VELA_INJ_Magnet_Controller_Obj(nullptr),
virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj_Exists(false),
virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj(nullptr)
{
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
    if(virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj)
    {
        delete virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj;
               virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj = nullptr;
    }
}
//______________________________________________________________________________
magnetController& VCmagnets::virtual_VELA_INJ_Magnet_Controller()
//void VCmagnets::virtual_VELA_INJ_Magnet_Controller()
{
    std::cout << "creating object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;

    if( virtual_VELA_INJ_Magnet_Controller_Obj_Exists )
    {

    }
    else
    {
        virtual_VELA_INJ_Magnet_Controller_Obj = new magnetController( true, true, mconf, nconf, true );
    }
    return *virtual_VELA_INJ_Magnet_Controller_Obj;
}
//______________________________________________________________________________
//magnetController VCmagnets::virtual_VELA_INJ_Magnet_Controller_NOEPICS()
magnetController& VCmagnets::virtual_VELA_INJ_Magnet_Controller_NOEPICS()
{
    std::cout << "creating object" << std::endl;
    std::string mconf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_MAG_CONFIG;
    std::string nconf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_MAG_NR_PSU_CONFIG;
    if( virtual_VELA_INJ_Magnet_Controller_Obj_Exists )
    {

    }
    else
    {
        virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj = new magnetController( true, true, mconf, nconf, false );
    }
    return *virtual_VELA_INJ_Magnet_Controller_NOEPICS_Obj;
}
//______________________________________________________________________________
