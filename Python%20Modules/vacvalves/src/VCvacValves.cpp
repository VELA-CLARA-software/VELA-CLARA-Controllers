#include "VCvacValves.h"
// stl

#include <iostream>

VCvacValves::VCvacValves():
virtual_VELA_INJ_Vac_Valve_Controller_Obj_Exists(false),
virtual_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_INJ_Vac_Valve_Controller_Obj_Exists(false),
offline_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_INJ_Vac_Valve_Controller_Obj_Exists(false),
physical_VELA_INJ_Vac_Valve_Controller_Obj(nullptr)
{
    //ctor
}
//______________________________________________________________________________
VCvacValves::~VCvacValves()
{
    if(virtual_VELA_INJ_Vac_Valve_Controller_Obj)
    {
        delete virtual_VELA_INJ_Vac_Valve_Controller_Obj;
               virtual_VELA_INJ_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Vac_Valve_Controller_Obj)
    {
        delete offline_VELA_INJ_Vac_Valve_Controller_Obj;
               offline_VELA_INJ_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Vac_Valve_Controller_Obj)
    {
        delete physical_VELA_INJ_Vac_Valve_Controller_Obj;
               physical_VELA_INJ_Vac_Valve_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_VELA_INJ_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_VALVE_CONFIG;

    if( virtual_VELA_INJ_Vac_Valve_Controller_Obj_Exists )
    {

    }
    else
    {
        virtual_VELA_INJ_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, true, true, true );
    }
    return *virtual_VELA_INJ_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_VELA_INJ_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_VALVE_CONFIG;

    if( offline_VELA_INJ_Vac_Valve_Controller_Obj_Exists )
    {

    }
    else
    {
        offline_VELA_INJ_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, true, true, true );
    }
    return *offline_VELA_INJ_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_VELA_INJ_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_VALVE_CONFIG;

    if( physical_VELA_INJ_Vac_Valve_Controller_Obj_Exists )
    {

    }
    else
    {
        physical_VELA_INJ_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, true, true, true );
    }
    return *physical_VELA_INJ_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
#ifdef BUILD_DLL
#endif //BUILD_DLL



