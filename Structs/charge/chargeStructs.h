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

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( CHARGE_DIAG_TYPE, (WCM) (VELA_ICT1) (ED_FCUP) (VELA_ICT2) (S02_FCUP) (SP1_FCUP) (UNKNOWN_CHARGE_DIAG) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( CHARGE_PV_TYPE, (V) (Q) (UNKNOWN_CHARGE_PV) )

    /// monType could be used to switch in the staticCallbackFunction
    /// For the charge this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)

    struct pvStruct
    {
        CHARGE_DIAG_TYPE diagType;
        CHARGE_PV_TYPE   pvType;
        chid             CHID;
        std::string      pvSuffix, objName;
        unsigned long    COUNT, MASK;
        chtype           CHTYPE;
    };

    struct dataObject
    {
        dataObject() :                   shotCounts( -2 ),
                                         wcm( UTL::DUMMY_DOUBLE ),
                                         s02FCUP( UTL::DUMMY_DOUBLE ),
                                         numShots( UTL::ZERO_INT ),
                                         buffer( UTL::BUFFER_TEN ) {}
        std::string                      name, pvRoot;
        CHARGE_DIAG_TYPE                 diagType;
        bool                             isAContinuousMonitorStruct, isATemporaryMonitorStruct;
        bool                             isMonitoring;
        double                           wcm, s02FCUP, charge, voltage;
        int                              numShots, shotCounts;
        double                           timeStamp;
        size_t                           buffer;
        std::vector< double >            chargeVec, voltageVec;
        boost::circular_buffer< double > chargeBuffer, voltageBuffer;
        std::vector< double >            timeStamps;
        std::vector< std::string >       strTimeStamps;
    #ifndef __CINT__
        std::map< CHARGE_PV_TYPE, pvStruct > pvMonStructs;
        std::map< CHARGE_PV_TYPE, pvStruct > pvComStructs;
    #endif
    };

    struct chargeObject
    {
        std::string name;
//        chargeNumObject numObject;
        std::map< std::string, dataObject > dataObjects;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > iLockStates;
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::iLockPVStruct > iLockPVStructs;
    };

    struct monitorStruct /// We use pointers when we wnat acces to the object (data) or... just  make a copy
    {
        CHARGE_PV_TYPE    monType;
        CHARGE_DIAG_TYPE  diagType;
        chargeObject*     chargeObject;
        std::string       name;
        chtype            CHTYPE;
        void *            val;
        chargeInterface * interface;
        evid              EVID;
    };
}
#endif

