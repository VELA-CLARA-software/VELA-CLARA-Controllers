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
//  FileName:    virtualCathodeInterface.cpp
//  Description:
//
//
//*/
// project includes
#include "virtualCathodeInterface.h"
#include "configDefinitions.h"
// stl includes
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
virtualCathodeInterface::virtualCathodeInterface(bool* show_messages,
                                   bool* show_debug_messages,
                                   const bool startVirtualMachine,
                                   const bool shouldStartEPICs,
                                   const std::string& configFile
                                 ):
configReader(configFile, show_messages, show_debug_messages, startVirtualMachine),
interface(show_messages, show_debug_messages)
{
    if(shouldStartEPICs)
        message("virtualCathodeData shouldStartEPICs is true");
    else
        message("virtualCathodeData shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
virtualCathodeInterface::~virtualCathodeInterface()
{
//    killILockMonitors();
//    for(auto && it : continuousMonitorStructs)
//    {
//        killMonitor(it);
//        delete it;
//    }
//    debugMessage("virtualCathodeInterface DESTRUCTOR COMPLETE ");
}
////______________________________________________________________________________
//void virtualCathodeInterface::killMonitor(pilaserStructs::monitorStruct * ms)
//{
//    int status = ca_clear_subscription(ms -> EVID);
//    //if(status == ECA_NORMAL)
//        //debugMessage(ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
////    else
//        //debugMessage("ERROR virtualCathodeInterface: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType));
//}
//______________________________________________________________________________
void virtualCathodeInterface::initialise()
{
    /* read config */
    configFileRead = configReader.readConfig();
    UTL::STANDARD_PAUSE;
    if(configFileRead)
    {
        message("The virtualCathodeInterface has read the config file"
                ", acquiring objects");
        bool getDataSuccess = initObjects();
        if( getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The virtualCathodeInterface has acquired objects"
                        ", connecting to EPICS");
                initChids();
                startMonitors();
                /* The pause allows EPICS to catch up. */
                UTL::STANDARD_PAUSE;
            }
            else
            {
             message("The virtualCathodeInterface has acquired objects"
                     ", NOT connecting to EPICS");
            }
        }
        else
        {
            message("!!!The virtualCathodeInterface received an Error "
                    "while getting laser data!!!");
        }
    }
}
//______________________________________________________________________________
bool virtualCathodeInterface::initObjects()
{
    bool ans = configReader.getVirtualCathodeDataObject(object);
    debugMessage("pilaser.pvMonStructs.size() = ", object.pvMonStructs.size());
    // add the first data_struct to the buffer
    addToBuffer();
    // on start-up the monitored data MUST go in the first elment of the buffer
    // so set those pointers in image_data_buffer_next_update
    for(auto&& it: object.image_data_buffer.front())
    {
        object.image_data_buffer_next_update[it.first] = &object.image_data_buffer.front();
    }
    return ans;
}
//______________________________________________________________________________
void virtualCathodeInterface::initChids()
{
    message("\n", "Searching for VirtualCathodeData chids...");
    for(auto && it : object.pvMonStructs)
    {
        addChannel(object.pvRoot, it.second);
    }
    // currently there are no command only PVs for the PIL
    int status = sendToEpics("ca_create_channel",
                             "Found VirtualCathodeData ChIds.",
                             "!!TIMEOUT!! Not all VirtualCathodeData ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        UTL::PAUSE_500;
        message("\n", "Checking VirtualCathodeData ChIds ");
        for(auto && it : object.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        UTL::STANDARD_PAUSE;
    }
    else if (status == ECA_NORMAL)
    {
        allChidsInitialised = true;
    }
}
//______________________________________________________________________________
void virtualCathodeInterface::addChannel(const std::string & pvRoot,
                                         virtualCathodeStructs::pvStruct & pv)
{
    std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    message("Create channel to ", s);
}
//______________________________________________________________________________
void virtualCathodeInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : object.pvMonStructs)
    {
        continuousMonitorStructs.push_back(new virtualCathodeStructs::monitorStruct());
        continuousMonitorStructs.back() -> monType    = it.first;
        continuousMonitorStructs.back() -> object = &object;
        continuousMonitorStructs.back() -> interface  = this;
        ca_create_subscription(it.second.CHTYPE,
                               it.second.COUNT,
                               it.second.CHID,
                               it.second.MASK,
                               virtualCathodeInterface::staticEntryMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back() -> EVID);
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to VirtualCathodeData Monitors",
                             "!!TIMEOUT!! Subscription to VirtualCathodeData monitors failed");
    if (status == ECA_NORMAL)
    {
        allMonitorsStarted = true;
    }
}
//____________________________________________________________________________________________
void virtualCathodeInterface::staticEntryMonitor(const event_handler_args args)
{
    using namespace virtualCathodeStructs;
    monitorStruct*ms = static_cast<monitorStruct *>(args.usr);
    ms->interface->updateValue(args,ms -> monType);
}
//____________________________________________________________________________________________
void virtualCathodeInterface::updateValue(const event_handler_args args,
                                          virtualCathodeStructs::PV_TYPE pv)
{
    /*
        update the values,
        using the pointer at object.image_data_buffer_next_update.at(pv)
        to access the correct part of image_data_buffer
    */
    getDBRdouble_timestamp(args,
                           object.image_data_buffer_next_update.at(pv)->at(pv).time,
                           object.image_data_buffer_next_update.at(pv)->at(pv).value
    );
    /*
        we now KNOW image_data_buffer must be extended, but ...
        we need to check if it already has been extended
        check if the pointer is the same as the address of the back element
    */
    if(object.image_data_buffer_next_update.at(pv) == &object.image_data_buffer.back())
    {
        /*
            image_data_buffer has not already been extended
            so extend it
        */
        addToBuffer();
    }
    /*
        update image_data_buffer_next_update to point at the
        back of image_data_buffer
    */
    object.image_data_buffer_next_update.at(pv) = &object.image_data_buffer.back();
}
//____________________________________________________________________________________________
void virtualCathodeInterface::addToBuffer()
{
    object.image_data_buffer.push_back( virtualCathodeStructs::static_blank_image_data::create_blank_image_data());
    if(object.image_data_buffer.size() > object.buffer_size)
    {
        object.image_data_buffer.pop_front();
    }
}
////____________________________________________________________________________________________
//double virtualCathodeInterface::getHpos()
//{
//    return pilaser.hPos;
//}
////____________________________________________________________________________________________
//double virtualCathodeInterface::getVpos()
//{
//    return pilaser.vPos;
//}
////____________________________________________________________________________________________
//double virtualCathodeInterface::getIntensity()
//{
//    return pilaser.intensity;
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setHpos(double value)
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_POS),value);
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setHpos(int value)
//{
//    return setHpos((double)value);
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setVpos(double value)
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_POS),value);
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setVpos(int value)
//{
//    return setVpos((double)value);
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setIntensity(double value)
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::INTENSITY),value);
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setIntensity(int value)
//{
//    return setIntensity((double)value);
//}
////____________________________________________________________________________________________
//const pilaserStructs::pilaserObject& virtualCathodeInterface::getPILObjConstRef()
//{
//    return pilaser;
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::setValue(pilaserStructs::pvStruct& pvs, double value)
//{
//    bool ret = false;
//    ca_put(pvs.CHTYPE,pvs.CHID,&value);
//    std::stringstream ss;
//    ss << "Timeout setting pilaser," << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
//    int status = sendToEpics("ca_put","",ss.str().c_str());
//    if(status==ECA_NORMAL)
//        ret=true;
//    return ret;
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::openShutter()
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::ON),1);
//}
////____________________________________________________________________________________________
//bool virtualCathodeInterface::closeShutter()
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::OFF),1);
//}
////____________________________________________________________________________________________
//



