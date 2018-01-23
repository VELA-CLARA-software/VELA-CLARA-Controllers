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

#include "invertedMagnetronGaugeInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <ctime>

//______________________________________________________________________________
invertedMagnetronGaugeInterface::invertedMagnetronGaugeInterface( const std::string & configFileLocation1,
                                                                  const bool *show_messages_ptr,
                                                                  const bool *show_debug_messages_ptr,
                                                                  const bool shouldStartEPICS,
                                                                  const bool startVirtualMachine,
                                                                  const VELA_ENUM::MACHINE_AREA myMachineArea ):
configReader( configFileLocation1, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine),
interface( show_messages_ptr, show_debug_messages_ptr ),
shouldStartEPICS( shouldStartEPICS ),
startVM( startVirtualMachine ),
machineArea( myMachineArea ),
dummyname("DUMMY")
{
    initialise();
}
//______________________________________________________________________________
invertedMagnetronGaugeInterface::~invertedMagnetronGaugeInterface()
{
//    for( auto it : continuousMonitorStructs )
//    {
        debugMessage("delete invertedMagnetronGaugeInterface continuousMonitorStructs entry.");
//        delete it;
//    }
}
//______________________________________________________________________________
const invertedMagnetronGaugeStructs::vacImgObject & invertedMagnetronGaugeInterface::getIMGObjConstRef(const std::string & imgName)
{
    if( entryExists(allVacImgData,imgName))
    {
        return allVacImgData.at(imgName);
    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readVacImgConfig();

    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        initVacImgObjects();
        /// subscribe to the channel ids
        initVacImgChids();
        /// start the monitors: set up the callback functions
        monitorVacImgs();
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::getImgNames( std::vector< std::string >  & vacImgNames )
{
    vacImgNames.clear();
    for( auto const & it : allVacImgData )
        vacImgNames.push_back( it.first );
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::initVacImgObjects()
{
    const std::vector< invertedMagnetronGaugeStructs::vacImgObject > vacImgObjs = configReader.getVacImgObjects();
    for( auto const & it : vacImgObjs )
        allVacImgData[ it.name ] = it;
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::initVacImgChids()
{
    message( "\n", "Searching for VacImg chids...");
    for( auto && it1 : allVacImgData )
    {
        addILockChannels( it1.second.numIlocks, it1.second.pvRoot, it1.second.name, it1.second.iLockPVStructs );
        for( auto && it2 : it1.second.pvComStructs )
            addChannel( it1.second.pvRoot, it2.second );
        for( auto && it2 : it1.second.pvMonStructs  )
            addChannel( it1.second.pvRoot, it2.second );
    }
    int status = sendToEpics( "ca_create_channel", "Found VacImg chids.", "!!TIMEOUT!! Not all vacImg ChIds found." );

    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));
        for( auto & it : allVacImgData )
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
    else if( status == ECA_NORMAL )
    {
        allChidsInitialised = true; /// interface base class member  not actually used but good to know
    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::addChannel( const std::string & pvRoot, invertedMagnetronGaugeStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::monitorVacImgs()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for( auto && it1 : allVacImgData )
    {
//        monitorIlocks( it1.second.iLockPVStructs, it1.second.iLockStates );
        // iterate over the velaINJscreenObject PvMon
        for( auto && it2 : it1.second.pvMonStructs )
        {
            std::cout << it1.first <<  " monitorIMGs " << ENUM_TO_STRING( it2.first ) << std::endl;

            addIMGObjectMonitors( it2.second,  it1.second );

            ca_create_subscription( it2.second.CHTYPE,
                                    it2.second.COUNT,
                                    it2.second.CHID,
                                    it2.second.MASK,
                                    invertedMagnetronGaugeInterface::staticEntryVacImgMonitor,
                                    (void*)continuousMonitorStructs.back(),
                                    &continuousMonitorStructs.back()->EVID );
        }
    }
    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to IMG Monitors", "!!TIMEOUT!! Subscription to IMG monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member, not actually used but good to know
}
//_______________________________________________________________________________________________________________
void invertedMagnetronGaugeInterface::addIMGObjectMonitors( invertedMagnetronGaugeStructs::pvStruct & pvs,  invertedMagnetronGaugeStructs::vacImgObject & obj )
{
    continuousMonitorStructs.push_back( new invertedMagnetronGaugeStructs::monitorStruct() );
    continuousMonitorStructs.back() -> interface = this;
    continuousMonitorStructs.back() -> monType   = pvs.pvType;
    continuousMonitorStructs.back() -> CHTYPE    = pvs.CHTYPE;
    continuousMonitorStructs.back() -> obj       = (void*)&obj;
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::staticEntryVacImgMonitor( const event_handler_args args )
{
    invertedMagnetronGaugeStructs::monitorStruct * ms = reinterpret_cast< invertedMagnetronGaugeStructs::monitorStruct*>(args.usr);
    //std::cout << "staticEntryScreenMonitor " << std::endl;
    switch ( ms -> monType )// based on which monitor we call a differnwet update function
    {
        case invertedMagnetronGaugeStructs::IMG_PV_TYPE::STA:
            ms->interface->updateSta( ms, *(unsigned short*)args.dbr );
            break;
        case invertedMagnetronGaugeStructs::IMG_PV_TYPE::P:
            ms->interface->updateP( ms, *(double*)args.dbr );
            break;
    }
}
//_________________________________________________________________________________________________________________
void invertedMagnetronGaugeInterface::updateSta( invertedMagnetronGaugeStructs::monitorStruct * ms, const unsigned short args )
{
    invertedMagnetronGaugeStructs::vacImgObject * obj = reinterpret_cast<invertedMagnetronGaugeStructs::vacImgObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string imgName = obj -> name;
    if( entryExists(allVacImgData, imgName ) )
    {
        switch( args )
        {
            case 0:
                allVacImgData.at(imgName).vacImgState = invertedMagnetronGaugeStructs::IMG_STATE::IMG_GOOD;
                break;
//            case 1:
//                allVacImgData.at(imgName).vacImgState = invertedMagnetronGaugeStructs::IMG_STATE::IMG_BAD;
//                break;
//            case 2:
//                allVacImgData.at(imgName).vacImgState = invertedMagnetronGaugeStructs::IMG_STATE::IMG_ERROR;
//                break;
            default:
                allVacImgData.at(imgName).vacImgState = invertedMagnetronGaugeStructs::IMG_STATE::IMG_BAD;
        }
    }
    message(imgName,": IMG state is ", ENUM_TO_STRING(allVacImgData.at(imgName).vacImgState));
}
//_________________________________________________________________________________________________________________
void invertedMagnetronGaugeInterface::updateP( invertedMagnetronGaugeStructs::monitorStruct * ms, const double args )
{
    invertedMagnetronGaugeStructs::vacImgObject * obj = reinterpret_cast<invertedMagnetronGaugeStructs::vacImgObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string imgName = obj -> name;
    if( entryExists(allVacImgData, imgName ) )
    {
        allVacImgData.at(imgName).p = args;
    }
}
//______________________________________________________________________________
void invertedMagnetronGaugeInterface::cancelDataMonitors()
{
    for( auto it = temporaryMonitorStructs.begin(); it != temporaryMonitorStructs.end(); ++it)
    {
        ca_clear_subscription( (*it) -> EVID );
    }
}
//______________________________________________________________________________
double invertedMagnetronGaugeInterface::getImgP( const std::string & vacImg )
{
    if( allVacImgData.count( vacImg ) )
    {
        return allVacImgData[ vacImg ].p;
    }
    else
    {
        message("IMG not defined");
        return 0;
    }
}
//______________________________________________________________________________
invertedMagnetronGaugeStructs::vacImgObject invertedMagnetronGaugeInterface::getVacImgObject( const std::string & name )
{
    invertedMagnetronGaugeStructs::vacImgObject r;
    auto iter = allVacImgData.find( name );
    if (iter != allVacImgData.end() )
        r = iter -> second;
    else
    {
        std::stringstream s;
        s << "ERROR!!! " << name << " DOES NOT EXIST";
        r.name = s.str();
        r.pvRoot = s.str();
        r.vacImgState =  invertedMagnetronGaugeStructs::IMG_STATE::IMG_ERROR;
    }
    return r;
}
//______________________________________________________________________________
invertedMagnetronGaugeStructs::IMG_STATE invertedMagnetronGaugeInterface::getImgState( const  std::string & objName )
{
    invertedMagnetronGaugeStructs::IMG_STATE r =  invertedMagnetronGaugeStructs::IMG_STATE::IMG_ERROR;
    auto iter = allVacImgData.find( objName );
    if (iter != allVacImgData.end() )
        r = iter -> second.vacImgState;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > invertedMagnetronGaugeInterface::getILockStates( const std::string & objName )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
    auto iter = allVacImgData.find( objName );
    if( iter != allVacImgData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  invertedMagnetronGaugeInterface::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allVacImgData.find( name );
    if( iter != allVacImgData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
//______________________________________________________________________________
