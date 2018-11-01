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

#include "l01ModInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>

l01ModInterface::l01ModInterface(const std::string &l01ModConf,
                                const bool startVirtualMachine,
                                const bool& show_messages_ptr,
                                const bool& show_debug_messages_ptr,
                                const bool shouldStartEPICs
                           ):
configReader(l01ModConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr),
interface(show_messages_ptr,show_debug_messages_ptr,shouldStartEPICs)
//shouldStartEPICs(shouldStartEPICs)
{
    if(shouldStartEPICs)
        message("l01ModInterface shouldStartEPICs is true");
    else
        message("l01ModInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
l01ModInterface::~l01ModInterface()
{
    killILockMonitors();
    for(auto && it : continuousMonitorStructs)
    {
        killMonitor(it);
        delete it;
    }
}
//______________________________________________________________________________
void l01ModInterface::killMonitor(rfModStructs::l01_monitorStruct* ms)
{
    int status = ca_clear_subscription(ms -> EVID);
}
//______________________________________________________________________________
void l01ModInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // MAGIC_NUMBER
    if(configFileRead)
    {
        message("The l01ModInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = configReader.getL01ModObject(l01Mod);
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The l01ModInterface has acquired objects, connecting to EPICS");
                //std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions

                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // MAGIC_NUMBER
            }
            else
             message("The l01ModInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message("!!!The l01ModInterface received an Error while getting laser data!!!");
    }
}
//______________________________________________________________________________
void l01ModInterface::initChids()
{
    message("\n", "Searching for l01Mod ChIds...");

    for(auto && it : l01Mod.pvMonStructs)
    {
        addChannel(l01Mod.pvRoot, it.second);
    }
    // currently there are no command only PVs for the PIL
    for(auto && it : l01Mod.pvComStructs)
    {
        addChannel(l01Mod.pvRoot, it.second);
    }
    //addILockChannels(l01Mod.numIlocks, l01Mod.pvRoot, l01Mod.name, l01Mod.iLockPVStructs);
    int status = sendToEpics("ca_create_channel", "Found l01Mod ChIds.", "!!TIMEOUT!! Not all l01Mod ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//MAGIC_NUMBER
        message("\n", "Checking l01Mod ChIds ");
        for(auto && it : l01Mod.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto && it : l01Mod.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // MAGIC_NUMBER
    }
    else if (status == ECA_NORMAL)
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void l01ModInterface::addChannel(const std::string& pvRoot, rfModStructs::l01_pvStruct& pv)
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel(s1.c_str(), 0, 0, 0, &pv.CHID);//MAGIC_NUMBER
    debugMessage("Create channel to ", s1);
}
////______________________________________________________________________________
void l01ModInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : l01Mod.pvMonStructs)
    {
        continuousMonitorStructs.push_back(new rfModStructs::l01_monitorStruct());
        continuousMonitorStructs.back()->monType     = it.first;
        continuousMonitorStructs.back()->rfModObject = &l01Mod;
        continuousMonitorStructs.back()->interface  = this;
        ca_create_subscription(it.second.CHTYPE, it.second.COUNT,  it.second.CHID,
                               it.second.MASK, l01ModInterface::staticEntryL01ModMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back()->EVID);

        message("Monitoring ", ENUM_TO_STRING(it.first));
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to L01 modulator Monitors",
                              "!!TIMEOUT!! Subscription to L01 modulator monitors failed");
    if (status == ECA_NORMAL)
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
const rfModStructs::l01ModObject& l01ModInterface::getObjConstRef()
{
    return l01Mod;
}
//______________________________________________________________________________
bool l01ModInterface::reset()
{
    message("HELLO RESET!");

    bool ret = false;

    ca_put(l01Mod.pvComStructs[rfModStructs::L01_MOD_PV_TYPE::L01_RESET].CHTYPE,
           l01Mod.pvComStructs[rfModStructs::L01_MOD_PV_TYPE::L01_RESET].CHID,
           &EPICS_ACTIVATE);
    int status = sendToEpics("ca_put", "", "TIMEOUT Activating L01 Modulator Reset");
    if(status == ECA_NORMAL)
    {
        ca_put(l01Mod.pvComStructs[rfModStructs::L01_MOD_PV_TYPE::L01_RESET].CHTYPE,
               l01Mod.pvComStructs[rfModStructs::L01_MOD_PV_TYPE::L01_RESET].CHID,
               &EPICS_SEND);

        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // MAGIC_NUMBER

        status = sendToEpics("ca_put",  "", "TIMEOUT Sending L01 Modulator Reset");
        if(status == ECA_NORMAL)
            ret = true;

    }
    return ret;
}
//____________________________________________________________________________________________

//______________________________________________________________________________
void l01ModInterface::updateErrorWordList(const int part, const char * value)
{
    l01Mod.modErrorWords[part] = value;
#ifdef BUILD_DLL
    l01Mod.modErrorWordsL[part] = value;
#endif
}
//______________________________________________________________________________
void l01ModInterface::updateErrorDescList(const int part, const char * value)
{
    l01Mod.modErrorDescs[part] = value;
#ifdef BUILD_DLL
    l01Mod.modErrorDescsL[part] = value;
#endif
}
//______________________________________________________________________________
void l01ModInterface::staticEntryL01ModMonitor(const event_handler_args args)
{
    rfModStructs::l01_monitorStruct* ms = static_cast<rfModStructs::l01_monitorStruct*>(args.usr);


    if(ms->monType == rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ)
    {
    ms->interface->message("staticEntryL01ModMonitor PASSED ", ENUM_TO_STRING(ms->monType));
    }

    switch(ms->monType)
    {
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_1:
            ms->interface->message("modErrorWords[0] = ",  (const char *)args.dbr);
            //ms->interface->updateErrorWordList(0, (const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_2:
            ms->interface->updateErrorWordList(1, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[1]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_3:
            ms->interface->updateErrorWordList(2, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[2]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_4:
            ms->interface->updateErrorWordList(3, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[3]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_5:
            ms->interface->updateErrorWordList(4, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[4]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_6:
            ms->interface->updateErrorWordList(5, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[5]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_7:
            ms->interface->updateErrorWordList(6, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[6]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_8:
            ms->interface->updateErrorWordList(7, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[7]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_9:
            ms->interface->updateErrorWordList(8, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[8]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_10:
            ms->interface->updateErrorWordList(9, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[9]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_11:
            ms->interface->updateErrorWordList(10, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[10]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_12:
            ms->interface->updateErrorWordList(11, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[11]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_13:
            ms->interface->updateErrorWordList(12, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[12]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_14:
            ms->interface->updateErrorWordList(13, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[13]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_15:
            ms->interface->updateErrorWordList(14, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[14]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_16:
            ms->interface->updateErrorWordList(15, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[15]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_17:
            ms->interface->updateErrorWordList(16, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[16]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_18:
            ms->interface->updateErrorWordList(17, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[17]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_19:
            ms->interface->updateErrorWordList(18, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[18]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_WORD_20:
            ms->interface->updateErrorWordList(19, (const char *)args.dbr);
            //ms->interface->message("modErrorWords[19]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_1:
            ms->interface->updateErrorDescList(0, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[0]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_2:
            ms->interface->updateErrorDescList(1, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[1]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_3:
            ms->interface->updateErrorDescList(2, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[2]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_4:
            ms->interface->updateErrorDescList(3, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[3]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_5:
            ms->interface->updateErrorDescList(4, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[4]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_6:
            ms->interface->updateErrorDescList(5, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[5]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_7:
            ms->interface->updateErrorDescList(6, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[6]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_8:
            ms->interface->updateErrorDescList(7, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[7]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_9:
            ms->interface->updateErrorDescList(8, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[8]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_10:
            ms->interface->updateErrorDescList(9, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[9]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_11:
            ms->interface->updateErrorDescList(10, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[10]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_12:
            ms->interface->updateErrorDescList(11, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[11]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_13:
            ms->interface->updateErrorDescList(12, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[12]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_14:
            ms->interface->updateErrorDescList(13, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[13]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_15:
            ms->interface->updateErrorDescList(14, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[14]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_16:
            ms->interface->updateErrorDescList(15, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[15]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_17:
            ms->interface->updateErrorDescList(16, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[16]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_18:
            ms->interface->updateErrorDescList(17, (const char *)args.dbr);
            ms->interface->message("modErrorDescs[17]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_19:
            ms->interface->updateErrorDescList(18, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[18]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ERROR_DESC_20:
            ms->interface->updateErrorDescList(19, (const char *)args.dbr);
            //ms->interface->message("modErrorDescs[19]= ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_READ:
            ms->rfModObject->hvps_voltage_read = *(double*)args.dbr;
            //ms->interface->message("hvps_voltage_read = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HVPS_CURRENT_READ:
            ms->rfModObject->hvps_current_read = *(double*)args.dbr;
            //ms->interface->message("hvps_voltage_read = ",(const char *)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HEATER_VOLTAGE:
            ms->rfModObject->heater_voltage = *(double*)args.dbr;
            //ms->interface->message("heater_voltage = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HEATER_CURRENT:
            ms->rfModObject->heater_current = *(double*)args.dbr;
            //ms->interface->message("heater_current = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::RESET_VOLTAGE:
            ms->rfModObject->reset_voltage = *(double*)args.dbr;
            //ms->interface->message("reset_voltage =",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::RESET_CURRENT:
            ms->rfModObject->reset_current = *(double*)args.dbr;
            //ms->interface->message("reset_current =",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ION_PUMP_VOLTAGE:
            ms->rfModObject->ion_pump_voltage = *(double*)args.dbr;
            //ms->interface->message("ion_pump_voltage = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::ION_PUMP_CURRENT:
            ms->rfModObject->ion_pump_current = *(double*)args.dbr;
            //ms->interface->message("ion_pump_current = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SUPPLY_TEMP:
            ms->rfModObject->supply_temp = *(double*)args.dbr;
            //ms->interface->message("supply_temp = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SUPPLY_PRESSURE:
            ms->rfModObject->supply_pressure = *(double*)args.dbr;
            //ms->interface->message("supply_pressure = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::RETURN_TEMP:
            ms->rfModObject->return_temp = *(double*)args.dbr;
            //ms->interface->message("return_temp = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::RETURN_PRESSURE:
            ms->rfModObject->return_pressure = *(double*)args.dbr;
            //ms->interface->message("return_pressure = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::BODY_FLOW_RATE:
            ms->rfModObject->body_flow_rate = *(double*)args.dbr;
            //ms->interface->message("body_flow_rate = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_FLOW_RATE:
            ms->rfModObject->solenoid_flow_rate = *(double*)args.dbr;
            //ms->interface->message("solenoid_flow_rate = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::TANK_FLOW_RATE:
            ms->rfModObject->tank_flow_rate = *(double*)args.dbr;
            //ms->interface->message("tank_flow_rate = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::COLLECTOR_RETURN_RATE:
            ms->rfModObject->collector_return_rate = *(double*)args.dbr;
            //ms->interface->message("collector_return_rate = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::BODY_RETURN_TEMPERATURE:
            ms->rfModObject->body_return_temperature = *(double*)args.dbr;
            //ms->interface->message("body_return_temperature = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::COLLECTOR_FLOW_RATE:
            ms->rfModObject->collector_flow_rate = *(double*)args.dbr;
            //ms->interface->message("collector_flow_rate = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_1_VOLTAGE:
            ms->rfModObject->solenoid_1_voltage = *(double*)args.dbr;
            //ms->interface->message("solenoid_1_voltage = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_2_VOLTAGE:
            ms->rfModObject->solenoid_2_voltage = *(double*)args.dbr;
            //ms->interface->message("solenoid_2_voltage = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_3_VOLTAGE:
            ms->rfModObject->solenoid_3_voltage = *(double*)args.dbr;
            //ms->interface->message("solenoid_3_voltage = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_1_CURRENT:
            ms->rfModObject->solenoid_1_current = *(double*)args.dbr;
            //ms->interface->message("solenoid_1_current = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_2_CURRENT:
            ms->rfModObject->solenoid_2_current = *(double*)args.dbr;
            //ms->interface->message("solenoid_2_current = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SOLENOID_3_CURRENT:
            ms->rfModObject->solenoid_3_current = *(double*)args.dbr;
            //ms->interface->message("solenoid_3_current = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_SET_READ:
            ms->rfModObject->hvps_voltage_set_read = *(double*)args.dbr;
            //ms->interface->message("hvps_voltage_set_read = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_LOW_ALARM_SET_READ:
            ms->rfModObject->hvps_voltage_low_alarm_set_read = *(double*)args.dbr;
            //ms->interface->message("hvps_voltage_low_alarm_set_read = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::HVPS_VOLTAGE_HI_ALARM_SET_READ:
            ms->rfModObject->hvps_voltage_hi_alarm_set_read = *(double*)args.dbr;
            //ms->interface->message("hvps_voltage_hi_alarm_set_read = ",*(double*)args.dbr);
            break;
        case rfModStructs::L01_MOD_PV_TYPE::L01_FAULT:
            ms->interface->updateL01_Fault(args);
            //ms->rfModObject->system_state_put = *(int*)args.dbr;
            break;
        case rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ:

            ms->interface->message("static entry passed rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ");
            ms->interface->message("static entry passed rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ");
            ms->interface->message("static entry passed rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ");
            ms->interface->message("static entry passed rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_READ");

            ms->interface->updateL01_StateRead(args);
            break;
        default:
            ms->interface->message("!!ERROR!! staticEntryL01ModMonitor passed unknown PV = ", ENUM_TO_STRING(ms->monType));
    }
}
//______________________________________________________________________________
void l01ModInterface::updateL01_StateRead(const event_handler_args args)
{
//    message("BITS ");
//    int x = *(int*)args.dbr;
//    for (int i = 0; i < 16; i++)
//    {
//        printf("%d", (x & 0x8000) >> 15);
//            x <<= 1;
//    }
//    printf("\n");
//    message("BITS");


    message("updateL01_StateRead passed, ",*(int*)args.dbr);

    switch(*(int*)args.dbr)
    {
        case 0:
            message("SYSTEM_STATE_READ = 0 = STATE_OFF ");
            l01Mod.main_state = rfModStructs::L01_MOD_STATE::L01_OFF;
            break;
        case 1:
            message("SYSTEM_STATE_READ = 1 = STANDBY ");
            l01Mod.main_state = rfModStructs::L01_MOD_STATE::L01_STANDBY;
            break;
        case 2:
            message("SYSTEM_STATE_READ = 2 = HV ?????");
            l01Mod.main_state = rfModStructs::L01_MOD_STATE::L01_HV_ON;
            break;
        case 3:
            l01Mod.main_state = rfModStructs::L01_MOD_STATE::L01_RF_ON;
            message("SYSTEM_STATE_READ = 3 = TRANSMIT ");
            break;
        default:
            message("updateL01_StateRead passed  = ",*(int*)args.dbr);
            l01Mod.main_state = rfModStructs::L01_MOD_STATE::STATE_UNKNOWN;
            break;
    }
}
//______________________________________________________________________________
void l01ModInterface::updateL01_Fault(const event_handler_args args)
{
    switch(*(int*)args.dbr)
    {
        case 0:
            l01Mod.l01_fault = rfModStructs::L01_MOD_FAULT::NO_FAULT;
            message("l01_fault = 0 = NO_FAULT");
            break;
        case 1:
            message("l01_fault = 1 = FAULT ");
            l01Mod.l01_fault = rfModStructs::L01_MOD_FAULT::FAULT;
            //l01Mod.l01_fault = rfModStructs::L01_MOD_STATE::NO_FAULT;
            break;
        case 2:
            message("l01_fault  = 2");
            //l01Mod.l01_fault = rfModStructs::L01_MOD_STATE::NO_FAULT;
            break;
        case 3:
            message("l01_fault  = 2");
            //l01Mod.l01_fault = rfModStructs::L01_MOD_STATE::NO_FAULT;//
            break;
        default:
            message("updateL01_Fault passed  = ",*(int*)args.dbr);
            break;
    }
}
//______________________________________________________________________________
bool l01ModInterface::setOff()
{
    return setModState(rfModStructs::L01_MOD_STATE::L01_OFF);
}
//______________________________________________________________________________
bool l01ModInterface::setModState(const rfModStructs::L01_MOD_STATE v)const
{
    ca_put(
        l01Mod.pvComStructs.at(rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_PUT).CHTYPE,
        l01Mod.pvComStructs.at(rfModStructs::L01_MOD_PV_TYPE::SYSTEM_STATE_PUT).CHID,
        &v);
    bool ret = false;
    int status = sendToEpics("ca_put", "", "Timeout sending Set LO1 Mod values");
    if (status == ECA_NORMAL)
        ret = true;
    return ret;
}








