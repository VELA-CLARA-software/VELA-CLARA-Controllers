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
        /// New scheem - we just have 1 constructor, but we have a higher level class that create these objects
        liberaLLRFController();
        liberaLLRFController(const bool show_messages,const bool show_debug_messagese,
                       const std::string & laserConf,const bool startVirtualMachine,
                       const bool shouldStartEPICs,const llrfStructs::LLRF_TYPE type);
        ~liberaLLRFController( );

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



      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );
      // any functions that return a map need a wrapper to convert to a python dictionary
      // (we need the functions that return std::map types when building c++ applications)
#ifdef BUILD_DLL
//        boost::python::dict getILockStates_Py( std::string magName );
//        boost::python::dict getILockStatesStr_Py( std::string magName );
//        boost::python::dict getMagPSUStateDefinition();
#endif // BUILD_DLL

    protected:
    private:

        void initialise();

        const bool shouldStartEPICs;

        liberallrfInterface localInterface;
};



#endif // VELA_MAG_CONTROLLER_H_
