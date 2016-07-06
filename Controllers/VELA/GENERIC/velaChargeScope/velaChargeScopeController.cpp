#include "velaChargeScopeController.h"
// stl
#include <iostream>

//______________________________________________________________________________
velaChargeScopeController::velaChargeScopeController( const std::string configFileLocation1,
                                  const std::string configFileLocation2, const  bool show_messages, const bool show_debug_messages )
: scopeController( configFileLocation1, configFileLocation2, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaChargeScopeController::velaChargeScopeController( const  bool show_messages, const bool show_debug_messages  )
: scopeController("C:\\Users\\wln24624\\Documents\\VELA\\Software\\c++\\Config\\velaQScope.config",
                  "C:\\Users\\wln24624\\Documents\\VELA\\Software\\c++\\Config\\velaQScopeTrace.config",
                  show_messages, show_debug_messages )
{}

velaChargeScopeController::~velaChargeScopeController(){}    //dtor

#ifdef BUILD_DLL
boost::python::dict velaChargeScopeController::getScopeStateDefinition()
{
    std::map< VELA_ENUM::TRIG_STATE,  std::string  > m;

    m[ VELA_ENUM::TRIG_STATE::NOTRIG ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::NOTRIG  ); // ENUM_TO_STRING MACRO in structs.h
    m[ VELA_ENUM::TRIG_STATE::TRIG   ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG    );
    m[ VELA_ENUM::TRIG_STATE::TRIG_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG_ERROR   );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
void velaChargeScopeController::monitorTracesForNShots_Py( size_t N )
{
    monitorTracesForNShots( N );
}
//______________________________________________________________________________
void velaChargeScopeController::monitorNumsForNShots_Py( size_t N )
{
    monitorNumsForNShots( N );
}
//______________________________________________________________________________
std::vector< std::vector< double > > velaChargeScopeController::getScopeTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return getScopeTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > velaChargeScopeController::getMinOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return getMinOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > velaChargeScopeController::getMaxOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return getMaxOfTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > velaChargeScopeController::getAreaUnderTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return getAreaUnderTraces( name, pvType );
}
//______________________________________________________________________________
std::vector< double > velaChargeScopeController::getTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return getTimeStamps( name, pvType );
}
//______________________________________________________________________________
std::vector< std::string > velaChargeScopeController::getStrTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return getStrTimeStamps( name, pvType );
}
//______________________________________________________________________________
double velaChargeScopeController::getScopeP1_Py( const std::string & name )
{
    return getScopeP1( name );
}
//______________________________________________________________________________
double velaChargeScopeController::getScopeP2_Py( const std::string & name )
{
    return getScopeP2( name );
}
//______________________________________________________________________________
double velaChargeScopeController::getWCMQ_Py()
{
    return getWCMQ();
}
//______________________________________________________________________________
double velaChargeScopeController::getICT1Q_Py()
{
    return getICT1Q();
}
//______________________________________________________________________________
double velaChargeScopeController::getICT2Q_Py()
{
    return getICT2Q();
}
//______________________________________________________________________________
double velaChargeScopeController::getFCUPQ_Py()
{
    return getFCUPQ();
}
//______________________________________________________________________________
double velaChargeScopeController::getEDFCUPQ_Py()
{
    return getEDFCUPQ();
}
//______________________________________________________________________________
std::vector< std::string > velaChargeScopeController::getScopeNames_Py()
{
    return getScopeNames();
}
////______________________________________________________________________________
//double velaChargeScopeController::get_CA_PEND_IO_TIMEOUT()
//{
//    return localInterface.get_CA_PEND_IO_TIMEOUT( );
//}
////______________________________________________________________________________
//void velaChargeScopeController::set_CA_PEND_IO_TIMEOUT( double val )
//{
//    localInterface.set_CA_PEND_IO_TIMEOUT( val );
//}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE velaChargeScopeController::getScopeState_Py( const std::string & name )
//{
//    return getScopeState( name );
//}
#endif //BUILD_DLL


