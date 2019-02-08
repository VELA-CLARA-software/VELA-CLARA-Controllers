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
//  FileName:    ba1StagesController.h
//  Description:
//
//
//*/
#ifndef _SHUTTER_CONTROLLER_H_
#define _SHUTTER_CONTROLLER_H_
// project
#include "ba1StagesStructs.h"
#include "ba1StagesInterface.h"
#include "controller.h"
#include "structs.h"
// stl
#include <string>
#include <vector>
#include <map>
//______________________________________________________________________________
class ba1StagesController : public controller
{
    public:
        ba1StagesController(bool& show_messages,
                          bool& show_debug_messages,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const std::string& configFile,
                          const std::string& name
                          );
        ba1StagesController& ba1StagesController::operator= ( const ba1StagesController& other ) = delete;
        ~ba1StagesController();

        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string& name)const;


        double getMinPos(const std::string& stage)const;
        double getMaxPos(const std::string& stage)const;

        double getStagePosition(const std::string& stage)const;
        bool setStagePosition(const std::string& stage, double val);
        bool screenOut(const std::string& stage);
        bool screenIn(const std::string& stage);
        bool isReadPosEqualSetPos(const std::string& stage, double tolerance);

        std::vector<std::string> getStageNames()const;

#ifdef BUILD_DLL
        boost::python::list getStageNames_Py()const;
#endif

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
        ba1StagesInterface localInterface;
};
//______________________________________________________________________________
#endif // _SHUTTER_CONTROLLER_H_
