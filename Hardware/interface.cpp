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
//  Last edit:   19-03-2018
//  FileName:    interface.cpp
//  Description:
//
//
//*/
// project includes
#include "interface.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>

interface::interface(const bool& show_messages,
                     const bool& show_debug_messages):
interface(show_messages, show_debug_messages, false)
{}
//______________________________________________________________________________
interface::interface(const bool& show_messages,
                     const bool& show_debug_messages,
                     const bool  shouldStartEPICs
                     ):
interface(show_messages, show_debug_messages, shouldStartEPICs, false)
{}
//______________________________________________________________________________
interface::interface(const bool& show_messages,
                     const bool& show_debug_messages,
                     const bool shouldStartEPICs,
                     const bool startVirtualMachine):
thisCaContext(nullptr),
configFileRead(false),
startVirtualMachine(startVirtualMachine),
shouldStartEPICs(shouldStartEPICs),
CA_PEND_IO_TIMEOUT(UTL::FIVE_DOUBLE),
EPICS_ACTIVATE(UTL::EPICS_ACTIVATE),
EPICS_SEND(UTL::EPICS_SEND),
EPICS_RESET(UTL::EPICS_RESET),
DBL_ERR_NUM(UTL::DBL_ERR_NUM),
allChidsInitialised(false),
allMonitorsStarted (false),
baseObject(show_messages, show_debug_messages,"from int")
{
    /* This 'enables' callbacks, monitoring, etc
        thisCaContext is the current (AND ONLY) context,
        use it to join from new threads with ca_attach_context
        you need to attach to this context if multi-threading
    */
    message("Interface base constructor.");
    ca_context_create(ca_enable_preemptive_callback);
    thisCaContext = ca_current_context();
}
//______________________________________________________________________________
interface::~interface()
{
    for(auto&& it : continuousILockMonitorStructs)
    {
       debugMessage("delete interface continuousILockMonitorStructs entry.");
       delete it;
    }
    debugMessage("interface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void interface::killILockMonitors()
{
    for(auto&& it : continuousILockMonitorStructs)
    {
        int status = ca_clear_subscription(it->EVID);
        if(status == ECA_NORMAL)
        {
            debugMessage(it->objName," ", ENUM_TO_STRING(it->iLockNumber),
                         " monitoring = false ");
        }
        else
            message("ERROR: in killILockMonitors: ca_clear_subscription failed "
                    "for", it->objName, " ",ENUM_TO_STRING(it->iLockNumber));
    }
}
//______________________________________________________________________________
double interface::get_CA_PEND_IO_TIMEOUT()const
{
    return CA_PEND_IO_TIMEOUT;
}
//______________________________________________________________________________
void interface::set_CA_PEND_IO_TIMEOUT(double val)
{
    CA_PEND_IO_TIMEOUT = val;
}
//______________________________________________________________________________
void interface::printStatusResult(const int   status,
                                  const char* success,
                                  const char* timeout) const
{
    if(SHOW_MESSAGES)
    {
        switch (status)
        {
            case ECA_NORMAL:
                if(strlen(success) != UTL::ZERO_SIZET)
                    printMessage(success);
                break;
            case ECA_TIMEOUT:
                if(strlen(timeout) != UTL::ZERO_SIZET)
                    printMessage(timeout);
                break;
            default:
                printMessage("!!! Unexpected error while searching: ",
                             ca_message(status));
        }
    }
}
//______________________________________________________________________________
void interface::addILockChannels(const int numIlocks,
                                 const std::string& pvRoot,
                                 const std::string& objName,
                                 map_ilck_pvstruct& iLockPVStructs)
{
  for(int i = UTL::ONE_INT; i < numIlocks + UTL::ONE_INT; ++i)
    {
        switch(i)
        {
            case UTL::ONE_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_1].pv = pvRoot + "Ilk1";
                break;
            case UTL::TWO_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_2].pv = pvRoot + "Ilk2";
                break;
            case UTL::THREE_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_3].pv = pvRoot + "Ilk3";
                break;
            case UTL::FOUR_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_4].pv = pvRoot + "Ilk4";
                break;
            case UTL::FIVE_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_5].pv = pvRoot + "Ilk5";
                break;
            case UTL::SIX_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_6].pv = pvRoot + "Ilk6";
                break;
            case UTL::SEVEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_7].pv = pvRoot + "Ilk7";
                break;
            case UTL::EIGHT_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_8].pv = pvRoot + "Ilk8";
                break;
            case UTL::NINE_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_9].pv = pvRoot + "Ilk9";
                break;
            case UTL::TEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_10].pv = pvRoot + "Ilk10";
                break;
            case UTL::ELEVEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_11].pv = pvRoot + "Ilk11";
                break;
            case UTL::TWELVE_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_12].pv = pvRoot + "Ilk12";
                break;
            case UTL::THIRTEEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_13].pv = pvRoot + "Ilk13";
                break;
            case UTL::FOURTEEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_14].pv = pvRoot + "Ilk14";
                break;
            case UTL::FIFTEEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_15].pv = pvRoot + "Ilk15";
                break;
            case UTL::SIXTEEN_INT:
                iLockPVStructs[HWC_ENUM::ILOCK_NUMBER::ILOCK_16].pv = pvRoot + "Ilk16";
                break;
        }
        //debugMessage("Added Ilock_", i , " chid to ilkChidMap.");
    }
    for(auto&& it : iLockPVStructs)
    {
        it.second.objName = objName;

        /*
        https://epics.anl.gov/base/R3-14/8-docs/CAref.html#ca_create_channel
        PRIORITY
        The priority level for dispatch within the server or network with 0 specifying
        the lowest dispatch priority and 99 the highest. This parameter currently does
        not impact dispatch priorities within the client, but this might change in the
        future. The abstract priority range specified is mapped into an operating system
        specific range of priorities within the server. This parameter is ignored if the
        server is running on a network or operating system that does not have native
        support for prioritized delivery or execution respectively. Specifying many
        different priorities within the same program can increase resource consumption
        in the client and the server because an independent virtual circuit, and
        associated data structures, is created for each priority that is used on a
        particular server.
        */
        capri priority = UTL::PRIORITY_0;
        ca_create_channel(it.second.pv.c_str(), nullptr, nullptr, priority,&it.second.CHID);
        debugMessage("Create channel to ", it.second.pv);
    }
}
//______________________________________________________________________________
void interface::monitorIlocks(map_ilck_pvstruct& iLockPVStructs,
                              map_ilck_state & iLockStates)
{
    iLockStates.clear();
    for(auto&& it : iLockPVStructs)
    {
        continuousILockMonitorStructs.push_back(new HWC_ENUM::iLockMonitorStruct());
        continuousILockMonitorStructs.back() -> iLockState   =&iLockStates[it.first];
        continuousILockMonitorStructs.back() -> interface    = this;
        continuousILockMonitorStructs.back() -> iLockNumber  = it.first;
        continuousILockMonitorStructs.back() -> objName      = it.second.objName ;
        /*
           ca_create_subscription accepts a void * user argument,
           we pass a poiniter to the monitor struct,
           in the callback function this is cast back and the data can then be updated
           If you pass DBF_STRING and recast as a char * in the callback
           you can get the state as GOOD, BAD, OPEN, CLOSED etc,
           This is useful for debugging, but
           in general i'm just going to subscribe to the DBR_ENUM
        */
        ca_create_subscription(it.second.CHTYPE,
                               it.second.COUNT,
                               it.second.CHID,
                               it.second.MASK,
                               interface::staticEntryILockMonitor,
                               (void*)continuousILockMonitorStructs.back(),
                             &continuousILockMonitorStructs.back() ->EVID);
       // sending to epics should happen in the derived class
    }
}
//______________________________________________________________________________
void interface::staticEntryILockMonitor(event_handler_args args)
{
    /*  recast args.usr (a void *) to a monitor struct pointer,
        then dereference
    */
    HWC_ENUM::iLockMonitorStruct ms =
        *reinterpret_cast<HWC_ENUM::iLockMonitorStruct*> (args.usr);

    /*  Not sure how to decode EPICS enums apart from trial and error
        you can test with DBF_STRING as the callback type
        // If subscribed to DBF_STRING use this to get the string
        //char * val = (char *)args.dbr;
        switch based on the epics value, args.dbr,
        and update the ILockState with the correct enum
    */
    switch(getDBRunsignedShort(args))
    {
        case UTL::ZERO_US:
            *ms.iLockState = HWC_ENUM::ILOCK_STATE::ILOCK_BAD;
            break;
        case UTL::ONE_US:
            *ms.iLockState = HWC_ENUM::ILOCK_STATE::ILOCK_GOOD;
            break;
        default:
            *ms.iLockState = HWC_ENUM::ILOCK_STATE::ILOCK_ERROR;
    }
    /*  You can maker make debug messages easier
        to understand by using ENUM_TO_STRING
        \\ms.interface -> debugMessage(ms.objName,
                                       " new ILock_",
                                       ms.iLockNumber + 1 ,
                                       " state = ",
                                       ENUM_TO_STRING(*ms.iLockState));
    */
}
//______________________________________________________________________________
int interface::sendToEpics(const char* ca,
                           const char* mess1,
                           const char* mess2) const
{
    int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
    SEVCHK(status, ca);
    printStatusResult(status, mess1, mess2);
    return status;
}
//______________________________________________________________________________
int interface::sendToEpics(const std::string& ca,const std::string& mess1,
                           const std::string& mess2) const
{
    int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
    SEVCHK(status, ca.c_str());
    printStatusResult(status, mess1.c_str(), mess2.c_str());
    return status;
}
//______________________________________________________________________________
int interface::sendToEpics2(const char* ca,
                            const char* mess1,
                            const char* mess2) const
{
    int status = ca_flush_io();
    SEVCHK(status, ca);
    printStatusResult(status, mess1, mess2);
    return status;
}
//______________________________________________________________________________
int interface::sendToEpics2(const std::string& ca,
                            const std::string& mess1,
                            const std::string& mess2) const
{
    int status = ca_flush_io();
    SEVCHK(status, ca.c_str());
    printStatusResult(status, mess1.c_str(), mess2.c_str());
    return status;
}
//______________________________________________________________________________
void interface::checkCHIDState(const chid& CHID, const std::string& name)
{
    channel_state chidState  = ca_state(CHID);
    switch(chidState)
    {
        case cs_never_conn:
            message(name, " valid chid, server not found or unavailable");
            break;
        case cs_prev_conn:
            message(name, " valid chid, previously connected to server");
            break;
        case cs_conn:
            /* if everything is fine don't print a message */
            //message(name, " valid chid, connected to server");
            break;
        case cs_closed:
            message(name, " channel deleted by user");
            break;
   }
}
//______________________________________________________________________________
bool interface::interfaceInitReport() const
{
    bool ret = true;

    if(!configFileRead)
    {
        ret = false;
        message("interfaceInitReport: Error Reading Config File");
    }
    if(shouldStartEPICs)
    {
        if(!allChidsInitialised)
        {
            ret = false;
            message("interfaceInitReport: Error Initilaising Chids.");
        }
        if(!allMonitorsStarted)
        {
            ret = false;
            message("interfaceInitReport: Error Staring Monitors.");
        }
    }
    else
        message("interfaceInitReport: Controller Initiliased with NO Monitoring");
    return ret;
}
//______________________________________________________________________________
bool interface::interfaceInitReport(bool shouldStartEPICs) const
{
    return interfaceInitReport();
}
//______________________________________________________________________________
void interface::attachTo_thisCAContext()
{
    if(thisCaContext)
        ca_attach_context(thisCaContext);
}
//______________________________________________________________________________
void interface::detachFrom_thisCAContext()
{
    ca_detach_context();
}
//______________________________________________________________________________
bool interface::isTimeType(const long type) const
{
    bool r = false;
    switch(type)
    {
        case DBR_TIME_STRING:
            r = true;
            break;
        case DBR_TIME_INT:
            r = true;
            break;
        case DBR_TIME_LONG:
            r = true;
            break;
        case  DBR_TIME_FLOAT:
            r = true;
            break;
        case DBR_TIME_ENUM:
            r = true;
            break;
        case DBR_TIME_CHAR:
            r = true;
            break;
        case DBR_TIME_DOUBLE:
            r = true;
            break;
        default:
            r = false;
    }
    return r;
}
//______________________________________________________________________________
void interface::updateTime(const epicsTimeStamp& stamp, double& val, std::string& str)
{
    /* for hints look in epicsTime.h */
    char timeString[UTL::BUFFER_36];
    epicsTimeToStrftime(timeString, sizeof(timeString),
                        "%a %b %d %Y %H:%M:%S.%f", &stamp);
    val = ((double)stamp.nsec * UTL::TEN_POWER_MINUS_NINE) +
          (double)stamp.secPastEpoch;
    /*  prove it works
        std::cout <<std::setprecision(15) <<std::showpoint<<  val <<std::endl;
    */
    str = timeString;
}
//______________________________________________________________________________
long long interface::msChronoTime() const
{
//https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    long long duration = value.count();
    return duration;
}
//______________________________________________________________________________
void interface::updateBoolState(const event_handler_args& args, bool& parameter)
{   /*
       some simple states don't have an enum, they are either true or false
       This can probably be used by many hardware controllers
    */
    if(*(int*)args.dbr == UTL::ONE_INT)
    {
        parameter = true;
    }
    else
    {
        parameter = false;
    }
}
//______________________________________________________________________________
std::string interface::getDBRstring(const event_handler_args& args)
{
    std::string s1;
    s1.assign((const char*)args.dbr);
    return s1;
}
//______________________________________________________________________________
double interface::getDBRdouble(const event_handler_args& args)
{
    return *(double*)args.dbr;
}
//______________________________________________________________________________
long interface::getDBRlong(const event_handler_args& args)
{
    return *(long*)args.dbr;
}
//______________________________________________________________________________
int interface::getDBRint(const event_handler_args& args)
{
    return *(int*)args.dbr;
}
//______________________________________________________________________________
unsigned short interface::getDBRunsignedShort(const event_handler_args& args)
{
    return *(unsigned short*)args.dbr;
}
//______________________________________________________________________________
unsigned long interface::getDBRunsignedLong(const event_handler_args& args)
{
    return *(unsigned long*)args.dbr;
}
//______________________________________________________________________________
bool interface::getDBRbool(const event_handler_args& args)
{
    return *(bool*)args.dbr;
}
//______________________________________________________________________________
void interface::getDBRdouble_timestamp(const event_handler_args& args,
                                       HWC_ENUM::epics_timestamp& ts,
                                       double& val)
{
    const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
    val = p->value;
    ts.etime = p->stamp;
    updateTime(ts.etime, ts.time_ns, ts.time_Str);
}
//______________________________________________________________________________
void interface::getDBRdouble_timestamp(const event_handler_args& args,
                                       double& val)
{
    const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
    val = p->value;
    //ts.etime = p->stamp;
    //updateTime(ts.etime, ts.time_ns, ts.time_Str);
}
//______________________________________________________________________________
void interface::pause_x(std::chrono::milliseconds x) const
{
    std::this_thread::sleep_for(x);
}
//______________________________________________________________________________
void interface::standard_pause() const
{
    pause_2000();
}
//______________________________________________________________________________
void interface::pause_2000() const
{
    pause_x(UTL::STANDARD_PAUSE);
}
//______________________________________________________________________________
void interface::pause_300() const
{
    pause_x(UTL::PAUSE_300);
}
//______________________________________________________________________________
void interface::pause_500() const
{
    pause_x(UTL::PAUSE_500);
}
//______________________________________________________________________________
void interface::pause_2() const
{
    pause_x(UTL::PAUSE_2);
}
//______________________________________________________________________________
void interface::pause_1() const
{
    pause_x(UTL::PAUSE_1);
}
