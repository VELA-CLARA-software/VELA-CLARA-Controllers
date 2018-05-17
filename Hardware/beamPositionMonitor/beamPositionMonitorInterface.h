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

#ifndef VELA_BPM_INTERFACE_H
#define VELA_BPM_INTERFACE_H
// djs
#include "interface.h"
#include "beamPositionMonitorStructs.h"
#include "beamPositionMonitorConfigReader.h"
#include "structs.h"
//stl
#include <vector>
#include <string>
#include <boost/circular_buffer.hpp>

class beamPositionMonitorInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        beamPositionMonitorInterface::beamPositionMonitorInterface();//const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        beamPositionMonitorInterface( const std::string & configFileLocation,
                                      const bool& show_messages_ptr,
                                      const bool& show_debug_messages_ptr,
                                      const bool shouldStartEPICS,
                                      const bool startVirtualMachine,
                                      const HWC_ENUM::MACHINE_AREA myMachineArea );

        ~beamPositionMonitorInterface();

        bool monitoringData = false;
        bool isMonitoringBPMData( const std::string & bpmName );
        bool isNotMonitoringBPMData( const std::string & bpmName );
        const bool isDataBufferFull( const std::string & bpmName );
        const bool isDataBufferNotFull( const std::string & bpmName );
        const bool isXBufferFull( const std::string & bpmName );
        const bool isYBufferFull( const std::string & bpmName );
        const bool isXBufferNotFull( const std::string & bpmName );
        const bool isYBufferNotFull( const std::string & bpmName );

        double calcX( const std::string & bpm, double u11, double u12, double u13, double u14, double mn, double xn );
        double calcY( const std::string & bpm, double u21, double u22, double u23, double u24, double mn, double yn );
        double calcQ( const std::string & bpm, std::vector< double > rawData, double att1cal, double att2cal, double v1cal, double v2cal, double qcal );
        double getBPMResolution( const std::string & bpm );
        double getX( const std::string & bpm );
        double getY( const std::string & bpm );
        double getQ( const std::string & bpm );
        double getPosition( const std::string & bpm );
        double getXFromPV(  const std::string & bpm  );
        double getYFromPV(  const std::string & bpm  );
        const beamPositionMonitorStructs::bpmDataObject & getBPMDataObject( const std::string & name );
        const size_t getBufferSize();
        std::vector< std::vector< double > > getBPMRawData( const std::string & bpmName );
        std::vector< double > getBPMXVec( const std::string & bpmName );
        std::vector< double > getBPMYVec( const std::string & bpmName );
        std::vector< double > getBPMQVec( const std::string & bpmName );
        std::vector< double > getTimeStamps( const std::string & bpmName );
        std::vector< std::string > getStrTimeStamps( const std::string & bpmName );
        boost::circular_buffer< std::vector< double > > getBPMRawDataBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMXPVBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMYPVBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMXBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMYBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMQBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getTimeStampsBuffer( const std::string & bpmName );
        boost::circular_buffer< std::string > getStrTimeStampsBuffer( const std::string & bpmName );
        long getRA1( const std::string & bpm );
        long getRA2( const std::string & bpm );
        long getRD1( const std::string & bpm );
        long getRD2( const std::string & bpm );
        long getSA1( const std::string & bpm );
        long getSA2( const std::string & bpm );
        long getSD1( const std::string & bpm );
        long getSD2( const std::string & bpm );
        void setSA1( const std::string & bpmName, long sa1 );
        void setSA2( const std::string & bpmName, long sa1 );
        void setSD1( const std::string & bpmName, long sa1 );
        void setSD2( const std::string & bpmName, long sa1 );
        void setX( const std::string & bpmName, double val );
        void setY( const std::string & bpmName, double val );
        void setBufferSize( const std::string & bpmName, size_t bufferSize );
        void reCalAttenuation( const std::string & bpmName, double qScope );
        void addToMonitorStructs( std::vector< beamPositionMonitorStructs::monitorStruct * > & msv, beamPositionMonitorStructs::pvStruct & pv,
                                  beamPositionMonitorStructs::bpmDataObject * bpmObj );
        void monitorDataForNShots( size_t N, const std::vector< std::string > & bpmNames );
        void monitorDataForNShots( size_t N, const std::string & name );
        void setBufferSize( size_t bufferSize );
        void clearBuffers();

        std::vector< std::string > getBPMNames();
        bool hasTrig( const std::string & bpm );
        bool hasNoTrig( const std::string & bpm );
        std::map< const std::string, bool > isMonitoring;

        beamPositionMonitorStructs::bpmObject bpmObj;
        beamPositionMonitorStructs::bpmObject getBPMObject( const std::string & bpmName );

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & bpmName )const;
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & bpmName )const;

    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.
        void initialise();
        const HWC_ENUM::MACHINE_AREA machineArea;

        beamPositionMonitorConfigReader configReader;

        bool initBPMObjects();
        void initBPMChids();
        void addChannel( const std::string & pvRoot, beamPositionMonitorStructs::pvStruct & pv );

        void monitorBPMs();
        void updateLong( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );
        void updateLong1( std::string name, long args );
        void updateData( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );
        void updateValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );
        void updateXValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );
        void updateYValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );

        /// As an overly complicated example let's try some function pointers. Toggling (open / close) the bpm is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members
        typedef bool (beamPositionMonitorInterface::*isOCMemFn)( const std::string & );
        typedef void (beamPositionMonitorInterface::*OCMemFn)  ( const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, beamPositionMonitorInterface & obj );

        /// static function that can be called back from epics to update values
        static void staticEntryrMonitor( const event_handler_args args );
        void resetDataVectors( size_t N );
        bool isADataPV( beamPositionMonitorStructs::BPM_PV_TYPE pv );

        void killMonitor( beamPositionMonitorStructs::monitorStruct * ms );
        void killCallBack( beamPositionMonitorStructs::monitorStruct * ms, beamPositionMonitorStructs::rawDataStruct * bpmdo );
        void killCallBack( beamPositionMonitorStructs::monitorStruct * ms, beamPositionMonitorStructs::bpmDataObject * bpmdo );
        void * addTemporaryMonitorStruct( beamPositionMonitorStructs::bpmObject * bpmObjp , beamPositionMonitorStructs::BPM_PV_TYPE monType, std::vector< std::vector< double > > & bpmData, beamPositionMonitorStructs::dataCollectionResult * stat);

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< beamPositionMonitorStructs::monitorStruct * > continuousMonitorStructs;
        std::vector< beamPositionMonitorStructs::monitorStruct * > dataMonitorStructs;

        std::map< std::string, beamPositionMonitorStructs::bpmObject > allBPMData; /// All the bpm data is stored in this map, keyed by the bpm name

        std::vector< std::vector< double > > allBPMMonitorData;

        std::map< bool, beamPositionMonitorStructs::bpmObject* > isMonitoringMap;
//
        bool beamPositionMonitorInterface::allDataIsCollected( std::vector< beamPositionMonitorStructs::dataCollectionResult > status );
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
