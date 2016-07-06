#include "scopeInterface.h"
//djs
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstring>
#include <typeinfo>
#include <stdlib.h>
#include <epicsTime.h>

scopeInterface::scopeInterface( const std::string configFileLocation1,
                                const std::string configFileLocation2, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr )
: configReader( configFileLocation1, configFileLocation2, show_messages_ptr, show_debug_messages_ptr ), interface( show_messages_ptr, show_debug_messages_ptr )
{
    initialise();
}
//______________________________________________________________________________
scopeInterface::scopeInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
: configReader( show_messages_ptr, show_debug_messages_ptr  ), interface( show_messages_ptr, show_debug_messages_ptr  )
{
    initialise();
}
//______________________________________________________________________________
scopeInterface::~scopeInterface()
{
//    for( auto it : continuousMonitorStructs )
//    {
        debugMessage("delete scopeInterface continuousMonitorStructs entry.");
//        delete it;
//    }
}
//______________________________________________________________________________
void scopeInterface::initialise()
{
    /// The config file reader

    configFileRead = configReader.readConfigFiles();

    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file

        initScopeObjects();

        /// subscribe to the channel ids

        initScopeChids();

        /// start the monitors: set up the callback functions

        monitorScopes();

        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
    }
}
//______________________________________________________________________________
void scopeInterface::initScopeObjects()
{
    scopeObj = configReader.getScopeObject();
}
//______________________________________________________________________________
void scopeInterface::initScopeChids()
{
    message( "\n", "Searching for scope chids..." );
    // trace objects
    for( auto && it1 : scopeObj.traceObjects )
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );
    // llrf
    for( auto && it : scopeObj.numObject.pvMonStructs )
        addChannel( scopeObj.numObject.pvRoot, it.second );
    // send
    int status = sendToEpics( "ca_create_channel", "Found scope chids.", "!!TIMEOUT!! Not all scope ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBERR
        for( auto && it1 : scopeObj.traceObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        for( auto && it : scopeObj.numObject.pvMonStructs )
            checkCHIDState(  it.second.CHID,  it.second.pvSuffix );
        message("");
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void scopeInterface::addChannel( const std::string & pvRoot, scopeStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << ":" << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void scopeInterface::monitorScopes()
{
    continuousMonitorStructs.clear();

    for( auto && it1 : scopeObj.traceObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
//            if( !isADataPV( it2.second.pvType ) )
//            {   //NOT SURE ABOUT THIS "IF" -----> I WANT TO SET THIS BOOL TO TRUE EVEN IF IT IS A DATA PV
                it1.second.isAContinuousMonitorStruct = true;
                if( it1.second.isAContinuousMonitorStruct )
                {
                    resetTraceVectors( 1 );
                    addToMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );
                }
//            }
        }
    }

//
    for( auto && it : scopeObj.numObject.pvMonStructs )
    {
        scopeObj.numObject.isAContinuousMonitorStruct = true;
        if( scopeObj.numObject.isAContinuousMonitorStruct )
        {
            resetNumVectors( 1 );
            addToMonitorStructs( continuousMonitorStructs, it.second, NULL  );
        }
    }

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to scope Monitors", "!!TIMEOUT!! Subscription to scope Monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
void scopeInterface::addToMonitorStructs( std::vector< scopeStructs::monitorStruct * > & msv, scopeStructs::pvStruct & pv,  scopeStructs::scopeTraceData * traceObj   )
{
    msv.push_back( new scopeStructs::monitorStruct() );
    msv.back() -> monType      = pv.pvType;
    msv.back() -> scopeObject  = &scopeObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;
    msv.back() -> EVID         = &pv.EVID;

    switch( pv.pvType )
    {
        case scopeStructs::SCOPE_PV_TYPE::TR1:
        {
            // For the traces we can just pass in the trace object for value, and figure everything out from them in the callback functions
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::TR2:
        {
            // For the traces we can just pass in the trace object for value, and figure everything out from them in the callback functions
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::TR3:
        {
            // For the traces we can just pass in the trace object for value, and figure everything out from them in the callback functions
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::TR4:
        {
            // For the traces we can just pass in the trace object for value, and figure everything out from them in the callback functions
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P1:
        {
            msv.back() -> val = (void*) &scopeObj.numObject.p1;
//            msv.back() -> val = (void*) &scopeObj.numObject.p1Vec.back();
            msv.back() -> objName = scopeObj.numObject.name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P2:
        {
            msv.back() -> val = (void*) &scopeObj.numObject.p2;
//            msv.back() -> val = (void*) &scopeObj.numObject.p2Vec.back();
            msv.back() -> objName = scopeObj.numObject.name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P3:
        {
            msv.back() -> val = (void*) &scopeObj.numObject.p3;
//            msv.back() -> val = (void*) &scopeObj.numObject.p3Vec.back();
            msv.back() -> objName = scopeObj.numObject.name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P4:
        {
            msv.back() -> val = (void*) &scopeObj.numObject.p4;
//            msv.back() -> val = (void*) &scopeObj.numObject.p4Vec.back();
            msv.back() -> objName = scopeObj.numObject.name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  scopeInterface::staticEntryrMonitor, (void*)msv.back(), &pv.EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void scopeInterface::staticEntryrMonitor( const event_handler_args args )
{
    scopeStructs::monitorStruct * ms = static_cast< scopeStructs::monitorStruct *> ( args.usr );
    switch( ms -> monType )
    {
        case scopeStructs::SCOPE_PV_TYPE::TR1:
            ms->interface->updateTrace( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::TR2:
            ms->interface->updateTrace( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::TR3:
            ms->interface->updateTrace( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::TR4:
            ms->interface->updateTrace( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::P1:
            ms->interface->updateValue( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::P2:
            ms->interface->updateValue( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::P3:
            ms->interface->updateValue( ms, args );
            break;
        case scopeStructs::SCOPE_PV_TYPE::P4:
            ms->interface->updateValue( ms, args );
            break;
    }
}
//______________________________________________________________________________
void scopeInterface::updateTrace( scopeStructs::monitorStruct * ms, const event_handler_args args )
{
    /// this could be better, with the type passed from the config
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    scopeStructs::scopeTraceData * td = reinterpret_cast< scopeStructs::scopeTraceData *> (ms -> val);

    if( td->isAContinuousMonitorStruct)
    {
        for( auto && it1 : scopeObj.traceObjects )
        {
            it1.second.numShots = 1;
            for( auto && it2 : it1.second.shotCounts )
            {
                it2.second = 0;
            }
        }
    }
    const dbr_double_t * value = &(p  -> value);
    size_t i =0;
    int zero = 0;

    updateTime( p->stamp, td->timeStamps.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ],
               td->strTimeStamps.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ]  );
    for( auto && it : td->traceData.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ] )
    {
        it = *( &p->value + i );
        ++i;
    }
    if( td -> isATemporaryMonitorStruct )
    {
        if( td -> numShots > -1 )
        {
            ++td -> shotCounts.at( ms -> monType );
        }
        if( td->shotCounts.at( ms -> monType ) == td->numShots )
        {
            td->isMonitoringMap.at( ms -> monType ) = false;
            message( "Collected ", td->shotCounts.at( ms -> monType ), " shots for ", td -> pvRoot );
            ms->interface->killCallBack( ms, td );
            monitoringTraces = false;
    //        td -> shotCounts.at( ms -> monType ) = zero;
        }
    }
}
//______________________________________________________________________________
void scopeInterface::updateValue( scopeStructs::monitorStruct * ms, const event_handler_args args )
{
    switch( ms -> CHTYPE )
    {
        case DBR_DOUBLE:
        {
            *(double*)ms -> val = *(double*)args.dbr;
//            ms->interface->message(ms->scopeObject->name," ",ms->objName," ",ENUM_TO_STRING( ms -> monType), " = ", *(double*)ms -> val );
            break;
        }

        default:
            ms->interface->debugMessage("scopeInterface::updateValue recieved unexpected chtype");
    }
}
//______________________________________________________________________________
void scopeInterface::monitorTracesForNShots( size_t N )
{
    if( !monitoringTraces )
    {
        traceMonitorStructs.clear();
        debugMessage( "Starting scope Traces Monitor " );
        resetTraceVectors( N );
        debugMessage( "Vectors Reset" );
        for( auto && it1 : scopeObj.traceObjects )
        {
            it1.second.numShots = N;
            it1.second.isAContinuousMonitorStruct=false;
            it1.second.isATemporaryMonitorStruct=true;
            for( auto && it2 : it1.second.pvMonStructs )
            {
                if( isATracePV( it2.first ) )
                {
                    for( auto it3 : it1.second.isMonitoringMap )
                    {
                        it3.second = true;
                    }
                    monitoringTraces = true;

                    addToMonitorStructs( traceMonitorStructs, it2.second, &it1.second  );
                }
            }
        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to scope Trace Monitors failed" );
        if ( status == ECA_NORMAL )
            monitoringTraces = true; /// interface base class member
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void scopeInterface::resetTraceVectors( size_t N )
{
    traceMonitorStructs.clear();
    for( auto && it : scopeObj.traceObjects )
    {   /// Clear all trace data + timestamps
        /// Resize to N shots
        for( auto && it2 : it.second.traceData )
        {

            it2.second.clear();
            it2.second.resize( N );
            for( auto && it3 : it2.second )
            {
                it3.resize( it.second.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::TR1 ].COUNT );
            }
        }
        for( auto && it3 : it.second.timeStamps )
        {
            it3.second.clear();
            it3.second.resize( N );
        }
        for( auto && it4 : it.second.strTimeStamps )
        {
            it4.second.clear();
            it4.second.resize( N );
        }
        for( auto && it5 : it.second.shotCounts )
        {
            it5.second = 0;
        }
        // resize sub-vectors to COUNT elements
//        for( auto && it2 : it.second.tr1TraceData )
//            it2.resize( it.second.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::TR1 ].COUNT );
//        for( auto && it2 : it.second.tr2TraceData )
//            it2.resize( it.second.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::TR2 ].COUNT );
//        for( auto && it2 : it.second.tr3TraceData )
//            it2.resize( it.second.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::TR3 ].COUNT );
//        for( auto && it2 : it.second.tr4TraceData )
//            it2.resize( it.second.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::TR4 ].COUNT );
    }
}
//______________________________________________________________________________
void scopeInterface::monitorNumsForNShots( size_t N )
{
    if( !monitoringNums )
    {
        numMonitorStructs.clear();
        debugMessage( "Starting scope numbers Monitor " );
        resetNumVectors( N );
        std::string name = "WVF01";
        debugMessage( "Vectors Reset" );
        scopeObj.numObject.numShots = N;
        for( auto && it2 : scopeObj.numObject.pvMonStructs )
        {
            if( isANumPV( it2.first ) )
            {
                monitoringNums = true;
//                addToMonitorStructs( numMonitorStructs, it2.second, &scopeObj.traceObjects.at( name )  );
                addToMonitorStructs( numMonitorStructs, it2.second, NULL  );
            }
        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to scope Trace Monitors failed" );
        if ( status == ECA_NORMAL )
            monitoringNums = true; /// interface base class member
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void scopeInterface::resetNumVectors( size_t N )
{
    numMonitorStructs.clear();
    /// Clear all vectors for scope P values
    scopeObj.numObject.p1Vec.clear();
    scopeObj.numObject.p2Vec.clear();
    scopeObj.numObject.p3Vec.clear();
    scopeObj.numObject.p4Vec.clear();
    scopeObj.numObject.p1TimeStamps.clear();
    scopeObj.numObject.p2TimeStamps.clear();
    scopeObj.numObject.p3TimeStamps.clear();
    scopeObj.numObject.p4TimeStamps.clear();
    scopeObj.numObject.strP1TimeStamps.clear();
    scopeObj.numObject.strP2TimeStamps.clear();
    scopeObj.numObject.strP3TimeStamps.clear();
    scopeObj.numObject.strP4TimeStamps.clear();
    /// Resize to N shots
    scopeObj.numObject.p1Vec.resize( N );
    scopeObj.numObject.p2Vec.resize( N );
    scopeObj.numObject.p3Vec.resize( N );
    scopeObj.numObject.p4Vec.resize( N );
    scopeObj.numObject.p1TimeStamps.resize( N );
    scopeObj.numObject.p2TimeStamps.resize( N );
    scopeObj.numObject.p3TimeStamps.resize( N );
    scopeObj.numObject.p4TimeStamps.resize( N );
    scopeObj.numObject.strP1TimeStamps.resize( N );
    scopeObj.numObject.strP2TimeStamps.resize( N );
    scopeObj.numObject.strP3TimeStamps.resize( N );
    scopeObj.numObject.strP4TimeStamps.resize( N );
    // resize sub-vectors to COUNT elements
//    scopeObj.numObject.p1Vec.resize( scopeObj.numObject.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::P1 ].COUNT );
//    scopeObj.numObject.p2Vec.resize( scopeObj.numObject.pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::P2 ].COUNT );
}
//______________________________________________________________________________
std::vector< std::vector< double > > scopeInterface::getScopeTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }

    return scopeObj.traceObjects.at( name ).traceData.at( pvType );
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getMinOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    /// For the WCM and ED-FCUP we need the first minimum of the trace
    std::vector< double > minElements;
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }

    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        minElements.push_back( (*std::min_element( it.begin(), it.end() ) ) );
    }

    return minElements;
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getMaxOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    /// For spectrometer FCUP we need the peak-to-peak voltage
    std::vector< double > maxElements;
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }

    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        maxElements.push_back( (*std::max_element( it.begin(), it.end() ) ) );
    }

    return maxElements;
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getAreaUnderTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    /// For now this just uses the rectangle rule to calculate the area under a scope trace -
    /// there is probably a better way to do this.
    /// Currently we only need to integrate the ICT traces
    std::vector< double > areaElements;
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }
    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum_of_elems = 0.0;
        std::vector< double > curveAreas;
        for( auto && it2 : it )
        {
//            std::cout << it2 << std::endl;
            double curveElement = ( it2 * scopeObj.traceObjects.at( name ).timebase ) / scopeObj.traceObjects.at( name ).pvMonStructs[ scopeStructs::SCOPE_PV_TYPE::TR1 ].COUNT;
            curveAreas.push_back( curveElement );
        }
        sum_of_elems = std::accumulate( curveAreas.begin(), curveAreas.end(), 0.0 );
        areaElements.push_back( sum_of_elems );
    }

    return areaElements;
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }
    return scopeObj.traceObjects.at( name ).timeStamps.at( pvType );
}
//______________________________________________________________________________
std::vector< std::string > scopeInterface::getStrTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }
    return scopeObj.traceObjects.at( name ).strTimeStamps.at( pvType );
}
//______________________________________________________________________________
double scopeInterface::getScopeP1( const std::string & name )
{
    return scopeObj.numObject.p1;
}
//______________________________________________________________________________
double scopeInterface::getScopeP2( const std::string & name )
{
    return scopeObj.numObject.p2;
}
//______________________________________________________________________________
double scopeInterface::getScopeP3( const std::string & name )
{
    return scopeObj.numObject.p3;
}
//______________________________________________________________________________
double scopeInterface::getScopeP4( const std::string & name )
{
    return scopeObj.numObject.p4;
}
//______________________________________________________________________________
void scopeInterface::killCallBack( scopeStructs::monitorStruct * ms, scopeStructs::scopeTraceData * td )
{
    int status = ca_clear_subscription( *ms -> EVID );
    if( status == ECA_NORMAL)
    {
//        debugMessage( ms -> scopeObject, " monitoring = false ");
        td->isMonitoring = false;

//        isMonitoringMap[ ms -> scopeObject ] = false;
//        monitoringTraces = false;
        delete ms;
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->scopeObject );
    }
}
//______________________________________________________________________________
bool scopeInterface::isATracePV( scopeStructs::SCOPE_PV_TYPE pv )
{
    bool ret = false;
    if( pv == scopeStructs::SCOPE_PV_TYPE::TR1 )
        ret = true;
    else if( pv == scopeStructs::SCOPE_PV_TYPE::TR2 )
        ret = true;
    else if( pv == scopeStructs::SCOPE_PV_TYPE::TR3 )
        ret = true;
    else if( pv == scopeStructs::SCOPE_PV_TYPE::TR4 )
        ret = true;
    return ret;
}
//______________________________________________________________________________
bool scopeInterface::isANumPV( scopeStructs::SCOPE_PV_TYPE pv )
{
    bool ret = false;
    if( pv == scopeStructs::SCOPE_PV_TYPE::P1 )
        ret = true;
    else if( pv == scopeStructs::SCOPE_PV_TYPE::P2 )
        ret = true;
    return ret;
}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE scopeInterface::getScopeState( const  std::string & objName )
//{
//    VELA_ENUM::TRIG_STATE r =  VELA_ENUM::TRIG_STATE::TRIG_ERROR;
//    auto iter = allScopeData.find( objName );
//    if (iter != allScopeData.end() )
//        r = iter -> second.scopeState;
//    return r;
//}
//______________________________________________________________________________
std::vector< double > scopeInterface::getAvgNoise( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType )
{
    /// Here we take the mean of a part of the scope trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the scope hasn't been set up properly for taking this measurement
    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }
    std::vector< double > noise, noiseTotal;
    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum = std::accumulate( it.end()-int(200), it.end(), 0.0 );
        noise.push_back( sum / 200.0 );
        noiseTotal.push_back( noise.back() * scopeObj.traceObjects.at( name ).timebase );
    }
//    std::cout << noiseTotal << std::endl;
    return noiseTotal;
}
//______________________________________________________________________________
double scopeInterface::getWCMQ()
{
    double wcmQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::WCM )
            {
                minVal = getMinOfTraces( it.first, it2.second.pvType ).back();
                wcmQ   = minVal * -250;
            }
        }
    }

    if( wcmQ == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND WCM AMONG pvMonStructs, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return wcmQ;
}
//______________________________________________________________________________
double scopeInterface::getICT1Q()
{
    double ict1q = UTL::DUMMY_DOUBLE;
    double traceArea, noise;
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::ICT1 )
            {
                traceArea = getAreaUnderTraces( it.first, it2.second.pvType ).back();
                noise = getAvgNoise( it.second.name, it2.second.pvType ).back();
                ict1q = ( noise - traceArea ) * 0.8;
            }
        }
    }
    if( ict1q == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND ICT1 AMONG pvMonStructs, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return ( ict1q * pow( 10, 12 ) );
}
//______________________________________________________________________________
double scopeInterface::getICT2Q()
{
    double ict2q = UTL::DUMMY_DOUBLE;
    double traceArea, noise;
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::ICT2 )
            {
                traceArea = getAreaUnderTraces( it.second.name, it2.second.pvType ).back();
                noise = getAvgNoise( it.second.name, it2.second.pvType ).back();
                ict2q = ( noise - traceArea ) * 0.8;
            }
        }
    }
    if( ict2q == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND ICT2 AMONG pvMonStructs, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return ( ict2q * pow( 10, 12 ) );
}
//______________________________________________________________________________
double scopeInterface::getFCUPQ()
{
    double fcupQ = UTL::DUMMY_DOUBLE;
    for( auto && it : scopeObj.numObject.pvMonStructs )
    {
        if( it.first == scopeStructs::SCOPE_PV_TYPE::P1 && it.second.diagType == VELA_ENUM::DIAG_TYPE::FCUP )
        {
            fcupQ = scopeObj.numObject.p1;
        }
        else if( it.first == scopeStructs::SCOPE_PV_TYPE::P2 && it.second.diagType == VELA_ENUM::DIAG_TYPE::FCUP )
        {
            fcupQ = scopeObj.numObject.p2;
        }
        else if( it.first == scopeStructs::SCOPE_PV_TYPE::P3 && it.second.diagType == VELA_ENUM::DIAG_TYPE::FCUP )
        {
            fcupQ = scopeObj.numObject.p3;
        }
        else if( it.first == scopeStructs::SCOPE_PV_TYPE::P4 && it.second.diagType == VELA_ENUM::DIAG_TYPE::FCUP )
        {
            fcupQ = scopeObj.numObject.p4;
        }
    }
    if( fcupQ == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND FCUP AMONG pvMonStructs, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return fcupQ;
}
//______________________________________________________________________________
double scopeInterface::getEDFCUPQ()
{
    double edfcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::ED_FCUP )
            {
                minVal  = getMinOfTraces( it.first, it2.second.pvType ).back();
                edfcupQ = minVal * -100;
            }
        }
    }
    if( edfcupQ == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND ED-FCUP AMONG pvMonStructs, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return edfcupQ;
}
//______________________________________________________________________________
std::vector< std::string > scopeInterface::getScopeNames()
{
    std::vector< std::string > scopeNames;
    scopeNames.push_back( scopeObj.numObject.name );
    for( auto && iter : scopeObj.traceObjects )
    {
        message( "Scope Name ", iter.first );
        scopeNames.push_back( iter.first );
    }

    return scopeNames;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > scopeInterface::getILockStates( const std::string & objName )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
    auto iter = allScopeData.find( objName );
    if( iter != allScopeData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  scopeInterface::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allScopeData.find( name );
    if( iter != allScopeData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
//______________________________________________________________________________
bool scopeInterface::toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, scopeInterface & obj )
{
    if( CALL_MEMBER_FN(obj, f1)( name ) ) /// CALL_MEMBER_FN MACRO in structs.h
    {
        CALL_MEMBER_FN(obj, f3)( name ) ;

        time_t timeStart = time( 0 );  /// Start Clock

        while( CALL_MEMBER_FN(obj, f1)( name ) )
        {
            std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); /// MAGIC NUMBER
            time_t currentTime = time( 0 );
            time_t timeDif = currentTime - timeStart;
            if( timeDif > waitTime )
            {
                message( "Toggle ", name, " timed out after ", waitTime," seconds");
                break;
            }
        }
    }
    /// Prevously we have still needed a sleep here, they shouldn't be needed, THIS NEEDS CHECKING
// std::this_thread::sleep_for(std::chrono::milliseconds( 20 ));
    return CALL_MEMBER_FN(obj, f2)( name );
}
