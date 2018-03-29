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

class shutterInterface;

namespace shutterStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStuct;
    struct shutterObject;
    struct pvStruct;

    /// Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SHUTTER_PV_TYPE, (Sta) (On) (Off) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SHUTTER_STATE,(CLOSED)
                                                      (OPEN)
                                                      (TIMING)
                                                      (ERROR)
                                        )
//
//    struct monitorStuct
//    {// PIL_SHUTTER_PV_TYPE monType;
//        shutterObject * shutObj;
//        shutterInterface * interface;
//#ifndef __CINT__
////        evid EventID;
//#endif
//    };
//
//    /// The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
//    /// The rest are paramaters passed to EPICS, ca_create_channel, ca_create_subscription etc..
//
//    struct pvStruct
//    {
//        PIL_SHUTTER_PV_TYPE pvType;
//        chid                CHID;
//        std::string         pvSuffix;
//        unsigned long       COUNT, MASK;
//        chtype              CHTYPE;
//        evid                EVID;
//    };
//
//    /// The main hardware object holds
//
//    struct shutterObject
//    {
//        std::string name, pvRoot;
//        HWC_ENUM::SHUTTER_STATE shutterState;
//        int numIlocks;
//        std::map< HWC_ENUM::ILOCK_NUMBER , HWC_ENUM::ILOCK_STATE > iLockStates;
//#ifndef __CINT__
//        /// keep data seperate from epics stuff
//        std::map< PIL_SHUTTER_PV_TYPE, pvStruct > pvMonStructs;
//        std::map< PIL_SHUTTER_PV_TYPE, pvStruct > pvComStructs;
//        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;
//#endif
//    };
}
#endif
