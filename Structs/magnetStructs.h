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
    struct nrPSUObject;
    struct magnetStateStruct;
    struct monitorStruct;

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MAG_TYPE, (QUAD) (DIP) (HCOR) (VCOR) (BSOL) (SOL) (SEXT) (UNKNOWN_MAGNET_TYPE) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MAG_REV_TYPE, (NR) (BIPOLAR) (NR_GANGED) (POS) (UNKNOWN_MAG_REV_TYPE) ) /// Yeah NR_GANGED, just when you thought it was already too complicated

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MAG_PV_TYPE, (SI) (RI) (Sta) (On) (Off) (UNKOWN_MAG_PV_TYPE) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MAG_PSU_TYPE, (PSU) (PSU_N) (PSU_R) (UNKOWN_MAG_PSU_TYPE) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MAG_CONTROLLER_TYPE, (VELA_INJ) (VELA_BA1) (VELA_BA2) (CLARA_INJ)(UNKNOWN_CONTROLLER_TYPE) )

    /// These can't go in VELA_ENUM as they need a pvType.
    struct pvStruct
    {   // proviude a default constructor
        pvStruct() : pvSuffix( "UNKNOWN_PV_SUFFIX" ), objName( "UNKNOWN_OBJECT_NAME"),COUNT( UTL::ZERO_INT ), MASK(UTL::ZERO_INT), pvType(UNKOWN_MAG_PV_TYPE) {} // proviude a default constructor
        MAG_PV_TYPE   pvType;
        chid          CHID;
        std::string   pvSuffix, objName;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
    };


    struct  nrPSUObject
    {   // proviude a default constructor
        nrPSUObject() : isGanged( false ), parentMagnet( "UNKNOWN_PARENT_MAGNET" ), pvRoot( UTL::UNKNOWN_PVROOT),
                        psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),
                        numIlocks(UTL::ZERO_INT ) {} // proviude a default constructor
        std::string  parentMagnet, pvRoot;
        bool isGanged;//, canFlip;/// canflip? probably refactor as function...
        VELA_ENUM::MAG_PSU_STATE psuState;
        size_t numIlocks;
        std::vector< std::string > gangMembers;
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    #endif
    };

    struct  magnetObject
    {   // proviude a default constructor
        magnetObject() : magType (MAG_TYPE::UNKNOWN_MAGNET_TYPE), isGanged( false ), name("UNKNOWN_NAME"),pvRoot( UTL::UNKNOWN_PVROOT),
                psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),canNRFlip( false ),samePSURoot( false ),
                psuRoot("PROBABLY_SAME_AS_PV_ROOT"),
                magRevType( MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE ),
                si(UTL::DUMMY_DOUBLE), ri(UTL::DUMMY_DOUBLE), siWithPol(UTL::DUMMY_DOUBLE), riWithPol(UTL::DUMMY_DOUBLE), riTolerance(UTL::DUMMY_DOUBLE),
                /// err... , an atomic_bool for isDegaussing( false ) does not work ... http://stackoverflow.com/questions/15750917/initializing-stdatomic-bool
                /// ... which is probably evil && dangerous
                numIlocks( UTL::ZERO_INT ),degTolerance(0.1),//MAGIC_NUMBER
                //added deguassing initialisers here
                numDegaussSteps(UTL::ZERO_INT), maxWaitTime(UTL::ZERO_INT), numDegaussElements(UTL::ZERO_INT),
                magneticLength(UTL::ZERO_DOUBLE),position(UTL::ZERO_DOUBLE),
                manufacturer("UNKNOWN_MANUFACTURER"), serialNumber("UNKNOWN_SERIAL_NUMBER"),
                magnetBranch("UNKNOWN_MAGNET_BRANCH"),
                measurementDataLocation("UNKNOWN_MEASUREMENT_DATA_LOCATION") {} // proviude a default constructor
        MAG_TYPE magType;           /// dipole, quad etc.
        MAG_REV_TYPE  magRevType;   /// reverse type, NR, bipolar etc.
        VELA_ENUM::MAG_PSU_STATE psuState;
        size_t numIlocks,numDegaussSteps, maxWaitTime, numDegaussElements;
        nrPSUObject nPSU, rPSU;
        bool isGanged, canNRFlip, samePSURoot;
        std::vector< std::string > gangMembers;
        double si, ri, siWithPol, riWithPol, riTolerance,position,magneticLength,degTolerance;
        std::string name, pvRoot, psuRoot, manufacturer, serialNumber, measurementDataLocation,magnetBranch;
        std::vector< double > degValues, fieldIntegralCoefficients;
//        std::atomic< bool > isDegaussing;/// NO thread safe copy constructor malarkey...  http://stackoverflow.com/questions/29332897/error-c2280-attempting-to-reference-a-deleted-function-atomicint
        std::map< VELA_ENUM::ILOCK_NUMBER , VELA_ENUM::ILOCK_STATE > iLockStates;
    #ifndef __CINT__
        std::map< MAG_PV_TYPE, pvStruct > pvMonStructs;
        std::map< MAG_PV_TYPE, pvStruct > pvComStructs;
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::iLockPVStruct > iLockPVStructs;
    #endif
    };

    struct monitorStruct
    {   // proviude a default constructor
        monitorStruct(): monType(UNKOWN_MAG_PV_TYPE),objName("UNKNOWN"),
                         psuType( UNKOWN_MAG_PSU_TYPE ),interface( nullptr ),EVID( nullptr ){}
        MAG_PV_TYPE      monType;
        std::string          objName;
        chtype               CHTYPE;
        MAG_PSU_TYPE         psuType;
        magnetInterface *interface;
        evid                 EVID;
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
                        resetToZero(true),controllerType(UNKNOWN_CONTROLLER_TYPE){}
        magnetInterface          *interface;
        std::vector<std::string>  magsToDeguass;
        std::thread              *thread;
        size_t                    key;
        bool                      resetToZero;
        MAG_CONTROLLER_TYPE       controllerType;
    };
    /// one-stop shop for magnet state
    struct magnetStateStruct
    {   // proviude a default constructor
        magnetStateStruct():numMags(UTL::ZERO_INT),controllerType(UNKNOWN_CONTROLLER_TYPE){};
        size_t numMags;
        MAG_CONTROLLER_TYPE controllerType;
        std::vector< std::string > magNames;
        std::vector< VELA_ENUM::MAG_PSU_STATE > psuStates;
        std::vector< double > siValues,riValues;
    };
}
#endif // _VELA_MAGNET_STRUCTS_H_
