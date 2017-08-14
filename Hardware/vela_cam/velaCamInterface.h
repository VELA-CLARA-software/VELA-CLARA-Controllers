#ifndef VELACAMINTERFACE_H
#define VELACAMINTERFACE_H

#include <string>
#include <vector>
#include <map>

#include "velaCamStructs.h"

class velaCamInterface : public interface
{
    public:
        velaCamInterface(const std::string &allGunProtsConf,
                                const bool startVirtualMachine,
                                const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                                const bool shouldStartEPICs ):
        virtual ~velaCamInterface();

        void startMonitors();
        void startMonitors(const std::vector< std::string > & screenName );

        bool getCamDat( const std::string & screenName, size_t numShots, unsigned char * ptrToVecToFill, size_t totNumDataPoints );

        int  acquiringData(const std::string & screenName );
        bool accquiringData(const std::string & screenName );

        void serverON(const std::string & screenName  );

        void getActiveCameraServers(std::map< std::string, velaCamStructs::CAM_SERVER_STATE > & activeMap ) ;

        std::vector< std::string > getActiveCameraServers( );

    /// If joining from another thread run this to attach to the main context

       void attachContext();

    protected:

    private:

    /// Private constructor for singletons...

        velaCamInterface();
        static bool instanceFlag;
        static int SHOTCOUNT; /// meh
        static velaCamInterface * velaCamInterfaceObject; /// This class is a singleton, see .cpp

    #ifndef _OFFLINE_MODE
        ca_client_context * thisCaContext;
        chid activeCamera;
        static void staticEntryCamServerStatusMonitor( struct event_handler_args args ); /// static function that can be called back from epics
        static void staticEntryCamDataMonitor(struct event_handler_args args);
    #endif

    /// Set up the magnetObject map, connect to channels

        void initCamObjects();
        void getCamChIDs();

    /// All the camera data is stored in this map!

        std::map< std::string, velaCamStructs::camObject > allCameraData;

    /// vector of monitorStructs to pass along with callback function

        std::vector< velaCamStructs::monitorStuct* > continuousMonitorStructs;
        bool serverIsNotBeingMonitored( const std::string & screenName );
        void addContinuousMonitorStructs(const std::string & screenName );

    /// This is the struct Used for each data acquisition, when multiple cameras become available this will get trickier...

        velaCamStructs::monitorStuct dataMonitorStruct;
        enum velaCamStructs::CAM_SERVER_STATE getState(unsigned short epicsSta);

    /// Monitor Server Status

        void checkServerStatus( const std::vector< std::string > & screenName );
        void checkServerStatus( const std::string & screenName );
        void allServerOFF();

        void printStatusResult( int status, const char * success, const char* timeout );
};
#endif // VELACAMINTERFACE_H
