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
#include "blmStructs.h"
#include "blmConfigReader.h"
#include "structs.h"
//stl
#include <vector>
#include <string>
//boost
#include <boost/circular_buffer.hpp>

class blmInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        blmInterface::blmInterface();//const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        blmInterface( const std::string & configFileLocation1,
                        const bool& show_messages_ptr,
                        const bool& show_debug_messages_ptr,
                        const bool shouldStartEPICS,
                        const bool startVirtualMachine,
                        const HWC_ENUM::MACHINE_AREA myMachineArea );

        ~blmInterface();

        void getBLMNames( std::vector< std::string >  & blmNames );
        double getBLMP1( const std::string & blm );
        double getBLMP2( const std::string & blm );
        double getBLMP3( const std::string & blm );
        double getBLMP4( const std::string & blm );
        double getWCMQ()   ;
        double getICT1Q( const int part1, const int part2 )  ;
        double getICT2Q( const int part1, const int part2 )  ;
        double getFCUPQ()  ;
        double getEDFCUPQ();
        void setBufferSize( size_t bufferSize );
        void setNumBufferSize( size_t bufferSize );
        void setTraceBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
        void setTimebase( const std::string & blmName, const double timebase );
        double getTimebase( const std::string & blmName );
        size_t getBufferSize( const std::string & blmName );
        const blmStructs::DIAG_TYPE getDiagType( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        const std::string getDiagTypeStr( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getBLMTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getBLMNums( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getBLMP1Vec( const std::string & name );
        std::vector< double > getBLMP2Vec( const std::string & name );
        std::vector< double > getBLMP3Vec( const std::string & name );
        std::vector< double > getBLMP4Vec( const std::string & name );
        boost::circular_buffer< double > getBLMNumBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::circular_buffer< double > getBLMP1Buffer( const std::string & name );
        boost::circular_buffer< double > getBLMP2Buffer( const std::string & name );
        boost::circular_buffer< double > getBLMP3Buffer( const std::string & name );
        boost::circular_buffer< double > getBLMP4Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTraceBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::circular_buffer< std::vector< double > > getBLMTR1Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTR2Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTR3Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTR4Buffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< std::string > getStrTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        void updateTrace( blmStructs::monitorStruct * ms, const event_handler_args args );
        void updateValue( blmStructs::monitorStruct * ms, const event_handler_args args );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, blmStructs::SCOPE_PV_TYPE channel, size_t N );
        void monitorNumsForNShots( size_t N );
        void monitorANumForNShots( const std::string num, blmStructs::SCOPE_PV_TYPE channel, size_t N );
        void cancelDataMonitors();
        std::vector< double > getAvgNoise( const std::string & name, blmStructs::SCOPE_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< std::string > getBLMNames();
        std::vector< std::string > getBLMPVs();
        std::vector< std::string > getBLMTracePVs();
        std::vector< std::string > getBLMNumPVs();
        const blmStructs::blmTraceData & getBLMTraceDataStruct( const std::string & blmName );
        const blmStructs::blmNumObject & getBLMNumDataStruct( const std::string & blmName );
        bool monitoringTraces = false;
        bool monitoringNums = false;
        bool isMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        bool isMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );

        blmStructs::blmObject getBLMObject( const std::string & blmName );

        void killTraceCallBack( blmStructs::monitorStruct * ms );
        void killNumCallBack( blmStructs::monitorStruct * ms );
        bool killTraceMonitors();

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & blmName )const;
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & blmName )const;

    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.
        void initialise();
        const HWC_ENUM::MACHINE_AREA machineArea;
        blmConfigReader configReader;

        bool initBLMObjects();
        void initBLMChids();
        void addChannel( const std::string & pvRoot, blmStructs::pvStruct & pv );
        void monitorBLMs();
        void clearContinuousMonitorStructs();
        void clearContinuousTraceMonitorStructs();
        void clearContinuousNumMonitorStructs();
        void addToTraceMonitorStructs( std::vector< blmStructs::monitorStruct * > & msv, blmStructs::pvStruct & pv,  blmStructs::blmTraceData * traobj );
        void addToNumMonitorStructs( std::vector< blmStructs::monitorStruct * > & msv, blmStructs::pvStruct & pv,  blmStructs::blmNumObject * numobj );

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the blm is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members
        typedef  bool (blmInterface::*isOCMemFn)(const std::string & );
        typedef  void (blmInterface::*OCMemFn)  (const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, blmInterface & obj );

        /// static function that can be called back from epics to update values
        static void staticEntryrMonitor( const event_handler_args args );

        bool isATracePV( blmStructs::SCOPE_PV_TYPE pv );
        bool isANumPV( blmStructs::SCOPE_PV_TYPE pv );
        void resetTraceVectors( size_t N );
        void resetATraceVector( const std::string blmName, blmStructs::SCOPE_PV_TYPE channel, size_t N );
        void resetNumVectors( size_t N );
        void resetANumVector( const std::string blmName, blmStructs::SCOPE_PV_TYPE channel, size_t N );

        blmStructs::blmObject blmObj;

        std::vector< blmStructs::monitorStruct* > numMonitorStructs;
        std::vector< blmStructs::monitorStruct* > traceMonitorStructs;
        void addTraceMonitorStructs( const std::string &blmName );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< blmStructs::monitorStruct * > continuousMonitorStructs;
        std::vector< blmStructs::monitorStruct * > continuousTraceMonitorStructs;
        std::vector< blmStructs::monitorStruct * > continuousNumMonitorStructs;

        std::map< std::string, blmStructs::blmObject > allBLMData; /// All the blm data is stored in this map, keyed by the blm name

        std::vector< std::vector< double > > allBLMMonitorData;
        std::vector< std::vector< double > > traceMonitorData;
        std::map< bool, blmStructs::blmObject* > isMonitoringMap;
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
