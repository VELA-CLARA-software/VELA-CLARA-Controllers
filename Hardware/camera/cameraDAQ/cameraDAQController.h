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

#ifndef CAM_DAQ_CONTROLLER_H
#define CAM_DAQ_CONTROLLER_H
// stl
#include <string>
#include <vector>
#include <map>
// djs
#include "cameraDAQInterface.h"
#include "structs.h"
#include "cameraStructs.h"
#include "controller.h"


class cameraDAQController  : public controller
{
    public:
        cameraDAQController();
        cameraDAQController(const bool show_messages,
                            const bool show_debug_messagese,
                            const std::string &camConf,
                            const bool startVirtualMachine,
                            const bool shouldStartEPICs,
                            const VELA_ENUM::MACHINE_AREA myMachineArea);
        ~cameraDAQController( );
        // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );
        // This pure virtual method MUST be overwritten in the derived controller ( making this an abstract base class)
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name );

        ///Functions Accessible to Python Controller///
        //Generic Functions
        bool isON ( const std::string & cam );
        bool isOFF( const std::string & cam );
        bool isAquiring( const std::string & cam );
        bool isNotAquiring( const std::string & cam );
        std::string selectedCamera();
        bool setCamera(const std::string & cam);
        bool startAcquiring();
        bool stopAcquiring();
        bool startVCAcquiring();
        bool stopVCAcquiring();
        //DAQ Specific Functions
        bool collectAndSave (const int & numbOfShots);
        bool killCollectAndSave();
        const cameraStructs::cameraDAQObject &getCamDAQObjConstRef( const std::string & camName  );
        const cameraStructs::cameraDAQObject &getSelectedDAQRef();
        const cameraStructs::cameraDAQObject &getVCDAQRef();

    protected:
    private:
        // The interface to EPICS
        cameraDAQInterface  localInterface;
        void initialise();
        const bool shouldStartEPICs;
        const VELA_ENUM::MACHINE_AREA myMachineArea;
};

#endif // cameraDAQController_H
