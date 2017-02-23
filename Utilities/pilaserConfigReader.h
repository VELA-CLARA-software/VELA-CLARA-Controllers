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
#include "pilaserStructs.h"


class pilaserConfigReader:public configReader
{
    public:
        pilaserConfigReader(const std::string & configFileLocation1,const bool startVirtualMachine,
                          const bool* show_messages_ptr, const bool* show_debug_messages_ptr );
        ~pilaserConfigReader();

        bool readConfig( );
        bool getpilaserObject(pilaserStructs::pilaserObject & obj);

    private:
        pilaserStructs::pilaserObject pilaserObject;
        std::vector<pilaserStructs::pvStruct> pvMonStructs;
        std::vector<pilaserStructs::pvStruct> pvComStructs;

        // hand pointer so we cna keep track of the last PV struct we were adding data to
        //std::vector< pilaserStructs::pvStruct > * lastPVStruct;

        void addToPVStruct(std::vector< pilaserStructs::pvStruct > & pvs, const pilaserStructs::PILASER_PV_TYPE pvtype, const std::string& pvSuffix);

        void addTopilaserObjectsV1( const std::vector<std::string> &keyVal );
        void addToPVCommandMapV1  ( const std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1  ( const std::vector<std::string> &keyVal );

        const bool usingVirtualMachine;
};
#endif //UTL_FILE_IO_H
