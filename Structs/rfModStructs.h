#ifndef _RF_MOD_STRUCTS_H_
#define _RF_MOD_STRUCTS_H_
//
#include "structs.h"
//stl
#include <vector>
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class gunModInterface;

namespace rfModStructs
{
//    struct pvStruct;
//    struct rfPowerObject;
//    struct rfPowerMonitorStruct;
//    struct monitorStuct;
//    struct rfLLRFObject;
//    struct rfModObject;
//    struct rfObject;
//
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( GUN_MOD_PV_TYPE, (MOD_STATE_SET) (MOD_STATE_READ) (MOD_ERROR_READ) (MOD_ERROR_READ_STR)
                                                     (MOD_WARMUP_TIME) (MOD_RESET) )
//
//    /// For the ilocks, it's either ok or bad...
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MOD_EXILOCK1,  ( BAD ) ( GOOD ) ( ERROR ) ( UNKNOWN ) );
//    /// These are used to actually send commands to epics ( i think )
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MOD_SET_STATE, ( SET_MOD_OFF ) ( SET_MOD_STANDBY ) ( SET_MOD_HV_ON ) ( SET_MOD_TRIG ) );
//
//    /// These are possible modulator states
//    /// These come from CS3A_scandinova.db /home/controls/ioc/ebtf/CS3A/db
//    /// We keep the same numbers as the control system, therefore UNKNOWN1                             //    (ERROR1)          = 0,
//                                                                                                       //    (UNKNOWN1          = 1 /// This is one i made up
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( GUN_MOD_STATE, (ERROR1) (UNKNOWN1) (OFF) (off_Request)            //    (OFF)             = 2,
                                        (HV_Intrlock) (Standby_Request) (Standby) (HV_Off_Requ)        //    (off_Request)     = 3,
                                        (Trigger_Interl) (HV_Request) (HV_On) (Trig_Off_Req)           //    (HV_Intrlock)     = 4,
                                        (Trig_Request) (Trig)  );                                       //    (Standby_Request) = 5,
                                                                                                       //    (Standby)         = 6,
                                                                                                       //    (HV_Off_Requ)     = 7,
    /// These can't go in VELA_ENUM as they need a pvType.                                             //    (Trigger_Interl)  = 8,
    struct pvStruct                                                                                    //    (HV_Request)      = 9,
    {                                                                                                  //    (HV_On)           = 10,
        pvStruct() : pvSuffix( "UNKNOWN" ), objName("UNKNOWN"),COUNT(0),MASK(0){}
        GUN_MOD_PV_TYPE pvType;                                                                         //    (Trig_Off_Req)    = 11,
        chid            CHID;                                                                           //    (Trig_Request)    = 12,
        std::string     pvSuffix, objName;                                                              //    (Trig)            = 13
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
    };


//
    struct gunModObject
    {
        gunModObject() : state( GUN_MOD_STATE::ERROR1 ),
        //ilck(UNKNOWN),
        safelyWarmedUP( false ) {}
        std::string name, pvRoot;
        GUN_MOD_STATE    state;
        long         warmuptime;
        //MOD_EXILOCK1 ilck, ilckstr;
        bool safelyWarmedUP;
        std::vector< std::string > goodModErrorReadStr, badModErrorReadStr;
    #ifndef __CINT__
        std::map< GUN_MOD_PV_TYPE, pvStruct > pvMonStructs, pvComStructs;
    #endif
    };

    struct monitorStruct
    {
        monitorStruct() : rfModObject(nullptr),interface(nullptr){}
        GUN_MOD_PV_TYPE  monType;
        gunModObject*    rfModObject;
        chtype           CHTYPE;
        //void *           val; /// most of the rfObject Values appear to be doubles, which is handy... recast in staticEntry function...
        gunModInterface *interface;
        evid             EVID;
    };



//    struct rfLLRFObject
//    {
//        rfLLRFObject() : name( "UNKNOWN" ), pvRoot("UNKNOWN"),ampR(0.0),ampW(0), phi(0.0){}
//        std::string name, pvRoot;
//        double ampR, phi, crestPhi;
//        long ampW;
//    #ifndef __CINT__
//        std::map< RF_PV_TYPE, pvStruct > pvMonStructs;
//    #endif
//    };


//    struct rfTraceData
//    {
//        rfTraceData() : shotCount( -2 ), numShots( -2 ) {}
//        int shotCount, numShots; /// we allow -1 values here so NOT a size_t
//        std::vector< double > timeStamps;
//        std::vector< std::string > strTimeStamps;
//        std::vector< std::vector< double > > traceData;
//    };
//
//
//    struct rfPowerObject
//    {
//        rfPowerObject() : name("UNKNOWN"),pvRoot("UNKNOWN"),fwd( 0.0 ), rev( 0.0 ), fwdM( 0.0 ), revM( 0.0) {}
//        std::string name, pvRoot;
//        double fwd, rev, fwdM, revM, ratio;
//        rfTraceData revT, fwdT;
//    #ifndef __CINT__
//        std::map< RF_PV_TYPE, pvStruct > pvMonStructs;
//    #endif
//    };
//
//    struct rfObject
//    {
//        rfObject() : name( "UNKNOWN" ), klyFWDLimit( 0.0) {}
//        std::string name;
//        rfModObject mod;
//        rfLLRFObject llrf;
//        double klyFWDLimit;
//        std::map< std::string, rfPowerObject > powerObjects; /// For gun (and TDC>) have two of these, the cavity and the klystron
//    };
//

}
#endif//_RF_MOD_STRUCTS_H_
