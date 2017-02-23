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
        llrfInterface(const std::string &llrfConf,
                      const bool  startVirtualMachine,
                      const bool* show_messages_ptr,
                      const bool* show_debug_messages_ptr,
                      const bool  shouldStartEPICs,
                      const llrfStructs::LLRF_TYPE type);

        ~llrfInterface();

        llrfStructs::LLRF_TYPE getType(){return myLLRFType;}



        long     getAmpRead();
        long     getAmpWrite();
        long     getPhiLLRF();

        double   getPhi();
        double   getAmp();

        bool     setPhiLLRF(long value);
        bool     setAmpLLLRF(long value);

        bool     setPhi(double value);// degrees relative to crest
        bool     setAmp(double value);// MV / m ampliutude

        double getPhiCalibration();
        double getAmpCalibration();
        double getCrestPhiLLRF(); // in LLRF units



        const llrfStructs::llrfObject& getLLRFObjConstRef();

        /// These are pure virtual methods, so need to have some implmentation in derived classes
        // ********this will need updating*******
        IlockMap1 getILockStates(const std::string & name   ){IlockMap1 r;return r;}
        IlockMap2 getILockStatesStr(const std::string & name){IlockMap2 r;return r;}


    private:
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs,usingVirtualMachine;

        void killMonitor( llrfStructs::monitorStruct * ms );

        template<typename T>
        bool setValue( llrfStructs::pvStruct& pvs, T value);
        template<typename T>
        bool setValue2( llrfStructs::pvStruct& pvs, T value);

        static void staticEntryLLRFMonitor(const event_handler_args args);

        bool setAMPMVM();
        bool setPHIDEG();
        bool setAmpRead();

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel( const std::string & pvRoot, llrfStructs::pvStruct & pv );
        void startMonitors();

        //std::map< std::string, magnetStructs::magnetObject > allMagnetData;

        llrfStructs::llrfObject llrf;
        llrfStructs::LLRF_TYPE myLLRFType;

        std::vector< llrfStructs::monitorStruct * > continuousMonitorStructs;

        llrfConfigReader configReader; /// class member so we can pass in file path in ctor
        ///message
};
#endif // VELA_MAG_INTERFACE_H
