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

#ifndef VELA_MAG_INTERFACE_H
#define VELA_MAG_INTERFACE_H
// djs
#include "interface.h"
#include "rfModStructs.h"
#include "structs.h"
#include "gunModConfigReader.h"
#include "gunModHexStringMap.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>

class gunModInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        gunModInterface::gunModInterface();
        gunModInterface( const std::string &pilaserConf,
                         const bool startVirtualMachine,
                         const bool* show_messages_ptr,
                         const bool* show_debug_messages_ptr,
                         const bool shouldStartEPICs);

        ~gunModInterface();

        const rfModStructs::gunModObject& getGunObjConstRef();

        bool isErrorStateGood() const;
        bool isWarmedUp() const;
        bool isNotWarmedUp() const;
        bool isInTrig() const;
        bool isInHVOn() const;
        bool isInStandby() const;
        bool isInOff() const;
        void reset();
        bool resetAndWait(const size_t waitTime);
        rfModStructs::GUN_MOD_STATE getMainState() const;
        rfModStructs::GUN_MOD_ERR_STATE getErrorState() const;

        bool waitForModState(rfModStructs::GUN_MOD_STATE state, const time_t waitTime);



        // These are pure virtual methods, so need an implEmentation in derived classes
        IlockMap1 getILockStates( const std::string & name   ){IlockMap1 r;return r;}
        IlockMap2 getILockStatesStr( const std::string & name){IlockMap2 r;return r;}

    private:
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs;
        bool allChidsInitialised;

        static void staticEntryGunModMonitor( const event_handler_args args);

        void updateMainState(const void * argsdbr);
        void updateWarmUpTime(const long val);
        void updateHexString(const event_handler_args& args);
        void updateErrorState();

        const std::map<std::string,std::string> gun_mod_hex_map;
        bool convertHexStringToMessage();

        void initialise();
        void initChids();
        void addChannel(const std::string & pvRoot, rfModStructs::pvStruct & pv);
        void startMonitors();

        rfModStructs::gunModObject gunMod;
        std::vector<rfModStructs::monitorStruct*> continuousMonitorStructs;
        gunModConfigReader configReader; /// class member so we can pass in file path in ctor

        // const member function pointer typedef
        typedef bool(gunModInterface::*ABoolMemFn)(void) const;

        bool waitFor(ABoolMemFn f1,gunModInterface& obj,const std::string& message,
                     const time_t waitTime,const size_t pause = 100);
        bool waitFor(ABoolMemFn f1,gunModInterface& obj,const char * message,
                     const time_t waitTime,const size_t pause = 100 );

};
#endif // VELA_MAG_INTERFACE_H
