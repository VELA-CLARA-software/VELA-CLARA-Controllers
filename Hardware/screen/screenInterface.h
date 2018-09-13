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
//  Author:      ADB/DJS
//  Last edit:   07-08-2018
//  FileName:    screenInterface.cpp
//  Description:
//
//
//*/
#ifndef _SCREEN_INTERFACE_H_
#define _SCREEN_INTERFACE_H_
//djs
#include "interface.h"
#include "screenStructs.h"
#include "screenConfigReader.h"
#include "structs.h"
//stl
#include <vector>
#include <string>
#include <atomic>



class screenInterface: public interface
{
    public:

            ///Not a singleton, two construction methods...

        screenInterface::screenInterface();
        screenInterface(const std::string & conf1,
                        const bool& show_messages_ptr,
                        const bool& show_debug_messages_ptr,
                        const bool shouldStartEPICs,
                        const bool startVirtualMachine,
                        const HWC_ENUM::MACHINE_AREA myMachineArea );
        ~screenInterface();
        /// GETTERS
        bool isHOut(screenStructs::screenObject& scr);
        bool isVOut(screenStructs::screenObject& scr);
        bool isHIn(screenStructs::screenObject& scr);
        bool isVIn(screenStructs::screenObject& scr);
        bool is_HandV_OUT(screenStructs::screenObject& scr);
        bool isScreenIn(screenStructs::screenObject& scr);
        bool isHMoving(screenStructs::screenObject& scr);
        bool isVMoving(screenStructs::screenObject& scr);
        bool isScreenMoving(screenStructs::screenObject& scr);
        bool isClearForBeam(screenStructs::screenObject& scr);
        bool isHOut(const std::string & name);
        bool isVOut(const std::string & name);
        bool isHIn(const std::string & name);
        bool isVIn(const std::string & name);
        bool isMover(const std::string & name);
        bool isVMover(const std::string & name);
        bool isHVMover(const std::string & name);
        bool isPneumatic(const std::string & name);
        bool isClearForBeam(const std::string & name);
        bool isHMoving(const std::string & name);
        bool isVMoving(const std::string & name);
        bool isPMoving(const std::string & name);
        bool isScreenMoving(const std::string & name);
        const screenStructs::SCREEN_STATE getScreenState(const std::string & name);
        const screenStructs::SCREEN_TYPE getScreenType(const std::string & name);
        const std::string getScreenTypeStr(const std::string & name);
        std::vector< std::string > getScreenNames();
        const screenStructs::screenObject & getScreenObject(const std::string & name);
        std::vector< screenStructs::SCREEN_STATE > getAvailableDevices(const std::string & name);
        bool isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta);
        bool isYAGIn(const std::string & name);
        bool isScreenIn(const std::string & name );
        bool isHElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isVElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isPElement(const std::string & name, const screenStructs::SCREEN_STATE e);
        bool isHEnabled(const std::string & name);
        bool isVEnabled(const std::string & name);
        double getACTPOS(const std::string & name);
        double getJDiff(const std::string & name);
        double getDevicePosition(const std::string & name, const screenStructs::SCREEN_STATE state);
        double getPosition(const std::string & name);
        bool isVELAPneumatic(const std::string & name);
        bool isVELAHVMover(const std::string & name);
        bool isCLARAHVMover(const std::string & name);
        bool isCLARAVMover(const std::string & name);
        std::vector<bool> isScreenIn( const std::vector<std::string> & name );
        std::vector<bool> isScreenOUT( const std::vector<std::string> & name );
        std::vector<bool> exists_and_isLocked(const std::string& name);
        std::vector<bool> exists_and_isNotLocked(const std::string& name);
        double get_H_ACTPOS(const std::string & name);
        double get_V_ACTPOS(const std::string & name);
        const std::vector<std::string> getVELAPneumaticScreens(  const std::vector< std::string > & names );
        const std::vector<std::string> getVELAHVMoverScreens(   const std::vector< std::string > & names );
        const std::vector<std::string> getCLARAHVMoverScreens(  const std::vector< std::string > & names );
        const std::vector<std::string> getCLARAVMoverScreens(   const std::vector< std::string > & names );
        const std::vector<std::string> getMoverScreens(   const std::vector< std::string > & names );
        const std::vector<std::string> getHVMoverScreens(   const std::vector< std::string > & names );
        const std::vector< screenStructs::screenObject > getScreensWithCameras();
        const std::vector< std::string > getNamesOfScreensWithCameras();
        /// SETTERS
        void moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state );
        void insertYAG( const std::string & name );
        void moveScreenOut( const std::string & name );
        void resetPosition( const std::string & name );
        void jogScreen( const std::string & name, const double jog );
        void setPosition( const std::string & name, const double setPos );
        bool setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state );
        bool setScreenTrigger(const std::string & name );
        bool setScreenTrigger(const std::string & name, const screenStructs::SCREEN_STATE & state );
        bool setEX(const std::string & name );
        bool setEN( const std::string & name, const screenStructs::DRIVER_DIRECTION direction );

        /// OLD FUNCTIONS

        // i want to make sure so python does not have access to the second arguemnt
        void checkScreenCHIDStates();

        /// These are pure virtual method in the base class and MUST be overwritten in the derived interface...
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & name )const;
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string >  getILockStatesStr( const std::string & name )const;

    protected:
    private:

        /// called from the constructor to set-up chids, monitors, etc.
        void initialise();
        screenConfigReader configReader; ///class member so we can pass in file path in ctor

        bool initScreenObjects();
        void initScreenChids();
        void addChannel(const std::string& pvRoot, screenStructs::pvStruct& pv);//if this doesn't work then try commented out function in shutter program
        void initIsLockedMap();

        void monitorScreens();// calls the below functions to add to continuousMonitorStructsDEV
        void addScreenObjectMonitors(screenStructs::pvStruct& pvs, screenStructs::screenObject& obj);
//        void addScreenDriverStatusMonitors(screenStructs::pvStruct& pvs,screenStructs::screenDriverStatus& obj);// see screenstructs for a screenDriver

        // this map conatins all the screen objects, which combine online and offline data
        std::map<std::string,screenStructs::screenObject>allScreentData;
        void updateSta(screenStructs::monitorStruct* ms, const  unsigned short args );
        void updateScreenState(screenStructs::screenObject& scr);
        void update_STA_Bit_map(screenStructs::monitorStruct* ms, const int argsdbr );

        void updateRPOS(screenStructs::monitorStruct* ms, const double args);

        void updateACTPOS(screenStructs::monitorStruct* ms,const double args);
        void updateEN(screenStructs::monitorStruct* ms, const unsigned short args);
        void updateGetDev( screenStructs::monitorStruct * ms, const unsigned short args );
        void updateDevState( screenStructs::monitorStruct * ms, const unsigned short args );
        void updateDevCent( screenStructs::monitorStruct * ms, const double args );
        void updateJDiff( screenStructs::monitorStruct * ms, const double args );
        void updateMoving( screenStructs::monitorStruct * ms, const unsigned short args );

        // version to use when we already know the answer
        bool entryExists2(const std::string& name, bool weKnowEntryExists = false );

        // helper function for main screenMoveTo() function
        bool get_NOT_Locked_and_NOT_MovingScreens( const std::vector<std::string>& scrIN,
                                                  std::vector<std::string>& scrOUT,
                                                  const std::vector<screenStructs::SCREEN_STATE>& staIN,
                                                  std::vector<screenStructs::SCREEN_STATE>& staOUT );
        bool sortPnuematicMoverScreen( const std::vector<std::string>& scrIN,
                                       std::vector<std::string>& VELA_PNEUMATIC_Scr,
                                       std::vector<std::string>& VELA_HV_MOVER_Scr,
                                       std::vector<std::string>& CLARA_PNEUMATIC_Scr,
                                       std::vector<std::string>& CLARA_HV_MOVER_Scr,
                                       std::vector<std::string>& CLARA_V_MOVER_Scr,
                                       const std::vector<screenStructs::SCREEN_STATE>& staIN,
                                       std::vector<screenStructs::SCREEN_STATE>& VELA_PNEUMATIC_Sta,
                                       std::vector<screenStructs::SCREEN_STATE>& VELA_HV_MOVER_Sta,
                                       std::vector<screenStructs::SCREEN_STATE>& CLARA_PNEUMATIC_Sta,
                                       std::vector<screenStructs::SCREEN_STATE>& CLARA_HV_MOVER_Sta,
                                       std::vector<screenStructs::SCREEN_STATE>& CLARA_V_MOVER_Sta );

        // 'existential quantification' - ahem
        bool isHorizontal(screenStructs::DRIVER_DIRECTION dir );
        bool isVertical  (screenStructs::DRIVER_DIRECTION dir );



        // Functions to move VELA_PNEUMATIC Screens to position states
        bool move_VELA_PNEUMATIC_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool send_VELA_PNEUMATIC_Command(const std::vector<chtype*>& CHTYPE,const std::vector<chid*>& CHID,const std::string& m1,const std::string& m2);
        bool move_CLARA_PNEUMATIC_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool send_CLARA_PNEUMATIC_Command(const std::vector<chtype*>& CHTYPE,const std::vector<chid*>& CHID,const std::string& m1,const std::string& m2);

        // Functions to move HV_MOVER Screens to position states (plus some overloaded versions to make things more readable)
        bool move_VELA_HV_MOVER_Screens  (const std::vector<std::string>& names,const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool move_CLARA_HV_MOVER_Screens  (const std::vector<std::string>& names,const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool move_CLARA_V_MOVER_Screens  (const std::vector<std::string>& names,const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool set_VELA_HV_MOVER_Position(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const screenStructs::SCREEN_STATE& sta );

        std::map<std::string,std::atomic<bool> > isLockedMap; /// std::atomic< bool > are not CopyConstructible, so this is held locally
        size_t dualMoveNum;

        void killFinishedMoveThreads();
        static void staticEntryScreenMonitor( const event_handler_args args );
        bool is_OUT_AND_VDriveEnabled(const std::string & name);
        bool is_IN_OR_OUT(const screenStructs::SCREEN_STATE sta );
        bool screen_is_out_AND_sta_is_in(const std::string & name, const screenStructs::SCREEN_STATE sta );
        bool screen_is_in_AND_sta_is_out(const std::string & name, const screenStructs::SCREEN_STATE sta );

        typedef  bool(screenInterface::*isOCMemFn)(const std::string & );
        typedef  void(screenInterface::*OCMemFn)  (const std::string & );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< screenStructs::monitorStruct * > continuousMonitorStructsDEV;
        std::vector< screenStructs::monitorStruct * > continuousMonitorStructs;
        screenStructs::screenObject screenObject;

        const HWC_ENUM::MACHINE_AREA myMachineArea;


};
#endif //_SCREEN_INTERFACE__H_
