//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //

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

//______________________________________________________________________________
scopeInterface::scopeInterface( const std::string & configFileLocation1, const std::string & configFileLocation2, const bool* show_messages_ptr,
                                                            const bool * show_debug_messages_ptr,   const bool shouldStartEPICS,
                                                            const bool startVirtualMachine, const VELA_ENUM::MACHINE_AREA myMachineArea ):
configReader( configFileLocation1, configFileLocation2, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine),
interface( show_messages_ptr, show_debug_messages_ptr ),
shouldStartEPICS( shouldStartEPICS ),
startVM( startVirtualMachine ),
machineArea( myMachineArea )
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
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initScopeObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICS )
            {
                std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initScopeChids();
                /// start the monitors: set up the callback functions
                monitorScopes();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); // MAGIC_NUMBER
            }
            else
                message("The scopeInterface Read Config files, Not Starting EPICS Monitors" );
        }
        else
            message( "!!!The scopeInterface received an Error while getting scope data!!!" );
    }
}
//______________________________________________________________________________
bool scopeInterface::initScopeObjects()
{
    scopeObj = configReader.getScopeObject();
    return true;
}
//______________________________________________________________________________
void scopeInterface::initScopeChids()
{
    message( "\n", "Searching for scope chids..." );
    // trace objects
    for( auto && it1 : scopeObj.traceObjects )
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );
    // num objects
    for( auto && it1 : scopeObj.numObjects )
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );
    // send
    int status = sendToEpics( "ca_create_channel", "Found scope chids.", "!!TIMEOUT!! Not all scope ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBERR
        for( auto && it1 : scopeObj.traceObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        for( auto && it1 : scopeObj.numObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
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
                it1.second.isAContinuousMonitorStruct = false;
                it1.second.isATemporaryMonitorStruct = false;
//                if( it1.second.isAContinuousMonitorStruct )
//                {
//                    resetTraceVectors( 1 );
//                    addToTraceMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );
//                }
//            }
        }
    }

//
    for( auto && it1 : scopeObj.numObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
//            if( !isADataPV( it2.second.pvType ) )
//            {   //NOT SURE ABOUT THIS "IF" -----> I WANT TO SET THIS BOOL TO TRUE EVEN IF IT IS A DATA PV
                it1.second.isAContinuousMonitorStruct = true;
                it1.second.isATemporaryMonitorStruct = false;
//                if( it1.second.isAContinuousMonitorStruct )
//                {
//                    resetNumVectors( 1 );
//                    addToNumMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );
//                }
//            }
        }
    }

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to scope Monitors", "!!TIMEOUT!! Subscription to scope Monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
void scopeInterface::addToTraceMonitorStructs( std::vector< scopeStructs::monitorStruct * > & msv, scopeStructs::pvStruct & pv,  scopeStructs::scopeTraceData * traceObj   )
{
    msv.push_back( new scopeStructs::monitorStruct() );
    msv.back() -> monType      = pv.pvType;
    msv.back() -> scopeObject  = &scopeObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;
//    msv.back() -> EVID         = &pv.EVID;

    switch( pv.pvType )
    {
        case scopeStructs::SCOPE_PV_TYPE::TR1:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::TR2:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::TR3:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::TR4:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  scopeInterface::staticEntryrMonitor, (void*)msv.back(), &msv.back() -> EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void scopeInterface::addToNumMonitorStructs( std::vector< scopeStructs::monitorStruct * > & msv, scopeStructs::pvStruct & pv,  scopeStructs::scopeNumObject * numObj   )
{
    msv.push_back( new scopeStructs::monitorStruct() );
    msv.back() -> monType      = pv.pvType;
    msv.back() -> scopeObject  = &scopeObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;
//    msv.back() -> EVID         = &pv.EVID;

    switch( pv.pvType )
    {
        case scopeStructs::SCOPE_PV_TYPE::P1:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P2:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P3:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        case scopeStructs::SCOPE_PV_TYPE::P4:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  scopeInterface::staticEntryrMonitor, (void*)msv.back(), &msv.back() -> EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void scopeInterface::staticEntryrMonitor( const event_handler_args args )
{
    scopeStructs::monitorStruct * ms = static_cast< scopeStructs::monitorStruct *> ( args.usr );
    std::cout<<ENUM_TO_STRING(ms->monType)<<std::endl;
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
    td->isMonitoringMap.at( ms -> monType ) = true;
    if( td->isAContinuousMonitorStruct)
    {
        for( auto && it1 : scopeObj.traceObjects )
        {
            it1.second.numShots = 1;
//            for( auto && it2 : it1.second.shotCounts )
//            {
//                it2.second = 0;
//            }
            it1.second.shotCounts.at( ms -> monType ) = 0;
        }
    }
    const dbr_double_t * value = &(p  -> value);
    size_t i =1;
    int zero = 0;

    updateTime( p->stamp, td->timeStamps.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ],
               td->strTimeStamps.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ]  );

    /// resizes the trace vectors dynamically in case the trace being sent to EPICS from the scope changes size
    /// - limited by the array size set in EPICS database (currently (6/4/17) 2000 points)
    td->traceData.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ].resize(static_cast< int >(*( &p->value ) ) );

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
            message( "Collected ", td->shotCounts.at( ms -> monType ), " shots for ", td -> pvRoot, ":", ENUM_TO_STRING( ms->monType ) );
            td->isMonitoringMap.at( ms->monType ) = false;
            killTraceCallBack( ms );
        }
    }
}
//______________________________________________________________________________
void scopeInterface::updateValue( scopeStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    scopeStructs::scopeNumObject * scno = reinterpret_cast< scopeStructs::scopeNumObject* > (ms -> val);

    scno->isMonitoringMap.at( ms -> monType ) = true;

    if( scno->isAContinuousMonitorStruct)
    {
        for( auto && it1 : scopeObj.numObjects )
        {
            it1.second.numShots = 1;
            for( auto && it2 : it1.second.shotCounts )
            {
                it2.second = 0;
            }
        }
    }

    const dbr_double_t * val = &(p  -> value);
    size_t i = 0;
    updateTime( p->stamp, scno->numTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ],
               scno->numStrTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ]  );

    scno->numData.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ] = *( &p -> value );

    if( scno -> isATemporaryMonitorStruct )
    {
        if( scno -> numShots > -1 )
        {
            ++scno -> shotCounts.at( ms -> monType );
        }
        if( scno->shotCounts.at( ms -> monType ) == scno->numShots )
        {
            message( "Collected ", scno->shotCounts.at( ms -> monType ), " shots for ", scno -> pvRoot, ":", ENUM_TO_STRING( ms->monType ) );
            scno->isMonitoringMap.at( ms->monType ) = false;
            ms->interface->killNumCallBack( ms );
        }
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
//                    monitoringTraces = true;

                    addToTraceMonitorStructs( traceMonitorStructs, it2.second, &it1.second  );
                }
            }
        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to scope Trace Monitors failed" );
//        if ( status == ECA_NORMAL )
//            for( auto && it : scopeObj.traceObjects )
//                for( auto && it1: it.second.isMonitoringMap )
//                    it1.second = true;
//            monitoringTraces = true; /// interface base class member
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void scopeInterface::monitorATraceForNShots( const std::string trace, scopeStructs::SCOPE_PV_TYPE channel, size_t N )
{
    if( !isMonitoringScopeTrace( trace, channel ) )
    {
        traceMonitorStructs.clear();
        debugMessage( "Starting scope Traces Monitor for ", channel );
//        resetATraceVector( trace, channel, N );
        resetTraceVectors( N );
        debugMessage( "Vector ", channel, " Reset" );
        scopeObj.traceObjects.at( trace );
        scopeObj.traceObjects.at( trace ).numShots = N;
        scopeObj.traceObjects.at( trace ).isAContinuousMonitorStruct=false;
        scopeObj.traceObjects.at( trace ).isATemporaryMonitorStruct=true;
        scopeObj.traceObjects.at( trace ).isMonitoringMap.at( channel ) = true;

        addToTraceMonitorStructs( traceMonitorStructs, scopeObj.traceObjects.at( trace ).pvMonStructs.at( channel ), &scopeObj.traceObjects.at( trace ) );
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to scope Trace Monitors failed" );
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
void scopeInterface::resetATraceVector( const std::string scopeName, scopeStructs::SCOPE_PV_TYPE channel, size_t N )
{
    traceMonitorStructs.clear();
    scopeObj.traceObjects.at( scopeName ).traceData.at( channel ).clear();
    scopeObj.traceObjects.at( scopeName ).traceData.at( channel ).resize( N );
    for( auto && it3 : scopeObj.traceObjects.at( scopeName ).traceData.at( channel ) )
    {
        it3.clear();
        it3.resize( scopeObj.traceObjects.at( scopeName ).pvMonStructs[ channel ].COUNT );
    }
    scopeObj.traceObjects.at( scopeName ).timeStamps.at( channel ).clear();
    scopeObj.traceObjects.at( scopeName ).timeStamps.at( channel ).resize( N );
    scopeObj.traceObjects.at( scopeName ).strTimeStamps.at( channel ).clear();
    scopeObj.traceObjects.at( scopeName ).strTimeStamps.at( channel ).resize( N );
    scopeObj.traceObjects.at( scopeName ).shotCounts.at( channel ) = 0;
}
//______________________________________________________________________________
void scopeInterface::monitorNumsForNShots( size_t N )
{
    if( !monitoringNums )
    {
        numMonitorStructs.clear();
        debugMessage( "Starting scope Nums Monitor " );
        resetNumVectors( N );
        debugMessage( "Vectors Reset" );
        for( auto && it1 : scopeObj.numObjects )
        {
            it1.second.numShots = N;
            it1.second.isAContinuousMonitorStruct=false;
            it1.second.isATemporaryMonitorStruct=true;
            for( auto && it2 : it1.second.pvMonStructs )
            {
                if( isANumPV( it2.first ) )
                {
                    addToNumMonitorStructs( traceMonitorStructs, it2.second, &it1.second  );
                }
            }
        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to scope Num Monitors failed" );
        if ( status == ECA_NORMAL )
            for( auto && it : scopeObj.numObjects )
                for( auto && it1: it.second.isMonitoringMap )
                    it1.second = true;
//            monitoringNums = true; /// interface base class member
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
    for( auto && it : scopeObj.numObjects )
    {   /// Clear all trace data + timestamps
        /// Resize to N shots
        for( auto && it2 : it.second.numData )
        {

            it2.second.clear();
            it2.second.resize( N );
        }
        for( auto && it3 : it.second.numTimeStamps )
        {
            it3.second.clear();
            it3.second.resize( N );
        }
        for( auto && it4 : it.second.numStrTimeStamps )
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
bool scopeInterface::isMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    if( scopeObj.traceObjects.at( scopeName ).isMonitoringMap.at( pvType ) == true )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool scopeInterface::isNotMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return !isMonitoringScopeTrace( scopeName, pvType );
}
//______________________________________________________________________________
bool scopeInterface::isMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    if( scopeObj.numObjects.at( scopeName ).isMonitoringMap.at( pvType ) == true )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool scopeInterface::isNotMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return !isMonitoringScopeNum( scopeName, pvType );
}
//______________________________________________________________________________
const scopeStructs::scopeTraceData & scopeInterface::getScopeTraceDataStruct( const std::string & scopeName )
{
    if( entryExists( scopeObj.traceObjects, scopeName ) && scopeObj.traceObjects.at( scopeName ).traceData.size() != 0 )
    {
        return scopeObj.traceObjects.at( scopeName );
    }
}
//______________________________________________________________________________
const scopeStructs::scopeNumObject & scopeInterface::getScopeNumDataStruct( const std::string & scopeName )
{
    if( entryExists( scopeObj.numObjects, scopeName ) && scopeObj.numObjects.at( scopeName ).numData.size() != 0 )
    {
        return scopeObj.numObjects.at( scopeName );
    }
}
//______________________________________________________________________________
std::vector< std::vector< double > > scopeInterface::getScopeTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
//    while( scopeObj.traceObjects.at( name ).isMonitoringMap.at( pvType ) )
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
//    }

    return scopeObj.traceObjects.at( name ).traceData.at( pvType );
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getScopeNums( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return scopeObj.numObjects.at( name ).numData.at( pvType );
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getMinOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    /// For the WCM and ED-FCUP we need the first minimum of the trace
    std::vector< double > minElements;

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

    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum_of_elems = 0.0;
        std::vector< double > curveAreas;
        for( auto && it2 : it )
        {
//            std::cout << it2 << std::endl;
            double curveElement = ( it2 * scopeObj.traceObjects.at( name ).timebase );// / it.size();
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
    return scopeObj.traceObjects.at( name ).timeStamps.at( pvType );
}
//______________________________________________________________________________
std::vector< std::string > scopeInterface::getStrTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType )
{
    return scopeObj.traceObjects.at( name ).strTimeStamps.at( pvType );
}
//______________________________________________________________________________
double scopeInterface::getScopeP1( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P1 ).back();
}
//______________________________________________________________________________
double scopeInterface::getScopeP2( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P1 ).back();
}
//______________________________________________________________________________
double scopeInterface::getScopeP3( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P1 ).back();
}
//______________________________________________________________________________
double scopeInterface::getScopeP4( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P1 ).back();
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getScopeP1Vec( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P1 );
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getScopeP2Vec( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P2 );
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getScopeP3Vec( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P3 );
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getScopeP4Vec( const std::string & name )
{
    return scopeObj.numObjects.at( name ).numData.at( scopeStructs::SCOPE_PV_TYPE::P4 );
}
//______________________________________________________________________________
void scopeInterface::killTraceCallBack( scopeStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        delete ms;
        debugMessage("killed callback to ",ENUM_TO_STRING(ms->monType));
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->scopeObject );
    }
}
//______________________________________________________________________________
void scopeInterface::killNumCallBack( scopeStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        delete ms;
        debugMessage("killed callback to ",ENUM_TO_STRING(ms->monType));
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
    else if( pv == scopeStructs::SCOPE_PV_TYPE::P3 )
        ret = true;
    else if( pv == scopeStructs::SCOPE_PV_TYPE::P4 )
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
std::vector< double > scopeInterface::getAvgNoise( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 )
{
    /// Here we take the mean of a part of the scope trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the scope hasn't been set up properly for taking this measurement
    std::vector< double > noise, noiseTotal;
    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum = std::accumulate( it.begin()+part1, it.begin()+part2, 0.0 );
        noise.push_back( sum / ( part2 - part1 ) );
        noiseTotal.push_back( noise.back());// * scopeObj.traceObjects.at( name ).timebase );
    }
//    std::cout << noiseTotal << std::endl;
    return noiseTotal;
}
//______________________________________________________________________________
std::vector< std::vector< double > > scopeInterface::getPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 )
{
    /// Here we take the mean of a part of the scope trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the scope hasn't been set up properly for taking this measurement

    std::vector< std::vector< double > > vecPart( scopeObj.traceObjects.at( name ).numShots );
    int i = 0;
    for( auto && it : scopeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        std::vector< double >::const_iterator vec1 = it.begin() + part1;
        std::vector< double >::const_iterator vec2 = it.begin() + part2;
        std::vector< double > newVec( vec1, vec2 );
        vecPart.at( i ).insert( vecPart.at( i ).begin(), newVec.begin(), newVec.end() );
        i++;
    }

    return vecPart;
}
//______________________________________________________________________________
std::vector< double > scopeInterface::getAreaUnderPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 )
{
    double sum_of_elems;
    std::vector< double > areaElements;
    std::vector< std::vector< double > > vecPart = getPartOfTrace( name, pvType, part1, part2 );

    for( auto && it : vecPart )
    {
        double sum_of_elems = 0.0;
        std::vector< double > curveAreas;
        for( auto && it2 : it )
        {
            double curveElement = ( it2 * scopeObj.traceObjects.at( name ).timebase );// / it.size();
            curveAreas.push_back( curveElement );
        }
        sum_of_elems = std::accumulate( curveAreas.begin(), curveAreas.end(), 0.0 );
        areaElements.push_back( sum_of_elems );
    }

    return areaElements;
}
//______________________________________________________________________________
double scopeInterface::getWCMQ()
{
    double wcmQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : scopeObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::WCM && it2.second.scopeType == scopeStructs::SCOPE_TYPE::NUM )
            {
                minVal = getScopeNums( it.first, it2.second.pvType ).back();
                wcmQ   = minVal * -250;
            }
        }
    }
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::WCM && it2.second.scopeType == scopeStructs::SCOPE_TYPE::ARRAY )
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
double scopeInterface::getICT1Q( const int part1, const int part2 )
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
                noise = getAvgNoise( it.second.name, it2.second.pvType, part1, part2 ).back();
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
double scopeInterface::getICT2Q( const int part1, const int part2 )
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
                noise = getAvgNoise( it.second.name, it2.second.pvType, part1, part2 ).back();
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
    double minVal;
    for( auto && it : scopeObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::FCUP && it2.second.scopeType == scopeStructs::SCOPE_TYPE::NUM )
            {
                minVal = getScopeNums( it.first, it2.second.pvType ).back();
                fcupQ   = minVal * -250;
            }
        }
    }
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::FCUP && it2.second.scopeType == scopeStructs::SCOPE_TYPE::ARRAY )
            {
                minVal = getMinOfTraces( it.first, it2.second.pvType ).back();
                fcupQ   = minVal * -250;
            }
        }
    }
    if( fcupQ == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND FCUP AMONG pvMonStructs, IS IT DEFINED IN THE CONFIG FILE????");
    }

    message("WARNING!!!! THIS IS NOT THE FCUP CHARGE - IT NEEDS TO BE CALIBRATED");
    return fcupQ;
}
//______________________________________________________________________________
double scopeInterface::getEDFCUPQ()
{
    double edfcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : scopeObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::ED_FCUP && it2.second.scopeType == scopeStructs::SCOPE_TYPE::NUM )
            {
                minVal = getScopeNums( it.first, it2.second.pvType ).back();
                edfcupQ   = minVal * -100;
            }
        }
    }
    for( auto && it : scopeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == VELA_ENUM::DIAG_TYPE::ED_FCUP && it2.second.scopeType == scopeStructs::SCOPE_TYPE::ARRAY )
            {
                minVal = getMinOfTraces( it.first, it2.second.pvType ).back();
                edfcupQ   = minVal * -100;
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
    for( auto && iter : scopeObj.traceObjects )
    {
        message( "Scope Name ", iter.first );
        scopeNames.push_back( iter.first );
    }
    for( auto && iter : scopeObj.numObjects )
    {
        message( "Scope Name ", iter.first );
        scopeNames.push_back( iter.first );
    }

    return scopeNames;
}
//______________________________________________________________________________
std::vector< std::string > scopeInterface::getScopePVs()
{
    std::vector< std::string > scopeNames;
    for( auto && iter : scopeObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "Scope PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            scopeNames.push_back( s.str() );
        }
    }
    for( auto && iter : scopeObj.numObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "Scope PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            scopeNames.push_back( s.str() );
        }
    }

    return scopeNames;
}
//______________________________________________________________________________
std::vector< std::string > scopeInterface::getScopeTracePVs()
{
    std::vector< std::string > scopeNames;
    for( auto && iter : scopeObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "Scope PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            scopeNames.push_back( s.str() );
        }
    }

    return scopeNames;
}
//______________________________________________________________________________
std::vector< std::string > scopeInterface::getScopeNumPVs()
{
    std::vector< std::string > scopeNames;
    for( auto && iter : scopeObj.numObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "Scope PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            scopeNames.push_back( s.str() );
        }
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
    return CALL_MEMBER_FN(obj, f2)( name );
}
