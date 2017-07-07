#ifndef _RF_MOD_STRUCTS_H_
#define _RF_MOD_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
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
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( GUN_MOD_PV_TYPE, (STATE_SET) (STATE_READ) (ERROR_READ) (ERROR_READ_STR)(WARMUP_TIME)
                                                          (RESET)(MAGPS1_CURR_READ)(MAGPS2_CURR_READ)(MAGPS3_CURR_READ)(MAGPS4_CURR_READ)
                                                          (MAGPS1_VOLT_READ)(MAGPS2_VOLT_READ)(MAGPS3_VOLT_READ)(MAGPS4_VOLT_READ)(CT_READ)
                                                          (CVD_READ)(PULSE_WIDTH_READ)(PULSE_WIDTH_FWHM_READ)(IONP_PRESSURE_READ))

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
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( GUN_MOD_STATE, (ERROR1)(UNKNOWN1)(OFF)(off_Request)            //    (OFF)             = 2,
                                        (HV_Intrlock)(Standby_Request)(Standby)(HV_Off_Requ)        //    (off_Request)     = 3,
                                        (Trigger_Interl)(HV_Request)(HV_On)(Trig_Off_Req)           //    (HV_Intrlock)     = 4,
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
        safelyWarmedUP(false),
        MagPs1CurrRead(UTL::DUMMY_DOUBLE),MagPs2CurrRead(UTL::DUMMY_DOUBLE),MagPs3CurrRead(UTL::DUMMY_DOUBLE),MagPs4CurrRead(UTL::DUMMY_DOUBLE),
        MagPs1VoltRead(UTL::DUMMY_DOUBLE),MagPs2VoltRead(UTL::DUMMY_DOUBLE),MagPs3VoltRead(UTL::DUMMY_DOUBLE),MagPs4VoltRead(UTL::DUMMY_DOUBLE),
        PrfSet(UTL::DUMMY_DOUBLE),PrfRead(UTL::DUMMY_DOUBLE){}
        std::string name, pvRoot;
        GUN_MOD_STATE state;
        long          warmuptime;
        //MOD_EXILOCK1 ilck, ilckstr;
        bool safelyWarmedUP;
        double MagPs1CurrRead,MagPs2CurrRead,MagPs3CurrRead,MagPs4CurrRead;
        double MagPs1VoltRead,MagPs2VoltRead,MagPs3VoltRead,MagPs4VoltRead;
        double HvPs1CurrRead,HvPs2CurrRead,HvPs3CurrRead,HvPs4CurrRead;
        double HvPs1VoltRead,HvPs2VoltRead,HvPs3VoltRead,HvPs4VoltRead;
        double PrfSet, PrfRead;
        double CtRead, CvdRead;
        double PlswthSet,PlswthRead,PlswthFwhmRead;
        double ionp;

        std::vector< std::string > goodModErrorReadStr, badModErrorReadStr;
    #ifndef __CINT__
        std::map< GUN_MOD_PV_TYPE, pvStruct > pvMonStructs, pvComStructs;
    #endif
    };



    DEFINE_ENUM_WITH_STRING_CONVERSIONS( L01_MOD_PV_TYPE, (SYSTEM_STATE_READ) (SYSTEM_STATE_PUT) (HVPS_VOLTAGE_SET) (HVPS_VOLTAGE_SET_READ)
                                                          (HVPS_VOLTAGE_LOW_ALARM_SET)
                                                          (HVPS_VOLTAGE_LOW_ALARM_SET_READ) (HVPS_VOLTAGE_HI_ALARM_SET) (HVPS_VOLTAGE_HI_ALARM_SET_READ)
                                                          (HVPS_VOLTAGE_READ) (HVPS_CURRENT_READ) (HEATER_VOLTAGE) (HEATER_CURRENT)  (RESET_VOLTAGE)
                                                          (RESET_CURRENT)  (ION_PUMP_VOLTAGE) (ION_PUMP_CURRENT) (SUPPLY_TEMP)  (SUPPLY_PRESSURE)
                                                          (RETURN_TEMP)  (RETURN_PRESSURE)  (BODY_FLOW_RATE)  (COLLECTOR_FLOW_RATE) (SOLENOID_FLOW_RATE)
                                                          (TANK_FLOW_RATE)  (COLLECTOR_RETURN_RATE)  (BODY_RETURN_TEMPERATURE)  (SOLENOID_1_VOLTAGE)
                                                          (SOLENOID_2_VOLTAGE)  (SOLENOID_3_VOLTAGE)  (SOLENOID_1_CURRENT) (SOLENOID_2_CURRENT)
                                                          (SOLENOID_3_CURRENT) (L01_RESET) (L01_FAULT)
                                                          (ERROR_WORD_1) (ERROR_DESC_1)
                                                          (ERROR_WORD_2) (ERROR_DESC_2)
                                                          (ERROR_WORD_3) (ERROR_DESC_3)
                                                          (ERROR_WORD_4) (ERROR_DESC_4)
                                                          (ERROR_WORD_5) (ERROR_DESC_5)
                                                          (ERROR_WORD_6) (ERROR_DESC_6)
                                                          (ERROR_WORD_7) (ERROR_DESC_7)
                                                          (ERROR_WORD_8) (ERROR_DESC_8)
                                                          (ERROR_WORD_9) (ERROR_DESC_9)
                                                          (ERROR_WORD_10) (ERROR_DESC_10)
                                                          (ERROR_WORD_11) (ERROR_DESC_11)
                                                          (ERROR_WORD_12) (ERROR_DESC_12)
                                                          (ERROR_WORD_13) (ERROR_DESC_13)
                                                          (ERROR_WORD_14) (ERROR_DESC_14)
                                                          (ERROR_WORD_15) (ERROR_DESC_15)
                                                          (ERROR_WORD_16) (ERROR_DESC_16)
                                                          (ERROR_WORD_17) (ERROR_DESC_17)
                                                          (ERROR_WORD_18) (ERROR_DESC_18)
                                                          (ERROR_WORD_19) (ERROR_DESC_19)
                                                          (ERROR_WORD_20) (ERROR_DESC_20))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( L01_MOD_STATE, (ERROR)(FAULT) );



    struct l01ModObject
    {
        l01ModObject() :
            state(L01_MOD_STATE::ERROR),
            safelyWarmedUP(false)
            {}
        std::string name, pvRoot;
        L01_MOD_STATE state;
        long          warmuptime;
        //MOD_EXILOCK1 ilck, ilckstr;
        bool safelyWarmedUP;
        double hvpsVSet,hvpsVSetRead,hvpsVRead;
        std::vector< std::string > modErrorWords, modErrorDescs;
    #ifndef __CINT__
        std::map< L01_MOD_PV_TYPE, pvStruct > pvMonStructs, pvComStructs;
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
