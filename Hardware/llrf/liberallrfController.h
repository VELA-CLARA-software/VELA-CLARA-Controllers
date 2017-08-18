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

#ifndef LASER_CONTROLLER_H_
#define LASER_CONTROLLER_H_
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
        liberaLLRFController(const bool show_messages,const bool show_debug_messagese,
                       const std::string & laserConf,const bool startVirtualMachine,
                       const bool shouldStartEPICs,const llrfStructs::LLRF_TYPE type);
        ~liberaLLRFController();

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

        std::vector<std::string> getChannelNames();
        std::vector<double> getTraceValues(const std::string& name);
        llrfStructs::rf_trace getTraceData(const std::string& name);
        std::deque<llrfStructs::rf_trace> getTraceBuffer(const std::string& name);

#ifdef BUILD_DLL
        boost::python::list getChannelNames_Py();
        boost::python::list getTraceValues_Py(const std::string& name);
        boost::python::list getTraceBuffer_Py(const std::string& name);
#endif
        std::vector<double> getChannelTrace();
        std::vector<double> getCavRevPower();
        std::vector<double> getCavFwdPower();
        std::vector<double> getKlyRevPower();
        std::vector<double> getKlyFwdPower();
        std::vector<double> getCavRevPhase();
        std::vector<double> getCavFwdPhase();
        std::vector<double> getKlyRevPhase();
        std::vector<double> getKlyFwdPhase();
#ifdef BUILD_DLL
        boost::python::list getCavRevPower_Py();
        boost::python::list getCavFwdPower_Py();
        boost::python::list getKlyRevPower_Py();
        boost::python::list getKlyFwdPower_Py();
        boost::python::list getCavRevPhase_Py();
        boost::python::list getCavFwdPhase_Py();
        boost::python::list getKlyRevPhase_Py();
        boost::python::list getKlyFwdPhase_Py();
#endif
        llrfStructs::rf_trace getCavRevPowerData();
        llrfStructs::rf_trace getCavFwdPowerData();
        llrfStructs::rf_trace getKlyRevPowerData();
        llrfStructs::rf_trace getKlyFwdPowerData();
        llrfStructs::rf_trace getCavRevPhaseData();
        llrfStructs::rf_trace getCavFwdPhaseData();
        llrfStructs::rf_trace getKlyRevPhaseData();
        llrfStructs::rf_trace getKlyFwdPhaseData();

        std::deque<llrfStructs::rf_trace> getCavRevPowerBuffer();
        std::deque<llrfStructs::rf_trace> getCavFwdPowerBuffer();
        std::deque<llrfStructs::rf_trace> getKlyRevPowerBuffer();
        std::deque<llrfStructs::rf_trace> getKlyFwdPowerBuffer();
        std::deque<llrfStructs::rf_trace> getCavRevPhaseBuffer();
        std::deque<llrfStructs::rf_trace> getCavFwdPhaseBuffer();
        std::deque<llrfStructs::rf_trace> getKlyRevPhaseBuffer();
        std::deque<llrfStructs::rf_trace> getKlyFwdPhaseBuffer();
#ifdef BUILD_DLL
        boost::python::list getCavRevPowerBuffer_Py();
        boost::python::list getCavFwdPowerBuffer_Py();
        boost::python::list getKlyRevPowerBuffer_Py();
        boost::python::list getKlyFwdPowerBuffer_Py();
        boost::python::list getCavRevPhaseBuffer_Py();
        boost::python::list getCavFwdPhaseBuffer_Py();
        boost::python::list getKlyRevPhaseBuffer_Py();
        boost::python::list getKlyFwdPhaseBuffer_Py();
#endif

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
        bool setHighMask(const std::string& name, std::vector<double>& value);
        bool setLowMask(const std::string& name, std::vector<double>& value);
        bool setNumBufferTraces(const std::string&name, size_t value);
        bool setCheckMask(const std::string&name, bool value);
        bool setKeepRollingAverage(const std::string&name, bool value);
        bool setNumRollingAverageTraces(const std::string&name, size_t value );
        // Qantifiers
        bool isTracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotTracePV(const llrfStructs::LLRF_PV_TYPE pv);
        bool isMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotMonitoring(const llrfStructs::LLRF_PV_TYPE pv);

        bool isCheckingMask(const std::string& name);
        bool isCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);
        bool isNotCheckingMask(const std::string& name);
        bool isNotCheckingMask(const llrfStructs::LLRF_PV_TYPE pv);


        // Starters
        void startTraceMonitoring();
        bool startTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
        bool startTraceMonitoring(const std::string& name);
        bool startCavFwdTraceMonitor();
        bool startCavRevTraceMonitor();
        bool startKlyFwdTraceMonitor();
        bool startKlyRevTraceMonitor();
        // Stoppers
        bool stopTraceMonitoring(const llrfStructs::LLRF_PV_TYPE pv);
        bool stopTraceMonitoring(const std::string& name);
        void stopTraceMonitoring();
        bool stopCavFwdTraceMonitor();
        bool stopCavRevTraceMonitor();
        bool stopKlyFwdTraceMonitor();
        bool stopKlyRevTraceMonitor();
        // pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT(double val);
        std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> getILockStates(const std::string & name);
        std::map<VELA_ENUM::ILOCK_NUMBER, std::string> getILockStatesStr(const std::string & name);
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
