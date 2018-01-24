#ifndef _VELA_CLARA_PIL_STRUCTS_H_
#define _VELA_CLARA_PIL_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class pilaserInterface;

namespace pilaserStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;
    struct pilaserObject;
    struct pvStruct;
    // Use this MACRO to define enums. Consider putting ENUMS that are more 'global' in structs.h
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( PILASER_PV_TYPE,(H_POS) (V_POS) (INTENSITY)
                                        (SHUTTER_1_OPEN) (SHUTTER_2_OPEN)
                                        (SHUTTER_1_CLOSE) (SHUTTER_2_CLOSE)
                                        (UNKNOWN_PILASER_PV_TYPE) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( PILASER_STATUS, (ON)(OFF) (ERROR)(UNKNOWN) )
    // monType could be used to switch in the staticCallbackFunction
    // For the shutter this is basically redundant, there is only one monitor: "Sta"
    // (apart from interlocks, these are handled in the base class)
    // monType could be used to switch in the statisCallbackFunction
    struct monitorStruct
    {
        monitorStruct():monType(UNKNOWN_PILASER_PV_TYPE),interface(nullptr),
                        EVID(nullptr),pilaserObj(nullptr){}
        PILASER_PV_TYPE   monType;
        pilaserInterface* interface;
        chtype            CHTYPE;
        evid              EVID;
        pilaserObject*    pilaserObj;
    };
    // The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    // The rest are paramaters passed to EPICS, ca_create_channel, ca_create_subscription etc..
    struct pvStruct
    {
        PILASER_PV_TYPE pvType;
        chid          CHID;
        std::string   pvSuffix;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
    };
    // The main hardware object holds ...
    struct pilaserObject
    {
        pilaserObject():status(PILASER_STATUS::UNKNOWN),hPos(UTL::DUMMY_DOUBLE),
                        vPos(UTL::DUMMY_DOUBLE),intensity(UTL::DUMMY_DOUBLE),
                        name(UTL::UNKNOWN_NAME),pvRoot(UTL::UNKNOWN_PVROOT){};
        std::string name, pvRoot;
        double hPos, vPos, intensity;
        int numIlocks;
        PILASER_STATUS status;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
#ifndef __CINT__
        /// keep data seperate from epics stuff
        std::map< PILASER_PV_TYPE, pvStruct > pvMonStructs;
        std::map< PILASER_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
#endif
    };
}
#endif
