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
//               to EPICS. They set up monitors, get and put data and often also do
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
#include <chrono>
#include <thread>
// project includes
#include "baseObject.h"
#include "structs.h"
//______________________________________________________________________________
class interface : public baseObject
{
    public:
        interface(const bool& show_messages,
                  const bool& show_debug_messages);
        interface(const bool& show_messages,
                  const bool& show_debug_messages,
                  const bool shouldStartEPICs);
        interface(const bool& show_messages,
                  const bool& show_debug_messages,
                  const bool shouldStartEPICs,
                  const bool startVirtualMachine);
        interface& interface::operator= ( const interface& other ) = delete;

        /* typedefs for long type names */
        using map_ilck_state =
            std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
                    ;
        using map_ilck_string =
            std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
                    ;
        using map_ilck_pvstruct =
            std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::iLockPVStruct>;
        /* These pure virtual methods MUST be overwritten in the derived
           interface (making this an abstract base class)
           This also means the destructor need not be protected
        */
        //virtual map_ilck_string getILockStatesStr(const std::string& name) const = 0;
        //virtual map_ilck_state  getILockStates   (const std::string& name) const = 0;

        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);

        /*  Reports back if the main init tasks:
            reading config, finding chids, setting up monitors
            (add your own if needed)
        */
        bool interfaceInitReport(bool shouldStartEPICs)const;
        bool interfaceInitReport()const;

    protected:
        /*
            protected destructor to make sure this class is never instantiated
           the compiler won't let us call delete on any base class pointers
        */
        ~interface();
        // this should be called in the derived interface destructor
        void killILockMonitors();
        /*
            The below was written back in the days when i assumed operational
           consistency:
            some EPICS constants
            send ...1 to enable open / close shutter, screen etc,
                therefore these are available to all interface classes
            send ...0 to send open / close shutter
                (???, i'm guessing here, but it seems to work)
           I'm also not 100 % certain on the type,
           but unsigned short works so far
           Other constant numbers could go here if needed
        */
        const bool shouldStartEPICs, startVirtualMachine;
        unsigned short EPICS_ACTIVATE, EPICS_SEND, EPICS_RESET;
        const double DBL_ERR_NUM;
        bool configFileRead, allChidsInitialised, allMonitorsStarted;
        double CA_PEND_IO_TIMEOUT;
        static void updateTime(const epicsTimeStamp& stamp,
                               double& val,
                               std::string& str);
#ifndef __CINT__
        /*
            This is the 'only' ca_client_context
            attach and detach to this when multi-threading
        */
        ca_client_context * thisCaContext;

        void attachTo_thisCAContext();
        void detachFrom_thisCAContext();

        void addILockChannels(const int numIlocks,
                              const std::string& pvRoot,
                              const std::string& objName,
                              map_ilck_pvstruct& iLockPVStructs);
        void monitorIlocks(map_ilck_pvstruct& iLockPVStructs,
                           map_ilck_state&    iLockStates);
        static void staticEntryILockMonitor(event_handler_args args);

        /*
            can probably write a generic template function for the
            below based on args.type
        */
        static unsigned short getDBRunsignedShort(const event_handler_args& args);
        static unsigned long  getDBRunsignedLong (const event_handler_args& args);
        static std::string    getDBRstring       (const event_handler_args& args);
        static size_t         getDBRsizet        (const event_handler_args& args);
        static double         getDBRdouble       (const event_handler_args& args);
        static long           getDBRlong         (const event_handler_args& args);
        static int            getDBRint          (const event_handler_args& args);
        static bool           getDBRbool         (const event_handler_args& args);
        static void        getDBRdouble_timestamp(const event_handler_args& args,
                                                  HWC_ENUM::epics_timestamp& ts,
                                                  double& val);
        void getDBRdouble_timestamp(const event_handler_args& args, double& val);
        void updateBoolState(const event_handler_args& args, bool& parameter);


        static double getTime(const epicsTimeStamp& stamp);


        void setChannelCountandType(const chid& CHID,
                                    chtype& CHTYPE,
                                    unsigned long& COUNT);

        void checkCHIDState(const chid& CHID, const std::string& name);
        int sendToEpics2(const char* ca,const char* mess1,const char* mess2) const;
        int sendToEpics (const char* ca,const char* mess1,const char* mess2) const;
        int sendToEpics2(const std::string& ca,
                         const std::string& mess1,
                         const std::string& mess2) const;
        int sendToEpics (const std::string& ca,
                         const std::string& mess1,
                         const std::string& mess2) const;
#endif

        bool iLocksAreGood(const map_ilck_state& iLockStates) const;
        long long msChronoTime()const;

        bool isTimeType(const long type) const;

        void printStatusResult(const int   status,
                               const char* success,
                               const char* timeout) const;

        /*  This is a vector of pointers...
            "no!!" you say "let's follow Bjarne Stroustrup's advice and:
            "Store many objects in a container by value."?"
http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
            ok... maybe one day we re-factor, for now:
                        REMEMBER TO DELETE THIS IN THE DESTRUCTOR
                        REMEMBER TO DELETE THIS IN THE DESTRUCTOR
                        REMEMBER TO DELETE THIS IN THE DESTRUCTOR
                        REMEMBER TO DELETE THIS IN THE DESTRUCTOR
                        REMEMBER TO DELETE THIS IN THE DESTRUCTOR
                        REMEMBER TO DELETE THIS IN THE DESTRUCTOR
        */
        std::vector<HWC_ENUM::iLockMonitorStruct*> continuousILockMonitorStructs;
        /*
            Pauses
        */
        void pause_x(std::chrono::milliseconds x) const;
        void standard_pause() const;
        void pause_2000() const;
        void pause_1000() const;
        void pause_300()  const;
        void pause_500()  const;
        void pause_50()   const;
        void pause_2()    const;
        void pause_1()    const;
        /*
            We often check if entries exist in maps,
            use these functions to do it safely
            be careful to pick the correct one!
        */
        template<class T>
        bool entryExists(const std::map<std::string,T>& m,const std::string& name)const
        {
            bool ret = false;
            auto it = m.find(name);
                if(it != m.end())
                    ret = true;
            return ret;
        }
        template<class T>
        bool entryExists(const std::map<std::string,T>& m,std::string& name)const
        {
            bool ret = false;
            auto it = m.find(name);
                if(it != m.end())
                    ret = true;
            return ret;
        }
        template<class T, class U>
        bool entryExists(const std::map<U,T>& m, const U& name)const
        {
            bool ret = false;
            auto it = m.find(name);
                if(it != m.end())
                    ret = true;
            return ret;
        }
        /*
            some caput templates...
            more often than not roll your own
        */
        template<class T, class U>
        int caput(U TYPE, chid& CHID,T& com,const char* mess1,const char* mess2)
        {
            ca_put(DBR_LONG, CHID,&com);
            return sendToEpics("ca_put", mess1, mess2);
        }
        template<class T, class U>
        int caput(U TYPE, chid& CHID,const T& com,const char* mess1,const char* mess2)
        {
            ca_put(TYPE, CHID,&com);
            return sendToEpics("ca_put", mess1, mess2);
        }
    private:

};
//______________________________________________________________________________




//______________________________________________________________________________
#endif // _INTERFACE_BASE_H
