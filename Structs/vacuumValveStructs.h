#ifndef _VELA_QBOX_STRUCTS_H_
#define _VELA_QBOX_STRUCTS_H_
//
#include "structs.h"
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class vacuumValveInterface;

namespace vacuumValveStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStruct;
    struct vacValveObject;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(VALVE_STATE, (VALVE_CLOSED)
                                                     (VALVE_OPEN)
                                                     (VALVE_TIMING)
                                                     (VALVE_ERROR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( VAC_VALVE_PV_TYPE, (Sta) (On) (Off) )

    /// monType could be used to switch in the staticCallbackFunction
    /// For the vacValve this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)

    struct monitorStruct
    {
        VAC_VALVE_PV_TYPE     monType;
        vacValveObject*       vacValveObj;
        vacuumValveInterface* interface;
        std::string           objName;
        chtype                CHTYPE;
        void*                 obj;
        evid*                 EVID;
    };

    struct pvStruct
    {
        VAC_VALVE_PV_TYPE   pvType;
        chid                CHID;
        std::string         pvSuffix;
        unsigned long       COUNT, MASK;
        chtype              CHTYPE;
        evid                EVID;
    };


    struct vacValveObject
    {
        std::string name, pvRoot;
        VALVE_STATE vacValveState;
        int numIlocks;
        std::map< HWC_ENUM::ILOCK_NUMBER , HWC_ENUM::ILOCK_STATE > iLockStates;

#ifndef __CINT__
        /// keep data seperate from epics stuff

        std::map< VAC_VALVE_PV_TYPE, pvStruct > pvMonStructs;
        std::map< VAC_VALVE_PV_TYPE, pvStruct > pvComStructs;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;
////
////        std::map< PIL_SHUTTER_PV_TYPE, chid > chidMap;
////        std::map< HWC_ENUM::ILOCK_NUMBER , chid > ilkChidMap;
#endif
    };
}
#endif
