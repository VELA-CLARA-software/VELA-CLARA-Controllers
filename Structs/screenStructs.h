#ifndef _VELA_SCREEN_STRUCTS_H_
#define _VELA_SCREEN_STRUCTS_H_

//
#include "structs.h"
#include "configDefinitions.h"
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
    struct screenObjectDEV;
    struct screenDriver;
    struct COMPLEX_YAG_Object;
    struct SIMPLE_YAG_Object;


    ///Use this MACRO to define enums. COnsider putting ENUMS that are more 'global' in structs.h

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_STATE, (SCREEN_IN) (SCREEN_OUT) (SCREEN_ERROR)
                                         (SCREEN_UNKNOWN) (SCREEN_H_MIRROR) (SCREEN_H_50U_SLIT)
                                        (SCREEN_H_25U_SLIT) (SCREEN_H_63MM_HOLE) (SCREEN_H_10MM_HOLE)
                                        (SCREEN_H_YAG)(SCREEN_V_YAG) (SCREEN_V_SLIT) (SCREEN_MOVING)  )


    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_PV_TYPE, (Sta) (On) (Off) (H_MABS) (V_MABS) (H_RPOS)
                                         (V_RPOS) (STOP) (H_PROT01) (V_PROT01) (PROT03) (PROT05)
                                         (H_RPWRLOSS) (V_RPWRLOSS) (UNKNOWN_SCREEN_PV_TYPE) )


    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_TYPE, (VELA_PNEUMATIC) (VELA_HV_MOVER) (UNKNOWN_SCREEN_TYPE))

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
        pvStruct():pvSuffix( "UNKNOWN_PV_SUFFIX" ), objName( UTL::UNKNOWN_NAME ),
                   COUNT( UTL::ZERO_INT ), MASK(UTL::ZERO_INT), pvType(UNKNOWN_SCREEN_PV_TYPE) {}
        SCREEN_PV_TYPE pvType;
        chid           CHID;
        std::string    pvSuffix,objName;
        unsigned long  COUNT, MASK;
        chtype         CHTYPE;
        evid           EVID;
        unsigned long  value;
    };


    // some screens have a mover, with slits, mirrors etc, and a H and V position
    struct screenDriver
    {
        screenDriver():
            h_State(SCREEN_ERROR),v_State(SCREEN_ERROR),h_position(UTL::DUMMY_DOUBLE),
            v_position(UTL::DUMMY_DOUBLE), H_MIRROR_POS(UTL::DUMMY_DOUBLE),
            H_50U_SLIT_POS(UTL::DUMMY_DOUBLE), H_25U_SLIT_POS(UTL::DUMMY_DOUBLE),
            H_63MM_HOLE_POS(UTL::DUMMY_DOUBLE), H_10MM_HOLE_POS(UTL::DUMMY_DOUBLE),
            V_YAG_POS(UTL::DUMMY_DOUBLE), V_SLIT_POS(UTL::DUMMY_DOUBLE),
            H_SLIT_POS(UTL::DUMMY_DOUBLE), H_OUT_POS(UTL::DUMMY_DOUBLE),
            V_OUT_POS(UTL::DUMMY_DOUBLE){}
        // these are the positions of each elemnt on the mover(offline data),
        // and the mover's current position
        double h_position, v_position, H_MIRROR_POS, H_50U_SLIT_POS, H_25U_SLIT_POS,
               H_63MM_HOLE_POS, H_10MM_HOLE_POS, V_YAG_POS, V_SLIT_POS, H_SLIT_POS, H_OUT_POS, V_OUT_POS;
        screenStructs::SCREEN_STATE h_State;
        screenStructs::SCREEN_STATE v_State;
    };


    /// The main hardware object holds
    struct screenObjectDEV
    {
        screenObjectDEV():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),screenType(UNKNOWN_SCREEN_TYPE){}
        std::string name, pvRoot;
        screenStructs::SCREEN_TYPE screenType;
        screenStructs::screenDriver mover;
        screenStructs::SCREEN_STATE screenState;
        int numIlocks;

        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs;
        std::map< SCREEN_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    #endif
    };




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


//    struct pnuematicScreenObject///remaining YAG object
//    {
//        pnuematicScreenObject() : IN(9999.9999), OUT(9999.9999), position_value(9999.99) {}
//        std::string name, pvRoot;
//        double position_value;
//        double IN, OUT;
//        screenStructs::SCREEN_STATE screenState;
//        int numIlocks;
//        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
//
//#ifndef __CINT__ /// Channel IDs
//        /// keep data seperate from epics stuff
//        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs;
//        std::map< SCREEN_PV_TYPE, pvStruct > pvComStructs;
//        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
//#endif // __CINT__
//
//    };





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

