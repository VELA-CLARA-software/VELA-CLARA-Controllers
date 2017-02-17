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

#ifndef VELA_MAG_INTERFACE_H
#define VELA_MAG_INTERFACE_H
// djs
#include "interface.h"
#include "llrfStructs.h"
#include "structs.h"
#include "llrfConfigReader.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>

class llrfInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<VELA_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        llrfInterface::llrfInterface();
        llrfInterface( const std::string &laserConf,
                         const bool startVirtualMachine,
                         const bool* show_messages_ptr,
                         const bool* show_debug_messages_ptr,
                         const bool shouldStartEPICs);

        ~llrfInterface();
      //  llrfInterface(const llrfInterface& origin, const bool* show_messages_ptr, const bool * show_debug_messages_ptr); // add this line

        /// These are pure virtual methods, so need to have some implmentation in derived classes
        IlockMap1 getILockStates( const std::string & name   ){ IlockMap1 r;return r; }
        IlockMap2 getILockStatesStr( const std::string & name){ IlockMap2 r;return r; }


    private:
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs;

        void killMonitor( llrfStructs::monitorStruct * ms );

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel( const std::string & pvRoot, llrfStructs::pvStruct & pv );
        void startMonitors();

        //std::map< std::string, magnetStructs::magnetObject > allMagnetData;

        llrfStructs::laserObject laser;

        std::vector< llrfStructs::monitorStruct * > continuousMonitorStructs;

        llrfConfigReader configReader; /// class member so we can pass in file path in ctor
        ///message
};
#endif // VELA_MAG_INTERFACE_H
