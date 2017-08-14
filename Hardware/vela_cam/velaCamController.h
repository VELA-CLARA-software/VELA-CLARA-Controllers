#ifndef VELACAMCONTROLLER_H
#define VELACAMCONTROLLER_H
// stl
#include <string>
#include <vector>
#include <map>
// djs
#include "velaCamStructs.h"
#include "controller.h"
#include "velaCamInterface.h"

class cameraDataObject;
class cameraImageCropper;
class beamSizeCalculator;

class velaCamController : public controller
{
    public:
        velaCamController();
        velaCamController(const bool show_messages, const bool show_debug_messagese,
                          const std::string &velaCamConf, const bool startVirtualMachine,
                          const bool shouldStartEPICs );
        virtual ~velaCamController();

        void attachContext(); /// An EPICS thing

        void startMonitors();
        void startMonitors( const std::vector< std::string > & screenName );

    /// here bool & success is probably pointless as the testing happens in the gui when the graph is drawn... ?
    /// unless we pass a reference all the way from there....

        std::vector< unsigned char > & getRawRef ( const std::string & screenName, velaCamStructs::DATA_TYPE type);
        std::vector< double >        & getProcRef( const std::string & screenName, velaCamStructs::DATA_TYPE type );

        unsigned char * getRawRef ( const std::string & screenName, velaCamStructs::DATA_TYPE type, size_t part );
        double        * getProcRef( const std::string & screenName, velaCamStructs::DATA_TYPE type, size_t part );

    /// These are where the image is cut to get just the screen

        velaCamStructs::imgCutStruct getIMGCutParam( const std::string &  screenName );
        void setIMGCutParam( const std::string &  screenName, velaCamStructs::imgCutStruct & p );

    /// Clear and Resize the vectors

        void resetRaw( const std::string &  screenName, velaCamStructs::DATA_TYPE type, size_t numShots );

    ///

        void serverON( const std::string & screenName );
        void serverON( const char * screenName  );

        void getActiveCameraServers( std::map< std::string, velaCamStructs::CAM_SERVER_STATE > & activeMap );
        std::vector< std::string >  getActiveCameraServers(  );

    /// Get some new data, filling up the vector from startShot * (numPix) into the vector

        bool getData( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots );
        bool addData( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots, size_t startShot );

    /// Cut the image and do whatever processing is in the data object

        bool process( const std::string & screen, velaCamStructs::DATA_TYPE type );

    protected:
    private:

    /// The interface to EPICS
        //velaCamInterface   * camInterface;

//        cameraImageCropper * imageCropper;
//        beamSizeCalculator * bsCalculator;

    /// Checks if object for that screen exists or creates one

        void getObject( const std::string & screenName );

    /// Map of Data objects for each screen, holds raw and processed Data

        std::map< std::string, cameraDataObject * > allCamDataObjects;

        const bool shouldStartEPICs;
        velaCamInterface localInterface;


};

#endif // VELACAMCONTROLLER_H
