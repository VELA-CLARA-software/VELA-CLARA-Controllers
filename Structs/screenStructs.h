#ifndef _VELA_SCREEN_STRUCTS_H_
#define _VELA_SCREEN_STRUCTS_H_

//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
#include <thread>

//epics
#ifndef __CINT__
#include <cadef.h>
#endif // __CINT__

class screenInterface;

namespace screenStructs
{   //Forward declare structs, gcc seems to like this
    struct pvStruct;
    struct velaINJscreenObject;
    struct screenObject;
    struct monitorStruct;
    struct screenDriver;
    struct COMPLEX_YAG_Object;
    struct SIMPLE_YAG_Object;

    //Use this MACRO to define enums. oOnsider putting ENUMS that are more 'global' in structs.h
    // screen types
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SCREEN_TYPE, (VELA_PNEUMATIC)  (VELA_HV_MOVER)
                                                     (CLARA_HV_MOVER)  (CLARA_V_MOVER)
                                                     (CLARA_PNEUMATIC) (UNKNOWN_SCREEN_TYPE))
    // screen states, (and cassette elements) Although not all are needed, we keep H and V version of each element
    // ***** ADD TO THIS IF CLARA HAS MORE ELEMENTS *****
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SCREEN_STATE,
                                        // states for the simple VELA_PNEUMATIC screens
                                        (SCREEN_IN)(SCREEN_OUT)(SCREEN_MOVING)(SCREEN_ERROR)(SCREEN_UNKNOWN)
                                        // states for the VELA_HV_MOVER screens
                                        (H_MIRROR)        (V_MIRROR)
                                        (H_50U_SLIT)      (V_50U_SLIT)
                                        (H_25U_SLIT)      (V_25U_SLIT)
                                        (H_6p3MM_HOLE)    (V_6p3MM_HOLE)
                                        (H_10MM_HOLE)     (V_10MM_HOLE)
                                        (H_YAG)           (V_YAG)
                                        (H_SLIT)          (V_SLIT)
                                        (H_RF)            (V_RF)
                                        (H_OUT)           (V_OUT)
                                        (H_SLIT_1)        (H_SLIT_2)
                                        (H_SLIT_3)        (H_4MM_APERTURE)
                                        (H_6MM_APERTURE)  (H_9MM_APERTURE)
                                        (V_GRATICULE)     (V_SLIT_1)
                                        (V_YAG_MIRROR)    (H_YAG_MIRROR)
                                        (V_COLLIMATOR)    (V_RF_CAGE)
                                        (UNKNOWN_POSITION) //) // if the mover is not in a predifend position
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SCREEN_POS,
                                        // states for the simple VELA_PNEUMATIC screens
                                        //(SCREEN_IN)(SCREEN_OUT)(SCREEN_MOVING)(SCREEN_ERROR)(SCREEN_UNKNOWN)
                                        // states for the VELA_HV_MOVER screens
                                        (H_MIRROR_POS)        (V_MIRROR_POS)
                                        (H_50U_SLIT_POS)      (V_50U_SLIT_POS)
                                        (H_25U_SLIT_POS)      (V_25U_SLIT_POS)
                                        (H_6p3MM_HOLE_POS)    (V_6p3MM_HOLE_POS)
                                        (H_10MM_HOLE_POS)     (V_10MM_HOLE_POS)
                                        (H_YAG_POS)           (V_YAG_POS)
                                        (H_SLIT_POS)          (V_SLIT_POS)
                                        (H_RF_POS)            (V_RF_POS)
                                        (H_OUT_POS)           (V_OUT_POS)
                                        (H_SLIT_1_POS)        (H_SLIT_2_POS)
                                        (H_SLIT_3_POS)        (H_4MM_APERTURE_POS)
                                        (H_6MM_APERTURE_POS)  (H_9MM_APERTURE_POS)
                                        (V_GRATICULE_POS)     (V_SLIT_1_POS)
                                        (V_YAG_MIRROR_POS)    (H_YAG_MIRROR_POS)
                                        (V_COLLIMATOR_POS)    (V_RF_CAGE_POS)
                                        (UNKNOWN_POS) ) // if the mover is not in a predifend position
    // screen PV types
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SCREEN_PV_TYPE, (Sta) (On) (Off) (MABS) (RPOS) (STOP) (PROT01) (PROT03) (PROT05)
                                                        (RPWRLOSS) (UNKNOWN_SCREEN_PV_TYPE) (STA) (ACTPOS) (MOVING)
                                                        (EN) (READY) (CAL) (JOGDOWN) (JOGUP) (JOGINC) (POS) )
    // the screen driver status, moving, disabled etc...
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(DRIVER_STATE, (DRIVER_MOVING)(DRIVER_STATIONARY)(DRIVER_DISABLED)(DRIVER_ENABLED)
                                                       (DRIVER_ERROR) )
    // screen drivers come in two types
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(DRIVER_DIRECTION, (HORIZONTAL)(VERTICAL)(NONE) )
    // turns out this is really useful, but itlooks like cancer
    // it is used by the conifg reader and the interface, so although not a struct it's going to live here
    // ***** ADD TO THIS IF CLARA HAS MORE ELEMENTS *****
    const std::map<std::string,SCREEN_STATE>hCassetteElementMap={{UTL::MIRROR         ,H_MIRROR},
                                                                {UTL::SLIT_50_UM      ,H_50U_SLIT},
                                                                {UTL::SLIT_25_UM      ,H_25U_SLIT},
                                                                {UTL::HOLE_6p3_MM     ,H_6p3MM_HOLE},
                                                                {UTL::HOLE_10_MM      ,H_10MM_HOLE},
                                                                {UTL::YAG             ,H_YAG},
                                                                {UTL::RF              ,H_RF},
                                                                {UTL::SLIT            ,H_SLIT},
                                                                {UTL::H_SLIT_1        ,H_SLIT_1},
                                                                {UTL::H_SLIT_2        ,H_SLIT_2},
                                                                {UTL::H_SLIT_3        ,H_SLIT_3},
                                                                {UTL::H_4MM_APERTURE  ,H_4MM_APERTURE},
                                                                {UTL::H_6MM_APERTURE  ,H_6MM_APERTURE},
                                                                {UTL::H_9MM_APERTURE  ,H_9MM_APERTURE},
                                                                {UTL::H_YAG_MIRROR    ,H_YAG_MIRROR},
                                                                {UTL::OUT             ,H_OUT},
                                                                {UTL::MIRROR_POS         ,H_MIRROR_POS},
                                                                {UTL::SLIT_50_UM_POS      ,H_50U_SLIT_POS},
                                                                {UTL::SLIT_25_UM_POS      ,H_25U_SLIT_POS},
                                                                {UTL::HOLE_6p3_MM_POS     ,H_6p3MM_HOLE_POS},
                                                                {UTL::HOLE_10_MM_POS      ,H_10MM_HOLE_POS},
                                                                {UTL::YAG_POS             ,H_YAG_POS},
                                                                {UTL::RF_POS              ,H_RF_POS},
                                                                {UTL::SLIT_POS            ,H_SLIT_POS},
                                                                {UTL::H_SLIT_1_POS        ,H_SLIT_1_POS},
                                                                {UTL::H_SLIT_2_POS        ,H_SLIT_2_POS},
                                                                {UTL::H_SLIT_3_POS        ,H_SLIT_3_POS},
                                                                {UTL::H_4MM_APERTURE_POS  ,H_4MM_APERTURE_POS},
                                                                {UTL::H_6MM_APERTURE_POS  ,H_6MM_APERTURE_POS},
                                                                {UTL::H_9MM_APERTURE_POS  ,H_9MM_APERTURE_POS},
                                                                {UTL::H_YAG_MIRROR_POS    ,H_YAG_MIRROR_POS},
                                                                {UTL::OUT_POS             ,H_OUT_POS}};
    const std::map<std::string,SCREEN_STATE>vCassetteElementMap={{UTL::MIRROR         ,V_MIRROR},
                                                                {UTL::SLIT_50_UM      ,V_50U_SLIT},
                                                                {UTL::SLIT_25_UM      ,V_25U_SLIT},
                                                                {UTL::HOLE_6p3_MM     ,V_6p3MM_HOLE},
                                                                {UTL::HOLE_10_MM      ,V_10MM_HOLE},
                                                                {UTL::YAG             ,V_YAG},
                                                                {UTL::RF              ,V_RF},
                                                                {UTL::SLIT            ,V_SLIT},
                                                                {UTL::V_SLIT_1        ,V_SLIT_1},
                                                                {UTL::V_GRATICULE     ,V_GRATICULE},
                                                                {UTL::V_COLLIMATOR    ,V_COLLIMATOR},
                                                                {UTL::V_YAG_MIRROR    ,V_YAG_MIRROR},
                                                                {UTL::V_RF_CAGE       ,V_RF_CAGE},
                                                                {UTL::OUT             ,V_OUT},
                                                                {UTL::MIRROR_POS         ,V_MIRROR_POS},
                                                                {UTL::SLIT_50_UM_POS      ,V_50U_SLIT_POS},
                                                                {UTL::SLIT_25_UM_POS      ,V_25U_SLIT_POS},
                                                                {UTL::HOLE_6p3_MM_POS     ,V_6p3MM_HOLE_POS},
                                                                {UTL::HOLE_10_MM_POS      ,V_10MM_HOLE_POS},
                                                                {UTL::YAG_POS             ,V_YAG_POS},
                                                                {UTL::RF_POS              ,V_RF_POS},
                                                                {UTL::SLIT_POS            ,V_SLIT_POS},
                                                                {UTL::V_SLIT_1_POS        ,V_SLIT_1_POS},
                                                                {UTL::V_GRATICULE_POS     ,V_GRATICULE_POS},
                                                                {UTL::V_COLLIMATOR_POS    ,V_COLLIMATOR_POS},
                                                                {UTL::V_YAG_MIRROR_POS    ,V_YAG_MIRROR_POS},
                                                                {UTL::V_RF_CAGE_POS       ,V_RF_CAGE_POS},
                                                                {UTL::OUT_POS             ,V_OUT_POS}};
    const std::map<std::string,SCREEN_STATE>hCassettePosMap   = {{UTL::MIRROR_POS         ,H_MIRROR_POS},
                                                                {UTL::SLIT_50_UM_POS      ,H_50U_SLIT_POS},
                                                                {UTL::SLIT_25_UM_POS      ,H_25U_SLIT_POS},
                                                                {UTL::HOLE_6p3_MM_POS     ,H_6p3MM_HOLE_POS},
                                                                {UTL::HOLE_10_MM_POS      ,H_10MM_HOLE_POS},
                                                                {UTL::YAG_POS             ,H_YAG_POS},
                                                                {UTL::RF_POS              ,H_RF_POS},
                                                                {UTL::SLIT_POS            ,H_SLIT_POS},
                                                                {UTL::H_SLIT_1_POS        ,H_SLIT_1_POS},
                                                                {UTL::H_SLIT_2_POS        ,H_SLIT_2_POS},
                                                                {UTL::H_SLIT_3_POS        ,H_SLIT_3_POS},
                                                                {UTL::H_4MM_APERTURE_POS  ,H_4MM_APERTURE_POS},
                                                                {UTL::H_6MM_APERTURE_POS  ,H_6MM_APERTURE_POS},
                                                                {UTL::H_9MM_APERTURE_POS  ,H_9MM_APERTURE_POS},
                                                                {UTL::H_YAG_MIRROR_POS    ,H_YAG_MIRROR_POS},
                                                                {UTL::OUT_POS             ,H_OUT_POS}};
    const std::map<std::string,SCREEN_STATE>vCassettePosMap =   {{UTL::MIRROR_POS         ,V_MIRROR_POS},
                                                                {UTL::SLIT_50_UM_POS      ,V_50U_SLIT_POS},
                                                                {UTL::SLIT_25_UM_POS      ,V_25U_SLIT_POS},
                                                                {UTL::HOLE_6p3_MM_POS     ,V_6p3MM_HOLE_POS},
                                                                {UTL::HOLE_10_MM_POS      ,V_10MM_HOLE_POS},
                                                                {UTL::YAG_POS             ,V_YAG_POS},
                                                                {UTL::RF_POS              ,V_RF_POS},
                                                                {UTL::SLIT_POS            ,V_SLIT_POS},
                                                                {UTL::V_SLIT_1_POS        ,V_SLIT_1_POS},
                                                                {UTL::V_GRATICULE_POS     ,V_GRATICULE_POS},
                                                                {UTL::V_COLLIMATOR_POS    ,V_COLLIMATOR_POS},
                                                                {UTL::V_YAG_MIRROR_POS    ,V_YAG_MIRROR_POS},
                                                                {UTL::V_RF_CAGE_POS       ,V_RF_CAGE_POS},
                                                                {UTL::OUT_POS             ,V_OUT_POS}};
    ///The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    ///The rest are parameters passed to EPICS, ca_create_channel, ca_create_subscription etc...

    struct pvStruct
    {
        pvStruct():pvSuffix( "UNKNOWN_PV_SUFFIX" ),
                   COUNT( UTL::ZERO_INT ), MASK(UTL::ZERO_INT), pvType(UNKNOWN_SCREEN_PV_TYPE), dir(NONE){}
        SCREEN_PV_TYPE pvType;
        chid           CHID;
        std::string    pvSuffix;
        unsigned long  COUNT, MASK;
        chtype         CHTYPE;
        unsigned long  value;
        DRIVER_DIRECTION dir;
    };
    // these are the parameters for the status of a  VELA_HV_MOVER_SCREEN YAG-01,02,03
    struct screenDriverStatus
    {
        screenDriverStatus():position(UTL::DUMMY_DOUBLE),
                             state(DRIVER_ERROR),
                             trajectory(DRIVER_ERROR),STA(UTL::DUMMY_INT),
                             numSTABits(UTL::DUMMY_INT),parentScreen("UNKNOWN_PARENT"),
                             dir(NONE){}
        std::string parentScreen;
        double position;
        // the bits in these numbers are the alarams/locks for the driver,
        // the bits map to the following (not 100% sure of order, but I expect this is correct)
        int STA, numSTABits;
        // we need to know the order of the bits, so that is in a vector based on the config file
        std::vector<std::string> STA_bit_label;
        std::map<std::string, bool> STA_bit_map;
        DRIVER_DIRECTION dir;
        DRIVER_STATE     trajectory; // this comes from the "Trajectory in process" bit
        DRIVER_STATE     state;      // PROT01 PV, is the driver enabled, or disabled,
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs,pvComStructs;
    };
    // a screen cassette holds the positions (all known offline) of elements on a screen mover
    struct screenCassette
    {
        screenCassette():posTolerance(UTL::DUMMY_DOUBLE),dir(NONE),screenState(UNKNOWN_POSITION),screenPos(UNKNOWN_POS),
                         parentScreen("UNKNOWN_PARENT"){}
            std::string      parentScreen;
            DRIVER_DIRECTION dir;
            SCREEN_STATE     screenState;                          // updated with which element is in the beam
            SCREEN_STATE     screenPos;                          // updated with which element is in the beam
            double           posTolerance;
            std::map<SCREEN_STATE,bool>   cassetteElements;         // holds if an element exists on this cassette(from config)
            std::map<SCREEN_STATE,double> cassetteElementsPosition; // position element on cassette (from config)
            std::map<SCREEN_STATE,double> cassetteElementsPos;      // position element on cassette (from config)
    };
    // a screenDriver has an H and V status and an H and V cassette
    struct screenDriver
    {   screenDriver(): positionError(false), homeError(false),parentScreen("UNKNOWN_PARENT")
        {};
        bool positionError, homeError;
        std::string        parentScreen;
        screenDriverStatus hDriverSTA,vDriverSTA;
        screenCassette     hCassette,vCassette;
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs,pvComStructs;
    };
    /// The main hardware object holds
    struct velaINJscreenObject
    {
        velaINJscreenObject():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),screenType(UNKNOWN_SCREEN_TYPE){}
        std::string name, pvRoot;
        SCREEN_TYPE       screenType;
        screenDriver      driver;
        SCREEN_STATE      screenState;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs,pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    #endif
    };
    struct screenObject
    {
        screenObject():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),screenType(UNKNOWN_SCREEN_TYPE){}
        std::string name, pvRoot;
        SCREEN_TYPE       screenType;
        screenDriver      driver;
        SCREEN_STATE      screenState;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs,pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    #endif
    };
    /// monType could be used to switch in the statisCallbackFunction
    struct monitorStruct
    {
        monitorStruct():monType(UNKNOWN_SCREEN_PV_TYPE),interface(nullptr),EVID(nullptr),
                        obj(nullptr),dir(NONE){}
        SCREEN_PV_TYPE      monType;
        void*               obj;
        screenInterface*    interface;
        chtype              CHTYPE;
        evid                EVID;
        DRIVER_DIRECTION    dir;
    };
    /// This holds all info for actually moving a screen in a complex fashion
    struct HV_dualMoveStruct
    {   HV_dualMoveStruct():interface(nullptr),thread(nullptr),key(UTL::ZERO_INT),isComplete(false){}
        screenInterface* interface;
        std::thread*     thread;
        size_t           key;
        bool             isComplete;
        std::vector<std::string>                 dualMotorMoves_Scr;
        std::vector<screenStructs::SCREEN_STATE> dualMotorMoves_Sta;
    };
}
#endif // VELA_SCREEN_STRUCTS_H_

