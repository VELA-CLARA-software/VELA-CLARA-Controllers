#include "VCgun_RF.h"
#include <iostream>

VCgun_RF::VCgun_RF()
{
   std::cout << "Instantiated a VCgun_RF " << std::endl;
    //ctor
}
//______________________________________________________________________________
VCgun_RF::~VCgun_RF()
{
    //dtor
//    if(virtual_VELA_Screen_Controller_Obj)
//    {
//        delete virtual_VELA_Screen_Controller_Obj;
//               virtual_VELA_Screen_Controller_Obj = nullptr;
//    }
//    if(physical_VELA_Screen_Controller_Obj)
//    {
//        delete physical_VELA_Screen_Controller_Obj;
//               physical_VELA_Screen_Controller_Obj = nullptr;
//    }
//    if(offline_VELA_Screen_Controller_Obj)
//    {
//        delete offline_VELA_Screen_Controller_Obj;
//               offline_VELA_Screen_Controller_Obj = nullptr;
//    }
//    if(virtual_CLARA_Screen_Controller_Obj)
//    {
//        delete virtual_CLARA_Screen_Controller_Obj;
//               virtual_CLARA_Screen_Controller_Obj = nullptr;
//    }
//    if(offline_CLARA_Screen_Controller_Obj)
//    {
//        delete offline_CLARA_Screen_Controller_Obj;
//               offline_CLARA_Screen_Controller_Obj = nullptr;
//    }
//    if(physical_CLARA_Screen_Controller_Obj)
//    {
//        delete physical_CLARA_Screen_Controller_Obj;
//               physical_CLARA_Screen_Controller_Obj = nullptr;
//    }
//    if(virtual_VELA_CLARA_Screen_Controller_Obj)
//    {
//        delete virtual_VELA_CLARA_Screen_Controller_Obj;
//               virtual_VELA_CLARA_Screen_Controller_Obj = nullptr;
//    }
//    if(physical_VELA_CLARA_Screen_Controller_Obj)
//    {
//        delete physical_VELA_CLARA_Screen_Controller_Obj;
//               physical_VELA_CLARA_Screen_Controller_Obj = nullptr;
//    }
//    if(offline_VELA_CLARA_Screen_Controller_Obj)
//    {
//        delete offline_VELA_CLARA_Screen_Controller_Obj;
//               offline_VELA_CLARA_Screen_Controller_Obj = nullptr;
//    }
}
//______________________________________________________________________________
//RFGunController& VCgun_RF::virtual_VELA_RFGUN_Controller()
////void VCgun_RF::virtual_VELA_INJ_Magnet_Controller()
//{
//    std::cout << "creating virtual_VELA_Screen_Controller object" << std::endl;
//    std::string simpleConf  = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_SIMPLE_SCREENS_CONFIG;
//    std::string complexConf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_COMPLEX_SCREENS_CONFIG;
//
//    if( virtual_VELA_Screen_Controller_Obj )
//    {
//
//    }
//    else
//    {
//        virtual_VELA_Screen_Controller_Obj = new screenController(   true, true,complexConf, simpleConf, true );
//    }
//    return *virtual_VELA_Screen_Controller_Obj;
//}
//______________________________________________________________________________
















