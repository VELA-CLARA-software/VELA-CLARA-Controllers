#ifndef _VELA_BLM_STRUCTS_H_
#define _VELA_BLM_STRUCTS_H_
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

class blmInterface;


namespace blmStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStruct;
    struct blmObject;
    struct blmTraceData;
    struct pvStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( BLM_PV_TYPE, (CH1WAVE) (CH2WAVE) (CH3WAVE) (CH4WAVE) (CH1TIME) (CH2TIME) (CH3TIME) (CH4TIME) (UNKNOWN) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( BLM_DATA_TYPE, (WAVE) (TIME) (UNKNOWN_BLM_DATA_TYPE) )

    /// monType could be used to switch in the staticCallbackFunction
    /// For the blm this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)

    struct pvStruct
    {
        BLM_PV_TYPE    pvType;
        BLM_DATA_TYPE  blmDataType;
        chid           CHID;
        std::string    pvSuffix, objName;
        unsigned long  COUNT, MASK;
        chtype         CHTYPE;
    };

    struct blmTraceData
    {
        blmTraceData() : timeCH1ShotCount( UTL::ZERO_INT ),
                         timeCH2ShotCount( UTL::ZERO_INT ),
                         timeCH3ShotCount( UTL::ZERO_INT ),
                         timeCH4ShotCount( UTL::ZERO_INT ),
                         waveCH1ShotCount( UTL::ZERO_INT ),
                         waveCH2ShotCount( UTL::ZERO_INT ),
                         waveCH3ShotCount( UTL::ZERO_INT ),
                         waveCH4ShotCount( UTL::ZERO_INT ),
                         numShots( -2 ),
                         buffer( UTL::BUFFER_TEN ) {}
        std::string                          name, pvRoot;
        bool                                 isAContinuousMonitorStruct, isATemporaryMonitorStruct, isMonitoring;
        bool                                 isMonitoringWaveCH1, isMonitoringWaveCH2, isMonitoringWaveCH3, isMonitoringWaveCH4;
        bool                                 isMonitoringTimeCH1, isMonitoringTimeCH2, isMonitoringTimeCH3, isMonitoringTimeCH4;
        double                               timebase, noiseFloor;
        int                                  waveCH1ShotCount, waveCH2ShotCount, waveCH3ShotCount, waveCH4ShotCount, numShots; /// we allow -1 values here so NOT a size_t
        int                                  timeCH1ShotCount, timeCH2ShotCount, timeCH3ShotCount, timeCH4ShotCount;
        size_t                               buffer;
        std::vector< std::vector< double > > waveCH1TraceData, waveCH2TraceData, waveCH3TraceData, waveCH4TraceData;
        std::vector< std::vector< double > > timeCH1TraceData, timeCH2TraceData, timeCH3TraceData, timeCH4TraceData;
    #ifndef __CINT__
        std::map< BLM_PV_TYPE, std::vector< std::vector< double > > >            traceData;
        std::map< BLM_PV_TYPE, std::vector< double > >                           timeStamps;
        std::map< BLM_PV_TYPE, std::vector< std::string  > >                     strTimeStamps;
        std::map< BLM_PV_TYPE, int >                                             shotCounts;
        std::map< BLM_PV_TYPE, bool >                                            isMonitoringMap;
        std::map< BLM_PV_TYPE, boost::circular_buffer< std::vector< double > > > traceDataBuffer;
        std::map< BLM_PV_TYPE, pvStruct > pvMonStructs;
    #endif
    };

    struct blmObject
    {
        std::string name;
//        blmNumObject numObject;
        std::map< std::string, blmTraceData > traceObjects; /// There are four blm trace objects
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > iLockStates;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;

    };

    struct monitorStruct /// We use pointers when we wnat acces to the object (data) or... just  make a copy
    {
        BLM_PV_TYPE        monType;
        blmObject*         blmObject;
        std::string        objName;
        chtype             CHTYPE;
        void *             val;
        blmInterface *     interface;
        evid               EVID;
    };
}
#endif

