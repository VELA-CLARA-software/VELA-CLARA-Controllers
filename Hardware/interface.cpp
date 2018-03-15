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

#include "interface.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
//interface::interface():EPICS_ACTIVATE (1), EPICS_SEND(0), EPICS_RESET(1),DBL_ERR_NUM(-9999.9999){}

interface::interface(const bool* show_messages_ptr, const  bool * show_debug_messages_ptr):
thisCaContext(nullptr),
configFileRead(false),
CA_PEND_IO_TIMEOUT(5.0),  //MAGIC_NUMBER
baseObject(show_messages_ptr, show_debug_messages_ptr),
EPICS_ACTIVATE (1),         //MAGIC_NUMBER
EPICS_SEND(0),              //MAGIC_NUMBER
EPICS_RESET(1),           //MAGIC_NUMBER
DBL_ERR_NUM(-9999.9999),   //MAGIC_NUMBER
allChidsInitialised(false),
allMonitorsStarted (false)
{
    /// This 'enables' callbacks, monitoring, etc
    ca_context_create(ca_enable_preemptive_callback);
    /// This is the current (AND ONLY) context,
    /// use it to join from new threads with ca_attach_context
    /// you need to attach to this context if multi-threading
    thisCaContext = ca_current_context();
}
//______________________________________________________________________________
interface::~interface()
{
    for(auto && it : continuousILockMonitorStructs)
    {
       debugMessage("delete interface continuousILockMonitorStructs entry.");
        delete it;
    }
    debugMessage("interface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void interface::killILockMonitors()
{
    for(auto && it : continuousILockMonitorStructs)
    {
        int status = ca_clear_subscription(it->EVID);
        if(status == ECA_NORMAL)
            debugMessage(it->objName," ", ENUM_TO_STRING(it->iLockNumber), " monitoring = false ");
        else
            message("ERROR: in killILockMonitors: ca_clear_subscription failed for ", it->objName, " ", ENUM_TO_STRING(it->iLockNumber));
    }
}
//______________________________________________________________________________
double interface::get_CA_PEND_IO_TIMEOUT()
{
    return CA_PEND_IO_TIMEOUT;
}
//______________________________________________________________________________
void interface::set_CA_PEND_IO_TIMEOUT(double val)
{
    CA_PEND_IO_TIMEOUT = val;
}
//______________________________________________________________________________
void interface::printStatusResult(const int status, const char * success, const char* timeout)
{
    if(*SHOW_MESSAGES_PTR)
    {
        switch (status)
        {
            case ECA_NORMAL:
                if(strlen(success) != 0)//MAGIC_NUMBER
                    printMessage(success);
                break;
            case ECA_TIMEOUT:
                if(strlen(timeout) != 0)//MAGIC_NUMBER
                    printMessage(timeout);
                break;
            default:
                printMessage("!!! Unexpected error while searching: ", ca_message(status));
        }
    }
}
//______________________________________________________________________________
void interface::addILockChannels(const int numIlocks, const std::string & pvRoot,const std::string & objName, std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct> & iLockPVStructs)
{
  for(int  i = 1; i <numIlocks + 1 ; ++i)//MAGIC_NUMBER
    {
        switch(i)
        {
            case 1://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_1 ].pv = pvRoot + "Ilk1";
                break;
            case 2://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_2 ].pv = pvRoot + "Ilk2";
                break;
            case 3://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_3 ].pv = pvRoot + "Ilk3";
                break;
            case 4://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_4 ].pv = pvRoot + "Ilk4";
                break;
            case 5://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_5 ].pv = pvRoot + "Ilk5";
                break;
            case 6://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_6 ].pv = pvRoot + "Ilk6";
                break;
            case 7://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_7 ].pv = pvRoot + "Ilk7";
                break;
            case 8://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_8 ].pv = pvRoot + "Ilk8";
                break;
            case 9://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_9 ].pv = pvRoot + "Ilk9";
                break;
            case 10://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_10 ].pv = pvRoot + "Ilk10";
                break;
            case 11://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_11 ].pv = pvRoot + "Ilk11";
                break;
            case 12://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_12 ].pv = pvRoot + "Ilk12";
                break;
            case 13://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_13 ].pv = pvRoot + "Ilk13";
                break;
            case 14://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_14 ].pv = pvRoot + "Ilk14";
                break;
            case 15://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_15 ].pv = pvRoot + "Ilk15";
                break;
            case 16://MAGIC_NUMBER
                iLockPVStructs[ VELA_ENUM::ILOCK_NUMBER::ILOCK_16 ].pv = pvRoot + "Ilk16";
                break;
        }
        //debugMessage("Added Ilock_", i , " chid to ilkChidMap.");
    }

    for(auto && it : iLockPVStructs)
    {
        it.second.objName = objName;
        ca_create_channel(it.second.pv.c_str(), 0, 0, 0, &it.second.CHID);//MAGIC_NUMBER
        debugMessage("Create channel to ", it.second.pv);
    }
}
//______________________________________________________________________________
void interface::monitorIlocks(std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct> & iLockPVStructs, std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> & iLockStates)
{
    ///!continuousILockMonitorStructs.clear();  NO don't call clear here, call in dervied class before loops
    iLockStates.clear();
    for(auto && it : iLockPVStructs)
    {
        continuousILockMonitorStructs.push_back(new VELA_ENUM::iLockMonitorStruct());
        continuousILockMonitorStructs.back() -> iLockState   = &iLockStates[ it.first ];
        continuousILockMonitorStructs.back() -> interface    = this;
        continuousILockMonitorStructs.back() -> iLockNumber  = it.first;
        continuousILockMonitorStructs.back() -> objName      = it.second.objName ;

        // ca_create_subscription accepts a void * user argument, we pass a poiniter to the monitor struct,
        // in the callback function this is cast back and the data can then be updated

        // If you pass DBF_STRING and recast as a char * in the callback you can get the state as GOOD, BAD, OPEN, CLOSED etc,
        // This is useful for debugging, but in general i'm just going to subscribe to the DBR_ENUM

        ca_create_subscription(it.second.CHTYPE, it.second.COUNT, it.second.CHID, it.second.MASK, interface::staticEntryILockMonitor,
                               (void*)continuousILockMonitorStructs.back(), &continuousILockMonitorStructs.back() ->EVID);
       // sending to epics should happen in the derived class
    }
}
//______________________________________________________________________________
void interface::staticEntryILockMonitor(event_handler_args args)
{
    /// recast args.usr (a void *) to a monitor struct pointer, then dereference

    VELA_ENUM::iLockMonitorStruct ms = * reinterpret_cast<VELA_ENUM::iLockMonitorStruct *> (args.usr);

    /// Not sure how to decode these apart from trial and error
    /// you can test with DBF_STRING as the callback type

    //unsigned short val = *(unsigned short*)args.dbr;

    /// If subscribed to DBF_STRING use this to get the message
    //char * val = (char *)args.dbr;

    /// now we can switch based on the value and update the ILockState with the correct enum

    switch(*(unsigned short*)args.dbr)
    {
        case 0://MAGIC_NUMBER
            *ms.iLockState = VELA_ENUM::ILOCK_STATE::ILOCK_BAD;
            break;
        case 1://MAGIC_NUMBER
            *ms.iLockState = VELA_ENUM::ILOCK_STATE::ILOCK_GOOD;
            break;
        default:
            *ms.iLockState = VELA_ENUM::ILOCK_STATE::ILOCK_ERROR;
    }
    /// make debug messages easier to understand by using ENUM_TO_STRING
    //ms.interface -> debugMessage(ms.objName, " new ILock_", ms.iLockNumber + 1 ,  " state = ", ENUM_TO_STRING(*ms.iLockState));
}
//______________________________________________________________________________
int interface::sendToEpics(const char * ca, const char * mess1, const char * mess2)
{
    int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
    SEVCHK(status, ca);
    printStatusResult(status, mess1, mess2);
    return status;
}
//______________________________________________________________________________
int interface::sendToEpics(const  std::string & ca,const  std::string & mess1,const  std::string & mess2)
{
    int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
    SEVCHK(status, ca.c_str());
    printStatusResult(status, mess1.c_str(), mess2.c_str());
    return status;
}
//______________________________________________________________________________
int interface::sendToEpics2(const char * ca, const char * mess1, const char * mess2)
{
    int status = ca_flush_io();
    SEVCHK(status, ca);
    printStatusResult(status, mess1, mess2);
    return status;
}
//______________________________________________________________________________
int interface::sendToEpics2(const  std::string & ca,const  std::string & mess1,const  std::string & mess2)
{
    int status = ca_flush_io();
    SEVCHK(status, ca.c_str());
    printStatusResult(status, mess1.c_str(), mess2.c_str());
    return status;
}
//______________________________________________________________________________
void interface::checkCHIDState(const chid & CHID, const std::string & name)
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
            //message(name, " valid chid, connected to server");
            break;
        case cs_closed:
            message(name, " channel deleted by user");
            break;
   }
}
//______________________________________________________________________________
bool interface::interfaceInitReport(bool shouldStartEPICs)
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
bool interface::iLocksAreGood(std::map<VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE> & iLockStates)
{
    bool ret = true;
    for(auto && it : iLockStates)
        if(it.second != VELA_ENUM::ILOCK_STATE::ILOCK_GOOD)
            ret = false;

    return ret;
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
bool interface::isTimeType(const long type)
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
/// WRITTEN THIS WAY, THIS FUNCTION COULD GO IN THE INTERFACE BASE CLASS
//______________________________________________________________________________
void interface::updateTime(const epicsTimeStamp & stamp, double & val, std::string & str)
{// look in epicsTime.h
//    const dbr_time_char * pTD = (const struct dbr_time_char *) args.dbr;
//    epicsUInt32 sec  = pTD ->stamp.secPastEpoch;   /* seconds since 0000 Jan 1, 1990 */
//    epicsUInt32 nsec = pTD ->stamp.nsec;
    char timeString[36];//MAGIC_NUMBER
    epicsTimeToStrftime (timeString, sizeof (timeString), "%a %b %d %Y %H:%M:%S.%f", &stamp);
//    const dbr_char_t * pValue = &pTD -> value;
    val =  ((double)stamp.nsec * 0.000000001) + (double)stamp.secPastEpoch;//MAGIC_NUMBER
    // prove it works
    //std::cout <<std::setprecision(15) <<std::showpoint<<  val <<std::endl;
    str = timeString;
}
//______________________________________________________________________________
long long interface::msChronoTime()
{
    //https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    long long duration = value.count();
    return duration;
}
//____________________________________________________________________________________________
void interface::updateBoolState(const event_handler_args& args, bool& parameter)
{   /* some simple states don't have an enum, they are either true or false
       This can probably be used by many hardware controllers
    */
    if(*(int*)args.dbr == 1)
    {
        parameter= true;
    }
    else
    {
        parameter= false;
    }
}
//______________________________________________________________________________
std::string interface::getDBRstring(const event_handler_args& args) const
{
    //std::stringstream ss;
    std::string s1;
//    const char * char_array = (const char*)args.dbr;
//    size_t Size = strlen(char_array );
//    message("size = ", Size);
    s1.assign((const char*)args.dbr);
    return s1;
}
//______________________________________________________________________________
double interface::getDBRdouble(const event_handler_args& args) const
{
    return *(double*)args.dbr;
}
//______________________________________________________________________________
long interface::getDBRlong(const event_handler_args& args) const
{
    return *(long*)args.dbr;
}
//______________________________________________________________________________
int interface::getDBRint(const event_handler_args& args) const
{
    return *(int*)args.dbr;
}
//______________________________________________________________________________
unsigned short interface::getDBRUnsignedShort(const event_handler_args& args) const
{
    return *(unsigned short*)args.dbr;
}


