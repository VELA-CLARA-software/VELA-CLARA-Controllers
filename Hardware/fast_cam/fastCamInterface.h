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
//  Last edit:   17-05-2018
//  FileName:    fastCamInterface.h
//  Description:
//
//
//*/
#ifndef _FAST_CAM_INTERFACE_H
#define _FAST_CAM_INTERFACE_H
// project includes
#include "interface.h"
#include "fastCamStructs.h"
#include "fastCamConfigReader.h"
// stl includes
#include <string>
#include <vector>
#include <map>
//______________________________________________________________________________
class fastCamInterface : public interface
{
    public:
        fastCamInterface(const std::string& velaCamConf,
                         const std::string& claraCamConf,
                         const bool startVirtualMachine,
                         const bool& show_messages,
                         const bool& show_debug_messages,
                         const bool shouldStartEPICs,
                         const HWC_ENUM::MACHINE_AREA myMachineArea);
        ~fastCamInterface();

//        void startMonitors();
//        void startMonitors(const std::vector< std::string > & screenName );
//
//        bool getCamDat( const std::string & screenName, size_t numShots, unsigned char * ptrToVecToFill, size_t totNumDataPoints );
//
//        int  acquiringData(const std::string & screenName );
//        bool accquiringData(const std::string & screenName );
//
//        void serverON(const std::string & screenName  );
//
//        void getActiveCameraServers(std::map< std::string, velaCamStructs::CAM_SERVER_STATE > & activeMap ) ;
//
//        std::vector< std::string > getActiveCameraServers( );


    protected:

    private:

    /// Private constructor for singletons...

        fastCamInterface();

        void initialise();
        bool initObjects();
        void addDummyElementToAllMAgnetData();
        void initChids();

    /// All the camera data is stored in this map!

        std::map<std::string, fastCamStructs::fastCamObject> cameraObjects;

    /// vector of monitorStructs to pass along with callback function

        std::vector<fastCamStructs::monitorStruct*> continuousMonitorStructs;
        void addContinuousMonitorStructs(const std::string& screenName );

        const HWC_ENUM::MACHINE_AREA myMachineArea;

        static void staticEntryMonitor(const event_handler_args args);
        void updateValue(const fastCamStructs::FAST_CAM_PV pv,
                         const std::string& name,
                         const event_handler_args& args);

        fastCamConfigReader configReader;
        void startMonitors();
        void addChannel(const std::string& pvRoot,
                        fastCamStructs::pvStruct& pv);

//        void checkServerStatus(const std::vector<std::string>& screenName);
//        void checkServerStatus(const std::string& screenName);
//        void allServerOFF();

};
#endif // _FAST_CAM_INTERFACE_H
