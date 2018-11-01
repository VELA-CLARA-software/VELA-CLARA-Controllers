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
        killTraceCallBack( it );
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

    // send
    int status = sendToEpics( "ca_create_channel", "Found blm chids.", "!!TIMEOUT!! Not all blm ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        UTL::PAUSE_500;
        for( auto && it1 : blmObj.traceObjects )
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
        case blmStructs::BLM_PV_TYPE::CH1WAVE:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH2WAVE:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH3WAVE:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH4WAVE:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH1TIME:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH2TIME:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH3TIME:
        {
            msv.back() -> val = (void*)traceObj;
            msv.back() -> objName = traceObj -> name;
            break;
        }
        case blmStructs::BLM_PV_TYPE::CH4TIME:
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
void blmInterface::staticEntryrMonitor( const event_handler_args args )
{
    blmStructs::monitorStruct * ms = static_cast< blmStructs::monitorStruct *> ( args.usr );
    ms->interface->updateTrace( ms, args );
}
//______________________________________________________________________________
void blmInterface::updateTrace( blmStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    blmStructs::blmTraceData * td = &ms->interface->blmObj.traceObjects.at( ms->objName );

    if( td->isAContinuousMonitorStruct )
    {
        td->shotCounts.at( ms -> monType ) = 0;
        if( td->timeStamps.at(ms->monType).size() == 0 )
        {
//            setBufferSize(td->buffer);
            td->timeStamps.at(ms->monType).push_back(1);
            td->strTimeStamps.at(ms->monType).push_back(UTL::UNKNOWN_STRING);
//            td->traceDataBuffer.at(ms->monType).resize(td->buffer);
            for( auto && it : td->traceDataBuffer.at(ms -> monType) )
            {
                it.resize( td->pvMonStructs[ blmStructs::BLM_PV_TYPE::CH1WAVE ].COUNT );
            }
        }
    }
    const dbr_double_t * value = &(p  -> value);
    size_t i =1;
    int zero = 0;

    updateTime( p->stamp, td->timeStamps.at( ms -> monType ).back(),
               td->strTimeStamps.at( ms -> monType ).back() );

    /// resizes the trace vectors dynamically in case the trace being sent to EPICS from the blm changes size
    /// - limited by the array size set in EPICS database (currently (6/4/17) 2000 points)
//    td->traceData.at( ms -> monType )[ td->shotCounts.at( ms -> monType ) ].resize(static_cast< int >(*( &p->value ) ) );

    std::vector< double > rawVectorContainer(td->pvMonStructs[ blmStructs::BLM_PV_TYPE::CH1WAVE ].COUNT);

    for( auto && it : rawVectorContainer )
    {
        it = *( &p->value + i );
        ++i;
    }
    td->traceDataBuffer.at( ms -> monType ).push_back(rawVectorContainer);
    td->timeStampsBuffer.at( ms -> monType ).push_back(td->timeStamps.at(ms->monType).back());
    td->strTimeStampsBuffer.at( ms -> monType ).push_back(td->strTimeStamps.at(ms->monType).back());

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
void blmInterface::clearContinuousMonitorStructs()
{
    if( continuousMonitorStructs.size() != 0 )
    {
        for( auto && it : continuousMonitorStructs )
        {
           killTraceCallBack(it);
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
            killTraceCallBack(it);

        }
        continuousTraceMonitorStructs.clear();
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
                    for( auto it3 : it1.second.isMonitoringMap )
                    {
                        it3.second = true;
                    }
//                    monitoringTraces = true;

                    addToTraceMonitorStructs( traceMonitorStructs, it2.second, &it1.second  );
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
void blmInterface::monitorATraceForNShots( const std::string trace, blmStructs::BLM_PV_TYPE channel, size_t N )
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
                it3.resize( it.second.pvMonStructs[ blmStructs::BLM_PV_TYPE::CH1WAVE ].COUNT );
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
//            it2.resize( it.second.pvMonStructs[ blmStructs::BLM_PV_TYPE::TR1 ].COUNT );
//        for( auto && it2 : it.second.tr2TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::BLM_PV_TYPE::TR2 ].COUNT );
//        for( auto && it2 : it.second.tr3TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::BLM_PV_TYPE::TR3 ].COUNT );
//        for( auto && it2 : it.second.tr4TraceData )
//            it2.resize( it.second.pvMonStructs[ blmStructs::BLM_PV_TYPE::TR4 ].COUNT );
    }
}
//______________________________________________________________________________
void blmInterface::resetATraceVector( const std::string blmName, blmStructs::BLM_PV_TYPE channel, size_t N )
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
bool blmInterface::isMonitoringBLMTrace( const std::string & blmName, blmStructs::BLM_PV_TYPE pvType )
{
    if( blmObj.traceObjects.at( blmName ).isMonitoringMap.at( pvType ) == true )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool blmInterface::isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::BLM_PV_TYPE pvType )
{
    return !isMonitoringBLMTrace( blmName, pvType );
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
std::vector< std::vector< double > > blmInterface::getBLMTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).traceData.at( pvType );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getMinOfTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
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
std::vector< double > blmInterface::getMaxOfTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
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
std::vector< double > blmInterface::getAreaUnderTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
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
std::vector< double > blmInterface::getTimeStamps( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).timeStamps.at( pvType );
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getStrTimeStamps( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).strTimeStamps.at( pvType );
}
//______________________________________________________________________________
boost::circular_buffer< double > blmInterface::getTimeStampsBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).timeStampsBuffer.at( pvType );
}
//______________________________________________________________________________
boost::circular_buffer< std::string > blmInterface::getStrTimeStampsBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    return blmObj.traceObjects.at( name ).strTimeStampsBuffer.at( pvType );
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH1Waveform( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH1WAVE ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH2Waveform( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH2WAVE ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH3Waveform( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH3WAVE ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH4Waveform( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH4WAVE ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH1Time( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH1TIME ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH2Time( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH2TIME ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH3Time( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH3TIME ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMCH4Time( const std::string & name )
{
    return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH4TIME ).back();
}
//______________________________________________________________________________
std::vector< double > blmInterface::getBLMTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    if( entryExists( blmObj.traceObjects, name ) )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( pvType ).back();
    }
}
//______________________________________________________________________________
void blmInterface::setBufferSize( size_t bufferSize )
{
    for( auto && it : blmObj.traceObjects )
    {
        for( auto && it1 : it.second.traceDataBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        for( auto && it1 : it.second.timeStampsBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        for( auto && it1 : it.second.strTimeStampsBuffer )
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
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH1WaveformBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH1WAVE ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH1WAVE );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH2WaveformBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH2WAVE ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH2WAVE );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH3WaveformBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH3WAVE ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH3WAVE );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH4WaveformBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH4WAVE ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH4WAVE );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH1TimeBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH1TIME ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH1TIME );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH2TimeBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH2TIME ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH2TIME );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH3TimeBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH3TIME ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH3TIME );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMCH4TimeBuffer( const std::string & name )
{
    if( entryExists( blmObj.traceObjects, name ) && blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH4TIME ).size() != 0 )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( blmStructs::BLM_PV_TYPE::CH4TIME );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > blmInterface::getBLMTraceBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType )
{
    if( entryExists( blmObj.traceObjects, name ) )
    {
        return blmObj.traceObjects.at( name ).traceDataBuffer.at( pvType );
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
bool blmInterface::isATimePV( blmStructs::BLM_DATA_TYPE pv )
{
    bool ret = false;
    if( pv == blmStructs::BLM_DATA_TYPE::TIME )
        ret = true;
    return ret;
}
//______________________________________________________________________________
bool blmInterface::isAWaveformPV( blmStructs::BLM_DATA_TYPE pv )
{
    bool ret = false;
    if( pv == blmStructs::BLM_DATA_TYPE::WAVE )
        ret = true;
    return ret;
}
//______________________________________________________________________________
std::vector< double > blmInterface::getAvgNoise( const std::string & name, blmStructs::BLM_PV_TYPE & pvType, const int part1, const int part2 )
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
std::vector< std::vector< double > > blmInterface::getPartOfTrace( const std::string & name, blmStructs::BLM_PV_TYPE & pvType, const int part1, const int part2 )
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
std::vector< double > blmInterface::getAreaUnderPartOfTrace( const std::string & name, blmStructs::BLM_PV_TYPE & pvType, const int part1, const int part2 )
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
std::vector< std::string > blmInterface::getBLMNames()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        message( "BLM Name ", iter.first );
        blmNames.push_back( iter.first );
    }
    return blmNames;
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getBLMPVStrings()
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
std::vector< std::string > blmInterface::getBLMWaveformPVStrings()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            if( isAWaveformPV( iter2.second.blmDataType ) )
            {
                std::stringstream s;
                message( "BLM PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
                s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
                blmNames.push_back( s.str() );
            }
        }
    }
    return blmNames;
}
//______________________________________________________________________________
std::vector< std::string > blmInterface::getBLMTimePVStrings()
{
    std::vector< std::string > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            if( isATimePV( iter2.second.blmDataType ) )
            {
                std::stringstream s;
                message( "BLM PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
                s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
                blmNames.push_back( s.str() );
            }
        }
    }
    return blmNames;
}
//______________________________________________________________________________
std::vector< blmStructs::BLM_PV_TYPE > blmInterface::getBLMPVs()
{
    std::vector< blmStructs::BLM_PV_TYPE > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            blmNames.push_back( iter2.second.pvType );
        }
    }
    return blmNames;
}
//______________________________________________________________________________
std::vector< blmStructs::BLM_PV_TYPE > blmInterface::getBLMWaveformPVs()
{
    std::vector< blmStructs::BLM_PV_TYPE > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            if( isAWaveformPV( iter2.second.blmDataType ) )
            {
                blmNames.push_back( iter2.second.pvType );
                message(blmNames.back());
            }
        }
    }
    return blmNames;
}
//______________________________________________________________________________
std::vector< blmStructs::BLM_PV_TYPE > blmInterface::getBLMTimePVs()
{
    std::vector< blmStructs::BLM_PV_TYPE > blmNames;
    for( auto && iter : blmObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            if( isATimePV( iter2.second.blmDataType ) )
            {
                blmNames.push_back( iter2.second.pvType );
            }
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
