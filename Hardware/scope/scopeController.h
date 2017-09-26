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

#ifndef SCOPE_CONTROLLER_H
#define SCOPE_CONTROLLER_H

// project
#include "scopeInterface.h"
#include "controller.h"
// stl
#include <string>
#include <vector>
// boost
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
#include <boost/circular_buffer.hpp>
#endif

class scopeController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        scopeController();// const bool show_messages = true, const bool show_debug_messages = true );
        scopeController( const std::string &configFileLocation1, const std::string &configFileLocation2,
                         const bool show_messages, const bool show_debug_messages, const bool shouldStartEPICS,
                         const bool startVirtualMachine, const VELA_ENUM::MACHINE_AREA myMachineArea );
        ~scopeController();
//
//        bool hasTrig( const std::string & scopeName );
//        bool hasNoTrig( const std::string & scopeName );

        bool monitoringTraces;
        bool monitoringNums;
        bool isMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isMonitoringScopeTrace_str( const std::string & scopeName, const std::string & pvType );
        bool isMonitoringScopeTraces( const std::string & scopeName );
        bool isMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isMonitoringScopeNum_str( const std::string & scopeName, const std::string & pvType );
        bool isMonitoringScopeNums( const std::string & scopeName );
        bool isNotMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringScopeTrace_str( const std::string & scopeName, const std::string & pvType );
        bool isNotMonitoringScopeTraces( const std::string & scopeName );
        bool isNotMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringScopeNum_str( const std::string & scopeName, const std::string & pvType );
        bool isNotMonitoringScopeNums( const std::string & scopeName );

        double getScopeP1( const std::string & scopeName );
        double getScopeP2( const std::string & scopeName );
        double getScopeP3( const std::string & scopeName );
        double getScopeP4( const std::string & scopeName );
        double getWCMQ()   ;
        double getICT1Q( const int part1, const int part2 )  ;
        double getICT2Q( const int part1, const int part2 )  ;
        double getFCUPQ()  ;
        double getEDFCUPQ();
        void setBufferSize( size_t bufferSize );
        void setNumBufferSize( size_t bufferSize );
        void setTraceBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
        const scopeStructs::scopeTraceData & getScopeTraceDataStruct( const std::string & scopeName );
        const scopeStructs::scopeNumObject & getScopeNumDataStruct( const std::string & scopeName );
        scopeStructs::SCOPE_PV_TYPE getScopePVType( const std::string & pvType );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, scopeStructs::SCOPE_PV_TYPE channel, size_t N );
        void monitorATraceForNShots_str( const std::string trace, const std::string & channel, size_t N );
        void monitorNumsForNShots( size_t N );
        void monitorANumForNShots( const std::string num, scopeStructs::SCOPE_PV_TYPE channel, size_t N );
        void monitorANumForNShots_str( const std::string num, const std::string & channel, size_t N );

        std::vector< std::vector< double > > getScopeTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getScopeTraces_str( const std::string & name, const std::string & pvType );
        std::vector< double > getScopeNums( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getScopeNums_str( const std::string & name, const std::string & pvType );
        std::vector< double > getScopeP1Vec( const std::string & name );
        std::vector< double > getScopeP2Vec( const std::string & name );
        std::vector< double > getScopeP3Vec( const std::string & name );
        std::vector< double > getScopeP4Vec( const std::string & name );
        boost::circular_buffer< double > getScopeP1Buffer( const std::string & name );
        boost::circular_buffer< double > getScopeP2Buffer( const std::string & name );
        boost::circular_buffer< double > getScopeP3Buffer( const std::string & name );
        boost::circular_buffer< double > getScopeP4Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR1Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR2Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR3Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR4Buffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::string > getStrTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< std::vector< double > > getPartOfTrace_str( const std::string & name, const std::string & pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAvgNoise( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );

        std::vector< std::string > getScopeNames();
        std::vector< std::string > getScopePVs();
        std::vector< std::string > getScopeTracePVs();
        std::vector< std::string > getScopeNumPVs();

        #ifdef BUILD_DLL
        boost::python::list getScopeNums_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getScopeNums_Py_str( const std::string & name, const std::string & pvType );
        boost::python::list getScopeP1Vec_Py( const std::string & name );
        boost::python::list getScopeP2Vec_Py( const std::string & name );
        boost::python::list getScopeP3Vec_Py( const std::string & name );
        boost::python::list getScopeP4Vec_Py( const std::string & name );
        boost::python::list getScopeP1Buffer_Py( const std::string & name );
        boost::python::list getScopeP2Buffer_Py( const std::string & name );
        boost::python::list getScopeP3Buffer_Py( const std::string & name );
        boost::python::list getScopeP4Buffer_Py( const std::string & name );
        boost::python::list getScopeTR1Buffer_Py( const std::string & name );
        boost::python::list getScopeTR2Buffer_Py( const std::string & name );
        boost::python::list getScopeTR3Buffer_Py( const std::string & name );
        boost::python::list getScopeTR4Buffer_Py( const std::string & name );
        boost::python::list getMinOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getMinOfTraces_Py_str( const std::string & name, const std::string & pvType );
        boost::python::list getMaxOfTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getMaxOfTraces_Py_str( const std::string & name, const std::string & pvType );
        boost::python::list getAreaUnderTraces_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getAreaUnderTraces_Py_str( const std::string & name, const std::string & pvType );
        boost::python::list getTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getTimeStamps_Py_str( const std::string & name, const std::string & pvType );
        boost::python::list getStrTimeStamps_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getStrTimeStamps_Py_str( const std::string & name, const std::string & pvType );
        boost::python::list getAreaUnderPartOfTrace_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getAreaUnderPartOfTrace_Py_str( const std::string & name, const std::string & pvType, const int part1, const int part2 );
        boost::python::list getAvgNoise_Py( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getAvgNoise_Py_str( const std::string & name, const std::string & pvType, const int part1, const int part2 );
        boost::python::list getScopeNames_Py();
        boost::python::list getScopePVs_Py();
        boost::python::list getScopeTracePVs_Py();
        boost::python::list getScopeNumPVs_Py();
        #endif
        /// write a method that returns string version of enums using ENUM_TO_STRING

//        VELA_ENUM::TRIG_STATE getScopeState( const std::string & scopeName );
//        std::string getScopeStateStr( const std::string & name );

        double get_CA_PEND_IO_TIMEOUT();
        void set_CA_PEND_IO_TIMEOUT( double val );

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & objName );


    protected:
    private:

        void initialise();

        /// No singletons, no pointers, let's just have an object

        scopeInterface localInterface;

        std::vector< std::string > scopeNames;

        const bool shouldStartEPICS;
        const VELA_ENUM::MACHINE_AREA machineArea;
};


#endif // scopeController_H
