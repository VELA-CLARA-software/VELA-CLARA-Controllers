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
//  Last edit:   29-03-2018
//  FileName:    shutterInterface.cpp
//  Description:
//
//
//*/
// project includes
#include "shutterInterface.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
using namespace shutterStructs;
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
shutterInterface::shutterInterface(const bool& show_messages,
                                   const bool& show_debug_messages,
                                   const bool startVirtualMachine,
                                   const bool shouldStartEPICs,
                                   const std::string& configFile
                                 ):
configReader(configFile,
             show_messages,
             show_debug_messages,
             startVirtualMachine),
//interface(show_messages, show_debug_messages)
interface(show_messages,
          show_debug_messages,
          shouldStartEPICs,
          startVirtualMachine)
{
    message("Constructing a shutterInterface");
    initialise();
}
//______________________________________________________________________________
shutterInterface::~shutterInterface()
{
    for(auto&& it : continuousMonitorStructs)
    {
        debugMessage("delete shutterInterface continuousMonitorStructs entry.");
        delete it;
    }
}
//______________________________________________________________________________
void shutterInterface::initialise()
{
    /*
        The config file reader
    */
    configFileRead = configReader.readConfig();
    if(configFileRead)
    {
        message("The shutterInterface has read the config file, "
                "acquiring objects");
        /* Initialise the objects based on what is read from the config file */
        bool getDataSuccess = initObjects();
        if(getDataSuccess )
        {
            if(shouldStartEPICs)
            {
                message("The shutterInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                startMonitors();
                /* The pause allows EPICS callbacks to catch up. */
                pause_2000();
            }
            else
             message("The shutterInterface has acquired objects, "
                     "NOT connecting to EPICS");
        }
        else
            message("!!!The shutterInterface received an Error "
                    "while getting laser data!!!" );
    }
}
//______________________________________________________________________________
bool shutterInterface::initObjects()
{
    const std::vector<shutterObject>shutterObjs=configReader.getShutterObjects();
    for(auto const& it:shutterObjs)
    {
        allShutterData[it.name] = it;
    }
    return true;
}
//______________________________________________________________________________
void shutterInterface::initChids()
{
    message("\n", "Searching for Shutter chids...");
    for(auto&& it1:allShutterData)
    {
        addILockChannels(it1.second.numIlocks,
                         it1.second.pvRoot,
                         it1.second.name,
                         it1.second.iLockPVStructs);

        for(auto&& it2:it1.second.pvComStructs)
        {
            addChannel(it1.second.pvRoot, it2.second);
        }
        for(auto&& it2:it1.second.pvMonStructs)
        {
            addChannel(it1.second.pvRoot, it2.second);
        }
    }
    int status = sendToEpics("ca_create_channel",
                             "Found Shutter chids.",
                             "!!TIMEOUT!! Not all shutter ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        UTL::STANDARD_PAUSE;
        for(const auto & it : allShutterData)
        {
            message("\n", "Checking Chids for ", it.first);
            for(auto & it2 : it.second.pvMonStructs)
                checkCHIDState(it2.second.CHID, it2.second.pvSuffix);
            for(auto & it2 : it.second.pvComStructs)
                checkCHIDState(it2.second.CHID, it2.second.pvSuffix);
            for(auto & it2 : it.second.iLockPVStructs)
                checkCHIDState(it2.second.CHID, it2.second.pv);
        }
        message("");
    }
    else if (status == ECA_NORMAL)
    {
        allChidsInitialised = true;
    }
}
//______________________________________________________________________________
void shutterInterface::addChannel(const std::string& pvRoot, pvStruct& pv)
{
    const std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    debugMessage("Create channel to ", s);
}
//______________________________________________________________________________
void shutterInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();
    for(auto && it : allShutterData)
    {
        monitorIlocks(it.second.iLockPVStructs, it.second.iLockStates);
        for(auto && it2 : it.second.pvMonStructs )
        {
            /*
                init structs 'correctly'
http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
            */
            continuousMonitorStructs.push_back(new shutterStructs::monitorStuct());
            continuousMonitorStructs.back()->interface = this;
            continuousMonitorStructs.back()->shutObj   = &(it.second);
            continuousMonitorStructs.back()->CHTYPE    = it2.second.CHTYPE;
            /*
                For the shutter there is only 1 monitor type
            */
            continuousMonitorStructs.back()->monType = it2.first;
            /*
                If you pass DBF_STRING and recast as a char * in the callback
                you can get the state as GOOD, BAD, OPEN, CLOSED etc,
                This is useful for debugging, but in general i'm just going
                to subscribe to the DBR_ENUM
            */
            /*
                ca_create_subscription accepts a void * user argument,
                we pass a pointer to the monitor struct,
                in the callback function this is cast back and the data
                can then be updated:
            void* usrArg = reinterpret_cast<void*>(continuousMonitorStructs.back());
            */
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   shutterInterface::staticEntryShutterMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back()->EVID);
        }
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to Shutter Monitors",
                             "!!TIMEOUT!! Subscription to All Shutter monitors failed");
    if (status == ECA_NORMAL)
    {
        allMonitorsStarted = true;
    }
}
//______________________________________________________________________________
void shutterInterface::staticEntryShutterMonitor(const event_handler_args args)
{
    /*
        recast args.usr (a void *) to a monitor struct pointer
    */
    monitorStuct* ms = reinterpret_cast<monitorStuct*>(args.usr);
    /*
        Not sure how to decode these apart from trial and error
        you can test with DBF_STRING as the callback type
    */
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            ms->shutObj->state = SHUTTER_STATE::CLOSED;
            break;
        case UTL::ONE_US:
            ms->shutObj->state = SHUTTER_STATE::OPEN;
            break;
        case UTL::TWO_US:
            ms->shutObj->state = SHUTTER_STATE::TIMING;
            break;
        default:
            ms->shutObj->state = SHUTTER_STATE::ERROR;
    }
    /*
        make debug messages easier to understand
        by using ENUM_TO_STRING MACRO
    */
    ms->interface->debugMessage(ms->shutObj->name, " new state = ",
                                ENUM_TO_STRING(ms->shutObj -> state));
    /*
        //If subscribed to DBF_STRING use this to get the message
        char * val = (char *)args.dbr;
        // now we can switch based on the monitor type and then
        // update the correct part of the shutter object data using val
        // For the shutter this is basically redundant,
        // there is only one monitor the "Sta"
        // (apart from interlocks, these are handled in the base class)
        switch(lms.monType)
        {
            case shutterStructs::Sta:
                ....
            case SomeOtherPVType:
                ....
            }
    */
}
//
//     __   __                         __   __
//    /  ` /  \  |\/|  |\/|  /\  |\ | |  \ /__`
//    \__, \__/  |  |  |  | /~~\ | \| |__/ .__/
//
//______________________________________________________________________________
void shutterInterface::open(const std::string& name)
{
    if(isClosed(name))
    {
        pvStruct& pvs =
            allShutterData.at(name).pvComStructs.at(SHUTTER_PV_TYPE::On);
        toggleShutter(pvs.CHTYPE,pvs.CHID,
                      "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO SHUTTER OPEN",
                      "!!TIMEOUT!! FAILED TO SEND SHUTTER OPEN");
    }
}
//______________________________________________________________________________
void shutterInterface::close(const std::string & name)
{
    if(isOpen(name))
    {
        pvStruct& pvs =
            allShutterData.at(name).pvComStructs.at(SHUTTER_PV_TYPE::Off);
        toggleShutter(pvs.CHTYPE,pvs.CHID,
                      "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO SHUTTER OPEN",
                      "!!TIMEOUT!! FAILED TO SEND SHUTTER OPEN");
    }
}
//______________________________________________________________________________
void shutterInterface::toggleShutter(chtype& cht, chid& chi,
                                     const char* m1,
                                     const char* m2)
{
    int status = caput(cht, chi, EPICS_ACTIVATE, "", m1);
    if(status == ECA_NORMAL)
    {
        caput(cht, chi, EPICS_SEND, "", m2);
    }
}
//______________________________________________________________________________
bool shutterInterface::isOpen(const std::string & name) const
{
    return is_in_state(name, SHUTTER_STATE::OPEN);
}
//______________________________________________________________________________
bool shutterInterface::isClosed(const std::string & name) const
{
    return is_in_state(name, SHUTTER_STATE::CLOSED);
}
//______________________________________________________________________________
bool shutterInterface::is_in_state(const std::string & name, SHUTTER_STATE sta) const
{
        bool ret = false;
    if(entryExists(allShutterData, name))
    {
        if(allShutterData.at(name).state == sta)
        {
            ret = true;
        }
    }
    return ret;
}
//______________________________________________________________________________
bool shutterInterface::areAllOpen()
{
    for(auto&&it:allShutterData)
    {
        if(it.second.state != SHUTTER_STATE::OPEN )
            return false;
    }
    return true;
}
//______________________________________________________________________________
bool shutterInterface::areAllClosed()
{
    for(auto&&it:allShutterData)
    {
        if(it.second.state != SHUTTER_STATE::CLOSED )
            return false;
    }
    return true;
}
//______________________________________________________________________________
bool shutterInterface::openAndWait(const std::string& name, const time_t waitTime)
{
    return toggleShutterAndWait(&shutterInterface::isClosed,
                                &shutterInterface::isOpen,
                                &shutterInterface::open,
                                name, waitTime, *this);
    return false;
}
//______________________________________________________________________________
bool shutterInterface::closeAndWait(const std::string& name, const time_t waitTime)
{
    return toggleShutterAndWait(&shutterInterface::isOpen,
                                &shutterInterface::isClosed,
                                &shutterInterface::close,
                                name, waitTime, *this);
    return false;
}
//______________________________________________________________________________
bool shutterInterface::toggleShutterAndWait(isOCMemFn f1, isOCMemFn f2, OCMemFn f3,
                                            const std::string& name,
                                            const time_t waitTime,
                                            shutterInterface & obj)
{
    /*
        CALL_MEMBER_FN is a MACRO in structs.h
    */
    if(CALL_MEMBER_FN(obj, f1)(name))
    {
        CALL_MEMBER_FN(obj, f3)(name) ;
        /*
            Start Timers
        */
        time_t timeStart = timeNow();
        time_t currentTime = timeNow();
        time_t timeDif = currentTime - timeStart;
        while(CALL_MEMBER_FN(obj, f1)(name))
        {
            pause_1();
            currentTime = timeNow();
            timeDif = currentTime - timeStart;
            if(timeDif > waitTime)
            {
                message("Toggle open/close ", name,
                        " timed out after ", waitTime, " seconds");
                break;
            }
        }
    }
    /*
        Prevously we have still needed a sleep here, (!!!???)
        This SHOULDN'T BE NEEDED,
        THIS NEEDS CHECKING
        pause_2();
    */
    return CALL_MEMBER_FN(obj, f2)(name);
}
////      __   ___ ___ ___  ___  __   __
////     / _` |__   |   |  |__  |__) /__`
////     \__> |___  |   |  |___ |  \ .__/
////
//______________________________________________________________________________
std::vector<std::string> shutterInterface::getShutterNames() const

{
    std::vector<std::string> shutterNames;
    for(auto const& it:allShutterData)
        shutterNames.push_back(it.first);
    return shutterNames;
}
//______________________________________________________________________________
shutterObject* shutterInterface::getShutterObject(const std::string& name)
{
    if(entryExists(allShutterData, name))
        return &allShutterData.at(name);
    else
        return nullptr;
}
//______________________________________________________________________________
const shutterObject& shutterInterface::getShutterObjConstRef(const std::string&name)const
{
    if(entryExists(allShutterData, name))
        return allShutterData.at(name);
    else
        return dummObject;
}
//______________________________________________________________________________
SHUTTER_STATE shutterInterface::getShutterState(const std::string &name)
{
    if(entryExists(allShutterData, name))
        return allShutterData.at(name).state;
    else
        return SHUTTER_STATE::ERROR;
}
//______________________________________________________________________________
using namespace HWC_ENUM;
std::map<ILOCK_NUMBER,ILOCK_STATE>
    shutterInterface::getILockStates(const std::string& name) const
{
    if(entryExists(allShutterData, name))
        return allShutterData.at(name).iLockStates;
    else
    {
        std::map<ILOCK_NUMBER, ILOCK_STATE> r =
            {
                {ILOCK_NUMBER::ILOCK_ERR,ILOCK_STATE::ILOCK_ERROR}
            };
        return r;
    }
}
//______________________________________________________________________________
std::map<ILOCK_NUMBER, std::string>
    shutterInterface::getILockStatesStr(const std::string& name)const
{
    std::map<ILOCK_NUMBER, std::string> r;
    if(entryExists(allShutterData, name))
    {
        for(auto const& it : allShutterData.at(name).iLockStates)
        {
            r[it.first] = ENUM_TO_STRING(it.second);
        }
    }
    else
    {
        r[ILOCK_NUMBER::ILOCK_ERR] = ENUM_TO_STRING(ILOCK_STATE::ILOCK_ERROR);
    }
    return r;
}
//______________________________________________________________________________
