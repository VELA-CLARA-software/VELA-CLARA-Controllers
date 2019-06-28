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
//  Last edit:   19-03-2018
//  FileName:    gunProtInterface.cpp
//  Description:
//
//
//*/
#include "gunProtInterface.h"
// project includes
#include "configDefinitions.h"
// stl includes
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>
#include <stdlib.h>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
gunProtInterface::gunProtInterface(const std::string& config_file,
                                   const bool  startVirtualMachine,
                                   const bool& show_messages_ptr,
                                   const bool& show_debug_messages_ptr,
                                   const bool  shouldStartEPICs):
configReader(config_file, startVirtualMachine,show_messages_ptr, show_debug_messages_ptr),
interface(show_messages_ptr,show_debug_messages_ptr, shouldStartEPICs),
currentMode(rfProtStructs::RF_PROT_TYPE::NOT_KNOWN),
EPICS_ACTIVATE_FAIL("Failed to send EPICS_ACTIVATE"),
EPICS_SEND_FAIL("Failed to send EPICS_SEND")
{
    initialise();
    message("initialise  fin");
}
//______________________________________________________________________________
gunProtInterface::~gunProtInterface()
{
    killILockMonitors();
    for(auto && it : continuousMonitorStructs)
    {
        killMonitor(it);
        delete it;
    }
    debugMessage("gunProtInterface DESTRUCTOR COMPLETE");
}
//______________________________________________________________________________
void gunProtInterface::killMonitor(rfProtStructs::monitorStruct* ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
    {
        debugMessage(ms->rfProtObject->name, " ",
                     ENUM_TO_STRING(ms->monType), " monitoring = false ");
    }
    else
    {
        debugMessage("ERROR gunProtInterface: in killMonitor: "
                     "ca_clear_subscription failed for ",
                     ms->rfProtObject->name, " ", ENUM_TO_STRING(ms->monType));
    }
}
//          ___               __  ___
// | |\ | |  |  |  /\  |    |  / |__
// | | \| |  |  | /~~\ |___ | /_ |___
//
//______________________________________________________________________________
void gunProtInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    UTL::STANDARD_PAUSE;
    if(configFileRead)
    {
        message("The gunProtInterface has read the config file, "
                "acquiring objects");
        /* initialise the objects based on what is read from the config file */
        bool getDataSuccess = configReader.getrfProtObjects(allGunProts);
        if(getDataSuccess)
        {
            message("Found ", allGunProts.size(), " RF objects");
            if(shouldStartEPICs)
            {
                message("The gunProtInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                message("The gunProtInterface is trying to start monitors");
                startMonitors();
                /* The pause allows EPICS callbacks to catch up.*/
                pause_2000();
            }
            else
             message("The gunProtInterface has acquired objects, "
                     "NOT connecting to EPICS");
        }
        else
            message("!!!The gunProtInterface received an Error while "
                    "getting RFprotection config data !!!");
    }
}
//______________________________________________________________________________
void gunProtInterface::initChids()
{
    message("\n", "Searching for allGunProts ChIds...");
    for(auto&& allGunProts_it: allGunProts)
    {
        /*  iterate over command and monitor stucts
            adding channels
        */
        for(auto&& pvMonStructs_it:allGunProts_it.second.pvMonStructs)
        {
         addChannel(allGunProts_it.second.pvRoot, pvMonStructs_it.second);
        }
        for(auto&& pvComStructs_it : allGunProts_it.second.pvComStructs)
        {
         addChannel(allGunProts_it.second.pvRoot, pvComStructs_it.second);
        }
    }
    /* connect channels */
    int status = sendToEpics("ca_create_channel",
                             "Found allGunProts ChIds.",
                             "!!TIMEOUT!! Not all allGunProts ChIds found.");
    /* if connection fails run a diagnositc: checkCHIDState */
    if(status != ECA_NORMAL)
    {
        UTL::PAUSE_500;
        message("\n", "Checking allGunProts ChIds ");
        for(auto&& allGunProts_it: allGunProts)
        {
            for(auto&& pvMonStructs_it:allGunProts_it.second.pvMonStructs)
            {
                checkCHIDState(pvMonStructs_it.second.CHID,
                               ENUM_TO_STRING(pvMonStructs_it.first));
            }

            for(auto&& pvComStructs_it:allGunProts_it.second.pvComStructs)
            {
                checkCHIDState(pvComStructs_it.second.CHID,
                               ENUM_TO_STRING(pvComStructs_it.first) );
            }
        }
        UTL::PAUSE_500;
    }
    else if(status == ECA_NORMAL)
    {
        allChidsInitialised = true;
    }
}
////______________________________________________________________________________
void gunProtInterface::addChannel(const std::string& pvRoot,
                                  rfProtStructs::pvStruct& pv)
{
    std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    message("Create channel to ", s);
}
////______________________________________________________________________________
void gunProtInterface::startMonitors()
{
    /*  monitors get passed a rfProtStructs::monitorStruct pointer
        contained in continuousMonitorStructs
        This holds all the data required to update values on EPICS callback
    */
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();
    for(auto && obj : allGunProts)
    {
        for(auto && it : obj.second.pvMonStructs)
        {
            continuousMonitorStructs.push_back(new rfProtStructs::monitorStruct());
            continuousMonitorStructs.back() -> rfProtObject = &obj.second;
            continuousMonitorStructs.back() -> interface    = this;
            continuousMonitorStructs.back() -> monType      = it.first;
            continuousMonitorStructs.back() -> CHTYPE       = it.second.CHTYPE;
            ca_create_subscription(it.second.CHTYPE,
                                   it.second.COUNT,
                                   it.second.CHID,
                                   it.second.MASK,
                                   gunProtInterface::staticEntryMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back()->EVID);
        }
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to gun protection Monitors",
                             "!!TIMEOUT!! Subscription to gun protection failed");
    if (status == ECA_NORMAL)
    {
        allMonitorsStarted = true;
    }
}
//____________________________________________________________________________________________
void gunProtInterface::staticEntryMonitor(const event_handler_args args)
{
    rfProtStructs::monitorStruct*ms = static_cast<rfProtStructs::monitorStruct*>(args.usr);
//    ms->interface->debugMessage("staticEntryMonitor ",
//                                ENUM_TO_STRING(ms -> monType)," ",
//                                ms -> rfProtObject -> name
//                                );

    switch(ms -> monType)
    {
        case rfProtStructs::RF_PROT_PV_TYPE::STATUS:
            ms->interface->updateProtStatus(*(ms->rfProtObject), args);
            break;
        case rfProtStructs::RF_PROT_PV_TYPE::CMI:
            //ms->rfProtObject -> cmi = *(unsigned long*)args.dbr;
            ms->rfProtObject -> cmi = getDBRlong(args);
            ms->interface->updateCMIBits(*(ms->rfProtObject));
            break;
        default:
            ms->interface->message("!!! ERROR !!! Unknown Monitor Type passed "
                                   "to gunProtInterface::staticEntryPILMonitor");
            break;
    }
//    ms->interface->debugMessage("staticEntryMonitor fin");
}
//____________________________________________________________________________________________
void gunProtInterface::updateCMIBits(rfProtStructs::rfProtObject& obj)
{
    //message(obj.name, " new cmi value =   ", obj.cmi);
    /*
        the bits in the cmi number code the state of the protection
    */
    size_t counter = UTL::ZERO_SIZET;
    for(auto bit = 0; bit < 8; ++bit)//MAGIC_NUMBER
    {
        //message(obj.name, " bit ", bit, " value = ",  (obj.cmi &( 1 << bit)) >> bit);
        if( std::find(obj.gunProtKeyBits.begin(),
                      obj.gunProtKeyBits.end(), bit) != obj.gunProtKeyBits.end())
        {
            obj.gunProtKeyBitValues[counter] = (obj.cmi &( 1 << bit)) >> bit;
            //message("obj.gunProtKeyBitValues part ",
            //        counter, " = ",
            //        obj.gunProtKeyBitValues[counter]);
            ++counter;
        }
    }

    if(allkeybitsaregood(obj))
    {
        message("Allkeybits for ", ENUM_TO_STRING(obj.protType), " ARE GOOD");
        switch( obj.protType)
        {
            case rfProtStructs::RF_PROT_TYPE::CLARA_HRRG:
                currentMode = obj.protType;
                break;
            case rfProtStructs::RF_PROT_TYPE::VELA_LRRG:
                currentMode = obj.protType;
                break;
            case rfProtStructs::RF_PROT_TYPE::VELA_HRRG:
                currentMode = obj.protType;
                break;
            case rfProtStructs::RF_PROT_TYPE::CLARA_LRRG:
                currentMode = obj.protType;
                break;
            case rfProtStructs::RF_PROT_TYPE::TEST:
                currentMode = obj.protType;
                break;
            default:
                currentMode = rfProtStructs::RF_PROT_TYPE::NO_MODE;
        }
        message("RF GUN Mode is ", ENUM_TO_STRING(currentMode));
    }
}
//____________________________________________________________________________________________
bool gunProtInterface::allkeybitsaregood(const std::string & name) const
{
    bool r = false;
    if(entryExists(allGunProts, name))
    {
        r = allkeybitsaregood(allGunProts.at(name));
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::allkeybitsaregood(const rfProtStructs::rfProtObject& obj) const
{
    bool r = false;
    if(isNotGeneralProt(obj.name) && isNotEnableProt(obj.name))
    {
        r = true;
        for(auto&& it : obj.gunProtKeyBitValues)
        {
            if(!it)
                r = false;
        }
    }
    return r;
}
//____________________________________________________________________________________________
void gunProtInterface::updateProtStatus(rfProtStructs::rfProtObject& obj,
                                        const event_handler_args args)
{   //message("updateProtStatus(,const event_handler_args args) called");
    switch(args.type)
    {
        case DBR_TIME_ENUM:
            {
                //message("DBR_TIME_ENUM");
                const dbr_time_enum* pTD = ( const struct dbr_time_enum *) args.dbr;
                updateProtStatus(obj, (unsigned short)pTD ->value);
            }
            break;
        case DBR_ENUM:
            {
                //message("DBR_ENUM");
                updateProtStatus(obj,  getDBRunsignedShort(args));
            }
            break;
    }
    //message("updateProtStatus(,const event_handler_args args) FIN");
}
//____________________________________________________________________________________________
void gunProtInterface::updateProtStatus(rfProtStructs::rfProtObject& obj,
                                        const unsigned short value)
{   //message(obj.name , " value =  ", value, " updateProtStatus(obj,const long value)");
    switch(value)
    {
       case UTL::ZERO_US:
           obj.status = rfProtStructs::RF_PROT_STATUS::BAD;
           //message(obj.name , " status = BAD ");
           break;
       case UTL::ONE_US:
           obj.status = rfProtStructs::RF_PROT_STATUS::GOOD;
           //message(obj.name , " status = GOOD ");
           break;
        default:
           obj.status = rfProtStructs::RF_PROT_STATUS::ERROR;
           //message(obj.name , " status = ERROR ");
    }
}
//____________________________________________________________________________________________
std::string gunProtInterface::getGeneralProtName() const
{
    for( auto&& it : allGunProts)
    {
        if( isProtOfType( it.first, rfProtStructs::RF_PROT_TYPE::GENERAL))
            return it.first;
    }
    std::string r = UTL::UNKNOWN_STRING;
    return r;
}
//____________________________________________________________________________________________
std::string gunProtInterface::getEnableProtName() const
{
    for(auto&& it : allGunProts)
    {
        if( isProtOfType( it.first, rfProtStructs::RF_PROT_TYPE::ENABLE))
            return it.first;
    }
    std::string r = UTL::UNKNOWN_STRING;
    return r;
}
//____________________________________________________________________________________________
std::string gunProtInterface::getCurrentModeProtName() const
{
    message("getCurrentModeProtName ", ENUM_TO_STRING(currentMode));
    for(auto&& it : allGunProts)
    {
        if( isProtOfType( it.first, currentMode))
            return it.first;
    }
    std::string r = UTL::UNKNOWN_STRING;
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::isProtOfType(const std::string& name,
                                    const rfProtStructs::RF_PROT_TYPE type) const
{
    bool r = false;
    if(entryExists(allGunProts,name))
    {
        r = allGunProts.at(name).protType == type;
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::isGeneralProt(const std::string& name) const
{
    return isProtOfType(name, rfProtStructs::RF_PROT_TYPE::GENERAL);
}
//____________________________________________________________________________________________
bool gunProtInterface::isNotGeneralProt(const std::string& name) const
{
    return !isGeneralProt(name);
}
//____________________________________________________________________________________________
bool gunProtInterface::isEnableProt(const std::string& name) const
{
    return isProtOfType(name, rfProtStructs::RF_PROT_TYPE::ENABLE);
}
//____________________________________________________________________________________________
bool gunProtInterface::isNotEnableProt(const std::string& name) const
{
    return !isEnableProt(name);
}
//____________________________________________________________________________________________
bool gunProtInterface::enable()const
{
    /*  enable everything, by going through each protection in sequence
        these are the generic names (inorder) */
    std::string genname = getGeneralProtName();
    std::string ennname = getEnableProtName();
    std::string modname = getCurrentModeProtName();

    bool carryon = true;

    if(genname == UTL::UNKNOWN_STRING)
    {
        message("!!ERROR!! in gunProtInterface::enable() genname = ",
                     genname," can't complete enbale");
        carryon = false;
    }
    if(ennname == UTL::UNKNOWN_STRING)
    {
        message("!!ERROR!! in gunProtInterface::enable() ennname = ",
                ennname," can't complete enbale");
        carryon = false;
    }
    if(modname == UTL::UNKNOWN_STRING)
    {
        message("!!ERROR!! in gunProtInterface::enable() modname = ",
                modname," can't complete enbale");
        carryon = false;
    }
    /*  if names appear to be good, cycle through each, resetting and enabling,
        with pauses after each commnd */
    if(carryon)
    {
        debugMessage("Resetting RF_GUN General Protection");
        reset(genname);
        pause_300();
        debugMessage("Enabling General Protection");
        carryon = enable(genname);
        if( carryon )
        {
            pause_300();
            if( isGood(genname))
            {
                debugMessage("Enabling RFGUN General Protection Success");
                debugMessage("Resetting Curent Mode Protection");
                reset(modname);
                pause_300();
                debugMessage("Enabling ", ENUM_TO_STRING(currentMode)," Protection");
                carryon = enable(modname);
                pause_300();
                if( isGood(modname))
                {
                    debugMessage("Enabling ",
                                 ENUM_TO_STRING(currentMode),
                                 " RFGUN Protection Success");
                    debugMessage("Resetting RFGUN Enable Protection");
                    reset(ennname);
                    pause_300();
                    debugMessage("Enabling Enable Protection");
                    carryon = enable(ennname);
                    pause_300();
                    if(isGood(ennname))
                    {
                        carryon = true;
                        debugMessage("Enabling RFGUN Enable Protection Success");
                    }
                    else
                    {
                        carryon = false;
                        debugMessage("Enabling RFGUN Enable Protection Failure");
                    }

                }
                else
                {
                    carryon = false;
                    debugMessage("Enabling ",
                                  ENUM_TO_STRING(currentMode),
                                  "RFGUN Protection Failure");;
                }
            }
            else
            {
                debugMessage("Enabling General Protection Failure");
                carryon = false;
            }
        }
    }
    return carryon;
}
//____________________________________________________________________________________________
bool gunProtInterface::reset() const
{
    std::vector<std::string> names;
    for(auto&& it : allGunProts)
    {
        names.push_back( it.first);
    }
    return reset(names);
}
//____________________________________________________________________________________________
bool gunProtInterface::reset(const std::string& name) const
{
    const std::vector<std::string> v{name};
    return reset(v);
}
//____________________________________________________________________________________________
bool gunProtInterface::reset(const std::vector<std::string>& names) const
{
    return sendCommand(names,rfProtStructs::RF_PROT_PV_TYPE::RESET);
}
//____________________________________________________________________________________________
bool gunProtInterface::enable(const std::string& name) const
{
    const std::vector<std::string> v{name};
    return enable(v);
}
//____________________________________________________________________________________________
bool gunProtInterface::enable(const std::vector<std::string>& names) const
{
    return sendCommand(names,rfProtStructs::RF_PROT_PV_TYPE::ON);
}
//____________________________________________________________________________________________
bool gunProtInterface::disable(const std::string& name)const
{
    const std::vector<std::string> v{name};
    return disable(v);
}
//____________________________________________________________________________________________
bool gunProtInterface::disable(const std::vector<std::string>& names)const
{
    return sendCommand(names,rfProtStructs::RF_PROT_PV_TYPE::OFF);
}
//____________________________________________________________________________________________
bool gunProtInterface::exists_in_allGunProts(const std::string& name,
                                             rfProtStructs::RF_PROT_PV_TYPE pv) const
{
    if(entryExists(allGunProts, name))
        if(entryExists(allGunProts.at(name).pvComStructs, pv))
           return true;
    return false;
}
//____________________________________________________________________________________________
bool gunProtInterface::sendCommand(const std::vector<std::string>& names,
                                   rfProtStructs::RF_PROT_PV_TYPE pv
                                  ) const
{
    bool r = false;
    std::vector<const chid*> CHIDS;
    std::vector<const chtype*> CHTYPES;
    for( auto && name : names)
    {
        if(exists_in_allGunProts(name,pv))
        {
            CHIDS.push_back( &allGunProts.at(name).pvComStructs.at(pv).CHID);
            CHTYPES.push_back( &allGunProts.at(name).pvComStructs.at(pv).CHTYPE);
        }
    }
    if(CHIDS.size() > UTL::ZERO_SIZET)
    {
        r = sendCommand(CHTYPES, CHIDS, EPICS_ACTIVATE_FAIL, EPICS_SEND_FAIL);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::sendCommand(const std::vector<const chtype*>& CHTYPE,
                                   const std::vector<const chid*>& CHID,
                                   const std::string& m1, const std::string& m2) const
{
    bool ret = false;
    for(size_t i = UTL::ZERO_SIZET; i < CHTYPE.size(); ++i)
        ca_put(*CHTYPE[i], *CHID[i], &EPICS_ACTIVATE);

//    message( "activate");

    int status = sendToEpics( "ca_put", "", m1.c_str());
    if (status == ECA_NORMAL)
    {
        for(size_t i = UTL::ZERO_SIZET; i < CHTYPE.size(); ++i)
            ca_put(*CHTYPE[i], *CHID[i], &EPICS_SEND);

        int status = sendToEpics("ca_put", "", m2.c_str());
        if(status == ECA_NORMAL)
            ret = true;
    }
    else
        message("EPICS_ACTIVATE did not return ECA_NORMAL");
    return ret;
}
//____________________________________________________________________________________________
bool gunProtInterface::isGood(const std::string & name) const
{
    bool ans = false;
    if(entryExists(allGunProts, name))
        if(allGunProts.at(name).status == rfProtStructs::RF_PROT_STATUS::GOOD)
            ans = true;
    return ans;
}
//____________________________________________________________________________________________
bool gunProtInterface::isNotGood(const std::string & name) const
{
    return !isGood(name);
}
//____________________________________________________________________________________________
bool gunProtInterface::isBad(const std::string& name) const
{
    bool ans = false;
    if(entryExists(allGunProts, name))
        if(allGunProts.at(name).status == rfProtStructs::RF_PROT_STATUS::BAD)
            ans = true;
    return ans;
}
//____________________________________________________________________________________________
const rfProtStructs::rfProtObject&
    gunProtInterface::getRFProtObjConstRef(const std::string& name)  const
{
    if(entryExists(allGunProts, name))
    {
        return allGunProts.at(name);
    }
    rfProtStructs::rfProtObject r = rfProtStructs::rfProtObject();
    return r;
}
//____________________________________________________________________________________________
interface::map_ilck_state gunProtInterface::getILockStates(const std::string& name) const
{
    interface::map_ilck_state r;
    if(entryExists(allGunProts, name))
    {
        return allGunProts.at(name).iLockStates;
    }
    return r;
}
//____________________________________________________________________________________________
interface::map_ilck_string gunProtInterface::getILockStatesStr(const std::string& name) const
{
    interface::map_ilck_string r;
    if(entryExists(allGunProts, name))
    {
        for( auto && it : allGunProts.at(name).iLockStates)
            r.at(it.first) = ENUM_TO_STRING(it.second);
    }
    return r;
}
//____________________________________________________________________________________________
std::vector<std::string> gunProtInterface::getProtNames() const
{
    std::vector<std::string> r;
    for(auto&& it: allGunProts)
    {
        r.push_back(it.second.name);
    }
    return r;
}
//____________________________________________________________________________________________

//bool gunProtInterface::reset(const std::vector<std::string>& names)
//{
//    bool r = false;
//    std::vector<chid*> CHIDS;
//    std::vector<chtype*> CHTYPES;
//    for( auto && name : names)
//    {
//        if(entryExists(allGunProts, name))
//        {
//            CHIDS.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::RESET].CHID);
//            CHTYPES.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::RESET].CHTYPE);
//        }
//    }
//    if(CHIDS.size()>0)
//    {
//        r = sendCommand(CHTYPES, CHIDS, EPICS_ACTIVATE_FAIL, EPICS_SEND_FAIL);
//    }
//    return r;
//}
//____________________________________________________________________________________________
////____________________________________________________________________________________________
//bool gunProtInterface::reset(const std::string& name) const
//{
//    bool r = false;
//    rfProtStructs::RF_PROT_PV_TYPE reset = rfProtStructs::RF_PROT_PV_TYPE::RESET;
//    if(exists_in_allGunProts(name,reset))
//    {
//        r = sendCommand(
//            allGunProts.at(name).pvComStructs.at(reset).CHTYPE,
//            allGunProts.at(name).pvComStructs.at(reset).CHID,
//            EPICS_ACTIVATE_FAIL, EPICS_SEND_FAIL);
//    }
//    return r;
//}
////____________________________________________________________________________________________
//bool gunProtInterface::enable(const std::string& name)
//{
//    bool r = false;
//    if(entryExists(allGunProts, name))
//    {
//        std::string m1 = "Failed to send EPICS_ACTIVATE";
//        std::string m2 = "Failed to send EPICS_SEND";
//        r = sendCommand(
//            allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::ON].CHTYPE,
//            allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::ON].CHID,
//            EPICS_ACTIVATE_FAIL, EPICS_SEND_FAIL);
//    }
//    return r;
//}
////____________________________________________________________________________________________
//bool gunProtInterface::enable(const std::vector<std::string>& names)
//{
//    bool r = false;
//    std::vector<chid*> CHIDS;
//    std::vector<chtype*> CHTYPES;
//    for( auto && name : names)
//    {
//        if(entryExists(allGunProts, name))
//        {
//            CHIDS.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::ON].CHID);
//            CHTYPES.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::ON].CHTYPE);
//        }
//    }
//    if(CHIDS.size()>0)
//    {
//        std::string m1 = "Failed to send EPICS_ACTIVATE";
//        std::string m2 = "Failed to send EPICS_SEND";
//        r = sendCommand(CHTYPES, CHIDS, EPICS_ACTIVATE_FAIL, EPICS_SEND_FAIL);
//    }
//    return r;
//}
////____________________________________________________________________________________________
//bool gunProtInterface::disable(const std::string& name)
//{
//    bool r = false;
//    if(entryExists(allGunProts, name))
//    {
//        std::string m1 = "Failed to send EPICS_ACTIVATE";
//        std::string m2 = "Failed to send EPICS_SEND";
//
//                    if(entryExists(allGunProts.at(name).pvComStructs, off))
//            {
//                CHIDS.push_back(&allGunProts.at(name).pvComStructs.at(off).CHID);
//                CHTYPES.push_back(&allGunProts.at(name).pvComStructsat(off).CHTYPE);
//            }
//
//        r = sendCommand(
//            allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::OFF].CHTYPE,
//            allGunProts[name].pvComStructs[rfProtStructs::RF_PROT_PV_TYPE::OFF].CHID,
//            EPICS_ACTIVATE_FAIL, EPICS_SEND_FAIL);
//    }
//    return r;
//}
////____________________________________________________________________________________________
//bool gunProtInterface::disable(const std::vector<std::string>& names)
//{
//    bool r = false;
//    std::vector<chid*> CHIDS;
//    std::vector<chtype*> CHTYPES;
//    rfProtStructs::RF_PROT_PV_TYPE off = rfProtStructs::RF_PROT_PV_TYPE::OFF;
//    for( auto && name : names)
//    {
//        if(entryExists(allGunProts, name))
//        {
//            if(entryExists(allGunProts.at(name).pvComStructs, off))
//            {
//                CHIDS.push_back(&allGunProts.at(name).pvComStructs.at(off).CHID);
//                CHTYPES.push_back(&allGunProts.at(name).pvComStructsat(off).CHTYPE);
//            }
//        }
//    }
//    if(CHIDS.size() > UTL::ZERO_SIZET)
//    {
//        std::string m1 = "Failed to send EPICS_ACTIVATE";
//        std::string m2 = "Failed to send EPICS_SEND";
//        r = sendCommand(CHTYPES,CHIDS,m1,m2);
//    }
//    return r;
//}
////____________________________________________________________________________________________
//
////____________________________________________________________________________________________
//bool gunProtInterface::sendCommand(const chtype& CHTYPE,  const chid& CHID,
//                                   const std::string& m1, const std::string& m2) const
//{
//    bool ret = false;
//    ca_put(CHTYPE, CHID, &EPICS_ACTIVATE);
////    message( "activate");
//    int status = sendToEpics( "ca_put", "", m1.c_str());
//    if (status == ECA_NORMAL)
//    {
//        ca_put(CHTYPE, CHID, &EPICS_SEND);
//        int status = sendToEpics("ca_put", "", m2.c_str());
//        if(status == ECA_NORMAL)
//            ret = true;
//            //std::this_thread::sleep_for(std::chrono::milliseconds( 50)); // MAGIC_NUMBER
////        else
////            message( " status == ECA_NORMAL");
//    }
//    else
//        message("EPICS_ACTIVATE did not return ECA_NORMAL");
//    return ret;
//}
