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

#ifndef CAM_DAQ_INTERFACE_H
#define CAM_DAQ_INTERFACE_H

#include <string>
#include <vector>
#include <atomic>
#include <map>

#include "cameraStructs.h"
#include "cameraInterface.h"
#include "cameraDAQConfigReader.h"

using namespace cameraStructs;

class cameraDAQInterface : public cameraInterface
{
    public:
        cameraDAQInterface();
        cameraDAQInterface( const std::string &Conf,
                         const bool startVirtualMachine,
                         bool& show_messages_ptr,
                         bool& show_debug_messages_ptr,
                         const bool shouldStartEPICs,
                         const HWC_ENUM::MACHINE_AREA myMachineArea );
        ~cameraDAQInterface();


        ///Functions Accessible to Python Controller///
        bool collectAndSave (const int & numbOfShots);
        bool collectAndSaveJPG ();
        bool collectAndSaveVC (const int & numbOfShots);
        bool staticCollectAndSave(cameraObject camera,const int & numbOfShots);
        bool staticCollectAndSaveJPG(cameraObject camera,const int & numbOfShots);
        bool killCollectAndSave();
        bool killCollectAndSaveJPG();
        bool killCollectAndSaveVC();
        bool collect(cameraObject camera,unsigned short &comm, const int & numbOfShots);
        bool save(cameraObject camera,unsigned short &comm);
        bool collectJPG(cameraObject camera,unsigned short &comm,const int & numbOfShots);
        bool saveJPG(cameraObject camera,unsigned short &comm);
        std::string getlatestDirectory();

        const cameraObject &getCamDAQObjConstRef(const std::string &cam);
        const cameraObject &getSelectedDAQRef();
        const cameraObject &getVCDAQRef();

    protected:
    private:
        const HWC_ENUM::MACHINE_AREA myMachineArea;
        const bool shouldStartEPICs;
        void initialise();
        cameraDAQConfigReader configReader;
        bool initObjects();
        void initChids();
        void startMonitors();
        void killMonitor( cameraStructs::monitorDAQStruct * ms );
        static void staticEntryDAQMonitor(const event_handler_args args);
        void updateState(const unsigned short value,const std::string &cameraName);
        void updateAcquiring(const unsigned short value,const std::string &cameraName);
        void updateCapturing(const unsigned short value,const std::string &cameraName);
        void updateWriteState(const unsigned short value,const std::string &cameraName);
        void updateWriteCheck(const unsigned short value,const std::string &cameraName);
        void updateNumCaptured(const unsigned long value,const std::string &cameraName);

        void updateNumCapturedJPG(const unsigned long value,const std::string &cameraName);
        void updateNumCaptureJPG(const unsigned long value,const std::string &cameraName);

        void updateNumCapture(const unsigned long value,const std::string &cameraName);
        void updateWriteErrorMessage(const void *const value,const std::string &cameraName);
        void updateExposure(const double value,const std::string &cameraName);
        void updateAcquirePeriod(const double value,const std::string &cameraName);
        void updateFrequency(const double value,const std::string &cameraName);
        void updateSensorTemp(const double value,const std::string &cameraName);

        void updateWriteCheckJPG(const unsigned short value,const std::string&cameraName);
        void updateWriteStateJPG(const unsigned short value,const std::string&cameraName);
        void updateCapturingJPG(const unsigned short value,const std::string&cameraName);
        void updateWriteErrorMessageJPG(const void *value,const std::string&cameraName);


        ///Useful Functions for the Class///
        std::string getWriteErrorMessage();
        std::string getWriteErrorMessageJPG();
        bool makeANewDirectoryAndName(cameraObject &camera,const int &numbOfShots);
        bool makeANewDirectoryAndNameJPG(cameraObject &camera,const int &numbOfShots);
        bool setNumberOfShots(const int &numberOfShots);
        bool setNumberOfShotsJPG(const int &numberOfShots);
        bool setStartFileNumber(const int &startNumber);
        bool setStartFileNumberJPG(const int &startNumber);
        void updateSelectedOrVC(const std::string cameraName);

};
#endif // CAM_DAQ_INTERFACE_H
