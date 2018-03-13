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
// project include
#include "gunModInterface.h"
//#include "dburt.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
gunModInterface::gunModInterface( const std::string &gunModConf,
                                const bool startVirtualMachine,
                                const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                                const bool shouldStartEPICs ):
configReader(gunModConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs( shouldStartEPICs ),
allChidsInitialised(false),
gun_mod_hex_map(rfModStructs::gunModHexErrorCodes::create())
{
    if(shouldStartEPICs)
        message("gunModInterface shouldStartEPICs is true");
    else
        message("gunModInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
gunModInterface::~gunModInterface()
{
}
//______________________________________________________________________________
void gunModInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(UTL::STANDARD_PAUSE);
    if( configFileRead )
    {
        message("The gunModInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = configReader.getGunModObject(gunMod);
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                message("The gunModInterface has acquired objects, connecting to EPICS");
                //std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions
                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(UTL::STANDARD_PAUSE); // MAGIC_NUMBER
            }
            else
             message("The gunModInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message( "!!!The gunModInterface received an Error while getting laser data!!!" );
    }
}
//______________________________________________________________________________
//bool gunModInterface::initObjects()
//{
//    bool ans = configReader.getpilaserObject(pilaser);
//    debugMessage( "pilaser.pvComStructs.size() = ", pilaser.pvComStructs.size() );
//    debugMessage( "pilaser.pvMonStructs.size() = ", pilaser.pvMonStructs.size() );
//    return ans;
//}
////______________________________________________________________________________
void gunModInterface::initChids()
{
    message( "\n", "Searching for gunMod ChIds...");

    for( auto && it : gunMod.pvMonStructs )
    {
        addChannel( gunMod.pvRoot, it.second );
    }
    // currently there are no command only PVs for the PIL
    for( auto && it : gunMod.pvComStructs )
    {
        addChannel( gunMod.pvRoot, it.second );
    }
    //addILockChannels( gunMod.numIlocks, gunMod.pvRoot, gunMod.name, gunMod.iLockPVStructs );
    int status = sendToEpics( "ca_create_channel", "Found gunMod ChIds.", "!!TIMEOUT!! Not all gunMod ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));//MAGIC_NUMBER
        message("\n", "Checking gunMod ChIds ");
        for( auto && it : gunMod.pvMonStructs )
        {
            checkCHIDState( it.second.CHID, ENUM_TO_STRING( it.first ) );
        }
        for( auto && it : gunMod.pvComStructs)
        {
            checkCHIDState( it.second.CHID, ENUM_TO_STRING( it.first ) );
        }
        std::this_thread::sleep_for(std::chrono::milliseconds( 5000 )); // MAGIC_NUMBER
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
////______________________________________________________________________________
void gunModInterface::addChannel( const std::string & pvRoot, rfModStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );//MAGIC_NUMBER
    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void gunModInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for( auto && it : gunMod.pvMonStructs )
    {
        continuousMonitorStructs.push_back( new rfModStructs::monitorStruct() );
        continuousMonitorStructs.back() -> monType     = it.first;
        continuousMonitorStructs.back() -> rfModObject = &gunMod;
        continuousMonitorStructs.back() -> interface  = this;
        ca_create_subscription(it.second.CHTYPE, it.second.COUNT,  it.second.CHID,
                               it.second.MASK, gunModInterface::staticEntryGunModMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back() -> EVID);
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to gun modulator Monitors",
                             "!!TIMEOUT!! Subscription to gun modulator monitors failed");
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; // interface base class member
}
//____________________________________________________________________________________________
const rfModStructs::gunModObject& gunModInterface::getGunObjConstRef()
{
    return gunMod;
}
//____________________________________________________________________________________________
void gunModInterface::staticEntryGunModMonitor(const event_handler_args args)
{
    rfModStructs::monitorStruct*ms = static_cast<rfModStructs::monitorStruct*>(args.usr);
    switch(ms -> monType)
    {
        case rfModStructs::GUN_MOD_PV_TYPE::MAIN_STATE_READ:
            ms->interface->updateMainState(args.dbr);
            break;
//        case rfModStructs::GUN_MOD_PV_TYPE::ERROR_READ:
//            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::ERROR_READ = ",*(double*)args.dbr);
//            ms->interface->updateModIlock(args);
//            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ERROR_READ_HEX_STR:
            ms->interface->updateHexString(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::WARMUP_TIME:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::WARMUP_TIME = ",*(long*)args.dbr);
            ms->interface->updateWarmUpTime(*(long*)args.dbr);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS1_CURR_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS1_CURR_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs1CurrRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS2_CURR_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS2_CURR_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs2CurrRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS3_CURR_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS3_CURR_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs3CurrRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS4_CURR_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS4_CURR_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs4CurrRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS1_VOLT_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS1_VOLT_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs1VoltRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS2_VOLT_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS2_VOLT_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs2VoltRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS3_VOLT_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS3_VOLT_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs3VoltRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS4_VOLT_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::MAGPS4_VOLT_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.MagPs4VoltRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::CT_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::CT_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.CtRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::CVD_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::CVD_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.CvdRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.PlswthRead  =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_FWHM_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_FWHM_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.PlswthFwhmRead =  *(double*)args.dbr;
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::IONP_PRESSURE_READ:
            ms->interface->debugMessage("rfModStructs::GUN_MOD_PV_TYPE::IONP_PRESSURE_READ = ",*(double*)args.dbr);
            ms->interface->gunMod.ionp =  *(double*)args.dbr;
            break;
        default:
            ms->interface->message("!!! ERROR !!! Unknown Monitor Type passed to gunModInterface::staticEntryGunModMonitor");
            break;
    }
}
//____________________________________________________________________________________________
void gunModInterface::updateMainState(const void * argsdbr)
{
    switch( *(unsigned short*)argsdbr )
    {
        case 0:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::NOT_CONNECTED;
            break;
        case 1:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::STANDYBY_INTERLOCK;
            break;
        case 2:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::OFF;
            break;
        case 3:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::OFF_REQUEST;
            break;
        case 4:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_INTERLOCK;
            break;
        case 5:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::STANDBY_REQUEST;
            break;
        case 6:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::STANDBY;
            break;
        case 7:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_OFF_REQUEST;
            break;
        case 8:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::TRIGGER_INTERLOCK;
            break;
        case 9:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_REQUEST;
            break;
        case 10:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_ON;
            break;
        case 11:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::TRIG_OFF_REQUEST;
            break;
        case 12:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::TRIG_REQUEST;
            break;
        case 13:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::TRIG;
            break;
        default:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::UNKNOWN_STATE;
            break;
    }
    message( gunMod.name," ",gunMod.name," state changed to ",ENUM_TO_STRING(gunMod.main_state) );
}
//______________________________________________________________________________
void gunModInterface::updateHexString(const event_handler_args& args)
{
    std::stringstream ss;
    ss << *(char*)args.dbr;
    gunMod.hex_state_str = ss.str();
    convertHexStringToMessage();
    updateErrorState();
}
//______________________________________________________________________________
bool gunModInterface::convertHexStringToMessage()
{
    if( entryExists(gun_mod_hex_map,gunMod.hex_state_str))
    {
        gunMod.hex_state_message = gun_mod_hex_map.at(gunMod.hex_state_str);
        return true;
    }
    else
    {
        gunMod.hex_state_message = gunMod.hex_state_str + " is UNKNOWN HexCode";
    }
    return false;
}
//______________________________________________________________________________
void gunModInterface::updateErrorState()
{
    /* based on the hexcode */
    if(std::find(rfModStructs::good_gun_hex_codes.begin(),
                 rfModStructs::good_gun_hex_codes.end(),
                 gunMod.hex_state_str) != rfModStructs::good_gun_hex_codes.end())
    {
        gunMod.error_state = rfModStructs::GUN_MOD_ERR_STATE::GOOD;
    }
    else
    {
        if(entryExists(gun_mod_hex_map,gunMod.hex_state_str))
        {
            gunMod.error_state = rfModStructs::GUN_MOD_ERR_STATE::BAD;
        }
        else
        {
            gunMod.error_state = rfModStructs::GUN_MOD_ERR_STATE::UNKNOWN;
        }
    }
}
//____________________________________________________________________________________________
void gunModInterface::updateWarmUpTime(const long val)
{
    gunMod.warmuptime =  val;
    if( gunMod.warmuptime == 0 )
    {
        gunMod.safelyWarmedUP = true;
        message( "Gun Modulator Safely Warmed Up:");
    }
    else
    {
        gunMod.safelyWarmedUP = false;
    }
}
//______________________________________________________________________________
bool gunModInterface::isWarmedUp() const
{
    return gunMod.safelyWarmedUP;
}
//______________________________________________________________________________
bool gunModInterface::isNotWarmedUp() const
{
    return !isWarmedUp();
}
//______________________________________________________________________________
bool gunModInterface::isInTrig() const
{
    if(gunMod.main_state == rfModStructs::GUN_MOD_STATE::TRIG)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isInHVOn() const
{
    if(gunMod.main_state == rfModStructs::GUN_MOD_STATE::HV_ON)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isInStandby() const
{
    if(gunMod.main_state  == rfModStructs::GUN_MOD_STATE::STANDBY)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isInOff() const
{
    if(gunMod.main_state == rfModStructs::GUN_MOD_STATE::OFF)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
rfModStructs::GUN_MOD_STATE gunModInterface::getMainState() const
{
    return gunMod.main_state;
}
//______________________________________________________________________________
rfModStructs::GUN_MOD_ERR_STATE gunModInterface::getErrorState() const
{
    return gunMod.error_state;
}
//______________________________________________________________________________
bool gunModInterface::isErrorStateGood() const
{
    if(gunMod.error_state == rfModStructs::GUN_MOD_ERR_STATE::GOOD)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
void gunModInterface::reset()
{
    caput( gunMod.pvComStructs.at(rfModStructs::GUN_MOD_PV_TYPE::RESET).CHTYPE,
           gunMod.pvComStructs.at(rfModStructs::GUN_MOD_PV_TYPE::RESET).CHID,
           EPICS_RESET, "" , "!!gunModInterface TIMEOUT!! In modReset() ");
}
//______________________________________________________________________________
bool gunModInterface::resetAndWait( const size_t waitTime )
{
    message("modreset");
    reset();
    return waitFor( &gunModInterface::isErrorStateGood, *this, "Timeout waiting for Modulator to reset ",  waitTime ); // MAGIC_NUMBER
}
//______________________________________________________________________________
bool gunModInterface::waitFor(ABoolMemFn f1,gunModInterface& obj, const std::string& m,
                           const  time_t waitTime, const size_t pause)
{
    return waitFor(f1, obj, m.c_str(), waitTime);
}
//______________________________________________________________________________
bool gunModInterface::waitFor(ABoolMemFn f1,gunModInterface& obj, const char * m,
                             const time_t waitTime,const size_t pause )
{
    time_t t0 = timeNow();
    bool notTimedOut = true;
    while(true)
    {
        if(CALL_MEMBER_FN(obj, f1)())
        {
            break;
        }
        if(timeNow() > t0 + waitTime)
        {
            message(m);
            notTimedOut = false;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    }
    return notTimedOut;
}
//______________________________________________________________________________
bool gunModInterface::waitForModState(rfModStructs::GUN_MOD_STATE state, time_t waitTime )
{
    time_t t0 = timeNow();
    bool notTimedOut = true;
    while(true)
    {
        if(state == getMainState())
        {
            break;
        }
        else if( timeNow() > t0 + waitTime )
        {
            message("Timed Out While Waiting For Modulator to reach state = ", ENUM_TO_STRING(state));
            notTimedOut = false;
            break;
        }
    }
    return notTimedOut;
}








