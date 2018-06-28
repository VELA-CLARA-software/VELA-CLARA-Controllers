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
                         const bool& show_messages_ptr,
                         const bool& show_debug_messages_ptr,
                         const bool shouldStartEPICS,
                         const bool startVirtualMachine,
                         const HWC_ENUM::MACHINE_AREA myMachineArea );
        ~chargeInterface();

        void getChargeNames( std::vector< std::string >  & chargeNames );

        void setBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
//        void monitorForNShots( const std::vector< std::string > charges, size_t N );
//        void monitorForNShots( const std::string charge, size_t N );
        void cancelDataMonitors();
        size_t getBufferSize( const std::string & chargeName );
        const chargeStructs::CHARGE_DIAG_TYPE getDiagType( const std::string & chargeName );
        const std::string getDiagTypeStr( const std::string & chargeName );
        const chargeStructs::dataObject & getChargeDataStruct( const std::string & chargeName );
        std::vector< std::vector< double > > getChargeData( const std::string & name );
        boost::circular_buffer< double > getChargeBuffer( const std::string & name );
        boost::circular_buffer< double > getVoltageBuffer( const std::string & name );
        boost::circular_buffer< double > getWCMChargeBuffer();
        boost::circular_buffer< double > getWCMVoltageBuffer();
        boost::circular_buffer< double > getS02FCUPChargeBuffer();
        boost::circular_buffer< double > getS02FCUPVoltageBuffer();
        boost::circular_buffer< double > getSP1FCUPChargeBuffer();
        boost::circular_buffer< double > getSP1FCUPVoltageBuffer();
        std::vector< double > getTimeStamps( const std::string & name );
        std::vector< std::string > getStrTimeStamps( const std::string & name );
        std::vector< std::string > getChargeDiagnosticNames();
        std::vector< std::string > getChargePVs();
        double getCharge( const std::string & name );
        double getVoltage( const std::string & name );
        double getWCMCharge();
        double getWCMVoltage();
        double getS02FCUPCharge();
        double getS02FCUPVoltage();
        double getSP1FCUPCharge();
        double getSP1FCUPVoltage();
        bool monitoringCharge = false;
        const bool isChargeBufferFull( const std::string & chargeName );
        const bool isChargeBufferNotFull( const std::string & chargeName );
        const bool isVoltageBufferFull( const std::string & chargeName );
        const bool isVoltageBufferNotFull( const std::string & chargeName );
        const bool isWCMBufferFull();
        const bool isWCMBufferNotFull();
        const bool isS02FCUPBufferFull();
        const bool isS02FCUPBufferNotFull();
        const bool isSP1FCUPBufferFull();
        const bool isSP1FCUPBufferNotFull();

        const chargeStructs::chargeObject getChargeObject( const std::string & chargeName );

        void killTraceCallBack( chargeStructs::monitorStruct * ms );
        void killCallBack( chargeStructs::monitorStruct *& ms );
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

        bool initChargeObjects();
        void initChargeChids();
        void addChannel( const std::string & pvRoot, chargeStructs::pvStruct & pv );
        void monitorCharges();
        void clearContinuousMonitorStructs();
        void addToMonitorStructs( std::vector< chargeStructs::monitorStruct * > & msv, chargeStructs::pvStruct & pv,  chargeStructs::dataObject * datobj );
        void updateValue( chargeStructs::monitorStruct * ms, const event_handler_args args );
//        void addToNumMonitorStructs( std::vector< chargeStructs::monitorStruct * > & msv, chargeStructs::pvStruct & pv,  chargeStructs::chargeNumObject * numobj );

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the charge is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members
        typedef  bool (chargeInterface::*isOCMemFn)(const std::string & );
        typedef  void (chargeInterface::*OCMemFn)  (const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, chargeInterface & obj );

        /// static function that can be called back from epics to update values
        static void staticEntryrMonitor( const event_handler_args args );

        void resetChargeVectors( size_t N );
        void resetAChargeVector( const std::string chargeName, size_t N );

        chargeStructs::chargeObject chargeObj;

        std::vector< chargeStructs::monitorStruct* > chargeMonitorStructs;
        void addTraceMonitorStructs( const std::string &chargeName );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< chargeStructs::monitorStruct * > continuousMonitorStructs;

        std::map< std::string, chargeStructs::chargeObject > allChargeData; /// All the charge data is stored in this map, keyed by the charge name

        std::vector< std::vector< double > > allChargeMonitorData;
        std::vector< std::vector< double > > traceMonitorData;
        std::map< bool, chargeStructs::dataObject* > isMonitoringMap;
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
