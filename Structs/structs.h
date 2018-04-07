/*
//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
//
//  Author:      DJS
//  Last edit:   19-03-2018
//  FileName:    structs.h
//  Description: Structs that are used by multiple Hardware Controllers Go here
//               Plus the macros:
//                  DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE
//                  CALL_MEMBER_FN
//
//
//*/
#ifndef _HWC_STRUCTS_H_
#define _HWC_STRUCTS_H_
//stl includes
#include <string>
/*
    vela_clara_enums is a .pyd that contains all the
    enums that are shared amongst many HWC. In Python programmes
    that use many HWC it can(should) be imported first
    in every file so that only the vela_clara_enums namespace
    has the definitions. (making accesing these enums consistant
    throughout the python programme.
*/
#ifndef __VC_ENUM_ONLY__
// project includes
#include "configDefinitions.h"
//stl includes
#include <string>
#include <vector>
#include <map>
// epics includes
#include <cadef.h>

#endif // __VC_ENUM_ONLY__

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//                                                                            \
    /// This macro creates an enum and also enables us to                     \
    /// get strings of the entries with ENUM_TO_STRING                        \
//
#include <boost/preprocessor.hpp>
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
    case elem : return BOOST_PP_STRINGIZE(elem);
#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
    enum name {                                                               \
        BOOST_PP_SEQ_ENUM(enumerators)                                        \
    };                                                                        \
    inline  std::string ENUM_TO_STRING(name v)                                \
    {                                                                         \
        switch(v)                                                             \
        {                                                                     \
            BOOST_PP_SEQ_FOR_EACH(                                           \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
                name,                                                         \
                enumerators                                                   \
         )                                                                    \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                                     \
    }                                                                         \
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
/// Another MACRO (!) for member-function-pointers
/// https://isocpp.org/wiki/faq/pointers-to-members
#define CALL_MEMBER_FN(object,ptrToMember)((object).*(ptrToMember))

/*
    forward declare interface class,
    but NOT when compiling VELA_CLARA_enums
*/
#ifndef __VC_ENUM_ONLY__
class interface;
#endif // __VC_ENUM_ONLY__
namespace HWC_ENUM
{
    /* Which mode is the controller to be used for */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MACHINE_MODE, (OFFLINE)
                                                      (VIRTUAL)
                                                      (PHYSICAL)
                                                      (UNKNOWN_MACHINE_MODE)
                                        )
    /* Which area of the machine is the controller for */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MACHINE_AREA, (VELA_INJ)
                                                      (VELA_BA1)
                                                      (VELA_BA2)
                                                      (CLARA_PH1)
                                                      (CLARA_INJ)
                                                      (CLARA_2_VELA)
                                                      (CLARA_S01)
                                                      (CLARA_S02)
                                                      (CLARA_L01)
                                                      (UNKNOWN_AREA)
                                                      (RF_GUN)
                                                      (RF_L01)
                                                      (USER)
                                        )
    /* Controller type, defined in controller derived class */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CONTROLLER_TYPE,(MAGNET)
                                                        (LLRF)
                                                        (BPM)
                                                        (CAMERA_IA)
                                                        (CAMERA_DAQ)
                                                        (SCREEN)
                                                        (SCOPE)
                                                        (GENERAL_MONITOR)
                                                        (PI_LASER)
                                                        (PI_LASER_MIRROR)
                                                        (VIRTUAL_CATHODE_ANALYSIS)
                                                        (VAC_VALVES)
                                                        (SHUTTER)
                                                        (IMG)
                                                        (RF_MOD)
                                                        (RF_PROT)
                                                        (UNKNOWN_CONTROLLER_TYPE)
                                        )
    /* INTERLOCK STATES (LEGACY FROM WHEN WE THOUGHT HARDWARE WAS HARMONISED*/
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(ILOCK_NUMBER, (ILOCK_1)
                                                      (ILOCK_2)
                                                      (ILOCK_3)
                                                      (ILOCK_4)
                                                      (ILOCK_5)
                                                      (ILOCK_6)
                                                      (ILOCK_7)
                                                      (ILOCK_8)
                                                      (ILOCK_9)
                                                      (ILOCK_10)
                                                      (ILOCK_11)
                                                      (ILOCK_12)
                                                      (ILOCK_13)
                                                      (ILOCK_14)
                                                      (ILOCK_15)
                                                      (ILOCK_16)
                                                      (ILOCK_17)
                                                      (ILOCK_18)
                                                      (ILOCK_19)
                                                      (ILOCK_20)
                                                      (ILOCK_21)
                                                      (ILOCK_22)
                                                      (ILOCK_23)
                                                      (ILOCK_24)
                                                      (ILOCK_ERR)
                                    )
    /* State of interlock */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(ILOCK_STATE, (ILOCK_BAD)
                                                     (ILOCK_GOOD)
                                                     (ILOCK_ERROR)
                                        )

    /* Generic State */
    DEFINE_ENUM_WITH_STRING_CONVERSIONS(STATE, (BAD)
                                               (GOOD)
                                               (ERR)
                                               (ON)
                                               (OFF)
                                               (UNKNOWN)
                                        )

/*
    vela_clara_enums is a .pyd that contains all the
    enums that are shared amongst many HWC. In Python programmes
    that use many HWC it can(should) be imported first
    in every file so that only the vela_clara_enums namespace
    has the defintions. (making accesing this enums consistant
    throughout the python programme.
    Put all ENUMS ABOVE this commen and structs below
*/
#ifndef __VC_ENUM_ONLY__
    /*
        PV structs are used to set up the EPICS channels and subscriptions
        they should be held in a map keyed by an ILOCK_NUMBER
        for derived classes this info needs to go in the config file.

        see EPICS channel access manual:

        https://epics.anl.gov/base/R3-14/8-docs/CAref.html

        e.g:
        MASK: A mask with bits set for each of the event trigger types requested.
              The event trigger mask must be a bitwise or of one or more of the
              following constants.
              DBE_VALUE: Trigger events when the channel value exceeds
                         the monitor dead band
              DBE_LOG:   Trigger events when the channel value exceeds
                         the archival dead band
              DBE_ALARM: Trigger events when the channel alarm state changes.

    */
    struct iLockPVStruct
    {
        // ALWAYS proviude a default constructor
        iLockPVStruct():
            COUNT(UTL::ONE_SIZET),
            CHTYPE(DBR_ENUM),
            MASK(DBE_VALUE)
            {}
        chid                CHID;
        std::string         pv, objName;
        const unsigned long COUNT, MASK;
        const chtype        CHTYPE;
    };
    /*
        monitorStructs are recast as void* and passed to ca_create_supscription.
        Static callback functions can then recast the monitorStruct and get
        access to all data required to update the values, etc., from within the
        callback function.
    */
    struct iLockMonitorStruct
    {
        // ALWAYS provide a default constructor
        iLockMonitorStruct():
            objName(UTL::UNKNOWN_STRING),
            iLockState(nullptr),
            interface(nullptr),
            iLockNumber(ILOCK_ERR)
            {}
        std::string objName;
        ILOCK_NUMBER iLockNumber;
        ILOCK_STATE* iLockState;
        /* an interface pointer allows us to access interface class
           members from within the static callback function */
        interface* interface;
        evid       EVID;
    };

    struct epics_timestamp
    {
        epics_timestamp():
            time_ns(UTL::ZERO_DOUBLE),
            time_Str(UTL::EMPTY_STRING)
            {}
        double         time_ns;
        std::string    time_Str;
        epicsTimeStamp etime;
    };
#endif // __VC_ENUM_ONLY__
}
#endif // _HWC_STRUCTS_H_
//______________________________________________________________________________
//
// OLD STUFF TO BE COMMENTED OUT AT SOMEPOINT
//
//______________________________________________________________________________
//namespace HWC_ENUM /* seems like a really out of date name now */
//{
//    /* SHOULD GO IN THE SHUTTER CONTROLLER */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(SHUTTER_STATE, (SHUTTER_CLOSED)
//                                                       (SHUTTER_OPEN)
//                                                       (SHUTTER_TIMING)
//                                                       (SHUTTER_ERROR)
//                                        )
//    /* Which mode is the controller to be used for */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MACHINE_MODE, (OFFLINE)
//                                                      (VIRTUAL)
//                                                      (PHYSICAL)
//                                                      (UNKNOWN_MACHINE_MODE)
//                                        )
//    /* Which area of the machine is the controller for */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MACHINE_AREA, (VELA_INJ)
//                                                      (VELA_BA1)
//                                                      (VELA_BA2)
//                                                      (CLARA_PH1)
//                                                      (CLARA_INJ)
//                                                      (CLARA_2_VELA)
//                                                      (CLARA_S01)
//                                                      (CLARA_S02)
//                                                      (CLARA_L01)
//                                                      (UNKNOWN_AREA)
//                                                      (USER)
//                                        )
//    /* Controller type, defined in controller derived class */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(CONTROLLER_TYPE,(MAGNET)
//                                                        (LLRF)
//                                                        (BPM)
//                                                        (CAMERA_IA)
//                                                        (CAMERA_DAQ)
//                                                        (SCREEN)
//                                                        (SCOPE)
//                                                        (GENERAL_MONITOR)
//                                                        (PI_LASER)
//                                                        (VAC_VALVES)
//                                                        (SHUTTER)
//                                                        (IMG)
//                                                        (RF_MOD)
//                                                        (RF_PROT)
//                                                        (UNKNOWN_CONTROLLER_TYPE)
//                                        )
//    /* SHOULD GO IN THE MAGNET CONTROLLER */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(MAG_PSU_STATE, (MAG_PSU_OFF)
//                                        (MAG_PSU_ON)
//                                        (MAG_PSU_TIMING)
//                                        (MAG_PSU_ERROR)
//                                        (MAG_PSU_NONE)
//                                        )
//    /* SHOULD GO IN THE ... ALEX BRYNES ??? */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(DIAG_TYPE, (WCM)
//                                                   (ICT1)
//                                                   (ED_FCUP)
//                                                   (ICT2)
//                                                   (FCUP)
//                                                   (UNKNOWN_DIAG_TYPE)
//                                        )
//    /* SHOULD NOT EXIST */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(QBOX_STATE, (QBOX_STOP)
//                                                    (QBOX_RUN)
//                                                    (QBOX_TIMING)
//                                                    (QBOX_ERROR)
//                                        )
//    /* SHOULD NOT EXIST */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(TRIG_STATE, (TRIG)
//                                                    (NOTRIG)
//                                                    (TRIG_ERROR)
//                                        )
//    /* SHOULD GO IN TH EVLAVE CONTORLLER */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(VALVE_STATE, (VALVE_CLOSED)
//                                                     (VALVE_OPEN)
//                                                     (VALVE_TIMING)
//                                                     (VALVE_ERROR)
//                                        )
//    /* INTERLOCK STATES (LEGACY FROM WHEN THOUGHT HARDWARE WAS HARMONISED*/
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(ILOCK_NUMBER, (ILOCK_1)
//                                                      (ILOCK_2)
//                                                      (ILOCK_3)
//                                                      (ILOCK_4)
//                                                      (ILOCK_5)
//                                                      (ILOCK_6)
//                                                      (ILOCK_7)
//                                                      (ILOCK_8)
//                                                      (ILOCK_9)
//                                                      (ILOCK_10)
//                                                      (ILOCK_11)
//                                                      (ILOCK_12)
//                                                      (ILOCK_13)
//                                                      (ILOCK_14)
//                                                      (ILOCK_15)
//                                                      (ILOCK_16)
//                                                      (ILOCK_17)
//                                                      (ILOCK_18)
//                                                      (ILOCK_19)
//                                                      (ILOCK_20)
//                                                      (ILOCK_21)
//                                                      (ILOCK_22)
//                                                      (ILOCK_23)
//                                                      (ILOCK_24)
//                                                      (ILOCK_ERR)
//                                    )
//    /* State of interlock */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(ILOCK_STATE, (ILOCK_BAD)
//                                                     (ILOCK_GOOD)
//                                                     (ILOCK_ERROR)
//                                        )
//
//    /* Generic State */
//    DEFINE_ENUM_WITH_STRING_CONVERSIONS(STATE, (BAD)
//                                               (GOOD)
//                                               (ERR)
//                                               (UNKNOWN)
//                                        )
//    /*
//        PV structs are used to set up the EPICS channels and subscriptions
//        they should be held in a map keyed by an ILOCK_NUMBER
//        for derived classes this info needs to go in the config file.
//
//        see EPICS channel access manual:
//
//        https://epics.anl.gov/base/R3-14/8-docs/CAref.html
//
//        e.g:
//        MASK: A mask with bits set for each of the event trigger types requested.
//              The event trigger mask must be a bitwise or of one or more of the
//              following constants.
//              DBE_VALUE: Trigger events when the channel value exceeds
//                         the monitor dead band
//              DBE_LOG:   Trigger events when the channel value exceeds
//                         the archival dead band
//              DBE_ALARM: Trigger events when the channel alarm state changes.
//
//    */
//    struct iLockPVStruct
//    {
//        // ALWAYS proviude a default constructor
//        iLockPVStruct():
//            COUNT(UTL::ONE_SIZET),
//            CHTYPE(DBR_ENUM),
//            MASK(DBE_VALUE)
//            {}
//        chid                CHID;
//        std::string         pv, objName;
//        const unsigned long COUNT, MASK;
//        const chtype        CHTYPE;
//    };
//    /*
//        monitorStructs are recast as void* and passed to ca_create_supscription
//        static callback functions can then recast the monitorStruct and get
//        access to all data required to update the values, etc., from within the
//        callback functions
//    */
//    struct iLockMonitorStruct
//    {
//        // ALWAYS proviude a default constructor
//        iLockMonitorStruct():
//            objName(UTL::UNKNOWN_STRING),
//            iLockState(nullptr),
//            interface(nullptr),
//            iLockNumber(ILOCK_ERR)
//            {}
//        std::string objName;
//        ILOCK_NUMBER iLockNumber;
//        ILOCK_STATE* iLockState;
//        /* an interface pointer allows us to access interface class
//           members from within the callback function */
//        interface* interface;
//        evid                EVID;
//    };
//}
//#endif // _HWC_STRUCTS_H_
