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
bool liberaLLRFController::setTORACQMEvent()
{
    return localInterface.setTORACQMEvent();
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
    std::string EVID;
    std::string time;
    std::string value;
    std::string name;

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

        EVID = "EVID_"  + omt.name + "_";
        //time = "time_"  + it2.name + "_";
        value= "value_" + omt.name + "_";
        name = "name_"  + omt.name + "_";
        time = "time_"  + omt.name + "_";
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


//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setUnwrapPhaseTolerance(const double value)
{
    return localInterface.setUnwrapPhaseTolerance(value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setUnwrapPhaseTolerance(const std::string& name,const double value)
{
    return localInterface.setUnwrapPhaseTolerance(name, value);
}

//--------------------------------------------------------------------------------------------------
























//______________________________________________________________________________
llrfStructs::LLRF_TYPE liberaLLRFController::getType()
{
    return localInterface.getType();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpFF()
{
    return localInterface.getAmpFF();
}
//______________________________________________________________________________
size_t liberaLLRFController::getShotCount(const std::string& name)
{
    return localInterface.getShotCount(name);
}
//______________________________________________________________________________
double liberaLLRFController::getAmpSP()
{
    return localInterface.getAmpSP();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiFF()
{
    return localInterface.getPhiFF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiSP()
{
    return localInterface.getPhiSP();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpLLRF()
{
    return localInterface.getAmpLLRF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiLLRF()
{
    return localInterface.getPhiLLRF();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiDEG()
{
    return localInterface.getPhiDEG();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpMVM()
{
    return localInterface.getAmpMVM();
}
//______________________________________________________________________________
double liberaLLRFController::getPulseLength()
{
    return localInterface.getPulseLength();
}
//______________________________________________________________________________
double liberaLLRFController::getPulseOffset()
{
    return localInterface.getPulseOffset();
}
//______________________________________________________________________________
double liberaLLRFController::getPhiCalibration()
{
    return localInterface.getPhiCalibration();
}
//______________________________________________________________________________
double liberaLLRFController::getAmpCalibration()
{
    return localInterface.getAmpCalibration();
}
//______________________________________________________________________________
double liberaLLRFController::getCrestPhiLLRF()
{
    return localInterface.getCrestPhiLLRF();
}

//______________________________________________________________________________
size_t liberaLLRFController::getTraceLength()
{
    return localInterface.getTraceLength();
}
//______________________________________________________________________________
std::vector<std::string> liberaLLRFController::getChannelNames()
{
    return localInterface.getChannelNames();
}
//______________________________________________________________________________
bool liberaLLRFController::setTraceSCAN(const std::string& trace, const llrfStructs::LLRF_SCAN value)
{
    return localInterface.setTraceSCAN(trace, value);
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
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getTraceValues(const std::string& name)
{
    return localInterface.getTraceValues(name);
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getTraceData(const std::string& name)
{
    return localInterface.getTraceData(name);
}
//______________________________________________________________________________
std::vector<double>liberaLLRFController::getAverageTraceData(const std::string& name)
{
    return localInterface.getAverageTraceData(name);
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getTraceBuffer(const std::string& name)
{
    return localInterface.getTraceBuffer(name);
}
//______________________________________________________________________________



std::vector<double> liberaLLRFController::getTimeVector()const
{
    return localInterface.getTimeVector();
}


//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getTimeVector_Py()const
{
    return toPythonList(getTimeVector());
}

boost::python::list liberaLLRFController::getAverageTraceData_Py(const std::string& name)
{
    return toPythonList(getAverageTraceData(name));
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getChannelNames_Py()
{
    return toPythonList(getChannelNames());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTraceNames_Py()
{
    return toPythonList(getTraceNames());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTraceValues_Py(const std::string& name)
{
    return toPythonList(getTraceValues(name));
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getTraceBuffer_Py(const std::string& name)
{
    return toPythonList(getTraceBuffer(name));
}



//______________________________________________________________________________
boost::python::dict liberaLLRFController::dump_traces()
{
    const llrfStructs::liberallrfObject& ref = localInterface.getLLRFObjConstRef();

    boost::python::dict dictionary;

    std::string EVID;
    std::string time;
    std::string value;
    std::string name;

    int i;
    for(auto&& it: ref.tracesToSaveOnBreakDown)
    {
        if(ref.trace_data.at(it).hi_mask_set)
        {
            dictionary[ std::string(it + "_hi_mask") ] = toPythonList(ref.trace_data.at(it).high_mask);
        }
        if(ref.trace_data.at(it).low_mask_set)
        {
            dictionary[ std::string(it + "_lo_mask") ] = toPythonList(ref.trace_data.at(it).low_mask);
            dictionary[ std::string(it + "_mask_floor") ] = ref.trace_data.at(it).mask_floor;
        }
        i = 0;
        for(auto && it2: ref.trace_data.at(it).traces)
        {
                EVID = "EVID_"  + it2.name + "_";
                time = "time_"  + it2.name + "_";
                value= "value_" + it2.name + "_";
                name = "name_"  + it2.name + "_";
                time = "time_"  + it2.name + "_";
                dictionary[EVID  += std::to_string(i)] = it2.EVID;
                dictionary[time  += std::to_string(i)] = it2.time;
                dictionary[value += std::to_string(i)] = toPythonList(it2.value);
                dictionary[name  += std::to_string(i)] = it2.name;
                ++i;
        }
    }

    return dictionary;
}
#endif
//______________________________________________________________________________
std::string liberaLLRFController::fullCavityTraceName(const std::string& name)
{
    return localInterface.fullCavityTraceName(name);
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavRevPower()
{
    return localInterface.getCavRevPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavFwdPower()
{
    return localInterface.getCavFwdPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyRevPower()
{
    return localInterface.getKlyRevPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyFwdPower()
{
    return localInterface.getKlyFwdPower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavRevPhase()
{
    return localInterface.getCavRevPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavFwdPhase()
{
    return localInterface.getCavFwdPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyRevPhase()
{
    return localInterface.getKlyRevPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyFwdPhase()
{
    return localInterface.getKlyFwdPhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getProbePower()
{
    return localInterface.getProbePower();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getProbePhase()
{
    return localInterface.getProbePhase();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavRevPowerAv()
{
    return localInterface.getCavRevPowerAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavFwdPowerAv()
{
    return localInterface.getCavFwdPowerAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyRevPowerAv()
{
    return localInterface.getKlyRevPowerAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyFwdPowerAv()
{
    return localInterface.getKlyFwdPowerAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavRevPhaseAv()
{
    return localInterface.getCavRevPhaseAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getCavFwdPhaseAv()
{
    return localInterface.getCavFwdPhaseAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyRevPhaseAv()
{
    return localInterface.getKlyRevPhaseAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getKlyFwdPhaseAv()
{
    return localInterface.getKlyFwdPhaseAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getProbePowerAv()
{
    return localInterface.getProbePowerAv();
}
//______________________________________________________________________________
std::vector<double> liberaLLRFController::getProbePhaseAv()
{
    return localInterface.getProbePhaseAv();
}
#ifdef BUILD_DLL
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPowerAv_Py()
{
    return toPythonList(getCavRevPowerAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPowerAv_Py()
{
    return toPythonList(getCavFwdPowerAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPowerAv_Py()
{
    return toPythonList(getKlyRevPowerAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPowerAv_Py()
{
    return toPythonList(getKlyFwdPowerAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPhaseAv_Py()
{
    return toPythonList(getCavRevPhaseAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPhaseAv_Py()
{
    return toPythonList(getCavFwdPhaseAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPhaseAv_Py()
{
    return toPythonList(getKlyRevPhaseAv());
}
//______________________________________________________________________________
boost::python::list  liberaLLRFController::getKlyFwdPhaseAv_Py()
{
    return toPythonList(getKlyFwdPhaseAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPower_Py()
{
    return toPythonList(getCavRevPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPower_Py()
{
    return toPythonList(getKlyFwdPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPower_Py()
{
    return toPythonList(getCavFwdPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPower_Py()
{
    return toPythonList(getKlyRevPower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPhase_Py()
{
    return toPythonList(getCavRevPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPhase_Py()
{
    return toPythonList(getKlyFwdPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPhase_Py()
{
    return toPythonList(getCavFwdPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPhase_Py()
{
    return toPythonList(getKlyRevPhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getProbePower_Py()
{
    return toPythonList(getProbePower());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getProbePhase_Py()
{
    return toPythonList(getProbePhase());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPowerBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavRevPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getCavFwdPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyRevPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getKlyFwdPhaseBuffer_Py()
{
    return toPythonList(getCavRevPowerBuffer());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getProbePowerAv_Py()
{
    return toPythonList(getProbePowerAv());
}
//______________________________________________________________________________
boost::python::list liberaLLRFController::getProbePhaseAv_Py()
{
    return toPythonList(getProbePhaseAv());
}
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
llrfStructs::rf_trace liberaLLRFController::getCavRevPowerData()
{
    return localInterface.getCavRevPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavFwdPowerData()
{
    return localInterface.getCavFwdPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyRevPowerData()
{
    return localInterface.getKlyRevPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyFwdPowerData()
{
    return localInterface.getKlyFwdPowerData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavRevPhaseData()
{
    return localInterface.getCavRevPhaseData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getCavFwdPhaseData()
{
    return localInterface.getCavFwdPhaseData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyRevPhaseData()
{
    return localInterface.getKlyRevPhaseData();
}
//______________________________________________________________________________
llrfStructs::rf_trace liberaLLRFController::getKlyFwdPhaseData()
{
    return localInterface.getKlyFwdPhaseData();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavRevPowerBuffer()
{
    return localInterface.getCavRevPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavFwdPowerBuffer()
{
    return localInterface.getCavFwdPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyRevPowerBuffer()
{
    return localInterface.getKlyRevPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyFwdPowerBuffer()
{
    return localInterface.getKlyFwdPowerBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavRevPhaseBuffer()
{
    return localInterface.getCavRevPhaseBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getCavFwdPhaseBuffer()
{
    return localInterface.getCavFwdPhaseBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyRevPhaseBuffer()
{
    return localInterface.getKlyRevPhaseBuffer();
}
//______________________________________________________________________________
std::vector<llrfStructs::rf_trace> liberaLLRFController::getKlyFwdPhaseBuffer()
{
    return localInterface.getKlyFwdPhaseBuffer();
}
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
bool liberaLLRFController::resetToValue(const double value)
{
    return localInterface.resetToValue(value);
}
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
//______________________________________________________________________________
bool liberaLLRFController::shouldCheckMasks(const std::string& name)
{
    return localInterface.shouldCheckMasks(name);
}


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
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::clearTraceRollingAverage(const std::string& name)
{
    return localInterface.clearTraceRollingAverage(name);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setKeepRollingAverageNoReset(const bool value)
{
    localInterface.setKeepRollingAverageNoReset(value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setKeepRollingAverageNoReset(const std::string&name, const bool value)
{
    return localInterface.setKeepRollingAverageNoReset(name, value);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setShouldKeepRollingAverage()
{
    localInterface.setShouldKeepRollingAverage();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setShouldKeepRollingAverage(const std::string&name)
{
    return localInterface.setShouldKeepRollingAverage(name);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setShouldNotKeepRollingAverage()
{
    localInterface.setShouldNotKeepRollingAverage();
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setKeepRollingAverage(const std::string&name, bool value)
{
    return localInterface.setKeepRollingAverage(name, value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setTraceRollingAverageSize(const std::string&name, const size_t value)
{
    return localInterface.setTraceRollingAverageSize(name, value);
}
//--------------------------------------------------------------------------------------------------
void liberaLLRFController::setAllRollingAverageSize(const size_t value)
{
    localInterface.setAllRollingAverageSize(value);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::setShouldNotKeepRollingAverage(const std::string& name)
{
    return localInterface.setShouldNotKeepRollingAverage(name);
}
//--------------------------------------------------------------------------------------------------
std::vector<double> liberaLLRFController::getTraceRollingAverage(const std::string&name)const
{
    return localInterface.getTraceRollingAverage(name);
}
//--------------------------------------------------------------------------------------------------
#ifdef BUILD_DLL
boost::python::list liberaLLRFController::getTraceRollingAverage_Py(const std::string&name)const
{
    return toPythonList(getTraceRollingAverage(name));
}
#endif
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getTraceRollingAverageSize(const std::string&name)const
{
    return localInterface.getTraceRollingAverageSize(name);
}
//--------------------------------------------------------------------------------------------------
size_t liberaLLRFController::getTraceRollingAverageCount(const std::string&name) const
{
    return localInterface.getTraceRollingAverageCount(name);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::isKeepingRollingAverage(const std::string&name)const
{
    return localInterface.isKeepingRollingAverage(name);
}
//--------------------------------------------------------------------------------------------------
bool liberaLLRFController::hasRollingAverage(const std::string&name)const
{
    return localInterface.hasRollingAverage(name);
}










//____________________________________________________________________________________________
bool liberaLLRFController::setMeanStartEndTime(const double start, const double end, const std::string&name)
{
    return localInterface.setMeanStartEndTime(start, end,name);
}
//____________________________________________________________________________________________
bool liberaLLRFController::setMeanStartIndex(const std::string&name, size_t  value)
{
    return localInterface.setMeanStartIndex(name, value);
}
//____________________________________________________________________________________________
bool liberaLLRFController::setMeanStopIndex(const std::string&name, size_t  value)
{
    return localInterface.setMeanStopIndex(name, value);
}

//______________________________________________________________________________
bool liberaLLRFController::isTracePV(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.Is_TracePV(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotTracePV(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.IsNot_TracePV(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.isNotMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::isMonitoring(const std::string& name)
{
    return localInterface.isMonitoring(name);
}
//______________________________________________________________________________
bool liberaLLRFController::isNotMonitoring(const std::string& name)
{
    return localInterface.isNotMonitoring(name);
}
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
//______________________________________________________________________________
void liberaLLRFController::startTraceMonitoring()
{
    localInterface.startTraceMonitoring();
}
//______________________________________________________________________________
bool liberaLLRFController::startTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.startTraceMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::startTraceMonitoring(const std::string& name)
{
    return localInterface.startTraceMonitoring(name);
}
//______________________________________________________________________________
bool liberaLLRFController::startCavFwdTraceMonitor()
{
    return localInterface.startCavFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::startCavRevTraceMonitor()
{
    return localInterface.startCavRevTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::startKlyFwdTraceMonitor()
{
    return localInterface.startKlyFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::startKlyRevTraceMonitor()
{
    return localInterface.startKlyRevTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv)
{
    return localInterface.stopTraceMonitoring(pv);
}
//______________________________________________________________________________
bool liberaLLRFController::stopTraceMonitoring(const std::string& name)
{
    return localInterface.stopTraceMonitoring(name);
}
//______________________________________________________________________________
void liberaLLRFController::stopTraceMonitoring()
{
    return localInterface.stopTraceMonitoring();
}
//______________________________________________________________________________
bool liberaLLRFController::stopCavFwdTraceMonitor()
{
    return localInterface.stopCavFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopCavRevTraceMonitor()
{
    return localInterface.stopCavRevTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopKlyFwdTraceMonitor()
{
    return localInterface.stopKlyFwdTraceMonitor();
}
//______________________________________________________________________________
bool liberaLLRFController::stopKlyRevTraceMonitor()
{
    return localInterface.stopKlyRevTraceMonitor();
}
//______________________________________________________________________________
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


