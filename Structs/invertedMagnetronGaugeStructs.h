#ifndef _VELA_VAC_IMG_STRUCTS_H_
#define _VELA_VAC_IMG_STRUCTS_H_
//
#include "structs.h"
//stl
#include <string>
#include <map>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class invertedMagnetronGaugeInterface;


namespace invertedMagnetronGaugeStructs
{
    /// Forward declare structs, gcc seems to like this...

    struct monitorStruct;
    struct vacImgObject;
    struct pvStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( IMG_PV_TYPE, (STA) (P) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( IMG_STATE, (IMG_BAD) (IMG_GOOD) (IMG_ERROR) (UNKNOWN_IMG_STATE) )
    enum dataCollectionResult {  waiting, collected, timeout };

    /// monType could be used to switch in the staticCallbackFunction
    /// For the qBox this is basically redundant, there is only one monitor: "Sta"
    /// (apart from interlocks, these are handled in the base class)

    struct monitorStruct
    {
        IMG_PV_TYPE                       monType;
        pvStruct *                        pvStr;
        vacImgObject *                    vacImgObj;
        void*                             obj;
        invertedMagnetronGaugeInterface * interface;
        chtype                            CHTYPE;
        evid                              EVID;
    };

    struct pvStruct
    {
        IMG_PV_TYPE         pvType;
        chid                CHID;
        std::string         pvSuffix;
        unsigned long       COUNT, MASK;
        chtype              CHTYPE;
        evid                EVID;
    };


    struct vacImgObject
    {

        std::string name, pvRoot;
        double p;
        std::vector< double > pVec, timeStamps;
        std::vector< std::string > strTimeStamps;
        size_t pCounter;
        IMG_STATE vacImgState;
        int numIlocks;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;

#ifndef __CINT__
        /// keep data seperate from epics stuff

        std::map< IMG_PV_TYPE, pvStruct > pvMonStructs;
        std::map< IMG_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
////
////        std::map< PIL_SHUTTER_PV_TYPE, chid > chidMap;
////        std::map< VELA_ENUM::ILOCK_NUMBER , chid > ilkChidMap;
#endif
    };
}
#endif
