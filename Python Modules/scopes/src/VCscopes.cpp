#include "VCscopes.h"
// stl
#include <iostream>

VCscopes::VCscopes():
virtual_VELA_INJ_Scope_Controller_Obj(nullptr),
offline_VELA_INJ_Scope_Controller_Obj(nullptr),
physical_VELA_INJ_Scope_Controller_Obj(nullptr),
virtual_CLARA_INJ_Scope_Controller_Obj(nullptr),
offline_CLARA_INJ_Scope_Controller_Obj(nullptr),
physical_CLARA_INJ_Scope_Controller_Obj(nullptr),
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
CLARA_2_VELA( VELA_ENUM::MACHINE_AREA::CLARA_2_VELA ),
UNKNOWN_AREA( VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA )
{
    //ctor
}
//______________________________________________________________________________
VCscopes::~VCscopes()
{
    if(virtual_VELA_INJ_Scope_Controller_Obj)
    {
        delete virtual_VELA_INJ_Scope_Controller_Obj;
               virtual_VELA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_Scope_Controller_Obj)
    {
        delete offline_VELA_INJ_Scope_Controller_Obj;
               offline_VELA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_Scope_Controller_Obj)
    {
        delete physical_VELA_INJ_Scope_Controller_Obj;
               physical_VELA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_INJ_Scope_Controller_Obj)
    {
        delete virtual_CLARA_INJ_Scope_Controller_Obj;
               virtual_CLARA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(offline_CLARA_INJ_Scope_Controller_Obj)
    {
        delete offline_CLARA_INJ_Scope_Controller_Obj;
               offline_CLARA_INJ_Scope_Controller_Obj = nullptr;
    }
    if(physical_CLARA_INJ_Scope_Controller_Obj)
    {
        delete physical_CLARA_INJ_Scope_Controller_Obj;
               physical_CLARA_INJ_Scope_Controller_Obj = nullptr;
    }
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
scopeController & VCscopes::virtual_CLARA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( virtual_CLARA_INJ_Scope_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withVM, CLARA_INJ );
    }
    return *virtual_CLARA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_CLARA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( offline_CLARA_INJ_Scope_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_INJ );
    }
    return *offline_CLARA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_CLARA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::CLARA_SCOPE_TRACE_CONFIG;

    if( physical_CLARA_INJ_Scope_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_INJ );
    }
    return *physical_CLARA_INJ_Scope_Controller_Obj;
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
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_INJ )
        return offline_CLARA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_INJ )
        return virtual_CLARA_INJ_Scope_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_INJ )
        return physical_CLARA_INJ_Scope_Controller();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
//boost::python::dict velaChargeScopeController::getScopeStateDefinition()
//{
//    std::map< scopeStructs, std::vector< double > > m;
//
//    m[ VELA_ENUM::TRIG_STATE::NOTRIG ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::NOTRIG  ); // ENUM_TO_STRING MACRO in structs.h
//    m[ VELA_ENUM::TRIG_STATE::TRIG   ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG    );
//    m[ VELA_ENUM::TRIG_STATE::TRIG_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG_ERROR   );
//    return enumStringMapToPythonDict( m );
//}
////______________________________________________________________________________
//void velaChargeScopeController::monitorTracesForNShots_Py( size_t N )
//{
//    monitorTracesForNShots( N );
//}
////______________________________________________________________________________
//void velaChargeScopeController::monitorNumsForNShots_Py( size_t N )
//{
//    monitorNumsForNShots( N );
//}
////______________________________________________________________________________
//std::vector< std::vector< double > > velaChargeScopeController::getScopeTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
//{
//    return getScopeTraces( name, pvType );
//}
////______________________________________________________________________________
//std::vector< double > velaChargeScopeController::getMinOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
//{
//    return getMinOfTraces( name, pvType );
//}
////______________________________________________________________________________
//std::vector< double > velaChargeScopeController::getMaxOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
//{
//    return getMaxOfTraces( name, pvType );
//}
////______________________________________________________________________________
//std::vector< double > velaChargeScopeController::getAreaUnderTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
//{
//    return getAreaUnderTraces( name, pvType );
//}
////______________________________________________________________________________
//std::vector< double > velaChargeScopeController::getTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
//{
//    return getTimeStamps( name, pvType );
//}
////______________________________________________________________________________
//std::vector< std::string > velaChargeScopeController::getStrTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
//{
//    return getStrTimeStamps( name, pvType );
//}
////______________________________________________________________________________
//double velaChargeScopeController::getScopeP1_Py( const std::string & name )
//{
//    return getScopeP1( name );
//}
////______________________________________________________________________________
//double velaChargeScopeController::getScopeP2_Py( const std::string & name )
//{
//    return getScopeP2( name );
//}
////______________________________________________________________________________
//double velaChargeScopeController::getWCMQ_Py()
//{
//    return getWCMQ();
//}
////______________________________________________________________________________
//double velaChargeScopeController::getICT1Q_Py()
//{
//    return getICT1Q();
//}
////______________________________________________________________________________
//double velaChargeScopeController::getICT2Q_Py()
//{
//    return getICT2Q();
//}
////______________________________________________________________________________
//double velaChargeScopeController::getFCUPQ_Py()
//{
//    return getFCUPQ();
//}
////______________________________________________________________________________
//double velaChargeScopeController::getEDFCUPQ_Py()
//{
//    return getEDFCUPQ();
//}
////______________________________________________________________________________
//std::vector< std::string > velaChargeScopeController::getScopeNames_Py()
//{
//    return getScopeNames();
//}
//////______________________________________________________________________________
////double velaChargeScopeController::get_CA_PEND_IO_TIMEOUT()
////{
////    return localInterface.get_CA_PEND_IO_TIMEOUT( );
////}
//////______________________________________________________________________________
////void velaChargeScopeController::set_CA_PEND_IO_TIMEOUT( double val )
////{
////    localInterface.set_CA_PEND_IO_TIMEOUT( val );
////}
//////______________________________________________________________________________
////VELA_ENUM::TRIG_STATE velaChargeScopeController::getScopeState_Py( const std::string & name )
////{
////    return getScopeState( name );
////}
#endif //BUILD_DLL



