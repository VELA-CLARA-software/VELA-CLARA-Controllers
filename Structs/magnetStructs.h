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


    /// These can't go in VELA_ENUM as they need a pvType.
    struct pvStruct
    {   // proviude a default constructor
        pvStruct() : pvSuffix( "UNKNOWN" ), objName( "UNKNOWN"),COUNT( 0 ), MASK(0), pvType(UNKOWN_MAG_PV_TYPE) {} // proviude a default constructor
        MAG_PV_TYPE   pvType;
        chid          CHID;
        std::string   pvSuffix, objName;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
    };


    struct  nrPSUObject
    {   // proviude a default constructor
        nrPSUObject() : isGanged( false ), parentMagnet( "UNKNOWN" ), pvRoot( "UNKNOWN"),
                        psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),
                        numIlocks( 0 ) {} // proviude a default constructor
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
        magnetObject() : magType (MAG_TYPE::UNKNOWN_MAGNET_TYPE), isGanged( false ), name("UNKNOWN"),pvRoot( "UNKNOWN"),
                psuState( VELA_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR ),canNRFlip( false ),samePSURoot( false ),
                magRevType( MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE ),
                si(-999.999), ri(-999.999), siWithPol(-999.999), riWithPol(-999.999), riTolerance(-999.999),
                /// err... , an atomic_bool for isDegaussing( false ) does not work ... http://stackoverflow.com/questions/15750917/initializing-stdatomic-bool
                /// ... which is probably evil && dangerous
                numIlocks( 0 ),
                //added deguassing initialisers here
                numDegaussSteps(0), maxWaitTime(0), numDegaussElements(0) {} // proviude a default constructor
        MAG_TYPE magType;           /// dipole, quad etc.
        MAG_REV_TYPE  magRevType;   /// reverse type, NR, bipolar etc.
        VELA_ENUM::MAG_PSU_STATE psuState;
        size_t numIlocks;
        nrPSUObject nPSU, rPSU;
        std::string  name, pvRoot, psuRoot;
        bool isGanged, canNRFlip, samePSURoot;
        std::vector< std::string > gangMembers;
        double si, ri, siWithPol, riWithPol, riTolerance,position,slope,intercept;

        //DEGUASSING: added here by Tim Price

        size_t numDegaussSteps, maxWaitTime, numDegaussElements;
        std::vector< double > degValues;
        double  degTolerance;

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
        degaussValues():numDegaussSteps(0), maxWaitTime(0), numDegaussElements(0){}
        size_t numDegaussSteps, maxWaitTime, numDegaussElements;
        std::map< std::string, std::vector< double > > degValues;
        std::map< std::string, double > degTolerance;
    };
    /// This holds all info for actually degaussing some magnets, passed into new thread when degaussing
    struct degaussStruct
    {   // proviude a default constructor
        degaussStruct():interface(nullptr),thread(nullptr),key(0),resetToZero(true){}
        magnetInterface          *interface;
        std::vector< std::string > magsToDeguass;
        std::thread               *thread;
        size_t                     key;
        bool                       resetToZero;
    };
    /// one-stop shop for magnet state
    struct magnetStateStruct
    {   // proviude a default constructor
        magnetStateStruct():numMags(0) {};
        size_t numMags;
        std::vector< std::string > magNames;
        std::vector< VELA_ENUM::MAG_PSU_STATE > psuStates;
        std::vector< double > siValues,riValues;
    };
}
#endif // _VELA_MAGNET_STRUCTS_H_
