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
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        gunModInterface::gunModInterface();
        gunModInterface(const std::string &pilaserConf,
                         const bool startVirtualMachine,
                         const bool& show_messages_ptr,
                         const bool& show_debug_messages_ptr,
                         const bool shouldStartEPICs);

        ~gunModInterface();

        const rfModStructs::gunModObject& getGunObjConstRef();

        bool isErrorStateGood() const;
        bool isWarmedUp() const;
        bool isNotWarmedUp() const;
        bool isInRFOn() const;
        bool isInHVOn() const;
        bool isInStandby() const;
        bool isInOff() const;
        void reset();
        bool resetAndWait(const size_t waitTime);
        rfModStructs::GUN_MOD_STATE getMainState() const;
        rfModStructs::GUN_MOD_ERR_STATE getErrorState() const;

        bool waitForModState(rfModStructs::GUN_MOD_STATE state, const time_t waitTime);



        // These are pure virtual methods, so need an implEmentation in derived classes
        IlockMap1 getILockStates(const std::string & name  ){IlockMap1 r;return r;}
        IlockMap2 getILockStatesStr(const std::string & name){IlockMap2 r;return r;}

        bool setOff() const;
        bool setStandby() const;
        bool setHVOn() const;
        bool setRFOn() const;
        bool setModState(const rfModStructs::GUN_MOD_STATE_SET v)const;

    private:
        // MOVE TO BASE CLASS
        //const bool shouldStartEPICs;
        bool allChidsInitialised;

        const std::string CURR,VOLT;

        void killMonitor(rfModStructs::monitorStruct* ms);

        static void staticEntryGunModMonitor(const event_handler_args args);
        void updateValue(const event_handler_args& args,const rfModStructs::GUN_MOD_PV_TYPE pv);
        void updateMAGPS_PV(const event_handler_args& args,const size_t num,const rfModStructs::GUN_MOD_PV_TYPE pv);
        void updateHVPS_PV(const event_handler_args& args,const size_t num,const rfModStructs::GUN_MOD_PV_TYPE pv);
        void updateMainState(const event_handler_args& args);
        void updateWarmUpTime(const event_handler_args& args);
        void updateHexString(const event_handler_args& args);
        void updateStateSetRead(const event_handler_args& args);
        void updateErrorState();

        void updateCtRead(const event_handler_args& args);
        void updateCvdRead(const event_handler_args& args);
        void updatePlswthRead(const event_handler_args& args);
        void updatePlswthFwhmRead(const event_handler_args& args);
        void updateIlock_string(const event_handler_args& args, size_t ilock_number);
        void check_ilock_changed(const std::string& new_val,std::string& present_val,size_t num);

        size_t getPVNum(const rfModStructs::GUN_MOD_PV_TYPE pv);

        int is_MAGPS_or_HVCPS_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const;
        bool isMAGPS_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const;
        bool isHVPS_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const;
        bool isVOLT_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const;
        bool isCURR_PV(const rfModStructs::GUN_MOD_PV_TYPE pv) const;

        void updateHoldRFOnState(const event_handler_args& args);


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
                     const time_t waitTime,const size_t pause = 100);

};
#endif // VELA_MAG_INTERFACE_H
