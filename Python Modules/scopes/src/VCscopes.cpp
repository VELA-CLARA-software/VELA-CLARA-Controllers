#include "VCscopes.h"
// stl
#include <iostream>

VCscopes::VCscopes():
virtual_VELA_INJ_Scope_Controller_Obj(nullptr),
offline_VELA_INJ_Scope_Controller_Obj(nullptr),
physical_VELA_INJ_Scope_Controller_Obj(nullptr),
virtual_VELA_BA1_Scope_Controller_Obj(nullptr),
offline_VELA_BA1_Scope_Controller_Obj(nullptr),
physical_VELA_BA1_Scope_Controller_Obj(nullptr),
virtual_VELA_BA2_Scope_Controller_Obj(nullptr),
offline_VELA_BA2_Scope_Controller_Obj(nullptr),
physical_VELA_BA2_Scope_Controller_Obj(nullptr),
virtual_CLARA_S01_Scope_Controller_Obj(nullptr),
offline_CLARA_S01_Scope_Controller_Obj(nullptr),
physical_CLARA_S01_Scope_Controller_Obj(nullptr),
virtual_CLARA_PH1_Scope_Controller_Obj(nullptr),
offline_CLARA_PH1_Scope_Controller_Obj(nullptr),
physical_CLARA_PH1_Scope_Controller_Obj(nullptr),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
showDebugMessages(false),
showMessages(true),
VELA_INJ ( VELA_ENUM::MACHINE_AREA::VELA_INJ  ),
VELA_BA1 ( VELA_ENUM::MACHINE_AREA::VELA_BA1  ),
VELA_BA2 ( VELA_ENUM::MACHINE_AREA::VELA_BA2  ),
CLARA_S01( VELA_ENUM::MACHINE_AREA::CLARA_S01 ),
CLARA_PH1( VELA_ENUM::MACHINE_AREA::CLARA_PH1 ),
CLARA_2_VELA( VELA_ENUM::MACHINE_AREA::CLARA_2_VELA ),
UNKNOWN_AREA( VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA )
{
    //ctor
}
//______________________________________________________________________________
VCscopes::~VCscopes()
{
//    if(virtual_VELA_INJ_Scope_Controller_Obj)
//    {
//        delete virtual_VELA_INJ_Scope_Controller_Obj;
//               virtual_VELA_INJ_Scope_Controller_Obj = nullptr;
//    }
//    if(offline_VELA_INJ_Scope_Controller_Obj)
//    {
//        delete offline_VELA_INJ_Scope_Controller_Obj;
//               offline_VELA_INJ_Scope_Controller_Obj = nullptr;
//    }
//    if(physical_VELA_INJ_Scope_Controller_Obj)
//    {
//        delete physical_VELA_INJ_Scope_Controller_Obj;
//               physical_VELA_INJ_Scope_Controller_Obj = nullptr;
//    }
//    if(virtual_VELA_BA1_Scope_Controller_Obj)
//    {
//        delete virtual_VELA_BA1_Scope_Controller_Obj;
//               virtual_VELA_BA1_Scope_Controller_Obj = nullptr;
//    }
//    if(offline_VELA_BA1_Scope_Controller_Obj)
//    {
//        delete offline_VELA_BA1_Scope_Controller_Obj;
//               offline_VELA_BA1_Scope_Controller_Obj = nullptr;
//    }
//    if(physical_VELA_BA1_Scope_Controller_Obj)
//    {
//        delete physical_VELA_BA1_Scope_Controller_Obj;
//               physical_VELA_BA1_Scope_Controller_Obj = nullptr;
//    }
//    if(virtual_VELA_BA2_Scope_Controller_Obj)
//    {
//        delete virtual_VELA_BA2_Scope_Controller_Obj;
//               virtual_VELA_BA2_Scope_Controller_Obj = nullptr;
//    }
//    if(offline_VELA_BA2_Scope_Controller_Obj)
//    {
//        delete offline_VELA_BA2_Scope_Controller_Obj;
//               offline_VELA_BA2_Scope_Controller_Obj = nullptr;
//    }
//    if(physical_VELA_BA2_Scope_Controller_Obj)
//    {
//        delete physical_VELA_BA2_Scope_Controller_Obj;
//               physical_VELA_BA2_Scope_Controller_Obj = nullptr;
//    }
//    if(virtual_CLARA_S01_Scope_Controller_Obj)
//    {
//        delete virtual_CLARA_S01_Scope_Controller_Obj;
//               virtual_CLARA_S01_Scope_Controller_Obj = nullptr;
//    }
//    if(offline_CLARA_S01_Scope_Controller_Obj)
//    {
//        delete offline_CLARA_S01_Scope_Controller_Obj;
//               offline_CLARA_S01_Scope_Controller_Obj = nullptr;
//    }
//    if(physical_CLARA_S01_Scope_Controller_Obj)
//    {
//        delete physical_CLARA_S01_Scope_Controller_Obj;
//               physical_CLARA_S01_Scope_Controller_Obj = nullptr;
//    }
}    //dtor
//______________________________________________________________________________
scopeController & VCscopes::virtual_VELA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( virtual_VELA_INJ_Scope_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withVM, VELA_INJ );
    }
    return *virtual_VELA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( offline_VELA_INJ_Scope_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_INJ );
    }
    return *offline_VELA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( physical_VELA_INJ_Scope_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_INJ );
    }
    return *physical_VELA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_VELA_BA1_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( virtual_VELA_BA1_Scope_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA1_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA1 );
    }
    return *virtual_VELA_BA1_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_BA1_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( offline_VELA_BA1_Scope_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA1_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA1 );
    }
    return *offline_VELA_BA1_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_BA1_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( physical_VELA_BA1_Scope_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA1_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA1 );
    }
    return *physical_VELA_BA1_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_VELA_BA2_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( virtual_VELA_BA2_Scope_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA2_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA2 );
    }
    return *virtual_VELA_BA2_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_BA2_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( offline_VELA_BA2_Scope_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA2_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA2 );
    }
    return *offline_VELA_BA2_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_BA2_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( physical_VELA_BA2_Scope_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA2_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA2 );
    }
    return *physical_VELA_BA2_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_CLARA_S01_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( virtual_CLARA_S01_Scope_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_S01_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S01 );
    }
    return *virtual_CLARA_S01_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_CLARA_S01_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( offline_CLARA_S01_Scope_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_S01_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S01 );
    }
    return *offline_CLARA_S01_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_CLARA_S01_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( physical_CLARA_S01_Scope_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_S01_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S01 );
    }
    return *physical_CLARA_S01_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::virtual_CLARA_PH1_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( virtual_CLARA_PH1_Scope_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_PH1_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withVM, CLARA_PH1 );
    }
    return *virtual_CLARA_PH1_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_CLARA_PH1_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( offline_CLARA_PH1_Scope_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_PH1_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_PH1 );
    }
    return *offline_CLARA_PH1_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_CLARA_PH1_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( physical_CLARA_PH1_Scope_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_PH1_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_PH1 );
    }
    return *physical_CLARA_PH1_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::getScopeController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area )
{

    if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_INJ )
        return offline_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_INJ )
        return virtual_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_INJ )
        return physical_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA1 )
        return offline_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA1 )
        return virtual_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA1 )
        return physical_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA2 )
        return offline_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA2 )
        return virtual_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA2 )
        return physical_VELA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S01 )
        return offline_CLARA_S01_Scope_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S01 )
        return virtual_CLARA_S01_Scope_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S01 )
        return physical_CLARA_S01_Scope_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_Scope_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_Scope_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_Scope_Controller();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
#endif //BUILD_DLL



