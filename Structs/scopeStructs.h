#ifndef _VELA_SCOPE_STRUCTS_H_
#define _VELA_SCOPE_STRUCTS_H_
//
#include "structs.h"
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class scopeInterface;


namespace scopeStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStruct;
    struct scopeObject;
    struct scopeTraceData;
    struct pvStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCOPE_PV_TYPE, (P1) (P2) (P3) (P4) (TR1) (TR2) (TR3) (TR4) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SCOPE_TYPE, (NUM) (ARRAY) )

    /// monType could be used to switch in the staticCallbackFunction
    /// For the scope this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)

    struct pvStruct
    {
        SCOPE_PV_TYPE             pvType;
        VELA_ENUM::DIAG_TYPE      diagType;
        chid                      CHID;
        std::string               pvSuffix, objName;
        unsigned long             COUNT, MASK;
        chtype                    CHTYPE;
        evid                      EVID;
    };

    struct scopeNumObject
    {
        scopeNumObject() : p1ShotCount( 0 ), p2ShotCount( 0 ), p3ShotCount( 0 ), p4ShotCount( 0 ), numShots( 0 ) {}
        std::string                name, pvRoot;
        bool                       isAContinuousMonitorStruct, isATemporaryMonitorStruct;
        bool                       isMonitoring;
        double                     p1, p2, p3, p4;
        int                        p1ShotCount, p2ShotCount, p3ShotCount, p4ShotCount, numShots;
        double                     p1TimeStamp, p2TimeStamp, p3TimeStamp, p4TimeStamp;
        std::vector< double >      p1Vec, p2Vec, p3Vec, p4Vec;
        std::vector< std::string > strP1TimeStamps, strP2TimeStamps, strP3TimeStamps, strP4TimeStamps;
        std::vector< double >      p1TimeStamps, p2TimeStamps, p3TimeStamps, p4TimeStamps;
//        double *p1Mon, *p2Mon;
    #ifndef __CINT__
        std::map< SCOPE_PV_TYPE, pvStruct > pvMonStructs;
    #endif
    };

    struct scopeTraceData
    {
        scopeTraceData() : tr1ShotCount( 0 ), tr2ShotCount( 0 ), tr3ShotCount( 0 ), tr4ShotCount( 0 ), numShots( -2 ) {}
        std::string                          name, pvRoot;
        VELA_ENUM::DIAG_TYPE                 diagType;
        bool                                 isAContinuousMonitorStruct, isATemporaryMonitorStruct, isMonitoring;
        bool                                 isMonitoringTr1, isMonitoringTr2, isMonitoringTr3, isMonitoringTr4;
        double                               timebase, noiseFloor;
        int                                  tr1ShotCount, tr2ShotCount, tr3ShotCount, tr4ShotCount, numShots; /// we allow -1 values here so NOT a size_t
        std::vector< double >                tr1TimeStamps, tr2TimeStamps, tr3TimeStamps, tr4TimeStamps;
        std::vector< std::string >           strTr1TimeStamps, strTr2TimeStamps, strTr3TimeStamps, strTr4TimeStamps;
        std::vector< std::vector< double > > tr1TraceData, tr2TraceData, tr3TraceData, tr4TraceData;
    #ifndef __CINT__
        std::map< SCOPE_PV_TYPE, std::vector< std::vector< double > > > traceData;
        std::map< SCOPE_PV_TYPE, std::vector< double > >                timeStamps;
        std::map< SCOPE_PV_TYPE, std::vector< std::string  > >          strTimeStamps;
        std::map< SCOPE_PV_TYPE, int >                                  shotCounts;
        std::map< SCOPE_PV_TYPE, bool >                                 isMonitoringMap;

        std::map< SCOPE_PV_TYPE, pvStruct > pvMonStructs;
    #endif
    };


    struct scopeObject
    {
        std::string name;
        scopeNumObject numObject;
        std::map< std::string, scopeTraceData > traceObjects; /// There are four scope trace objects
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;

    };

    struct monitorStruct /// We use pointers when we wnat acces to the object (data) or... just  make a copy
    {
        SCOPE_PV_TYPE      monType;
        scopeObject*       scopeObject;
        std::string        objName;
        chtype             CHTYPE;
        void *             val;
        scopeInterface *interface;
        evid               *EVID;
    };
}
#endif

