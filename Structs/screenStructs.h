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
                                                       (SCREEN_UNKNOWN) (SCREEN_H_MIRROR)
                                                       (SCREEN_H_50U_SLIT)(SCREEN_H_25U_SLIT)
                                                       (H_6_POINT_3MM_HOLE_POS) (SCREEN_H_10MM_HOLE)
                                                       (SCREEN_H_YAG)(SCREEN_V_YAG) (SCREEN_V_SLIT)
                                                       (SCREEN_MOVING) (SCREEN_STATIONARY))


    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_PV_TYPE, (Sta) (On) (Off) (MABS) (RPOS) (STOP) (PROT01) (PROT03) (PROT05)
                                                         (RPWRLOSS) (UNKNOWN_SCREEN_PV_TYPE)
                                                         (STA) )


    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCREEN_TYPE, (VELA_PNEUMATIC) (VELA_HV_MOVER) (UNKNOWN_SCREEN_TYPE))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( DRIVER_DIRECTION, (HORIZONTAL) (VERTICAL) (NONE) )


    ///The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    ///The rest are parameters passed to EPICS, ca_create_channel, ca_create_subscription etc...

    struct pvStruct
    {
        pvStruct():pvSuffix( "UNKNOWN_PV_SUFFIX" ), objName( UTL::UNKNOWN_NAME ),
                   COUNT( UTL::ZERO_INT ), MASK(UTL::ZERO_INT), pvType(UNKNOWN_SCREEN_PV_TYPE), dir(NONE){}
        SCREEN_PV_TYPE pvType;
        chid           CHID;
        std::string    pvSuffix, objName;
        unsigned long  COUNT, MASK;
        chtype         CHTYPE;
        unsigned long  value;
        DRIVER_DIRECTION dir;
    };



    // these are the parameters for the status of a  VELA_HV_MOVER_SCREEN YAG-01,02,03
    struct screenDriverStatus
    {
        screenDriverStatus():position(UTL::DUMMY_DOUBLE),
                             driverDisabled(false), positionErrord(false), homeErrord(false), isMovingd(false),
                             STA(UTL::DUMMY_INT),numSTABits(UTL::DUMMY_INT),parentScreen("UNKNOWN_PARENT"),
                             dir(NONE){}
        std::string parentScreen;
        double position;
        bool driverDisabled, positionError, homeError, isMoving;
        // the bits in these numbers are the alarams/locks for the driver,
        // the bits map to the following (not 100% sure of order, but I expect this is correct)
        int STA, numSTABits;
        // we need to know the order of the bits, so that is in a vector based on the config file
        std::vector< std::string > STA_bit_label;
        std::map< std::string, bool > STA_bit_map;
        DRIVER_DIRECTION dir;
    };
    // a screen cassette holds the positions (all known offline) of elements on a screen mover
    struct screenCassette
    {
        screenCassette():
            MIRROR_POS(UTL::DUMMY_DOUBLE),      SLIT_50_UM_POS(UTL::DUMMY_DOUBLE),
            SLIT_25_UM_POS(UTL::DUMMY_DOUBLE),  HOLE_6p3_MM_POS(UTL::DUMMY_DOUBLE),
            HOLE_10_MM_POS(UTL::DUMMY_DOUBLE),  YAG_POS(UTL::DUMMY_DOUBLE),
            SLIT_POS(UTL::DUMMY_DOUBLE),        RF_POS(UTL::DUMMY_DOUBLE),
            OUT_POS(UTL::DUMMY_DOUBLE){}
            double MIRROR_POS, SLIT_50_UM_POS, SLIT_25_UM_POS,HOLE_6p3_MM_POS,
                   HOLE_10_MM_POS, YAG_POS, SLIT_POS, RF_POS, OUT_POS;
    };




    // some screens have a mover, with slits, mirrors etc, and a H and V position
//    struct screenDriver
//    {
//        screenDriver():
//            h_position(UTL::DUMMY_DOUBLE),
//            v_position(UTL::DUMMY_DOUBLE), H_MIRROR_POS(UTL::DUMMY_DOUBLE),
//            H_50U_SLIT_POS(UTL::DUMMY_DOUBLE), H_25U_SLIT_POS(UTL::DUMMY_DOUBLE),
//            H_6_POINT_3MM_HOLE_POS(UTL::DUMMY_DOUBLE), H_10MM_HOLE_POS(UTL::DUMMY_DOUBLE),
//            V_YAG_POS(UTL::DUMMY_DOUBLE), V_SLIT_POS(UTL::DUMMY_DOUBLE),
//            H_SLIT_POS(UTL::DUMMY_DOUBLE), H_OUT_POS(UTL::DUMMY_DOUBLE),
//            H_PROT01(UTL::DUMMY_DOUBLE), V_PROT01(UTL::DUMMY_DOUBLE),
//            V_OUT_POS(UTL::DUMMY_DOUBLE), H_isMoving(false), V_isMoving(false),
//            h_STA( UTL::DUMMY_INT ), v_STA( UTL::DUMMY_INT), numSTABits( UTL::DUMMY_INT)
//            vertical_disabled((false), horizontal_disabled((false);
//            position_error(false), home_error(false);
//            {}
////            Trajectory_in_Progress(false), Historical_Pos HW_Limits(false), Historical_Neg HW_Limits(false),
////            Index Report_Avalables(false),Wraparound_Occurreds(false), Excessive_Position_Errors(false),
////            Temperature_Faults(false), Motor_is_Offs(false), Index_Inputs(false),Pos_HW_Limit_Asserteds(false),
////            Neg_HW_Limit_Asserteds(false), Maths_Overflows(false), Index_Errors(false), Syntax_Errors(false),
////             Over_Currents(false), Programme_Checksum_Errors(false)
//        // these are the positions of each elemnt on the mover (offline data),
//        double H_MIRROR_POS, H_50U_SLIT_POS, H_25U_SLIT_POS,
//               H_6_POINT_3MM_HOLE_POS, H_10MM_HOLE_POS, V_YAG_POS, V_SLIT_POS, H_SLIT_POS, H_OUT_POS, V_OUT_POS;
//        // and the mover's current position
//        double h_position, v_position;
//
//        bool vertical_disabled, horizontal_disabled;
//        bool  position_error, home_error;
//
//
//        double H_PROT01, V_PROT01;
//        int h_STA, v_STA, numSTABits;// the bits in these numbers are the alarams/locks for the mover, the bits map to the following (not 100% sure of order)
//        // we need to know the order of the bits, so that is in a vector based on the config file
//        std::vector< std::string > H_STA_bit_label,V_STA_bit_label;
//        std::map< std::string, bool > H_STA_bit_map, V_STA_bit_map;
//        bool H_isMoving, V_isMoving;
////        bool Trajectory_in_Progress, Historical_Pos HW_Limit, Historical_Neg HW_Limit, Index Report_Avalable,
////             Wraparound_Occurred, Excessive_Position_Error, Temperature_Fault, Motor_is_Off, Index_Input,
////             Pos_HW_Limit_Asserted, Neg_HW_Limit_Asserted, Maths_Overflow, Index_Error, Syntax_Error,
////             Over_Current, Programme_Checksum_Error,
//    };





    /// The main hardware object holds
    struct screenObjectDEV
    {
        screenObjectDEV():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),screenType(UNKNOWN_SCREEN_TYPE){}
        std::string name, pvRoot;
        SCREEN_TYPE        screenType;
        screenDriverStatus hDriverSTA,vDriverSTA;
        screenCassette     hCassette,vCassette;
        SCREEN_STATE       screenState;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs;
        std::map< SCREEN_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    #endif
    };
    /// monType could be used to switch in the statisCallbackFunction
    struct monitorStructDEV
    {
        monitorStructDEV(): monType(UNKNOWN_SCREEN_PV_TYPE),interface(nullptr),EVID(nullptr),
                            obj(nullptr),dir(NONE){}
        SCREEN_PV_TYPE      monType;
        screenObjectDEV*    obj;
        screenInterface*    interface;
        chtype              CHTYPE;
        evid                EVID;
        DRIVER_DIRECTION    dir;
    };


    /// below is going to go

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

