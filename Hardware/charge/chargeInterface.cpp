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

#include "chargeInterface.h"
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
chargeInterface::chargeInterface( const std::string & configFileLocation1,
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
chargeInterface::~chargeInterface()
{
    for( auto it : continuousMonitorStructs )
    {
        killNumCallBack( it );
        debugMessage("delete chargeInterface continuousMonitorStructs entry.");
        delete it;
    }
    debugMessage("chargeInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void chargeInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfigFiles();
    pause_2000();
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initChargeObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                /// subscribe to the channel ids
                initChargeChids();
                /// start the monitors: set up the callback functions
                monitorCharges();
                /// The pause allows EPICS to catch up.
                pause_500();
            }
            else
                message("The chargeInterface Read Config files, Not Starting EPICS Monitors" );
        }
        else
            message( "!!!The chargeInterface received an Error while getting charge data!!!" );
    }
}
//______________________________________________________________________________
bool chargeInterface::initChargeObjects()
{
    chargeObj = configReader.getChargeObject();
    return true;
}
//______________________________________________________________________________
void chargeInterface::initChargeChids()
{
    message( "\n", "Searching for charge chids..." );
    for( auto && it1 : chargeObj.dataObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
            addChannel( it1.second.pvRoot, it2.second );
        }
        for( auto && it2 : it1.second.pvComStructs )
        {
            addChannel( it1.second.pvRoot, it2.second );
        }
    }

    // send
    int status = sendToEpics( "ca_create_channel", "Found charge chids.", "!!TIMEOUT!! Not all charge ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        UTL::PAUSE_500;
        for( auto && it1 : chargeObj.dataObjects )
        {
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
            for( auto && it2 : it1.second.pvComStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        }
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void chargeInterface::addChannel( const std::string & pvRoot, chargeStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << ":" << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void chargeInterface::monitorcharges()
{
    continuousMonitorStructs.clear();
    for( auto && it1 : chargeObj.dataObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
            continuousMonitorStructs.push_back(new chargeStructs::monitorStruct());
            continuousMonitorStructs.back()->monType = it2.first;
            continuousMonitorStructs.back()->objName = it1.second.name;
            continuousMonitorStructs.back()->interface = this;
            continuousMonitorStructs.back()->val = &it1.second;
            continuousMonitorStructs.back()->diagType = it2.second.diagType;
            it1.second.isAContinuousMonitorStruct = true;
            it1.second.isATemporaryMonitorStruct = false;
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   chargeInterface::staticEntryrMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back()->EVID);
            debugMessage("Adding monitor for ",it1.second.name, " ",ENUM_TO_STRING(it2.first));
        }
    }

    for( auto && it1 : chargeObj.dataObjects )
        for( auto && it2 : it1.second.pvComStructs )
            addToMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to charge Monitors", "!!TIMEOUT!! Subscription to charge Monitors failed" );
    if ( status == ECA_NORMAL )
    {
        allMonitorsStarted = true; /// interface base class member
    }
}
//______________________________________________________________________________
void chargeInterface::addToMonitorStructs( std::vector< chargeStructs::monitorStruct * > & msv, chargeStructs::pvStruct & pv,  chargeStructs::dataObject * dataOb   )
{
    msv.push_back( new chargeStructs::monitorStruct() );
    msv.back() -> monType      = pv.pvType;
    msv.back() -> chargeObject = &chargeObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;

    switch( pv.pvType )
    {
        case chargeStructs::CHARGE_PV_TYPE::WCM:
        {
            msv.back() -> val  = (void*)dataOb;
            msv.back() -> name = dataOb -> name;
            break;
        }
        case chargeStructs::CHARGE_PV_TYPE::S02_FCUP:
        {
            msv.back() -> val  = (void*)dataOb;
            msv.back() -> name = dataOb -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  chargeInterface::staticEntryrMonitor, (void*)msv.back(), &msv.back() -> EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void chargeInterface::staticEntryrMonitor( const event_handler_args args )
{
    chargeStructs::monitorStruct * ms = static_cast< chargeStructs::monitorStruct *> ( args.usr );
    switch( ms -> monType )
    {
        case chargeStructs::CHARGE_PV_TYPE::WCM:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
        case chargeStructs::CHARGE_PV_TYPE::S02_FCUP:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
    }
}
//______________________________________________________________________________
void chargeInterface::updateValue( chargeStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    chargeStructs::dataObject * dobj = reinterpret_cast< chargeStructs::chargeNumObject* > (ms -> val);

//    scno->isMonitoringMap.at( ms -> monType ) = true;

    if( dobj->isAContinuousMonitorStruct )
    {
        dobj->shotCounts = 0;
        if( dobj->timeStamps.size() == 0 )
        {
            dobj->timeStamps.push_back(1);
            dobj->strTimeStamps.push_back(UTL::UNKNOWN_STRING);
            dobj->chargeVec.push_back(UTL::DUMMY_DOUBLE);
            dobj->chargeBuffer.resize(dobj->buffer);
        }
    }

    const dbr_double_t * val = &(p  -> value);
    size_t i = 0;

    updateTime( p->stamp, dobj->timeStamps[ dobj->shotCounts.at( ms -> monType ) ], dobj->strTimeStamps[ dobj->shotCounts.at( ms -> monType ) ]  );

    dobj->charge = *( &p -> value );
    dobj->timeStamp = dobj->timeStamps[ dobj->shotCounts.at( ms -> monType ) ];

    switch( ms -> monType )
    {
        case chargeStructs::charge_PV_TYPE::WCM:
            {
                dobj->wcm = *( &p -> value );
                break;
            }
        case chargeStructs::charge_PV_TYPE::S02_FCUP:
            {
                dobj->s02FCUP = *( &p -> value );
                break;
            }
    }
    dobj->chargeVec[ dobj->shotCounts ] = *( &p -> value );
    dobj->chargeBuffer.push_back(*( &p -> value ));

    if( dobj -> isATemporaryMonitorStruct )
    {
        if( dobj -> numShots > -1 )
        {
            ++dobj -> shotCounts;
        }
        if( dobj->shotCounts == dobj->numShots )
        {
            message( "Collected ", dobj->shotCounts, " shots for ", dobj -> pvRoot, ":", ENUM_TO_STRING( ms->monType ) );
            dobj->isMonitoring = false;
            ms->interface->killCallBack( ms );
        }
    }
}
//______________________________________________________________________________
void chargeInterface::clearContinuousMonitorStructs()
{
    if( continuousMonitorStructs.size() != 0 )
    {
        for( auto && it : continuousMonitorStructs )
        {
            killCallBack(it);
        }
        continuousMonitorStructs.clear();
    }
}
//______________________________________________________________________________
void chargeInterface::monitorChargesForNShots( const std::vector< std::string charge >, size_t N )
{
    if( !monitoringTraces )
    {
        continuousMonitorStructs.clear();
        clearContinuousMonitorStructs();
        debugMessage( "Starting charge Monitor " );
        resetChargeVectors( N );
        debugMessage( "Vectors Reset" );
        for( auto && it1 : chargeObj.traceObjects )
        {
            it1.second.numShots = N;
            it1.second.isAContinuousMonitorStruct=false;
            it1.second.isATemporaryMonitorStruct=true;
            it1.second.isMonitoring=true;
            for( auto && it2 : it1.second.pvMonStructs )
            {
                for( auto it3 : it1.second.isMonitoringMap )
                {
                    it3.second = true;
                }
//                    monitoringTraces = true;
                addToMonitorStructs( chargeMonitor, it2.second, &it1.second  );
            }
        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to charge Monitors failed" );
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void chargeInterface::monitorAChargeForNShots( const std::string chargeName, size_t N )
{
    if( isNotMonitoringCharge( chargeName ) )
    {
        chargeMonitorStructs.clear();
        clearContinuousMonitorStructs();
        debugMessage( "Starting charge Monitor for ", chargeName );
//        resetATraceVector( trace, channel, N );
        resetAChargeVector( chargeName, N );
        debugMessage( "Vector ", channel, " Reset" );
        chargeObj.dataObjects.at( chargeName ).numShots = N;
        chargeObj.dataObjects.at( chargeName ).isAContinuousMonitorStruct = false;
        chargeObj.dataObjects.at( chargeName ).isATemporaryMonitorStruct = true;
        chargeObj.dataObjects.at( chargeName ).isMonitoringMap.at( channel ) = true;
        chargeObj.dataObjects.at( chargeName ).isMonitoring = true;

        addToMonitorStructs( chargeMonitorStructs, chargeObj.dataObjects.at( chargeName ).pvMonStructs.at( chargeObj.dataObjects.at( chargeName ).diagType ), &chargeObj.dataObjects.at( chargeName ) );
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to charge Trace Monitors failed" );
    }
    else
    {
        message( "Already Monitoring Traces " ); /// make more useful
    }
}
//______________________________________________________________________________
void chargeInterface::resetChargeVectors( size_t N )
{
    chargeMonitorStructs.clear();
    for( auto && it : chargeObj.dataObjects )
    {   /// Clear all trace data + timestamps
        /// Resize to N shots
        for( auto && it2 : it.second.chargeVec )
        {
            it2.second.clear();
            it2.second.resize( N );
        }
        for( auto && it2 : it.second.chargeBuffer )
        {
            it2.second.clear();
            it2.second.resize( N );
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
    }
}
//______________________________________________________________________________
void chargeInterface::resetAChargeVector( const std::string chargeName, chargeStructs::charge_PV_TYPE channel, size_t N )
{
    chargeMonitor.at( chargeName ).clear();
    chargeObj.dataObjects.at( chargeName ).chargeVec.at( channel ).clear();
    chargeObj.dataObjects.at( chargeName ).chargeVec.at( channel ).resize( N );
    chargeObj.dataObjects.at( chargeName ).chargeBuffer.at( channel ).clear();
    chargeObj.dataObjects.at( chargeName ).chargeBuffer.at( channel ).resize( N );
    chargeObj.dataObjects.at( chargeName ).timeStamps.clear();
    chargeObj.dataObjects.at( chargeName ).timeStamps.resize( N );
    chargeObj.dataObjects.at( chargeName ).strTimeStamps.clear();
    chargeObj.dataObjects.at( chargeName ).strTimeStamps.resize( N );
    chargeObj.dataObjects.at( chargeName ).shotCounts = 0;
}
//______________________________________________________________________________
bool chargeInterface::isMonitoringChargeTrace( const std::string & chargeName )
{
    if( chargeObj.traceObjects.at( chargeName ).isMonitioring == true )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool chargeInterface::isNotMonitoringCharge( const std::string & chargeName )
{
    return !isMonitoringchargeTrace( chargeName );
}
//______________________________________________________________________________
const chargeStructs::dataObject & chargeInterface::getChargeDataStruct( const std::string & chargeName )
{
    if( entryExists( chargeObj.dataObject, chargeName ) )
    {
        return chargeObj.dataObjects.at( chargeName );
    }
}
//______________________________________________________________________________
size_t chargeInterface::getBufferSize( const std::string & chargeName )
{
    if( entryExists( chargeObj.dataObjects, chargeName ) )
    {
        return chargeObj.dataObjects.at( chargeName ).buffer;
    }
    else
    {
        message("ERROR!!!!! charge not defined in config file!!!!");
        return UTL::DUMMY_SIZET;
    }
}
//______________________________________________________________________________
const chargeStructs::CHARGE_PV_TYPE chargeInterface::getDiagType( const std::string & chargeName )
{
    if( entryExists( chargeObj.dataObjects, chargeName ) )
    {
        return chargeObj.dataObjects.at( chargeName ).diagType;
    }
    else
    {
        message("ERROR!!!!! charge not defined in config file!!!!");
        return chargeStructs::CHARGE_PV_TYPE::UNKNOWN;
    }
}
//______________________________________________________________________________
const std::string chargeInterface::getDiagTypeStr( const std::string & chargeName )
{
    return getDiagTypeStr( chargeName, pvType);
}
//______________________________________________________________________________
std::vector< double > chargeInterface::getChargeVector( const std::string & name )
{
    return chargeObj.dataObjects.at( name ).chargeVec;
}
//______________________________________________________________________________
std::vector< double > chargeInterface::getWCMVector( const std::string & name )
{
    double wcmQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : chargeObj.dataObjects )
    {
            if( it2.second.diagType == chargeStructs::CHARGE_PV_TYPE::WCM )
            {
                wcmQ = it.second.chargeVec;
            }
    }
    if( wcmQ == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND WCM, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return wcmQ;
}
//______________________________________________________________________________
std::vector< double > chargeInterface::getS02FCUPVector( const std::string & name )
{
    double fcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : chargeObj.dataObjects )
    {
            if( it2.second.diagType == chargeStructs::CHARGE_PV_TYPE::S02_FCUP )
            {
                fcupQ = it.second.chargeVec;
            }
    }
    if( fcupQ == UTL::DUMMY_DOUBLE )
    {
        message("DID NOT FIND WCM, IS IT DEFINED IN THE CONFIG FILE????");
    }
    return fcupQ;
}
//______________________________________________________________________________
std::vector< double > chargeInterface::getTimeStamps( const std::string & name )
{
    return chargeObj.dataObjects.at( name ).timeStamps;
}
//______________________________________________________________________________
std::vector< std::string > chargeInterface::getStrTimeStamps( const std::string & name )
{
    return chargeObj.dataObjects.at( name ).strTimeStamps;
}
//______________________________________________________________________________
double chargeInterface::getCharge( const std::string & name )
{
    return chargeObj.dataObjects.at( name ).chargeVec.back();
}
//______________________________________________________________________________
double chargeInterface::getWCMCharge( const std::string & name )
{
    return getWCMVector(name).back();
}
//______________________________________________________________________________
double chargeInterface::getS02FCUPCharge( const std::string & name )
{
    return getS02FCUPVector(name).back();
}
//______________________________________________________________________________
void chargeInterface::setBufferSize( size_t bufferSize )
{
    for( auto && it : chargeObj.dataObjects )
    {
        for( auto && it1 : it.second.chargeBuffer )
        {
            it1.second.clear();
            it1.second.resize( bufferSize );
        }
        it.second.buffer = bufferSize;
    }
}
}
//______________________________________________________________________________
void chargeInterface::restartContinuousMonitoring()
{
    monitorCharges();
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getChargeBuffer( const std::string & name )
{
    if( entryExists( chargeObj.dataObjects, name ) )
    {
        return chargeObj.dataObjects.at( name ).chargeBuffer;
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getWCMBuffer( const std::string & name )
{
    boost::circular_buffer< double > wcmQ;
    if( entryExists( chargeObj.dataObjects, name ) )
    {
        for( auto && it : chargeObj.dataObjects )
        {
                if( it2.second.diagType == chargeStructs::CHARGE_PV_TYPE::S02_FCUP )
                {
                    wcmQ = it.second.chargeBuffer;
                }
        }
    }
    return wcmQ;
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getS02FCUPBuffer( const std::string & name )
{
    boost::circular_buffer< double > fcupQ;
    if( entryExists( chargeObj.dataObjects, name ) )
    {
        for( auto && it : chargeObj.dataObjects )
        {
                if( it2.second.diagType == chargeStructs::CHARGE_PV_TYPE::S02_FCUP )
                {
                    fcupQ = it.second.chargeBuffer;
                }
        }
    }
    return fcupQ;
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getchargeP2Buffer( const std::string & name )
{
    if( entryExists( chargeObj.numObjects, name ) && chargeObj.numObjects.at( name ).numDataBuffer.at( chargeStructs::charge_PV_TYPE::P2 ).size() != 0 )
    {
        return chargeObj.numObjects.at( name ).numDataBuffer.at( chargeStructs::charge_PV_TYPE::P2 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getchargeP3Buffer( const std::string & name )
{
    if( entryExists( chargeObj.numObjects, name ) && chargeObj.numObjects.at( name ).numDataBuffer.at( chargeStructs::charge_PV_TYPE::P3 ).size() != 0 )
    {
        return chargeObj.numObjects.at( name ).numDataBuffer.at( chargeStructs::charge_PV_TYPE::P3 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getchargeP4Buffer( const std::string & name )
{
    if( entryExists( chargeObj.numObjects, name ) && chargeObj.numObjects.at( name ).numDataBuffer.at( chargeStructs::charge_PV_TYPE::P4 ).size() != 0 )
    {
        return chargeObj.numObjects.at( name ).numDataBuffer.at( chargeStructs::charge_PV_TYPE::P4 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > chargeInterface::getchargeTraceBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType )
{
    if( entryExists( chargeObj.traceObjects, name ) )
    {
        return chargeObj.traceObjects.at( name ).traceDataBuffer.at( pvType );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > chargeInterface::getchargeTR1Buffer( const std::string & name )
{
    if( entryExists( chargeObj.traceObjects, name ) && chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR1 ).size() != 0 )
    {
        return chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR1 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > chargeInterface::getchargeTR2Buffer( const std::string & name )
{
    if( entryExists( chargeObj.traceObjects, name ) && chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR2 ).size() != 0 )
    {
        return chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR2 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > chargeInterface::getchargeTR3Buffer( const std::string & name )
{
    if( entryExists( chargeObj.traceObjects, name ) && chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR3 ).size() != 0 )
    {
        return chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR3 );
    }
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > chargeInterface::getchargeTR4Buffer( const std::string & name )
{
    if( entryExists( chargeObj.traceObjects, name ) && chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR4 ).size() != 0 )
    {
        return chargeObj.traceObjects.at( name ).traceDataBuffer.at( chargeStructs::charge_PV_TYPE::TR4 );
    }
}
//______________________________________________________________________________
void chargeInterface::killTraceCallBack( chargeStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        delete ms;
        debugMessage("killed callback to ",ENUM_TO_STRING(ms->monType));
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->chargeObject );
    }
}
//______________________________________________________________________________
void chargeInterface::killNumCallBack( chargeStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        delete ms;
        debugMessage("killed callback to ",ENUM_TO_STRING(ms->monType));
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->chargeObject );
    }
}
//______________________________________________________________________________
bool chargeInterface::isATracePV( chargeStructs::charge_PV_TYPE pv )
{
    bool ret = false;
    if( pv == chargeStructs::charge_PV_TYPE::TR1 )
        ret = true;
    else if( pv == chargeStructs::charge_PV_TYPE::TR2 )
        ret = true;
    else if( pv == chargeStructs::charge_PV_TYPE::TR3 )
        ret = true;
    else if( pv == chargeStructs::charge_PV_TYPE::TR4 )
        ret = true;
    return ret;
}
//______________________________________________________________________________
bool chargeInterface::isANumPV( chargeStructs::charge_PV_TYPE pv )
{
    bool ret = false;
    if( pv == chargeStructs::charge_PV_TYPE::P1 )
        ret = true;
    else if( pv == chargeStructs::charge_PV_TYPE::P2 )
        ret = true;
    else if( pv == chargeStructs::charge_PV_TYPE::P3 )
        ret = true;
    else if( pv == chargeStructs::charge_PV_TYPE::P4 )
        ret = true;
    return ret;
}
//______________________________________________________________________________
std::vector< double > chargeInterface::getAvgNoise( const std::string & name, chargeStructs::charge_PV_TYPE & pvType, const int part1, const int part2 )
{
    /// Here we take the mean of a part of the charge trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the charge hasn't been set up properly for taking this measurement
    std::vector< double > noise, noiseTotal;
    for( auto && it : chargeObj.traceObjects.at( name ).traceData.at( pvType ) )
    {
        double sum = std::accumulate( it.begin()+part1, it.begin()+part2, 0.0 );
        noise.push_back( sum / ( part2 - part1 ) );
        noiseTotal.push_back( noise.back());// * chargeObj.traceObjects.at( name ).timebase );
    }
//    std::cout << noiseTotal << std::endl;
    return noiseTotal;
}
//______________________________________________________________________________
std::vector< std::vector< double > > chargeInterface::getPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE & pvType, const int part1, const int part2 )
{
    /// Here we take the mean of a part of the charge trace which has no beam on it
    /// for noise subtraction - this is potentially stupid as the region of interest
    /// may change with time. If this function isn't behaving sensibly then it is
    /// possible that the charge hasn't been set up properly for taking this measurement

    std::vector< std::vector< double > > vecPart( chargeObj.traceObjects.at( name ).traceData.at( pvType ).size() );
    int i = 0;

    for( auto && it : chargeObj.traceObjects.at( name ).traceData.at( pvType ) )
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
std::vector< double > chargeInterface::getAreaUnderPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE & pvType, const int part1, const int part2 )
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
            double curveElement = ( it2 * chargeObj.traceObjects.at( name ).timebase );// / it.size();
            curveAreas.push_back( curveElement );
        }
        sum_of_elems = std::accumulate( curveAreas.begin(), curveAreas.end(), 0.0 );
        areaElements.push_back( sum_of_elems );
    }

    return areaElements;
}
//______________________________________________________________________________
double chargeInterface::getWCMQ()
{
    double wcmQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : chargeObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == chargeStructs::DIAG_TYPE::WCM && it2.second.chargeType == chargeStructs::charge_TYPE::NUM )
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
double chargeInterface::getICT1Q( const int part1, const int part2 )
{
    double ict1q = UTL::DUMMY_DOUBLE;
    double traceArea, noise;
    for( auto && it : chargeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == chargeStructs::DIAG_TYPE::ICT1 )
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
double chargeInterface::getICT2Q( const int part1, const int part2 )
{
    double ict2q = UTL::DUMMY_DOUBLE;
    double traceArea, noise;
    for( auto && it : chargeObj.traceObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == chargeStructs::DIAG_TYPE::ICT2 )
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
double chargeInterface::getFCUPQ()
{
    double fcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : chargeObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == chargeStructs::DIAG_TYPE::FCUP && it2.second.chargeType == chargeStructs::charge_TYPE::NUM )
            {
                minVal = getchargeNums( it.first, it2.second.pvType ).back();
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
double chargeInterface::getEDFCUPQ()
{
    double edfcupQ = UTL::DUMMY_DOUBLE;
    double minVal;
    for( auto && it : chargeObj.numObjects )
    {
        for( auto && it2 : it.second.pvMonStructs )
        {
            if( it2.second.diagType == chargeStructs::DIAG_TYPE::ED_FCUP && it2.second.chargeType == chargeStructs::charge_TYPE::NUM )
            {
                minVal = getchargeNums( it.first, it2.second.pvType ).back();
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
std::vector< std::string > chargeInterface::getchargeNames()
{
    std::vector< std::string > chargeNames;
    for( auto && iter : chargeObj.traceObjects )
    {
        message( "charge Name ", iter.first );
        chargeNames.push_back( iter.first );
    }
    for( auto && iter : chargeObj.numObjects )
    {
        message( "charge Name ", iter.first );
        chargeNames.push_back( iter.first );
    }

    return chargeNames;
}
//______________________________________________________________________________
std::vector< std::string > chargeInterface::getchargePVs()
{
    std::vector< std::string > chargeNames;
    for( auto && iter : chargeObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "charge PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            chargeNames.push_back( s.str() );
        }
    }
    for( auto && iter : chargeObj.numObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "charge PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            chargeNames.push_back( s.str() );
        }
    }

    return chargeNames;
}
//______________________________________________________________________________
std::vector< std::string > chargeInterface::getchargeTracePVs()
{
    std::vector< std::string > chargeNames;
    for( auto && iter : chargeObj.traceObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "charge PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            chargeNames.push_back( s.str() );
        }
    }

    return chargeNames;
}
//______________________________________________________________________________
std::vector< std::string > chargeInterface::getchargeNumPVs()
{
    std::vector< std::string > chargeNames;
    for( auto && iter : chargeObj.numObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "charge PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            chargeNames.push_back( s.str() );
        }
    }

    return chargeNames;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > chargeInterface::getILockStates( const std::string & objName )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > r;
    auto iter = allchargeData.find( objName );
    if( iter != allchargeData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  chargeInterface::getILockStatesStr( const std::string & name )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allchargeData.find( name );
    if( iter != allchargeData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
//______________________________________________________________________________
bool chargeInterface::toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, chargeInterface & obj )
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
