#ifndef _VELA_CLARA_PIL_STRUCTS_H_
#define _VELA_CLARA_PIL_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
//stl
#include <string>
#include <vector>
#include <map>
//epics
#include <cadef.h>
#include <epicsTime.h>

class  VCgeneralMonitor;

namespace gmStructs
{
    // Forward declare structs, gcc seems to like this...
    struct monitorStuct;

    struct pvStruct;
    // The hardware object holds a map keyed by PV type, with pvStruct values, some values come from the config
    // The rest are paramaters passed to EPICS, ca_create_channel, ca_create_subscription etc..
    struct pvStruct
    {
        pvStruct():pvFullName("UNKNOWN"),id("UNKNOWN"),COUNT(1),
                   MASK(DBE_VALUE),CHIDConnected(false),MonitorConnected(false),isArrayPV(false){}//MAGIC_NUMBER
        chid          CHID;
        std::string   pvFullName,id;
        unsigned long COUNT, MASK;
        chtype        CHTYPE;
        evid          EVID;
        bool          CHIDConnected,MonitorConnected,isArrayPV;
    };
    template <typename T>
    struct dataEntry
    {   //dataEntry()//defaults ?
        epicsTimeStamp t;
        T  v;
    };
    // monType could be used to switch in the staticCallbackFunction
    // For the shutter this is basically redundant, there is only one monitor: "Sta"
    // (apart from interlocks, these are handled in the base class)
    // monType could be used to switch in the statisCallbackFunction
    template <typename T>
    struct pvData
    {
        pvData():id("UNKNOWN"),fully_connected(false){}
        pvStruct                   pvs;
        std::vector<dataEntry<T>*> data;
        std::string                id;
        bool                       fully_connected;

    };
    // this gets sent t to the callback functionk, adn from the ID and interfcace everything can be
    // updated
    struct monitorStruct
    {
        monitorStruct():id("UNKNOWN"),gm(nullptr){}
        VCgeneralMonitor* gm;
        std::string       id;
    };
}
#endif
