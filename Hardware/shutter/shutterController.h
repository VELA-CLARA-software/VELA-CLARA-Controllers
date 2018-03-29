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
//  FileName:    VCpilaser.cpp
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



class shutterController : public controller
{
    public:
        //shutterController();
        shutterController(bool* show_messages,
                          bool* show_debug_messagese,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const std::string& configFile,
                          const std::string& name
                          );
//        shutterController(const std::string configFileLocation, bool* show_messages , bool* show_debug_messages);
        ~shutterController( );

//        void open( const std::string & name );
//        void close( const std::string & name );
//        bool isOpen( const std::string & name );
//        bool isClosed( const std::string & name );
//        bool closeAndWait( const std::string & name, const time_t waitTime = 2);
//        bool openAndWait ( const std::string & name, const time_t waitTime = 2);
//
//        std::vector< std::string > getShutterNames();
//
//        /// write a method that returns string version of enums using ENUM_TO_STRING
//
//        HWC_ENUM::SHUTTER_STATE getShutterState( const std::string & name );
//        std::string getShutterStateStr( const std::string & name );
//
//        void openShutter1();
//        void closeShutter1();
//        void openShutter2();
//        void closeShutter2();
//
//        bool openAndWaitShutter1 ( const time_t waitTime = 2 );
//        bool openAndWaitShutter2 ( const time_t waitTime = 2 );
//        bool closeAndWaitShutter1( const time_t waitTime = 2 );
//        bool closeAndWaitShutter2( const time_t waitTime = 2 );
//
//        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
//        /// write a method that returns string version of enums using ENUM_TO_STRING
//
//        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
//        std::map< HWC_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & name );
//
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);
//
//    private:
        void initialise();
//        /// No singletons, no pointers, let's just have an object
        shutterInterface localInterface;
//        std::vector< std::string > pilShutterNames;
};

#endif // _SHUTTER_CONTROLLER_H_
