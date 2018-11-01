///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef CONFIG_READER_PyIL_SHUTTER_H
#define CONFIG_READER_PyIL_SHUTTER_H
// stl
#include <string>
#include <vector>
#include <map>
// me
#include "configReader.h"
#include "llrfStructs.h"


class liberallrfConfigReader:public configReader
{
    public:
        liberallrfConfigReader(const std::string & llrfFileLocation1,
                               const bool startVirtualMachine,
                               const bool& show_messages,
                               const bool& show_debug_messages);
        ~liberallrfConfigReader();

        bool readConfig();

        bool getliberallrfObject(llrfStructs::liberallrfObject& obj);


        void setType(const llrfStructs::LLRF_TYPE typein);

    private:


        llrfStructs::LLRF_TYPE type;

        //int channel;
        std::string temp_name, temp_pvroot;

        llrfStructs::liberallrfObject llrfObj;
        // these structs are temporary, they are combined into pvMonStructs, pvComStructs after reading all files
        std::vector<llrfStructs::pvStruct> pvOneTraceMonStructs;
        std::vector<llrfStructs::pvStruct> pvOneTraceComStructs;

        std::vector<llrfStructs::pvStruct> pvSCANMonStructs;
        std::vector<llrfStructs::pvStruct> pvSCANComStructs;

        std::vector<llrfStructs::pvStruct> pvMonStructs;
        std::vector<llrfStructs::pvStruct> pvComStructs;


        void addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtype,const std::string& pvSuffix);
        void addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtype);

        llrfStructs::LLRF_PV_TYPE getPVType(const llrfStructs::LLRF_PV_TYPE main, const llrfStructs::LLRF_PV_TYPE sub);

        void addToliberallrfObjectsV1 (const std::vector<std::string>& keyVal);


//        void addToTraceObjectsV1(const std::vector<std::string> &keyVal);
        void addToPVCommandMapV1(const std::vector<std::string>& keyVal);
        void addToPVMonitorMapV1(const std::vector<std::string>& keyVal);
//        void addToTracePVMap(const std::vector<std::string>& keyVal);

        void addToPVMapV1(std::vector<llrfStructs::pvStruct>& pvs, const std::vector<std::string>& keyVal);


        bool readConfig(const std::string & configFile1);

        llrfStructs::pvStruct* lastPVstruct;

//        bool readTraceConfig();

        /*
            different flags for which config file is being read
        */
        bool readingMainRF;
        bool readingOneTrace;
        bool readingTraceSCAN;


        bool add_trace_type;
        bool traceMatches_LLRF_TYPE(const std::string& val) const;

        const bool usingVirtualMachine;


        bool getOneTraceConfig();
        bool getSCANConfig();

        void addToMainObj(const std::vector<std::string>& keyVal);
        void addToOneTrace(const std::vector<std::string>& keyVal);
        void addToSCAN(const std::vector<std::string>& keyVal);


        llrfStructs::scan* lastScanStruct;
        llrfStructs::one_trace_info* lastOneTraceInfoStruct;


};
#endif //UTL_FILE_IO_H
