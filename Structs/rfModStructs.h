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
class l01ModInterface;

namespace rfModStructs
{
    struct pvStruct;
    struct rfPowerObject;
    struct rfPowerMonitorStruct;
    struct monitorStuct;
    struct rfLLRFObject;
    struct rfModObject;
    struct rfObject;

    // GUNS MODULATOR (SCANDINOVA)
    // enums for all the pvs we monitor/control
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(GUN_MOD_PV_TYPE, (IONP_PRESSURE_READ)
                                                         (PULSE_WIDTH_READ)
                                                         (MAGPS1_CURR_READ)
                                                         (MAGPS2_CURR_READ)
                                                         (MAGPS3_CURR_READ)
                                                         (MAGPS4_CURR_READ)
                                                         (MAGPS1_VOLT_READ)
                                                         (MAGPS2_VOLT_READ)
                                                         (MAGPS3_VOLT_READ)
                                                         (MAGPS4_VOLT_READ)
                                                         (HVPS1_CURR_READ)
                                                         (HVPS2_CURR_READ)
                                                         (HVPS3_CURR_READ)
                                                         (HVPS1_VOLT_READ)
                                                         (HVPS2_VOLT_READ)
                                                         (HVPS3_VOLT_READ)
                                                         (ILOCK1_STR)
                                                         (ILOCK2_STR)
                                                         (ILOCK3_STR)
                                                         (ILOCK4_STR)
                                                         (ILOCK5_STR)
                                                         (ERROR_READ_HEX_STR)
                                                         (WARMUP_TIME)
                                                         (MAIN_STATE_READ)
                                                         //(ERROR_READ)
                                                         (STATE_SET)
                                                         (CVD_READ)
                                                         (CT_READ)
                                                         (RESET)
                                                         (PULSE_WIDTH_FWHM_READ)
                                        );
    // a derived state based on if there is an error,
    // the mod is either 'good' - i.e. you can change its main_state
    // or bad - i.e. there is an interlock
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( GUN_MOD_ERR_STATE,(BAD)
                                                           (GOOD)
                                                           (UNKNOWN)
                                        );
//
//    /// These are possible modulator main_states
//    /// These come from CS3A_scandinova.db /home/controls/ioc/ebtf/CS3A/db
//    /// We keep the same numbers as the control system, therefore UNKNOWN1
//
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( GUN_MOD_STATE,
                                        (NOT_CONNECTED)      // field(ZRST, "Init/not conne.")
                                        (STANDYBY_INTERLOCK) // field(ONST, "Standby Interl.")
                                        (OFF)                // field(TWST, "OFF")
                                        (OFF_REQUEST)        // field(THST, "Off Request")
                                        (HV_INTERLOCK)       // field(FRST, "HV Intrlock")
                                        (STANDBY_REQUEST)    // field(FVST, "Standby Request")
                                        (STANDBY)            // field(SXST, "Standby")
                                        (HV_OFF_REQUEST)     // field(SVST, "HV Off Requ.")
                                        (TRIGGER_INTERLOCK)  // field(EIST, "Trigger Interl.")
                                        (HV_REQUEST)         // field(NIST, "HV Request")
                                        (HV_ON)              // field(TEST, "HV On")
                                        (TRIG_OFF_REQUEST)   // field(ELST, "Trig Off Req.")
                                        (TRIG_REQUEST)       // field(TVST, "Trig Request")
                                        (TRIG)               // field(TTST, "Trig")
                                        (UNKNOWN_STATE)      // my default state on instantiation
                                        );
    struct pvStruct
    {
        pvStruct() : pvSuffix(UTL::UNKNOWN_STRING),
                     objName(UTL::UNKNOWN_NAME),
                     COUNT(0),//MAGIC_NUMBER
                     MASK(0)//MAGIC_NUMBER
                     {}
        GUN_MOD_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
    };

    struct gunModObject
    {
        gunModObject() :
            main_state(GUN_MOD_STATE::UNKNOWN_STATE),
            hex_state_str(UTL::UNKNOWN_STRING),
            hex_state_message(UTL::UNKNOWN_STRING),
            error_state(GUN_MOD_ERR_STATE::UNKNOWN),

            safelyWarmedUP(false),
            warmuptime(UTL::ZERO_LONG),

            MagPs1CurrRead(UTL::DUMMY_DOUBLE),
            MagPs2CurrRead(UTL::DUMMY_DOUBLE),
            MagPs3CurrRead(UTL::DUMMY_DOUBLE),
            MagPs4CurrRead(UTL::DUMMY_DOUBLE),
            MagPs1VoltRead(UTL::DUMMY_DOUBLE),
            MagPs2VoltRead(UTL::DUMMY_DOUBLE),
            MagPs3VoltRead(UTL::DUMMY_DOUBLE),
            MagPs4VoltRead(UTL::DUMMY_DOUBLE),

            HvPs1CurrRead(UTL::DUMMY_DOUBLE),
            HvPs2CurrRead(UTL::DUMMY_DOUBLE),
            HvPs3CurrRead(UTL::DUMMY_DOUBLE),
            HvPs1VoltRead(UTL::DUMMY_DOUBLE),
            HvPs2VoltRead(UTL::DUMMY_DOUBLE),
            HvPs3VoltRead(UTL::DUMMY_DOUBLE),

            PrfSet(UTL::DUMMY_DOUBLE),
            PrfRead(UTL::DUMMY_DOUBLE),
            CtRead(UTL::DUMMY_DOUBLE),
            CvdRead(UTL::DUMMY_DOUBLE),
            PlswthRead(UTL::DUMMY_DOUBLE),
            PlswthFwhmRead(UTL::DUMMY_DOUBLE),
            ionp(UTL::DUMMY_DOUBLE),
            //error_read(UTL::UNKNOWN_STRING),

            ilock1(UTL::UNKNOWN_STRING),
            ilock2(UTL::UNKNOWN_STRING),
            ilock3(UTL::UNKNOWN_STRING),
            ilock4(UTL::UNKNOWN_STRING),
            ilock5(UTL::UNKNOWN_STRING)

            {}
        std::string name, pvRoot, hex_state_str, hex_state_message, ilock1,ilock2,ilock3,ilock4,ilock5;
        std::vector<std::vector<std::string>> interlock_history;
        GUN_MOD_STATE main_state;
        GUN_MOD_ERR_STATE error_state;
        long   warmuptime;
        bool   safelyWarmedUP;
        double MagPs1CurrRead,MagPs2CurrRead,MagPs3CurrRead,MagPs4CurrRead,
               MagPs1VoltRead,MagPs2VoltRead,MagPs3VoltRead,MagPs4VoltRead,
               HvPs1CurrRead,HvPs2CurrRead,HvPs3CurrRead,
               HvPs1VoltRead,HvPs2VoltRead,HvPs3VoltRead,
               PrfSet, PrfRead,
               CtRead, CvdRead,
               PlswthSet,PlswthRead,PlswthFwhmRead,
               ionp;
        std::vector<std::string> goodModErrorReadStr, badModErrorReadStr;
        //
        #ifdef BUILD_DLL
            boost::python::list goodModErrorReadStrL, badModErrorReadStrL;
        #endif
        //
        std::map<GUN_MOD_PV_TYPE, pvStruct> pvMonStructs, pvComStructs;
    };

    // LINAC-01 MODULATOR (DTI)

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( L01_MOD_PV_TYPE, (SYSTEM_STATE_READ)
                                                          (SYSTEM_STATE_PUT)
                                                          (HVPS_VOLTAGE_SET)
                                                          (HVPS_VOLTAGE_SET_READ)
                                                          (HVPS_VOLTAGE_LOW_ALARM_SET)
                                                          (HVPS_VOLTAGE_LOW_ALARM_SET_READ)
                                                          (HVPS_VOLTAGE_HI_ALARM_SET)
                                                          (HVPS_VOLTAGE_HI_ALARM_SET_READ)
                                                          (HVPS_VOLTAGE_READ)
                                                          (HVPS_CURRENT_READ)
                                                          (HEATER_VOLTAGE)
                                                          (HEATER_CURRENT)
                                                          (RESET_VOLTAGE)
                                                          (RESET_CURRENT)
                                                          (ION_PUMP_VOLTAGE)
                                                          (ION_PUMP_CURRENT)
                                                          (SUPPLY_TEMP)
                                                          (SUPPLY_PRESSURE)
                                                          (RETURN_TEMP)
                                                          (RETURN_PRESSURE)
                                                          (BODY_FLOW_RATE)
                                                          (COLLECTOR_FLOW_RATE)
                                                          (SOLENOID_FLOW_RATE)
                                                          (TANK_FLOW_RATE)
                                                          (COLLECTOR_RETURN_RATE)
                                                          (BODY_RETURN_TEMPERATURE)
                                                          (SOLENOID_1_VOLTAGE)
                                                          (SOLENOID_2_VOLTAGE)
                                                          (SOLENOID_3_VOLTAGE)
                                                          (SOLENOID_1_CURRENT)
                                                          (SOLENOID_2_CURRENT)
                                                          (SOLENOID_3_CURRENT)
                                                          (L01_RESET)
                                                          (L01_FAULT)
                                                          (ERROR_WORD_1)  (ERROR_DESC_1)
                                                          (ERROR_WORD_2)  (ERROR_DESC_2)
                                                          (ERROR_WORD_3)  (ERROR_DESC_3)
                                                          (ERROR_WORD_4)  (ERROR_DESC_4)
                                                          (ERROR_WORD_5)  (ERROR_DESC_5)
                                                          (ERROR_WORD_6)  (ERROR_DESC_6)
                                                          (ERROR_WORD_7)  (ERROR_DESC_7)
                                                          (ERROR_WORD_8)  (ERROR_DESC_8)
                                                          (ERROR_WORD_9)  (ERROR_DESC_9)
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

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(L01_MOD_STATE, (STATE_UNKNOWN)
                                                       (STATE_OFF)
                                                       (L01_STANDBY)
                                                       (HV)
                                                       (TRANSMIT))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(L01_MOD_FAULT, (FAULT_UNKNOWN)(NO_FAULT)(FAULT))

    /// These can't go in VELA_ENUM as they need a pvType.
    struct l01_pvStruct
    {
      l01_pvStruct():
            pvSuffix("UNKNOWN"),
            objName("UNKNOWN"),
            COUNT(0),
            MASK(0)
            {}
        L01_MOD_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
    };

    struct l01ModObject
    {
        l01ModObject() :
            system_state_read(L01_MOD_STATE::STATE_UNKNOWN),
            l01_fault(L01_MOD_FAULT::FAULT_UNKNOWN),
            safelyWarmedUP(false),
            modErrorDescs(20,""),
            modErrorWords(20,""),
            heater_voltage(UTL::DUMMY_DOUBLE),
            heater_current(UTL::DUMMY_DOUBLE),
            reset_voltage(UTL::DUMMY_DOUBLE),
            reset_current(UTL::DUMMY_DOUBLE),
            hvps_current_read(UTL::DUMMY_DOUBLE),
            ion_pump_voltage(UTL::DUMMY_DOUBLE),
            ion_pump_current(UTL::DUMMY_DOUBLE),
            supply_temp(UTL::DUMMY_DOUBLE),
            supply_pressure(UTL::DUMMY_DOUBLE),
            return_temp(UTL::DUMMY_DOUBLE),
            return_pressure(UTL::DUMMY_DOUBLE),
            body_flow_rate(UTL::DUMMY_DOUBLE),
            collector_flow_rate(UTL::DUMMY_DOUBLE),
            solenoid_flow_rate(UTL::DUMMY_DOUBLE),
            tank_flow_rate(UTL::DUMMY_DOUBLE),
            collector_return_rate(UTL::DUMMY_DOUBLE),
            body_return_temperature(UTL::DUMMY_DOUBLE),
            solenoid_1_voltage(UTL::DUMMY_DOUBLE),
            hvps_voltage_read(UTL::DUMMY_DOUBLE),
            solenoid_2_voltage(UTL::DUMMY_DOUBLE),
            solenoid_3_voltage(UTL::DUMMY_DOUBLE),
            solenoid_1_current(UTL::DUMMY_DOUBLE),
            solenoid_2_current(UTL::DUMMY_DOUBLE),
            solenoid_3_current(UTL::DUMMY_DOUBLE),
            //system_state_read(UTL::DUMMY_DOUBLE),
            hvps_voltage_set_read(UTL::DUMMY_DOUBLE),
            hvps_voltage_low_alarm_set_read(UTL::DUMMY_DOUBLE),
            hvps_voltage_hi_alarm_set_read(UTL::DUMMY_DOUBLE),
            system_state_put(UTL::DUMMY_INT)
            {}
        std::string name, pvRoot;
        L01_MOD_STATE system_state_read;
        long          warmuptime;
        bool          safelyWarmedUP;
        double        heater_voltage,heater_current,
                      reset_voltage,
                      reset_current,hvps_current_read,
                      ion_pump_voltage,
                      ion_pump_current,supply_temp,supply_pressure,
                      return_temp,return_pressure,body_flow_rate,
                      collector_flow_rate,solenoid_flow_rate,tank_flow_rate,
                      collector_return_rate,body_return_temperature,
                      solenoid_1_voltage,hvps_voltage_read,
                      solenoid_2_voltage,solenoid_3_voltage,
                      solenoid_1_current,solenoid_2_current,
                      solenoid_3_current,
                      hvps_voltage_set_read,hvps_voltage_low_alarm_set_read,
                      hvps_voltage_hi_alarm_set_read;
        L01_MOD_FAULT l01_fault;
        int           system_state_put;
        std::vector<std::string> modErrorWords, modErrorDescs;
        std::map<L01_MOD_PV_TYPE, l01_pvStruct> pvMonStructs, pvComStructs;
        //
        #ifdef BUILD_DLL
            boost::python::list modErrorWordsL, modErrorDescsL;
        #endif
        //
    };
    struct l01_monitorStruct
    {
        l01_monitorStruct():
            rfModObject(nullptr),
            interface(nullptr)
            {}
        L01_MOD_PV_TYPE  monType;
        l01ModObject*    rfModObject;
        chtype           CHTYPE;
        //void *           val; /// most of the rfObject Values appear to be doubles, which is handy... recast in staticEntry function...
        l01ModInterface* interface;
        evid             EVID;
    };
    struct monitorStruct
    {
        monitorStruct() :
            rfModObject(nullptr),
            interface(nullptr)
            {}
        GUN_MOD_PV_TYPE  monType;
        gunModObject*    rfModObject;
        chtype           CHTYPE;
        //void *           val; /// most of the rfObject Values appear to be doubles, which is handy... recast in staticEntry function...
        gunModInterface* interface;
        evid             EVID;
    };
}
#endif//_RF_MOD_STRUCTS_H_
