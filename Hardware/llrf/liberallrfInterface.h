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
#include <vector>
#include <thread>

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
        llrfStructs::LLRF_TYPE getType();
        size_t getTraceLength();
        size_t getNumOutsideMaskTraces();
        size_t getActivePulseCount();
        void addPulseCountOffset(size_t val);

        void setActivePulsePowerLimit(const double& val);
        double getActivePulsePowerLimit();

        size_t getIndex(const double time);
        double getTime(const size_t  time);
        bool setAbsoluteTimeMask(const double s1,const double s2,const double s3,
                                 const double s4,const double value2,const  std::string& name);
        bool setPercentTimeMask(const double s1,const double s2,const double s3,
                                const double s4,const double value2,const  std::string& name);


        size_t getShotCount(const std::string& name);

        std::vector<llrfStructs::outside_mask_trace>  getOutsideMaskData();
        llrfStructs::outside_mask_trace getOutsideMaskData(const size_t part);
        bool isOutsideMaskDataFinishedCollecting(size_t part);

        std::vector<std::string> getChannelNames();
        std::vector<std::string> getTraceNames();

        std::vector<double> getTraceValues(const std::string& name);
        std::vector<double> getAverageTraceData(const std::string& name);
        llrfStructs::rf_trace getTraceData(const std::string& name);
        std::vector<llrfStructs::rf_trace> getTraceBuffer(const std::string& name);

        void setTracesToSaveOnBreakDown(const std::vector<std::string>& name);
        std::vector<std::string> getTracesToSaveOnBreakDown();

        std::vector<double> getCavRevPower();
        std::vector<double> getCavFwdPower();
        std::vector<double> getKlyRevPower();
        std::vector<double> getKlyFwdPower();
        std::vector<double> getCavRevPhase();
        std::vector<double> getCavFwdPhase();
        std::vector<double> getKlyRevPhase();
        std::vector<double> getKlyFwdPhase();

        std::vector<double> getProbePower();
        std::vector<double> getProbePhase();


        std::vector<double> getCavRevPowerAv();
        std::vector<double> getCavFwdPowerAv();
        std::vector<double> getKlyRevPowerAv();
        std::vector<double> getKlyFwdPowerAv();
        std::vector<double> getCavRevPhaseAv();
        std::vector<double> getCavFwdPhaseAv();
        std::vector<double> getKlyRevPhaseAv();
        std::vector<double> getKlyFwdPhaseAv();

        std::vector<double> getProbePowerAv();
        std::vector<double> getProbePhaseAv();


        llrfStructs::rf_trace getCavRevPowerData();
        llrfStructs::rf_trace getCavFwdPowerData();
        llrfStructs::rf_trace getKlyRevPowerData();
        llrfStructs::rf_trace getKlyFwdPowerData();
        llrfStructs::rf_trace getCavRevPhaseData();
        llrfStructs::rf_trace getCavFwdPhaseData();
        llrfStructs::rf_trace getKlyRevPhaseData();
        llrfStructs::rf_trace getKlyFwdPhaseData();

        std::vector<llrfStructs::rf_trace> getCavRevPowerBuffer();
        std::vector<llrfStructs::rf_trace> getCavFwdPowerBuffer();
        std::vector<llrfStructs::rf_trace> getKlyRevPowerBuffer();
        std::vector<llrfStructs::rf_trace> getKlyFwdPowerBuffer();
        std::vector<llrfStructs::rf_trace> getCavRevPhaseBuffer();
        std::vector<llrfStructs::rf_trace> getCavFwdPhaseBuffer();
        std::vector<llrfStructs::rf_trace> getKlyRevPhaseBuffer();
        std::vector<llrfStructs::rf_trace> getKlyFwdPhaseBuffer();

        const llrfStructs::liberallrfObject& getLLRFObjConstRef();
        llrfStructs::LLRF_PV_TYPE getLLRFPVType(const std::string& name);

        llrfStructs::LLRF_PV_TYPE getEVID_pv(llrfStructs::LLRF_PV_TYPE pv);
        llrfStructs::LLRF_PV_TYPE getSCAN_pv(llrfStructs::LLRF_PV_TYPE pv);

        std::string getLLRFChannelName(const llrfStructs::LLRF_PV_TYPE pv);
        std::vector<double> getLoMask(const std::string&name);
        std::vector<double> getHiMask(const std::string&name);
        size_t getNumBufferTraces(const std::string&name);

        // SETTERS
        bool setPhiSP(double value);
        bool setPhiFF(double value);
        bool setAmpSP(double value);
        bool setAmpHP(double value);
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

        bool setTraceSCAN(const std::string& trace, const llrfStructs::LLRF_SCAN value);
        bool setAllTraceSCAN( const llrfStructs::LLRF_SCAN value);

        bool setCavRevPwrHiMask(const std::vector<double>& value);
        bool setCavRevPwrLoMask(const std::vector<double>& value);
        bool setHighMask(const std::string&name,const std::vector<double>& value);
        bool setLowMask(const std::string&name,const std::vector<double>& value);


        bool setCavRevPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);
        bool setCavRevPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);
        bool setCavFwdPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);
        bool setCavFwdPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);

        bool setPercentMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string& name);
        bool setAbsoluteMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string& name);

        void startTimer();
        void offsetTimer(long long value);
        long long elapsedTime();

        double getBreakDownRate();

        bool setDropAmpOnOutsideMaskDetection(const std::string& name, bool state, double amp_val = 0.0);
        bool setDropAmpValue(const std::string& name, double amp_val);

        bool setNumContinuousOutsideMaskCount(const std::string& name, size_t val);


        bool trigOff();
        bool trigExt();
        bool trigInt();
        llrfStructs::TRIG getTrigSource();


        bool clearMask(const std::string&name);
        bool clearRollingAverage(const std::string&name);
        //bool clearRollingAverage();
        bool setMeanStartEndTime(const double start, const double end, const std::string&name);

        bool setMeanStartIndex(const std::string&name, size_t  value);
        bool setMeanStopIndex(const std::string&name, size_t  value);

        bool setNumBufferTraces(const std::string&name,const size_t value);
        void setNumBufferTraces(const size_t value);


        bool setCheckMask(const std::string&name, bool value);
        void setGlobalCheckMask(bool value);

        bool setShouldCheckMask(const std::string&name);
        void setGlobalShouldCheckMask();

        bool setShouldNotCheckMask(const std::string&name);
        void setGlobalShouldNotCheckMask();

        void resetAverageTraces();
        void setShouldKeepRollingAverage();
        void setShouldNotKeepRollingAverage();


        bool setKeepRollingAverage(const std::string&name, bool value);
        bool setShouldKeepRollingAverage(const std::string&name);
        bool setShouldNotKeepRollingAverage(const std::string&name);

        bool setNumRollingAverageTraces(const std::string&name,const  size_t value );
        void setNumRollingAverageTraces(const size_t value);
        size_t getNumRollingAverageTraces(const std::string&name);

        //  quantification
        bool Is_TracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool IsNot_TracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool Is_EVID_PV(const llrfStructs::LLRF_PV_TYPE pv);
        bool IsNot_EVID_PV(const llrfStructs::LLRF_PV_TYPE pv);
        bool Is_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv);
        bool IsNot_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv);
        bool Is_Time_Vector_PV(const llrfStructs::LLRF_PV_TYPE pv);

        bool setMaskFloor(const std::string& name, double value);

        bool shouldCheckMasks(const std::string& name);
        void setNumExtraTraces(size_t value);
        size_t getNumExtraTraces();


        bool isMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv);

        bool isMonitoring(const std::string& name);
        bool isNotMonitoring(const std::string& name);

        bool isCheckingMask(const std::string& name);
        bool isNotCheckingMask(const std::string& name);
        bool isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);

        bool isFFLocked();
        bool isFFNotLocked();
        bool RFOutput();
        bool interlockActive();
        bool interlockNotActive();

        // start trace monitoring (not automatic as mostly not needed)
        void startTraceMonitoring();
        bool startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv);
        bool startTraceMonitoring(const std::string& name);

        bool startCavFwdTraceMonitor();
        bool startCavRevTraceMonitor();
        bool startKlyFwdTraceMonitor();
        bool startKlyRevTraceMonitor();

        // stop trace monitoring (not automatic as mostly not needed)
        bool stopTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv);
        bool stopTraceMonitoring(const std::string& name);
        void stopTraceMonitoring();

        bool stopCavFwdTraceMonitor();
        bool stopCavRevTraceMonitor();
        bool stopKlyFwdTraceMonitor();
        bool stopKlyRevTraceMonitor();

        std::string fullCavityTraceName(const std::string& name);

        /// These are pure virtual methods, so need to have some implmentation in derived classes
        // ********this will likely need updating*******
        IlockMap1 getILockStates(const std::string& name   ){IlockMap1 r;return r;}
        IlockMap2 getILockStatesStr(const std::string& name){IlockMap2 r;return r;}

    private:
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs,usingVirtualMachine;

        size_t evid_id;


        bool first_pulse,can_start_new_thread,evid_ID_SET;
        size_t initial_pulsecount,last_pulse_count;

        void killMonitor(llrfStructs::monitorStruct* ms);

        template<typename T>
        bool setValue(llrfStructs::pvStruct& pvs,T value);
        template<typename T>
        bool setValue2(llrfStructs::pvStruct& pvs,T value);

        // These are private version that do the converstion to LLRF units
        bool setPHIDEG();
        bool setAMPMVM();

        static void staticEntryLLRFMonitor(const event_handler_args args);

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string& pvRoot, llrfStructs::pvStruct & pv );
        void startMonitors();

        void updateEVID(const event_handler_args& args,llrfStructs::rf_trace_data& trace);
        void updateSCAN(const event_handler_args& args,llrfStructs::rf_trace_data& trace);
//        void updateTraceValues(const event_handler_args& args,llrfStructs::rf_trace& trace);
        void updateValues(const event_handler_args& args,llrfStructs::rf_trace& trace);
        void updateTrace(const event_handler_args& args, llrfStructs::rf_trace_data& trace);
        //void updateTimeVector(const event_handler_args& args);

        void updateTraceIndex(size_t& index,const size_t trace_size);
        void updateTraceIndex(int& index,const size_t trace_size);
        void updateTraceCutMean(llrfStructs::rf_trace& trace);

        bool isTraceInMask(llrfStructs::rf_trace_data& trace);
        bool shouldCheckMasks(llrfStructs::rf_trace_data& trace);
        void addToOutsideMaskTraces(llrfStructs::rf_trace_data& trace,const std::string& name);
        bool shouldSubtractTraceFromRollingAverage(llrfStructs::rf_trace_data& trace);
        void calcRollingAverage(llrfStructs::rf_trace_data& trace);
        void updateRollingSum(llrfStructs::rf_trace_data& trace);
        void resetAverageTraces(llrfStructs::rf_trace_data& trace);


        void updateActivePulseCount(const std::string& evid);

        // breakdown timer duration

        llrfStructs::liberallrfObject llrf;
        llrfStructs::LLRF_TYPE myLLRFType;

        std::vector< llrfStructs::monitorStruct * > continuousMonitorStructs;

        std::vector<llrfStructs::setAmpHP_Struct>  setAmpHP_Threads;
        void setAmpSPCallback(const double value);
        //static void staticEntrySetAmp(liberallrfInterface* interface, const double value);
        static void staticEntrySetAmp(llrfStructs::setAmpHP_Struct & );
        void kill_finished_setAmpHP_threads();

        void set_evid_ID_SET(llrfStructs::rf_trace_data& trace);


        liberallrfConfigReader configReader; /// class member so we can pass in file path in ctor
        ///message
};
#endif // VELA_MAG_INTERFACE_H
