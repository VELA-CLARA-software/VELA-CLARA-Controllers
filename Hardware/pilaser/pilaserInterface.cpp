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
//  FileName:    pilaserInterface.cpp
//  Description:
//
//
//*/
#include "pilaserInterface.h"
//djs
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
//______________________________________________________________________________
pilaserInterface::pilaserInterface(bool& show_messages,
                                   bool& show_debug_messages,
                                   const bool startVirtualMachine,
                                   const bool shouldStartEPICs,
                                   const std::string& configFile
                                  ):
configReader(configFile,  show_messages, show_debug_messages,startVirtualMachine),
interface(show_messages,show_debug_messages, shouldStartEPICs,startVirtualMachine)
{
    initialise();
}
//______________________________________________________________________________
pilaserInterface::~pilaserInterface()
{
    killILockMonitors();
    for(auto&& it:continuousMonitorStructs)
    {
        killMonitor(it);
        delete it;
    }
    debugMessage("pilaserInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void pilaserInterface::killMonitor(pilaserStructs::monitorStruct* ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
        debugMessage(ms->pilaserObj, " ",
                     ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR pilaserInterface: in killMonitor: "
                     "ca_clear_subscription failed for ",
                     ms->pilaserObj, " ",
                     ENUM_TO_STRING(ms->monType));
}
//______________________________________________________________________________
void pilaserInterface::initialise()
{
    std::cout << "pilaserInterface::initialise()" << std::endl;
    /* The config file reader */
    configFileRead = configReader.readConfig();
    if(configFileRead)
    {
        message("The pilaserInterface has read the config file, "
                "acquiring objects");
        /*initialise the objects based on what is read from the config file */
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The pilaserInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                startMonitors();
                /* The pause allows EPICS callbacks to catch up. */
                pause_2000();
            }
            else
             message("The pilaserInterface has acquired objects, "
                     "NOT connecting to EPICS");
        }
        else
            message("!!!The pilaserInterface received an Error "
                    "while getting laser data!!!");
    }
}
//______________________________________________________________________________
bool pilaserInterface::initObjects()
{
    return configReader.getpilaserObject(pilaser);
}
//______________________________________________________________________________
void pilaserInterface::initChids()
{
    message("\n", "Searching for PILaser chids...");

    for(auto&& it:pilaser.pvMonStructs)
    {
        addChannel(pilaser.pvRoot, it.second);
    }
    for(auto&& it:pilaser.pvComStructs)
    {
        addChannel(pilaser.pvRoot, it.second);
    }
    //addILockChannels(pilaser.numIlocks, pilaser.pvRoot, pilaser.name, pilaser.iLockPVStructs );
    int status = sendToEpics("ca_create_channel",
                             "Found PILaser ChIds.",
                             "!!TIMEOUT!! Not all PILaser ChIds found.");
    if(status == ECA_TIMEOUT )
    {
        pause_500();
        message("\n", "Checking PILaser ChIds ");
        for(auto&& it:pilaser.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto&& it:pilaser.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        pause_2000();
    }
    else if (status == ECA_NORMAL)
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void pilaserInterface::addChannel(const std::string& pvRoot,
                                  pilaserStructs::pvStruct& pv)
{
    const std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    debugMessage("Create channel to ", s);
}
//______________________________________________________________________________
void pilaserInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto&& it:pilaser.pvMonStructs)
    {
        continuousMonitorStructs.push_back(new pilaserStructs::monitorStruct());
        continuousMonitorStructs.back()->monType    = it.first;
        continuousMonitorStructs.back()->pilaserObj = &pilaser;
        continuousMonitorStructs.back()->interface  = this;
        ca_create_subscription(it.second.CHTYPE,
                               it.second.COUNT,
                               it.second.CHID,
                               it.second.MASK,
                               pilaserInterface::staticEntryPILMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back() -> EVID);
    }
    int status = sendToEpics("ca_create_subscription", "Succesfully Subscribed to PILaser Monitors", "!!TIMEOUT!! Subscription to PILaser monitors failed" );
    if (status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//____________________________________________________________________________________________
void pilaserInterface::staticEntryPILMonitor(const event_handler_args args)
{
    pilaserStructs::monitorStruct*ms = static_cast<pilaserStructs::monitorStruct *>(args.usr);
    switch(ms -> monType)
    {
        case pilaserStructs::PILASER_PV_TYPE::INTENSITY:
            //ms->interface->debugMessage("PILaser H_pos = ",*(double*)args.dbr);
            ms->pilaserObj->intensity =  getDBRdouble(args);
            // update charg e
            break;
        case pilaserStructs::PILASER_PV_TYPE::STABILISATION:
            //ms->interface->debugMessage("PILaser V_POS = ",*(double*)args.dbr);
            ms->pilaserObj->stabilisation_status =  HWC_ENUM::STATE::ERR;
            break;
        case pilaserStructs::PILASER_PV_TYPE::STATUS:
            //ms->interface->debugMessage("PILaser intensity = ",*(double*)args.dbr);
            ms->pilaserObj->status =  HWC_ENUM::STATE::ERR;
            break;
        case pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_READ:
            //ms->interface->debugMessage("PILaser intensity = ",*(double*)args.dbr);
            ms->pilaserObj->HWP =  getDBRdouble(args);
            break;
        default:
            ms->interface->message("!!! ERROR !!! Unknown Monitor Type passed to pilaserInterface::staticEntryPILMonitor");
            break;
    }
}
//____________________________________________________________________________________________
int pilaserInterface::setHWP(const double value)
{
    // To Write
    return caput<double>
            (pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_SET).CHTYPE,
             pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_SET).CHID,
             value,
             "", "Timeout sending Half Wave Plate Value");
}
//____________________________________________________________________________________________
double pilaserInterface::getHWP()
{
    return pilaser.HWP;
}
//____________________________________________________________________________________________
bool pilaserInterface::setCharge(const double value)
{
    // To Write
    return false;
}
//____________________________________________________________________________________________
bool pilaserInterface::setIntensity(const double value)
{
    // To Write
    return false;
}
//____________________________________________________________________________________________
double pilaserInterface::getIntensity() const
{
    return pilaser.intensity;
}
//____________________________________________________________________________________________
std::string pilaserInterface::getName() const
{
    return pilaser.name;
}
//____________________________________________________________________________________________
HWC_ENUM::STATE pilaserInterface::getStatus() const
{
    return pilaser.status;
}
//____________________________________________________________________________________________
double pilaserInterface::getStabilisationStatus() const
{
    return pilaser.stabilisation_status;
}
//____________________________________________________________________________________________
bool pilaserInterface::isOn() const
{
    return pilaser.status == HWC_ENUM::STATE::ON;
}
//____________________________________________________________________________________________
bool pilaserInterface::isOff() const
{
    return pilaser.status == HWC_ENUM::STATE::OFF;
}
//____________________________________________________________________________________________
bool pilaserInterface::isStabilisationOn() const
{
    return pilaser.stabilisation_status == HWC_ENUM::STATE::ON;
}
//____________________________________________________________________________________________
bool pilaserInterface::isStabilisationOff() const
{
    return pilaser.stabilisation_status == HWC_ENUM::STATE::OFF;
}
//____________________________________________________________________________________________
bool pilaserInterface::enableStabilisation()
{
    // to write
    return false;
}
//____________________________________________________________________________________________
bool pilaserInterface::disableStabilisation()
{
    // to write
    return false;
}
//____________________________________________________________________________________________
const pilaserStructs::pilaserObject& pilaserInterface::getPILObjConstRef() const
{
    return pilaser;
}
//____________________________________________________________________________________________
bool pilaserInterface::setValue(pilaserStructs::pvStruct& pvs,const double value)
{
    bool ret = false;
//    ca_put(pvs.CHTYPE,pvs.CHID,&value);
//    std::stringstream ss;
//    ss << "Timeout setting pilaser," << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
//    int status = sendToEpics("ca_put","",ss.str().c_str());
//    if(status==ECA_NORMAL)
//        ret=true;
    return ret;
}


