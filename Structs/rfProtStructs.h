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


    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_PV_TYPE, (RESET)(STATUS)(ON)(OFF))

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

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(RF_GUN_PROT_STATUS, (GOOD)(BAD)(ERROR)(UNKNOWN))

    struct rfGunProtObject
    {
        rfGunProtObject() : status(UNKNOWN),name("unknown"){}
        std::string         name,pvRoot;
        RF_GUN_PROT_STATUS  status;
        size_t              numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
        std::map< RF_GUN_PROT_PV_TYPE, pvStruct > pvMonStructs;
        std::map< RF_GUN_PROT_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    };

//    struct rfGunProtObject
//    {
//        rfGunProtObject() : state(GUN_MOD_STATE::ERROR1 ),
//        safelyWarmedUP(false ) {}
//        std::string name, pvRoot;
//        GUN_MOD_STATE    state;
//        long         warmuptime;
//        //MOD_EXILOCK1 ilck, ilckstr;
//        bool safelyWarmedUP;
//        std::vector< std::string > goodModErrorReadStr, badModErrorReadStr;
//    #ifndef __CINT__
//        std::map< GUN_MOD_PV_TYPE, pvStruct > pvMonStructs, pvComStructs;
//    #endif
//    };

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



//    struct rfLLRFObject
//    {
//        rfLLRFObject() : name("UNKNOWN" ), pvRoot("UNKNOWN"),ampR(0.0),ampW(0), phi(0.0){}
//        std::string name, pvRoot;
//        double ampR, phi, crestPhi;
//        long ampW;
//    #ifndef __CINT__
//        std::map< RF_PV_TYPE, pvStruct > pvMonStructs;
//    #endif
//    };


//    struct rfTraceData
//    {
//        rfTraceData() : shotCount(-2 ), numShots(-2 ) {}
//        int shotCount, numShots; /// we allow -1 values here so NOT a size_t
//        std::vector< double > timeStamps;
//        std::vector< std::string > strTimeStamps;
//        std::vector< std::vector< double > > traceData;
//    };
//
//
//    struct rfPowerObject
//    {
//        rfPowerObject() : name("UNKNOWN"),pvRoot("UNKNOWN"),fwd(0.0 ), rev(0.0 ), fwdM(0.0 ), revM(0.0) {}
//        std::string name, pvRoot;
//        double fwd, rev, fwdM, revM, ratio;
//        rfTraceData revT, fwdT;
//    #ifndef __CINT__
//        std::map< RF_PV_TYPE, pvStruct > pvMonStructs;
//    #endif
//    };
//
//    struct rfObject
//    {
//        rfObject() : name("UNKNOWN" ), klyFWDLimit(0.0) {}
//        std::string name;
//        rfModObject mod;
//        rfLLRFObject llrf;
//        double klyFWDLimit;
//        std::map< std::string, rfPowerObject > powerObjects; /// For gun (and TDC>) have two of these, the cavity and the klystron
//    };
//

}
#endif//_RF_MOD_STRUCTS_H_
