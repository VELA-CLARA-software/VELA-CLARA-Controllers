#include "VCscopes.h"
// stl
#include <iostream>

VCscopes::VCscopes():
virtual_VELA_INJ_Scope_Controller_Obj_Exists(false),
virtual_VELA_INJ_Scope_Controller_Obj(nullptr),
offline_VELA_INJ_Scope_Controller_Obj_Exists(false),
offline_VELA_INJ_Scope_Controller_Obj(nullptr),
physical_VELA_INJ_Scope_Controller_Obj_Exists(false),
physical_VELA_INJ_Scope_Controller_Obj(nullptr)
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
}    //dtor
//______________________________________________________________________________
scopeController & VCscopes::virtual_VELA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH_VM + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH_VM + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( virtual_VELA_INJ_Scope_Controller_Obj_Exists )
    {

    }
    else
    {
        virtual_VELA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, true, true, true );
    }
    return *virtual_VELA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::offline_VELA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH_VM + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH_VM + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( offline_VELA_INJ_Scope_Controller_Obj_Exists )
    {

    }
    else
    {
        offline_VELA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, true, true, false );
    }
    return *offline_VELA_INJ_Scope_Controller_Obj;
}
//______________________________________________________________________________
scopeController & VCscopes::physical_VELA_INJ_Scope_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string scopeconf1 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_CONFIG;
    const std::string scopeconf2 = UTL::CONFIG_PATH + UTL::VELA_SCOPE_TRACE_CONFIG;

    if( physical_VELA_INJ_Scope_Controller_Obj_Exists )
    {

    }
    else
    {
        physical_VELA_INJ_Scope_Controller_Obj = new scopeController( scopeconf1, scopeconf2, true, true, true );
    }
    return *physical_VELA_INJ_Scope_Controller_Obj;
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


