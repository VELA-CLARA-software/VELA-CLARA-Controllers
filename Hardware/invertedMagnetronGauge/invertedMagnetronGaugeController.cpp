#include "invertedMagnetronGaugeController.h"
// stl
#include <iostream>
// djs
#include "invertedMagnetronGaugeInterface.h"

//______________________________________________________________________________
invertedMagnetronGaugeController::invertedMagnetronGaugeController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: controller( show_messages, show_debug_messages ), localInterface( configFileLocation, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
invertedMagnetronGaugeController::invertedMagnetronGaugeController( const  bool show_messages, const bool show_debug_messages  )
: controller( show_messages, show_debug_messages ), localInterface( &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES )
{
    initialise();
}
//______________________________________________________________________________
void invertedMagnetronGaugeController::initialise()
{
    /// Keep a local copy of all the existing shutter names for openShutter1(), openShutter2(), etc...

    localInterface.getImgNames( vacImgNames );

    message("The invertedMagnetronGaugeController succesfully instantiated a invertedMagnetronGaugeInterface.");
}
//______________________________________________________________________________
invertedMagnetronGaugeController::~invertedMagnetronGaugeController(){}    //dtor
//______________________________________________________________________________
double invertedMagnetronGaugeController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void invertedMagnetronGaugeController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
std::vector< std::vector< double > > invertedMagnetronGaugeController::monitorForNCounts( const std::string & vacImgName, int numcounts )
{
    return localInterface.monitorForNCounts( vacImgName, numcounts );
}
//______________________________________________________________________________
double invertedMagnetronGaugeController::getImgP( const std::string & name )
{
    return localInterface.getImgP( name );
}
//______________________________________________________________________________
VELA_ENUM::IMG_STATE  invertedMagnetronGaugeController::getImgState( const std::string & name )
{
    return localInterface.getImgState( name );
}
//______________________________________________________________________________
std::string invertedMagnetronGaugeController::getImgStateStr( const std::string & name )
{
    return ENUM_TO_STRING(localInterface.getImgState( name ));
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > invertedMagnetronGaugeController::getILockStates( const std::string & objName )
{
    return localInterface.getILockStates( objName );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > invertedMagnetronGaugeController::getILockStatesStr( const std::string & objName )
{
    return localInterface.getILockStatesStr( objName );
}
//______________________________________________________________________________
