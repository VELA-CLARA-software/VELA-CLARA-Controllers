///
/// Duncan Scott August 2015
///
/// Holds all the structs required by different classes
/// one place to make updates
///
#ifndef _VELA_MAGNET_STRUCTS_H_
#define _VELA_MAGNET_STRUCTS_H_
//proj
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <vector>
#include <string>
#include <map>
#include <thread>
//epics
#ifndef __CINT__
#include <cadef.h>
#endif

class magnetInterface;


namespace magnetStructs
{
    struct degaussStruct;
    struct magnetObject;
    struct pvStruct;
    //struct nrPSUObject;
    struct magnetStateStruct;
    struct monitorStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MAG_TYPE, (QUAD) (DIP) (HCOR) (VCOR) (BSOL) (SOL) (SEXT)
                                                  (UNKNOWN_MAGNET_TYPE))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MAG_REV_TYPE, (NR) (BIPOLAR) (NR_GANGED) (POS) (UNKNOWN_MAG_REV_TYPE))
    /// Yeah NR_GANGED, just when you thought it was already too complicated
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MAG_PV_TYPE, (SETI) (GETSETI) (READI) (RPOWER) (SPOWER) (RILK)
                                                     (UNKNOWN_MAG_PV_TYPE))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MAG_PSU_STATE,   (ON)   (OFF) (ERROR) (NONE))
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MAG_ILOCK_STATE, (GOOD) (BAD))
    /// These can't go in VELA_ENUM as they need a pvType.
    struct pvStruct
    {   // proviude a default constructor
        pvStruct() : pvSuffix("UNKNOWN_PV_SUFFIX"), objName(UTL::UNKNOWN_NAME),COUNT(UTL::ZERO_INT), MASK(UTL::ZERO_INT), pvType(UNKNOWN_MAG_PV_TYPE) {}
        MAG_PV_TYPE   pvType;
        chid          CHID;
        std::string   pvSuffix, objName;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
    };

    // initialisation values for magnetObject
    const std::string SAME_AS_PV_ROOT = "SAME_AS_PV_ROOT";
    const std::string UNKNOWN_MANUFACTURER = "UNKNOWN_MANUFACTURER";
    const std::string UNKNOWN_SERIAL_NUMBER = "UNKNOWN_SERIAL_NUMBER";
    const std::string UNKNOWN_MAGNET_BRANCH = "UNKNOWN_MAGNET_BRANCH";
    const std::string UNKNOWN_MEASUREMENT_DATA_LOCATION = "UNKNOWN_MEASUREMENT_DATA_LOCATION";

    struct  magnetObject
    {
        magnetObject():magType(MAG_TYPE::UNKNOWN_MAGNET_TYPE),
                       revType(MAG_REV_TYPE::BIPOLAR),
                       machineArea(VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA),
                       numDegaussSteps(UTL::ZERO_INT),
                       maxWaitTime(UTL::ZERO_INT),
                       numDegaussElements(UTL::ZERO_INT),
                       magneticLength(UTL::ZERO_DOUBLE),
                       position(UTL::ZERO_DOUBLE),
                       manufacturer(UNKNOWN_MANUFACTURER),
                       serialNumber(UNKNOWN_SERIAL_NUMBER),
                       magnetBranch(UNKNOWN_MAGNET_BRANCH),
                       measurementDataLocation(UNKNOWN_MEASUREMENT_DATA_LOCATION),
                       siWithPol(UTL::DUMMY_DOUBLE),
                       riWithPol(UTL::DUMMY_DOUBLE),
                       riTolerance(UTL::DUMMY_DOUBLE),
                       maxI(UTL::DUMMY_DOUBLE),
                       minI(UTL::DUMMY_DOUBLE),
                       psuRoot(SAME_AS_PV_ROOT),
                       SETIequalREADI(false)
                       {}
        MAG_TYPE magType;           /// dipole, quad etc.
        MAG_PSU_STATE psuState;
        VELA_ENUM::MACHINE_AREA  machineArea;
        MAG_REV_TYPE revType;
        bool SETIequalREADI;
        double siWithPol,    // this is the GETSI (i.e. read-only) value in controls 2017 scheme that is consistent between VELA / CALRA
               setsiWithPol, // use this value in the controller to actually set the SI  (i.e. write-only)
               riWithPol, riTolerance, position, magneticLength, degTolerance, maxI, minI;
        std::string name, pvRoot, psuRoot, manufacturer, serialNumber, measurementDataLocation,magnetBranch;
        std::vector< double > degValues, fieldIntegralCoefficients;
        size_t numIlocks,numDegaussSteps, maxWaitTime, numDegaussElements;
        MAG_ILOCK_STATE iLock;
        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
    };

    struct monitorStruct
    {   // proviude a default constructor
        monitorStruct(): monType(UNKNOWN_MAG_PV_TYPE),
                         objName("UNKNOWN"),
                         interface(nullptr),EVID(nullptr){}
        MAG_PV_TYPE      monType;
        std::string      objName;
        chtype           CHTYPE;
        magnetInterface* interface;
        evid             EVID;
    };

    /// This holds all offline info about degaussing
    struct degaussValues
    {   // proviude a default constructor
        degaussValues():numDegaussSteps(UTL::ZERO_INT), maxWaitTime(UTL::ZERO_INT), numDegaussElements(0){}//MAGIC_NUMBER
        size_t numDegaussSteps, maxWaitTime, numDegaussElements;
        std::map< std::string, std::vector< double > > degValues;
        std::map< std::string, double > degTolerance;
    };
    /// This holds all info for actually degaussing some magnets, passed into new thread when degaussing
    struct degaussStruct
    {   // proviude a default constructor
        degaussStruct():interface(nullptr),thread(nullptr),key(UTL::ZERO_INT),
                        resetToZero(true),machineArea(VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA){}
        magnetInterface          *interface;
        std::vector<std::string>  magsToDeguass;
        std::thread              *thread;
        size_t                    key;
        bool                      resetToZero;
        VELA_ENUM::MACHINE_AREA   machineArea;
    };
    /// one-stop shop for magnet state
    struct magnetStateStruct
    {   // proviude a default constructor
        magnetStateStruct():numMags(UTL::ZERO_INT),machineArea(VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA){};
        size_t numMags;
        VELA_ENUM::MACHINE_AREA machineArea;
        std::vector< std::string > magNames;
        std::vector< MAG_PSU_STATE > psuStates;
        std::vector< double > siValues,riValues;
    };
}
#endif // _VELA_MAGNET_STRUCTS_H_
