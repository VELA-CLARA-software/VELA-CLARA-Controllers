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
//  Last edit:   06-02-2019
//  FileName:    ba1StagesInterface.cpp
//  Description:
//
//
//*/
// project includes
#include "ba1StagesInterface.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
using namespace ba1StagesStructs;
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
ba1StagesInterface::ba1StagesInterface(const bool& show_messages,
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
    message("Constructing a ba1StagesInterface");
    initialise();
}
//______________________________________________________________________________
ba1StagesInterface::~ba1StagesInterface()
{
    for(auto&& it : continuousMonitorStructs)
    {
        debugMessage("delete ba1StagesInterface continuousMonitorStructs entry.");
        delete it;
    }
}
//______________________________________________________________________________
void ba1StagesInterface::initialise()
{
    /*
        The config file reader
    */
    configFileRead = configReader.readConfig();
    if(configFileRead)
    {
        message("The ba1StagesInterface has read the config file, "
                "acquiring objects");
        /* Initialise the objects based on what is read from the config file */
        bool getDataSuccess = initObjects();
        if(getDataSuccess )
        {
            if(shouldStartEPICs)
            {
                message("The ba1StagesInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                startMonitors();
                /* The pause allows EPICS callbacks to catch up. */
                pause_500();
            }
            else
             message("The ba1StagesInterface has acquired objects, "
                     "NOT connecting to EPICS");
        }
        else
            message("!!!The ba1StagesInterface received an Error "
                    "while getting laser data!!!" );
    }
}
//______________________________________________________________________________
bool ba1StagesInterface::initObjects()
{
    const std::vector<ba1StagesObject>ba1StagesObjs = configReader.getBA1StageObjects();
    for(auto const& it:ba1StagesObjs)
    {
        allBA1StageData[it.name] = it;
    }
    return true;
}
//______________________________________________________________________________
void ba1StagesInterface::initChids()
{
    message("\n", "Searching for ba1Stage ChIds...");
    for(auto&& it1:allBA1StageData)
    {
//        addILockChannels(it1.second.numIlocks,
//                         it1.second.pvRoot,
//                         it1.second.name,
//                         it1.second.iLockPVStructs);

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
                             "Found ba1Stage ChIds.",
                             "!!TIMEOUT!! Not all ba1Stages ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        UTL::STANDARD_PAUSE;
        for(const auto & it : allBA1StageData)
        {
            message("\n", "Checking Chids for ", it.first);
            for(auto & it2 : it.second.pvMonStructs)
            {
                checkCHIDState(it2.second.CHID, it2.second.pvSuffix);
            }
            for(auto & it2 : it.second.pvComStructs)
            {
                checkCHIDState(it2.second.CHID, it2.second.pvSuffix);
            }
//            for(auto & it2 : it.second.iLockPVStructs)
//                checkCHIDState(it2.second.CHID, it2.second.pv);
        }
        message("");
    }
    else if (status == ECA_NORMAL)
    {
        allChidsInitialised = true;
    }
}
//______________________________________________________________________________
void ba1StagesInterface::addChannel(const std::string& pvRoot, pvStruct& pv)
{
    const std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    debugMessage("Create channel to ", s);
}
//______________________________________________________________________________
void ba1StagesInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();
    for(auto && it : allBA1StageData)
    {
        //monitorIlocks(it.second.iLockPVStructs, it.second.iLockStates);
        for(auto && it2 : it.second.pvMonStructs )
        {
            /*
                init structs 'correctly'
http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
            */
            continuousMonitorStructs.push_back(new ba1StagesStructs::monitorStruct());
            continuousMonitorStructs.back()->interface = this;
            continuousMonitorStructs.back()->stageObj   = &(it.second);
            continuousMonitorStructs.back()->CHTYPE    = it2.second.CHTYPE;
            /*
                For the ba1Stages there is only 1 monitor type
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
                                   ba1StagesInterface::staticEntryBA1StageMonitor,
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

// These functions are called from EPICS subscriptions
void ba1StagesInterface::staticEntryBA1StageMonitor(const event_handler_args args)
{
    ba1StagesStructs::monitorStruct*ms = static_cast<ba1StagesStructs::monitorStruct *>(args.usr);
    ms->interface->updateBa1StageValue(ms->monType, *(ms->stageObj), args);
}

//______________________________________________________________________________
void ba1StagesInterface::updateBa1StageValue(const ba1StagesStructs::BA1STAGE_PV_TYPE pv, ba1StagesStructs::ba1StagesObject& stageObj,
                                          const event_handler_args& args)
{
    /*
        recast args.usr (a void *) to a monitor struct pointer
    */
    monitorStuct* ms = reinterpret_cast<monitorStuct*>(args.usr);
    /*
        Not sure how to decode these apart from trial and error
        you can test with DBF_STRING as the callback type
    */
    using namespace ba1StagesStructs;
    switch(pv)
    {
        case BA1STAGE_PV_TYPE::RPOS:
            stageObj.r_position = getDBRdouble(args);
            message(stageObj.name, " new RPOS = ", stageObj.r_position);
            break;
        case BA1STAGE_PV_TYPE::MPOS:
            stageObj.m_position = getDBRdouble(args);
            message(stageObj.name, " new MPOS = ", stageObj.m_position);
            break;
        default:
            message("ERROR UNKNOWN PV PASSED TO staticEntryBA1StageMonitor");
    }
}
//
//     __   __                         __   __
//    /  ` /  \  |\/|  |\/|  /\  |\ | |  \ /__`
//    \__, \__/  |  |  |  | /~~\ | \| |__/ .__/
//
//______________________________________________________________________________

size_t ba1StagesInterface::getStageNumber(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).stage_number;
    }
    return UTL::ZERO_SIZET;
}
//______________________________________________________________________________
size_t ba1StagesInterface::getStagePrecision(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).precision;
    }
    return UTL::ZERO_SIZET;
}
//______________________________________________________________________________
double ba1StagesInterface::getStageSetPosition(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).m_position;
    }
    return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
double ba1StagesInterface::getStagePosition(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).r_position;
    }
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------
bool ba1StagesInterface::setStagePosition(const std::string& stage, double val)
{
    if(entryExists(allBA1StageData, stage))
    {
        // test to see if the requested value is outside th emina / max position

        bool can_move = true;

        if(val < allBA1StageData.at(stage).min_pos)
        {
            message(val, " < min_pos (", allBA1StageData.at(stage).min_pos,")");
            can_move = false;
        }
        if(val > allBA1StageData.at(stage).max_pos)
        {
            message(val, " > max_pos (", allBA1StageData.at(stage).max_pos,")");
            can_move = false;
        }


        if(can_move)
        {
            message(val," is in acceptable range, Attempting to move ", stage);
            return  moveTo(allBA1StageData.at(stage).pvMonStructs.at(ba1StagesStructs::BA1STAGE_PV_TYPE::MPOS), val);
        }
        else
        {
            message(val, " was outside acceptable range, not moving");
        }

//
//
//        if(val > allBA1StageData.at(stage).min_pos)
//        {
//            message(val," > ", allBA1StageData.at(stage).min_pos);
//            if(val < allBA1StageData.at(stage).max_pos)
//            {
//                message(val," < ", allBA1StageData.at(stage).max_pos);
//                message("Attempting to move ", stage);
//                return  moveTo(allBA1StageData.at(stage).pvMonStructs.at(ba1StagesStructs::BA1STAGE_PV_TYPE::MPOS), val);
//            }
//            else
//            {
//                message(val, " > max  possible val (", allBA1StageData.at(stage).max_pos,")");
//            }
//        }
//        else
//        {
//            message(val, " < min possible val (", allBA1StageData.at(stage).min_pos,")");
//        }
    }
    else
    {
        message("Can't find stage = ", stage);
    }
    return false;
}
//--------------------------------------------------------------------------------
bool ba1StagesInterface::setDevice(const std::string& stage, const std::string& device)
{
    if(entryExists(allBA1StageData, stage))
    {
        if(entryExists(allBA1StageData.at(stage).device_position_map, device))
        {
            return setStagePosition(stage, allBA1StageData.at(stage).device_position_map.at(device));
        }
        else
        {
            message("Can't find device = ", device, " on stage = ", stage);
        }
    }
    else
    {
        message("Can't find stage = ", stage);
    }
    return false;
}
//--------------------------------------------------------------------------------
double ba1StagesInterface::getDevicePos(const std::string& stage, const std::string& device)
{
    if(entryExists(allBA1StageData, stage))
    {
        if(entryExists(allBA1StageData.at(stage).device_position_map, device))
        {
            return allBA1StageData.at(stage).device_position_map.at(device);
        }
        else
        {
            message("Can't find device = ", device, " on stage = ", stage);
        }
    }
    else
    {
        message("Can't find stage = ", stage);
    }
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------
bool ba1StagesInterface::moveTo(const ba1StagesStructs::pvStruct& pvs, double value)
{
    std::stringstream ss;
    ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    bool ret = false;
    ca_put(pvs.CHTYPE, pvs.CHID, &value);
    //debugMessage(ss);
    ss.str("");
    ss << "Timeout setting stage position to " << value;
    int status = sendToEpics("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}
//--------------------------------------------------------------------------------
std::vector<std::string> ba1StagesInterface::getDevices(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).devices;
    }
    std::vector<std::string> r;
    return r;
}
//--------------------------------------------------------------------------------
std::map<std::string, double>  ba1StagesInterface::getStageDeviceAndPositionMap(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).device_position_map;
    }
    std::map<std::string, double>  r;
    return r;
}
//--------------------------------------------------------------------------------
bool ba1StagesInterface::screenIn(const std::string& stage)
{
    if(entryExists(allBA1StageData, stage))
    {
        if( allBA1StageData.at(stage).has_yag )
        {
            return moveTo(allBA1StageData.at(stage).pvComStructs.at(ba1StagesStructs::BA1STAGE_PV_TYPE::MPOS),
                          allBA1StageData.at(stage).yag_in_pos);
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool ba1StagesInterface::screenOut(const std::string& stage)
{
    if(entryExists(allBA1StageData, stage))
    {
        if( allBA1StageData.at(stage).has_yag )
        {
            return moveTo(allBA1StageData.at(stage).pvComStructs.at(ba1StagesStructs::BA1STAGE_PV_TYPE::MPOS),
                          allBA1StageData.at(stage).yag_out_pos);
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
std::vector<std::string> ba1StagesInterface::getStageNames()const
{
    std::vector<std::string> r;
    for(auto&& it: allBA1StageData)
    {
        r.push_back(it.first);
    }
    return r;
}
//--------------------------------------------------------------------------------
bool ba1StagesInterface::isReadPosEqualSetPos(const std::string& stage, double tolerance)
{
    if(entryExists(allBA1StageData, stage))
    {
        message(allBA1StageData.at(stage).r_position," ",allBA1StageData.at(stage).m_position);
        return areSame( allBA1StageData.at(stage).r_position, allBA1StageData.at(stage).m_position, tolerance);
    }
    return false;
}
//--------------------------------------------------------------------------------
double ba1StagesInterface::getMaxPos(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).max_pos;
    }
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------
double ba1StagesInterface::getMinPos(const std::string& stage)const
{
    if(entryExists(allBA1StageData, stage))
    {
        return allBA1StageData.at(stage).min_pos;
    }
    return UTL::DUMMY_DOUBLE;
}
















//______________________________________________________________________________
using namespace HWC_ENUM;
std::map<ILOCK_NUMBER,ILOCK_STATE>
    ba1StagesInterface::getILockStates(const std::string& name) const
{
//    if(entryExists(allBA1StageData, name))
//        return allBA1StageData.at(name).iLockStates;
//    else
//    {
        std::map<ILOCK_NUMBER, ILOCK_STATE> r =
            {
                {ILOCK_NUMBER::ILOCK_ERR,ILOCK_STATE::ILOCK_ERROR}
            };
        return r;
//    }
}
//______________________________________________________________________________
std::map<ILOCK_NUMBER, std::string>
    ba1StagesInterface::getILockStatesStr(const std::string& name)const
{
    std::map<ILOCK_NUMBER, std::string> r;
//    if(entryExists(allBA1StageData, name))
//    {
//        for(auto const& it : allBA1StageData.at(name).iLockStates)
//        {
//            r[it.first] = ENUM_TO_STRING(it.second);
//        }
//    }
//    else
//    {
        r[ILOCK_NUMBER::ILOCK_ERR] = ENUM_TO_STRING(ILOCK_STATE::ILOCK_ERROR);
//    }
    return r;
}
//______________________________________________________________________________
