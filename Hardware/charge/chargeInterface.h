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

#ifndef VELA_charge_INTERFACE_H
#define VELA_charge_INTERFACE_H
// djs
#include "interface.h"
#include "chargeStructs.h"
#include "chargeConfigReader.h"
#include "structs.h"
//stl
#include <vector>
#include <string>
//boost
#include <boost/circular_buffer.hpp>

class chargeInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        chargeInterface::chargeInterface();//const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        chargeInterface( const std::string & configFileLocation1,
                        const std::string & configFileLocation2,
                        const bool& show_messages_ptr,
                        const bool& show_debug_messages_ptr,
                        const bool shouldStartEPICS,
                        const bool startVirtualMachine,
                        const HWC_ENUM::MACHINE_AREA myMachineArea );

        ~chargeInterface();

        void getchargeNames( std::vector< std::string >  & chargeNames );
        double getchargeP1( const std::string & charge );
        double getchargeP2( const std::string & charge );
        double getchargeP3( const std::string & charge );
        double getchargeP4( const std::string & charge );
        double getWCMQ()   ;
        double getICT1Q( const int part1, const int part2 )  ;
        double getICT2Q( const int part1, const int part2 )  ;
        double getFCUPQ()  ;
        double getEDFCUPQ();
        void setBufferSize( size_t bufferSize );
        void setNumBufferSize( size_t bufferSize );
        void setTraceBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
        void setTimebase( const std::string & chargeName, const double timebase );
        double getTimebase( const std::string & chargeName );
        size_t getBufferSize( const std::string & chargeName );
        const chargeStructs::DIAG_TYPE getDiagType( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        const std::string getDiagTypeStr( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        std::vector< std::vector< double > > getchargeTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getchargeNums( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getchargeP1Vec( const std::string & name );
        std::vector< double > getchargeP2Vec( const std::string & name );
        std::vector< double > getchargeP3Vec( const std::string & name );
        std::vector< double > getchargeP4Vec( const std::string & name );
        boost::circular_buffer< double > getchargeNumBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::circular_buffer< double > getchargeP1Buffer( const std::string & name );
        boost::circular_buffer< double > getchargeP2Buffer( const std::string & name );
        boost::circular_buffer< double > getchargeP3Buffer( const std::string & name );
        boost::circular_buffer< double > getchargeP4Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTraceBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::circular_buffer< std::vector< double > > getchargeTR1Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTR2Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTR3Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTR4Buffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< std::string > getStrTimeStamps( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        void updateTrace( chargeStructs::monitorStruct * ms, const event_handler_args args );
        void updateValue( chargeStructs::monitorStruct * ms, const event_handler_args args );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, chargeStructs::charge_PV_TYPE channel, size_t N );
        void monitorNumsForNShots( size_t N );
        void monitorANumForNShots( const std::string num, chargeStructs::charge_PV_TYPE channel, size_t N );
        void cancelDataMonitors();
        std::vector< double > getAvgNoise( const std::string & name, chargeStructs::charge_PV_TYPE & pvType, const int part1, const int part2 );
        std::vector< std::string > getchargeNames();
        std::vector< std::string > getchargePVs();
        std::vector< std::string > getchargeTracePVs();
        std::vector< std::string > getchargeNumPVs();
        const chargeStructs::chargeTraceData & getchargeTraceDataStruct( const std::string & chargeName );
        const chargeStructs::chargeNumObject & getchargeNumDataStruct( const std::string & chargeName );
        bool monitoringTraces = false;
        bool monitoringNums = false;
        bool isMonitoringchargeTrace( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        bool isMonitoringchargeNum( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        bool isNotMonitoringchargeTrace( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        bool isNotMonitoringchargeNum( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );

        chargeStructs::chargeObject getchargeObject( const std::string & chargeName );

        void killTraceCallBack( chargeStructs::monitorStruct * ms );
        void killNumCallBack( chargeStructs::monitorStruct * ms );
        bool killTraceMonitors();

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & chargeName )const;
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & chargeName )const;

    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.
        void initialise();
        const HWC_ENUM::MACHINE_AREA machineArea;
        chargeConfigReader configReader;

        bool initchargeObjects();
        void initchargeChids();
        void addChannel( const std::string & pvRoot, chargeStructs::pvStruct & pv );
        void monitorcharges();
        void clearContinuousMonitorStructs();
        void clearContinuousTraceMonitorStructs();
        void clearContinuousNumMonitorStructs();
        void addToTraceMonitorStructs( std::vector< chargeStructs::monitorStruct * > & msv, chargeStructs::pvStruct & pv,  chargeStructs::chargeTraceData * traobj );
        void addToNumMonitorStructs( std::vector< chargeStructs::monitorStruct * > & msv, chargeStructs::pvStruct & pv,  chargeStructs::chargeNumObject * numobj );

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the charge is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members
        typedef  bool (chargeInterface::*isOCMemFn)(const std::string & );
        typedef  void (chargeInterface::*OCMemFn)  (const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, chargeInterface & obj );

        /// static function that can be called back from epics to update values
        static void staticEntryrMonitor( const event_handler_args args );

        bool isATracePV( chargeStructs::charge_PV_TYPE pv );
        bool isANumPV( chargeStructs::charge_PV_TYPE pv );
        void resetTraceVectors( size_t N );
        void resetATraceVector( const std::string chargeName, chargeStructs::charge_PV_TYPE channel, size_t N );
        void resetNumVectors( size_t N );
        void resetANumVector( const std::string chargeName, chargeStructs::charge_PV_TYPE channel, size_t N );

        chargeStructs::chargeObject chargeObj;

        std::vector< chargeStructs::monitorStruct* > numMonitorStructs;
        std::vector< chargeStructs::monitorStruct* > traceMonitorStructs;
        void addTraceMonitorStructs( const std::string &chargeName );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< chargeStructs::monitorStruct * > continuousMonitorStructs;
        std::vector< chargeStructs::monitorStruct * > continuousTraceMonitorStructs;
        std::vector< chargeStructs::monitorStruct * > continuousNumMonitorStructs;

        std::map< std::string, chargeStructs::chargeObject > allchargeData; /// All the charge data is stored in this map, keyed by the charge name

        std::vector< std::vector< double > > allchargeMonitorData;
        std::vector< std::vector< double > > traceMonitorData;
        std::map< bool, chargeStructs::chargeObject* > isMonitoringMap;
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
