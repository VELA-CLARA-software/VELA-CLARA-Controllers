#include "velaCamInterface.h"
//djs
#include "globalVelaCams.h"
#include "globalVelaScreens.h"
//stl
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <algorithm>

int velaCamInterface::SHOTCOUNT = 0; /// meh

bool velaCamInterface::instanceFlag = false;
velaCamInterface* velaCamInterface::velaCamInterfaceObject = NULL;

    /// The construction method is for creating a singleton, seemed like a worthwhile idea
    /// at the time now for Fun :o) probably practically pointless

velaCamInterface & velaCamInterface::getInstance( )
{
    if( !instanceFlag )
    {
        velaCamInterfaceObject = new velaCamInterface(  ); // constructor
        instanceFlag = true;
        return *velaCamInterfaceObject;
    }
    else
        return *velaCamInterfaceObject;
}
//______________________________________________________________________________
velaCamInterface::velaCamInterface()
{
#ifndef _OFFLINE_MODE
    /// This 'enables' callbacks, monitoring, etc

    ca_context_create ( ca_enable_preemptive_callback );

    /// This is the current (AND ONLY) context,
    /// use it to join from new threads with ca_attach_context

    thisCaContext = ca_current_context();
#endif

    initCamObjects(); /// Set up the magnetObject map
    getCamChIDs();    /// Connect to channels

    std::vector< std::string > screenNames = globalVelaCams::getScreenNames();

    checkServerStatus( screenNames ); /// On init, read in current server states.
#ifndef _OFFLINE_MODE
    sleep( 1 );
#endif
}
//______________________________________________________________________________
velaCamInterface::~velaCamInterface()
{
    //dtor
}
//______________________________________________________________________________
void velaCamInterface::attachContext()
{
#ifndef _OFFLINE_MODE
    ca_attach_context( thisCaContext );
#endif
}
//______________________________________________________________________________
void velaCamInterface::initCamObjects() /// Set up the magnetObject map
{
    std::map< std::string, std::string > screenCamServerNumMap = globalVelaCams::getCamServerNumMap();
    std::map< std::string, std::string > screenCamServerPVMap  = globalVelaCams::getCamServerPVMap();
    std::map< std::string, std::string > screenCamDataPVMap    = globalVelaCams::getCamDataPVMap();
    const std::vector< std::string >     screenNames     = globalVelaCams::getScreenNames();

   // std::vector< std::string >::const_iterator i;

    std::string tempName;
    std::cout << std::endl;

    std::map< std::string, std::string  >::const_iterator i;

    for( i = screenCamServerNumMap.begin(); i != screenCamServerNumMap.end(); ++i )
    {
        std::cout << "Create " << i -> first << "-cam...\t";
        allCameraData[ i -> first  ].name = i -> first;

        if( screenCamServerPVMap.count( i -> first ) )
            allCameraData[ i -> first ].serverPV = screenCamServerPVMap[ i -> first ] ;
        else
            std::cout << "Can't find " << i -> first << " server PV" <<std::endl;

        if( screenCamDataPVMap.count( i -> first )  )
            allCameraData[ i -> first ].dataPV = screenCamDataPVMap[ i -> first ] ;
        else
            std::cout << "Can't find " << i -> first << " cam Data PV" <<std::endl;

        if( screenCamServerNumMap.count( i -> first ) )
            allCameraData[ i -> first ].serverNum = screenCamServerNumMap[ i -> first ];
        else
            std::cout << "Can't find " << i -> first << " server Number" <<std::endl;

        allCameraData[ i -> first ].camServerState  = velaCamStructs::k_ERR;
        std::cout << " ...success." << std::endl;
    }



//
//    for( i = screenNames.begin(); i != screenNames.end(); ++i )
//    {
//        tempName = *i;
//
//        std::cout << "Create " << *i << "-cam...\t";
//        allCameraData[ tempName ].name = tempName;
//
//        if( allCameraData.count( tempName ) )
//            std::cout << allCameraData[ tempName ].name ;
//
//        if( screenCamServerPVMap.count( *i ) )
//            allCameraData[ *i ].serverPV = screenCamServerPVMap[ *i ] ;
//        else
//        {
//            std::cout << "Can't find " << *i << " server PV" <<std::endl;
//            allCameraData.erase ( tempName );
//            std::cout << "Deleting " << tempName  << std::endl;
//        }
//
//        if( screenCamDataPVMap.count( *i ) &&  allCameraData.count( tempName  ) )
//            allCameraData[ *i ].dataPV = screenCamDataPVMap[ *i ] ;
//        else
//            std::cout << "Can't find " << *i << " cam Data PV" <<std::endl;
//
//        if( screenCamServerNumMap.count( *i ) &&  allCameraData.count( tempName  ) )
//            allCameraData[ *i ].serverNum = screenCamServerNumMap[ *i ];
//        else
//            std::cout << "Can't find " << *i << " server Number" <<std::endl;
//
//        allCameraData[ *i ].camServerState  = velaCamStructs::k_ERR;
//        std::cout << " ...success." << std::endl;
//    }
    std::cout << std::endl;
}
//______________________________________________________________________________
void velaCamInterface::getCamChIDs()    /// Connect to channels
{
#ifndef _OFFLINE_MODE
    std::cout << std::endl;
    std::cout << "Searching For Camera Server ChIDs ...";
    std::map< std::string, velaCamStructs::camObject >::iterator i;

    std::stringstream s1, s2, s3;

    for( i = allCameraData.begin(); i != allCameraData.end(); ++i )
    {

        std::cout <<"Connecting " << i -> second.name << std::endl;

        std::stringstream s1, s2;
        s1 << i -> second.serverPV << ":STATUS";
        s2 << i -> second.serverPV << ":STOP";
        s3 << i -> second.serverPV << ":START";

        ca_create_channel ( s1.str().c_str(), 0, 0, 0, &(i -> second.serv_Status_chid) );
        ca_create_channel ( s2.str().c_str(), 0, 0, 0, &(i -> second.serv_Stop_chid) );
        ca_create_channel ( s3.str().c_str(), 0, 0, 0, &(i -> second.serv_Start_chid) );

        s1.str(""); // clear stringstreams
        s2.str("");
        s3.str("");
    }

    int status = ca_pend_io( 5.0 );
    SEVCHK(status, "ca_create_channel");

    printStatusResult( status, "Found." ,"!!TIMEOUT!! Not all Camera ChIds found. Checking Channel Status."  );

#endif
}
//______________________________________________________________________________
void velaCamInterface::checkServerStatus(const std::string & screenName )
{
    std::vector< std::string > screenNames;
    screenNames.push_back( screenName );
    checkServerStatus( screenNames );
}
//______________________________________________________________________________
void velaCamInterface::checkServerStatus(const std::vector< std::string > & screenName )
{
#ifndef _OFFLINE_MODE

    std::cout << "Acquiring Camera Server Statuses...";

    std::map< std::string, velaCamStructs::camObject >::iterator i;

    for( i = allCameraData.begin(); i != allCameraData.end(); ++i )
        ca_get(DBR_ENUM, i -> second.serv_Status_chid, &(i -> second.epicsCamServerState) ) ;

    int status = ca_pend_io( 2.0 );
    SEVCHK(status, "ca_get");

    printStatusResult( status, "success", "!!TIMEOUT!! Finding Current Camera Server States failed" );

    for( i = allCameraData.begin(); i != allCameraData.end(); ++i )
    {
        switch( i -> second.epicsCamServerState )
        {
            case 0:
                std::cout << i -> second.name << " server is ON" <<std::endl;
                i -> second.camServerState = velaCamStructs::k_ON;
                break;
            case 1:
                std::cout << i -> second.name << " server is OFF" <<std::endl;
                i -> second.camServerState = velaCamStructs::k_OFF;
                break;
            case 2:
                std::cout << i -> second.name << " server is 2 = ???" <<std::endl;
                break;
            default:
                std::cout << i -> second.name << " server is unknown" <<std::endl;
                break;
        }
    }
#endif
}
///
/// _______                                   _______               __ __
///|     __|.-----.----.--.--.-----.----.    |   |   |.-----.-----.|__|  |_.-----.----.
///|__     ||  -__|   _|  |  |  -__|   _|    |       ||  _  |     ||  |   _|  _  |   _|
///|_______||_____|__|  \___/|_____|__|      |__|_|__||_____|__|__||__|____|_____|__|
///
/// Sets up the permanent monitoring of server states
///
//______________________________________________________________________________
void velaCamInterface::startMonitors()
{
    std::vector< std::string > scr = globalVelaCams::getScreenNames();
    startMonitors( scr );
}
//______________________________________________________________________________
void velaCamInterface::startMonitors(const std::vector< std::string > & screenName )
{
    /// These are passed as part of the the user argument struct with the
    /// callback function so that we can identify which
    /// camera parameter is being updated
    /// With a struct you can easily add more parameters to the callback

#ifdef _OFFLINE_MODE
    std::cout << "_OFFLINE_MODE set: No Camera Monitors available" << std::endl;
#else
    std::stringstream s1, s2, s3, s4;
    std::vector< std::string >::const_iterator i;

    for( i = screenName.begin(); i != screenName.end(); ++i )
        if( allCameraData.count( *i ) ) /// Check the screen name is in the object list
            if( serverIsNotBeingMonitored( *i ) )
                addContinuousMonitorStructs( *i );

    int status = ca_pend_io( 2.0 );
    SEVCHK(status, "ca_create_subscription");

    printStatusResult( status, "Subscribed to Camera Server States", "!!TIMEOUT!! Subscription to Camera Server States failed" );
#endif
}
//______________________________________________________________________________
bool velaCamInterface::serverIsNotBeingMonitored(const std::string & screenName )
{
    bool ret = false;
    std::vector< velaCamStructs::monitorStuct* > ::iterator i;
    for( i = continuousMonitorStructs.begin(); i != continuousMonitorStructs.end(); ++i )
    {
        if( (*i) -> monType == velaCamStructs::k_serverStatus )
        {
            if( screenName.compare( (*i) -> camObj -> name ) == 0 )
            {
                ret = true;
                break;
            }
        }
    }
    return ret;
}
//______________________________________________________________________________
void velaCamInterface::addContinuousMonitorStructs(const std::string & screenName )
{
    unsigned long COUNT = 1; /// number of parameters we expect back
    continuousMonitorStructs.push_back( new velaCamStructs::monitorStuct );
    continuousMonitorStructs.back() -> camObj  = &allCameraData[ screenName ];
    continuousMonitorStructs.back() -> interface = this;                            /// pointer to this object so we can access non-static members
    continuousMonitorStructs.back() -> monType = velaCamStructs::k_serverStatus;   /// type of callback
    void * usrArg = reinterpret_cast< void *>( continuousMonitorStructs.back() );
#ifndef _OFFLINE_MODE
    ca_create_subscription( DBR_ENUM, COUNT, allCameraData[ screenName ].serv_Status_chid, DBE_VALUE,
                            velaCamInterface::staticEntryCamServerStatusMonitor, usrArg, NULL );
#endif
}
//______________________________________________________________________________
#ifndef _OFFLINE_MODE
void velaCamInterface::staticEntryCamServerStatusMonitor( struct event_handler_args args ) /// static function that can be called back from epics
{
    velaCamStructs::monitorStuct lms = * static_cast< velaCamStructs::monitorStuct *> ( args.usr );

    unsigned short sta;
    enum velaCamStructs::CAM_SERVER_STATE newSta;

    switch( lms.monType  )
    {
        case velaCamStructs::k_serverStatus:
            sta = *(unsigned short*) args.dbr;
            newSta = lms.interface ->  getState( sta );
            lms.camObj -> camServerState = newSta;

            std::cout << lms.camObj -> name << " Server Status Monitor Called. New State = " <<  newSta << std::endl;
            break;
        default:
            break;
    }
}
#endif// _OFFLINE_MODE
/// _______         __        ______                      _____         __
///|     __|.-----.|  |_     |      |.---.-.--------.    |     \.---.-.|  |_.---.-.
///|    |  ||  -__||   _|    |   ---||  _  |        |    |  --  |  _  ||   _|  _  |
///|_______||_____||____|    |______||___._|__|__|__|    |_____/|___._||____|___._|
///
/// Sets up a monitor on camera data, and flag on camera object and then acquires num. shots
/// accquiringData() allows you to test from outside if data is still being acquired
//______________________________________________________________________________
bool velaCamInterface::getCamDat( const std::string & screenName, size_t numShots, unsigned char * ptrToVecToFill , size_t totNumDataPoints )
{
    bool returnVal = false;
#ifdef _OFFLINE_MODE
    std::cout << "_OFFLINE_MODE set: No Cameras available" << std::endl;
    allCameraData[ screenName ].aquiringData = false;
#else
    //ca_attach_context( thisCaContext );

    std::cout << "The VELA Camera Interface is Acquiring " << numShots << " Shots, for " << screenName << std::endl;

    if( allCameraData.count( screenName ) )
    {
        allCameraData[ screenName ].aquiringData = true;
        std::stringstream s4;

        s4 << allCameraData[ screenName ].dataPV;

//        std::cout <<  "Starting monitor with PV = " << s4.str() << std::endl;

        ca_create_channel ( s4.str().c_str(), 0, 0, 0, &allCameraData[ screenName ].data_chid);

        int status2 = ca_pend_io(5.0);
        SEVCHK(status2, "ca_create_channel");

        printStatusResult( status2, "" ,"!!TIMEOUT!! Camera ChId not found. Checking Channel Status?"  );

    /// set the dataMonitorStruct parameters for this acquisition

        dataMonitorStruct.numShots  = numShots;
        dataMonitorStruct.camObj    = &allCameraData[ screenName ];
        dataMonitorStruct.interface = this;
        dataMonitorStruct.monType   = velaCamStructs::k_data;
        dataMonitorStruct.shotCount = 0;
        dataMonitorStruct.vecToFill = ptrToVecToFill;

        SHOTCOUNT = 0;

        void * arg0 = static_cast< void *> ( &dataMonitorStruct );

//        ca_create_subscription(DBR_CHAR, totNumDataPoints, allCameraData[ screenName ].data_chid,
//                               DBE_VALUE,  velaCamInterface::staticEntryCamDataMonitor, arg0, &(dataMonitorStruct.EventID) );

        ca_create_subscription(DBR_TIME_CHAR, totNumDataPoints, allCameraData[ screenName ].data_chid,
                               DBE_VALUE,  velaCamInterface::staticEntryCamDataMonitor, arg0, &(dataMonitorStruct.EventID) );

        int status = ca_pend_io( 2.0 );
        SEVCHK(status, "ca_create_subscription");
        printStatusResult( status, "", "!!TIMEOUT!! Subscription to Camera failed" );
    }
#endif
    return returnVal;
}
//______________________________________________________________________________
bool velaCamInterface::accquiringData(const std::string & screenName )
{
    bool ret = false;
    if( allCameraData.count( screenName ) )
        ret = allCameraData[ screenName ].aquiringData;
    return ret;
}
//______________________________________________________________________________
int velaCamInterface::acquiringData( const std::string & screenName )
{
    int ret = -1;
    if( allCameraData.count( screenName ) )
        ret = allCameraData[ screenName ].aquiringData ? 1 : 0;
    return ret;
}
//______________________________________________________________________________
#ifndef _OFFLINE_MODE
void velaCamInterface::staticEntryCamDataMonitor(struct event_handler_args args) /// static function that can be called back from epics
{
    velaCamStructs::monitorStuct lms = * ( static_cast< velaCamStructs::monitorStuct *> ( args.usr  ) );


    lms.shotCount = SHOTCOUNT;

    if ( args.status != ECA_NORMAL )
    {
        std::cout << "args.status != ECA_NORMAL, wasn't expecting that" << std::endl;
    }
    else if ( args.type == DBR_TIME_CHAR )
    {
        const struct dbr_time_char * pTD = ( const struct dbr_time_char * ) args.dbr;
        char timeString[36];
        epicsTimeToStrftime( timeString, sizeof ( timeString ), "%a %b %d %Y %H:%M:%S.%f", & pTD->stamp );


//        std::cout << std::endl;
//        std::cout << "CAMERA DATA COLLECTED AT TIME = " << timeString << std::endl;
//        std::cout << std::endl;

        const dbr_char_t * pValue = &pTD -> value;

//        std::cout << "Example Data = ";
//        for( size_t i =0; i < 10; ++i)
//            std::cout << (int)pValue[i] << ", ";
//            std::cout << std::endl;

        for( int i = 0; i < args.count; ++i )
            *( lms.vecToFill + i + lms.shotCount * args.count ) = pValue[i];

    }
    else if( args.type == DBR_CHAR )
    {
        for( int i = 0; i < args.count; ++i )
        {
            *( ( unsigned char *) lms.vecToFill  + i + lms.shotCount * args.count )
                = ( (unsigned char (*)) (args.dbr) ) [i] ;

            std::cout << "CAM DATA = ";
            if( i < 10 )
                std::cout << (int)( (unsigned char (*))args.dbr)[ i ] << ", ";
            std::cout << std::endl;
        }

    }

    //std::cout << " Shot COUNT = " << lms.shotCount << " ADDING ONE TO SHOT COUNT " << std::endl;


    SHOTCOUNT += 1;
    lms.shotCount = SHOTCOUNT;

    //std::cout << " Shot COUNT = " << lms.shotCount << std::endl;


    if( lms.shotCount == lms.numShots )
    {
        std::cout << lms.camObj -> name << " acquired " << lms.shotCount << " / " << lms.numShots << " cancelling Subscription " << std::endl;
        std::cout << std::endl;
        ca_clear_subscription( lms.EventID );
        lms.camObj -> aquiringData = false;
    }
    else
    {
        std::cout << lms.camObj ->name << " acquired " << lms.shotCount << " / " << lms.numShots << std::endl;
    }
}
#endif// _OFFLINE_MODE
/// _______         __           ___     _______         __        _______
///|     __|.-----.|  |_        /  /    |     __|.-----.|  |_     |     __|.-----.----.--.--.-----.----.-----.
///|    |  ||  -__||   _|     ,' ,'     |__     ||  -__||   _|    |__     ||  -__|   _|  |  |  -__|   _|__ --|
///|_______||_____||____|    /__/       |_______||_____||____|    |_______||_____|__|  \___/|_____|__| |_____|
///
///
//______________________________________________________________________________
enum velaCamStructs::CAM_SERVER_STATE velaCamInterface::getState( unsigned short epicsSta )
{
    /// this function is used to convert between the numbers returned by EPICS
    /// and my encoding in the vela...structs.h file...

    enum velaCamStructs::CAM_SERVER_STATE ret;
    switch(epicsSta)
    {
        case 0:
            ret = velaCamStructs::k_ON;
            break;
        case 1:
            ret = velaCamStructs::k_OFF;
            break;
        default:
            ret = velaCamStructs::k_ERR;
    }
    return ret;
}
//______________________________________________________________________________
void velaCamInterface::allServerOFF()
{
#ifndef _OFFLINE_MODE

    std::cout << "\tAll Camera Servers OFF ...";

    std::map< std::string, velaCamStructs::camObject >::iterator i;

    unsigned long stopVal = 1;

    for( i = allCameraData.begin(); i != allCameraData.end(); ++i )
        ca_put(DBR_ENUM, i -> second.serv_Stop_chid, &stopVal);

    int status = ca_pend_io( 2.0 );
    SEVCHK(status, "ca_put");

    printStatusResult( status, "success", "FAILED !!! ca_put TIMEOUT !!! " );
#endif
}
//______________________________________________________________________________
void velaCamInterface::serverON(const std::string & screenName   )
{
#ifdef _OFFLINE_MODE
    if( allCameraData.count( screenName ) )
        std::cout << "FOUND " << screenName << " camera.";
    std::cout << "\t  _OFFLINE_MODE Can't Switch on " << screenName << " server..." << std::endl;

#else
    allServerOFF();

    if( allCameraData.count( screenName ) )
    {
        unsigned long startVal = 1;
        ca_put(DBR_ENUM, allCameraData[ screenName ].serv_Start_chid, &startVal);
        int status = ca_pend_io( 2.0 );
        SEVCHK(status, "ca_put");
        printStatusResult( status, "success", "FAILED !!! ca_put TIMEOUT !!! " );
    }
    else
        std::cout << "FAILED camera not found" << std::endl;

    sleep(1);

    std::vector< std::string > screenNames = globalVelaCams::getScreenNames();

    checkServerStatus( screenNames ); /// On init, read in current server states.
#endif
}
//______________________________________________________________________________
void velaCamInterface::getActiveCameraServers(std::map< std::string, velaCamStructs::CAM_SERVER_STATE > & activeMap )
{
    std::map< std::string, velaCamStructs::CAM_SERVER_STATE >::iterator i;
    for( i = activeMap.begin(); i != activeMap.end(); ++i )
    {
        if ( allCameraData.count( i -> first ) )
        {
            i -> second = allCameraData[ i -> first ].camServerState;
        }
    }
}
//______________________________________________________________________________
std::vector< std::string > velaCamInterface::getActiveCameraServers( )
{
    std::map< std::string, velaCamStructs::camObject >::iterator i;
    std::vector< std::string > ret;
    for( i = allCameraData.begin(); i != allCameraData.end(); ++i )
        if( i -> second.camServerState ==  velaCamStructs::k_ON )
            ret.push_back( i -> second.name);
    return ret;
}
//______________________________________________________________________________
void velaCamInterface::printStatusResult( int status, const char * success, const char* timeout)
{
#ifndef _OFFLINE_MODE
    switch (status)
    {
        case ECA_NORMAL:
            if( strlen( success ) != 0 )
                std::cout << success << std::endl;
            break;
        case ECA_TIMEOUT:
            if( strlen( timeout ) != 0 )
                std::cout << timeout << std::endl;
            //checkCHIDState();
            break;
        default:
            std::cout << "!!! Unexpected error while searching: " << ca_message( status ) << std::endl;
    }
#endif
}


