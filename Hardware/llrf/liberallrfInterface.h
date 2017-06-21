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
#include "llrfStructs.h"
#include "structs.h"
#include "liberallrfConfigReader.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>

class liberallrfInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        liberallrfInterface::liberallrfInterface();
        liberallrfInterface(const std::string &llrfConf,
                      const bool  startVirtualMachine,
                      const bool* show_messages_ptr,
                      const bool* show_debug_messages_ptr,
                      const bool  shouldStartEPICs,
                      const llrfStructs::LLRF_TYPE type);

        ~liberallrfInterface();

        // GETTERS
        double getAmpFF();
        double getAmpSP();
        double getPhiFF();
        double getPhiSP();
        double getAmpLLRF();
        double getPhiLLRF();
        double getPhiDEG();
        double getAmpMVM();
        double getPulseLength();
        double getPulseOffset();
        double getPhiCalibration();
        double getAmpCalibration();
        double getCrestPhiLLRF();
        bool   isLocked();
        llrfStructs::LLRF_TYPE getType();
        size_t getPowerTraceLength();
        std::vector<double> getCavRevPower();
        std::vector<double> getCavFwdPower();
        std::vector<double> getKlyRevPower();
        std::vector<double> getKlyFwdPower();
        const llrfStructs::liberallrfObject& getLLRFObjConstRef();

        // SETTERS
        bool setPhiSP(double value);
        bool setPhiFF(double value);
        bool setAmpSP(double value);
        bool setAmpFF(double value);
        bool setAmpLLRF(double value);
        bool setPhiLLRF(double value);
        bool setAmpMVM(double value);
        bool setPhiDEG(double value);
        bool setPulseLength(double value);
        bool setPulseOffset(double value);
        void setPhiCalibration(double value);
        void setAmpCalibration(double value);
        void setCrestPhiLLRF(double value); // in LLRF units


        /// These are pure virtual methods, so need to have some implmentation in derived classes
        // ********this will likely need updating*******
        IlockMap1 getILockStates(const std::string & name   ){IlockMap1 r;return r;}
        IlockMap2 getILockStatesStr(const std::string & name){IlockMap2 r;return r;}


        // existential quantification
        bool Is_TracePV(llrfStructs::LLRF_PV_TYPE pv);
        bool IsNot_TracePV(llrfStructs::LLRF_PV_TYPE pv);


        bool isMonitoring(llrfStructs::LLRF_PV_TYPE pv);
        bool isNotMonitoring(llrfStructs::LLRF_PV_TYPE pv);

        void startTraceMonitoring();
        bool startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv);

        bool startCavFwdTraceMonitor();
        bool startCavRevTraceMonitor();
        bool startKlyFwdTraceMonitor();
        bool startKlyRevTraceMonitor();

        bool stopTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv);
        void stopTraceMonitoring();
        bool stopCavFwdTraceMonitor();
        bool stopCavRevTraceMonitor();
        bool stopKlyFwdTraceMonitor();
        bool stopKlyRevTraceMonitor();

    private:
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs,usingVirtualMachine;

        void killMonitor( llrfStructs::monitorStruct * ms );

        template<typename T>
        bool setValue( llrfStructs::pvStruct& pvs, T value);
        template<typename T>
        bool setValue2( llrfStructs::pvStruct& pvs, T value);

        // These are private version tha tto the converstion to LLRF units
        bool setPHIDEG();
        bool setAMPMVM();

        static void staticEntryLLRFMonitor(const event_handler_args args);

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel( const std::string & pvRoot, llrfStructs::pvStruct & pv );
        void startMonitors();

        void updateTrace(const event_handler_args args,llrfStructs::rf_trace_data& trace);

        //std::map< std::string, magnetStructs::magnetObject > allMagnetData;

        llrfStructs::liberallrfObject llrf;
        llrfStructs::LLRF_TYPE myLLRFType;

        std::vector< llrfStructs::monitorStruct * > continuousMonitorStructs;

        liberallrfConfigReader configReader; /// class member so we can pass in file path in ctor
        ///message
};
#endif // VELA_MAG_INTERFACE_H
