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

#include "gunProtInterface.h"
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
gunProtInterface::gunProtInterface(const std::string &allGunProtsConf,
                                const bool startVirtualMachine,
                                const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                                const bool shouldStartEPICs ):
configReader(allGunProtsConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs(shouldStartEPICs )
{
    if(shouldStartEPICs )
        message("gunProtInterface shouldStartEPICs is true");
    else
        message("gunProtInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
gunProtInterface::~gunProtInterface()
{
    killILockMonitors();
    for(auto && it : continuousMonitorStructs )
    {
        killMonitor(it );
        delete it;
    }
    debugMessage("gunProtInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void gunProtInterface::killMonitor(rfProtStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription(ms -> EVID );
    if(status == ECA_NORMAL)
        debugMessage(ms->rfProtObject->name, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR gunProtInterface: in killMonitor: ca_clear_subscription failed for ", ms->rfProtObject->name, " ", ENUM_TO_STRING(ms->monType) );
}
//______________________________________________________________________________
void gunProtInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 )); // MAGIC_NUMBER
    if(configFileRead )
    {
        message("The gunProtInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = configReader.getrfGunProtObjects(allGunProts);
        if(getDataSuccess )
        {
            if(shouldStartEPICs )
            {
                message("The gunProtInterface has acquired objects, connecting to EPICS");
                //std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initChids();
                /// start the monitors: set up the callback functions
                startMonitors();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds(2000 )); // MAGIC_NUMBER
            }
            else
             message("The gunProtInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message("!!!The gunProtInterface received an Error while getting laser data!!!" );
    }
}
//______________________________________________________________________________
//bool gunProtInterface::initObjects()
//{
//    bool ans = configReader.getpilaserObject(pilaser);
//    debugMessage("pilaser.pvComStructs.size() = ", pilaser.pvComStructs.size() );
//    debugMessage("pilaser.pvMonStructs.size() = ", pilaser.pvMonStructs.size() );
//    return ans;
//}
////______________________________________________________________________________
void gunProtInterface::initChids()
{
    message("\n", "Searching for allGunProts ChIds...");

    for(auto && allGunProts_it: allGunProts )
    {
        for(auto && pvMonStructs_it : allGunProts_it.second.pvMonStructs )
        {
         addChannel(allGunProts_it.second.pvRoot, pvMonStructs_it.second );
        }

        for(auto && pvComStructs_it : allGunProts_it.second.pvComStructs )
        {
         addChannel(allGunProts_it.second.pvRoot, pvComStructs_it.second );
        }
        addILockChannels(allGunProts_it.second.numIlocks, allGunProts_it.second.pvRoot, allGunProts_it.first, allGunProts_it.second.iLockPVStructs );
    }

    int status = sendToEpics("ca_create_channel", "Found allGunProts ChIds.", "!!TIMEOUT!! Not all allGunProts ChIds found." );
    if(status != ECA_NORMAL )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500 ));//MAGIC_NUMBER
        message("\n", "Checking allGunProts ChIds ");

        for(auto && allGunProts_it: allGunProts )
        {
            for(auto && pvMonStructs_it : allGunProts_it.second.pvMonStructs )
            {
             checkCHIDState(pvMonStructs_it.second.CHID, ENUM_TO_STRING(pvMonStructs_it.first ) );
            }

            for(auto && pvComStructs_it : allGunProts_it.second.pvComStructs )
            {
             checkCHIDState(pvComStructs_it.second.CHID, ENUM_TO_STRING(pvComStructs_it.first )  );
            }
            //addILockChannels(allGunProts_it.second.numIlocks, allGunProts_it.second.pvRoot, allGunProts_it.first, allGunProts_it.second.iLockPVStructs );
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5000 )); // MAGIC_NUMBER
    }
    if(status == ECA_NORMAL)
    {
        allChidsInitialised = true;  /// interface base class member
    }
}
////______________________________________________________________________________
void gunProtInterface::addChannel(const std::string & pvRoot, rfProtStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel(s1.c_str(), 0, 0, 0, &pv.CHID );//MAGIC_NUMBER
    debugMessage("Create channel to ", s1 );
}
////______________________________________________________________________________
void gunProtInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && obj : allGunProts)
    {
        for(auto && it : obj.second.pvMonStructs )
        {
            continuousMonitorStructs.push_back(new rfProtStructs::monitorStruct() );
            continuousMonitorStructs.back() -> monType         = it.first;
            continuousMonitorStructs.back() -> rfProtObject    = &obj.second;
            continuousMonitorStructs.back() -> interface       = this;
            ca_create_subscription(it.second.CHTYPE, it.second.COUNT,  it.second.CHID,
                                   it.second.MASK, gunProtInterface::staticEntryallGunProtsMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back() -> EVID);
        }
    }
    int status = sendToEpics("ca_create_subscription", "Succesfully Subscribed to gun protection Monitors", "!!TIMEOUT!! Subscription to gun modulator protection failed" );
    if (status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
////____________________________________________________________________________________________
void gunProtInterface::staticEntryallGunProtsMonitor(const event_handler_args args)
{   std::cout << "staticEntryallGunProtsMonitor() called" << std::endl;
    rfProtStructs::monitorStruct*ms = static_cast<rfProtStructs::monitorStruct*>(args.usr);
    switch(ms -> monType)
    {
        case rfProtStructs::RF_GUN_PROT_PV_TYPE::STATUS:
            ms->interface->message(ms->rfProtObject->name, " RF_GUN_PROT_PV_TYPE::STATUS = ", *(unsigned short*)args.dbr);
            ms->interface->updateProtStatus(*(ms->rfProtObject), *(unsigned short*)args.dbr);

            break;
        default:
            ms->interface->message("!!! ERROR !!! Unknown Monitor Type passed to gunProtInterface::staticEntryPILMonitor");
            break;
    }
}
//____________________________________________________________________________________________
void gunProtInterface::updateProtStatus(rfProtStructs::rfGunProtObject& obj,const unsigned short value)
{
     switch(value)
     {
        case 0:
            obj.status = rfProtStructs::RF_GUN_PROT_STATUS::GOOD;
            message(obj.name , " status = GOOD ");
            break;
        case 1:
            obj.status = rfProtStructs::RF_GUN_PROT_STATUS::BAD;
            message(obj.name , " status = BAD ");
            break;
        default:
            obj.status = rfProtStructs::RF_GUN_PROT_STATUS::ERROR;
            message(obj.name , " status = ERROR ");
     }
}
//____________________________________________________________________________________________
bool gunProtInterface::reset()
{
    std::vector<std::string> names;
    for(auto && it  : allGunProts)
    {
        names.push_back( it.first );
    }
    return reset(names);
}
//____________________________________________________________________________________________
bool gunProtInterface::reset(const std::vector<std::string>& names )
{
    bool r = false;
    std::vector<chid*> CHIDS;
    std::vector<chtype*> CHTYPES;
    for( auto && name : names )
    {
        if(entryExists(allGunProts, name))
        {
            CHIDS.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::RESET].CHID );
            CHTYPES.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::RESET].CHTYPE );
        }
    }
    if(CHIDS.size()>0)
    {
        std::string m1 = "Failed to send EPICS_ACTIVATE";
        std::string m2 = "Failed to send EPICS_SEND";
        r = sendCommand(CHTYPES,CHIDS,m1,m2);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::reset(const std::string& name)
{
    bool r = false;
    if(entryExists(allGunProts, name ) )
    {
        std::string m1 = "Failed to send EPICS_ACTIVATE";
        std::string m2 = "Failed to send EPICS_SEND";
        r = sendCommand(
            allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::RESET].CHTYPE,
            allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::RESET].CHID,m1,m2);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::enable(const std::string& name)
{
    bool r = false;
    if(entryExists(allGunProts, name ) )
    {
        std::string m1 = "Failed to send EPICS_ACTIVATE";
        std::string m2 = "Failed to send EPICS_SEND";
        r = sendCommand(
            allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::ON].CHTYPE,
            allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::ON].CHID,m1,m2);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::enable(const std::vector<std::string>& names)
{
    bool r = false;
    std::vector<chid*> CHIDS;
    std::vector<chtype*> CHTYPES;
    for( auto && name : names )
    {
        if(entryExists(allGunProts, name))
        {
            CHIDS.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::ON].CHID);
            CHTYPES.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::ON].CHTYPE);
        }
    }
    if(CHIDS.size()>0)
    {
        std::string m1 = "Failed to send EPICS_ACTIVATE";
        std::string m2 = "Failed to send EPICS_SEND";
        r = sendCommand(CHTYPES,CHIDS,m1,m2);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::disable(const std::string& name)
{
    bool r = false;
    if(entryExists(allGunProts, name))
    {
        std::string m1 = "Failed to send EPICS_ACTIVATE";
        std::string m2 = "Failed to send EPICS_SEND";
        r = sendCommand(
            allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::OFF].CHTYPE,
            allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::OFF].CHID,m1,m2);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::disable(const std::vector<std::string>& names)
{
    bool r = false;
    std::vector<chid*> CHIDS;
    std::vector<chtype*> CHTYPES;
    for( auto && name : names )
    {
        if(entryExists(allGunProts, name))
        {
            CHIDS.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::OFF].CHID);
            CHTYPES.push_back( &allGunProts[name].pvComStructs[rfProtStructs::RF_GUN_PROT_PV_TYPE::OFF].CHTYPE);
        }
    }
    if(CHIDS.size()>0)
    {
        std::string m1 = "Failed to send EPICS_ACTIVATE";
        std::string m2 = "Failed to send EPICS_SEND";
        r = sendCommand(CHTYPES,CHIDS,m1,m2);
    }
    return r;
}
//____________________________________________________________________________________________
bool gunProtInterface::sendCommand(const std::vector<chtype*>& CHTYPE, const std::vector<chid*>& CHID, const std::string& m1, const std::string& m2)
{
    bool ret = false;
    for(size_t i = 0; i < CHTYPE.size(); ++i)
        ca_put(*CHTYPE[i], *CHID[i], &EPICS_ACTIVATE);
//    message( "activate" );
    int status = sendToEpics( "ca_put", "", m1.c_str());
    if (status == ECA_NORMAL )
    {
        for(size_t i = 0; i < CHTYPE.size(); ++i)// MAGIC_NUMBER
            ca_put(*CHTYPE[i], *CHID[i], &EPICS_SEND);

        int status = sendToEpics("ca_put", "", m2.c_str());
        if(status == ECA_NORMAL)
            ret = true;
            //std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); // MAGIC_NUMBER
//        else
//            message( " status == ECA_NORMAL" );
    }
    else
        message("EPICS_ACTIVATE did not return ECA_NORMAL");
    return ret;
}
//____________________________________________________________________________________________
bool gunProtInterface::sendCommand(const chtype& CHTYPE, const chid& CHID, const std::string& m1, const std::string& m2)
{
    bool ret = false;
    ca_put(CHTYPE, CHID, &EPICS_ACTIVATE);
//    message( "activate" );
    int status = sendToEpics( "ca_put", "", m1.c_str());
    if (status == ECA_NORMAL )
    {
        ca_put(CHTYPE, CHID, &EPICS_SEND);
        int status = sendToEpics("ca_put", "", m2.c_str());
        if(status == ECA_NORMAL)
            ret = true;
            //std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); // MAGIC_NUMBER
//        else
//            message( " status == ECA_NORMAL" );
    }
    else
        message("EPICS_ACTIVATE did not return ECA_NORMAL");
    return ret;
}
//____________________________________________________________________________________________
bool gunProtInterface::isGood(const std::string & name )
{
    bool ans = false;
    if(entryExists(allGunProts, name ) )
        if(allGunProts[name].status  == rfProtStructs::RF_GUN_PROT_STATUS::GOOD )
            ans = true;
    return ans;
}
//____________________________________________________________________________________________
bool gunProtInterface::isNotGood(const std::string & name )
{
    return !isGood(name);
}
//____________________________________________________________________________________________
bool gunProtInterface::isBad(const std::string& name)
{
    bool ans = false;
    if(entryExists(allGunProts, name ) )
        if(allGunProts[name].status  == rfProtStructs::RF_GUN_PROT_STATUS::BAD)
            ans = true;
    return ans;
}
//____________________________________________________________________________________________
gunProtInterface::IlockMap1 gunProtInterface::getILockStates(const std::string& name)
{
    IlockMap1 r;
    if(entryExists(allGunProts, name))
    {
        return allGunProts[name].iLockStates;
    }
    return r;
}
//____________________________________________________________________________________________
gunProtInterface::IlockMap2 gunProtInterface::getILockStatesStr(const std::string& name)
{
    IlockMap2 r;
    if(entryExists(allGunProts, name))
    {
        for( auto && it : allGunProts[name].iLockStates)
            r[it.first] = ENUM_TO_STRING(it.second);
    }
    return r;
}
