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

#include "magnetInterface.h"
//djs
#include "dburt.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
magnetInterface::magnetInterface( const std::string &magConf, const std::string &NRConf,
                                  const bool startVirtualMachine,
                                  const bool *show_messages_ptr, const bool* show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const magnetStructs::MAG_CONTROLLER_TYPE myControllerType ):
configReader( magConf, NRConf, startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface( show_messages_ptr, show_debug_messages_ptr ), degaussNum( 0 ), dummyName("DUMMY"),//MAGIC_NUMBER
shouldStartEPICs( shouldStartEPICs ), myControllerType(myControllerType)
{
//    if( shouldStartEPICs )
//    message("magnet magnetInterface shouldStartEPICs is true");
//    else
//    message("magnet magnetInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
magnetInterface::~magnetInterface()
{
//    debugMessage( "magnetInterface DESTRUCTOR CALLED");
    for (auto && it : degaussStructsMap )
    {
        debugMessage("in magnetInterface: delete degauss thread ", it.first );
        it.second.thread->join();
        delete it.second.thread;
    }
    killILockMonitors();
    for( auto && it : continuousMonitorStructs )
    {
        killMonitor( it );
        debugMessage("delete ", it -> objName, " ", ENUM_TO_STRING(it->monType), " continuousMonitorStructs entry.");
        delete it;
    }
//    debugMessage( "magnetInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void magnetInterface::killMonitor( magnetStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
        debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR magnetInterface: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}
//______________________________________________________________________________
void magnetInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions
                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
            }
            else
                message("The magnetInterface Read Config files, Not Starting EPICS Monitors" );
        }
        else
            message( "!!!The magnetInterface received an Error while getting magnet data!!!" );
    }
}
//______________________________________________________________________________
bool magnetInterface::initObjects()
{
    bool magDatSuccess = configReader.getInjMagData( allMagnetData );
    degStruct = configReader.getDeguassStruct();
    addDummyElementToAllMAgnetData();
    return magDatSuccess;
}
//______________________________________________________________________________
void magnetInterface::initChids()
{
    message( "\n", "Searching for Magnet chids...");
    for( auto && magObjIt : allMagnetData )
    {   // The correctors and BSOL PSU are all EBT-INJ-MAG-HVCOR-01, they have the psuRoot field non-empty
        // to make this more clever we should just have one value they all point to... ?
        if( isACor( magObjIt.first ) || isABSol( magObjIt.first ) )
        {
            addILockChannels( magObjIt.second.numIlocks, magObjIt.second.psuRoot, magObjIt.first, magObjIt.second.iLockPVStructs );
            for( auto && it2 : magObjIt.second.pvComStructs )
            {
                addChannel( magObjIt.second.psuRoot, it2.second );
            }
            for( auto && it2 : magObjIt.second.pvMonStructs  )
            {// yeah - the Sta is for the psuRoot, not the pvRoot... :-(
                if( it2.first == magnetStructs::MAG_PV_TYPE::Sta )
                    addChannel( magObjIt.second.psuRoot, it2.second );
                else
                    addChannel( magObjIt.second.pvRoot, it2.second );
            }
        }
        else
        {
            addILockChannels( magObjIt.second.numIlocks, magObjIt.second.pvRoot, magObjIt.first, magObjIt.second.iLockPVStructs );
            for( auto && it2 : magObjIt.second.pvComStructs )
                addChannel( magObjIt.second.pvRoot, it2.second );
            for( auto && it2 : magObjIt.second.pvMonStructs  )
                addChannel( magObjIt.second.pvRoot, it2.second );
        }// add in the NR-Type psu chids
        if( isNRorNRGanged( magObjIt.first ) )
        {
            addILockChannels( magObjIt.second.nPSU.numIlocks, magObjIt.second.nPSU.pvRoot, magObjIt.second.nPSU.parentMagnet+" N-PSU", magObjIt.second.nPSU.iLockPVStructs );
            addILockChannels( magObjIt.second.rPSU.numIlocks, magObjIt.second.rPSU.pvRoot, magObjIt.second.rPSU.parentMagnet+" R-PSU", magObjIt.second.rPSU.iLockPVStructs );
            for( auto && it2 : magObjIt.second.nPSU.pvComStructs )
                addChannel( magObjIt.second.nPSU.pvRoot, it2.second );
            for( auto && it2 : magObjIt.second.nPSU.pvMonStructs )
                addChannel( magObjIt.second.nPSU.pvRoot, it2.second );
            for( auto && it2 : magObjIt.second.rPSU.pvComStructs )
                addChannel( magObjIt.second.rPSU.pvRoot, it2.second );
            for( auto && it2 : magObjIt.second.rPSU.pvMonStructs )
                addChannel( magObjIt.second.rPSU.pvRoot, it2.second );
        }
    }
    int status = sendToEpics( "ca_create_channel", "Found Magnet ChIds.", "!!TIMEOUT!! Not all Magnet ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));//MAGIC_NUMBER
        for( const auto & magObjIt : allMagnetData )
        {
            message("\n", "Checking Chids for ", magObjIt.first );
            for( auto & it2 : magObjIt.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
            for( auto & it2 : magObjIt.second.pvComStructs )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
            for( auto & it2 : magObjIt.second.iLockPVStructs )
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );

            if( isNRorNRGanged( magObjIt.first ) )
            {
                message("\n", "Checking Chids for ", magObjIt.first + " N-PSU" );
                for( auto && it2 : magObjIt.second.nPSU.pvComStructs )
                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
                for( auto && it2 : magObjIt.second.nPSU.pvMonStructs )
                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );

                message("\n", "Checking Chids for ", magObjIt.first + " R-PSU" );
                for( auto && it2 : magObjIt.second.rPSU.pvComStructs )
                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
                for( auto && it2 : magObjIt.second.rPSU.pvMonStructs )
                    checkCHIDState( it2.second.CHID, ENUM_TO_STRING( it2.first ) );
            }
        }
        message("");
        std::this_thread::sleep_for(std::chrono::milliseconds( 5000 )); // MAGIC_NUMBER
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void magnetInterface::addChannel( const std::string & pvRoot, magnetStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void magnetInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();
    for( auto && it : allMagnetData )
    {
        /// Base class function for ilocks
        monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
        if( isNRorNRGanged( it.first ) )
        {   // add NR-PSU ilocks
            monitorIlocks( it.second.nPSU.iLockPVStructs, it.second.nPSU.iLockStates );
            monitorIlocks( it.second.rPSU.iLockPVStructs, it.second.rPSU.iLockStates );
            // add NR-PSU pvMon Structs
            for( auto && it2 : it.second.nPSU.pvMonStructs  )
            {
                continuousMonitorStructs.push_back( new magnetStructs::monitorStruct() );
                continuousMonitorStructs.back() -> monType   = it2.first;
                continuousMonitorStructs.back() -> objName   = it.second.nPSU.parentMagnet;
                continuousMonitorStructs.back() -> psuType   = magnetStructs::MAG_PSU_TYPE::PSU_N;
                continuousMonitorStructs.back() -> interface = this;
                ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
                                        it2.second.MASK, magnetInterface::staticEntryMagnetMonitor,
                                        (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back() -> EVID );
            }
            for( auto && it2 : it.second.rPSU.pvMonStructs )
            {
                continuousMonitorStructs.push_back( new magnetStructs::monitorStruct() );
                continuousMonitorStructs.back() -> monType   = it2.first;
                continuousMonitorStructs.back() -> objName   = it.second.rPSU.parentMagnet;
                continuousMonitorStructs.back() -> psuType   = magnetStructs::MAG_PSU_TYPE::PSU_R;
                continuousMonitorStructs.back() -> interface = this;
                ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
                                        it2.second.MASK, magnetInterface::staticEntryMagnetMonitor,
                                        (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back() -> EVID );
            }
        }// Then add main monitors...
        for( auto && it2 : it.second.pvMonStructs )
        {
            continuousMonitorStructs.push_back( new magnetStructs::monitorStruct() );
            continuousMonitorStructs.back() -> monType   = it2.first;
            continuousMonitorStructs.back() -> objName = it.second.name;
            continuousMonitorStructs.back() -> psuType = magnetStructs::MAG_PSU_TYPE::PSU;
            continuousMonitorStructs.back() -> interface = this;
            ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID,
                                    it2.second.MASK, magnetInterface::staticEntryMagnetMonitor,
                                    (void*)continuousMonitorStructs.back(), &continuousMonitorStructs.back()->EVID );
            /// If you pass DBF_STRING and recast as a char * in the callback you can get the state as GOOD, BAD, OPEN, CLOSED etc,
            /// This is useful for debugging, but in general i'm just going to subscribe to the DBR_ENUM
            /// ca_create_subscription accepts a void * user argument, we pass a pointer to the monitor struct,
            /// in the callback function this is cast back and the data can then be updated
            /// void * usrArg = reinterpret_cast< void *>( continuousMonitorStructs.back() );
        }
    }
    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Magnet Monitors", "!!TIMEOUT!! Subscription to Magnet monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
void magnetInterface::staticEntryMagnetMonitor( const event_handler_args args)
{
    magnetStructs::monitorStruct*ms = static_cast<  magnetStructs::monitorStruct *>(args.usr);
    switch( ms -> monType )
    {
        case magnetStructs::MAG_PV_TYPE::RI:
            ms->interface->updateRI( *(double*)args.dbr, ms->objName );
            break;
        case magnetStructs::MAG_PV_TYPE::SI:
            ms->interface->updateSI( *(double*)args.dbr, ms->objName );
            break;
        case magnetStructs::MAG_PV_TYPE::Sta:
            ms->interface->updatePSUSta( *(unsigned short*)args.dbr, ms->objName, ms->psuType );
            break;
        default:
            ms->interface->debugMessage( "!!! ERROR !!! Unknown Monitor Type passed to magnetInterface::staticEntryMagnetMonitor" );
            break;
    }
}
//______________________________________________________________________________
void magnetInterface::updateRI( const double value, const  std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
    {
        allMagnetData[magName].ri = roundToN( value, 4 ); /// MAGIC_NUMBER
        updateRI_WithPol( magName );
        if( areNotSame( allMagnetData[magName].ri, value, allMagnetData[magName].riTolerance ) ) /// this is to stop loads of annoying callbacks being printed to screen
            debugMessage(magName," NEW RI = ", allMagnetData[magName].riWithPol );
    }
}
//______________________________________________________________________________
void magnetInterface::updateRI_WithPol( const  std::string & magName )
{
    allMagnetData[magName].riWithPol = allMagnetData[magName].ri;
    if( isON_psuR( magName ) )
        allMagnetData[magName].riWithPol *= -1.0; /// MAGIC_NUMBER
}
//______________________________________________________________________________
void magnetInterface::updateSI( const double value, const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
    {
        allMagnetData[magName].si = value;//roundToN( value, 4 ); /// MAGIC_NUMBER;
        updateSI_WithPol( magName );
        debugMessage(magName," NEW SI = ", allMagnetData[magName].siWithPol);
    }
}
//______________________________________________________________________________
void magnetInterface::updateSI_WithPol( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
    {
        allMagnetData[magName].siWithPol = allMagnetData[magName].si;
            if( isNRorNRGanged( magName  )  )
                if( isON_psuR( magName ) )
                    allMagnetData[magName].siWithPol *= -1.0; /// MAGIC_NUMBER
    }
}
//______________________________________________________________________________
void magnetInterface::updatePSUSta( const unsigned short value, const std::string & magName, const magnetStructs::MAG_PSU_TYPE psuType)
{
    if( entryExists(allMagnetData, magName ) )
    {
        VELA_ENUM::MAG_PSU_STATE newstate = VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR;
        switch( value )
        {
            case 0:
                newstate = VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF;
                break;
            case 1:
                newstate =  VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON;
                break;
            case 2:
                newstate =  VELA_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING;
                break;
            default:
                newstate =  VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR;
        }
        switch( psuType )
        {
            case magnetStructs::MAG_PSU_TYPE::PSU:
                allMagnetData[ magName ].psuState  = newstate;
                //debugMessage( magName, " New PSU State = ", ENUM_TO_STRING( allMagnetData[ magName ].psuState ) );
                /// If the polarity has changed we need to change SI & RI
                updateSI_WithPol( magName );
                updateRI_WithPol( magName );
                break;
            case magnetStructs::MAG_PSU_TYPE::PSU_R:
                allMagnetData[ magName ].rPSU.psuState  = newstate;
                //debugMessage( magName, " New PSU_R State = ", ENUM_TO_STRING( allMagnetData[ magName ].rPSU.psuState ) );
                updateSI_WithPol( magName );
                updateRI_WithPol( magName );
                break;
            case magnetStructs::MAG_PSU_TYPE::PSU_N:
                allMagnetData[ magName ].nPSU.psuState  = newstate;
                //debugMessage( magName, " New PSU_N State = ", ENUM_TO_STRING( allMagnetData[ magName ].nPSU.psuState ) );
                break;
            default:
                ;
        }
    }
}
///           ___  ____
///  ___     / __)(_  _)
/// (___)    \__ \ _)(_
///          (___/(____)
///
/// These are wrapper functions that interface to the main setSI function setSI_MAIN()
//______________________________________________________________________________
bool magnetInterface::setSI( const std::string & magName, const double value)
{// yay - c++11 has initilization lists ...
    const vec_s mags = { magName };
    const vec_d   vals = { value   };
    return setSI( mags, vals);
}
//______________________________________________________________________________
bool magnetInterface::setSI( const vec_s &magNames, const  vec_d &values )
{// yay - c++11 has initilization lists ...
    bool carryOn = false;
    if( magNames.size() == values.size() )
    {
        carryOn = true;
        for( auto && it : magNames )
            if( entryExistsAndIsDegaussing( it )  )
            {
                carryOn = false;
                message( "ERROR: In setSI: ", it, " does not exist or is degaussing." );
            }
        if( carryOn )
            setSI_MAIN( magNames, values);
    }
    else
        message( "ERROR: In setSI: Passed number of magnets != Passed number of Values");
    return carryOn;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::setSI( const vec_s & magNames, const vec_d & values, const vec_d & tolerances, const size_t timeOUT )
{
    bool carryOn = setSI( magNames, values );

    vec_s magsToReturn;

    if( carryOn )
    {
        magsToReturn = waitForMagnetsToSettle( magNames, values, tolerances, 45 );///MMMMMAAAGIIC NUMBER

        if( magsToReturn.size() == magNames.size() )
            message( "All Magnets Probably Settled." );
        else
            message( "All Magnets Did NOT Settle." );
    }
    return magsToReturn;
}

//______________________________________________________________________________
bool magnetInterface::setSI( const std::string & magName, const double value, const double tolerance, const size_t timeOUT )
{
    const vec_s mags = { magName };
    const vec_d   vals = { value   };
    const vec_d   tols = { tolerance   };
    const vec_s magsRet =  setSI( mags, vals, tols, timeOUT );
    if( magsRet[0] == magName )
        return true;
    else
        return false;
}

//_____________________________________________________________
void magnetInterface::setSI_MAIN( const vec_s &magNames, const  vec_d &values )
{ /// THIS IS THE MAIN SET SI FUNCTION

    if( !shouldStartEPICs )
    {
        for( auto i = 0; i < magNames.size(); ++i )
        {
            if( entryExists( allMagnetData, magNames[i] ) )
                allMagnetData[ magNames[i] ].siWithPol = values[i];
        }
    }
    else
    {

        if( magNames.size() == values.size() )
        {
            /// The magnets can be divided into two types
            /// Those that can be set straight away and those that need N-R flipping
            vec_s magnetsToSet;
            vec_s magnetsToFlipThenSet;

            vec_d magnetsToSetValues;
            vec_d magnetsToFlipThenSetValues;
            /// yeah - feck - it's 3 types... April 2015
            vec_s presentGangMembers;
            vec_d presentGangMembersValues;

            /// Iterate over each magnet to decide if it is a set, or a flip-then-set, OR in a gang
            size_t val_pos = 0;
            for( auto & magNameIt : magNames )
            {
                switch( allMagnetData[ magNameIt ].magRevType )           /// First get the magnet reverse type,
                {
                    case magnetStructs::MAG_REV_TYPE::NR_GANGED:
                        presentGangMembers.push_back( magNameIt );
                        presentGangMembersValues.push_back( values[ val_pos ] );
                        break;

                    case magnetStructs::MAG_REV_TYPE::NR:/// Magnet PSU is of  N-R type
                        setNRSIVectors( magNameIt, values[ val_pos ], magnetsToSet,magnetsToFlipThenSet,magnetsToSetValues, magnetsToFlipThenSetValues );
                        break;

                    case magnetStructs::MAG_REV_TYPE::BIPOLAR:  /// Magnet PSU can be set to negative values
                        magnetsToSet.push_back( magNameIt);
                        magnetsToSetValues.push_back( values[ val_pos ]  );
                        break;

                    case magnetStructs::MAG_REV_TYPE::POS:      /// Magnet PSU can only be positive
                        debugMessage(magNameIt, "  REALLY??? A POSITIVE ONLY MAGNET??? ");
                        break;
                }
                ++val_pos;
            }

            /// we need to do further checks on the N-R ganged members
            if( presentGangMembers.size() > 0 )//MAGIC_NUMBER
                setNRGangedSIVectors( presentGangMembers, presentGangMembersValues, magnetsToSet, magnetsToFlipThenSet,magnetsToSetValues,magnetsToFlipThenSetValues );

            /// Now we can send out some values.
            /// First, zero the magnets that require NR flipping

            if( magnetsToFlipThenSet.size() > 0 )// MAGIC_NUMBER
            {   //std::cout<<"HI"<<std::endl;
    //            debugMessage(" ZERO FLIPPING MAGNETS " );
                const vec_d zeros( magnetsToFlipThenSet.size(), 0.0 );// MAGIC_NUMBER;
                setSINoFlip( magnetsToFlipThenSet, zeros);
                //waitForMagnetsToSettle( magnetsToFlipThenSet, zeros, tol, 45 );
            }

            /// Second, send out values to magnets that DO NOT require flipping

            if( magnetsToSet.size() > 0 )// MAGIC_NUMBER
                setSINoFlip( magnetsToSet, magnetsToSetValues );

            /// Finally, flip magnets and

            if( magnetsToFlipThenSet.size() > 0 )// MAGIC_NUMBER
                setSIWithFlip( magnetsToFlipThenSet, magnetsToFlipThenSetValues );
        }
    }
}
//______________________________________________________________________________
void magnetInterface::setNRSIVectors( const std::string & magName, const double val, vec_s & magnetsToSet, vec_s & magnetsToFlipThenSet,
                                                vec_d & magnetsToSetValues, vec_d & magnetsToFlipThenSetValues)
{
    if( shouldPolarityFlip( magName, val ) )  /// check if it needs flipping
    {
        //message("shouldPolarityFlip = true");
        magnetsToFlipThenSet.push_back( magName );
        magnetsToFlipThenSetValues.push_back( std::abs( val ) ); /// !! v_NR magnets always take positive current !!
        //debugMessage(  magName, " is NR Type and needs flipping.");
    }
    else
    {
        //message("shouldPolarityFlip = false");
        magnetsToSet.push_back( magName );
        magnetsToSetValues.push_back( std::abs( val ) );
        //debugMessage(  magName, " is NR Type and DOES NOT flipping.");
    }

}
//______________________________________________________________________________
bool magnetInterface::nrGanged_SI_Vals_AreSensible( const vec_s & magNames, const vec_d & values )
{
    /// To make this much simpler we are going to assume there is only 1 gang
    /// I guess to change this we need to have vectors for each gang.

    // we have already checked if the entryExists and has ganged members
    vec_s gangMembers = allMagnetData[ magNames[0] ].nPSU.gangMembers;
    gangMembers.push_back( magNames[0] );
    vec_d finalSIValues;

    size_t count = 0;// MAGIC_NUMBER
    for( size_t i = 0; i < gangMembers.size(); ++i  )// MAGIC_NUMBER
    {
        if ( std::find( magNames.begin(), magNames.end(), gangMembers[i] ) != magNames.end())
        {
            finalSIValues.push_back( values[ count ] );
            ++count;
        }
        else
            finalSIValues.push_back( getSI( gangMembers[i] ) );
        //message("nrGanged_SI_Vals_AreSensible = ", gangMembers[i], " value is ", finalSIValues[i] );
    }
    return polaritiesMatch( finalSIValues );
}
//______________________________________________________________________________
void magnetInterface::setNRGangedSIVectors( const vec_s & magNames, const vec_d & values, vec_s & magnetsToSet, vec_s & magnetsToFlipThenSet,
                                                vec_d & magnetsToSetValues, vec_d & magnetsToFlipThenSetValues )
{   /// To make this much simpler we are going to assume there is only 1 gang
    /// I guess to change this we need to have vectors for each gang.
    if( nrGanged_SI_Vals_AreSensible( magNames, values ) )
    {
        //message( "Ganged polarities DO match " );
        if( shouldPolarityFlip( magNames[0], values[0] ) )
        {
            //message("shouldPolarityFlip = true");
            magnetsToFlipThenSet.insert(magnetsToFlipThenSet.end(), magNames.begin(), magNames.end());
            magnetsToFlipThenSetValues.insert(magnetsToFlipThenSetValues.end(), values.begin(), values.end());
            for( auto && it : magnetsToFlipThenSetValues)
                it = std::abs( it );
        }
        else
        {
            //message("shouldPolarityFlip = false");
            magnetsToSet.insert( magnetsToSet.end(), magNames.begin(), magNames.end() );
            magnetsToSetValues.insert(magnetsToSetValues.end(), values.begin(), values.end());
            for( auto && it : magnetsToSetValues)
                it = std::abs( it );
        }
    }
    else
        message( "ERROR: From setSI: Ganged polarities DO NOT match" );
}
//______________________________________________________________________________
bool magnetInterface::setSINoFlip( const vec_s & magNames, const vec_d & values)
{/// THIS IS THE ONLY SI FUNCTION THAT ACTUALLY TALKS TO EPICS
    bool ret = false;
    if( magNames.size() == values.size() )
    {
        for( size_t i = 0; i < magNames.size(); ++i)// MAGIC_NUMBER
            if( entryExists( allMagnetData, magNames[i] ) )
                ca_put( allMagnetData[ magNames[i] ].pvMonStructs[ magnetStructs::MAG_PV_TYPE::SI ].CHTYPE,
                        allMagnetData[ magNames[i] ].pvMonStructs[ magnetStructs::MAG_PV_TYPE::SI ].CHID,
                        &values[ i ] );
        int status = sendToEpics( "ca_put", "", "Timeout sending SI values");
        if ( status == ECA_NORMAL )
            ret = true;
    }
    return ret;
}
//______________________________________________________________________________
magnetInterface::vec_b magnetInterface::setSIWithFlip( const vec_s & magNames, const vec_d & values)
{
    const vec_b ans = flipNR( magNames );
    vec_s flippedMagnets;
    vec_d flippedMagnetsVals ;
    for( size_t i = 0; i < ans.size(); ++i )// MAGIC_NUMBER
    {
        if( ans[ i ] )
        {
            flippedMagnets.push_back( magNames[i] );
            flippedMagnetsVals.push_back( values[i] );
        }
        else
            message("ERROR: From setSIWithFlip: ", magNames[i], " did not flip" );
    }
    setSINoFlip( flippedMagnets , flippedMagnetsVals );
    return ans;
}
//______________________________________________________________________________
const magnetInterface::vec_b  magnetInterface::flipNR( const vec_s & magNames )
{
   vec_b hasFlipped( magNames.size(), false );
   vec_b initial_is_PSUN_ON;

    /// Save Initial State so we know if they have flipped or not
    /// we're not going to check if N and R are consistant
    /// for true entires in initial_is_PSUN_ON we will switch on the r_PSU, adn vice-versa
    for( auto && it : magNames )
        if( isON_psuN( it ) )
            initial_is_PSUN_ON.push_back( true );
        else
            initial_is_PSUN_ON.push_back( false);

    time_t startTime   = time( 0 );//MAGIC_NUMBER
    time_t waitTime    = 45; /// MAGIC_NUMBER

    bool shouldbreak = false;

    vec_s N_ToSwitchON;
    vec_s R_ToSwitchON;

    while( true ) /// Here we just keep looping until time-out, or the magnets get flipped...
    {
        N_ToSwitchON.clear();
        R_ToSwitchON.clear();
        for( size_t i = 0; i < magNames.size(); ++i ) /// loop through each magnet
        {
            if( !hasFlipped[i] )                             /// has the magnet flipped ?
                if( canNRFlip( magNames[i] ) )               /// can the magnet flip ? (i.e. interlocks good)
                    if( initial_is_PSUN_ON[ i ] )            /// turn on R, or turn on N PSU
                        R_ToSwitchON.push_back( magNames[i] );
                    else
                        N_ToSwitchON.push_back( magNames[i] );
        }// For loop
        switchONpsu_R( R_ToSwitchON );
        switchONpsu_N( N_ToSwitchON );
        // all while loops should have a pause
        std::this_thread::sleep_for(std::chrono::milliseconds( 200 )); // MAGIC_NUMBER

        for( size_t i = 0; i < magNames.size(); ++i ) /// loop through each magnet, to check if it has flipped
        {
            if( initial_is_PSUN_ON[ i ] && isON_psuR( magNames[i] ) ) /// Init N = true, and R = true
            {   //debugMessage( magNames[i], " init N = true, N = false, FLIPPED! ");
                hasFlipped[i] = true;
            }
            if( !initial_is_PSUN_ON[ i ] && isON_psuN( magNames[i] ) )/// Init N = false, and N = true
            {   //debugMessage( magNames[i], " init N = false, N = true, FLIPPED! ");
                hasFlipped[i] = true;
            }
        }// For loop
        /// Now decide whether we should break out the main WHILE through successful flipping ....
        shouldbreak = true;
        for( auto && it : hasFlipped )
            if( !it )
                shouldbreak = false;

        /// ... or update currentTime time and loop again
        if( time(0) > startTime + waitTime )
            shouldbreak = true;

        if( shouldbreak )
            break;

    } // Main While
    if( !shouldbreak )
        message("ERROR: From flipNR: Trying to Flip NR PSU failed");
    return hasFlipped;
}
//______________________________________________________________________________
///
/// ____  ___  __  __
///(  _ \/ __)(  )(  )
/// )___/\__ \ )(__)(
///(__)  (___/(______)
///
//______________________________________________________________________________
bool magnetInterface::switchONpsu(const std::string & magName )
{
    const vec_s mags = { magName };
    return switchONpsu( mags );
}
//______________________________________________________________________________
bool magnetInterface::switchONpsu( const vec_s & magNames )
{
    vec_s mags;
    for( auto && it : magNames )
        if( entryExists( allMagnetData, it ) )
            if( isOFF( it ) )
                mags.push_back( it );
    return togglePSU( mags, magnetStructs::MAG_PV_TYPE::On, magnetStructs::MAG_PSU_TYPE::PSU);
}
//______________________________________________________________________________
bool magnetInterface::switchOFFpsu(const std::string & magName )
{
    const vec_s mags = { magName };
    return switchOFFpsu( mags );
}
//______________________________________________________________________________
bool magnetInterface::switchOFFpsu( const vec_s & magNames )
{
    vec_s mags;
    for( auto && it : magNames )
        if( entryExists( allMagnetData, it ) )
            if( isON( it ) )
                mags.push_back( it );
    return togglePSU( mags, magnetStructs::MAG_PV_TYPE::Off, magnetStructs::MAG_PSU_TYPE::PSU);
}
//______________________________________________________________________________
bool  magnetInterface::switchONpsu_N( const vec_s & magNames )
{
    return togglePSU( magNames, magnetStructs::MAG_PV_TYPE::On, magnetStructs::MAG_PSU_TYPE::PSU_N);
}
//______________________________________________________________________________
bool magnetInterface::switchONpsu_R( const vec_s & magNames )
{
    return togglePSU( magNames, magnetStructs::MAG_PV_TYPE::On, magnetStructs::MAG_PSU_TYPE::PSU_R);
}
//______________________________________________________________________________
bool magnetInterface::togglePSU( const vec_s & magNames, magnetStructs::MAG_PV_TYPE pvtype, magnetStructs::MAG_PSU_TYPE psutype)
{
    bool success = false;
    std::vector< chid* > CHIDS;
    std::vector< chtype* > CHTYPE;
    for( auto && it : magNames )
//        if( entryExists(allMagnetData, it ) )
//        {
            switch( psutype )
            {
                case magnetStructs::MAG_PSU_TYPE::PSU:
                    if( iLocksAreGood( allMagnetData[ it ].iLockStates ) )
                    {
                        if( pvtype == magnetStructs::MAG_PV_TYPE::On )
                        {
                            CHTYPE.push_back( &allMagnetData[ it ].pvComStructs[ magnetStructs::MAG_PV_TYPE::On ].CHTYPE );
                            CHIDS.push_back ( &allMagnetData[ it ].pvComStructs[ magnetStructs::MAG_PV_TYPE::On ].CHID   );
                        }
                        else if( pvtype == magnetStructs::MAG_PV_TYPE::Off )
                        {
                            CHTYPE.push_back( &allMagnetData[ it ].pvComStructs[ magnetStructs::MAG_PV_TYPE::Off ].CHTYPE );
                            CHIDS.push_back ( &allMagnetData[ it ].pvComStructs[ magnetStructs::MAG_PV_TYPE::Off ].CHID   );
                        }
                    }
                    else
                        message( "ERROR: From togglePSU, ", it, " PSU Ilocks are not all good.");
                    break;
                case magnetStructs::MAG_PSU_TYPE::PSU_N:
                    if( iLocksAreGood( allMagnetData[ it ].nPSU.iLockStates ) )
                    {
                        CHTYPE.push_back( &allMagnetData[ it ].nPSU.pvComStructs[ magnetStructs::MAG_PV_TYPE::On  ].CHTYPE );
                        CHIDS.push_back ( &allMagnetData[ it ].nPSU.pvComStructs[ magnetStructs::MAG_PV_TYPE::On  ].CHID   );
                    }
                    else
                        message( "ERROR: From togglePSU, ", it, " PSU_N Ilocks are not all good.");
                    break;
                case magnetStructs::MAG_PSU_TYPE::PSU_R:
                    if( iLocksAreGood( allMagnetData[ it ].rPSU.iLockStates ) )
                    {
                        CHTYPE.push_back( &allMagnetData[ it ].rPSU.pvComStructs[ magnetStructs::MAG_PV_TYPE::On  ].CHTYPE );
                        CHIDS.push_back ( &allMagnetData[ it ].rPSU.pvComStructs[ magnetStructs::MAG_PV_TYPE::On  ].CHID   );
                    }
                    else
                        message( "ERROR: From togglePSU, ", it, " PSU_R Ilocks are not all good.");
                    break;
            }
//        }
    if( CHTYPE.size() > 0 )// MAGIC_NUMBER
    {
        std::string m1 = "Timeout sending EPICS_ACTIVATE to inj Mag PSU";
        std::string m2 = "Timeout sending EPICS_SEND to inj Mag PSU";
        success = sendCommand( CHTYPE,  CHIDS,  m1, m2  );
    }
    return success;
}
//______________________________________________________________________________
bool magnetInterface::sendCommand( const std::vector< chtype* > & CHTYPE, const std::vector< chid* > & CHID, const std::string & m1, const std::string & m2  )
{
    bool ret = false;
    for( size_t i = 0; i < CHTYPE.size(); ++i )
        ca_put( *CHTYPE[i], *CHID[i], &EPICS_ACTIVATE );

    int status = sendToEpics( "ca_put", "", m1.c_str() );
    if ( status == ECA_NORMAL )
    {
        for( size_t i = 0; i < CHTYPE.size(); ++i )// MAGIC_NUMBER
            ca_put( *CHTYPE[i], *CHID[i], &EPICS_SEND );

        int status = sendToEpics( "ca_put", "", m2.c_str());
        if ( status == ECA_NORMAL )
            ret = true;
            //std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); // MAGIC_NUMBER
    }
    return ret;
}
//______________________________________________________________________________
/// ____  ____  ___    __    __  __  ___  ___                             ///
///(  _ \( ___)/ __)  /__\  (  )(  )/ __)/ __)                            ///
/// )(_) ))__)( (_-. /(__)\  )(__)( \__ \\__ \                            ///
///(____/(____)\___/(__)(__)(______)(___/(___/                            ///
///
//______________________________________________________________________________
bool magnetInterface::isDegaussing( const std::string & magName )
{
    if ( entryExists( isDegaussingMap, magName) )
        return (bool)isDegaussingMap[ magName ];
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isNotDegaussing( const std::string & magName )
{
    return !isDegaussing( magName);
}
//______________________________________________________________________________
bool magnetInterface::entryExistsAndIsDegaussing( const std::string & magName )
{
    if( entryExists( allMagnetData, magName) && isDegaussing( magName) )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::entryExistsAndIsNotDegaussing( const std::string & magName )
{
    if( entryExists( allMagnetData, magName) && isNotDegaussing( magName) )
        return true;
    else
        return false;
}
//______________________________________________________________________________
size_t magnetInterface::degaussAll( bool resetToZero )
{
    return deGauss ( getMagnetNames(), resetToZero );
}
//______________________________________________________________________________
size_t magnetInterface::deGauss( const std::string & mag, bool resetToZero  )
{
    const vec_s mags = { mag };
    return deGauss( mags, resetToZero );
}
//______________________________________________________________________________
size_t magnetInterface::deGauss( const  vec_s & mag, bool resetToZero  )
{
    killFinishedDegaussThreads();
    vec_s magToDegChecked;
    for( auto && it : mag )
    {
        if( entryExists( allMagnetData, it) ) /// is it a real magnet
        {
            if( entryExists( isDegaussingMap, it) ) /// it is a real magnet that is or has been degaussed
            {
                if( isDegaussingMap[it] )
                    message( it, " is still degaussing ");
                else
                    magToDegChecked.push_back( it ); /// it's not currently degaussing so add to the checkedMagnte list
            }
            else
            {
                isDegaussingMap[it] = true;
                magToDegChecked.push_back( it );
            }
        }
        else
            message( "ERROR: in deGauss", it, " name not known, can't degauss." );
    }
    if( magToDegChecked.size() > 0 )//MAGIC_NUMBER
    {
        degaussStructsMap[ degaussNum ].resetToZero   = resetToZero;
        degaussStructsMap[ degaussNum ].interface     = this;
        degaussStructsMap[ degaussNum ].magsToDeguass = magToDegChecked;
        degaussStructsMap[ degaussNum ].key           = degaussNum;
        degaussStructsMap[ degaussNum ].controllerType= myControllerType;
        degaussStructsMap[ degaussNum ].thread        = new std::thread( staticEntryDeGauss, std::ref(degaussStructsMap[ degaussNum ] ) );
        ++degaussNum;
    }
    return degaussNum;
}
//______________________________________________________________________________
void magnetInterface::killFinishedDegaussThreads()
{
    /// http://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it

    for (auto it = degaussStructsMap.cbegin(); it != degaussStructsMap.cend() /* not hoisted */; /* no increment */)
    {
        bool shouldKill = true;

        for( auto && it2 : it -> second.magsToDeguass )
            if( isDegaussingMap[ it2 ] )
                shouldKill = false;
        if( shouldKill )
        {
            /// join before deleting...
            /// http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
            it -> second.thread->join();
            delete it -> second.thread;
            degaussStructsMap.erase( it++ );
        }
        else
            ++it;
    }
    message("FIN killFinishedDegaussThreads");

}
//______________________________________________________________________________
void magnetInterface::killDegaussThread( size_t N )
{   /// Once you've started degaussing then you have to wait for it to finish...
    ///ha!
    /// http://stackoverflow.com/questions/13893060/i-want-to-kill-a-stdthread-using-its-thread-object
}
//______________________________________________________________________________
void magnetInterface::staticEntryDeGauss( const magnetStructs::degaussStruct & ds )
{/// This is the main degauss function

    time_t timeStart = time( 0 ); /// start clock

    ds.interface -> attachTo_thisCAContext(); /// base member function
    ds.interface -> printDegauss();

    ds.interface->debugMessage("\n","\tDEGAUSS UPDATE: Attempting to Degauss: ","\n");

    vec_s magToDegOriginal = ds.magsToDeguass;


    ds.interface->debugMessage("\n","\tDEGAUSS UPDATE: Saving Current Settings","\n");
    magnetStructs::magnetStateStruct originalState;
    originalState.controllerType = ds.controllerType;
    originalState = ds.interface -> getCurrentMagnetState( magToDegOriginal );

    /// Do we reset the current settings? or leave at zero (making sure the N-R state gets set back).

    vec_b init_PSU_N_State;
    if( ds.resetToZero )
        for( auto && it : magToDegOriginal )
            init_PSU_N_State.push_back( ds.interface->isON_psuN( it ) );

    /// we also have to set any ganged magnets that are not degaussing to zero during the degaussing procedure

    vec_s gangedMagToZero;
    vec_d gangedMagToZeroSIValues;

    ds.interface->checkGangedMagnets( magToDegOriginal, gangedMagToZero, gangedMagToZeroSIValues );

    if( gangedMagToZero.size() > 0 )//MAGIC_NUMBER
    {
        ds.interface->message("\n","\tDEGAUSS UPDATE: Attempting To Deguass NR-Ganged Magnet(s), zeroing gang memebrs ","\n" );
        const vec_d zeros( gangedMagToZero.size(), 0.0 );//MAGIC_NUMBER
        ds.interface->setSI( gangedMagToZero, zeros );
    }

    /// switch on the magnets that are to be degaussed

    ds.interface->switchONpsu( magToDegOriginal );

    vec_s magToDegOLD      = ds.magsToDeguass;
    vec_d values, tolerances;

    /// now check which magnets have actually switched on

    vec_s magToDeg;
    vec_s lostMagnets;

    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER

    for( auto && it : magToDegOriginal )
        if( ds.interface-> isON( it ) )
            magToDeg.push_back( it );

    if( magToDeg.size() != magToDegOriginal.size() )
    {
        ds.interface->getSetDifference( magToDegOriginal,magToDeg, lostMagnets );
        ds.interface->message("\n", "\tDEGAUSS UPDATE: Error Switching on:" );
        for( auto && it : lostMagnets )
            ds.interface->message(  "\t              : ",it);
        ds.interface->message("\n", "\t              : These magnets will not be degaussed." );
    }

    ds.interface->message("\n", "\tDEGAUSS UPDATE: Vectors Initialised Starting Degaussing","\n" );

    for( size_t j = 0; j < ds.interface->allMagnetData[magToDeg[j]].numDegaussSteps; ++j )//MAGIC_NUMBER
    {
        if( magToDeg.size() == 0 )//MAGIC_NUMBER
        {
           ds.interface->message("\n","\tDEGAUSS UPDATE: Degaussing Has FAILED - all magnets lost during the procedure - ;o(","\n");
           break;
        }
        values.clear();
        tolerances.clear();

        ds.interface->getDegaussValues( magToDeg, values, tolerances, j );

        /// Call the main function so we don't check for isDegaussing...

        ds.interface->message("get vals");

        ds.interface->setSI_MAIN( magToDeg, values );


        magToDeg = ds.interface->waitForMagnetsToSettle( magToDeg, values, tolerances, 45 );//MAGIC_NUMBER
        //check for lost magnets

        if( magToDeg.size() != magToDegOLD.size() )
        {
            ds.interface->getSetDifference( magToDegOriginal, magToDeg, lostMagnets );
            for( auto && it : lostMagnets )
                ds.interface->message( "\tDEGAUSS ERROR : ", it, " lost during degaussing, SI probably did not settle.");
        }
        magToDegOLD = magToDeg;
        ds.interface->message( "\n","\tDEGAUSS UPDATE: STAGE ", j + 1, " COMPLETE", "\n");//MAGIC_NUMBER
    }
    ds.interface->printDegaussResults( magToDeg, magToDegOriginal );

    if( ds.resetToZero  )
    {
        ds.interface->message( "\tDEGAUSS UPDATE: Re-setting zero." );
        //size_t i = 0;
        for( auto  i = 0; i < init_PSU_N_State.size(); ++i )//MAGIC_NUMBER
        {
            vec_s N_On_list;
            vec_s R_On_list;
            if( init_PSU_N_State[ i ] )
                N_On_list.push_back(  magToDegOriginal[ i ] );
            else
                R_On_list.push_back(  magToDegOriginal[ i ] );

            ds.interface->switchONpsu_R( N_On_list );
            ds.interface->switchONpsu_N( R_On_list );
        }
    }
    else
    {
        ds.interface -> applyMagnetStateStruct( originalState );
    }

    ds.interface->printFinish();

    if( gangedMagToZero.size() > 0 )//MAGIC_NUMBER
    {
        ds.interface->message("\n","\tDEGAUSS UPDATE: Attempting To reset Gang Memeber SIs ","\n" );
        ds.interface->setSI( gangedMagToZero, gangedMagToZeroSIValues );
    }

//    ds.interface->switchOFFpsu( magsToSwitchOff );
//    ds.interface->switchONpsu_N( magsToSetToN );
//    ds.interface->switchONpsu_R( magsToSetToR );

    ds.interface -> detachFrom_thisCAContext( ) ;

    for( auto && it : magToDegOriginal )
        ds.interface->isDegaussingMap[it] = false;


    time_t timeFinish = time( 0 ); /// start clock//MAGIC_NUMBER

    ds.interface->message( "Degaussing took ", timeFinish- timeStart, " seconds." );

}
//______________________________________________________________________________
void magnetInterface::checkGangedMagnets( vec_s & magToDeg, vec_s & gangedMagToZero, vec_d & gangedMagToZeroSIValues )
{
    /// To make this much simpler we are going to assume there is only 1 gang
    /// I guess to change this we need to have vectors for each gang.
    /// First we find out what memebers of magToDeg are ganged

    vec_s gangMembersInMagToDeg;

    for( auto && it : magToDeg )
        if( isNRGanged( it ) )
            gangMembersInMagToDeg.push_back( it );

    if( gangMembersInMagToDeg.size() > 0 )//MAGIC_NUMBER
    {
        /// Now we find out which ganged magnets are in

        vec_s gangMembers = allMagnetData[ gangMembersInMagToDeg[0] ].nPSU.gangMembers; /// this is ok because we assume ther eis only 1 gang

        getSetDifference( gangMembers , gangMembersInMagToDeg, gangedMagToZero );

        for( auto && it : gangedMagToZero )
            gangedMagToZeroSIValues.push_back( getSI( it ) );
    }
}
//______________________________________________________________________________
void magnetInterface::getDegaussValues( vec_s & magToDeg, vec_d & values, vec_d & tolerances, size_t step)
{

    for( auto && it : magToDeg )
    {
        if( isACor( it ) )
        {
            values.push_back( allMagnetData[ it ].degValues[ step ]  );
            tolerances.push_back( allMagnetData[ it ].degTolerance );
        }
        else if( isAQuad( it ) )
        {
            values.push_back( allMagnetData[ it ].degValues[ step ]  );
            tolerances.push_back( allMagnetData[ it ].degTolerance );
        }
        else if( isADip( it ) )
        {
            values.push_back( allMagnetData[ it ].degValues[ step ]  );
            tolerances.push_back( allMagnetData[ it ].degTolerance );
        }
        else if( isABSol( it ) )
        {
            values.push_back( allMagnetData[ it ].degValues[ step ]  );
            tolerances.push_back( allMagnetData[ it ].degTolerance );;
        }
        else if( isASol( it ) )
        {
            values.push_back( allMagnetData[ it ].degValues[ step ]  );
            tolerances.push_back( allMagnetData[ it ].degTolerance );
        }
        else
        {
            message("ERROR: From getDegaussValues: ", it, " MAG_TYPE not known!" );
        }
    }
}
//______________________________________________________________________________
void magnetInterface::printDegaussResults( const vec_s & magToDegSuccess, const vec_s & magToDegOriginal )
{
//    std::vector< std::string >::const_iterator it2;

    if( magToDegSuccess.size() == magToDegOriginal.size() )
    {
        message("\n","\tDEGAUSS SUMMARY: ALL MAGNETS DEGUASSED","\n" );
        printVec(    "\t        SUCCESS: ", magToDegSuccess, 4 ); /// MAGIC_NUMBER
    }
    else
    {
        message("\n","\tDEGAUSS SUMMARY: SOME MAGNETS FAILED TO DEGUASS ","\n" );

        vec_s failedToDeg;

        getSetDifference( magToDegOriginal , magToDegSuccess, failedToDeg );

        message("\n","\tDEGAUSS SUMMARY: ALL MAGNETS ***DID NOT*** DEGUASS","\n" );

        printVec( "\t        SUCCESS: ", magToDegSuccess, 4 ); /// MAGIC_NUMBER
        printVec( "\t         FAILED: ", failedToDeg, 4 ); /// MAGIC_NUMBER

    }
}
//______________________________________________________________________________
void magnetInterface::printVec( const std::string & p1, const vec_s & v, size_t numPerLine )
{
    size_t counter = 0;//MAGIC_NUMBER
    std::stringstream s;
    for( auto && it : v )
    {
        ++counter;
        s << it << " ";
        if( counter % numPerLine == 0 )//MAGIC_NUMBER
        {
            message(1);
            message( p1 , s.str() );
            counter = 0;//MAGIC_NUMBER
            s.str( std::string() ); // clear stringstream
        }
    }
    if( 0 < counter && counter < numPerLine ) // print remaining entries...
        message( p1 , s.str() );
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::waitForMagnetsToSettle( const vec_s &mags, const vec_d &values, const vec_d &tolerances, const time_t waitTime )
{
    /// This is surprisingly complicated.
    /// As there are NO guarantees that RI will get close to SI we have to check if RI is changing
    /// Also we have to check if RI is supposed to be zero, because sometimes we hang
    /// around at zero while flipping before setting the SI value we actually want
    /// I've done the conditional flags very verbose just to make sure i follow what I'm doing

    bool timeOut     = false;

    vec_d oldRIValues( mags.size(), -999.99 );//MAGIC_NUMBER
    vec_d currentRIValues( mags.size() );
    vec_b magnetSettledState( mags.size(), false );
    vec_s settledMags;

    /// To test if we are supposed to be setting zero.
    /// have a vector of bool indicating if they actually ARE being set to zero

    vec_b settingZero;

    for( auto && it : values )
        settingZero.push_back( it == 0.0 );//MAGIC_NUMBER

    time_t timeStart = time( 0 ); /// start clock

    while( true )
	{
        currentRIValues = getRI( mags );
        bool shouldBreak = true;

        for( size_t i = 0 ; i <  mags.size(); ++i )//MAGIC_NUMBER
        {
            if( magnetSettledState[ i ]  ) /// magnet has settled
            {

            }
            else{ /// magnet has not settled


                if( isRIequalVal( mags[i], values[i], tolerances[i] ) ) /// The acid test!
                {
                    magnetSettledState[i] = true;
                    //debugMessage( mags[i], " RI == val. SETTLED = True. ", currentRIValues[i], ", ", values[i], ", ", tolerances[i] );
                }
                else if( settingZero[i] ) /// We are supposed to be setting zero....
                {

                    if( areSame( oldRIValues[i], currentRIValues[i], tolerances[i] ) )
                    {
                        magnetSettledState[i] = true;
                        //debugMessage( mags[i], " is setting 0.0 and RI_new == RI_old. SETTLED = True ", currentRIValues[i], ", ", oldRIValues[i], ", ", tolerances[i]);
                    }
                }
                else /// we are not setting zero...
                {
                    if( areSame( oldRIValues[i], currentRIValues[i], tolerances[i] ) && areSame( 0.0, currentRIValues[i], tolerances[i]  )  )//MAGIC_NUMBER
                    {
                        /// RI is not changing, it's at zero, but zero isn't the final RI value we expect.
                    }
                    else if( areSame( oldRIValues[i], currentRIValues[i], tolerances[i] ) && areNotSame( 0.0, currentRIValues[i], tolerances[i] ) )//MAGIC_NUMBER
                    {
                        magnetSettledState[i] = true;
                        //debugMessage( mags[i], " RI_new != 0.0 && RI_new == RI_old RI. SETTLED = True ",currentRIValues[i], ", ", oldRIValues[i], ", ", tolerances[i] );
                    }
                }
            }
        }
        for( auto && it : magnetSettledState )
            if( !it )
                shouldBreak = false;

        if( time(0) - timeStart > waitTime )
            timeOut = true;

        if( shouldBreak )
        {
            //debugMessage( "I think the magnets have settled Breaking out of loop.");
            break;
        }
        oldRIValues = currentRIValues;

        if( timeOut )
        {
            debugMessage( "Timeout waiting for the magnets To Settle.");
            break;
        }
        /// really 2000 milliseconds while we wait for RI to update >>>> ?
        /// YES!!
        std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
	} /// while

    for( size_t i = 0; i < mags.size(); ++i )//MAGIC_NUMBER
        if( magnetSettledState[i] )
            settledMags.push_back( mags[i] );

    return settledMags;
}
//______________________________________________________________________________
bool magnetInterface::isAQuad( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magType == magnetStructs::MAG_TYPE::QUAD;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isADip( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magType == magnetStructs::MAG_TYPE::DIP;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isABSol(const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magType == magnetStructs::MAG_TYPE::BSOL;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isASol( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magType == magnetStructs::MAG_TYPE::SOL;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isAHCor( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magType == magnetStructs::MAG_TYPE::HCOR;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isAVCor( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magType == magnetStructs::MAG_TYPE::VCOR;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isACor( const std::string & magName )
{
    if( isAVCor( magName ) || isAHCor( magName ) )
        return  true;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isBipolar( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magRevType == magnetStructs::MAG_REV_TYPE::BIPOLAR;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isNR( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magRevType == magnetStructs::MAG_REV_TYPE::NR;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isNRGanged( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return  allMagnetData[ magName ].magRevType == magnetStructs::MAG_REV_TYPE::NR_GANGED;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isNRorNRGanged( const std::string & magName )
{
    if( isNR( magName ) || isNRGanged( magName ) )
        return  true;
    else
        return false;
}
//______________________________________________________________________________
void magnetInterface::setRITolerance( const std::string & magName, double val)
{
    if( entryExists( allMagnetData, magName ) )
        allMagnetData[ magName ].riTolerance = val;
}
//______________________________________________________________________________
void magnetInterface::setRITolerance(const vec_s & magNames, const vec_d & vals )
{
    for( auto  i = 0; i <  magNames.size(); ++i )
        if( entryExists( allMagnetData, magNames[i] ) )
            allMagnetData[  magNames[i] ].riTolerance = vals[i];
}
//______________________________________________________________________________
double magnetInterface::getRITolerance( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].riTolerance;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________



std::vector< double >  magnetInterface::getRITolerance( const std::vector< std::string > & magNames )
{
    std::vector< double > a;
//    for( auto && it : magNames )
//        a.push_back( getRI(it) );
    return a;
}
//______________________________________________________________________________
bool magnetInterface::shouldPolarityFlip( const std::string & magName, const double val )
{
    /// This function checks the N - R State of a magnet and the
    /// requested value and decides if it needs to flip polarity

    bool shouldPolarityFlip = false;
    if( entryExists( allMagnetData, magName ) )
    {
//        message( "PSU_N state = ", ENUM_TO_STRING(allMagnetData[ magName ].nPSU.psuState ) );
//        message( "PSU_R state = ", ENUM_TO_STRING(allMagnetData[ magName ].rPSU.psuState ) );
//        message( "PSU_N on? = ", isON_psuN( magName ) );
//        message( "PSU_R on? = ", isON_psuR( magName ) );
//        message( "val       = ", val );
        if( val < 0.0 )//MAGIC_NUMBER
        {
            if( isON_psuN( magName ) && isOFF_psuR( magName ) )
            {
                shouldPolarityFlip = true;
                //message(val," < 0.0, PSU_N is ON and PSU_R is OFF shouldPolarityFlip = true" );
            }
            //else
                //message("isON_psuN( magName ) && isOFF_psuR( magName ) FALSE");
        }
        else if( val > 0.0 )//MAGIC_NUMBER
        {
            if( isON_psuR( magName ) && isOFF_psuN( magName )  )
            {
                shouldPolarityFlip = true;
                //message(val," > 0.0, PSU_N is OFF and PSU_R is ON shouldPolarityFlip = true" );
            }
            //else
                //message("isON_psuR( magName ) && isOFF_psuN( magName ) FALSE");
        }
    }
    return shouldPolarityFlip;
}
//______________________________________________________________________________
bool magnetInterface::isON_psuN( const std::string & magName )
{
    bool ans = false;
    if( entryExists( allMagnetData, magName ) )
        if( allMagnetData[ magName ].nPSU.psuState == VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON )
             ans = true;
    return ans;
}
//______________________________________________________________________________
bool magnetInterface::isON_psuR( const std::string & magName )
{
    bool ans = false;
    if( entryExists( allMagnetData, magName ) )
        if( allMagnetData[ magName ].rPSU.psuState == VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON )
             ans = true;
    return ans;
}
//______________________________________________________________________________
bool magnetInterface::isON(const std::string & magName )
{
    bool ans = false;
    if( entryExists( allMagnetData, magName ) )
        if( allMagnetData[ magName ].psuState == VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON )
            ans = true;
    return ans;
}
//______________________________________________________________________________
bool magnetInterface::isOFF_psuN( const std::string & magName )
{
    return !isON_psuN(magName);
}
//______________________________________________________________________________
bool magnetInterface::isOFF_psuR( const std::string & magName )
{
    return !isON_psuR( magName );
}
//______________________________________________________________________________
bool magnetInterface::isOFF(const std::string & magName )
{
    return !isON( magName );
}
//______________________________________________________________________________
bool magnetInterface::canNRFlip(const std::string & magName )
{
    bool ret = false;
    if( entryExists( allMagnetData, magName ) )
    {
        if( isNRorNRGanged( magName ) )
        {
            ret = true; // change to true, but set back to false if tests fail
            auto c = 0;//MAGIC_NUMBER
            for( const auto & it : allMagnetData[ magName].nPSU.iLockStates )
                if( it.second )
                    ++c;
            if( allMagnetData[ magName].nPSU.iLockStates.size() != c )
                ret = false;

            c = 0;//MAGIC_NUMBER
            for( const auto & it : allMagnetData[ magName].rPSU.iLockStates )
                if( it.second )
                    ++c;
            if( allMagnetData[ magName].nPSU.iLockStates.size() != c )
                ret = false;
        }
    }
    return ret;
}
//______________________________________________________________________________
bool magnetInterface::isRIequalVal( const std::string & magName, const  double value, const double tolerance )
{
    bool ret = false;

    if( entryExists( allMagnetData, magName ) )
        ret = areSame( allMagnetData[ magName ].riWithPol, value, tolerance );

//        if( ret )
//            debugMessage( magName, ", ri = val ", allMagnetData[ magName ].riWithPol,", ", value, ", ", tolerance);
//        else
//            debugMessage( magName, ", ri != val ", allMagnetData[ magName ].riWithPol,", ", value, ", ", tolerance);
//    if( ret )
//        std::cout << " THE SAME " << std::endl;
//    else
//        std::cout << " NOT THE SAME " << std::endl;
    return ret;
}




///
///   __   ___ ___ ___  ___  __   __
///  / _` |__   |   |  |__  |__) /__`
///  \__> |___  |   |  |___ |  \ .__/
///
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getMagnetNames()
{
    vec_s r;
    for( auto && it : allMagnetData )
        r.push_back( it.first );
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getQuadNames()
{
    vec_s r;
    for( auto && it : allMagnetData )
        if( isAQuad( it.first ) )
            r.push_back( it.first );

    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getHCorNames()
{
    vec_s r;
    for( auto && it : allMagnetData )
        if( isAHCor( it.first ) )
            r.push_back( it.first );

    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getVCorNames()
{
    vec_s r;
    for( auto && it : allMagnetData )
        if( isAVCor( it.first ) )
            r.push_back( it.first );

    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getDipNames()
{
    vec_s r;
    for( auto && it : allMagnetData )
        if( isADip( it.first ) )
            r.push_back( it.first );

    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getSolNames()
{
    vec_s r;
    for( auto && it : allMagnetData )
        if( isASol( it.first ) || isABSol( it.first ) )
            r.push_back( it.first );

    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
double magnetInterface::getSI( const std::string &magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].siWithPol;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
magnetInterface::vec_d magnetInterface::getSI( const vec_s &magNames )
{
    vec_d a;
    for( auto && it : magNames )
        a.push_back( getSI(it) );
    return a;
}
//______________________________________________________________________________
double magnetInterface::getRI( const std::string &magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].riWithPol;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
magnetInterface::vec_d magnetInterface::getRI( const vec_s &magNames )
{
    vec_d a;
    for( auto && it : magNames )
        a.push_back( getRI(it) );
    return a;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getCurrentMagnetState()
{
    magnetStructs::magnetStateStruct ret;
    ret.controllerType = myControllerType;
    for( auto && it : allMagnetData )
    {
        ret.magNames.push_back( it.first );
        ret.siValues.push_back( it.second.siWithPol );
        ret.riValues.push_back( it.second.riWithPol );
        ret.psuStates.push_back( it.second.psuState );
        ++ret.numMags;
    }
    return ret;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getCurrentMagnetState( const vec_s & s )
{
    magnetStructs::magnetStateStruct ret;
    ret.controllerType = myControllerType;
    for( auto && it : s )
    {
        if( entryExists(allMagnetData, it) )
        {
            ret.magNames.push_back( it );
            ret.siValues.push_back( allMagnetData[ it ].siWithPol );
            ret.riValues.push_back( allMagnetData[ it ].riWithPol );
            ret.psuStates.push_back( allMagnetData[ it ].psuState );
            ++ret.numMags;
        }
    }
    return ret;
}
//______________________________________________________________________________
void magnetInterface::applyMagnetStateStruct( const magnetStructs::magnetStateStruct & magState )
{
    if( shouldStartEPICs )
    {

        if( magState.controllerType == myControllerType )
        {
            message("applyMagnetStateStruct");
            vec_s magsToSwitchOn, magsToSwitchOff;
            for( size_t i = 0; i < magState.numMags; ++i )
            {
                if(  magState.psuStates[i] == VELA_ENUM::MAG_PSU_STATE::MAG_PSU_OFF )
                    magsToSwitchOff.push_back( magState.magNames[i] );
                else if(  magState.psuStates[i]  == VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ON )
                    magsToSwitchOn.push_back( magState.magNames[i] );
                message("Found ", magState.magNames[i] );
            }
            switchONpsu( magsToSwitchOn );
            switchOFFpsu( magsToSwitchOff  );
            setSI( magState.magNames, magState.siValues );
        }
        else
        {
            message("Can't apply magnet settings, this is a ", ENUM_TO_STRING(myControllerType), " magnet controller and the magnet settings are for  ", ENUM_TO_STRING(magState.controllerType));
        }
    }
    else
    {
        message("Can't apply magnet settings - we are in offline mode");
    }
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getDBURT( const std::string & fileName )
{
    /// create a dburt object
    dburt dbr(SHOW_DEBUG_MESSAGES_PTR, SHOW_MESSAGES_PTR,myControllerType);
    return dbr.readDBURT( fileName );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getDBURTCorOnly( const std::string & fileName )
{
    magnetStructs::magnetStateStruct ms1 = getDBURT( fileName );
    magnetStructs::magnetStateStruct ms2;
    ms2.controllerType = ms1.controllerType;
    size_t i = 0;//MAGIC_NUMBER
    for( auto && it : ms1.magNames )
    {
        if( isACor( it ) )
        {
            ms2.magNames.push_back( it );
            ms2.psuStates.push_back( ms1.psuStates[i] );
            ms2.siValues.push_back( ms1.siValues[i] );
            ++ms2.numMags;
        }
        ++i;
    }
    return ms2;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getDBURTQuadOnly( const std::string & fileName )
{
    magnetStructs::magnetStateStruct ms1 = getDBURT( fileName );
    magnetStructs::magnetStateStruct ms2;
    ms2.controllerType = ms1.controllerType;
    size_t i = 0;//MAGIC_NUMBER
    for( auto && it : ms1.magNames )
    {
        if( isAQuad( it ) )
        {
            ms2.magNames.push_back( it );
            ms2.psuStates.push_back( ms1.psuStates[i] );
            ms2.siValues.push_back( ms1.siValues[i] );
            ++ms2.numMags;
        }
        ++i;
    }
    return ms2;
}
//______________________________________________________________________________
void magnetInterface::applyDBURT( const std::string & fileName )
{
    applyMagnetStateStruct( getDBURT(fileName) );
}
//______________________________________________________________________________
void magnetInterface::applyDBURTCorOnly( const std::string & fileName )
{
    applyMagnetStateStruct( getDBURTCorOnly(fileName) );
}
//______________________________________________________________________________
void magnetInterface::applyDBURTQuadOnly( const std::string & fileName )
{
    applyMagnetStateStruct( getDBURTQuadOnly(fileName) );
}
//______________________________________________________________________________
bool magnetInterface::writeDBURT( const magnetStructs::magnetStateStruct & ms, const std::string & fileName, const std::string & comments, const std::string & keywords )
{
    /// create a dburt object
    dburt dbr(SHOW_DEBUG_MESSAGES_PTR, SHOW_MESSAGES_PTR,myControllerType);
    /// write the file with the dburt  and return the result
    return dbr.writeDBURT( ms, fileName, comments );
}
//______________________________________________________________________________
bool magnetInterface::writeDBURT(const std::string & fileName, const std::string & comments,const std::string & keywords)
{
    magnetStructs::magnetStateStruct ms =  magnetInterface::getCurrentMagnetState();
    ms.controllerType = myControllerType;
    return writeDBURT( ms, fileName, comments,keywords );
}
//______________________________________________________________________________
/// Reverse types
magnetStructs::MAG_REV_TYPE magnetInterface::getMagRevType( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].magRevType;
    else
        return magnetStructs::UNKNOWN_MAG_REV_TYPE;
}
//______________________________________________________________________________
std::vector<  magnetStructs::MAG_REV_TYPE >  magnetInterface::getMagRevType( const std::vector< std::string > & magNames )
{
    std::vector< magnetStructs::MAG_REV_TYPE > a;
    for( auto && it : magNames )
        a.push_back( getMagRevType(it) );
    return a;
}
//______________________________________________________________________________
magnetStructs::MAG_TYPE magnetInterface::getMagType( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].magType;
    else
        return magnetStructs::UNKNOWN_MAGNET_TYPE;
}
//______________________________________________________________________________
std::vector< magnetStructs::MAG_TYPE > magnetInterface::getMagType( const std::vector< std::string > & magNames )
{
    std::vector<  magnetStructs::MAG_TYPE > a;
    for( auto && it : magNames )
        a.push_back( getMagType(it) );
    return a;
}
//______________________________________________________________________________
VELA_ENUM::MAG_PSU_STATE magnetInterface::getMagPSUState( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].psuState;
    else
        return VELA_ENUM::MAG_PSU_NONE;
}
//______________________________________________________________________________
std::vector<  VELA_ENUM::MAG_PSU_STATE > magnetInterface::getMagPSUState( const std::vector< std::string > & magNames )
{
    std::vector< VELA_ENUM::MAG_PSU_STATE >  a;
    for( auto && it : magNames )
        a.push_back( getMagPSUState(it) );
    return a;
}
//______________________________________________________________________________
std::vector< double > magnetInterface::getDegValues( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].degValues;
    else
    {
        std::vector< double > r;
        r.push_back( UTL::DUMMY_DOUBLE );
        return r;
    }
}
//______________________________________________________________________________
std::vector< std::vector< double > > magnetInterface::getDegValues( const std::vector< std::string > & magNames )
{
    std::vector< std::vector< double > >  a;
    for( auto && it : magNames )
        a.push_back( getDegValues(it) );
    return a;
}
//______________________________________________________________________________
size_t magnetInterface::getNumDegSteps( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].numDegaussSteps;
    else
        return UTL::ZERO_SIZET;
}
//______________________________________________________________________________
std::vector< size_t > magnetInterface::getNumDegSteps( const std::vector< std::string > & magNames )
{
    std::vector< size_t >  a;
    for( auto && it : magNames )
        a.push_back( getNumDegSteps(it) );
    return a;
}
/// BJAS ADDITIONS
//______________________________________________________________________________
double magnetInterface::getPosition( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].position;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
std::vector< double > magnetInterface::getPosition( const std::vector< std::string > & magNames )
{
    std::vector< double >  a;
    for( auto && it : magNames )
        a.push_back( getPosition(it) );
    return a;
}
//______________________________________________________________________________
std::vector< double > magnetInterface::getFieldIntegralCoefficients( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].fieldIntegralCoefficients;
    else
    {
        std::vector< double > r;
        r.push_back( UTL::DUMMY_DOUBLE );
        return r;
    }
}
//______________________________________________________________________________
std::vector<std::vector< double >> magnetInterface::getFieldIntegralCoefficients( const std::vector< std::string > & magNames )
{
    std::vector< std::vector< double > >  a;
    for( auto && it : magNames )
        a.push_back( getFieldIntegralCoefficients(it) );
    return a;
}
//______________________________________________________________________________
///
double magnetInterface::getMagneticLength( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].magneticLength;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
std::vector< double > magnetInterface::getMagneticLength( const std::vector< std::string > & magNames )
{
    std::vector< double >  a;
    for( auto && it : magNames )
        a.push_back( getMagneticLength(it) );
    return a;
}
//______________________________________________________________________________
std::string magnetInterface::getManufacturer( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].manufacturer;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string> magnetInterface::getManufacturer( const std::vector< std::string > & magNames )
{
    std::vector< std::string >  a;
    for( auto && it : magNames )
        a.push_back( getManufacturer(it) );
    return a;
}
//______________________________________________________________________________
std::string magnetInterface::getSerialNumber( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].serialNumber;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string>  magnetInterface::getSerialNumber( const std::vector< std::string > & magNames )
{
    std::vector< std::string >  a;
    for( auto && it : magNames )
        a.push_back( getSerialNumber(it) );
    return a;

}
//______________________________________________________________________________
std::string magnetInterface::getMagnetBranch( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].magnetBranch;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string>  magnetInterface::getMagnetBranch( const std::vector< std::string > & magNames )
{
    std::vector< std::string >  a;
    for( auto && it : magNames )
        a.push_back( getMagnetBranch(it) );
    return a;

}
//______________________________________________________________________________
std::string magnetInterface::getMeasurementDataLocation( const std::string & magName )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ].measurementDataLocation;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
std::vector<std::string>  magnetInterface::getMeasurementDataLocation( const std::vector< std::string > & magNames )
{
    std::vector< std::string >  a;
    for( auto && it : magNames )
        a.push_back( getMeasurementDataLocation(it) );
    return a;

}
//______________________________________________________________________________
void magnetInterface::printDegauss()
{
    debugMessage("\n","\n","\t ____  ____  ___    __    __  __  ___  ___ ");
    debugMessage("\t(  _ \\( ___)/ __)  /__\\  (  )(  )/ __)/ __)");
    debugMessage("\t )(_) ))__)( (_-. /(__)\\  )(__)( \\__ \\\\__ \\");
    debugMessage("\t(____/(____)\\___/(__)(__)(______)(___/(___/","\n","\n");
}
//______________________________________________________________________________
void magnetInterface::printFinish()
{
    debugMessage("\n","\n","\t  ____  ____  _  _  ____  ___  _   _  ____  ____");
    debugMessage("\t ( ___)(_  _)( \\( )(_  _)/ __)( )_( )( ___)(  _ \\ ");
    debugMessage("\t  )__)  _)(_  )  (  _)(_ \\__ \\ ) _ (  )__)  )(_) )");
    debugMessage("\t (__)  (____)(_)\\_)(____)(___/(_) (_)(____)(____/  ","\n","\n");

}
//______________________________________________________________________________
void magnetInterface::showMagRevType()
{
    for( auto && it : allMagnetData )
        message( it.first," rev type = ", ENUM_TO_STRING(it.second.magRevType ) );
}
//______________________________________________________________________________
const magnetStructs::magnetObject & magnetInterface::getMagObjConstRef( const std::string & magName  )
{
    if( entryExists( allMagnetData, magName ) )
        return allMagnetData[ magName ];
    else
        return allMagnetData[ dummyName ];
    /// HAVE A DUMMY MAGNET TO RETURN ON ERROR?
}
//______________________________________________________________________________
const magnetStructs::magnetObject * magnetInterface::getMagObjConstPtr( const std::string & magName  )
{
    if( entryExists( allMagnetData, magName ) )
        return &allMagnetData[ magName ];
    else
        return &allMagnetData[ dummyName ];
}
//______________________________________________________________________________
void magnetInterface::addDummyElementToAllMAgnetData()
{
    allMagnetData[ dummyName ].name = dummyName;
    allMagnetData[ dummyName ].name = dummyName;
}

/// SHELL COMMANDS TO ACTIVATE PSUs
////______________________________________________________________________________
//void magnetInterface::switchON_N_psu( std::string magName )
//{
//    /// execute via bash / shell command, don't like this, but sometimes more robust...
//
//    if( allMagnetData.count( magName ) )
//    {
//        std::stringstream st;
////        st << "EPICS_CA_ADDR_LIST=192.168.83.255;";
//        st << "caput " << allMagnetData[ magName ].epicsPSUN << ":On 1;" << "caput " << allMagnetData[ magName ].epicsPSUN << ":On 0";
//        exec( st.str().c_str() );
//    }
//}
////______________________________________________________________________________
//void magnetInterface::switchON_N_psu( vec_s & magNames )
//{
//    /// execute via bash / shell command, don't like this, but sometimes more robust...
//    std::stringstream st;
//
////    st << "EPICS_CA_ADDR_LIST=192.168.83.255;";
//
//    vec_s::iterator it;
//    for( it = magNames.begin(); it != magNames.end(); ++it )
//        if( allMagnetData.count( *it ) )
//            st << "caput " << allMagnetData[ *it ].epicsPSUN << ":On 1;" << "caput " << allMagnetData[ *it ].epicsPSUN << ":On 0";
//
//    exec( st.str().c_str() );
//}
////______________________________________________________________________________
//void magnetInterface::switchON_R_psu( std::string magName )
//{
//    /// execute via bash / shell command, don't like this, but sometimes more robust...
//
//    std::cout << "switchON_R_psu called " << std::endl;
//
//    if( allMagnetData.count( magName ) )
//    {
//        std::stringstream st;
////        st << "EPICS_CA_ADDR_LIST=192.168.83.255;";
//        st << "caput " << allMagnetData[ magName ].epicsPSUR << ":On 1;" << "caput " << allMagnetData[ magName ].epicsPSUR << ":On 0";
//        exec( st.str().c_str() );
//    }
//}
////______________________________________________________________________________
//void magnetInterface::switchON_R_psu( vec_s & magNames )
//{
//    /// execute via bash / shell command, don't like this, but sometimes more robust...
//
//    //std::cout << "switchON_R_psu called " << std::endl;
//
//    std::stringstream st;
////    st << "EPICS_CA_ADDR_LIST=192.168.83.255;";
//    vec_s::iterator it;
//    for( it = magNames.begin(); it != magNames.end(); ++it )
//        if( allMagnetData.count( *it ) )
//            st << "caput " << allMagnetData[ *it ].epicsPSUR << ":On 1;" << "caput " << allMagnetData[ *it ].epicsPSUR << ":On 0";
//    exec( st.str().c_str() );
//}
////______________________________________________________________________________

/////
///// __  __  ____  ____  __    ____  ____  ____  ____  ___                             ///
/////(  )(  )(_  _)(_  _)(  )  (_  _)(_  _)(_  _)( ___)/ __)                            ///
///// )(__)(   )(   _)(_  )(__  _)(_   )(   _)(_  )__) \__ \                            ///
/////(______) (__) (____)(____)(____) (__) (____)(____)(___/                            ///
/////
/////
//
//bool magnetInterface::gangPolaritiesMatch( const std::string & mag1, const  vec_s & magNames,const  vec_d & values)
//{
//    /// first find the names of the magnets in the gang...
//
//    vec_s gang = allMagnetData[ mag1 ].gangMembers;
//    gang.push_back( mag1 );
//
////    for( int i = 0; i < gang.size(); ++ i)
////    {
////        std::cout << "gang[" << i << "] = " << gang[i] << std::endl;
////    }
//
//    /// now we're going toiterate over the gang members and test if they are being changed
//    /// i.e. are they in magNames
//    /// test the polarity of all the current RI values, or the new requested ones
//    /// iof all the polarities are the same we're good to go, otherwise we can't polarity flip
//    /// of course we can polarity flip if riIsBelowFlipValue
//
//
//    vec_s::const_iterator it1, it2;
//    std::vector< int > gangPol;
//
//    for( it1 = gang.begin(); it1 != gang.end(); ++it1 )
//    {
//        //std::cout << "1"<< std::endl;
//        it2 = std::find( magNames.begin(), magNames.end(), *it1 );
//        //std::cout << "2"<< std::endl;
//        if( it2 != magNames.end() )
//        {
//            //std::cout << "checking Polarity for " << *it2 << " value = " << values[ std::distance(magNames.begin(), it2) ] << std::endl;
//            gangPol.push_back( getPolarityOrRIBelowFlipValue( values[ std::distance(magNames.begin(), it2) ] ) );
//
//        }
//        else
//        {
//            //std::cout << "checking Polarity for " << * it1 << " (not in list) value = " << allMagnetData[ * it1 ].riValueWithPolarity << std::endl;
//            gangPol.push_back( getPolarityOrRIBelowFlipValue( allMagnetData[ * it1 ].riValueWithPolarity ) );
//        }
//    }
//
//    /// so we have either positive, negative or irrelevant in gangPol...
//    /// if there is a + and - then we return false, otherwise we're good to go
//
//    bool posFound = false;
//    bool negFound = false;
//    bool zeroFound = false;
////
////    for( int i = 0; i < gangPol.size(); ++ i)
////    {
////        std::cout << gang[i] << " pol = " << gangPol[i] << std::endl;
////    }
//
//    std::vector< int >::iterator it3;
//    for( it3 = gangPol.begin(); it3 != gangPol.end(); ++it3 )
//    {
//        if( *it3  == 1 )
//        {
//            posFound = true;
//            //std::cout << "Found a pos " << std::endl;
//
//        }
//        if( *it3  == -1 )
//        {
//            negFound = true;
//            //std::cout << "Found a neg" << std::endl;
//        }
//        if( *it3  == 0 )
//        {
//            zeroFound = true;
//            //std::cout << "Found a zero" << std::endl;
//        }
//    }
//
//    bool ans = true;
//
//    if( posFound == true && negFound == true )
//    {
//        std::cout << std::endl;
//        std::cout << "!!! ERROR !!!" << std::endl;
//        std::cout << "Requested Polarities of ganged magnets is not possible, please try again." << std::endl;
//        std::cout << "!!! ERROR !!!" << std::endl;
//        std::cout << std::endl;
//        ans = false;
//    }
//
//
//    if( posFound == true && zeroFound == true ||  negFound == true && zeroFound == true )
//    {
//        std::cout << std::endl;
//        std::cout << "!!! Warning !!!" << std::endl;
//        std::cout << "Polarity flipping of ganged magnets may have caused some magnets to reverse, check this is what you wanted" << std::endl;
//        std::cout << "!!! Warning !!!" << std::endl;
//        std::cout << std::endl;
//    }
//
////    std::cout << "Returning ";
////    ans ? std::cout << " true" : std::cout << " false";
////    std::cout << std::endl;
//
//    return ans;
//}
////______________________________________________________________________________
//int magnetInterface::getPolarityOrRIBelowFlipValue( double val )
//{
//    int ret;
//    if( riIsBelowFlipValue( TMath::Abs(  val ) ) ) /// This includes 0.0
//        ret = 0;
//    else if( val > 0.0 )
//        ret = 1;
//    else if( val < 0.0 )
//        ret = -1;
//    return ret;
//}
////______________________________________________________________________________
//bool magnetInterface::getPolarity( double val )
//{
//    return val > 0.0;
//}
////______________________________________________________________________________
//vec_s magnetInterface::gangMemberPresent(const  std::string & mag1, const  vec_s & magNames )
//{
//    /// return gangmebers of mag1 that are in magNames
//
//    vec_s ret;
//    vec_s::const_iterator it;
//    for( it = magNames.begin(); it != magNames.end(); ++it )
//        if( std::find(allMagnetData[ mag1 ].gangMembers.begin(), allMagnetData[ mag1 ].gangMembers.end(), *it) != allMagnetData[ mag1 ].gangMembers.end() )
//            ret.push_back( *it );
//    return ret;
//}
////______________________________________________________________________________
//bool magnetInterface::gangMembersCanFlip( std::string & m )
//{
//    bool canFLipFlag = true;
//    vec_s::iterator it;
//    for( it = allMagnetData[ m ].gangMembers.begin(); it != allMagnetData[ m ].gangMembers.end(); ++it  )
//        if( riIsNOTBelowFlipValue( allMagnetData[  *it ].riValue ) )
//           canFLipFlag = false;
//    return canFLipFlag;
//}

//______________________________________________________________________________
//bool magnetInterface::isRIequalVal( vec_s  magNames, vec_d values, vec_d tolerances )
//{
//    bool ret = true;
//    for( size_t i = 0; i < magNames.size(); ++i )
//        if( areNotSame( allMagnetData[ magNames[i] ].riValueWithPolarity, values[i], tolerances[i] ) )
//            ret = false;
//    return ret;
//}
////______________________________________________________________________________
//vec_s  magnetInterface::checkRIequalToVal( vec_s  magNames, vec_d values, vec_d tolerances )
//{
//    /// returns all the magNames where RI equals values
//
//    std::vector< std::string  > ans;
//    for( size_t i = 0; i < magNames.size(); ++i )
//        if( areSame( allMagnetData[ magNames[i] ].riValueWithPolarity, values[i], tolerances[i] ) )
//            ans.push_back( magNames[i] );
//    return ans;
//}
////______________________________________________________________________________
//enum magnetStructs::MAG_PSU_STATE magnetInterface::getState(unsigned short epicsSta)
//{
//    /// This switches from the EPICS Sta values to my enum magnetStructs::MAG_PSU_STATE,
//    /// NB THE EN-CODING MAY BE WRONG!
//
//    enum magnetStructs::MAG_PSU_STATE ret;
//    switch(epicsSta)
//    {
//        case 0:
//            ret = magnetStructs::k_OFF;
//            break;
//        case 1:
//            ret = magnetStructs::k_ON;
//            break;
//        case 2:
//            ret = magnetStructs::k_TIMING;
//            break;
//        default:
//            ret = magnetStructs::k_ERR;
//    }
//    return ret;
//}
////______________________________________________________________________________
//const  magnetStructs::magnetObject * magnetInterface::getMagnetData( std::string magName )
//{
//    return &allMagnetData[ magName ];
//}
////______________________________________________________________________________
//const magnetStructs::mag_Obj_Map  & magnetInterface::getMagnetData(   ) const
//{
//    return allMagnetData;
//}
//vec_d magnetInterface::getAllMagSI()
//{
//    vec_d siStates( allMagnetData.size() );
//    std::map<std::string,  magnetStructs::magnetObject >::iterator it;
//    size_t i = 0;
//    for(it = allMagnetData.begin(); it != allMagnetData.end(); ++it )
//    {
//        siStates[ i ] = it -> second.siValue;
//        ++i;
//    }
//    return siStates;
//}
//vec_d magnetInterface::getAllMagSIWithPolarity()
//{
//    vec_d siStates( allMagnetData.size() );
//    std::map<std::string,  magnetStructs::magnetObject >::iterator it;
//    size_t i = 0;
//    for(it = allMagnetData.begin(); it != allMagnetData.end(); ++it )
//    {
//        siStates[ i ] = it -> second.siValueWithPolarity;
//        ++i;
//    }
//    return siStates;
//}
//std::vector< magnetStructs::MAG_PSU_STATE  > magnetInterface::getAllMagPSUState()
//{
//    std::vector< magnetStructs::MAG_PSU_STATE > psuStates( allMagnetData.size() );
//    std::map<std::string,  magnetStructs::magnetObject >::iterator it;
//    size_t i = 0;
//    for(it = allMagnetData.begin(); it != allMagnetData.end(); ++it )
//    {
//        psuStates[ i ] = it -> second.epicsPSU_Sta;
//        ++i;
//    }
//    return psuStates;
//}
//std::vector< std::string  > magnetInterface::getAllMagNames()
//{
//    vec_s magNames( allMagnetData.size() );
//    std::map<std::string,  magnetStructs::magnetObject >::iterator it;
//    size_t i = 0;
//    for(it = allMagnetData.begin(); it != allMagnetData.end(); ++it )
//    {
//        magNames[ i ] = it -> first;
//        ++i;
//    }
//    return magNames;
//}
////______________________________________________________________________________
//std::string magnetInterface::exec(const char* cmd) /// General Function execute shell command and return return string
//{
//    std::string result;
//    //std::cout << "Executing " << cmd << std::endl;
//#ifdef WIN32
//    FILE* pipe = _popen(cmd, "r");
//#else
//    FILE* pipe = popen(cmd, "r");
//#endif
//    if (!pipe)
//    {
//        result =  "ERROR";
//    }else{
//
//        char buffer[128];
//        while(!feof(pipe))
//        {
//            if(fgets(buffer, 128, pipe) != NULL) result += buffer;
//        }
//    #ifdef WIN32
//        _pclose(pipe);
//    #else
//        pclose(pipe);
//    #endif
//        /// caget returns vales with a newline break... get rid of newline
//        std::string::size_type pos = 0;
//        while ( ( pos = result.find ("\n", pos) ) != std::string::npos )  result.erase( pos, 2 );
//    }
//    return result;
//}
