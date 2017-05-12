#ifndef _RF_MOD_STRUCTS_H_
#define _RF_MOD_STRUCTS_H_
//
#include "structs.h"
//stl
#include <vector>
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class gunProtInterface;

namespace rfProtStructs
{
//    struct pvStruct;
//    struct rfPowerObject;
//    struct rfPowerMonitorStruct;
//    struct monitorStuct;
//    struct rfLLRFObject;
//    struct rfModObject;
//    struct rfObject;


    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_PV_TYPE, (RESET)(STATUS)(ON)(OFF)(CMI))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_TYPE, (CLARA_HRRG)(VELA_LRRG)(VELA_HRRG)(CLARA_LRRG)(TEST)(NOT_KNOWN)(GENERAL)(ENABLE)(NO_MODE))

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_STATUS, (GOOD)(BAD)(ERROR)(UNKNOWN))

    /// These can't go in VELA_ENUM as they need a pvType.
    struct pvStruct
    {
        pvStruct() : pvSuffix("UNKNOWN" ), objName("UNKNOWN"),COUNT(0),MASK(0){}
        RF_GUN_PROT_PV_TYPE pvType;
        chid            CHID;
        std::string     pvSuffix, objName;
        unsigned long   COUNT, MASK;
        chtype          CHTYPE;
    };

    struct rfGunProtObject
    {
        rfGunProtObject() : status(UNKNOWN),protType(NOT_KNOWN),name("unknown"),numIlocks(0){}
        std::string         name,pvRoot;
        RF_GUN_PROT_STATUS  status;
        unsigned long       cmi;
        std::vector<int>    gunProtKeyBits;// the bit position of the key bits in the prot cmi
        std::vector<bool>   gunProtKeyBitValues;// calues of the key bits
        size_t              numIlocks;
        RF_GUN_PROT_TYPE        protType;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
        std::map< RF_GUN_PROT_PV_TYPE, pvStruct > pvMonStructs;
        std::map< RF_GUN_PROT_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    };

    struct monitorStruct
    {
        monitorStruct() : rfProtObject(nullptr),interface(nullptr){}
        RF_GUN_PROT_PV_TYPE   monType;
        rfGunProtObject*  rfProtObject;
        chtype            CHTYPE;
        //void *           val; /// most of the rfObject Values appear to be doubles, which is handy... recast in staticEntry function...
        gunProtInterface *interface;
        evid              EVID;
    };
}
#endif//_RF_MOD_STRUCTS_H_
