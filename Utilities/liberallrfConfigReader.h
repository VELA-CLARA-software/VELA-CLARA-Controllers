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
        liberallrfConfigReader(const std::string & llrfFileLocation1,const bool startVirtualMachine,
                               const bool* show_messages_ptr, const bool* show_debug_messages_ptr );
        ~liberallrfConfigReader();

        bool readConfig();

        bool getliberallrfObject(llrfStructs::liberallrfObject& obj );


        void setType(const llrfStructs::LLRF_TYPE typein);

    private:


        llrfStructs::LLRF_TYPE type;

        //int channel;
        std::string temp_name, temp_pvroot;

        llrfStructs::liberallrfObject llrfObj;
        // these structs are temporary, they are combined into pvMonStructs, pvComStructs after reading all files
        std::vector<llrfStructs::pvStruct> pvPowerTraceStructs;
        std::vector<llrfStructs::pvStruct> pvPhaseTraceStructs;

        std::vector<llrfStructs::pvStruct> pvLocalDerStructs;

        std::vector<llrfStructs::pvStruct> pvMonStructs;
        std::vector<llrfStructs::pvStruct> pvComStructs;

        void addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtype,const std::string& pvSuffix);
        void addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtype);

        llrfStructs::LLRF_PV_TYPE getPVType( const llrfStructs::LLRF_PV_TYPE main, const llrfStructs::LLRF_PV_TYPE sub);

        void addToliberallrfObjectsV1 (const std::vector<std::string> &keyVal);
        void addToTraceObjectsV1(const std::vector<std::string> &keyVal);
        void addToPVCommandMapV1(const std::vector<std::string> &keyVal);
        void addToPVMonitorMapV1(const std::vector<std::string> &keyVal);
        void addToTracePVMap(const std::vector<std::string>& keyVal);
        void addToPVMapV1       (const std::vector<std::string>& keyVal);


        bool readConfig(const std::string & configFile1 );

        llrfStructs::pvStruct* lastPVstruct;

        bool readTraceConfig();


        const bool usingVirtualMachine;


};
#endif //UTL_FILE_IO_H
