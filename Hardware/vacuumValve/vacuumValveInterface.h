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

#ifndef VELA_VAC_VALVE_INTERFACE_H
#define VELA_VAC_VALVE_INTERFACE_H
// djs
#include "interface.h"
#include "vacuumValveStructs.h"
#include "vacuumValveConfigReader.h"
//stl
#include <vector>
#include <string>

class vacuumValveInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        vacuumValveInterface();//const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        vacuumValveInterface( const std::string & configFileLocation, const bool * show_messages_ptr,
                                      const bool * show_debug_messages_ptr,   const bool shouldStartEPICS,
                                      const bool startVirtualMachine, const VELA_ENUM::MACHINE_AREA myMachineArea );

        ~vacuumValveInterface();

        void findVacValveNames( std::vector< std::string >  & vacValveNames );
        std::vector< std::string > getVacValveNames();

        void openVacValve( const  std::string & vacValve );
        void closeVacValve( const std::string & vacValve );
        bool isOpen( const std::string & vacValve );
        bool isClosed( const std::string & vacValve );

        bool closeAndWait( const std::string & name, const time_t waitTime );
        bool openAndWait( const std::string & name, const time_t waitTime );

        void updateValveState( vacuumValveStructs::monitorStruct * ms, const unsigned short args );

        vacuumValveStructs::vacValveObject getVacValveObject( const std::string & vacValveName );
        bool entryExists2(const std::string & name, bool weKnowEntryExists );

        VELA_ENUM::VALVE_STATE getVacValveState( const std::string & vacValveName );

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & vacValveName );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & vacValveName );

        std::map< std::string, vacuumValveStructs::vacValveObject > allVacValveData; /// All the vacValve data is stored in this map, keyed by the vacValve name

//
    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.

        void initialise();
        const bool shouldStartEPICS;
        const bool startVM;
        const VELA_ENUM::MACHINE_AREA machineArea;

        vacuumValveConfigReader configReader;

        bool initVacValveObjects();
        void initVacValveChids();
        //void addChannel( std::map< std::string, vacuumValveStructs::vacValveObject >::iterator & it1, std::map< vacuumValveStructs::VAC_VALVE_PV_TYPE, std::string >::const_iterator & it2 );
        void addChannel( const std::string & pvRoot, vacuumValveStructs::pvStruct & pv );
        void addValveObjectMonitors( vacuumValveStructs::pvStruct & pvs,  vacuumValveStructs::vacValveObject & obj );

        void monitorVacValves();

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the vacValve is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members

        typedef  bool (vacuumValveInterface::*isOCMemFn)(const std::string & );
        typedef  void (vacuumValveInterface::*OCMemFn)  (const std::string & );
        bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, vacuumValveInterface & obj );

        /// static function that can be called back from epics to update values

        static void staticEntryrMonitor( const event_handler_args args );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor

        std::vector< vacuumValveStructs::monitorStruct * > continuousMonitorStructs;

};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
