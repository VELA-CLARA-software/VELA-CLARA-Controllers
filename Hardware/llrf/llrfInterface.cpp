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

#include "llrfInterface.h"
//djs
#include "dburt.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
llrfInterface::llrfInterface( const std::string &laserConf,
                                const bool startVirtualMachine,
                                const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                                const bool shouldStartEPICs ):
configReader(laserConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs( shouldStartEPICs )
{
//    if( shouldStartEPICs )
//    message("magnet llrfInterface shouldStartEPICs is true");
//    else
//    message("magnet llrfInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
llrfInterface::~llrfInterface()
{
    killILockMonitors();
    for( auto && it : continuousMonitorStructs )
    {
        killMonitor( it );
        delete it;
    }
//    debugMessage( "llrfInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void llrfInterface::killMonitor( llrfStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    //if( status == ECA_NORMAL)
        //debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
//    else
        //debugMessage("ERROR llrfInterface: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}
//______________________________________________________________________________
void llrfInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if( configFileRead )
    {
        message("The llrfInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if( getDataSuccess )
        {

            if( shouldStartEPICs )
            {
                message("The llrfInterface has acquired objects, connecting to EPICS");
                //std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions
                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
            }
            else
             message("The llrfInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message( "!!!The llrfInterface received an Error while getting magnet data!!!" );
    }
}
//______________________________________________________________________________
bool llrfInterface::initObjects()
{
//    bool magDatSuccess = configReader.getMagData( allMagnetData );
//    degStruct = configReader.getDeguassStruct();
//    addDummyElementToAllMAgnetData();
//    // set the machine area on each magent, this allows for flavour switching functions, such as switchON etc..
//    for( auto && it : allMagnetData )
//        it.second.machineArea = myMachineArea;
    return false;
}
//______________________________________________________________________________
void llrfInterface::initChids()
{
//    message( "\n", "Searching for Magnet chids...");
//    for( auto && magObjIt : allMagnetData )
//    {   // The correctors and BSOL PSU are all EBT-INJ-MAG-HVCOR-01, they have the psuRoot field non-empty
//        // to make this more clever we should just have one value they all point to... ?
//        if( isACor( magObjIt.first ) || isABSol( magObjIt.first ) )
//        {
//            addILockChannels( magObjIt.second.numIlocks, magObjIt.second.psuRoot, magObjIt.first, magObjIt.second.iLockPVStructs );
//            for( auto && it2 : magObjIt.second.pvComStructs )
//            {
//                addChannel( magObjIt.second.psuRoot, it2.second );
//            }
//            for( auto && it2 : magObjIt.second.pvMonStructs  )
//            {// yeah - the Sta is for the psuRoot, not the pvRoot... :-(
//                if( it2.first == magnetStructs::MAG_PV_TYPE::Sta )
//                    addChannel( magObjIt.second.psuRoot, it2.second );
//                else
//                    addChannel( magObjIt.second.pvRoot, it2.second );
//            }
//        }
//        else
//        {
//            addILockChannels( magObjIt.second.numIlocks, magObjIt.second.pvRoot, magObjIt.first, magObjIt.second.iLockPVStructs );
//            for( auto && it2 : magObjIt.second.pvComStructs )
//                addChannel( magObjIt.second.pvRoot, it2.second );
//            for( auto && it2 : magObjIt.second.pvMonStructs  )
//                addChannel( magObjIt.second.pvRoot, it2.second );
//        }// add in the NR-Type psu chids
//        if( isNRorNRGanged( magObjIt.first ) )
//        {
//            addILockChannels( magObjIt.second.nPSU.numIlocks, magObjIt.second.nPSU.pvRoot, magObjIt.second.nPSU.parentMagnet+" N-PSU", magObjIt.second.nPSU.iLockPVStructs );
//            addILockChannels( magObjIt.second.rPSU.numIlocks, magObjIt.second.rPSU.pvRoot, magObjIt.second.rPSU.parentMagnet+" R-PSU", magObjIt.second.rPSU.iLockPVStructs );
//            for( auto && it2 : magObjIt.second.nPSU.pvComStructs )
//                addChannel( magObjIt.second.nPSU.pvRoot, it2.second );
//            for( auto && it2 : magObjIt.second.nPSU.pvMonStructs )
//                addChannel( magObjIt.second.nPSU.pvRoot, it2.second );
//            for( auto && it2 : magObjIt.second.rPSU.pvComStructs )
//                addChannel( magObjIt.second.rPSU.pvRoot, it2.second );
//            for( auto && it2 : magObjIt.second.rPSU.pvMonStructs )
//                addChannel( magObjIt.second.rPSU.pvRoot, it2.second );
//        }
//    }
//    int status = sendToEpics( "ca_create_channel", "Found Magnet ChIds.", "!!TIMEOUT!! Not all Magnet ChIds found." );
//    if( status == ECA_TIMEOUT )
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));//MAGIC_NUMBER
//        for( const auto & magObjIt : allMagnetData )
//        {
//            message("\n", "Checking Chids for ", magObjIt.first );
//            for( auto & it2 : magObjIt.second.pvMonStructs )
//                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//            for( auto & it2 : magObjIt.second.pvComStructs )
//                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//            for( auto & it2 : magObjIt.second.iLockPVStructs )
//                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//
//            if( isNRorNRGanged( magObjIt.first ) )
//            {
//                message("\n", "Checking Chids for ", magObjIt.first + " N-PSU" );
//                for( auto && it2 : magObjIt.second.nPSU.pvComStructs )
//                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//                for( auto && it2 : magObjIt.second.nPSU.pvMonStructs )
//                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//
//                message("\n", "Checking Chids for ", magObjIt.first + " R-PSU" );
//                for( auto && it2 : magObjIt.second.rPSU.pvComStructs )
//                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//                for( auto && it2 : magObjIt.second.rPSU.pvMonStructs )
//                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
//            }
//        }
//        message("");
//        std::this_thread::sleep_for(std::chrono::milliseconds( 5000 )); // MAGIC_NUMBER
//    }
//    else if ( status == ECA_NORMAL )
//        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void llrfInterface::addChannel( const std::string & pvRoot, llrfStructs::pvStruct & pv )
{
//    std::string s1 = pvRoot + pv.pvSuffix;
//    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );//MAGIC_NUMBER
//    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void llrfInterface::startMonitors()
{
//    continuousMonitorStructs.clear();
//    continuousILockMonitorStructs.clear();
//    for( auto && it : allMagnetData )
//    {
//        /// Base class function for ilocks
//        monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
//        if( isNRorNRGanged( it.first ) )
//        {   // add NR-PSU ilocks
//            monitorIlocks( it.second.nPSU.iLockPVStructs, it.second.nPSU.iLockStates );
//            monitorIlocks( it.second.rPSU.iLockPVStructs, it.second.rPSU.iLockStates );
//            // add NR-PSU pvMon Structs
//            for( auto && it2 : it.second.nPSU.pvMonStructs  )
//            {
//                continuousMonitorStructs.push_back( new magnetStructs::monitorStruct() );
//                continuousMonitorStructs.back() -> monType   = it2.first;
//                continuousMonitorStructs.back() -> objName   = it.second.nPSU.parentMagnet;
//                continuousMonitorStructs.back() -> psuType   = magnetStructs::MAG_PSU_TYPE::PSU_N;
//                continuousMonitorStructs.back() -> interface = this;
//                ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
//                                        it2.second.MASK, llrfInterface::staticEntryMagnetMonitor,
//                                        (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back() -> EVID );
//            }
//            for( auto && it2 : it.second.rPSU.pvMonStructs )
//            {
//                continuousMonitorStructs.push_back( new magnetStructs::monitorStruct() );
//                continuousMonitorStructs.back() -> monType   = it2.first;
//                continuousMonitorStructs.back() -> objName   = it.second.rPSU.parentMagnet;
//                continuousMonitorStructs.back() -> psuType   = magnetStructs::MAG_PSU_TYPE::PSU_R;
//                continuousMonitorStructs.back() -> interface = this;
//                ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
//                                        it2.second.MASK, llrfInterface::staticEntryMagnetMonitor,
//                                        (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back() -> EVID );
//            }
//        }// Then add main monitors...
//        for( auto && it2 : it.second.pvMonStructs )
//        {
//            continuousMonitorStructs.push_back( new magnetStructs::monitorStruct() );
//            continuousMonitorStructs.back() -> monType   = it2.first;
//            continuousMonitorStructs.back() -> objName = it.second.name;
//            continuousMonitorStructs.back() -> psuType = magnetStructs::MAG_PSU_TYPE::PSU;
//            continuousMonitorStructs.back() -> interface = this;
//            ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
//                                    it2.second.MASK, llrfInterface::staticEntryMagnetMonitor,
//                                    (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back()->EVID );
//            /// If you pass DBF_STRING and recast as a char * in the callback you can get the state as GOOD, BAD, OPEN, CLOSED etc,
//            /// This is useful for debugging, but in general i'm just going to subscribe to the DBR_ENUM
//            /// ca_create_subscription accepts a void * user argument, we pass a pointer to the monitor struct,
//            /// in the callback function this is cast back and the data can then be updated
//            /// void * usrArg = reinterpret_cast< void *>( continuousMonitorStructs.back() );
//        }
//    }
//    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Magnet Monitors", "!!TIMEOUT!! Subscription to Magnet monitors failed" );
//    if ( status == ECA_NORMAL )
//        allMonitorsStarted = true; /// interface base class member
}

