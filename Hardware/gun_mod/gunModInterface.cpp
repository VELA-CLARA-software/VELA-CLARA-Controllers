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
gunModInterface::gunModInterface(const std::string &gunModConf,
                                const bool startVirtualMachine,
                                const bool& show_messages_ptr, const bool& show_debug_messages_ptr,
                                const bool shouldStartEPICs):
configReader(gunModConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr),
interface(show_messages_ptr,show_debug_messages_ptr,shouldStartEPICs),
//shouldStartEPICs(shouldStartEPICs),
allChidsInitialised(false),
gun_mod_hex_map(rfModStructs::gunModHexErrorCodes::create()),
VOLT("VOLT"),
CURR("CURR")
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
    killILockMonitors();
    for(auto && it:continuousMonitorStructs)
    {
        killMonitor(it);
        delete it;
    }
}
//______________________________________________________________________________
void gunModInterface::killMonitor(rfModStructs::monitorStruct* ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
    {
        message("delete ",ENUM_TO_STRING(ms->monType));
        delete ms;
    }
}
//______________________________________________________________________________
void gunModInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(UTL::STANDARD_PAUSE);
    if(configFileRead)
    {
        message("The gunModInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = configReader.getGunModObject(gunMod);
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
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
            message("!!!The gunModInterface received an Error while getting laser data!!!");
    }
}
//______________________________________________________________________________
void gunModInterface::initChids()
{
    message("\n", "Searching for gunMod ChIds...");

    for(auto && it : gunMod.pvMonStructs)
    {
        addChannel(gunMod.pvRoot, it.second);
    }
    // currently there are no command only PVs for the PIL
    for(auto && it : gunMod.pvComStructs)
    {
        addChannel(gunMod.pvRoot, it.second);
    }
    //addILockChannels(gunMod.numIlocks, gunMod.pvRoot, gunMod.name, gunMod.iLockPVStructs);
    int status = sendToEpics("ca_create_channel", "Found gunMod ChIds.", "!!TIMEOUT!! Not all gunMod ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//MAGIC_NUMBER
        message("\n", "Checking gunMod ChIds ");
        for(auto && it : gunMod.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto && it : gunMod.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // MAGIC_NUMBER
    }
    else if (status == ECA_NORMAL)
        allChidsInitialised = true;  /// interface base class member
}
////______________________________________________________________________________
void gunModInterface::addChannel(const std::string & pvRoot, rfModStructs::pvStruct & pv)
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel(s1.c_str(), 0, 0, 0, &pv.CHID);//MAGIC_NUMBER
    debugMessage("Create channel to ", s1);
}
//______________________________________________________________________________
void gunModInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : gunMod.pvMonStructs)
    {
        continuousMonitorStructs.push_back(new rfModStructs::monitorStruct());
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
    if (status == ECA_NORMAL)
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
    // do a test for a magnet or hv number or other PV_TYPEe

    switch(ms->interface->is_MAGPS_or_HVCPS_PV(ms -> monType))
    {
        case UTL::ZERO_INT:
            ms->interface->updateValue(args,ms->monType);
            break;
        case UTL::ONE_INT:
            ms->interface->updateMAGPS_PV(args, ms->interface->getPVNum(ms->monType),ms->monType);
            break;
        case UTL::TWO_INT:
            ms->interface->updateHVPS_PV(args, ms->interface->getPVNum(ms->monType),ms->monType);
            break;
        default:
            ms->interface->message("ERROR in valu epassed to gunModInterface::staticEntryGunModMonitor");
    }
}
//____________________________________________________________________________________________
void gunModInterface::updateValue(const event_handler_args& args,const rfModStructs::GUN_MOD_PV_TYPE pv)
{
    switch(pv)
    {
        case rfModStructs::GUN_MOD_PV_TYPE::MAIN_STATE_READ:
            updateMainState(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ERROR_READ_HEX_STR:
            updateHexString(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::STATE_SET_READ:
            updateStateSetRead(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::WARMUP_TIME:
            updateWarmUpTime(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::CT_READ:
            updateCtRead(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::CVD_READ:
            updateCvdRead(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_READ:
            updatePlswthRead(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_FWHM_READ:
            updatePlswthFwhmRead(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::IONP_PRESSURE_READ:
            debugMessage("rfModStructs::GUN_MOD_PV_TYPE::IONP_PRESSURE_READ = ", getDBRdouble(args));
            gunMod.ionp = getDBRdouble(args);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ILOCK1_STR:
            updateIlock_string(args, UTL::ONE_SIZET);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ILOCK2_STR:
            updateIlock_string(args, UTL::TWO_SIZET);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ILOCK3_STR:
            updateIlock_string(args, UTL::THREE_SIZET);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ILOCK4_STR:
            updateIlock_string(args, UTL::FOUR_SIZET);
            break;
        case rfModStructs::GUN_MOD_PV_TYPE::ILOCK5_STR:
            updateIlock_string(args, UTL::FIVE_SIZET);
            break;
        default:
            message("!!! ERROR !!! Unknown Monitor Type", ENUM_TO_STRING(pv)," passed to gunModInterface::staticEntryGunModMonitor");
            break;
    }
}
//____________________________________________________________________________________________
// REALLY CANCEROUS BELOW :(
//____________________________________________________________________________________________
void gunModInterface::updateMAGPS_PV(const event_handler_args& args,const size_t num,const rfModStructs::GUN_MOD_PV_TYPE pv)
{
    double val = getDBRdouble(args);
    if(isCURR_PV(pv))
    {
        switch(num)
        {
            case UTL::ONE_SIZET:
                gunMod.MagPs1CurrRead = val;
                break;
            case UTL::TWO_SIZET:
                gunMod.MagPs2CurrRead = val;
                break;
            case UTL::THREE_SIZET:
                gunMod.MagPs3CurrRead = val;
                break;
            case UTL::FOUR_SIZET:
                gunMod.MagPs4CurrRead = val;
                break;
        }
    }
    else if(isVOLT_PV(pv))
    {
        switch(num)
        {
            case UTL::ONE_SIZET:
                gunMod.MagPs1VoltRead = val;
                break;
            case UTL::TWO_SIZET:
                gunMod.MagPs2VoltRead = val;
                break;
            case UTL::THREE_SIZET:
                gunMod.MagPs3VoltRead = val;
                break;
            case UTL::FOUR_SIZET:
                gunMod.MagPs4VoltRead = val;
                break;
        }
    }
}
//____________________________________________________________________________________________
void gunModInterface::updateHVPS_PV(const event_handler_args& args,const size_t num,const rfModStructs::GUN_MOD_PV_TYPE pv)
{
    double val = getDBRdouble(args);
    if(isCURR_PV(pv))
    {
        switch(num)
        {
            case UTL::ONE_SIZET:
                gunMod.HvPs1CurrRead = val;
                break;
            case UTL::TWO_SIZET:
                gunMod.HvPs2CurrRead = val;
                break;
            case UTL::THREE_SIZET:
                gunMod.HvPs3CurrRead = val;
                break;
        }
    }
    else if(isVOLT_PV(pv))
    {
        switch(num)
        {
            case UTL::ONE_SIZET:
                gunMod.HvPs1VoltRead = val;
                break;
            case UTL::TWO_SIZET:
                gunMod.HvPs2VoltRead = val;
                break;
            case UTL::THREE_SIZET:
                gunMod.HvPs3VoltRead = val;
                break;
        }
    }
}
//____________________________________________________________________________________________
size_t gunModInterface::getPVNum(const rfModStructs::GUN_MOD_PV_TYPE pv)
{
    const std::string pv_string = ENUM_TO_STRING(pv);
    std::size_t const n = pv_string.find_first_of("1234");
    return n;
}
//____________________________________________________________________________________________
void gunModInterface::updateMainState(const event_handler_args& args)
{
    message("updateMainState, passed = ",getDBRunsignedShort(args));
    switch( getDBRunsignedShort(args))
    {
        case 0:
            gunMod.main_state_string = "Init/not conne.";
            gunMod.main_state        = rfModStructs::GUN_MOD_STATE::NOT_CONNECTED;
            break;
        case 1:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::STANDYBY_INTERLOCK;
            gunMod.main_state_string = "Standby Interl.";
            break;
        case 2:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::OFF;
            gunMod.main_state_string = "Off";
            break;
        case 3:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::OFF_REQUEST;
            gunMod.main_state_string = "Off Request";
            break;
        case 4:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_INTERLOCK;
            gunMod.main_state_string = "HV Intrlock";
            break;
        case 5:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::STANDBY_REQUEST;
            gunMod.main_state_string = "Standby Request";
            break;
        case 6:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::STANDBY;
            gunMod.main_state_string = "Standby";
            break;
        case 7:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_OFF_REQUEST;
            gunMod.main_state_string = "HV Off Requ.";
            break;
        case 8:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::RF_ON_INTERLOCK;
            gunMod.main_state_string = "RF On Interl.";
            break;
        case 9:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_REQUEST;
            gunMod.main_state_string = "HV Request";
            break;
        case 10:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::HV_ON;
            gunMod.main_state_string = "HV On";
            break;
        case 11:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::RF_OFF_REQUEST;
            gunMod.main_state_string = "RF Off Req.";
            break;
        case 12:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::RF_ON_REQUEST;
            gunMod.main_state_string = "RF On Request";
            break;
        case 13:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::RF_ON;
            gunMod.main_state_string = "RF On";
            break;
        default:
            gunMod.main_state= rfModStructs::GUN_MOD_STATE::UNKNOWN_STATE;
            gunMod.main_state_string = "Unknown";
            break;
    }
    message(gunMod.name," ",gunMod.name," state changed to ",ENUM_TO_STRING(gunMod.main_state));
}
//______________________________________________________________________________
void gunModInterface::updateHexString(const event_handler_args& args)
{
    gunMod.hex_state_str = getDBRstring(args);
    convertHexStringToMessage();
    updateErrorState();
}
//______________________________________________________________________________
bool gunModInterface::convertHexStringToMessage()
{
    if(entryExists(gun_mod_hex_map,gunMod.hex_state_str))
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
void gunModInterface::updateStateSetRead(const event_handler_args& args)
{

    switch( getDBRunsignedShort(args))
    {
        case 0:
            gunMod.state_set_read        = rfModStructs::GUN_MOD_STATE_SET::SET_OFF;
            gunMod.state_set_read_string = "OFF";
            break;
        case 1:
            gunMod.state_set_read        = rfModStructs::GUN_MOD_STATE_SET::SET_STANDBY;
            gunMod.state_set_read_string = "STANDBY";
            break;
        case 2:
            gunMod.state_set_read        = rfModStructs::GUN_MOD_STATE_SET::SET_HV_ON;
            gunMod.state_set_read_string = "HV_ON";
            break;
        case 3:
            gunMod.state_set_read        = rfModStructs::GUN_MOD_STATE_SET::SET_RF_ON;
            gunMod.state_set_read_string = "RF_ON";
            break;
        default:
            gunMod.state_set_read        = rfModStructs::GUN_MOD_STATE_SET::UNKNOWN_SET_STATE;
            gunMod.state_set_read_string = "UNKNOWN_STATE";
            break;
    }
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
void gunModInterface::updateWarmUpTime(const event_handler_args& args)
{
    gunMod.warmuptime =  getDBRlong(args);
    debugMessage("rfModStructs::GUN_MOD_PV_TYPE::WARMUP_TIME = ",gunMod.warmuptime);
    if(gunMod.warmuptime == 0)
    {
        //if the mode is standby or better??

        gunMod.safelyWarmedUP = true;
        message("Gun Modulator Safely Warmed Up:");

        //    !!!!!!!!!!!!!!this is not correct!!!!!!!!!!!!!!!!
        //    !!!!!!!!!!!!!!this is not correct!!!!!!!!!!!!!!!!
        //    !!!!!!!!!!!!!!this is not correct!!!!!!!!!!!!!!!!
        //    !!!!!!!!!!!!!!this is not correct!!!!!!!!!!!!!!!!

    }
    else
    {
        gunMod.safelyWarmedUP = false;
    }
}
//____________________________________________________________________________________________
void gunModInterface::updateCtRead(const event_handler_args& args)
{
    gunMod.CtRead = getDBRdouble(args);
    //debugMessage("rfModStructs::GUN_MOD_PV_TYPE::CT_READ = ",gunMod.CtRead);
}
//____________________________________________________________________________________________
void gunModInterface::updateCvdRead(const event_handler_args& args)
{
    gunMod.CvdRead = getDBRdouble(args);
    //debugMessage("rfModStructs::GUN_MOD_PV_TYPE::CVD_READ = ",gunMod.CvdRead);
}
//____________________________________________________________________________________________
void gunModInterface::updatePlswthRead(const event_handler_args& args)
{
    gunMod.PlswthRead = getDBRdouble(args);
    //debugMessage("rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_READ = ", gunMod.PlswthRead);
}
//____________________________________________________________________________________________
void gunModInterface::updatePlswthFwhmRead(const event_handler_args& args)
{
    gunMod.PlswthFwhmRead = getDBRdouble(args);
    //debugMessage("rfModStructs::GUN_MOD_PV_TYPE::PULSE_WIDTH_FWHM_READ = ", gunMod.PlswthFwhmRead);
}
//____________________________________________________________________________________________
void gunModInterface::updateIlock_string(const event_handler_args& args, size_t ilock_number)
{
    const std::string new_val = getDBRstring(args);
    switch(ilock_number)
    {
        case 1:
            check_ilock_changed(new_val, gunMod.ilock1,ilock_number);
            break;
        case 2:
            check_ilock_changed(new_val, gunMod.ilock2,ilock_number);
            break;
        case 3:
            check_ilock_changed(new_val, gunMod.ilock3,ilock_number);
            break;
        case 4:
            check_ilock_changed(new_val, gunMod.ilock4,ilock_number);
            break;
        case 5:
            check_ilock_changed(new_val, gunMod.ilock5,ilock_number);
            break;
        default:
            message("!!ERROR IN updateIlock_string!! ",ilock_number," is an unknown/unepected ilock to update with message= ",new_val);
    }
}
//____________________________________________________________________________________________
void gunModInterface::check_ilock_changed(const std::string& new_val,std::string& present_val,size_t num)
{
    if(new_val == present_val)
    {
    }
    else
    {
        present_val = new_val;
        //debugMessage("rfModStructs::GUN_MOD_PV_TYPE::ILOCK",num,"_STR = ",present_val);
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
bool gunModInterface::isInRFOn() const
{
    if(gunMod.main_state == rfModStructs::GUN_MOD_STATE::RF_ON)
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
    caput(gunMod.pvComStructs.at(rfModStructs::GUN_MOD_PV_TYPE::RESET).CHTYPE,
           gunMod.pvComStructs.at(rfModStructs::GUN_MOD_PV_TYPE::RESET).CHID,
           EPICS_RESET, "" , "!!gunModInterface TIMEOUT!! In modReset() ");
}
//______________________________________________________________________________
bool gunModInterface::resetAndWait(const size_t waitTime)
{
    message("modreset");
    reset();
    return waitFor(&gunModInterface::isErrorStateGood, *this, "Timeout waiting for Modulator to reset ",  waitTime); // MAGIC_NUMBER
}
//______________________________________________________________________________
bool gunModInterface::waitFor(ABoolMemFn f1,gunModInterface& obj, const std::string& m,
                           const  time_t waitTime, const size_t pause)
{
    return waitFor(f1, obj, m.c_str(), waitTime);
}
//______________________________________________________________________________
bool gunModInterface::waitFor(ABoolMemFn f1,gunModInterface& obj, const char * m,
                             const time_t waitTime,const size_t pause)
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
bool gunModInterface::waitForModState(rfModStructs::GUN_MOD_STATE state, time_t waitTime)
{
    time_t t0 = timeNow();
    bool notTimedOut = true;
    while(true)
    {
        if(state == getMainState())
        {
            break;
        }
        else if(timeNow() > t0 + waitTime)
        {
            message("Timed Out While Waiting For Modulator to reach state = ", ENUM_TO_STRING(state));
            notTimedOut = false;
            break;
        }
    }
    return notTimedOut;
}
//______________________________________________________________________________
int gunModInterface::is_MAGPS_or_HVCPS_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const
{
    if(isMAGPS_PV(pv))
    {
        return UTL::ONE_INT;
    }
    else if(isHVPS_PV(pv))
    {
        return UTL::TWO_INT;
    }
    return UTL::ZERO_INT;
}
//______________________________________________________________________________
bool gunModInterface::isMAGPS_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const
{
    switch(pv)
    {
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS1_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS2_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS3_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS4_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS1_VOLT_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS2_VOLT_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS3_VOLT_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::MAGPS4_VOLT_READ:
            return true;
        default:
            return false;
    }
}
//______________________________________________________________________________
bool gunModInterface::isHVPS_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const
{
    switch(pv)
    {
        case rfModStructs::GUN_MOD_PV_TYPE::HVPS1_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::HVPS2_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::HVPS3_CURR_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::HVPS1_VOLT_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::HVPS2_VOLT_READ:
            return true;
        case rfModStructs::GUN_MOD_PV_TYPE::HVPS3_VOLT_READ:
            return true;
        default:
            return false;
    }
}
//______________________________________________________________________________
bool gunModInterface::isVOLT_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const
{
    if(stringIsSubString(ENUM_TO_STRING(pv),VOLT))
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::isCURR_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const
{
    if(stringIsSubString(ENUM_TO_STRING(pv),CURR))
    {
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool gunModInterface::setOff() const
{
    return setModState(rfModStructs::GUN_MOD_STATE_SET::SET_OFF);
}
//______________________________________________________________________________
bool gunModInterface::setStandby() const
{
    return setModState(rfModStructs::GUN_MOD_STATE_SET::SET_STANDBY);
}
//______________________________________________________________________________
bool gunModInterface::setHVOn() const
{
    return setModState(rfModStructs::GUN_MOD_STATE_SET::SET_HV_ON);
}
//______________________________________________________________________________
bool gunModInterface::setRFOn() const
{
    return setModState(rfModStructs::GUN_MOD_STATE_SET::SET_RF_ON);
}
//______________________________________________________________________________
bool gunModInterface::setModState(const rfModStructs::GUN_MOD_STATE_SET v)const
{
    ca_put(
        gunMod.pvComStructs.at(rfModStructs::GUN_MOD_PV_TYPE::STATE_SET).CHTYPE,
        gunMod.pvComStructs.at(rfModStructs::GUN_MOD_PV_TYPE::STATE_SET).CHID,
        &v);
    bool ret = false;
    int status = sendToEpics("ca_put", "", "Timeout sending Set Mod values");
    if (status == ECA_NORMAL)
        ret = true;
    return ret;
}




