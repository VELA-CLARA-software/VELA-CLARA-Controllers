/// Structs that are used by multiple Hardware Controllers Go here
/// Plus the macros ...

#ifndef _VELA_ENUM_H_
#define _VELA_ENUM_H_
#include <string>
//stl
#include <vector>
// epics
#include <cadef.h>
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//                                                                            \
    /// This macro creates an enum and also enables us to                     \
    /// get strings of the entries with ENUM_TO_STRING                        \
//
#include <boost/preprocessor.hpp>
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
    case elem : return BOOST_PP_STRINGIZE( elem );
#define DEFINE_ENUM_WITH_STRING_CONVERSIONS( name, enumerators )              \
    enum name {                                                               \
        BOOST_PP_SEQ_ENUM( enumerators )                                      \
    };                                                                        \
    inline  std::string ENUM_TO_STRING( name v )                              \
    {                                                                         \
        switch( v )                                                           \
        {                                                                     \
            BOOST_PP_SEQ_FOR_EACH(                                            \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
                name,                                                         \
                enumerators                                                   \
            )                                                                 \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                                     \
    }                                                                         \
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
/// Another MACRO (!) for member-function-pointers
/// https://isocpp.org/wiki/faq/pointers-to-members

#define CALL_MEMBER_FN(object,ptrToMember)((object).*(ptrToMember))

class interface;

namespace VELA_ENUM/// this seems like a really out of date name now...
{
   /// >>>> ???
   /// SHould these go here? should the names be rationalised?

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( SHUTTER_STATE, (SHUTTER_CLOSED) (SHUTTER_OPEN) (SHUTTER_TIMING) (SHUTTER_ERROR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MACHINE_MODE, (OFFLINE ) (VIRTUAL ) (PHYSICAL) )
    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MACHINE_AREA, (VELA_INJ) (VELA_BA1) (VELA_BA2) (CLARA_PH1)
                                                       (CLARA_INJ) (CLARA_2_VELA) (CLARA_S01) (CLARA_S02) (UNKNOWN_AREA) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( MAG_PSU_STATE, (MAG_PSU_OFF) (MAG_PSU_ON) (MAG_PSU_TIMING) (MAG_PSU_ERROR) (MAG_PSU_NONE) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( DIAG_TYPE, (WCM) (ICT1) (ED_FCUP) (ICT2) (FCUP) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( QBOX_STATE, (QBOX_STOP) (QBOX_RUN) (QBOX_TIMING) (QBOX_ERROR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( TRIG_STATE, (TRIG) (NOTRIG) (TRIG_ERROR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( VALVE_STATE, (VALVE_CLOSED) (VALVE_OPEN) (VALVE_TIMING) (VALVE_ERROR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( IMG_STATE, (IMG_BAD) (IMG_GOOD) (IMG_ERROR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( ILOCK_NUMBER, (ILOCK_1 ) (ILOCK_2 ) (ILOCK_3 ) (ILOCK_4 ) (ILOCK_5 ) (ILOCK_6 ) (ILOCK_7 )
                                                       (ILOCK_8 ) (ILOCK_9 ) (ILOCK_10) (ILOCK_11) (ILOCK_12) (ILOCK_13) (ILOCK_14)
                                                       (ILOCK_15) (ILOCK_16) (ILOCK_17) (ILOCK_18) (ILOCK_19) (ILOCK_20) (ILOCK_21)
                                                       (ILOCK_22) (ILOCK_23) (ILOCK_24) (ILOCK_ERR) )

    DEFINE_ENUM_WITH_STRING_CONVERSIONS( ILOCK_STATE, (ILOCK_BAD) (ILOCK_GOOD) (ILOCK_ERROR) )



    /// PV stricts are used to set up the channels and subscriptions
    /// they should be held in a map keyed by an ILOCK_NUMBER
    /// for derived calsses this info needs to go in the config file
    //MASK
    //A mask with bits set for each of the event trigger types requested. The event trigger mask must be a bitwise or of one or more of the following constants.
    //DBE_VALUE - Trigger events when the channel value exceeds the monitor dead band
    //DBE_LOG - Trigger events when the channel value exceeds the archival dead band
    //DBE_ALARM - Trigger events when the channel alarm state changes.

    struct iLockPVStruct
    {
        iLockPVStruct() : COUNT(1), CHTYPE( DBR_ENUM ), MASK( DBE_VALUE ) {} // proviude a default constructor
        chid                CHID;
        std::string         pv, objName;
        const unsigned long COUNT, MASK;
        const chtype        CHTYPE;
    };

    /// these are passed to ca_create_supscription callback functions

    struct iLockMonitorStruct
    {
        iLockMonitorStruct() : objName("UNKNOWN"), iLockState(nullptr), interface(nullptr), iLockNumber( ILOCK_ERR ) {} // proviude a default constructor
        std::string objName;
        VELA_ENUM::ILOCK_NUMBER iLockNumber;  /// Used in debugMessage;
        VELA_ENUM::ILOCK_STATE  * iLockState; /// this value comes from the map in the derived hardware class
        interface * interface;            /// this allows us to access hardware base class members, e.g. debugMessage;
        evid                EVID;
    };
}
#endif // _VELA_ENUM_H_
