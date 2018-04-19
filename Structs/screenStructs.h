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
    struct screenObject;
    struct monitorStruct;

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
                                        (H_RETRACTED)     (V_RETRACTED)
                                        (H_MAX)           (V_MAX)
                                        (H_SLIT_1)        (V_RF)
                                        (H_SLIT_2)        (V_MIRROR)
                                        (H_SLIT_3)        (V_YAG)
                                        (H_APT_1)         (V_GRAT)
                                        (H_APT_2)         (V_SLIT_1)
                                        (H_APT_3)         (V_COL)
                                        (H_RF)            (YAG)
                                        (H_OUT)           (RETRACTED)
                                        (UNKNOWN_POSITION) (UNKNOWN_POS) ) // if the mover is not in a predifend position

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SCREEN_PV_TYPE, (H_SDEV) (H_GET_DEV) (H_DEV_STATE) (H_MAX_POS) (H_DEV_CENT) (H_MOVING) (H_JOG) (H_JDIFF) (H_ACTPOS)
                                                        (H_TRIGGER) (H_TGTPOS) (H_EX) (H_EN) (H_READY) (H_CAL) (H_JOGDOWN) (H_JOGUP) (H_JOGINC)
                                                        (V_SDEV) (V_GET_DEV) (V_DEV_STATE) (V_MAX_POS) (V_DEV_CENT) (V_MOVING) (V_JOG) (V_JDIFF) (V_ACTPOS)
                                                        (V_TRIGGER) (V_TGTPOS) (V_EX) (V_EN) (V_READY) (V_CAL) (V_JOGDOWN) (V_JOGUP) (V_JOGINC)
                                                        (SDEV) (GET_DEV) (DEV_STATE) (MAX_POS) (DEV_CENT) (MOVING) (ACTPOS) (TRIGGER) (EN) (READY)
                                                        (UNKNOWN_SCREEN_PV_TYPE) )
    // the screen driver status, moving, disabled etc...
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(DRIVER_STATE, (H_DRIVER_MOVING)(H_DRIVER_STATIONARY)(H_DRIVER_DISABLED)(H_DRIVER_ENABLED)
                                                      (V_DRIVER_MOVING)(V_DRIVER_STATIONARY)(V_DRIVER_DISABLED)(V_DRIVER_ENABLED)
                                                      (P_DRIVER_MOVING) (P_DRIVER_STATIONARY) (P_DRIVER_DISABLED) (P_DRIVER_ENABLED)
                                                      (H_DRIVER_ERROR) (V_DRIVER_ERROR) (P_DRIVER_ERROR) (UNKNOWN_DRIVER_STATE) )
    // screen drivers come in two types
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(DRIVER_DIRECTION, (HORIZONTAL) (VERTICAL) (PNEUMATIC) (NONE) )
    // turns out this is really useful, but itlooks like cancer
    // it is used by the conifg reader and the interface, so although not a struct it's going to live here
    // ***** ADD TO THIS IF CLARA HAS MORE ELEMENTS *****    const std::string PV_SUFFIX_H_MOVING    = "PV_SUFFIX_H_MOVING";

    const std::vector<std::string>hElements={UTL::PV_SUFFIX_H_READY,
                                             UTL::PV_SUFFIX_H_GET_DEV,
                                             UTL::PV_SUFFIX_H_DEV_STATE,
                                             UTL::PV_SUFFIX_H_MAX_POS,
                                             UTL::PV_SUFFIX_H_DEV_CENT,
                                             UTL::PV_SUFFIX_H_ACTPOS,
                                             UTL::PV_SUFFIX_H_EN,
                                             UTL::PV_SUFFIX_H_SDEV,
                                             UTL::PV_SUFFIX_H_TRIGGER,
                                             UTL::PV_SUFFIX_H_EX,
                                             UTL::PV_SUFFIX_H_TGTPOS,
                                             UTL::PV_SUFFIX_H_JOG,
                                             UTL::PV_SUFFIX_H_JDIFF};
    const std::vector<std::string>vElements={UTL::PV_SUFFIX_V_READY,
                                             UTL::PV_SUFFIX_V_GET_DEV,
                                             UTL::PV_SUFFIX_V_DEV_STATE,
                                             UTL::PV_SUFFIX_V_MAX_POS,
                                             UTL::PV_SUFFIX_V_DEV_CENT,
                                             UTL::PV_SUFFIX_V_ACTPOS,
                                             UTL::PV_SUFFIX_V_EN,
                                             UTL::PV_SUFFIX_V_SDEV,
                                             UTL::PV_SUFFIX_V_TRIGGER,
                                             UTL::PV_SUFFIX_V_EX,
                                             UTL::PV_SUFFIX_V_TGTPOS,
                                             UTL::PV_SUFFIX_V_JOG,
                                             UTL::PV_SUFFIX_V_JDIFF};
    ///The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    ///The rest are parameters passed to EPICS, ca_create_channel, ca_create_subscription etc...

    struct pvStruct
    {
        pvStruct():pvSuffix( "UNKNOWN_PV_SUFFIX" ),
                   COUNT( UTL::ZERO_INT ), MASK(UTL::ZERO_INT), pvType(UNKNOWN_SCREEN_PV_TYPE), dir(NONE){}
        SCREEN_PV_TYPE   pvType;
        chid             CHID;
        std::string      pvSuffix;
        unsigned long    COUNT, MASK;
        chtype           CHTYPE;
        unsigned long    value;
        DRIVER_DIRECTION dir;
    };
    /// The main hardware object holds
    struct screenObject
    {
        screenObject():name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT),screenType(UNKNOWN_SCREEN_TYPE),
                       screenHState(UNKNOWN_POSITION),screenSetHState(UNKNOWN_POSITION),
                       screenVState(UNKNOWN_POSITION),screenSetVState(UNKNOWN_POSITION),
                       screenPState(UNKNOWN_POSITION),screenSetPState(UNKNOWN_POSITION),
                       devCentH(UTL::DUMMY_DOUBLE),devCentV(UTL::DUMMY_DOUBLE), devCentP(UTL::DUMMY_DOUBLE),
                       actPOSH(UTL::DUMMY_DOUBLE),actPOSV(UTL::DUMMY_DOUBLE), actPOSP(UTL::DUMMY_DOUBLE){}
        std::string name, pvRoot;
        SCREEN_TYPE       screenType;
        SCREEN_STATE      screenHState, screenSetHState, screenVState, screenSetVState, screenPState, screenSetPState;
        double            devCentH, devCentV, devCentP;
        double            actPOSH, actPOSV, actPOSP;
        double            jDiffH, jDiffV;
        bool              hMoving, vMoving, pMoving;
        double            position;
        DRIVER_STATE      hDriverState, vDriverState;
        std::map< SCREEN_STATE, bool >             elementExists;         // holds if an element exists on this cassette(from config)
        std::map< SCREEN_STATE, DRIVER_DIRECTION > elementDirection;
        std::map< SCREEN_STATE, unsigned short >   elementPositions;      // position element on cassette (from config)
        std::map< SCREEN_STATE, double >           devicePositions;      // position element on cassette (from config)
        int numIlocks;
        std::map< HWC_ENUM::ILOCK_NUMBER , HWC_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< SCREEN_PV_TYPE, pvStruct > pvMonStructs;
        std::map< SCREEN_PV_TYPE, pvStruct > pvComStructs;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;
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
}
#endif // VELA_SCREEN_STRUCTS_H_

