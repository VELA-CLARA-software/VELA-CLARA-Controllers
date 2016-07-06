#ifndef _VELA_SCREEN_STRUCTS_H_
#define _VELA_SCREEN_STRUCTS_H_

//
#include "structs.h"
//stl
#include <string>
#include <map>

//epics
#ifndef __CINT__
#include <cadef.h>
#endif // __CINT__


                                                                        \

class screenInterface;

namespace screenStructs
{


    ///Forward declare structs, gcc seems to like this...
    struct pvStruct;
    struct screenObject;
    struct monitorStruct;
    struct COMPLEX_YAG_Object;
    struct SIMPLE_YAG_Object;



    ///Use this MACRO to define enums. COnsider putting ENUMS that are more 'global' in structs.h

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_STATE, (SCREEN_IN) (SCREEN_OUT) (SCREEN_ERROR) (SCREEN_UNKNOWN) (SCREEN_H_MIRROR) (SCREEN_H_50U_SLIT) (SCREEN_H_25U_SLIT)
                                        (SCREEN_H_63MM_HOLE) (SCREEN_H_10MM_HOLE) (SCREEN_H_YAG)(SCREEN_V_YAG) (SCREEN_V_SLIT) (SCREEN_MOVING)  )


    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_PV_TYPE, (Sta) (On) (Off) (H_MABS) (V_MABS) (H_RPOS) (V_RPOS) (STOP) (H_PROT01) (V_PROT01)
                                        (PROT03) (PROT05) (H_RPWRLOSS) (V_RPWRLOSS) )

//    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_POSITION, (YAG_01_V_YAG) (YAG_01_V_SLIT) (YAG_01_H_MIRROR) (YAG_01_H_50U_SLIT)
//                                        (YAG_01_H_25U_SLIT) (YAG_01_H_63MMHOLE) (YAG_01_H_10MMHOLE) (YAG_02_V_YAG) (YAG_02_V_SLIT)
//                                        (YAG_02_H_SLIT) (YAG_03_V_YAG) (YAG_03_H_SLIT) )

    /// monType could be used to switch in the statisCallbackFunction


    struct monitorStruct
    {
        SCREEN_PV_TYPE monType;
        COMPLEX_YAG_Object * compObj;
        SIMPLE_YAG_Object * simpObj;
        chtype      CHTYPE;
        screenInterface * interface;

#ifndef __CINT__
 //       evid EventID;
     #endif // __CINT__

    };

    ///The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    ///The rest are parameters passed to EPICS, ca_create_channel, ca_create_subscription etc...

    struct pvStruct
    {
        SCREEN_PV_TYPE pvType;
        chid           CHID;
        std::string    pvSuffix;
        unsigned long  COUNT, MASK;
        chtype         CHTYPE;
        evid           EVID;
        unsigned long  value;
    };


    /// The main hardware object holds


    struct SIMPLE_YAG_Object ///remaining YAG object
    {
        SIMPLE_YAG_Object() : IN(9999.9999), OUT(9999.9999), position_value(9999.99) {}
        std::string name, pvRoot;
        double position_value;
        double IN, OUT;
        screenStructs::SCREEN_STATE screenState;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;

#ifndef __CINT__ /// Channel IDs
        /// keep data seperate from epics stuff
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs;
        std::map< SCREEN_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
#endif // __CINT__

    };


    struct COMPLEX_YAG_Object
    {
        COMPLEX_YAG_Object() : H_MIRROR(9999.9999),H_50U_SLIT(9999.9999),H_25U_SLIT(9999.9999), H_63MM_HOLE(9999.9999),
        H_10MM_HOLE(9999.9999), V_YAG(9999.9999),V_SLIT(9999.9999), H_SLIT(9999.9999), H_OUT(9999.9999),V_OUT(9999.9999),
        h_position_value(9999.9999), v_position_value(9999.9999), vertical_disabled(true), horizontal_disabled(true),
        position_error(true), home_error(true), H_RPWRLOSS(9999), V_RPWRLOSS(9999) {}
        std::string name, pvRoot;
        double h_position_value, v_position_value;
        double H_MIRROR, H_50U_SLIT, H_25U_SLIT, H_63MM_HOLE, H_10MM_HOLE, V_YAG, V_SLIT, H_SLIT, H_OUT, V_OUT;
        screenStructs::SCREEN_STATE h_screenState;
        screenStructs::SCREEN_STATE v_screenState;
        bool vertical_disabled, horizontal_disabled;
        bool  position_error, home_error;
        int H_RPWRLOSS, V_RPWRLOSS;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > iLockStates;

#ifndef __CINT__ ///Channel IDs
        /// keep data sepeate from epics stuff
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs;
        std::map< SCREEN_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
#endif // __CINT__

    };


    struct screenObject
    {
        std::string name;
        std::map< std::string, COMPLEX_YAG_Object > complexObjects;
        std::map< std::string, SIMPLE_YAG_Object > simpleObjects;
    };



}
#endif // VELA_SCREEN_STRUCTS_H_

