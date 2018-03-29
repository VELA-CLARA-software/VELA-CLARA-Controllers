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
#include "shutterStructs.h"


class shutterConfigReader : public configReader
{
    public:
        shutterConfigReader( const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        shutterConfigReader( const std::string & configFileLocation,  const bool * showMessages, const  bool * showDebugMessages );
        ~shutterConfigReader();
//
//        bool readPILShutterConfig( );
//        const std::vector< shutterStructs::shutterObject >                 getShutterObjects();

    private:

//
//        std::vector< shutterStructs::shutterObject > shutterObjects;
//        std::vector< shutterStructs::pvStruct      > pvMonStructs;
//        std::vector< shutterStructs::pvStruct      > pvComStructs;
//
//        void addPVStruct( std::vector< shutterStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );
//
//        void addToShutterObjectsV1( const std::vector<std::string> &keyVal );
//        void addToPVCommandMapV1  ( const std::vector<std::string> &keyVal );
//        void addToPVMonitorMapV1  ( const std::vector<std::string> &keyVal );

};
#endif //UTL_FILE_IO_H
