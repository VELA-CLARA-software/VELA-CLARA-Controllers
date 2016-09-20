#include "velaCameraController.h"
// stl
velaCameraController::velaCameraController( const std::string configFileLocation, const std::string configIOCFileLocation, const  bool show_messages, const bool show_debug_messages )
: cameraController( configFileLocation, configIOCFileLocation, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaCameraController::velaCameraController(const bool RealMachine, const  bool show_messages, const bool show_debug_messages  )
: cameraController((RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_CAMERA_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_CAMERA_CONFIG,
                   (RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_CAMERA_IOC_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_CAMERA_IOC_CONFIG,
                   show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaCameraController::~velaCameraController(){}    //dtor
//______________________________________________________________________________

#ifdef BUILD_DLL
//______________________________________________________________________________
//boost::python::dict velaCameraController::getILockStatesStr_Py( std::string magName )
//{
//    return enumStringMapToPythonDict( getILockStatesStr( magName ) );
//}
////______________________________________________________________________________
//boost::python::dict velaCameraController::getILockStates_Py( std::string magName )
//{
//    return enumMapToPythonDict( getILockStates( magName ) );
//}
//______________________________________________________________________________
bool velaCameraController::isON_Py( const std::string cam )
{
    return isON( cam );
}
//______________________________________________________________________________
bool velaCameraController::isOFF_Py( const std::string cam )
{
    return isON( cam );
}
//______________________________________________________________________________
bool velaCameraController::isMonitoring_Py( const std::string cam )
{
    return isMonitoring( cam );
}
//______________________________________________________________________________
bool velaCameraController::isNotMonitoring_Py( const std::string cam )
{
    return isNotMonitoring( cam );
}
//______________________________________________________________________________
bool velaCameraController::start_Py1(const std::string cam )
{
    return start( cam );
}
//______________________________________________________________________________
bool velaCameraController::start_Py2( const std::vector< std::string  > cam )
{
    return start( cam );
}
//______________________________________________________________________________
bool velaCameraController::stop_Py1(const std::string cam )
{
    return stop( cam );
}
//______________________________________________________________________________
bool velaCameraController::stop_Py2( const std::vector< std::string  >  cam )
{
    return stop( cam );
}
//______________________________________________________________________________
bool velaCameraController::startCamDataMonitor_Py( const std::string cam, size_t N )
{
    return startCamDataMonitor( cam, N );
}
//______________________________________________________________________________
std::vector< std::vector< cameraStructs::camDataType >> velaCameraController::getRawData_Py(const std::string name )
{
    return getRawData( name );
}
//______________________________________________________________________________
std::vector< double > velaCameraController::getRawDataStamp_Py(const std::string name )
{
    return getRawDataStamp( name );
}
//______________________________________________________________________________
std::vector< std::string > velaCameraController::getRawDataStampStr_Py( const std::string name )
{
    return getRawDataStampStr( name );
}
//______________________________________________________________________________
bool velaCameraController::startAndWait_Py1( const std::string cam, size_t timeout )
{
    return startAndWait( cam, timeout );
}
//______________________________________________________________________________
std::vector< std::string > velaCameraController::startAndWait_Py2( const std::vector< std::string  > cam, size_t timeout )
{
    return startAndWait( cam, timeout );
}
#endif



