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
//  FileName:    liberaLLRFController.cpp
//  Description:
//
//
//
//*/
#include "liberaLLRFController.h"
#include <fstream>
#include <iostream>
// stl
liberaLLRFController::liberaLLRFController(bool& show_messages,
                                           bool& show_debug_messages,
                                           const std::string & config,
                                           const bool startVirtualMachine,
                                           const bool shouldStartEPICs,
                                           const llrfStructs::LLRF_TYPE type,
                                           const HWC_ENUM::CONTROLLER_TYPE c_type,
                                           const std::string& name):
controller(show_messages,show_debug_messages,c_type,name),
localInterface(config,startVirtualMachine,SHOW_MESSAGES,SHOW_DEBUG_MESSAGES,shouldStartEPICs,type),
shouldStartEPICs(shouldStartEPICs)
{
    initialise();
}
//______________________________________________________________________________
liberaLLRFController::~liberaLLRFController(){}    //dtor




//---------------------------------------------------------------------------------
/*  ___  __        __   ___           ___                                   ___  __
     |  |__)  /\  /  ` |__      |\/| |__   /\  |\ |    \  /  /\  |    |  | |__  /__`
     |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \|     \/  /~~\ |___ \__/ |___ .__/

*/
double liberaLLRFController::getMean(const std::string&name)const
{
    return localInterface.getMean(name);
}
size_t liberaLLRFController::getMeanStartIndex(const std::string&name)const
{
    return localInterface.getMeanStartIndex(name);
}
size_t liberaLLRFController::getMeanStopIndex(const std::string&name)const
{
    return localInterface.getMeanStopIndex(name);
}
bool liberaLLRFController::setMeanStartEndTime(const double start, const double end, const std::string&name)
{
    return localInterface.setMeanStartEndTime(start, end,name);
}
bool liberaLLRFController::setMeanStartIndex(const std::string&name, size_t  value)
{
    return localInterface.setMeanStartIndex(name, value);
}
bool liberaLLRFController::setMeanStopIndex(const std::string&name, size_t  value)
{
    return localInterface.setMeanStopIndex(name, value);
}
double liberaLLRFController::getCutMean(const std::string&name)const
{
    return getMean(name);
}
double liberaLLRFController::getKlyFwdPwrCutMean()const
{
    return getCutMean(UTL::KLYSTRON_FORWARD_POWER);
}
double liberaLLRFController::getKlyFwdPhaCutMean()const
{
    return getCutMean(UTL::KLYSTRON_FORWARD_PHASE);
}
double liberaLLRFController::getKlyRevPwrCutMean()const
{
    return getCutMean(UTL::KLYSTRON_REVERSE_POWER);
}
double liberaLLRFController::getKlyRevPhaCutMean()const
{
    return getCutMean(UTL::KLYSTRON_REVERSE_PHASE);
}
double liberaLLRFController::getCavFwdPwrCutMean()const
{
    return getCutMean(UTL::KLYSTRON_FORWARD_POWER);
}
double liberaLLRFController::getCavFwdPhaCutMean()const
{
    return getCutMean(UTL::CAVITY_FORWARD_PHASE);
}
double liberaLLRFController::getCavRevPwrCutMean()const
{
    return getCutMean(UTL::CAVITY_REVERSE_POWER);
}
double liberaLLRFController::getCavRevPhaCutMean()const
{
    return getCutMean(UTL::CAVITY_REVERSE_PHASE);
}
double liberaLLRFController::getProbePwrCutMean()const
{
    return getCutMean(UTL::CAVITY_PROBE_POWER);
}
double liberaLLRFController::getProbePhaCutMean()const
{
    return getCutMean(UTL::CAVITY_PROBE_PHASE);
}
//--------------------------------------------------------------------------------------------------
/* ___  __        __   ___
    |  |__)  /\  /  ` |__      |\/|  /\  \_/
    |  |  \ /~~\ \__, |___     |  | /~~\ / \

    It doesn't much sense to ask for the trace max of a phase trace ...
*/
double liberaLLRFController::getTraceMax(const std::string& name)const
{
    return localInterface.getTraceMax(name);
}
double liberaLLRFController::getKlyFwdPwrMax()const
{
    return getTraceMax(UTL::KLYSTRON_FORWARD_POWER);
}
double liberaLLRFController::getKlyRevPwrMax()const
{
    return getTraceMax(UTL::KLYSTRON_REVERSE_POWER);
}
double liberaLLRFController::getCavFwdPwrMax()const
{
    return getTraceMax(UTL::CAVITY_FORWARD_POWER);
}
double liberaLLRFController::getCavRevPwrMax()const
{
    return getTraceMax(UTL::CAVITY_REVERSE_POWER);
}
double liberaLLRFController::getProbePwrMax()const
{
    return getTraceMax(UTL::CAVITY_PROBE_POWER);
}
//--------------------------------------------------------------------------------------------------






//______________________________________________________________________________
void liberaLLRFController::initialise()
{
    if(localInterface.interfaceInitReport())
        message("liberaLLRFController instantiation success.");
}
//______________________________________________________________________________
bool liberaLLRFController::setTORSCANToIOIntr()
{
    return localInterface.setTORSCANToIOIntr();
}
//______________________________________________________________________________
bool liberaLLRFController::setTORSCANToPassive()
{
    return localInterface.setTORSCANToPassive();
}
//______________________________________________________________________________
bool liberaLLRFController::resetTORSCANToIOIntr()
{
    return localInterface.resetTORSCANToIOIntr();
}
//______________________________________________________________________________
bool liberaLLRFController::setTORACQMEvent()
{
    return localInterface.setTORACQMEvent();
}
//------------------------------------------------------------------------------
int liberaLLRFController::getTORACQM()const
{
    return localInterface.getTORACQM();
}
//------------------------------------------------------------------------------
int liberaLLRFController::getTORSCAN()const
{
    return localInterface.getTORSCAN();
}
//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/

*/
bool liberaLLRFController::setUsePercentMask(const std::string& trace)
{
    return localInterface.setUsePercentMask(trace);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setUseAbsoluteMask(const std::string& trace)
{
    return localInterface.setUseAbsoluteMask(trace);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskValue(const std::string& trace, double value)
{
    return localInterface.setMaskValue(trace,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskStartIndex(const std::string& trace, size_t value)
{
    return localInterface.setMaskStartIndex(trace,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskEndIndex(const std::string& trace, size_t value)
{
    return localInterface.setMaskEndIndex(trace,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskFloor(const std::string& name, double value)
{
    return localInterface.setMaskFloor(name,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskAbsMinValue(const std::string& name, double value)
{
    return localInterface.setMaskAbsMinValue(name,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskWindowIndices(const std::string& name, size_t window_start, size_t window_end)
{
    return localInterface.setMaskWindowIndices(name,window_start, window_end);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskStartTime(const std::string& name, double value)
{
    return localInterface.setMaskStartTime(name,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskEndTime(const std::string& name, double value)
{
    return localInterface.setMaskEndTime(name,value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskWindowTimes(const std::string& name, double window_start, double window_end)
{
    return localInterface.setMaskWindowTimes(name,window_start, window_end);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setCheckMask(const std::string&name, bool value)
{
    return localInterface.setCheckMask(name, value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setShouldCheckMask(const std::string&name)
{
    return localInterface.setShouldCheckMask(name);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setShouldNotCheckMask(const std::string&name)
{
    return localInterface.setShouldNotCheckMask(name);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::clearMask(const std::string&name)
{
    return localInterface.clearMask(name);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setHiMask(const std::string&name,const std::vector<double>& value)
{
    return localInterface.setHiMask(name, value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setLoMask(const std::string&name,const std::vector<double>& value)
{
    return localInterface.setLoMask(name, value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskParamatersIndices(const std::string& name, bool isPercent,
                              double mask_value, double mask_floor, double mask_abs_min,
                              size_t start, size_t end, size_t window_start, size_t window_end)
{
    return localInterface.setMaskParamatersIndices(name, isPercent, mask_value, mask_floor, mask_abs_min,
                                                   start, end, window_start, window_end);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setMaskParamatersTimes(const std::string& name, bool isPercent,
                            double mask_value, double mask_floor, double mask_abs_min,
                            double start, double end,
                            double window_start, double window_end)
{
    return localInterface.setMaskParamatersTimes(name, isPercent, mask_value, mask_floor, mask_abs_min,
                                                 start, end, window_start, window_end);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setNumContinuousOutsideMaskCount(size_t value)
{
    return localInterface.setNumContinuousOutsideMaskCount(value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setNumContinuousOutsideMaskCount(const std::string&name, size_t value)
{
    return localInterface.setNumContinuousOutsideMaskCount(name, value);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setGlobalCheckMask(bool value)
{
    localInterface.setGlobalCheckMask(value);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setGlobalShouldCheckMask()
{
    localInterface.setGlobalShouldCheckMask();
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setGlobalShouldNotCheckMask()
{
    localInterface.setGlobalShouldNotCheckMask();
}
//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    / _` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    \__> |___  |   |  |___ |  \ .__/

    //
*/

bool liberaLLRFController::isGlobalCheckMask()
{
    return localInterface.isGlobalCheckMask();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isPercentMask(const std::string& name)
{
    return localInterface.isPercentMask(name);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isAbsoluteMask(const std::string& name)
{
    return localInterface.isAbsoluteMask(name);
}
//--------------------------------------------------------------------------------------------------
#ifdef BUILD_DLL
bool liberaLLRFController::setHiMask_Py(const std::string&name, boost::python::list& value)
{
    return setHiMask(name, to_std_vector<double>(value));
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setLoMask_Py(const std::string&name, boost::python::list& value)
{
    return setLoMask(name, to_std_vector<double>(value));
}
//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
std::vector<double> liberaLLRFController::getHiMask(const std::string&name)
{
    return localInterface.getHiMask(name);
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberaLLRFController::getLoMask(const std::string&name)
{
    return localInterface.getLoMask(name);
}
//--------------------------------------------------------------------------------------------------
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getHiMask_Py(const std::string&name)
{
    return toPythonList(getHiMask(name));
}
//--------------------------------------------------------------------------------------------------
boost::python::list liberaLLRFController::getLoMask_Py(const std::string&name)
{
    return toPythonList(getLoMask(name));
}
//--------------------------------------------------------------------------------------------------
#endif
double liberaLLRFController::getMaskValue(const std::string& name)
{
    return localInterface.getMaskValue(name);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getMaskStartIndex(const std::string& name)
{
    return localInterface.getMaskStartIndex(name);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getMaskEndIndex(const std::string& name)
{
    return localInterface.getMaskEndIndex(name);
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getMaskFloor(const std::string& name)
{
    return localInterface.getMaskFloor(name);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getMaskWindowStartIndex(const std::string& name)
{
    return localInterface.getMaskWindowStartIndex(name);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getMaskWindowEndIndex(const std::string& name)
{
    return localInterface.getMaskWindowEndIndex(name);
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getMaskStartTime(const std::string& name)
{
    return localInterface.getMaskStartTime(name);
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getMaskEndTime(const std::string& name)
{
    return localInterface.getMaskEndTime(name);
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getMaskWindowStartTime(const std::string& name)
{
    return localInterface.getMaskWindowStartTime(name);
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getMaskWindowEndTime(const std::string& name)
{
    return localInterface.getMaskWindowEndTime(name);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getOutsideMaskEventCount()const
{
    return localInterface.getOutsideMaskEventCount();
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getOutsideMaskEventDataSize()
{
    return localInterface.getOutsideMaskEventDataSize();
}
//--------------------------------------------------------------------------------------------------
llrfStructs::outside_mask_event_data liberaLLRFController::getOutsideMaskEventData()
{
    return localInterface.getOutsideMaskEventData();
}
//--------------------------------------------------------------------------------------------------
const llrfStructs::outside_mask_event_data& liberaLLRFController::getOutsideMaskEventData_CRef()
{
    return localInterface.getOutsideMaskEventData_CRef();
}


#ifdef BUILD_DLL
boost::python::dict liberaLLRFController::getOutsideMaskEventData_Py()
{
    const llrfStructs::outside_mask_event_data& data = localInterface.getOutsideMaskEventData_CRef();
    boost::python::dict dictionary;
    boost::python::dict dictionary2;
    boost::python::list list;

    std::string EVID;
    std::string time;
    std::string value;
    std::string name;

//    dictionary[ std::string("hi_mask") ] = toPythonList(data.hi_mask);
//    dictionary[ std::string("lo_mask") ] = toPythonList(data.lo_mask);
    dictionary[ std::string("trace_name") ] = boost::python::object(data.trace_that_caused_OME);
    dictionary[ std::string("mask_floor") ] = boost::python::object(data.mask_floor);
    dictionary[ std::string("time_vector") ] = getTimeVector_Py();
    dictionary[ std::string("is_collecting") ] = boost::python::object(data.is_collecting);
    dictionary[ std::string("num_collected_traces") ] = boost::python::object(data.num_collected);
    dictionary[ std::string("message") ] = boost::python::object(data.outside_mask_trace_message);
    dictionary[ std::string("num_events") ] = boost::python::object(data.num_events);
    dictionary[ std::string("outside_mask_index") ] = boost::python::object(data.outside_mask_index);

//    std::vector<std::string> traces_to_save_on_break_down = getTracesToSaveOnBreakDown();

//    int i = -2;
    size_t j = 0;
    for(auto&&it: data.trace_data)
    {
        /// !!!!!!!!!!!!! FOR SOME REASON THIS IS THE ONLY WAY I COULD PROPERLY COPY THE DICTIONARY TO EXPORT
        /// getOMT_Dict returns a dictionary of data from a single pulse
        /// this loop combines those dictionaries into another dictariony keyed by index
        /// maybe a simlar issue to 'how to clear a list'

       // message("Getting dictionary for ", it.name );


        int i = -2;
        std::string EVID;
        std::string time;
        std::string value;
        std::string name;

        std::string hi_mask = "hi_mask_" + it.name;
        std::string lo_mask = "lo_mask_" + it.name;

        dictionary[hi_mask] = toPythonList(it.hi_mask);
        //message("ADDED hi_mask for", it.name );

        dictionary[lo_mask] = toPythonList(it.lo_mask);
        //message("ADDED lo_mask for", it.name );


        for(auto&& it2: it.data_buffer)
        {
            //message("ADDED data_buffer ", i + 2, " for ", it.name );
            EVID = "EVID_"  + it.name + "_";
            //time = "time_"  + it2.name + "_";
            value= "value_" + it.name + "_";
            name = "name_"  + it.name + "_";
            time = "time_"  + it.name + "_";
            dictionary[EVID  += std::to_string(i)] = i + 2;
            dictionary[time  += std::to_string(i)] = it2.first;
            dictionary[value += std::to_string(i)] = toPythonList(it2.second);
            dictionary[name  += std::to_string(i)] = it.name;
            ++i;
        }
        //dictionary[std::to_string(j)] = getOMT_Dict(it);
        //++j;
        //dictionary.clear();
    }
    //message("getOutsideMaskData_Py Returning a dictionary ");
    return dictionary;
}
//______________________________________________________________________________
//boost::python::dict liberaLLRFController::getOutsideMaskEventDataPart_Py(const size_t part)
//{
//    return getOMT_Dict(getOutsideMaskEventDataPart(part));
//}
//______________________________________________________________________________
boost::python::dict liberaLLRFController::getOMT_Dict(const llrfStructs::outside_mask_event_trace_data& omt)
{
    //message("getOMT_Dict called ");
    boost::python::dict dictionary;
    int i = -2;
    //EVID;
//    std::string time;
//    std::string value;
//    std::string name;

    std::string hi_mask = "hi_mask_" + omt.name;
    std::string lo_mask = "lo_mask_" + omt.name;

    dictionary[hi_mask] = omt.hi_mask;
    message("ADDED hi_mask for", omt.name );

    dictionary[lo_mask] = omt.lo_mask;
    message("ADDED lo_mask for", omt.name );


    for(auto&& it2: omt.data_buffer)
    {
        message("ADDED data_buffer for ", omt.name );
//        for(auto i: it2.second )
//        {
//            message(i);
//        }

        std::string EVID = "EVID_"  + omt.name + "_";
        //time = "time_"  + it2.name + "_";
        std::string value= "value_" + omt.name + "_";
        std::string name = "name_"  + omt.name + "_";
        std::string time = "time_"  + omt.name + "_";
        dictionary[EVID  += std::to_string(i)] = i + 2;
        dictionary[time  += std::to_string(i)] = it2.first;
        dictionary[value += std::to_string(i)] = toPythonList(it2.second);
        dictionary[name  += std::to_string(i)] = omt.name;
        ++i;
    }
//    }
    return dictionary;
}
#endif


//______________________________________________________________________________
void liberaLLRFController::setTracesToSaveOnOutsideMaskEvent(const std::vector<std::string>& name)
{
    localInterface.setTracesToSaveOnOutsideMaskEvent(name);
}
//______________________________________________________________________________
void liberaLLRFController::setTracesToSaveOnOutsideMaskEvent_2(const std::string& name)
{
    localInterface.setTracesToSaveOnOutsideMaskEvent(name);
}
//______________________________________________________________________________
void liberaLLRFController::setNumExtraTracesOnOutsideMaskEvent(const size_t value)
{
    localInterface.setNumExtraTracesOnOutsideMaskEvent(value);
}
//____________________________________________________________________________________________
size_t liberaLLRFController::getNumExtraTracesOnOutsideMaskEvent() const
{
    return localInterface.getNumExtraTracesOnOutsideMaskEvent();
}
//______________________________________________________________________________
std::vector<std::string> liberaLLRFController::getTracesToSaveOnOutsideMaskEvent()
{
    return localInterface.getTracesToSaveOnOutsideMaskEvent();
}
//______________________________________________________________________________
bool liberaLLRFController::setDropAmpOnOutsideMaskEvent(const std::string& name,bool state, double amp_val)
{
    return localInterface.setDropAmpOnOutsideMaskEvent(name, state,amp_val);
}
//______________________________________________________________________________
bool liberaLLRFController::setDropAmpOnOutsideMaskEventValue(const std::string& name, double amp_val)
{
    return localInterface.setDropAmpOnOutsideMaskEventValue(name, amp_val);
}
//______________________________________________________________________________
bool liberaLLRFController::isOutsideMaskEventDataCollecting()const
{
    return localInterface.isOutsideMaskEventDataCollecting();
}
//______________________________________________________________________________
bool liberaLLRFController::canGetOutsideMaskEventData()const
{
    return localInterface.canGetOutsideMaskEventData();
}

#ifdef BUILD_DLL
//______________________________________________________________________________
void liberaLLRFController::setTracesToSaveOnOutsideMaskEvent_Py(const boost::python::list& name)
{
    setTracesToSaveOnOutsideMaskEvent(to_std_vector<std::string>(name));
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTracesToSaveOnOutsideMaskEvent_Py()
{
    return toPythonList(getTracesToSaveOnOutsideMaskEvent());
}
#endif

//--------------------------------------------------------------------------------------------------
/*        __  ___         ___     __             __   ___  __
     /\  /  `  |  | \  / |__     |__) |  | |    /__` |__  /__`
    /~~\ \__,  |  |  \/  |___    |    \__/ |___ .__/ |___ .__/
*/
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getActivePulseCount()const
{
    return localInterface.getActivePulseCount();
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::addActivePulseCountOffset(const size_t val)
{
    localInterface.addActivePulseCountOffset(val);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setActivePulsePowerLimit(const double val)
{
    localInterface.setActivePulsePowerLimit(val);
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getActivePulsePowerLimit() const
{
    return localInterface.getActivePulsePowerLimit();
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setActivePulseCount(const size_t val)
{
    localInterface.setActivePulseCount(val);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setAndApplyPulseShape(std::vector<double>& values)
{
    message("setAndApplyPulseShape");
    return localInterface.setAndApplyPulseShape(values);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setPulseShape(std::vector<double>& values)
{
    return localInterface.setPulseShape(values);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::applyPulseShape()
{
    return localInterface.applyPulseShape();
}
//--------------------------------------------------------------------------------------------------
#ifdef BUILD_DLL
bool liberaLLRFController::setAndApplyPulseShape_Py(boost::python::list values)
{
    return setAndApplyPulseShape( to_std_vector<double>(values) );
}
bool liberaLLRFController::setPulseShape_Py(boost::python::list values)
{
    return setPulseShape( to_std_vector<double>(values) );
}
#endif

//--------------------------------------------------------------------------------------------------





//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setKeepKlyFwdPwrRS(bool val)
{
    localInterface.setActivePulseCount(val);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::keepKlyFwdPwrRS()
{
    localInterface.keepKlyFwdPwrRS();
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::dontKeepKlyFwdPwrRS()
{
    localInterface.dontKeepKlyFwdPwrRS();
}
//--------------------------------------------------------------------------------------------------
std::tuple<size_t,double,double> liberaLLRFController::getKlyFwdPwrRSState(int ampSP_setting)
{
    return localInterface.getKlyFwdPwrRSState(ampSP_setting);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance)
{
    localInterface.setKlyFwdPwrRSState(amp_sp, n, old_mean, old_variance);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isKeepingKlyFwdPwrRS()
{
    return localInterface.isKeepingKlyFwdPwrRS();
}

//--------------------------------------------------------------------------------------------------
#ifdef BUILD_DLL
//--------------------------------------------------------------------------------------------------
boost::python::list liberaLLRFController::getKlyFwdPwrRSState_Py(int ampSP_setting)
{
    std::tuple<size_t,double,double> r = getKlyFwdPwrRSState(ampSP_setting);
    boost::python::list rlist;
    rlist.append( std::get<UTL::ZERO_SIZET>(r));
    rlist.append( std::get<UTL::ONE_SIZET>(r));
    rlist.append( std::get<UTL::TWO_SIZET>(r));
    return rlist;
}
#endif
//--------------------------------------------------------------------------------------------------





//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setTracesToSaveWhenDumpingCutOneTraceData(const std::vector<std::string>& names)
{
    return localInterface.setTracesToSaveWhenDumpingCutOneTraceData(names);
}
//--------------------------------------------------------------------------------------------------
std::vector<std::string> liberaLLRFController::getTracesToSaveWhenDumpingCutOneTraceData()const
{
    return localInterface.getTracesToSaveWhenDumpingCutOneTraceData();
}
//--------------------------------------------------------------------------------------------------
std::vector< std::pair<std::string, std::vector<double>> > liberaLLRFController::getOneTraceData()const
{
    return localInterface.getOneTraceData();
}
#ifdef BUILD_DLL
//--------------------------------------------------------------------------------------------------

void liberaLLRFController::setTracesToSaveWhenDumpingCutOneTraceData_Py(boost::python::list& names)
{
    setTracesToSaveWhenDumpingCutOneTraceData(to_std_vector<std::string>(names));
}
//--------------------------------------------------------------------------------------------------
boost::python::list liberaLLRFController::getTracesToSaveWhenDumpingCutOneTraceData_Py()const
{
    return toPythonList(getTracesToSaveWhenDumpingCutOneTraceData());
}
//--------------------------------------------------------------------------------------------------
boost::python::dict liberaLLRFController::getCutOneTraceData_Py()
{
    message("getCutOneTraceData_Py 1");

    std::vector<std::string> keys;
    std::vector<std::string> timestamps;
    std::vector<std::vector<double>> data;

    localInterface.getCutOneTraceData(keys,timestamps,data);

    boost::python::dict dictionary;

    std::string timestamp_suffix = "_timestamp";
    std::string data_suffix      = "_data";

    //message("getCutOneTraceData_Py 2");
    //sanity check

    if( keys.size() == timestamps.size() && keys.size() == data.size() )
    {
        for(auto i = UTL::ZERO_SIZET; i < data.size(); ++i)
        {
            std::string timestamp_key = keys[i] + timestamp_suffix;
            std::string data_key      = keys[i] + data_suffix;

            dictionary[ timestamp_key ] = timestamps[i];
            dictionary[ data_key      ] = toPythonList(data[i]);

            //message("Added ", timestamp_key );
            //message("Added ", data_key      );
        }
    }
    return dictionary;

}
//--------------------------------------------------------------------------------------------------
boost::python::dict liberaLLRFController::getOneTraceData_Py()const
{
    boost::python::dict dictionary;

    std::vector< std::pair<std::string, std::vector<double>> > data = getOneTraceData();

    bool first = true;
    size_t counter = 0;
    for(auto&&it:data)
    {
        std::string time = std::string("time_");
        std::string data = std::string("data_");
        dictionary[ time  += std::to_string(counter) ] = it.first;
        dictionary[ data  += std::to_string(counter) ] = toPythonList(it.second);
        ++counter;
    }
    return dictionary;
}
#endif
//--------------------------------------------------------------------------------------------------
std::pair<std::string, std::vector<double>> liberaLLRFController::getTraceData(const std::string& name)const
{
    return localInterface.getTraceData(name);
}
//--------------------------------------------------------------------------------------------------
#ifdef BUILD_DLL
boost::python::dict liberaLLRFController::getTraceData_Py(const std::string& name)
{
    std::pair<std::string, std::vector<double>> ret = localInterface.getTraceData(name);
    boost::python::dict dictionary;
    dictionary[ ret.first] = toPythonList(ret.second);
    return dictionary;
}
//--------------------------------------------------------------------------------------------------

#endif

//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isAmpFFLocked()const
{
    return localInterface.isAmpFFLocked();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isAmpFFNotLocked()const
{
    return localInterface.isAmpFFNotLocked();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isPhaseFFLocked()const
{
    return localInterface.isPhaseFFLocked();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isPhaseFFNotLocked()const
{
    return localInterface.isPhaseFFNotLocked();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isRFOutput() const
{
    return localInterface.isRFOutput();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isNotRFOutput() const
{
    return localInterface.isNotRFOutput();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::enableRFOutput()
{
    return localInterface.enableRFOutput();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::disableRFOutput()
{
    return localInterface.disableRFOutput();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::enableRFandLock() // also does the amp and phase lock check boxes ...
{
    return localInterface.enableRFandLock();
}

bool liberaLLRFController::isTrigExternal() // also does the amp and phase lock check boxes ...
{
    return localInterface.isTrigExternal();
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isFFLocked()const
{
    return localInterface.isFFLocked();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isFFNotLocked()const
{
    return localInterface.isFFNotLocked();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::RFOutput()const
{
    return localInterface.RFOutput();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isInterlockActive()const
{
    return localInterface.isInterlockActive();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isInterlockNotActive()const
{
    return localInterface.isInterlockNotActive();
}
bool liberaLLRFController::setInterlockNonActive()
{
    return localInterface.setInterlockNonActive();
}

bool liberaLLRFController::lockAmpFF()
{
    return localInterface.lockAmpFF();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::lockPhaseFF()
{
    return localInterface.lockPhaseFF();
}
//--------------------------------------------------------------------------------------------------


////--------------------------------------------------------------------------------------------------
//bool liberaLLRFController::externalTriggerOn()
//{
//    return localInterface.externalTriggerOn();
//}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setUnwrapPhaseTolerance(const std::string& name,const double value)
{
    return localInterface.setUnwrapPhaseTolerance(name, value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setUnwrapPhaseTolerance(const double value)
{
    return localInterface.setUnwrapPhaseTolerance(value);
}
//--------------------------------------------------------------------------------------------------



/* These functions calculte the rep rate of data as received by this interface. */
void liberaLLRFController::setNumTracesToEstimateRepRate(size_t value)
{
     localInterface.setNumTracesToEstimateRepRate(value);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getNumTracesToEstimateRepRate() const
{
    return localInterface.getNumTracesToEstimateRepRate();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getTraceRepRate() const
{
    return localInterface.getTraceRepRate();
}
//--------------------------------------------------------------------------------------------------








bool liberaLLRFController::canKeepAlive()const
{
    return localInterface.canKeepAlive();
}

void liberaLLRFController::setKeepAlive(bool val)
{
    localInterface.setKeepAlive(val);
}
void liberaLLRFController::keepAlive()
{
    localInterface.keepAlive();
}










//______________________________________________________________________________
llrfStructs::LLRF_TYPE liberaLLRFController::getType()const
{
    return localInterface.getType();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpFF()const
{
    return localInterface.getAmpFF();
}
//______________________________________________________________________________
size_t liberaLLRFController::getPulseCount()const
{
    return localInterface.getPulseCount();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpSP()const
{
    return localInterface.getAmpSP();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiFF()const
{
    return localInterface.getPhiFF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiSP()const
{
    return localInterface.getPhiSP();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpLLRF()const
{
    return localInterface.getAmpLLRF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiLLRF()const
{
    return localInterface.getPhiLLRF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiDEG()const
{
    return localInterface.getPhiDEG();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpMVM()const
{
    return localInterface.getAmpMVM();
}
//______________________________________________________________________________
double liberaLLRFController::getPulseLength()const
{
    return localInterface.getPulseLength();
}
//______________________________________________________________________________
double liberaLLRFController::getPulseOffset()const
{
    return localInterface.getPulseOffset();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiCalibration()const
{
    return localInterface.getPhiCalibration();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpCalibration()const
{
    return localInterface.getAmpCalibration();
}
//______________________________________________________________________________
double liberaLLRFController::getCrestPhiLLRF()const
{
    return localInterface.getCrestPhiLLRF();
}
//____________________________________________________________________________________________
double liberaLLRFController::getMaxAmpSP()const
{
    return localInterface.getMaxAmpSP();
}
//______________________________________________________________________________
size_t liberaLLRFController::getTraceLength()const
{
    return localInterface.getTraceLength();
}
//______________________________________________________________________________
//std::vector<std::string> liberaLLRFController::getChannelNames()
//{
//    return localInterface.getChannelNames();
//}
//______________________________________________________________________________
bool liberaLLRFController::setTraceSCAN(const std::string& name, const llrfStructs::LLRF_PV_TYPE pv, const llrfStructs::LLRF_SCAN value)
{
    return localInterface.setTraceSCAN(name, pv,  value);
}
//______________________________________________________________________________
bool  liberaLLRFController::setAllTraceSCAN(const llrfStructs::LLRF_SCAN value)
{
    return localInterface.setAllTraceSCAN(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAllSCANToPassive()
{
    return localInterface.setAllSCANToPassive();
}
//______________________________________________________________________________
std::vector<std::string> liberaLLRFController::getTraceNames()
{
    return localInterface.getTraceNames();
}
//----------------------------------------------------------------------------------
bool liberaLLRFController::setPowerRemoteTraceSCAN10sec(const std::string& name)
{
    return localInterface.setPowerRemoteTraceSCAN10sec(name);

}

















//______________________________________________________________________________

//llrfStructs::rf_trace liberaLLRFController::getTraceData(const std::string& name)
//{
//    return localInterface.getTraceData(name);
//}


//______________________________________________________________________________

////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getTraceBuffer(const std::string& name)
//{
//    return localInterface.getTraceBuffer(name);
//}
//______________________________________________________________________________



std::vector<double> liberaLLRFController::getTimeVector()const
{
    return localInterface.getTimeVector();
}

std::vector<double> liberaLLRFController::getPulseShape()const
{
    return localInterface.getPulseShape();
}
bool liberaLLRFController::getPulseShapeUpToDate()const
{
    return localInterface.getPulseShapeUpToDate();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getTimeVector_Py()const
{
    return toPythonList(getTimeVector());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getPulseShape_Py()const
{
    return toPythonList(getPulseShape());
}
//______________________________________________________________________________
//______________________________________________________________________________
//boost::python::list liberaLLRFController::getChannelNames_Py()
//{
//    return toPythonList(getChannelNames());
//}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTraceNames_Py()
{
    return toPythonList(getTraceNames());
}
#endif


//--------------------------------------------------------------------------------------------------
/* ___  __        __   ___                         ___  __
    |  |__)  /\  /  ` |__     \  /  /\  |    |  | |__  /__`
    |  |  \ /~~\ \__, |___     \/  /~~\ |___ \__/ |___ .__/

     Returns the current rolling_average of the requested trace
*/
std::vector<double> liberaLLRFController::getTraceValues(const std::string& name)const
{
    return localInterface.getTraceValues(name);
}
std::vector<double> liberaLLRFController::getCavRevPwr()const
{
    return getTraceValues(UTL::CAVITY_REVERSE_POWER);
}
std::vector<double> liberaLLRFController::getCavFwdPwr()const
{
    return getTraceValues(UTL::CAVITY_FORWARD_POWER);
}
std::vector<double> liberaLLRFController::getKlyRevPwr()const
{
    return getTraceValues(UTL::KLYSTRON_REVERSE_POWER);
}
std::vector<double> liberaLLRFController::getKlyFwdPwr()const
{
    return getTraceValues(UTL::KLYSTRON_FORWARD_POWER);
}
std::vector<double> liberaLLRFController::getCavRevPha()const
{
    return getTraceValues(UTL::CAVITY_REVERSE_PHASE);
}
std::vector<double> liberaLLRFController::getCavFwdPha()const
{
    return getTraceValues(UTL::CAVITY_FORWARD_PHASE);
}
std::vector<double> liberaLLRFController::getKlyRevPha()const
{
    return getTraceValues(UTL::KLYSTRON_REVERSE_PHASE);
}
std::vector<double> liberaLLRFController::getKlyFwdPha()const
{
    return getTraceValues(UTL::KLYSTRON_FORWARD_PHASE);
}
std::vector<double> liberaLLRFController::getProbePwr()const
{
    return getTraceValues(UTL::CAVITY_PROBE_POWER);
}
std::vector<double> liberaLLRFController::getProbePha()const
{
    return getTraceValues(UTL::CAVITY_PROBE_PHASE);
}
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getTraceValues_Py(const std::string& name)const
{
    return toPythonList(getTraceValues(name));
}
boost::python::list liberaLLRFController::getCavRevPwr_Py()const
{
    return toPythonList(getCavRevPwr());
}
boost::python::list liberaLLRFController::getCavFwdPwr_Py()const
{
    return toPythonList(getCavFwdPwr());
}
boost::python::list liberaLLRFController::getKlyRevPwr_Py()const
{
    return toPythonList(getKlyRevPwr());
}
boost::python::list liberaLLRFController::getKlyFwdPwr_Py()const
{
    return toPythonList(getKlyFwdPwr());
}
boost::python::list liberaLLRFController::getCavRevPha_Py()const
{
    return toPythonList(getCavRevPha());
}
boost::python::list liberaLLRFController::getCavFwdPha_Py()const
{
    return toPythonList(getCavFwdPha());
}
boost::python::list liberaLLRFController::getKlyRevPha_Py()const
{
    return toPythonList(getKlyRevPha());
}
boost::python::list liberaLLRFController::getKlyFwdPha_Py()const
{
    return toPythonList(getKlyFwdPha());
}
boost::python::list liberaLLRFController::getProbePha_Py()const
{
    return toPythonList(getProbePha());
}
boost::python::list liberaLLRFController::getProbePwr_Py()const
{
    return toPythonList(getProbePwr());
}
#endif
//--------------------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------------------
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getTraceDataBuffer(const std::string& name)const
{
    return localInterface.getTraceDataBuffer(name);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getCavRevPwrBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_REVERSE_POWER);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getCavFwdPwrBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_FORWARD_POWER);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getKlyRevPwrBuffer()const
{
    return getTraceDataBuffer(UTL::KLYSTRON_REVERSE_POWER);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getKlyFwdPwrBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_FORWARD_POWER);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getCavRevPhaBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_REVERSE_PHASE);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getCavFwdPhaBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_FORWARD_PHASE);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getKlyRevPhaBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_REVERSE_PHASE);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getKlyFwdPhaBuffer()const
{
    return getTraceDataBuffer(UTL::CAVITY_REVERSE_POWER);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getProbePwrBuffer( )const
{
    return getTraceDataBuffer(UTL::CAVITY_PROBE_POWER);
}
std::vector<std::pair<std::string, std::vector<double>>> liberaLLRFController::getProbePhaBuffer( )const
{
    return getTraceDataBuffer(UTL::CAVITY_PROBE_PHASE);
}
#ifdef BUILD_DLL
boost::python::dict liberaLLRFController::getTraceDataBuffer_Py(const std::string& name)const
{
    std::vector<std::pair<std::string, std::vector<double>>>  ret = localInterface.getTraceDataBuffer(name);
    boost::python::dict dictionary;
    for(auto&&it:ret)
    {
        dictionary[ it.first] = toPythonList(it.second);
    }
    return dictionary;
}
boost::python::dict liberaLLRFController::getCavRevPwrBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::CAVITY_REVERSE_POWER);
}
boost::python::dict liberaLLRFController::getCavFwdPwrBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::CAVITY_FORWARD_POWER);
}
boost::python::dict liberaLLRFController::getKlyRevPwrBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::KLYSTRON_REVERSE_POWER);
}
boost::python::dict liberaLLRFController::getKlyFwdPwrBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::KLYSTRON_FORWARD_POWER);
}
boost::python::dict liberaLLRFController::getCavRevPhaBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::CAVITY_REVERSE_PHASE);
}
boost::python::dict liberaLLRFController::getCavFwdPhaBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::CAVITY_FORWARD_PHASE);
}
boost::python::dict liberaLLRFController::getKlyRevPhaBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::KLYSTRON_REVERSE_PHASE);
}
boost::python::dict liberaLLRFController::getKlyFwdPhaBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::KLYSTRON_FORWARD_PHASE);
}
boost::python::dict liberaLLRFController::getProbePwrBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::CAVITY_PROBE_POWER);
}
boost::python::dict liberaLLRFController::getProbePhaBuffer_Py()const
{
    return getTraceDataBuffer_Py(UTL::CAVITY_PROBE_PHASE);
}
#endif
















//______________________________________________________________________________
std::string liberaLLRFController::fullLLRFTraceName(const std::string& name)const
{
    return localInterface.fullLLRFTraceName(name);
}
//______________________________________________________________________________
std::string liberaLLRFController::shortLLRFTraceName(const std::string& name)const
{
    return localInterface.shortLLRFTraceName(name);
}





//--------------------------------------------------------------------------------------------------






//______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavRevPowerAv_Py()
//{
//    return toPythonList(getCavRevPowerAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavFwdPowerAv_Py()
//{
//    return toPythonList(getCavFwdPowerAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyRevPowerAv_Py()
//{
//    return toPythonList(getKlyRevPowerAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyFwdPowerAv_Py()
//{
//    return toPythonList(getKlyFwdPowerAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavRevPhaseAv_Py()
//{
//    return toPythonList(getCavRevPhaseAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavFwdPhaseAv_Py()
//{
//    return toPythonList(getCavFwdPhaseAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyRevPhaseAv_Py()
//{
//    return toPythonList(getKlyRevPhaseAv());
//}
////______________________________________________________________________________
//boost::python::list  liberaLLRFController::getKlyFwdPhaseAv_Py()
//{
//    return toPythonList(getKlyFwdPhaseAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavRevPower_Py()
//{
//    return toPythonList(getCavRevPower());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyFwdPower_Py()
//{
//    return toPythonList(getKlyFwdPower());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavFwdPower_Py()
//{
//    return toPythonList(getCavFwdPower());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyRevPower_Py()
//{
//    return toPythonList(getKlyRevPower());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavRevPhase_Py()
//{
//    return toPythonList(getCavRevPhase());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyFwdPhase_Py()
//{
//    return toPythonList(getKlyFwdPhase());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavFwdPhase_Py()
//{
//    return toPythonList(getCavFwdPhase());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyRevPhase_Py()
//{
//    return toPythonList(getKlyRevPhase());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getProbePower_Py()
//{
//    return toPythonList(getProbePower());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getProbePhase_Py()
//{
//    return toPythonList(getProbePhase());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavRevPowerBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavFwdPowerBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyRevPowerBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyFwdPowerBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavRevPhaseBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getCavFwdPhaseBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyRevPhaseBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getKlyFwdPhaseBuffer_Py()
//{
//    return toPythonList(getCavRevPowerBuffer());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getProbePowerAv_Py()
//{
//    return toPythonList(getProbePowerAv());
//}
////______________________________________________________________________________
//boost::python::list liberaLLRFController::getProbePhaseAv_Py()
//{
//    return toPythonList(getProbePhaseAv());
//}

#ifdef BUILD_DLL
//______________________________________________________________________________
bool liberaLLRFController::setHighMask(const std::string& name,const boost::python::list& value)
{
    //std::vector<double> t = to_std_vector<double>(value);
    return setHighMask(name,to_std_vector<double>(value));
    //return setHighMask(name,to_std_vector<double>(value));
}
//______________________________________________________________________________
bool liberaLLRFController::setLowMask(const std::string& name,const boost::python::list& value)
{
    return setLowMask(name,to_std_vector<double>(value));
}
//______________________________________________________________________________
bool liberaLLRFController::setCavRevPwrLoMask_Py(const boost::python::list& value)
{
    return setCavRevPwrLoMask(to_std_vector<double>(value));
}
//______________________________________________________________________________
bool liberaLLRFController::setCavRevPwrHiMask_Py(const boost::python::list& value)
{
    return setCavRevPwrHiMask(to_std_vector<double>(value));
}
#endif
//______________________________________________________________________________
bool liberaLLRFController::setCavRevPwrHiMask(const std::vector<double>& value)
{
    return localInterface.setCavRevPwrHiMask(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setCavRevPwrLoMask(const std::vector<double>& value)
{
    return localInterface.setCavRevPwrLoMask(value);
}
//______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getCavRevPowerData()
//{
//    return localInterface.getCavRevPowerData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getCavFwdPowerData()
//{
//    return localInterface.getCavFwdPowerData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getKlyRevPowerData()
//{
//    return localInterface.getKlyRevPowerData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getKlyFwdPowerData()
//{
//    return localInterface.getKlyFwdPowerData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getCavRevPhaseData()
//{
//    return localInterface.getCavRevPhaseData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getCavFwdPhaseData()
//{
//    return localInterface.getCavFwdPhaseData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getKlyRevPhaseData()
//{
//    return localInterface.getKlyRevPhaseData();
//}
////______________________________________________________________________________
//llrfStructs::rf_trace liberaLLRFController::getKlyFwdPhaseData()
//{
//    return localInterface.getKlyFwdPhaseData();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavRevPowerBuffer()
//{
//    return localInterface.getCavRevPowerBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavFwdPowerBuffer()
//{
//    return localInterface.getCavFwdPowerBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyRevPowerBuffer()
//{
//    return localInterface.getKlyRevPowerBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyFwdPowerBuffer()
//{
//    return localInterface.getKlyFwdPowerBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavRevPhaseBuffer()
//{
//    return localInterface.getCavRevPhaseBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavFwdPhaseBuffer()
//{
//    return localInterface.getCavFwdPhaseBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyRevPhaseBuffer()
//{
//    return localInterface.getKlyRevPhaseBuffer();
//}
////______________________________________________________________________________
//std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyFwdPhaseBuffer()
//{
//    return localInterface.getKlyFwdPhaseBuffer();
//}
//______________________________________________________________________________
const llrfStructs::liberallrfObject& liberaLLRFController::getLLRFObjConstRef()
{
    return localInterface.getLLRFObjConstRef();
}
//______________________________________________________________________________
const llrfStructs::rf_trace_data& liberaLLRFController::getTraceDataConstRef(const std::string& name)
{
    return localInterface.getTraceDataConstRef(name);
}
//______________________________________________________________________________
llrfStructs::TRIG liberaLLRFController::getTrigSource()
{
    return localInterface.getTrigSource();
}
//______________________________________________________________________________
//  __   ___ ___ ___  ___  __   __
// /__` |__   |   |  |__  |__) /__`
// .__/ |___  |   |  |___ |  \ .__/
//______________________________________________________________________________

//______________________________________________________________________________
bool liberaLLRFController::trigOff()
{
    return localInterface.trigOff();
}
//______________________________________________________________________________
bool liberaLLRFController::trigExt()
{
    return localInterface.trigExt();
}
//______________________________________________________________________________
bool liberaLLRFController::trigInt()
{
    return localInterface.trigInt();
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiSP(double value)
{
    return localInterface.setPhiSP(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiFF(double value)
{
    return localInterface.setPhiFF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpSP(double value)
{
    return localInterface.setAmpSP(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpHP(double value)
{
    return localInterface.setAmpHP(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpFF(double value)
{
    return localInterface.setAmpFF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpLLRF(double value)
{
    return localInterface.setAmpLLRF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiLLRF(double value)
{
    return localInterface.setPhiLLRF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setAmpMVM(double value)
{
    return localInterface.setAmpMVM(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPhiDEG(double value)
{
    return localInterface.setPhiDEG(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPulseLength(double value)
{
    return localInterface.setPulseLength(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setPulseOffset(double value)
{
    return localInterface.setPulseOffset(value);
}
//______________________________________________________________________________
void liberaLLRFController::setPhiCalibration(double value)
{
    localInterface.setPhiCalibration(value);
}
//______________________________________________________________________________
void liberaLLRFController::setAmpCalibration(double value)
{
    localInterface.setAmpCalibration(value);
}
//______________________________________________________________________________
void liberaLLRFController::setCrestPhiLLRF(double value)
{
    localInterface.setCrestPhiLLRF(value);
}
//______________________________________________________________________________
bool liberaLLRFController::setHighMask(const std::string&name,const  std::vector<double>& value)
{
    return localInterface.setHiMask(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setLowMask(const std::string&name,const std::vector<double>& value)
{
    return localInterface.setLoMask(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setCavRevPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return localInterface.setCavRevPwrMaskPercent(s1,s2,s3,s4,value);
}
//______________________________________________________________________________
bool liberaLLRFController::setCavRevPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return localInterface.setCavRevPwrMaskAbsolute(s1,s2,s3,s4,value);
}
//______________________________________________________________________________
bool liberaLLRFController::setCavFwdPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return localInterface.setCavFwdPwrMaskPercent(s1,s2,s3,s4,value);
}
//______________________________________________________________________________
bool liberaLLRFController::setCavFwdPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value)
{
    return localInterface.setCavFwdPwrMaskAbsolute(s1,s2,s3,s4,value);
}
//______________________________________________________________________________
bool liberaLLRFController::setInfiniteMasks(const std::string& name)
{
    return localInterface.setInfiniteMasks(name);
}
//______________________________________________________________________________
size_t liberaLLRFController::getIndex(const double time)
{
    return localInterface.getIndex(time);
}
//______________________________________________________________________________
double liberaLLRFController::getTime(const size_t  time)
{
    return localInterface.getTime(time);
}
//______________________________________________________________________________
bool liberaLLRFController::setInfiniteMaskEndByPower(const std::string& power_trace,const std::string& phase_trace,const double level)
{
    return localInterface.setInfiniteMaskEndByPower(power_trace,phase_trace,level);
}
//______________________________________________________________________________
bool liberaLLRFController::disableInfiniteMaskEndByPower(const std::string& phase_trace)
{
    return localInterface.disableInfiniteMaskEndByPower(phase_trace);
}
//______________________________________________________________________________
size_t liberaLLRFController::getMaskInfiniteEndByPowerIndex(const std::string& name)
{
    return localInterface.getMaskInfiniteEndByPowerIndex(name);
}
//______________________________________________________________________________
double liberaLLRFController::getMaskInfiniteEndByPowerTime(const std::string& name)
{
    return localInterface.getMaskInfiniteEndByPowerTime(name);
}
//______________________________________________________________________________
bool liberaLLRFController::setAbsoluteTimeMask(const double s1,const double s2,const double s3,const double s4,const double value2,const  std::string& name)
{
    return localInterface.setAbsoluteTimeMask(s1,s2,s3,s4,value2,name);
}
//______________________________________________________________________________
bool liberaLLRFController::setPercentTimeMask(const double s1,const double s2,const double s3,const double s4,const double value2,const  std::string& name)
{
    return localInterface.setPercentTimeMask(s1,s2,s3,s4,value2,name);
}
//______________________________________________________________________________
bool liberaLLRFController::setPercentMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name)
{
    return localInterface.setPercentMask(s1,s2,s3,s4,value2,name);
}
//______________________________________________________________________________
bool liberaLLRFController::setAbsoluteMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name)
{
    return localInterface.setAbsoluteMask(s1,s2,s3,s4,value2,name);
}
////______________________________________________________________________________
//bool liberaLLRFController::shouldCheckMasks(const std::string& name)
//{
//    return localInterface.shouldCheckMasks(name);
//}


//--------------------------------------------------------------------------------------------------
/*
        ___  __        __   ___     __        ___  ___  ___  __      __    __  ___
         |  |__)  /\  /  ` |__     |__) |  | |__  |__  |__  |__)    /__` |  / |__
         |  |  \ /~~\ \__, |___    |__) \__/ |    |    |___ |  \    .__/ | /_ |___
*/
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setAllTraceBufferSize(const size_t value)
{
    return localInterface.setAllTraceBufferSize(value);
}
bool liberaLLRFController::setIndividualTraceBufferSize(const std::string&name,const size_t value)
{
    return localInterface.setIndividualTraceBufferSize(name, value);
}
//______________________________________________________________________________
bool liberaLLRFController::setIndividualTraceBufferSize(const size_t value)
{
    return localInterface.setIndividualTraceBufferSize(value);
}


//______________________________________________________________________________
double liberaLLRFController::getBreakDownRate()
{
    return localInterface.getBreakDownRate();
}
//______________________________________________________________________________
void liberaLLRFController::startTimer()
{
    localInterface.startTimer();
}
//______________________________________________________________________________
void liberaLLRFController::offsetTimer(long long value)
{
    localInterface.offsetTimer(value);
}
//______________________________________________________________________________
long long liberaLLRFController::elapsedTime()
{
    return localInterface.elapsedTime();
}



/*
         __   __                    __                ___  __        __   ___  __
        |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
        |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

// Set the parameters for the rolling avergaes
*/
void liberaLLRFController::clearAllRollingAverage()
{
    localInterface.clearAllRollingAverage();
}
bool liberaLLRFController::clearTraceRollingAverage(const std::string& name)
{
    return localInterface.clearTraceRollingAverage(name);
}
void liberaLLRFController::setKeepRollingAverageNoReset(const bool value)
{
    localInterface.setKeepRollingAverageNoReset(value);
}
bool liberaLLRFController::setKeepRollingAverageNoReset(const std::string&name, const bool value)
{
    return localInterface.setKeepRollingAverageNoReset(name, value);
}
void liberaLLRFController::setShouldKeepRollingAverage()
{
    localInterface.setShouldKeepRollingAverage();
}
bool liberaLLRFController::setShouldKeepRollingAverage(const std::string&name)
{
    return localInterface.setShouldKeepRollingAverage(name);
}
void liberaLLRFController::setShouldNotKeepRollingAverage()
{
    localInterface.setShouldNotKeepRollingAverage();
}
bool liberaLLRFController::setKeepRollingAverage(const std::string&name, bool value)
{
    return localInterface.setKeepRollingAverage(name, value);
}
bool liberaLLRFController::setTraceRollingAverageSize(const std::string&name, const size_t value)
{
    return localInterface.setTraceRollingAverageSize(name, value);
}
void liberaLLRFController::setAllRollingAverageSize(const size_t value)
{
    localInterface.setAllRollingAverageSize(value);
}
bool liberaLLRFController::setShouldNotKeepRollingAverage(const std::string& name)
{
    return localInterface.setShouldNotKeepRollingAverage(name);
}
size_t liberaLLRFController::getTraceRollingAverageSize(const std::string&name)const
{
    return localInterface.getTraceRollingAverageSize(name);
}
size_t liberaLLRFController::getTraceRollingAverageCount(const std::string&name) const
{
    return localInterface.getTraceRollingAverageCount(name);
}
bool liberaLLRFController::isKeepingRollingAverage(const std::string&name)const
{
    return localInterface.isKeepingRollingAverage(name);
}
bool liberaLLRFController::hasRollingAverage(const std::string&name)const
{
    return localInterface.hasRollingAverage(name);
}
std::vector<double> liberaLLRFController::getRollingAverage(const std::string&name)const
{
    return localInterface.getRollingAverage(name);
}
std::vector<double> liberaLLRFController::getCavRevPwrRollingAverage()const
{
    return getRollingAverage(UTL::CAVITY_REVERSE_POWER);
}
std::vector<double> liberaLLRFController::getCavFwdPwrRollingAverage()const
{
    return getRollingAverage(UTL::CAVITY_FORWARD_POWER);
}
std::vector<double> liberaLLRFController::getKlyRevPwrRollingAverage()const
{
    return getRollingAverage(UTL::KLYSTRON_REVERSE_POWER);
}
std::vector<double> liberaLLRFController::getKlyFwdPwrRollingAverage()const
{
    return getRollingAverage(UTL::KLYSTRON_FORWARD_POWER);
}
std::vector<double> liberaLLRFController::getCavRevPhaRollingAverage()const
{
    return getRollingAverage(UTL::CAVITY_REVERSE_PHASE);
}
std::vector<double> liberaLLRFController::getCavFwdPhaRollingAverage()const
{
    return getRollingAverage(UTL::CAVITY_FORWARD_PHASE);
}
std::vector<double> liberaLLRFController::getKlyRevPhaRollingAverage()const
{
    return getRollingAverage(UTL::KLYSTRON_REVERSE_PHASE);
}
std::vector<double> liberaLLRFController::getKlyFwdPhaRollingAverage()const
{
    return getRollingAverage(UTL::KLYSTRON_FORWARD_PHASE);
}
std::vector<double> liberaLLRFController::getProbePwrRollingAverage()const
{
    return getRollingAverage(UTL::CAVITY_PROBE_POWER);
}
std::vector<double> liberaLLRFController::getProbePhaRollingAverage()const
{
    return getRollingAverage(UTL::CAVITY_PROBE_PHASE);
}
std::vector<std::vector<double>> liberaLLRFController::getRollingAverageData(const std::string&name)const
{
    return localInterface.getRollingAverageData(name);
}
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getRollingAverage_Py(const std::string& name)const
{
    return toPythonList(getRollingAverage(name));
}
boost::python::list liberaLLRFController::getCavRevPwrRollingAverage_Py()const
{
    return toPythonList(getCavRevPwrRollingAverage());
}
boost::python::list liberaLLRFController::getCavFwdPwrRollingAverage_Py()const
{
    return toPythonList(getCavFwdPwrRollingAverage());
}
boost::python::list liberaLLRFController::getKlyRevPwrRollingAverage_Py()const
{
    return toPythonList(getKlyRevPwrRollingAverage());
}
boost::python::list liberaLLRFController::getKlyFwdPwrRollingAverage_Py()const
{
    return toPythonList(getKlyFwdPwrRollingAverage());
}
boost::python::list liberaLLRFController::getCavRevPhaRollingAverage_Py()const
{
    return toPythonList(getCavRevPhaRollingAverage());
}
boost::python::list liberaLLRFController::getCavFwdPhaRollingAverage_Py()const
{
    return toPythonList(getCavFwdPhaRollingAverage());
}
boost::python::list liberaLLRFController::getKlyRevPhaRollingAverage_Py()const
{
    return toPythonList(getKlyRevPhaRollingAverage());
}
boost::python::list liberaLLRFController::getKlyFwdPhaRollingAverage_Py()const
{
    return toPythonList(getKlyFwdPhaRollingAverage());
}
boost::python::list liberaLLRFController::getProbePwrRollingAverage_Py()const
{
    return toPythonList(getProbePwrRollingAverage());
}
boost::python::list liberaLLRFController::getProbePhaRollingAverage_Py()const
{
    return toPythonList(getProbePhaRollingAverage());
}
boost::python::list liberaLLRFController::getRollingAverageData_Py(const std::string&name)const
{
    /* a vector of vect or to python list shouldbe a generic templat fucntion somewhere */
    std::vector<std::vector<double>> r = getRollingAverageData(name);
    boost::python::list return_list;
    for(auto&&it:r)
    {
        boost::python::list list_1;
        list_1.append( toPythonList(it)) ;
        return_list.append( list_1 );
    }
    return return_list;
}
//--------------------------------------------------------------------------------------------------
#endif





////______________________________________________________________________________
//bool liberaLLRFController::isTracePV(const llrfStructs::LLRF_PV_TYPE pv)
//{
//    return localInterface.Is_TracePV(pv);
//}
////______________________________________________________________________________
//bool liberaLLRFController::isNotTracePV(const llrfStructs::LLRF_PV_TYPE pv)
//{
//    return localInterface.IsNot_TracePV(pv);
//}
//______________________________________________________________________________
//bool liberaLLRFController::isMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
//{
//    return localInterface.isMonitoring(pv);
//}
////______________________________________________________________________________
//bool liberaLLRFController::isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
//{
//    return localInterface.isNotMonitoring(pv);
//}
////______________________________________________________________________________
//bool liberaLLRFController::isMonitoring(const std::string& name)
//{
//    return localInterface.isMonitoring(name);
//}
////______________________________________________________________________________
//bool liberaLLRFController::isNotMonitoring(const std::string& name)
//{
//    return localInterface.isNotMonitoring(name);
//}
//______________________________________________________________________________
bool liberaLLRFController::isCheckingMask(const std::string& name)
{
    return localInterface.isCheckingMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotCheckingMask(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotCheckingMask(const std::string& name)
{
    return localInterface.isCheckingMask(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotCheckingMask(pv);
}












//---------------------------------------------------------------------------------------------------------
/*
.____    .____   _____________________ .__        __               .__                 __
|    |   |    |  \______   \_   _____/ |__| _____/  |_  ___________|  |   ____   ____ |  | __  ______
|    |   |    |   |       _/|    __)   |  |/    \   __\/ __ \_  __ \  |  /  _ \_/ ___\|  |/ / /  ___/
|    |___|    |___|    |   \|     \    |  |   |  \  | \  ___/|  | \/  |_(  <_> )  \___|    <  \___ \
|_______ \_______ \____|_  /\___  /    |__|___|  /__|  \___  >__|  |____/\____/ \___  >__|_ \/____  >
        \/       \/      \/     \/             \/          \/                       \/     \/     \/
*/
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::areLLRFTraceInterlocksGood() const
{
    return localInterface.areLLRFTraceInterlocksGood();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::areLLRFTraceInterlocksBad() const
{
    return localInterface.areLLRFTraceInterlocksBad();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh1Interlock() const
{
    return localInterface.getCh1Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh1InterlockEnable() const
{
    return localInterface.getCh1InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh1InterlockU() const
{
    return localInterface.getCh1InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh1InterlockP() const
{
    return localInterface.getCh1InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh1InterlockPdbm() const
{
    return localInterface.getCh1InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh2Interlock() const
{
    return localInterface.getCh2Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh2InterlockEnable() const
{
    return localInterface.getCh2InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh2InterlockU() const
{
    return localInterface.getCh2InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh2InterlockP() const
{
    return localInterface.getCh2InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh2InterlockPdbm() const
{
    return localInterface.getCh2InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh3Interlock() const
{
    return localInterface.getCh3Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh3InterlockEnable() const
{
    return localInterface.getCh3InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh3InterlockU() const
{
    return localInterface.getCh3InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh3InterlockP() const
{
    return localInterface.getCh3InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh3InterlockPdbm() const
{
    return localInterface.getCh3InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh4Interlock() const
{
    return localInterface.getCh4Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh4InterlockEnable() const
{
    return localInterface.getCh4InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh4InterlockU() const
{
    return localInterface.getCh4InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh4InterlockP() const
{
    return localInterface.getCh4InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh4InterlockPdbm() const
{
    return localInterface.getCh4InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh5Interlock() const
{
    return localInterface.getCh5Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh5InterlockEnable() const
{
    return localInterface.getCh5InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh5InterlockU() const
{
    return localInterface.getCh5InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh5InterlockP() const
{
    return localInterface.getCh5InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh5InterlockPdbm() const
{
    return localInterface.getCh5InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh6Interlock() const
{
    return localInterface.getCh6Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh6InterlockEnable() const
{
    return localInterface.getCh6InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh6InterlockU() const
{
    return localInterface.getCh6InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh6InterlockP() const
{
    return localInterface.getCh6InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh6InterlockPdbm() const
{
    return localInterface.getCh6InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh7Interlock() const
{
    return localInterface.getCh7Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh7InterlockEnable() const
{
    return localInterface.getCh7InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh7InterlockU() const
{
    return localInterface.getCh7InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh7InterlockP() const
{
    return localInterface.getCh7InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh7InterlockPdbm() const
{
    return localInterface.getCh7InterlockPdbm();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh8Interlock() const
{
    return localInterface.getCh8Interlock();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::getCh8InterlockEnable() const
{
    return localInterface.getCh8InterlockEnable();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh8InterlockU() const
{
    return localInterface.getCh8InterlockU();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh8InterlockP() const
{
    return localInterface.getCh8InterlockP();
}
//--------------------------------------------------------------------------------------------------
double liberaLLRFController::getCh8InterlockPdbm() const
{
    return localInterface.getCh8InterlockPdbm();
}







////______________________________________________________________________________
//void liberaLLRFController::startTraceMonitoring()
//{
//    localInterface.startTraceMonitoring();
//}
////______________________________________________________________________________
//bool liberaLLRFController::startTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
//{
//    return localInterface.startTraceMonitoring(pv);
//}
////______________________________________________________________________________
//bool liberaLLRFController::startTraceMonitoring(const std::string& name)
//{
//    return localInterface.startTraceMonitoring(name);
//}
////______________________________________________________________________________
//bool liberaLLRFController::startCavFwdTraceMonitor()
//{
//    return localInterface.startCavFwdTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::startCavRevTraceMonitor()
//{
//    return localInterface.startCavRevTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::startKlyFwdTraceMonitor()
//{
//    return localInterface.startKlyFwdTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::startKlyRevTraceMonitor()
//{
//    return localInterface.startKlyRevTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::stopTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
//{
//    return localInterface.stopTraceMonitoring(pv);
//}
////______________________________________________________________________________
//bool liberaLLRFController::stopTraceMonitoring(const std::string& name)
//{
//    return localInterface.stopTraceMonitoring(name);
//}
////______________________________________________________________________________
//void liberaLLRFController::stopTraceMonitoring()
//{
//    return localInterface.stopTraceMonitoring();
//}
////______________________________________________________________________________
//bool liberaLLRFController::stopCavFwdTraceMonitor()
//{
//    return localInterface.stopCavFwdTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::stopCavRevTraceMonitor()
//{
//    return localInterface.stopCavRevTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::stopKlyFwdTraceMonitor()
//{
//    return localInterface.stopKlyFwdTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::stopKlyRevTraceMonitor()
//{
//    return localInterface.stopKlyRevTraceMonitor();
//}
////______________________________________________________________________________
//bool liberaLLRFController::setNumContinuousOutsideMaskCount(const std::string& name, size_t val)
//{
//    return localInterface.setNumContinuousOutsideMaskCount(name,val);
//}
//______________________________________________________________________________
#ifdef BUILD_DLL
//______________________________________________________________________________
//____________________________________________________________________________________________
double liberaLLRFController::get_CA_PEND_IO_TIMEOUT() const
{
  return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//_____________________________________________________________________________________________
void liberaLLRFController::set_CA_PEND_IO_TIMEOUT(const double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > liberaLLRFController::getILockStates(const std::string & name)
{
    return localInterface.getILockStates(name);
}
//______________________________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string > liberaLLRFController::getILockStatesStr(const std::string & name)
{
    return localInterface.getILockStatesStr(name);
}
//______________________________________________________________________________
//______________________________________________________________________________
//boost::python::dict liberaLLRFController::getILockStatesStr_Py(std::string magName)
//{
//    return enumStringMapToPythonDict(getILockStatesStr(magName));
//}
////______________________________________________________________________________
//boost::python::dict liberaLLRFController::getILockStates_Py(std::string magName)
//{
//    return enumMapToPythonDict(getILockStates(magName));
//}
////______________________________________________________________________________
//boost::python::dict liberaLLRFController::getMagPSUStateDefinition()
//{
//    std::map< HWC_ENUM::MAG_PSU_STATE,  std::string  > m;
//
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF ] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON ] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ON); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING ] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR); // ENUM_TO_STRING MACRO in velaStructs.h
//    m[ HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE ] = ENUM_TO_STRING(HWC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE); // ENUM_TO_STRING MACRO in velaStructs.h
//
//    return enumStringMapToPythonDict(m);
//}
//______________________________________________________________________________
#endif // BUILD_DLL


