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
//  Last edit:   13-04-2018
//  FileName:    liberallrfInterface.cpp
//  Description:
//
//
//
//*/
#ifndef __VELA_CLARA_LLRF_INTERFACE_H__
#define __VELA_CLARA_LLRF_INTERFACE_H__
// djs
#include "interface.h"
#include "llrfStructs.h"
#include "structs.h"
#include "liberallrfConfigReader.h"

#include "running_stat.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>
#include <vector>
#include <thread>
// timing



class liberallrfInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        liberallrfInterface::liberallrfInterface();
        liberallrfInterface(const std::string &llrfConf,
                            const bool  startVirtualMachine,
                            const bool& show_messages,
                            const bool& show_debug_messages_ptr,
                            const bool  shouldStartEPICs,
                            const llrfStructs::LLRF_TYPE type);

        ~liberallrfInterface();



/*
    ___  __        __   ___     __        ___  ___  ___  __      __    __  ___
     |  |__)  /\  /  ` |__     |__) |  | |__  |__  |__  |__)    /__` |  / |__
     |  |  \ /~~\ \__, |___    |__) \__/ |    |    |___ |  \    .__/ | /_ |___

        rolling average buffers, for individual tarces
*/
        bool setAllTraceBufferSize(const size_t value);
        bool setIndividualTraceBufferSize(const std::string& name, const size_t value);
        bool setIndividualTraceBufferSize(const size_t value);

/*
    ___  __        __   ___                          ___  __
     |  |__)  /\  /  ` |__      \  /  /\  |    |  | |__  /__`
     |  |  \ /~~\ \__, |___      \/  /~~\ |___ \__/ |___ .__/

    Simple processing for traces to find the max, mean etc.
*/
        double getTraceMax(const std::string& name);
        double getKlyFwdPwrMax();
        double getKlyFwdPhaMax();
        double getKlyRevPwrMax();
        double getKlyRevPhaMax();
        double getCavFwdPwrMax();
        double getCavFwdPhaMax();
        double getCavRevPwrMax();
        double getCavRevPhaMax();
        double getProbePwrMax();
        double getProbePhaMax();

//--------------------------------------------------------------------------------------------------
/*
          __  ___         ___     __             __   ___  __
     /\  /  `  |  | \  / |__     |__) |  | |    /__` |__  /__`
    /~~\ \__,  |  |  \/  |___    |    \__/ |___ .__/ |___ .__/
*/
//--------------------------------------------------------------------------------------------------
        void   setActivePulsePowerLimit(const double val);
        double getActivePulsePowerLimit() const;
        size_t getActivePulseCount() const;
        void addActivePulseCountOffset(const size_t val);
        void setActivePulseCount(const size_t value);
    private:
        void updateActivePulses();// private
    public:




        /// we will always have trace monitoring on???
        void startTraceMonitoring();
        bool startTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv);
        bool startTraceMonitoring(const std::string& name);

//--------------------------------------------------------------------------------------------------
/*       __   __                    __                ___  __        __   ___  __
        |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
        |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

// Set the parameters for the rolling avergaes
*/
        void updateTraceRollingAverage(llrfStructs::rf_trace_data& data );
        void clearAllRollingAverage();
        bool clearTraceRollingAverage(const std::string& name);
        void clearTraceRollingAverage(llrfStructs::rf_trace_data& trace); /// internal
        // Exposed to Python
        // setters
        void setKeepRollingAverageNoReset(const bool value);
        bool setKeepRollingAverageNoReset(const std::string&name, const bool value);
        void setShouldKeepRollingAverage();
        bool setShouldKeepRollingAverage(const std::string&name);
        void setShouldNotKeepRollingAverage();
        bool setShouldNotKeepRollingAverage(const std::string& name);
        bool setKeepRollingAverage(const std::string&name, bool value);
        bool setTraceRollingAverageSize(const std::string&name, const size_t value);
        void setAllRollingAverageSize(const size_t value);
        std::vector<double> getRollingAverage(const std::string&name)const;
        std::vector<std::vector<double>> getRollingAverageData(const std::string&name)const;

        size_t getTraceRollingAverageSize(const std::string&name)const;
        size_t getTraceRollingAverageCount(const std::string&name)const;
        bool isKeepingRollingAverage(const std::string&name)const;
        bool hasRollingAverage(const std::string&name)const;
//--------------------------------------------------------------------------------------------------


/*
    ___  __        __   ___           ___            __
     |  |__)  /\  /  ` |__      |\/| |__   /\  |\ | /__`
     |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \| .__/
*/
        bool setMeanStartEndTime(const double start, const double end, const std::string&name);
        bool setMeanStartIndex(const std::string&name, size_t  value);
        bool setMeanStopIndex(const std::string&name, size_t  value);
    private:
        void updateTraceCutMeans();
        void getTraceCutMean(llrfStructs::rf_trace_data& tracedata);
        std::tuple<size_t,double,double> dummy_amp_set_kly_fwd_rs_state;
    public:
        void setKeepKlyFwdPwrRS(bool val);
        void keepKlyFwdPwrRS();
        void dontKeepKlyFwdPwrRS();
        std::tuple<size_t,double,double> getKlyFwdPwrRSState(int ampSP_setting);
        void setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance);



        /* masks are based on rolling averages */
        void updateRollingAveragesAndMasks();
        void checkCollectingFutureTraces();
        void checkForOutsideMaskTrace();


        /* update the amp_set_kly_fwd running stats */
        void update_amp_set_kly_fwd_rs();


//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/

*/
        bool setUsePercentMask(const std::string& trace);
        bool setUseAbsoluteMask(const std::string& trace);
        bool setMaskValue(const std::string& trace,const double value);
        bool setMaskStartIndex(const std::string& trace, size_t value);
        bool setMaskEndIndex(const std::string& trace, size_t value);
        bool setMaskAbsMinValue(const std::string& name, double value);
        bool setMaskFloor(const std::string& name, double value);
        bool setMaskWindowIndices(const std::string& trace, size_t window_start, size_t window_end);
        bool setMaskStartTime(const std::string& trace, double value);
        bool setMaskEndTime(const std::string& trace, double value);
        bool setMaskWindowTimes(const std::string& trace, double window_start, double window_end);
        bool setShouldCheckMask(const std::string&name);
        bool setShouldNotCheckMask(const std::string&name);
        bool setCheckMask(const std::string& name, bool value);
        bool setHiMask(const std::string& name, const std::vector<double>& value);
        bool setLoMask(const std::string& name, const std::vector<double>& value);
        bool setNumContinuousOutsideMaskCount(size_t value);
        bool setNumContinuousOutsideMaskCount(const std::string&name, size_t value);
        bool clearMask(const std::string&name);
        bool setMaskParamatersIndices(const std::string& name, bool isPercent,
                                      double mask_value, double mask_floor, double mask_abs_min,
                                      size_t start, size_t end, size_t window_start, size_t window_end);
        bool setMaskParamatersTimes(const std::string& name, bool isPercent,
                                    double mask_value,   double mask_floor, double mask_abs_min,
                                    double start,        double end,
                                    double window_start, double window_end);
        void setGlobalCheckMask(bool value);
        void setGlobalShouldCheckMask();
        void setGlobalShouldNotCheckMask();

    private:// not availble higher up
        void setNewMask(llrfStructs::rf_trace_data& data);
    public:
//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    / _` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    \__> |___  |   |  |___ |  \ .__/

    //
*/
        std::vector<double> getLoMask(const std::string&name)const;
        std::vector<double> getHiMask(const std::string&name)const;

        double getMaskValue(const std::string& name);
        size_t getMaskStartIndex(const std::string& name);
        size_t getMaskEndIndex(const std::string& name);
        double getMaskFloor(const std::string& name);
        size_t getMaskWindowStartIndex(const std::string& name);
        size_t getMaskWindowEndIndex(const std::string& name);
        double getMaskStartTime(const std::string& name);
        double getMaskEndTime(const std::string& name);
        double getMaskWindowStartTime(const std::string& name);
        double getMaskWindowEndTime(const std::string& name);
//--------------------------------------------------------------------------------------------------
        bool isCheckingMask(const std::string& name);
        bool isNotCheckingMask(const std::string& name);
        bool isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);
//--------------------------------------------------------------------------------------------------
        std::vector<double> getCavRevPower()const;
        std::vector<double> getCavFwdPower()const;
        std::vector<double> getKlyRevPower()const;
        std::vector<double> getKlyFwdPower()const;
        std::vector<double> getCavRevPhase()const;
        std::vector<double> getCavFwdPhase()const;
        std::vector<double> getKlyRevPhase()const;
        std::vector<double> getKlyFwdPhase()const;
        std::vector<double> getProbePower()const;
        std::vector<double> getProbePhase()const;
//--------------------------------------------------------------------------------------------------


        bool set_mask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,
                      const double value, const std::string& name,const bool isPercent);

        bool setCavRevPwrHiMask(const std::vector<double>& value);
        bool setCavRevPwrLoMask(const std::vector<double>& value);

        bool setCavRevPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,
                                     const size_t s4,const double value);
        bool setCavRevPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,
                                      const size_t s4,const double value);
        bool setCavFwdPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,
                                     const size_t s4,const double value);
        bool setCavFwdPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,
                                      const size_t s4,const double value);
        bool setAbsoluteTimeMask(const double s1,const double s2,const double s3,
                                 const double s4,const double value2,const  std::string& name);
        bool setPercentTimeMask(const double s1,const double s2,const double s3,
                                const double s4,const double value,const  std::string& name);
        bool setInfiniteMasks(const std::string& name);
        bool setPercentMask(const size_t s1,const size_t s2,const size_t s3,
                            const size_t s4,const double value,const std::string& name);
        bool setAbsoluteMask(const size_t s1,const size_t s2,const size_t s3,
                             const size_t s4,const double value,const std::string& name);
        /* all mask setting ends up here */


        size_t getOutsideMaskEventCount()const;



        // MASK CHECKING
        int updateIsTraceInMask(llrfStructs::rf_trace_data& trace);

        //bool liberallrfInterface::shouldCheckMasks(llrfStructs::rf_trace_data& trace)


        void handleTraceInMaskResult(llrfStructs::rf_trace_data& trace, int result);
        void handlePassedMask(llrfStructs::rf_trace_data& trace);
        void handleFailedMask(llrfStructs::rf_trace_data& trace);




        void setTracesToSaveOnOutsideMaskEvent(const std::vector<std::string>& name);
        void setTracesToSaveOnOutsideMaskEvent(const std::string& name);
        void setNumExtraTracesOnOutsideMaskEvent(const size_t value);
        size_t getNumExtraTracesOnOutsideMaskEvent()const;

        bool setDropAmpOnOutsideMaskEvent(const std::string& name, bool state, double amp_val = 0.0);
        bool setDropAmpOnOutsideMaskEventValue(const std::string& name, double amp_val);

        std::vector<std::string> getTracesToSaveOnOutsideMaskEvent();
        size_t getOutsideMaskEventDataSize();

        llrfStructs::outside_mask_event_data  getOutsideMaskEventData();
        const llrfStructs::outside_mask_event_data&  getOutsideMaskEventData_CRef();

        bool isOutsideMaskEventDataCollecting()const;
        bool canGetOutsideMaskEventData()const;

        void newOutsideMaskEvent(const llrfStructs::rf_trace_data& trace);

        void copyTraceDataToOMED();
        void OME_interlock(const llrfStructs::setAmpHP_Struct& s);




        bool setInterlockActive();
        bool setInterlockNonActive();

        bool isInterlockActive()const;
        bool isInterlockNotActive()const;

        bool lockAmpFF();
        bool lockPhaseFF();
        bool unlockAmpFF();
        bool unlockPhaseFF();
        bool enableRFOutput();
        bool isRFOutput() const;
        bool isNotRFOutput() const;
        bool RFOutput()const;
        bool isFFLocked()const;
        bool isFFNotLocked()const;
        bool isAmpFFLocked()const;
        bool isAmpFFNotLocked()const;
        bool isPhaseFFLocked()const;
        bool isPhaseFFNotLocked()const;


        bool setUnwrapPhaseTolerance(const double value);
        bool setUnwrapPhaseTolerance(const std::string& name,const double value);

/*
       _________.__               .__             ________        __    __
      /   _____/|__| _____ ______ |  |   ____    /  _____/  _____/  |__/  |_  ___________  ______
      \_____  \ |  |/     \\____ \|  | _/ __ \  /   \  ____/ __ \   __\   __\/ __ \_  __ \/  ___/
      /        \|  |  Y Y  \  |_> >  |_\  ___/  \    \_\  \  ___/|  |  |  | \  ___/|  | \/\___ \
     /_______  /|__|__|_|  /   __/|____/\___  >  \______  /\___  >__|  |__|  \___  >__|  /____  >
             \/          \/|__|             \/          \/     \/                \/           \/

    i.e. things to expose to python
*/
        double getAmpFF()const;
        double getAmpSP()const;
        double getPhiFF()const;
        double getPhiSP()const;
        double getAmpLLRF()const;
        double getPhiLLRF()const;
        double getPhiDEG()const;
        double getAmpMVM()const;
        double getPulseLength()const;
        double getPulseOffset()const;
        double getPhiCalibration()const;
        double getAmpCalibration()const;
        double getCrestPhiLLRF()const;
        llrfStructs::LLRF_TYPE getType()const;
        size_t getTraceLength()const;
        /*
            convert trace index to time and vice-versa
        */
        size_t getIndex(const double time)const;
        double getTime(const size_t  time)const;

        size_t getPulseCount()const;


//        std::vector<std::string> getChannelNames()const;
        std::vector<std::string> getTraceNames()const;

        std::vector<double> getTraceValues(const std::string& name)const;
        std::vector<double> getTimeVector()const;

        std::vector<double> getAverageTraceData(const std::string& name)const;

        std::vector< std::pair<std::string, std::vector<double>> > getOneTraceData()const;

        llrfStructs::LLRF_SCAN getSCAN(const event_handler_args& args)const;
        llrfStructs::LLRF_ACQM getACQM(const event_handler_args& args)const;

//        llrfStructs::rf_trace getTraceData(const std::string& name);
//        std::vector<llrfStructs::rf_trace> getTraceBuffer(const std::string& name);


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

        const llrfStructs::liberallrfObject& getLLRFObjConstRef();
        const llrfStructs::rf_trace_data& getTraceDataConstRef(const std::string& name);


//        llrfStructs::rf_trace getCavRevPowerData();
//        llrfStructs::rf_trace getCavFwdPowerData();
//        llrfStructs::rf_trace getKlyRevPowerData();
//        llrfStructs::rf_trace getKlyFwdPowerData();
//        llrfStructs::rf_trace getCavRevPhaseData();
//        llrfStructs::rf_trace getCavFwdPhaseData();
//        llrfStructs::rf_trace getKlyRevPhaseData();
//        llrfStructs::rf_trace getKlyFwdPhaseData();
//
//        std::vector<llrfStructs::rf_trace> getCavRevPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getCavFwdPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyRevPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyFwdPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getCavRevPhaseBuffer();
//        std::vector<llrfStructs::rf_trace> getCavFwdPhaseBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyRevPhaseBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyFwdPhaseBuffer();



//        bool setCavFwdPwrSCAN(short val);


        llrfStructs::LLRF_PV_TYPE getLLRFPVType(const std::string& name)const;

        llrfStructs::LLRF_PV_TYPE getEVID_pv(llrfStructs::LLRF_PV_TYPE pv);
//        llrfStructs::LLRF_PV_TYPE getSCAN_pv(llrfStructs::LLRF_PV_TYPE pv);

        std::string getLLRFChannelName(const llrfStructs::LLRF_PV_TYPE pv)const;

        size_t getNumBufferTraces(const std::string&name)const;

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



/*   _____  _____          _   _
    / ____|/ ____|   /\   | \ | |
   | (___ | |       /  \  |  \| |
    \___ \| |      / /\ \ | . ` |
    ____) | |____ / ____ \| |\  |
   |_____/ \_____/_/    \_\_| \_|
*/
        bool setTraceSCAN(const std::string& trace, const llrfStructs::LLRF_SCAN value);
        bool setAllSCANToPassive();
        bool setAllTraceSCAN(const llrfStructs::LLRF_SCAN value);

        bool setTORSCANToIOIntr();
        bool setTORSCANToPassive();
        bool resetTORSCANToIOIntr();
        bool setTORACQMEvent();
        void updateSCAN(const std::string& name, const llrfStructs::LLRF_PV_TYPE pv, const event_handler_args& args);


        bool setInfiniteMaskEndByPower(const std::string& power_trace,const std::string& phase_trace,const double level);
        bool disableInfiniteMaskEndByPower(const std::string& phase_trace);
        size_t getMaskInfiniteEndByPowerIndex(const std::string& name);
        double getMaskInfiniteEndByPowerTime(const std::string& name);



        void startTimer();
        void offsetTimer(long long value);
        long long elapsedTime();

        double getBreakDownRate();

//        bool setNumContinuousOutsideMaskCount(const std::string& name, size_t val);

        bool externalTriggerOff();
        bool externalTriggerOn();

        bool trigOff();
        bool trigExt();
        bool trigInt();
        llrfStructs::TRIG getTrigSource()const;

//        bool shouldCheckMasks(const std::string& name);



        //  quantification
//        bool Is_TracePV(const llrfStructs::LLRF_PV_TYPE pv);
//        bool IsNot_TracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool Is_EVID_PV(const llrfStructs::LLRF_PV_TYPE pv);
        bool IsNot_EVID_PV(const llrfStructs::LLRF_PV_TYPE pv);
        bool Is_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv);
        bool IsNot_SCAN_PV(llrfStructs::LLRF_PV_TYPE pv);
        bool Is_Time_Vector_PV(const llrfStructs::LLRF_PV_TYPE pv);


        //bool isPhaseTrace(const std::string& name);

//        bool isMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
//        bool isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
//
//        bool isMonitoring(const std::string& name);
//        bool isNotMonitoring(const std::string& name);




        // start trace monitoring (not automatic as mostly not needed)
//        bool startCavFwdTraceMonitor();
//        bool startCavRevTraceMonitor();
//        bool startKlyFwdTraceMonitor();
//        bool startKlyRevTraceMonitor();
//        // stop trace monitoring (not automatic as mostly not needed)
//        bool stopTraceMonitoring(llrfStructs::LLRF_PV_TYPE pv);
//        bool stopTraceMonitoring(const std::string& name);
//        void stopTraceMonitoring();
//        bool stopCavFwdTraceMonitor();
//        bool stopCavRevTraceMonitor();
//        bool stopKlyFwdTraceMonitor();
//        bool stopKlyRevTraceMonitor();

        std::string fullLLRFTraceName(const std::string& name)const;
        std::string shortLLRFTraceName(const std::string& name_in)const;


        /// These are pure virtual methods, so need to have some implmentation in derived classes
        // ********this will likely need updating*******
        IlockMap1 getILockStates(const std::string& name  ){IlockMap1 r;return r;}
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
        bool updatePHIDEG();
        bool updateAMPMVM();

        void updateTimeVector(const event_handler_args& args);

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string& pvRoot, llrfStructs::pvStruct & pv);
        void startMonitors();
        void addTo_continuousMonitorStructs(const std::pair<llrfStructs::LLRF_PV_TYPE,llrfStructs::pvStruct>& it);


        /* EPICS Callbacks - parameters are updated form here */
        static void staticEntryLLRFMonitor(const event_handler_args args);
        void updateLLRFValue(const llrfStructs::LLRF_PV_TYPE pv, const std::string& objName, const event_handler_args& args);

        void updateAllTracesEVID(const event_handler_args& args);
        void updateAllTraces(const event_handler_args& args);
        void unwrapPhaseTrace(llrfStructs::rf_trace_data& data);




        int evid_miss_count;// = -2;
        int data_miss_count;// = -2;

        int evid_call_count;// = -2;
        int data_call_count;// = -2;


        bool can_ran_off;

        runningStat data_time_RS;


//        void updateEVID(const event_handler_args& args,llrfStructs::rf_trace_data& trace);
//        void updateSCAN(const event_handler_args& args,llrfStructs::rf_trace_data& trace);

        void updateTrigState(const event_handler_args& args);
        void updateValues(const event_handler_args& args,llrfStructs::rf_trace& trace);
//        void updateTrace(const event_handler_args& args, llrfStructs::rf_trace_data& trace);
//        int updateIsTraceInMask(llrfStructs::rf_trace_data& trace);
//        void handleTraceInMaskResult(llrfStructs::rf_trace_data& trace, int result);
//        void handlePassedMask(llrfStructs::rf_trace_data& trace);
//        void handleFailedMask(llrfStructs::rf_trace_data& trace);
        void updateTraceIndex(size_t& index,const size_t trace_size);
        void updateTraceIndex(int& index,const size_t trace_size);



//        bool shouldCheckMasks(llrfStructs::rf_trace_data& trace);


        // OLD
        //void addToOutsideMaskTraces(llrfStructs::rf_trace_data& trace,const std::string& name);




        bool setMaskInfiniteEnd(const std::string& trace_name, size_t index);


        llrfStructs::liberallrfObject llrf;
        llrfStructs::LLRF_TYPE myLLRFType;

        std::vector< llrfStructs::monitorStruct * > continuousMonitorStructs;

        std::vector<llrfStructs::setAmpHP_Struct>  setAmpHP_Threads;
        void setAmpSPCallback(const double value);
        //static void staticEntrySetAmp(liberallrfInterface* interface, const double value);
        static void staticEntrySetAmp(llrfStructs::setAmpHP_Struct& s);
        void kill_finished_setAmpHP_threads();
        bool resetToValueFromInterlockActive(const double value);
        bool resetToValueFromTrigOff(const double value);





//        void set_evid_ID_SET(llrfStructs::rf_trace_data& trace);

        std::stringstream outside_mask_trace_message;

        // duymmy data to return when real data doesn't exist
        const llrfStructs::rf_trace_data dummy_trace_data;
//        const llrfStructs::outside_mask_trace dummy_outside_mask_trace;


        liberallrfConfigReader configReader; /// class member so we can pass in file path in ctor
        ///message

        std::pair<epicsTimeStamp, std::vector<double>> temp_all_trace_item;
        std::pair<epicsTimeStamp, std::vector<double>> temp_trace_item;
        std::pair<std::string, std::vector<double>> temp_OMED_trace_item;


};
#endif // VELA_MAG_INTERFACE_H
