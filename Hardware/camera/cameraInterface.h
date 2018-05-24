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
using namespace cameraStructs;

class cameraInterface : public interface
{
    public:
        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        cameraInterface();
        cameraInterface(bool& show_messages_ptr,
                        bool& show_debug_messages_ptr);
        ~cameraInterface();

        map_ilck_state getILockStates(const std::string &name)
        {
            IlockMap1 r;
            return r;
        }
        map_ilck_string getILockStatesStr(const std::string &name)
        {
            IlockMap2 r;
            return r;
        }

        ///Objects to hold Data///
        std::vector<monitorDAQStruct*> continuousMonitorDAQStructs;
        std::vector<monitorIAStruct*> continuousMonitorIAStructs;
        std::map<std::string,cameraObject>    allCamData;

        //cameraIAObject   selectedIACamera;
        //cameraIAObject  &selectedIACameraRef;
        //cameraIAObject   vcIACamera;
        //cameraIAObject  &vcIACameraRef;

        cameraObject  selectedCameraObj;
        cameraObject &selectedCameraRef;
        cameraObject  vcCameraObj;
        cameraObject &vcCameraRef;

        ///Functions Accessible to Python Controller///
        bool isON (const std::string &cam); //Can use Camera name or asociated
        bool isOFF(const std::string &cam); //screen name.
        bool isAcquiring(const std::string &cam);
        bool isNotAcquiring(const std::string &cam);
        std::string selectedCamera();
        bool setCamera(const std::string &cam);
        bool startAcquiring();
        bool stopAcquiring();
        bool startVCAcquiring();//VC independant of all other cameras
        bool stopVCAcquiring();
        vec_s getCameraNames();

        ///Useful Functions for the Controller///
        bool isCollecting(const std::string&cameraName);
        bool isSaving(const std::string&cameraName);


        bool isCollectingJPG(const std::string&cameraName);
        bool isSavingJPG(const std::string&cameraName);
        std::string useCameraFrom(const std::string camOrScreen);
        bool shortCaput(const unsigned short &comm, pvStruct &S);
        bool doubleCaput(const double &comm, pvStruct& S);

    protected:
        void addChannel(const std::string &pvRoot, pvStruct &pv);

    private:

};
#endif // CAM_INTERFACE_H
