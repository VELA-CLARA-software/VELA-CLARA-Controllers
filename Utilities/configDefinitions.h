#ifndef CONFIG_DEFINITIONS_H
#define CONFIG_DEFINITIONS_H

#include <string>

namespace UTL
{
    const double DUMMY_DOUBLE = -999.999;
    const double ZERO_DOUBLE = 0.0;
    const int    ZERO_INT = 0;
    const size_t ZERO_SIZET = 0;
    const long   DUMMY_LONG   = -999;
    const std::string  UNKNOWN_STRING = "UNKNOWN";
    const std::string  UNKNOWN_PVROOT = "UNKNOWN_PVROOT";
    const std::string  UNKNOWN_NAME   = "UNKNOWN_NAME";


    const std::string END_OF_LINE = ";";
    const std::string SLASH_SLASH = "\\";
    const std::string EQUALS_SIGN = "=";
    const char EQUALS_SIGN_C      = '=';
    const char COLON_C            = ':';
    const char DOUBLE_QUOTE_C     = '"';
    const char SPACE_C            = ' ';
    const char TAB_C              = '\t';

    const std::string CONFIG_PATH = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Software\\VELA_CLARA_PYDs\\Config\\";
    //const std::string CONFIG_PATH_VM = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Software\\VELA_CLARA_PYDs\\Config\\VM\\";
    const std::string VM_PREFIX   = "VM-";

    const std::string VELA_PIL_SHUTTER_CONFIG          = "velaPhotoInjectorLaserShutter.config";
    const std::string VELA_IMG_CONFIG                  = "velaInvertedMagnetronGauge.config";
    const std::string VELA_VALVE_CONFIG                = "velaVacuumValve.config";
    const std::string VELA_QBOX_CONFIG                 = "qbox.config";
    const std::string VELA_INJ_BPM_CONFIG              = "velaINJBeamPositionMonitors.config";
    const std::string VELA_BA1_BPM_CONFIG              = "velaBA1BeamPositionMonitors.config";
    const std::string VELA_BA2_BPM_CONFIG              = "velaBA2BeamPositionMonitors.config";
    const std::string CLARA_INJ_BPM_CONFIG             = "claraINJBeamPositionMonitors.config";
    const std::string CLARA_2_VELA_BPM_CONFIG          = "clara2VELABeamPositionMonitors.config";
/// Qscope
    const std::string VELA_SCOPE_CONFIG                = "velaQScope.config";
    const std::string VELA_SCOPE_TRACE_CONFIG          = "velaQScopeTrace.config";
/// VELA Gun
    const std::string VELA_RF_GUN_PWR_CONFIG           = "velaRFGunPower.config";
    const std::string VELA_RF_GUN_MOD_CONFIG           = "velaRFGunModulator.config";
    const std::string VELA_RF_GUN_LLRF_CONFIG          = "velaRFGunLLRF.config";
/// Magnets
    const std::string VELA_INJ_MAG_NR_PSU_CONFIG       = "velaINJMagnetsNRpsu.config";
    const std::string VELA_BA1_MAG_NR_PSU_CONFIG       = "velaBA1MagnetsNRpsu.config";
    const std::string VELA_BA2_MAG_NR_PSU_CONFIG       = "velaBA2MagnetsNRpsu.config";
    const std::string CLARA_INJ_MAG_NR_PSU_CONFIG      = "claraINJMagnetsNRpsu.config";
    const std::string VELA_INJ_MAG_CONFIG              = "velaINJMagnets.config";
    const std::string VELA_BA1_MAG_CONFIG              = "velaBA1Magnets.config";
    const std::string VELA_BA2_MAG_CONFIG              = "velaBA2Magnets.config";
    const std::string CLARA_INJ_MAG_CONFIG             = "claraINJMagnets.config";
/// Screens
    const std::string VELA_INJ_SCREENS_CONFIG  = "velaINJScreens.config";

// the below has to go
    const std::string VELA_INJ_COMPLEX_SCREENS_CONFIG  = "velaINJComplexScreens.config";
    const std::string VELA_INJ_SIMPLE_SCREENS_CONFIG   = "velaINJSimpleScreens.config";
    const std::string VELA_BA2_COMPLEX_SCREENS_CONFIG  = "velaBA2ComplexScreens.config";
    const std::string VELA_BA2_SIMPLE_SCREENS_CONFIG   = "velaBA2SimpleScreens.config";
    const std::string VELA_BA1_COMPLEX_SCREENS_CONFIG  = "velaBA1ComplexScreens.config";
    const std::string VELA_BA1_SIMPLE_SCREENS_CONFIG   = "velaBA1SimpleScreens.config";
    const std::string CLARA_INJ_SIMPLE_SCREENS_CONFIG  = "claraINJSimpleScreens.config";
    const std::string CLARA_INJ_COMPLEX_SCREENS_CONFIG = "claraINJComplexScreens.config";

/// Cameras
    const std::string VELA_CAMERA_CONFIG               = "velaCamera.config";
    const std::string VELA_CAMERA_IOC_CONFIG           = "velaCameraIOC.config";
    /// These are the keywords used in hardware config files

    /// General Keywords

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

    // PVs
    const std::string PV_SUFFIX_ON          = "PV_SUFFIX_ON";
    const std::string PV_SUFFIX_OFF         = "PV_SUFFIX_OFF";
    const std::string PV_SUFFIX_STA         = "PV_SUFFIX_STA";
    const std::string NUMBER_OF_INTERLOCKS  = "NUMBER_OF_INTERLOCKS";

    const std::string PV_COUNT  = "PV_COUNT" ;
    const std::string PV_CHTYPE = "PV_CHTYPE";
    const std::string PV_MASK   = "PV_MASK"  ;

    // RF POWER
    const std::string PV_SUFFIX_REV   = "PV_SUFFIX_REV";
    const std::string PV_SUFFIX_FWD   = "PV_SUFFIX_FWD";
    const std::string PV_SUFFIX_REVT  = "PV_SUFFIX_REVT";
    const std::string PV_SUFFIX_FWDT  = "PV_SUFFIX_FWDT";
    const std::string PV_SUFFIX_REVTM = "PV_SUFFIX_REVTM";
    const std::string PV_SUFFIX_FWDTM = "PV_SUFFIX_FWDTM";
    const std::string PV_SUFFIX_RATIO = "PV_SUFFIX_RATIO";

    // QBOX
    const std::string PV_QBOX_STATE    = "PV_QBOX_STATE";
    const std::string PV_QBOX_SETSTATE = "PV_QBOX_SETSTATE";

    // Vac
    const std::string PV_VAC_PRESSURE = "PV_VAC_PRESSURE";

    // Scope
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

    // BPM
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
    const std::string ATT1CAL               = "ATT1CAL";
    const std::string ATT2CAL               = "ATT2CAL";
    const std::string V1CAL                 = "V1CAL";
    const std::string V2CAL                 = "V2CAL";
    const std::string QCAL                  = "QCAL";
    const std::string MN                    = "MN";
    const std::string XN                    = "XN";
    const std::string YN                    = "YN";

    // LLRF
    const std::string PV_SUFFIX_AMPR = "PV_SUFFIX_AMPR";
    const std::string PV_SUFFIX_PHI  = "PV_SUFFIX_PHI";
    const std::string PV_SUFFIX_AMPW = "PV_SUFFIX_AMPW";

    // Modulator
    const std::string PV_SUFFIX_RESET     = "PV_SUFFIX_RESET";
    const std::string PV_SUFFIX_STATESET  = "PV_SUFFIX_STATESET";
    const std::string PV_SUFFIX_STATEREAD = "PV_SUFFIX_STATEREAD";
    const std::string PV_SUFFIX_EXILOCK1  = "PV_SUFFIX_EXILOCK1";
    const std::string PV_SUFFIX_WARMUPT   = "PV_SUFFIX_WARMUPT";

    // NR-PSU
    const std::string PARENT_MAGNET = "PARENT_MAGNET";
    const std::string MAG_GANG_MEMBER = "MAG_GANG_MEMBER";
    const std::string PV_ROOT_N   = "PV_ROOT_N";
    const std::string PV_ROOT_R   = "PV_ROOT_R";
    // Magnet

    const std::string UNKNOWN_MAGNET_STRING = "UNKNOWN_MAGNET";
    const std::string PV_SUFFIX_RI = "PV_SUFFIX_RI";
    const std::string PV_SUFFIX_SI = "PV_SUFFIX_SI";
    const std::string PV_PSU_ROOT  = "PV_PSU_ROOT";

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

    // chtypes

     const std::string DBR_STRING_STR = "DBR_STRING";
     const std::string DBR_INT_STR	  = "DBR_INT";
     const std::string DBR_SHORT_STR  = "DBR_SHORT";
     const std::string DBR_FLOAT_STR  = "DBR_FLOAT";
     const std::string DBR_ENUM_STR	  = "DBR_ENUM";
     const std::string DBR_CHAR_STR	  = "DBR_CHAR"  ;
     const std::string DBR_LONG_STR	  = "DBR_LONG"  ;
     const std::string DBR_DOUBLE_STR = "DBR_DOUBLE";
     const std::string DBR_TIME_STRING_STR ="DBR_TIME_STRING";
     const std::string DBR_TIME_INT_STR	   ="DBR_TIME_INT";
     const std::string DBR_TIME_SHORT_STR  ="DBR_TIME_SHORT";
     const std::string DBR_TIME_FLOAT_STR  ="DBR_TIME_FLOAT";
     const std::string DBR_TIME_ENUM_STR   ="DBR_TIME_ENUM";
     const std::string DBR_TIME_CHAR_STR	  ="DBR_TIME_CHAR"  ;
     const std::string DBR_TIME_LONG_STR	  ="DBR_TIME_LONG"  ;
     const std::string DBR_TIME_DOUBLE_STR ="DBR_TIME_DOUBLE";

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
//    const std::string PV_RF_GUN_MOD_EXILOCK1  = "PV_RF_GUN_MOD_EXILOCK1";
//    const std::string PV_RF_GUN_MOD_WARMUPT   = "PV_RF_GUN_MOD_WARMUPT";
//    const std::string PV_RF_GUN_MOD_RESET     = "PV_RF_GUN_MOD_RESET";
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
    const std::string PV_SUFFIX_START     ="PV_SUFFIX_START"    ;
    const std::string PV_SUFFIX_GAIN      ="PV_SUFFIX_GAIN"     ;
    const std::string PV_SUFFIX_BLACK     ="PV_SUFFIX_BLACK"    ;
    const std::string PV_SUFFIX_DATA      ="PV_SUFFIX_DATA"     ;
    const std::string PV_SUFFIX_GAIN_RBV  ="PV_SUFFIX_GAIN_RBV" ;
    const std::string PV_SUFFIX_BLACK_RBV ="PV_SUFFIX_BLACK_RBV";
    const std::string PV_SUFFIX_CAM_COUNT ="PV_SUFFIX_CAM_COUNT";
    const std::string PV_SUFFIX_CAM_STATE ="PV_SUFFIX_CAM_STATE";
    const std::string PV_SUFFIX_CAM_LIMIT ="PV_SUFFIX_CAM_LIMIT";
    const std::string SCREEN        =  "SCREEN";
    const std::string X0            =  "X0";
    const std::string Y0            =  "Y0";
    const std::string XRAD          =  "XRAD";
    const std::string YRAD          =  "YRAD";
    const std::string X_PIX_2_MM    =  "X_PIX_2_MM";
    const std::string Y_PIX_2_MM    =  "Y_PIX_2_MM";

    /// SCREEN YAG 1/2/3 COMMAND PVs

    const std::string PV_SUFFIX_H_MABS      = "PV_SUFFIX_H_MABS";
    const std::string PV_SUFFIX_V_MABS      = "PV_SUFFIX_V_MABS";
    const std::string PV_SUFFIX_STOP        = "PV_SUFFIX_STOP";

    ///SCREEN YAG 1/2/3 MONITOR PVs

    const std::string PV_SUFFIX_H_PROT01    = "PV_SUFFIX_H_PROT01";
    const std::string PV_SUFFIX_V_PROT01    = "PV_SUFFIX_V_PROT01";
    const std::string PV_SUFFIX_PROT03      = "PV_SUFFIX_PROT03";
    const std::string PV_SUFFIX_PROT05      = "PV_SUFFIX_PROT05";
    const std::string PV_SUFFIX_H_RPWRLOSS  = "PV_SUFFIX_H_RPWRLOSS";
    const std::string PV_SUFFIX_V_RPWRLOSS  = "PV_SUFFIX_V_RPWRLOSS";

    const std::string PV_SUFFIX_H_RPOS      = "PV_SUFFIX_H_RPOS";
    const std::string PV_SUFFIX_V_RPOS      = "PV_SUFFIX_V_RPOS";

    /// SCREEN YAG 1/2/3 POSITIONS

    const std::string H_MIRROR_POS      = "H_MIRROR_POS";
    const std::string H_50U_SLIT_POS    = "H_50U_SLIT_POS";
    const std::string H_25U_SLIT_POS    = "H_25U_SLIT_POS";
    const std::string H_63MM_HOLE_POS    = "H_63MM_HOLE_POS";
    const std::string H_10MM_HOLE_POS   = "H_10MM_HOLE_POS";
    const std::string V_YAG_POS         = "V_YAG_POS";
    const std::string V_SLIT_POS        = "V_SLIT_POS";
    const std::string H_SLIT_POS        = "H_SLIT_POS";
    const std::string H_OUT_POS         = "H_OUT_POS";
    const std::string V_OUT_POS         = "V_OUT_POS";
    const std::string OUT           = "OUT";
    const std::string IN            = "IN";


}
#endif //CONFIG_DEFINITIONS_H
