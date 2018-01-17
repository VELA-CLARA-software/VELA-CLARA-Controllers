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

#include "liberallrfInterface.h"
//djs
#include "dburt.h"
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
                              const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                              const bool shouldStartEPICs ,
                              const llrfStructs::LLRF_TYPE type
                              ):
configReader(llrfConf,startVirtualMachine,show_messages_ptr,show_debug_messages_ptr),
interface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs(shouldStartEPICs),
usingVirtualMachine(startVirtualMachine),
myLLRFType(type),
first_pulse(true),
initial_pulsecount(0),
next_amp_drop(0),
newthread(nullptr)
{
    // we've moved the trace data out into a new config file vased on th e type of llrf contorlller
    configReader.setType(type);
//    if(shouldStartEPICs )
//    message("magnet liberallrfInterface shouldStartEPICs is true");
//    else
//    message("magnet liberallrfInterface shouldStartEPICs is false");
    // MOVE TO init list

    initialise();
}
//______________________________________________________________________________
liberallrfInterface::~liberallrfInterface()
{
    killILockMonitors();
    for(auto && it : continuousMonitorStructs )
    {
        killMonitor(it );
        delete it;
    }
//    debugMessage("liberallrfInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void liberallrfInterface::killMonitor(llrfStructs::monitorStruct * ms)
{
    int status = ca_clear_subscription(ms->EVID);
    // should check if ca_clear_subscription was a success
    delete ms;
}
//______________________________________________________________________________
void liberallrfInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 )); // MAGIC_NUMBER
    if(configFileRead )
    {
        message("The liberallrfInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if(getDataSuccess )
        {
            message("LLRF objects acquired");
            if(shouldStartEPICs )
            {
                message("The liberallrfInterface has acquired objects, connecting to EPICS");
                // subscribe to the channel ids
                initChids();
                // start the monitors: set up the callback functions
                debugMessage("Starting Monitors");
                startMonitors();
                // The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // MAGIC_NUMBER
            }
            else
             message("The liberallrfInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message("!!!The liberallrfInterface received an Error while getting llrf data!!!");
    }
}
//______________________________________________________________________________
bool liberallrfInterface::initObjects()
{
    bool success = configReader.getliberallrfObject(llrf);

    // set the TRACE sizes to defaults etc...
    if(success)
    {
        for(auto&& it:llrf.pvMonStructs)
        {
            if( Is_TracePV(it.first) )
            {
                debugMessage("creating trace_data for, ", it.second.name);
                if(entryExists(llrf.trace_data,it.second.name))
                {
                    message("!!!!ERROR IN TRACES CONFIG FILE DETECTED!!!!!");
                    message("!!!!ABORTING INSTANTIATION!!!!!");
                    return false;
                }
                llrf.trace_data[it.second.name].trace_size = it.second.COUNT;
                llrf.trace_data.at(it.second.name).name = it.second.name;
                debugMessage("set trace COUNT = ",  it.second.COUNT);
                success = setNumBufferTraces(it.second.name, llrf.trace_data.at(it.second.name).buffersize);
                if( !success )
                {
                    message("!!!!ERROR IN setNumBufferTraces DETECTED!!!!!");
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
    message("\n", "Searching for LLRF chids...");
    for(auto && it : llrf.pvMonStructs )
    {
        addChannel(llrf.pvRoot, it.second );
    }
    // command only PVs for the LLRF to set "high level" phase and amplitide
    for(auto && it : llrf.pvComStructs )
    {
        addChannel(llrf.pvRoot, it.second );
    }
    addILockChannels(llrf.numIlocks, llrf.pvRoot, llrf.name, llrf.iLockPVStructs );
    int status=sendToEpics("ca_create_channel","Found LLRF ChIds.",
                           "!!TIMEOUT!! Not all LLRF ChIds found." );
    if(status==ECA_TIMEOUT)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//MAGIC_NUMBER
        message("\n","Checking LLRF ChIds ");
        for(auto && it : llrf.pvMonStructs )
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first ) );
        }
        for(auto && it : llrf.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first ) );
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // MAGIC_NUMBER
    }
    else if (status == ECA_NORMAL )
        allChidsInitialised = true;  // interface base class member
}
//______________________________________________________________________________
void liberallrfInterface::addChannel(const std::string & pvRoot, llrfStructs::pvStruct & pv)
{
    std::string s1;
    // TEMPORARY HACK FOR HIGH LEVEL llrf PARAMATERS
    if(pv.pvType == llrfStructs::LLRF_PV_TYPE::AMP_MVM)
    {
        s1 =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else if( pv.pvType == llrfStructs::LLRF_PV_TYPE::PHI_DEG )
    {
        s1 =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else
    {
        s1 = pvRoot + pv.pvSuffix;
    }
    ca_create_channel(s1.c_str(), 0, 0, 0, &pv.CHID );//MAGIC_NUMBER
    debugMessage("Create channel to ", s1 );
}
//______________________________________________________________________________
void liberallrfInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto && it : llrf.pvMonStructs )
    {   // if using the VM we don't monito AMP Read
//        if(usingVirtualMachine && it.first == llrfStructs::LLRF_PV_TYPE::AMP_R )
//        {
//            message("For VM AMP_R is not monitored");
//        }
//        else
//        {
        if(IsNot_TracePV(it.first) && IsNot_EVID_PV(it.first) && IsNot_SCAN_PV(it.first) )
        {
            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
            continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
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
                             "Succesfully Subscribed to LLRF Monitors",
                             "!!TIMEOUT!! Subscription to LLRF monitors failed" );
    if (status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//____________________________________________________________________________________________
void liberallrfInterface::startTraceMonitoring()
{
    for(auto && it : llrf.pvMonStructs )
    {
        if(Is_TracePV(it.first) )
        {
            if(isNotMonitoring(it.first) )
            {
                startTraceMonitoring(it.first);
            }
//            if(isNotMonitoring(it.first) )
//            {
//                debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
//                continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
//                continuousMonitorStructs.back() -> monType    = it.first;
//                continuousMonitorStructs.back() -> llrfObj = &llrf;
//                continuousMonitorStructs.back() -> interface  = this;
//                ca_create_subscription(it.second.CHTYPE,
//                                   it.second.COUNT,
//                                   it.second.CHID,
//                                   it.second.MASK,
//                                   liberallrfInterface::staticEntryLLRFMonitor,
//                                   (void*)continuousMonitorStructs.back(),
//                                   &continuousMonitorStructs.back() -> EVID);
//            }
        }
    }
//    int status = sendToEpics("ca_create_subscription", "Succesfully Subscribed to LLRF Trace Monitors", "!!TIMEOUT!! Subscription to LLRF Trace monitors failed" );
//    if (status == ECA_NORMAL )
//        debugMessage("All LLRF Trace Monitors Started");

}
//____________________________________________________________________________________________
bool liberallrfInterface::startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    if(Is_TracePV(pv) )
    {
        if(isNotMonitoring(pv))
        {
            std::string name = getLLRFChannelName(pv);
            llrf.trace_data[name].traces.resize( llrf.trace_data[name].buffersize );
            for( auto && it2: llrf.trace_data[name].traces)
                it2.value.resize( llrf.pvMonStructs.at(pv).COUNT);

            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(pv), " (",getLLRFChannelName(pv) ,")");
            continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
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
            ss <<"Succesfully Subscribed to LLRF Trace Monitor " << ENUM_TO_STRING(pv);
            std::string s1 = ss.str();
            ss.str(std::string());
            ss <<"!!TIMEOUT!! Subscription to LLRF Trace monitor " << ENUM_TO_STRING(pv) << " failed";
            std::string s2 = ss.str();


            llrfStructs::LLRF_PV_TYPE EVIDpv = getEVID_pv(pv);

            if(  EVIDpv != llrfStructs::LLRF_PV_TYPE::UNKNOWN  )
            {
                debugMessage("ca_create_subscription to ", ENUM_TO_STRING(EVIDpv));
                continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
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

            llrfStructs::LLRF_PV_TYPE SCANpv = getSCAN_pv(pv);

            if(  EVIDpv != llrfStructs::LLRF_PV_TYPE::UNKNOWN  )
            {
                debugMessage("ca_create_subscription to ", ENUM_TO_STRING(SCANpv));
                continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
                continuousMonitorStructs.back() -> monType   = SCANpv;
                continuousMonitorStructs.back() -> llrfObj   = &llrf;
                continuousMonitorStructs.back() -> interface = this;
                continuousMonitorStructs.back() -> name      = getLLRFChannelName(SCANpv);
                ca_create_subscription(llrf.pvMonStructs.at(SCANpv).CHTYPE,
                                       llrf.pvMonStructs.at(SCANpv).COUNT,
                                       llrf.pvMonStructs.at(SCANpv).CHID,
                                       llrf.pvMonStructs.at(SCANpv).MASK,
                                       liberallrfInterface::staticEntryLLRFMonitor,
                                       (void*)continuousMonitorStructs.back(),
                                       &continuousMonitorStructs.back() -> EVID);
            }

            int status = sendToEpics("ca_create_subscription",s1.c_str(),s2.c_str());
            if (status == ECA_NORMAL )
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
std::vector<std::string> liberallrfInterface::getChannelNames()
{
    std::vector<std::string>  r;
    for(auto && it:llrf.pvMonStructs )
    {
        if( Is_TracePV(it.first) )
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
    for(auto && it:llrf.trace_data )
    {
        r.push_back(it.first);
    }
    return r;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getLLRFPVType(const std::string& name)
{
    std::string n = fullCavityTraceName(name);
    for(auto && it:llrf.pvMonStructs )
    {
        if( it.second.name == n )
        {
            return it.first;
        }
    }
    return llrfStructs::LLRF_PV_TYPE::UNKNOWN;
}
//____________________________________________________________________________________________
std::string liberallrfInterface::getLLRFChannelName(const llrfStructs::LLRF_PV_TYPE pv)
{
    for(auto && it:llrf.pvMonStructs )
    {
        if( it.first == pv )
        {
            return it.second.name;
        }
    }
    return ENUM_TO_STRING(llrfStructs::LLRF_PV_TYPE::UNKNOWN);
}
//____________________________________________________________________________________________
bool liberallrfInterface::startCavFwdTraceMonitor()
{
    bool a1 = startTraceMonitoring( fullCavityTraceName(UTL::CAVITY_FORWARD_PHASE));
    bool a2 = startTraceMonitoring( fullCavityTraceName(UTL::CAVITY_FORWARD_POWER));
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
    for(auto && it : continuousMonitorStructs )
    {
        if(Is_TracePV(it->monType)  )
        {
            killMonitor(it );
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
//____________________________________________________________________________________________
void liberallrfInterface::staticEntryLLRFMonitor(const event_handler_args args)
{
    // Make the below neater at some point!
    llrfStructs::monitorStruct*ms = static_cast<  llrfStructs::monitorStruct *>(args.usr);

    if(ms->interface->Is_TracePV(ms -> monType))
    {
        //ms->interface->message("staticEntryLLRFMonitor called with TRACE PV, for ", ms->name );
        ms->interface->updateTrace(args,ms->llrfObj->trace_data.at( ms->name ) );
    }
    else if(ms->interface->Is_EVID_PV(ms -> monType))
    {
        //ms->interface->message("staticEntryLLRFMonitor called with EVID PV", ms->name);
        ms->interface->updateEVID(args,ms->llrfObj->trace_data.at( ms->name ) );
    }
    else if(ms->interface->Is_SCAN_PV(ms -> monType))
    {
        //ms->interface->message("staticEntryLLRFMonitor called with EVID PV", ms->name);
        ms->interface->updateSCAN(args,ms->llrfObj->trace_data.at( ms->name ) );
    }
    else
    {
        //ms->interface->message("staticEntryLLRFMonitor called with normal PV = ", ENUM_TO_STRING(ms -> monType) );
        switch(ms -> monType)
        {
            case llrfStructs::LLRF_PV_TYPE::LIB_ILOCK_STATE:
                if(*(int*)args.dbr == 1)
                {
                    ms->interface->llrf.interlock_state = true;
                }
                else
                {
                    ms->interface->llrf.interlock_state = false;
                }
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE:
                if(*(int*)args.dbr == 1)
                {
                    ms->interface->llrf.ff_amp_lock_state = true;
                }
                else
                {
                    ms->interface->llrf.ff_amp_lock_state = false;
                }
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE:
                if(*(int*)args.dbr == 1)
                {
                    ms->interface->llrf.ff_ph_lock_state = true;
                }
                else
                {
                    ms->interface->llrf.ff_ph_lock_state = false;
                }
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_RF_OUTPUT:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_LOCK = ",*(bool*)args.dbr);
                if(*(int*)args.dbr == 1)
                {
                    ms->interface->llrf.rf_output = true;
                }
                else
                {
                    ms->interface->llrf.rf_output = false;
                }
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_AMP_FF = ",*(double*)args.dbr);
                ms->llrfObj->amp_ff = *(double*)args.dbr;
                ms->llrfObj->amp_MVM = (ms->llrfObj->amp_ff) * (ms->llrfObj->ampCalibration);

                if( ms->llrfObj->amp_ff < 10)
                {
                    //ms->interface->setGlobalCheckMask(false);
                    ms->interface->message("Recieved setting amp to zero = ", ms->interface->elapsedTime() );
                }


                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_AMP_SP = ",*(double*)args.dbr);
                ms->llrfObj->amp_sp = *(double*)args.dbr;
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PHI_FF:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_PHI_FF = ",*(double*)args.dbr);
                ms->llrfObj->phi_ff = *(double*)args.dbr;
                ms->llrfObj->phi_DEG = (ms->llrfObj->phi_ff) * (ms->llrfObj->phiCalibration);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PHI_SP:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_PHI_SP = ",*(double*)args.dbr);
                ms->llrfObj->phi_sp = *(double*)args.dbr;
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_TIME_VECTOR:
                // the time_vector is simlar to a trace, but we can just update the values straight away
                ms->interface->updateValues(args,ms->llrfObj->time_vector);
                break;
            case llrfStructs::LLRF_PV_TYPE::LIB_PULSE_LENGTH:
                //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_PULSE_LENGTH = ",*(double*)args.dbr);
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
        //ms->interface->message("staticEntryLLRFMonitor FIN");
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTraceCutMean(llrfStructs::rf_trace& trace)
{
    if( trace.mean_stop_index > trace.mean_start_index)
    {
        double tempmean = UTL::ZERO_DOUBLE;
        for( auto i =  trace.mean_start_index; i < trace.mean_stop_index; ++i)
        {
            tempmean += trace.value[i];
        }
        trace.mean = tempmean / (double) ( trace.mean_stop_index - trace.mean_start_index);
        //message(trace.name,"   trace.mean = ",trace.mean," tempmean = ",tempmean, " delta = ", (double) ( trace.mean_stop_index - trace.mean_start_index));
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTrace(const event_handler_args& args, llrfStructs::rf_trace_data& trace)
{
    //trace.traces[trace.current_trace].procStart = elapsedTime();

    // called from staticEntryLLRFMonitor, and will update a power or a phase trace
    // the time_vector is similar but handled in ----some-function----
    // update the new trace values
    //debugMessage("updateTrace START, trace.current_trace  = ", trace.current_trace );
    updateValues(args, trace.traces[trace.current_trace]);
    trace.traces[trace.current_trace].mean_start_index = trace.mean_start_index;
    trace.traces[trace.current_trace].mean_stop_index = trace.mean_stop_index;
    updateTraceCutMean(trace.traces[trace.current_trace]);


    for(auto&& it: trace.add_next_trace_to_outside_mask_trace)
    {
        llrf.outside_mask_traces[it].traces.push_back( trace.traces[trace.current_trace] );

//        if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//        {
//            message(trace.name," added future pulse for part  ", it);
//        }
    }
    trace.add_next_trace_to_outside_mask_trace.clear();

//    if(trace.add_next_trace_to_outside_mask_trace )
//    {
//        //message("adding next trace, ", trace.current_trace);
//        if( trace.outside_mask_trace_part <= llrf.outside_mask_traces.size() )
//        {
//            llrf.outside_mask_traces[trace.outside_mask_trace_part].traces.push_back( trace.traces[trace.current_trace] );
//            trace.add_next_trace_to_outside_mask_trace = false;
//        }
//    }



    // check masks
    if(shouldCheckMasks(trace))// checks against
    {
        //debugMessage("CHECKING MASKS ",trace.name);
        bool trace_good = isTraceInMask(trace);
        if( trace_good )
        {
            //debugMessage("Trace was good");
            // update rolling average
            // at the moment the rolling average will NOT get updated with an outside mask trace,
            // this is probably what we want to do, but maybe not... ?
            if(trace.keep_rolling_average)
            {
                //debugMessage("Calculating Rolling Average");
                calcRollingAverage(trace);
            }

//            trace.traces[trace.current_trace].procEnd = elapsedTime();
//            if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//            {
//                message(trace.name," proc time 1a = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//            }

        }
        else
        {
            //debugMessage("Trace was bad");
            // if the trace is "bad" add it to outside_mask_traces
            llrfStructs::monitorStruct*ms = static_cast< llrfStructs::monitorStruct *>(args.usr);
            addToOutsideMaskTraces(trace,  ms->name);
            //message(" trace.outside_mask_trace_part, ",  trace.outside_mask_trace_part);

//            trace.traces[trace.current_trace].procEnd = elapsedTime();
//            if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//            {
//                message(trace.name," proc time 1b = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//            }
        }
    }
    else
    {
        // update rolling average
        // at the moment the rolling average will get updated with an outside mas trace,
        // this is probably not what we want to do
        if(trace.keep_rolling_average)
        {
            //debugMessage("Calculating Rolling Average");
            calcRollingAverage(trace);
        }
        //debugMessage("NOT CHECKING MASKS ");

//        trace.traces[trace.current_trace].procEnd = elapsedTime();
//        if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//        {
//            message(trace.name," proc time 1c = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//        }
    }
    // the trace index tells us which part of 'traces' is the next to update
    // it circles 'round

    trace.latest_trace_index = trace.current_trace;
    updateTraceIndex(trace.current_trace, trace.traces.size());

//    trace.traces[trace.current_trace].procEnd = elapsedTime();
//    if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//    {
//        message(trace.name," proc time 1d = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//    }


    updateTraceIndex(trace.previous_trace, trace.traces.size());

//    trace.traces[trace.current_trace].procEnd = elapsedTime();
//
//    if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//    {
//
//        message(trace.name," proc time 1e = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//    }

    updateTraceIndex(trace.previous_previous_trace, trace.traces.size());

//    trace.traces[trace.current_trace].procEnd = elapsedTime();
//    if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//    {
//        message(trace.name," proc time 1f = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//    }

    //debugMessage("NEW trace.current_trace  = ", trace.current_trace );
    //debugMessage("updateTrace FIN ");
    trace.shot += UTL::ONE_SIZET; // the difference between this and pulse count indicates how many traces we miss!
    //message("trace.shot = ", trace.shot);

//    trace.traces[trace.current_trace].procEnd = elapsedTime();
//    if(trace.name == fullCavityTraceName(UTL::CAVITY_REVERSE_POWER))
//    {
//        message(trace.name," proc time 2 = ", trace.traces[trace.current_trace].procEnd - trace.traces[trace.current_trace].procStart);
//    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateValues(const event_handler_args& args, llrfStructs::rf_trace& trace)
{
    // this function actually gets the new values from EPICS and adds them to the trace.value vector
    // all LLRF traces should be updated using this function
    if(isTimeType(args.type))
    {
        const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
        size_t counter = 0;//MAGIC_NUMBER
        const dbr_double_t * pValue;
        pValue = &p->value;
        for(auto && it : trace.value)
        {
            //it = *((double const* const) p->value + counter);
            it = pValue[counter];
            ++counter;
        }
        trace.etime = p->stamp;// MAGIC_NUMBER
        updateTime(trace.etime, trace.time, trace.timeStr);
    }
    else
    {
        size_t i = 0;//MAGIC_NUMBER

        for(auto && it:trace.value)
        {
            it = *((double*)args.dbr + i);
    //        std::cout << trace.second[i] << " ";
            ++i;
        }
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateEVID(const event_handler_args& args,llrfStructs::rf_trace_data& trace)
{
    llrfStructs::rf_trace &t = trace.traces[trace.evid_current_trace];
    //debugMessage("updateEVID START, trace.evid_current_trace  = ", trace.evid_current_trace );
    if(isTimeType(args.type))
    {
        const dbr_time_string* p = nullptr;
        p = (const struct dbr_time_string*)args.dbr;
        //message("m1a");

        if(p != nullptr)
        {
            //message("m1aa");
            if( (*p).value == '\0' )
            {
                //message("m1a");
                t.EVID = "";
                //trace.traces[trace.evid_current_trace].EVID = "";
                message("null string passsed back by EPICS!!!!");
            }
            else
            {
                t.EVID = p->value;
                //message("t.EVID ", t.EVID );
            }
            t.EVID_etime = p->stamp;
            updateTime(t.EVID_etime,
                       t.EVID_time,
                       t.EVID_timeStr);
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
    updateTraceIndex(trace.evid_current_trace , trace.traces.size() );

    if(UTL::KLYSTRON_FORWARD_POWER == trace.name )
    {
        updateActivePulseCount(t.EVID);
    }
    // debugMessage("NEW trace.evid_current_trace  = ", trace.evid_current_trace, " ", t.EVID );
    // debugMessage("updateEVID FIN ");
}
//____________________________________________________________________________________________
void liberallrfInterface::updateActivePulseCount(const std::string& evid)
{
    // active pulses are when the amplitude setting is above 0
    size_t count = getSize(evid);
    if(first_pulse)
    {
        initial_pulsecount = count;
        first_pulse = false;
    }
    if(llrf.amp_ff > 0.0)//
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
            llrf.activePulseCount = count - initial_pulsecount + llrf.pulseCountOffset;
        }
        else
        {
            llrf.activePulseCount = count - initial_pulsecount + llrf.pulseCountOffset;
        }
    }
    //message("initial_pulse = ",initial_pulsecount,", previous count = ",llrf.previous_pulseCount,", active pulse count = ", llrf.activePulseCount,"  count ",count,"  evid", evid );

    llrf.previous_pulseCount = count;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getActivePulseCount()
{
    return llrf.activePulseCount;
}
//____________________________________________________________________________________________
void liberallrfInterface::setPulseCountOffset(size_t val)
{
    llrf.pulseCountOffset = val;
}
//____________________________________________________________________________________________
void liberallrfInterface::updateSCAN(const event_handler_args& args,llrfStructs::rf_trace_data& trace)
{
    switch( *(int*)args.dbr)
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
bool liberallrfInterface::isTraceInMask(llrfStructs::rf_trace_data& trace)
{
    size_t breakdown_count = 0;
    bool reset = true;
    std::vector<double>& to_check = trace.traces[trace.current_trace].value;
    std::vector<double>& hi = trace.high_mask;
    std::vector<double>& lo = trace.low_mask;
    for(auto i = 0; i < to_check.size(); ++i)
    {
        if(to_check[i] > trace.mask_floor)
        {
            if(to_check[i] > hi[i])
            {
                breakdown_count += 1;
                reset = false;
                if(breakdown_count == trace.num_continuous_outside_mask_count)
                {
                    message(trace.name," gave: ",trace.rolling_average[i]," ",to_check[i], " > ", hi[i]," at i = ",i," us = ", llrf.time_vector.value[i]);
                    if(trace.drop_amp_on_breakdown)
                    {
                        // stop checking masks
                        //message("setGlobalCheckMask(false);");
                        //setGlobalCheckMask(false);
                        // set amp to drop_value
                        next_amp_drop = trace.amp_drop_value;

                        message("setting amp to zero = ", elapsedTime() );
                        setAmpFFCallback();
                    }
                    else
                        message("trace.drop_amp_on_breakdown is False");
                    return false;
                }
            }
            else
            {
                breakdown_count = 0;
            }
            if(to_check[i] < lo[i])
            {
                breakdown_count += 1;
                reset = false;
                if(breakdown_count == trace.num_continuous_outside_mask_count)
                {
                    message(trace.name," gave: ",trace.rolling_average[i]," ",to_check[i], " > ", hi[i]," at i = ",i," us = ", llrf.time_vector.value[i]);
                    if(trace.drop_amp_on_breakdown)
                    {
                        // stop checking masks
                        //message("setGlobalCheckMask(false);");
                        //setGlobalCheckMask(false);
                        // set amp to drop_value
                        next_amp_drop = trace.amp_drop_value;

                        message("setting amp to zero = ", elapsedTime() );

                        setAmpFFCallback();
                    }
                    else
                        message("trace.drop_amp_on_breakdown is False");

                    return false;
                }
            }
            if(reset)
            {
                breakdown_count = 0;
            }
            else
            {
                reset = true;
            }
        }
    }
    return true;
}
//____________________________________________________________________________________________
void liberallrfInterface::setAmpFFCallback()
{
    if(newthread)
    {
        newthread -> join();
        delete newthread;
        newthread = nullptr;
    }
    newthread = new std::thread(staticEntrySetAmp, this );
    newthread->detach();
}
//____________________________________________________________________________________________
void liberallrfInterface::staticEntrySetAmp(liberallrfInterface* interface)
{
    //std::cout<< " new thread running" <<std::endl;
    interface->attachTo_thisCAContext(); /// base member function
    //std::cout<< " seting zero amp ?" <<std::endl;
    interface->setAmpSP( interface -> next_amp_drop );
    //std::cout<< " new thread fin" <<std::endl;
}
//____________________________________________________________________________________________
void liberallrfInterface::addToOutsideMaskTraces(llrfStructs::rf_trace_data& trace,const std::string& name)
{
//    if(trace.drop_amp_on_breakdown)
//    {
//        // stop checking masks
//        message("setGlobalCheckMask(false);");
//        setGlobalCheckMask(false);
//        // set amp to drop_value
//        next_amp_drop = trace.amp_drop_value;
//        setAmpFFCallback();
//    }

    // add new outside_mask_trace struct to outside_mask_traces
    llrf.outside_mask_traces.push_back( llrfStructs::outside_mask_trace() );
    // time of breakdown
    llrf.outside_mask_traces.back().time = elapsedTime();
    // fill in data from where the trace that flagged a break down
    llrf.outside_mask_traces.back().trace_name = name;
    llrf.outside_mask_traces.back().high_mask  = trace.high_mask;
    llrf.outside_mask_traces.back().low_mask   = trace.low_mask;

    // save all the required traces (current, plus previous 2)
    // set the save next trace flag, adn the part of the
    for(auto && it: llrf.tracesToSaveOnBreakDown )
    {
        if( isMonitoring(it) )
        {
            //message("adding break down data for ", it );

            llrfStructs::rf_trace_data& t = llrf.trace_data.at(it);
            //add the oldest trace 1st
            if( t.previous_previous_trace > -1 )
            {
                llrf.outside_mask_traces.back().traces.push_back( t.traces[ t.previous_previous_trace] );
                //message(trace.previous_previous_trace," added previous_previous_trace",trace.traces[trace.previous_previous_trace].EVID);
            }
            //add the 2nd oldest trace
            if( t.previous_trace > -1 )
            {
                llrf.outside_mask_traces.back().traces.push_back( t.traces[ t.previous_trace] );
                //message(trace.previous_trace," added previous_trace, ",trace.traces[trace.previous_trace].EVID);
            }
            //add the latest  trace
            llrf.outside_mask_traces.back().traces.push_back(  t.traces[ t.current_trace] );
            //message(trace.current_trace, " added current_trace,", trace.traces[trace.current_trace].EVID);

            // set the keep next trace flag and that is added to...

            t.add_next_trace_to_outside_mask_trace.push_back( llrf.outside_mask_traces.size() - 1 );

            //t.add_next_trace_to_outside_mask_trace = true;
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
void liberallrfInterface::updateRollingSum(llrfStructs::rf_trace_data& trace)
{
    std::vector<double>& to_add = trace.traces[trace.current_trace].value;
    std::vector<double>& sum    = trace.rolling_sum;
    std::vector<double>& max    = trace.rolling_max;
    std::vector<double>& min    = trace.rolling_min;

    // update the rolling sum with the new values...
    for(auto i1=to_add.begin(), i2=sum.begin(), i3=min.begin(), i4=max.begin();
             i1<to_add.end() && i2<sum.end() && i3<min.end() && i4<max.end();
             ++i1,++i2,++i3,++i4)
    {
        *i2 += *i1;
        if( *i1 < *i3 )
            *i3 = *i1;
        else if( *i1 > *i4 )
            *i4 = *i3;
    }
    //debugMessage("Sum[0] = ", sum[0]);
    // update the counter
    ++trace.rolling_sum_counter;
    // set the flag depending on the number of traces in the rolling sum
    if(trace.rolling_sum_counter >= trace.average_size)
    {
        //debugMessage("Calculating has_average = true");
        trace.has_average = true;
    }
    else
    {
        //debugMessage("Calculating has_average = false");
        trace.has_average = false;
    }

    //debugMessage("updateRollingSum, current_trace =  ", trace.current_trace, ", sub_trace =  ", trace.sub_trace);

    // test to see if we should subtract a set of data
    if(shouldSubtractTraceFromRollingAverage(trace))
    {
        // if so subtract the last trace
        std::vector<double>& to_sub = trace.traces[trace.sub_trace].value;
        for(auto i1=to_sub.begin(), i2=sum.begin(); i1<to_sub.end() && i2<sum.end(); ++i1,++i2)
        {
            *i2 -= *i1;
        }
        updateTraceIndex(trace.sub_trace, trace.traces.size());
    }

}
//____________________________________________________________________________________________
void liberallrfInterface::updateTraceIndex(size_t& index, const  size_t trace_size )
{
    index += 1;
    if(index == trace_size)
    {
        index = 0;
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTraceIndex(int & index, const size_t trace_size )
{
    index += 1;
    if(index == (int)trace_size)
    {
        index = 0;
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::shouldSubtractTraceFromRollingAverage(llrfStructs::rf_trace_data& trace)
{
    // we only subtract AFTER rolling_sum_counter is larger than average_size
    if(trace.rolling_sum_counter > trace.average_size)
        // paranoid sanity check
        if(trace.buffersize >= trace.average_size)
        {
            //debugMessage("shoudlSubtractTraceFromRollingAverage = true");
            return true;
        }
    //debugMessage("shoudlSubtractTraceFromRollingAverage = false");
    return false;
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
        bool a = shouldCheckMasks( llrf.trace_data.at(n));
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
//____________________________________________________________________________________________
void liberallrfInterface::resetAverageTraces()
{
    for(auto && it : llrf.trace_data )
    {
        resetAverageTraces(it.second);
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::resetAverageTraces(llrfStructs::rf_trace_data& trace)
{
    trace.has_average = false;
    trace.rolling_sum_counter = 0;//MAGIC_NUMBER
    // this does rest current_trace, MEANING SOME DATA WILL BE LOST
    // WE COULD DO THIS WITH A MORE SMART METHOD, BUT... the added complexity
    // to implement does not seem worth it
    trace.sub_trace = 0;//MAGIC_NUMBER
    trace.current_trace = 0;//MAGIC_NUMBER
    trace.previous_trace = UTL::MINUS_ONE_INT;//MAGIC_NUMBER
    trace.previous_previous_trace = UTL::MINUS_TWO_INT;//MAGIC_NUMBER
    trace.rolling_average.clear();
    trace.rolling_average.resize(trace.trace_size);
    trace.rolling_sum.clear();
    trace.rolling_sum.resize(trace.trace_size);

    trace.rolling_max.clear();
    trace.rolling_max.resize(trace.trace_size, -std::numeric_limits<double>::infinity());
    trace.rolling_min.clear();
    trace.rolling_min.resize(trace.trace_size, std::numeric_limits<double>::infinity());
    trace.rolling_sd.clear();
    trace.rolling_sd.resize(trace.trace_size);

}
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
size_t liberallrfInterface::getNumBufferTraces(const std::string&name)
{
        const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).buffersize;
    }
    else
        message("liberallrfInterface::getNumBufferTraces ERROR, trace ", n, " does not exist");
    return 0;//MAGIC_NUMBER
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
llrfStructs::outside_mask_trace liberallrfInterface::getOutsideMaskData(const size_t part)
{
    if(part <= llrf.outside_mask_traces.size() - 1)
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
std::vector<double> liberallrfInterface::getTraceValues(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        //return llrf.trace_data.at(name).traces.back().value;
        return llrf.trace_data.at(n).traces[llrf.trace_data.at(name).latest_trace_index].value;
    }
    else
        message("liberallrfInterface::getTraceValues ERROR, trace ", n, " does not exist");
    std::vector<double> r(1, 0);//MAGIC_NUMBER
    return r;
}
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
std::vector<double>liberallrfInterface::getAverageTraceData(const std::string& name)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        if(llrf.trace_data.at(n).rolling_sum_counter > 0)
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
std::vector<double> liberallrfInterface::getProbePower()
{
    return getAverageTraceData(UTL::CAVITY_PROBE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePhase()
{
    return getAverageTraceData(UTL::CAVITY_PROBE_PHASE);
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
    if(value>llrf.maxAmp )
    {
        message("Error!! Requested amplitude, ",value,"  too high");
        return false;
    }
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpFF(double value)
{
    if(value>llrf.maxAmp )
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
    llrf.tracesToSaveOnBreakDown = name;
}
//____________________________________________________________________________________________
void liberallrfInterface::setCrestPhiLLRF(double value) // in LLRF units
{
    llrf.crestPhi = value;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setTraceSCAN(const std::string& name, const llrfStructs::LLRF_SCAN value)
{
    if(entryExists(llrf.trace_data, name))
    {
        for(auto&& it: llrf.pvMonStructs)
        {
            if(it.second.name == name && Is_SCAN_PV(it.first) )
            {
                if(isMonitoring(it.first))
                    return setValue(it.second,value);
            }
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAllTraceSCAN( const llrfStructs::LLRF_SCAN value)
{
    for(auto&& it: llrf.pvMonStructs)
    {
        if(isMonitoring(it.first) && Is_SCAN_PV(it.first) )
            setValue(it.second,value);
    }
    return true;
}
//____________________________________________________________________________________________
bool  liberallrfInterface::setMeanStartIndex(const std::string&name, size_t  value)
{
    if(entryExists(llrf.trace_data, name))
    {
        if( llrf.trace_data.at(name).trace_size - 1 >= value  )
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
        if( llrf.trace_data.at(name).trace_size - 1 >= value  )
        {
            llrf.trace_data.at(name).mean_stop_index = value;
            return true;
        }
    }
    //message(name," NOT FOUND!");
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setHighMask(const std::string&name,const std::vector<double>& value)
{
    if(entryExists(llrf.trace_data, name))
    {
        if( value.size() == llrf.trace_data.at(name).trace_size)
        {
            llrf.trace_data.at(name).high_mask = value;
            llrf.trace_data.at(name).hi_mask_set = true;
            //debugMessage(name, "  llrf.trace_data.at(name).hi_mask_set =  ", llrf.trace_data.at(name).hi_mask_set);
            return true;
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setLowMask(const std::string&name,const std::vector<double>& value)
{
    if(entryExists(llrf.trace_data, name))
    {
        if( value.size() == llrf.trace_data.at(name).trace_size)
        {
            llrf.trace_data.at(name).low_mask = value;
            llrf.trace_data.at(name).low_mask_set = true;
            return true;
        }
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
/// THE NEXT TWO FUNCTIONS COULD BE COMBINED AND NEATENED UP
//____________________________________________________________________________________________
bool liberallrfInterface::setPercentMask(const size_t s1,const size_t s2,const size_t s3,
                                         const size_t s4,const double value2,const  std::string& name)
{
    // automaticall set the mask based on the rolling_average for cavity_rev_power trace
    // between element 0    and s1 will be set to default hi/lo (+/-infinity)
    // between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average
    // between elemnent s2+1 and s3 will be set very default hi/lo (+/-infinity)
    // between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average
    // between element s3+1 and s4 will be set very default hi/lo (+/-infinity)

    const double value = value2 / 100.0;

    if(entryExists(llrf.trace_data, name))
    {
        // if we're keeping an average pulse
        if(llrf.trace_data.at(name).has_average)
        {
            std::vector<double> & ra = llrf.trace_data.at(name).rolling_average;

            // sanity check on s1,s2,s3,s4
            if(0 <= s1 && s4 <= ra.size() - 1)
            {
                if( s1 <= s2 && s2 <= s3 && s3 <= s4)
                {
                    std::vector<double> hi_mask(ra.size(), 0.0);
                    std::vector<double> lo_mask(ra.size(), 0.0);

                    for(auto i = 0; i <= s1; ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    for(auto i = s1; i <= s2; ++i)
                    {
                        hi_mask[i] = ra[i] + ra[i] * value;
                        lo_mask[i] = ra[i] - ra[i] * value;
                    }
                    for(auto i = s2+1; i <= s3; ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    for(auto i = s3+1; i <= s4; ++i)
                    {
                        hi_mask[i] = ra[i] + ra[i] * value;
                        lo_mask[i] = ra[i] - ra[i] * value;
                    }
                    for(auto i = s4+1; i < ra.size(); ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    // apply mask values
                    if(setHighMask(name,hi_mask))
                    {
                        if(setLowMask(name,lo_mask))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAbsoluteMask(const size_t s1,const size_t s2,
                                          const size_t s3,const size_t s4,
                                          const double value2,const  std::string& name)
{
    // automaticall set the mask based on the rolling_average for cavity_rev_power trace
    // between element 0    and s1 will be set to default hi/lo (+/-infinity)
    // between element s1+1 and s2 will be set by rolling_average +/- value
    // between element s2+1 and s3 will be set very default hi/lo (+/-infinity)
    // between element s3+1 and s4 will be set by rolling_average +/- value
    // between element s3+1 and -1 will be set very default hi/lo (+/-infinity

    const double value = value2 / 100.0;

    if(entryExists(llrf.trace_data, name))
    {
        // if we're keeping an average pulse
        if(llrf.trace_data.at(name).has_average)
        {
            std::vector<double> & ra = llrf.trace_data.at(name).rolling_average;

            // sanity check on s1,s2,s3,s4
            if(0 <= s1 && s4 <= ra.size() - 1)
            {
                if( s1 <= s2 && s2 <= s3 && s3 <= s4)
                {
                    std::vector<double> hi_mask(ra.size(), 0.0);
                    std::vector<double> lo_mask(ra.size(), 0.0);

                    for(auto i = 0; i <= s1; ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                        //message(i, ra[i]);
                    }
                    for(auto i = s1+1; i <= s2; ++i)
                    {
                        hi_mask[i] = ra[i] + value;
                        lo_mask[i] = ra[i] - value;
                        //message(i, ra[i]);
                    }
                    for(auto i = s2+1; i <= s3; ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                        //message(i,ra[i]);
                    }
                    for(auto i = s3+1; i <= s4; ++i)
                    {
                        hi_mask[i] = ra[i] + value;
                        lo_mask[i] = ra[i] - value;
                    }
                    for(auto i = s4+1; i < ra.size(); ++i)
                    {
                        hi_mask[i] =   std::numeric_limits<double>::infinity();
                        lo_mask[i] = - std::numeric_limits<double>::infinity();
                    }
                    // apply mask values
                    if(setHighMask(name,hi_mask))
                    {
                        if(setLowMask(name,lo_mask))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::clearMask(const std::string&name)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).high_mask.clear();
        llrf.trace_data.at(name).low_mask.clear();
        llrf.trace_data.at(name).hi_mask_set  = false;
        llrf.trace_data.at(name).low_mask_set = false;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
//bool liberallrfInterface::clearRollingAverage()
//{
//    bool ans  = true;
//    bool temp = false;
//    for( auto&& it: llrf.trace_data)
//    {
//        if(it.keep_rolling_average)
//        {
//            temp = resetAverageTraces(it.name);
//            if( !temp )
//            {
//                ans = false;
//            }
//        }
//    }
//    return ans;
//}
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
void liberallrfInterface::setNumBufferTraces(const size_t value)
{
    for( auto && it: llrf.trace_data )
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
        llrf.trace_data.at(n).traces.resize( llrf.trace_data.at(n).buffersize );

        // update the rf_trace object with the size of a trace and its parent name
        for( auto && it2: llrf.trace_data.at(n).traces)
        {
            it2.value.resize(llrf.trace_data.at(n).trace_size);
            it2.name = n;
        }
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setShouldCheckMask(const std::string&name)
{
    return setCheckMask(name, true);
}
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
void liberallrfInterface::setShouldKeepRollingAverage()
{
    for( auto && it: llrf.trace_data )
    {
        setShouldKeepRollingAverage(it.first);
    }
}
//____________________________________________________________________________________________
void liberallrfInterface::setShouldNotKeepRollingAverage()
{
    for( auto && it: llrf.trace_data )
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
        resetAverageTraces( llrf.trace_data.at(n));
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
void liberallrfInterface::setNumRollingAverageTraces(const size_t value)
{
    for( auto && it: llrf.trace_data )
    {
        setNumRollingAverageTraces(it.first, value);
    }
}
//____________________________________________________________________________________________
bool liberallrfInterface::setNumRollingAverageTraces(const std::string&name,const size_t value )
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        if(value <= llrf.trace_data.at(n).buffersize)
            llrf.trace_data.at(n).average_size = value;
        else
        {
            llrf.trace_data.at(n).average_size = llrf.trace_data.at(n).buffersize;
            message("Number of Rolling Average traces is greater than buffer, using ",llrf.trace_data.at(n).average_size);
        }
        resetAverageTraces(llrf.trace_data.at(n));
        return true;
    }
    else
        message("liberallrfInterface ERROR, trace ", n, " does not exist");
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
        if(val > llrf.maxAmp )
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
bool liberallrfInterface::setPHIDEG()
{// ONLY ever called from staticEntryLLRFMonitor
    double val = (llrf.phi_ff) * (llrf.phiCalibration);
    debugMessage("setPHIDEG PHI_DEG to, ",val, ", calibration = ", llrf.phiCalibration);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::PHI_DEG),val);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAMPMVM()
{// ONLY ever called from staticEntryLLRFMonitor
    double val = (llrf.amp_ff) * (llrf.ampCalibration);
    debugMessage("setAMPMVM AMP_MVM to, ",val, ", calibration = ", llrf.ampCalibration);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::AMP_MVM),val);
}
//____________________________________________________________________________________________
const llrfStructs::liberallrfObject& liberallrfInterface::getLLRFObjConstRef()
{
    return llrf;
}
//____________________________________________________________________________________________
std::vector<std::string> liberallrfInterface::getTracesToSaveOnBreakDown()
{
    return llrf.tracesToSaveOnBreakDown;
}
//____________________________________________________________________________________________
template<typename T>
bool liberallrfInterface::setValue(llrfStructs::pvStruct& pvs, T value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE,pvs.CHID,&value);
    std::stringstream ss;
    ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    debugMessage(ss);
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
    return r;
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
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getSCAN_pv(llrfStructs::LLRF_PV_TYPE pv)
{
    switch(pv)
    {
         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_SCAN;
         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM:
             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_SCAN;
    }
    return llrfStructs::LLRF_PV_TYPE::UNKNOWN;
}
//____________________________________________________________________________________________
void liberallrfInterface::startTimer()
{
    llrf.timer_start = msChronoTime();
}
//____________________________________________________________________________________________
void liberallrfInterface::offsetTimer(long long value)
{
    llrf.timer_start += value;
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
    for(auto && it : continuousMonitorStructs )
    {
        if(it->monType == pv )
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
double liberallrfInterface::getBreakDownRate()
{
    return llrf.breakdown_rate;
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
    if( entryExists(llrf.trace_data,n))
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
bool liberallrfInterface::setDropAmpValue(const std::string& name, double amp_val)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, fullCavityTraceName(n)))
    {
        llrf.trace_data.at(n).amp_drop_value = amp_val;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setDropAmpOnOutsideMaskDetection(const std::string& name,  bool state, double amp_val)
{
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
bool liberallrfInterface::setNumContinuousOutsideMaskCount(const std::string& name, size_t val)
{
    const std::string n = fullCavityTraceName(name);
    if(entryExists(llrf.trace_data, fullCavityTraceName(n)))
    {
        llrf.trace_data.at(n).num_continuous_outside_mask_count = val;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
std::string liberallrfInterface::fullCavityTraceName(const std::string& name)
{
    if( myLLRFType == llrfStructs::LLRF_TYPE::CLARA_HRRG || myLLRFType == llrfStructs::LLRF_TYPE::VELA_HRRG )
    {
        if(name == UTL::CAVITY_REVERSE_PHASE)
            return UTL::HRRG_CAVITY_REVERSE_PHASE;

        if(name == UTL::CAVITY_FORWARD_PHASE)
            return UTL::HRRG_CAVITY_FORWARD_PHASE;

        if(name == UTL::CAVITY_REVERSE_POWER)
            return UTL::HRRG_CAVITY_REVERSE_POWER;

        if( name == UTL::CAVITY_FORWARD_POWER)
            return UTL::HRRG_CAVITY_FORWARD_POWER;
    }
    else if( myLLRFType == llrfStructs::LLRF_TYPE::CLARA_LRRG || myLLRFType == llrfStructs::LLRF_TYPE::VELA_LRRG )
    {
        if(name == UTL::CAVITY_REVERSE_PHASE)
            return UTL::LRRG_CAVITY_REVERSE_PHASE;

        if(name == UTL::CAVITY_FORWARD_PHASE)
            return UTL::LRRG_CAVITY_FORWARD_PHASE;

        if(name == UTL::CAVITY_REVERSE_POWER)
            return UTL::LRRG_CAVITY_REVERSE_POWER;

        if( name == UTL::CAVITY_FORWARD_POWER)
            return UTL::LRRG_CAVITY_FORWARD_POWER;
    }
    return name;
}




