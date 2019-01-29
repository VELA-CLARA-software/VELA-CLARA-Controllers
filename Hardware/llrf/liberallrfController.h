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
//  Last edit:   16-10-2018
//  FileName:    liberaLLRFController.h
//  Description:
//
//
//
//*/
#ifndef _LIBERA_LLRF_CONTROLLER_H_
#define _LIBERA_LLRF_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "liberallrfInterface.h"
#include "controller.h"
// boost.python
#ifdef BUILD_DLL
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/overloads.hpp>
#endif

class liberaLLRFController : public controller
{
    public:
        //liberaLLRFController();
        liberaLLRFController(bool& show_messages,
                             bool& show_debug_messagese,
                             const std::string & config,
                             const bool startVirtualMachine,
                             const bool shouldStartEPICs,
                             const llrfStructs::LLRF_TYPE type,
                             const HWC_ENUM::CONTROLLER_TYPE c_type,
                             const std::string& name);

        ~liberaLLRFController();

/*
    ___  __        __   ___     __        ___  ___  ___  __      __    __  ___
     |  |__)  /\  /  ` |__     |__) |  | |__  |__  |__  |__)    /__` |  / |__
     |  |  \ /~~\ \__, |___    |__) \__/ |    |    |___ |  \    .__/ | /_ |___
*/
        bool setAllTraceBufferSize(const size_t value);
        bool setIndividualTraceBufferSize(const std::string& name, const size_t value);
        bool setIndividualTraceBufferSize(const size_t value);

//--------------------------------------------------------------------------------------------------
/*       __   __                    __                ___  __        __   ___  __
        |__) /  \ |    |    | |\ | / _`     /\  \  / |__  |__)  /\  / _` |__  /__`
        |  \ \__/ |___ |___ | | \| \__>    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

  methods for the trace rolling avergaes
*/
        void clearAllRollingAverage();
        bool clearTraceRollingAverage(const std::string& name);
        void setKeepRollingAverageNoReset(const bool value);
        bool setKeepRollingAverageNoReset(const std::string&name, const bool value);
        void setShouldKeepRollingAverage();
        bool setShouldKeepRollingAverage(const std::string&name);
        void setShouldNotKeepRollingAverage();
        bool setKeepRollingAverage(const std::string&name, bool value);
        bool setTraceRollingAverageSize(const std::string&name, const size_t value);
        void setAllRollingAverageSize(const size_t value);
        bool setShouldNotKeepRollingAverage(const std::string& name);
        std::vector<double> getRollingAverage(const std::string&name)const;
        std::vector<std::vector<double>> getRollingAverageData(const std::string&name)const;
#ifdef BUILD_DLL
        boost::python::list getRollingAverage_Py(const std::string&name)const;
        boost::python::list getRollingAverageData_Py(const std::string&name)const;
#endif
        size_t getTraceRollingAverageSize(const std::string&name)const;
        size_t getTraceRollingAverageCount(const std::string&name)const;
        bool isKeepingRollingAverage(const std::string&name)const;
        bool hasRollingAverage(const std::string&name)const;
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    /__` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    .__/ |___  |   |  |___ |  \ .__/

*/
        bool setUsePercentMask(const std::string& name);
        bool setUseAbsoluteMask(const std::string& name);
        bool setMaskValue(const std::string& name, double value);
        bool setMaskAbsMinValue(const std::string& name, double value);
        bool setMaskStartIndex(const std::string& name, size_t value);
        bool setMaskEndIndex(const std::string& name, size_t value);
        bool setMaskFloor(const std::string& name, double value);
        bool setMaskWindowIndices(const std::string& name, size_t window_start, size_t window_end);
        bool setMaskStartTime(const std::string& name, double value);
        bool setMaskEndTime(const std::string& name, double value);
        bool setMaskWindowTimes(const std::string& name, double window_start, double window_end);
        bool setShouldCheckMask(const std::string&name);
        bool setShouldNotCheckMask(const std::string&name);
        bool setCheckMask(const std::string&name, bool value);
        bool setMaskParamatersIndices(const std::string& name, bool isPercent,
                                      double mask_value, double mask_floor, double mask_abs_min,
                                      size_t start, size_t end, size_t window_start, size_t window_end);
        bool setMaskParamatersTimes(const std::string& name, bool isPercent,
                                    double mask_value, double mask_floor, double mask_abs_min,
                                    double start, double end,
                                    double window_start, double window_end);
        bool setHiMask(const std::string&name,const std::vector<double>& value);
        bool setLoMask(const std::string&name,const std::vector<double>& value);
#ifdef BUILD_DLL
        bool setHiMask_Py(const std::string&name, boost::python::list& value);
        bool setLoMask_Py(const std::string&name, boost::python::list& value);
#endif
        bool setNumContinuousOutsideMaskCount(size_t value);
        bool setNumContinuousOutsideMaskCount(const std::string&name, size_t value);
        void setGlobalCheckMask(bool value);
        void setGlobalShouldCheckMask();
        void setGlobalShouldNotCheckMask();
        bool clearMask(const std::string&name);

//--------------------------------------------------------------------------------------------------
/*             __           __   ___ ___ ___  ___  __   __
    |\/|  /\  /__` |__/    / _` |__   |   |  |__  |__) /__`
    |  | /~~\ .__/ |  \    \__> |___  |   |  |___ |  \ .__/

    //
*/
        std::vector<double> getHiMask(const std::string&name);
        std::vector<double> getLoMask(const std::string&name);
#ifdef BUILD_DLL
        boost::python::list getHiMask_Py(const std::string&name);
        boost::python::list getLoMask_Py(const std::string&name);
#endif
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

//--------------------------------------------------------------------------------------------------
/*        __  ___         ___     __             __   ___  __
     /\  /  `  |  | \  / |__     |__) |  | |    /__` |__  /__`
    /~~\ \__,  |  |  \/  |___    |    \__/ |___ .__/ |___ .__/
*/
//--------------------------------------------------------------------------------------------------
        void setActivePulsePowerLimit(const double val);
        double getActivePulsePowerLimit()const;
        size_t getActivePulseCount()const;
        void addActivePulseCountOffset(const size_t val);
        void setActivePulseCount(const size_t value);






        void setTracesToSaveOnOutsideMaskEvent(const std::vector<std::string>& name);
        void setTracesToSaveOnOutsideMaskEvent_2(const std::string& name);

        void setNumExtraTracesOnOutsideMaskEvent(const size_t value);
        size_t getNumExtraTracesOnOutsideMaskEvent()const;


        size_t getOutsideMaskEventDataSize();


        llrfStructs::outside_mask_event_data  getOutsideMaskEventData();
        const llrfStructs::outside_mask_event_data&  getOutsideMaskEventData_CRef();


        //llrfStructs::outside_mask_trace getOutsideMaskEventDataPart(const size_t part);

        bool isOutsideMaskEventDataCollecting()const;
        bool canGetOutsideMaskEventData()const;
        size_t getPulseCount()const;
        bool setDropAmpOnOutsideMaskEvent(const std::string& name, bool state, double amp_val = 0.0);
        bool setDropAmpOnOutsideMaskEventValue(const std::string& name, double amp_val);


        std::vector<std::string> getTracesToSaveOnOutsideMaskEvent();

#ifdef BUILD_DLL
        void setTracesToSaveOnOutsideMaskEvent_Py(const boost::python::list& name);
        boost::python::dict getOutsideMaskEventData_Py();
        boost::python::dict getOutsideMaskEventDataPart_Py(const size_t part);
        boost::python::list getTracesToSaveOnOutsideMaskEvent_Py();
        // converts the Outside_Mask_Data to a python dictionary
        // callled be
    private:
        boost::python::dict getOMT_Dict(const llrfStructs::outside_mask_event_trace_data& omt);
    public:
#endif








        bool setPercentMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name);
        bool setAbsoluteMask(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value2,const std::string name);

        bool setHighMask(const std::string& name, const std::vector<double>& value);
        bool setLowMask(const std::string& name, const std::vector<double>& value);

        bool setCavRevPwrHiMask(const std::vector<double>& value);
        bool setCavRevPwrLoMask(const std::vector<double>& value);
        bool setCavRevPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);
        bool setCavRevPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);
        bool setCavFwdPwrMaskPercent(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);
        bool setCavFwdPwrMaskAbsolute(const size_t s1,const size_t s2,const size_t s3,const size_t s4,const double value);




        bool setAbsoluteTimeMask(const double s1,const double s2,const double s3,
                                 const double s4,const double value2,const  std::string& name);
        bool setPercentTimeMask(const double s1,const double s2,const double s3,
                                const double s4,const double value2,const  std::string& name);


#ifdef BUILD_DLL
        bool setInfiniteMasks(const std::string& name);
        bool setHighMask(const std::string& name,const  boost::python::list& value);
        bool setLowMask(const std::string& name,const  boost::python::list& value);
        bool setCavRevPwrHiMask_Py(const boost::python::list& value);
        bool setCavRevPwrLoMask_Py(const boost::python::list& value);
#endif
        //bool setNumContinuousOutsideMaskCount(const std::string& name, size_t val);
        bool shouldCheckMasks(const std::string& name);


        int getTORACQM()const;
        int getTORSCAN()const;


        bool setTORSCANToIOIntr();
        bool setTORACQMEvent();
        bool setTORSCANToPassive();
        bool resetTORSCANToIOIntr();


        bool isKeepingKlyFwdPwrRS();
        void setKeepKlyFwdPwrRS(bool val);
        void keepKlyFwdPwrRS();
        void dontKeepKlyFwdPwrRS();
        std::tuple<size_t,double,double> getKlyFwdPwrRSState(int ampSP_setting);
        void setKlyFwdPwrRSState(int amp_sp, size_t n, double old_mean, double old_variance);
#ifdef BUILD_DLL
        boost::python::list getKlyFwdPwrRSState_Py(int ampSP_setting);
#endif

        /* these functions enable you to CHOOSE which traces from onetracedata you want to retuyrn !!oooh  */

        void setTracesToSaveWhenDumpingCutOneTraceData(const std::vector<std::string>& names);
        std::vector<std::string> getTracesToSaveWhenDumpingCutOneTraceData()const;

        std::vector< std::pair<std::string, std::vector<double>> > getOneTraceData()const;
#ifdef BUILD_DLL
        boost::python::dict getOneTraceData_Py()const;
        void setTracesToSaveWhenDumpingCutOneTraceData_Py(boost::python::list& names);
        boost::python::list getTracesToSaveWhenDumpingCutOneTraceData_Py()const;
        boost::python::dict getCutOneTraceData_Py();
#endif
        bool applyPulseShape();
        bool setAndApplyPulseShape(std::vector<double>& values);
        bool setPulseShape(std::vector<double>& values);
#ifdef BUILD_DLL
        bool setAndApplyPulseShape_Py(boost::python::list values);
        bool setPulseShape_Py(boost::python::list values);
#endif


        bool setInterlockNonActive();
//        bool externalTriggerOn();

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

        bool trigOff();
        bool trigExt();
        bool trigInt();
        bool isTrigExternal();

        /* These functions calculte the rep rate of data as received by this interface. */
        void setNumTracesToEstimateRepRate(size_t value);
        size_t getNumTracesToEstimateRepRate() const;
        double getTraceRepRate() const;


        llrfStructs::TRIG getTrigSource();

        bool setTraceSCAN(const std::string& trace, const llrfStructs::LLRF_SCAN value);
        bool setAllTraceSCAN(const llrfStructs::LLRF_SCAN value);
        bool setAllSCANToPassive();


//____________________________________________________________________________________________
        bool isAmpFFLocked()const;
        bool isAmpFFNotLocked()const;
        bool isPhaseFFLocked()const;
        bool isPhaseFFNotLocked()const;
        bool isFFLocked()const;
        bool isFFNotLocked()const;

        bool lockAmpFF();
        bool lockPhaseFF();


        bool RFOutput()const;
        bool isRFOutput() const;
        bool isNotRFOutput() const;
        bool isInterlockActive()const;
        bool isInterlockNotActive()const;
//____________________________________________________________________________________________


        bool setUnwrapPhaseTolerance(const double value);
        bool setUnwrapPhaseTolerance(const std::string& name,const double value);


        //bool   isLocked();

        llrfStructs::LLRF_TYPE getType();
        size_t getTraceLength();

//        std::vector<std::string> getChannelNames();
        std::vector<std::string> getTraceNames();

//        bool usePercentMask(const std::string& trace);
//        bool useAbsoluteMask(const std::string& trace);
//        bool setMaskValue(const std::string& trace, double value);

        size_t getOutsideMaskEventCount()const;

//        llrfStructs::rf_trace getTraceData(const std::string& name);
//        std::vector<llrfStructs::rf_trace> getTraceBuffer(const std::string& name);

#ifdef BUILD_DLL
        boost::python::list getChannelNames_Py();
        boost::python::list getTraceNames_Py();

        boost::python::list getTimeVector_Py()const;
        boost::python::list getPulseShape_Py()const;

#endif
        std::vector<double> getPulseShape()const;
        bool getPulseShapeUpToDate()const;

        std::vector<double> getTimeVector()const;

//--------------------------------------------------------------------------------------------------
/* ___  __        __   ___                         ___  __
    |  |__)  /\  /  ` |__     \  /  /\  |    |  | |__  /__`
    |  |  \ /~~\ \__, |___     \/  /~~\ |___ \__/ |___ .__/

     Returns the current rolling_average of the requested trace
*/
        std::pair<std::string, std::vector<double>> getTraceData(const std::string& name)const;
        std::vector<double> getTraceValues(const std::string& name)const;
        std::vector<double> getCavRevPwr()const;
        std::vector<double> getCavFwdPwr()const;
        std::vector<double> getKlyRevPwr()const;
        std::vector<double> getKlyFwdPwr()const;
        std::vector<double> getCavRevPha()const;
        std::vector<double> getCavFwdPha()const;
        std::vector<double> getKlyRevPha()const;
        std::vector<double> getKlyFwdPha()const;
        std::vector<double> getProbePwr()const;
        std::vector<double> getProbePha()const;
#ifdef BUILD_DLL
        boost::python::dict getTraceData_Py(const std::string& name);
        boost::python::list getTraceValues_Py(const std::string& name)const;
        boost::python::list getCavRevPwr_Py()const;
        boost::python::list getCavFwdPwr_Py()const;
        boost::python::list getKlyRevPwr_Py()const;
        boost::python::list getKlyFwdPwr_Py()const;
        boost::python::list getCavRevPha_Py()const;
        boost::python::list getCavFwdPha_Py()const;
        boost::python::list getKlyRevPha_Py()const;
        boost::python::list getKlyFwdPha_Py()const;
        boost::python::list getProbePha_Py()const;
        boost::python::list getProbePwr_Py()const;
#endif
//--------------------------------------------------------------------------------------------------
/*
    ___  __        __   ___               ___  __        __   ___  __
     |  |__)  /\  /  ` |__      /\  \  / |__  |__)  /\  / _` |__  /__`
     |  |  \ /~~\ \__, |___    /~~\  \/  |___ |  \ /~~\ \__> |___ .__/

     Returns the current rolling_average of the requested trace
*/
        /* this is the main funciton, the below specific ones all call this */
        std::vector<double> getAverageTraceData(const std::string& name)const;
        std::vector<double> getCavRevPwrAv()const;
        std::vector<double> getCavFwdPwrAv()const;
        std::vector<double> getKlyRevPwrAv()const;
        std::vector<double> getKlyFwdPwrAv()const;
        std::vector<double> getCavRevPhaAv()const;
        std::vector<double> getCavFwdPhaAv()const;
        std::vector<double> getKlyRevPhaAv()const;
        std::vector<double> getKlyFwdPhaAv()const;
        std::vector<double> getProbePwrAv()const;
        std::vector<double> getProbePhaAv()const;
#ifdef BUILD_DLL
        boost::python::list getAverageTraceData_Py(const std::string& name)const;
        boost::python::list getCavRevPwrAv_Py()const;
        boost::python::list getCavFwdPwrAv_Py()const;
        boost::python::list getKlyRevPwrAv_Py()const;
        boost::python::list getKlyFwdPwrAv_Py()const;
        boost::python::list getCavRevPhaAv_Py()const;
        boost::python::list getCavFwdPhaAv_Py()const;
        boost::python::list getKlyRevPhaAv_Py()const;
        boost::python::list getKlyFwdPhaAv_Py()const;
        boost::python::list getProbePwrAv_Py()const;
        boost::python::list getProbePhaAv_Py()const;
#endif
//--------------------------------------------------------------------------------------------------
/*

___  __        __   ___     __        ___  ___  ___  __   __
 |  |__)  /\  /  ` |__     |__) |  | |__  |__  |__  |__) /__`
 |  |  \ /~~\ \__, |___    |__) \__/ |    |    |___ |  \ .__/

     Returns the current datat buffer of the requested trace
*/
        std::vector<std::pair<std::string, std::vector<double>>> getTraceDataBuffer(const std::string& name)const;
        std::vector<std::pair<std::string, std::vector<double>>> getCavRevPwrBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getCavFwdPwrBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getKlyRevPwrBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getKlyFwdPwrBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getCavRevPhaBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getCavFwdPhaBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getKlyRevPhaBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getKlyFwdPhaBuffer()const;
        std::vector<std::pair<std::string, std::vector<double>>> getProbePwrBuffer( )const;
        std::vector<std::pair<std::string, std::vector<double>>> getProbePhaBuffer( )const;
#ifdef BUILD_DLL
        boost::python::dict getTraceDataBuffer_Py(const std::string& name)const;
        boost::python::dict getCavRevPwrBuffer_Py()const;
        boost::python::dict getCavFwdPwrBuffer_Py()const;
        boost::python::dict getKlyRevPwrBuffer_Py()const;
        boost::python::dict getKlyFwdPwrBuffer_Py()const;
        boost::python::dict getCavRevPhaBuffer_Py()const;
        boost::python::dict getCavFwdPhaBuffer_Py()const;
        boost::python::dict getKlyRevPhaBuffer_Py()const;
        boost::python::dict getKlyFwdPhaBuffer_Py()const;
        boost::python::dict getProbePwrBuffer_Py()const;
        boost::python::dict getProbePhaBuffer_Py()const;
#endif

        //boost::python::dict dump_traces();
//        llrfStructs::rf_trace getCavRevPowerData();
//        llrfStructs::rf_trace getCavFwdPowerData();
//        llrfStructs::rf_trace getKlyRevPowerData();
//        llrfStructs::rf_trace getKlyFwdPowerData();
//        llrfStructs::rf_trace getCavRevPhaseData();
//        llrfStructs::rf_trace getCavFwdPhaseData();
//        llrfStructs::rf_trace getKlyRevPhaseData();
//        llrfStructs::rf_trace getKlyFwdPhaseData();

//        std::vector<llrfStructs::rf_trace> getCavRevPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getCavFwdPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyRevPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyFwdPowerBuffer();
//        std::vector<llrfStructs::rf_trace> getCavRevPhaseBuffer();
//        std::vector<llrfStructs::rf_trace> getCavFwdPhaseBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyRevPhaseBuffer();
//        std::vector<llrfStructs::rf_trace> getKlyFwdPhaseBuffer();
#ifdef BUILD_DLL
//        boost::python::list getCavRevPowerBuffer_Py();
//        boost::python::list getCavFwdPowerBuffer_Py();
//        boost::python::list getKlyRevPowerBuffer_Py();
//        boost::python::list getKlyFwdPowerBuffer_Py();
//        boost::python::list getCavRevPhaseBuffer_Py();
//        boost::python::list getCavFwdPhaseBuffer_Py();
//        boost::python::list getKlyRevPhaseBuffer_Py();
//        boost::python::list getKlyFwdPhaseBuffer_Py();
#endif

        const llrfStructs::liberallrfObject& getLLRFObjConstRef();
        const llrfStructs::rf_trace_data& getTraceDataConstRef(const std::string& name);

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

        bool enableRFOutput();
        bool disableRFOutput();
        bool enableRFandLock();

        bool setInfiniteMaskEndByPower(const std::string& power_trace,const std::string& phase_trace,const double level);
        bool disableInfiniteMaskEndByPower(const std::string& phase_trace);
        size_t getMaskInfiniteEndByPowerIndex(const std::string& name);
        double getMaskInfiniteEndByPowerTime(const std::string& name);


//        bool setCheckMask(const std::string&name, bool value);
//        bool setShouldCheckMask(const std::string&name);
//        bool setShouldNotCheckMask(const std::string&name);


        void startTimer();
        long long elapsedTime();
        void offsetTimer(long long value);
        double getBreakDownRate();


        bool setMeanStartEndTime(const double start, const double end, const std::string&name);
        bool setMeanStartIndex(const std::string&name, size_t  value);
        bool setMeanStopIndex(const std::string&name, size_t  value);

        size_t getMeanStartIndex(const std::string&name)const;
        size_t getMeanStopIndex(const std::string&name)const;

        double getMean(const std::string&name)const;
        double getCutMean(const std::string&name)const;
        double getKlyFwdPwrCutMean()const;
        double getKlyFwdPhaCutMean()const;
        double getKlyRevPwrCutMean()const;
        double getKlyRevPhaCutMean()const;
        double getCavFwdPwrCutMean()const;
        double getCavFwdPhaCutMean()const;
        double getCavRevPwrCutMean()const;
        double getCavRevPhaCutMean()const;
        double getProbePwrCutMean()const;
        double getProbePhaCutMean()const;



        /* convert trace indices to times and vice-versa */
        size_t getIndex(const double time);
        double getTime(const size_t  time);


        std::string fullLLRFTraceName(const std::string& name)const;
        std::string shortLLRFTraceName(const std::string& name)const;

        // Qantifiers
        bool isTracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotTracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool isMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv);

        bool isMonitoring(const std::string& name);
        bool isNotMonitoring(const std::string& name);

        bool isCheckingMask(const std::string& name);
        bool isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotCheckingMask(const std::string& name);
        bool isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);

        bool isGlobalCheckMask();
        bool isPercentMask(const std::string& name);
        bool isAbsoluteMask(const std::string& name);

        // Starters
//        void startTraceMonitoring();
//        bool startTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
//        bool startTraceMonitoring(const std::string& name);
//        bool startCavFwdTraceMonitor();
//        bool startCavRevTraceMonitor();
//        bool startKlyFwdTraceMonitor();
//        bool startKlyRevTraceMonitor();
        // Stoppers
//        bool stopTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
//        bool stopTraceMonitoring(const std::string& name);
//        void stopTraceMonitoring();
//        bool stopCavFwdTraceMonitor();
//        bool stopCavRevTraceMonitor();
//        bool stopKlyFwdTraceMonitor();
//        bool stopKlyRevTraceMonitor();
        // pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(const double val);
        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> getILockStates(const std::string & name);
        std::map<HWC_ENUM::ILOCK_NUMBER, std::string> getILockStatesStr(const std::string & name);
        // any functions that return a map need a wrapper to convert to a python dictionary
        // (we need the functions that return std::map types when building c++ applications)
//#ifdef BUILD_DLL
//        boost::python::dict getILockStates_Pystd::string magName);
//        boost::python::dict getILockStatesStr_Pystd::string magName);
//        boost::python::dict getMagPSUStateDefinition();
//#endif // BUILD_DLL

    protected:
    private:

        void initialise();
        const bool shouldStartEPICs;
        liberallrfInterface localInterface;


};
#endif // VELA_MAG_CONTROLLER_H_
