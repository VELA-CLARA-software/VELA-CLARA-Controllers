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
//  FileName:    virtualCathodeInterface.h
//  Description:
//
//
//*/
#ifndef VELA_MAG_INTERFACE_H
#define VELA_MAG_INTERFACE_H
// djs
#include "interface.h"
#include "virtualCathodeStructs.h"
#include "structs.h"
#include "virtualCathodeConfigReader.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>
//______________________________________________________________________________
class virtualCathodeInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        virtualCathodeInterface(bool& show_messages,
                                bool& show_debug_messages,
                                const bool startVirtualMachine,
                                const bool shouldStartEPICs,
                                const std::string& configFile
                               );
        virtualCathodeInterface& virtualCathodeInterface::operator= ( const virtualCathodeInterface& other ) = delete;
        ~virtualCathodeInterface();

//        double getHpos();
//        double getVpos();
//        double getIntensity();
//        // setters , not sure of type for these parameters (or if they will exist...)
//        bool setHpos(double value);
//        bool setHpos(int value);
//        bool setVpos(double value);
//        bool setVpos(int value);
//        bool setIntensity(double value);
//        bool setIntensity(int value);
//        const pilaserStructs::pilaserObject &getPILObjConstRef();

//        /// These are pure virtual methods, so need to have some implmentation in derived classes
//        IlockMap1 getILockStates(const std::string & name   ){ IlockMap1 r;return r; }
//        IlockMap2 getILockStatesStr(const std::string & name){ IlockMap2 r;return r; }

    private:



//
//        void killMonitor(pilaserStructs::monitorStruct * ms );
//
        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string & pvRoot, virtualCathodeStructs::pvStruct & pv );
        void startMonitors();
        void addToBuffer();

//        // all client set functions route to here
//        bool setValue(pilaserStructs::pvStruct& pvs, double value);
//
        virtualCathodeStructs::virtualCathodeDataObject object;
//
        std::vector<virtualCathodeStructs::monitorStruct*> continuousMonitorStructs;
//        // all EPICS callbacks route here
        static void staticEntryMonitor(const event_handler_args args);
        void updateValue(const event_handler_args args, virtualCathodeStructs::PV_TYPE pv);
        virtualCathodeConfigReader configReader; /// class member so we can pass in file path in ctor
        ///message
};
//______________________________________________________________________________
#endif // VELA_MAG_INTERFACE_H
