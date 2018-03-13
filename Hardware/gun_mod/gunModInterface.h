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


        // These are pure virtual methods, so need to have some implmentation in derived classes
        IlockMap1 getILockStates( const std::string & name   ){ IlockMap1 r;return r; }
        IlockMap2 getILockStatesStr( const std::string & name){ IlockMap2 r;return r; }

        bool isModWarmedUp();
        bool isModNotWarmedUp();
        bool isModInTrig();
        bool isModInHVOn();
        bool isModInStandby();
        bool isModInOff();
        void modReset();
        bool modResetAndWait(const size_t waitTime);
        rfModStructs::GUN_MOD_STATE getModMainState() const;
        rfModStructs::GUN_MOD_ERR_STATE getModErrorState() const;


    private:
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs;
        bool allChidsInitialised;

        static void staticEntryGunModMonitor( const event_handler_args args);

        void updateMainState(const void * argsdbr);
        void updateWarmUpTime(const long val);
        void updateHexString(const event_handler_args& args);

        rfModStructs::GUN_MOD_ERR_STATE convertModErrorReadStr(const char * epicsModErrCodeString);
        rfModStructs::GUN_MOD_ERR_STATE convertModErrorRead(const double v);


//
//        void killMonitor( pilaserStructs::monitorStruct * ms );
//
        void initialise();
//        bool initObjects();
        void initChids();
        void addChannel(const std::string & pvRoot, rfModStructs::pvStruct & pv);
        void startMonitors();
//        // all client set functions route to here
//        bool setValue( pilaserStructs::pvStruct& pvs, double value);
//
        rfModStructs::gunModObject gunMod;
//
        std::vector<rfModStructs::monitorStruct*> continuousMonitorStructs;
        // all EPICS callbacks route here
//
        gunModConfigReader configReader; /// class member so we can pass in file path in ctor
//        ///message



};
#endif // VELA_MAG_INTERFACE_H
