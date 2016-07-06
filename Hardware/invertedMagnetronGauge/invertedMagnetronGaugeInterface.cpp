#include "invertedMagnetronGaugeInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <ctime>

invertedMagnetronGaugeInterface::invertedMagnetronGaugeInterface( const std::string & configFileLocation, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr )
: configReader( configFileLocation, show_messages_ptr, show_debug_messages_ptr ), interface( show_messages_ptr, show_debug_messages_ptr )
{
    initialise();
}
//______________________________________________________________________________
invertedMagnetronGaugeInterface::invertedMagnetronGaugeInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
: configReader( show_messages_ptr, show_debug_messages_ptr  ), interface( show_messages_ptr, show_debug_messages_ptr  )
{
    initialise();
}
//______________________________________________________________________________
invertedMagnetronGaugeInterface::~invertedMagnetronGaugeInterface()
{
//    for( auto it : continuousMonitorStructs )
//    {
        debugMessage("delete invertedMagnetronGaugeInterface continuousMonitorStructs entry.");
//        delete it;
//    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::initialise()
{
    /// The config file reader

    configFileRead = configReader.readVacImgConfig();

    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file

        initVacImgObjects();

        /// subscribe to the channel ids

        initVacImgChids();

        /// start the monitors: set up the callback functions

        monitorVacImgs();

        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::getImgNames( std::vector< std::string >  & vacImgNames )
{
    vacImgNames.clear();
    for( auto const & it : allVacImgData )
        vacImgNames.push_back( it.first );
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::initVacImgObjects()
{
    const std::vector< invertedMagnetronGaugeStructs::vacImgObject > vacImgObjs = configReader.getVacImgObjects();
    for( auto const & it : vacImgObjs )
        allVacImgData[ it.name ] = it;
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::initVacImgChids()
{
    message( "\n", "Searching for VacImg chids...");
    for( auto && it1 : allVacImgData )
    {
        addILockChannels( it1.second.numIlocks, it1.second.pvRoot, it1.second.name, it1.second.iLockPVStructs );
        for( auto && it2 : it1.second.pvComStructs )
            addChannel( it1.second.pvRoot, it2.second );
        for( auto && it2 : it1.second.pvMonStructs  )
            addChannel( it1.second.pvRoot, it2.second );
    }
    int status = sendToEpics( "ca_create_channel", "Found VacImg chids.", "!!TIMEOUT!! Not all vacImg ChIds found." );

    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
        for( auto & it : allVacImgData )
        {
            message("\n", "Checking Chids for ", it.first );
            for( auto & it2 : it.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
            for( auto & it2 : it.second.pvComStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
            for( auto & it2 : it.second.iLockPVStructs )
                checkCHIDState( it2.second.CHID, it2.second.pv );
        }
        message("");
    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::addChannel( const std::string & pvRoot, invertedMagnetronGaugeStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::monitorVacImgs()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for( auto && it : allVacImgData )
    {
        monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
        for( auto && it2 : it.second.pvMonStructs  )
        {
            /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
            /// init structs 'correctly'

            continuousMonitorStructs.push_back( new invertedMagnetronGaugeStructs::monitorStruct() );
            continuousMonitorStructs.back() -> vacImgObj   = &( it.second );
            continuousMonitorStructs.back() -> interface = this;

            /// For the vacImg there is only 1 monitor type
            /// maybe it.second.pvMonStructs does not need to be a map???, pvComStructs probably does though...
//            continuousMonitorStructs.back().monType   = it2.first;

            /// If you pass DBF_STRING and recast as a char * in the callback you can get the state as GOOD, BAD, OPEN, CLOSED etc,
            /// This is useful for debugging, but in general i'm just going to subscribe to the DBR_ENUM

            /// ca_create_subscription accepts a void * user argument, we pass a pointer to the monitor struct,
            /// in the callback function this is cast back and the data can then be updated
            /// void * usrArg = reinterpret_cast< void *>( continuousMonitorStructs.back() );
            switch( it2.first )
            {
            case invertedMagnetronGaugeStructs::IMG_PV_TYPE::P:
                ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID, it2.second.MASK,  invertedMagnetronGaugeInterface::staticEntryVacImgMonitor, (void*)continuousMonitorStructs.back(), 0); // &continuousMonitorStructs.back().EventID );
                break;
            case invertedMagnetronGaugeStructs::IMG_PV_TYPE::STA:
                ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID, it2.second.MASK,  invertedMagnetronGaugeInterface::staticEntryVacImgMonitor, (void*)continuousMonitorStructs.back(), 0); // &continuousMonitorStructs.back().EventID );
                break;
            }
        }
    }
    sendToEpics( "ca_create_subscription", "Subscribed to VacImg Monitors", "!!TIMEOUT!! Subscription to VacImg monitors failed" );
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::staticEntryVacImgMonitor( const event_handler_args args )
{
    /// recast args.usr ( a void * ) to a monitor struct pointer, then dereference

    invertedMagnetronGaugeStructs::monitorStruct ms = * reinterpret_cast<invertedMagnetronGaugeStructs::monitorStruct*>(args.usr);

    /// Not sure how to decode these apart from trial and error
    /// you can test with DBF_STRING as the callback type

//    if( *(unsigned short*)args.dbr == 0 )
//        ms.vacImgObj -> vacImgState = VELA_ENUM::TRIG_STATE::NOTRIG;
//    else if( *(unsigned short*)args.dbr == 1 )
//        ms.vacImgObj -> vacImgState = VELA_ENUM::TRIG_STATE::TRIG;
//    else
//        ms.vacImgObj -> vacImgState = VELA_ENUM::TRIG_STATE::TRIG_ERROR;
    //double value = *(double*)args.dbr;

    int status = args.status;
    //struct dbr_time_double * pTD;
    int elementCount = 0;
    struct dbr_time_double * pTD;
    dbr_double_t *val;
    dbr_double_t *put;
    char timeString[36];
    std::clock_t start;
    double duration;
    double timeout = 2.0;
//
    start = std::clock();
    /// If subscribed to DBF_STRING use this to get the message
    //char * val = (char *)args.dbr;
    pTD = ( struct dbr_time_double * ) args.dbr;
    epicsTimeToStrftime ( timeString, sizeof ( timeString ), "%a %b %d %Y %H:%M:%S.%f", &pTD -> stamp );
    put = (dbr_double_t*)args.usr;
    elementCount = args.count;

    /// now we can switch based on the monitor type and then update the correct part of img object data using val
    switch( ms.monType )
    {
        case invertedMagnetronGaugeStructs::IMG_PV_TYPE::P:
            {
                val = &pTD -> value;
                for ( int i = 1; i < args.count; i++ )
                {
                    put[i] += val[i];
                }
                if( status == ECA_NORMAL )
                {
                    *( ms.pMon + ms.vacImgObj -> pCounter ) = *val;
                    ms.vacImgObj -> p = *val;
                    ++ms.vacImgObj -> pCounter;
                    if( ms.vacImgObj -> pCounter == ms.maxCounter )
                    {
                        ca_clear_subscription( ms.EventID );
                        *(ms.status) = invertedMagnetronGaugeStructs::dataCollectionResult::collected;
                    }
                    break;
                }
            }
        case invertedMagnetronGaugeStructs::IMG_PV_TYPE::STA:
            {
                if( status == ECA_NORMAL )
                {
                    if( *(unsigned short*)args.dbr == 0 )
                        ms.vacImgObj -> vacImgState = VELA_ENUM::IMG_STATE::IMG_GOOD;
                    else if( *(unsigned short*)args.dbr == 1 )
                        ms.vacImgObj -> vacImgState = VELA_ENUM::IMG_STATE::IMG_BAD;
                    else
                        ms.vacImgObj -> vacImgState = VELA_ENUM::IMG_STATE::IMG_ERROR;
                }
                break;
            }
        //default:
        //    std::cout << " -> UNKNOWN " << std::endl;
    }

    /// make debug messages easier to understand by using ENUM_TO_STRING
    //ms.interface -> debugMessage( ms.vacImgObj -> name,  " new state = ", ENUM_TO_STRING(ms.vacImgObj -> vacImgState ));
    /// If subscribed to DBF_STRING use this to get the message
    //char * val = (char *)args.dbr;
    /// now we can switch based on the monitor type and then update the correct part of the vacImg object data using val
    /// For the vacImg this is basically redundant, there is only one monitor the "Sta"
    /// (apart from interlocks, these are handled in the bas class)
//    switch( lms.monType )
//    {
//        case invertedMagnetronGaugeStructs::Sta:
//                ....
//        case SomeOtherPVType:
//                ....
//    }
}
//______________________________________________________________________________
void * invertedMagnetronGaugeInterface::addTemporaryMonitorStruct( invertedMagnetronGaugeStructs::vacImgObject * vacImgObjp , invertedMagnetronGaugeStructs::IMG_PV_TYPE pvType, std::vector< std::vector< double > > & vacImgData, invertedMagnetronGaugeStructs::dataCollectionResult * stat )
{
    temporaryMonitorStructs.push_back( new invertedMagnetronGaugeStructs::monitorStruct );
    temporaryMonitorStructs.back() -> vacImgObj  = vacImgObjp;
    temporaryMonitorStructs.back() -> maxCounter  = (int) vacImgData[0].size();
    temporaryMonitorStructs.back() -> status = stat;
    temporaryMonitorStructs.back() -> interface = this;    /// pointer to this object so we can access non-static members
    temporaryMonitorStructs.back() -> monType = pvType;   /// type of callback

    switch( pvType )
    {
        case invertedMagnetronGaugeStructs::IMG_PV_TYPE::P:
            {
                temporaryMonitorStructs.back() -> pMon = &( vacImgData[ 0 ][ 0 ] );
                break;
            }
        case invertedMagnetronGaugeStructs::IMG_PV_TYPE::STA:
            {
                break;
            }
    }
    return reinterpret_cast< void *>(temporaryMonitorStructs.back());
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::getNewVacImgData( const std::string vacImg1, std::vector< std::vector< double > > & vacImgData, std::vector< invertedMagnetronGaugeStructs::dataCollectionResult > & status  )
{
    temporaryMonitorStructs.clear();

//    for( auto && it : allVacImgData )
//    {
//        monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
//        for( auto && it2 : it.second.pvMonStructs  )
        for( auto && it2 : allVacImgData[ vacImg1 ].pvMonStructs  )
        {
            allVacImgData[ vacImg1 ].pCounter = 0;

            void * usrArg; /// a generic void * to recst the mpass to epics for call back

//            unsigned long COUNT1 = 1;
            switch ( it2.first )
            {
                case invertedMagnetronGaugeStructs::IMG_PV_TYPE::P:
                {
                    usrArg = addTemporaryMonitorStruct( &( allVacImgData[ vacImg1 ] ), invertedMagnetronGaugeStructs::IMG_PV_TYPE::P, vacImgData, &status[0] );
                    ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID, it2.second.MASK,  invertedMagnetronGaugeInterface::staticEntryVacImgMonitor, usrArg, &( temporaryMonitorStructs.back() -> EventID ) );
    //                ca_create_subscription( DBR_TIME_DOUBLE, COUNT1, it -> second.chidMap.at( it2 -> first ), DBE_VALUE,  invertedMagnetronGaugeInterface::staticEntryVacImgMonitor, usrArg, &(temporaryMonitorStructs.back() -> EventID) ) ;
                    break;
                }
                case invertedMagnetronGaugeStructs::IMG_PV_TYPE::STA:
                {
                    break;
                }
            }
        }

//    }
    int epicsStatus = ca_pend_io( 5.0 );
    SEVCHK(epicsStatus, "ca_create_subscription");

    printStatusResult( epicsStatus, "Monitoring VacImgssss", "!!TIMEOUT!! Subscription to VacImgs failed" );

}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::cancelDataMonitors()
{
    for( auto it = temporaryMonitorStructs.begin(); it != temporaryMonitorStructs.end(); ++it)
    {
        ca_clear_subscription( (*it) -> EventID );
    }
}
//______________________________________________________________________________
std::vector< std::vector < double > > invertedMagnetronGaugeInterface::monitorForNCounts( const std::string & vacImgName, int numcounts )
{
    allVacImgMonitorData.clear();
    allVacImgMonitorData.resize( 1 );

    for( size_t i = 0; i < allVacImgMonitorData.size(); ++i )
//        allVacImgMonitorData[ i ].resize( numcounts, -99.95 );
        allVacImgMonitorData[ i ].resize( numcounts );
//        std::cout << allVacImgMonitorData[ 2 ].size() << std::endl;

    std::vector< invertedMagnetronGaugeStructs::dataCollectionResult > status ( 1 , invertedMagnetronGaugeStructs::waiting );

    invertedMagnetronGaugeInterface::getNewVacImgData( vacImgName, allVacImgMonitorData, status );


    time_t timeToAbort = time( 0 ) + (int) ceil( (double) numcounts / 0.1 ) ;

    while( time( 0 ) <  timeToAbort )
    {
        if( allDataIsCollected ( status ) )
        {
            std::cout << "Data collected." << std::endl;
            lastResult = k_ACQUIRED;
            invertedMagnetronGaugeInterface::cancelDataMonitors();
            break;
        }

    }
    if( !allDataIsCollected ( status ) )
    {
        lastResult = k_TIMEOUT;
        std::cout << "TIMEOUT During Data Collection!!!!!" << std::endl;
        invertedMagnetronGaugeInterface::cancelDataMonitors();
    }

    return allVacImgMonitorData;
}
//______________________________________________________________________________
bool invertedMagnetronGaugeInterface::allDataIsCollected( std::vector< invertedMagnetronGaugeStructs::dataCollectionResult > status )
{
    bool ret = true;

    if( status[0] != invertedMagnetronGaugeStructs::dataCollectionResult::collected )
        ret = false;
//    if( status[1] != invertedMagnetronGaugeStructs::collected )
//        ret = false;
//    if( status[2] != invertedMagnetronGaugeStructs::collected )
//        ret = false;
//    if( status[3] != velaBPMStructs::collected )
//        ret = false;
    return ret;
}
//______________________________________________________________________________
double invertedMagnetronGaugeInterface::getImgP( const std::string & vacImg )
{
    if( allVacImgData.count( vacImg ) )
    {
        return allVacImgData[ vacImg ].p;
    }
    else
        return 0;
}
//______________________________________________________________________________
invertedMagnetronGaugeStructs::vacImgObject invertedMagnetronGaugeInterface::getVacImgObject( const std::string & name )
{
    invertedMagnetronGaugeStructs::vacImgObject r;
    auto iter = allVacImgData.find( name );
    if (iter != allVacImgData.end() )
        r = iter -> second;
    else
    {
        std::stringstream s;
        s << "ERROR!!! " << name << " DOES NOT EXIST";
        r.name = s.str();
        r.pvRoot = s.str();
        r.vacImgState =  VELA_ENUM::IMG_STATE::IMG_ERROR;
    }
    return r;
}
//______________________________________________________________________________
VELA_ENUM::IMG_STATE invertedMagnetronGaugeInterface::getImgState( const  std::string & objName )
{
    VELA_ENUM::IMG_STATE r =  VELA_ENUM::IMG_STATE::IMG_ERROR;
    auto iter = allVacImgData.find( objName );
    if (iter != allVacImgData.end() )
        r = iter -> second.vacImgState;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > invertedMagnetronGaugeInterface::getILockStates( const std::string & objName )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
    auto iter = allVacImgData.find( objName );
    if( iter != allVacImgData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  invertedMagnetronGaugeInterface::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allVacImgData.find( name );
    if( iter != allVacImgData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
////______________________________________________________________________________
//bool invertedMagnetronGaugeInterface::toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, invertedMagnetronGaugeInterface & obj )
//{
//    if( CALL_MEMBER_FN(obj, f1)( name ) ) /// CALL_MEMBER_FN MACRO in structs.h
//    {
//        CALL_MEMBER_FN(obj, f3)( name ) ;
//
//        time_t timeStart = time( 0 );  /// Start Clock
//
//        while( CALL_MEMBER_FN(obj, f1)( name ) )
//        {
//            std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); /// MAGIC NUMBER
//            time_t currentTime = time( 0 );
//            time_t timeDif = currentTime - timeStart;
//            if( timeDif > waitTime )
//            {
//                message( "Toggle ", name, " timed out after ", waitTime," seconds");
//                break;
//            }
//        }
//    }
//    /// Prevously we have still needed a sleep here, they shouldn't be needed, THIS NEEDS CHECKING
//// std::this_thread::sleep_for(std::chrono::milliseconds( 20 ));
//    return CALL_MEMBER_FN(obj, f2)( name );
//}
