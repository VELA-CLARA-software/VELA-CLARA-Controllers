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

#ifndef CAM_INTERFACE_H
#define CAM_INTERFACE_H
//tp
#include "cameraStructs.h"
#include "interface.h"
//stl
#include <string>
#include <vector>
#include <atomic>
#include <map>

class cameraInterface : public interface
{
    public:
        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        cameraInterface();
        cameraInterface(const bool* show_messages_ptr,
                        const bool* show_debug_messages_ptr);
        ~cameraInterface();
        IlockMap1 getILockStates( const std::string & name   ){ IlockMap1 r;return r; }
        IlockMap2 getILockStatesStr( const std::string & name){ IlockMap2 r;return r; }

        ///Functions Accessible to Python Controller///
        bool isON ( const std::string & cam );
        bool isOFF( const std::string & cam );
        bool isAquiring( const std::string & cam );
        bool isNotAquiring( const std::string & cam );
        // we're going have cameras named by screen AND camera number
        std::string selectedCamera();
        bool setCamera(const std::string & cam);
        bool startAquiring();
        bool stopAquiring();
        // assume the VC can act independant of all other cameras
        bool startVCAquiring();
        bool stopVCAquiring();

        std::vector<cameraStructs::monitorDAQStruct*> continuousMonitorDAQStructs;
        std::map< std::string, cameraStructs::cameraDAQObject > allCamDAQData;
        std::map< std::string, cameraStructs::cameraIAObject > allCamIAData;

        cameraStructs::cameraIAObject *selectedIACamera;
        cameraStructs::cameraDAQObject selectedDAQCamera;
        cameraStructs::cameraDAQObject &selectedDAQCameraRef;
        cameraStructs::cameraIAObject *vcIACamera;
        cameraStructs::cameraDAQObject vcDAQCamera;
        cameraStructs::cameraDAQObject &vcDAQCameraRef;
        ///Useful Functions for the Controller///
        bool isCollecting(const std::string&cameraName);
        bool isSaving(const std::string&cameraName);
        std::string useCameraFrom(const std::string camOrScreen);
    protected:
        void addChannel( const std::string & pvRoot, cameraStructs::pvStruct & pv );


    private:


};
#endif // CAM_INTERFACE_H
