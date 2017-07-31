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
        liberallrfConfigReader(const std::string & configFileLocation1,const bool startVirtualMachine,
                               const bool* show_messages_ptr, const bool* show_debug_messages_ptr );
        ~liberallrfConfigReader();

        bool readConfig();

        bool getliberallrfObject(llrfStructs::liberallrfObject& obj );


    private:


        llrfStructs::liberallrfObject llrfObj;
        std::vector<llrfStructs::pvStruct> pvMonStructs;
        std::vector<llrfStructs::pvStruct> pvComStructs;

        void addToPVStruct(std::vector<llrfStructs::pvStruct>& pvs,const llrfStructs::LLRF_PV_TYPE pvtype,const std::string& pvSuffix);

        void addToliberallrfObjectsV1 (const std::vector<std::string> &keyVal);
        void addToPVCommandMapV1(const std::vector<std::string> &keyVal);
        void addToPVMonitorMapV1(const std::vector<std::string> &keyVal);
        void addToPVMapV1       (const std::vector<std::string>& keyVal);

        llrfStructs::pvStruct* lastPVstruct;


        const bool usingVirtualMachine;


};
#endif //UTL_FILE_IO_H
