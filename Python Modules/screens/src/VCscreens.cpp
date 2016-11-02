#include "VCscreens.h"
#include <iostream>

VCscreens::VCscreens():
virtual_VELA_Screen_Controller_Obj(nullptr),
physical_VELA_Screen_Controller_Obj(nullptr),
offline_VELA_Screen_Controller_Obj(nullptr),

virtual_CLARA_Screen_Controller_Obj(nullptr),
physical_CLARA_Screen_Controller_Obj(nullptr),
offline_CLARA_Screen_Controller_Obj(nullptr),

virtual_VELA_CLARA_Screen_Controller_Obj(nullptr),
physical_VELA_CLARA_Screen_Controller_Obj(nullptr),
offline_VELA_CLARA_Screen_Controller_Obj(nullptr)
{
   std::cout << "Instantiated a VCscreens " << std::endl;
    //ctor
}
//______________________________________________________________________________
VCscreens::~VCscreens()
{
    //dtor
    if(virtual_VELA_Screen_Controller_Obj)
    {
        delete virtual_VELA_Screen_Controller_Obj;
               virtual_VELA_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_Screen_Controller_Obj)
    {
        delete physical_VELA_Screen_Controller_Obj;
               physical_VELA_Screen_Controller_Obj = nullptr;
    }
    if(offline_VELA_Screen_Controller_Obj)
    {
        delete offline_VELA_Screen_Controller_Obj;
               offline_VELA_Screen_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_Screen_Controller_Obj)
    {
        delete virtual_CLARA_Screen_Controller_Obj;
               virtual_CLARA_Screen_Controller_Obj = nullptr;
    }
    if(offline_CLARA_Screen_Controller_Obj)
    {
        delete offline_CLARA_Screen_Controller_Obj;
               offline_CLARA_Screen_Controller_Obj = nullptr;
    }
    if(physical_CLARA_Screen_Controller_Obj)
    {
        delete physical_CLARA_Screen_Controller_Obj;
               physical_CLARA_Screen_Controller_Obj = nullptr;
    }
    if(virtual_VELA_CLARA_Screen_Controller_Obj)
    {
        delete virtual_VELA_CLARA_Screen_Controller_Obj;
               virtual_VELA_CLARA_Screen_Controller_Obj = nullptr;
    }
    if(physical_VELA_CLARA_Screen_Controller_Obj)
    {
        delete physical_VELA_CLARA_Screen_Controller_Obj;
               physical_VELA_CLARA_Screen_Controller_Obj = nullptr;
    }
    if(offline_VELA_CLARA_Screen_Controller_Obj)
    {
        delete offline_VELA_CLARA_Screen_Controller_Obj;
               offline_VELA_CLARA_Screen_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_Screen_Controller()
//void VCscreens::virtual_VELA_INJ_Magnet_Controller()
{
    std::cout << "creating virtual_VELA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;

    if( virtual_VELA_Screen_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, true );
    }
    return *virtual_VELA_Screen_Controller_Obj;
}
//______________________________________________________________________________
//screenController VCscreens::offline_VELA_INJ_Magnet_Controller()
screenController& VCscreens::physical_VELA_Screen_Controller()
{
    std::cout << "creating offline_VELA_INJ_Magnet_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
    if( physical_VELA_Screen_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, true );
    }
    return *physical_VELA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_Screen_Controller()
{
    std::cout << "creating offline_VELA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
    if( offline_VELA_Screen_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, false );
    }
    return *offline_VELA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_CLARA_Screen_Controller()
{
    std::cout << "creating virtual_CLARA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;

    if( virtual_CLARA_Screen_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, true );
    }
    return *virtual_CLARA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_CLARA_Screen_Controller()
{
    std::cout << "creating physical_CLARA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
    if( physical_CLARA_Screen_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_Screen_Controller_Obj = new screenController( true, true,complexConf, simpleConf, true );
    }
    return *physical_CLARA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_CLARA_Screen_Controller()
{
    std::cout << "creating offline_CLARA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
    if( offline_CLARA_Screen_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, false );
    }
    return *offline_CLARA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::virtual_VELA_CLARA_Screen_Controller()
{
    std::cout << "creating virtual_VELA_CLARA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;

    if( virtual_VELA_CLARA_Screen_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_CLARA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, true );
    }
    return *virtual_VELA_CLARA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::physical_VELA_CLARA_Screen_Controller()
{
    std::cout << "creating physical_VELA_CLARA_Screen_Controller object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
    if( physical_VELA_CLARA_Screen_Controller_Obj )
    {
    }
    else
    {
        physical_VELA_CLARA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, true );
    }
    return *physical_VELA_CLARA_Screen_Controller_Obj;
}
//______________________________________________________________________________
screenController& VCscreens::offline_VELA_CLARA_Screen_Controllerj()
{
    std::cout << "creating offline_VELA_CLARA_Screen_Controllerj object" << std::endl;
    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
    if( offline_VELA_CLARA_Screen_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_CLARA_Screen_Controller_Obj = new screenController(  true, true,complexConf, simpleConf, false );
    }
    return *offline_VELA_CLARA_Screen_Controller_Obj;
}
//______________________________________________________________________________


















