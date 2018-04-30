#ifndef _VELA_charge_STRUCTS_H_
#define _VELA_charge_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
//boost
#include <boost/circular_buffer.hpp>
#endif

class chargeInterface;


namespace chargeStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStruct;
    struct chargeObject;
    struct chargeTraceData;
    struct pvStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( charge_PV_TYPE, (P1) (P2) (P3) (P4) (TR1) (TR2) (TR3) (TR4) (UNKNOWN) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( charge_TYPE, (NUM) (ARRAY) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( charge_NAME, (CLARAcharge01) (VELAcharge02) (UNKNOWN_charge_NAME) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( DIAG_TYPE, (WCM) (ICT1) (ED_FCUP) (ICT2) (FCUP) (UNKNOWN_DIAG_TYPE) )

    /// monType could be used to switch in the staticCallbackFunction
    /// For the charge this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)

    struct pvStruct
    {
        charge_PV_TYPE             pvType;
        chargeStructs::DIAG_TYPE      diagType;
        charge_TYPE                chargeType;
        chid                      CHID;
        std::string               pvSuffix, objName;
        unsigned long             COUNT, MASK;
        chtype                    CHTYPE;
    };

    struct chargeNumObject
    {
        chargeNumObject() : p1ShotCount( -2 ),
                           p2ShotCount( -2 ),
                           p3ShotCount( -2 ),
                           p4ShotCount( -2 ),
                           p1( UTL::DUMMY_DOUBLE ),
                           p2( UTL::DUMMY_DOUBLE ),
                           p3( UTL::DUMMY_DOUBLE ),
                           p4( UTL::DUMMY_DOUBLE ),
                           numShots( UTL::ZERO_INT ),
                           buffer( UTL::BUFFER_TEN ) {}
        std::string                name, pvRoot;
        chargeStructs::DIAG_TYPE       diagType;
        charge_NAME      chargeName;
        bool                       isAContinuousMonitorStruct, isATemporaryMonitorStruct;
        bool                       isMonitoring;
        double                     p1, p2, p3, p4;
        int                        p1ShotCount, p2ShotCount, p3ShotCount, p4ShotCount, numShots, shotCount;
        double                     p1TimeStamp, p2TimeStamp, p3TimeStamp, p4TimeStamp;
        size_t                     buffer;
        std::vector< double >      p1Vec, p2Vec, p3Vec, p4Vec;
        std::vector< std::string > strP1TimeStamps, strP2TimeStamps, strP3TimeStamps, strP4TimeStamps;
        std::vector< double >      p1TimeStamps, p2TimeStamps, p3TimeStamps, p4TimeStamps;
//        double *p1Mon, *p2Mon;
    #ifndef __CINT__
        std::map< charge_PV_TYPE, pvStruct > pvMonStructs;
        std::map< charge_PV_TYPE, std::vector< double > > numData;
        std::map< charge_PV_TYPE, std::vector< double > > numTimeStamps;
        std::map< charge_PV_TYPE, std::vector< std::string > > numStrTimeStamps;
        std::map< charge_PV_TYPE, int > shotCounts;
        std::map< charge_PV_TYPE, bool > isMonitoringMap;
        std::map< charge_PV_TYPE, boost::circular_buffer< double > > numDataBuffer;
    #endif
    };

    struct chargeTraceData
    {
        chargeTraceData() : tr1ShotCount( 0 ),
                           tr2ShotCount( 0 ),
                           tr3ShotCount( 0 ),
                           tr4ShotCount( 0 ),
                           numShots( -2 ),
                           buffer( UTL::BUFFER_TEN ) {}
        std::string                          name, pvRoot;
        chargeStructs::DIAG_TYPE              diagType;
        charge_NAME                           chargeName;
        bool                                 isAContinuousMonitorStruct, isATemporaryMonitorStruct, isMonitoring;
        bool                                 isMonitoringTr1, isMonitoringTr2, isMonitoringTr3, isMonitoringTr4;
        double                               timebase, noiseFloor;
        int                                  tr1ShotCount, tr2ShotCount, tr3ShotCount, tr4ShotCount, numShots; /// we allow -1 values here so NOT a size_t
        size_t                               buffer;
        std::vector< double >                tr1TimeStamps, tr2TimeStamps, tr3TimeStamps, tr4TimeStamps;
        std::vector< std::string >           strTr1TimeStamps, strTr2TimeStamps, strTr3TimeStamps, strTr4TimeStamps;
        std::vector< std::vector< double > > tr1TraceData, tr2TraceData, tr3TraceData, tr4TraceData;
    #ifndef __CINT__
        std::map< charge_PV_TYPE, std::vector< std::vector< double > > >            traceData;
        std::map< charge_PV_TYPE, std::vector< double > >                           timeStamps;
        std::map< charge_PV_TYPE, std::vector< std::string  > >                     strTimeStamps;
        std::map< charge_PV_TYPE, int >                                             shotCounts;
        std::map< charge_PV_TYPE, bool >                                            isMonitoringMap;
        std::map< charge_PV_TYPE, boost::circular_buffer< std::vector< double > > > traceDataBuffer;
        std::map< charge_PV_TYPE, pvStruct > pvMonStructs;
    #endif
    };


    struct chargeObject
    {
        std::string name;
//        chargeNumObject numObject;
        std::map< std::string, chargeNumObject > numObjects;
        std::map< std::string, chargeTraceData > traceObjects; /// There are four charge trace objects
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > iLockStates;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;

    };

    struct monitorStruct /// We use pointers when we wnat acces to the object (data) or... just  make a copy
    {
        charge_PV_TYPE        monType;
        chargeStructs::DIAG_TYPE diagType;
        chargeObject*         chargeObject;
        std::string          objName;
        chtype               CHTYPE;
        void *               val;
        chargeInterface *     interface;
        evid                 EVID;
    };
}
#endif

