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
//  Author:      DJS
//  Last edit:   19-03-2018
//  FileName:    configDefinitions.h
//  Description: Constant definitions for strings, numbers, etc. to be defined here
//               Mostly used for configfile defintions,
//               but also numerical constants that are used throughout
//
//*/
#ifndef CONFIG_DEFINITIONS_H
#define CONFIG_DEFINITIONS_H
// stl includes
#include <string>
#include <vector>
#include <map>
#include <chrono>
// epics includes
#include <cadef.h>
// boost.python includes
#ifdef BUILD_DLL
#include <boost/python/def.hpp>
#endif // BUILD_DLL
//______________________________________________________________________________
namespace UTL
{
    /// numerical and string constants
    const double DUMMY_DOUBLE = -999.999;
    const double ONEEIGHTY_ZERO_DOUBLE = 180.0;
    const double TEN_POWER_MINUS_NINE  = 0.000000001;
    const double ZERO_DOUBLE  = 0.0;
    const double TEN_DOUBLE   = 10.0;
    const double FIVE_DOUBLE  = 5.0;
    const size_t ZERO_SIZET   = 0;
    const size_t ONE_SIZET    = 1;
    const size_t TWO_SIZET    = 2;
    const size_t THREE_SIZET  = 3;
    const size_t FOUR_SIZET   = 4;
    const size_t FIVE_SIZET   = 5;
    const long   DUMMY_LONG   = -999;
    const int    DUMMY_INT    = -9999;
    const size_t DUMMY_SIZET  = 999;
    const size_t TEN_SIZET    = 10;
    const size_t HUNDRED_SIZET= 100;
    const size_t BUFFER_TEN   = 10;
    const size_t BUFFER_EIGHTY= 80;
    const size_t BUFFER_36    = 36;
    const long   ZERO_LONG    = 0;
    const int    ZERO_INT     = 0;
    const int    ONE_INT      = 1;
    const int    TWO_INT      = 2;
    const int    THREE_INT    = 3;
    const int    FOUR_INT     = 4;
    const int    FIVE_INT     = 5;
    const int    SIX_INT      = 6;
    const int    SEVEN_INT    = 7;
    const int    EIGHT_INT    = 8;
    const int    NINE_INT     = 9;
    const int    TEN_INT      = 10;
    const int    ELEVEN_INT   = 11;
    const int    TWELVE_INT   = 12;
    const int    THIRTEEN_INT = 13;
    const int    FOURTEEN_INT = 14;
    const int    FIFTEEN_INT  = 15;
    const int    SIXTEEN_INT  = 16;
    const int    MINUS_ONE_INT= -1;
    const int    MINUS_TWO_INT= -2;
    const unsigned   ONE_UINT = 1;

    const capri PRIORITY_0  = 0;
    const capri PRIORITY_99 = 99;

    const unsigned short ZERO_US  = 0;
    const unsigned short ONE_US   = 1;
    const unsigned short TWO_US   = 2;
    const unsigned short THREE_US = 3;
    const unsigned long  ZERO_UL  = 0;

    const unsigned short EPICS_ACTIVATE = ONE_US;
    const unsigned short EPICS_SEND     = ZERO_US;
    const unsigned short EPICS_RESET    = ONE_US;
    const double         DBL_ERR_NUM    =-9999.9999;

    const std::chrono::milliseconds STANDARD_PAUSE(2000);
    const std::chrono::milliseconds PAUSE_300(300);
    const std::string  UNKNOWN_STRING = "UNKNOWN";
    const std::string  UNKNOWN_PVROOT = "UNKNOWN_PVROOT";
    const std::string  UNKNOWN_NAME   = "UNKNOWN_NAME";
    const std::string EMPTY_STRING = "";
    const std::string END_OF_LINE = ";";
    const std::string SLASH_SLASH = "\\";
    const std::string EQUALS_SIGN = "=";
    const char EQUALS_SIGN_C      = '=';
    const char COLON_C            = ':';
    const char DOUBLE_QUOTE_C     = '"';
    const char SPACE_C            = ' ';
    const char TAB_C              = '\t';
    const char COMMA_C            = ',';
    // default config paths -
    const std::string CONFIG_PATH = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Software\\VELA_CLARA_PYDs\\Config\\";
    const std::string APCLARA1_CONFIG_PATH = "\\\\apclara1.dl.ac.uk\\ControlRoomApps\\Controllers\\Config\\";
    // Virtual Machine PVs have this prefix ENFORCED
    const std::string VM_PREFIX   = "VM-";
    // config file names
    const std::string NO_CONFIG_FILE = "NO_CONFIG_FILE";
    const std::string PHASE = "PHASE";
    const std::string POWER = "POWER";

    /// General Keywords
    /// These are the keywords used in hardware config files
    const std::string START_OF_DATA     = "START_OF_DATA";
    const std::string END_OF_DATA       = "END_OF_DATA";
    const std::string VERSION           = "VERSION";
    const std::string NAME              = "NAME";
    const std::string PV_ROOT           = "PV_ROOT";
    const std::string OBJECTS_START     = "OBJECTS_START";
    const std::string NUMBER_OF_OBJECTS = "NUMBER_OF_OBJECTS";
    const std::string PV_MONITORS_START = "PV_MONITORS_START";
    const std::string PV_COMMANDS_START = "PV_COMMANDS_START";
    const std::string PV_ARRAY_MONITORS_START = "PV_ARRAY_MONITORS_START";
    const std::string NUMBER_OF_ILOCKS  = "NUMBER_OF_ILOCKS";
    // General PV suffixes
    const std::string PV_SUFFIX_ON          = "PV_SUFFIX_ON";
    const std::string PV_SUFFIX_OFF         = "PV_SUFFIX_OFF";
    const std::string PV_SUFFIX_STA         = "PV_SUFFIX_STA";
    const std::string PV_SUFFIX_RST         = "PV_SUFFIX_RST";
    const std::string PV_SUFFIX_CMI         = "PV_SUFFIX_CMI";
    const std::string NUMBER_OF_INTERLOCKS  = "NUMBER_OF_INTERLOCKS";
    // General EPICS PV names
    const std::string PV_COUNT  = "PV_COUNT" ;
    const std::string PV_CHTYPE = "PV_CHTYPE";
    const std::string PV_MASK   = "PV_MASK"  ;
    const std::string PV_NAME   = "PV_NAME"  ;

/// Laser shutters
    const std::string VELA_PIL_SHUTTER_CONFIG          = "velaPhotoInjectorpilaserShutter.config";
    const std::string VELA_QBOX_CONFIG                 = "qbox.config";
/// BPMs
    const std::string VELA_INJ_BPM_CONFIG              = "velaINJBeamPositionMonitors.config";
    const std::string VELA_BA1_BPM_CONFIG              = "velaBA1BeamPositionMonitors.config";
    const std::string VELA_BA2_BPM_CONFIG              = "velaBA2BeamPositionMonitors.config";
    const std::string CLARA_INJ_BPM_CONFIG             = "claraINJBeamPositionMonitors.config";
    const std::string CLARA_2_VELA_BPM_CONFIG          = "clara2VELABeamPositionMonitors.config";
    const std::string CLARA_S01_BPM_CONFIG             = "claraS01BeamPositionMonitors.config";
    const std::string CLARA_S02_BPM_CONFIG             = "claraS02BeamPositionMonitors.config";
    const std::string CLARA_PH1_BPM_CONFIG             = "claraPH1BeamPositionMonitors.config";
    // PVs
    const std::string PV_SUFFIX_BPM_SA1     = "PV_SUFFIX_BPM_SA1";
    const std::string PV_SUFFIX_BPM_SA2     = "PV_SUFFIX_BPM_SA2";
    const std::string PV_SUFFIX_BPM_SD1     = "PV_SUFFIX_BPM_SD1";
    const std::string PV_SUFFIX_BPM_SD2     = "PV_SUFFIX_BPM_SD2";
    const std::string PV_SUFFIX_BPM_RA1     = "PV_SUFFIX_BPM_RA1";
    const std::string PV_SUFFIX_BPM_RA2     = "PV_SUFFIX_BPM_RA2";
    const std::string PV_SUFFIX_BPM_RD1     = "PV_SUFFIX_BPM_RD1";
    const std::string PV_SUFFIX_BPM_RD2     = "PV_SUFFIX_BPM_RD2";
    const std::string PV_SUFFIX_BPM_X       = "PV_SUFFIX_BPM_X";
    const std::string PV_SUFFIX_BPM_Y       = "PV_SUFFIX_BPM_Y";
    const std::string PV_SUFFIX_BPM_DATA    = "PV_SUFFIX_BPM_DATA";
    // BPMobject member variables
    const std::string ATT1CAL               = "ATT1CAL";
    const std::string ATT2CAL               = "ATT2CAL";
    const std::string V1CAL                 = "V1CAL";
    const std::string V2CAL                 = "V2CAL";
    const std::string QCAL                  = "QCAL";
    const std::string MN                    = "MN";
    const std::string XN                    = "XN";
    const std::string YN                    = "YN";


/// Qscope
    const std::string VELA_SCOPE_CONFIG                = "velaQScope.config";
    const std::string VELA_SCOPE_TRACE_CONFIG          = "velaQScopeTrace.config";
    const std::string CLARA_SCOPE_CONFIG               = "claraQScope.config";
    const std::string CLARA_SCOPE_TRACE_CONFIG         = "claraQScopeTrace.config";

/// Gun Modulator
    const std::string VELA_RF_GUN_PWR_CONFIG           = "velaRFGunPower.config";
    const std::string RF_GUN_MOD_CONFIG                = "RFGunModulator.config";
    const std::string VELA_RF_GUN_LLRF_CONFIG          = "velaRFGunLLRF.config";
    // Gun Modulator PVs
    const std::string PV_SUFFIX_GUN_MOD_PULSE_WIDTH_FWHM_READ = "PV_SUFFIX_GUN_MOD_PULSE_WIDTH_FWHM_READ";
    const std::string PV_SUFFIX_GUN_MOD_IONP_PRESSURE_READ    = "PV_SUFFIX_GUN_MOD_IONP_PRESSURE_READ";

    const std::string PV_SUFFIX_GUN_MOD_MAGPS1_CURR_READ      = "PV_SUFFIX_GUN_MOD_MAGPS1_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS2_CURR_READ      = "PV_SUFFIX_GUN_MOD_MAGPS2_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS3_CURR_READ      = "PV_SUFFIX_GUN_MOD_MAGPS3_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS4_CURR_READ      = "PV_SUFFIX_GUN_MOD_MAGPS4_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS1_VOLT_READ      = "PV_SUFFIX_GUN_MOD_MAGPS1_VOLT_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS2_VOLT_READ      = "PV_SUFFIX_GUN_MOD_MAGPS2_VOLT_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS3_VOLT_READ      = "PV_SUFFIX_GUN_MOD_MAGPS3_VOLT_READ";
    const std::string PV_SUFFIX_GUN_MOD_MAGPS4_VOLT_READ      = "PV_SUFFIX_GUN_MOD_MAGPS4_VOLT_READ";
    const std::string PV_SUFFIX_GUN_MOD_HVPS1_CURR_READ      = "PV_SUFFIX_GUN_MOD_HVPS1_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_HVPS2_CURR_READ      = "PV_SUFFIX_GUN_MOD_HVPS2_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_HVPS3_CURR_READ      = "PV_SUFFIX_GUN_MOD_HVPS3_CURR_READ";
    const std::string PV_SUFFIX_GUN_MOD_HVPS1_VOLT_READ      = "PV_SUFFIX_GUN_MOD_HVPS1_VOLT_READ";
    const std::string PV_SUFFIX_GUN_MOD_HVPS2_VOLT_READ      = "PV_SUFFIX_GUN_MOD_HVPS2_VOLT_READ";
    const std::string PV_SUFFIX_GUN_MOD_HVPS3_VOLT_READ      = "PV_SUFFIX_GUN_MOD_HVPS3_VOLT_READ";

    const std::string PV_SUFFIX_GUN_MOD_ILOCK1 = "PV_SUFFIX_GUN_MOD_ILOCK1";
    const std::string PV_SUFFIX_GUN_MOD_ILOCK2 = "PV_SUFFIX_GUN_MOD_ILOCK2";
    const std::string PV_SUFFIX_GUN_MOD_ILOCK3 = "PV_SUFFIX_GUN_MOD_ILOCK3";
    const std::string PV_SUFFIX_GUN_MOD_ILOCK4 = "PV_SUFFIX_GUN_MOD_ILOCK4";
    const std::string PV_SUFFIX_GUN_MOD_ILOCK5 = "PV_SUFFIX_GUN_MOD_ILOCK5";


    const std::string PV_SUFFIX_GUN_MOD_PULSE_WIDTH_READ      = "PV_SUFFIX_GUN_MOD_PULSE_WIDTH_READ";
    const std::string PV_SUFFIX_GUN_MOD_STATE_READ_STRING = "PV_SUFFIX_GUN_MOD_STATE_READ_STRING";
    const std::string PV_SUFFIX_GUN_MOD_MAIN_STATE_SET  = "PV_SUFFIX_GUN_MOD_MAIN_STATE_SET";
    const std::string PV_SUFFIX_GUN_MOD_MAIN_STATE_READ  = "PV_SUFFIX_GUN_MOD_MAIN_STATE_READ";
    //const std::string PV_SUFFIX_GUN_MOD_ERR_STATE  = "PV_SUFFIX_GUN_MOD_ERR_STATE";
    const std::string PV_SUFFIX_GUN_MOD_CVD_READ  = "PV_SUFFIX_GUN_MOD_CVD_READ";
    const std::string PV_SUFFIX_GUN_MOD_ERR_SVAL  = "PV_SUFFIX_GUN_MOD_ERR_SVAL";
    const std::string PV_SUFFIX_GUN_MOD_WARMUPT   = "PV_SUFFIX_GUN_MOD_WARMUPT";
    const std::string PV_SUFFIX_GUN_MOD_CT_READ   = "PV_SUFFIX_GUN_MOD_CT_READ";
    const std::string PV_SUFFIX_GUN_MOD_RESET         = "PV_SUFFIX_GUN_MOD_RESET";

/// Magnets
    const std::string VELA_INJ_MAG_CONFIG              = "velaINJMagnets.config";
    const std::string VELA_BA1_MAG_CONFIG              = "velaBA1Magnets.config";
    const std::string VELA_BA2_MAG_CONFIG              = "velaBA2Magnets.config";
    const std::string CLARA_INJ_MAG_CONFIG             = "claraINJMagnets.config";// not going to use
    const std::string CLARA_PH1_MAG_CONFIG             = "claraPH1Magnets.config";
/// Screens
    const std::string VELA_INJ_SCREENS_CONFIG  = "velaINJScreens.config";
    const std::string CLARA_PH1_SCREENS_CONFIG  = "claraPH1Screens.config";
/// pilaser (DJS faked 17-02-2017)
    const std::string PILASER_CONFIG        = "velaclaraPILaser.config";
    // laser PVs
    const std::string PV_SUFFIX_LASER_INTENSITY = "PV_SUFFIX_LASER_INTENSITY";
    const std::string PV_SUFFIX_LASER_H_POS = "PV_SUFFIX_LASER_H_POS";
    const std::string PV_SUFFIX_LASER_V_POS = "PV_SUFFIX_LASER_V_POS";


/// Cameras
    const std::string DAQ_CAMERA_CONFIG               = "cameraDAQ.config";
    const std::string CAMERA_CONFIG               = "camera.config";
    //const std::string VELA_CAMERA_IOC_CONFIG           = "velaCameraIOC.config";
/// Vacuum
    const std::string VELA_INJ_IMG_CONFIG              = "velaINJInvertedMagnetronGauge.config";
    const std::string VELA_BA1_IMG_CONFIG              = "velaBA1InvertedMagnetronGauge.config";
    const std::string VELA_BA2_IMG_CONFIG              = "velaBA2InvertedMagnetronGauge.config";
    const std::string CLARA_INJ_IMG_CONFIG             = "claraINJInvertedMagnetronGauge.config";
    const std::string CLARA_2_VELA_IMG_CONFIG          = "clara2VELAInvertedMagnetronGauge.config";
    const std::string CLARA_S01_IMG_CONFIG             = "claraS01InvertedMagnetronGauge.config";
    const std::string CLARA_S02_IMG_CONFIG             = "claraS02InvertedMagnetronGauge.config";
    const std::string CLARA_PH1_IMG_CONFIG             = "claraPH1InvertedMagnetronGauge.config";
    const std::string VELA_INJ_VALVE_CONFIG            = "velaINJVacuumValve.config";
    const std::string VELA_BA1_VALVE_CONFIG            = "velaBA1VacuumValve.config";
    const std::string VELA_BA2_VALVE_CONFIG            = "velaBA2VacuumValve.config";
    const std::string CLARA_INJ_VALVE_CONFIG           = "claraINJVacuumValve.config";
    const std::string CLARA_2_VELA_VALVE_CONFIG        = "clara2VELAVacuumValve.config";
    const std::string CLARA_S01_VALVE_CONFIG           = "claraS01VacuumValve.config";
    const std::string CLARA_S02_VALVE_CONFIG           = "claraS02VacuumValve.config";
    const std::string CLARA_PH1_VALVE_CONFIG           = "claraPH1VacuumValve.config";
/// LIBERA LLRF
    const std::string CLARA_LRRG_LLRF_CONFIG = "CLARA_LRRG_LLRF.config";
    const std::string CLARA_HRRG_LLRF_CONFIG = "CLARA_HRRG_LLRF.config";
    const std::string CLARA_L01_LLRF_CONFIG = "CLARA_L01_LLRF.config";
    // llrf traces
    const std::string GUN_LLRF_TRACES = "gun_LLRF_TRACES.config";
    const std::string L01_LLRF_TRACES = "L01_LLRF_TRACES.config";

    // Faked the VM versions atm until (probably not needed now 14/7/2017
    const std::string VM_CLARA_LRRG_LLRF_CONFIG = "vmCLARA_LRRG_LLRF.config";
    const std::string VM_CLARA_L01_LLRF_CONFIG = "vmCLARA_L01_LLRF.config";

    // Not sure about this yet at all,
    // hopefully very similar to CLARA_LRRG_LLRF_GUN_CONFIG
    // (its the same physical LLRF equipment)
    const std::string VM_VELA_HRRG_LLRF_CONFIG    = "vmVELA_HRRG_LLRF.config";
    const std::string VELA_HRRG_LLRF_CONFIG    = "VELA_HRRG_LLRF.config";

    // llrf object parameters
    const std::string LLRF_MAX_AMPLITUDE = "LLRF_MAX_AMPLITUDE";
    const std::string LLRF_AMP_CALIBRATION = "LLRF_AMP_CALIBRATION";
    const std::string LLRF_PHI_CALIBRATION = "LLRF_PHI_CALIBRATION";
    const std::string LLRF_CREST_PHI = "LLRF_CREST_PHI";
    const std::string LLRF_PULSE_LATENCY = "LLRF_PULSE_LATENCY";

    // new LLRF pvs that should hopefully be similar for all CLARA / VELA LLRF
    // except, probably, the VELA TDC :((
    const std::string PV_SUFFIX_TRIG_SOURCE     = "PV_SUFFIX_TRIG_SOURCE";
    const std::string PV_SUFFIX_LIB_AMP_FF      = "PV_SUFFIX_LIB_AMP_FF";
    const std::string PV_SUFFIX_LIB_AMP_SP      = "PV_SUFFIX_LIB_AMP_SP";
    const std::string PV_SUFFIX_LIB_PHI_FF      = "PV_SUFFIX_LIB_PHI_FF";
    const std::string PV_SUFFIX_LIB_PHI_SP      = "PV_SUFFIX_LIB_PHI_SP";
    const std::string PV_SUFFIX_LIB_TIME_VECTOR = "PV_SUFFIX_LIB_TIME_VECTOR";
    const std::string PV_SUFFIX_LIB_PULSE_LENGTH= "PV_SUFFIX_LIB_PULSE_LENGTH";
    const std::string PV_SUFFIX_LIB_RF_OUTPUT        = "PV_SUFFIX_LIB_RF_OUTPUT";
    const std::string PV_SUFFIX_ILOCK_STATE     = "PV_SUFFIX_ILOCK_STATE";
    const std::string PV_SUFFIX_FF_AMP_LOCK_STATE   = "PV_SUFFIX_FF_AMP_LOCK_STATE";
    const std::string PV_SUFFIX_FF_PHASE_LOCK_STATE   = "PV_SUFFIX_FF_PHASE_LOCK_STATE";
    const std::string PV_SUFFIX_LIB_PULSE_OFFSET= "PV_SUFFIX_LIB_PULSE_OFFSET";
    const std::string PV_SUFFIX_LIB_CH1_PHASE_REM  = "PV_SUFFIX_LIB_CH1_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH2_PHASE_REM  = "PV_SUFFIX_LIB_CH2_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH3_PHASE_REM  = "PV_SUFFIX_LIB_CH3_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH4_PHASE_REM  = "PV_SUFFIX_LIB_CH4_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH5_PHASE_REM  = "PV_SUFFIX_LIB_CH5_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH6_PHASE_REM  = "PV_SUFFIX_LIB_CH6_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH7_PHASE_REM  = "PV_SUFFIX_LIB_CH7_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH8_PHASE_REM  = "PV_SUFFIX_LIB_CH8_PHASE_REM";
    const std::string PV_SUFFIX_LIB_CH1_PWR_REM    = "PV_SUFFIX_LIB_CH1_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH2_PWR_REM    = "PV_SUFFIX_LIB_CH2_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH3_PWR_REM    = "PV_SUFFIX_LIB_CH3_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH4_PWR_REM    = "PV_SUFFIX_LIB_CH4_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH5_PWR_REM    = "PV_SUFFIX_LIB_CH5_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH6_PWR_REM    = "PV_SUFFIX_LIB_CH6_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH7_PWR_REM    = "PV_SUFFIX_LIB_CH7_PWR_REM";
    const std::string PV_SUFFIX_LIB_CH8_PWR_REM    = "PV_SUFFIX_LIB_CH8_PWR_REM";
    const std::string LLRF_CHANNEL    = "LLRF_CHANNEL";


    const std::string PV_SUFFIX_PWR_REM_EVID = "PV_SUFFIX_PWR_REM_EVID";
    const std::string PV_SUFFIX_PWR_REM_SCAN = "PV_SUFFIX_PWR_REM_SCAN";
    const std::string PV_SUFFIX_PWR_REM      = "PV_SUFFIX_PWR_REM";

    const std::string PV_SUFFIX_PHASE_REM_EVID = "PV_SUFFIX_PHASE_REM_EVID";
    const std::string PV_SUFFIX_PHASE_REM_SCAN = "PV_SUFFIX_PHASE_REM_SCAN";
    const std::string PV_SUFFIX_PHASE_REM      = "PV_SUFFIX_PHASE_REM";

    const std::string PV_SUFFIX_AMP_DER_DERIVATIVE      = "PV_SUFFIX_AMP_DER_DERIVATIVE";
    const std::string PV_SUFFIX_AMP_DER_EVID            = "PV_SUFFIX_AMP_DER_EVID";
    const std::string PV_SUFFIX_AMP_DER_SCAN            = "PV_SUFFIX_AMP_DER_SCAN";
    const std::string PV_SUFFIX_PHASE_DER_SCAN          = "PV_SUFFIX_PHASE_DER_SCAN";
    const std::string PV_SUFFIX_PHASE_DER_EVID          = "PV_SUFFIX_PHASE_DER_EVID";
    const std::string PV_SUFFIX_PHASE_DER_DERIVATIVE    = "PV_SUFFIX_PHASE_DER_DERIVATIVE";
    const std::string PV_SUFFIX_PWR_LOC_EVID            = "PV_SUFFIX_PWR_LOC_EVID";
    const std::string PV_SUFFIX_PWR_LOC_SCAN            = "PV_SUFFIX_PWR_LOC_SCAN";
    const std::string PV_SUFFIX_PWR_LOC_POWER           = "PV_SUFFIX_PWR_LOC_POWER";

    const std::string PV_SUFFIX_LIB_CH1_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH1_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH2_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH2_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH3_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH3_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH4_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH4_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH5_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH5_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH6_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH6_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH7_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH7_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH8_PHASE_REM_EVID  = "PV_SUFFIX_LIB_CH8_PHASE_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH1_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH1_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH2_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH2_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH3_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH3_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH4_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH4_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH5_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH5_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH6_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH6_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH7_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH7_PWR_REM_EVID";
    const std::string PV_SUFFIX_LIB_CH8_PWR_REM_EVID    = "PV_SUFFIX_LIB_CH8_PWR_REM_EVID";
//    const std::string PV_SUFFIX_LIB_KLY_FWD     = "PV_SUFFIX_LIB_KLY_FWD";
//    const std::string PV_SUFFIX_LIB_KLY_REV     = "PV_SUFFIX_LIB_KLY_REV";
    // specific names for common channels that have named functions to start/stop them in the llrfinterface/controller
    const std::string LRRG_CAVITY_REVERSE_PHASE   = "LRRG_CAVITY_REVERSE_PHASE";
    const std::string HRRG_CAVITY_REVERSE_PHASE   = "HRRG_CAVITY_REVERSE_PHASE";
    const std::string CAVITY_REVERSE_PHASE        = "CAVITY_REVERSE_PHASE";

    const std::string LRRG_CAVITY_FORWARD_PHASE   = "LRRG_CAVITY_FORWARD_PHASE";
    const std::string HRRG_CAVITY_FORWARD_PHASE   = "HRRG_CAVITY_FORWARD_PHASE";
    const std::string CAVITY_FORWARD_PHASE        = "CAVITY_FORWARD_PHASE";

    const std::string LRRG_CAVITY_REVERSE_POWER   = "LRRG_CAVITY_REVERSE_POWER";
    const std::string HRRG_CAVITY_REVERSE_POWER   = "HRRG_CAVITY_REVERSE_POWER";
    const std::string CAVITY_REVERSE_POWER        = "CAVITY_REVERSE_POWER";

    const std::string LRRG_CAVITY_FORWARD_POWER   = "LRRG_CAVITY_FORWARD_POWER";
    const std::string HRRG_CAVITY_FORWARD_POWER   = "HRRG_CAVITY_FORWARD_POWER";
    const std::string CAVITY_FORWARD_POWER        = "CAVITY_FORWARD_POWER";

    const std::string KLYSTRON_REVERSE_PHASE = "KLYSTRON_REVERSE_PHASE";
    const std::string KLYSTRON_REVERSE_POWER = "KLYSTRON_REVERSE_POWER";
    const std::string KLYSTRON_FORWARD_PHASE = "KLYSTRON_FORWARD_PHASE";
    const std::string KLYSTRON_FORWARD_POWER = "KLYSTRON_FORWARD_POWER";
    const std::string CAVITY_PROBE_POWER = "CAVITY_PROBE_POWER";
    const std::string CAVITY_PROBE_PHASE = "CAVITY_PROBE_PHASE";

// L01 MODULATOR
    const std::string L01_MOD_CONFIG  = "claraL01Modulator.config";





// RF POWER
    const std::string PV_SUFFIX_REV   = "PV_SUFFIX_REV";
    const std::string PV_SUFFIX_FWD   = "PV_SUFFIX_FWD";
    const std::string PV_SUFFIX_REVT  = "PV_SUFFIX_REVT";
    const std::string PV_SUFFIX_FWDT  = "PV_SUFFIX_FWDT";
    const std::string PV_SUFFIX_REVTM = "PV_SUFFIX_REVTM";
    const std::string PV_SUFFIX_FWDTM = "PV_SUFFIX_FWDTM";
    const std::string PV_SUFFIX_RATIO = "PV_SUFFIX_RATIO";

/// RF Protection
    const std::string RF_GUN_PROTECTION_CONFIG          = "RFGunProtection.config";
    const std::string GUN_PROT_KEY_BITS = "GUN_PROT_KEY_BITS";
    const std::string RF_GUN_PROT_TYPE = "RF_GUN_PROT_TYPE";
    const std::string ENABLE     = "ENABLE";
    const std::string TEST       = "TEST";
    const std::string CLARA_LRRG = "CLARA_LRRG";
    const std::string CLARA_HRRG = "CLARA_HRRG";
    const std::string VELA_LRRG  = "VELA_LRRG";
    const std::string VELA_HRRG  = "VELA_HRRG";
    const std::string GENERAL    = "GENERAL";

    // QBOX
    const std::string PV_QBOX_STATE    = "PV_QBOX_STATE";
    const std::string PV_QBOX_SETSTATE = "PV_QBOX_SETSTATE";

    // Vac
    const std::string PV_VAC_PRESSURE      = "PV_VAC_PRESSURE";
    const std::string PV_VAC_PRESSURE_PRES = "PV_VAC_PRESSURE_PRES";

    /// Scope
    // Scope PVs
    const std::string PV_SUFFIX_P1    = "PV_SUFFIX_P1";
    const std::string PV_SUFFIX_P2    = "PV_SUFFIX_P2";
    const std::string PV_SUFFIX_P3    = "PV_SUFFIX_P3";
    const std::string PV_SUFFIX_P4    = "PV_SUFFIX_P4";
    const std::string PV_SUFFIX_TR1   = "PV_SUFFIX_TR1";
    const std::string PV_SUFFIX_TR2   = "PV_SUFFIX_TR2";
    const std::string PV_SUFFIX_TR3   = "PV_SUFFIX_TR3";
    const std::string PV_SUFFIX_TR4   = "PV_SUFFIX_TR4";
    const std::string PV_SCOPE_TYPE   = "PV_SCOPE_TYPE";
    const std::string TIMEBASE        = "TIMEBASE";
    const std::string NOISE_FLOOR     = "NOISE_FLOOR";
    const std::string DIAG_TYPE       = "DIAG_TYPE";
    const std::string WCM             = "WCM";
    const std::string ICT1            = "ICT1";
    const std::string ICT2            = "ICT2";
    const std::string FCUP            = "FCUP";
    const std::string ED_FCUP         = "ED_FCUP";
    const std::string SCOPE_PV_TYPE   = "SCOPE_PV_TYPE";
    const std::string SCOPE_NAME      = "SCOPE_NAME";
    const std::string CLARASCOPE01    = "CLARASCOPE01";
    const std::string VELASCOPE02     = "VELASCOPE02";



    // NR-PSU
    const std::string PARENT_MAGNET   = "PARENT_MAGNET";
    const std::string MAG_GANG_MEMBER = "MAG_GANG_MEMBER";
    const std::string PV_ROOT_N       = "PV_ROOT_N";
    const std::string PV_ROOT_R       = "PV_ROOT_R";
    // Magnet
    const std::string UNKNOWN_MAGNET_STRING = "UNKNOWN_MAGNET";
    const std::string PV_SUFFIX_RI = "PV_SUFFIX_RI";
    const std::string PV_SUFFIX_SI = "PV_SUFFIX_SI";
    const std::string PV_PSU_ROOT  = "PV_PSU_ROOT";
    const std::string PV_SUFFIX_SETPOWER  = "PV_SUFFIX_SETPOWER"; // clara phase 1 magnets have this for power - may change>>??
    // clara ... Expect VELA to TAKE on these names ALD
    const std::string PV_SUFFIX_SPOWER  = "PV_SUFFIX_SPOWER";
    const std::string PV_SUFFIX_RPOWER  = "PV_SUFFIX_RPOWER";
    const std::string PV_SUFFIX_READI   = "PV_SUFFIX_READI";
    const std::string PV_SUFFIX_SETI    = "PV_SUFFIX_SETI";
    const std::string PV_SUFFIX_GETSETI = "PV_SUFFIX_GETSETI";
    const std::string PV_SUFFIX_RILK    = "PV_SUFFIX_RILK";





    // LEGACY LLRF PVS from the Dark Days oF VELA 2015 :(
    const std::string PV_SUFFIX_AMPR = "PV_SUFFIX_AMPR";
    const std::string PV_SUFFIX_PHI  = "PV_SUFFIX_PHI";
    const std::string PV_SUFFIX_AMPW = "PV_SUFFIX_AMPW";
    const std::string PV_SUFFIX_AMP_MVM = "PV_SUFFIX_AMP_MVM";
    const std::string PV_SUFFIX_PHI_DEG = "PV_SUFFIX_PHI_DEG";


    const std::string MAG_TYPE  = "MAG_TYPE";
    const std::string SOL   = "SOL";
    const std::string BSOL  = "BSOL";
    const std::string QUAD  = "QUAD";
    const std::string DIP   = "DIP";
    const std::string HCOR  = "HCOR";
    const std::string VCOR  = "VCOR";
    const std::string SEXT  = "SEXT";

    const std::string MAG_REV_TYPE  = "MAG_REV_TYPE";
    const std::string NR        = "NR";
    const std::string BIPOLAR   = "BIPOLAR";
    const std::string NR_GANGED = "NR_GANGED";
    const std::string POS       = "POS";

    const std::string MAGNET_BRANCH   = "MAGNET_BRANCH";

    const std::string MAG_SET_MAX_WAIT_TIME  = "MAG_SET_MAX_WAIT_TIME";
    // deguass
    //const std::string NUM_DEGAUSS_ELEMENTS = "NUM_DEGAUSS_ELEMENTS";
    const std::string NUM_DEGAUSS_STEPS = "NUM_DEGAUSS_STEPS";
    const std::string DEGAUSS_VALUES    = "DEGAUSS_VALUES";
    const std::string DEGAUSS_TOLERANCE = "DEGAUSS_TOLERANCE";
    const std::string RI_TOLERANCE = "RI_TOLERANCE";
    const std::string POSITION  = "POSITION";
    const std::string FIELD_INTEGRAL_COEFFICIENTS ="FIELD_INTEGRAL_COEFFICIENTS";
//    const std::string SLOPE     = "SLOPE";
//    const std::string INTERCEPT = "intercept";
    const std::string MANUFACTURER = "MANUFACTURER";
    const std::string SERIAL_NUMBER = "SERIAL_NUMBER";
    const std::string MEASUREMENT_DATA_LOCATION = "MEASUREMENT_DATA_LOCATION";
    const std::string MAGNETIC_LENGTH = "MAGNETIC_LENGTH";
    const std::string MAG_MAX_I = "MAG_MAX_I";
    const std::string MAG_MIN_I = "MAG_MIN_I";

    // chtypes


     const std::string DBR_TIME_ARRAY_DOUBLE_STR = "DBR_TIME_ARRAY_DOUBLE";
     const std::string DBR_TIME_ARRY_INT_STR     = "DBR_TIME_ARRAY_INT";
     const std::string DBR_ARRAY_DOUBLE_STR      = "DBR_ARRAY_DOUBLE";
     const std::string DBR_TIME_STRING_STR       = "DBR_TIME_STRING";
     const std::string DBR_TIME_DOUBLE_STR       = "DBR_TIME_DOUBLE";
     const std::string DBR_TIME_SHORT_STR        = "DBR_TIME_SHORT";
     const std::string DBR_TIME_FLOAT_STR        = "DBR_TIME_FLOAT";
     const std::string DBR_TIME_ENUM_STR         = "DBR_TIME_ENUM";
     const std::string DBR_TIME_CHAR_STR         = "DBR_TIME_CHAR";
     const std::string DBR_TIME_LONG_STR         = "DBR_TIME_LONG";
     const std::string DBR_ARRY_INT_STR          = "DBR_ARRAY_INT";
     const std::string DBR_TIME_INT_STR	         = "DBR_TIME_INT";
     const std::string DBR_DOUBLE_STR            = "DBR_DOUBLE";
     const std::string DBR_STRING_STR            = "DBR_STRING";
     const std::string DBR_SHORT_STR             = "DBR_SHORT";
     const std::string DBR_FLOAT_STR             = "DBR_FLOAT";
     const std::string DBR_ENUM_STR	             = "DBR_ENUM";
     const std::string DBR_CHAR_STR	             = "DBR_CHAR";
     const std::string DBR_LONG_STR	             = "DBR_LONG";
     const std::string DBR_INT_STR	             = "DBR_INT";
     /// there are many others... e.g.
//    DBR_STS_ENUM	10	DBR_TIME_DOUBLE	20	DBR_CTRL_INT
//    DBR_STS_CHAR	11	DBR_GR_STRING	21	DBR_CTRL_FLOAT
//    DBR_STS_LONG	12	DBR_GR_SHORT	22	DBR_CTRL_ENUM
//    DBR_STS_DOUBLE	13	DBR_GR_INT	22	DBR_CTRL_CHAR	32
//    DBR_TIME_STRING	14	DBR_GR_FLOAT	23	DBR_CTRL_LONG
//    DBR_TIME_INT	15	DBR_GR_ENUM	24	DBR_CTRL_DOUBLE	34


    //MASK
    const std::string DBE_VALUE_STR = "DBE_VALUE";
    const std::string DBE_LOG_STR	 = "DBE_LOG";
    const std::string DBE_ALARM_STR = "DBE_ALARM";

    const std::string PV_SUFFIX_EXILOCK2  = "PV_SUFFIX_EXILOCK2";
    const std::string BAD_MOD_ERR  = "BAD_MOD_ERR";
    const std::string GOOD_MOD_ERR = "GOOD_MOD_ERR";


    // PV suffixes
    // clara L01
    const std::string PV_SUFFIX_HVPS_VOLTAGE_LOW_ALARM_SET_READ =
                      "PV_SUFFIX_HVPS_VOLTAGE_LOW_ALARM_SET_READ";
    const std::string PV_SUFFIX_HVPS_VOLTAGE_HI_ALARM_SET_READ  =
                      "PV_SUFFIX_HVPS_VOLTAGE_HI_ALARM_SET_READ";
    const std::string PV_SUFFIX_HVPS_VOLTAGE_LOW_ALARM_SET      =
                      "PV_SUFFIX_HVPS_VOLTAGE_LOW_ALARM_SET";
    const std::string PV_SUFFIX_HVPS_VOLTAGE_HI_ALARM_SET       =
                      "PV_SUFFIX_HVPS_VOLTAGE_HI_ALARM_SET";
    const std::string PV_SUFFIX_BODY_RETURN_TEMPERATURE         = "PV_SUFFIX_BODY_RETURN_TEMPERATURE";
    const std::string PV_SUFFIX_HVPS_VOLTAGE_SET_READ           = "PV_SUFFIX_HVPS_VOLTAGE_SET_READ";
    const std::string PV_SUFFIX_COLLECTOR_RETURN_RATE           = "PV_SUFFIX_COLLECTOR_RETURN_RATE";
    const std::string PV_SUFFIX_COLLECTOR_FLOW_RATE             = "PV_SUFFIX_COLLECTOR_FLOW_RATE";
    const std::string PV_SUFFIX_SOLENOID_FLOW_RATE              = "PV_SUFFIX_SOLENOID_FLOW_RATE";
    const std::string PV_SUFFIX_SOLENOID_1_VOLTAGE              = "PV_SUFFIX_SOLENOID_1_VOLTAGE";
    const std::string PV_SUFFIX_SOLENOID_2_VOLTAGE              = "PV_SUFFIX_SOLENOID_2_VOLTAGE";
    const std::string PV_SUFFIX_SOLENOID_3_VOLTAGE              = "PV_SUFFIX_SOLENOID_3_VOLTAGE";
    const std::string PV_SUFFIX_SOLENOID_1_CURRENT              = "PV_SUFFIX_SOLENOID_1_CURRENT";
    const std::string PV_SUFFIX_SOLENOID_2_CURRENT              = "PV_SUFFIX_SOLENOID_2_CURRENT";
    const std::string PV_SUFFIX_SOLENOID_3_CURRENT              = "PV_SUFFIX_SOLENOID_3_CURRENT";
    const std::string PV_SUFFIX_HVPS_VOLTAGE_READ               = "PV_SUFFIX_HVPS_VOLTAGE_READ" ;
    const std::string PV_SUFFIX_HVPS_CURRENT_READ               = "PV_SUFFIX_HVPS_CURRENT_READ" ;
    const std::string PV_SUFFIX_SYSTEM_STATE_READ               = "PV_SUFFIX_SYSTEM_MAIN_STATE_READ";
    const std::string PV_SUFFIX_SYSTEM_STATE_PUT                = "PV_SUFFIX_SYSTEM_STATE_PUT";
    const std::string PV_SUFFIX_HVPS_VOLTAGE_SET                = "PV_SUFFIX_HVPS_VOLTAGE_SET";
    const std::string PV_SUFFIX_ION_PUMP_VOLTAGE                = "PV_SUFFIX_ION_PUMP_VOLTAGE";
    const std::string PV_SUFFIX_ION_PUMP_CURRENT                = "PV_SUFFIX_ION_PUMP_CURRENT";
    const std::string PV_SUFFIX_RETURN_PRESSURE                 = "PV_SUFFIX_RETURN_PRESSURE";
    const std::string PV_SUFFIX_SUPPLY_PRESSURE                 = "PV_SUFFIX_SUPPLY_PRESSURE";
    const std::string PV_SUFFIX_HEATER_SETBACK                  = "PV_SUFFIX_HEATER_SETBACK";
    const std::string PV_SUFFIX_HEATER_VOLTAGE                  = "PV_SUFFIX_HEATER_VOLTAGE";
    const std::string PV_SUFFIX_HEATER_CURRENT                  = "PV_SUFFIX_HEATER_CURRENT";
    const std::string PV_SUFFIX_RESET_VOLTAGE                   = "PV_SUFFIX_RESET_VOLTAGE";
    const std::string PV_SUFFIX_RESET_CURRENT                   = "PV_SUFFIX_RESET_CURRENT";
    const std::string PV_SUFFIX_BODY_FLOW_RATE                  = "PV_SUFFIX_BODY_FLOW_RATE";
    const std::string PV_SUFFIX_TANK_FLOW_RATE                  = "PV_SUFFIX_TANK_FLOW_RATE";
    const std::string PV_SUFFIX_SUPPLY_TEMP                     = "PV_SUFFIX_SUPPLY_TEMP";
    const std::string PV_SUFFIX_RETURN_TEMP                     = "PV_SUFFIX_RETURN_TEMP";
    const std::string PV_SUFFIX_L01_RESET                       = "PV_SUFFIX_L01_RESET";
    const std::string PV_SUFFIX_L01_FAULT                       = "PV_SUFFIX_L01_FAULT";

    const std::string PV_SUFFIX_ERROR_WORD_1  = "PV_SUFFIX_ERROR_WORD_1";
    const std::string PV_SUFFIX_ERROR_WORD_2  = "PV_SUFFIX_ERROR_WORD_2";
    const std::string PV_SUFFIX_ERROR_WORD_3  = "PV_SUFFIX_ERROR_WORD_3";
    const std::string PV_SUFFIX_ERROR_WORD_4  = "PV_SUFFIX_ERROR_WORD_4";
    const std::string PV_SUFFIX_ERROR_WORD_5  = "PV_SUFFIX_ERROR_WORD_5";
    const std::string PV_SUFFIX_ERROR_WORD_6  = "PV_SUFFIX_ERROR_WORD_6";
    const std::string PV_SUFFIX_ERROR_WORD_7  = "PV_SUFFIX_ERROR_WORD_7";
    const std::string PV_SUFFIX_ERROR_WORD_8  = "PV_SUFFIX_ERROR_WORD_8";
    const std::string PV_SUFFIX_ERROR_WORD_9  = "PV_SUFFIX_ERROR_WORD_9";
    const std::string PV_SUFFIX_ERROR_WORD_10 = "PV_SUFFIX_ERROR_WORD_10";
    const std::string PV_SUFFIX_ERROR_WORD_11 = "PV_SUFFIX_ERROR_WORD_11";
    const std::string PV_SUFFIX_ERROR_WORD_12 = "PV_SUFFIX_ERROR_WORD_12";
    const std::string PV_SUFFIX_ERROR_WORD_13 = "PV_SUFFIX_ERROR_WORD_13";
    const std::string PV_SUFFIX_ERROR_WORD_14 = "PV_SUFFIX_ERROR_WORD_14";
    const std::string PV_SUFFIX_ERROR_WORD_15 = "PV_SUFFIX_ERROR_WORD_15";
    const std::string PV_SUFFIX_ERROR_WORD_16 = "PV_SUFFIX_ERROR_WORD_16";
    const std::string PV_SUFFIX_ERROR_WORD_17 = "PV_SUFFIX_ERROR_WORD_17";
    const std::string PV_SUFFIX_ERROR_WORD_18 = "PV_SUFFIX_ERROR_WORD_18";
    const std::string PV_SUFFIX_ERROR_WORD_19 = "PV_SUFFIX_ERROR_WORD_19";
    const std::string PV_SUFFIX_ERROR_WORD_20 = "PV_SUFFIX_ERROR_WORD_20";
    const std::string PV_SUFFIX_ERROR_DESC_1  = "PV_SUFFIX_ERROR_DESC_1";
    const std::string PV_SUFFIX_ERROR_DESC_2  = "PV_SUFFIX_ERROR_DESC_2";
    const std::string PV_SUFFIX_ERROR_DESC_3  = "PV_SUFFIX_ERROR_DESC_3";
    const std::string PV_SUFFIX_ERROR_DESC_4  = "PV_SUFFIX_ERROR_DESC_4";
    const std::string PV_SUFFIX_ERROR_DESC_5  = "PV_SUFFIX_ERROR_DESC_5";
    const std::string PV_SUFFIX_ERROR_DESC_6  = "PV_SUFFIX_ERROR_DESC_6";
    const std::string PV_SUFFIX_ERROR_DESC_7  = "PV_SUFFIX_ERROR_DESC_7";
    const std::string PV_SUFFIX_ERROR_DESC_8  = "PV_SUFFIX_ERROR_DESC_8";
    const std::string PV_SUFFIX_ERROR_DESC_9  = "PV_SUFFIX_ERROR_DESC_9";
    const std::string PV_SUFFIX_ERROR_DESC_10 = "PV_SUFFIX_ERROR_DESC_10";
    const std::string PV_SUFFIX_ERROR_DESC_11 = "PV_SUFFIX_ERROR_DESC_11";
    const std::string PV_SUFFIX_ERROR_DESC_12 = "PV_SUFFIX_ERROR_DESC_12";
    const std::string PV_SUFFIX_ERROR_DESC_13 = "PV_SUFFIX_ERROR_DESC_13";
    const std::string PV_SUFFIX_ERROR_DESC_14 = "PV_SUFFIX_ERROR_DESC_14";
    const std::string PV_SUFFIX_ERROR_DESC_15 = "PV_SUFFIX_ERROR_DESC_15";
    const std::string PV_SUFFIX_ERROR_DESC_16 = "PV_SUFFIX_ERROR_DESC_16";
    const std::string PV_SUFFIX_ERROR_DESC_17 = "PV_SUFFIX_ERROR_DESC_17";
    const std::string PV_SUFFIX_ERROR_DESC_18 = "PV_SUFFIX_ERROR_DESC_18";
    const std::string PV_SUFFIX_ERROR_DESC_19 = "PV_SUFFIX_ERROR_DESC_19";
    const std::string PV_SUFFIX_ERROR_DESC_20 = "PV_SUFFIX_ERROR_DESC_20";


//
//    /// RF_GUN Keywords
//
//    const std::string RF_GUN_PyOW_OBJ_START  = "RF_GUN_PyOW_OBJ_START";
//    const std::string RF_GUN_MOD_OBJ_START  = "RF_GUN_MOD_OBJ_START";
//    const std::string RF_GUN_OBJ_START      = "RF_GUN_OBJ_START";
//    const std::string PV_PyOW_MONITORS_START = "PV_PyOW_MONITORS_START";
//
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_REV   = "RF_GUN_PyOW_PyV_SUFFIX_REV";
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_FWD   = "RF_GUN_PyOW_PyV_SUFFIX_FWD";
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_REVT  = "RF_GUN_PyOW_PyV_SUFFIX_REVT";
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_FWDT  = "RF_GUN_PyOW_PyV_SUFFIX_FWDT";
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_REVTM = "RF_GUN_PyOW_PyV_SUFFIX_REVTM";
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_FWDTM = "RF_GUN_PyOW_PyV_SUFFIX_FWDTM";
//    const std::string RF_GUN_PyOW_PyV_SUFFIX_RATIO = "RF_GUN_PyOW_PyV_SUFFIX_RATIO";
//
//    const std::string RFGUNPWR    = "RFGUNPWR";
//    const std::string RFGUNKLYPWR = "RFGUNKLYPWR";
//
//    const std::string PV_RF_GUN_MOD_STATESET  = "PV_RF_GUN_MOD_STATESET";
//    const std::string PV_RF_GUN_MOD_STATEREAD = "PV_RF_GUN_MOD_STATEREAD";
//    const std::string PV_RF_GUN_MOD_ERR_STATE  = "PV_RF_GUN_MOD_ERR_STATE";
//    const std::string PV_RF_GUN_MOD_WARMUPT   = "PV_RF_GUN_MOD_WARMUPT";
//    const std::string PV_RF_GUN_RESET     = "PV_RF_GUN_RESET";
//
//    const std::string REV_TRACE_SIZE = "REV_TRACE_SIZE";
//    const std::string FWD_TRACE_SIZE = "FWD_TRACE_SIZE";
//
//    const std::string PV_RF_GUN_FWD       = "PV_RF_GUN_FWD";
//    const std::string PV_RF_GUN_REV       = "PV_RF_GUN_REV";
//    const std::string PV_RF_GUN_KLY_FWD   = "PV_RF_GUN_KLY_FWD";
//    const std::string PV_RF_GUN_KLY_REV   = "PV_RF_GUN_KLY_REV";
//    const std::string PV_RF_GUN_PyHI       = "PV_RF_GUN_PyHI";
//    const std::string PV_RF_GUN_AMP_WRITE = "PV_RF_GUN_AMP_WRITE";
//    const std::string PV_RF_GUN_AMP_READ  = "PV_RF_GUN_AMP_READ";
//    const std::string PV_RF_GUN_RATIO     = "PV_RF_GUN_RATIO";
//    const std::string RF_GUN_SAFE_AMP     = "RF_GUN_SAFE_AMP";
//
//    /// MAGNETS
//
//    const std::string INJ_MAG_MONITORS_PyV_START = "INJ_MAG_MONITORS_PyV_START";
//    const std::string INJ_MAG_PyV_START          = "INJ_MAG_PyV_START";
//    const std::string NR_PyV_MONITORS_START      = "NR_PyV_MONITORS_START";
//    const std::string NR_PyV_START               = "NR_PyV_START";
//    const std::string INJ_MAG_PyV_SUFFIX_SI      = "INJ_MAG_PyV_SUFFIX_SI";
//    const std::string INJ_MAG_PyV_SUFFIX_RI      = "INJ_MAG_PyV_SUFFIX_RI";
//
//    const std::string MAG_TYPE   = "MAG_TYPE";
//

//
//    const std::string MAG_REV_TYPE            = "MAG_REV_TYPE";
//    const std::string MAG_REV_TYPE_NR         = "NR";
//    const std::string MAG_REV_TYPE_NR_BIPOLAR = "BIPOLAR";
//    const std::string MAG_REV_TYPE_NR_GANGED  = "NR_GANGED";
//    const std::string MAG_REV_TYPE_PyOS        = "POS";
//    const std::string PARENT_MAGNET           = "PARENT_MAGNET";
//    const std::string MAG_PySU_ROOT            = "MAG_PySU_ROOT";
//    const std::string MAG_GANG_MEMBER         = "MAG_GANG_MEMBER";
//
//    const std::string BSOL_DEGAUSS_VALUES  = "BSOL_DEGAUSS_VALUES";
//    const std::string DIP_DEGAUSS_VALUES   = "DIP_DEGAUSS_VALUES";
//    const std::string SOL_DEGAUSS_VALUES   = "SOL_DEGAUSS_VALUES";
//    const std::string QUAD_DEGAUSS_VALUES  = "QUAD_DEGAUSS_VALUES";
//
//    const std::string QUAD_DEGAUSS_TOLERANCE = "QUAD_DEGAUSS_TOLERANCE";
//    const std::string BSOL_DEGAUSS_TOLERANCE = "BSOL_DEGAUSS_TOLERANCE";
//    const std::string DIP_DEGAUSS_TOLERANCE  = "DIP_DEGAUSS_TOLERANCE";
//    const std::string SOL_DEGAUSS_TOLERANCE  = "SOL_DEGAUSS_TOLERANCE";

   //Cameras

    const std::string PV_DAQ_MONITORS_START = "PV_DAQ_MONITORS_START";
    const std::string PV_DAQ_COMMANDS_START = "PV_DAQ_COMMANDS_START";
    const std::string PV_IA_MONITORS_START = "PV_IA_MONITORS_START";
    const std::string PV_IA_COMMANDS_START = "PV_IA_COMMANDS_START";
    ///DAQ///
    const std::string PV_DAQ_SUFFIX_CAM_STATE       ="PV_DAQ_SUFFIX_CAM_STATE";
    const std::string PV_DAQ_SUFFIX_START_ACQUIRE   ="PV_DAQ_SUFFIX_START_ACQUIRE";
    const std::string PV_DAQ_SUFFIX_STOP_ACQUIRE    ="PV_DAQ_SUFFIX_STOP_ACQUIRE";
    const std::string PV_DAQ_SUFFIX_ACQUIRE_RBV     ="PV_DAQ_SUFFIX_ACQUIRE_RBV";


    const std::string PV_DAQ_SUFFIX_CAPTURE         ="PV_DAQ_SUFFIX_CAPTURE";
    const std::string PV_DAQ_SUFFIX_CAPTURE_RBV     ="PV_DAQ_SUFFIX_CAPTURE_RBV";
    const std::string PV_DAQ_SUFFIX_NUM_CAPTURE     ="PV_DAQ_SUFFIX_NUM_CAPTURE";
    const std::string PV_DAQ_SUFFIX_NUM_CAPTURE_RBV ="PV_DAQ_SUFFIX_NUM_CAPTURE_RBV";
    const std::string PV_DAQ_SUFFIX_NUM_CAPTURED    ="PV_DAQ_SUFFIX_NUM_CAPTURED";
    const std::string PV_DAQ_SUFFIX_WRITE_CHECK     ="PV_DAQ_SUFFIX_WRITE_CHECK";
    const std::string PV_DAQ_SUFFIX_WRITE_MESSAGE   ="PV_DAQ_SUFFIX_WRITE_MESSAGE";
    const std::string PV_DAQ_SUFFIX_WRITE           ="PV_DAQ_SUFFIX_WRITE";
    const std::string PV_DAQ_SUFFIX_WRITE_RBV       ="PV_DAQ_SUFFIX_WRITE_RBV";
    const std::string PV_DAQ_SUFFIX_DATA            ="PV_DAQ_SUFFIX_DATA";
    const std::string PV_DAQ_SUFFIX_BKGRND_DATA     ="PV_DAQ_SUFFIX_BKGRND_DATA";
    const std::string PV_DAQ_SUFFIX_FILE_PATH       ="PV_DAQ_SUFFIX_FILE_PATH";
    const std::string PV_DAQ_SUFFIX_FILE_NAME       ="PV_DAQ_SUFFIX_FILE_NAME";
    const std::string PV_DAQ_SUFFIX_FILE_TEMPLATE   ="PV_DAQ_SUFFIX_FILE_TEMPLATE";
    const std::string PV_DAQ_SUFFIX_FILE_NUMBER     ="PV_DAQ_SUFFIX_FILE_NUMBER";
    const std::string PV_DAQ_SUFFIX_EXPOSURE_RBV    ="PV_DAQ_SUFFIX_EXPOSURE_RBV";
    const std::string PV_DAQ_SUFFIX_ACQ_PERIOD_RBV  ="PV_DAQ_SUFFIX_ACQ_PERIOD_RBV";
    const std::string PV_DAQ_SUFFIX_FREQ_RBV        ="PV_DAQ_SUFFIX_FREQ_RBV";
    const std::string PV_DAQ_SUFFIX_SENSOR_TEMP_RBV ="PV_DAQ_SUFFIX_SENSOR_TEMP_RBV";

    const std::string PV_DAQ_SUFFIX_FILE_PATH_J       ="PV_DAQ_SUFFIX_FILE_PATH_J";
    const std::string PV_DAQ_SUFFIX_FILE_NAME_J     ="PV_DAQ_SUFFIX_FILE_NAME_J";
    const std::string PV_DAQ_SUFFIX_CAPTURE_J       ="PV_DAQ_SUFFIX_CAPTURE_J";
    const std::string PV_DAQ_SUFFIX_FILE_NUMBER_J   ="PV_DAQ_SUFFIX_FILE_NUMBER_J";
    const std::string PV_DAQ_SUFFIX_WRITE_J         ="PV_DAQ_SUFFIX_WRITE_J";
    const std::string PV_DAQ_SUFFIX_NUM_CAPTURE_J   ="PV_DAQ_SUFFIX_NUM_CAPTURE_J";

    const std::string PV_DAQ_SUFFIX_CAPTURE_RBV_J   ="PV_DAQ_SUFFIX_CAPTURE_RBV_J";
    const std::string PV_DAQ_SUFFIX_NUM_CAPTURED_J  ="PV_DAQ_SUFFIX_NUM_CAPTURED_J";
    const std::string PV_DAQ_SUFFIX_NUM_CAPTURE_RBV_J="PV_DAQ_SUFFIX_NUM_CAPTURE_RBV_J";
    const std::string PV_DAQ_SUFFIX_WRITE_CHECK_J   ="PV_DAQ_SUFFIX_WRITE_CHECK_J";
    const std::string PV_DAQ_SUFFIX_WRITE_RBV_J     ="PV_DAQ_SUFFIX_WRITE_RBV_J";
    const std::string PV_DAQ_SUFFIX_WRITE_MESSAGE_J ="PV_DAQ_SUFFIX_WRITE_MESSAGE_J";

    ///IA///
    const std::string PV_IA_SUFFIX_CAM_STATE        ="PV_IA_SUFFIX_CAM_STATE";
    const std::string PV_IA_SUFFIX_START_ACQUIRE    ="PV_IA_SUFFIX_START_ACQUIRE";
    const std::string PV_IA_SUFFIX_STOP_ACQUIRE     ="PV_IA_SUFFIX_STOP_ACQUIRE";
    const std::string PV_IA_SUFFIX_ACQUIRE_RBV      ="PV_IA_SUFFIX_ACQUIRE_RBV";
    const std::string PV_IA_SUFFIX_START_IA         ="PV_IA_SUFFIX_START_IA";
    const std::string PV_IA_SUFFIX_SET_B            ="PV_IA_SUFFIX_SET_B";
    const std::string PV_IA_SUFFIX_USE_B            ="PV_IA_SUFFIX_USE_B";
    const std::string PV_IA_SUFFIX_USE_NPOINT       ="PV_IA_SUFFIX_USE_NPOINT";

    const std::string PV_IA_SUFFIX_START_IA_RBV       ="PV_IA_SUFFIX_START_IA_RBV";
    const std::string PV_IA_SUFFIX_USE_B_RBV       ="PV_IA_SUFFIX_USE_B_RBV";
    const std::string PV_IA_SUFFIX_USE_NPOINT_RBV       ="PV_IA_SUFFIX_USE_NPOINT_RBV";
    const std::string PV_IA_SUFFIX_CENTER_X_RBV         ="PV_IA_SUFFIX_CENTER_X_RBV";
    const std::string PV_IA_SUFFIX_CENTER_Y_RBV         ="PV_IA_SUFFIX_CENTER_Y_RBV";
    const std::string PV_IA_SUFFIX_MASK_X_RBV           ="PV_IA_SUFFIX_MASK_X_RBV";
    const std::string PV_IA_SUFFIX_MASK_Y_RBV           ="PV_IA_SUFFIX_MASK_Y_RBV";
    const std::string PV_IA_SUFFIX_MASK_X_RAD_RBV       ="PV_IA_SUFFIX_MASK_X_RAD_RBV";
    const std::string PV_IA_SUFFIX_MASK_Y_RAD_RBV       ="PV_IA_SUFFIX_MASK_Y_RAD_RBV";

    const std::string PV_IA_SUFFIX_CENTER_X         ="PV_IA_SUFFIX_CENTER_X";
    const std::string PV_IA_SUFFIX_CENTER_Y         ="PV_IA_SUFFIX_CENTER_Y";
    const std::string PV_IA_SUFFIX_MASK_X           ="PV_IA_SUFFIX_MASK_X";
    const std::string PV_IA_SUFFIX_MASK_Y           ="PV_IA_SUFFIX_MASK_Y";
    const std::string PV_IA_SUFFIX_MASK_X_RAD       ="PV_IA_SUFFIX_MASK_X_RAD";
    const std::string PV_IA_SUFFIX_MASK_Y_RAD       ="PV_IA_SUFFIX_MASK_Y_RAD";
    const std::string PV_IA_SUFFIX_PIX_MM           ="PV_IA_SUFFIX_PIX_MM";
    const std::string PV_IA_SUFFIX_STEP_SIZE        ="PV_IA_SUFFIX_STEP_SIZE";

    const std::string PV_IA_SUFFIX_X                ="PV_IA_SUFFIX_X";
    const std::string PV_IA_SUFFIX_Y                ="PV_IA_SUFFIX_Y" ;
    const std::string PV_IA_SUFFIX_SIGMA_X          ="PV_IA_SUFFIX_SIGMA_X";
    const std::string PV_IA_SUFFIX_SIGMA_Y          ="PV_IA_SUFFIX_SIGMA_Y";
    const std::string PV_IA_SUFFIX_COV_XY           ="PV_IA_SUFFIX_COV_XY";
    const std::string PV_IA_SUFFIX_X_PIX            ="PV_IA_SUFFIX_X_PIX";
    const std::string PV_IA_SUFFIX_Y_PIX            ="PV_IA_SUFFIX_Y_PIX";
    const std::string PV_IA_SUFFIX_SIGMA_X_PIX      ="PV_IA_SUFFIX_SIGMA_X_PIX";
    const std::string PV_IA_SUFFIX_SIGMA_Y_PIX      ="PV_IA_SUFFIX_SIGMA_Y_PIX";
    const std::string PV_IA_SUFFIX_COV_XY_PIX       ="PV_IA_SUFFIX_COV_XY_PIX";
    const std::string PV_IA_SUFFIX_SUM_PIX_INTENSITY="PV_IA_SUFFIX_SUM_PIX_INTENSITY";

    // Camera Object Componentns
    const std::string SCREEN_NAME               ="SCREEN_NAME";
    const std::string X_MASK_RAD                ="X_MASK_RAD";
    const std::string Y_MASK_RAD                ="Y_MASK_RAD";
    const std::string X_MASK                    ="X_MASK";
    const std::string Y_MASK                    ="Y_MASK";
    const std::string X_CENTER                  ="X_CENTER";
    const std::string Y_CENTER                  ="Y_CENTER";
    const std::string PIX_2_MM_RATIO            ="PIX_2_MM_RATIO";
    const std::string MAX_SHOTS_NUMBER          ="MAX_SHOTS_NUMBER";
    const std::string IP_ADDRESS_STREAM         ="IP_ADDRESS_STREAM";

    /// SCREENS (VELA_PNEUMATIC_SCREEN and VELA_HV_MOVER_SCREEN)
    const std::string VELA_PNEUMATIC  =  "VELA_PNEUMATIC";
    const std::string CLARA_PNEUMATIC =  "CLARA_PNEUMATIC";
    const std::string VELA_HV_MOVER   =  "VELA_HV_MOVER";
    const std::string CLARA_HV_MOVER  =  "CLARA_HV_MOVER";
    const std::string CLARA_V_MOVER   =  "CLARA_V_MOVER";
    const std::string SCREEN_TYPE     =  "SCREEN_TYPE";
    /// SCREENS Update 8/1/18
    const std::string PV_SUFFIX_H_MOVING    = "PV_SUFFIX_H_MOVING";
    const std::string PV_SUFFIX_H_READY     = "PV_SUFFIX_H_READY";
    const std::string PV_SUFFIX_H_GET_DEV   = "PV_SUFFIX_H_GET_DEV";
    const std::string PV_SUFFIX_H_DEV_STATE = "PV_SUFFIX_H_DEV_STATE";
    const std::string PV_SUFFIX_H_MAX_POS   = "PV_SUFFIX_H_H_MAX_POS";
    const std::string PV_SUFFIX_H_DEV_CENT  = "PV_SUFFIX_H_DEV_CENT";
    const std::string PV_SUFFIX_H_ACTPOS    = "PV_SUFFIX_H_ACTPOS";
    const std::string PV_SUFFIX_H_EN        = "PV_SUFFIX_H_EN";
    const std::string PV_SUFFIX_H_SDEV      = "PV_SUFFIX_H_SDEV";
    const std::string PV_SUFFIX_H_TRIGGER   = "PV_SUFFIX_H_TRIGGER";
    const std::string PV_SUFFIX_H_EX        = "PV_SUFFIX_H_EX";
    const std::string PV_SUFFIX_H_TGTPOS    = "PV_SUFFIX_H_TGTPOS";
    const std::string PV_SUFFIX_H_JOG       = "PV_SUFFIX_H_JOG";
    const std::string PV_SUFFIX_H_JDIFF     = "PV_SUFFIX_H_JDIFF";
    const std::string PV_SUFFIX_V_MOVING    = "PV_SUFFIX_V_MOVING";
    const std::string PV_SUFFIX_V_READY     = "PV_SUFFIX_V_READY";
    const std::string PV_SUFFIX_V_GET_DEV   = "PV_SUFFIX_V_GET_DEV";
    const std::string PV_SUFFIX_V_DEV_STATE = "PV_SUFFIX_V_DEV_STATE";
    const std::string PV_SUFFIX_V_MAX_POS   = "PV_SUFFIX_V_H_MAX_POS";
    const std::string PV_SUFFIX_V_DEV_CENT  = "PV_SUFFIX_V_DEV_CENT";
    const std::string PV_SUFFIX_V_ACTPOS    = "PV_SUFFIX_V_ACTPOS";
    const std::string PV_SUFFIX_V_EN        = "PV_SUFFIX_V_EN";
    const std::string PV_SUFFIX_V_SDEV      = "PV_SUFFIX_V_SDEV";
    const std::string PV_SUFFIX_V_TRIGGER   = "PV_SUFFIX_V_TRIGGER";
    const std::string PV_SUFFIX_V_EX        = "PV_SUFFIX_V_EX";
    const std::string PV_SUFFIX_V_TGTPOS    = "PV_SUFFIX_V_TGTPOS";
    const std::string PV_SUFFIX_V_JOG       = "PV_SUFFIX_V_JOG";
    const std::string PV_SUFFIX_V_JDIFF     = "PV_SUFFIX_V_JDIFF";
    const std::string PV_SUFFIX_SDEV        = "PV_SUFFIX_SDEV";
    const std::string PV_SUFFIX_MOVING      = "PV_SUFFIX_MOVING";
    const std::string PV_SUFFIX_READY       = "PV_SUFFIX_READY";
    const std::string PV_SUFFIX_DEV_STATE   = "PV_SUFFIX_DEV_STATE";
    const std::string PV_SUFFIX_GET_DEV     = "PV_SUFFIX_GET_DEV";
    const std::string PV_SUFFIX_DEV_CENT    = "PV_SUFFIX_DEV_CENT";
    const std::string PV_SUFFIX_TRIGGER     = "PV_SUFFIX_TRIGGER";
    const std::string PV_SUFFIX_MAX_POS     = "PV_SUFFIX_MAX_POS";
    const std::string PV_SUFFIX_ACTPOS      = "PV_SUFFIX_ACTPOS";
    const std::string PV_SUFFIX_EN          = "PV_SUFFIX_EN";
    /// SCREEN YAG 1/2/3 COMMAND PVs
    const std::string PV_SUFFIX_H_RETRACTED = "PV_SUFFIX_H_RETRACTED";
    const std::string PV_SUFFIX_H_SLIT_1    = "PV_SUFFIX_H_SLIT_1";
    const std::string PV_SUFFIX_H_SLIT_2    = "PV_SUFFIX_H_SLIT_2";
    const std::string PV_SUFFIX_H_SLIT_3    = "PV_SUFFIX_H_SLIT_3";
    const std::string PV_SUFFIX_H_APT_1     = "PV_SUFFIX_H_APT_1";
    const std::string PV_SUFFIX_H_APT_2     = "PV_SUFFIX_H_APT_2";
    const std::string PV_SUFFIX_H_APT_3     = "PV_SUFFIX_H_APT_3";
    const std::string PV_SUFFIX_V_RETRACTED = "PV_SUFFIX_V_RETRACTED";
    const std::string PV_SUFFIX_V_MAX       = "PV_SUFFIX_V_MAX";
    const std::string PV_SUFFIX_V_RF        = "PV_SUFFIX_V_RF";
    const std::string PV_SUFFIX_V_MIRROR    = "PV_SUFFIX_V_MIRROR";
    const std::string PV_SUFFIX_V_YAG       = "PV_SUFFIX_V_YAG";
    const std::string PV_SUFFIX_V_GRAT      = "PV_SUFFIX_V_GRAT";
    const std::string PV_SUFFIX_V_COL       = "PV_SUFFIX_V_COL";

    const std::string H_RETRACTED = "H_RETRACTED";
    const std::string H_SLIT_1    = "H_SLIT_1";
    const std::string H_SLIT_2    = "H_SLIT_2";
    const std::string H_SLIT_3    = "H_SLIT_3";
    const std::string H_APT_1     = "H_APT_1";
    const std::string H_APT_2     = "H_APT_2";
    const std::string H_APT_3     = "H_APT_3";
    const std::string V_RETRACTED = "V_RETRACTED";
    const std::string V_MAX       = "V_MAX";
    const std::string V_SLIT_1    = "V_SLIT_1";
    const std::string V_RF        = "V_RF";
    const std::string V_MIRROR    = "V_MIRROR";
    const std::string V_YAG       = "V_YAG";
    const std::string V_GRAT      = "V_GRAT";
    const std::string V_COL       = "V_COL";
    const std::string H_RETRACTED_POS = "H_RETRACTED_POS";
    const std::string H_SLIT_1_POS    = "H_SLIT_1_POS";
    const std::string H_SLIT_2_POS    = "H_SLIT_2_POS";
    const std::string H_SLIT_3_POS    = "H_SLIT_3_POS";
    const std::string H_APT_1_POS     = "H_APT_1_POS";
    const std::string H_APT_2_POS     = "H_APT_2_POS";
    const std::string H_APT_3_POS     = "H_APT_3_POS";
    const std::string V_RETRACTED_POS = "V_RETRACTED_POS";
    const std::string V_MAX_POS       = "V_MAX_POS";
    const std::string V_SLIT_1_POS    = "V_SLIT_1_POS";
    const std::string V_RF_POS        = "V_RF_POS";
    const std::string V_MIRROR_POS    = "V_MIRROR_POS";
    const std::string V_YAG_POS       = "V_YAG_POS";
    const std::string V_GRAT_POS      = "V_GRAT_POS";
    const std::string V_COL_POS       = "V_COL_POS";
    const std::string YAG             = "YAG";
    const std::string RETRACTED       = "RETRACTED";
    // SCREEN YAG 1/2/3 COMMAND PVs
    const std::string PV_SUFFIX_H_MABS = "PV_SUFFIX_H_MABS";
    const std::string PV_SUFFIX_V_MABS = "PV_SUFFIX_V_MABS";
    const std::string PV_SUFFIX_STOP   = "PV_SUFFIX_STOP";
//    const std::string PV_SUFFIX_H_ACTPOS   = "PV_SUFFIX_H_ACTPOS";
//    const std::string PV_SUFFIX_V_ACTPOS   = "PV_SUFFIX_V_ACTPOS";
    //SCREEN YAG 1/2/3 MONITOR PVs
    const std::string PV_SUFFIX_H_RPWRLOSS = "PV_SUFFIX_H_RPWRLOSS";
    const std::string PV_SUFFIX_V_RPWRLOSS = "PV_SUFFIX_V_RPWRLOSS";
    const std::string PV_SUFFIX_H_PROT01   = "PV_SUFFIX_H_PROT01";
    const std::string PV_SUFFIX_V_PROT01   = "PV_SUFFIX_V_PROT01";
    const std::string PV_SUFFIX_PROT03     = "PV_SUFFIX_PROT03";
    const std::string PV_SUFFIX_PROT05     = "PV_SUFFIX_PROT05";
    const std::string PV_SUFFIX_H_RPOS     = "PV_SUFFIX_H_RPOS";
    const std::string PV_SUFFIX_V_RPOS     = "PV_SUFFIX_V_RPOS";
    const std::string PV_SUFFIX_H_STA      = "PV_SUFFIX_H_STA";
    const std::string PV_SUFFIX_V_STA      = "PV_SUFFIX_V_STA";
    const std::string PV_SUFFIX_H_POS      = "PV_SUFFIX_H_POS";
    const std::string PV_SUFFIX_V_POS      = "PV_SUFFIX_V_POS";
    // other screen config file parameters
    // defintiions of each bit for the STA int
    const std::string SCREEN_DRIVER_POS_TOLERANCE = "SCREEN_DRIVER_POS_TOLERANCE";
    const std::string EXCESSIVE_POSITION_ERROR    = "EXCESSIVE_POSITION_ERROR";
    const std::string PROGRAMME_CHECKSUM_ERROR    = "PROGRAMME_CHECKSUM_ERROR";
    const std::string HISTORICAL_POS_HW_LIMIT     = "HISTORICAL_POS_HW_LIMIT";
    const std::string HISTORICAL_NEG_HW_LIMIT     = "HISTORICAL_NEG_HW_LIMIT";
    const std::string TRAJECTORY_IN_PROGRESS      = "TRAJECTORY_IN_PROGRESS";
    const std::string POS_HW_LIMIT_ASSERTED       = "POS_HW_LIMIT_ASSERTED";
    const std::string NEG_HW_LIMIT_ASSERTED       = "NEG_HW_LIMIT_ASSERTED";
    const std::string INDEX_REPORT_AVALABLE       = "INDEX_REPORT_AVALABLE";
    const std::string WRAPAROUND_OCCURRED         = "WRAPAROUND_OCCURRED";
    const std::string TEMPERATURE_FAULT           = "TEMPERATURE_FAULT";
    const std::string MATHS_OVERFLOW              = "MATHS_OVERFLOW";
    const std::string NUM_STA_BITS                = "NUM_STA_BITS";
    const std::string MOTOR_IS_OFF                = "MOTOR_IS_OFF";
    const std::string SYNTAX_ERROR                = "SYNTAX_ERROR";
    const std::string INDEX_INPUT                 = "INDEX_INPUT";
    const std::string OVER_CURRENT                = "OVER_CURRENT";
    const std::string INDEX_ERROR                 = "INDEX_ERROR";
    const std::string STA_BIT                     = "STA_BIT";
    // master list of everything that can go on a screen driver cassette
    // probably should try and  get rid of this
//    const std::vector<std::string> allScreenCassetteElements = { MIRROR_POS, SLIT_50_UM_POS, SLIT_25_UM_POS,
//                                                                   HOLE_6p3_MM_POS, HOLE_10_MM_POS, YAG_POS, RF_POS,
//                                                                   SLIT_POS, OUT_POS };

#ifdef BUILD_DLL
    const boost::python::arg NAME_ARG   = boost::python::arg("name");
    const boost::python::arg NAMES_ARG  = boost::python::arg("names");
    const boost::python::arg VALUE_ARG  = boost::python::arg("value");
    const boost::python::arg VALUES_ARG = boost::python::arg("values");
#endif //BUILD_DLL


}
#endif //CONFIG_DEFINITIONS_H
