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

#ifndef vela_VAC_VALVE_CONTROLLER_H
#define vela_VAC_VALVE_CONTROLLER_H

// project
#include "vacuumValveStructs.h"
#include "vacuumValveInterface.h"
#include "controller.h"
#include "structs.h"
// stl
#include <string>
#include <vector>


class vacuumValveController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        vacuumValveController();// const bool show_messages = true, const bool show_debug_messages = true );
        vacuumValveController( const std::string & configFileLocation, const bool show_messages,
                                       const bool show_debug_messages, const bool shouldStartEPICS,
                                       const bool startVirtualMachine, const VELA_ENUM::MACHINE_AREA myMachineArea );
        ~vacuumValveController();

        void openVacValve( const std::string & vacValveName );
        void closeVacValve( const std::string & vacValveName );
        bool isOpen( const std::string & vacValveName );
        bool isClosed( const std::string & vacValveName );
        bool openAndWait( const std::string & vacValveName, const time_t waitTime = 2);
        bool closeAndWait ( const std::string & vacValveName, const time_t waitTime = 2);
        std::vector< std::string > getVacValveNames();

        /// write a method that returns string version of enums using ENUM_TO_STRING

        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );

        VELA_ENUM::VALVE_STATE getVacValveState( const std::string & vacValveName );
        std::string getVacValveStateStr( const std::string & name );

        const vacuumValveStructs::vacValveObject& getVacValveObjConstRef( const std::string & vacValveName);

        void openValve1();
        void closeValve1();
        void openValve2();
        void closeValve2();
        void openValve3();
        void closeValve3();
        void openValve4();
        void closeValve4();
        void openValve5();
        void closeValve5();
        void openValve6();
        void closeValve6();
        void openValve7();
        void closeValve7();

        bool openAndWaitValve1( const time_t waitTime );
        bool closeAndWaitValve1( const time_t waitTime );
        bool openAndWaitValve2( const time_t waitTime );
        bool closeAndWaitValve2( const time_t waitTime );
        bool openAndWaitValve3( const time_t waitTime );
        bool closeAndWaitValve3( const time_t waitTime );
        bool openAndWaitValve4( const time_t waitTime );
        bool closeAndWaitValve4( const time_t waitTime );
        bool openAndWaitValve5( const time_t waitTime );
        bool closeAndWaitValve5( const time_t waitTime );
        bool openAndWaitValve6( const time_t waitTime );
        bool closeAndWaitValve6( const time_t waitTime );
        bool openAndWaitValve7( const time_t waitTime );
        bool closeAndWaitValve7( const time_t waitTime );

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & objName );

    protected:
    private:

        void initialise();

        /// No singletons, no pointers, let's just have an object

        vacuumValveInterface localInterface;

        bool shouldStartEPICS;
        const VELA_ENUM::MACHINE_AREA machineArea;

        std::vector< std::string > vacValveNames;
};


#endif // vacuumValveController_H
