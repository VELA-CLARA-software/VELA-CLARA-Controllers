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
//  FileName:    interface.h
//  Description: The interface base class. Interface classes manage the connection
//               to EPICS. The set up monitors, get and put data and often also do
//               some data processing. They hold the virtual hardware objects that
//               contain all relevant online/offline data. They hold the config
//               readers for each hardware type.
//               For more information you should try reading the documentation
//
//
//*/
#ifndef _INTERFACE_BASE_H
#define _INTERFACE_BASE_H
// epics includes
#ifndef __CINT__
#include <cadef.h>
#endif
// stl includes
#include <iostream>
#include <map>
// project includes
#include "baseObject.h"
#include "structs.h"
//______________________________________________________________________________
class interface : public baseObject
{
    public:
        interface(const bool* show_messages_ptr,
                  const bool* show_debug_messages_ptr);
        interface(const bool* show_messages_ptr,
                  const bool* show_debug_messages_ptr,
                  const bool  shouldStartEPICs);
        ~interface();

        /* These pure virtual methods MUST be overwritten in the derived interface
           (making this an abstract base class)
           This also means the destructor need not be protected
        */
        virtual std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string & name) = 0;
        virtual std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string & name) = 0;

        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT(double val);

        /*  Reports back if the main init tasks:
            reading config, finding chids, setting up monitors (add your own if needed)
            has worked
        */
        bool interfaceInitReport(bool shouldStartEPICs = true);

    protected:
        // this should be called in the derived interface destructor
        void killILockMonitors();

        /* The below was written back in the days when i assumed operational consistency:
            some EPICS contsants
            send ...1 to enable open / close shutter, screen etc,
                therefore these are available to all interface classes
           send ...0 to send open / close shutter
                (???, i'm guessing here, but it seems to work)
           i'm also not 100 % certain on the type, but unsigned short works so far
           Other constant numbers could go here if needed
        */
        const bool shouldStartEPICs;
        const unsigned short EPICS_ACTIVATE, EPICS_SEND, EPICS_RESET;
        const double DBL_ERR_NUM;
        bool configFileRead, allChidsInitialised, allMonitorsStarted;
        double CA_PEND_IO_TIMEOUT;

        void updateTime(const epicsTimeStamp & stamp, double & val, std::string & str);

        /* We often check if entries exist in maps, use this function to do it safe */
        template<class T>
        bool entryExists(const std::map<std::string, T> & m, const std::string & name) const
        {
            bool ret = false;
            auto it = m.find(name);
                if(it != m.end())
                    ret = true;
            return ret;
        }

#ifndef __CINT__
        /*  This is the 'only' ca_client_context
            attach and detach to this when multi-threading
        */
        ca_client_context * thisCaContext;

        void attachTo_thisCAContext();
        void detachFrom_thisCAContext();

        void addILockChannels(const int numIlocks, const std::string & pvRoot, const std::string & objName,std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct> & iLockPVStructs);
        void monitorIlocks(std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct>  & iLockPVStructs, std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> & iLockStates);
        static void staticEntryILockMonitor(event_handler_args args);


        std::string getDBRstring(const event_handler_args& args) const;
        double getDBRdouble(const event_handler_args& args) const;
        long getDBRlong(const event_handler_args& args) const;
        int getDBRint(const event_handler_args& args) const;
        unsigned short getDBRunsignedShort(const event_handler_args& args) const;
        bool getDBRbool(const event_handler_args& args) const;



        template<class T, class U>
        int caput(U TYPE, chid & CHID, T & com, const char * mess1, const char * mess2)
        {
            ca_put(TYPE, CHID, &com); /// TYPE is DBR_ENUM, etc.
            return sendToEpics("ca_put", mess1, mess2);
        }

        void checkCHIDState(const chid & CHID, const std::string & name);
        int sendToEpics(const char * ca, const char * mess1, const char * mess2);
        int sendToEpics2(const char * ca, const char * mess1, const char * mess2);
        int sendToEpics(const  std::string & ca,const  std::string & mess1,const  std::string & mess2);
        int sendToEpics2(const  std::string & ca,const  std::string & mess1,const  std::string & mess2);
#endif

        bool iLocksAreGood(std::map<HWC_ENUM::ILOCK_NUMBER , HWC_ENUM::ILOCK_STATE> & iLockStates);

        void updateBoolState(const event_handler_args& args, bool& parameter);

        long long msChronoTime();

        bool isTimeType(const long);

        void printStatusResult(const int status, const char * success, const char * timeout);

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// though... maybe one day we re-factor, for now remember to delete in the destructor

        std::vector<HWC_ENUM::iLockMonitorStruct *> continuousILockMonitorStructs;
    private:

};

#endif // _INTERFACE_BASE_H
