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
//  Last edit:   22-01-2019
//  FileName:    liberallrfInterface.cpp
//  Description:
//
//
//
//*/

#include "liberallrfInterface.h"
#include "configDefinitions.h"
/* STL */
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <thread>
#include <math.h>
#include <utility> // std::pair (!)
#include <mutex>
#include <chrono>
#include <stdlib.h>

#include <bitset>

/* seems like we might be in some state of mutex-hell here, wher eto use them adn where not?? */
std::mutex mtx;    // mutex for critical section
/* boost */
#include <boost/circular_buffer.hpp>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
liberallrfInterface::liberallrfInterface(const std::string &llrfConf,
                                         const bool startVirtualMachine,
                                         const bool& show_messages,
                                         const bool& show_debug_messages,
                                         const bool  shouldStartEPICs ,
                                         const llrfStructs::LLRF_TYPE type
                                         ):
configReader(llrfConf,startVirtualMachine,show_messages,show_debug_messages),
interface(show_messages,show_debug_messages),
shouldStartEPICs(shouldStartEPICs),
usingVirtualMachine(startVirtualMachine),
myLLRFType(type),
dummy_trace_data(llrfStructs::rf_trace_data())
{
    std::get<UTL::ZERO_SIZET>(dummy_amp_set_kly_fwd_rs_state) = UTL::ZERO_SIZET;
    std::get<UTL::ONE_SIZET>(dummy_amp_set_kly_fwd_rs_state) = UTL::ZERO_DOUBLE;
    std::get<UTL::TWO_SIZET>(dummy_amp_set_kly_fwd_rs_state) = UTL::ZERO_DOUBLE;
    /* set the llrf type of the config reader */
    configReader.setType(type);
    initialise();
}
//
// ________                   __
// \______ \   ____   _______/  |________  ____ ___.__.
//  |    |  \_/ __ \ /  ___/\   __\_  __ \/  _ <   |  |
//  |    `   \  ___/ \___ \  |  |  |  | \(   <_>)___  |
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
    debugMessage("liberallrfInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void liberallrfInterface::killMonitor(llrfStructs::monitorStruct * ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
    {
        delete ms;
    }
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
            if(Is_Time_Vector_PV(it.first))
            {
                llrf.time_vector.resize(it.second.COUNT);
            }
            if(Is_Pulse_Shape_Vector_PV(it.first))
            {
                llrf.pulse_shape.resize(it.second.COUNT);
            }
        }
        /*
            Here we are calculating the indices in the ALL-TRACES RECORD, that correpsond to each
            individual trace
        */
        using namespace UTL;
        std::pair<size_t, size_t> temp;
        for(auto i = ZERO_SIZET; i < llrf.all_traces.num_traces; ++i)
        {
            temp.first  = i * llrf.all_traces.num_elements_per_trace + llrf.all_traces.num_start_zeroes_per_trace;
            temp.second = (i + ONE_SIZET) * llrf.all_traces.num_elements_per_trace - ONE_SIZET;
            llrf.all_traces.trace_indices.push_back(temp);
            //message("Trace ", i , ": ",temp.first , " - ", temp.second );
        }
        /*
            Here we print out where we think each trace is in the ONE RECORD,
            and what its indicies are in the ONE RECORD
        */
        message("\nPrinting Offline 'One Record' Trace Information\n");
        for(auto&& it: llrf.all_traces.trace_info)
        {
            message(it.second.name," is a ", ENUM_TO_STRING(it.second.type)," trace, position = ",it.second.position,
                    ", indices = ",
                    llrf.all_traces.trace_indices[it.second.position].first,
                    " / ",
                    llrf.all_traces.trace_indices[it.second.position].second);
            it.second.start_index = llrf.all_traces.trace_indices[it.second.position].first;
            it.second.stop_index = llrf.all_traces.trace_indices[it.second.position].second;
        }
        /*
            Initialise the Trace Data buffers

            First the individual trace buffers, these are held in the top level llrfobject
            we use llrf.all_traces.trace_info to set these

            Second the AllTrace Buffer, (this is genreally much larger so we can dump more data)
        */
        // some temp containersd to copy to new trace_data entries
        std::vector<double> t( llrf.all_traces.num_elements_per_trace_used, UTL::ZERO_DOUBLE );
        for(auto&& it: llrf.all_traces.trace_info)
        {
            // create the entry in the map
            llrf.trace_data[it.second.name].name = it.second.name;
            // initilise the vector for this entry
            llrf.trace_data.at(it.second.name).hi_mask = t;
            llrf.trace_data.at(it.second.name).lo_mask = t;
            llrf.trace_data.at(it.second.name).rolling_average = t;
            llrf.trace_data.at(it.second.name).rolling_sum = t;
            llrf.trace_data.at(it.second.name).rolling_max = t;
            llrf.trace_data.at(it.second.name).rolling_min = t;
            llrf.trace_data.at(it.second.name).rolling_sd  = t;

            /* THERE'S SEPERATE FUNCTION TO INIT THE TRACE BUFFER AS IT CAN BE CALLED FROM PYTHON*/
            setIndividualTraceBufferSize( it.second.name, llrf.trace_data.at(it.second.name).buffersize );
        }
        /* sanity check */
        if( setAllTraceBufferSize(llrf.all_traces.buffer_size) )
        {
            message("Set all_trace buffer size to ", llrf.all_traces.buffer_size );
        }
        else
        {
            message("Error setting all_trace buffer size to ", llrf.all_traces.buffer_size );
        }
    }
    llrf.type = myLLRFType;
    /* set the interface */
    llrf.interface = this;
    return success;
}
//______________________________________________________________________________
void liberallrfInterface::initChids()
{
    message("\n", "liberallrfInterface is searching for LLRF ChIds...");
    for(auto && it:llrf.pvMonStructs)
    {
        addChannel(llrf.pvRoot, it.second);
    }
    /* command only PVs for the LLRF to set "high level" phase and amplitide */
    for(auto && it:llrf.pvComStructs)
    {
        addChannel(llrf.pvRoot, it.second);
    }
    for(auto && it:llrf.pvOneTraceMonStructs)
    {
        addChannel(llrf.pvRoot + llrf.pvRoot_oneTrace, it.second);
    }
    for(auto && it:llrf.pvOneTraceComStructs)
    {
        addChannel(llrf.pvRoot + llrf.pvRoot_oneTrace, it.second);
    }
    /* for the scans: loop over each SCAN object and then include the main pvroot, the scan pvroot then the pvsuffix */
    for(auto && it:llrf.trace_scans)
    {
        for(auto && it2: it.second.pvSCANStructs)
        {
            addChannel(llrf.pvRoot + it.second.pvroot, it2.second);
        }
    }
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
        /* SCANS only have one flavour */
        for(auto &&it:llrf.trace_scans)
        {
            message("CHECK CHIDS FOR ", it.first);
            for(auto && it2: it.second.pvSCANStructs)
            {
                checkCHIDState(it2.second.CHID, ENUM_TO_STRING(it2.first));
            }
        }

        for(auto && it : llrf.pvOneTraceMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto && it : llrf.pvOneTraceComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        /* pause */
        pause_2000();
    }
    else if (status == ECA_NORMAL)
    {
        allChidsInitialised = true;  // interface base class member
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::addChannel(const std::string & pvRoot, llrfStructs::pvStruct& pv)
{
    std::string s;
    if(pv.pvType == llrfStructs::LLRF_PV_TYPE::AMP_MVM)
    {
        s =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else if(pv.pvType == llrfStructs::LLRF_PV_TYPE::PHI_DEG)
    {
        s =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else
    {
        s = pvRoot + pv.pvSuffix;
    }
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_99, &pv.CHID);
    debugMessage("Create channel to ", s);
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto&& it:llrf.pvMonStructs)
    {
        addTo_continuousMonitorStructs(it);
    }
    for(auto&& it:llrf.pvOneTraceMonStructs)
    {
        addTo_continuousMonitorStructs(it);
    }
    for(auto&& it:llrf.trace_scans)
    {
        //debugMessage("creating subscriptions to ", it.first);
        for(auto&& it2: it.second.pvSCANStructs)
        {
            addTo_continuousMonitorStructs(it2);
            continuousMonitorStructs.back()->name = it.first;
//            message("Using ", continuousMonitorStructs.back() -> name,
//                    " and ",  ENUM_TO_STRING(continuousMonitorStructs.back() -> monType));
        }
    }
    int status = sendToEpics("ca_create_subscription",
                             "liberallrfInterface succesfully subscribed to LLRF monitors.",
                             "!!TIMEOUT!! Subscription to LLRF monitors failed, liberallrfInterface");
    if (status == ECA_NORMAL)
    {
        allMonitorsStarted = true; // interface base class member
    }
/*
//    LOOP OVER ALL CONTINUOUS MONITOR STRUCTS TO GET names debugging
//    for(auto&&it:continuousMonitorStructs)
//    {
//        message(it->name," with ", ENUM_TO_STRING(it -> monType) );
//    }
*/
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::addTo_continuousMonitorStructs(const std::pair<llrfStructs::LLRF_PV_TYPE,llrfStructs::pvStruct>& it)
{
    debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
    continuousMonitorStructs.push_back(new llrfStructs::monitorStruct());
    continuousMonitorStructs.back() -> monType   = it.first;
    continuousMonitorStructs.back() -> llrfObj   = &llrf;
    continuousMonitorStructs.back() -> interface = this;
    continuousMonitorStructs.back() -> name      = ENUM_TO_STRING(llrf.type);
    ca_create_subscription(it.second.CHTYPE,
                           it.second.COUNT,
                           it.second.CHID,
                           it.second.MASK,
                           liberallrfInterface::staticEntryLLRFMonitor,
                           (void*)continuousMonitorStructs.back(),
                           &continuousMonitorStructs.back() -> EVID);
}
//--------------------------------------------------------------------------------------------------
//
// _________        .__  .__ ___.                  __
// \_   ___ \_____  |  | |  |\_ |__ _____    ____ |  | __
// /    \  \/\__  \ |  | |  | | __ \\a__  \ _/ ___\|  |/ /
// \     \____/ __ \|  |_|  |_| \_\ \/ __ \\  \___|    <
//  \______  (____  /____/____/___  (____  /\___  >__|_ \
//         \/     \/              \/     \/     \/     \/
//
// These functions are called from EPICS subscriptions
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::staticEntryLLRFMonitor(const event_handler_args args)
{
    llrfStructs::monitorStruct*ms = static_cast<llrfStructs::monitorStruct *>(args.usr);
    ms->interface->updateLLRFValue(ms->monType, ms->name, args);
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateLLRFValue(const llrfStructs::LLRF_PV_TYPE pv, const std::string& objName,
                                          const event_handler_args& args)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    //message("callback ", ENUM_TO_STRING(pv), " ", objName );
    using namespace llrfStructs;
    switch(pv)
    {
        case LLRF_PV_TYPE::PULSE_SHAPE:
            updatePulseShape(args);
            break;
        case LLRF_PV_TYPE::PULSE_SHAPE_APPLY:
            updateBoolState(args, llrf.pulse_shape_up_to_date);
            break;

        case LLRF_PV_TYPE::PHASE_LOOP_LOCK:
            updateBoolState(args, llrf.phase_loop_lock);
            break;
//        This was a special PV graham coz gave me to get som etiming signal, but i don't use it anymore, maybe in the future i will
//        case LLRF_PV_TYPE::TIMING_TRIGGER:
//            llrf.timing_trigger = *(short*)args.dbr;
//            break;
        case LLRF_PV_TYPE::TRIG_SOURCE:
            updateTrigState(args);
            message("TRIG_SOURCE = ", ENUM_TO_STRING(llrf.trig_source));

            break;
        case LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE:
            updateBoolState(args, llrf.ff_amp_lock_state);
            message("LIB_FF_AMP_LOCK_STATE = ", llrf.ff_amp_lock_state);
            break;
        case LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE:
            updateBoolState(args, llrf.ff_ph_lock_state);
            message("LIB_FF_PHASE_LOCK_STATE = ", llrf.ff_ph_lock_state);
            break;
        case LLRF_PV_TYPE::LIB_RF_OUTPUT:
            updateBoolState(args, llrf.rf_output);
            message("LIB_RF_OUTPUT = ", llrf.rf_output);

            break;
        case LLRF_PV_TYPE::LIB_AMP_FF:
            llrf.amp_ff = getDBRdouble(args);
            llrf.amp_MVM = (llrf.amp_ff) * (llrf.ampCalibration);
            break;
        case LLRF_PV_TYPE::LIB_AMP_SP:
            llrf.amp_sp = getDBRdouble(args);
            llrf.amp_MVM = (llrf.amp_sp) * (llrf.ampCalibration);
            break;
        case LLRF_PV_TYPE::LIB_PHI_FF:
            llrf.phi_ff = getDBRdouble(args);
            llrf.phi_DEG = (llrf.phi_ff) * (llrf.phiCalibration);
            break;
        case LLRF_PV_TYPE::LIB_PHI_SP:
            {

            llrf.phi_sp = getDBRdouble(args);
            llrf.phi_DEG = (llrf.phi_sp) * (llrf.phiCalibration);
            message("LLRF_PV_TYPE::LIB_PHI_SP = ", llrf.phi_sp);

            double t = *(double*)args.dbr;

            std::cout << std::bitset<32>(*(double*)args.dbr) << std::endl;

            }
            break;
        case LLRF_PV_TYPE::LIB_TIME_VECTOR:
            /* the time_vector is simlar to a trace, but we can just update the values straight away */
            updateTimeVector(args);
            break;
        case LLRF_PV_TYPE::LIB_PULSE_LENGTH:
            /*Dec 2018, Jan 2019, this stopped working, (number was nonsense, then... it started working again, with no change to this code!!!*/
            llrf.pulse_length = getDBRdouble(args);
            break;
        case LLRF_PV_TYPE::LIB_PULSE_OFFSET:
            llrf.pulse_offset = getDBRdouble(args);
            break;
        case LLRF_PV_TYPE::ALL_TRACES:
             updateAllTraces(args);
             break;
        case LLRF_PV_TYPE::ALL_TRACES_SCAN:
             llrf.all_traces.scan = getSCAN(args);
             break;
        case LLRF_PV_TYPE::ALL_TRACES_ACQM:
             llrf.all_traces.acqm = getACQM(args);
             break;
        case LLRF_PV_TYPE::ALL_TRACES_EVID:
            updateAllTracesEVID(args);
            break;
        case LLRF_PV_TYPE::LIB_PWR_REM_SCAN:
            updateSCAN(objName, pv, args);
            break;
        case LLRF_PV_TYPE::LIB_PHASE_REM_SCAN:
            updateSCAN(objName, pv, args);
            break;
        case LLRF_PV_TYPE::LIB_AMP_DER_SCAN:
            updateSCAN(objName, pv, args);
            break;
        case LLRF_PV_TYPE::LIB_PHASE_DER_SCAN:
            updateSCAN(objName, pv, args);
            break;
        case LLRF_PV_TYPE::LIB_PWR_LOCAL_SCAN:
            updateSCAN(objName, pv, args);
            break;
        case LLRF_PV_TYPE::INTERLOCK:
            switch(getDBRunsignedShort(args))
            {
                case 0: llrf.interlock_state = llrfStructs::INTERLOCK_STATE::NON_ACTIVE; break;
                case 1: llrf.interlock_state = llrfStructs::INTERLOCK_STATE::ACTIVE; break;
                default: llrf.interlock_state = llrfStructs::INTERLOCK_STATE::UNKNOWN_INTERLOCK_STATE;
            }
            message("INTERLOCK = ", ENUM_TO_STRING(llrf.interlock_state));

            break;
        default:
            debugMessage("ERROR updateLLRFValue passed Unknown PV Type = ", ENUM_TO_STRING(pv));
    }
    //message("callback ", ENUM_TO_STRING(pv), " FIN");
}
/*
     ___  __     __   __      __                  __        __                __   __       ___  ___  __
    |__  |__) | /  ` /__`    /  `  /\  |    |    |__)  /\  /  ` |__/    |  | |__) |  \  /\   |  |__  /__`
    |___ |    | \__, .__/    \__, /~~\ |___ |___ |__) /~~\ \__, |  \    \__/ |    |__/ /~~\  |  |___ .__/

*/
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateTimeVector(const event_handler_args& args)
{
    // mutex from updateLLRFValue()
    const dbr_double_t* p_data = (const dbr_double_t*)args.dbr;
    std::copy(p_data, p_data + llrf.time_vector.size(), llrf.time_vector.begin());
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updatePulseShape(const event_handler_args& args)
{
    // mutex from updateLLRFValue()
    const dbr_double_t* p_data = (const dbr_double_t*)args.dbr;
    std::copy(p_data, p_data + llrf.pulse_shape.size(), llrf.pulse_shape.begin());
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateSCAN(const std::string& name, const llrfStructs::LLRF_PV_TYPE pv, const event_handler_args& args)
{
    // mutex from updateLLRFValue()
    if(entryExists(llrf.trace_scans, name))
    {
        if(entryExists(llrf.trace_scans.at(name).value,pv))
        {
            llrf.trace_scans.at(name).value.at(pv) = getSCAN(args);
        }
        else
        {
            message(pv, " is not in trace_scans(",name,")");
        }
    }
    else
    {
        message(name, " is not in trace_scans");
    }
    //message(name, " ", ENUM_TO_STRING(pv), " = ", ENUM_TO_STRING(llrf.trace_scans.at(name).value.at(pv))  );
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateAllTraces(const event_handler_args& args)
{
    // mutex from updateLLRFValue()

    /* push back temp data to all_traces.data_buffer */
    llrf.all_traces.data_buffer.push_back( temp_all_trace_item );
    /*
        Pointer to the data + timestamp
    */
    const dbr_time_double* p_data = (const struct dbr_time_double*)args.dbr;
    /*
        Get timestamp and add to data_buffer
    */
    llrf.all_traces.data_buffer.back().first = p_data->stamp;
    if(llrf.pulse_count == UTL::ZERO_SIZET)
    {
        llrf.all_traces.trace_time_0 = llrf.all_traces.data_buffer.back().first;
    }
    /*
        Get the data and add to data_buffer, this assumes the data is 'new'
        there is a way to set up the LLRF such that it sends the same data multiple times
    */
    const dbr_double_t* pValue;
    pValue = &p_data->value;

    std::copy(pValue, pValue + llrf.all_traces.data_buffer.back().second.size(),
                  llrf.all_traces.data_buffer.back().second.begin());
    /*
        Update llrf.pulse_count
    */
    llrf.pulse_count += UTL::ONE_SIZET;
    /*
        copy data to individual trace buffers
    */
    for(auto&& it: llrf.all_traces.trace_info)
    {
        //message(it.first);
        /* push_back temp data */
        llrf.trace_data.at(it.first).data_buffer.push_back( temp_trace_item );

        /* copy timestamp to trace data_buffer*/
        llrf.trace_data.at(it.first).data_buffer.back().first = llrf.all_traces.data_buffer.back().first;
        /* copy data to trace data_buffer */
        auto data_start = llrf.all_traces.data_buffer.back().second.begin() + llrf.all_traces.trace_indices.at(it.second.position).first;
        auto data_end   = data_start + llrf.all_traces.num_elements_per_trace_used;
        std::copy( data_start, data_end, llrf.trace_data.at(it.first).data_buffer.back().second.begin() );
        /*
            If this is a power trace then square it and divide by 100
            ELSE if it is a phase trace add 180, so that phase goes from 0 to 360 degrees
            (positive numbers are easier to handle compared with -180 to 180
        */
        if( it.second.type == llrfStructs::TRACE_TYPE::POWER)
        {
            for( auto &v : llrf.trace_data.at(it.first).data_buffer.back().second )
            {
                v *= v;
                v /= UTL::ONEHUNDRED_DOUBLE;
            }
        }
        else if(  it.second.type == llrfStructs::TRACE_TYPE::PHASE )
        {
            for( auto &v : llrf.trace_data.at(it.first).data_buffer.back().second )
            {
                v += UTL::ONEEIGHTY_DOUBLE;
            }
            //unwrapPhaseTrace(llrf.trace_data.at(it.first));
        }
        // print max value for each trace
//        message(*std::max_element(llrf.trace_data.at(it.first).data_buffer.back().second.begin(),
//         llrf.trace_data.at(it.first).data_buffer.back().second.begin()+1000));
    }
    /* CHECK_KLYSTRON_FWD_PWR for active pulse */
    llrf.kly_fwd_power_max = getKlyFwdPwrMax();
    updateActivePulses();
    /*
        NOW WE FOLLOW LC PROCEDURE
        if collecting future traces, collect and  increment counter  */
    checkCollectingFutureTraces();

    /* if we have an active pulse .. */
    if(llrf.can_increase_active_pulses)
    {
        /* ... checkForOutsideMaskTrace */
        checkForOutsideMaskTrace();
        /* update masks UNLESS there is was new_outside_mask_event */
        if(llrf.new_outside_mask_event)
        {
            //
        }
        else
        {
            updateRollingAveragesAndMasks();
            /*
                if we have can_increase_active_pulses and there was no
                event update the amp_set kly_fwd_pwr running stats
            */
            update_amp_set_kly_fwd_rs();
        }
    }
    else
    {
        //message("can_increase_active_pulses = false");
    }
    /*
        Update the mean values for a trace
    */
    updateTraceCutMeans();
    /*
        estimate trace DAQ frequency
    */
    updateDAQFreqEstimate();
    /*
        by the time we get to here this MUST be false!
    */
    llrf.new_outside_mask_event = false;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateDAQFreqEstimate()
{
    // mutex from updateLLRFValue()
    /* add current tim eto vetcor of times */
    llrf.trace_times_for_rep_rate.push_back(std::chrono::high_resolution_clock::now()) ;
    /* if vector has desired number of entries calc rep -rate */
    if(llrf.trace_times_for_rep_rate.size() > llrf.num_traces_to_estimate_rep_rate)
    {
        /* pop first entry */
        llrf.trace_times_for_rep_rate.erase(llrf.trace_times_for_rep_rate.begin());
        /* time diff (seconds) between first adn last entry */
        std::chrono::duration<double> diff = llrf.trace_times_for_rep_rate.back() -  llrf.trace_times_for_rep_rate.front();
        /* divide the number of entries by diff to give rep-rate in hertz */
        llrf.trace_rep_rate = (double)llrf.trace_times_for_rep_rate.size() / diff.count();
        //message("llrf.trace_rep_rate = ", llrf.trace_rep_rate);
    }
}

//--------------------------------------------------------------------------------------------------
llrfStructs::LLRF_ACQM  liberallrfInterface::getACQM(const event_handler_args& args)const
{
    // mutex from updateLLRFValue()
    using namespace llrfStructs;
    switch( getDBRunsignedLong(args) )
    {
        case 0:
            return LLRF_ACQM::UNKNOWN_ACQM;
        case 1:
            return LLRF_ACQM::ACQM_NOW;
        case 2:
            return LLRF_ACQM::ACQM_EVENT;
        default:
            return LLRF_ACQM::UNKNOWN_ACQM;
    }
}
//--------------------------------------------------------------------------------------------------
llrfStructs::LLRF_SCAN liberallrfInterface::getSCAN(const event_handler_args& args)const
{
    // mutex from updateLLRFValue()
    using namespace llrfStructs;
    switch( getDBRunsignedLong(args) )
    {
        case 0:
            return LLRF_SCAN::PASSIVE;
        case 1:
            return LLRF_SCAN::EVENT;
        case 2:
            return LLRF_SCAN::IO_INTR;
        case 3:
            return LLRF_SCAN::TEN;
        case 4:
            return LLRF_SCAN::FIVE;
        case 5:
            return LLRF_SCAN::TWO;
        case 6:
            return LLRF_SCAN::ONE;
        case 7:
            return LLRF_SCAN::ZERO_POINT_FIVE;
        case 8:
            return LLRF_SCAN::ZERO_POINT_TWO;
        case 9:
            return LLRF_SCAN::ZERO_POINT_ONE;
        case 10:
            return LLRF_SCAN::ZERO_POINT_ZERO_FIVE;
        default:
            return LLRF_SCAN::ZERO_POINT_ZERO_FIVE;
    }
}





//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setNumTracesToEstimateRepRate(size_t value)
{
    llrf.num_traces_to_estimate_rep_rate = value;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getNumTracesToEstimateRepRate() const
{
    return llrf.num_traces_to_estimate_rep_rate;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getTraceRepRate() const
{
    return llrf.trace_rep_rate;
}

//--------------------------------------------------------------------------------------------------
void liberallrfInterface::update_amp_set_kly_fwd_rs()
{
    if(llrf.keep_kly_fwd_pow_running_stat)
    {
        runningStat& rs_ref = llrf.amp_set_kly_fwd_rs[(int)llrf.amp_sp];
        std::tuple<size_t, double, double>& rs_state_ref = llrf.amp_set_kly_fwd_rs_state[(int)llrf.amp_sp];

        rs_ref.Push( llrf.trace_data.at(UTL::KLYSTRON_FORWARD_POWER).mean );

        rs_ref.getCurrentState(std::get<UTL::ZERO_SIZET>(rs_state_ref),
                               std::get<UTL::ONE_SIZET>(rs_state_ref),
                               std::get<UTL::TWO_SIZET>(rs_state_ref));
//
//        message("new val  = ", llrf.trace_data.at(UTL::KLYSTRON_FORWARD_POWER).mean,
//                ", count    = ", std::get<UTL::ZERO_SIZET>(rs_state_ref),
//                ", old_mean = ", std::get<UTL::ONE_SIZET>(rs_state_ref),
//                ", old_var  = ", std::get<UTL::TWO_SIZET>(rs_state_ref)
//        );
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateRollingAveragesAndMasks()
{
    for(auto&&it:llrf.trace_data)
    {
        //message("updateTraceRollingAverage START ", it.first);
        updateTraceRollingAverage(it.second);
        //message("updateTraceRollingAverage FIN ", it.first);
        /*
            we probably want to check that the global check mask is true here,
            we DONT want to update masks when the RF has been switched off.
        */
        if( it.second.check_mask && it.second.has_average )
        {
            //message("UPDATE MASK START ", it.first);
            setNewMask(it.second);
            //message("UPDATE MASK FIN", it.first);
        }
    }
}
//--------------------------------------------------------------------------------------------------
// THIS FUNCTION ACTUALLY SETS THE MASK VALUES BASED ON THE ROLLING AVERAGE AND
// THE USER SEPCIFIED MASK VALUES
void liberallrfInterface::setNewMask(llrfStructs::rf_trace_data& data)
{
    // this function assumes everything is good to set a new mask
    // automatically set the mask based on the rolling_average
    //
    // 1) element 0                   and mask_start        set to default hi/lo (+/-inf)
    // 2) element mask_start+1        and mask_window_start set by rolling_average +/- mask_value
    // 3) element mask_window_start+1 and mask_window_end   set very default hi/lo (+/-inf)
    // 4) element mask_window_end+1   and mask_end          set by rolling_average +/- mask_value
    // 5) element mask_end+1          and end               set very default hi/lo (+/-inf)

    //message(data.name," setting new mask");
    /* ref to the latest rolling average, this is used to define the mask */
    std::vector<double>& ra = data.rolling_average;

    //message(data.name, " set new mask data =  ", ra[100]);

    double hi_max =  std::numeric_limits<double>::infinity();
    double lo_min = -std::numeric_limits<double>::infinity();
    //
    //data.hi_mask.resize(ra.size(), UTL::ZERO_DOUBLE);
    //data.lo_mask.resize(ra.size(), UTL::ZERO_DOUBLE);
    /* ref to hi and lo masks */
    std::vector<double>& hi_mask = data.hi_mask;
    std::vector<double>& lo_mask = data.lo_mask;

//    message("hi_mask.size()",hi_mask.size());
//    message("lo_mask.size()",lo_mask.size());
    //
    // mask_value is what to add or subtrace from the
    // rolling_average to set the hi/lo mask
    double mask_value = UTL::ZERO_DOUBLE;
    std::string mask_type;
    if(data.use_percent_mask)
    {
        mask_type = "setPercentMask";
    }
    else if(data.use_abs_mask)
    {
        mask_type = "setAbsoluteMask";
        mask_value = data.mask_value;
    }
    //
    // MASK part 1
    for(auto i = UTL::ZERO_SIZET; i <= data.mask_start; ++i)
    {
        hi_mask[i] = hi_max;
        lo_mask[i] = lo_min;
    }
    //message("2aa");
    //
    // MASK part 2
    for(auto i = data.mask_start + UTL::ONE_SIZET; i <= data.mask_end; ++i)
    {
        if(data.use_percent_mask)
        {
            /* data.mask_value is in units o fpercent, so mulitply by 0.01 */
            mask_value = fabs(ra[i] * data.mask_value * UTL::ZERO_POINT_ONE);
            if( mask_value < data.mask_abs_min )
            {
                mask_value = data.mask_abs_min;
            }
        }
        hi_mask[i] = ra[i] + mask_value;
        lo_mask[i] = ra[i] - mask_value;

        //std::cout << hi_mask[i] <<  " " << lo_mask[i] << ". ";
    }
    //std::cout << std::endl;
    //message(mask_type," 3");
    //
    // MASK part 3
    for(auto i = data.mask_window_start + UTL::ONE_SIZET; i <= data.mask_window_end; ++i)
    {
        hi_mask[i] = hi_max;
        lo_mask[i] = lo_min;
    }
    //
    // MASK part 4
//    for(auto i = data.mask_window_end + UTL::ONE_SIZET; i <= data.mask_end; ++i)
//    {
//        if(data.use_percent_mask)
//        {
//            mask_value = fabs(ra[i] * data.mask_value);
//            if( mask_value < data.mask_abs_min )
//            {
//                mask_value = data.mask_abs_min;
//            }
//        }
//        hi_mask[i] = ra[i] + mask_value;
//        lo_mask[i] = ra[i] - mask_value;
//    }
    //
    // MASK part 5
    for(auto i = data.mask_end + UTL::ONE_SIZET; i < ra.size(); ++i)
    {
        hi_mask[i] = hi_max;
        lo_mask[i] = lo_min;
    }
    //message(mask_type," 5");
    data.lo_mask_set = true;
    data.hi_mask_set = true;
}


//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/

    // set the basic paramters for a mask, its type, ( % or Abs)
    // the value
    // start end, floor and window

*/
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// There is a high level, global check mask funciton, this can be set from various places
// to enable / disable checking masks
//
void liberallrfInterface::setGlobalCheckMask(bool value)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    llrf.check_mask = value;
    /*
        we are coupling this to taking rolling averages (!)
    */
    if(value)
    {
        for(auto&it:llrf.trace_data)
        {
            it.second.keep_rolling_average = true;
        }
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setGlobalShouldCheckMask()
{
    setGlobalCheckMask(true);
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setGlobalShouldNotCheckMask()
{
    setGlobalCheckMask(false);
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setShouldCheckMask(const std::string&name)
{
    return setCheckMask(name, true);
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setShouldNotCheckMask(const std::string&name)
{
    return setCheckMask(name, false);
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setCheckMask(const std::string&name, bool value)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).check_mask = value;
        setKeepRollingAverage(n,value);
        return true;
    }
    message("liberallrfInterface setCheckMask, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setUsePercentMask(const std::string& name)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).use_percent_mask  = true;
        llrf.trace_data.at(n).use_abs_mask = false;
        return true;
    }
    message("liberallrfInterface setUsePercentMask, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setUseAbsoluteMask(const std::string& name)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).use_percent_mask  = false;
        llrf.trace_data.at(n).use_abs_mask = true;
        return true;
    }
    message("liberallrfInterface setUseAbsoluteMask, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskAbsMinValue(const std::string& name,const double value)
{
    if(value < UTL::ZERO_DOUBLE)
    {
        message("liberallrfInterface::setMaskValue ERROR, mask_value can not be < 0");
        return false;
    }
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).mask_abs_min = value;
        return true;
    }
    message("liberallrfInterface::setMaskValue ERROR, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskValue(const std::string& name,const double value)
{
    if(value < UTL::ZERO_DOUBLE)
    {
        message("liberallrfInterface::setMaskValue ERROR, mask_value can not be < 0");
        return false;
    }
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).mask_value  = value;
        return true;
    }
    message("liberallrfInterface::setMaskValue ERROR, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskStartIndex(const std::string& name, size_t value)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    message("setMaskStartIndex, ", n, " ", value);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).mask_start = value;
        return true;
    }
    message("liberallrfInterface setMaskStartIndex, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskEndIndex(const std::string& name, size_t value)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).mask_end = value;
        return true;
    }
    message("liberallrfInterface setMaskEndIndex, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskFloor(const std::string& name, double value)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).mask_floor = value;
        return true;
    }
    message("liberallrfInterface setMaskFloor, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskWindowIndices(const std::string& name, size_t window_start, size_t window_end )
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).mask_window_start = window_start;
        llrf.trace_data.at(n).mask_window_end   = window_end;
        return true;
    }
    message("liberallrfInterface setMaskWindowIndices, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskStartTime(const std::string& name, double value)
{
    return setMaskStartIndex(name, getIndex(value));
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskEndTime(const std::string& name, double value)
{
    return setMaskEndIndex(name, getIndex(value));
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskWindowTimes(const std::string& name, double window_start, double window_end)
{
    return setMaskWindowIndices(name, getIndex(window_start), getIndex(window_end));
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskParamatersIndices(const std::string& name, bool isPercent,
                                                   double mask_value, double mask_floor, double mask_abs_min,
                                                   size_t start, size_t end,
                                                   size_t window_start, size_t window_end
                                                   )
{
    // sanity check
    if(UTL::ZERO_SIZET <= start && end <= llrf.all_traces.num_elements_per_trace_used)
    {
        if(start <= end && window_start <= window_end )
        {
            bool r = false;
            if(isPercent)
            {
                r = setUsePercentMask(name);
            }
            else
            {
                r = setUseAbsoluteMask(name);
            }
            if(r)
            {
                if( setMaskValue(name, mask_value) )
                {
                    if(setMaskStartIndex(name , start))
                    {
                        if( setMaskEndIndex(name, end))
                        {
                            if(setMaskWindowIndices(name, window_start, window_end))
                            {
                                if( setMaskAbsMinValue(name, mask_abs_min))
                                {
                                    if( setMaskFloor(name, mask_floor))
                                    {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMaskParamatersTimes(const std::string& name, bool isPercent,
                                                 double mask_value, double mask_floor, double mask_abs_min,
                                                 double start, double end,
                                                 double window_start, double window_end
                                                   )
{
    return setMaskParamatersIndices(name, isPercent, mask_value, mask_floor, mask_abs_min,
                                    getIndex(start), getIndex(end),
                                    getIndex(window_start),getIndex(window_end));
}
//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    / _` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    \__> |___  |   |  |___ |  \ .__/
    //
*/
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::isGlobalCheckMask()
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.check_mask;
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getHiMask(const std::string& name) const
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    //message()
    if(entryExists(llrf.trace_data, n))
    {
        if(llrf.trace_data.at(n).hi_mask_set)
        {
            return llrf.trace_data.at(n).hi_mask;
        }
    }
    message("liberallrfInterface::getHiMask ERROR, trace ", n, " does not exist");
    std::vector<double> r{UTL::DUMMY_DOUBLE};//MAGIC_NUMBER
    return r;
}
//---------------------------------------------------------------------------- ----------------------
std::vector<double> liberallrfInterface::getLoMask(const std::string& name) const
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        if(llrf.trace_data.at(n).lo_mask_set)
        {
            return llrf.trace_data.at(n).lo_mask;
        }
    }
    message("liberallrfInterface::getLoMask ERROR, trace ", n, " does not exist");
    std::vector<double> r{UTL::DUMMY_DOUBLE};//MAGIC_NUMBER
    return r;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getMaskValue(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).mask_value;
    }
    message("liberallrfInterface::getMaskValue ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getMaskStartIndex(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).mask_start;
    }
    message("liberallrfInterface::getMaskStartIndex ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getMaskEndIndex(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).mask_end;
    }
    message("liberallrfInterface::getMaskEndIndex ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getMaskFloor(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).mask_floor;
    }
    message("liberallrfInterface::getMaskFloor ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getMaskWindowStartIndex(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).mask_window_start;
    }
    message("liberallrfInterface::getMaskWindowStartIndex ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getMaskWindowEndIndex(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).mask_window_end;
    }
    message("liberallrfInterface::getMaskWindowEndIndex ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getMaskStartTime(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return getTime(llrf.trace_data.at(n).mask_start);
    }
    message("liberallrfInterface::getMaskStartTime ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getMaskEndTime(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return getTime(llrf.trace_data.at(n).mask_end);
    }
    message("liberallrfInterface::getMaskEndTime ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getMaskWindowStartTime(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return getTime(llrf.trace_data.at(n).mask_window_start);
    }
    message("liberallrfInterface::getMaskWindowStartTime ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getMaskWindowEndTime(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return getTime(llrf.trace_data.at(n).mask_window_end);
    }
    message("liberallrfInterface::getMaskWindowEndTime ERROR, trace ", n, " does not exist");
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
//
//
// MASKS
//----------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isPercentMask(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        return llrf.trace_data.at(n).use_percent_mask;
    }
    return false;
}
//----------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isAbsoluteMask(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        return llrf.trace_data.at(n).use_abs_mask;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::isCheckingMask(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        return llrf.trace_data.at(n).use_percent_mask;
    }
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
bool liberallrfInterface::clearMask(const std::string&name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).hi_mask.clear();
        llrf.trace_data.at(n).lo_mask.clear();
        llrf.trace_data.at(n).hi_mask_set  = false;
        llrf.trace_data.at(n).lo_mask_set = false;
        return true;
    }
    message("liberallrfInterface::clearMask ERROR, trace ", n, " does not exist");
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setInfiniteMasks(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        std::vector<double> p_inf(llrf.trace_data.at(n).trace_size,  std::numeric_limits<double>::infinity());
        std::vector<double> n_inf(llrf.trace_data.at(n).trace_size, -std::numeric_limits<double>::infinity());
        llrf.trace_data.at(n).hi_mask = p_inf;
        llrf.trace_data.at(n).lo_mask = n_inf;
        llrf.trace_data.at(n).hi_mask_set = true;
        llrf.trace_data.at(n).lo_mask_set = true;
        return true;
    }
    message("liberallrfInterface::setInfiniteMasks ERROR, trace ", n, " does not exist");
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setHiMask(const std::string&name,const std::vector<double>& value)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        if(value.size() == llrf.trace_data.at(n).trace_size)
        {
            llrf.trace_data.at(n).hi_mask = value;
            llrf.trace_data.at(n).hi_mask_set = true;
            return true;
        }
    }
    message("liberallrfInterface::setHiMask ERROR, trace ", n, " does not exist");
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setLoMask(const std::string&name,const std::vector<double>& value)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        if(value.size() == llrf.trace_data.at(n).trace_size)
        {
            llrf.trace_data.at(n).lo_mask = value;
            llrf.trace_data.at(n).lo_mask_set = true;
            return true;
        }
    }
    message("liberallrfInterface::setLoMask ERROR, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setNumContinuousOutsideMaskCount(size_t value)
{
    bool r = true;
    bool temp;
    for(auto&& it: llrf.trace_data)
    {
        temp = setNumContinuousOutsideMaskCount(it.first,value);
        if( !temp )
        {
            r = false;
        }
    }
    return r;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setNumContinuousOutsideMaskCount(const std::string&name, size_t value)
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(name).num_continuous_outside_mask_count = value;
        return true;
    }
    message("liberallrfInterface::setInfiniteMasks ERROR, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::checkForOutsideMaskTrace()
{
    //std::chrono::high_resolution_clock::time_point start2 = std::chrono::high_resolution_clock::now();
    for( auto& it: llrf.trace_data) // loop over each trace
    {
        if( llrf.check_mask && it.second.check_mask && it.second.hi_mask_set && it.second.lo_mask_set ) // if everything is ok, check mask
        {
            int result = updateIsTraceInMask(it.second);
            handleTraceInMaskResult(it.second,result);
            if( result == UTL::ZERO_INT)
            {
                message("Found an OME, not checking any more traces this pulse!");
                break;
            }
        }
    }
//    if(llrf.check_mask)
//    {
//        std::chrono::high_resolution_clock::time_point end1 = std::chrono::high_resolution_clock::now();
//        message("Timing ",std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start2).count() );
//    }
}
//--------------------------------------------------------------------------------------------------
int liberallrfInterface::updateIsTraceInMask(llrfStructs::rf_trace_data& trace)
{
    outside_mask_trace_message.str("");
    /* to fail, you must get consecutive points outside the mask */
    size_t hi_breakdown_count = UTL::ZERO_INT;
    size_t lo_breakdown_count = UTL::ZERO_INT;
    /* ref for ease of reading */
    auto& to_check = trace.data_buffer.back().second;
    auto& hi = trace.hi_mask;
    auto& lo = trace.lo_mask;
    /* main loop iterate over the trace values  */
    for(auto i = 0; i < to_check.size(); ++i)
    {
        /* if we are above the mask floor */
        if(to_check[i] > trace.mask_floor)
        {
            /* if we are above the mask increase hi_breakdown_count */
            if(to_check[i] > hi[i])
            {
                hi_breakdown_count += UTL::ONE_SIZET;
                /* if we have too many consecutive hi_breakdown_count trace fails */
                if(hi_breakdown_count == trace.num_continuous_outside_mask_count)
                {

                    double t;
                    std::string str;
                    updateTime(trace.data_buffer.back().first, t, str);

                    /* write a message*/
                    outside_mask_trace_message << trace.name << " HI MASK FAIL: current average = "
                    << trace.rolling_average[i] << ", " << to_check[i] << " > "
                    << hi[i] << " at i = " << i << " us = " << llrf.time_vector[i] << ". Trace time-stamp = " << str;
                    trace.outside_mask_index = i;
                    trace.outside_mask_trace_message = outside_mask_trace_message.str();
                    message("LLRF CONTROLLER MESSAGE: ", trace.outside_mask_trace_message);
                    /* return code 0 = FAIL */
                    return UTL::ZERO_INT;
                }
            }
            else
            {
                /* if the value is good reset hi_breakdown_count to zero */
                hi_breakdown_count = UTL::ZERO_INT;
            }
            /* if we are belwo the lo mask, increase lo_breakdown_count */
            if(to_check[i] < lo[i])
            {
                lo_breakdown_count += UTL::ONE_INT;
                /* if we have too many consecutive lo_breakdown_count trace fails */
                if(lo_breakdown_count == trace.num_continuous_outside_mask_count)
                {
                    double t;
                    std::string str;
                    updateTime(trace.data_buffer.back().first, t, str);
                    /* write a message*/
                    outside_mask_trace_message << trace.name << " LO MASK FAIL: current average = "
                    << trace.rolling_average[i] << ", " << to_check[i] << " < "
                    << lo[i] << " at i = " << i << " us = " << llrf.time_vector[i] << ". Trace time-stamp = " << str;
                    trace.outside_mask_index = i;
                    trace.outside_mask_trace_message = outside_mask_trace_message.str();
                    message("LLRF CONTROLLER MESSAGE: ", trace.outside_mask_trace_message);
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
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::handleTraceInMaskResult(llrfStructs::rf_trace_data& trace, int result)
{
    //message(trace.name, "TraceInMaskResult = ", result );
    switch(result)
    {
        case UTL::ONE_INT: /* passed mask */
            {
                handlePassedMask(trace);
//                if(counter % 1000 == 2)
//                {
//                    message(trace.name, " Passed masks ");
//                    for(auto i = 60; i < 200; ++i)
//                    {
//                        std::cout << trace.hi_mask[i] << " ";
//                    }
//                    std::cout <<std::endl;
//                    for(auto i = 60; i < 200; ++i)
//                    {
//                        std::cout << trace.lo_mask[i] << " ";
//                    }
//                    std::cout <<std::endl;
//
//                }
//                ++counter;
            }
            break;
        case UTL::ZERO_INT: /* failed mask */
            {
//            std::chrono::high_resolution_clock::time_point end2 = std::chrono::high_resolution_clock::now();
//            message("Timing ",std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - foundbreakdowntime).count() );
                handleFailedMask(trace);
                break;
            }
        case UTL::MINUS_ONE_INT:
            /* not checking masks or no active pulses */
            break;
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::handlePassedMask(llrfStructs::rf_trace_data& trace)
{
    // err... do nothing
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getOutsideMaskEventCount()const
{
    return llrf.omed_count;
}
//---------------------------------------------------------------------------------------------------------
void liberallrfInterface::handleFailedMask(llrfStructs::rf_trace_data& trace)
{
    llrf.omed.can_get_data = false;
    llrf.omed_count += UTL::ONE_SIZET;
    // Stop Checking Masks
    // FIRST Drop the Amplitude
    if(trace.drop_amp_on_breakdown)
    {
        /* stop checking masks */
        llrf.check_mask = false;
        message("LLRF CONTROLLER MESSAGE: llrf.check_mask (GlobalCheckMask)  is FALSE;");
        /* set amp to drop_value */
        message("handleFailedMask() is setting amp to = ", trace.amp_drop_value, ", time = ",  elapsedTime());
        setAmpSPCallback(trace.amp_drop_value);
    }
    // SECOND STOP ADDING TO THE ROLLING AVERAGE
    //
    message("handleFailedMask() is setting keep_rolling_average = FALSE, for all traces");
    for(auto&it:llrf.trace_data)
    {
        it.second.keep_rolling_average = false;
    }
    //
    newOutsideMaskEvent(trace);
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::setAmpHP(double value)
{
    setAmpSPCallback(value);
    return true;
}
//---------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------
void liberallrfInterface::setAmpSPCallback(const double value)
{
    setAmpHP_Threads.push_back(llrfStructs::setAmpHP_Struct());
    setAmpHP_Threads.back().interface = this;
    setAmpHP_Threads.back().value  = value;
    setAmpHP_Threads.back().thread = new std::thread(staticEntrySetAmp, std::ref(setAmpHP_Threads.back()));
    kill_finished_setAmpHP_threads();
}
//---------------------------------------------------------------------------------------------------------
void liberallrfInterface::staticEntrySetAmp(llrfStructs::setAmpHP_Struct& s)
{
    /*
        time how long this function takes to run
    */
//    std::chrono::high_resolution_clock::time_point start1 = std::chrono::high_resolution_clock::now();

    s.interface->attachTo_thisCAContext(); /// base member function

    s.interface->OME_interlock(s);

    s.can_kill = true;
    /*
        time how long this function takes to run
    */
//    std::chrono::high_resolution_clock::time_point end1 = std::chrono::high_resolution_clock::now();
//    s.interface->message("SetAmp thread took ", std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count(), " us");
}
//---------------------------------------------------------------------------------------------------------
void liberallrfInterface::OME_interlock(const llrfStructs::setAmpHP_Struct& s)
{
    trigOff();
    //externalTriggerOff();
    //std::chrono::milliseconds x(100000);
    //std::this_thread::sleep_for(x);
    resetToValueFromTrigOff(s.value);
}
//---------------------------------------------------------------------------------------------------------
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
    if(!isTrigExternal())
    {
        return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::TRIG_SOURCE),llrfStructs::TRIG::EXTERNAL);
    }
    return true;
}
//____________________________________________________________________________________________
//bool liberallrfInterface::externalTriggerOff()
//{
//    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::TIMING_TRIGGER),UTL::ZERO_U_SHORT);
//}
////____________________________________________________________________________________________
//bool liberallrfInterface::externalTriggerOn()
//{
//    if(!isTrigExternal())
//    {
//        return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::TIMING_TRIGGER),UTL::ONE_U_SHORT);
//    }
//    return true;
//}
//____________________________________________________________________________________________
bool liberallrfInterface::resetToValueFromTrigOff(const double value)
{
    bool r = setAmpSP(value);
    if(!r)
    {
        message("resetToValue failed to send amp SP value");
        return false;
    }
    pause_2();
    pause_2();
    pause_2();
    pause_2();
    pause_2();
    pause_2();
    pause_2();
    pause_2();

    //r = externalTriggerOn();
    r = trigExt();
    if(!r)
    {
        message("resetToValue failed to send set trig to External");
        return false;
    }
    return r;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::resetToValueFromInterlockActive(const double value)
{
   // message("resetToValue Start");
    bool r = setAmpSP(value);
    if( r == false )
    {
        message("resetToValue failed to send amp SP value");
        return false;
    }

    pause_1();
    //message("resetToValue setInterlockNonActive");

    r = setInterlockNonActive();
    if( r == false )
    {
        message("resetToValue failed to setInterlockNonActive");
        return false;
    }

    pause_2();
    pause_2();
    pause_2();
    //message("resetToValue enableRFOutput");

    r = enableRFandLock();

    return r;
}
//--------------------------------------------------------------------------------------------------
std::vector<std::string> liberallrfInterface::getTracesToSaveWhenDumpingCutOneTraceData()const
{
    return llrf.traces_to_get_when_dumping_all_traces_data_buffer;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setTracesToSaveWhenDumpingCutOneTraceData(const std::vector<std::string>& names)
{
    std::vector<std::string> checked_names;
    for(auto&& i: names)
    {
        const std::string n = fullLLRFTraceName(i);
        if(entryExists(llrf.trace_data,n))
        {
            checked_names.push_back(n);
        }
    }
    llrf.traces_to_get_when_dumping_all_traces_data_buffer = checked_names;
}

//---------------------------------------------------------------------------------------------------------
boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> liberallrfInterface::copy_buffer()
{
    // This is the function to return the all_trace data dump ....
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> copydata_buffer =  llrf.all_traces.data_buffer;
    return copydata_buffer;
}
//---------------------------------------------------------------------------------------------------------
//std::vector< std::tuple<std::string,std::string, std::vector<double>>>
void liberallrfInterface::getCutOneTraceData(std::vector<std::string>& ret_keys,
                                             std::vector<std::string>& ret_timestamps,
                                             std::vector<std::vector<double>>& ret_data)
{
    // This is the function to return the all_trace data dump ....
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();

        // This is the function to return the all_trace data dump ....
    boost::circular_buffer<std::pair<epicsTimeStamp, std::vector<double>>> copydata_buffer =  copy_buffer();

    message("get buffer, size = ", copydata_buffer.size());

    /* this funciton copies the requested trace from llrf.all_traces.data_buffer
        It probably makes too mnay copies of the data - neaten up when i'm sure it works ....
     */
    message("liberallrfInterface::getCutOneTraceData()");
    //std::vector< std::tuple<std::string, std::string, std::vector<double>> > r;

    ret_keys.clear();
    ret_timestamps.clear();
    ret_data.clear();

    if( llrf.pulse_count > copydata_buffer.size() )
    {
        /* we iterate over llrf.trace_data.at(it.first).data_buffer */
        int counter = UTL::ZERO_INT;
        for(auto i = UTL::ZERO_SIZET; i < copydata_buffer.size(); ++i)
        {
            /* counter as string for key names in final python dict */
            std::string counter_str = std::to_string(counter);
            /* data timestamp */
            std::string timestamp;
            double temp_d;
            updateTime( copydata_buffer[i].first, temp_d, timestamp);


            message("data_buffer_it times_stamp = ", timestamp);


            /* iterate over each traces_to_get_when_dumping_all_traces_data_buffer */
            for(auto&& trace_name: llrf.traces_to_get_when_dumping_all_traces_data_buffer)
            {
                //message("Searching for ",trace_name, " data, part ",counter);
                /* trace info holds the indices we require to get a specifctrace from llrf.all_traces.data_buffer */
                if(entryExists(llrf.all_traces.trace_info, trace_name))
                {
                    llrf.all_traces.trace_info.at(trace_name);

                    //message("data_start_index = ", llrf.all_traces.trace_info.at(trace_name).start_index,
                    //        " data_end_index = ", llrf.all_traces.trace_info.at(trace_name).stop_index );

                    auto data_start = copydata_buffer[i].second.begin() + llrf.all_traces.trace_info.at(trace_name).start_index;
                    auto data_end   = data_start + llrf.all_traces.trace_info.at(trace_name).stop_index+1;


                    /* get numerical data in its own vector */

                    //message("Copy numbers start ");

                    std::vector<double>numerical_trace_data_temp;

                    for(auto i_k = llrf.all_traces.trace_info.at(trace_name).start_index;
                             i_k < llrf.all_traces.trace_info.at(trace_name).stop_index; ++i_k  )
                    {
                        numerical_trace_data_temp.push_back(copydata_buffer[i].second[i_k]);
                    }
                    //message("numerical_trace_data_temp.size() = ", numerical_trace_data_temp.size());
                    //std::copy( data_start, data_end, numerical_trace_data_temp.begin() );
                    message("Copy numbers fin ");

                    std::string next_key = trace_name + "_" + counter_str;
                    //message("next_key = ",next_key );
                    ret_keys.push_back( next_key );

                    //message("next time stamp = ", timestamp);
                    ret_timestamps.push_back( timestamp );

                    double temp_d2 = numerical_trace_data_temp.front();
                    //message("next data = ", temp_d2);
                    ret_data.push_back( numerical_trace_data_temp );

                    message(ret_keys.back());
                    message(ret_timestamps.back());
                }

            }
            /* increment counter for next data_buffer */
            counter += UTL::ONE_INT;
        }
    }
    //return r;
}
//---------------------------------------------------------------------------------------------------------
std::vector< std::pair<std::string, std::vector<double>> >  liberallrfInterface::getOneTraceData()const
{
    // This is the function to return the all_trace data dump ....
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::vector< std::pair<std::string, std::vector<double>>> r;

    message("getOneTraceData");

    size_t index = UTL::ZERO_SIZET;
    for(auto i = UTL::ZERO_SIZET; i < llrf.all_traces.data_buffer.size(); ++i)
    {
        std::pair<std::string, std::vector<double>> d;
        double temp_d;
        updateTime( llrf.all_traces.data_buffer[i].first, temp_d, d.first);

        d.second = llrf.all_traces.data_buffer[i].second;
        r.push_back(d);
       // message(index," ", r.back().first);
        ++index;
    }
    return r;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::setInterlockActive()
{
    if( isInterlockNotActive() )
        return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::INTERLOCK),llrfStructs::INTERLOCK_STATE::ACTIVE);
    else
        return true;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::setInterlockNonActive()
{
    if( isInterlockActive() )
        return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::INTERLOCK),llrfStructs::INTERLOCK_STATE::NON_ACTIVE);
    else
        return true;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isInterlockActive() const
{
    return llrf.interlock_state == llrfStructs::INTERLOCK_STATE::ACTIVE;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isInterlockNotActive() const
{
    return llrf.interlock_state == llrfStructs::INTERLOCK_STATE::NON_ACTIVE;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::lockAmpFF()
{
    if(isAmpFFNotLocked())
        return setValue<short>(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE), UTL::ONE_US);
    return true;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::lockPhaseFF()
{
    if(isPhaseFFNotLocked())
    {
        message("isPhaseFFNotLocked() == TRUE, attempting to lock phase");
        return setValue<short>(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE),UTL::ONE_US);
    }
    else
    {
        message("isPhaseFFNotLocked() == FALSE, no need to lock phase");

    }
    return true;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::unlockAmpFF()
{
    if(isAmpFFLocked())
        return setValue<short>(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_FF_AMP_LOCK_STATE),UTL::ZERO_US);
    return true;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::unlockPhaseFF()
{
    if(isPhaseFFLocked())
        return setValue<short>(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_FF_PHASE_LOCK_STATE),UTL::ZERO_US);
    return true;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::enableRFOutput()
{
    if(isNotRFOutput())
        return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_RF_OUTPUT),UTL::ONE_US);
    return true;
}

bool liberallrfInterface::enableRFandLock()
{
    bool  r = enableRFOutput();
    if( r == false )
    {
        message("resetToValue failed to enableRFOutput");
        return false;
    }
    pause_2();
    pause_2();
    //message("resetToValue lockPhaseFF");
    r = lockPhaseFF();
    if( r == false )
    {
        message("enableRFandLock failed to lockPhaseFF");
        return false;
    }
    pause_2();
    pause_2();
    //message("resetToValue lockAmpFF");
    r = lockAmpFF();
    if( r == false )
    {
        message("enableRFandLock failed to lockAmpFF");
        return false;
    }
    pause_2();
    pause_2();
    pause_2();

    if( isRFOutput() )
    {
        if( isPhaseFFLocked() )
        {
            if( isAmpFFLocked())
            {
                return true;
            }
        }
    }
    return false;
    //message("resetToValue Returned True");
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::disableRFOutput()
{
    if(isRFOutput())
        return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::LIB_RF_OUTPUT),UTL::ZERO_US);
    return true;
}


//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isRFOutput() const
{
    return llrf.rf_output;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isNotRFOutput() const
{
    return !isRFOutput();
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isFFLocked()const
{
    return isAmpFFLocked() && isPhaseFFLocked();
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isFFNotLocked()const
{
    return isAmpFFNotLocked() && isPhaseFFNotLocked();
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isAmpFFLocked()const
{
    return llrf.ff_amp_lock_state;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isAmpFFNotLocked()const
{
    return !isFFLocked();
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isPhaseFFLocked()const
{
    return llrf.ff_ph_lock_state;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isPhaseFFNotLocked()const
{
    return !isPhaseFFLocked();
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::RFOutput()const
{
    return llrf.rf_output;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isTrigOff()const
{
    return llrf.trig_source == llrfStructs::TRIG::OFF;
}
//---------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isTrigExternal()const
{
    return llrf.trig_source == llrfStructs::TRIG::EXTERNAL;
}



//---------------------------------------------------------------------------------------------------------
void liberallrfInterface::newOutsideMaskEvent(const llrfStructs::rf_trace_data& trace)
{
    llrf.new_outside_mask_event = true;
    llrf.omed.is_collecting = true;
    llrf.omed.num_events = UTL::ONE_SIZET;
    llrf.omed.can_get_data = false;
    message("newOutsideMaskEvent() setting llrf.omed.can_get_data = FALSE");
    llrf.omed.num_collected = UTL::THREE_SIZET;
    llrf.omed.num_still_to_collect = llrf.omed.extra_traces_on_outside_mask_event;
    llrf.omed.trace_that_caused_OME = trace.name;
    llrf.omed.outside_mask_trace_message = trace.outside_mask_trace_message;
    llrf.omed.outside_mask_index = trace.outside_mask_index;
    llrf.omed.trace_data.clear();
    llrf.omed.hi_mask = trace.hi_mask;
    llrf.omed.lo_mask = trace.lo_mask;
    llrf.omed.mask_floor = trace.mask_floor;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::checkCollectingFutureTraces()
{
    if( llrf.omed.is_collecting )
    {
        llrf.omed.num_still_to_collect -= UTL::ONE_SIZET;
        llrf.omed.num_collected += UTL::ONE_SIZET;

        message("checkCollectingFutureTraces(): Num collected = ", llrf.omed.num_collected, "/",llrf.omed.extra_traces_on_outside_mask_event + UTL::THREE_SIZET,
                " (",llrf.omed.num_still_to_collect  ,")");

        // we now check if there is a breakdown in the future trace
        // this is basically copy-pasta from the main breakdown checking routines,
        // so could be better integrated with those funcitons

        if(llrf.can_increase_active_pulses)
        {
            message("checkCollectingFutureTraces(): checking current trace for OMED");
            for( auto& it: llrf.trace_data) // loop over each trace
            {
                // only check the masks we should be checking
                if(it.second.check_mask && it.second.hi_mask_set && it.second.lo_mask_set)
                {
                    int result = updateIsTraceInMask(it.second);
                    if(result == UTL::ZERO_INT)
                    {
                        message("checkCollectingFutureTraces(): found another breakdown in ", it.second.name);
                        llrf.omed.num_events+= UTL::ONE_SIZET;
                        break;
                    }
                }
            }
        }
        else
        {
            message("checkCollectingFutureTraces(): can't check for OMED as llrf.can_increase_active_pulses = FALSE");
        }
        //
        // sanity check
        if( llrf.omed.num_still_to_collect == UTL::ZERO_SIZET )
        {
            if( llrf.omed.num_collected == llrf.omed.extra_traces_on_outside_mask_event + UTL::THREE_SIZET)
            {
                copyTraceDataToOMED();
                llrf.omed.is_collecting = false;
                llrf.omed.can_get_data  = true;
                message("FINSHED COLLECTING EXTRA TRACES, can_get_data = True," );
            }
            else
            {
                message("ERROR NEVER SHOW THIS MESSAGE");
            }
        }
        else
        {
            //message("Collecting future traces = ", llrf.omed.num_still_to_collect);
        }
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::copyTraceDataToOMED()
{
    // loop over each traces_to_save_on_outside_mask_event
    // and copy the last 3 + extra_traces_on_outside_mask_event (probably 5)

    //message("copyTraceDataToOMED");

    size_t num_traces_to_copy = llrf.omed.extra_traces_on_outside_mask_event + UTL::THREE_SIZET;

    for(auto&&trace_name:llrf.omed.traces_to_save_on_outside_mask_event )
    {
        llrf.omed.trace_data.push_back( llrfStructs::outside_mask_event_trace_data() );
        llrf.omed.trace_data.back().name = trace_name;
        llrf.omed.trace_data.back().hi_mask = llrf.trace_data.at(trace_name).hi_mask;
        llrf.omed.trace_data.back().lo_mask = llrf.trace_data.at(trace_name).lo_mask;

        size_t buffer_end_index = llrf.trace_data.at(trace_name).buffersize - UTL::ONE_SIZET;
        //message("omed data_buffer  buffer_end_index  = ", buffer_end_index);
        for(auto i = 0; i < num_traces_to_copy; ++i)
        {
            std::pair<epicsTimeStamp, std::vector<double>>& data_ref = llrf.trace_data.at(trace_name).data_buffer[ buffer_end_index - i];
            double temp_d;
            updateTime( data_ref.first, temp_d, temp_OMED_trace_item.first);
            temp_OMED_trace_item.second = data_ref.second;
            llrf.omed.trace_data.back().data_buffer.push_back( temp_OMED_trace_item );


            //message( temp_OMED_trace_item.first, " ", temp_OMED_trace_item.second[0], " ", temp_OMED_trace_item.second[1] );

//            message(trace_name);
//            size_t i2 = 0;
//            for(auto i: data_ref.second)
//            {
//                std::cout << i << " ";
//                if( i2 == 20 )
//                {
//                    break;
//                }
//                i2 += 1;
//            }
//            std::cout << std::endl;
//            std::cout << std::endl;
        }
    }
    //message("copyTraceDataToOMED has finished");
}


//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setTracesToSaveOnOutsideMaskEvent(const std::vector<std::string>& name)
{
    std::vector<std::string> checked_names;
    for(auto&& i: name)
    {
        const std::string n = fullLLRFTraceName(i);
        if(entryExists(llrf.trace_data,n))
        {
            checked_names.push_back(n);
        }
    }
    llrf.omed.traces_to_save_on_outside_mask_event = checked_names;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setTracesToSaveOnOutsideMaskEvent(const std::string& name)
{
    std::vector<std::string> n;
    n.push_back(name);
    setTracesToSaveOnOutsideMaskEvent(n);
}
//--------------------------------------------------------------------------------------------------
std::vector<std::string> liberallrfInterface::getTracesToSaveOnOutsideMaskEvent()
{
    return llrf.omed.traces_to_save_on_outside_mask_event;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setNumExtraTracesOnOutsideMaskEvent(const size_t value)
{
    setIndividualTraceBufferSize(value + UTL::THREE_SIZET);
    llrf.omed.extra_traces_on_outside_mask_event = value;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getNumExtraTracesOnOutsideMaskEvent() const
{
    return llrf.omed.extra_traces_on_outside_mask_event;
}
//--------------------------------------------------------------------------------------------------
/// this needs to change, as we know each part is complete (this is an hangover from old trace data beign seperate
                                                            // just need an iscollecting fuction
bool liberallrfInterface::isOutsideMaskEventDataCollecting()const
{
    return llrf.omed.is_collecting;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::canGetOutsideMaskEventData()const
{
    return llrf.omed.can_get_data;
}
//--------------------------------------------------------------------------------------------------


//____________________________________________________________________________________________
size_t liberallrfInterface::getOutsideMaskEventDataSize()
{
    return llrf.omed.num_collected;
}

//____________________________________________________________________________________________
bool liberallrfInterface::setDropAmpOnOutsideMaskEvent(const std::string& name,const  bool state,const double amp_val)
{   // whether to drop amp when detetcing an outside_mask_trace
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, fullLLRFTraceName(n)))
    {
        llrf.trace_data.at(n).amp_drop_value = amp_val;
        llrf.trace_data.at(n).drop_amp_on_breakdown  = state;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setDropAmpOnOutsideMaskEventValue(const std::string& name, double amp_val)
{   // amplitude to set after detecting outside mask
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, fullLLRFTraceName(n)))
    {
        llrf.trace_data.at(n).amp_drop_value = amp_val;
        return true;
    }
    return false;
}
//____________________________________________________________________________________________
llrfStructs::outside_mask_event_data liberallrfInterface::getOutsideMaskEventData()
{
    return llrf.omed;
}

//____________________________________________________________________________________________
const llrfStructs::outside_mask_event_data& liberallrfInterface::getOutsideMaskEventData_CRef()
{
    return llrf.omed;
}





















//--------------------------------------------------------------------------------------------------
// NOT USED ANYMORE
bool liberallrfInterface::set_mask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,
                      const double value, const std::string& name,const bool isPercent)
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    // automatically set the mask based on the rolling_average for cavity_rev_power trace
    // between element 0    and s1 will be set to default hi/lo (+/-infinity)
    // between element s1+1 and s2 will be set by rolling_average +/- value percent of rolling_average
    // between element s2+1 and s3 will be set very default hi/lo (+/-infinity)
    // between element s3+1 and s4 will be set by rolling_average +/- value percent of rolling_average
    // between element s3+1 and s4 will be set very default hi/lo (+/-infinity)
//    double temp = 0.0;
//    std::string mask_type;
//    if(isPercent)
//    {
//        mask_type = "setPercentMask";
//    }
//    else
//    {
//        mask_type = "setAbsoluteMask";
//    }
//    const std::string n = fullLLRFTraceName(name);
//    if(entryExists(llrf.trace_data, n))
//    {
//        // if we're keeping an average pulse
//        if(llrf.trace_data.at(n).has_average)
//        {
//            std::vector<double> & ra = llrf.trace_data.at(n).rolling_average;
//
//            // sanity check on s1,s2,s3,s4
//            if(0 <= s1 && s4 <= ra.size() - 1)
//            {
//                if(s1 <= s2 && s2 <= s3 && s3 <= s4)
//                {
//                    double hi_max = -std::numeric_limits<double>::infinity();
//                    double lo_min =  std::numeric_limits<double>::infinity();
//
//                    std::vector<double> hi_mask(ra.size(), 0.0);
//                    std::vector<double> lo_mask(ra.size(), 0.0);
//
//                    for(auto i = 0; i <= s1; ++i)
//                    {
//                        hi_mask[i] =   std::numeric_limits<double>::infinity();
//                        lo_mask[i] = - std::numeric_limits<double>::infinity();
//                    }
//                    for(auto i = s1; i <= s2; ++i)
//                    {
//                        if(isPercent)
//                        {
//                            temp = ra[i] * value;
//                        }
//                        else
//                        {
//                            temp = value;
//                        }
//                        hi_mask[i] = ra[i] + temp;
//                        lo_mask[i] = ra[i] - temp;
//                        if(hi_mask[i] > hi_max)
//                        {
//                           hi_max = hi_mask[i];
//                        }
//                        if(lo_mask[i] < lo_min)
//                        {
//                           lo_min = hi_mask[i];
//                        }
//                    }
//                    for(auto i = s2+1; i <= s3; ++i)
//                    {
//                        hi_mask[i] =   std::numeric_limits<double>::infinity();
//                        lo_mask[i] = - std::numeric_limits<double>::infinity();
//                    }
//                    for(auto i = s3+1; i <= s4; ++i)
//                    {
//                        if(isPercent)
//                        {
//                            temp = ra[i] * value;
//                        }
//                        else
//                        {
//                            temp = value;
//                        }
//                        hi_mask[i] = ra[i] + temp;
//                        lo_mask[i] = ra[i] - temp;
//                        if(hi_mask[i] > hi_max)
//                        {
//                           hi_max = hi_mask[i];
//                        }
//                        if(lo_mask[i] < lo_min)
//                        {
//                           lo_min = hi_mask[i];
//                        }
//                    }
//                    for(auto i = s4+1; i < ra.size(); ++i)
//                    {
//                        hi_mask[i] =   std::numeric_limits<double>::infinity();
//                        lo_mask[i] = - std::numeric_limits<double>::infinity();
//                    }
//                    // apply mask values
//                    if(setHiMask(n,hi_mask))
//                    {
//                        message("hi_max = ",hi_max);
//                        if(setLoMask(n,lo_mask))
//                        {
//                            message("lo_min = ", lo_min);
//                            return true;
//                        }
//                        else
//                        {
//                            message(n," setLowMask failed,  ",mask_type," fail");
//                        }
//                    }
//                    else
//                    {
//                        message(n," setHighMask failed,,  ",mask_type," fail");
//                    }
//                }
//                else
//                {
//                    message(n,s1," <= ",s2," && ",s2," <= ",s3," && ",s3," <= ",s4," failed,  ",mask_type," fail");
//                }
//            }
//            else
//            {
//                message(n," 0 <= ",s1," && ",s4," <= ",ra.size() - 1," failed, ",mask_type," fail");
//            }
//        }
//        else
//        {
//            message(n," does not exist,  ",mask_type," fail");
//        }
//    }
//    else
//    {
//        message(n," does not exist,  ",mask_type," fail");
//    }
    return false;
}





























/*
     __   __                    __                ___  __        __   ___  __
    |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
    |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

// Rolling averages for each individual trace can be kept
*/
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateTraceRollingAverage(llrfStructs::rf_trace_data& data )
{
    ////std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    // call heirarchy:
    // updateAllTraces(const event_handler_args& args)
    // updateMasks()
    //
    //
    //message("updateTraceRollingAverage, ", data.name);
    if(data.keep_rolling_average)
    {
        //message("keep_rolling_average = true, ", data.name);
        if(data.rolling_average_size == UTL::ZERO_SIZET)
        {

        }
        else if(data.rolling_average_size == UTL::ONE_SIZET)
        {
            data.rolling_average = data.data_buffer.back().second;
            data.rolling_average_count = data.rolling_average_size;
            data.has_average = true;
        }
        else
        {
            // append new data to average_trace_values
            data.average_trace_values.push_back( data.data_buffer.back().second );

            // add the new trace to data.rolling_sum
            std::vector<double>& to_add = data.average_trace_values.back();
            std::vector<double>& sum    = data.rolling_sum;

            for(auto&& sum_it = sum.begin(), to_add_it = to_add.begin();
                       sum_it < sum.end() && to_add_it < to_add.end();
                       ++to_add_it         , ++sum_it )
            {
                *sum_it += *to_add_it;
            }

            // if we have aquuired data.average_size traces then we can take an average
            if(data.average_trace_values.size() == data.rolling_average_size)
            {
                data.has_average = true;
            }
            //
            if(data.average_trace_values.size() > data.rolling_average_size)
            {
//                debugMessage(data.name, "  average_trace_values.size() > data.rolling_average_size ",
//                             data.average_trace_values.size()," >  ",data.rolling_average_size);
                // if so subtract the first trace in average_trace_values
                std::vector<double>& to_sub = data.average_trace_values.front();
                for(auto i1=to_sub.begin(), i2=sum.begin(); i1<to_sub.end() && i2<sum.end(); ++i1,++i2)
                {
                    *i2 -= *i1;
                }
                // erase the trace we just subtraced
                data.average_trace_values.erase(data.average_trace_values.begin());
            }
            if(data.has_average)
            {
                //message("has_average");
                std::vector<double>& av  = data.rolling_average;
                double s = (double)data.rolling_average_size;
                for(auto i1=sum.begin(), i2=av.begin(); i1<sum.end() && i2<av.end(); ++i1,++i2)
                {
                    *i2 = *i1 / s;
                }

                //message(data.name," new average, max  = ", *std::max_element(av.begin(),av.begin()+1000));
             }
            data.rolling_average_count = data.average_trace_values.size();
        }
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::clearAllRollingAverage()
{
    for(auto && it : llrf.trace_data)
    {
        clearTraceRollingAverage(it.second);
    }
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::clearTraceRollingAverage(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    if(entryExists(llrf.trace_data, n))
    {
        //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
        clearTraceRollingAverage(llrf.trace_data.at(n));
        return true;
    }
    //message("liberallrfInterface ERROR, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::clearTraceRollingAverage(llrfStructs::rf_trace_data& trace)
{
    // any function that calls this should have got the lock !!
    message("clearTraceRollingAverage ", trace.name);
    trace.has_average = false;
//    trace.current_trace = UTL::ZERO_SIZET;
//    trace.previous_trace = UTL::MINUS_ONE_INT;
//    trace.previous_previous_trace = UTL::MINUS_TWO_INT;
    trace.rolling_average_count = UTL::ZERO_SIZET;
    trace.rolling_average.clear();
    trace.rolling_average.resize(trace.trace_size);
    trace.rolling_sum.clear();
    trace.rolling_sum.resize(trace.trace_size);
    trace.average_trace_values = {};


    message("clearTraceRollingAverage FIN 1");


}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setShouldKeepRollingAverage()
{
    for(auto && it: llrf.trace_data)
    {
        setShouldKeepRollingAverage(it.first);
    }
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setShouldNotKeepRollingAverage()
{
    for(auto && it: llrf.trace_data)
    {
        setShouldNotKeepRollingAverage(it.first);
    }
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setShouldKeepRollingAverage(const std::string&name)
{
    return setKeepRollingAverage(name, true);
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setShouldNotKeepRollingAverage(const std::string&name)
{
    return setKeepRollingAverage(name, false);
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setKeepRollingAverage(const std::string&name, bool value)
{
    ////std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).keep_rolling_average = value;
        //clearTraceRollingAverage(llrf.trace_data.at(n));
        return true;
    }
    message("liberallrfInterface setKeepRollingAverage, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setKeepRollingAverageNoReset(const bool value)
{
    ////std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    for(auto && it: llrf.trace_data)
    {
        it.second.keep_rolling_average = value;
    }
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setKeepRollingAverageNoReset(const std::string&name, const bool value)
{

    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
        llrf.trace_data.at(n).keep_rolling_average = value;
    }
    message("liberallrfInterface setKeepRollingAverageNoReset, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setAllRollingAverageSize(const size_t value)
{
    for(auto && it: llrf.trace_data)
    {
        setTraceRollingAverageSize(it.first, value);
    }
    pause_1();
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setTraceRollingAverageSize(const std::string&name,const size_t value)
{
    const std::string n = fullLLRFTraceName(name);
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).rolling_average_size = value;
        //clearTraceRollingAverage(llrf.trace_data.at(n));
        return true;
    }
    message("liberallrfInterface setTraceRollingAverageSize, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------

std::vector<std::vector<double>> liberallrfInterface::getRollingAverageData(const std::string&name)const
{
    std::vector<std::vector<double>> r;
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        r =  llrf.trace_data.at(n).average_trace_values;
        r.push_back(llrf.trace_data.at(n).rolling_average);
        return r;
    }
    message("liberallrfInterface getRollingAverageData, trace ", n, " does not exist");
    return r;
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getRollingAverage(const std::string&name)const
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {

//        message("execute getRollingAverage, mnax = ",*max_element(std::begin( llrf.trace_data.at(n).rolling_average),
//                     std::end  ( llrf.trace_data.at(n).rolling_average) ));
        return llrf.trace_data.at(n).rolling_average;
    }
    message("liberallrfInterface getRollingAverage, trace ", n, " does not exist");
    std::vector<double> d(llrf.all_traces.num_elements_per_trace_used,UTL::DUMMY_DOUBLE);
    return d;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getTraceRollingAverageSize(const std::string&name)const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).rolling_average_size;
    }
    message("liberallrfInterface getTraceRollingAverageSize, trace ", n, " does not exist");
    return UTL::ZERO_SIZET;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getTraceRollingAverageCount(const std::string&name)const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).rolling_average_count;
    }
    message("liberallrfInterface getTraceRollingAverageCount, trace ", n, " does not exist");
    return UTL::ZERO_SIZET;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::isKeepingRollingAverage(const std::string&name)const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).keep_rolling_average;
    }
    message("liberallrfInterface isKeepingRollingAverage, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::hasRollingAverage(const std::string&name)const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).has_average;
    }
    message("liberallrfInterface hasRollingAverage, trace ", n, " does not exist");
    return false;
}











































//--------------------------------------------------------------------------------------------------
/*
    ___  __        __   ___     __        ___  ___  ___  __      __    __  ___
     |  |__)  /\  /  ` |__     |__) |  | |__  |__  |__  |__)    /__` |  / |__
     |  |  \ /~~\ \__, |___    |__) \__/ |    |    |___ |  \    .__/ | /_ |___


    Set the number of Buffer Traces in the ON_RECORD trace data
    Set the number of buffer traces held in the individual trace buffers
*/
bool liberallrfInterface::setAllTraceBufferSize(const size_t value)
{
    // min buffer size is FIVE
    size_t new_val = UTL::FIVE_SIZET;
    if(value > UTL::FIVE_SIZET)
    {
        new_val  = value;
    }

    std::vector<double> temp2( (size_t)llrf.all_traces.num_traces * llrf.all_traces.num_elements_per_trace, UTL::ZERO_DOUBLE);

    // temp_all_trace_item is used to push_back each call to updateTraces

    temp_all_trace_item.first = epicsTimeStamp();
    temp_all_trace_item.second = temp2;

    llrf.all_traces.data_buffer.assign( new_val, temp_all_trace_item);
    message("All-trace buffer size = ", llrf.all_traces.data_buffer.size());
    return llrf.all_traces.data_buffer.size() == new_val;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setIndividualTraceBufferSize(const size_t value)
{
    bool ret = true;
    bool temp;
    for(auto && it: llrf.trace_data)
    {
        temp = setIndividualTraceBufferSize(it.first, value);
        if(!temp)
        {
            ret = false;
        }
    }
    return ret;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setIndividualTraceBufferSize(const std::string&name, const size_t value)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string full_name = fullLLRFTraceName(name);
    std::vector<double> t( llrf.all_traces.num_elements_per_trace_used, UTL::ZERO_DOUBLE );

    // temp_trace_item is used to push_back each individual trace in updateAllTarces
    temp_trace_item.first = epicsTimeStamp();
    temp_trace_item.second = t;

    if(entryExists(llrf.trace_data, full_name))
    {
//        if(llrf.trace_data.at(full_name).buffersize != value)
//        {
            llrf.trace_data.at(full_name).buffersize = value;
            llrf.trace_data.at(full_name).data_buffer.assign(value , temp_trace_item);
            llrf.trace_data.at(full_name).trace_size = llrf.all_traces.num_elements_per_trace_used;
//        }
//        llrf.trace_data.at(full_name).hi_mask = t;
//        llrf.trace_data.at(full_name).lo_mask = t;
//        llrf.trace_data.at(full_name).rolling_average = t;
//        llrf.trace_data.at(full_name).rolling_sum = t;
//        llrf.trace_data.at(full_name).rolling_max = t;
//        llrf.trace_data.at(full_name).rolling_min = t;
//        llrf.trace_data.at(full_name).rolling_sd  = t;

        message(full_name," individual-trace buffer size = ", llrf.trace_data.at(full_name).data_buffer.size());
        return llrf.trace_data.at(full_name).data_buffer.size() == value;
    }
    return false;
}



//--------------------------------------------------------------------------------------------------
/*        __  ___         ___     __             __   ___  __
     /\  /  `  |  | \  / |__     |__) |  | |    /__` |__  /__`
    /~~\ \__,  |  |  \/  |___    |    \__/ |___ .__/ |___ .__/
*/
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateActivePulses()
{
    /* if there is no trigger or RF output then this IS not an active pulse */
    if( isTrigOff() )
    {
        llrf.can_increase_active_pulses = false;
    }
    else if( isNotRFOutput() )
    {
        llrf.can_increase_active_pulses = false;
    }
    /*  if we get exactly the same max as previous pulse we assume
        its a repeat and don't increase pulse count
    */
    else if(llrf.last_kly_fwd_power_max == llrf.kly_fwd_power_max)
    {
        llrf.duplicate_pulse_count += UTL::ONE_SIZET;
        llrf.can_increase_active_pulses = false;
        message("llrf.last_kly_fwd_power_max == llrf.kly_fwd_power_max ==> llrf.can_increase_active_pulses = false");

        std::vector<double> kfp =  getKlyFwdPower();
        bool equal = true;
        for(auto i = 0; i <kfp.size(); ++i )
        {
            if( kfp[i] != llrf.last_kly_fwd_pwr_trace[i] )
            {
                message("last_kly_fwd_power_max = kly_fwd_power_max, but trace data is different");
                equal = false;
                break;
            }
        }
        if(equal)
        {
                message("last_kly_fwd_power_max = kly_fwd_power_max, AND trace data is the same");
        }

    }
    else if(llrf.kly_fwd_power_max > llrf.active_pulse_kly_power_limit)
    {
        llrf.can_increase_active_pulses = true;
    }
    else
    {
        //message("llrf.last_kly_fwd_power_max < llrf.kly_fwd_power_max ==> llrf.can_increase_active_pulses = false");
        llrf.can_increase_active_pulses = false;
    }
    /* the llrf object holds this paramter as well as the trace */
    if(llrf.can_increase_active_pulses)
    {
        llrf.active_pulse_count += UTL::ONE_SIZET;
    }
    //message("AP max ", llrf.kly_fwd_power_max , " (", llrf.last_kly_fwd_power_max,") count = ",llrf.active_pulse_count, ", increase  = ",llrf.can_increase_active_pulses);
    /* update last_kly_fwd_power_max */
    llrf.last_kly_fwd_power_max  = llrf.kly_fwd_power_max;
    llrf.last_kly_fwd_pwr_trace  = getKlyFwdPower();
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getActivePulsePowerLimit() const
{
    return llrf.active_pulse_kly_power_limit;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::setActivePulsePowerLimit(const double value)
{
    llrf.active_pulse_kly_power_limit = value;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getActivePulseCount()const
{
    return llrf.active_pulse_count;
}
//____________________________________________________________________________________________
void liberallrfInterface::setActivePulseCount(const size_t value)
{
    llrf.active_pulse_count = value;
}
//____________________________________________________________________________________________
void liberallrfInterface::addActivePulseCountOffset(const size_t val)
{
    llrf.active_pulse_count += val;
}
//--------------------------------------------------------------------------------------------------
/*
     __   __   __   __   ___  __   __     ___  __        __   ___                         ___  __
    |__) |__) /  \ /  ` |__  /__` /__`     |  |__)  /\  /  ` |__     \  /  /\  |    |  | |__  /__`
    |    |  \ \__/ \__, |___ .__/ .__/     |  |  \ /~~\ \__, |___     \/  /~~\ |___ \__/ |___ .__/

    Simple processing for traces to find the max, mean etc.
*/
double liberallrfInterface::getTraceMax(const std::string& name)
{
    if(entryExists(llrf.trace_data, name))
    {
        llrf.trace_data.at(name).trace_max =
            *max_element(std::begin( llrf.trace_data.at(name).data_buffer.back().second ),
                         std::end  ( llrf.trace_data.at(name).data_buffer.back().second ) );
        return llrf.trace_data.at(name).trace_max;
    }
    return UTL::DUMMY_DOUBLE;
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getKlyFwdPwrMax()
{
    return getTraceMax(UTL::KLYSTRON_FORWARD_POWER);
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getKlyFwdPhaMax()
{
    return getTraceMax(UTL::KLYSTRON_FORWARD_PHASE);
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getKlyRevPwrMax()
{
    return getTraceMax(UTL::KLYSTRON_REVERSE_POWER);
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getKlyRevPhaMax()
{
    return getTraceMax(UTL::KLYSTRON_REVERSE_PHASE);
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getCavFwdPwrMax()
{
    return getTraceMax(fullLLRFTraceName(UTL::CAVITY_FORWARD_POWER));
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getCavFwdPhaMax()
{
    return getTraceMax(fullLLRFTraceName(UTL::CAVITY_FORWARD_PHASE));
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getCavRevPwrMax()
{
    return getTraceMax(fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER));
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getCavRevPhaMax()
{
    return getTraceMax(fullLLRFTraceName(UTL::CAVITY_REVERSE_PHASE));
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getProbePwrMax()
{
    return getTraceMax(UTL::CAVITY_PROBE_POWER);
}
//--------------------------------------------------------------------------------------------------
double liberallrfInterface::getProbePhaMax()
{
    return getTraceMax(UTL::CAVITY_PROBE_PHASE);
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getTraceValues(const std::string& name)const
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data,n))
    {
        return llrf.trace_data.at(n).data_buffer.back().second;
    }
    message("liberallrfInterface::getTraceValues ERROR, trace ", n, " does not exist");
    std::vector<double> r(UTL::ONE_SIZET, UTL::DUMMY_DOUBLE);//MAGIC_NUMBER
    return r;
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getCavRevPower()const
{
    return getTraceValues(fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER));
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getCavRevPhase()const
{
    return getTraceValues(fullLLRFTraceName(UTL::CAVITY_REVERSE_PHASE));
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getCavFwdPower()const
{
    return getTraceValues(fullLLRFTraceName(UTL::CAVITY_FORWARD_POWER));
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getCavFwdPhase()const
{
    return getTraceValues(fullLLRFTraceName(UTL::CAVITY_FORWARD_PHASE));
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getKlyRevPower()const
{
    return getTraceValues(UTL::KLYSTRON_REVERSE_POWER);
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getKlyRevPhase()const
{
    return getTraceValues(UTL::KLYSTRON_REVERSE_PHASE);
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getKlyFwdPower()const
{
    return getTraceValues(UTL::KLYSTRON_FORWARD_POWER);
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberallrfInterface::getKlyFwdPhase()const
{
    return getTraceValues(UTL::KLYSTRON_FORWARD_PHASE);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePower()const
{
    return getTraceValues(UTL::CAVITY_PROBE_POWER);
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getProbePhase()const
{
    return getTraceValues(UTL::CAVITY_PROBE_PHASE);
}









/*
    ___  __        __   ___           ___                                   ___  __
     |  |__)  /\  /  ` |__      |\/| |__   /\  |\ |    \  /  /\  |    |  | |__  /__`
     |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \|     \/  /~~\ |___ \__/ |___ .__/

*/
//-------------------------------------------------------------------------------------------------------------------
bool liberallrfInterface::setMeanStartEndTime(const double start, const double end, const std::string&name)
{
    bool a = setMeanStartIndex(name, getIndex(start));
    if(a)
    {
        a = setMeanStopIndex(name, getIndex(end));
    }
    return a;
}
//-------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getMeanStopIndex(const std::string&name)const
{
    if(entryExists(llrf.trace_data, name))
    {
        return llrf.trace_data.at(name).mean_stop_index;
    }
    return UTL::ZERO_SIZET;
}
//-------------------------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getMeanStartIndex(const std::string&name)const
{
    if(entryExists(llrf.trace_data, name))
    {
        return llrf.trace_data.at(name).mean_start_index;
    }
    return UTL::ZERO_SIZET;
}
//-------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------
void liberallrfInterface::updateTraceCutMeans()
{
    for(auto&it:llrf.trace_data)
    {
        getTraceCutMean(it.second);
    }
}
//-------------------------------------------------------------------------------------------------------------------
void liberallrfInterface::getTraceCutMean(llrfStructs::rf_trace_data& trace)
{
    /*
        The trace cut mean is the mean value between two indices on a trace
    */
    if(trace.mean_stop_index == trace.mean_start_index)
    {
//        message("trace.value[tracedata.mean_start_index] = ", trace.mean_start_index);
        trace.mean = trace.data_buffer.back().second[trace.mean_start_index];
    }
    else if(trace.mean_stop_index > trace.mean_start_index)
    {
        trace.mean = std::accumulate(trace.data_buffer.back().second.cbegin() + trace.mean_start_index,
                                     trace.data_buffer.back().second.cbegin() + trace.mean_stop_index,
                                     UTL::ZERO_DOUBLE) / ( trace.mean_stop_index - trace.mean_start_index);

//        if( trace.name == HRRG_CAVITY_PROBE_POWER )
//        {
//
//        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
double liberallrfInterface::getMean(const std::string&name)const
{
    if(entryExists(llrf.trace_data, name))
    {
        return llrf.trace_data.at(name).mean;
    }
    return UTL::DUMMY_DOUBLE;
}
//-------------------------------------------------------------------------------------------------------------------
void liberallrfInterface::setKeepKlyFwdPwrRS(bool val)
{
    llrf.keep_kly_fwd_pow_running_stat = val;
}
//-------------------------------------------------------------------------------------------------------------------
void liberallrfInterface::keepKlyFwdPwrRS()
{
    setKeepKlyFwdPwrRS(true);
}
//-------------------------------------------------------------------------------------------------------------------
void liberallrfInterface::dontKeepKlyFwdPwrRS()
{
    setKeepKlyFwdPwrRS(false);
}
//-------------------------------------------------------------------------------------------------------------------
std::tuple<size_t,double,double> liberallrfInterface::getKlyFwdPwrRSState(int ampSP_setting)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    if(entryExists(llrf.amp_set_kly_fwd_rs_state,ampSP_setting))
    {
        return llrf.amp_set_kly_fwd_rs_state.at(ampSP_setting);
    }
    return dummy_amp_set_kly_fwd_rs_state;
}
//-------------------------------------------------------------------------------------------------------------------
void liberallrfInterface::setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance)
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    /* Get ref to running stat object, key is amp_sp*/
    runningStat& rs_ref = llrf.amp_set_kly_fwd_rs[(int)llrf.amp_sp];
    /* apply passed values to running stat object*/
    rs_ref.setInitialValues(n, old_mean, old_variance);
}

//-------------------------------------------------------------------------------------------------------------------
bool liberallrfInterface::isKeepingKlyFwdPwrRS()
{
    return llrf.keep_kly_fwd_pow_running_stat;
}




//
//
////____________________________________________________________________________________________
//void liberallrfInterface::startTraceMonitoring()
//{
//    for(auto&& it:llrf.pvOneTraceMonStructs)
//    {
//        addTo_continuousMonitorStructs(it);
//    }
//}
////____________________________________________________________________________________________
//bool liberallrfInterface::startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv)
//{
//    if(Is_TracePV(pv))
//    {
//        if(isNotMonitoring(pv))
//        {
//            std::string name = getLLRFChannelName(pv);
//            //llrf.trace_data[name].traces.resize(llrf.trace_data[name].buffersize);
//            //for(auto && it2: llrf.trace_data[name].traces)
//            //    it2.value.resize(llrf.pvMonStructs.at(pv).COUNT);
//
//            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(pv), " (",getLLRFChannelName(pv) ,")");
//            continuousMonitorStructs.push_back(new llrfStructs::monitorStruct());
//            continuousMonitorStructs.back() -> monType   = pv;
//            continuousMonitorStructs.back() -> llrfObj   = &llrf;
//            continuousMonitorStructs.back() -> interface = this;
//            continuousMonitorStructs.back() -> name      = getLLRFChannelName(pv);
//            ca_create_subscription(llrf.pvMonStructs.at(pv).CHTYPE,
//                               llrf.pvMonStructs.at(pv).COUNT,
//                               llrf.pvMonStructs.at(pv).CHID,
//                               llrf.pvMonStructs.at(pv).MASK,
//                               liberallrfInterface::staticEntryLLRFMonitor,
//                               (void*)continuousMonitorStructs.back(),
//                               &continuousMonitorStructs.back() -> EVID);
//            std::stringstream ss;
//            ss <<"liberallrfInterface succesfully subscribed to LLRF trace monitor: " << ENUM_TO_STRING(pv);
//            std::string s1 = ss.str();
//            ss.str(std::string());
//            ss <<"!!TIMEOUT!! Subscription to LLRF Trace monitor " << ENUM_TO_STRING(pv) << " failed";
//            std::string s2 = ss.str();
//
//            /* makes sure the evid associated with the trace is also started */
//            llrfStructs::LLRF_PV_TYPE EVIDpv = getEVID_pv(pv);
//            if( EVIDpv != llrfStructs::LLRF_PV_TYPE::UNKNOWN )
//            {
//                debugMessage("ca_create_subscription to ", ENUM_TO_STRING(EVIDpv));
//                continuousMonitorStructs.push_back(new llrfStructs::monitorStruct());
//                continuousMonitorStructs.back() -> monType   = EVIDpv;
//                continuousMonitorStructs.back() -> llrfObj   = &llrf;
//                continuousMonitorStructs.back() -> interface = this;
//                continuousMonitorStructs.back() -> name      = getLLRFChannelName(EVIDpv);
//                ca_create_subscription(llrf.pvMonStructs.at(EVIDpv).CHTYPE,
//                                       llrf.pvMonStructs.at(EVIDpv).COUNT,
//                                       llrf.pvMonStructs.at(EVIDpv).CHID,
//                                       llrf.pvMonStructs.at(EVIDpv).MASK,
//                                       liberallrfInterface::staticEntryLLRFMonitor,
//                                       (void*)continuousMonitorStructs.back(),
//                                       &continuousMonitorStructs.back() -> EVID);
//            }
//            int status = sendToEpics("ca_create_subscription",s1.c_str(),s2.c_str());
//            if (status == ECA_NORMAL)
//            {
//                debugMessage(ENUM_TO_STRING(pv)," Monitor Started");
//            }
//            else
//            {
//                killMonitor(continuousMonitorStructs.back());
//            }
//            return isMonitoring(pv);
//        }
//    }
//    else
//        return false;
//}
////____________________________________________________________________________________________
//bool liberallrfInterface::startTraceMonitoring(const std::string& name)
//{
//    std::string n = fullLLRFTraceName(name);
//    debugMessage("STARTING ", n, ", getLLRFPVType(name) = ", getLLRFPVType(n));
//    return startTraceMonitoring(getLLRFPVType(n));
//}
//
//
//




























// we're not going to do this as EVID is flakey
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::updateAllTracesEVID(const event_handler_args& args)
{
//    bool update_buffer = false;
//    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
//    const dbr_time_string* p = nullptr;
//    p = (const struct dbr_time_string*)args.dbr;
//    //message("m1a");
//
//    if(p != nullptr)
//    {
//        //message("m1aa");
//        if((*p).value == '\0')//MAGIC_STRING
//        {
//            llrf.all_traces.EVID_Str = "";
//            message("!!updateAllTracesEVID!!: null string passsed back by EPICS!!!!");
//        }
//        else
//        {
//            if( llrf.all_traces.EVID_Str == p->value )
//            {
//                //message("updateAllTracesEVID called with same EVID_Str as previous call, ", llrf.all_traces.EVID_Str);
//            }
//            else
//            {
//                update_buffer = true;
//                llrf.all_traces.EVID_Str = p->value;
//                //message("t.EVID ", t.EVID);
//
//                llrf.all_traces.EVID_etime = p->stamp;
//                updateTime(llrf.all_traces.EVID_etime,
//                           llrf.all_traces.EVID_time,
//                           llrf.all_traces.EVID_timeStr);
//
//                if(isDigits(llrf.all_traces.EVID_Str))//MAGIC_STRING
//                {
//                    llrf.all_traces.EVID = getNum(llrf.all_traces.EVID_Str);
//                }
//                //message("llrf.all_traces.EVID = ", llrf.all_traces.EVID);
//            }
//        }
//    }
//    else
//    {
//        llrf.all_traces.EVID_Str = "nullptr";
//        llrf.all_traces.EVID = UTL::MINUS_ONE_INT;
//    }
//
//    if(update_buffer)
//    {
//        /*
//            update buffer with latest values,
//            Q:? is it better to cretae a temp object and add that to buffer or,
//                should we push back the buffer and add straight to it??
//        */
//        //std::pair<int,std::string> temp;
//
//        llrf.all_traces.EVID_buffer.push_back();
//
//        llrf.all_traces.EVID_buffer.back().first = llrf.all_traces.EVID;
//        llrf.all_traces.EVID_buffer.back().second = llrf.all_traces.EVID_Str;
//
//
//        if(EVID_0 == -1)
//        {
//            EVID_0 = llrf.all_traces.EVID_buffer.back().first;
//
//            message("EVID_0 = ", EVID_0  );
//        }
//
//    //    message("New back: ", llrf.all_traces.EVID_buffer.back().first, " / ",llrf.all_traces.EVID_buffer.back().second);
//    //    message("New end1: ", (llrf.all_traces.EVID_buffer.end() - 1) ->first, " / ", (llrf.all_traces.EVID_buffer.end()-1)->second);
//    //    message("New end2: ", (llrf.all_traces.EVID_buffer.end() - 2) ->first, " / ", (llrf.all_traces.EVID_buffer.end()-2)->second);
//
//        auto it1 = llrf.all_traces.EVID_buffer.end() - 1;
//        auto it2 = llrf.all_traces.EVID_buffer.end() - 2;
//
//        if( it1->first - it2->first  != UTL::ONE_INT )
//        {
//            evid_miss_count += UTL::ONE_INT;
//            //message("WE LOST AN ALL-TRACE EVID, ", evid_miss_count );
//        }
//
//        evid_call_count += UTL::ONE_INT;
//
//        //message(evid_call_count, ", ",llrf.all_traces.EVID, " - ", EVID_0," = ", llrf.all_traces.EVID - EVID_0 );
//
//
//        //message("evid = , ", evid_call_count," / ", llrf.all_traces.EVID - EVID_0);
//
//
//        if( evid_call_count % 600 == 0 )
//        {
//            message(llrf.all_traces.EVID_timeStr, ", evid_call_count = , ", evid_call_count,
//                    " Lost EVID ", llrf.all_traces.EVID - EVID_0 - evid_call_count);
//        }
//
//    }

}








/*   _____  _____          _   _
    / ____|/ ____|   /\   | \ | |
   | (___ | |       /  \  |  \| |
    \___ \| |      / /\ \ | . ` |
    ____) | |____ / ____ \| |\  |
   |_____/ \_____/_/    \_\_| \_|
*/
//____________________________________________________________________________________________
bool liberallrfInterface::setTraceSCAN(const std::string& name, const llrfStructs::LLRF_SCAN value)
{
    const std::string n = fullLLRFTraceName(name);
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    if(entryExists(llrf.trace_data, n))
    {
        for(auto&& it: llrf.pvMonStructs)
        {
            if(it.second.name == n && Is_SCAN_PV(it.first))
            {
//                if(isMonitoring(it.first))
//                {
                    return setValue(it.second,value);
//                }
            }
        }
    }
    return false;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setAllSCANToPassive()
{
    bool r = true;
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    for(auto &&it:llrf.trace_scans)
    {
        //message("setAllSCANToPassive,  pvSCANStructs.size() = ", it.second.pvSCANStructs.size());
        for(auto&& it2: it.second.pvSCANStructs)
        {
            //message("set passive for ", ENUM_TO_STRING(it2.first));
            bool t = setValue(it2.second, llrfStructs::LLRF_SCAN::PASSIVE);
            if( !t )
            {
                r = false;
            }
        }
    }
    return r;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setTORSCANToIOIntr()
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();

    if(llrf.all_traces.scan == llrfStructs::LLRF_SCAN::IO_INTR )
    {
        return true;
    }
    if( entryExists(llrf.pvOneTraceMonStructs, llrfStructs::LLRF_PV_TYPE::ALL_TRACES_SCAN) )
    {

        //message("llrfStructs::LLRF_SCAN::IO_INTR = ", llrfStructs::LLRF_SCAN::IO_INTR);
        return setValue(llrf.pvOneTraceMonStructs.at(llrfStructs::LLRF_PV_TYPE::ALL_TRACES_SCAN),
                        llrfStructs::LLRF_SCAN::IO_INTR);
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
bool liberallrfInterface::setTORSCANToPassive()
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();

    if(llrf.all_traces.scan == llrfStructs::LLRF_SCAN::PASSIVE )
    {
        return true;
    }
    if( entryExists(llrf.pvOneTraceMonStructs, llrfStructs::LLRF_PV_TYPE::ALL_TRACES_SCAN) )
    {

        //message("llrfStructs::LLRF_SCAN::IO_INTR = ", llrfStructs::LLRF_SCAN::IO_INTR);
        return setValue(llrf.pvOneTraceMonStructs.at(llrfStructs::LLRF_PV_TYPE::ALL_TRACES_SCAN),
                        llrfStructs::LLRF_SCAN::PASSIVE);
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
bool liberallrfInterface::resetTORSCANToIOIntr()
{
    if(setTORSCANToPassive())
    {
        pause_500();
        return setTORSCANToIOIntr();
    }
    return false;
}


//-------------------------------------------------------------------------------------------------------
int liberallrfInterface::getTORACQM()const
{
    return (int)llrf.all_traces.acqm;
}
//-------------------------------------------------------------------------------------------------------
int liberallrfInterface::getTORSCAN()const
{
    return (int)llrf.all_traces.scan;
}
//____________________________________________________________________________________________
bool liberallrfInterface::setTORACQMEvent()
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();

    if(llrf.all_traces.acqm == llrfStructs::LLRF_ACQM::ACQM_EVENT )
    {
        return true;
    }
    if(entryExists(llrf.pvOneTraceMonStructs, llrfStructs::LLRF_PV_TYPE::ALL_TRACES_ACQM))
    {
        /// THIS DOES NTO WORK PROPERLY!!!!!
        /// THIS DOES NTO WORK PROPERLY!!!!!
        /// THIS DOES NTO WORK PROPERLY!!!!!
        /// THIS DOES NTO WORK PROPERLY!!!!!
        /// THIS DOES NTO WORK PROPERLY!!!!!


        message("llrfStructs::LLRF_ACQM::ACQM_EVENT = ", llrfStructs::LLRF_ACQM::ACQM_EVENT);
        return setValue(llrf.pvOneTraceMonStructs.at(llrfStructs::LLRF_PV_TYPE::ALL_TRACES_ACQM),
                        llrfStructs::LLRF_ACQM::ACQM_EVENT);
    }
    return false;
}



//____________________________________________________________________________________________
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
bool liberallrfInterface::IsNot_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    return !Is_SCAN_PV(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::Is_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    bool r = false;
//    if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_LOCAL_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_AMP_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
//    else if(pv == llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_DER_SCAN)
//    {
//        r = true;
//    }
    return r;
}

























































































































//____________________________________________________________________________________________
//void liberallrfInterface::updateTrace(const event_handler_args& args, llrfStructs::rf_trace_data& trace)
//{   /* called from staticEntryLLRFMonitor, and will update a power or a phase trace */
//    /* ref to trace to update */
////    auto& current_trace = trace.traces[trace.current_trace];
//
//    /* update the new trace values */
//    updateValues(args, current_trace);
//
//    /* find the maximum value in the trace */
//    trace.latest_max = *std::max_element(current_trace.value.begin(), current_trace.value.end());
//
//    /* if Klystron Forward Power, update can_increase_active_pulses, based on max value */
//    if(trace.name == UTL::KLYSTRON_FORWARD_POWER)
//    {
//        updateCanIncreaseActivePulses(trace.latest_max);
//    }
//    /* on detecting an outside-mask-trace we might be adding 'future traces'
//       the place to add them, and how many to add are contained in a vector of pairs:
//        outside_mask_data_index_to_add_future_traces_to
//       the first  number is the index  of outside_mask_traces to add data to
//       the second number is the number of traces STILL to add
//       when the second number gets to zero we shoudl delete the entry, as all data has been collected */
//    for(auto && it = trace.outside_mask_data_index_to_add_future_traces_to.begin();
//                it != trace.outside_mask_data_index_to_add_future_traces_to.end() /* not hoisted */; /* no increment */)
//    {
//        llrf.outside_mask_traces[it->first].traces.push_back(current_trace);
//        if(llrf.outside_mask_traces[it->first].traces.size() == llrf.outside_mask_traces[it->first].num_traces_to_collect)
//        {
//            llrf.outside_mask_traces[it->first].is_collecting = false;
//        }
//        it->second -= UTL::ONE_SIZET;
//        if(it->second == UTL::ZERO_SIZET)
//        {
//            trace.outside_mask_data_index_to_add_future_traces_to.erase(it);
//        }
//        else
//        {
//            ++it;
//        }
//    }
//
//    /* check masks */
//    if(shouldCheckMasks(trace))
//    {   //debugMessage("CHECKING MASKS ",trace.name);
//        int trace_in_mask_result = updateIsTraceInMask(trace);
//        handleTraceInMaskResult(trace, trace_in_mask_result);
//    }
//
//    /* calc means */
//    updateTraceCutMean(trace, current_trace);
//
//    /* the trace index tells us which part of 'traces' is the next to update
//       it circles 'round
//       first,set the ltest trace index, used for accesing latest data */
//    trace.latest_trace_index = trace.current_trace;
//    /* then update all indices */
//    updateTraceIndex(trace.current_trace, trace.traces.size());
//    updateTraceIndex(trace.previous_trace, trace.traces.size());
//    updateTraceIndex(trace.previous_previous_trace, trace.traces.size());
//    /* update shot count
//       the difference between this and pulse count indicates how many traces we miss! */
//    trace.shot += UTL::ONE_SIZET;
    //message("trace.shot = ", trace.shot);
//}
//____________________________________________________________________________________________
void liberallrfInterface::updateValues(const event_handler_args& args, llrfStructs::rf_trace& trace)
{
    /* this function actually gets the new values from EPICS and adds them to the trace.value vector
       all LLRF traces should be updated using this function

    /* pointer to array we are expecting depends on type channel */
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
//____________________________________________________________________________________________
bool liberallrfInterface::IsNot_EVID_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    return !Is_EVID_PV(pv);
}
//____________________________________________________________________________________________
bool liberallrfInterface::Is_EVID_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    bool r = false;

    return r;
}
//____________________________________________________________________________________________
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
//--------------------------------------------------------------------------------------------
bool liberallrfInterface::Is_Pulse_Shape_Vector_PV(llrfStructs::LLRF_PV_TYPE pv)
{
    if(pv == llrfStructs::LLRF_PV_TYPE::PULSE_SHAPE)
        return true;
    return false;
}
//
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
    const std::string phase = fullLLRFTraceName(phase_trace);
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
    return false;
}

//____________________________________________________________________________________________
void liberallrfInterface::offsetTimer(long long value)
{   // add an ofset to the internal timer
    llrf.timer_start += value;
}
//____________________________________________________________________________________________
//____________________________________________________________________________________________
bool liberallrfInterface::setPulseLength(double value)
{
    message("setPulseLength setting ",value);
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
    message("liberallrfInterface::setPhiSP = ", value);
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
void liberallrfInterface::setCrestPhiLLRF(double value) // in LLRF units
{
    llrf.crestPhi = value;
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
bool liberallrfInterface::setMaskInfiniteEnd(const std::string& trace_name, size_t index)
{
    return false;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getMaskInfiniteEndByPowerIndex(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
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
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrHiMask(const std::vector<double>& value)
{
    return setHiMask(fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrLoMask(const std::vector<double>& value)
{
    return setLoMask(fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER),value);
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setPercentMask(s1,s2,s3,s4,value,fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavRevPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setAbsoluteMask(s1,s2,s3,s4,value,fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER));
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavFwdPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setPercentMask(s1,s2,s3,s4,value,fullLLRFTraceName(UTL::CAVITY_FORWARD_POWER));
}
//____________________________________________________________________________________________
bool liberallrfInterface::setCavFwdPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return setAbsoluteMask(s1,s2,s3,s4,value,fullLLRFTraceName(UTL::CAVITY_FORWARD_POWER));
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
    std::stringstream ss;
    ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    bool ret = false;
    ca_put(pvs.CHTYPE, pvs.CHID, &value);
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
#
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
//std::vector<std::string> liberallrfInterface::getChannelNames()const
//{
//    std::vector<std::string>  r;
//    for(auto && it:llrf.pvMonStructs)
//    {
//        if(Is_TracePV(it.first))
//        {
//            r.push_back(it.second.name);
//        }
//    }
//    return r;
//}
//____________________________________________________________________________________________
std::vector<std::string> liberallrfInterface::getTraceNames()const
{
    std::vector<std::string>  r;
    for(auto && it:llrf.trace_data)
    {
        r.push_back(it.first);
    }
    return r;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getLLRFPVType(const std::string& name)const
{
    std::string n = fullLLRFTraceName(name);
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
std::string liberallrfInterface::getLLRFChannelName(const llrfStructs::LLRF_PV_TYPE pv)const
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
std::string liberallrfInterface::fullLLRFTraceName(const std::string& name_in)const
{
    using namespace UTL;
    std::string name = name_in;
    if(name == KRPOW)
    {
        name = KLYSTRON_REVERSE_POWER;
    }
    else if(name == KRPHA)
    {
        name = KLYSTRON_REVERSE_PHASE;
    }
    else if(name == KFPOW)
    {
        name = KLYSTRON_FORWARD_POWER;
    }
    else if(name == KFPHA)
    {
        name = KLYSTRON_FORWARD_PHASE;
    }
    else if(name == CRPOW)
    {
        name = CAVITY_REVERSE_PHASE;
    }
    else if(name == CRPHA)
    {
        name = CAVITY_REVERSE_PHASE;
    }
    else if(name == CFPHA)
    {
        name = CAVITY_FORWARD_PHASE;
    }
    else if(name == CFPOW)
    {
        name = CAVITY_FORWARD_POWER;
    }
    else if(name == CPPOW)
    {
        name = CAVITY_PROBE_POWER;
    }
    else if(name == CPPHA)
    {
        name = CAVITY_PROBE_PHASE;
    }


    if(myLLRFType == llrfStructs::LLRF_TYPE::CLARA_HRRG || myLLRFType == llrfStructs::LLRF_TYPE::VELA_HRRG)
    {
        if(name == CAVITY_REVERSE_PHASE)
            return HRRG_CAVITY_REVERSE_PHASE;

        if(name == CAVITY_FORWARD_PHASE)
            return HRRG_CAVITY_FORWARD_PHASE;

        if(name == CAVITY_REVERSE_POWER)
            return HRRG_CAVITY_REVERSE_POWER;

        if(name == CAVITY_FORWARD_POWER)
            return HRRG_CAVITY_FORWARD_POWER;

        if(name == CAVITY_PROBE_POWER)
            return HRRG_CAVITY_PROBE_POWER;

        if(name == CAVITY_PROBE_PHASE)
            return HRRG_CAVITY_PROBE_PHASE;


    }
    else if(myLLRFType == llrfStructs::LLRF_TYPE::CLARA_LRRG || myLLRFType == llrfStructs::LLRF_TYPE::VELA_LRRG)
    {
        if(name == CAVITY_REVERSE_PHASE)
            return LRRG_CAVITY_REVERSE_PHASE;

        if(name == CAVITY_FORWARD_PHASE)
            return LRRG_CAVITY_FORWARD_PHASE;

        if(name == CAVITY_REVERSE_POWER)
            return LRRG_CAVITY_REVERSE_POWER;

        if(name == CAVITY_FORWARD_POWER)
            return LRRG_CAVITY_FORWARD_POWER;
    }
    else if(myLLRFType == llrfStructs::LLRF_TYPE::L01)
    {
        if(name == CAVITY_REVERSE_PHASE)
            return L01_CAVITY_REVERSE_PHASE;

        if(name == CAVITY_FORWARD_PHASE)
            return L01_CAVITY_FORWARD_PHASE;

        if(name == CAVITY_REVERSE_POWER)
            return L01_CAVITY_REVERSE_POWER;

        if(name == CAVITY_FORWARD_POWER)
            return L01_CAVITY_FORWARD_POWER;

        if(name == CAVITY_PROBE_POWER)
            return L01_CAVITY_PROBE_POWER;

        if(name == CAVITY_PROBE_PHASE)
            return L01_CAVITY_PROBE_PHASE;
    }

    return name;
}




std::string liberallrfInterface::shortLLRFTraceName(const std::string& name_in)const
{
    using namespace UTL;
    std::string name = name_in;
    if( stringIsSubString(name,KLYSTRON_REVERSE_POWER))
    {
        name = KRPOW;
    }
    else if( stringIsSubString(name,KLYSTRON_REVERSE_PHASE))
    {
        name = KRPHA;
    }
    else if( stringIsSubString(name, KLYSTRON_FORWARD_POWER))
    {
        name = KFPOW;
    }
    else if(stringIsSubString(name, KLYSTRON_FORWARD_PHASE))
    {
        name = KFPHA;
    }
    else if(stringIsSubString(name, CAVITY_REVERSE_POWER))
    {
        name = CRPOW;
    }
    else if(stringIsSubString(name, CAVITY_REVERSE_PHASE))
    {
        name = CRPHA;
    }
    else if(stringIsSubString(name, CAVITY_FORWARD_PHASE))
    {
        name = CFPHA;
    }
    else if(stringIsSubString(name, CAVITY_FORWARD_POWER))
    {
        name = CFPOW;
    }
    else if(stringIsSubString(name, CAVITY_PROBE_POWER))
    {
        name = CPPOW;
    }
    else if(stringIsSubString(name, CAVITY_PROBE_PHASE))
    {
        name = CPPHA;
    }

   // message("shortLLRFTraceName name_in = ", name_in, ", name_out =  ", name);

    return name;
}

//____________________________________________________________________________________________
llrfStructs::LLRF_PV_TYPE liberallrfInterface::getEVID_pv(llrfStructs::LLRF_PV_TYPE pv)
{
    switch(pv)
    {
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PWR_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH1_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH2_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH3_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH4_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH5_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH6_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH7_PHASE_REM_EVID;
//         case llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM:
//             return llrfStructs::LLRF_PV_TYPE::LIB_CH8_PHASE_REM_EVID;
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
/*
       _________.__               .__             ________        __    __
      /   _____/|__| _____ ______ |  |   ____    /  _____/  _____/  |__/  |_  ___________  ______
      \_____  \ |  |/     \\____ \|  | _/ __ \  /   \  ____/ __ \   __\   __\/ __ \_  __ \/  ___/
      /        \|  |  Y Y  \  |_> >  |_\  ___/  \    \_\  \  ___/|  |  |  | \  ___/|  | \/\___ \
     /_______  /|__|__|_|  /   __/|____/\___  >  \______  /\___  >__|  |__|  \___  >__|  /____  >
             \/          \/|__|             \/          \/     \/                \/           \/

    i.e. things to expose to python
*/
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getPulseShape()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.pulse_shape;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::getPulseShapeUpToDate()const
{
    return llrf.pulse_shape_up_to_date;
}
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getTimeVector()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.time_vector;
}


//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setPulseShape(std::vector<double>& values)
{
    bool ret = false;



    if(entryExists(llrf.pvMonStructs,llrfStructs::LLRF_PV_TYPE::PULSE_SHAPE))
    {
        llrfStructs::pvStruct& pvs = llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::PULSE_SHAPE);

        if( values.size() == pvs.COUNT )
        {
            message("Trying to set values");
            std::stringstream ss;
            ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << values[0] << "...";

            // int ca_array_put ( chtype TYPE,unsigned long COUNT,chid CHID, const void *PVALUE)
            ca_array_put(pvs.CHTYPE, pvs.COUNT, pvs.CHID, &values[0]);
            //debugMessage(ss);
            ss.str("");
            ss << "Timeout setting llrf, " << ENUM_TO_STRING(pvs.pvType) << " value to " << values[0] << "...";

            message("Trying sendToEpics");

            int status = sendToEpics("ca_array_put","",ss.str().c_str());
            if(status==ECA_NORMAL)
            {
                ret=true;
            }
        }
    }
    message("setPulseShape");
    return ret;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::applyPulseShape()
{
    bool ret = false;
    if(entryExists(llrf.pvMonStructs,llrfStructs::LLRF_PV_TYPE::PULSE_SHAPE_APPLY))
    {
        llrfStructs::pvStruct &pvs = llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::PULSE_SHAPE_APPLY);

        std::stringstream ss;
        ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << UTL::ONE_US;

        ca_put(pvs.CHTYPE, pvs.CHID, &UTL::ONE_US);
        //debugMessage(ss);
        ss.str("");
        ss << "Timeout setting llrf, " << ENUM_TO_STRING(pvs.pvType) << " value to " << UTL::ONE_US;
        int status = sendToEpics("ca_array_put","",ss.str().c_str());
        if(status==ECA_NORMAL)
        {
            ret=true;
        }
    }
    return ret;
    return true;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setAndApplyPulseShape(std::vector<double>& values)
{
    message("setAndApplyPulseShape");
    if(setPulseShape(values))
    {
        pause_50();
        message("applyPulseShape");
        return applyPulseShape();
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
size_t liberallrfInterface::getIndex(const double time) const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    auto r = std::lower_bound(llrf.time_vector.begin(), llrf.time_vector.end(),time);
    return r - llrf.time_vector.begin();
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiCalibration()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.phiCalibration;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpCalibration()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.ampCalibration;
}
//____________________________________________________________________________________________
double liberallrfInterface::getCrestPhiLLRF()const// in LLRF units
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.crestPhi;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpFF()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.amp_ff;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpSP()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.amp_sp;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpMVM()const// physics units
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.amp_MVM;
}
//____________________________________________________________________________________________
double liberallrfInterface::getAmpLLRF()const// physics units
{
    return getAmpFF();
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiFF()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.phi_ff;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiSP()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.phi_sp;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiDEG()const// physics units
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.phi_DEG;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPhiLLRF()const// physics units
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return getPhiFF();
}
//____________________________________________________________________________________________
double liberallrfInterface::getPulseLength()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.pulse_length;
}
//____________________________________________________________________________________________
double liberallrfInterface::getPulseOffset() const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.pulse_offset;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getPulseCount()const
{
    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    return llrf.pulse_count;

}
//____________________________________________________________________________________________
double liberallrfInterface::getBreakDownRate()
{
    return llrf.breakdown_rate;
}
//____________________________________________________________________________________________
double liberallrfInterface::getTime(const size_t index)const
{
    return llrf.time_vector[index];
}
//____________________________________________________________________________________________
const llrfStructs::liberallrfObject& liberallrfInterface::getLLRFObjConstRef()
{
    return llrf;
}
//____________________________________________________________________________________________
const llrfStructs::rf_trace_data& liberallrfInterface::getTraceDataConstRef(const std::string& name)
{
    const std::string n = fullLLRFTraceName(name);
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
//-----------------------------------------------------------------------------------------------------------
std::pair<std::string, std::vector<double>> liberallrfInterface::getTraceData(const std::string& name)const
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::pair<std::string, std::vector<double>> ret;
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        double temp_d;
        updateTime( llrf.trace_data.at(n).data_buffer.back().first, temp_d, ret.first);
        ret.second = llrf.trace_data.at(n).data_buffer.back().second;
    }
    else
    {
        message("liberallrfInterface::getTraceDataConstRef() ERROR, trace ", n, " does not exist");
    }
    return ret;
}
//-----------------------------------------------------------------------------------------------------------
std::vector<std::pair<std::string, std::vector<double>>> liberallrfInterface::getTraceDataBuffer(const std::string& name)const
{
    std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::vector< std::pair<std::string, std::vector<double>>> r;

    const std::string n = fullLLRFTraceName(name);
    for(auto i: llrf.trace_data.at(n).data_buffer)
    {
        std::pair<std::string, std::vector<double>> d;
        double temp_d;
        updateTime( i.first, temp_d, d.first);
        d.second = i.second;
        r.push_back(d);
       // message(index," ", r.back().first);
    }
    return r;
}
//____________________________________________________________________________________________
llrfStructs::LLRF_TYPE liberallrfInterface::getType()const
{
    return llrf.type;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getTraceLength()const
{
    return llrf.traceLength;
}

//____________________________________________________________________________________________
llrfStructs::TRIG liberallrfInterface::getTrigSource()const
{
    return llrf.trig_source;
}
//____________________________________________________________________________________________
size_t liberallrfInterface::getNumBufferTraces(const std::string&name)const
{
    const std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        return llrf.trace_data.at(n).buffersize;
    }
    else
        message("liberallrfInterface::getNumBufferTraces ERROR, trace ", n, " does not exist");
    return UTL::ZERO_SIZET;
}
//____________________________________________________________________________________________
//size_t liberallrfInterface::getNumRollingAverageTraces(const std::string&name)
//{
//    const std::string n = fullLLRFTraceName(name);
//    if(entryExists(llrf.trace_data,n))
//    {
//        return llrf.trace_data.at(n).average_size;
//    }
//    else
//        message("liberallrfInterface::getNumRollingAverageTraces ERROR, trace ", n, " does not exist");
//    return UTL::ZERO_SIZET;
//}
//
// GET LATEST TRACE VALUES

//
// GET LATEST TRACE DATA STRUCT
//____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getTraceData(const std::string& name)
//{
//    const std::string n = fullLLRFTraceName(name);
//    if(entryExists(llrf.trace_data,n))
//    {
//        //return llrf.trace_data.at(name).traces.back();
//        return llrf.trace_data.at(n).traces.back();
//    }
//    else
//        message("liberallrfInterface::getTraceData ERROR, trace ", n, " does not exist");
//    llrfStructs::rf_trace r;
//    return r;
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getCavRevPowerData()
//{
//    return getTraceData(UTL::CAVITY_REVERSE_POWER);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getCavRevPhaseData()
//{
//    return getTraceData(UTL::CAVITY_REVERSE_PHASE);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getCavFwdPowerData()
//{
//    return getTraceData(UTL::CAVITY_FORWARD_POWER);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getCavFwdPhaseData()
//{
//    return getTraceData(UTL::CAVITY_FORWARD_PHASE);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getKlyRevPowerData()
//{
//    return getTraceData(UTL::KLYSTRON_REVERSE_POWER);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getKlyRevPhaseData()
//{
//    return getTraceData(UTL::KLYSTRON_REVERSE_PHASE);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getKlyFwdPowerData()
//{
//    return getTraceData(UTL::KLYSTRON_FORWARD_POWER);
//}
////____________________________________________________________________________________________
//llrfStructs::rf_trace liberallrfInterface::getKlyFwdPhaseData()
//{
//    return getTraceData(UTL::KLYSTRON_FORWARD_PHASE);
//}
//
//


//
// GET THE ENTIRE TRACE DATA BUFFER
//
//____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getTraceBuffer(const std::string& name)
//{
//    const std::string n = fullLLRFTraceName(name);
//    if(entryExists(llrf.trace_data,n))
//    {
//        return llrf.trace_data.at(n).traces;
//    }
//    else
//        message("liberallrfInterface::getTraceBuffer ERROR, trace ", n, " does not exist");
//    std::vector<llrfStructs::rf_trace> r;
//    return r;
//}
//____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavRevPowerBuffer()
//{
//    return getTraceBuffer(fullLLRFTraceName(UTL::CAVITY_REVERSE_POWER));
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavRevPhaseBuffer()
//{
//    return getTraceBuffer(fullLLRFTraceName(UTL::CAVITY_REVERSE_PHASE));
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavFwdPowerBuffer()
//{
//    return getTraceBuffer(fullLLRFTraceName(UTL::CAVITY_FORWARD_POWER));
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getCavFwdPhaseBuffer()
//{
//    return getTraceBuffer(fullLLRFTraceName(UTL::CAVITY_FORWARD_PHASE));
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyRevPowerBuffer()
//{
//    return getTraceBuffer(UTL::KLYSTRON_REVERSE_POWER);
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyRevPhaseBuffer()
//{
//    return getTraceBuffer(UTL::KLYSTRON_REVERSE_PHASE);
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyFwdPowerBuffer()
//{
//    return getTraceBuffer(UTL::KLYSTRON_FORWARD_POWER);
//}
////____________________________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberallrfInterface::getKlyFwdPhaseBuffer()
//{
//    return getTraceBuffer(UTL::KLYSTRON_REVERSE_PHASE);
//}
//
// GET TRACE AVERAGES
//
//____________________________________________________________________________________________
std::vector<double> liberallrfInterface::getAverageTraceData(const std::string& name)const
{
    const std::string n = fullLLRFTraceName(name);
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
////____________________________________________________________________________________________















//--------------------------------------------------------------------------------------------------
/*   __             __   ___                    __        __
    |__) |__|  /\  /__` |__     |  | |\ | |  | |__)  /\  |__)
    |    |  | /~~\ .__/ |___    \__/ | \| |/\| |  \ /~~\ |

    NOT USED AT THE MOMENT, BUT LEAVING IN Just in case, this tries to take out the
    360 degree wrap-around on phase data
*/
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setUnwrapPhaseTolerance(const double value)
{
    bool r = true;
    for(auto&&it:llrf.trace_data)
    {
        bool r2 = setUnwrapPhaseTolerance(it.first,value);
        if( r2 == false)
        {
            r = false;
        }
    }
    return r;
}
//--------------------------------------------------------------------------------------------------
bool liberallrfInterface::setUnwrapPhaseTolerance(const std::string& name,const double value)
{
//    //std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
    std::string n = fullLLRFTraceName(name);
    if(entryExists(llrf.trace_data, n))
    {
        llrf.trace_data.at(n).phase_tolerance = value;
        return true;
    }
    message("liberallrfInterface::setMaskValue ERROR, trace ", n, " does not exist");
    return false;
}
//--------------------------------------------------------------------------------------------------
void liberallrfInterface::unwrapPhaseTrace(llrfStructs::rf_trace_data& trace_data)
{
/*
    Mathematica function to do this, based on a tolerance of phase jumps ...
    Unwrap[lst_List,D_,tolerance_]:= Module[{tol,jumps}, tol=If[Head[tolerance]===Scaled,\[CapitalDelta] tolerance[[1]],tolerance];

    (*Finds differences between subsequent points*)
    jumps=Differences[lst];
    (* Finds where difference is larger than a given tolerance and sets these numbers to +- 1 depending on sign of  difference and all others to zero*)
    jumps=-Sign[jumps]Unitize[Chop[Abs[jumps],tol]];
    (* Makes list of successive accumulated total of our 1s & -1s ie. history of how many jumps have been made,
       adds a 0 to the beginning to make up for the one lost calculating differences, then multiplies by 360  *)
    jumps=D Prepend[Accumulate[jumps],0];
    (*adds our multiples of 360 to the original phase*)
    jumps+lst
    ]

*/
    // Things TO DO
    // only unwrap numbers between mask start and end ...
    // make faster , maybe all inside one loop

//    std::vector<double>& phase_trace = trace_data.data_buffer.back().second;
//
//    double jump_sum = 0.0;
//    for(auto i = 0; i < phase_trace.size() - 1; ++i )
//    {
//        if( i < trace_data.mask_start )
//        {
//
//        }
//        else if( trace_data.mask_start <= i && i < trace_data.mask_end )
//        {
//            double jump = phase_trace[i + 1] - phase_trace[i];
//
//            double t = fabs( jump );
//            if(t < trace_data.phase_tolerance )//MAGIC_NUMBER
//            {
//                t = UTL::ZERO_DOUBLE;
//            }
//            else
//            {
//                t = UTL::ONE_DOUBLE;
//            }
//            jump = -(double)sgn(jump) * t;
//
//            jump_sum += jump;
//
//            //j *= 180.0;//MAGIC_NUMBER
//
//            if( i < trace_data.mask_start )
//            {
//
//            }
//            phase_trace[i] += jump_sum * trace_data.phase_tolerance;
//        }
//        else
//        {
//            phase_trace[i] += jump_sum * trace_data.phase_tolerance;
//        }
//
//    }
//    phase_trace.back() +=  jump_sum * trace_data.phase_tolerance;
//

//
////    for(auto& it: phase_trace)
////    {
////        std::cout << it << " ";
////    }
////    std::cout << std::endl;
////    std::cout << std::endl;
//
//    std::vector<double> jumps(phase_trace.size()-1, UTL::ZERO_DOUBLE);
//
//
//    // std::adjacent_difference c++17
//    // std::adjacent_difference(phase_trace.begin(), phase_trace.end(), jumps.begin());
//    //
//    for(auto i = 0; i < jumps.size(); ++i )
//    {
//        jumps[i] = phase_trace[i+1] - phase_trace[i];
//        //std::cout << jumps[i] << " ";
//    }
////    std::cout << std::endl;
////    std::cout << std::endl;
//
//
////    for(auto&  data_it = phase_trace.begin(), jumps_it = jumps.begin();
////               data_it < phase_trace.end() && jumps_it < jumps.end();
////               ++data_it , ++jumps_it )
////    {
////            *jumps_it = *(data_it + 1) - *data_it;
////            std::cout << *jumps_it << " ";
////    }
////    std::cout << std::endl;
////    std::cout << std::endl;
//
//    for(auto& it:jumps)
//    {
//        //jumps=-Sign[jumps]Unitize[Chop[Abs[jumps],tol]];
//
//
//        //std::cout << it << " ";
//
//    }
////    std::cout << std::endl;
////    std::cout << std::endl;
//
//
//    std::partial_sum(jumps.begin(), jumps.end(), jumps.begin(), std::plus<double>());
//
////    for(auto& it: jumps)
////    {
////        std::cout << it << " ";
////    }
////    std::cout << std::endl;
////    std::cout << std::endl;
//
//
//    for(auto& j: jumps)
//    {
//        j *= 180.0;//MAGIC_NUMBER
//    }
//    jumps.insert( jumps.begin(), 0.0 ); //MAGIC_NUMBER
//
//
//    for(auto&& data_it = phase_trace.begin(), jump_it = jumps.begin();
//               data_it < phase_trace.end() && jump_it < jumps.end();
//               ++data_it , ++jump_it )
//    {
//        *data_it += *jump_it;
////        std::cout << *data_it << " ";
//    }
////    std::cout << std::endl;
////    std::cout << std::endl;
//   // message(data.name, " unwrapped phase ");
}




