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
//  Last edit:   29-03-2018
//  FileName:    shutterController.h
//  Description:
//
//
//*/
#ifndef _SHUTTER_CONTROLLER_H_
#define _SHUTTER_CONTROLLER_H_
// project
#include "shutterStructs.h"
#include "shutterInterface.h"
#include "controller.h"
#include "structs.h"
// stl
#include <string>
#include <vector>
#include <map>
//______________________________________________________________________________
class shutterController : public controller
{
    public:
        shutterController(bool& show_messages,
                          bool& show_debug_messages,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const std::string& configFile,
                          const std::string& name
                          );
        shutterController& shutterController::operator= ( const shutterController& other ) = delete;
        ~shutterController();

        bool closeAndWait(const std::string& name, const time_t waitTime);
        bool openAndWait (const std::string& name, const time_t waitTime);
        bool isClosed(const std::string& name) const;
        bool isOpen  (const std::string& name) const;
        void close(const std::string& name);
        void open (const std::string& name);

        std::vector<std::string> getShutterNames()const;
        const shutterStructs::shutterObject&
            getShutterObjConstRef(const std::string& name )const;

        shutterStructs::SHUTTER_STATE getShutterState(const std::string& name);
        std::string getShutterStateStr(const std::string& name );

        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string& name)const;

//
//        bool openAndWaitShutter1 (const time_t waitTime = 2 );
//        bool openAndWaitShutter2 (const time_t waitTime = 2 );
//        bool closeAndWaitShutter1(const time_t waitTime = 2 );
//        bool closeAndWaitShutter2(const time_t waitTime = 2 );
//
        bool interfaceInitReport();
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);
//
    private:


        //void initialise();
        shutterInterface localInterface;
};
//______________________________________________________________________________
#endif // _SHUTTER_CONTROLLER_H_
