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

class cameraDAQInterface : public cameraInterface
{
    public:
        cameraDAQInterface();
        cameraDAQInterface( const std::string &Conf,
                         const bool startVirtualMachine,
                         const bool* show_messages_ptr,
                         const bool* show_debug_messages_ptr,
                         const bool shouldStartEPICs,
                         const VELA_ENUM::MACHINE_AREA myMachineArea );
        ~cameraDAQInterface();

        bool collectAndSave (const int & numbOfShots);
        //bool collectAndSave (const int & numbOfShots, const std::string & comments);
        bool killCollectAndSave();
        bool collect(unsigned short &comm);
        bool write(unsigned short &comm);
        bool makeANewDirectory();
        const cameraStructs::cameraDAQObject &getCamDAQObjConstRef( const std::string & camName  );
        const cameraStructs::cameraDAQObject &getSelectedDAQRef();
        cameraStructs::cameraDAQObject* getSelectedDAQPtr();
        const cameraStructs::cameraDAQObject &getVCDAQRef();
    protected:

    private:
        //void cameraDAQInterface::addChannel( const std::string & pvRoot, cameraStructs::pvStruct & pv )
        const VELA_ENUM::MACHINE_AREA myMachineArea;
        const bool shouldStartEPICs;
        void initialise();
        cameraDAQConfigReader configReader;
        bool initObjects();
        void initChids();
        void startMonitors();
        static void staticEntryDAQMonitor(const event_handler_args args);
        void updateState(const unsigned short value,const std::string&cameraName);
        void updateAquiring(const unsigned short value,const std::string&cameraName);
        void updateWriteState(const unsigned short value,const std::string&cameraName);
        void updateNumCaptured(const unsigned long value,const std::string&cameraName);
        void updateNumCapture(const unsigned long value,const std::string&cameraName);
        std::string getWriteMessage();
        void killMonitor( cameraStructs::monitorDAQStruct * ms );
};
#endif // CAM_DAQ_INTERFACE_H
