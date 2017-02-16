#ifndef _VELA_PIL_SHUTTER_STRUCTS_H_
#define _VELA_PIL_SHUTTER_STRUCTS_H_
//
#include "structs.h"
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class laserInterface;

namespace laserStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStuct;
    struct laserObject;
    struct pvStruct;

    /// Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( LASER_PV_TYPE, (sta)(X_POS) (Y_POS) (INTENSITY) (UNKNOWN) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( LASER_STATUS, (ON)(OFF) (ERROR) )

    /// monType could be used to switch in the staticCallbackFunction
    /// For the shutter this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)
    /// monType could be used to switch in the statisCallbackFunction
    struct monitorStruct
    {
        monitorStruct():monType(UNKNOWN),interface(nullptr),EVID(nullptr),
                        obj(nullptr),laserObj(nullptr){}
        LASER_PV_TYPE   monType;
        void*           obj;
        laserInterface* interface;
        chtype          CHTYPE;
        evid            EVID;
        laserObject*    laserObj;
    };

    /// The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    /// The rest are paramaters passed to EPICS, ca_create_channel, ca_create_subscription etc..

    struct pvStruct
    {
        LASER_PV_TYPE pvType;
        chid          CHID;
        std::string   pvSuffix;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };

    /// The main hardware object holds

    struct laserObject
    {
        std::string name, pvRoot;
        VELA_ENUM::SHUTTER_STATE shutterState;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
#ifndef __CINT__
        /// keep data seperate from epics stuff
        std::map< LASER_PV_TYPE, pvStruct > pvMonStructs;
        std::map< LASER_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
#endif
    };


}
#endif
