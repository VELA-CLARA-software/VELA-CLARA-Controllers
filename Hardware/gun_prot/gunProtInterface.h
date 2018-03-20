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
//  FileName:    gunProtInterface.h
//  Description:
//
//
//*/
#ifndef GUN_PROT_INTERFACE_H
#define GUN_PROT_INTERFACE_H
// project includes
#include "interface.h"
#include "rfProtStructs.h"
#include "structs.h"
#include "gunProtConfigReader.h"
// stl inlcudes
#include <vector>
#include <string>
#include <atomic>
#include <map>
//______________________________________________________________________________
class gunProtInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        gunProtInterface::gunProtInterface();
        gunProtInterface(const std::string &config_file,
                         const bool startVirtualMachine,
                         const bool* show_messages_ptr,
                         const bool* show_debug_messages_ptr,
                         const bool shouldStartEPICs);

        ~gunProtInterface();

        bool isGood(const std::string& name);
        bool isNotGood(const std::string& name);
        bool isBad(const std::string& name);
        bool isNotGeneralProt(const std::string& name);
        bool isGeneralProt(const std::string& name);
        bool isNotEnableProt(const std::string& name);
        bool isEnableProt(const std::string& name);
        bool isProtOfType(const std::string& name, const rfProtStructs::RF_GUN_PROT_TYPE type);



        bool reset();
        bool reset(const std::vector<std::string>& names);
        bool reset(const std::string& name);

        bool enable(const std::string& name);
        bool enable(const std::vector<std::string>& names);
        bool enable();

        bool disable(const std::string& name);
        bool disable(const std::vector<std::string>& names);

        std::string gunProtInterface::getGeneralProtName();
        std::string gunProtInterface::getEnableProtName();
        std::string gunProtInterface::getCurrentModeProtName();

        const rfProtStructs::rfGunProtObject& getRFProtObjConstRef(const std::string& name);

        // These are pure virtual methods, so need to have some implmentation in derived classes
        IlockMap1 getILockStates(const std::string& name   );
        IlockMap2 getILockStatesStr(const std::string& name);

        bool allkeybitsaregood(const std::string& name);

    private:
        void killMonitor(rfProtStructs::monitorStruct* ms);

        void initialise();
        void initChids();
        void addChannel(const std::string& pvRoot, rfProtStructs::pvStruct& pv);
        void startMonitors();


        rfProtStructs::RF_GUN_PROT_TYPE currentMode;


        bool allkeybitsaregood(const rfProtStructs::rfGunProtObject& obj);

        bool sendCommand(const std::vector<chtype*>& CHTYPE,const std::vector<chid*>& CHID, const std::string& m1, const std::string& m2);
        bool sendCommand(const chtype& CHTYPE, const chid& CHID, const std::string& m1, const std::string& m2);

        std::map<std::string, rfProtStructs::rfGunProtObject> allGunProts;

        std::vector<rfProtStructs::monitorStruct*> continuousMonitorStructs;
        // all EPICS callbacks route here
        static void staticEntryMonitor(const event_handler_args args);

        void updateProtStatus(rfProtStructs::rfGunProtObject& obj,const unsigned short value);
        void updateProtStatus(rfProtStructs::rfGunProtObject& obj,const event_handler_args args);

        void updateCMIBits(rfProtStructs::rfGunProtObject& obj);

        gunProtConfigReader configReader; /// class member so we can pass in file path in ctor
};
//______________________________________________________________________________
#endif // GUN_PROT_INTERFACE_H
