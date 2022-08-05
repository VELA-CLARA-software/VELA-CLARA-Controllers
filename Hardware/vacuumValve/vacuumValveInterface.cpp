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

#include "vacuumValveInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

vacuumValveInterface::vacuumValveInterface( const std::string & configFileLocation,
                                            const bool& show_messages_ptr,
                                            const bool& show_debug_messages_ptr,
                                            const bool shouldStartEPICs,
                                            const bool startVirtualMachine,
                                            const HWC_ENUM::MACHINE_AREA myMachineArea ):
configReader( configFileLocation, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine ),
interface( show_messages_ptr, show_debug_messages_ptr, shouldStartEPICs ),
shouldStartEPICs( shouldStartEPICs ),
startVM( startVirtualMachine ),
machineArea( myMachineArea ),
dummyname("DUMMY")
{
    std::cout << " VAC VALV INTERFACE CONSTRUCTOR " << std::endl;
    std::cout << "CONFIG LOCATION: " << configFileLocation << std::endl;
    initialise();
    std::cout << " INITIALIZED IN INTERFACE VAC VALVE " << std::endl;
}
////______________________________________________________________________________
//vacuumValveInterface::vacuumValveInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
//: configReader( show_messages_ptr, show_debug_messages_ptr  ), interface( show_messages_ptr, show_debug_messages_ptr  )
//{
//    initialise();
//}
//______________________________________________________________________________
vacuumValveInterface::~vacuumValveInterface()
{
//    for (auto && it : allVacValveData )
//    {
//        debugMessage("in vacuumValveInterface: delete allVacValveData ", it.first );
//    }
//        debugMessage("delete vacuumValveInterface continuousMonitorStructs entry.");
}
//______________________________________________________________________________
const vacuumValveStructs::vacValveObject& vacuumValveInterface::getVacValveObjConstRef(const std::string & vacValveName)
{
    if( entryExists(allVacValveData,vacValveName))
    {
        return allVacValveData.at(vacValveName);

    }
}
//______________________________________________________________________________
void vacuumValveInterface::initialise( )
{
    /// The config file reader

    configFileRead = configReader.readConfigFiles();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    std::cout << "** IN INITIALIZE FOR INTERFACE **" << std::endl;
    if( configFileRead )
    {
        std::cout << "** CONFIG FILE READ **" << std::endl;
        /// initialise the objects based on what is read from the config file

        bool getDataSuccess = initVacValveObjects();
        if( getDataSuccess )
        {
            /// subscribe to the channel ids
            if( shouldStartEPICs )
            {
            initVacValveChids();

            /// start the monitors: set up the callback functions

            monitorVacValves();

            }
            else
            {
                message("The vacValveInterface Read Config files, Not Starting EPICS Monitors" );
            }
        }
        else
        {
            message( "!!!The vacValveInterface received an Error while getting vac valve data!!!" );
        }

        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBER
    }
    std::cout << " CONFIG FILE NOT READ " << std::endl;
    message(allVacValveData.size());
}
//______________________________________________________________________________
void vacuumValveInterface::findVacValveNames( std::vector< std::string >  & vacValveNames )
{
    vacValveNames.clear();
    for( auto const & it : allVacValveData )
        vacValveNames.push_back( it.first );
}
//______________________________________________________________________________
bool vacuumValveInterface::initVacValveObjects()
{
    const std::vector< vacuumValveStructs::vacValveObject > vacValveObjs = configReader.getVacValveObjects();
    for( auto const & it : vacValveObjs )
    {
        allVacValveData[ it.name ] = it;
    }
    return true;
}
//______________________________________________________________________________
void vacuumValveInterface::initVacValveChids()
{
    message( "\n", "Searching for VacValve chids...");

    for( auto && it1 : allVacValveData )
    {
        addILockChannels( it1.second.numIlocks, it1.second.pvRoot, it1.second.name, it1.second.iLockPVStructs );
        for( auto && it2 : it1.second.pvComStructs )
            addChannel( it1.second.pvRoot, it2.second );
        for( auto && it2 : it1.second.pvMonStructs  )
            addChannel( it1.second.pvRoot, it2.second );
    }
    int status = sendToEpics( "ca_create_channel", "Found VacValve chids.", "!!TIMEOUT!! Not all vacValve ChIds found." );

    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
        for( auto & it : allVacValveData )
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
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;
}
//______________________________________________________________________________
void vacuumValveInterface::addChannel( const std::string & pvRoot, vacuumValveStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void vacuumValveInterface::monitorVacValves()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for( auto && it : allVacValveData )
    {
        monitorIlocks( it.second.iLockPVStructs, it.second.iLockStates );
        for( auto && it2 : it.second.pvMonStructs  )
        {
            /// http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
            /// init structs 'correctly'

            addValveObjectMonitors( it2.second, it.second );

            /// For the vacValve there is only 1 monitor type
            /// maybe it.second.pvMonStructs does not need to be a map???, pvComStructs probably does though...
            //continuousMonitorStructs.back().monType   = it2.first;

            /// If you pass DBF_STRING and recast as a char * in the callback you can get the state as GOOD, BAD, OPEN, CLOSED etc,
            /// This is useful for debugging, but in general i'm just going to subscribe to the DBR_ENUM

            /// ca_create_subscription accepts a void * user argument, we pass a pointer to the monitor struct,
            /// in the callback function this is cast back and the data can then be updated
            /// void * usrArg = reinterpret_cast< void *>( continuousMonitorStructs.back() );

            ca_create_subscription( it2.second.CHTYPE, it2.second.COUNT,  it2.second.CHID, it2.second.MASK,
                                    vacuumValveInterface::staticEntryrMonitor, (void*)continuousMonitorStructs.back(), 0 );//&it2.second.EVID ); // &continuousMonitorStructs.back().EventID );
        }
    }
    int status = sendToEpics( "ca_create_subscription", "Subscribed to VacValve Monitors", "!!TIMEOUT!! Subscription to VacValve monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true;
}
//______________________________________________________________________________
void vacuumValveInterface::addValveObjectMonitors( vacuumValveStructs::pvStruct & pvs,  vacuumValveStructs::vacValveObject & obj )
{
    continuousMonitorStructs.push_back( new vacuumValveStructs::monitorStruct() );
    continuousMonitorStructs.back() -> interface = this;
    continuousMonitorStructs.back() -> monType   = pvs.pvType;
    continuousMonitorStructs.back() -> CHTYPE    = pvs.CHTYPE;
    continuousMonitorStructs.back() -> obj       = (void*)&obj;
}
//______________________________________________________________________________
void vacuumValveInterface::staticEntryrMonitor( const event_handler_args args )
{
    vacuumValveStructs::monitorStruct * ms = static_cast< vacuumValveStructs::monitorStruct *> ( args.usr );
    switch( ms -> monType )
    {
        case vacuumValveStructs::VAC_VALVE_PV_TYPE::Sta:
            {
                ms->interface->updateValveState( ms, *(unsigned short*)args.dbr );
                break;
            }
        default:
            {
                break;
            }
    }
}
//______________________________________________________________________________
void vacuumValveInterface::updateValveState( vacuumValveStructs::monitorStruct * ms, const unsigned short args )
{
    vacuumValveStructs::vacValveObject* obj = reinterpret_cast< vacuumValveStructs::vacValveObject* > (ms->obj);
    switch( args )
    {
        case 0:
            {
                obj->vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_CLOSED;
                break;
            }
        case 1:
            {
                obj->vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_OPEN;
                break;
            }
        default:
            {
                obj->vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_ERROR;
            }
    }
    ms->interface->debugMessage( obj->name ," vacValveState = ", ENUM_TO_STRING(obj->vacValveState) );
}
////______________________________________________________________________________
//void vacuumValveInterface::staticEntryVacValveMonitor( const event_handler_args args )
//{
//    /// recast args.usr ( a void * ) to a monitor struct pointer, then dereference
//
//    vacuumValveStructs::monitorStruct ms = * reinterpret_cast<vacuumValveStructs::monitorStruct*>(args.usr);
//
//    /// Not sure how to decode these apart from trial and error
//    /// you can test with DBF_STRING as the callback type
//
//    if( *(unsigned short*)args.dbr == 0 )
//    {
//        ms.vacValveObj -> vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_CLOSED;
//    }
//    else if( *(unsigned short*)args.dbr == 1 )
//    {
//        ms.vacValveObj -> vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_OPEN;
//    }
//    else
//    {
//        ms.vacValveObj -> vacValveState = vacuumValveStructs::VALVE_STATE::VALVE_ERROR;
//    }
//
//    /// make debug messages easier to understand by using ENUM_TO_STRING
//    ms.interface -> debugMessage( ms.vacValveObj -> name,  " new state = ", ENUM_TO_STRING(ms.vacValveObj -> vacValveState ));
//    /// If subscribed to DBF_STRING use this to get the message
//    //char * val = (char *)args.dbr;
//    /// now we can switch based on the monitor type and then update the correct part of the vacValve object data using val
//    /// For the vacValve this is basically redundant, there is only one monitor the "Sta"
//    /// (apart from interlocks, these are handled in the bas class)
////    switch( lms.monType )
////    {
////        case vacuumValveStructs::Sta:
////                ....
////        case SomeOtherPVType:
////                ....
////    }
//}
//_________________________________________________________________________________________________________________
bool vacuumValveInterface::entryExists2(const std::string & name, bool weKnowEntryExists )
{// This version of the function is so we don't have to check the entry exists in the map each time
     if( weKnowEntryExists )
        return weKnowEntryExists;
    else
        return entryExists( allVacValveData, name );
}
//______________________________________________________________________________
void vacuumValveInterface::openVacValve( const std::string & vacValve )
{
    if( entryExists2( vacValve, true ) )
    {
        if( isClosed( vacValve ) )
        {
            chtype & cht = allVacValveData[ vacValve ].pvComStructs[ vacuumValveStructs::VAC_VALVE_PV_TYPE::On ].CHTYPE;
            chid   & chi = allVacValveData[ vacValve ].pvComStructs[ vacuumValveStructs::VAC_VALVE_PV_TYPE::On ].CHID;
            int status = caput( cht, chi, EPICS_ACTIVATE, "", "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO VAC VALVE OPEN" );
            if( status == ECA_NORMAL )
                int status2 = caput(cht, chi, EPICS_SEND, "", "!!TIMEOUT!! FAILED TO SEND VAC VALVE OPEN" );
        }
        else
        {
            message("Valve ", vacValve, " is already open.");
        }
    }
    else
    {
        message("Valve ", vacValve, " does not exist - is it defined in the config file?");
    }
}
//______________________________________________________________________________
void vacuumValveInterface::closeVacValve( const std::string & vacValve )
{
    if( entryExists2( vacValve, true ) )
    {
        if( isOpen( vacValve ) )
        {
            chtype & cht = allVacValveData[ vacValve ].pvComStructs[ vacuumValveStructs::VAC_VALVE_PV_TYPE::Off ].CHTYPE;
            chid   & chi = allVacValveData[ vacValve ].pvComStructs[ vacuumValveStructs::VAC_VALVE_PV_TYPE::Off ].CHID;
            int status = caput( cht, chi, EPICS_ACTIVATE, "", "!!TIMEOUT!! FAILED TO EPICS_SEND EPICS_ACTIVATE TO VAC VALVE CLOSE" );
            if( status == ECA_NORMAL )
                int status2 = caput( cht, chi, EPICS_SEND, "", "!!TIMEOUT!! FAILED TO EPICS_SEND VAC VALVE CLOSE" );
        }
        else
        {
            message("Valve ", vacValve, " is already closed.");
        }
    }
    else
    {
        message("Valve ", vacValve, " does not exist - is it defined in the config file?");
    }
}
//______________________________________________________________________________
bool vacuumValveInterface::isOpen( const std::string & vacValve )
{
    message(allVacValveData.size());
    bool ret = false;
    if( entryExists2( vacValve, true ) )
    {
        message(ENUM_TO_STRING(allVacValveData[ vacValve ].vacValveState));
        if( allVacValveData[ vacValve ].vacValveState == vacuumValveStructs::VALVE_STATE::VALVE_OPEN )
        {
            ret = true;
            message("Valve is open");
        }
        else
        {
            message("Valve is closed");
        }
    }

    if( ret )
        debugMessage( vacValve, " is open");
    else
        debugMessage( vacValve, " is NOT open");

    return ret;
}
//______________________________________________________________________________
bool vacuumValveInterface::isClosed( const std::string & vacValve )
{
    bool ret = false;
    if( entryExists2( vacValve, true ) )
    {
        message(ENUM_TO_STRING(allVacValveData[ vacValve ].vacValveState));
        if( allVacValveData[ vacValve ].vacValveState == vacuumValveStructs::VALVE_STATE::VALVE_CLOSED )
        {
            ret = true;
            message("Valve is closed");
        }
        else
        {
            message("Valve is open");
        }
    }

    if( ret )
        debugMessage( vacValve, " is closed");
    else
        debugMessage( vacValve, " is NOT closed");

    return ret;
}
//______________________________________________________________________________
std::vector< std::string > vacuumValveInterface::getVacValveNames()
{
    std::vector< std::string > vacValveNames;
    for( auto && iter : allVacValveData )
    {
        message( "Vac Valve Name ", iter.first );
        vacValveNames.push_back( iter.first );
    }

    return vacValveNames;
}
//______________________________________________________________________________
vacuumValveStructs::vacValveObject vacuumValveInterface::getVacValveObject( const std::string & name )
{
    vacuumValveStructs::vacValveObject r;
    auto iter = allVacValveData.find( name );
    if (iter != allVacValveData.end() )
        r = iter -> second;
    else
    {
        std::stringstream s;
        s << "ERROR!!! " << name << " DOES NOT EXIST";
        r.name = s.str();
        r.pvRoot = s.str();
        r.vacValveState =  vacuumValveStructs::VALVE_STATE::VALVE_ERROR;
    }
    return r;
}
//______________________________________________________________________________
vacuumValveStructs::VALVE_STATE vacuumValveInterface::getVacValveState( const  std::string & objName )
{
    vacuumValveStructs::VALVE_STATE r =  vacuumValveStructs::VALVE_STATE::VALVE_ERROR;
    auto iter = allVacValveData.find( objName );
    if (iter != allVacValveData.end() )
        r = iter -> second.vacValveState;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > vacuumValveInterface::getILockStates( const std::string & objName )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > r;
    auto iter = allVacValveData.find( objName );
    if( iter != allVacValveData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  vacuumValveInterface::getILockStatesStr( const std::string & name )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allVacValveData.find( name );
    if( iter != allVacValveData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
//______________________________________________________________________________
bool vacuumValveInterface::openAndWait( const std::string & name, const time_t waitTime )
{
    return toggleAndWait(&vacuumValveInterface::isClosed, &vacuumValveInterface::isOpen, &vacuumValveInterface::openVacValve, name, waitTime, *this);
}
//______________________________________________________________________________
bool vacuumValveInterface::closeAndWait( const std::string & name, const time_t waitTime )
{
    return toggleAndWait(&vacuumValveInterface::isOpen, &vacuumValveInterface::isClosed, &vacuumValveInterface::closeVacValve, name, waitTime, *this);
}
//______________________________________________________________________________
bool vacuumValveInterface::toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, vacuumValveInterface & obj )
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
