#include "RFGunController.h"
// stl
RFGunController::RFGunController( const std::string configFileLocation1,
                                  const std::string configFileLocation2,
                                  const std::string configFileLocation3, const  bool show_messages, const bool show_debug_messages )
: controller( show_messages, show_debug_messages ), localInterface( configFileLocation1, configFileLocation2, configFileLocation3, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
RFGunController::RFGunController( const  bool show_messages, const bool show_debug_messages  )
: controller( show_messages, show_debug_messages ), localInterface( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
RFGunController::~RFGunController(){}    //dtor
//______________________________________________________________________________
void RFGunController::initialise()
{
    if( localInterface.interfaceInitReport() )
        message("RFGunController instantiation success.");
}
void RFGunController::setAmp(const long val )
{
    localInterface.setAmp( val );
}
//______________________________________________________________________________
void RFGunController::setPhi(const double val)
{
    localInterface.setPhi( val );
}
//______________________________________________________________________________
long RFGunController::getAmpW()
{
    return localInterface.getAmpW( );
}
//______________________________________________________________________________
double RFGunController::getAmpR( )
{
    return localInterface.getAmpR( );
}
//______________________________________________________________________________
double RFGunController::getPhi( )
{
    return localInterface.getPhi( );
}
//______________________________________________________________________________
double RFGunController::getFwd( const std::string & name )
{
    return localInterface.getFwd( name );
}
//______________________________________________________________________________
double RFGunController::getRev( const std::string & name )
{
    return localInterface.getRev( name );
}
//______________________________________________________________________________
double RFGunController::getRatio( const std::string & name)
{
    return localInterface.getRatio( name );
}
//______________________________________________________________________________
double RFGunController::getFwdCursor( const std::string & name )
{
    return localInterface.getFwdCursor( name );
}
//______________________________________________________________________________
bool   RFGunController::setFwdCursor( const std::string & name, const double val )
{
    return localInterface.setFwdCursor( name, val );
}
//______________________________________________________________________________
double RFGunController::getRevCursor( const std::string & name )
{
    return localInterface.getRevCursor( name );
}
//______________________________________________________________________________
bool RFGunController::setRevCursor( const std::string & name, const double val )
{
    return localInterface.setRevCursor( name, val );
}
//______________________________________________________________________________
bool RFGunController::setAllCursors(const double val )
{
    return localInterface.setAllCursors( val );
}
//______________________________________________________________________________
void RFGunController::monitorTracesForNShots( const size_t N )
{
    return localInterface.monitorTracesForNShots( N );
}
//______________________________________________________________________________
RFGunStructs::MOD_STATE RFGunController::getModState()
{
    return localInterface.getModState();
}
//______________________________________________________________________________
RFGunStructs::MOD_EXILOCK1 RFGunController::getModiLock()
{
    return localInterface.getModiLock();
}
//______________________________________________________________________________
std::string RFGunController::getModiLockStr()
{
    return ENUM_TO_STRING( localInterface.getModiLock() );
}
//______________________________________________________________________________
std::string RFGunController::getModStateStr()
{
    return ENUM_TO_STRING( localInterface.getModState() );
}
//______________________________________________________________________________
bool RFGunController::killTraceMonitors()
{
    return localInterface.killTraceMonitors();
}
//______________________________________________________________________________
bool RFGunController::isModILockStateGood()
{
    return localInterface.isModILockStateGood();
}
//______________________________________________________________________________
bool RFGunController::isModILockStateNotGood()
{
    return localInterface.isModILockStateNotGood();
}
//______________________________________________________________________________
bool RFGunController::isMonitoringTraces()
{
    return localInterface.isMonitoringTraces();
}
//______________________________________________________________________________
bool RFGunController::isNotMonitoringTraces()
{
    return localInterface.isNotMonitoringTraces();
}
//______________________________________________________________________________
bool RFGunController::isModWarmedUp()
{
    return localInterface.isModWarmedUp();
}
//______________________________________________________________________________
bool  RFGunController::isModNotWarmedUp()
{
    return localInterface.isModNotWarmedUp();
}
//______________________________________________________________________________
bool RFGunController::isModInHVOn()
{
    return localInterface.isModInHVOn();
}
//______________________________________________________________________________
bool RFGunController::isModInTrig()
{
    return localInterface.isModInTrig();
}
//______________________________________________________________________________
bool RFGunController::isModInOff()
{
    return localInterface.isModInOff();
}
//______________________________________________________________________________
bool RFGunController::isModInStandby()
{
    return localInterface.isModInStandby();
}
//______________________________________________________________________________
std::vector< std::string > RFGunController::getRFPowerNames()
{
    return localInterface.getRFPowerNames();
}
//______________________________________________________________________________
std::vector< std::vector< double >> RFGunController::getRevT( const std::string & name )
{
    return localInterface.getRevT( name );
}
//______________________________________________________________________________
std::vector< std::vector< double >> RFGunController::getFwdT(const std::string & name  )
{
    return localInterface.getFwdT( name );
}
//______________________________________________________________________________
std::vector< double > RFGunController::getRevTStamp(const std::string & name  )
{
    return localInterface.getRevTStamp( name );
}
//______________________________________________________________________________
std::vector< double > RFGunController::getFwdTStamp(const std::string & name  )
{
    return localInterface.getFwdTStamp( name );
}
//______________________________________________________________________________
std::vector< std::string > RFGunController::getRevTStampStr( const std::string & name )
{
    return localInterface.getRevTStampStr( name );
}
//______________________________________________________________________________
std::vector< std::string > RFGunController::getFwdTStampStr( const std::string & name  )
{
    return localInterface.getFwdTStampStr( name );
}
//______________________________________________________________________________
void RFGunController::modReset()
{
    return localInterface.modReset();
}
//______________________________________________________________________________
bool RFGunController::modResetAndWait( const size_t waitTime )
{
    return localInterface.modResetAndWait( waitTime);
}
//______________________________________________________________________________
void RFGunController::setModToTrig()
{
    return localInterface.setModToTrig();
}
//______________________________________________________________________________
void RFGunController::setModToHVOn()
{
    return localInterface.setModToHVOn();
}
//______________________________________________________________________________
void RFGunController::setModToStandby()
{
    return localInterface.setModToStandby();
}
//______________________________________________________________________________
void RFGunController::setModToOff()
{
    return localInterface.setModToOff();
}
//______________________________________________________________________________
double RFGunController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void RFGunController::set_CA_PEND_IO_TIMEOUT( const double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
bool RFGunController::switchOnRF( )
{
    return localInterface.switchOnRF();
}
//______________________________________________________________________________
bool RFGunController::switchOffRF(  )
{
    return localInterface.switchOffRF();
}




