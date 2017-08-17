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
#include <chrono>
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
myLLRFType(type)
{
//    if(shouldStartEPICs )
//    message("magnet liberallrfInterface shouldStartEPICs is true");
//    else
//    message("magnet liberallrfInterface shouldStartEPICs is false");
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
    int status = ca_clear_subscription(ms -> EVID);
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

            if(shouldStartEPICs )
            {
                message("The liberallrfInterface has acquired objects, connecting to EPICS");
                // subscribe to the channel ids
                initChids();
                // start the monitors: set up the callback functions
                debugMessage("Starting Monitors");
                startMonitors();
                // The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds(2000 )); // MAGIC_NUMBER
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
    if( success )
    {
//        llrf.cav_f_power.value.resize(
//            llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_CAV_FWD).COUNT);
//        llrf.cav_r_power.value.resize(
//            llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_CAV_REV).COUNT);
//        llrf.kly_f_power.value.resize(
//            llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_KLY_FWD).COUNT);
//        llrf.kly_r_power.value.resize(
//            llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_KLY_REV).COUNT);
//        llrf.time_vector.value.resize(
//            llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_TIME_VECTOR).COUNT);
//    message("llrf.cav_f_power.size() = ",  llrf.cav_f_power.value.size());
//    message("llrf.cav_r_power.size() = ",  llrf.cav_r_power.value.size());
//    message("llrf.kly_f_power.size() = ",  llrf.kly_f_power.value.size());
//    message("llrf.kly_r_power.size() = ",  llrf.kly_r_power.value.size());
//    message("llrf.time_vector.size() = ",  llrf.time_vector.value.size());
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
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // MAGIC_NUMBER
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
        if(IsNot_TracePV(it.first) )
        {
            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
            continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
            continuousMonitorStructs.back() -> monType    = it.first;
            continuousMonitorStructs.back() -> llrfObj = &llrf;
            continuousMonitorStructs.back() -> interface  = this;
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
                debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
                continuousMonitorStructs.push_back(new llrfStructs::monitorStruct() );
                continuousMonitorStructs.back() -> monType    = it.first;
                continuousMonitorStructs.back() -> llrfObj = &llrf;
                continuousMonitorStructs.back() -> interface  = this;
                ca_create_subscription(it.second.CHTYPE,
                                   it.second.COUNT,
                                   it.second.CHID,
                                   it.second.MASK,
                                   liberallrfInterface::staticEntryLLRFMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back() -> EVID);
            }
        }
    }
    int status = sendToEpics("ca_create_subscription", "Succesfully Subscribed to LLRF Trace Monitors", "!!TIMEOUT!! Subscription to LLRF Trace monitors failed" );
    if (status == ECA_NORMAL )
        debugMessage("All LLRF Trace Monitors Started");

}
//____________________________________________________________________________________________
bool liberallrfInterface::startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
{
    if(Is_TracePV(pv) )
    {
        if(isNotMonitoring(pv) )
        {
            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(pv));
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
        }
        std::stringstream ss;
        ss <<"Succesfully Subscribed to LLRF Trace Monitor " << ENUM_TO_STRING(pv);
        std::string s1 = ss.str();
        ss.str(std::string());
        ss <<"!!TIMEOUT!! Subscription to LLRF Trace monitor " << ENUM_TO_STRING(pv) << " failed";
        std::string s2 = ss.str();

        int status = sendToEpics("ca_create_subscription",s1.c_str(),s2.c_str());
        if (status == ECA_NORMAL )
            debugMessage(ENUM_TO_STRING(pv)," Monitor Started");
        return isMonitoring(pv);
    }
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::startTraceMonitoring(const std::string& name)
{
    return startTraceMonitoring( getLLRFPVType(name) );
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
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getLLRFPVType(const std::string& name)
{
    for(auto && it:llrf.pvMonStructs )
    {
        if( it.second.name == name )
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
    bool a1 = startTraceMonitoring(UTL::CAVITY_FORWARD_PHASE);
    bool a2 = startTraceMonitoring(UTL::CAVITY_FORWARD_POWER);
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::startCavRevTraceMonitor()
{
    bool a1 = startTraceMonitoring(UTL::CAVITY_REVERSE_PHASE);
    bool a2 = startTraceMonitoring(UTL::CAVITY_REVERSE_POWER);
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
    return stopTraceMonitoring( getLLRFPVType(name) );
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
    bool a1 = stopTraceMonitoring(UTL::CAVITY_FORWARD_PHASE);
    bool a2 = stopTraceMonitoring(UTL::CAVITY_FORWARD_POWER);
    if(a1 && a2)
        return true;
    else
        return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::stopCavRevTraceMonitor()
{
    bool a1 = stopTraceMonitoring(UTL::CAVITY_REVERSE_PHASE);
    bool a2 = stopTraceMonitoring(UTL::CAVITY_REVERSE_POWER);
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
        ms->interface->updateTrace(args,ms->llrfObj->trace_data.at( ms->name ) );
    switch(ms -> monType)
    {
        case llrfStructs::LLRF_PV_TYPE::LIB_LOCK:
            //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_LOCK = ",*(bool*)args.dbr);
            ms->interface->llrf.islocked = *(bool*)args.dbr;
            break;
        case llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF:
            //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_AMP_FF = ",*(double*)args.dbr);
            ms->llrfObj->amp_ff = *(double*)args.dbr;
            ms->llrfObj->amp_MVM = (ms->llrfObj->amp_ff) * (ms->llrfObj->ampCalibration);
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
            //ms->interface->debugMessage("staticEntryLLRFMonitor LIB_TIME_VECTOR = ",*(double*)args.dbr);
            ms->interface->updateTrace(args,ms->llrfObj->time_vector);
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
}
//____________________________________________________________________________________________
void liberallrfInterface::updateTrace(const event_handler_args& args,llrfStructs::rf_trace_data& trace)
{
    trace.traces.push_back(llrfStructs::rf_trace());
    trace.traces.back().value.resize(trace.trace_size);
    updateValues(args, trace.traces.back());

    // check mask
    if( shouldCheckMasks(trace) )
    {
        bool trace_good = checkTraceIsInMask(trace);
    }
    if( trace.keep_rolling_average )
    {
        bool trace_good = checkTraceIsInMask(trace);
    }

}
//____________________________________________________________________________________________
bool liberallrfInterface::checkTraceIsInMask(llrfStructs::rf_trace_data& trace)
{
    return false;
}
//____________________________________________________________________________________________
//____________________________________________________________________________________________
void liberallrfInterface::calcRollingAverage(llrfStructs::rf_trace_data& trace)
{
    //
}
//____________________________________________________________________________________________
bool liberallrfInterface::shouldCheckMasks(llrfStructs::rf_trace_data& trace)
{
    return trace.check_mask && trace.hi_mask_set && trace.low_mask_set;
}
//____________________________________________________________________________________________
void liberallrfInterface::updateValues(const event_handler_args& args,llrfStructs::rf_trace& trace)
{
    if(isTimeType(args.type) )
    {
        const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
        size_t counter = 0;
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
        size_t i = 0;
        for(auto && it : trace.value)
        {
            it = *((double*)args.dbr + i);
    //        std::cout << trace.second[i] << " ";
            ++i;
        }
    }
    trace.shot += 1;// MAGIC_NUMBER
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
std::vector<double> liberallrfInterface::getHighMask(const std::string&name)
{
    if(entryExists(llrf.trace_data, name))
    {
        return llrf.trace_data.at(name).high_mask;
    }
    std::vector<double> r(1, 0);
    return r;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getLowMask(const std::string&name)
{
    if(entryExists(llrf.trace_data, name))
    {
        return llrf.trace_data.at(name).low_mask;
    }
    std::vector<double> r(1, 0);
    return r;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getNumBufferTraces(const std::string&name)
{
    if(entryExists(llrf.trace_data, name))
    {
        return llrf.trace_data.at(name).buffersize;
    }
    return 0;
}
//____________________________________________________________________________________________
bool liberallrfInterface::isLocked()
{
    return llrf.islocked;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_TYPE liberallrfInterface::getType()
{
    return llrf.type;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getPowerTraceLength()
{
    return llrf.powerTraceLength;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getTraceValues(const std::string& name)
{
    if(entryExists(llrf.trace_data,name))
    {
        return llrf.trace_data.at(name).traces.back().value;
    }
    std::vector<double> r(1, 0);
    return r;
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getTraceData(const std::string& name)
{
    if(entryExists(llrf.trace_data,name))
    {
        return llrf.trace_data.at(name).traces.back();
    }
    llrfStructs::rf_trace r;
    return r;
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getTraceBuffer(const std::string& name)
{
    if(entryExists(llrf.trace_data,name))
    {
        return llrf.trace_data.at(name).traces;
    }
    std::deque<llrfStructs::rf_trace> r;
    return r;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavRevPower()
{
    return getTraceValues(UTL::CAVITY_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavRevPhase()
{
    return getTraceValues(UTL::CAVITY_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavFwdPower()
{
    return getTraceValues(UTL::CAVITY_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getCavFwdPhase()
{
    return getTraceValues(UTL::CAVITY_FORWARD_PHASE);
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
    return getTraceData(UTL::CAVITY_REVERSE_POWER);
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavRevPhaseData()
{
    return getTraceData(UTL::CAVITY_REVERSE_PHASE);
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavFwdPowerData()
{
    return getTraceData(UTL::CAVITY_FORWARD_POWER);
}
//____________________________________________________________________________________________
llrfStructs::rf_trace liberallrfInterface::getCavFwdPhaseData()
{
    return getTraceData(UTL::CAVITY_FORWARD_PHASE);
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
std::deque<llrfStructs::rf_trace> liberallrfInterface::getCavRevPowerBuffer()
{
    return getTraceBuffer(UTL::CAVITY_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getCavRevPhaseBuffer()
{
    return getTraceBuffer(UTL::CAVITY_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getCavFwdPowerBuffer()
{
    return getTraceBuffer(UTL::CAVITY_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getCavFwdPhaseBuffer()
{
    return getTraceBuffer(UTL::CAVITY_FORWARD_PHASE);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getKlyRevPowerBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_REVERSE_POWER);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getKlyRevPhaseBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_REVERSE_PHASE);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getKlyFwdPowerBuffer()
{
    return getTraceBuffer(UTL::KLYSTRON_FORWARD_POWER);
}
//____________________________________________________________________________________________
std::deque<llrfStructs::rf_trace> liberallrfInterface::getKlyFwdPhaseBuffer()
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
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_AMP_SP),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpFF(double value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_AMP_FF),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAmpLLRF(double value)
{
    return setAmpFF(value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setPhiLLRF(double value)
{
    return setPhiFF(value);
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
void liberallrfInterface::setCrestPhiLLRF(double value) // in LLRF units
{
    llrf.crestPhi = value;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setHighMask(const std::string&name, std::vector<double>& value)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).high_mask = value;
        llrf.trace_data.at(name).hi_mask_set  = true;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setLowMask(const std::string&name, std::vector<double>& value)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).low_mask = value;
        llrf.trace_data.at(name).low_mask_set = true;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setNumBufferTraces(const std::string&name, size_t value)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).buffersize = value;

        // and set their sizes based on buffersize and COUNT

        llrf.trace_data.at(name).traces.clear();
        llrf.trace_data.at(name).traces.resize( llrf.trace_data.at(name).buffersize );
//        for( auto && it2: llrf.trace_data.at(name).traces)
//            it2.vresize(it.COUNT);
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCheckMask(const std::string&name, bool value = true)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).check_mask = value;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setKeepRollingAverage(const std::string&name, bool value = true)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).keep_rolling_average = value;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setNumRollingAverageTraces(const std::string&name, size_t value )
{
    if(entryExists(llrf.trace_data, name))
    {
        if(value <= llrf.trace_data.at(name).buffersize)
            llrf.trace_data.at(name).average_size = value;
        else
            llrf.trace_data.at(name).average_size = llrf.trace_data.at(name).buffersize;
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
    return isMonitoring( getLLRFPVType(name));
}
//____________________________________________________________________________________________
bool liberallrfInterface::isNotMonitoring(const std::string& name)
{
    return isNotMonitoring( getLLRFPVType(name));
}








