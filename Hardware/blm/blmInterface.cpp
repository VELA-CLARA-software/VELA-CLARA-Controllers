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

#include "blmInterface.h"
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
blmInterface::blmInterface( const std::string & configFileLocation1,
                                const bool& show_messages_ptr,
                                const bool& show_debug_messages_ptr,
                                const bool shouldStartEPICs,
                                const bool startVirtualMachine,
                                const HWC_ENUM::MACHINE_AREA myMachineArea ):
configReader( configFileLocation1, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine),
interface( show_messages_ptr, show_debug_messages_ptr, shouldStartEPICs ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
blmInterface::~blmInterface()
{
    for( auto it : continuousMonitorStructs )
    {
        killNumCallBack( it );
        debugMessage("delete blmInterface continuousMonitorStructs entry.");
        delete it;
    }
    debugMessage("blmInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void blmInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfigFiles();
    pause_2000();
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initBLMObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                /// subscribe to the channel ids
                initBLMChids();
                /// start the monitors: set up the callback functions
                monitorBLMs();
                /// The pause allows EPICS to catch up.
                pause_500();
            }
            else
                message("The blmInterface Read Config files, Not Starting EPICS Monitors" );
        }
        else
            message( "!!!The blmInterface received an Error while getting blm data!!!" );
    }
}
//______________________________________________________________________________
bool blmInterface::initBLMObjects()
{
    blmObj = configReader.getBLMObject();
    return true;
}
//______________________________________________________________________________
void blmInterface::initBLMChids()
{
    message( "\n", "Searching for blm chids..." );
    // trace objects
    for( auto && it1 : blmObj.traceObjects )
        for( auto && it2 : it1.second.pvMonStructs )
        {
            addChannel( it1.second.pvRoot, it2.second );
        }

    // num objects
    for( auto && it1 : blmObj.numObjects )
        for( auto && it2 : it1.second.pvMonStructs )
        {
            addChannel( it1.second.pvRoot, it2.second );
        }

    // send
    int status = sendToEpics( "ca_create_channel", "Found blm chids.", "!!TIMEOUT!! Not all blm ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        UTL::PAUSE_500;
        for( auto && it1 : blmObj.traceObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        for( auto && it1 : blmObj.numObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        message("");
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void blmInterface::addChannel( const std::string & pvRoot, blmStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << ":" << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void blmInterface::monitorBLMs()
{
    continuousMonitorStructs.clear();
    for( auto && it1 : blmObj.traceObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
            {
                continuousTraceMonitorStructs.push_back(new blmStructs::monitorStruct());
                continuousTraceMonitorStructs.back()->monType = it2.first;
                continuousTraceMonitorStructs.back()->objName = it1.second.name;
                continuousTraceMonitorStructs.back()->interface = this;
                continuousTraceMonitorStructs.back()->val = &it1.second;
                continuousTraceMonitorStructs.back()->diagType = it2.second.diagType;
                it1.second.isAContinuousMonitorStruct = true;
                it1.second.isATemporaryMonitorStruct = false;
                ca_create_subscription(it2.second.CHTYPE,
                                       it2.second.COUNT,
                                       it2.second.CHID,
                                       it2.second.MASK,
                                       blmInterface::staticEntryrMonitor,
                                       (void*)continuousTraceMonitorStructs.back(),
                                       &continuousTraceMonitorStructs.back()->EVID);
                debugMessage("Adding monitor for ",it1.second.name, " ",ENUM_TO_STRING(it2.first));
            }
        }
    }
    for( auto && it1 : blmObj.numObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
            {
                continuousNumMonitorStructs.push_back(new blmStructs::monitorStruct());
                continuousNumMonitorStructs.back()->monType = it2.first;
                continuousNumMonitorStructs.back()->objName = it1.second.name;
                continuousNumMonitorStructs.back()->interface = this;
                continuousNumMonitorStructs.back()->val = &it1.second;
                continuousNumMonitorStructs.back()->diagType = it2.second.diagType;
                it1.second.isAContinuousMonitorStruct = true;
                it1.second.isATemporaryMonitorStruct = false;
                ca_create_subscription(it2.second.CHTYPE,
                                       it2.second.COUNT,
                                       it2.second.CHID,
                                       it2.second.MASK,
                                       blmInterface::staticEntryrMonitor,
                                       (void*)continuousNumMonitorStructs.back(),
                                       &continuousNumMonitorStructs.back()->EVID);
                debugMessage("Adding monitor for ",it1.second.name, " ",ENUM_TO_STRING(it2.first));
            }
        }
    }

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to blm Monitors", "!!TIMEOUT!! Subscription to blm Monitors failed" );
    if ( status == ECA_NORMAL )
    {
        allMonitorsStarted = true; /// interface base class member
    }
}
//______________________________________________________________________________
void blmInterface::addToTraceMonitorStructs( std::vector< blmStructs::monitorStruct * > & msv, blmStructs::pvStruct & pv,  blmStructs::blmTraceData * traceObj   )
{
    msv.push_back( new blmStructs::monitorStruct() );
    msv.back() -> monType      = pv.pvType;
    msv.back() -> blmObject  = &blmObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;
//    msv.back() -> EVID         = &pv.EVID;

    switch( pv.pvType )
    {
        case blmStructs::SCOPE_PV_TYPE::TR1:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::SCOPE_PV_TYPE::TR2:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::SCOPE_PV_TYPE::TR3:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::SCOPE_PV_TYPE::TR4:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  blmInterface::staticEntryrMonitor, (void*)msv.back(), &msv.back() -> EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void blmInterface::addToNumMonitorStructs( std::vector< blmStructs::monitorStruct * > & msv, blmStructs::pvStruct & pv,  blmStructs::blmNumObject * numObj   )
{
    msv.push_back( new blmStructs::monitorStruct() );
    msv.back() -> monType      = pv.pvType;
    msv.back() -> blmObject  = &blmObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;
//    msv.back() -> EVID         = &pv.EVID;

    switch( pv.pvType )
    {
        case blmStructs::SCOPE_PV_TYPE::P1:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        case blmStructs::SCOPE_PV_TYPE::P2:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        case blmStructs::SCOPE_PV_TYPE::P3:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        case blmStructs::SCOPE_PV_TYPE::P4:
        {
            msv.back() -> val = (void*)numObj;
            msv.back() -> objName = numObj -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  blmInterface::staticEntryrMonitor, (void*)msv.back(), &msv.back() -> EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void blmInterface::staticEntryrMonitor( const event_handler_args args )
{
    blmStructs::monitorStruct * ms = static_cast< blmStructs::monitorStruct *> ( args.usr );
    switch( ms -> monType )
    {
        case blmStructs::SCOPE_PV_TYPE::TR1:
            {
                ms->interface->updateTrace( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::TR2:
            {
                ms->interface->updateTrace( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::TR3:
            {
                ms->interface->updateTrace( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::TR4:
            {
                ms->interface->updateTrace( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P1:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P2:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P3:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P4:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
    }
}
//______________________________________________________________________________
void blmInterface::updateTrace( blmStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    blmStructs::blmTraceData * td = reinterpret_cast< blmStructs::blmTraceData *> (ms -> val);

    if( td->isAContinuousMonitorStruct )
    {
        td->shotCounts.at( ms -> monType ) = 0;
        td->diagType = ms -> diagType;
        if( td->timeStamps.at(ms->monType).size() == 0 )
        {
            td->timeStamps.at(ms->monType).push_back(1);
            td->strTimeStamps.at(ms->monType).push_back(UTL::UNKNOWN_STRING);
            td->traceData.at(ms->monType).resize(1);
            td->traceDataBuffer.at(ms->monType).resize(td->buffer);
        }
    }
    const dbr_double_t * value = &(p  -> value);
    size_t i =1;
    int zero = 0;

    updateTime( p->stamp, td->timeStamps.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ],
               td->strTimeStamps.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ]  );

    /// resizes the trace vectors dynamically in case the trace being sent to EPICS from the blm changes size
    /// - limited by the array size set in EPICS database (currently (6/4/17) 2000 points)
    td->traceData.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ].resize(static_cast< int >(*( &p->value ) ) );

    for( auto && it : td->traceData.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ] )
    {
        it = *( &p->value + i );
        ++i;
    }
    td->traceDataBuffer.at( ms -> monType ).push_back(td->traceData.at( ms -> monType ).back());

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
void blmInterface::updateValue( blmStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    blmStructs::blmNumObject * scno = reinterpret_cast< blmStructs::blmNumObject* > (ms -> val);

//    scno->isMonitoringMap.at( ms -> monType ) = true;

    if( scno->isAContinuousMonitorStruct )
    {
        scno->shotCounts.at( ms -> monType ) = 0;
        scno->diagType = ms -> diagType;
        if( scno->numTimeStamps.at(ms->monType).size() == 0 )
        {
            scno->numTimeStamps.at(ms->monType).push_back(1);
            scno->numStrTimeStamps.at(ms->monType).push_back(UTL::UNKNOWN_STRING);
            scno->numData.at(ms->monType).push_back(UTL::DUMMY_DOUBLE);
            scno->numDataBuffer.at(ms->monType).resize(scno->buffer);
        }
    }

    const dbr_double_t * val = &(p  -> value);
    size_t i = 0;

    updateTime( p->stamp, scno->numTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ],
               scno->numStrTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ]  );
    switch( ms -> monType )
    {
        case blmStructs::SCOPE_PV_TYPE::P1:
            {
                scno->p1 = *( &p -> value );
                scno->p1TimeStamp = scno->numTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ];
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P2:
            {
                scno->p2 = *( &p -> value );
                scno->p2TimeStamp = scno->numTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ];
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P3:
            {
                scno->p3 = *( &p -> value );
                scno->p3TimeStamp = scno->numTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ];
                break;
            }
        case blmStructs::SCOPE_PV_TYPE::P4:
            {
                scno->p4 = *( &p -> value );
                scno->p4TimeStamp = scno->numTimeStamps.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ];
                break;
            }
    }
    scno->numData.at( ms -> monType )[ scno->shotCounts.at( ms -> monType ) ] = *( &p -> value );
    scno->numDataBuffer.at( ms -> monType ).push_back(*( &p -> value ));

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
void blmInterface::clearContinuousMonitorStructs()
{
    if( continuousMonitorStructs.size() != 0 )
    {
        for( auto && it : continuousMonitorStructs )
        {
            killNumCallBack(it);

        }
        continuousMonitorStructs.clear();
    }
}
//______________________________________________________________________________
void blmInterface::clearContinuousTraceMonitorStructs()
{
    if( continuousTraceMonitorStructs.size() != 0 )
    {
        for( auto && it : continuousTraceMonitorStructs )
        {
            killNumCallBack(it);

        }
        continuousTraceMonitorStructs.clear();
    }
}
//______________________________________________________________________________
void blmInterface::clearContinuousNumMonitorStructs()
{
    if( continuousNumMonitorStructs.size() != 0 )
    {
        for( auto && it : continuousNumMonitorStructs )
        {
            killNumCallBack(it);

        }
        continuousNumMonitorStructs.clear();
    }
}
//______________________________________________________________________________
void blmInterface::monitorTracesForNShots( size_t N )
{
    if( !monitoringTraces )
    {
        traceMonitorStructs.clear();
        clearContinuousTraceMonitorStructs();
        debugMessage( "Starting blm Traces Monitor " );
        resetTraceVectors( N );
        debugMessage( "Vectors Reset" );
        for( auto && it1 : blmObj.traceObjects )
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
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to blm Trace Monitors failed" );
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void blmInterface::monitorATraceForNShots( const std::string trace, blmStructs::SCOPE_PV_TYPE channel, size_t N )
{
    if( !isMonitoringBLMTrace( trace, channel ) )
    {
        traceMonitorStructs.clear();
        clearContinuousTraceMonitorStructs();
        debugMessage( "Starting blm Traces Monitor for ", channel );
//        resetATraceVector( trace, channel, N );
        resetTraceVectors( N );
        debugMessage( "Vector ", channel, " Reset" );
        blmObj.traceObjects.at( trace );
        blmObj.traceObjects.at( trace ).numShots = N;
        blmObj.traceObjects.at( trace ).isAContinuousMonitorStruct = false;
        blmObj.traceObjects.at( trace ).isATemporaryMonitorStruct = true;
        blmObj.traceObjects.at( trace ).isMonitoringMap.at( channel ) = true;

        addToTraceMonitorStructs( traceMonitorStructs, blmObj.traceObjects.at( trace ).pvMonStructs.at( channel ), &blmObj.traceObjects.at( trace ) );
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to blm Trace Monitors failed" );
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void blmInterface::resetTraceVectors( size_t N )
{
    traceMonitorStructs.clear();
    for( auto && it : blmObj.traceObjects )
    {   /// Clear all trace data + timestamps
        /// Resize to N shots
        for( auto && it2 : it.second.traceData )
        {

            it2.second.clear();
            it2.second.resize( N );
            for( auto && it3 : it2.second )
            {
                it3.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR1 ].COUNT );
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
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR1 ].COUNT );
//        for( auto && it2 : it.second.tr2TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR2 ].COUNT );
//        for( auto && it2 : it.second.tr3TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR3 ].COUNT );
//        for( auto && it2 : it.second.tr4TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR4 ].COUNT );
    }
}
//______________________________________________________________________________
void blmInterface::resetATraceVector( const std::string blmName, blmStructs::SCOPE_PV_TYPE channel, size_t N )
{
    traceMonitorStructs.clear();
    blmObj.traceObjects.at( blmName ).traceData.at( channel ).clear();
    blmObj.traceObjects.at( blmName ).traceData.at( channel ).resize( N );
    for( auto && it3 : blmObj.traceObjects.at( blmName ).traceData.at( channel ) )
    {
        it3.clear();
        it3.resize( blmObj.traceObjects.at( blmName ).pvMonStructs[ channel ].COUNT );
    }
    blmObj.traceObjects.at( blmName ).timeStamps.at( channel ).clear();
    blmObj.traceObjects.at( blmName ).timeStamps.at( channel ).resize( N );
    blmObj.traceObjects.at( blmName ).strTimeStamps.at( channel ).clear();
    blmObj.traceObjects.at( blmName ).strTimeStamps.at( channel ).resize( N );
    blmObj.traceObjects.at( blmName ).shotCounts.at( channel ) = 0;
}
//______________________________________________________________________________
void blmInterface::monitorNumsForNShots( size_t N )
{
    if( !monitoringNums )
    {
        numMonitorStructs.clear();
        clearContinuousNumMonitorStructs();
        debugMessage( "Starting blm Nums Monitor " );
        resetNumVectors( N );
        debugMessage( "Vectors Reset" );
        for( auto && it1 : blmObj.numObjects )
        {
            it1.second.numShots = N;
            it1.second.isAContinuousMonitorStruct=false;
            it1.second.isATemporaryMonitorStruct=true;
            for( auto && it2 : it1.second.pvMonStructs )
            {
                if( isANumPV( it2.first ) )
                {
                    addToNumMonitorStructs( numMonitorStructs, it2.second, &it1.second  );
                }
            }
        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to blm Num Monitors failed" );
        if ( status == ECA_NORMAL )
            for( auto && it : blmObj.numObjects )
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
void blmInterface::monitorANumForNShots( const std::string num, blmStructs::SCOPE_PV_TYPE channel, size_t N )
{
    if( !isMonitoringBLMNum( num, channel ) )
    {
        numMonitorStructs.clear();
        clearContinuousNumMonitorStructs();
        debugMessage( "Starting blm Num Monitor for ", channel );
//        resetATraceVector( num, channel, N );
        resetNumVectors( N );
        debugMessage( "Vector ", channel, " Reset" );
        blmObj.numObjects.at( num );
        blmObj.numObjects.at( num ).numShots = N;
        blmObj.numObjects.at( num ).isAContinuousMonitorStruct = false;
        blmObj.numObjects.at( num ).isATemporaryMonitorStruct = true;
        blmObj.numObjects.at( num ).isMonitoringMap.at( channel ) = true;

        addToNumMonitorStructs( numMonitorStructs, blmObj.numObjects.at( num ).pvMonStructs.at( channel ), &blmObj.numObjects.at( num ) );
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to blm Num Monitors failed" );
    }
    else
    {
        message( "Already Monitoring Nums " ); /// make more useful
    }
}
//______________________________________________________________________________
void blmInterface::resetNumVectors( size_t N )
{
    numMonitorStructs.clear();
    for( auto && it : blmObj.numObjects )
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
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR1 ].COUNT );
//        for( auto && it2 : it.second.tr2TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR2 ].COUNT );
//        for( auto && it2 : it.second.tr3TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR3 ].COUNT );
//        for( auto && it2 : it.second.tr4TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::SCOPE_PV_TYPE::TR4 ].COUNT );
    }
}
//______________________________________________________________________________
void blmInterface::resetANumVector( const std::string blmName, blmStructs::SCOPE_PV_TYPE channel, size_t N )
{
    numMonitorStructs.clear();
    blmObj.numObjects.at( blmName ).numData.at( channel ).clear();
    blmObj.numObjects.at( blmName ).numData.at( channel ).resize( N );
    blmObj.numObjects.at( blmName ).numTimeStamps.at( channel ).clear();
    blmObj.numObjects.at( blmName ).numTimeStamps.at( channel ).resize( N );
    blmObj.numObjects.at( blmName ).numStrTimeStamps.at( channel ).clear();
    blmObj.numObjects.at( blmName ).numStrTimeStamps.at( channel ).resize( N );
    blmObj.numObjects.at( blmName ).shotCounts.at( channel ) = 0;
}
//______________________________________________________________________________
bool blmInterface::isMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    if( blmObj.traceObjects.at( blmName ).isMonitoringMap.at( pvType ) == true )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool blmInterface::isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return !isMonitoringBLMTrace( blmName, pvType );
}
//______________________________________________________________________________
bool blmInterface::isMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    if( blmObj.numObjects.at( blmName ).isMonitoringMap.at( pvType ) == true )
        return true;
    else
        return false;
}
//______________________________________________________________________________
void blmInterface::setTimebase( const std::string & blmName, const double timebase )
{
    if( entryExists( blmObj.traceObjects, blmName ) )
        {
            for( auto && it : blmObj.traceObjects )
            {
                it.second.timebase = timebase;
            }
        }
    else
    {
        message("ERROR!!!!! BLM not defined in config file");
    }
}
//______________________________________________________________________________
bool blmInterface::isNotMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return !isMonitoringBLMNum( blmName, pvType );
}
//______________________________________________________________________________
const blmStructs::blmTraceData & blmInterface::getBLMTraceDataStruct( const std::string & blmName )
{
    if( entryExists( blmObj.traceObjects, blmName ) && blmObj.traceObjects.at( blmName ).traceData.size() != 0 )
    {
        return blmObj.traceObjects.at( blmName );
    }
}
//______________________________________________________________________________
const blmStructs::blmNumObject & blmInterface::getBLMNumDataStruct( const std::string & blmName )
{
    if( entryExists( blmObj.numObjects, blmName ) && blmObj.numObjects.at( blmName ).numData.size() != 0 )
    {
        return blmObj.numObjects.at( blmName );
    }
}
//______________________________________________________________________________
double blmInterface::getTimebase( const std::string & blmName )
{
    if( entryExists( blmObj.traceObjects, blmName ) )
    {
        return blmObj.traceObjects.at( blmName ).timebase;
    }
    else
    {
        message("ERROR!!!!! BLM not defined in config file!!!!");
        return UTL::DUMMY_DOUBLE;
    }
}
//______________________________________________________________________________
size_t blmInterface::getBufferSize( const std::string & blmName )
{
    if( entryExists( blmObj.traceObjects, blmName ) )
    {
        return blmObj.traceObjects.at( blmName ).buffer;
    }
    else
    {
        message("ERROR!!!!! BLM not defined in config file!!!!");
        return UTL::DUMMY_SIZET;
    }
}
//______________________________________________________________________________
const blmStructs::DIAG_TYPE blmInterface::getDiagType( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    if( entryExists( blmObj.traceObjects, blmName ) && isATracePV( pvType ) )
    {
        return blmObj.traceObjects.at( blmName ).diagType;
    }
    else if( entryExists( blmObj.numObjects, blmName ) && isANumPV( pvType ) )
    {
        return blmObj.numObjects.at( blmName ).diagType;
    }
    else
    {
        message("ERROR!!!!! BLM not defined in config file!!!!");
        return blmStructs::DIAG_TYPE::UNKNOWN_DIAG_TYPE;
    }
}
//______________________________________________________________________________
const std::string blmInterface::getDiagTypeStr( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType )
{
    return getDiagTypeStr( blmName, pvType);
}
//______________________________________________________________________________
std::vector< std::vector< double > > blmInterface::getBLMTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).traceData.at( pvType );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMNums( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return blmObj.numObjects.at( name ).numData.at( pvType );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getMinOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    /// For the WCM and ED-FCUP we need the first minimum of the trace
    std::vector< double > minElements;

    for( auto && it : blmObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        minElements.push_back( (*std::min_element( it.begin(), it.end() ) ) );
    }

    return minElements;
}
//______________________________________________________________________________
std::vector< double > blmInterface::getMaxOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    /// For spectrometer FCUP we need the peak-to-peak voltage
    std::vector< double > maxElements;

    for( auto && it : blmObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        maxElements.push_back( (*std::max_element( it.begin(), it.end() ) ) );
    }

    return maxElements;
}
//______________________________________________________________________________
std::vector< double > blmInterface::getAreaUnderTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    /// For now this just uses the rectangle rule to calculate the area under a blm trace -
    /// there is probably a better way to do this.
    /// Currently we only need to integrate the ICT traces
    std::vector< double > areaElements;

    for( auto && it : blmObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum_of_elems = 0.0;
        std::vector< double > curveAreas;
        for( auto && it2 : it )
        {
//            std::cout << it2 << std::endl;
            double curveElement = ( it2 * blmObj.traceObjects.at( name ).timebase );// / it.size();
            curveAreas.push_back( curveElement );
        }
        sum_of_elems = std::accumulate( curveAreas.begin(), curveAreas.end(), 0.0 );
        areaElements.push_back( sum_of_elems );
    }

    return areaElements;
}
//______________________________________________________________________________
std::vector< double > blmInterface::getTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).timeStamps.at( pvType );
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getStrTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).strTimeStamps.at( pvType );
}
//______________________________________________________________________________
double blmInterface::getBLMP1( const std::string & name )
{
    return blmObj.numObjects.at( name ).p1;
}
//______________________________________________________________________________
double blmInterface::getBLMP2( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P2 ).back();
}
//______________________________________________________________________________
double blmInterface::getBLMP3( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P3 ).back();
}
//______________________________________________________________________________
double blmInterface::getBLMP4( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P4 ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMP1Vec( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P1 );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMP2Vec( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P2 );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMP3Vec( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P3 );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMP4Vec( const std::string & name )
{
    return blmObj.numObjects.at( name ).numData.at( blmStructs::SCOPE_PV_TYPE::P4 );
}
//______________________________________________________________________________
void blmInterface::setBufferSize( size_t bufferSize )
{
    for( auto && it : blmObj.numObjects )
    {
        for( auto && it1 : it.second.numDataBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        it.second.buffer = bufferSize;
    }
    for( auto && it : blmObj.traceObjects )
    {
        for( auto && it1 : it.second.traceDataBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        it.second.buffer = bufferSize;
    }
}
//______________________________________________________________________________
void blmInterface::setNumBufferSize( size_t bufferSize )
{
    for( auto && it : blmObj.numObjects )
    {
        for( auto && it1 : it.second.numDataBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        it.second.buffer = bufferSize;
    }
}
//______________________________________________________________________________
void blmInterface::setTraceBufferSize( size_t bufferSize )
{
    for( auto && it : blmObj.traceObjects )
    {
        for( auto && it1 : it.second.traceDataBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        it.second.buffer = bufferSize;
    }
}
//______________________________________________________________________________
void blmInterface::restartContinuousMonitoring()
{
    monitorBLMs();
}
//______________________________________________________________________________
boost::circular_buffer< double > blmInterface::getBLMNumBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    if( entryExists( blmObj.numObjects, name ) )
    {
        return blmObj.numObjects.at( name ).numDataBuffer.at( pvType );
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > blmInterface::getBLMP1Buffer( const std::string & name )
{
    if( entryExists( blmObj.numObjects, name ) && blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P1 ).size() != 0 )
    {
        return blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P1 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > blmInterface::getBLMP2Buffer( const std::string & name )
{
    if( entryExists( blmObj.numObjects, name ) && blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P2 ).size() != 0 )
    {
        return blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P2 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > blmInterface::getBLMP3Buffer( const std::string & name )
{
    if( entryExists( blmObj.numObjects, name ) && blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P3 ).size() != 0 )
    {
        return blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P3 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > blmInterface::getBLMP4Buffer( const std::string & name )
{
    if( entryExists( blmObj.numObjects, name ) && blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P4 ).size() != 0 )
    {
        return blmObj.numObjects.at( name ).numDataBuffer.at( blmStructs::SCOPE_PV_TYPE::P4 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMTraceBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType )
{
    if( entryExists( blmObj.traceObjects, name ) )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( pvType );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMTR1Buffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR1 ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR1 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMTR2Buffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR2 ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR2 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMTR3Buffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR3 ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR3 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMTR4Buffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR4 ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::SCOPE_PV_TYPE::TR4 );
    }
}
//______________________________________________________________________________
void blmInterface::killTraceCallBack( blmStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        delete ms;
        debugMessage("killed callback to ",ENUM_TO_STRING(ms->monType));
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->blmObject );
    }
}
//______________________________________________________________________________
void blmInterface::killNumCallBack( blmStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        delete ms;
        debugMessage("killed callback to ",ENUM_TO_STRING(ms->monType));
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->blmObject );
    }
}
//______________________________________________________________________________
bool blmInterface::isATracePV( blmStructs::SCOPE_PV_TYPE pv )
{
    bool ret = false;
    if( pv == blmStructs::SCOPE_PV_TYPE::TR1 )
        ret = true;
    else if( pv == blmStructs::SCOPE_PV_TYPE::TR2 )
        ret = true;
    else if( pv == blmStructs::SCOPE_PV_TYPE::TR3 )
        ret = true;
    else if( pv == blmStructs::SCOPE_PV_TYPE::TR4 )
        ret = true;
    return ret;
}
//______________________________________________________________________________
bool blmInterface::isANumPV( blmStructs::SCOPE_PV_TYPE pv )
{
    bool ret = false;
    if( pv == blmStructs::SCOPE_PV_TYPE::P1 )
        ret = true;
    else if( pv == blmStructs::SCOPE_PV_TYPE::P2 )
        ret = true;
    else if( pv == blmStructs::SCOPE_PV_TYPE::P3 )
        ret = true;
    else if( pv == blmStructs::SCOPE_PV_TYPE::P4 )
        ret = true;
    return ret;
}
//______________________________________________________________________________
std::vector< double > blmInterface::getAvgNoise( const std::string & name, blmStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 )
{
    /// Here we take the mean of a part of the blm trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the blm hasn't been set up properly for taking this measurement
    std::vector< double > noise, noiseTotal;
    for( auto && it : blmObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum = std::accumulate( it.begin()+part1, it.begin()+part2, 0.0 );
        noise.push_back( sum / ( part2 - part1 ) );
        noiseTotal.push_back( noise.back());// * blmObj.traceObjects.at( name ).timebase );
    }
//    std::cout << noiseTotal << std::endl;
    return noiseTotal;
}
//______________________________________________________________________________
std::vector< std::vector< double > > blmInterface::getPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 )
{
    /// Here we take the mean of a part of the blm trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the blm hasn't been set up properly for taking this measurement

    std::vector< std::vector< double > > vecPart( blmObj.traceObjects.at( name ).traceData.at( pvType ).size() );
    int i = 0;

    for( auto && it : blmObj.traceObjects.at( name ).traceData.at( pvType ) )
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
std::vector< double > blmInterface::getAreaUnderPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 )
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
            double curveElement = ( it2 * blmObj.traceObjects.at( name ).timebase );// / it.size();
            curveAreas.push_back( curveElement );
        }
        sum_of_elems = std::accumulate( curveAreas.begin(), curveAreas.end(), 0.0 );
        areaElements.push_back( sum_of_elems );
    }

    return areaElements;
}
//______________________________________________________________________________
double blmInterface::getWCMQ()
{
    double wcmQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : blmObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == blmStructs::DIAG_TYPE::WCM && it2.second.blmType == blmStructs::SCOPE_TYPE::NUM )
            {
                minVal = it.second.numData.at( it2.second.pvType ).back();
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
double blmInterface::getICT1Q( const int part1, const int part2 )
{
    double ict1q = UTL::DUMMY_DOUBLE;
    double traceArea, noise;
    for( auto && it : blmObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == blmStructs::DIAG_TYPE::ICT1 )
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
double blmInterface::getICT2Q( const int part1, const int part2 )
{
    double ict2q = UTL::DUMMY_DOUBLE;
    double traceArea, noise;
    for( auto && it : blmObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == blmStructs::DIAG_TYPE::ICT2 )
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
double blmInterface::getFCUPQ()
{
    double fcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : blmObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == blmStructs::DIAG_TYPE::FCUP && it2.second.blmType == blmStructs::SCOPE_TYPE::NUM )
            {
                minVal = getBLMNums( it.first, it2.second.pvType ).back();
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
double blmInterface::getEDFCUPQ()
{
    double edfcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : blmObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == blmStructs::DIAG_TYPE::ED_FCUP && it2.second.blmType == blmStructs::SCOPE_TYPE::NUM )
            {
                minVal = getBLMNums( it.first, it2.second.pvType ).back();
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
std::vector< std::string > blmInterface::getBLMNames()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        message( "BLM Name ", iter.first );
        blmNames.push_back( iter.first );
    }
    for( auto && iter : blmObj.numObjects )
    {
        message( "BLM Name ", iter.first );
        blmNames.push_back( iter.first );
    }

    return blmNames;
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getBLMPVs()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "BLM PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            blmNames.push_back( s.str() );
        }
    }
    for( auto && iter : blmObj.numObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "BLM PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            blmNames.push_back( s.str() );
        }
    }

    return blmNames;
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getBLMTracePVs()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "BLM PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            blmNames.push_back( s.str() );
        }
    }

    return blmNames;
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getBLMNumPVs()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.numObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "BLM PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            blmNames.push_back( s.str() );
        }
    }

    return blmNames;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > blmInterface::getILockStates( const std::string & objName )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > r;
    auto iter = allBLMData.find( objName );
    if( iter != allBLMData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  blmInterface::getILockStatesStr( const std::string & name )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allBLMData.find( name );
    if( iter != allBLMData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
//______________________________________________________________________________
bool blmInterface::toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, blmInterface & obj )
{
    if( CALL_MEMBER_FN(obj, f1)( name ) ) /// CALL_MEMBER_FN MACRO in structs.h
    {
        CALL_MEMBER_FN(obj, f3)( name ) ;

        time_t timeStart = time( 0 );  /// Start Clock

        while( CALL_MEMBER_FN(obj, f1)( name ) )
        {
            UTL::PAUSE_300;
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
