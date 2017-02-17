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


class llrfConfigReader:public configReader
{
    public:
        llrfConfigReader(const std::string & configFileLocation1,const bool startVirtualMachine,
                          const bool* show_messages_ptr, const bool* show_debug_messages_ptr );
        ~llrfConfigReader();

        bool readConfig( );
        const llrfStructs::laserObject getlaserObjects(){return laserObject;}

    private:


        llrfStructs::laserObject laserObject;
        std::vector<llrfStructs::pvStruct> pvMonStructs;
        std::vector<llrfStructs::pvStruct> pvComStructs;

        void addPVStruct( std::vector< llrfStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );

        void addTolaserObjectsV1( const std::vector<std::string> &keyVal );
        void addToPVCommandMapV1  ( const std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1  ( const std::vector<std::string> &keyVal );



        const bool usingVirtualMachine;


};
#endif //UTL_FILE_IO_H
