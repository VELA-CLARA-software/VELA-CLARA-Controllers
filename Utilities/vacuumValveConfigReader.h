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
        vacuumValveConfigReader( const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        vacuumValveConfigReader( const std::string & configFile_Location,  const bool * showMessages, const  bool * showDebugMessages );
        ~vacuumValveConfigReader();

        bool readVacValveConfig( );
        const std::vector< vacuumValveStructs::vacValveObject >                 getVacValveObjects();

    private:


        std::vector< vacuumValveStructs::vacValveObject > vacValveObjects;
        std::vector< vacuumValveStructs::pvStruct      > pvMonStructs;
        std::vector< vacuumValveStructs::pvStruct      > pvComStructs;

        void addPVStruct( std::vector< vacuumValveStructs::pvStruct > & pvStruct_v, std::vector<std::string> &keyVal );

        void addToVacValveObjectsV1( std::vector<std::string> &keyVal );
        void addToPVCommandMapV1  ( std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1  ( std::vector<std::string> &keyVal );

};
#endif //UTL_FILE_IO_H
