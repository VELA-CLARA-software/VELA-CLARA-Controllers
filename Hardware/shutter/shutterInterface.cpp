/*
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
//
//  Author:      DJS
//  Last edit:   29-03-2018
//  FileName:    shutterInterface.cpp
//  Description:
//
//
//*/
// project includes
#include "shutterInterface.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
shutterInterface::shutterInterface(bool& show_messages,
                                   bool& show_debug_messages,
                                   const bool startVirtualMachine,
                                   const bool shouldStartEPICs,
                                   const std::string& configFile
                                  ):
configReader(configFile,  show_messages, show_debug_messages,startVirtualMachine),
interface(show_messages, show_debug_messages)
{
    //initialise();
}
//______________________________________________________________________________
shutterInterface::~shutterInterface()
{
//    for( auto it : continuousMonitorStructs )
//    {
//        debugMessage("delete shutterInterface continuousMonitorStructs entry.");
//        delete it;
//    }
}

//void shutterInterface::initialise()
//{
//    /// The config file reader
//    configFileRead = configReader.readPILShutterConfig();
//    if( configFileRead )
//    {
//        /// initialise the objects based on what is read from the config file
//        initObjects();
//        /// subscribe to the channel ids
//        initChids();
//        /// start the monitors: set up the callback functions
//        startMonitors();
//        /// The pause allows EPICS to catch up.
//        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); // MAGIC NUMBER
//    }
//}
////______________________________________________________________________________
//void shutterInterface::initObjects()
//{
//    const std::vector< shutterStructs::shutterObject > shutterObjs = configReader.getShutterObjects();
//    for( auto const & it : shutterObjs )
//        allShutterData[it.name] = it;
//}
////______________________________________________________________________________
//void shutterInterface::initChids()
//{
//    message( "\n", "Searching for Shutter chids...");
//    for( auto && it1 : allShutterData )
//    {
//        addILockChannels( it1.second.numIlocks, it1.second.pvRoot, it1.second.name, it1.second.iLockPVStructs );
//        for( auto && it2 : it1.second.pvComStructs )
//            addChannel( it1.second.pvRoot, it2.second );
//        for( auto && it2 : it1.second.pvMonStructs  )
//            addChannel( it1.second.pvRoot, it2.second );
//    }
//    int status = sendToEpics( "ca_create_channel", "Found Shutter chids.", "!!TIMEOUT!! Not all shutter ChIds found." );
//    if( status == ECA_TIMEOUT )
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
//        for( const auto & it : allShutterData )
//        {
//            message("\n", "Checking Chids for ", it.first );
//            for( auto & it2 : it.second.pvMonStructs )
//                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
//            for( auto & it2 : it.second.pvComStructs )
//                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
//            for( auto & it2 : it.second.iLockPVStructs )
//                checkCHIDState( it2.second.CHID, it2.second.pv );
//        }
//        message("");
//    }
//    else if ( status == ECA_NORMAL )
//        allChidsInitialised = true;  /// interface base class member
//}
////______________________________________________________________________________
//void shutterInterface::addChannel( const std::string & pvRoot, shutterStructs::pvStruct & pv )
//{
//    std::string s1 = pvRoot + pv.pvSuffix;
//    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );
//    debugMessage( "Create channel to ", s1 );
//}
////______________________________________________________________________________
//void shutterInterface::checkShutterCHIDStates()
//{
//    std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC
//    for( auto & it : allShutterData )
//    {
//        message("\n", "Checking Chids for ", it.first );
//        for( auto & it2 : it.second.pvMonStructs )
//            checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
//        for( auto & it2 : it.second.pvComStructs )
//            checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
//        for( auto & it2 : it.second.iLockPVStructs )
//            checkCHIDState( it2.second.CHID, it2.second.pv );
//    }
//    message("");
//}
////______________________________________________________________________________
//void shutterInterface::startMonitors()
//{
//    continuousMonitorStructs.clear();
//    continuousILockMonitorStructs.clear();
//    for( auto && it : allShutterData )
//    {
//        /// Base class function
//        monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
//        for( auto && it2 : it.second.pvMonStructs  )
//        {
//            /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
//            /// init structs 'correctly'
//
//            continuousMonitorStructs.push_back( new shutterStructs::monitorStuct() );
//            continuousMonitorStructs.back() -> shutObj   = &( it.second );
//            continuousMonitorStructs.back() -> interface = this;
//
//            /// For the shutter there is only 1 monitor type
//            /// maybe it.second.pvMonStructs does not need to be a map???, pvComStructs probably does though...
//            //continuousMonitorStructs.back().monType   = it2.first;
//
//            /// If you pass DBF_STRING and recast as a char * in the callback you can get the state as GOOD, BAD, OPEN, CLOSED etc,
//            /// This is useful for debugging, but in general i'm just going to subscribe to the DBR_ENUM
//
//            /// ca_create_subscription accepts a void * user argument, we pass a pointer to the monitor struct,
//            /// in the callback function this is cast back and the data can then be updated
//            /// void * usrArg = reinterpret_cast< void *>( continuousMonitorStructs.back() );
//
//            ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID, it2.second.MASK,  shutterInterface::staticEntryShutterMonitor, (void*)continuousMonitorStructs.back(), 0); // &continuousMonitorStructs.back().EventID );
//        }
//    }
//    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Shutter Monitors", "!!TIMEOUT!! Subscription to Shutter monitors failed" );
//    if ( status == ECA_NORMAL )
//        allMonitorsStarted = true; /// interface base class member
//}
////______________________________________________________________________________
//void shutterInterface::staticEntryShutterMonitor( const event_handler_args args )
//{
//    /// recast args.usr ( a void * ) to a monitor struct pointer
//
//    shutterStructs::monitorStuct * ms = reinterpret_cast<shutterStructs::monitorStuct*>(args.usr);
//
//    /// Not sure how to decode these apart from trial and error
//    /// you can test with DBF_STRING as the callback type
//
//    if( *(unsigned short*)args.dbr == 0 )
//        ms -> shutObj -> shutterState = HWC_ENUM::SHUTTER_STATE::SHUTTER_CLOSED;
//    else if( *(unsigned short*)args.dbr == 1 )
//        ms -> shutObj -> shutterState = HWC_ENUM::SHUTTER_STATE::SHUTTER_OPEN;
//    else
//        ms -> shutObj -> shutterState = HWC_ENUM::SHUTTER_STATE::SHUTTER_ERROR;
//
//    /// make debug messages easier to understand by using ENUM_TO_STRING
//    ms -> interface -> debugMessage( ms -> shutObj -> name,  " new state = ", ENUM_TO_STRING(ms->shutObj -> shutterState ));
//    /// If subscribed to DBF_STRING use this to get the message
//    //char * val = (char *)args.dbr;
//    /// now we can switch based on the monitor type and then update the correct part of the shutter object data using val
//    /// For the shutter this is basically redundant, there is only one monitor the "Sta"
//    /// (apart from interlocks, these are handled in the base class)
////    switch( lms.monType )
////    {
////        case shutterStructs::Sta:
////                ....
////        case SomeOtherPVType:
////                ....
////    }
//}
//
//     __   __                         __   __
//    /  ` /  \  |\/|  |\/|  /\  |\ | |  \ /__`
//    \__, \__/  |  |  |  | /~~\ | \| |__/ .__/
//
//______________________________________________________________________________
void shutterInterface::open( const std::string & name )
{
//    if( isClosed( name ) )
//        toggleShutter( allShutterData[name].pvComStructs[shutterStructs::SHUTTER_PV_TYPE::On].CHTYPE,
//                        allShutterData[name].pvComStructs[shutterStructs::SHUTTER_PV_TYPE::On].CHID,
//                        "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO SHUTTER OPEN",
//                        "!!TIMEOUT!! FAILED TO SEND SHUTTER OPEN" );
}
//______________________________________________________________________________
void shutterInterface::close( const std::string & name )
{
//    if( isOpen( name ) )
//        toggleShutter( allShutterData[name].pvComStructs[shutterStructs::SHUTTER_PV_TYPE::Off].CHTYPE,
//            allShutterData[name].pvComStructs[shutterStructs::SHUTTER_PV_TYPE::Off].CHID,
//            "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO SHUTTER CLOSE",
//            "!!TIMEOUT!! FAILED TO SEND SHUTTER CLOSE" );
}
////______________________________________________________________________________
//void shutterInterface::toggleShutter( chtype &cht, chid &chi, const char *m1, const char *m2 )
//{
//    int status = caput( cht, chi, EPICS_ACTIVATE, "", m1 );
//    if( status == ECA_NORMAL )
//        caput( cht, chi, EPICS_SEND, "", m2 );
//}
//______________________________________________________________________________
bool shutterInterface::isOpen( const std::string & name ) const
{
    bool ret = false;
//    if(  entryExists( allShutterData, name ) )
//        if( allShutterData[name].shutterState == HWC_ENUM::SHUTTER_STATE::SHUTTER_OPEN )
//            ret = true;
//    if( ret )
//        debugMessage( name, " is open");
//    else
//        debugMessage( name, " is NOT open");
    return ret;
}
//______________________________________________________________________________
bool shutterInterface::isClosed( const std::string & name ) const
{
    bool ret = false;
//    if(  entryExists( allShutterData, name ) )
//        if( allShutterData[name].shutterState == HWC_ENUM::SHUTTER_STATE::SHUTTER_CLOSED )
//            ret = true;
//    if( ret )
//        debugMessage( name, " is closed");
//    else
//        debugMessage( name, " is NOT closed");
    return ret;
}
//______________________________________________________________________________
bool shutterInterface::openAndWait( const std::string & name, const time_t waitTime )
{
//    return toggleShutterAndWait(&shutterInterface::isClosed, &shutterInterface::isOpen, &shutterInterface::open, name, waitTime, *this);
    return false;
}
//______________________________________________________________________________
bool shutterInterface::closeAndWait( const std::string & name, const time_t waitTime )
{
//    return toggleShutterAndWait(&shutterInterface::isOpen, &shutterInterface::isClosed, &shutterInterface::close, name, waitTime, *this);
    return false;
}
//______________________________________________________________________________
//bool shutterInterface::toggleShutterAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, shutterInterface & obj )
//{
//    if( CALL_MEMBER_FN(obj, f1)( name ) ) /// CALL_MEMBER_FN MACRO in structs.h
//    {
//        CALL_MEMBER_FN(obj, f3)( name ) ;
//        time_t timeStart = time( 0 );  /// Start Clock
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
////      __   ___ ___ ___  ___  __   __
////     / _` |__   |   |  |__  |__) /__`
////     \__> |___  |   |  |___ |  \ .__/
////
////______________________________________________________________________________
//void shutterInterface::getShutterNames( std::vector< std::string >  & shutterNames )
//{
//    shutterNames.clear();
//    for( auto const & it : allShutterData )
//        shutterNames.push_back( it.first );
//}
////______________________________________________________________________________
//shutterStructs::shutterObject * shutterInterface::getShutterObject( const std::string &name )
//{
//    if( entryExists( allShutterData, name ) )
//        return &allShutterData[name];
//    else
//        return nullptr;
//}
////______________________________________________________________________________
//HWC_ENUM::SHUTTER_STATE shutterInterface::getShutterState( const  std::string &name )
//{
//    HWC_ENUM::SHUTTER_STATE r = HWC_ENUM::SHUTTER_STATE::SHUTTER_ERROR;
//    if( entryExists( allShutterData, name ) )
//        r = allShutterData[name].shutterState;
//    return r;
//}
////______________________________________________________________________________
//std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > shutterInterface::getILockStates( const std::string & name )
//{
//    if( entryExists( allShutterData, name ) )
//        return allShutterData[name].iLockStates;
//    else
//    {
//        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > r = {{HWC_ENUM::ILOCK_NUMBER::ILOCK_ERR, HWC_ENUM::ILOCK_STATE::ILOCK_ERROR}};
//        return r;
//    }
//}
////______________________________________________________________________________
//std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  shutterInterface::getILockStatesStr( const std::string & name )
//{
//    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
//
//    if( entryExists( allShutterData, name ) )
//        for( auto it : allShutterData[name].iLockStates )
//            r[it.first] = ENUM_TO_STRING( it.second );
//    else
//        r[HWC_ENUM::ILOCK_NUMBER::ILOCK_ERR] = ENUM_TO_STRING( HWC_ENUM::ILOCK_STATE::ILOCK_ERROR );
//    return r;
//}
//         ___
//    |  |  |  |
//    \__/  |  |___
//
//______________________________________________________________________________
