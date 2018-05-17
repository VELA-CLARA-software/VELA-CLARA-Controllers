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
    for( auto && it : continuousMonitorStructs )
    {
        if( it )
        {
            killCallBack( it );
        }
    }

    debugMessage("chargeInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void chargeInterface::killCallBack( chargeStructs::monitorStruct *& ms )
{
    message("first line" );
    int status = ca_clear_subscription( ms -> EVID );
    message("second line" );
    if( status == ECA_NORMAL)
    {
        message("killed callback to ",ENUM_TO_STRING(ms->monType));
        ms->isCollectingData = false;
        ms->interface->chargeObj.dataObjects.at(ms->name).isMonitoringMap.at(ms->monType) = false;
        delete ms;
        ms = nullptr;
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->name );
    }
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
void chargeInterface::monitorCharges()
{
    continuousMonitorStructs.clear();
    for( auto && it1 : chargeObj.dataObjects )
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
            continuousMonitorStructs.push_back(new chargeStructs::monitorStruct());
            continuousMonitorStructs.back()->monType = it2.first;
            continuousMonitorStructs.back()->name = it1.second.name;
            continuousMonitorStructs.back()->interface = this;
//            continuousMonitorStructs.back()->val = &it1.second;
            continuousMonitorStructs.back()->diagType = it2.second.diagType;
//            continuousMonitorStructs.back()->EVID = it2.second.EVID;
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

    int* value;

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
//    msv.back() -> chargeObject = &chargeObj;
    msv.back() -> interface    = this;
    msv.back() -> CHTYPE       = pv.CHTYPE;

    switch( pv.pvType )
    {
        case chargeStructs::CHARGE_PV_TYPE::Q:
        {
//            msv.back() -> val  = (void*)dataOb;
            msv.back() -> name = dataOb -> name;
            break;
        }
        case chargeStructs::CHARGE_PV_TYPE::V:
        {
//            msv.back() -> val  = (void*)dataOb;
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
        case chargeStructs::CHARGE_PV_TYPE::Q:
            {
                ms->interface->updateValue( ms, args );
                break;
            }
        case chargeStructs::CHARGE_PV_TYPE::V:
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
    chargeStructs::dataObject & dobj = ms -> interface -> chargeObj.dataObjects.at( ms->name );

    if( dobj.isAContinuousMonitorStruct )
    {
        dobj.shotCounts.at(ms->monType) = 0;
        if( dobj.timeStamps.size() == 0 )
        {
            setBufferSize(UTL::TEN_SIZET);
            dobj.timeStamps.push_back(UTL::DUMMY_DOUBLE);
            dobj.strTimeStamps.push_back(UTL::UNKNOWN_STRING);
            dobj.voltageVec.push_back(UTL::DUMMY_DOUBLE);
            dobj.chargeVec.push_back(UTL::DUMMY_DOUBLE);
        }
    }

    const dbr_double_t * val = &(p  -> value);
    size_t i = 0;

    updateTime( p->stamp, dobj.timeStamps[ dobj.shotCounts.at(ms->monType) ], dobj.strTimeStamps[ dobj.shotCounts.at(ms->monType) ]  );

    dobj.timeStamp = dobj.timeStamps.back();

    switch( ms -> monType )
    {
        case chargeStructs::CHARGE_PV_TYPE::Q:
            {
                dobj.charge = *( &p -> value );
                dobj.chargeBuffer.push_back(dobj.charge);
                ++dobj.chargeShots;
                break;
            }
        case chargeStructs::CHARGE_PV_TYPE::V:
            {
                dobj.voltage = *( &p -> value );
                dobj.voltageBuffer.push_back(dobj.voltage);
                ++dobj.voltageShots;
                break;
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
////______________________________________________________________________________
//void chargeInterface::monitorForNShots( const std::vector< std::string > charge, size_t N )
//{
//    for( auto && it : chargeObj.dataObjects )
//    {
//        if( isNotMonitoringCharge( it.second.name ) )
//        {
//            continuousMonitorStructs.clear();
//            clearContinuousMonitorStructs();
//            debugMessage( "Starting charge Monitor " );
//            resetChargeVectors( N );
//            debugMessage( "Vectors Reset" );
//            it.second.numShots = N;
//            it.second.isAContinuousMonitorStruct=false;
//            it.second.isATemporaryMonitorStruct=true;
//            it.second.isMonitoring=true;
//            for( auto && it2 : it.second.pvMonStructs )
//            {
//                addToMonitorStructs( chargeMonitorStructs, it2.second, &it.second  );
//            }
//            int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to charge Monitors failed" );
//        }
//        else
//        {
//            message( "Already Monitoring Traces " ); /// make more useful
//        }
//    }
//}
////______________________________________________________________________________
//void chargeInterface::monitorForNShots( const std::string chargeName, size_t N )
//{
//    if( isNotMonitoringCharge( chargeName ) )
//    {
////        continuousMonitorStructs.clear();
////        clearContinuousMonitorStructs();
//        debugMessage( "Starting charge Monitor for ", chargeName );
////        resetATraceVector( trace, channel, N );
//        resetAChargeVector( chargeName, N );
//        debugMessage( "Vector ", chargeName, " Reset" );
//        chargeObj.dataObjects.at( chargeName ).numShots = N;
//        chargeObj.dataObjects.at( chargeName ).isAContinuousMonitorStruct = false;
//        chargeObj.dataObjects.at( chargeName ).isATemporaryMonitorStruct = true;
//        chargeObj.dataObjects.at( chargeName ).isMonitoring = true;
//        for( auto && it2 : chargeObj.dataObjects.at( chargeName ).pvMonStructs )
//        {
//            addToMonitorStructs( chargeMonitorStructs, it2.second, &chargeObj.dataObjects.at( chargeName )  );
//            chargeMonitorStructs.back()->isCollectingData = true;
//        }
//        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to charge Trace Monitors failed" );
//        if ( status == ECA_NORMAL )
//        {
//            message("success!!");
//            for( auto && it : chargeObj.dataObjects.at( chargeName ).isMonitoringMap )
//            {
//                    it.second = true;
//            }
//        }
//
//    }
//    else
//    {
//        message( "Already Monitoring Traces " ); /// make more useful
//    }
//}
//______________________________________________________________________________
void chargeInterface::resetChargeVectors( size_t N )
{
    chargeMonitorStructs.clear();
    for( auto && it : chargeObj.dataObjects )
    {
        it.second.voltageBuffer.clear();
        it.second.voltageBuffer.resize( N );
        it.second.timeStamps.clear();
        it.second.timeStamps.resize( N );
        it.second.strTimeStamps.clear();
        it.second.strTimeStamps.resize( N );
        for( auto && it2 : it.second.shotCounts )
        {
            it2.second = 0;
        }
    }
}
//______________________________________________________________________________
void chargeInterface::resetAChargeVector( const std::string chargeName, size_t N )
{
//    chargeMonitorStructs.at( chargeName ).clear();
    chargeObj.dataObjects.at( chargeName ).chargeVec.clear();
    chargeObj.dataObjects.at( chargeName ).chargeVec.resize( N );
    chargeObj.dataObjects.at( chargeName ).chargeBuffer.clear();
    chargeObj.dataObjects.at( chargeName ).chargeBuffer.resize( N );
    chargeObj.dataObjects.at( chargeName ).timeStamps.clear();
    chargeObj.dataObjects.at( chargeName ).timeStamps.resize( N );
    chargeObj.dataObjects.at( chargeName ).strTimeStamps.clear();
    chargeObj.dataObjects.at( chargeName ).strTimeStamps.resize( N );
    for( auto && it : chargeObj.dataObjects.at( chargeName ).shotCounts )
    {
        it.second = 0;
    }
}
//______________________________________________________________________________
const bool chargeInterface::isChargeBufferFull( const std::string & chargeName )
{
    bool isFull = false;
    if( chargeObj.dataObjects.at(chargeName).chargeShots > chargeObj.dataObjects.at(chargeName).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool chargeInterface::isChargeBufferNotFull( const std::string & chargeName )
{
    return !isChargeBufferFull( chargeName );
}
//______________________________________________________________________________
const bool chargeInterface::isVoltageBufferFull( const std::string & chargeName )
{
    bool isFull = false;
    if( chargeObj.dataObjects.at(chargeName).voltageShots > chargeObj.dataObjects.at(chargeName).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool chargeInterface::isVoltageBufferNotFull( const std::string & chargeName )
{
    return !isVoltageBufferFull( chargeName );
}
//______________________________________________________________________________
const bool chargeInterface::isWCMBufferFull()
{
    bool isFull = false;
    if( chargeObj.dataObjects.at(UTL::WCM).chargeShots > chargeObj.dataObjects.at(UTL::WCM).buffer && chargeObj.dataObjects.at(UTL::WCM).voltageShots > chargeObj.dataObjects.at(UTL::WCM).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool chargeInterface::isWCMBufferNotFull()
{
    return !isWCMBufferFull();
}
//______________________________________________________________________________
const bool chargeInterface::isS02FCUPBufferFull()
{
    bool isFull = false;
    if( chargeObj.dataObjects.at(UTL::S02_FCUP).chargeShots > chargeObj.dataObjects.at(UTL::S02_FCUP).buffer && chargeObj.dataObjects.at(UTL::S02_FCUP).voltageShots > chargeObj.dataObjects.at(UTL::S02_FCUP).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool chargeInterface::isS02FCUPBufferNotFull()
{
    return !isS02FCUPBufferFull();
}
//______________________________________________________________________________
const chargeStructs::dataObject & chargeInterface::getChargeDataStruct( const std::string & chargeName )
{
    if( entryExists( chargeObj.dataObjects, chargeName ) )
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
const chargeStructs::CHARGE_DIAG_TYPE chargeInterface::getDiagType( const std::string & chargeName )
{
    if( entryExists( chargeObj.dataObjects, chargeName ) )
    {
        return chargeObj.dataObjects.at( chargeName ).diagType;
    }
    else
    {
        message("ERROR!!!!! charge not defined in config file!!!!");
        return chargeStructs::CHARGE_DIAG_TYPE::UNKNOWN_CHARGE_DIAG;
    }
}
//______________________________________________________________________________
const std::string chargeInterface::getDiagTypeStr( const std::string & chargeName )
{
    return ENUM_TO_STRING(getDiagType( chargeName ));
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
double chargeInterface::getVoltage( const std::string & name )
{
    return chargeObj.dataObjects.at( name ).voltageVec.back();
}
//______________________________________________________________________________
double chargeInterface::getWCMVoltage()
{
    return getWCMVoltageBuffer().back();
}
//______________________________________________________________________________
double chargeInterface::getS02FCUPVoltage()
{
    return getS02FCUPVoltageBuffer().back();
}
//______________________________________________________________________________
double chargeInterface::getWCMCharge()
{
    return getWCMChargeBuffer().back();
}
//______________________________________________________________________________
double chargeInterface::getS02FCUPCharge()
{
    return getS02FCUPChargeBuffer().back();
}
//______________________________________________________________________________
void chargeInterface::setBufferSize( size_t bufferSize )
{
    for( auto && it : chargeObj.dataObjects )
    {
        it.second.chargeBuffer.clear();
        it.second.chargeBuffer.resize( bufferSize );
        it.second.voltageBuffer.clear();
        it.second.voltageBuffer.resize( bufferSize );
        it.second.timeStampsBuffer.clear();
        it.second.timeStampsBuffer.resize( bufferSize );
        it.second.buffer = bufferSize;
        it.second.chargeShots = UTL::ZERO_INT;
        it.second.voltageShots = UTL::ZERO_INT;
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
boost::circular_buffer< double > chargeInterface::getVoltageBuffer( const std::string & name )
{
    if( entryExists( chargeObj.dataObjects, name ) )
    {
        return chargeObj.dataObjects.at( name ).voltageBuffer;
    }
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getWCMVoltageBuffer()
{
    boost::circular_buffer< double > wcmQ;
    for( auto && it : chargeObj.dataObjects )
    {
        if( it.second.diagType == chargeStructs::CHARGE_DIAG_TYPE::WCM )
        {
            wcmQ = it.second.voltageBuffer;
        }
    }
    return wcmQ;
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getS02FCUPVoltageBuffer()
{
    boost::circular_buffer< double > fcupQ;
    for( auto && it : chargeObj.dataObjects )
    {
        if( it.second.diagType == chargeStructs::CHARGE_DIAG_TYPE::S02_FCUP )
        {
            fcupQ = it.second.voltageBuffer;
        }
    }
    return fcupQ;
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getWCMChargeBuffer()
{
    boost::circular_buffer< double > wcmQ;
    for( auto && it : chargeObj.dataObjects )
    {
        if( it.second.diagType == chargeStructs::CHARGE_DIAG_TYPE::WCM )
        {
            wcmQ = it.second.chargeBuffer;
        }
    }
    return wcmQ;
}
//______________________________________________________________________________
boost::circular_buffer< double > chargeInterface::getS02FCUPChargeBuffer()
{
    boost::circular_buffer< double > fcupQ;
    for( auto && it : chargeObj.dataObjects )
    {
        if( it.second.diagType == chargeStructs::CHARGE_DIAG_TYPE::S02_FCUP )
        {
            fcupQ = it.second.chargeBuffer;
        }
    }
    return fcupQ;
}
//______________________________________________________________________________
std::vector< std::string > chargeInterface::getChargeDiagnosticNames()
{
    std::vector< std::string > chargeNames;
    for( auto && iter : chargeObj.dataObjects )
    {
        message( "charge Name ", iter.first );
        chargeNames.push_back( iter.first );
    }
    return chargeNames;
}
//______________________________________________________________________________
std::vector< std::string > chargeInterface::getChargePVs()
{
    std::vector< std::string > chargeNames;
    for( auto && iter : chargeObj.dataObjects )
    {
        for( auto && iter2 : iter.second.pvMonStructs )
        {
            std::stringstream s;
            message( "charge PV ", iter.second.pvRoot, ":", iter2.second.pvSuffix );
            s << iter.second.pvRoot << ":" << iter2.second.pvSuffix;
            chargeNames.push_back( s.str() );
        }
        for( auto && iter2 : iter.second.pvComStructs )
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
    auto iter = allChargeData.find( objName );
    if( iter != allChargeData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  chargeInterface::getILockStatesStr( const std::string & name )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allChargeData.find( name );
    if( iter != allChargeData.end() )
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
