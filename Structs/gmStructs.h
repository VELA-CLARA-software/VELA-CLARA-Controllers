#ifndef _VELA_CLARA_GM_STRUCTS_H_
#define _VELA_CLARA_GM_STRUCTS_H_
//
#include "structs.h"
#include "configDefinitions.h"
#include "running_stat.h"
//stl
#include <string>
#include <vector>
#include <map>
#include <utility>
//epics
#include <cadef.h>
#include <epicsTime.h>

//
#include "configDefinitions.h"

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
    {   dataEntry():
            c(UTL::ZERO_SIZET),
            rs_count(UTL::ZERO_SIZET),
            rs_count_max(UTL::ZERO_SIZET),
            rs_complete(false)
        {}
        epicsTimeStamp t; // epics timestamp for value
        double         s; // epics timestamp converted into nano-sec
        size_t         c; // count, how mnay times has epics updated this value
        T              v; // the actual value EPICS is to update
        runningStat    rs; //running_stat
        size_t         rs_count; //running_stat current count of entires used in running stat
        size_t         rs_count_max; //running_stat max count of entires to be used in running stat
        bool           rs_complete; // flag for when rs_count == rs_count_max
        std::vector<std::pair<std::string,T>> buffer;// a buffer of values up to size = rs_count_max
    };
    // monType could be used to switch in the staticCallbackFunction
    template <typename T>
    struct pvData
    {
        pvData():
            id("UNKNOWN"),
            fully_connected(false)
            {}
        pvStruct                   pvs;
        std::vector<dataEntry<T>*> data;
        std::string                id;
        bool                       fully_connected;
    };
    // this gets sent to the callback function, and from the ID and interface everything can be
    // updated
    struct monitorStruct
    {
        monitorStruct():id("UNKNOWN"),gm(nullptr){}
        VCgeneralMonitor* gm;
        std::string       id;
    };
}
#endif
