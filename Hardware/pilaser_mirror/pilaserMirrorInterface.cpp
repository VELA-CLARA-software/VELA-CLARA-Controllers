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
//  FileName:    pilaserMirrorInterface.cpp
//  Description:
//
//
//*/
#include "pilaserMirrorInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>
using namespace pilaserMirrorStructs;
//______________________________________________________________________________
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
pilaserMirrorInterface::pilaserMirrorInterface(bool& show_messages,
                         bool& show_debug_messages,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const std::string& configFile
                       ):
configReader(configFile,  show_messages, show_debug_messages,startVirtualMachine),
interface(show_messages,show_debug_messages,shouldStartEPICs,startVirtualMachine)

{
    message("Constructing a pilaserMirrorInterface");
    initialise();
}
//______________________________________________________________________________
pilaserMirrorInterface::~pilaserMirrorInterface()
{
    killILockMonitors();
    for(auto && it : continuousMonitorStructs)
    {
        killMonitor(it);
        delete it;
    }
    debugMessage("pilaserMirrorInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void pilaserMirrorInterface::killMonitor(monitorStruct * ms)
{
    int status = ca_clear_subscription(ms -> EVID);
}
//______________________________________________________________________________
void pilaserMirrorInterface::initialise()
{
    /* The config file reader */
    configFileRead = configReader.readConfig();
    if(configFileRead)
    {
        message("The pilaserMirrorInterface has read the config file, "
                "acquiring objects");
        /*initialise the objects based on what is read from the config file */
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The pilaserMirrorInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                startMonitors();
                /* The pause allows EPICS callbacks to catch up. */
                pause_2000();
            }
            else
             message("The pilaserMirrorInterface has acquired objects, "
                     "NOT connecting to EPICS");
        }
        else
            message("!!!The pilaserMirrorInterface received an Error "
                    "while getting laser data!!!");
    }
}
//______________________________________________________________________________
bool pilaserMirrorInterface::initObjects()
{
    return configReader.getpilMirrorObject(laserMirror);
}
//______________________________________________________________________________
void pilaserMirrorInterface::initChids()
{
    message("\n", "Searching for PILaserMirror chids...");

    for(auto && it : laserMirror.pvMonStructs)
    {
        addChannel(laserMirror.pvRoot, it.second);
    }
    for(auto && it : laserMirror.pvComStructs)
    {
        addChannel(laserMirror.pvRoot, it.second);
    }
//    addILockChannels(laserMirror.numIlocks,
//                       laserMirror.pvRoot,
//                       laserMirror.name,
//                       laserMirror.iLockPVStructs);
    int status = sendToEpics("ca_create_channel",
                             "Found PILaserMirror ChIds.",
                             "!!TIMEOUT!! Not all PILaserMirror ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        pause_500();
        message("\n", "Checking PILaserMirror ChIds ");
        for(auto && it : laserMirror.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto && it : laserMirror.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        pause_2000();
    }
    else if (status == ECA_NORMAL)
        allChidsInitialised = true;
}
//______________________________________________________________________________
void pilaserMirrorInterface::addChannel(const std::string& pvRoot,
                                            pvStruct& pv)
{
    const std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    debugMessage("Create channel to ", s);
}
//______________________________________________________________________________
void pilaserMirrorInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : laserMirror.pvMonStructs)
    {
        continuousMonitorStructs.push_back(new monitorStruct());
        continuousMonitorStructs.back()->pilMirrorObj = &laserMirror;
        continuousMonitorStructs.back()->monType    = it.first;
        continuousMonitorStructs.back()->interface  = this;
        ca_create_subscription(it.second.CHTYPE,
                               it.second.COUNT,
                               it.second.CHID,
                               it.second.MASK,
                               pilaserMirrorInterface::staticEntryPILMirrorMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back()->EVID);
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to PILaserMirror Monitors",
                             "!!TIMEOUT!! Subscription to PILaserMirror monitors failed");
    if (status == ECA_NORMAL)
        allMonitorsStarted = true; /// interface base class member
}
//____________________________________________________________________________________________
void pilaserMirrorInterface::staticEntryPILMirrorMonitor(const event_handler_args args)
{
    monitorStruct*ms = static_cast<monitorStruct*>(args.usr);
    switch(ms -> monType)
    {
        case pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::H_POS:
            ms->pilMirrorObj->hPos = getDBRdouble(args);
            break;
        case pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_POS:
            ms->pilMirrorObj->vPos = getDBRdouble(args);
            break;
        case pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::H_STEP:
            ms->pilMirrorObj->hStep = getDBRdouble(args);
            break;
        case pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_STEP:
            ms->pilMirrorObj->vStep = getDBRdouble(args);
            break;
        default:
            ms->interface->message("!!! ERROR !!! Unknown Monitor Type passed to "
                                   "pilaserMirrorInterface::staticEntryPILMonitor");
            break;
    }
}
//____________________________________________________________________________________________
double pilaserMirrorInterface::getHstep() const
{
    return laserMirror.hStep;
}
//____________________________________________________________________________________________
double pilaserMirrorInterface::getVstep() const
{
    return laserMirror.vStep;
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::setHstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            laserMirror.pvMonStructs.at(pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::H_STEP),value);
    }
    laserMirror.hStep = value;
    return s;
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::setVstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            laserMirror.pvMonStructs.at(pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_STEP),value);
    }
    if(value)
    laserMirror.vStep = value;
    return s;
}
//____________________________________________________________________________________________
double pilaserMirrorInterface::getHpos() const
{
    return laserMirror.hPos;
}
//____________________________________________________________________________________________
double pilaserMirrorInterface::getVpos() const
{
    return laserMirror.vPos;
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::setHpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            laserMirror.pvMonStructs.at(pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_STEP),value);
    }
    laserMirror.vStep = value;
    return s;
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::setVpos(const double value)
{
    return setValue(laserMirror.pvMonStructs.at(pilaserMirrorStructs::PIL_MIRROR_PV_TYPE::V_POS),value);
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::setValue(pilaserMirrorStructs::pvStruct& pvs,const double value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE, pvs.CHID, &value);
    std::stringstream ss;
    ss << "Timeout setting laserMirror," << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    int status = sendToEpics("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::moveH()
{
    pvStruct& pvs = laserMirror.pvComStructs.at(PIL_MIRROR_PV_TYPE::H_MOVE);
    return move(pvs.CHTYPE,pvs.CHID,
                  "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO MOVE H",
                  "!!TIMEOUT!! FAILED TO SEND MOVE H");
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::moveV()
{
    pvStruct& pvs = laserMirror.pvComStructs.at(PIL_MIRROR_PV_TYPE::V_MOVE);
    return move(pvs.CHTYPE,pvs.CHID,
                  "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO MOVE V",
                  "!!TIMEOUT!! FAILED TO SEND MOVE V");
}
//____________________________________________________________________________________________
bool pilaserMirrorInterface::move(chtype& cht, chid& chi,const char* m1,const char* m2)
{
    //need to figure out how to move th elaser form command line...

    int status = caput(cht, chi, EPICS_ACTIVATE, "", m1);
    if(status == ECA_NORMAL)
    {
        status = caput(cht, chi, EPICS_SEND, "", m2);
    }
    if(status == ECA_NORMAL)
        return true;
    return false;
}
//______________________________________________________________________________
const pilMirrorObject& pilaserMirrorInterface::getpilMirrorObjConstRef() const
{
    return laserMirror;
}
//____________________________________________________________________________________________
//bool pilaserMirrorInterface::setIntensity(double value)
//{
//    return setValue(laserMirror.pvMonStructs.at(laserMirrorStructs::PILASER_PV_TYPE::INTENSITY),value);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setIntensity(int value)
//{
//    return setIntensity((double)value);
//}
////____________________________________________________________________________________________
//const laserMirrorStructs::laserMirrorObject& pilaserMirrorInterface::getPILObjConstRef()
//{
//    return laserMirror;
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::openShutter()
//{
//    return setValue(laserMirror.pvMonStructs.at(laserMirrorStructs::PILASER_PV_TYPE::ON),1);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::closeShutter()
//{
//    return setValue(laserMirror.pvMonStructs.at(laserMirrorStructs::PILASER_PV_TYPE::OFF),1);
//}
////____________________________________________________________________________________________
//



