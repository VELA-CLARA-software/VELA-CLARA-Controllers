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

#ifndef VELA_RF_GUN_INTERFACE_H
#define VELA_RF_GUN_INTERFACE_H
// djs
#include "interface.h"
#include "RFGunStructs.h"
#include "RFGunConfigReader.h"
//
#include <string>
#include <vector>
#include <map>


class RFGunInterface : public interface
{
    public:

        typedef  bool ( RFGunInterface::*ABoolMemFn)( void );


        /// Not a singleton, two construction methods....

        RFGunInterface( const bool* show_messages_ptr, const bool* show_debug_messages_ptr  );
        RFGunInterface( const std::string configFileLocation1,
                        const std::string configFileLocation2,
                        const std::string configFileLocation3, const bool* show_messages_ptr, const bool* show_debug_messages_ptr  );
        ~RFGunInterface();


        std::vector< std::vector< double >> getRevT( const std::string & name );
        std::vector< std::vector< double >> getFwdT(const std::string & name  );
        std::vector< double > getRevTStamp(const std::string & name  );
        std::vector< double > getFwdTStamp(const std::string & name  );
        std::vector< std::string > getRevTStampStr( const std::string & name );
        std::vector< std::string > getFwdTStampStr( const std::string & name  );

        bool  setAmp( const long  val );
        bool  setPhi( const double val);

        std::vector< std::string > getRFPowerNames();

        double getFwd( const std::string & name );
        double getRev( const  std::string & name );
        double getRatio( const std::string & name  );
        double getPhi();
        double getAmpR();
        long   getAmpW();

        double getFwdCursor( const std::string & name );
        bool   setFwdCursor( const std::string & name, const double val );
        double getRevCursor( const std::string & name );
        bool   setRevCursor( const std::string & name, const double val );
        bool   setAllCursors(const double val );

        void modReset();
        bool modResetAndWait( const size_t waitTime );

    // Will have to chekc where these go... MAYBE NOT AVAIABLE TO PUBLIC?
        void setModToOff();
        void setModToTrig();
        void setModToStandby();
        void setModToHVOn();

        bool switchOnRF();

        bool switchOffRF();

        bool goToStandby();

        void startMonitors();

        void monitorTracesForNShots( const size_t N );

        bool killTraceMonitors();

        bool isMonitoringTraces();
        bool isNotMonitoringTraces();

        bool isModWarmedUp();
        bool isModNotWarmedUp();

        bool isModInHVOn();
        bool isModInTrig();
        bool isModInOff();
        bool isModInStandby();
//        bool isModIniLock();
//        bool isModNotIniLock();


        bool waitForModState( RFGunStructs::MOD_STATE state, const time_t waitTime );

        RFGunStructs::MOD_STATE getModState();
        RFGunStructs::MOD_EXILOCK1 getModiLock();

        bool isModILockStateGood();

        bool isModILockStateNotGood();


        /// These are pure virtual methods, so need to have some implmentation in derived classes

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name ) {
            std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
            return r;}
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name ) {
            std::map< VELA_ENUM::ILOCK_NUMBER, std::string > r;
            return r;}

    private:

        void killCallBack( RFGunStructs::monitorStruct * ms );

        void deleteTraceMonitorStructs();

        void initialise(); /// private, this all happens on contruction

        void initObjects();

        void initChids(); /// I prefer these more generic names to functions
        void addChannel( const std::string & pvRoot, RFGunStructs::pvStruct & pv );

        void addToMonitorStructs( std::vector< RFGunStructs::monitorStruct * > & msv, RFGunStructs::pvStruct & pv,  RFGunStructs::rfPowerObject * pwrObj = nullptr ); // we have two of these objects, so need to pass in which one if necessary

        /// static function that can be called back from epics to update values
        static void staticEntryMonitor( const event_handler_args args );
        void setStateRead( const void * argsdbr );
        void updateValue( RFGunStructs::monitorStruct * ms, const event_handler_args & args );
        void updateModIlock( RFGunStructs::monitorStruct * ms, const void * argsdbr );
        void updateTrace( RFGunStructs::monitorStruct * ms, const event_handler_args args);

        bool isATracePV( RFGunStructs::RF_PV_TYPE pv );
        bool isNotATracePV( RFGunStructs::RF_PV_TYPE pv );

        const RFGunStructs::MOD_SET_STATE SET_MOD_OFF, SET_MOD_STANDBY, SET_MOD_HV_ON, SET_MOD_TRIG ;

        void resetTraceVectors( const int N );

        RFGunConfigReader configReader; /// class member so we can pass in file path in ctor

        RFGunStructs::rfObject RFObject;

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< RFGunStructs::monitorStruct * > continuousMonitorStructs;
        std::vector< RFGunStructs::monitorStruct * > traceMonitorStructs;


        void setModState( RFGunStructs::MOD_SET_STATE state, const char * mess );
        bool setModStateAndWait( RFGunStructs::MOD_SET_STATE newState, time_t waitTime );


        bool waitFor( ABoolMemFn f1, RFGunInterface & obj, const  std::string & message, const  time_t waitTime,const  size_t pause = 100 );
        bool waitFor( ABoolMemFn f1, RFGunInterface & obj, const char * message, const  time_t waitTime,const  size_t pause = 100 );

        bool setOffToStandby( const time_t waitTime );
        bool setStandbyToHVOn( const time_t waitTime );
        bool setHVOnToTrig( const time_t waitTime );
        bool setStandbyToOff( const time_t waitTime );
        bool setHVOnToStandby(const  time_t waitTime );
        bool setTrigToHVOn( const time_t waitTime );

        bool isAGoodModErrorReadStr(const std::string & s);
        bool isABadModErrorReadStr(const std::string & s);

        RFGunStructs::MOD_EXILOCK1 convertModErrorReadStr( const char * s );
        RFGunStructs::MOD_EXILOCK1 convertModErrorRead( const double v );


        std::map< std::string,  bool > isMonitoringMap; /// std::atomic< bool > are not CopyConstructible, so this is held locally

};
#endif // VELA_RF_GUN_INTERFACE_H
