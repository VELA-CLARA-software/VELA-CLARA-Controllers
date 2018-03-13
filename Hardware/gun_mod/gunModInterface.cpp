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

#include "gunModInterface.h"
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
gunModInterface::gunModInterface( const std::string &gunModConf,
                                const bool startVirtualMachine,
                                const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                                const bool shouldStartEPICs ):
configReader(gunModConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs( shouldStartEPICs ),
allChidsInitialised(false)
{
    if( shouldStartEPICs )
        message("gunModInterface shouldStartEPICs is true");
    else
        message("gunModInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
gunModInterface::~gunModInterface()
{
//    killILockMonitors();
//    for( auto && it : continuousMonitorStructs )
//    {
//        killMonitor( it );
//        delete it;
//    }
//    debugMessage( "gunModInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
//void gunModInterface::killMonitor( pilaserStructs::monitorStruct * ms )
//{
//    int status = ca_clear_subscription( ms -> EVID );
//    //if( status == ECA_NORMAL)
//        //debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
////    else
//        //debugMessage("ERROR gunModInterface: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
//}
//______________________________________________________________________________
void gunModInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
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
                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
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
    rfModStructs::monitorStruct*ms = static_cast<rfModStructs::monitorStruct*>(args.usr);
    std::stringstream ss;
    ss << *(char*)args.dbr;

    gunMod.hex_state_str = ss.str();

    convertHexStringToMessage();

}
//______________________________________________________________________________
rfModStructs::GUN_GUN_MOD_ERR_STATE RFGunInterface::convertModErrorReadStr( const char * epicsModErrCodeString )
{
    std::stringstream ss;
    ss << epicsModErrCodeString;

    if( isAGoodModErrorReadStr( ss.str() ) )
        return rfModStructs::GUN_MOD_ERR_STATE::GOOD;
    else if( isABadModErrorReadStr( ss.str() ) )
        return rfModStructs::GUN_MOD_ERR_STATE::BAD;
    else
        return rfModStructs::GUN_MOD_ERR_STATE::UNKNOWN;
}
//______________________________________________________________________________
rfModStructs::GUN_GUN_MOD_ERR_STATE RFGunInterface::convertModErrorRead( const double v )
{
    switch( (int)v )
    {
        case 0:
            return rfModStructs::GUN_GUN_MOD_ERR_STATE::GOOD;
            break;
        case 2000:
            return rfModStructs::GUN_GUN_MOD_ERR_STATE::GOOD;
            break;
        default:
            return rfModStructs::GUN_GUN_MOD_ERR_STATE::BAD;
            break;
    }
}
//______________________________________________________________________________
bool RFGunInterface::isAGoodModErrorReadStr( const std::string & s )
{
//    for( auto && it : RFObject.mod.goodModErrorReadStr )
//        message( "RFObject.mod.goodModErrorReadStr = ", it);
//
//    message("Compare with ", s );
//
//    message(  s == RFObject.mod.goodModErrorReadStr[0] ) ;

    if (std::find( RFObject.mod.goodModErrorReadStr.begin(), RFObject.mod.goodModErrorReadStr.end(), s ) != RFObject.mod.goodModErrorReadStr.end())
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::isABadModErrorReadStr( const std::string & s )
{
    if (std::find( RFObject.mod.badModErrorReadStr.begin(), RFObject.mod.badModErrorReadStr.end(), s ) != RFObject.mod.badModErrorReadStr.end())
        return true;
    else
        return false;
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
bool gunModInterface::isModWarmedUp()
{
    return gunMod.safelyWarmedUP;
}
//______________________________________________________________________________
bool gunModInterface::isModNotWarmedUp()
{
    return !isModWarmedUp();
}
//______________________________________________________________________________
bool gunModInterface::isModInTrig()
{
    if(gunMod.state == rfModStructs::MOD_STATE::TRIG)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isModInHVOn()
{
    if(gunMod.state == rfModStructs::MOD_STATE::HV_OO)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isModInStandby()
{
    if(gunMod.state  == rfModStructs::MOD_STATE::STANDBY)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isModInOff()
{
    if(gunMod.state == rfModStructs::MOD_STATE::OFF)
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
void gunModInterface::modReset()
{
    caput( gunMod.pvComStructs[ rfModStructs::RF_PV_TYPE::MOD_RESET ].CHTYPE,
           gunMod.pvComStructs[ rfModStructs::RF_PV_TYPE::MOD_RESET ].CHID,
           EPICS_RESET, "" , "!!gunModInterface TIMEOUT!! In modReset() ");
}
//______________________________________________________________________________
bool gunModInterface::modResetAndWait( const size_t waitTime )
{
    message("modreset");
    modReset();
    return waitFor( &gunModInterface::isModILockStateGood, *this, "Timeout waiting for Modulator to reset ",  waitTime ); // MAGIC_NUMBER
}
//______________________________________________________________________________
rfModStructs::MOD_STATE gunModInterface::getModMainState() const
{
    return gunMod.main_state;
}
//______________________________________________________________________________
rfModStructs::MOD_STATE gunModInterface::getModErrorState() const
{
    return gunMod.error_state;
}
//______________________________________________________________________________
rfModStructs::GUN_GUN_MOD_ERR_STATE gunModInterface::getModiLock() const
{
    return gunMod.ilock_state;
}






bool convertHexStringToMessage()
{
    r = true;




}

























