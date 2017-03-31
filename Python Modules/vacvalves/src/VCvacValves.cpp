#include "VCvacValves.h"
// stl

#include <iostream>

VCvacValves::VCvacValves():
virtual_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_INJ_Vac_Valve_Controller_Obj(nullptr),
virtual_VELA_BA1_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_BA1_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_BA1_Vac_Valve_Controller_Obj(nullptr),
virtual_VELA_BA2_Vac_Valve_Controller_Obj(nullptr),
offline_VELA_BA2_Vac_Valve_Controller_Obj(nullptr),
physical_VELA_BA2_Vac_Valve_Controller_Obj(nullptr),
virtual_CLARA_S01_Vac_Valve_Controller_Obj(nullptr),
offline_CLARA_S01_Vac_Valve_Controller_Obj(nullptr),
physical_CLARA_S01_Vac_Valve_Controller_Obj(nullptr),
virtual_CLARA_S02_Vac_Valve_Controller_Obj(nullptr),
offline_CLARA_S02_Vac_Valve_Controller_Obj(nullptr),
physical_CLARA_S02_Vac_Valve_Controller_Obj(nullptr),
virtual_C2V_Vac_Valve_Controller_Obj(nullptr),
offline_C2V_Vac_Valve_Controller_Obj(nullptr),
physical_C2V_Vac_Valve_Controller_Obj(nullptr),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
showDebugMessages(false),
showMessages(true),
VELA_INJ ( VELA_ENUM::MACHINE_AREA::VELA_INJ ),
VELA_BA1 ( VELA_ENUM::MACHINE_AREA::VELA_BA1 ),
VELA_BA2 ( VELA_ENUM::MACHINE_AREA::VELA_BA2 ),
CLARA_INJ( VELA_ENUM::MACHINE_AREA::CLARA_INJ),
CLARA_S01( VELA_ENUM::MACHINE_AREA::CLARA_S01),
CLARA_S02( VELA_ENUM::MACHINE_AREA::CLARA_S02),
CLARA_2_VELA( VELA_ENUM::MACHINE_AREA::CLARA_2_VELA ),
UNKNOWN_AREA( VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA )
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
    if(virtual_VELA_BA1_Vac_Valve_Controller_Obj)
    {
        delete virtual_VELA_BA1_Vac_Valve_Controller_Obj;
               virtual_VELA_BA1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_Vac_Valve_Controller_Obj)
    {
        delete offline_VELA_BA1_Vac_Valve_Controller_Obj;
               offline_VELA_BA1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_Vac_Valve_Controller_Obj)
    {
        delete physical_VELA_BA1_Vac_Valve_Controller_Obj;
               physical_VELA_BA1_Vac_Valve_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_Vac_Valve_Controller_Obj)
    {
        delete virtual_VELA_BA2_Vac_Valve_Controller_Obj;
               virtual_VELA_BA2_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_Vac_Valve_Controller_Obj)
    {
        delete offline_VELA_BA2_Vac_Valve_Controller_Obj;
               offline_VELA_BA2_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_Vac_Valve_Controller_Obj)
    {
        delete physical_VELA_BA2_Vac_Valve_Controller_Obj;
               physical_VELA_BA2_Vac_Valve_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_S01_Vac_Valve_Controller_Obj)
    {
        delete virtual_CLARA_S01_Vac_Valve_Controller_Obj;
               virtual_CLARA_S01_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_CLARA_S01_Vac_Valve_Controller_Obj)
    {
        delete offline_CLARA_S01_Vac_Valve_Controller_Obj;
               offline_CLARA_S01_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_CLARA_S01_Vac_Valve_Controller_Obj)
    {
        delete physical_CLARA_S01_Vac_Valve_Controller_Obj;
               physical_CLARA_S01_Vac_Valve_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_S02_Vac_Valve_Controller_Obj)
    {
        delete virtual_CLARA_S02_Vac_Valve_Controller_Obj;
               virtual_CLARA_S02_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_CLARA_S02_Vac_Valve_Controller_Obj)
    {
        delete offline_CLARA_S02_Vac_Valve_Controller_Obj;
               offline_CLARA_S02_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_CLARA_S02_Vac_Valve_Controller_Obj)
    {
        delete physical_CLARA_S02_Vac_Valve_Controller_Obj;
               physical_CLARA_S02_Vac_Valve_Controller_Obj = nullptr;
    }
    if(virtual_C2V_Vac_Valve_Controller_Obj)
    {
        delete virtual_C2V_Vac_Valve_Controller_Obj;
               virtual_C2V_Vac_Valve_Controller_Obj = nullptr;
    }
    if(offline_C2V_Vac_Valve_Controller_Obj)
    {
        delete offline_C2V_Vac_Valve_Controller_Obj;
               offline_C2V_Vac_Valve_Controller_Obj = nullptr;
    }
    if(physical_C2V_Vac_Valve_Controller_Obj)
    {
        delete physical_C2V_Vac_Valve_Controller_Obj;
               physical_C2V_Vac_Valve_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
void VCvacValves::setQuiet()
{
    std::cout << "VCvacValves Quiet Mode Set." << std::endl;
    showDebugMessages = false;
    showMessages = false;
}
//______________________________________________________________________________
void VCvacValves::setVerbose()
{
    std::cout << "VCvacValves Verbose Mode Set." << std::endl;
    showDebugMessages = true;
    showMessages = true;
}
//______________________________________________________________________________
void VCvacValves::setMessage()
{
    std::cout << "VCvacValves Message Mode Set." << std::endl;
    showDebugMessages = false;
    showMessages = true;
}
//______________________________________________________________________________
void VCvacValves::setDebugMessage()
{
    std::cout << "VCvacValves DebugMessage Mode Set." << std::endl;
    showDebugMessages = true;
    showMessages = false;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_VELA_INJ_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;

    if( virtual_VELA_INJ_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_INJ_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withVM, VELA_INJ );
    }
    return *virtual_VELA_INJ_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_VELA_INJ_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;

    if( offline_VELA_INJ_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_INJ_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_INJ );
    }
    return *offline_VELA_INJ_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_VELA_INJ_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_INJ_VALVE_CONFIG;

    if( physical_VELA_INJ_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_INJ_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_INJ );
    }
    return *physical_VELA_INJ_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_VELA_BA1_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;

    if( virtual_VELA_BA1_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA1_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA1 );
    }
    return *virtual_VELA_BA1_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_VELA_BA1_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;

    if( offline_VELA_BA1_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA1_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA1 );
    }
    return *offline_VELA_BA1_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_VELA_BA1_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_BA1_VALVE_CONFIG;

    if( physical_VELA_BA1_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA1_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA1 );
    }
    return *physical_VELA_BA1_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_VELA_BA2_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;

    if( virtual_VELA_BA2_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA2_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA2 );
    }
    return *virtual_VELA_BA2_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_VELA_BA2_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;

    if( offline_VELA_BA2_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA2_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA2 );
    }
    return *offline_VELA_BA2_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_VELA_BA2_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::VELA_BA2_VALVE_CONFIG;

    if( physical_VELA_BA2_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA2_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA2 );
    }
    return *physical_VELA_BA2_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_CLARA_S01_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_S01_VALVE_CONFIG;

    if( virtual_CLARA_S01_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_S01_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S01 );
    }
    return *virtual_CLARA_S01_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_CLARA_S01_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_S01_VALVE_CONFIG;

    if( offline_CLARA_S01_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_S01_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S01 );
    }
    return *offline_CLARA_S01_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_CLARA_S01_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_S01_VALVE_CONFIG;

    if( physical_CLARA_S01_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_S01_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S01 );
    }
    return *physical_CLARA_S01_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_CLARA_S02_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_S02_VALVE_CONFIG;

    if( virtual_CLARA_S02_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_S02_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S02 );
    }
    return *virtual_CLARA_S02_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_CLARA_S02_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_S02_VALVE_CONFIG;

    if( offline_CLARA_S02_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_S02_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S02 );
    }
    return *offline_CLARA_S02_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_CLARA_S02_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_S02_VALVE_CONFIG;

    if( physical_CLARA_S02_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_S02_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S02 );
    }
    return *physical_CLARA_S02_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::virtual_C2V_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_VALVE_CONFIG;

    if( virtual_C2V_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        virtual_C2V_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_2_VELA );
    }
    return *virtual_C2V_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::offline_C2V_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_VALVE_CONFIG;

    if( offline_C2V_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        offline_C2V_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_2_VELA );
    }
    return *offline_C2V_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::physical_C2V_Vac_Valve_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string vacvalveconf1 = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_VALVE_CONFIG;

    if( physical_C2V_Vac_Valve_Controller_Obj )
    {

    }
    else
    {
        physical_C2V_Vac_Valve_Controller_Obj = new vacuumValveController( vacvalveconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_2_VELA );
    }
    return *physical_C2V_Vac_Valve_Controller_Obj;
}
//______________________________________________________________________________
vacuumValveController & VCvacValves::getVacValveController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area )
{
    if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_INJ )
        return offline_VELA_INJ_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_INJ )
        return physical_VELA_INJ_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA1 )
        return offline_VELA_BA1_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA1 )
        return physical_VELA_BA1_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA2 )
        return offline_VELA_BA2_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA2 )
        return physical_VELA_BA2_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S01 )
        return offline_CLARA_S01_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S01 )
        return virtual_CLARA_S01_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S01 )
        return physical_CLARA_S01_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S02 )
        return offline_CLARA_S02_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S02 )
        return virtual_CLARA_S02_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S02 )
        return physical_CLARA_S02_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_2_VELA )
        return offline_C2V_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_2_VELA )
        return virtual_C2V_Vac_Valve_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_2_VELA )
        return physical_C2V_Vac_Valve_Controller();

}
//______________________________________________________________________________
#ifdef BUILD_DLL
#endif //BUILD_DLL



