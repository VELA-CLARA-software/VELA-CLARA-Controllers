#ifndef VELA_CAM_INTERFACE_H
#define VELA_CAM_INTERFACE_H

#include <string>
#include <vector>
#include <atomic>
#include <map>

#include "cameraStructs.h"
#include "interface.h"
#include "cameraConfigReader.h"

class cameraInterface : public interface
{
    public:

        typedef std::vector< bool > vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector< double > vec_d;

        cameraInterface( const bool* show_messages_ptr, const bool* show_debug_messages_ptr  );
        cameraInterface( const std::string &configFile_Location, const std::string &configIOCFile_Location, const bool* show_messages_ptr, const bool* show_debug_messages_ptr  );
        ~cameraInterface();

        bool isON ( const std::string & cam );
        bool isOFF( const std::string & cam );

        bool start(const std::string & cam );
        bool start( const vec_s & cam );
        bool stop(const std::string & cam );
        bool stop( const vec_s & cams );

        bool startAndWait( const std::string & cam, size_t waitTime );
        vec_s startAndWait( const vec_s & cam, size_t waitTime );



        bool stopAll( );

        bool startCamDataMonitor( const std::string & cam, int N = -1 );

        bool isMonitoring( const std::string & cam );
        bool isNotMonitoring( const std::string & cam );
        bool entryExistsAndIsMonitoring( const std::string & cam, std::string & camToReturn );
        bool entryExistsAndIsNotMonitoring( const std::string & cam, std::string & camToReturn );


        std::vector< std::vector< cameraStructs::camDataType >> getRawData( const std::string & name  );
        std::vector< double > getRawDataStamp( const std::string & name  );
        std::vector< std::string > getRawDataStampStr( const std::string & name );



    protected:

    private:

    /// called from constructor to set-up chids, monitors, etc.
        void initialise();
        cameraConfigReader configReader; /// class member so we can pass in file path in ctor
        bool initObjects();
        void initChids();

        double numActiveCams;

        static void staticEntryMonitor(const event_handler_args args);

        void startMonitors();
        void addChannel( const std::string & pvRoot, cameraStructs::pvStruct & pv );
        void updateCamStatus( unsigned short value, const std::string & name );


        bool toggleCam( const vec_s & cams, cameraStructs::CAM_STATUS onORoff );
        bool sendCommand( const std::vector< chtype* > & CHTYPE, const std::vector< chid* > & CHID, const std::string & m1, const std::string & m2  );


        std::map< std::string, cameraStructs::camObject > allCamData;
        cameraStructs::camIOCObject camIOC;



        bool checkIfScreenName( const std::string & name, std::string & camName );


        void processData();



        void resetDataVectors( const std::string & cam, int N );

        static void staticEntryCamMonitor( const event_handler_args args );
        static void staticEntryCamMonitorStart( cameraStructs::camDAQStruct & ds  );


        void updateLongData( const std::string & name , const size_t  dataVecPart, const size_t count, const dbr_long_t * value );

        void updateTimeLongData( cameraStructs::camDAQStruct * ms,  size_t dataVecPart, const event_handler_args & args  );

        void killCallBack(  cameraStructs::camDAQStruct * ms  );


        void killCompletedThreads();

        size_t camDAQNum;

        std::map< std::string,  std::atomic< bool > > isMonitoringMap; /// std::atomic< bool > are not CopyConstructible, so this is held locally


        /// camAqStructsMap ? siz_t is threadID also, continaed in camDAQStruct
        std::map< size_t, cameraStructs::camDAQStruct > camDAQStructMap; ///


        void killMonitor( cameraStructs::monitorStruct * ms );

        std::vector<  cameraStructs::monitorStruct* > continuousMonitorStructs; /// vector of monitors to pass along with callback function

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name ) {
            std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
            return r;}
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name ) {
            std::map< VELA_ENUM::ILOCK_NUMBER, std::string > r;
            return r;}


};
#endif // VELA_CAM_INTERFACE_H
