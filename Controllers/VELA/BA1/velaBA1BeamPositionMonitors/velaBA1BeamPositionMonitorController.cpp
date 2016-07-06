#include "velaBA1BeamPositionMonitorController.h"
// stl
#include <iostream>


//______________________________________________________________________________
velaBA1BeamPositionMonitorController::velaBA1BeamPositionMonitorController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: beamPositionMonitorController( configFileLocation, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaBA1BeamPositionMonitorController::velaBA1BeamPositionMonitorController( const  bool show_messages, const bool show_debug_messages  )
:beamPositionMonitorController( "C:\\Users\\wln24624\\Documents\\VELA\\Software\\c++\\Config\\velaINJBeamPositionMonitors.config",
                                show_messages, show_debug_messages )
{}

//______________________________________________________________________________
velaBA1BeamPositionMonitorController::~velaBA1BeamPositionMonitorController(){}    //dtor

//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict velaBA1BeamPositionMonitorController::getBPMStateDefinition()
{
    std::map< VELA_ENUM::TRIG_STATE,  std::string  > m;

    m[ VELA_ENUM::TRIG_STATE::NOTRIG ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::NOTRIG  ); // ENUM_TO_STRING MACRO in structs.h
    m[ VELA_ENUM::TRIG_STATE::TRIG   ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG    );
    m[ VELA_ENUM::TRIG_STATE::TRIG_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG_ERROR   );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict velaBA1BeamPositionMonitorController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
////__________________________________________________a____________________________
//bool velaBA1BeamPositionMonitorController::hasTrig_Py( const std::string & name )
//{
//    return hasTrig( name );
//}
////______________________________________________________________________________
//bool velaBA1BeamPositionMonitorController::hasNoTrig_Py( const std::string & name )
//{
//    return hasNoTrig( name );
//}
//______________________________________________________________________________
beamPositionMonitorStructs::rawDataStruct velaBA1BeamPositionMonitorController::getAllBPMData_Py( const std::string & name, size_t N )
{
    return getAllBPMData( name, N );
}
//______________________________________________________________________________
std::vector< std::vector< double > > velaBA1BeamPositionMonitorController::getBPMRawData_Py( const std::string & name )
{
    return getBPMRawData( name );
}
//______________________________________________________________________________
std::vector< double > velaBA1BeamPositionMonitorController::getBPMXVec_Py( const std::string & name )
{
    return getBPMXVec( name );
}
//______________________________________________________________________________
std::vector< double > velaBA1BeamPositionMonitorController::getBPMYVec_Py( const std::string & name )
{
    return getBPMYVec( name );
}
//______________________________________________________________________________
std::vector< double > velaBA1BeamPositionMonitorController::getBPMQVec_Py( const std::string & name )
{
    return getBPMQVec( name );
}
//______________________________________________________________________________
std::vector< double > velaBA1BeamPositionMonitorController::getTimeStamps_Py( const std::string & name )
{
    return getTimeStamps( name );
}
//______________________________________________________________________________
std::vector< std::string > velaBA1BeamPositionMonitorController::getStrTimeStamps_Py( const std::string & name )
{
    return getStrTimeStamps( name );
}
//______________________________________________________________________________
void velaBA1BeamPositionMonitorController::monitorDataForNShots_Py( size_t N, const std::string & bpmNames )
{
    return monitorDataForNShots( N, bpmNames );
}
//______________________________________________________________________________
double velaBA1BeamPositionMonitorController::getXFromPV_Py( const std::string & name )
{
    return getXFromPV( name );
}
//______________________________________________________________________________
double velaBA1BeamPositionMonitorController::getYFromPV_Py( const std::string & name )
{
    return getYFromPV( name );
}
//______________________________________________________________________________
double velaBA1BeamPositionMonitorController::getX_Py( const std::string & name )
{
    return getX( name );
}
//______________________________________________________________________________
double velaBA1BeamPositionMonitorController::getY_Py( const std::string & name )
{
    return getY( name );
}
//______________________________________________________________________________
double velaBA1BeamPositionMonitorController::getQ_Py( const std::string & name )
{
    return getQ( name );
}
//______________________________________________________________________________
long velaBA1BeamPositionMonitorController::getRA1_Py( const std::string & name )
{
    return getRA1( name );
}
//______________________________________________________________________________
long velaBA1BeamPositionMonitorController::getRA2_Py( const std::string & name )
{
    return getRA2( name );
}
//______________________________________________________________________________
long velaBA1BeamPositionMonitorController::getRD1_Py( const std::string & name )
{
    return getRD1( name );
}
//______________________________________________________________________________
long velaBA1BeamPositionMonitorController::getRD2_Py( const std::string & name )
{
    return getRD2( name );
}
//______________________________________________________________________________
void velaBA1BeamPositionMonitorController::setSA1_Py( const std::string & bpmName, long sa1 )
{
    setSA1( bpmName, sa1 );
}
//______________________________________________________________________________
void velaBA1BeamPositionMonitorController::setSA2_Py( const std::string & bpmName, long sa2 )
{
    setSA2( bpmName, sa2 );
}
//______________________________________________________________________________
void velaBA1BeamPositionMonitorController::setSD1_Py( const std::string & bpmName, long sd1 )
{
    setSD1( bpmName, sd1 );
}
//______________________________________________________________________________
void velaBA1BeamPositionMonitorController::setSD2_Py( const std::string & bpmName, long sd2 )
{
    setSD2( bpmName, sd2 );
}
//______________________________________________________________________________
std::vector< std::string > velaBA1BeamPositionMonitorController::getBPMNames_Py()
{
    return getBPMNames();
}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE velaBA1BeamPositionMonitorController::getBPMState_Py( const std::string & name )
//{
//    return getBPMState( name );
//}
#endif //BUILD_DLL


