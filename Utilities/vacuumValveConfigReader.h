///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef CONFIG_READER_VAC_VALVE_H
#define CONFIG_READER_VAC_VALVE_H
// stl
#include <string>
#include <vector>
#include <map>
// me
#include "configReader.h"
#include "vacuumValveStructs.h"


class vacuumValveConfigReader : public configReader
{
    public:
        vacuumValveConfigReader();// const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        vacuumValveConfigReader( const std::string & vacValveConf,  const bool * showMessages,
                                         const bool * showDebugMessages, const bool startVirtualMachine );
        ~vacuumValveConfigReader();

//        bool readVacValveConfig();
        bool readConfigFiles();
        const std::vector< vacuumValveStructs::vacValveObject > getVacValveObjects();

    private:

        const std::string vacValveConf;
        const bool usingVirtualMachine;

        typedef void ( vacuumValveConfigReader::*aKeyValMemFn )( const std::vector<std::string> &keyVal );

        bool readConfig( vacuumValveConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );

        std::vector< vacuumValveStructs::vacValveObject > vacValveObjects;
        std::vector< vacuumValveStructs::pvStruct      > pvMonStructs;
        std::vector< vacuumValveStructs::pvStruct      > pvComStructs;

        void addPVStruct( std::vector< vacuumValveStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );

        void addToVacValveObjectsV1( const std::vector<std::string> &keyVal );
        void addToPVCommandMapV1  ( const std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1  ( const std::vector<std::string> &keyVal );

};
#endif //UTL_FILE_IO_H
