#include "velaCamController.h"
// stl
#include <iostream>
// djs
#include "velaCamInterface.h"
#include "cameraDataObject.h"
#include "globalVelaCams.h"
/// _______         __               ___
///|_     _|.-----.|  |_.-----.----.'  _|.---.-.----.-----.
/// _|   |_ |     ||   _|  -__|   _|   _||  _  |  __|  -__|
///|_______||__|__||____|_____|__| |__|  |___._|____|_____|
///
velaCamController::velaCamController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & allGunProtsConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs):
controller(show_messages,show_debug_messages),
localInterface(allGunProtsConf,startVirtualMachine,&SHOW_MESSAGES,&SHOW_DEBUG_MESSAGES,shouldStartEPICs),
shouldStartEPICs(shouldStartEPICs)
{
#ifndef _WINDOWS_MACHINE
    /// Set ENVIRONMENT Variables
    /// Here, in c++, DON'T use double quotes for multiple address
    /// The shell command would be ' export EPICS_CA_ADDR_LIST="192.168.83.255 148.79.168.63"  '
    /// This cost me a load of debugging time....

    char add[] = "EPICS_CA_ADDR_LIST=148.79.168.63 192.168.83.255 ";
    int ret1 = putenv( add );
    if( ret1 < 0 )
        std::cout << "putenv( " << add << " ) FAILED" << std::endl;

    char mBy[] = "EPICS_CA_MAX_ARRAY_BYTES=100000000";
    int ret2 = putenv( mBy );

    if( ret2 < 0 )
        std::cout << "putenv( " << mBy << " ) FAILED" << std::endl;
#endif

    /// There is only ever one velaCamInterface, it's a singleton

    camInterface = &velaCamInterface::getInstance( );
    std::cout << "The velaCamController got a velaCamInterface instance" << std::endl;

   // bsCalculator = new beamSizeCalculator();

}
//______________________________________________________________________________
velaCamController::~velaCamController()
{
}
//______________________________________________________________________________
void velaCamController::attachContext()
{
    camInterface -> attachContext();
}
//______________________________________________________________________________
void velaCamController::startMonitors()
{
    camInterface -> startMonitors();
}
//______________________________________________________________________________
void velaCamController::startMonitors(const std::vector< std::string > & screenNames )
{
    camInterface -> startMonitors( screenNames );
}
///
/// _____         __              _______ __     __              __
///|     \.---.-.|  |_.---.-.    |       |  |--.|__|.-----.----.|  |_.-----.
///|  --  |  _  ||   _|  _  |    |   -   |  _  ||  ||  -__|  __||   _|__ --|
///|_____/|___._||____|___._|    |_______|_____||  ||_____|____||____|_____|
///                                            |___|
///
/// Each screen can have an associated data object. These are held in "usual way" a map, allCamDataObjects
/// A cameraDataObject holds raw data, screen cuts, can process data etc. Here we need to be able to create
/// new objects, get new data, add data, process data, set cut parameters, etc
/// getObject() Should be called at the start of every function to check if the dataobject exists / or create one
///
//______________________________________________________________________________
void velaCamController::getObject(const std::string & screenName )
{
    std::string sn = screenName;
    if( !allCamDataObjects.count( sn ) )
        allCamDataObjects[ sn ] = new cameraDataObject( sn );
}
    /// Sometimes you want a references to the entire data, i.e. for plotting
    /// Sometimes you want a pointer to part of the data (in units of totNumPix) for acquiring new data
//______________________________________________________________________________
std::vector< unsigned char > & velaCamController::getRawRef(const std::string & screenName, velaCamStructs::DATA_TYPE type )
{
    getObject( screenName );
    return allCamDataObjects[ screenName ] -> getRawRef( type );
}
//______________________________________________________________________________
std::vector< double > & velaCamController::getProcRef(const std::string & screenName, velaCamStructs::DATA_TYPE type )
{
    getObject( screenName );
    return allCamDataObjects[ screenName ] -> getProcRef( type );
}
//______________________________________________________________________________
unsigned char * velaCamController::getRawRef(const std::string & screenName, velaCamStructs::DATA_TYPE type, size_t part  )
{
    getObject( screenName );
    return allCamDataObjects[ screenName ] -> getRawRef( type, part );
}
//______________________________________________________________________________
double * velaCamController::getProcRef(const std::string & screenName, velaCamStructs::DATA_TYPE type, size_t part  )
{
    getObject( screenName );
    return allCamDataObjects[ screenName ] -> getProcRef( type, part );
}
//______________________________________________________________________________
void velaCamController::resetRaw( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots )
{
    getObject( screen );
    return allCamDataObjects[ screen ] -> resetRaw( type, numShots );
}
//______________________________________________________________________________
/// _______         __           ___     ______                                         _____         __
///|     __|.-----.|  |_        /  /    |   __ \.----.-----.----.-----.-----.-----.    |     \.---.-.|  |_.---.-.
///|    |  ||  -__||   _|     ,' ,'     |    __/|   _|  _  |  __|  -__|__ --|__ --|    |  --  |  _  ||   _|  _  |
///|_______||_____||____|    /__/       |___|   |__| |_____|____|_____|_____|_____|    |_____/|___._||____|___._|
///
///
//______________________________________________________________________________
bool velaCamController::getData( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots )
{
    return addData( screen, type, numShots, 0 );
}
//______________________________________________________________________________
bool velaCamController::addData( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots, size_t startShot )
{
    bool ans = false;
    getObject( screen );
#ifdef _OFFLINE_MODE
    ans = allCamDataObjects[ screen ] -> setDataToDebug();
    allCamDataObjects[ screen ] -> setDataIsSet ( ans );
    allCamDataObjects[ screen ] -> setDataIsCut( false );
#else
    size_t totNumDataPoints = numShots * allCamDataObjects[ screen ] -> getNumPix( );

    camInterface -> getCamDat( screen, numShots, allCamDataObjects[ screen ] -> getRawRef( type, startShot ), totNumDataPoints );


    bool acquiringdata = camInterface -> accquiringData( screen );

    std::cout << "START Check aquiringdata? = " <<  acquiringdata << std::endl;


    time_t timeStart, currentTime, timeDif, TIMEOUT;
    timeDif = 0;
    timeStart = time( 0 );              /// Start Clock

    TIMEOUT = numShots * 1;
    bool timeout = false;

    while( acquiringdata )
    {
        acquiringdata = camInterface -> accquiringData( screen );
//        std::cout << "while Check aquiringdata? = " <<  acquiringdata << std::endl;
//#ifndef _WINDOWS_MACHINE
//        usleep( 100000 );
//#endif
        currentTime = time( 0 );
        timeDif = currentTime - timeStart;
        if( timeDif > TIMEOUT )
        {
            timeout = true;
            std::cout << "TIMOUT WHILE ACQUIRING CAM DATA " << std::endl;
            break;
        }
    }

    if( timeout )
    {
        /// !!!! SEND A CANCEL SUB TO THE INTERFACE !!!!
        std::cout << "TIME OUT IN velaCamController While acquiring Camera Data " << std::endl;
    }
    else
    {
        std::cout << std::endl;
        std::cout << "From CamController, DATA ACQUIRED!" << std::endl;

        allCamDataObjects[ screen ] -> setDataIsSet( true );
        allCamDataObjects[ screen ] -> setDataIsCut( false );
    }
    ans = !timeout;
#endif // _OFFLINE_MODE
    return ans;
}
//______________________________________________________________________________
bool velaCamController::process( const std::string & screen, velaCamStructs::DATA_TYPE type )
{
    getObject( screen );
    return allCamDataObjects[ screen ] -> process( type );
}
//______________________________________________________________________________
//bool velaCamController::processData( std::string & screen, bool & isDataSaturated )
//{
//    getObject( screen );
//    return allCamDataObjects[ screen ] -> processData( isDataSaturated );
//}
//______________________________________________________________________________
///
/// __                   ______         __     _______ __                    __
///|__|.--------.-----. |      |.--.--.|  |_  |     __|  |_.----.--.--.----.|  |_
///|  ||        |  _  | |   ---||  |  ||   _| |__     |   _|   _|  |  |  __||   _|
///|__||__|__|__|___  | |______||_____||____| |_______|____|__| |_____|____||____|
///             |_____|
///
/// See velaCamStructs, holds all info defining screen relative to entire image
//______________________________________________________________________________
velaCamStructs::imgCutStruct velaCamController::getIMGCutParam( const std::string & screenName )
{
    getObject( screenName );
    return allCamDataObjects[ screenName ] -> getIMGCutParam( );
}
//______________________________________________________________________________
void velaCamController::setIMGCutParam( const std::string & screenName, velaCamStructs::imgCutStruct & p )
{
    getObject( screenName );
    allCamDataObjects[ screenName ] -> setIMGCutParam( p );
}
//______________________________________________________________________________
/// ______                                       _______
///|      |.---.-.--------.-----.----.---.-.    |     __|.-----.----.--.--.-----.----.-----.
///|   ---||  _  |        |  -__|   _|  _  |    |__     ||  -__|   _|  |  |  -__|   _|__ --|
///|______||___._|__|__|__|_____|__| |___._|    |_______||_____|__|  \___/|_____|__| |_____|
///
///
//______________________________________________________________________________
void velaCamController::serverON( const std::string & screenName   )
{
    //std::cout << "velaCamController::serverON( " << screenName << " )" << std::endl;
    //std::cout << "This feature is disabled at the moment" << std::endl;
    camInterface -> serverON( screenName );
}
//______________________________________________________________________________
void velaCamController::serverON( const char * screenName   )
{
    std::string sn = screenName;
    std::cout << "velaCamController::serverON( " << screenName << " )  .... address === " << camInterface <<  std::endl;
    camInterface -> serverON( sn );
}
//______________________________________________________________________________
void velaCamController::getActiveCameraServers( std::map< std::string, velaCamStructs::CAM_SERVER_STATE > & activeMap )
{
    camInterface -> getActiveCameraServers( activeMap );
}
//______________________________________________________________________________
std::vector< std::string >  velaCamController::getActiveCameraServers(  )
{
    return camInterface -> getActiveCameraServers( );
}
