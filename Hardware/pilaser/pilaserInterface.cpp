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
                                   const std::string& vcDataConfig,
                                   const std::string& vcMirrorConfig,
                                   const std::string& pilaserConfig
                                  ):
configReader(pilaserConfig,  show_messages, show_debug_messages,startVirtualMachine),
vcConfigReader(vcMirrorConfig, vcDataConfig, show_messages, show_debug_messages,startVirtualMachine),
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
        debugMessage(ms->object->name, " ",
                     ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR pilaserInterface: in killMonitor: "
                     "ca_clear_subscription failed for ",
                     ms->object->name, " ",
                     ENUM_TO_STRING(ms->monType));
}
//______________________________________________________________________________
void pilaserInterface::initialise()
{
    std::cout << "pilaserInterface::initialise()" << std::endl;
    /* The config file reader */
    configFileRead = configReader.readConfig();
    if(configFileRead)
        configFileRead = vcConfigReader.readConfig();

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
    bool gotobj = configReader.getpilaserObject(pilaser);
    if(gotobj)
        gotobj = vcConfigReader.getVirtualCathodeObject(pilaser.vcData);
    return gotobj;
}
//______________________________________________________________________________
void pilaserInterface::initChids()
{
    message("\n", "Searching for PILaser chids...");
    /*
        This bit is shoe-horning a previous
        implementation in with minimum fuss :(
    */
    std::string s;
    // init chids
    for(auto&& it:pilaser.pvMonStructs)
    {
        addChannel(pilaser.pvRoot, it.second);
    }
    for(auto&& it:pilaser.pvComStructs)
    {
        addChannel(pilaser.pvRoot, it.second);
    }
    for(auto&& it:pilaser.vcData.pvMonStructs)
    {
        if(isVCMirror_PV(it.first))
        {
            s = pilaser.vcData.mirror.pvRoot;
        }
        else
        {
            s = pilaser.vcData.pvRoot;
        }
        addChannel(s, it.second);
    }
    for(auto&& it:pilaser.vcData.pvComStructs)
    {
        if(isVCMirror_PV(it.first))
        {
            s = pilaser.vcData.mirror.pvRoot;
        }
        else
        {
            s = pilaser.vcData.pvRoot;
        }
        addChannel(s, it.second);
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
        addToContinuousMonitorStructs(it.first,it.second);
    }
    for(auto&& it:pilaser.pvComStructs)
    {
        addToContinuousMonitorStructs(it.first,it.second);
    }
//    for(auto&& it:pilaser.vcData.mirror.pvComStructs)
//    {
//        addToContinuousMonitorStructs(it.first,it.second);
//    }
//    for(auto&& it:pilaser.vcData.mirror.pvMonStructs)
//    {
//        addToContinuousMonitorStructs(it.first,it.second);
//    }
    for(auto&& it:pilaser.vcData.pvComStructs)
    {
        addToContinuousMonitorStructs(it.first,it.second);
    }
    for(auto&& it:pilaser.vcData.pvMonStructs)
    {
        addToContinuousMonitorStructs(it.first,it.second);
    }

    int status = sendToEpics("ca_create_subscription", "Succesfully Subscribed to PILaser Monitors", "!!TIMEOUT!! Subscription to PILaser monitors failed" );
    if (status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//____________________________________________________________________________________________
void pilaserInterface::addToContinuousMonitorStructs(const pilaserStructs::PILASER_PV_TYPE pv, const pilaserStructs::pvStruct& st)
{
    continuousMonitorStructs.push_back(new pilaserStructs::monitorStruct());
    continuousMonitorStructs.back()->monType = pv;
    continuousMonitorStructs.back()->object  = &pilaser;
    continuousMonitorStructs.back()->interface  = this;
    ca_create_subscription(st.CHTYPE,
                           st.COUNT,
                           st.CHID,
                           st.MASK,
                           pilaserInterface::staticEntryMonitor,
                           (void*)continuousMonitorStructs.back(),
                           &continuousMonitorStructs.back() -> EVID);
}
//____________________________________________________________________________________________
void pilaserInterface::staticEntryMonitor(const event_handler_args args)
{
    using namespace pilaserStructs;
    monitorStruct*ms = static_cast<monitorStruct *>(args.usr);
    ms->interface->updateValue(args,ms -> monType);
}
//____________________________________________________________________________________________
void pilaserInterface::updateValue(const event_handler_args args,pilaserStructs::PILASER_PV_TYPE pv)
{
    using namespace pilaserStructs;
    switch(pv)
    {
    case PILASER_PV_TYPE::INTENSITY:
            pilaser.intensity = getDBRdouble(args);
            break;
        case PILASER_PV_TYPE::STABILISATION:
            pilaser.stabilisation_status = HWC_ENUM::STATE::ERR;
            break;
        case PILASER_PV_TYPE::STATUS:
            break;
        case PILASER_PV_TYPE::HALF_WAVE_PLATE_SET:
            break;
        case PILASER_PV_TYPE::HALF_WAVE_PLATE_READ:
            pilaser.HWP = getDBRdouble(args);
            break;
        case PILASER_PV_TYPE::X_RBV:
            pilaser.vcData.x = getDBRdouble(args);
            addToBuffer(pilaser.vcData.x,pilaser.vcData.x_buf);
            break;
        case PILASER_PV_TYPE::Y_RBV:
            pilaser.vcData.y = getDBRdouble(args);
            addToBuffer(pilaser.vcData.y,pilaser.vcData.y_buf);
            break;
        case PILASER_PV_TYPE::SIGMA_X_RBV:
            pilaser.vcData.sig_x = getDBRdouble(args);
            addToBuffer(pilaser.vcData.sig_x,pilaser.vcData.sig_x_buf);
            break;
        case PILASER_PV_TYPE::SIGMA_Y_RBV:
            pilaser.vcData.sig_y = getDBRdouble(args);
            addToBuffer(pilaser.vcData.sig_y,pilaser.vcData.sig_y_buf);
            break;
        case PILASER_PV_TYPE::COV_XY_RBV:
            pilaser.vcData.sig_xy = getDBRdouble(args);
            addToBuffer(pilaser.vcData.sig_xy,pilaser.vcData.sig_xy_buf);
            break;
        case PILASER_PV_TYPE::X_PIX:
            pilaser.vcData.x_pix = getDBRdouble(args);
            addToBuffer(pilaser.vcData.x_pix,pilaser.vcData.x_pix_buf);
            break;
        case PILASER_PV_TYPE::Y_PIX:
            pilaser.vcData.y_pix = getDBRdouble(args);
            addToBuffer(pilaser.vcData.y_pix,pilaser.vcData.y_pix_buf);
            break;
        case PILASER_PV_TYPE::SIGMA_X_PIX:
            pilaser.vcData.sig_x_pix = getDBRdouble(args);
            addToBuffer(pilaser.vcData.sig_x_pix,pilaser.vcData.sig_x_pix_buf);
            break;
        case PILASER_PV_TYPE::SIGMA_Y_PIX:
            pilaser.vcData.sig_y_pix = getDBRdouble(args);
            addToBuffer(pilaser.vcData.sig_y_pix,pilaser.vcData.sig_y_pix_buf);
            break;
        case PILASER_PV_TYPE::COV_XY_PIX:
            pilaser.vcData.sig_xy_pix = getDBRdouble(args);
            addToBuffer(pilaser.vcData.sig_xy_pix,pilaser.vcData.sig_xy_pix_buf);
            break;
        case PILASER_PV_TYPE::VC_INTENSITY:
            break;
        case PILASER_PV_TYPE::PIXEL_RESULTS:
            updatePixelResults(args);
            break;
        case PILASER_PV_TYPE::H_POS:
            pilaser.vcData.mirror.hPos = getDBRdouble(args);
            break;
        case PILASER_PV_TYPE::V_POS:
            pilaser.vcData.mirror.vPos = getDBRdouble(args);
            break;
        case PILASER_PV_TYPE::H_MREL:
            break;
        case PILASER_PV_TYPE::V_MREL:
            break;
        case PILASER_PV_TYPE::H_STEP:
            pilaser.vcData.mirror.hStep = getDBRdouble(args);
            break;
        case PILASER_PV_TYPE::V_STEP:
            pilaser.vcData.mirror.vStep = getDBRdouble(args);
            break;
        case PILASER_PV_TYPE::H_MOVE:
            break;
        case PILASER_PV_TYPE::V_MOVE:
            break;
        case PILASER_PV_TYPE::H_STOP:
            break;
        case PILASER_PV_TYPE::V_STOP:
            break;
        case PILASER_PV_TYPE::POS_UPDATE:
            break;
        default:
            message("ERROR nkown PV passed to pilaserInterface::updateValue, ",pv);
            break;
    }
}
//____________________________________________________________________________________________
void pilaserInterface::updatePixelResults(const event_handler_args& args)
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
        pilaser.vcData.pix_values_time = timeString;
        std::copy(pValue, pValue + pilaser.vcData.pix_values.size(), pilaser.vcData.pix_values.begin());

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
void pilaserInterface::addToBuffer(const double val,std::deque<double>& buffer)
{
    buffer.push_back(val);
    if(buffer.size() > pilaser.vcData.buffer_size )
    {
        buffer.pop_front();
    }
}
//____________________________________________________________________________________________
void pilaserInterface::updateAnalysisBuffers()
{

}
//____________________________________________________________________________________________
void pilaserInterface::setBufferSize(const size_t s)
{
    pilaser.vcData.buffer_size = s;
}
//____________________________________________________________________________________________
size_t pilaserInterface::getBufferSize(size_t s)
{
    return pilaser.vcData.buffer_size;
}
//____________________________________________________________________________________________
void pilaserInterface::clearBuffer()
{
    pilaser.vcData.x_buf.clear();
    pilaser.vcData.y_buf.clear();
    pilaser.vcData.sig_x_buf.clear();
    pilaser.vcData.sig_y_buf.clear();
    pilaser.vcData.sig_xy_buf.clear();
    pilaser.vcData.x_pix_buf.clear();
    pilaser.vcData.y_pix_buf.clear();
    pilaser.vcData.sig_x_pix_buf.clear();
    pilaser.vcData.sig_y_pix_buf.clear();
    pilaser.vcData.sig_xy_pix_buf.clear();
    pilaser.vcData.pix_values_buffer.clear();
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getXBuffer()const
{
    return pilaser.vcData.x_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getYBuffer()const
{
    return pilaser.vcData.y_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getSigXBuffer()const
{
    return pilaser.vcData.sig_x_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getSigYBuffer()const
{
    return pilaser.vcData.sig_y_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getSigXYBuffer()const
{
    return pilaser.vcData.sig_xy_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getXPixBuffer()const
{
    return pilaser.vcData.x_pix_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getYPixBuffer()const
{
    return pilaser.vcData.y_pix_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getSigXPixBuffer()const
{
    return pilaser.vcData.sig_x_pix_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getSigYPixBuffer()const
{
    return pilaser.vcData.sig_y_pix_buf;
}
//____________________________________________________________________________________________
std::deque<double> pilaserInterface::getSigXYPixBuffer()const
{
    return pilaser.vcData.sig_xy_pix_buf;
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
const pilaserStructs::pilMirrorObject& pilaserInterface::getpilMirrorObjConstRef() const
{
    return pilaser.vcData.mirror;
}
//____________________________________________________________________________________________
const pilaserStructs::virtualCathodeDataObject& pilaserInterface::getVCDataObjConstRef() const
{
    return pilaser.vcData;
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
//______________________________________________________________________________
bool pilaserInterface::isVCMirror_PV(const pilaserStructs::PILASER_PV_TYPE& pv)const
{
    using namespace pilaserStructs;
    switch(pv)
    {
        case PILASER_PV_TYPE::H_POS:
            return true;
        case PILASER_PV_TYPE::V_POS:
            return true;
        case PILASER_PV_TYPE::H_STEP:
            return true;
        case PILASER_PV_TYPE::V_STEP:
            return true;
        case PILASER_PV_TYPE::H_MOVE:
            return true;
        case PILASER_PV_TYPE::V_MOVE:
            return true;
        case PILASER_PV_TYPE::H_STOP:
            return true;
        case PILASER_PV_TYPE::V_STOP:
            return true;
        case PILASER_PV_TYPE::POS_UPDATE:
            return true;
        case PILASER_PV_TYPE::H_MREL:
            return true;
        case PILASER_PV_TYPE::V_MREL:
            return true;
        case PILASER_PV_TYPE::UNKNOWN:
            message("isVCMirror_PV passed UNKNOWN pv");
            break;
        default:
            return false;
    }
    return false;
}
//____________________________________________________________________________________________
double pilaserInterface::getHstep() const
{
    return pilaser.vcData.mirror.hStep;
}
//____________________________________________________________________________________________
double pilaserInterface::getVstep() const
{
    return pilaser.vcData.mirror.vStep;
}
//____________________________________________________________________________________________
bool pilaserInterface::setHstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_STEP),value);
    }
    pilaser.vcData.mirror.hStep = value;
    return s;
}
//____________________________________________________________________________________________
bool pilaserInterface::setVstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_STEP),value);
    }
    pilaser.vcData.mirror.vStep = value;
    return s;
}
//____________________________________________________________________________________________
double pilaserInterface::getHpos() const
{
    return pilaser.vcData.mirror.hPos;
}
//____________________________________________________________________________________________
double pilaserInterface::getVpos() const
{
    return pilaser.vcData.mirror.vPos;
}
//____________________________________________________________________________________________
bool pilaserInterface::setHpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_POS),value);
        return s;
    }
}
//____________________________________________________________________________________________
bool pilaserInterface::setVpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_POS),value);
        return s;
    }
}
//____________________________________________________________________________________________
bool pilaserInterface::moveH()
{
    pilaserStructs::pvStruct& pvs = pilaser.vcData.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::H_MOVE);
    return move(pvs.CHTYPE,pvs.CHID,
                  "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO MOVE H",
                  "!!TIMEOUT!! FAILED TO SEND MOVE H");
}
//____________________________________________________________________________________________
bool pilaserInterface::moveV()
{
    pilaserStructs::pvStruct& pvs = pilaser.vcData.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::V_MOVE);
    return move(pvs.CHTYPE,pvs.CHID,
                  "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO MOVE V",
                  "!!TIMEOUT!! FAILED TO SEND MOVE V");
}
//____________________________________________________________________________________________
bool pilaserInterface::move(chtype& cht, chid& chi,const char* m1,const char* m2)
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


//____________________________________________________________________________________________
//double pilaserInterface::getVstep() const
//{
//    return pilaser.vcData.mirror.vStep;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setHstep(const double value)
//{
//    bool s = true;
//    if(startVirtualMachine)
//    {
//        bool s = setValue(
//            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_STEP),value);
//    }
//    pilaser.vcData.mirror.hStep = value;
//    return s;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setVstep(const double value)
//{
//    bool s = true;
//    if(startVirtualMachine)
//    {
//        bool s = setValue(
//            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_STEP),value);
//    }
//    pilaser.vcData.mirror.vStep = value;
//    return s;
//}
////____________________________________________________________________________________________
//double pilaserInterface::getHpos() const
//{
//    return pilaser.vcData.mirror.hPos;
//}
////____________________________________________________________________________________________
//double pilaserInterface::getVpos() const
//{
//    return pilaser.vcData.mirror.vPos;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setHpos(const double value)
//{
//    bool s = true;
//    if(startVirtualMachine)
//    {
//        bool s = setValue(
//            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_POS),value);
//        return s;
//    }
//    //currently can't do ths on Real Machine
//    return false;
//}
////____________________________________________________________________________________________
