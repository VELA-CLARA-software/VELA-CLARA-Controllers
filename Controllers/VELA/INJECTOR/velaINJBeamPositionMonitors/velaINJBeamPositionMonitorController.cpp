#include "velaINJBeamPositionMonitorController.h"
// stl
#include <iostream>


//______________________________________________________________________________
velaINJBeamPositionMonitorController::velaINJBeamPositionMonitorController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: beamPositionMonitorController( configFileLocation, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaINJBeamPositionMonitorController::velaINJBeamPositionMonitorController( const  bool show_messages, const bool show_debug_messages   )
:beamPositionMonitorController( UTL::CONFIG_PATH+UTL::BPM_CONFIG,
                                show_messages, show_debug_messages )
{}

//______________________________________________________________________________
velaINJBeamPositionMonitorController::~velaINJBeamPositionMonitorController(){}    //dtor

//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict velaINJBeamPositionMonitorController::getBPMStateDefinition()
{
    std::map< VELA_ENUM::TRIG_STATE,  std::string  > m;

    m[ VELA_ENUM::TRIG_STATE::NOTRIG ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::NOTRIG  ); // ENUM_TO_STRING MACRO in structs.h
    m[ VELA_ENUM::TRIG_STATE::TRIG   ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG    );
    m[ VELA_ENUM::TRIG_STATE::TRIG_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG_ERROR   );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict velaINJBeamPositionMonitorController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
////__________________________________________________a____________________________
//bool velaINJBeamPositionMonitorController::hasTrig_Py( const std::string & name )
//{
//    return hasTrig( name );
//}
////______________________________________________________________________________
//bool velaINJBeamPositionMonitorController::hasNoTrig_Py( const std::string & name )
//{
//    return hasNoTrig( name );
//}
//______________________________________________________________________________
beamPositionMonitorStructs::rawDataStruct velaINJBeamPositionMonitorController::getAllBPMData_Py( const std::string & name, size_t N )
{
    return getAllBPMData( name, N );
}
//______________________________________________________________________________
std::vector< std::vector< double > > velaINJBeamPositionMonitorController::getBPMRawData_Py( const std::string & name )
{
    return getBPMRawData( name );
}
//______________________________________________________________________________
std::vector< double > velaINJBeamPositionMonitorController::getBPMXVec_Py( const std::string & name )
{
    return getBPMXVec( name );
}
//______________________________________________________________________________
std::vector< double > velaINJBeamPositionMonitorController::getBPMYVec_Py( const std::string & name )
{
    return getBPMYVec( name );
}
//______________________________________________________________________________
std::vector< double > velaINJBeamPositionMonitorController::getBPMQVec_Py( const std::string & name )
{
    return getBPMQVec( name );
}
//______________________________________________________________________________
std::vector< double > velaINJBeamPositionMonitorController::getTimeStamps_Py( const std::string & name )
{
    return getTimeStamps( name );
}
//______________________________________________________________________________
std::vector< std::string > velaINJBeamPositionMonitorController::getStrTimeStamps_Py( const std::string & name )
{
    return getStrTimeStamps( name );
}
//______________________________________________________________________________
bool velaINJBeamPositionMonitorController::isMonitoringBPMData_Py( const std::string & name )
{
    return isMonitoringBPMData( name );
}
//______________________________________________________________________________
bool velaINJBeamPositionMonitorController::isNotMonitoringBPMData_Py( const std::string & name )
{
    return isNotMonitoringBPMData( name );
}
//______________________________________________________________________________
void velaINJBeamPositionMonitorController::monitorDataForNShots_Py( size_t N, const std::string & bpmNames )
{
    return monitorDataForNShots( N, bpmNames );
}
//______________________________________________________________________________
double velaINJBeamPositionMonitorController::getXFromPV_Py( const std::string & name )
{
    return getXFromPV( name );
}
//______________________________________________________________________________
double velaINJBeamPositionMonitorController::getYFromPV_Py( const std::string & name )
{
    return getYFromPV( name );
}
//______________________________________________________________________________
double velaINJBeamPositionMonitorController::getX_Py( const std::string & name )
{
    return getX( name );
}
//______________________________________________________________________________
double velaINJBeamPositionMonitorController::getY_Py( const std::string & name )
{
    return getY( name );
}
//______________________________________________________________________________
double velaINJBeamPositionMonitorController::getQ_Py( const std::string & name )
{
    return getQ( name );
}
//______________________________________________________________________________
long velaINJBeamPositionMonitorController::getRA1_Py( const std::string & name )
{
    return getRA1( name );
}
//______________________________________________________________________________
long velaINJBeamPositionMonitorController::getRA2_Py( const std::string & name )
{
    return getRA2( name );
}
//______________________________________________________________________________
long velaINJBeamPositionMonitorController::getRD1_Py( const std::string & name )
{
    return getRD1( name );
}
//______________________________________________________________________________
long velaINJBeamPositionMonitorController::getRD2_Py( const std::string & name )
{
    return getRD2( name );
}
//______________________________________________________________________________
void velaINJBeamPositionMonitorController::setSA1_Py( const std::string & bpmName, long sa1 )
{
    setSA1( bpmName, sa1 );
}
//______________________________________________________________________________
void velaINJBeamPositionMonitorController::setSA2_Py( const std::string & bpmName, long sa2 )
{
    setSA2( bpmName, sa2 );
}
//______________________________________________________________________________
void velaINJBeamPositionMonitorController::setSD1_Py( const std::string & bpmName, long sd1 )
{
    setSD1( bpmName, sd1 );
}
//______________________________________________________________________________
void velaINJBeamPositionMonitorController::setSD2_Py( const std::string & bpmName, long sd2 )
{
    setSD2( bpmName, sd2 );
}
//______________________________________________________________________________
std::vector< std::string > velaINJBeamPositionMonitorController::getBPMNames_Py()
{
    return getBPMNames();
}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE velaINJBeamPositionMonitorController::getBPMState_Py( const std::string & name )
//{
//    return getBPMState( name );
//}
#endif //BUILD_DLL


