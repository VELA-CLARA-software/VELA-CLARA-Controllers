#include "cameraInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>

//#include <mutex>
//std::mutex mutex1;
//std::mutex mutex2;
//std::mutex mutex3;
//std::mutex mutex4;
//std::mutex mutex5;

//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
cameraInterface::cameraInterface( const std::string & configFile_Location, const std::string &configIOCFile_Location, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr )
: configReader( configFile_Location, configIOCFile_Location, show_messages_ptr, show_debug_messages_ptr ), interface( show_messages_ptr, show_debug_messages_ptr )
, numActiveCams( 0.0 ), camDAQNum( 0 )
{
    initialise();
}
//______________________________________________________________________________
cameraInterface::cameraInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
: configReader( show_messages_ptr, show_debug_messages_ptr  ), interface( show_messages_ptr, show_debug_messages_ptr  )
, numActiveCams( 0.0 ), camDAQNum( 0 )
{
    initialise();
}
//______________________________________________________________________________
cameraInterface::~cameraInterface()
{
    for (auto && it : camDAQStructMap )
    {
        debugMessage("in cameraInterface: delete camDAQ thread ", it.first );
        it.second.thread->join();
        delete it.second.thread;
    }
    for( auto it : continuousMonitorStructs )
    {
        killMonitor( it );
        debugMessage("delete ", it->objName, " ", ENUM_TO_STRING( it->monType), " continuousMonitorStructs entry.");
        delete it;
    }
    debugMessage( "cameraInterface DESTRUCTOR COMPLETE " );
}
//______________________________________________________________________________
void cameraInterface::killMonitor( cameraStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
        debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}
//______________________________________________________________________________
void cameraInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        initObjects();
        /// subscribe to the channel ids
        initChids();
        /// start the monitors: set up the callback functions
        startMonitors();
        /// The pause allows EPICS to catch up.
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); // MAGIC NUMBER
    }
}
//______________________________________________________________________________
bool cameraInterface::initObjects()
{
    bool a2 = configReader.getCamData( allCamData );

    bool a1 = configReader.getCamIOCData( camIOC );

    for( auto && it : allCamData )
        isMonitoringMap[ it.second.name ] = false;

    if( a2 && a1 )
        return true;
    else
        return false;
}
//______________________________________________________________________________
void cameraInterface::initChids()
{
    message( "\n", "Searching for Camera chids...");
    for( auto && camIt : allCamData )
    {
        //addILockChannels( magObjIt.second.numIlocks, magObjIt.second.psuRoot, magObjIt.first, magObjIt.second.iLockPVStructs );
        for( auto && it2 : camIt.second.pvComStructs )
            addChannel( camIt.second.pvRoot, it2.second );
        for( auto && it2 : camIt.second.pvMonStructs  )
                addChannel( camIt.second.pvRoot, it2.second );
    }
    // there is one IOC atm
    for( auto && it2 : camIOC.pvMonStructs  )
        addChannel( camIOC.pvRoot, it2.second );

    int status = sendToEpics( "ca_create_channel", "Found Camera ChIds.", "!!TIMEOUT!! Not all Camera ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBER
        for( auto && camIt : allCamData )
        {
            //addILockChannels( magObjIt.second.numIlocks, magObjIt.second.psuRoot, magObjIt.first, magObjIt.second.iLockPVStructs );
            for( auto && it2 : camIt.second.pvComStructs )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
            for( auto && it2 : camIt.second.pvMonStructs  )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
        }
        message("");
        std::this_thread::sleep_for(std::chrono::milliseconds( 5000 )); /// MAGIC NUMBER
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void cameraInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    for( auto && it : allCamData )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            /// Base class function for ilocks
            //monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
            continuousMonitorStructs.push_back( new cameraStructs::monitorStruct() );
            continuousMonitorStructs.back() -> monType   = it2.first;
            continuousMonitorStructs.back() -> objName   = it.first;
            continuousMonitorStructs.back() -> interface = this;
            ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
                                    it2.second.MASK, cameraInterface::staticEntryMonitor,
                                    (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back() -> EVID );
        }
    }
    /// one IOC object ( with only 1 channel to monitor )
    for( auto && it2 : camIOC.pvMonStructs )
    {
        continuousMonitorStructs.push_back( new cameraStructs::monitorStruct() );
        continuousMonitorStructs.back() -> monType   = it2.first;
        continuousMonitorStructs.back() -> objName   = camIOC.name;
        continuousMonitorStructs.back() -> interface = this;
        ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
                                    it2.second.MASK, cameraInterface::staticEntryMonitor,
                                    (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back()-> EVID);
    }
    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Camera Monitors",
                             "!!TIMEOUT!! Subscription to Camera monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
void cameraInterface::addChannel( const std::string & pvRoot, cameraStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void cameraInterface::staticEntryMonitor(const event_handler_args args)
{
    cameraStructs::monitorStruct*ms = static_cast<  cameraStructs::monitorStruct *>(args.usr);
    switch( ms -> monType )
    {
        case cameraStructs::CAM_PV_TYPE::CAM_GAIN_RBV:
            ms->interface->allCamData[ ms->objName ].gainRBV = *(long*)args.dbr ;
            ms->interface->debugMessage( ms->objName, ", ",ms->interface->allCamData[ ms->objName ].screen, " gainRBV = ", ms->interface->allCamData[ ms->objName ].gainRBV );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_BLACK_RBV:
            ms->interface->allCamData[ ms->objName ].blackRBV = *(long*)args.dbr ;
            ms->interface->debugMessage( ms->objName, ", ", ms->interface->allCamData[ ms->objName ].screen, " blackRBV = ", ms->interface->allCamData[ ms->objName ].blackRBV );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_STATE:
            ms->interface->updateCamStatus( *(unsigned short*)args.dbr, ms->objName  );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_NUM_ACTIVE:
            ms->interface->camIOC.numActiveCameras = (size_t)*(double*)args.dbr;
            ms->interface->debugMessage( "Num Active Cams = ", ms->interface->camIOC.numActiveCameras  );
            break;
        case cameraStructs::CAM_PV_TYPE::CAM_LIMIT:
            ms->interface->camIOC.numCamLimit = (size_t)*(double*)args.dbr;
            ms->interface->debugMessage( "Active Cam Limit = ", ms->interface->camIOC.numCamLimit  );
            break;
        default:
            ms->interface->debugMessage( "!!! ERROR !!! Unknown Monitor Type passed to cameraInterface::staticEntryMonitor" );
            break;
    }
}
//______________________________________________________________________________
void cameraInterface::updateCamStatus( unsigned short value, const std::string & name )
{
    switch( value )
    {
        case 0:
            allCamData[ name ].state = cameraStructs::CAM_STATUS::CAM_OFF;
            break;
        case 1:
            allCamData[ name ].state = cameraStructs::CAM_STATUS::CAM_ON;
            break;
        default:
            allCamData[ name ].state = cameraStructs::CAM_STATUS::CAM_ERROR;
    }
    debugMessage( name, ", ", allCamData[ name ].screen, " new state = ", ENUM_TO_STRING( allCamData[ name ].state ) );
}
//______________________________________________________________________________
bool cameraInterface::start(const std::string & cam )
{
    const vec_s cams = { cam };
    return start( cams );
}
//______________________________________________________________________________
bool cameraInterface::start( const vec_s & cam )
{
    vec_s cams2;
    std::string s;
    for( auto const & it : cam )
    {
        s = it;
        if( checkIfScreenName( it, s ) )
            if( isOFF( s ) )
                cams2.push_back( s );
    }
    return toggleCam( cams2, cameraStructs::CAM_STATUS::CAM_ON );
}
//______________________________________________________________________________
bool cameraInterface::startAndWait( const std::string & cam, size_t waitTime )
{
    const vec_s cams = { cam };
    vec_s r = startAndWait( cams, waitTime );
    if( r[0] == cam )
        return true;
    else
        return false;
}
//______________________________________________________________________________
cameraInterface::vec_s cameraInterface::startAndWait( const vec_s & cam, size_t waitTime )
{
    bool shouldBreak = false;

    start( cam );

    time_t timeStart = time( 0 ); /// start clock

    while( true )
    {
        shouldBreak = true;

        for( auto && it : cam )
            if( isOFF( it ) )
                shouldBreak = false;

        if( shouldBreak )
            break;

        if( time(0) - timeStart > waitTime )
        {
            message( "TIMEOUT: in startAndWait: after ", waitTime, " seconds requested cameras did not switch on.");
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds( 100 )); // MAGIC NUMBER
    }
    vec_s r;
    for( auto && it : cam )
        if( isON( it ) )
            r.push_back( it );
    return r;
}
//______________________________________________________________________________
bool cameraInterface::stop(const std::string & cam )
{
    const vec_s cams = { cam };
    return stop( cams );
}
//______________________________________________________________________________
bool cameraInterface::stop( const vec_s & cams )
{
    vec_s cams2;
    std::string s;
    for( auto const & it : cams )
    {
        s = it;
        if( checkIfScreenName( it, s ) )
            if( isON( s ) )
                cams2.push_back( s );
    }
    return toggleCam( cams2, cameraStructs::CAM_STATUS::CAM_OFF );
}
//______________________________________________________________________________
bool cameraInterface::stopAll( )
{
    vec_s cams2;
    for( auto const & it : allCamData )
        cams2.push_back( it.first );
    return toggleCam( cams2, cameraStructs::CAM_STATUS::CAM_OFF );
}
//______________________________________________________________________________
bool cameraInterface::toggleCam( const vec_s & cams, cameraStructs::CAM_STATUS onORoff )
{
    bool success = false;
    std::vector< chid* > CHIDS;
    std::vector< chtype* > CHTYPE;
    for( auto const & it : cams )
    {
        switch( onORoff )
        {
            case cameraStructs::CAM_STATUS::CAM_ON:
                CHTYPE.push_back( &allCamData[ it ].pvComStructs[ cameraStructs::CAM_PV_TYPE::CAM_START ].CHTYPE );
                CHIDS.push_back ( &allCamData[ it ].pvComStructs[ cameraStructs::CAM_PV_TYPE::CAM_START ].CHID   );

                break;
            case cameraStructs::CAM_STATUS::CAM_OFF:
                CHTYPE.push_back( &allCamData[ it ].pvComStructs[ cameraStructs::CAM_PV_TYPE::CAM_STOP ].CHTYPE );
                CHIDS.push_back ( &allCamData[ it ].pvComStructs[ cameraStructs::CAM_PV_TYPE::CAM_STOP ].CHID   );

                break;
        }
    }
    if( CHTYPE.size() > 0 )
    {
        std::string m1 = "Timeout sending EPICS_ACTIVATE to inj Mag PSU";
        std::string m2 = "Timeout sending EPICS_SEND to inj Mag PSU";
        success = sendCommand( CHTYPE,  CHIDS,  m1, m2  );
        killCompletedThreads();
    }
    return success;
}
//______________________________________________________________________________
bool cameraInterface::sendCommand( const std::vector< chtype* > & CHTYPE, const std::vector< chid* > & CHID, const std::string & m1, const std::string & m2  )
{
    bool ret = false;
    for( size_t i = 0; i < CHTYPE.size(); ++i )
        ca_put( *CHTYPE[i], *CHID[i], &EPICS_ACTIVATE );

    int status = sendToEpics( "ca_put", "", m1.c_str() );
    if ( status == ECA_NORMAL )
        ret = true;
    return ret;
}
//______________________________________________________________________________
bool cameraInterface::isON(const std::string & cam )
{
    std::string s = cam;
    checkIfScreenName( cam, s );
    bool ans = false;
    if( entryExists( allCamData, s ) )
        if( allCamData[ s ].state == cameraStructs::CAM_STATUS::CAM_ON )
            ans = true;
    return ans;
}
//______________________________________________________________________________
bool cameraInterface::isOFF(const std::string & cam )
{
    return !isON( cam );
}
//______________________________________________________________________________
bool cameraInterface::isMonitoring( const std::string & cam )
{
    std::string s = cam;
    checkIfScreenName( cam, s );
    message("isMonitoring looking for ", s );
    if( entryExists( isMonitoringMap, s) )
        return (bool)isMonitoringMap[ s ];
    else
        return false;
}
//______________________________________________________________________________
bool cameraInterface::isNotMonitoring( const std::string & cam )
{
    return !isMonitoring( cam );
}
//______________________________________________________________________________
bool cameraInterface::entryExistsAndIsMonitoring( const std::string & cam, std::string & camToReturn )
{
    camToReturn = cam;
    checkIfScreenName( cam, camToReturn );
    if( entryExists( isMonitoringMap, camToReturn) && isMonitoring( camToReturn ) )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool cameraInterface::entryExistsAndIsNotMonitoring( const std::string & cam, std::string & camToReturn )
{
    camToReturn = cam;
    checkIfScreenName( cam, camToReturn );
    if( entryExists( isMonitoringMap, camToReturn ) && isNotMonitoring( camToReturn ) )
        return true;
    else
        return false;
}
//______________________________________________________________________________
void cameraInterface::killCompletedThreads()
{
    bool shouldKill = true;

    /// http://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it

    message( "killCompletedThreads camDAQStructMap.size = ", camDAQStructMap.size() );

    for (auto && it = camDAQStructMap.begin(); it != camDAQStructMap.end() /* not hoisted */; /* no increment */)
    {
        shouldKill = true;

        if( isMonitoring( it -> second.objName ) )
            shouldKill = false;
//        else
//            message( it -> second.objName, " is NOT monitoring              killCompletedThreads" );

        if( shouldKill )
        {
//            message("Killing camDAQStructMap ", it -> first, " callback" );

            killCallBack( &(it -> second) );

//            message("Killing camDAQStructMap ", it -> first );

            /// join before deleting...
            /// http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
            it -> second.thread->join();
            delete it -> second.thread;
            camDAQStructMap.erase( it++ );
        }
        else
            ++it;
    }
}
//______________________________________________________________________________
bool cameraInterface::startCamDataMonitor( const std::string & cam, int N )
{
    killCompletedThreads();

    bool ret = false;

    if( N == 0 )
    {
        message("ERROR: in startCamDataMonitor: Requested 0 shots of camera data.");
        return ret;
    }

    std::string temp;

    if( entryExistsAndIsNotMonitoring( cam, temp ) )
    {
        /// !! SET THIS FIRST !!
        isMonitoringMap[ temp ] = true;

        resetDataVectors( temp, N );
        camDAQStructMap[ camDAQNum ].interface = this;
        camDAQStructMap[ camDAQNum ].threadID  = camDAQNum;
        camDAQStructMap[ camDAQNum ].objName   = temp;
        camDAQStructMap[ camDAQNum ].shotCount = 0;
        camDAQStructMap[ camDAQNum ].numShots  = N;
        camDAQStructMap[ camDAQNum ].thread    = new std::thread( cameraInterface::staticEntryCamMonitorStart, std::ref( camDAQStructMap[ camDAQNum ] ) );
        ++camDAQNum;
    }
    else
        message( "Warning: in startCamDataMonitor: ", cam, "  ( or " ,temp, " ) data is alread being monitored.");
    return ret;
}
//______________________________________________________________________________
void cameraInterface::staticEntryCamMonitorStart( cameraStructs::camDAQStruct & ds  )
{
    ds.interface->attachTo_thisCAContext(); /// base member function

    ca_create_subscription( ds.interface->allCamData[ ds.objName ].pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_DATA].CHTYPE,
                            ds.interface->allCamData[ ds.objName ].pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_DATA].COUNT ,
                            ds.interface->allCamData[ ds.objName ].pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_DATA].CHID  ,
                            ds.interface->allCamData[ ds.objName ].pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_DATA].MASK  ,
                            cameraInterface::staticEntryCamMonitor, (void*)&ds, &(ds.EVID) );


    std::string s1 = "Succesfully Subscribed to " + ds.objName + " Data Monitor.";
    std::string s2 = "!!TIMEOUT!! Subscription to " + ds.objName + " data monitor failed.";

    int status = ds.interface->sendToEpics( "ca_create_subscription", s1.c_str(), s2.c_str() );
    if ( status == ECA_NORMAL )
    {
        ;//ha
    }
    else
    {
        ds.interface->isMonitoringMap[ ds.objName ] = false;
    }
}
//______________________________________________________________________________
void cameraInterface::staticEntryCamMonitor( const event_handler_args args )
{
    cameraStructs::camDAQStruct*ms = static_cast<  cameraStructs::camDAQStruct *>(args.usr);
    // first decide whicH part of thE data vec we are updating...
    size_t dataVecPart = 0;
    if( ms->numShots > -1 )
        dataVecPart = ms->shotCount;

    if ( args.status != ECA_NORMAL )
    {
        ms->interface->message( "ERROR: in staticEntryCamMonitor: args.status != ECA_NORMAL, wasn't expecting that" );
    }
    else
    {
        switch( args.type  ) /// comes for free from EPICS
        {
            case DBR_TIME_CHAR:
                //ms->interface->updateTimeCharData( )
                break;
            case DBR_CHAR:
                //ms->interface->updateCharData( )
                break;
            case DBR_TIME_LONG:
                ms->interface->updateTimeLongData( ms, dataVecPart, args  );
                break;
            case DBR_LONG:
                ms->interface->updateLongData( ms->objName, dataVecPart, (size_t)args.count, (dbr_long_t *) args.dbr  );
                break;
            default:
               ms->interface->message( "ERROR: in staticEntryCamMonitor:", args.type, " not supported.");
        }
    }
    /// update numShots
    if( ms->numShots > -1 )
    {
        ++ms->shotCount;

        ms->interface->message(  ms->objName, " shot ", ms->shotCount, " / ", ms->numShots, " acquired.");

    }
    /// kill callback ?
    if( ms->shotCount == ms->numShots )
        ms->interface->killCallBack( ms );

    if( ms -> shouldProcess )
        ms->interface->processData();

}
//______________________________________________________________________________
void cameraInterface::updateTimeLongData( cameraStructs::camDAQStruct * ms,  size_t dataVecPart, const event_handler_args & args  )
{
    const dbr_time_long * p = ( const struct dbr_time_long * ) args.dbr;
    updateTime( p->stamp, allCamData[ ms->objName ].rawData.timeStamps[ dataVecPart ], allCamData[ ms->objName ].rawData.strTimeStamps[ dataVecPart ] );
    updateLongData(  ms->objName, dataVecPart, (size_t)args.count, &(p  -> value)  );
}
//______________________________________________________________________________
void cameraInterface::updateLongData( const std::string & name, const size_t dataVecPart, const size_t count, const dbr_long_t * value  )
{
    size_t i =0;
    for( auto && it : allCamData[ name ].rawData.cameraData[ dataVecPart ])
    {
        it = *(value + i);

        if( i == 0 )
            std::cout << allCamData[ name ].screen << " DATA = " ;
        if( i < 10 )
            std::cout << allCamData[ name ].rawData.cameraData[ dataVecPart ][i] << ", ";

        if( i == 9 )
            std::cout << std::endl;
        ++i;
    }
}
//______________________________________________________________________________
void cameraInterface::killCallBack( cameraStructs::camDAQStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        isMonitoringMap[ ms->objName ] = false;
        message( ms->objName, " monitoring = false ");
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->objName);
    }
}
//______________________________________________________________________________
void cameraInterface::resetDataVectors( const std::string & cam, int N )
{// call this before startign a thread and NO mutexes required...
    allCamData[ cam ].rawData.cameraData.clear();
    allCamData[ cam ].rawData.timeStamps.clear();
    allCamData[ cam ].rawData.strTimeStamps.clear();

    size_t numShots = 0;
    if( N < 0 )
        numShots = 1;
    else
        numShots = N;

    allCamData[ cam ].rawData.cameraData.resize( numShots );
    allCamData[ cam ].rawData.timeStamps.resize( numShots );
    allCamData[ cam ].rawData.strTimeStamps.resize( numShots );

    /// resize calls the constructor, reserve just saves some space so is "faster", but makes things easier...
    for( auto && it : allCamData[ cam ].rawData.cameraData )
        it.resize( allCamData[cam].pvComStructs[cameraStructs::CAM_PV_TYPE::CAM_DATA].COUNT );
}
//______________________________________________________________________________
std::vector< std::vector< cameraStructs::camDataType >> cameraInterface::getRawData( const std::string & name  )
{
    if( entryExists( allCamData, name ) )
        return allCamData[ name ].rawData.cameraData;

    std::string s;
    if( checkIfScreenName( name, s ) )
        return allCamData[ s ].rawData.cameraData;

    else
    {
        message("ERROR: in getRawData: ", name, " does not exist.");
        std::vector< std::vector< cameraStructs::camDataType >> t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< double > cameraInterface::getRawDataStamp( const std::string & name  )
{
    if( entryExists( allCamData, name ) )
        return allCamData[ name ].rawData.timeStamps;
    std::string s;
    if( checkIfScreenName( name, s ) )
        return allCamData[ s ].rawData.timeStamps;
    else
    {
        message("ERROR: in getRawDataStamp: ", name, " does not exist.");
        std::vector< double > t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< std::string > cameraInterface::getRawDataStampStr( const std::string & name )
{
    if( entryExists( allCamData, name ) )
        return allCamData[ name ].rawData.strTimeStamps;
    std::string s;
    if( checkIfScreenName( name, s ) )
        return allCamData[ s ].rawData.strTimeStamps;
    else
    {
        message("ERROR: in getRawDataStampStr: ", name, " does not exist.");
        std::vector< std::string > t;
        return t;
    }
}
//______________________________________________________________________________
bool cameraInterface::checkIfScreenName( const std::string & name, std::string & camName )
{
    for( auto && it : allCamData )
        if( it.second.screen == name )
        {
            camName = it.first;
            return true;
        }
    return false;
}
//______________________________________________________________________________
void cameraInterface::processData()
{

    /// The function to start On-The-Fly data processing

}


