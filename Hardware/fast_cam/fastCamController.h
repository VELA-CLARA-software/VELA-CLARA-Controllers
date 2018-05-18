/*
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
//
//  Author:      DJS
//  Last edit:   16-05-2018
//  FileName:    fastCamController.cpp
//  Description:
//
//
//
//
//*/
#ifndef _FAST_CAM_CONTROLLER_
#define _FAST_CAM_CONTROLLER_
// project includes
#include "fastCamInterface.h"
#include "controller.h"

// stl
#include <string>
#include <vector>
#include <map>



class fastCamController : public controller
{
    public:
        fastCamController(bool& show_messages,
                          bool& show_debug_messagese,
                          const std::string &conf,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const HWC_ENUM::MACHINE_AREA myMachineArea,
                          const std::string& name
                          );
        fastCamController(bool& show_messages,
                          bool& show_debug_messagese,
                          const std::string &velaCamConf,
                          const std::string &claraCamConf,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const HWC_ENUM::MACHINE_AREA myMachineArea,
                          const std::string& name
                          );
        fastCamController& fastCamController::operator= ( const fastCamController& other ) = delete;
        ~fastCamController();

      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);

        //void startMonitors();
        //void startMonitors( const std::vector< std::string > & screenName );

    /// here bool & success is probably pointless as the testing happens in the gui when the graph is drawn... ?
    /// unless we pass a reference all the way from there....

        //std::vector< unsigned char > & getRawRef ( const std::string & screenName, velaCamStructs::DATA_TYPE type);
        //std::vector< double >        & getProcRef( const std::string & screenName, velaCamStructs::DATA_TYPE type );

        //unsigned char * getRawRef ( const std::string & screenName, velaCamStructs::DATA_TYPE type, size_t part );
        //double        * getProcRef( const std::string & screenName, velaCamStructs::DATA_TYPE type, size_t part );

    /// These are where the image is cut to get just the screen

        //velaCamStructs::imgCutStruct getIMGCutParam( const std::string &  screenName );
        //void setIMGCutParam( const std::string &  screenName, velaCamStructs::imgCutStruct & p );

    /// Clear and Resize the vectors

        //void resetRaw( const std::string &  screenName, velaCamStructs::DATA_TYPE type, size_t numShots );

    ///

        //void serverON( const std::string & screenName );
        //void serverON( const char * screenName  );

        //void getActiveCameraServers( std::map< std::string, velaCamStructs::CAM_SERVER_STATE > & activeMap );
        //std::vector< std::string >  getActiveCameraServers(  );

    /// Get some new data, filling up the vector from startShot * (numPix) into the vector

        //bool getData( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots );
        //bool addData( const std::string & screen, velaCamStructs::DATA_TYPE type, size_t numShots, size_t startShot );

    /// Cut the image and do whatever processing is in the data object

        //bool process( const std::string & screen, velaCamStructs::DATA_TYPE type );

    protected:
    private:

        fastCamInterface localInterface;


    /// The interface to EPICS
        //fastCamInterface   * camInterface;

//        cameraImageCropper * imageCropper;
//        beamSizeCalculator * bsCalculator;

    /// Checks if object for that screen exists or creates one

        //void getObject( const std::string & screenName );

    /// Map of Data objects for each screen, holds raw and processed Data

        //std::map< std::string, cameraDataObject * > allCamDataObjects;

};

#endif // _FAST_CAM_CONTROLLER_
