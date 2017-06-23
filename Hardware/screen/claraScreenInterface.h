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

#ifndef claraScreenInterface_H
#define claraScreenInterface_H
//djs

#include "interface.h"
#include "screenStructs.h"
#include "screenConfigReader.h"
//stl
#include <vector>
#include <string>
#include <atomic>



class claraScreenInterface: public interface
{
    public:

            ///Not a singleton, two construction methods...


        claraScreenInterface(const std::string & conf1,
                        const bool startVirtualMachine,
                        const bool* show_messages_ptr,
                        const bool* show_debug_messages_ptr,
                        const bool shouldStartEPICs,
                        const VELA_ENUM::MACHINE_AREA myMachineArea );
        ~claraScreenInterface();

        // this is the main move funciton, all higher level versions (i.e. screenIN) end up here, this does all the hard work / logic
        bool screenMoveTo( const std::vector< std::string > & names, const std::vector< screenStructs::SCREEN_STATE > & states);
        bool screenMoveTo( const std::string & name, const screenStructs::SCREEN_STATE & states);
        bool screenIN ( const std::string & name );
        bool screenOUT( const std::string & name );
        bool screenOUT( const std::vector< std::string > & names );
        bool screenIN ( const std::vector< std::string > & names );
        bool setPosition(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const double value );


        // 'existential quantification' - ahem
        bool isMoving(const std::string& name,const bool weKnowEntryExists = false );
        bool isNotMoving(const std::string& name,const bool weKnowEntryExists = false );
        bool isHCassetteOUT(screenStructs::velaINJscreenObject& scr);
        bool isVCassetteOUT(screenStructs::velaINJscreenObject& scr);
        bool is_HandV_CassetteOUT(screenStructs::velaINJscreenObject& scr);
        bool is_VELA_PNEUMATIC(const std::string & name);
        bool is_VELA_HV_MOVER (const std::string & name);
        bool is_H_Element(screenStructs::SCREEN_STATE e);
        bool is_V_Element(screenStructs::SCREEN_STATE e);
        bool isScreenOUT(const std::string & name, const bool weKnowEntryExists = false );
        bool isScreenIN(const std::string & name, const bool weKnowEntryExists = false );
        std::vector<bool> isScreenIN( const std::vector<std::string> & name );
        std::vector<bool> isScreenOUT( const std::vector<std::string> & name );
        std::vector<bool> exists_and_isLocked(const std::string& name);
        std::vector<bool> exists_and_isNotLocked(const std::string& name);

        const std::vector<std::string> get_VELA_PNEUMATIC_Screens(  const std::vector< std::string > & names );
        const std::vector<std::string> get_VELA_HV_MOVER_Screens(  const std::vector< std::string > & names );

        // i want to make sure so python does not have access to the second arguemnt
        screenStructs::SCREEN_STATE getScreenState( const std::string & name, const bool weKnowEntryExists = false );

        bool isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta);

        void checkScreenCHIDStates();

        /// These are pure virtual method in the base class and MUST be overwritten in the derived interface...
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string >  getILockStatesStr( const std::string & name );

    protected:
    private:

        /// called from the constructor to set-up chids, monitors, etc.
        void initialise();
        screenConfigReader configReader; ///class member so we can pass in file path in ctor

        bool initvelaINJscreenObjects();
        void initScreenChids();
        void addChannel(const std::string& pvRoot, screenStructs::pvStruct& pv);//if this doesn't work then try commented out function in shutter program
        void initIsLockedMap();

        void monitorScreens();// calls the below functions to add to continuousMonitorStructsDEV
        void addvelaINJscreenObjectMonitors(screenStructs::pvStruct& pvs, screenStructs::velaINJscreenObject& obj);
        void addScreenDriverMonitors(screenStructs::pvStruct& pvs, screenStructs::screenDriver& obj);// see screenstructs for a screenDriver
        void addScreenDriverStatusMonitors(screenStructs::pvStruct& pvs,screenStructs::screenDriverStatus& obj);// see screenstructs for a screenDriver

        // this map conatins all the screen objects, which combine online and offline data
        std::map<std::string,screenStructs::velaINJscreenObject>allScreentData;

        // updates the VELA_PNEUMATIC screen state
        void updateSta(screenStructs::monitorStruct* ms, const  unsigned short args );
        // updates a VELA_HV_MOVER screenState, called after a driver enters DRIVER_STATIONARY state
        void updateScreenState(screenStructs::velaINJscreenObject& scr);
        //void update_STA_Bit_map( screenStructs::monitorStruct   * ms , const int argsdbr );
        void update_STA_Bit_map(screenStructs::monitorStruct* ms, const int argsdbr );

        void updateRPOS(screenStructs::monitorStruct* ms, const double args);
        // update RPOS updates the screen state, if there is a match
        void updateCassettePosition(screenStructs::screenCassette& cas, const double pos  );

        void updatePROT01(screenStructs::monitorStruct* ms,const double args);
        void updatePROT03(screenStructs::monitorStruct* ms,const double args);
        void updatePROT05(screenStructs::monitorStruct* ms,const double args);
        //void updateMABS(screenStructs::monitorStruct* ms,const double args);

        // version to use when we already know the answer
        bool entryExists2(const std::string& name, bool weKnowEntryExists = false );


        // helper function for main screenMoveTo() function
        bool get_NOT_Locked_and_NOT_MovingScreens(const std::vector<std::string>& scrIN,std::vector<std::string>& scrOUT,
                                                  const std::vector<screenStructs::SCREEN_STATE>& staIN,std::vector<screenStructs::SCREEN_STATE>& staOUT);
        bool sortPnuematicMoverScreen(const std::vector<std::string>& scrIN,std::vector<std::string>& VELA_PNEUMATIC_Scr,std::vector<std::string>& VELA_HV_MOVER__Scr,
                                                      const std::vector<screenStructs::SCREEN_STATE>& staIN,std::vector<screenStructs::SCREEN_STATE>& VELA_PNEUMATIC_Sta,
                                                      std::vector<screenStructs::SCREEN_STATE>& VELA_HV_MOVER__Sta);

        // 'existential quantification' - ahem
        bool isHorizontal(screenStructs::DRIVER_DIRECTION dir );
        bool isVertical  (screenStructs::DRIVER_DIRECTION dir );

        // Functions to move VELA_PNEUMATIC Screens to position states
        bool move_VELA_PNEUMATIC_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool send_VELA_PNEUMATIC_Command(const std::vector<chtype*>& CHTYPE,const std::vector<chid*>& CHID,const std::string& m1,const std::string& m2);

        // Functions to move HV_MOVER Screens to position states (plus some overloaded versions to make things more readable)
        bool move_VELA_HV_MOVER_Screens  (const std::vector<std::string>& names,const std::vector< screenStructs::SCREEN_STATE > & states  );
        bool set_VELA_HV_MOVER_Position(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const screenStructs::SCREEN_STATE& sta );
        bool set_VELA_HV_MOVER_Position(const screenStructs::screenDriverStatus& driverS, const double value);
        bool set_VELA_HV_MOVER_Position(const std::string& name,  const screenStructs::SCREEN_STATE& sta );

        // complex screen movements are handled in different threads,
        // HV_dualMoveStruct contains the info to run that thread
        std::map<size_t,screenStructs::HV_dualMoveStruct> screenDualMoveStructsMap;

        // screens are locked if this prgoramme is moving them, but...
        // they also have a trajectory bool, that locks them when moved external to this controller (i.e. by controls synoptic)

        std::map<std::string,std::atomic<bool> > isLockedMap; /// std::atomic< bool > are not CopyConstructible, so this is held locally

        static void staticEntryDualMove(screenStructs::HV_dualMoveStruct & ms );
        size_t dualMoveNum;

        void killFinishedMoveThreads();

        //static function that can be called back from epics to update values
        static void staticEntryScreenMonitor( const event_handler_args args );

        bool yagOnV(const screenStructs::screenDriver& scrdr );
        bool isVDriveEnabled (const screenStructs::screenDriver& scrdr);
        bool isHDriveEnabled (const screenStructs::screenDriver& scrdr);
        bool isHDriveDisabled(const screenStructs::screenDriver& scrdr);
        bool isVDriveDisabled(const screenStructs::screenDriver& scrdr);
        bool is_H_element_AND_HDriveEnabled(const screenStructs::SCREEN_STATE e,const screenStructs::screenDriver & scrdr);
        bool is_V_element_AND_VDriveEnabled(const screenStructs::SCREEN_STATE e,const screenStructs::screenDriver & scrdr);
        bool is_OUT_AND_VDriveEnabled(const std::string & name);
        bool is_IN_OR_OUT(const screenStructs::SCREEN_STATE sta );
        bool screen_is_out_AND_sta_is_in(const std::string & name, const screenStructs::SCREEN_STATE sta );
        bool screen_is_in_AND_sta_is_out(const std::string & name, const screenStructs::SCREEN_STATE sta );

        // return the psoition of teh element in the map
        double getElementPosition(const screenStructs::screenCassette & scrcas, screenStructs::SCREEN_STATE e );


        //std::map<std::string,  std::atomic< bool > > dualMoveNum; /// std::atomic< bool > are not CopyConstructible, so this is held locally

        //bool waitForElementToMove( const screenStructs::velaINJscreenObject & scr, screenStructs::SCREEN_STATE e, time_t TIMEOUT);


        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the shutter is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members

        typedef  bool(claraScreenInterface::*isOCMemFn)(const std::string & );
        typedef  void(claraScreenInterface::*OCMemFn)  (const std::string & );

        //void toggleScreen( chtype & cht, chid & chi, const char * m1 = "", const char * m2 = "");



//        static void UpdateDouble( screenStructs::monitorStruct * ms, const void * argsdbr );
//        static void UpdateEnum( screenStructs::monitorStruct * ms, const void * argsdbr );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        //std::vector< screenStructs::monitorStruct * > continuousMonitorStructs;
        std::vector< screenStructs::monitorStruct * > continuousMonitorStructsDEV;


        //void addToComplexMonitorStructs( std::vector<screenStructs::monitorStruct*>& cms, screenStructs::pvStruct& pv, screenStructs::COMPLEX_YAG_Object * COMPLEX_YAG  );
        //void addToSimpleMonitorStructs ( std::vector<screenStructs::monitorStruct*>& cms, screenStructs::pvStruct& pv, screenStructs::SIMPLE_YAG_Object * SIMPLE_YAG  );


        screenStructs::velaINJscreenObject velaINJscreenObject;

        const VELA_ENUM::MACHINE_AREA myMachineArea;
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs;


};
#endif //VELA_PyIL_SCREEN_INTERFACE_H