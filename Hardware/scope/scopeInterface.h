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

#ifndef VELA_SCOPE_INTERFACE_H
#define VELA_SCOPE_INTERFACE_H
// djs
#include "interface.h"
#include "scopeStructs.h"
#include "scopeConfigReader.h"
#include "structs.h"
//stl
#include <vector>
#include <string>
//boost
#include <boost/circular_buffer.hpp>

class scopeInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        scopeInterface::scopeInterface();//const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        scopeInterface( const std::string & configFileLocation1,
                        const std::string & configFileLocation2,
                        const bool* show_messages_ptr,
                        const bool* show_debug_messages_ptr,
                        const bool shouldStartEPICS,
                        const bool startVirtualMachine,
                        const HWC_ENUM::MACHINE_AREA myMachineArea );

        ~scopeInterface();

        void getScopeNames( std::vector< std::string >  & scopeNames );
        double getScopeP1( const std::string & scope );
        double getScopeP2( const std::string & scope );
        double getScopeP3( const std::string & scope );
        double getScopeP4( const std::string & scope );
        double getWCMQ()   ;
        double getICT1Q( const int part1, const int part2 )  ;
        double getICT2Q( const int part1, const int part2 )  ;
        double getFCUPQ()  ;
        double getEDFCUPQ();
        void setBufferSize( size_t bufferSize );
        void setNumBufferSize( size_t bufferSize );
        void setTraceBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
        void setTimebase( const std::string & scopeName, const double timebase );
        double getTimebase( const std::string & scopeName );
        size_t getBufferSize( const std::string & scopeName );
        const scopeStructs::DIAG_TYPE getDiagType( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        const std::string getDiagTypeStr( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getScopeTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getScopeNums( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getScopeP1Vec( const std::string & name );
        std::vector< double > getScopeP2Vec( const std::string & name );
        std::vector< double > getScopeP3Vec( const std::string & name );
        std::vector< double > getScopeP4Vec( const std::string & name );
        boost::circular_buffer< double > getScopeNumBuffer( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::circular_buffer< double > getScopeP1Buffer( const std::string & name );
        boost::circular_buffer< double > getScopeP2Buffer( const std::string & name );
        boost::circular_buffer< double > getScopeP3Buffer( const std::string & name );
        boost::circular_buffer< double > getScopeP4Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTraceBuffer( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        boost::circular_buffer< std::vector< double > > getScopeTR1Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR2Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR3Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getScopeTR4Buffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< std::string > getStrTimeStamps( const std::string & name, scopeStructs::SCOPE_PV_TYPE pvType );
        void updateTrace( scopeStructs::monitorStruct * ms, const event_handler_args args );
        void updateValue( scopeStructs::monitorStruct * ms, const event_handler_args args );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, scopeStructs::SCOPE_PV_TYPE channel, size_t N );
        void monitorNumsForNShots( size_t N );
        void monitorANumForNShots( const std::string num, scopeStructs::SCOPE_PV_TYPE channel, size_t N );
        void cancelDataMonitors();
        std::vector< double > getAvgNoise( const std::string & name, scopeStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< std::string > getScopeNames();
        std::vector< std::string > getScopePVs();
        std::vector< std::string > getScopeTracePVs();
        std::vector< std::string > getScopeNumPVs();
        const scopeStructs::scopeTraceData & getScopeTraceDataStruct( const std::string & scopeName );
        const scopeStructs::scopeNumObject & getScopeNumDataStruct( const std::string & scopeName );
        bool monitoringTraces = false;
        bool monitoringNums = false;
        bool isMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringScopeTrace( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringScopeNum( const std::string & scopeName, scopeStructs::SCOPE_PV_TYPE pvType );

        scopeStructs::scopeObject getScopeObject( const std::string & scopeName );

        void killTraceCallBack( scopeStructs::monitorStruct * ms );
        void killNumCallBack( scopeStructs::monitorStruct * ms );
        bool killTraceMonitors();

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & scopeName )const;
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & scopeName )const;

    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.
        void initialise();
        const HWC_ENUM::MACHINE_AREA machineArea;
        scopeConfigReader configReader;

        bool initScopeObjects();
        void initScopeChids();
        void addChannel( const std::string & pvRoot, scopeStructs::pvStruct & pv );
        void monitorScopes();
        void clearContinuousMonitorStructs();
        void clearContinuousTraceMonitorStructs();
        void clearContinuousNumMonitorStructs();
        void addToTraceMonitorStructs( std::vector< scopeStructs::monitorStruct * > & msv, scopeStructs::pvStruct & pv,  scopeStructs::scopeTraceData * traobj );
        void addToNumMonitorStructs( std::vector< scopeStructs::monitorStruct * > & msv, scopeStructs::pvStruct & pv,  scopeStructs::scopeNumObject * numobj );

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the scope is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members
        typedef  bool (scopeInterface::*isOCMemFn)(const std::string & );
        typedef  void (scopeInterface::*OCMemFn)  (const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, scopeInterface & obj );

        /// static function that can be called back from epics to update values
        static void staticEntryrMonitor( const event_handler_args args );

        bool isATracePV( scopeStructs::SCOPE_PV_TYPE pv );
        bool isANumPV( scopeStructs::SCOPE_PV_TYPE pv );
        void resetTraceVectors( size_t N );
        void resetATraceVector( const std::string scopeName, scopeStructs::SCOPE_PV_TYPE channel, size_t N );
        void resetNumVectors( size_t N );
        void resetANumVector( const std::string scopeName, scopeStructs::SCOPE_PV_TYPE channel, size_t N );

        scopeStructs::scopeObject scopeObj;

        std::vector< scopeStructs::monitorStruct* > numMonitorStructs;
        std::vector< scopeStructs::monitorStruct* > traceMonitorStructs;
        void addTraceMonitorStructs( const std::string &scopeName );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< scopeStructs::monitorStruct * > continuousMonitorStructs;
        std::vector< scopeStructs::monitorStruct * > continuousTraceMonitorStructs;
        std::vector< scopeStructs::monitorStruct * > continuousNumMonitorStructs;

        std::map< std::string, scopeStructs::scopeObject > allScopeData; /// All the scope data is stored in this map, keyed by the scope name

        std::vector< std::vector< double > > allScopeMonitorData;
        std::vector< std::vector< double > > traceMonitorData;
        std::map< bool, scopeStructs::scopeObject* > isMonitoringMap;
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
