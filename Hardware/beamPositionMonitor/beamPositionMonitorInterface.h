#ifndef VELA_BPM_INTERFACE_H
#define VELA_BPM_INTERFACE_H
// djs
#include "interface.h"
#include "beamPositionMonitorStructs.h"
#include "beamPositionMonitorConfigReader.h"
//stl
#include <vector>
#include <string>

class beamPositionMonitorInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        beamPositionMonitorInterface(const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        beamPositionMonitorInterface( const std::string & configFileLocation, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );

        ~beamPositionMonitorInterface();

        bool monitoringData = false;

        double calcX( const std::string & bpm, double u11, double u12, double u13, double u14 );
        double calcY( const std::string & bpm, double u21, double u22, double u23, double u24 );
        double calcQ( const std::string & bpm, std::vector< double > rawData );
        double getX( const std::string & bpm );
        double getY( const std::string & bpm );
        double getQ( const std::string & bpm );
        double getXFromPV(  const std::string & bpm  );
        double getYFromPV(  const std::string & bpm  );
        beamPositionMonitorStructs::rawDataStruct getAllBPMData( const std::string & name, size_t N );
        std::vector< std::vector< double > > getBPMRawData( const std::string & bpmName );
        std::vector< double > getBPMXVec( const std::string & bpmName );
        std::vector< double > getBPMYVec( const std::string & bpmName );
        std::vector< double > getBPMQVec( const std::string & bpmName );
        std::vector< double > getTimeStamps( const std::string & bpmName );
        std::vector< std::string > getStrTimeStamps( const std::string & bpmName );
        long getRA1(  const std::string & bpm  );
        long getRA2(  const std::string & bpm  );
        long getRD1(  const std::string & bpm  );
        long getRD2(  const std::string & bpm  );
        void setSA1(  const std::string & bpmName, long sa1 );
        void setSA2(  const std::string & bpmName, long sa1 );
        void setSD1(  const std::string & bpmName, long sa1 );
        void setSD2(  const std::string & bpmName, long sa1 );
        void reCalAtt( const std::string & bpmName, double qScope );
        void addToMonitorStructs( std::vector< beamPositionMonitorStructs::monitorStruct * > & msv, beamPositionMonitorStructs::pvStruct & pv,  beamPositionMonitorStructs::bpmDataObject * bpmObj );
        void monitorDataForNShots( size_t N, const std::string & bpmNames );

        std::vector< std::string > getBPMNames();

        void updateData( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );
        void updateValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args );
        bool hasTrig( const std::string & bpm );
        bool hasNoTrig( const std::string & bpm );
        std::map< const std::string, bool > isMonitoring;

        beamPositionMonitorStructs::bpmObject bpmObj;
        beamPositionMonitorStructs::bpmObject getBPMObject( const std::string & bpmName );

        void killCallBack( beamPositionMonitorStructs::monitorStruct * ms );///, beamPositionMonitorStructs::bpmDataObject *bpmdo );

        VELA_ENUM::TRIG_STATE getBPMState( const std::string & bpmName );

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & bpmName );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & bpmName );

    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.

        void initialise();

        beamPositionMonitorConfigReader configReader;

        void initBPMObjects();
        void initBPMChids();
        //void addChannel( std::map< std::string, beamPositionMonitorStructs::bpmObject >::iterator & it1, std::map< beamPositionMonitorStructs::BPM_PV_TYPE, std::string >::const_iterator & it2 );
        void addChannel( const std::string & pvRoot, beamPositionMonitorStructs::pvStruct & pv );

        void monitorBPMs();

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the bpm is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members

        typedef  bool (beamPositionMonitorInterface::*isOCMemFn)( const std::string & );
        typedef  void (beamPositionMonitorInterface::*OCMemFn)  ( const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, beamPositionMonitorInterface & obj );

        /// static function that can be called back from epics to update values

        static void staticEntryrMonitor( const event_handler_args args );
        void resetDataVectors( size_t N );
        bool isADataPV( beamPositionMonitorStructs::BPM_PV_TYPE pv );


        std::vector< beamPositionMonitorStructs::monitorStruct* > temporaryMonitorStructs;

        void * addTemporaryMonitorStruct( beamPositionMonitorStructs::bpmObject * bpmObjp , beamPositionMonitorStructs::BPM_PV_TYPE monType, std::vector< std::vector< double > > & bpmData, beamPositionMonitorStructs::dataCollectionResult * stat);

//        velaScopeInterface localScopeInterface;

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
