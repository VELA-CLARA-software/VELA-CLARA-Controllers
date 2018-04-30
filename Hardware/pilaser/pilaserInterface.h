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
//  Last edit:   29-03-2018
//  FileName:    pilaserInterface.h
//  Description:
//
//
//*/
#ifndef _PI_LASER_INTERFACE_H
#define _PI_LASER_INTERFACE_H
// djs
#include "interface.h"
#include "pilaserStructs.h"
#include "structs.h"
#include "pilaserConfigReader.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>
//______________________________________________________________________________
class pilaserInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        //pilaserInterface::pilaserInterface();
        pilaserInterface(const bool& show_messages,
                         const bool& show_debug_messages,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const std::string& configFile);

        ~pilaserInterface();

        bool setCharge(const double value);
        bool setIntensity(const double value);
        double getIntensity() const;
        std::string getName() const;
        HWC_ENUM::STATE getStatus() const;
        double getStabilisationStatus() const;
        bool isOn() const;
        bool isOff() const;
        bool isStabilisationOff() const;
        bool isStabilisationOn() const;
        bool disableStabilisation();
        bool enableStabilisation();
        const pilaserStructs::pilaserObject& getPILObjConstRef() const;

//        /// These are pure virtual methods, so need to have some implmentation in derived classes
//        IlockMap1 getILockStates(const std::string & name   ){ IlockMap1 r;return r; }
//        IlockMap2 getILockStatesStr(const std::string & name){ IlockMap2 r;return r; }



    private:

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string& pvRoot, pilaserStructs::pvStruct& pv);
        void startMonitors();
//        // all client set functions route to here
        bool setValue(pilaserStructs::pvStruct& pvs, const double value);

        void killMonitor(pilaserStructs::monitorStruct* ms);
//
//
        std::vector<pilaserStructs::monitorStruct*> continuousMonitorStructs;
//        // all EPICS callbacks route here
        static void staticEntryPILMonitor(const event_handler_args args);

        pilaserStructs::pilaserObject pilaser;
        pilaserConfigReader configReader;
};
//______________________________________________________________________________
#endif // _PI_LASER_INTERFACE_H
