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
//  Last edit:   13-04-2018
//  FileName:    liberallrfInterface.cpp
//  Description:
//
//
//
//*/
#include "liberallrfInterface.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <math.h>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
liberallrfInterface::liberallrfInterface(const std::string &llrfConf,
                              const bool startVirtualMachine,
                              const bool& show_messages,
                              const bool& show_debug_messages_ptr,
                              const bool shouldStartEPICs ,
                              const llrfStructs::LLRF_TYPE type
                             ):
configReader(llrfConf,startVirtualMachine,show_messages,show_debug_messages_ptr),
interface(show_messages,show_debug_messages_ptr),
shouldStartEPICs(shouldStartEPICs),
usingVirtualMachine(startVirtualMachine),
myLLRFType(type),
first_pulse(true),
initial_pulsecount(UTL::ZERO_SIZET),
evid_id(UTL::ZERO_SIZET),
evid_ID_SET(false),
dummy_trace_data(llrfStructs::rf_trace_data())
{
    /* set the llrf type of the config reader */
    configReader.setType(type);

    initialise();
}
//
// ________                   __
// \______ \   ____   _______/  |________  ____ ___.__.
//  |    |  \_/ __ \ /  ___/\   __\_  __ \/  _ <   |  |
//  |    `   \  ___/ \___ \  |  |  |  | \( <_>)___  |
// /_______  /\___  >____  > |__|  |__|   \____// ____|
//        \/     \/     \/                     \/
//______________________________________________________________________________
liberallrfInterface::~liberallrfInterface()
{
    killILockMonitors();
    for(auto && it : continuousMonitorStructs)
    {
        killMonitor(it);
        delete it;
    }
    /* kill threads safely */
    for(auto && it = setAmpHP_Threads.cbegin(); it != setAmpHP_Threads.cend() /* not hoisted */; /* no increment */)
    {
        while(!(it->can_kill))
        {

        }
        /// join before deleting...
        /// http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
        it->thread->join();
        delete it->thread;
        setAmpHP_Threads.erase(it++);
    }
//    debugMessage("liberallrfInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void liberallrfInterface::killMonitor(llrfStructs::monitorStruct * ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
        delete ms;
}
//______________________________________________________________________________
//
//
// .___       .__  __  .__       .__  .__
// |   | ____ |__|/  |_|__|____  |  | |__|_______ ____
// |   |/    \|  \   __\  \__  \ |  | |  \___   // __ \
// |   |   |  \  ||  | |  |/ __ \|  |_|  |/    /\  ___/
// |___|___|  /__||__| |__(____  /____/__/_____ \\___  >
//          \/                 \/              \/    \/
//
// INIT FOLLOWS THE STANDARD DESIGN PATTERN SO IS NOT WELL COMMENTED
// TRACE MONITORS MUST BE STARTED MANUALY AFTER INIT
//______________________________________________________________________________
void liberallrfInterface::initialise()
{
    /* The config file reader read file */
    configFileRead = configReader.readConfig();
    /* why is this here?? */
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // MAGIC_NUMBER
    if(configFileRead)
    {
        message("The liberallrfInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            message("LLRF objects acquired");
            if(shouldStartEPICs)
            {
                message("The liberallrfInterface has acquired objects, connecting to EPICS");
                // subscribe to the channel ids
                initChids();
                // start the monitors: set up the callback functions
                debugMessage("Starting Monitors");
                startMonitors();
                // The pause allows EPICS to catch up.
                standard_pause();
            }
            else
            {
                message("The liberallrfInterface has acquired objects, NOT connecting to EPICS");
            }
        }
        else
        {
            message("!!!The liberallrfInterface received an Error while getting llrf data!!!");
        }
    }
//    debugMessage("liberallrfInterface::initialise() FIN");
}
//______________________________________________________________________________
bool liberallrfInterface::initObjects()
{
    /* get the llrf objects, based on config file */
    bool success = configReader.getliberallrfObject(llrf);
    /* set the TRACE sizes to defaults etc... */
    if(success)
    {
        for(auto&& it:llrf.pvMonStructs)
        {
            if(Is_TracePV(it.first))
            {
                it.second.name = fullCavityTraceName(it.second.name);
                debugMessage("liberallrfInterface, creating trace_data for, ", it.second.name, ", element count = ",it.second.COUNT);
                if(entryExists(llrf.trace_data,it.second.name))
                {
                    message("!!!!ERROR IN TRACES CONFIG FILE DETECTED!!!!!");
                    message("!!!!ABORTING INSTANTIATION!!!!!");
                    return false;
                }
                llrf.trace_data[it.second.name].trace_size = it.second.COUNT;
                llrf.trace_data.at(it.second.name).name = it.second.name;
                success = setNumBufferTraces(it.second.name, llrf.trace_data.at(it.second.name).buffersize);
                if(!success)
                {
                    message("!!!!ERROR In setNumBufferTraces DETECTED!!!!! liberallrfInterface");
                    return false;
                }
                llrf.traceLength = it.second.COUNT;
            }
            else if(Is_Time_Vector_PV(it.first))
            {
                llrf.time_vector.value.resize(it.second.COUNT);
            }
        }
    }
    llrf.type = myLLRFType;
    return success;
}
//______________________________________________________________________________
void liberallrfInterface::initChids()
{
    message("\n", "liberallrfInterface is searching for LLRF ChIds...");
    for(auto && it : llrf.pvMonStructs)
    {
        addChannel(llrf.pvRoot, it.second);
    }
    // command only PVs for the LLRF to set "high level" phase and amplitide
    for(auto && it : llrf.pvComStructs)
    {
        addChannel(llrf.pvRoot, it.second);
    }
    addILockChannels(llrf.numIlocks, llrf.pvRoot, llrf.name, llrf.iLockPVStructs);
    int status=sendToEpics("ca_create_channel","liberallrfInterface found LLRF ChIds.",
                           "!!TIMEOUT!! Not all LLRF ChIds found, liberallrfInterface.");
    /* if timeout try investigating */
    if(status==ECA_TIMEOUT)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//MAGIC_NUMBER
        message("\n","liberallrfInterface will check the  LLRF ChIds ");
        for(auto && it : llrf.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto && it : llrf.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // MAGIC_NUMBER
    }
    else if (status == ECA_NORMAL)
    {
        allChidsInitialised = true;  // interface base class member
    }
}
//______________________________________________________________________________
void liberallrfInterface::addChannel(const std::string & pvRoot, llrfStructs::pvStruct& pv)
{
    std::string s1;
    // TEMPORARY HACK FOR HIGH LEVEL llrf PARAMATERS
    if(pv.pvType == llrfStructs::LLRF_PV_TYPE::AMP_MVM)
    {
        s1 =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else if(pv.pvType == llrfStructs::LLRF_PV_TYPE::PHI_DEG)
    {
        s1 =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else
    {
        s1 = pvRoot + pv.pvSuffix;
    }
    ca_create_channel(s1.c_str(), 0, 0, 0, &pv.CHID);//MAGIC_NUMBER
    debugMessage("Create channel to ", s1);
}
//______________________________________________________________________________
void liberallrfInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : llrf.pvMonStructs)
    {
        /* on startup we don't monitor trace and EVID PVs */
        if(IsNot_TracePV(it.first) && IsNot_EVID_PV(it.first))
        {
            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
            continuousMonitorStructs.push_back(new llrfStructs::monitorStruct());
            continuousMonitorStructs.back() -> monType   = it.first;
            continuousMonitorStructs.back() -> llrfObj   = &llrf;
            continuousMonitorStructs.back() -> interface = this;
            continuousMonitorStructs.back() -> name      = getLLRFChannelName(it.first);
            ca_create_subscription(it.second.CHTYPE,
                                   it.second.COUNT,
                                   it.second.CHID,
                                   it.second.MASK,
                                   liberallrfInterface::staticEntryLLRFMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back() -> EVID);
        }
    }
    int status = sendToEpics("ca_create_subscription",
                             "liberallrfInterface succesfully subscribed to LLRF monitors.",
                             "!!TIMEOUT!! Subscription to LLRF monitors failed, liberallrfInterface");
    if (status == ECA_NORMAL)
    {
        allMonitorsStarted = true; // interface base class member
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::startTraceMonitoring()
{
    for(auto && it : llrf.pvMonStructs)
    {
        if(Is_TracePV(it.first))
        {
            if(isNotMonitoring(it.first))
            {
                startTraceMonitoring(it.first);
            }
        }
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    if(Is_TracePV(pv))
    {
        if(isNotMonitoring(pv))
        {
            std::string name = getLLRFChannelName(pv);
            //llrf.trace_data[name].traces.resize(llrf.trace_data[name].buffersize);
            //for(auto && it2: llrf.trace_data[name].traces)
            //    it2.value.resize(llrf.pvMonStructs.at(pv).COUNT);

            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(pv), " (",getLLRFChannelName(pv) ,")");
            continuousMonitorStructs.push_back(new llrfStructs::monitorStruct());
            continuousMonitorStructs.back() -> monType   = pv;
            continuousMonitorStructs.back() -> llrfObj   = &llrf;
            continuousMonitorStructs.back() -> interface = this;
            continuousMonitorStructs.back() -> name      = getLLRFChannelName(pv);
            ca_create_subscription(llrf.pvMonStructs.at(pv).CHTYPE,
                               llrf.pvMonStructs.at(pv).COUNT,
                               llrf.pvMonStructs.at(pv).CHID,
                               llrf.pvMonStructs.at(pv).MASK,
                               liberallrfInterface::staticEntryLLRFMonitor,
                               (void*)continuousMonitorStructs.back(),
                               &continuousMonitorStructs.back() -> EVID);
            std::stringstream ss;
            ss <<"liberallrfInterface succesfully subscribed to LLRF trace monitor: " << ENUM_TO_STRING(pv);
            std::string s1 = ss.str();
            ss.str(std::string());
            ss <<"!!TIMEOUT!! Subscription to LLRF Trace monitor " << ENUM_TO_STRING(pv) << " failed";
            std::string s2 = ss.str();

            /* makes sure the evid associated with the trace is also started */
            llrfStructs::LLRF_PV_TYPE EVIDpv = getEVID_pv(pv);
            if( EVIDpv != llrfStructs::LLRF_PV_TYPE::UNKNOWN )
            {
                debugMessage("ca_create_subscription to ", ENUM_TO_STRING(EVIDpv));
                continuousMonitorStructs.push_back(new llrfStructs::monitorStruct());
                continuousMonitorStructs.back() -> monType   = EVIDpv;
                continuousMonitorStructs.back() -> llrfObj   = &llrf;
                continuousMonitorStructs.back() -> interface = this;
                continuousMonitorStructs.back() -> name      = getLLRFChannelName(EVIDpv);
                ca_create_subscription(llrf.pvMonStructs.at(EVIDpv).CHTYPE,
                                       llrf.pvMonStructs.at(EVIDpv).COUNT,
                                       llrf.pvMonStructs.at(EVIDpv).CHID,
                                       llrf.pvMonStructs.at(EVIDpv).MASK,
                                       liberallrfInterface::staticEntryLLRFMonitor,
                                       (void*)continuousMonitorStructs.back(),
                                       &continuousMonitorStructs.back() -> EVID);
            }
            int status = sendToEpics("ca_create_subscription",s1.c_str(),s2.c_str());
            if (status == ECA_NORMAL)
            {
                debugMessage(ENUM_TO_STRING(pv)," Monitor Started");
            }
            else
            {
                killMonitor(continuousMonitorStructs.back());
            }
            return isMonitoring(pv);
        }
    }
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::startTraceMonitoring(const std::string& name)
{
    std::string n = fullCavityTraceName(name);
    debugMessage("STARTING ", n, ", getLLRFPVType(name) = ", getLLRFPVType(n));
    return startTraceMonitoring(getLLRFPVType(n));
}
//
// _________        .__  .__ ___.                  __
// \_   ___ \_____  |  | |  |\_ |__ _____    ____ |  | __
// /    \  \/\__  \ |  | |  | | __ \\a__  \ _/ ___\|  |/ /
// \     \____/ __ \|  |_|  |_| \_\ \/ __ \\  \___|    <
//  \______  (____  /____/____/___  (____  /\___  >__|_ \
//         \/     \/              \/     \/     \/     \/
//
// These functions are called from EPICS subscriptions
//____________________________________________________________________________________________
void liberallrfInterface::staticEntryLLRFMonitor(const event_handler_args args)
{
    /* Make the below neater at some point! */
    llrfStructs::monitorStruct*ms = static_cast<llrfStructs::monitorStruct *>(args.usr);
    /* switch on the PV type, complex updates a ehandled in bespoke functions */

//    ms->interface->    debugMessage("staticEntryLLRFMonitor called ");

    if(ms->interface->Is_TracePV(ms -> monType))
    {   //ms->interface->message("staticEntryLLRFMonitor called with TRACE PV, for ", ms->name);
        ms->interface->updateTrace(args,ms->llrfObj->trace_data.at(ms->name));
    }
    else if(ms->interface->Is_EVID_PV(ms -> monType))
    {
        //ms->interface->message("staticEntryLLRFMonitor called with EVID PV", ms->name);
        ms->interface->updateEVID(args,ms->llrfObj->trace_data.at(ms->name));
    }
    else if(ms->interface->Is_SCAN_PV(ms -> monType))
    {   //ms->interface->message("staticEntryLLRFMonitor called with EVID PV", ms->name);
        ms->interface->updateSCAN(args,ms->llrfObj->trace_data.at(ms->name));
    }
    else
    {   //ms->interface->message("staticEntryLLRFMonitor called with normal PV = ", ENUM_TO_STRING(ms -> monType));
        switch(ms -> monType)
        {
            case llrfStructs::LLRF_PV_TYPE::TRIG_SOURCE:
                ms->interface->updateTrigState(args);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_ILOCK_STATE:
                ms->interface->updateBoolState(args, ms->interface->llrf.interlock_state);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE:
                ms->interface->updateBoolState(args, ms->interface->llrf.ff_amp_lock_state);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE:
                ms->interface->updateBoolState(args, ms->interface->llrf.ff_ph_lock_state);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_RF_OUTPUT:
                ms->interface->updateBoolState(args, ms->interface->llrf.rf_output);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_AMP_FF = ",*(double*)args.dbr);
                ms->llrfObj->amp_ff = *(double*)args.dbr;
                ms->llrfObj->amp_MVM = (ms->llrfObj->amp_ff) * (ms->llrfObj->ampCalibration);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_AMP_SP = ",*(double*)args.dbr);
                ms->llrfObj->amp_sp = *(double*)args.dbr;
                //(ms->llrfObj->amp_sp)
                ms->llrfObj->amp_MVM = (ms->llrfObj->amp_sp) * (ms->llrfObj->ampCalibration);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PHI_FF:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_PHI_FF = ",*(double*)args.dbr);
                ms->llrfObj->phi_ff = *(double*)args.dbr;
                ms->llrfObj->phi_DEG = (ms->llrfObj->phi_ff) * (ms->llrfObj->phiCalibration);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PHI_SP:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_PHI_SP = ",*(double*)args.dbr);
                ms->llrfObj->phi_sp = *(double*)args.dbr;
                ms->llrfObj->phi_DEG = (ms->llrfObj->phi_sp) * (ms->llrfObj->phiCalibration);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_TIME_VECTOR:
                /* the time_vector is simlar to a trace, but we can just update the values straight away */
                ms->interface->updateValues(args,ms->llrfObj->time_vector);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PULSE_LENGTH:
                ms->llrfObj->pulse_length = *(double*)args.dbr;
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PULSE_OFFSET:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_PULSE_OFFSET = ",*(double*)args.dbr);
                ms->llrfObj->pulse_offset = *(double*)args.dbr;
                break;
            default:
                ms->interface->debugMessage("ERROR staticEntryLLRFMonitor passed Unknown PV Type");
                // ERROR
        }
    }
//        ms->interface->    debugMessage("staticEntryLLRFMonitor FIN ");

}
//____________________________________________________________________________________________
void liberallrfInterface::updateTrace(const event_handler_args& args, llrfStructs::rf_trace_data& trace)
{   /* called from staticEntryLLRFMonitor, and will update a power or a phase trace */
    /* ref to trace to update */
    auto& current_trace = trace.traces[trace.current_trace];

    /* update the new trace values */
    updateValues(args, current_trace);

    /* find the maximum value in the trace */
    trace.latest_max = *std::max_element(current_trace.value.begin(), current_trace.value.end());

    /* if Klystron Forward Power, update can_increase_active_pulses, based on max value */
    if(trace.name == UTL::KLYSTRON_FORWARD_POWER)
    {
        updateCanIncreaseActivePulses(trace.latest_max);
    }
    /* on detecting an outside-mask-trace we might be adding 'future traces'
       the place to add them, and how many to add are conatined in a vector of pairs:
        outside_mask_data_index_to_add_future_traces_to
       the first  number is the index  of outside_mask_traces to add data to
       the second number is the number of traces STILL to add
       when the second number gets to zero we shoudl delete the entry, as all data has been collected */
    for(auto && it = trace.outside_mask_data_index_to_add_future_traces_to.begin();
                it != trace.outside_mask_data_index_to_add_future_traces_to.end() /* not hoisted */; /* no increment */)
    {
        llrf.outside_mask_traces[it->first].traces.push_back(current_trace);
        if(llrf.outside_mask_traces[it->first].traces.size() == llrf.outside_mask_traces[it->first].num_traces_to_collect)
        {
            llrf.outside_mask_traces[it->first].is_collecting = false;
        }
        it->second -= UTL::ONE_SIZET;
        if(it->second == UTL::ZERO_SIZET)
        {
            trace.outside_mask_data_index_to_add_future_traces_to.erase(it);
        }
        else
        {
            ++it;
        }
    }

    /* check masks */
    if(shouldCheckMasks(trace))
    {   //debugMessage("CHECKING MASKS ",trace.name);
        int trace_in_mask_result = updateIsTraceInMask(trace);
        handleTraceInMaskResult(trace, trace_in_mask_result);
    }

    /* calc means */
    updateTraceCutMean(trace, current_trace);

    /* the trace index tells us which part of 'traces' is the next to update
       it circles 'round
       first,set the ltest trace index, used for accesing latest data */
    trace.latest_trace_index = trace.current_trace;
    /* then update all indices */
    updateTraceIndex(trace.current_trace, trace.traces.size());
    updateTraceIndex(trace.previous_trace, trace.traces.size());
    updateTraceIndex(trace.previous_previous_trace, trace.traces.size());
    /* update shot count
       the difference between this and pulse count indicates how many traces we miss! */
    trace.shot += UTL::ONE_SIZET;
    //message("trace.shot = ", trace.shot);
}
//____________________________________________________________________________________________
void liberallrfInterface::updateValues(const event_handler_args& args, llrfStructs::rf_trace& trace)
{
    /* this function actually gets the new values from EPICS and adds them to the trace.value vector
       all LLRF traces should be updated using this function

    /* pointer to array we are expecting depenss on type channel */
    const dbr_double_t * pValue;
    /* if time _type get time and set where pValue points to */
    if(isTimeType(args.type))
    {
        const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
        pValue = &p->value;
        trace.etime = p->stamp;
        updateTime(trace.etime, trace.time, trace.timeStr);
    }
    else /* set where pValue points to */
    {
        pValue = (dbr_double_t*)args.dbr;
    }
    /* copy array to trace, assumes array is of correct size! */
    std::copy(pValue, pValue+trace.value.size(), trace.value.begin());

    if(stringIsSubString(trace.name,UTL::PHASE))
    {
        //https://stackoverflow.com/questions/21452217/increment-all-c-stdvector-values-by-a-constant-value
        //message(trace.name," is a phase trace");
        std::transform(std::begin(trace.value),std::end(trace.value),std::begin(trace.value),[](double x){return x+UTL::ONEEIGHTY_ZERO_DOUBLE;});//MAGIC_NUMBER
    }
    else
    {
        //message(trace.name," is not a phase trace");
    }

}
////____________________________________________________________________________________________
//bool liberallrfInterface::isPhaseTrace(const std::string& name)
//{
//    if(name.find("PHASE") != std::string::npos)//MAGIC_STRING
//        return true;
//    else
//        return false;
//}
//____________________________________________________________________________________________
int liberallrfInterface::updateIsTraceInMask(llrfStructs::rf_trace_data& trace)
{
    // is trace in masks is only checked if we are increasing the active pulses!!!
    /* only check active pulses, */
    /// this may be causing a bug: the klystron could be giving power but the traces from
    /// before the klystron became active are still being processed
    if(llrf.can_increase_active_pulses)// only check active pulses
    {
        /* to fail, you must get consecutive points outside the mask */
        size_t hi_breakdown_count = UTL::ZERO_INT;
        size_t lo_breakdown_count = UTL::ZERO_INT;
        /* ref for ease of reading */
        auto & to_check = trace.traces[trace.current_trace].value;
        auto & hi = trace.high_mask;
        auto & lo = trace.low_mask;
        /* main loop iterate over the trace values  */
        for(auto i = 0; i < to_check.size(); ++i)
        {
            /* if we are above the mask floor */
            if(to_check[i] > trace.mask_floor)
            {
                /* if we are above the mask increase hi_breakdown_count */
                if(to_check[i] > hi[i])
                {
                    hi_breakdown_count += 1;
                    /* if we have too many consecutive hi_breakdown_count trace fails */
                    if(hi_breakdown_count == trace.num_continuous_outside_mask_count)
                    {
                        /* write a message*/
                        outside_mask_trace_message << trace.name << " HI MASK FAIL: current average = "
                        << trace.rolling_average[i] << ", " << to_check[i] << " > "
                        << hi[i] << " at i = " << i << " us = " << llrf.time_vector.value[i]
                        << ", EVID " << trace.traces[trace.evid_current_trace].EVID << ", previous EVID = "
                        <<trace.traces[trace.previous_evid_trace].EVID;
                        message(outside_mask_trace_message.str());
                        trace.outside_mask_index = i;
                        /* return code 0 = FAIL */
                        return UTL::ZERO_INT;
                    }
                }
                else
                {
                    /* if the value is good reset hi_breakdown_count to zero */
                    hi_breakdown_count = UTL::ZERO_INT;
                }
                /* if we are above the mask increase lo_breakdown_count */
                if(to_check[i] < lo[i])
                {
                    lo_breakdown_count += UTL::ONE_INT;
                    /* if we have too many consecutive lo_breakdown_count trace fails */
                    if(lo_breakdown_count == trace.num_continuous_outside_mask_count)
                    {
                        /* write a message*/
                        outside_mask_trace_message << trace.name << " LO MASK FAIL: current average = "
                        << trace.rolling_average[i] << ", " << to_check[i] << " < "
                        << lo[i] << " at i = " << i << " us = " << llrf.time_vector.value[i]
                        << ", EVID " << trace.traces[trace.evid_current_trace].EVID << ", previous EVID = "
                        <<trace.traces[trace.previous_evid_trace].EVID;
                        message(outside_mask_trace_message.str());
                        trace.outside_mask_index = i;
                        /* return code 0 = FAIL */
                        return UTL::ZERO_INT;
                    }
                }
                else
                {
                    /* if the value is good reset lo_breakdown_count to zero */
                    lo_breakdown_count = UTL::ZERO_INT;
                }

            }//if(to_check[i] > trace.mask_floor)

        }//for(auto i = 0; i < to_check.size(); ++i)
        /* return code 1 = PASS */
        return UTL::ONE_INT;

    }//if(llrf.can_increase_active_pulses)
    /* return code -1 = couldn't check mask, pulses not active */
    return UTL::MINUS_ONE_INT;
}
//____________________________________________________________________________________________
void liberallrfInterface::handleTraceInMaskResult(llrfStructs::rf_trace_data& trace, int result)
{
    switch(result)
    {
        case UTL::ONE_INT: /* passed mask */
            handlePassedMask(trace);
            break;
        case UTL::ZERO_INT: /* failed mask */
            handleFailedMask(trace);
            break;
        case UTL::MINUS_ONE_INT:
            /* not checking masks or no active pulses */
            break;
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::handlePassedMask(llrfStructs::rf_trace_data& trace)
{
    if(trace.keep_rolling_average)
    {
        calcRollingAverage(trace);
    }
    if(trace.endInfiniteMask_Trace_Set)
    {
        // this should NOT move the end past the set limit ???
        // or have this as another option??
        auto& trace_bound   = trace.endMaskTrace_Bound;
        auto& power_trace_to_search = llrf.trace_data.at(trace_bound.first);
        auto& value_to_search  = power_trace_to_search.traces[power_trace_to_search.current_trace].value;

        //message(trace.name," looking for ", trace_bound.second, " in ", power_trace_to_search.name, " max = ",*std::max_element(value_to_search.begin(), value_to_search.end()));

        auto i = value_to_search.size()-1;
        for(; i>0 ;--i)
        {
            if(value_to_search[i] > trace_bound.second)
            {
                //auto idx = std::distance(begin(current_trace), bound.base()) - 1;
                //message("idx (us) == ", getTime(i));
                setMaskInfiniteEnd(trace.name, i);
                break;
            }
        }
//        auto bound = std::lower_bound(current_trace.rbegin(), current_trace.rend(), trace_bound.second);
//        if(bound != current_trace.rend())
//        {
//            auto idx = std::distance(begin(current_trace), bound.base()) - 1;
//            message("idx (us) == ", getTime(idx));
//            setMaskInfiniteEnd(trace.name, idx);
//        }
//        else
//        {
//            message("couldn't find ", trace_bound.second);
//        }
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::handleFailedMask(llrfStructs::rf_trace_data& trace)
{
    addToOutsideMaskTraces(trace, trace.name);
}
//____________________________________________________________________________________________
void liberallrfInterface::addToOutsideMaskTraces(llrfStructs::rf_trace_data& trace,const std::string& name)
{
    if(trace.drop_amp_on_breakdown)
    {
        /* stop checking masks */
        //message("setGlobalCheckMask(false);");
        setGlobalCheckMask(false);
        /* set amp to drop_value */
        //message("setting amp to next_amp_drop = ",next_amp_drop, ", time = ",  elapsedTime());
        setAmpSPCallback(trace.amp_drop_value);
    }
    /* add new outside_mask_trace struct to outside_mask_traces */
    llrf.outside_mask_traces.push_back(llrfStructs::outside_mask_trace());
    /* local timem of outside_mask_trace */
    llrf.outside_mask_traces.back().time = elapsedTime();
    /* fill in data from where the trace that flagged an outside_mask trace */
    llrf.outside_mask_traces.back().trace_name = name;
    llrf.outside_mask_traces.back().high_mask  = trace.high_mask;
    llrf.outside_mask_traces.back().low_mask   = trace.low_mask;
    llrf.outside_mask_traces.back().mask_floor = trace.mask_floor;
    llrf.outside_mask_traces.back().time_vector = llrf.time_vector.value;
    /* index for element that was outside mask */
    llrf.outside_mask_traces.back().outside_mask_index = trace.outside_mask_index;
    /* turn on is_collecting, to get future traces */
    llrf.outside_mask_traces.back().is_collecting = true;
    /* add in message generated in updateIsTraceInMask */
    llrf.outside_mask_traces.back().message = outside_mask_trace_message.str();
    /* clear local copy of message */
    outside_mask_trace_message.str("");
    /* initialise number of traces to collect to zero */
    llrf.outside_mask_traces.back().num_traces_to_collect = UTL::ZERO_SIZET;
    /* set the part of outside_mask_traces to add future traces to
       and how many future traces to add  */
    std::pair<size_t, size_t> ouside_index_numtraces = std::make_pair(llrf.outside_mask_traces.size() - 1, llrf.num_extra_traces);
    /* save all the required traces (current, plus previous 2)
       set the save next trace flag, and the part of the outside_mask_traces to add to */
    for(auto && it: llrf.tracesToSaveOnBreakDown)
    {
        if(isMonitoring(it))
        {
            /* add to number of traces to collect */
            llrf.outside_mask_traces.back().num_traces_to_collect += (3 + llrf.num_extra_traces);// MAGIC_NUMBER
            /* ref for reading ease */
            llrfStructs::rf_trace_data& t = llrf.trace_data.at(it);
            /* add the oldest trace 1st */
            if(t.previous_previous_trace > UTL::MINUS_ONE_INT)
            {
                llrf.outside_mask_traces.back().traces.push_back(t.traces[ t.previous_previous_trace]);
                //message(trace.previous_previous_trace," added previous_previous_trace",trace.traces[trace.previous_previous_trace].EVID);
            }
            /* add the 2nd oldest trace */
            if(t.previous_trace > UTL::MINUS_ONE_INT)
            {
                llrf.outside_mask_traces.back().traces.push_back(t.traces[ t.previous_trace]);
                //message(trace.previous_trace," added previous_trace, ",trace.traces[trace.previous_trace].EVID);
            }
            /* add the latest  trace */
            llrf.outside_mask_traces.back().traces.push_back( t.traces[ t.current_trace]);

            t.outside_mask_data_index_to_add_future_traces_to.push_back(ouside_index_numtraces);

            //t.add_next_trace = llrf.num_extra_traces;
            //t.outside_mask_data_index_to_add_future_traces_to = true;
            // ... this part of outside_maask_trace, wehen the next update trace is called !
            //t.outside_mask_trace_part = llrf.outside_mask_traces.size();
        }
        else
            message(it, " IS NOT MONITORING So not Saving Data  (addToOutsideMaskTraces)");
    }
    llrf.num_outside_mask_traces = llrf.outside_mask_traces.size();

    // NOT WORKING OR TESTED
    llrf.breakdown_rate = (double)llrf.num_outside_mask_traces / (double)llrf.outside_mask_traces.back().time / 1000.;//MAGIC_NUMBER
    //debugMessage("Added trace to outside_mask_traces");
}
//____________________________________________________________________________________________
void liberallrfInterface::calcRollingAverage(llrfStructs::rf_trace_data& trace)
{
    //message("calcRollingAverage");
    if(trace.average_size == 1)
    {
        trace.rolling_average = trace.traces[trace.current_trace].value;
        trace.has_average = true;
    }
    else
    {
        updateRollingSum(trace);
        if(trace.has_average)
        {
            std::vector<double>& sum = trace.rolling_sum;
            std::vector<double>& av  = trace.rolling_average;
            for(auto i1=sum.begin(), i2=av.begin(); i1<sum.end() && i2<av.end(); ++i1,++i2)
            {
                *i2 = *i1 / (double)trace.average_size;
            }
        }
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateRollingSum(llrfStructs::rf_trace_data& trace)
{
    // add current values to average_trace_values vector
    trace.average_trace_values.push(trace.traces[trace.current_trace].value);
    // set this new entry in average_trace_values as th evetcor to add to rolling_sum
    std::vector<double>& to_add = trace.average_trace_values.back();
    std::vector<double>& sum    = trace.rolling_sum;
    std::vector<double>& max    = trace.rolling_max;
    std::vector<double>& min    = trace.rolling_min;

    auto i = 0;
    // update the rolling sum with the new values...
    for(auto&& to_add_it=to_add.begin(), sum_it=sum.begin(), min_it=min.begin(), max_it=max.begin();
             to_add_it<to_add.end() && sum_it<sum.end() && min_it<min.end() && max_it<max.end();
             ++to_add_it,++sum_it,++min_it,++max_it)
    {
        *sum_it += *to_add_it;
        if(*to_add_it < *min_it)
        {
            //message("i = ",i, " New min ",*to_add_it, " < ", *min_it);
            *min_it = *to_add_it;
        }
        if(*to_add_it > *max_it)
        {
            //message("i = ",i, " New max ",*to_add_it, " > ", *max_it);
            *max_it = *to_add_it;
        }
        ++i;
    }
    //debugMessage(trace.average_trace_values.size(), " ",trace.average_size);
    // update the counter
    //++trace.rolling_sum_counter;
    // set the flag depending on the number of traces in the rolling sum
    if(trace.average_trace_values.size() == trace.average_size)
    {//debugMessage("Calculating has_average = true");
        trace.has_average = true;
    }
    //debugMessage("updateRollingSum, current_trace =  ", trace.current_trace, ", sub_trace =  ", trace.sub_trace);
    // test to see if we should subtract a set of data
    if(trace.average_trace_values.size() > trace.average_size)
    {
        // if so subtract the first trace in average_trace_values
        std::vector<double>& to_sub = trace.average_trace_values.back();
        for(auto i1=to_sub.begin(), i2=sum.begin(); i1<to_sub.end() && i2<sum.end(); ++i1,++i2)
        {
            *i2 -= *i1;
        }
        // erase the trace we just subtraced
        trace.average_trace_values.pop();
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTraceCutMean(llrfStructs::rf_trace_data& tracedata,llrfStructs::rf_trace& trace)
{
    if(tracedata.mean_stop_index >= tracedata.mean_start_index)
    {
        trace.mean_start_index = tracedata.mean_start_index;
        trace.mean_stop_index = tracedata.mean_stop_index;
        double tempmean = UTL::ZERO_DOUBLE;
        for(auto i = trace.mean_start_index; i < trace.mean_stop_index; ++i)
        {
            tempmean += trace.value[i];
        }
        trace.mean = tempmean / (double) (trace.mean_stop_index - trace.mean_start_index);
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::updatePHIDEG()
{   // ONLY ever called from staticEntryLLRFMonitor
    double val = (llrf.phi_ff) * (llrf.phiCalibration);
    debugMessage("setPHIDEG PHI_DEG to, ",val, ", calibration = ", llrf.phiCalibration);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::PHI_DEG),val);
}
//____________________________________________________________________________________________
bool liberallrfInterface::updateAMPMVM()
{   // ONLY ever called from staticEntryLLRFMonitor
    double val = (llrf.amp_ff) * (llrf.ampCalibration);
    debugMessage("setAMPMVM AMP_MVM to, ",val, ", calibration = ", llrf.ampCalibration);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::AMP_MVM),val);
}
//____________________________________________________________________________________________
void liberallrfInterface::updateCanIncreaseActivePulses(const double max)
{
    /*  if we get exactly the same max as previous pulse we assume
        its a repeat and don't increase pulse count */
    if(max == llrf.kly_fwd_power_max)
    {
        llrf.can_increase_active_pulses = false;
    }
    else if(max < llrf.active_pulse_kly_power_limit)
    {
        llrf.can_increase_active_pulses = false;
    }
    else
    {
        llrf.can_increase_active_pulses = true;
    }
    /* the llrf object holds this paramter as well as the trace */
    llrf.kly_fwd_power_max = max;
}
//____________________________________________________________________________________________
void liberallrfInterface::updateEVID(const event_handler_args& args,llrfStructs::rf_trace_data& trace)
{
    llrfStructs::rf_trace &t = trace.traces[trace.evid_current_trace];
    //debugMessage("updateEVID START, trace.name = ", trace.name);
    if(isTimeType(args.type))
    {
        const dbr_time_string* p = nullptr;
        p = (const struct dbr_time_string*)args.dbr;
        //message("m1a");

        if(p != nullptr)
        {
            //message("m1aa");
            if((*p).value == '\0')
            {
                //message("m1a");
                t.EVID = "";
                //trace.traces[trace.evid_current_trace].EVID = "";
                message("null string passsed back by EPICS!!!!");
            }
            else
            {
                t.EVID = p->value;
                //message("t.EVID ", t.EVID);

                t.EVID_etime = p->stamp;
                updateTime(t.EVID_etime,
                       t.EVID_time,
                       t.EVID_timeStr);
                //message("t.EVID ", t.EVID, " t.EVID_timeStr ", t.EVID_timeStr);

            }
        }
        else
        {
            t.EVID = "nullptr";
        }
    }
    else
    {
        t.EVID = *((std::string*)args.dbr);
    }
    // update evid_current_trace index and roll back to beginning
    //
    //
    // the trace index tells us which part of 'traces' is the next to update
    // it circles 'round
    // EVID has a different counter than the llrf traces
    // This means it can get de-synchronised with the llrf traces
    // We may jsut have to bite the bullet on this one until we get Beam-synchronous data
    // in EPICS 4
    updateTraceIndex(trace.evid_current_trace , trace.traces.size());
    updateTraceIndex(trace.previous_evid_trace, trace.traces.size());
    trace.traces[trace.evid_current_trace].EVID = "NOT_SET"; //MAGIC_STRING

    // active pulses are counted from
    if(UTL::KLYSTRON_FORWARD_POWER == trace.name)
    {
        updateActivePulseCount(t.EVID);
    }


    // debugMessage("NEW trace.evid_current_trace  = ", trace.evid_current_trace, " ", t.EVID);
    // debugMessage("updateEVID FIN ");
}
//____________________________________________________________________________________________
void liberallrfInterface::updateActivePulseCount(const std::string& evid)
{
    //message("updateActivePulseCount");
    // active pulses are when the amplitude setting is above 0

    if(first_pulse && isMonitoring(UTL::KLYSTRON_FORWARD_POWER))
    {
        //message("acquired first pulse!");
        initial_pulsecount = getSize(evid);
        first_pulse = false;
    }
    size_t count = getSize(evid) - initial_pulsecount;
    if(llrf.can_increase_active_pulses)//MAGIC_NUMBER need to test llrf output and trig !
    {
        if(count < llrf.previous_pulseCount)
        {
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "EVID CLOCKED!!! " << evid << std::endl;
            std::cout << "!DO SOME MATH!" <<  std::endl;
            std::cout << "!DO SOME MATH!" <<  std::endl;
            std::cout << "!DO SOME MATH!" <<  std::endl;
            std::cout << "!DO SOME MATH!" <<  std::endl;
            std::cout << "!DO SOME MATH!" <<  std::endl;
            llrf.activePulseCount = count - initial_pulsecount + llrf.pulseCountOffset - initial_pulsecount;
        }
        else
        {
            size_t active_counts = count - llrf.previous_pulseCount;
            llrf.activePulseCount = llrf.activePulseCount + active_counts;
        }
    }
    //message("initial_pulse = ",initial_pulsecount,", previous count = ",llrf.previous_pulseCount,", active pulse count = ", llrf.activePulseCount,"  count ",count,"  evid", evid);

    llrf.previous_pulseCount = count;
}
//____________________________________________________________________________________________
void liberallrfInterface::updateSCAN(const event_handler_args& args,llrfStructs::rf_trace_data& trace)
{
    switch(*(int*)args.dbr)
    {
        case 0:
            trace.scan = llrfStructs::LLRF_SCAN::PASSIVE;
            break;
        case 1:
            trace.scan = llrfStructs::LLRF_SCAN::EVENT;
            break;
        case 2:
            trace.scan = llrfStructs::LLRF_SCAN::IO_INTR;
            break;
        case 3:
            trace.scan = llrfStructs::LLRF_SCAN::TEN;
            break;
        case 4:
            trace.scan = llrfStructs::LLRF_SCAN::FIVE;
            break;
        case 5:
            trace.scan = llrfStructs::LLRF_SCAN::TWO;
            break;
        case 6:
            trace.scan = llrfStructs::LLRF_SCAN::ONE;
            break;
        case 7:
            trace.scan = llrfStructs::LLRF_SCAN::ZERO_POINT_FIVE;
            break;
        case 8:
            trace.scan = llrfStructs::LLRF_SCAN::ZERO_POINT_TWO;
            break;
        case 9:
            trace.scan = llrfStructs::LLRF_SCAN::ZERO_POINT_ONE;
            break;
        default:
            trace.scan = llrfStructs::LLRF_SCAN::UNKNOWN_SCAN;
    }
    //message("New SCAN for trace ", trace.name, " = ", ENUM_TO_STRING(trace.scan));
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTrigState(const event_handler_args& args)
{   /* manually figure out which int state corresponds to my enum */
    switch( *(int*)args.dbr)
    {
        case 0:
            llrf.trig_source = llrfStructs::TRIG::OFF;
            break;
        case 1:
            llrf.trig_source = llrfStructs::TRIG::EXTERNAL;
            break;
        case 2:
            llrf.trig_source = llrfStructs::TRIG::INTERNAL;
            break;
    }
}
void liberallrfInterface::set_evid_ID_SET(llrfStructs::rf_trace_data& trace)
{
    if(evid_ID_SET)
    {

    }
    else
    {
        if(trace.traces[trace.evid_current_trace].EVID == "NOT_SET")//MAGIC_STRING
        {
            evid_id  = getSize(trace.traces[trace.previous_evid_trace].EVID) +1;
            evid_ID_SET = true;
        }
        else
        {
            evid_id  = getSize(trace.traces[trace.previous_evid_trace].EVID);
            evid_ID_SET = true;
        }
    }
}

//____________________________________________________________________________________________
//void liberallrfInterface::calcStandardDeviation(llrfStructs::rf_trace_data& trace)
//{
//    //https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
//
//    double sum = std::accumulate(std::begin(v), std::end(v), 0.0);
//double m =  sum / v.size();
//
//double accum = 0.0;
//std::for_each (std::begin(v), std::end(v), [&](const double d) {
//    accum += (d - m) * (d - m);
//});
//
//double stdev = sqrt(accum / (v.size()-1));
//
//}
//____________________________________________________________________________________________
void liberallrfInterface::updateTraceIndex(size_t& index, const  size_t trace_size)
{
    index += 1;
    if(index == trace_size)
    {
        index = 0;
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTraceIndex(int & index, const size_t trace_size)
{
    index += 1;
    if(index == (int)trace_size)
    {
        index = 0;
    }
}
// below not use anymore
//____________________________________________________________________________________________
//bool liberallrfInterface::shouldSubtractTraceFromRollingAverage(llrfStructs::rf_trace_data& trace)
//{
//    // we only subtract AFTER rolling_sum_counter is larger than average_size
//    if(trace.rolling_sum_counter > trace.average_size)
//        // paranoid sanity check
//        if(trace.average_trace_values.size() >= trace.average_size)
//        {
//            //debugMessage("shoudlSubtractTraceFromRollingAverage = true");
//            return true;
//        }
//    //debugMessage("shoudlSubtractTraceFromRollingAverage = false");
//    return false;
//}
//____________________________________________________________________________________________
bool liberallrfInterface::Is_TracePV(llrfStructs::LLRF_PV_TYPE pv)
{
    bool r = false;
    if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM)
    {
        r = true;
    }
    return r;
}
//____________________________________________________________________________________________
bool liberallrfInterface::IsNot_EVID_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    return !Is_EVID_PV(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::Is_EVID_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    bool r = false;
    if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_EVID)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_EVID)
    {
        r = true;
    }
    return r;
}
//____________________________________________________________________________________________
bool liberallrfInterface::IsNot_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    return !Is_SCAN_PV(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::Is_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    bool r = false;
    if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_LOCAL_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_AMP_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_DER_SCAN)
    {
        r = true;
    }
    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_DER_SCAN)
    {
        r = true;
    }
    return r;
}
void liberallrfInterface::startTimer()
{
    llrf.timer_start = msChronoTime();
}
//____________________________________________________________________________________________
long long liberallrfInterface::elapsedTime()
{
    return msChronoTime() - llrf.timer_start;
}
//____________________________________________________________________________________________
bool liberallrfInterface::Is_Time_Vector_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    if(pv == llrfStructs::LLRF_PV_TYPE::LIB_TIME_VECTOR)
        return true;
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::IsNot_TracePV(llrfStructs::LLRF_PV_TYPE pv)
{
    return !Is_TracePV(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::isMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    bool r = false;
    for(auto && it : continuousMonitorStructs)
    {
        if(it->monType == pv)
        {
            r = true;
            break;
        }
    }
    return r;
}
//____________________________________________________________________________________________
bool liberallrfInterface::isNotMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    return !isMonitoring(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::isMonitoring(const std::string& name)
{
    return isMonitoring(getLLRFPVType(name));
}
//____________________________________________________________________________________________
bool liberallrfInterface::isNotMonitoring(const std::string& name)
{
    return isNotMonitoring(getLLRFPVType(name));
}
//____________________________________________________________________________________________
bool liberallrfInterface::isFFLocked()
{
    return llrf.ff_amp_lock_state;
}
//____________________________________________________________________________________________
bool liberallrfInterface::RFOutput()
{
    return llrf.rf_output;
}
//____________________________________________________________________________________________
bool liberallrfInterface::interlockActive()
{
    return llrf.interlock_state;
}
//____________________________________________________________________________________________
bool liberallrfInterface::interlockNotActive()
{
    return !interlockActive();
}
//____________________________________________________________________________________________
bool liberallrfInterface::isFFNotLocked()
{
    return !isFFLocked();
}
//____________________________________________________________________________________________
bool liberallrfInterface::isCheckingMask(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
        return llrf.trace_data.at(n).check_mask;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv)
{
    return isCheckingMask(getLLRFChannelName(pv));
}
//____________________________________________________________________________________________
bool liberallrfInterface::isNotCheckingMask(const std::string& name)
{
    return !isCheckingMask(name);
}
//____________________________________________________________________________________________
bool liberallrfInterface::isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv)
{
    return !isCheckingMask(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::shouldCheckMasks(llrfStructs::rf_trace_data& trace)
{
    //debugMessage("shouldCheckMasks = ", trace.check_mask, trace.hi_mask_set, trace.low_mask_set);
    return llrf.check_mask && trace.check_mask && trace.hi_mask_set && trace.low_mask_set;
}
//____________________________________________________________________________________________
bool liberallrfInterface::shouldCheckMasks(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        bool a = shouldCheckMasks(llrf.trace_data.at(n));
        if(a)
        {
        }
        else
        {
            message(n,
                    " llrf.check_mask = ",llrf.check_mask,
                    ", trace.check_mask =  " ,llrf.trace_data.at(n).check_mask,
                    ", trace.hi_mask_set =  " ,llrf.trace_data.at(n).hi_mask_set,
                    ", trace.low_mask_set =  " ,llrf.trace_data.at(n).low_mask_set
                   );
        }
        return a;

    }
    return false;
}
//
//
//
//   _________.__               .__             _________       __    __
//  /   _____/|__| _____ ______ |  |   ____    /   _____/ _____/  |__/  |_  ___________  ______
//  \_____  \ |  |/     \\____ \|  | _/ __ \   \_____  \_/ __ \   __\   __\/ __ \_  __ \/  ___/
//  /        \|  |  Y Y  \  |_> >  |_\  ___/   /        \  ___/|  |  |  | \  ___/|  | \/\___ \
// /_______  /|__|__|_|  /   __/|____/\___  > /_______  /\___  >__|  |__|  \___  >__|  /____  >
//         \/          \/|__|             \/          \/     \/                \/           \/
//
// i.e. to be exposed to python
//____________________________________________________________________________________________
//____________________________________________________________________________________________
bool liberallrfInterface::disableInfiniteMaskEndByPower(const std::string& phase_trace)
{
    const std::string phase = fullCavityTraceName(phase_trace);
    if(entryExists(llrf.trace_data,phase))
    {
        llrf.trace_data.at(phase).endInfiniteMask_Trace_Set = false;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setInfiniteMaskEndByPower(const std::string& power_trace,const std::string& phase_trace,const double level)
{
    if(stringIsSubString(power_trace,UTL::POWER))
    {
        if(stringIsSubString(phase_trace,UTL::PHASE))
        {
            const std::string power = fullCavityTraceName(power_trace);
            const std::string phase = fullCavityTraceName(phase_trace);

            if(entryExists(llrf.trace_data,power))
            {
                if(entryExists(llrf.trace_data,phase))
                {
                    llrf.trace_data.at(phase).endMaskTrace_Bound.first  = power;
                    llrf.trace_data.at(phase).endMaskTrace_Bound.second = level;
                    llrf.trace_data.at(phase).endInfiniteMask_Trace_Set = true;
                    message(llrf.trace_data.at(phase).name, "endInfiniteMask_Trace_Set = true");
                    return true;
                }
                else
                {
                    llrf.trace_data.at(phase).endInfiniteMask_Trace_Set = false;
                }
            }
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::trigOff()
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::TRIG_SOURCE),llrfStructs::TRIG::OFF);
}
//____________________________________________________________________________________________
bool liberallrfInterface::trigInt()
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::TRIG_SOURCE),llrfStructs::TRIG::INTERNAL);
}
//____________________________________________________________________________________________
bool liberallrfInterface::trigExt()
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::TRIG_SOURCE),llrfStructs::TRIG::EXTERNAL);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setDropAmpOnOutsideMaskDetection(const std::string& name,const  bool state,const double amp_val)
{   // whether to drop amp when detetcing an outside_mask_trace
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, fullCavityTraceName(n)))
    {
        llrf.trace_data.at(n).amp_drop_value = amp_val;
        llrf.trace_data.at(n).drop_amp_on_breakdown  = state;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setDropAmpValue(const std::string& name, double amp_val)
{   // amplitude to set after detecting outside mask
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, fullCavityTraceName(n)))
    {
        llrf.trace_data.at(n).amp_drop_value = amp_val;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setNumContinuousOutsideMaskCount(const std::string& name, const size_t val)
{   // number of continuous points outside_mask to register as outside_mask
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).num_continuous_outside_mask_count = val;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
void liberallrfInterface::offsetTimer(long long value)
{   // add an ofset to the internal timer
    llrf.timer_start += value;
}
//____________________________________________________________________________________________
void liberallrfInterface::addPulseCountOffset(size_t val)
{
    llrf.activePulseCount += val;
}
//____________________________________________________________________________________________
void liberallrfInterface::setActivePulsePowerLimit(const double& val)
{
    llrf.active_pulse_kly_power_limit = val;
}
//____________________________________________________________________________________________
void liberallrfInterface::setNumExtraTraces(size_t value)
{
    llrf.num_extra_traces = value;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPulseLength(double value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_PULSE_LENGTH),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPulseOffset(double value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_PULSE_OFFSET),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPhiSP(double value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_PHI_SP),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPhiFF(double value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_PHI_FF),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpSP(double value)
{
    if(value>llrf.maxAmp)
    {
        message("Error!! Requested amplitude, ",value,"  too high");
        return false;
    }
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpHP(double value)
{
    setAmpSPCallback(value);
    return true;
}
//____________________________________________________________________________________________
void liberallrfInterface::kill_finished_setAmpHP_threads()
{
    for(auto && it = setAmpHP_Threads.cbegin(); it != setAmpHP_Threads.cend() /* not hoisted */; /* no increment */)
    {
        if(it->can_kill)
        {
            /// join before deleting...
            /// http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
            //std::cout<< "it->threadit->thread->join" <<std::endl;
            it->thread->join();
            //std::cout<< "it->thread" <<std::endl;
            delete it->thread;
            //std::cout<< "delete" <<std::endl;
            setAmpHP_Threads.erase(it++);
            //std::cout<< "erase" <<std::endl;
        }
        else
        {
            ++it;
        }
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::setAmpSPCallback(const double value)
{
    setAmpHP_Threads.push_back(llrfStructs::setAmpHP_Struct());
    setAmpHP_Threads.back().interface = this;
    setAmpHP_Threads.back().value  = value;
    setAmpHP_Threads.back().thread = new std::thread(staticEntrySetAmp, std::ref(setAmpHP_Threads.back()));
    kill_finished_setAmpHP_threads();
}
//____________________________________________________________________________________________
void liberallrfInterface::staticEntrySetAmp(llrfStructs::setAmpHP_Struct & s)
{
    //std::cout<< " new thread running" <<std::endl;
    s.interface->attachTo_thisCAContext(); /// base member function
    //std::cout<< " seting trigOff" <<std::endl;
    s.interface->trigOff();

    //disable checkgin masks
    s.interface->setGlobalShouldNotCheckMask();

    std::this_thread::sleep_for(std::chrono::milliseconds(2)); // MAGIC_NUMBER
    //std::cout<< " seting setAmpSP" <<std::endl;
    s.interface->setAmpSP(s.value);
    std::this_thread::sleep_for(std::chrono::milliseconds(2)); // MAGIC_NUMBER
    //std::cout<< "new thread trigExt" <<std::endl;
    s.interface->trigExt();
    s.can_kill = true;
    //std::cout<< "new thread fin" <<std::endl;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpFF(double value)
{
    if(value>llrf.maxAmp)
    {
        message("Error!! Requested amplitude, ",value,"  too high");
        return false;
    }
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpLLRF(double value)
{
    return setAmpSP(value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPhiLLRF(double value)
{
    return setPhiSP(value);
}
//____________________________________________________________________________________________
void liberallrfInterface::setPhiCalibration(double value)
{
    llrf.phiCalibration = value;
}
//____________________________________________________________________________________________
void liberallrfInterface::setAmpCalibration(double value)
{
    llrf.ampCalibration = value;
}
//____________________________________________________________________________________________
void liberallrfInterface::setTracesToSaveOnBreakDown(const std::vector<std::string>& name)
{
    std::vector<std::string> n;
    for(auto&& i: name)
    {
        n.push_back(fullCavityTraceName(i));
    }
    llrf.tracesToSaveOnBreakDown = n;
}
//____________________________________________________________________________________________
void liberallrfInterface::setTracesToSaveOnBreakDown(const std::string& name)
{
    std::vector<std::string> n;
    n.push_back(name);
    setTracesToSaveOnBreakDown(n);
}
//____________________________________________________________________________________________
void liberallrfInterface::setCrestPhiLLRF(double value) // in LLRF units
{
    llrf.crestPhi = value;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setTraceSCAN(const std::string& name, const llrfStructs::LLRF_SCAN value)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        for(auto&& it: llrf.pvMonStructs)
        {
            if(it.second.name == n && Is_SCAN_PV(it.first))
            {
                if(isMonitoring(it.first))
                {
                    return setValue(it.second,value);
                }
            }
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAllSCANToPassive()
{
    for(auto&& it: llrf.pvMonStructs)
    {
        if(Is_SCAN_PV(it.first))
        {
            setValue(it.second, llrfStructs::LLRF_SCAN::PASSIVE);
        }
    }
    return true;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAllTraceSCAN(const llrfStructs::LLRF_SCAN value)
{
    for(auto&& it: llrf.pvMonStructs)
    {
        if(Is_SCAN_PV(it.first))
            setValue(it.second,value);
    }
    return true;
}
//____________________________________________________________________________________________
void liberallrfInterface::setNumBufferTraces(const size_t value)
{
    for(auto && it: llrf.trace_data)
    {
        setNumBufferTraces(it.first, value);
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::setNumBufferTraces(const std::string&name,const size_t value)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {

        // update the number of rf_trace objects
        llrf.trace_data.at(n).buffersize = value;

        // and set their sizes based on buffersize and COUNT

        llrf.trace_data.at(n).traces.clear();
        llrf.trace_data.at(n).traces.resize(llrf.trace_data.at(n).buffersize);

        // update the rf_trace object with the size of a trace and its parent name
        for(auto && it2: llrf.trace_data.at(n).traces)
        {
            it2.value.resize(llrf.trace_data.at(n).trace_size);
            it2.name = n;
        }
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpMVM(double value)// MV / m amplitude
{
    bool r = false;
    if(value<UTL::ZERO_DOUBLE)
    {
        message("Error!! you must set a positive amplitude for LLRF, not ", value);
    }
    else
    {
        //double val = std::round(value/llrf.ampCalibration);
        double val = value/llrf.ampCalibration;
        if(val > llrf.maxAmp)
        {
            message("Error!! Requested amplitude, ",val,"  too high");
        }
        else
        {
            debugMessage("Requested amplitude, ", value," MV/m = ", val," in LLRF units ");
            r = setAmpLLRF(val);
        }
    }
    return r;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPhiDEG(double value)// degrees relative to crest
{
    bool r = false;
    if(value<-180.0||value>180.0)//MAGIC_NUMBER
    {
        message("Error!! you must set phase between -180.0 and +180.0, not ", value);
    }
    else
    {
        double val = llrf.crestPhi + value/llrf.phiCalibration;

        debugMessage("Requested PHI, ", value," degrees  = ", val," in LLRF units ");
        r = setPhiLLRF(val);
    }
    return r;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setMeanStartEndTime(const double start, const double end, const std::string&name)
{
    bool a = setMeanStartIndex(name, getIndex(start));
    if(a)
    {
        a = setMeanStopIndex(name, getIndex(end));
    }
    return a;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setMeanStartIndex(const std::string&name, size_t  value)
{
    if(entryExists(llrf.trace_data, name))
    {
        if(llrf.trace_data.at(name).trace_size - 1 >= value )
        {
            llrf.trace_data.at(name).mean_start_index = value;
            return true;
        }
    }
//    else
//        message(name," NOT FOUND!");
    return false;
}
//____________________________________________________________________________________________
bool  liberallrfInterface::setMeanStopIndex(const std::string&name, size_t  value)
{
    if(entryExists(llrf.trace_data, name))
    {
        if(llrf.trace_data.at(name).trace_size - 1 >= value )
        {
            llrf.trace_data.at(name).mean_stop_index = value;
            return true;
        }
    }
    //message(name," NOT FOUND!");
    return false;
}
//
//
//
// __________       .__  .__  .__                    _____
// \______   \ ____ |  | |  | |__| ____    ____     /  _  \___  __ ________________     ____   ____   ______
//  |       _//  _ \|  | |  | |  |/    \  / ___\   /  /_\  \  \/ // __ \_  __ \__  \   / ___\_/ __ \ /  ___/
//  |    |   ( <_>)  |_|  |_|  |   |  \/ /_/  > /    |    \   /\  ___/|  | \// __ \_/ /_/  >  ___/ \___ \
//  |____|_  /\____/|____/____/__|___|  /\___  /  \____|__  /\_/  \___  >__|  (____  /\___  / \___  >____  >
//         \/                         \//_____/           \/          \/           \//_____/      \/     \/
//
// Set the parameters for the rolling avergaes
//____________________________________________________________________________________________
//____________________________________________________________________________________________
void liberallrfInterface::resetAverageTraces()
{
    for(auto && it : llrf.trace_data)
    {
        resetAverageTraces(it.second);
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::resetAverageTraces(llrfStructs::rf_trace_data& trace)
{
    trace.has_average = false;
    trace.current_trace = UTL::ZERO_SIZET;//MAGIC_NUMBER
    trace.previous_trace = UTL::MINUS_ONE_INT;//MAGIC_NUMBER
    trace.previous_previous_trace = UTL::MINUS_TWO_INT;//MAGIC_NUMBER
    trace.rolling_average.clear();
    trace.rolling_average.resize(trace.trace_size);
    trace.rolling_sum.clear();
    trace.rolling_sum.resize(trace.trace_size);
    trace.average_trace_values = {};
    //trace.average_trace_values.resize(trace.average_size);
    trace.rolling_max.clear();
    trace.rolling_max.resize(trace.trace_size, -std::numeric_limits<double>::infinity());
    trace.rolling_min.clear();
    trace.rolling_min.resize(trace.trace_size, std::numeric_limits<double>::infinity());
    trace.rolling_sd.clear();
    trace.rolling_sd.resize(trace.trace_size);
}
//____________________________________________________________________________________________
bool liberallrfInterface::isOutsideMaskDataFinishedCollecting(size_t part)
{
    if(part <= llrf.outside_mask_traces.size() - UTL::ONE_SIZET)
    {
        return !llrf.outside_mask_traces[part].is_collecting;
    }
    message("isOutsideMaskDataFinishedCollecting(size_t part)");
    return false;
}
//____________________________________________________________________________________________
void liberallrfInterface::setShouldKeepRollingAverage()
{
    for(auto && it: llrf.trace_data)
    {
        setShouldKeepRollingAverage(it.first);
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::setShouldNotKeepRollingAverage()
{
    for(auto && it: llrf.trace_data)
    {
        setShouldNotKeepRollingAverage(it.first);
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::setShouldKeepRollingAverage(const std::string&name)
{
    return setKeepRollingAverage(name, true);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setShouldNotKeepRollingAverage(const std::string&name)
{
    return setKeepRollingAverage(name, false);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setKeepRollingAverage(const std::string&name, bool value)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).keep_rolling_average = value;
        resetAverageTraces(llrf.trace_data.at(n));
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
void liberallrfInterface::setKeepRollingAverageNoReset(const bool value)
{
    for(auto && it: llrf.trace_data)
    {
        it.second.keep_rolling_average = value;
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::setKeepRollingAverageNoReset(const std::string&name, const bool value)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).keep_rolling_average = value;
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::setNumRollingAverageTraces(const size_t value)
{
    for(auto && it: llrf.trace_data)
    {
        setNumRollingAverageTraces(it.first, value);
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::setNumRollingAverageTraces(const std::string&name,const size_t value)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
//        if(value <= llrf.trace_data.at(n).buffersize)
//            llrf.trace_data.at(n).average_size = value;
//        else
//        {
        llrf.trace_data.at(n).average_size = value;
        //message("Number of Rolling Average traces is greater than buffer, using ",llrf.trace_data.at(n).average_size);
//        }
        resetAverageTraces(llrf.trace_data.at(n));
        return true;
    }
    else
        message("liberallrfInterface ERROR, trace ", n, " does not exist");
    return false;
}
//
//
// MASKS
//____________________________________________________________________________________________
bool liberallrfInterface::setShouldNotCheckMask(const std::string&name)
{
    return setCheckMask(name, false);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCheckMask(const std::string&name, bool value)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).check_mask = value;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
void liberallrfInterface::setGlobalCheckMask(bool value)
{
    llrf.check_mask = value;
}
//____________________________________________________________________________________________
void liberallrfInterface::setGlobalShouldCheckMask()
{
    setGlobalCheckMask(true);
}
//____________________________________________________________________________________________
void liberallrfInterface::setGlobalShouldNotCheckMask()
{
    setGlobalCheckMask(false);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setShouldCheckMask(const std::string&name)
{
    return setCheckMask(fullCavityTraceName(name), true);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPercentTimeMask(const double s1,const double s2,const double s3,
                                         const double s4,const double value2,const  std::string& name)
{
    // The mask is defined in terms of time, using the time_vector to find the indece
    return setPercentMask(getIndex(s1),getIndex(s2),getIndex(s3),getIndex(s4),value2,name);
}
//____________________________________________________________________________________________
/// THE NEXT TWO FUNCTIONS COULD BE COMBINED AND NEATENED UP
//____________________________________________________________________________________________
bool liberallrfInterface::setPercentMask(const size_t s1,const size_t s2,const size_t s3,
                                         const size_t s4,const double value,const  std::string& name)
{
    return set_mask(s1,s2,s3,s4,value/100.0,name,true);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAbsoluteTimeMask(const double s1,const double s2,const double s3,
                                         const double s4,const double value,const  std::string& name)
{
    // The mask is defined in terms of time, using the time_vector to find the indece
    return setAbsoluteMask(getIndex(s1),getIndex(s2),getIndex(s3),getIndex(s4),value,name);
}

//____________________________________________________________________________________________
bool liberallrfInterface::setAbsoluteMask(const size_t s1,const size_t s2,
                                          const size_t s3,const size_t s4,
                                          const double value,const  std::string& name)
{
    return set_mask(s1,s2,s3,s4,value,name,false);
}
//____________________________________________________________________________________________
bool liberallrfInterface::set_mask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,
                                   const double value, const std::string& name,
                                   const bool isPercent
                                  )
{
    // automatically set the mask based on the rolling_average for cavity_rev_power trace
    // between element 0    and s1 will be set to default hi/lo (+/-infinity)
    // between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average
    // between element s2+1 and s3 will be set very default hi/lo (+/-infinity)
    // between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average
    // between element s3+1 and s4 will be set very default hi/lo (+/-infinity)
    double temp = 0.0;
    std::string mask_type;
    if(isPercent)
    {
        mask_type = "setPercentMask";
    }
    else
    {
        mask_type = "setAbsoluteMask";
    }
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        // if we're keeping an average pulse
        if(llrf.trace_data.at(n).has_average)
        {
            std::vector<double> & ra = llrf.trace_data.at(n).rolling_average;

            // sanity check on s1,s2,s3,s4
            if(0 <= s1 && s4 <= ra.size() - 1)
            {
                if(s1 <= s2 && s2 <= s3 && s3 <= s4)
                {
                    double hi_max = -std::numeric_limits<double>::infinity();
                    double lo_min =  std::numeric_limits<double>::infinity();

                    std::vector<double> hi_mask(ra.size(), 0.0);
                    std::vector<double> lo_mask(ra.size(), 0.0);

                    for(auto i = 0; i <= s1; ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    for(auto i = s1; i <= s2; ++i)
                    {
                        if(isPercent)
                        {
                            temp = ra[i] * value;
                        }
                        else
                        {
                            temp = value;
                        }
                        hi_mask[i] = ra[i] + temp;
                        lo_mask[i] = ra[i] - temp;
                        if(hi_mask[i] > hi_max)
                        {
                           hi_max = hi_mask[i];
                        }
                        if(lo_mask[i] < lo_min)
                        {
                           lo_min = hi_mask[i];
                        }
                    }
                    for(auto i = s2+1; i <= s3; ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    for(auto i = s3+1; i <= s4; ++i)
                    {
                        if(isPercent)
                        {
                            temp = ra[i] * value;
                        }
                        else
                        {
                            temp = value;
                        }
                        hi_mask[i] = ra[i] + temp;
                        lo_mask[i] = ra[i] - temp;
                        if(hi_mask[i] > hi_max)
                        {
                           hi_max = hi_mask[i];
                        }
                        if(lo_mask[i] < lo_min)
                        {
                           lo_min = hi_mask[i];
                        }
                    }
                    for(auto i = s4+1; i < ra.size(); ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    // apply mask values
                    if(setHighMask(n,hi_mask))
                    {
                        message("hi_max = ",hi_max);
                        if(setLowMask(n,lo_mask))
                        {
                            message("lo_min = ", lo_min);
                            return true;
                        }
                        else
                        {
                            message(n," setLowMask failed,  ",mask_type," fail");
                        }
                    }
                    else
                    {
                        message(n," setHighMask failed,,  ",mask_type," fail");
                    }
                }
                else
                {
                    message(n,s1," <= ",s2," && ",s2," <= ",s3," && ",s3," <= ",s4," failed,  ",mask_type," fail");
                }
            }
            else
            {
                message(n," 0 <= ",s1," && ",s4," <= ",ra.size() - 1," failed, ",mask_type," fail");
            }
        }
        else
        {
            message(n," does not exist,  ",mask_type," fail");
        }
    }
    else
    {
        message(n," does not exist,  ",mask_type," fail");
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setMaskInfiniteEnd(const std::string& trace_name, size_t index)
{
    if(index + 1 < llrf.trace_data.at(trace_name).low_mask.size())
    {
        //assume entry eixtsts
        llrf.trace_data.at(trace_name).mask_end_by_power_index = index;
        auto lo_it = llrf.trace_data.at(trace_name).low_mask.begin();
        std::advance(lo_it, index);
        auto hi_it = llrf.trace_data.at(trace_name).high_mask.begin();
        std::advance(hi_it, index);
        for(;lo_it != llrf.trace_data.at(trace_name).low_mask.end() && hi_it != llrf.trace_data.at(trace_name).high_mask.end(); ++lo_it,++hi_it)
        {
            *lo_it = -std::numeric_limits<double>::infinity();
            *hi_it =  std::numeric_limits<double>::infinity();
        }
    }
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getMaskInfiniteEndByPowerIndex(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        //assume entry eixtsts
        return llrf.trace_data.at(n).mask_end_by_power_index;
    }
    return 0;
}
//____________________________________________________________________________________________
double liberallrfInterface::getMaskInfiniteEndByPowerTime(const std::string& name)
{
    return getTime(getMaskInfiniteEndByPowerIndex(name));
}
//____________________________________________________________________________________________
bool liberallrfInterface::clearMask(const std::string&name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).high_mask.clear();
        llrf.trace_data.at(n).low_mask.clear();
        llrf.trace_data.at(n).hi_mask_set  = false;
        llrf.trace_data.at(n).low_mask_set = false;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setInfiniteMasks(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrfStructs::rf_trace_data& r = llrf.trace_data.at(n);
        std::vector<double> p_inf(r.trace_size,  std::numeric_limits<double>::infinity());
        std::vector<double> n_inf(r.trace_size, -std::numeric_limits<double>::infinity());
        r.high_mask = p_inf;
        r.low_mask = n_inf;
        r.hi_mask_set  = true;
        r.low_mask_set = true;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setHighMask(const std::string&name,const std::vector<double>& value)
{
    if(entryExists(llrf.trace_data, name))
    {
//        if(llrf.trace_data.at(name).add_next_trace == 0)
//        {
            if(value.size() == llrf.trace_data.at(name).trace_size)
            {
                llrf.trace_data.at(name).high_mask = value;
                llrf.trace_data.at(name).hi_mask_set = true;
                //debugMessage(name," mask_hi[90] = ", llrf.trace_data.at(name).high_mask[90]);
                return true;
            }
//        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setLowMask(const std::string&name,const std::vector<double>& value)
{
    if(entryExists(llrf.trace_data, name))
    {
//        if(llrf.trace_data.at(name).add_next_trace == 0)
//        {
            if(value.size() == llrf.trace_data.at(name).trace_size)
            {
                llrf.trace_data.at(name).low_mask = value;
                llrf.trace_data.at(name).low_mask_set = true;
                //debugMessage(name," mask_lo[90] = ", llrf.trace_data.at(name).low_mask[90]);
                return true;
            }
//        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setMaskFloor(const std::string& name, double value)
{
    if(entryExists(llrf.trace_data, fullCavityTraceName(name)))
    {
        llrf.trace_data.at(fullCavityTraceName(name)).mask_floor = value;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrHiMask(const std::vector<double>& value)
{
    return setHighMask(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrLoMask(const std::vector<double>& value)
{
    return setLowMask(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setPercentMask(s1,s2,s3,s4,value,fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setAbsoluteMask(s1,s2,s3,s4,value,fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavFwdPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setPercentMask(s1,s2,s3,s4,value,fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavFwdPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setAbsoluteMask(s1,s2,s3,s4,value,fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
}

//____________________________________________________________________________________________

//____________________________________________________________________________________________

//_____________________________________________________________________________________________________
//_____________________________________________________________________________________________________
//
//
// .___        __                             .__      _________       __    __
// |   | _____/  |_  ___________  ____ _____  |  |    /   _____/ _____/  |__/  |_  ___________  ______
// |   |/    \   __\/ __ \_  __ \/    \\__  \ |  |    \_____  \_/ __ \   __\   __\/ __ \_  __ \/  ___/
// |   |   |  \  | \  ___/|  | \/   |  \/ __ \|  |__  /        \  ___/|  |  |  | \  ___/|  | \/\___ \
// |___|___|  /__|  \___  >__|  |___|  (____  /____/ /_______  /\___  >__|  |__|  \___  >__|  /____  >
//          \/          \/           \/     \/               \/     \/                \/           \/
//
// i.e. things used by functions in this class, not primarily to be exposed to python
//_____________________________________________________________________________________________________
template<typename T>
bool liberallrfInterface::setValue(llrfStructs::pvStruct& pvs, T value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE,pvs.CHID,&value);
    std::stringstream ss;
    ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    //debugMessage(ss);
    ss.str("");
    ss << "Timeout setting llrf, " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    int status = sendToEpics("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}
//____________________________________________________________________________________________
template<typename T>
bool liberallrfInterface::setValue2(llrfStructs::pvStruct& pvs, T value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE,pvs.CHID,&value);
    std::stringstream ss;
    ss << "setValue2 setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    debugMessage(ss);
    ss.str("");
    ss << "Timeout setting llrf, " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    int status = sendToEpics2("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}
//
//
//
// .___        __                             .__      ________        __    __
// |   | _____/  |_  ___________  ____ _____  |  |    /  _____/  _____/  |__/  |_  ___________  ______
// |   |/    \   __\/ __ \_  __ \/    \\__  \ |  |   /   \  ____/ __ \   __\   __\/ __ \_  __ \/  ___/
// |   |   |  \  | \  ___/|  | \/   |  \/ __ \|  |__ \    \_\  \  ___/|  |  |  | \  ___/|  | \/\___ \
// |___|___|  /__|  \___  >__|  |___|  (____  /____/  \______  /\___  >__|  |__|  \___  >__|  /____  >
//          \/          \/           \/     \/               \/     \/                \/           \/
//
//  i.e. things used by functions in this class, not primarily to be exposed to python
//____________________________________________________________________________________________
std::vector<std::string> liberallrfInterface::getChannelNames()
{
    std::vector<std::string>  r;
    for(auto && it:llrf.pvMonStructs)
    {
        if(Is_TracePV(it.first))
        {
            r.push_back(it.second.name);
        }
    }
    return r;
}
//____________________________________________________________________________________________
std::vector<std::string> liberallrfInterface::getTraceNames()
{
    std::vector<std::string>  r;
    for(auto && it:llrf.trace_data)
    {
        r.push_back(it.first);
    }
    return r;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getLLRFPVType(const std::string& name)
{
    std::string n = fullCavityTraceName(name);
    for(auto && it:llrf.pvMonStructs)
    {
        if(it.second.name == n)
        {
            return it.first;
        }
    }
    return llrfStructs::LLRF_PV_TYPE::UNKNOWN;
}
//____________________________________________________________________________________________
std::string liberallrfInterface::getLLRFChannelName(const llrfStructs::LLRF_PV_TYPE pv)
{
    for(auto && it:llrf.pvMonStructs)
    {
        if(it.first == pv)
        {
            return it.second.name;
        }
    }
    return ENUM_TO_STRING(llrfStructs::LLRF_PV_TYPE::UNKNOWN);
}
//____________________________________________________________________________________________
std::string liberallrfInterface::fullCavityTraceName(const std::string& name)
{
    if(myLLRFType == llrfStructs::LLRF_TYPE::CLARA_HRRG || myLLRFType == llrfStructs::LLRF_TYPE::VELA_HRRG)
    {
        if(name == UTL::CAVITY_REVERSE_PHASE)
            return UTL::HRRG_CAVITY_REVERSE_PHASE;

        if(name == UTL::CAVITY_FORWARD_PHASE)
            return UTL::HRRG_CAVITY_FORWARD_PHASE;

        if(name == UTL::CAVITY_REVERSE_POWER)
            return UTL::HRRG_CAVITY_REVERSE_POWER;

        if(name == UTL::CAVITY_FORWARD_POWER)
            return UTL::HRRG_CAVITY_FORWARD_POWER;
    }
    else if(myLLRFType == llrfStructs::LLRF_TYPE::CLARA_LRRG || myLLRFType == llrfStructs::LLRF_TYPE::VELA_LRRG)
    {
        if(name == UTL::CAVITY_REVERSE_PHASE)
            return UTL::LRRG_CAVITY_REVERSE_PHASE;

        if(name == UTL::CAVITY_FORWARD_PHASE)
            return UTL::LRRG_CAVITY_FORWARD_PHASE;

        if(name == UTL::CAVITY_REVERSE_POWER)
            return UTL::LRRG_CAVITY_REVERSE_POWER;

        if(name == UTL::CAVITY_FORWARD_POWER)
            return UTL::LRRG_CAVITY_FORWARD_POWER;
    }
    return name;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getEVID_pv(llrfStructs::LLRF_PV_TYPE pv)
{
    switch(pv)
    {
         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_EVID;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_EVID;
    }
    return llrfStructs::LLRF_PV_TYPE::UNKNOWN;
}
//____________________________________________________________________________________________
//llrfStructs::LLRF_PV_TYPE liberallrfInterface::getSCAN_pv(llrfStructs::LLRF_PV_TYPE pv)
//{
//    switch(pv)
//    {
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_SCAN;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_SCAN;
//    }
//    return llrfStructs::LLRF_PV_TYPE::UNKNOWN;
//}
//____________________________________________________________________________________________
//
//
//
//   _________.__               .__             ________        __    __
//  /   _____/|__| _____ ______ |  |   ____    /  _____/  _____/  |__/  |_  ___________  ______
//  \_____  \ |  |/     \\____ \|  | _/ __ \  /   \  ____/ __ \   __\   __\/ __ \_  __ \/  ___/
//  /        \|  |  Y Y  \  |_> >  |_\  ___/  \    \_\  \  ___/|  |  |  | \  ___/|  | \/\___ \
// /_______  /|__|__|_|  /   __/|____/\___  >  \______  /\___  >__|  |__|  \___  >__|  /____  >
//         \/          \/|__|             \/          \/     \/                \/           \/
//
// i.e. things to expose to python
//____________________________________________________________________________________________
double liberallrfInterface::getPhiCalibration()
{
    return llrf.phiCalibration;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpCalibration()
{
    return llrf.ampCalibration;
}
//____________________________________________________________________________________________
double liberallrfInterface::getCrestPhiLLRF()// in LLRF units
{
    return llrf.crestPhi;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpFF()
{
    return llrf.amp_ff;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpSP()
{
    return llrf.amp_sp;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpMVM()// physics units
{
    return llrf.amp_MVM;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpLLRF()// physics units
{
    return getAmpFF();
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiFF()
{
    return llrf.phi_ff;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiSP()
{
    return llrf.phi_sp;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiDEG()// physics units
{
    return llrf.phi_DEG;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiLLRF()// physics units
{
    return getPhiFF();
}
//____________________________________________________________________________________________
double liberallrfInterface::getPulseLength()
{
    return llrf.pulse_length;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPulseOffset()
{
    return llrf.pulse_offset;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getShotCount(const std::string& name)
{
    std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).shot;
    }
    else
        message("liberallrfInterface::getShotCount ERROR, trace ", n, " does not exist");
    return UTL::ZERO_SIZET;
}
//____________________________________________________________________________________________
double liberallrfInterface::getBreakDownRate()
{
    return llrf.breakdown_rate;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getIndex(const double time)
{
    auto r = std::lower_bound(llrf.time_vector.value.begin(), llrf.time_vector.value.end(),time);
    //message(std::distance(vec.begin(), it))
    return r - llrf.time_vector.value.begin();
}
//____________________________________________________________________________________________
double liberallrfInterface::getTime(const size_t index)
{
    return llrf.time_vector.value[index];
}
//____________________________________________________________________________________________
bool liberallrfInterface::clearRollingAverage(const std::string&name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        resetAverageTraces(llrf.trace_data.at(n));
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getActivePulseCount()
{
    return llrf.activePulseCount;
}
//____________________________________________________________________________________________
const llrfStructs::liberallrfObject& liberallrfInterface::getLLRFObjConstRef()
{
    return llrf;
}
//____________________________________________________________________________________________
const llrfStructs::rf_trace_data& liberallrfInterface::getTraceDataConstRef(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n);
    }
    else
    {
        message("liberallrfInterface::getTraceDataConstRef() ERROR, trace ", n, " does not exist");

    }
    return dummy_trace_data;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getNumExtraTraces()
{
    return llrf.num_extra_traces;
}
//____________________________________________________________________________________________
double liberallrfInterface::getActivePulsePowerLimit()
{
    return llrf.active_pulse_kly_power_limit;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getHiMask(const std::string&name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        if(llrf.trace_data.at(name).hi_mask_set)
        {
            return llrf.trace_data.at(n).high_mask;
        }
    }
    else
        message("liberallrfInterface::getHiMask ERROR, trace ", n, " does not exist");
    std::vector<double> r{UTL::DUMMY_DOUBLE};//MAGIC_NUMBER
    return r;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getLoMask(const std::string&name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        if(llrf.trace_data.at(n).low_mask_set)
        {
            return llrf.trace_data.at(n).low_mask;
        }
    }
    else
        message("liberallrfInterface::getLoMask ERROR, trace ", n, " does not exist");
    std::vector<double> r{UTL::DUMMY_DOUBLE};//MAGIC_NUMBER
    return r;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_TYPE liberallrfInterface::getType()
{
    return llrf.type;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getTraceLength()
{
    return llrf.traceLength;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getNumOutsideMaskTraces()
{
    return llrf.outside_mask_traces.size();
}
//____________________________________________________________________________________________
std::vector<llrfStructs::outside_mask_trace> liberallrfInterface::getOutsideMaskData()
{
    return llrf.outside_mask_traces;
}
//____________________________________________________________________________________________
llrfStructs::TRIG liberallrfInterface::getTrigSource()
{
    return llrf.trig_source;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getNumBufferTraces(const std::string&name)
{
        const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).buffersize;
    }
    else
        message("liberallrfInterface::getNumBufferTraces ERROR, trace ", n, " does not exist");
    return UTL::ZERO_SIZET;
}
//____________________________________________________________________________________________
llrfStructs::outside_mask_trace liberallrfInterface::getOutsideMaskData(const size_t part)
{
    if(part <= llrf.outside_mask_traces.size() - UTL::ONE_SIZET)
    {
        return llrf.outside_mask_traces[part];
    }
    else
    {
        message("ERROR!!! outside_mask_traces.size() = ",
                llrf.outside_mask_traces.size()," asked for part = ",part," dummy returned");
        return llrfStructs::outside_mask_trace();
    }
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getNumRollingAverageTraces(const std::string&name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        return llrf.trace_data.at(n).average_size;
    }
    else
        message("liberallrfInterface::getNumRollingAverageTraces ERROR, trace ", n, " does not exist");
    return UTL::ZERO_SIZET;
}
//____________________________________________________________________________________________
std::vector<std::string> liberallrfInterface::getTracesToSaveOnBreakDown()
{
    return llrf.tracesToSaveOnBreakDown;
}
//
// GET LATEST TRACE VALUES
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getTraceValues(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        //return llrf.trace_data.at(name).traces.back().value;
        message(llrf.trace_data.at(n).traces[llrf.trace_data.at(name).latest_trace_index].value[100]);
        message(n);
        return llrf.trace_data.at(n).traces[llrf.trace_data.at(name).latest_trace_index].value;
    }
    else
        message("liberallrfInterface::getTraceValues ERROR, trace ", n, " does not exist");
    std::vector<double> r(1, 0);//MAGIC_NUMBER
    return r;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavRevPower()
{
    return getTraceValues(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavRevPhase()
{
    return getTraceValues(fullCavityTraceName(UTL::CAVITY_REVERSE_PHASE));
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavFwdPower()
{
    return getTraceValues(fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavFwdPhase()
{
    return getTraceValues(fullCavityTraceName(UTL::CAVITY_FORWARD_PHASE));
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyRevPower()
{
    return getTraceValues(UTL::KLYSTRON_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyRevPhase()
{
    return getTraceValues(UTL::KLYSTRON_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyFwdPower()
{
    return getTraceValues(UTL::KLYSTRON_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyFwdPhase()
{
    return getTraceValues(UTL::KLYSTRON_FORWARD_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePower()
{
    return getTraceValues(UTL::CAVITY_PROBE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePhase()
{
    return getTraceValues(UTL::CAVITY_PROBE_PHASE);
}
//
// GET LATEST TRACE DATA STRUCT
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getTraceData(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        //return llrf.trace_data.at(name).traces.back();
        return llrf.trace_data.at(n).traces[llrf.trace_data.at(name).latest_trace_index];
    }
    else
        message("liberallrfInterface::getTraceData ERROR, trace ", n, " does not exist");
    llrfStructs::rf_trace r;
    return r;
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavRevPowerData()
{
    return getTraceData(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavRevPhaseData()
{
    return getTraceData(fullCavityTraceName(UTL::CAVITY_REVERSE_PHASE));
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavFwdPowerData()
{
    return getTraceData(fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavFwdPhaseData()
{
    return getTraceData(fullCavityTraceName(UTL::CAVITY_FORWARD_PHASE));
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getKlyRevPowerData()
{
    return getTraceData(UTL::KLYSTRON_REVERSE_POWER);
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getKlyRevPhaseData()
{
    return getTraceData(UTL::KLYSTRON_REVERSE_PHASE);
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getKlyFwdPowerData()
{
    return getTraceData(UTL::KLYSTRON_FORWARD_POWER);
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getKlyFwdPhaseData()
{
    return getTraceData(UTL::KLYSTRON_FORWARD_PHASE);
}
//
// GET THE ENTIRE TRACE DATA BUFFER
//
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getTraceBuffer(const std::string& name)
{
        const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        return llrf.trace_data.at(n).traces;
    }
    else
        message("liberallrfInterface::getTraceBuffer ERROR, trace ", n, " does not exist");
    std::vector<llrfStructs::rf_trace> r;
    return r;
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavRevPowerBuffer()
{
    return getTraceBuffer(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavRevPhaseBuffer()
{
    return getTraceBuffer(fullCavityTraceName(UTL::CAVITY_REVERSE_PHASE));
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavFwdPowerBuffer()
{
    return getTraceBuffer(fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavFwdPhaseBuffer()
{
    return getTraceBuffer(fullCavityTraceName(UTL::CAVITY_FORWARD_PHASE));
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyRevPowerBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyRevPhaseBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyFwdPowerBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyFwdPhaseBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_REVERSE_PHASE);
}
//
// GET TRACE AVERAGES
//
//____________________________________________________________________________________________
std::vector<double>liberallrfInterface::getAverageTraceData(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        if(llrf.trace_data.at(n).has_average)
        {
            //return llrf.trace_data.at(name).traces.back();
            return llrf.trace_data.at(n).rolling_average;
        }
    }
    else
        message("liberallrfInterface::getAverageTraceData ERROR, trace ", n, " does not exist");
    std::vector<double> r;
    return r;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavRevPowerAv()
{
    return getAverageTraceData(UTL::CAVITY_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavFwdPowerAv()
{
    return getAverageTraceData(UTL::CAVITY_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyRevPowerAv()
{
    return getAverageTraceData(UTL::KLYSTRON_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyFwdPowerAv()
{
    return getAverageTraceData(UTL::KLYSTRON_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavRevPhaseAv()
{
    return getAverageTraceData(UTL::CAVITY_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavFwdPhaseAv()
{
    return getAverageTraceData(UTL::CAVITY_FORWARD_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyRevPhaseAv()
{
    return getAverageTraceData(UTL::KLYSTRON_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getKlyFwdPhaseAv()
{
    return getAverageTraceData(UTL::KLYSTRON_FORWARD_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePowerAv()
{
    return getAverageTraceData(UTL::CAVITY_PROBE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePhaseAv()
{
    return getAverageTraceData(UTL::CAVITY_PROBE_PHASE);
}
//____________________________________________________________________________________________
//
//
// ___________                               _____                .__  __
// \__    ___/___________    ____  ____     /     \   ____   ____ |__|/  |_  ___________  ______
//   |    |  \_  __ \__  \ _/ ___\/ __ \   /  \ /  \ /  _ \ /    \|  \   __\/  _ \_  __ \/  ___/
//   |    |   |  | \// __ \\  \__\  ___/  /    Y    ( <_>)   |  \  ||  | ( <_>)  | \/\___ \
//   |____|   |__|  (____  /\___  >___  > \____|__  /\____/|___|  /__||__|  \____/|__|  /____  >
//                       \/     \/    \/          \/            \/                           \/
//
//____________________________________________________________________________________________
bool liberallrfInterface::startCavFwdTraceMonitor()
{
    bool a1 = startTraceMonitoring(fullCavityTraceName(UTL::CAVITY_FORWARD_PHASE));
    bool a2 = startTraceMonitoring(fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::startCavRevTraceMonitor()
{
    bool a1 = startTraceMonitoring(fullCavityTraceName(UTL::CAVITY_REVERSE_PHASE));
    bool a2 = startTraceMonitoring(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::startKlyFwdTraceMonitor()
{
    bool a1 = startTraceMonitoring(UTL::KLYSTRON_FORWARD_PHASE);
    bool a2 = startTraceMonitoring(UTL::KLYSTRON_FORWARD_POWER);
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::startKlyRevTraceMonitor()
{
    bool a1 = startTraceMonitoring(UTL::KLYSTRON_REVERSE_PHASE);
    bool a2 = startTraceMonitoring(UTL::KLYSTRON_REVERSE_POWER);
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopTraceMonitoring(const std::string& name)
{
    return stopTraceMonitoring(getLLRFPVType(name));
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    for(auto && it : continuousMonitorStructs)
    {
        if(Is_TracePV(it->monType))
        {
            if(it->monType == pv)
            {
                killMonitor(it);
                delete it;
            }
        }
    }
    return isNotMonitoring(pv);
}
//____________________________________________________________________________________________
void liberallrfInterface::stopTraceMonitoring()
{
    for(auto && it : continuousMonitorStructs)
    {
        if(Is_TracePV(it->monType) )
        {
            killMonitor(it);
            delete it;
        }
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopCavFwdTraceMonitor()
{
    bool a1 = stopTraceMonitoring(fullCavityTraceName(UTL::CAVITY_FORWARD_PHASE));
    bool a2 = stopTraceMonitoring(fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopCavRevTraceMonitor()
{
    bool a1 = stopTraceMonitoring(fullCavityTraceName(UTL::CAVITY_REVERSE_PHASE));
    bool a2 = stopTraceMonitoring(fullCavityTraceName(UTL::CAVITY_REVERSE_POWER));
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopKlyFwdTraceMonitor()
{
    bool a1 = stopTraceMonitoring(UTL::KLYSTRON_FORWARD_PHASE);
    bool a2 = stopTraceMonitoring(UTL::KLYSTRON_FORWARD_POWER);
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopKlyRevTraceMonitor()
{
    bool a1 = stopTraceMonitoring(UTL::KLYSTRON_REVERSE_PHASE);
    bool a2 = stopTraceMonitoring(UTL::KLYSTRON_REVERSE_POWER);
    if(a1 && a2)
        return true;
    else
        return false;
}












