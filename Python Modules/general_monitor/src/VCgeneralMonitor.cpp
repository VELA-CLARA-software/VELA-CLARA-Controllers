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
//  Last edit:   22-02-2019
//  FileName:    VCgeneralMonitor.cpp
//  Description:
//
//
//*/
// stl
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <thread>
#include <chrono>
#include <utility>
// epics
#include <epicsTime.h>
// VC-HWC
#include "VCgeneralMonitor.h"


VCgeneralMonitor::VCgeneralMonitor(const bool showmess ,const bool  showdebug ):
shouldShowDebugMessage(showdebug),
shouldShowMessage(showmess),
VCbase("VCpilaser"),
controller( shouldShowMessage, shouldShowDebugMessage,HWC_ENUM::CONTROLLER_TYPE::GENERAL_MONITOR,"General Monitor"),
pvMonitorMapCount(UTL::ZERO_INT),
defaultCOUNT(1),
defaultMASK(DBE_VALUE),
stringPrefix("t"),
intPrefix("i"),
shortPrefix("h"),
floatPrefix("f"),
enumPrefix("e"),
charPrefix("c"),
longPrefix("l"),
doublePrefix("d"),
vecdoublePrefix("D"),
vecintPrefix("I"),
ca_chid_successmess("Successfully found ChId."),
ca_chid_failuremess("!!TIMEOUT!! ChIds NOT found."),
ca_subs_successmess("Succesfully subscribed, now monitoring."),
ca_subs_failuremess("!!TIMEOUT!! Subscription to monitor failed."),
ca_create_channel_str("ca_create_channel"),
ca_create_subscription_str("ca_create_subscription"),
ca_clear_subscription_str("ca_clear_subscription"),
ca_csub_successmess("Succesfully un-subscribed, not monitoring."),
ca_csub_failuremess("!!TIMEOUT!! un-subscription to monitor failed."),
ca_clear_channel_str("ca_clear_channel"),
ca_cchi_successmess("Succesfully cleared ChId."),
ca_cchi_failuremess("!!TIMEOUT!! clearing ChID failed."),
returnFail("FAILED"),
CA_PEND_IO_TIMEOUT(2.0)
{
    // This 'enables' callbacks, monitoring, etc
    ca_context_create( ca_enable_preemptive_callback);
    // This is the current (AND ONLY) context,
    // use it to join from new threads with ca_attach_context
    // you need to attach to this context if multi-threading
    thisCaContext = ca_current_context();
}
//______________________________________________________________________________
VCgeneralMonitor::~VCgeneralMonitor()
{
    for( auto && it : continuousMonitorStructs)
    {
        delete it;
    }
}
//______________________________________________________________________________
void VCgeneralMonitor::setRunningStatCountMax(const std::string& id, const size_t value )
{
  if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             stringPVMap[id].data[0]->rs_count_max = value;
             //stringPVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             intPVMap[id].data[0]->rs_count_max = value;
             //intPVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            floatPVMap[id].data[0]->rs_count_max = value;
            //floatPVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            enumPVMap[id].data[0]->rs_count_max = value;
            //enumPVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            charPVMap[id].data[0]->rs_count_max = value;
            //charPVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            longPVMap[id].data[0]->rs_count_max = value;
            //longPVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            doublePVMap[id].data[0]->rs_count_max = value;
            //doublePVMap[id].data[0]->buffer.resize(value);
        }
    }

    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            vec_doublePVMap[id].data[0]->rs_count_max = value;
            //vec_doublePVMap[id].data[0]->buffer.resize(value);
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists(vec_intPVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            vec_intPVMap[id].data[0]->rs_count_max = value;
            //vec_intPVMap[id].data[0]->buffer.resize(value);
        }
    }
}
//______________________________________________________________________________
bool VCgeneralMonitor::isRunningStatComplete(const std::string& id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return stringPVMap[id].data[0]->rs_complete;
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return intPVMap[id].data[0]->rs_complete;
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return floatPVMap[id].data[0]->rs_complete;
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return enumPVMap[id].data[0]->rs_complete;
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return charPVMap[id].data[0]->rs_complete;
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return longPVMap[id].data[0]->rs_complete;
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return doublePVMap[id].data[0]->rs_complete;
        }
    }

    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return vec_doublePVMap[id].data[0]->rs_complete;
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists(vec_intPVMap, id))
        {//message("getPVStruct ",  id, " vec_intPVMap");
            return vec_intPVMap[id].data[0]->rs_complete;
        }
    }

    return false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isRunningStatNotComplete(const std::string& id)
{
    return !isRunningStatComplete(id);
}
//______________________________________________________________________________
void VCgeneralMonitor::clearRunningValues(const std::string& id )
{
  if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             stringPVMap[id].data[0]->rs.Clear();
             stringPVMap[id].data[0]->rs_count = 0;
             stringPVMap[id].data[0]->rs_complete = false;
             stringPVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             intPVMap[id].data[0]->rs.Clear();
             intPVMap[id].data[0]->rs_count = 0;
             intPVMap[id].data[0]->rs_complete = false;
             intPVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            floatPVMap[id].data[0]->rs.Clear();
            floatPVMap[id].data[0]->rs_count = 0;
            floatPVMap[id].data[0]->rs_complete = false;
            floatPVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            enumPVMap[id].data[0]->rs.Clear();
            enumPVMap[id].data[0]->rs_count = 0;
            enumPVMap[id].data[0]->rs_complete = false;
            enumPVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            charPVMap[id].data[0]->rs.Clear();
            charPVMap[id].data[0]->rs_count = 0;
            charPVMap[id].data[0]->rs_complete = false;
            charPVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            longPVMap[id].data[0]->rs.Clear();
            longPVMap[id].data[0]->rs_count = 0;
            longPVMap[id].data[0]->rs_complete = false;
            longPVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            doublePVMap[id].data[0]->rs.Clear();
            doublePVMap[id].data[0]->rs_count = 0;
            doublePVMap[id].data[0]->rs_complete = false;
            doublePVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            vec_doublePVMap[id].data[0]->rs_count = 0;
            vec_doublePVMap[id].data[0]->rs_complete = false;
            vec_doublePVMap[id].data[0]->buffer.clear();
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists(vec_intPVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            vec_intPVMap[id].data[0]->rs_count = 0;
            vec_intPVMap[id].data[0]->rs_complete = false;
            vec_intPVMap[id].data[0]->buffer.clear();
        }
    }


}
//______________________________________________________________________________
//______________________________________________________________________________
size_t VCgeneralMonitor::getRunningStatCountMax(const std::string & id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return stringPVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return intPVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return floatPVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return enumPVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return charPVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return longPVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return doublePVMap[id].data[0]->rs_count_max;
        }
    }

    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return vec_doublePVMap[id].data[0]->rs_count_max;
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists(vec_intPVMap, id))
        {//message("getPVStruct ",  id, " vec_intPVMap");
            return vec_intPVMap[id].data[0]->rs_count_max;
        }
    }

    return 0;
}


size_t VCgeneralMonitor::getBufferSize(const std::string & id)
{
    return getRunningStatCountMax(id);
}

//______________________________________________________________________________
size_t VCgeneralMonitor::getRunningStatCount(const std::string & id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return stringPVMap[id].data[0]->rs_count;
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return intPVMap[id].data[0]->rs_count;
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return floatPVMap[id].data[0]->rs_count;
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return enumPVMap[id].data[0]->rs_count;
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return charPVMap[id].data[0]->rs_count;
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return longPVMap[id].data[0]->rs_count;
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return doublePVMap[id].data[0]->rs_count;
        }
    }
    return 0;
}
//______________________________________________________________________________
size_t VCgeneralMonitor::getBufferCount(const std::string& id )
{
    return getRunningStatCount(id);
}
//______________________________________________________________________________
bool VCgeneralMonitor::isBufferFull(const std::string& id)
{
    return isRunningStatComplete(id);
}
//______________________________________________________________________________
void VCgeneralMonitor::clearBuffer(const std::string& id)
{
    clearRunningValues(id);
}
//______________________________________________________________________________
void VCgeneralMonitor::setBufferSize(const std::string& id, const size_t value  )
{
    setRunningStatCountMax(id, value );
}
//______________________________________________________________________________

#ifdef BUILD_DLL
//______________________________________________________________________________
boost::python::dict VCgeneralMonitor::getBuffer(const std::string& id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return toPythonDict(stringPVMap[id].data[0]->buffer);
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return toPythonDict(intPVMap[id].data[0]->buffer);
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return toPythonDict(floatPVMap[id].data[0]->buffer);
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return toPythonDict(enumPVMap[id].data[0]->buffer);
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return toPythonDict(charPVMap[id].data[0]->buffer);
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return toPythonDict(longPVMap[id].data[0]->buffer);
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {
            message("toPythonDict ",  id, " ...");
            return toPythonDict(doublePVMap[id].data[0]->buffer);
        }
    }
    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return toPythonDict2(vec_doublePVMap[id].data[0]->buffer);
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists(vec_intPVMap, id))
        {//message("getPVStruct ",  id, " vec_intPVMap");
            return toPythonDict2(vec_intPVMap[id].data[0]->buffer);
        }
    }
    boost::python::dict d ;
    return d;
}
//______________________________________________________________________________
boost::python::dict VCgeneralMonitor::getValue(const boost::python::list& ids)
{
    std::vector<std::string> idss = to_std_vector<std::string>(ids);
    std::vector<std::string> valid_ids;
    for(auto && it : idss)
    {
        if( isValidID(it))
            valid_ids.push_back(it);
    }
    boost::python::dict r;
    for(auto && it : valid_ids)
    {
        r[it] = getValue(it);
    }
    return r;
}
//______________________________________________________________________________
boost::python::dict VCgeneralMonitor::getCounterAndValue(const boost::python::list& ids)
{
    std::vector<std::string> idss = to_std_vector<std::string>(ids);
    std::vector<std::string> valid_ids;
    for(auto && it : idss)
    {
        if( isValidID(it))
            valid_ids.push_back(it);
    }
    boost::python::dict r;
    for(auto && it : valid_ids)
    {
        r[it] = getCounterAndValue(it);
    }
    return r;
}
//______________________________________________________________________________
boost::python::object VCgeneralMonitor::getRunningMean(const std::string & id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(stringPVMap[id].data[0]->rs.Mean());
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(intPVMap[id].data[0]->rs.Mean());
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return object(floatPVMap[id].data[0]->rs.Mean());
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return object(enumPVMap[id].data[0]->rs.Mean());
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return object(charPVMap[id].data[0]->rs.Mean());
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return object(longPVMap[id].data[0]->rs.Mean());
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return object(doublePVMap[id].data[0]->rs.Mean());
        }
    }
    return object();
}

//______________________________________________________________________________
boost::python::object VCgeneralMonitor::getRunningVariance(const std::string & id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(stringPVMap[id].data[0]->rs.Variance());
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(intPVMap[id].data[0]->rs.Variance());
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return object(floatPVMap[id].data[0]->rs.Variance());
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return object(enumPVMap[id].data[0]->rs.Variance());
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return object(charPVMap[id].data[0]->rs.Variance());
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return object(longPVMap[id].data[0]->rs.Variance());
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return object(doublePVMap[id].data[0]->rs.Variance());
        }
    }
    return object();
}

//______________________________________________________________________________
boost::python::object VCgeneralMonitor::getRunningStandardDeviation(const std::string & id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(stringPVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(intPVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return object(floatPVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return object(enumPVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return object(charPVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return object(longPVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return object(doublePVMap[id].data[0]->rs.StandardDeviation());
        }
    }
    return object();
}







//______________________________________________________________________________
boost::python::object VCgeneralMonitor::getValue(const std::string & id)
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(stringPVMap[id].data[0]->v);
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             return object(intPVMap[id].data[0]->v);
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            return object(floatPVMap[id].data[0]->v);
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            return object(enumPVMap[id].data[0]->v);
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            return object(charPVMap[id].data[0]->v);
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            return object(longPVMap[id].data[0]->v);
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            return object(doublePVMap[id].data[0]->v);
        }
    }
    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {
            return toPythonList(vec_doublePVMap[id].data[0]->v);
        }
    }
    else if(isArrayIntPV(id))
    {
        if( entryExists(vec_intPVMap,id))
        {
            return toPythonList( vec_intPVMap[id].data[0]->v);
        }
    }
    return object();
}
#endif
//______________________________________________________________________________
size_t VCgeneralMonitor::getCounter(const std::string & id)
{
    size_t r = 0;
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             r = stringPVMap[id].data[0]->c;
        }
    }
    else if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
             r = intPVMap[id].data[0]->c;
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            r = floatPVMap[id].data[0]->c;
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            r = enumPVMap[id].data[0]->c;
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            r = charPVMap[id].data[0]->c;
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            r = longPVMap[id].data[0]->c;
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            r = doublePVMap[id].data[0]->c;
        }
    }
    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {
            r = vec_doublePVMap[id].data[0]->c;
        }
    }
    else if(isArrayIntPV(id))
    {
        if( entryExists(vec_intPVMap,id))
        {
            r = vec_intPVMap[id].data[0]->c;
        }
    }
    return r;
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::dict VCgeneralMonitor::getCounterAndValue(const std::string& id)
{
    boost::python::dict r;
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
            r[stringPVMap[id].data[0]->c] = stringPVMap[id].data[0]->v;
        }
    }
    if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {    //message("getPVStruct ",  id, " isIntPV");
            r[intPVMap[id].data[0]->c] = intPVMap[id].data[0]->v;
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {//message("getPVStruct ",  id, " isFloatPV");
            r[floatPVMap[id].data[0]->c] = floatPVMap[id].data[0]->v;
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {//message("getPVStruct ",  id, " isEnumPV");
            r[enumPVMap[id].data[0]->c] = enumPVMap[id].data[0]->v;
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {//message("getPVStruct ",  id, " isCharPV");
            r[charPVMap[id].data[0]->c] = charPVMap[id].data[0]->v;
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {//message("getPVStruct ",  id, " isLongPV");
            r[longPVMap[id].data[0]->c] = longPVMap[id].data[0]->v;
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {//message("getPVStruct ",  id, " isDoublePV");
            r[doublePVMap[id].data[0]->c] = doublePVMap[id].data[0]->v;
        }
    }
    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {
            r[vec_doublePVMap[id].data[0]->c] = vec_doublePVMap[id].data[0]->v;
        }
    }
    else if(isArrayIntPV(id))
    {
        if( entryExists(vec_intPVMap,id))
        {
            r[vec_intPVMap[id].data[0]->c] = vec_intPVMap[id].data[0]->v;
        }
    }
    return r;
}
//______________________________________________________________________________
boost::python::dict VCgeneralMonitor::getCounterAndTotalValue(const std::string& id)
{
    if( isArrayPV(id))
    {
        if(isArrayDoublePV(id))
        {
            if(entryExists(vec_doublePVMap, id))
            {
                boost::python::dict r;
                r[vec_doublePVMap[id].data[0]->c] = std::accumulate(vec_doublePVMap[id].data[0]->v.begin(),
                                                                    vec_doublePVMap[id].data[0]->v.end(), 0);
                return r;
            }
        }
        else if(isArrayIntPV(id))
        {
            if( entryExists(vec_intPVMap,id))
            {
                boost::python::dict r;
                r[vec_intPVMap[id].data[0]->c] = std::accumulate(vec_intPVMap[id].data[0]->v.begin(),
                                                                 vec_intPVMap[id].data[0]->v.end(), 0);
                return r;
            }
        }
    }
    else
    {
        return getCounterAndValue(id);
    }
}
//______________________________________________________________________________
boost::python::object VCgeneralMonitor::getTotalValue(const std::string& id)
{
    if( isArrayPV(id))
    {
        if(isArrayDoublePV(id))
        {
            if(entryExists(vec_doublePVMap, id))
            {
                double total = std::accumulate(vec_doublePVMap[id].data[0]->v.begin(),
                                               vec_doublePVMap[id].data[0]->v.end(), 0);
                message("total = ", total);
                return object(total);
            }
        }
        else if(isArrayIntPV(id))
        {
            if( entryExists(vec_intPVMap,id))
            {
                int total = std::accumulate(vec_intPVMap[id].data[0]->v.begin(),
                                            vec_intPVMap[id].data[0]->v.end(), 0);
                message("total = ", total);
                return object(total);
            }
        }
    }
    else
    {
        return getValue(id);
    }
}
//______________________________________________________________________________
boost::python::object VCgeneralMonitor::getValue(const std::string & id,const int index)
{
    //gmStructs::pvStruct* pvs = nullptr;
    size_t index_positive;
    if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id)) // OMFG o.O
        {
            index_positive = getArrayIndex(index, vec_doublePVMap[id].data[0]->v.size());
            return object( vec_doublePVMap[id].data[0]->v[index_positive]);
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists(vec_intPVMap, id))
        {//            size_t index_positive = getArrayIndex(index, vec_intPVMap[id].data[0]->v.size());
            index_positive = getArrayIndex(index, vec_intPVMap[id].data[0]->v.size());
            return object(  vec_intPVMap[id].data[0]->v[index_positive]);
        }
    }
    return object();
}
#endif
//______________________________________________________________________________
size_t VCgeneralMonitor::getArrayIndex(const int index, const size_t vec_size)
{
    // first test to see if entries are oob
    size_t r = vec_size + 1; // init -to unacceptable value
    if(index > -1 && index + 1 >= vec_size)//MAGIC_NUMBER
    {
        r = 0;//MAGIC_NUMBER
    }
    else if(index < 0 && vec_size + index < 0)
    {
        r = 0;//MAGIC_NUMBER
    }
    else if(index < 0)//MAGIC_NUMBER
    {
        r = vec_size + index;
    }
    else if(index > -1)
    {
        r = index;
    }
    message("Array Size = ", vec_size, ", requested index = ", index, ", returning ", r);
    return r;
}
#ifdef BUILD_DLL
//______________________________________________________________________________
boost::python::list VCgeneralMonitor::getValue(const std::string & id,const int start_pos,const int end_pos)
{
    boost::python::list output;
    if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id)) // OMFG o.O
        {
            std::vector< size_t > pos = getArrayRegionOfInterest(start_pos, end_pos, vec_doublePVMap[id].data[0]->v.size());
            auto first = vec_doublePVMap[id].data[0]->v.begin() + pos[0];
            auto last  = vec_doublePVMap[id].data[0]->v.begin() + pos[1];
            std::vector< double > r(first, last);
            output = toPythonList( r);
        }
    }
    else if(isArrayIntPV(id))
    {
        if(entryExists( vec_intPVMap, id))
        {
            std::vector< size_t > pos = getArrayRegionOfInterest(start_pos, end_pos, vec_intPVMap[id].data[0]->v.size());
            auto first = vec_intPVMap[id].data[0]->v.begin() + pos[0];
            auto last  = vec_intPVMap[id].data[0]->v.begin() + pos[1];
            std::vector< int > r(first, last);
            output = toPythonList( r);
        }
    }
    return output;
}
#endif
//______________________________________________________________________________
std::vector< size_t > VCgeneralMonitor::getArrayRegionOfInterest(const int start_position,const int end_position,const size_t vec_size)
{
    // first tesst to see if entries are oob
    std::vector< size_t > r;
    if( start_position > -1 && start_position + 1 > vec_size )//MAGIC_NUMBER
    {
        r.push_back(0);//MAGIC_NUMBER
        r.push_back(vec_size);
    }
    else if( start_position < 0 && vec_size + start_position < 0 )
    {
        r.push_back(0);//MAGIC_NUMBER
        r.push_back(vec_size);
    }
    else if( end_position > -1 && end_position + 1 > vec_size )//MAGIC_NUMBER
    {
        r.push_back(0);//MAGIC_NUMBER
        r.push_back(vec_size);
    }
    else if( end_position < 0 && vec_size + end_position < 0 )
    {
        r.push_back(0);//MAGIC_NUMBER
        r.push_back(vec_size);
    }
    else if( start_position < 0 && end_position < 0 && end_position >= start_position )
    {
        r.push_back(vec_size + start_position);//MAGIC_NUMBER
        r.push_back(vec_size + end_position);
    }
    else if( start_position > 0  && end_position > 0 && start_position <= end_position )
    {
        r.push_back(start_position);//MAGIC_NUMBER
        r.push_back(end_position);
    }
    else
    {
        message("getArrayRegionOfInterest reached deafult option");
        r.push_back(0);//MAGIC_NUMBER
        r.push_back(vec_size-1);
    }
    std::stringstream ss;
    ss<< "Array Size = ";
    ss<< start_position;
    ss<< ", requested indices = ";
    ss<< start_position;
    ss<< ", ";
    ss<< end_position;
    ss<< ", returning ";
    ss<< r[0];
    ss<< ", ";
    ss<< r[1];
    message(ss);
    return r;
}
//______________________________________________________________________________
size_t VCgeneralMonitor::getPVCount(const std::string & id )
{
    if(isStringPV(id))
    {
        if(entryExists(stringPVMap, id))
        {
            //message("getPVStruct ",  id, " stringPVMap");
             return stringPVMap[id].pvs.COUNT;
            //message("getPVStruct found ",  id, " in stringPVMap");
//            success = true;
        }
    }
    if(isIntPV(id))
    {
        if(entryExists(intPVMap, id))
        {
            //message("getPVStruct ",  id, " isIntPV");
             return intPVMap[id].pvs.COUNT;
            //message("getPVStruct found ",  id, " in intPVMap");
//            success = true;
        }
    }
    else if(isFloatPV(id))
    {
        if(entryExists(floatPVMap, id))
        {
            //message("getPVStruct ",  id, " isFloatPV");
            return floatPVMap[id].pvs.COUNT;
        }
    }
    else if(isEnumPV(id))
    {
        if(entryExists(enumPVMap, id))
        {
            //message("getPVStruct ",  id, " isEnumPV");
            return enumPVMap[id].pvs.COUNT;
        }
    }
    else if(isCharPV(id))
    {
        if( entryExists(charPVMap, id))
        {
            //message("getPVStruct ",  id, " isCharPV");
            return charPVMap[id].pvs.COUNT;
        }
    }
    else if(isLongPV(id))
    {
        if(entryExists(longPVMap, id))
        {
            return longPVMap[id].pvs.COUNT;
        }
    }
    else if(isDoublePV(id))
    {
        if(entryExists(doublePVMap, id))
        {
            return doublePVMap[id].pvs.COUNT;
        }
    }
    else if(isArrayDoublePV(id))
    {
        if(entryExists(vec_doublePVMap, id))
        {
            return vec_doublePVMap[id].pvs.COUNT;
        }
    }
    else if(isArrayIntPV(id))
    {
        if( entryExists(vec_intPVMap,id))
        {
            return vec_intPVMap[id].pvs.COUNT;
        }
    }
    return 0; //MAGIC_NUMBER
}
//______________________________________________________________________________
bool VCgeneralMonitor::isValidID(const std::string& id)
{
    if(entryExists(intPVMap, id))
        return true;
    else if(entryExists(floatPVMap, id))
        return true;
    else if(entryExists(enumPVMap, id))
        return true;
    else if(entryExists(charPVMap, id))
        return true;
    else if(entryExists(longPVMap, id))
        return true;
    else if(entryExists(doublePVMap, id))
        return true;
    else if(entryExists(vec_doublePVMap, id))
        return true;
    else if(entryExists(vec_intPVMap, id))
        return true;
    else if(entryExists(stringPVMap, id))
        return true;
    return false;
}
//______________________________________________________________________________



//______________________________________________________________________________
std::string VCgeneralMonitor::connectPV(const std::string& pvFullName)
{
    debugMessage("Looking for ", pvFullName);
    std::string r = returnFail;
    chid CHID;
    ca_create_channel(pvFullName.c_str(), 0, 0, 0,&CHID);
    int status = sendToEpics(ca_create_channel_str,ca_chid_successmess,ca_chid_failuremess);
    if( status == ECA_TIMEOUT)
    {
        //removeChanel();
    }
    else if(status == ECA_NORMAL)
    {
        status = ca_field_type(CHID);
        int COUNT = ca_element_count(CHID);
            switch( status)
            {
                case 0:
                    message("PV ", pvFullName, " is a DBR_STRING, connecting to channel");
                    r = connectPV(pvFullName, "DBR_STRING");
                    break;
                case 1:
                    message("PV ", pvFullName, " is a DBR_INT, connecting to channel");
                    r = connectPV(pvFullName, "DBR_INT");
                case 2:
                    message("PV ", pvFullName, " is a DBR_FLOAT, connecting to channel");
                    r = connectPV(pvFullName, "DBR_FLOAT");
                    break;
                case 3:
                    message("PV ", pvFullName," is a DBR_ENUM, connecting to channel");
                    r = connectPV(pvFullName, "DBR_ENUM");
                case 4:
                    message("PV ", pvFullName, " is a DBR_CHAR, connecting to channel");
                    r = connectPV(pvFullName, "DBR_CHAR");
                    break;
                case 5:
                    message("PV ", pvFullName, " is a DBR_LONG, connecting to channel");
                    r = connectPV(pvFullName, "DBR_LONG");
                case 6:
                    if( COUNT > 1)
                    {
                        message("PV ", pvFullName, " is a DBR_ARRAY_DOUBLE, connecting to channel");
                        r= connectPV(pvFullName, "DBR_ARRAY_DOUBLE");
                    }
                    else
                    {
                        message("PV ", pvFullName, " is a DBR_DOUBLE, connecting to channel");
                        r= connectPV(pvFullName, "DBR_DOUBLE");
                    }
                    break;
                case 7:
                    message("PV ", pvFullName, " is a DBR_STS_STRING, connecting to channel");
                    r= connectPV(pvFullName, "DBR_STRING");
                    break;
                case 8:
                    message("PV ", pvFullName, " is a DBR_STS_SHORT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_SHORT");
                    break;
                case 9:
                    message("PV ", pvFullName, " is a DBR_STS_FLOAT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_FLOAT");
                    break;
                case 10:
                    message("PV ", pvFullName, " is a DBR_STS_ENUM, connecting to channel");
                    r= connectPV(pvFullName, "DBR_ENUM");
                case 11:
                    message("PV ", pvFullName, " is a DBR_STS_CHAR, connecting to channel");
                    r= connectPV(pvFullName, "DBR_CHAR");
                    break;
                case 12:
                    message("PV ", pvFullName," is a DBR_STS_LONG, connecting to channel");
                    r= connectPV(pvFullName, "DBR_LONG");
                case 13:
                    message("PV ", pvFullName, " is a DBR_STS_DOUBLE, connecting to channel");
                    r= connectPV(pvFullName, "DBR_DOUBLE");
                    break;
                case 14:
                    message("PV ", pvFullName, " is a DBR_TIME_STRING, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_STRING");
                case 15:
                    message("PV ", pvFullName, " is a DBR_TIME_INT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_INT");
                    break;
                case 16:
                    message("PV ", pvFullName, " is a DBR_TIME_FLOAT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_FLOAT");
                    break;
                case 17:
                    message("PV ", pvFullName, " is a DBR_TIME_ENUM, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_ENUM");
                    break;
                case 18:
                    message("PV ", pvFullName, " is a DBR_TIME_CHAR, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_CHAR");
                    break;
                case 19:
                    message("PV ", pvFullName, " is a DBR_TIME_LONG, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_LONG");
                case 20:
                    message("PV ", pvFullName, " is a DBR_TIME_DOUBLE, connecting to channel");
                    r= connectPV(pvFullName, "DBR_TIME_DOUBLE");
                    break;
                case 21:
                    message("PV ", pvFullName," is a DBR_GR_STRING, connecting to channel");
                    r= connectPV(pvFullName, "DBR_STRING");
                case 22:
                    message("PV ", pvFullName, " is a DBR_GR_INT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_INT");
                    break;
                case 23:
                    message("PV ", pvFullName, " is a DBR_GR_FLOAT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_FLOAT");
                case 24:
                    message("PV ", pvFullName, " is a DBR_GR_ENUM, connecting to channel");
                    r= connectPV(pvFullName, "DBR_ENUM");
                    break;
                case 25:
                    message("PV ", pvFullName, " is a DBR_GR_CHAR, connecting to channel");
                    r= connectPV(pvFullName, "DBR_CHAR");
                    break;
                case 26:
                    message("PV ", pvFullName, " is a DBR_GR_LONG, connecting to channel");
                    r= connectPV(pvFullName, "DBR_LONG");
                    break;
                case 27:
                    message("PV ", pvFullName, " is a DBR_GR_DOUBLE, connecting to channel");
                    r= connectPV(pvFullName, "DBR_DOUBLE");
                    break;
                case 28:
                    message("PV ", pvFullName, " is a DBR_CTRL_STRING, connecting to channel");
                    r= connectPV(pvFullName, "DBR_STRING");
                    break;
                case 29:
                    message("PV ", pvFullName, " is a DBR_CTRL_INT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_INT");
                    break;
                case 30:
                    message("PV ", pvFullName," is a DBR_CTRL_FLOAT, connecting to channel");
                    r= connectPV(pvFullName, "DBR_FLOAT");
                case 31:
                    message("PV ", pvFullName, " is a DBR_CTRL_ENUM, connecting to channel");
                    r= connectPV(pvFullName, "DBR_ENUM");
                    break;
                case 32:
                    message("PV ", pvFullName, " is a DBR_CTRL_CHAR, connecting to channel");
                    r= connectPV(pvFullName, "DBR_CHAR");
                    break;
                case 33:
                    message("PV ", pvFullName, " is a DBR_CTRL_LONG, connecting to channel");
                    r= connectPV(pvFullName, "DBR_LONG");
                    break;
                case 34:
                    message("PV ", pvFullName, " is a DBR_CTRL_DOUBLE, connecting to channel");
                    r= connectPV(pvFullName,"DBR_DOUBLE");
                    break;
                default:
                    message("PV ", pvFullName," is an unrecognised EPICS Type.");
            }
        }
        else
            message("When looking for ", pvFullName," EPICS return  status != ECA_NORMAL");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // MAGIC_NUMBER

    return r;
}
//______________________________________________________________________________
std::string VCgeneralMonitor::connectPV(const std::string & pvFullName,const std::string & pvType)
{
    std::string returnvalue = returnFail;
    //chtype tempchtype;
    //std::string id;
    debugMessage("Getting pvType for ", pvType);
    gmStructs::pvStruct* pvs =  getCHTYPEandPrefix(pvType);
    if(pvs)
    {// we recognise the PV channel type
        pvs->pvFullName = pvFullName;
        pvs->COUNT= defaultCOUNT;
        pvs->MASK = defaultMASK;
        debugMessage(pvType, " found. Setting up channel with id ",pvs->id, " to, ", pvFullName);
        bool shouldCarryOn = setUpChannel(*pvs);
        if(shouldCarryOn)
        {
            if(pvs->isArrayPV)
            {
                shouldCarryOn = getandSetArraySize(*pvs);
            }
            if(shouldCarryOn)
            {
                shouldCarryOn = setupMonitor(*pvs);
                if(shouldCarryOn)
                {
                    returnvalue = pvs->id;
                }
            }
        }
    }
    if(returnvalue == returnFail)
    {
        message("connectPV FAILED");
        disconnectPV(pvs->id);
    }
    else
    {
        ++pvMonitorMapCount;
    }
    return returnvalue;
}
//______________________________________________________________________________
void VCgeneralMonitor::staticEntryMonitor(const event_handler_args args)
{
    gmStructs::monitorStruct* ms = static_cast<gmStructs::monitorStruct*>(args.usr);
    //std::cout<<"updateValue called " << std::endl;
    ms->gm->updateValue( ms->id, args);
}
//______________________________________________________________________________
void VCgeneralMonitor::addSingleDouble(const std::string& id,
                                       const event_handler_args& args)
{   message("addSingleDouble called");
    if( doublePVMap[id].data.size() == 0)
    {
        doublePVMap[id].data.push_back(new gmStructs::dataEntry<double>());
    }
    doublePVMap[id].data[0]->v = *(double*)args.dbr;// MAGIC_NUMBER
    doublePVMap[id].data[0]->c += 1;// MAGIC_NUMBER



    if(doublePVMap[id].data[0]->rs_count < doublePVMap[id].data[0]->rs_count_max)
    {
        doublePVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER


        std::pair<std::string,double> new_pair;
        doublePVMap.at(id).data[0]->buffer.push_back(new_pair);
        doublePVMap.at(id).data[0]->buffer.back().first = getLocalTime();
        doublePVMap.at(id).data[0]->buffer.back().second = *(double*)args.dbr;

        doublePVMap[id].data[0]->rs.Push(doublePVMap[id].data[0]->v);
        if(doublePVMap[id].data[0]->rs_count ==  doublePVMap[id].data[0]->rs_count_max)
        {
            doublePVMap[id].data[0]->rs_complete = true;
        }
        else
        {
            doublePVMap[id].data[0]->rs_complete = false;
        }
    }
    message("DOUBLE");
}
//______________________________________________________________________________
void VCgeneralMonitor::addArrayDouble(const std::string & id,
                                      const event_handler_args& args)
{   //message(vec_doublePVMap[id].data.size());
    if( vec_doublePVMap[id].data.size() == 0)
    {
        vec_doublePVMap[id].data.push_back(new gmStructs::dataEntry<std::vector<double>>());
        //message(vec_doublePVMap[id].data.size());
        //message("resize to output array to  ", vec_doublePVMap[id].pvs.COUNT);
        vec_doublePVMap[id].data[0]->v.resize(vec_doublePVMap[id].pvs.COUNT);
    }
    size_t counter = 0;
    for( auto && it : vec_doublePVMap[id].data[0]->v)
    {
        it = *( (double*) args.dbr + counter);
        ++counter;
    }
    vec_doublePVMap[id].data[0]->c += 1;// MAGIC_NUMBER

    if(vec_doublePVMap[id].data[0]->rs_count < vec_doublePVMap[id].data[0]->rs_count_max)
    {
        vec_doublePVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
        std::pair<std::string,std::vector<double>> new_pair;
        vec_doublePVMap.at(id).data[0]->buffer.push_back(new_pair);
        vec_doublePVMap.at(id).data[0]->buffer.back().first = getLocalTime();
        vec_doublePVMap.at(id).data[0]->buffer.back().second = vec_doublePVMap[id].data[0]->v;

        if(vec_doublePVMap[id].data[0]->rs_count ==  vec_doublePVMap[id].data[0]->rs_count_max)
        {
            vec_doublePVMap[id].data[0]->rs_complete = true;
        }
        else
        {
            vec_doublePVMap[id].data[0]->rs_complete = false;
        }
    //std::cout<< "buffer[0].first = " << vec_doublePVMap.at(id).data[0]->buffer[0].first << std::endl;
    }
    //std::cout<< "buffer count = " << vec_doublePVMap[id].data[0]->rs_count << std::endl;
    //std::cout<< "buffer size  = " << vec_doublePVMap.at(id).data[0]->buffer.size() << std::endl;
}
//______________________________________________________________________________
void VCgeneralMonitor::addArrayInt(const std::string & id,const event_handler_args& args)
{   //message(vec_intPVMap[id].data.size());
    if( vec_intPVMap[id].data.size() == 0)
    {
        vec_intPVMap[id].data.push_back(new gmStructs::dataEntry<std::vector<int>>());
        //message(vec_intPVMap[id].data.size());
        //message("resize to output array to  ", vec_intPVMap[id].pvs.COUNT);
        vec_intPVMap[id].data[0]->v.resize(vec_intPVMap[id].pvs.COUNT);
    }
    size_t counter = 0;
    for( auto && it : vec_intPVMap[id].data[0]->v)
    {
        it = *( (int*) args.dbr + counter);
        ++counter;
    }
    vec_intPVMap[id].data[0]->c += 1;// MAGIC_NUMBER
    if(vec_intPVMap[id].data[0]->rs_count < vec_intPVMap[id].data[0]->rs_count_max)
    {
        vec_intPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
        std::pair<std::string,std::vector<int>> new_pair;
        vec_intPVMap.at(id).data[0]->buffer.push_back(new_pair);
        vec_intPVMap.at(id).data[0]->buffer.back().first = getLocalTime();
        vec_intPVMap.at(id).data[0]->buffer.back().second = vec_intPVMap[id].data[0]->v;

        if(vec_intPVMap[id].data[0]->rs_count ==  vec_intPVMap[id].data[0]->rs_count_max)
        {
            vec_intPVMap[id].data[0]->rs_complete = true;
        }
        else
        {
            vec_intPVMap[id].data[0]->rs_complete = false;
        }
    }

}
//______________________________________________________________________________
void VCgeneralMonitor::updateValue(const std::string & id,const event_handler_args& args)
{
    if(args.status != ECA_NORMAL)
    {
        std::cout<<"please god, no, never show this... "<<std::endl;
    }
    else
    {
        switch(args.type)
        {
            case DBR_TIME_STRING:
                updateTimeAndValue(id, args.dbr);
                break;
            case DBR_TIME_ENUM:
                updateTimeAndValue(id, args.dbr);
                break;
            case DBR_TIME_LONG:
                updateTimeAndValue(id, args.dbr);
                break;
            case DBR_TIME_DOUBLE:
                updateTimeAndValue(id, args.dbr);
                break;
            case  DBR_STRING:
                {
                    if(stringPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
                    {
                        stringPVMap.at(id).data.push_back(new gmStructs::dataEntry<std::string>());
                    }
                    std::string temp((const char *)args.dbr);
                    //message("temp = ", temp);
                    //stringPVMap.at(id).data[0]->v = *(std::string*)args.dbr;// meh
                    stringPVMap.at(id).data[0]->v = temp;// meh
                    stringPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
                 if(stringPVMap[id].data[0]->rs_count < stringPVMap[id].data[0]->rs_count_max)
                {
                    stringPVMap[id].data[0]->rs_count += 1;


            std::pair<std::string,std::string> new_pair;
            stringPVMap.at(id).data[0]->buffer.push_back(new_pair);
            stringPVMap.at(id).data[0]->buffer.back().first = getLocalTime();
            stringPVMap.at(id).data[0]->buffer.back().second = stringPVMap.at(id).data[0]->v;
                        if(stringPVMap.at(id).data[0]->buffer.size() ==  stringPVMap[id].data[0]->rs_count_max)
                        {
                            stringPVMap[id].data[0]->rs_complete = true;
                        }
                        else
                        {
                            stringPVMap[id].data[0]->rs_complete = false;
                        }
                    }
                }
                break;
            case  DBR_TIME_SHORT:
                updateTimeAndValue(id, args.dbr);
                break;
            case  DBR_DOUBLE:
                if( args.count == 1)
                {
                    addSingleDouble(id,args);
                }
                else if( args.count > 0)
                {
                    addArrayDouble(id,args);
                }
                break;
            case  DBR_INT:
                if( args.count == 1)
                {
                    if( intPVMap[id].data.size() == 0)
                    {
                        intPVMap[id].data.push_back(new gmStructs::dataEntry<int>());
                    }
                    intPVMap[id].data[0]->v = *(int*)args.dbr;// MAGIC_NUMBER
                    intPVMap[id].data[0]->c += 1;// MAGIC_NUMBER
                    intPVMap[id].data[0]->rs.Push((double)intPVMap[id].data[0]->v);


                    if(intPVMap[id].data[0]->rs_count <= intPVMap[id].data[0]->rs_count_max)
                    {
                    intPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER

            std::pair<std::string,int> new_pair;
            intPVMap.at(id).data[0]->buffer.push_back(new_pair);
            intPVMap.at(id).data[0]->buffer.back().first = getLocalTime();
            intPVMap.at(id).data[0]->buffer.back().second = *(int*)args.dbr;
                        intPVMap[id].data[0]->rs.Push(intPVMap[id].data[0]->v);
                        if(intPVMap[id].data[0]->rs_count ==  intPVMap[id].data[0]->rs_count_max)
                        {
                            intPVMap[id].data[0]->rs_complete = true;
                        }
                        else
                        {
                            intPVMap[id].data[0]->rs_complete = false;
                        }
                    }

                }
                else if( args.count > 0)
                {
                    addArrayInt(id,args);
                }
                break;
            case DBR_ENUM:
                //message("VCgeneralMonitor::updateValue() DBR_ENUM");
                if( enumPVMap[id].data.size() == 0)
                {
                    enumPVMap[id].data.push_back(new gmStructs::dataEntry<unsigned short>());
                }
                enumPVMap[id].data[0]->v = *(unsigned short*)args.dbr;// MAGIC_NUMBER
                enumPVMap[id].data[0]->c += 1;// MAGIC_NUMBER
                enumPVMap[id].data[0]->rs.Push((double)enumPVMap[id].data[0]->v);
                if(enumPVMap[id].data[0]->rs_count <= enumPVMap[id].data[0]->rs_count_max)
                {
                enumPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            std::pair<std::string,unsigned short> new_pair;
            enumPVMap.at(id).data[0]->buffer.push_back(new_pair);
            enumPVMap.at(id).data[0]->buffer.back().first = getLocalTime();
            enumPVMap.at(id).data[0]->buffer.back().second = *(unsigned short*)args.dbr;

                    enumPVMap[id].data[0]->rs.Push(enumPVMap[id].data[0]->v);
                    if(enumPVMap[id].data[0]->rs_count ==  enumPVMap[id].data[0]->rs_count_max)
                    {
                        enumPVMap[id].data[0]->rs_complete = true;
                    }
                    else
                    {
                        enumPVMap[id].data[0]->rs_complete = false;
                    }
                }

                break;
            case DBR_LONG:
                if( longPVMap[id].data.size() == 0)
                {
                    longPVMap[id].data.push_back(new gmStructs::dataEntry<long>());
                }
                longPVMap[id].data[0]->v = *(long*)args.dbr;// MAGIC_NUMBER
                longPVMap[id].data[0]->c += 1;// MAGIC_NUMBER
                longPVMap[id].data[0]->rs.Push((double)longPVMap[id].data[0]->v);
                if(longPVMap[id].data[0]->rs_count <= longPVMap[id].data[0]->rs_count_max)
                {
                longPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            std::pair<std::string,long> new_pair;
            longPVMap.at(id).data[0]->buffer.push_back(new_pair);
            longPVMap.at(id).data[0]->buffer.back().first = getLocalTime();
            longPVMap.at(id).data[0]->buffer.back().second = *(unsigned short*)args.dbr;

                    longPVMap[id].data[0]->rs.Push(longPVMap[id].data[0]->v);
                    if(longPVMap[id].data[0]->rs_count ==  longPVMap[id].data[0]->rs_count_max)
                    {
                        longPVMap[id].data[0]->rs_complete = true;
                    }
                    else
                    {
                        longPVMap[id].data[0]->rs_complete = false;
                    }
                }

                break;

            default:
                message("VCgeneralMonitor::updateValue() default switch, ", args.type);
        }
    }
}
//______________________________________________________________________________
//______________________________________________________________________________
void VCgeneralMonitor::updateTimeAndValue(const std::string & id,const  void * dbr)
{
    message("updateTimeAndValue");
    if(isStringPV(id))
    {
        // see db_access.h
        const dbr_time_string * p = (const struct dbr_time_string*)dbr;
        if( stringPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            stringPVMap.at(id).data.push_back(new gmStructs::dataEntry<std::string>());
        }
        stringPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        stringPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        stringPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        updateTime_ns(stringPVMap.at(id).data[0]->t, stringPVMap.at(id).data[0]->s);
        //
        // buffering
        if( stringPVMap.at(id).data[0]->buffer.size() < stringPVMap.at(id).data[0]->rs_count_max )
        {
            std::pair<std::string,std::string> new_pair;
            stringPVMap.at(id).data[0]->buffer.push_back(new_pair);

            updateTime_str(p->stamp , stringPVMap.at(id).data[0]->buffer.back().first );
            stringPVMap.at(id).data[0]->buffer.back().second = p->value;
            stringPVMap.at(id).data[0]->rs_complete = false;
        }
        else if(stringPVMap.at(id).data[0]->buffer.size() == stringPVMap.at(id).data[0]->rs_count_max)
        {
            stringPVMap.at(id).data[0]->rs_complete = true;
        }

        //message("stringPVMap.at(id).data[0]->v = ", stringPVMap.at(id).data[0]->v);
    }
    else if(isIntPV(id))
    {
        // see db_access.h
        const dbr_time_short * p = (const struct dbr_time_short*)dbr;
        if( intPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            intPVMap.at(id).data.push_back(new gmStructs::dataEntry<int>());
        }
        intPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        intPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        intPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        intPVMap[id].data[0]->rs.Push((double)intPVMap[id].data[0]->v);
        if(intPVMap[id].data[0]->rs_count <= intPVMap[id].data[0]->rs_count_max)
        {

            std::pair<std::string,int> new_pair;
            intPVMap.at(id).data[0]->buffer.push_back(new_pair);
            updateTime_str(p->stamp , intPVMap.at(id).data[0]->buffer.back().first );
            intPVMap.at(id).data[0]->buffer.back().second = p->value;
        intPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER

            intPVMap[id].data[0]->rs.Push(intPVMap[id].data[0]->v);
            if(intPVMap[id].data[0]->rs_count ==  intPVMap[id].data[0]->rs_count_max)
            {
                intPVMap[id].data[0]->rs_complete = true;
            }
            else
            {
                intPVMap[id].data[0]->rs_complete = false;
            }
        }
        updateTime_ns(intPVMap.at(id).data[0]->t, intPVMap.at(id).data[0]->s);
    }
    else if(isArrayIntPV(id))
    {
        if( vec_intPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            vec_intPVMap.at(id).data.push_back(new gmStructs::dataEntry<std::vector<int>>());
        }
        const dbr_time_short * p = (const struct dbr_time_short*)dbr;
        size_t counter = 0;
        for( auto && it : vec_intPVMap[id].data[0]->v)
        {
            it = *( (int*) p->value + counter);
            ++counter;
        }
        vec_intPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        vec_intPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        updateTime_ns(vec_intPVMap.at(id).data[0]->t, vec_intPVMap.at(id).data[0]->s);

    }
    else if(isFloatPV(id))
    {
        // see db_access.h
        const dbr_time_float * p = (const struct dbr_time_float*)dbr;
        if( floatPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            floatPVMap.at(id).data.push_back( new gmStructs::dataEntry<float>());
        }
        floatPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        floatPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        floatPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        if(floatPVMap[id].data[0]->rs_count < floatPVMap[id].data[0]->rs_count_max)
        {
           floatPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            floatPVMap[id].data[0]->rs.Push(floatPVMap[id].data[0]->v);

            std::pair<std::string,double> new_pair;
            floatPVMap.at(id).data[0]->buffer.push_back(new_pair);
            updateTime_str(p->stamp , floatPVMap.at(id).data[0]->buffer.back().first );
            floatPVMap.at(id).data[0]->buffer.back().second = p->value;

            if(floatPVMap[id].data[0]->rs_count ==  floatPVMap[id].data[0]->rs_count_max)
            {
                floatPVMap[id].data[0]->rs_complete = true;
            }
            else
            {
                floatPVMap[id].data[0]->rs_complete = false;
            }
        }
        updateTime_ns(floatPVMap.at(id).data[0]->t, floatPVMap.at(id).data[0]->s);

    }
    else if(isEnumPV(id))
    {
        // see db_access.h
        const dbr_time_enum * p = (const struct dbr_time_enum*)dbr;
        if( enumPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            enumPVMap.at(id).data.push_back( new gmStructs::dataEntry<unsigned short>());
        }
        enumPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        enumPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        enumPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        enumPVMap[id].data[0]->rs.Push((double)enumPVMap[id].data[0]->v);
        if(enumPVMap[id].data[0]->rs_count < enumPVMap[id].data[0]->rs_count_max)
        {
        enumPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            std::pair<std::string,unsigned short> new_pair;
            enumPVMap.at(id).data[0]->buffer.push_back(new_pair);
            updateTime_str(p->stamp , enumPVMap.at(id).data[0]->buffer.back().first );
            enumPVMap.at(id).data[0]->buffer.back().second = p->value;


            enumPVMap[id].data[0]->rs.Push(enumPVMap[id].data[0]->v);
            if(enumPVMap[id].data[0]->rs_count ==  enumPVMap[id].data[0]->rs_count_max)
            {
                enumPVMap[id].data[0]->rs_complete = true;
            }
            else
            {
                enumPVMap[id].data[0]->rs_complete = false;
            }
        }
        updateTime_ns(enumPVMap.at(id).data[0]->t, enumPVMap.at(id).data[0]->s);

    }
    else if(isCharPV(id))
    {
        // see db_access.h
        const dbr_time_char * p = (const struct dbr_time_char*)dbr;
        if( charPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            charPVMap.at(id).data.push_back( new gmStructs::dataEntry<char>());
        }
        charPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        charPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        charPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER

        charPVMap[id].data[0]->rs.Push((double)charPVMap[id].data[0]->v);
        if(charPVMap[id].data[0]->rs_count < charPVMap[id].data[0]->rs_count_max)
        {
            charPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            std::pair<std::string,char> new_pair;
            charPVMap.at(id).data[0]->buffer.push_back(new_pair);
            updateTime_str(p->stamp , charPVMap.at(id).data[0]->buffer.back().first );
            charPVMap.at(id).data[0]->buffer.back().second = p->value;

            charPVMap[id].data[0]->rs.Push(charPVMap[id].data[0]->v);
            if(charPVMap[id].data[0]->rs_count ==  charPVMap[id].data[0]->rs_count_max)
            {
                charPVMap[id].data[0]->rs_complete = true;
            }
            else
            {
                charPVMap[id].data[0]->rs_complete = false;
            }
        }

        updateTime_ns(charPVMap.at(id).data[0]->t, charPVMap.at(id).data[0]->s);

    }
    else if(isLongPV(id))
    {
        // see db_access.h
        const dbr_time_long * p = (const struct dbr_time_long*)dbr;
        if( longPVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            longPVMap.at(id).data.push_back( new gmStructs::dataEntry<long>());
        }
        longPVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        longPVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        longPVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        longPVMap[id].data[0]->rs.Push((double)longPVMap[id].data[0]->v);
        if(longPVMap[id].data[0]->rs_count < longPVMap[id].data[0]->rs_count_max)
        {
            longPVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            std::pair<std::string,long> new_pair;
            longPVMap.at(id).data[0]->buffer.push_back(new_pair);
            updateTime_str(p->stamp , longPVMap.at(id).data[0]->buffer.back().first );
            longPVMap.at(id).data[0]->buffer.back().second = p->value;

            longPVMap[id].data[0]->rs.Push(longPVMap[id].data[0]->v);
            if(longPVMap[id].data[0]->rs_count ==  longPVMap[id].data[0]->rs_count_max)
            {
                longPVMap[id].data[0]->rs_complete = true;
            }
            else
            {
                longPVMap[id].data[0]->rs_complete = false;
            }
        }
        updateTime_ns(longPVMap.at(id).data[0]->t, longPVMap.at(id).data[0]->s);

    }
    else if(isDoublePV(id))
    {
        // see db_access.h
        const dbr_time_double* p = (const struct dbr_time_double*)dbr;
        if( doublePVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            doublePVMap.at(id).data.push_back( new gmStructs::dataEntry<double>());
        }
        doublePVMap.at(id).data[0]->v = p->value;// MAGIC_NUMBER
        doublePVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        doublePVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        doublePVMap[id].data[0]->rs.Push((double)doublePVMap[id].data[0]->v);
        if(doublePVMap[id].data[0]->rs_count < doublePVMap[id].data[0]->rs_count_max)
        {
            doublePVMap[id].data[0]->rs_count += 1;// MAGIC_NUMBER
            std::pair<std::string,double> new_pair;
            doublePVMap.at(id).data[0]->buffer.push_back(new_pair);
            updateTime_str(p->stamp , doublePVMap.at(id).data[0]->buffer.back().first );
            doublePVMap.at(id).data[0]->buffer.back().second = p->value;

            doublePVMap[id].data[0]->rs.Push(doublePVMap[id].data[0]->v);
            if(doublePVMap[id].data[0]->rs_count ==  doublePVMap[id].data[0]->rs_count_max)
            {
                doublePVMap[id].data[0]->rs_complete = true;
            }
            else
            {
                doublePVMap[id].data[0]->rs_complete = false;
            }
        }
        updateTime_ns(doublePVMap.at(id).data[0]->t, doublePVMap.at(id).data[0]->s);
//        debugMessage("updated, value = ", doublePVMap.at(id).data[0]->v);// MAGIC_NUMBER
//        printTimeStamp( doublePVMap.at(id).data[0]->t);
    }
    else if(isArrayDoublePV(id))
    {
        if( vec_doublePVMap.at(id).data.size() == 0)// MAGIC_NUMBER
        {
            vec_doublePVMap.at(id).data.push_back(new gmStructs::dataEntry<std::vector<double>>());
        }
        const dbr_time_double* p = (const struct dbr_time_double*)dbr;
        size_t counter = 0;
        const dbr_double_t * pValue;
        pValue = &p->value;
        for(auto && it : vec_doublePVMap[id].data[0]->v)
        {
            //it = *((double const* const) p->value + counter);
            it = pValue[counter];
            ++counter;
        }
        vec_doublePVMap.at(id).data[0]->t = p->stamp;// MAGIC_NUMBER
        vec_doublePVMap.at(id).data[0]->c += 1;// MAGIC_NUMBER
        updateTime_ns(vec_doublePVMap.at(id).data[0]->t, vec_doublePVMap.at(id).data[0]->s);
    }
}
//______________________________________________________________________________
void VCgeneralMonitor::updateTime_ns(const epicsTimeStamp& stamp, double& s)
{
    s =  ((double)stamp.nsec * 0.000000001) + (double)stamp.secPastEpoch;//MAGIC_NUMBER
}
//______________________________________________________________________________
void VCgeneralMonitor::updateTime_str(const epicsTimeStamp& stamp, std::string& str)
{
    /* for hints look in epicsTime.h */
    char timeString[UTL::BUFFER_36];
    epicsTimeToStrftime(timeString, sizeof(timeString),"%a %b %d %Y %H:%M:%S.%f", &stamp);
    str = timeString;
}
//______________________________________________________________________________
//______________________________________________________________________________
void VCgeneralMonitor::printTimeStamp( const epicsTimeStamp & stamp)
{// look in epicsTime.h
//    const dbr_time_char * pTD = ( const struct dbr_time_char *) args.dbr;
//    epicsUInt32 sec  = pTD ->stamp.secPastEpoch;   /* seconds since 0000 Jan 1, 1990 */
//    epicsUInt32 nsec = pTD ->stamp.nsec;
    char timeString[36];//MAGIC_NUMBER
    epicsTimeToStrftime ( timeString, sizeof ( timeString), "%a %b %d %Y %H:%M:%S.%f", &stamp);
//    const dbr_char_t * pValue = &pTD -> value;
    double val =  ((double)stamp.nsec * 0.000000001) + (double)stamp.secPastEpoch;//MAGIC_NUMBER
    std::cout << std::setprecision(15) << std::showpoint<<  val << std::endl;
    std::string str = timeString;
    debugMessage("time string = ",str);// MAGIC_NUMBER
    debugMessage("time double = ",val);// MAGIC_NUMBER
    std::cout << std::setprecision(4) << std::showpoint<<   val << std::endl;
}
//______________________________________________________________________________
bool VCgeneralMonitor::setUpChannel(gmStructs::pvStruct& pvs)
{
    debugMessage("setUpChannel is connecting ", pvs.id, "  to ", pvs.pvFullName);
    ca_create_channel(pvs.pvFullName.c_str(), 0, 0, 0,&pvs.CHID);
    int status = sendToEpics(ca_create_channel_str,ca_chid_successmess,ca_chid_failuremess);
    if( status == ECA_TIMEOUT)
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL)
    {
        debugMessage(pvs.id, " channel created.");
        pvs.CHIDConnected = true;
    }
    return pvs.CHIDConnected;
}
//______________________________________________________________________________
bool VCgeneralMonitor::setupMonitor(gmStructs::pvStruct& pvs)
{
    continuousMonitorStructs.push_back( new gmStructs::monitorStruct());
    continuousMonitorStructs.back()->gm = this;

    message("setupMonitor is Setting up Monitor with id = ", pvs.id);
    continuousMonitorStructs.back()->id = pvs.id;

    ca_create_subscription(pvs.CHTYPE, pvs.COUNT,pvs.CHID, pvs.MASK,
                           VCgeneralMonitor::staticEntryMonitor,
                           (void*)continuousMonitorStructs.back(),
                           &pvs.EVID);

    int status = sendToEpics(ca_create_subscription_str,ca_subs_successmess,ca_subs_failuremess);
    if(status == ECA_TIMEOUT)
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL)
    {
        pvs.MonitorConnected = true;
        debugMessage( continuousMonitorStructs.back()->id , " subscription successfull.");

    }
    return pvs.MonitorConnected;
}
//______________________________________________________________________________
bool VCgeneralMonitor::getandSetArraySize(gmStructs::pvStruct& pvs)
{
    pvs.COUNT = ca_element_count(pvs.CHID);
    std::cout << "pvs.COUNT =  " << pvs.COUNT << std::endl;
    return true;
}
//______________________________________________________________________________
int VCgeneralMonitor::sendToEpics(const std::string& ca,const std::string& mess1,const std::string& mess2)
{
    int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
    SEVCHK(status, ca.c_str());
    printStatusResult(status,mess1.c_str(),mess2.c_str());
    return status;
}
//______________________________________________________________________________
void VCgeneralMonitor::printStatusResult( const int status, const char * success, const char* timeout)
{
    switch (status)
    {
        case ECA_NORMAL:
            if( strlen( success) != 0)//MAGIC_NUMBER
                debugMessage( success);
            break;
        case ECA_TIMEOUT:
            if( strlen( timeout) != 0)//MAGIC_NUMBER
                message( timeout);
            break;
        default:
            message("!!! Unexpected error while searching: ", ca_message( status));
    }
}
//______________________________________________________________________________
//gmStructs::pvStruct* VCgeneralMonitor::getCHTYPEandPrefix(const std::string & pvFullName,const std::string & pvType, bool & success)
gmStructs::pvStruct* VCgeneralMonitor::getCHTYPEandPrefix(const std::string & pvType)
{    // cancer

    gmStructs::pvStruct* r = nullptr;
    if(pvType == UTL::DBR_INT_STR)
    {
        r =  addToIntPVMap();
    }
    else if(pvType == UTL::DBR_SHORT_STR)
    {
        r = addToIntPVMap();// short is an INT (i hope)
        // "In practice, both the DBR_INT type code and the DBR_SHORT type code"
        // "refer to a 16 bit integer type, and are functionally equivalent."
    }
    else if(pvType == UTL::DBR_FLOAT_STR)
    {
        r = addToFloatPVMap();
    }
    else if(pvType == UTL::DBR_ENUM_STR)
    {
        r = addToEnumPVMap();
    }
    else if(pvType == UTL::DBR_CHAR_STR)
    {
        r = addToCharPVMap();
    }
//    else if(pvType == UTL::DBR_INT_STR)
//    {
//        success = true;
//        r = DBR_TIME_INT;
//        id = intPrefix;
//    }
    else if(pvType == UTL::DBR_LONG_STR)
    {
        r = addToLongPVMap();
    }
    else if(pvType == UTL::DBR_DOUBLE_STR)
    {
        r = addToDoublePVMap();
    }
    else if(pvType == UTL::DBR_ARRAY_DOUBLE_STR)
    {
        r  = addToVecDoublePVMap();
    }
    else if(pvType == UTL::DBR_TIME_ARRAY_DOUBLE_STR)
    {
        r  = addToVecDoublePVMap();
    }
    else if(pvType == UTL::DBR_STRING_STR)
    {
        r  = addToStringPVMap();
    }
    // the rest should probably not be used...
    else if(pvType == UTL::DBR_TIME_STRING_STR)
    {
        r = addToStringPVMap();
    }
    else if(pvType == UTL::DBR_TIME_INT_STR)
    {
        r = addToIntPVMap();
    }
    else if(pvType == UTL::DBR_TIME_SHORT_STR)
    {
        r = addToIntPVMap();
    }
    else if(pvType == UTL::DBR_TIME_FLOAT_STR)
    {
        r = addToFloatPVMap();
    }
    else if(pvType == UTL::DBR_TIME_ENUM_STR)
    {
        r = addToEnumPVMap();
    }
    else if(pvType == UTL::DBR_TIME_CHAR_STR)
    {
        r = addToCharPVMap();
    }
    else if(pvType == UTL::DBR_TIME_LONG_STR)
    {
        r = addToLongPVMap();
    }
    else if(pvType == UTL::DBR_TIME_DOUBLE_STR)
    {
        r = addToDoublePVMap();
    }
    return r;
}
//______________________________________________________________________________
bool VCgeneralMonitor::disconnectPV(const std::string & id)
{
    gmStructs::pvStruct* pvs = getPVStruct_Ptr(id);
    if( pvs)
    {
        if(pvs->MonitorConnected)
        {
            killMonitor(*pvs);
        }
        if(pvs->CHIDConnected)
        {
            killChannel(*pvs);
        }
        return deleteID(id);
    }
    return false;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::getPVStruct_Ptr(const std::string& id)
{
    gmStructs::pvStruct* pvs = nullptr;
    if(isStringPV(id))
    {   message("getPVStruct ",  id, " isStringPV");
        if( entryExists(stringPVMap, id))
        {
            pvs = &stringPVMap[id].pvs;
            message("getPVStruct found ",  id, " in stringPVMap");
        }
    }
    if(isIntPV(id))
    {   //message("getPVStruct ",  id, " isIntPV");
        if( entryExists(intPVMap, id))
        {
            pvs = &intPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in intPVMap");
        }
    }
    else if(isFloatPV(id))
    {   //message("getPVStruct ",  id, " isFloatPV");
        if( entryExists(floatPVMap, id))
        {
            pvs = &floatPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in floatPVMap");
        }
    }
    else if(isEnumPV(id))
    {   //message("getPVStruct ",  id, " isEnumPV");
        if( entryExists(enumPVMap, id))
        {
            pvs = &enumPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in enumPVMap");
        }
    }
    else if(isCharPV(id))
    {//message("getPVStruct ",  id, " isCharPV");
        if( entryExists(charPVMap, id))
        {
            pvs = &charPVMap[id].pvs;
            //message("getPVStruct found ",  id, " in charPVMap");
        }
    }
    else if(isLongPV(id))
    {//message("getPVStruct ",  id, " isLongPV");
        if( entryExists(longPVMap, id))
        {
            pvs = &longPVMap[id].pvs;
//            message("getPVStruct found ",  id, " in longPVMap");
        }
    }
    else if(isDoublePV(id))
    {//message("getPVStruct ",  id, " isDoublePV");
        if( entryExists(doublePVMap, id))
        {
            pvs = &doublePVMap[id].pvs;
//            message("getPVStruct found ",  id, " in doublePVMap");
        }
    }
    else if(isArrayDoublePV(id))
    {//message("getPVStruct ",  id, " isDoublePV");
        if( entryExists(doublePVMap, id))
        {
            pvs = &vec_doublePVMap[id].pvs;
//            message("getPVStruct found ",  id, " in doublePVMap");
        }
    }
    else if(isArrayDoublePV(id))
    {//message("getPVStruct ",  id, " isDoublePV");
        if( entryExists(doublePVMap, id))
        {
            pvs = &vec_doublePVMap[id].pvs;
//            message("getPVStruct found ",  id, " in doublePVMap");
        }
    }
    if(pvs)
    {
        debugMessage("getPVStruct found ", id);
    }
    else
    {
        message("getPVStruct failed to find ", id);
    }
    return pvs;
}
//______________________________________________________________________________
void VCgeneralMonitor::killMonitor(gmStructs::pvStruct& pvs)
{
    ca_clear_subscription( pvs.EVID);
    int status = sendToEpics(ca_clear_subscription_str,ca_csub_successmess,ca_csub_failuremess);
    if( status == ECA_TIMEOUT)
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL)
    {
        pvs.MonitorConnected = false;
        debugMessage(pvs.id, " killMonitor successfull.");
    }
    else
    {
         message(pvs.id, " killMonitor un-successfull.");
    }
}
//______________________________________________________________________________
void VCgeneralMonitor::killChannel(gmStructs::pvStruct& pvs)
{
    ca_clear_channel( pvs.CHID);
    int status = sendToEpics(ca_clear_channel_str,ca_cchi_successmess,ca_cchi_failuremess);
    if( status == ECA_TIMEOUT)
    {
        //removeChanel();
    }
    else if( status == ECA_NORMAL)
    {
        pvs.CHIDConnected = false;
        debugMessage(pvs.id, " killChannel successfull.");
    }
    else
    {
         message(pvs.id, " killChannel un-successfull.");
    }

}
//______________________________________________________________________________
template< class T >
bool VCgeneralMonitor::entryExists( std::map< std::string, T > & m, const std::string & name)
{
    bool ret = false;
    auto it = m.find( name);
        if( it != m.end())
            ret = true;
    return ret;
}
//______________________________________________________________________________
template< class T >
bool VCgeneralMonitor::deleteMapEntry(std::map<std::string, T>& map, const std::string& id)
{
    bool r = false;
    std::map<std::string, T >::iterator it = map.find(id) ;
    if( it != map.end())
    {
        map.erase( it);
        debugMessage(id," deleted");
        r = true;
    }
    else
        message("deleteMapEntry could not find id = ", id);
    return r;
}
//______________________________________________________________________________
bool VCgeneralMonitor::deleteID(const std::string& id)
{
    bool r = false;
    if(isIntPV(id))
    {
        r = deleteMapEntry(intPVMap,id);
    }
    else if(isFloatPV(id))
    {
        r = deleteMapEntry(floatPVMap,id);
    }
    else if(isEnumPV(id))
    {
        r = deleteMapEntry(enumPVMap,id);
    }
    else if(isCharPV(id))
    {
        r = deleteMapEntry(charPVMap,id);
    }
    else if(isLongPV(id))
    {
        r = deleteMapEntry(longPVMap,id);
    }
    else if(isDoublePV(id))
    {
        r = deleteMapEntry(doublePVMap,id);
    }
    else if(isStringPV(id))
    {
        r = deleteMapEntry(stringPVMap,id);
    }
    return r;
}
//______________________________________________________________________________
void VCgeneralMonitor::updateTime(const epicsTimeStamp& stamp, double& val, std::string& str)
{// look in epicsTime.h
//    const dbr_time_char * pTD = ( const struct dbr_time_char *) args.dbr;
//    epicsUInt32 sec  = pTD ->stamp.secPastEpoch;   /* seconds since 0000 Jan 1, 1990 */
//    epicsUInt32 nsec = pTD ->stamp.nsec;
    char timeString[36];//MAGIC_NUMBER
    epicsTimeToStrftime ( timeString, sizeof ( timeString), "%a %b %d %Y %H:%M:%S.%f", &stamp);
//    const dbr_char_t * pValue = &pTD -> value;
    val =  ( (double)stamp.nsec * 0.000000001) + (double)stamp.secPastEpoch;//MAGIC_NUMBER
    // prove it works
    //std::cout << std::setprecision(15) << std::showpoint<<   val << std::endl;
    str = timeString;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isStringPV(const std::string& id)
{
    return id.substr(0,1) == stringPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isIntPV(const std::string& id)
{
    return id.substr(0,1) == intPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isFloatPV(const std::string& id)
{
    return id.substr(0,1) == floatPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isEnumPV(const std::string& id)
{
    return id.substr(0,1) == enumPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isCharPV(const std::string& id)
{
    return id.substr(0,1) == charPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isLongPV(const std::string& id)
{
    return id.substr(0,1) == longPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isDoublePV(const std::string& id)
{
    return id.substr(0,1) == doublePrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isArrayDoublePV(const std::string & id)
{
    return id.substr(0,1) == vecdoublePrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isArrayIntPV(const std::string & id)
{
    return id.substr(0,1) == vecintPrefix  ? true : false;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isArrayPV(const std::string& id)
{
    bool r = false;
    if(isArrayDoublePV(id))
    {
        r = true;
    }
    else if(isArrayIntPV(id))
    {
        r = true;
    }
    return r;
}
//______________________________________________________________________________
bool VCgeneralMonitor::isConnected(const std::string & id)
{
    gmStructs::pvStruct* pvs = getPVStruct_Ptr(id);
    if( pvs)
    {
        debugMessage("isConnected found ", id);
        return pvs->CHIDConnected;
    }
    else
    {
        message("isConnected did not find ", id);
        return false;
    }
}
//______________________________________________________________________________
bool VCgeneralMonitor::isMonitoring(const std::string & id)
{
    gmStructs::pvStruct* pvs = getPVStruct_Ptr(id);
    if(pvs)
    {
        debugMessage("isMonitoring found ", id);
        return pvs->MonitorConnected;
    }
    else
    {
        message("isMonitoring did not find ", id);
        return false;
    }
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToStringPVMap()
{
    std::string id = stringPrefix;
    id += std::to_string(pvMonitorMapCount);
    stringPVMap[id].id = id;
    stringPVMap.at(id).pvs.id = id;
    stringPVMap.at(id).pvs.CHTYPE = DBR_TIME_STRING;
    debugMessage("connectPV Passed a DBR_STRING. Entry with id =  ",id, " created");
    return &stringPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToIntPVMap()
{
    std::string id = intPrefix;
    id += std::to_string(pvMonitorMapCount);
    intPVMap[id].id = id;
    intPVMap.at(id).pvs.id = id;
    intPVMap.at(id).pvs.CHTYPE = DBR_TIME_INT;
    debugMessage("connectPV Passed a DBR_INT. Entry with id =  ",id, " created");
    return &intPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToFloatPVMap()
{
    std::string id = floatPrefix;
    id += std::to_string(pvMonitorMapCount);
    floatPVMap[id].id = id;
    floatPVMap.at(id).pvs.CHTYPE = DBR_TIME_FLOAT;
    floatPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_FLOAT. Entry with id =  ",id, " created");
    return &floatPVMap[id].pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToDoublePVMap()
{
    std::string id = doublePrefix;
    id += std::to_string(pvMonitorMapCount);
    doublePVMap[id].id = id;
    doublePVMap.at(id).pvs.CHTYPE = DBR_TIME_DOUBLE;
    doublePVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_DOUBLE. Entry with id =  ",id, " created, doublemapo.size = ", doublePVMap.size());
    return &doublePVMap.at(id).pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToCharPVMap()
{
    std::string id = charPrefix;
    id += std::to_string(pvMonitorMapCount);
    charPVMap[id].id = id;
    charPVMap.at(id).pvs.CHTYPE = DBR_TIME_CHAR;
    charPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_CHAR. Entry with id =  ",id, " created");
    return &charPVMap.at(id).pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToLongPVMap()
{
    std::string id = longPrefix;
    id += std::to_string(pvMonitorMapCount);
    longPVMap[id].id = id;
    longPVMap.at(id).pvs.CHTYPE = DBR_TIME_LONG;
    longPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_LONG. Entry with id =  ",id, " created");
    return &longPVMap.at(id).pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToEnumPVMap()
{
    std::string id = enumPrefix;
    id += std::to_string(pvMonitorMapCount);
    enumPVMap[id].id = id;
    enumPVMap.at(id).pvs.CHTYPE = DBR_TIME_ENUM;
    enumPVMap.at(id).pvs.id = id;
    debugMessage("connectPV Passed a DBR_ENUM. Entry with id =  ",id, " created");
    return &enumPVMap.at(id).pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToVecDoublePVMap()
{
    std::string id = vecdoublePrefix;
    id += std::to_string(pvMonitorMapCount);
    vec_doublePVMap[id].id = id;
    vec_doublePVMap.at(id).pvs.CHTYPE = DBR_DOUBLE;
    vec_doublePVMap.at(id).pvs.id = id;
    vec_doublePVMap.at(id).pvs.isArrayPV = true;     // get ARRAY SIZE() after connected
    debugMessage("connectPV Passed a DBR_ARRAY_DOUBLE. Entry with id =  ",id, " created");
    return &vec_doublePVMap.at(id).pvs;
}
//______________________________________________________________________________
gmStructs::pvStruct* VCgeneralMonitor::addToVecIntPVMap()
{
    std::string id = vecintPrefix;
    id += std::to_string(pvMonitorMapCount);
    vec_intPVMap[id].id = id;
    vec_intPVMap.at(id).pvs.CHTYPE = DBR_TIME_INT;
    vec_intPVMap.at(id).pvs.id = id;
    vec_intPVMap.at(id).pvs.isArrayPV = true;     // get ARRAY SIZE() after connected
    debugMessage("connectPV Passed a DBR_ARRAY_INT. Entry with id =  ",id, " created");
    return &vec_intPVMap.at(id).pvs;
}
//______________________________________________________________________________
/// base class memebr functions
//______________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> VCgeneralMonitor::getILockStates(const std::string& name)
{
    std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> r;
    return r;
}
//______________________________________________________________________________
std::map<HWC_ENUM::ILOCK_NUMBER,std::string> VCgeneralMonitor::getILockStatesStr(const std::string & name)
{
    std::map<HWC_ENUM::ILOCK_NUMBER,std::string> r;
    return r;
}
//______________________________________________________________________________
double VCgeneralMonitor::get_CA_PEND_IO_TIMEOUT()const
{
    return CA_PEND_IO_TIMEOUT;
}
//______________________________________________________________________________
void VCgeneralMonitor::set_CA_PEND_IO_TIMEOUT( double val)
{
    CA_PEND_IO_TIMEOUT = val;
}










