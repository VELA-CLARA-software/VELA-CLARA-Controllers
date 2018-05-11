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
virtualCathodeInterface::virtualCathodeInterface(bool& show_messages,
                                   bool& show_debug_messages,
                                   const bool startVirtualMachine,
                                   const bool shouldStartEPICs,
                                   const std::string& vcMirrorConfig,
                                   const std::string& vcDataConfig
                                 ):
configReader(vcMirrorConfig,vcDataConfig, show_messages, show_debug_messages, startVirtualMachine),
interface(show_messages,show_debug_messages,shouldStartEPICs,startVirtualMachine)
{
    message("Constructing a virtualCathodeInterface");
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
//        //debugMessage("ERROR virtualCathodeInterf0ace: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType));
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
                ", acquiring vcObjects");
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
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
    bool ans = configReader.getVirtualCathodeObject(vcObject);
    debugMessage("pilaser.pvMonStructs.size() = ", vcObject.pvMonStructs.size());
    // add the first data_struct to the buffer
    addToBuffer();
    // on start-up the monitored data MUST go in the first elment of the buffer
    // so set those pointers in image_data_buffer_next_update
//    for(auto&& it: vcObject.image_data_buffer.front())
//    {
//        vcObject.image_data_buffer_next_update[it.first] = &vcObject.image_data_buffer.front();
//    }
    /*
        set the maps,
        This is NOT SAFE
    */
    vcObject.pixel_values_pos[vcObject.x_pos] = vcObject.x_name;
    vcObject.pixel_values_pos[vcObject.y_pos] = vcObject.y_name;
    vcObject.pixel_values_pos[vcObject.x_sigma_pos] = vcObject.x_sigma_name;
    vcObject.pixel_values_pos[vcObject.y_sigma_pos] = vcObject.y_sigma_name;
    vcObject.pixel_values_pos[vcObject.cov_pos] = vcObject.cov_name;
    vcObject.pixel_values[vcObject.x_name] = UTL::DUMMY_DOUBLE;
    vcObject.pixel_values[vcObject.y_name] = UTL::DUMMY_DOUBLE;
    vcObject.pixel_values[vcObject.x_sigma_name] = UTL::DUMMY_DOUBLE;
    vcObject.pixel_values[vcObject.y_sigma_name] = UTL::DUMMY_DOUBLE;
    vcObject.pixel_values[vcObject.cov_name] = UTL::DUMMY_DOUBLE;

    vcObject.pix_values.resize(vcObject.results_count); //MAGIC_NUMBER

    return ans;
}
//______________________________________________________________________________
void virtualCathodeInterface::initChids()
{
    message("\n", "Searching for VirtualCathodeData chids...");
    for(auto &&it:vcObject.pvMonStructs)
    {
        addChannel(it.second);
    }
    for(auto &&it:vcObject.pvComStructs)
    {
        addChannel(it.second);
    }
    int status = sendToEpics("ca_create_channel",
                             "Found VirtualCathodeData ChIds.",
                             "!!TIMEOUT!! Not all VirtualCathodeData ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        pause_500();
        message("\n", "Checking VirtualCathodeData ChIds ");
        for(auto && it : vcObject.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto && it : vcObject.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        pause_500();
    }
    else if (status == ECA_NORMAL)
    {
        allChidsInitialised = true;
    }
}
//______________________________________________________________________________
void virtualCathodeInterface::addChannel(virtualCathodeStructs::pvStruct & pv)
{
    std::string s;
    if(isVCMirror_PV(pv.pvType))
    {
        s = vcObject.mirror.pvRoot + pv.pvSuffix;
    }
    else
    {
        s = vcObject.pvRoot + pv.pvSuffix;
    }
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    message("Create channel to ", s);
}
//______________________________________________________________________________
void virtualCathodeInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : vcObject.pvMonStructs)
    {
        continuousMonitorStructs.push_back(new virtualCathodeStructs::monitorStruct());
        continuousMonitorStructs.back() -> monType    = it.first;
        continuousMonitorStructs.back() -> object = &vcObject;
        continuousMonitorStructs.back() -> interface  = this;
        ca_create_subscription(it.second.CHTYPE,
                               it.second.COUNT,
                               it.second.CHID,
                               it.second.MASK,
                               virtualCathodeInterface::staticEntryMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back() -> EVID);

        message(ENUM_TO_STRING(it.first)," it.second.CHTYPE, ", it.second.CHTYPE);
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
                                          virtualCathodeStructs::VC_PV_TYPE pv)
{
    switch(pv)
    {
        case virtualCathodeStructs::VC_PV_TYPE::X_RBV:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::Y_RBV:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::SIGMA_X_RBV:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::SIGMA_Y_RBV:

            break;

        case virtualCathodeStructs::VC_PV_TYPE::COV_XY_RBV:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::Y_PIX:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::X_PIX:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::SIGMA_X_PIX:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::SIGMA_Y_PIX:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::COV_XY_PIX:

            break;

        case virtualCathodeStructs::VC_PV_TYPE::VC_INTENSITY:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::H_POS:
            vcObject.mirror.hPos = getDBRdouble(args);
            break;
        case virtualCathodeStructs::VC_PV_TYPE::V_POS:
            vcObject.mirror.vPos = getDBRdouble(args);
            break;
        case virtualCathodeStructs::VC_PV_TYPE::H_STEP:
            vcObject.mirror.hStep = getDBRdouble(args);
            break;
        case virtualCathodeStructs::VC_PV_TYPE::V_STEP:
            vcObject.mirror.vStep = getDBRdouble(args);
            break;
        case virtualCathodeStructs::VC_PV_TYPE::H_MOVE:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::V_MOVE:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::H_STOP:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::V_STOP:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::POS_UPDATE:

            break;
        case virtualCathodeStructs::VC_PV_TYPE::PIXEL_RESULTS:
            updatePixelResults(args);
            break;

        default:
            message("ERROR nkown PV passed to virtualCathodeInterface::updateValue, ",pv);

    }
}
//____________________________________________________________________________________________
void virtualCathodeInterface::updatePixelResults(const event_handler_args& args)
{
    const double * pValue;
    /* if time _type get time and set where pValue points to */
    if(isTimeType(args.type))
    {
        const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
        pValue = &p->value;
        //updateTime(trace.etime, trace.time, trace.timeStr);

        /* for hints look in epicsTime.h */
        char timeString[UTL::BUFFER_36];
        epicsTimeToStrftime(timeString, sizeof(timeString),
                            "%a %b %d %Y %H:%M:%S.%f", &p->stamp);
        /*
            prove it works
            std::cout <<std::setprecision(15) <<std::showpoint<<  val <<std::endl;
        */
        vcObject.pix_values_time = timeString;
        std::copy(pValue, pValue + vcObject.pix_values.size(), vcObject.pix_values.begin());

    }
    else /* set where pValue points to */
    {
        pValue = (dbr_double_t*)args.dbr;
    }

    /*
        Now we have the neew values, update Buffers
    */
    updateAnalysisBuffers();
}
//____________________________________________________________________________________________
void virtualCathodeInterface::updateAnalysisBuffers()
{

}
//____________________________________________________________________________________________
void virtualCathodeInterface::setBufferSize(const size_t s)
{
    vcObject.buffer_size = s;
}
//____________________________________________________________________________________________
size_t virtualCathodeInterface::getBufferSize(size_t s)
{
    return vcObject.buffer_size;
}
//____________________________________________________________________________________________
size_t virtualCathodeInterface::clearBuffer()
{
    return vcObject.buffer_size;
}



//    for(auto&& it:vcObject.pixel_values_pos)
//    {
//        vcObject.pixel_values.at(it.second) = (double)vcObject.pix_values_epics[it.first];
//        message(it.second, " = ", vcObject.pixel_values.at(it.second));
//    }
//#ifdef BUILD_DLL
//    vcObject.pixel_values_dict = toPythonDict(vcObject.pixel_values);
//#endif // BUILD_DLL
//}
//____________________________________________________________________________________________
double virtualCathodeInterface::getHstep() const
{
    return vcObject.mirror.hStep;
}
//____________________________________________________________________________________________
double virtualCathodeInterface::getVstep() const
{
    return vcObject.mirror.vStep;
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::setHstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            vcObject.pvMonStructs.at(virtualCathodeStructs::VC_PV_TYPE::H_STEP),value);
    }
    vcObject.mirror.hStep = value;
    return s;
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::setVstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            vcObject.pvMonStructs.at(virtualCathodeStructs::VC_PV_TYPE::V_STEP),value);
    }
    vcObject.mirror.vStep = value;
    return s;
}
//____________________________________________________________________________________________
double virtualCathodeInterface::getHpos() const
{
    return vcObject.mirror.hPos;
}
//____________________________________________________________________________________________
double virtualCathodeInterface::getVpos() const
{
    return vcObject.mirror.vPos;
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::setHpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            vcObject.pvMonStructs.at(virtualCathodeStructs::VC_PV_TYPE::H_POS),value);
        return s;
    }
    //currently can't do ths on Real Machine
    return false;
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::setVpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            vcObject.pvMonStructs.at(virtualCathodeStructs::VC_PV_TYPE::V_POS),value);
        return s;
    }
    //currently can't do ths on Real Machine
    return false;
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::setValue(virtualCathodeStructs::pvStruct& pvs,const double value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE, pvs.CHID, &value);
    std::stringstream ss;
    ss << "Timeout setting vcObject.mirror," << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    int status = sendToEpics("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::moveH()
{
    virtualCathodeStructs::pvStruct& pvs = vcObject.pvComStructs.at(virtualCathodeStructs::VC_PV_TYPE::H_MOVE);
    return move(pvs.CHTYPE,pvs.CHID,
                  "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO MOVE H",
                  "!!TIMEOUT!! FAILED TO SEND MOVE H");
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::moveV()
{
    virtualCathodeStructs::pvStruct& pvs = vcObject.pvComStructs.at(virtualCathodeStructs::VC_PV_TYPE::V_MOVE);
    return move(pvs.CHTYPE,pvs.CHID,
                  "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO MOVE V",
                  "!!TIMEOUT!! FAILED TO SEND MOVE V");
}
//____________________________________________________________________________________________
bool virtualCathodeInterface::move(chtype& cht, chid& chi,const char* m1,const char* m2)
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
const virtualCathodeStructs::pilMirrorObject& virtualCathodeInterface::getpilMirrorObjConstRef() const
{
    return vcObject.mirror;
}
//______________________________________________________________________________
bool virtualCathodeInterface::isVCMirror_PV(const virtualCathodeStructs::VC_PV_TYPE& pv)const
{
    using namespace virtualCathodeStructs;
    switch(pv)
    {
        case VC_PV_TYPE::H_POS:
            return true;
        case VC_PV_TYPE::V_POS:
            return true;
        case VC_PV_TYPE::H_STEP:
            return true;
        case VC_PV_TYPE::V_STEP:
            return true;
        case VC_PV_TYPE::H_MOVE:
            return true;
        case VC_PV_TYPE::V_MOVE:
            return true;
        case VC_PV_TYPE::H_STOP:
            return true;
        case VC_PV_TYPE::V_STOP:
            return true;
        case VC_PV_TYPE::POS_UPDATE:
            return true;
        case VC_PV_TYPE::H_MREL:
            return true;
        case VC_PV_TYPE::V_MREL:
            return true;
        case VC_PV_TYPE::UNKNOWN:
            message("isVCMirror_PV passed UNKNOWN pv");
            break;
        default:
            return false;
    }
    return false;
}






//____________________________________________________________________________________________
void virtualCathodeInterface::addToBuffer()
{
//    vcObject.image_data_buffer.push_back(virtualCathodeStructs::static_blank_image_data::create_blank_image_data());
//    if(vcObject.image_data_buffer.size() > vcObject.buffer_size)
//    {
//        vcObject.image_data_buffer.pop_front();
//    }
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
