///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef CONFIG_READER_IMG_H
#define CONFIG_READER_IMG_H
// stl
#include <string>
#include <vector>
#include <map>
// me
#include "configReader.h"
#include "invertedMagnetronGaugeStructs.h"


class invertedMagnetronGaugeConfigReader : public configReader
{
    public:
        invertedMagnetronGaugeConfigReader();
        invertedMagnetronGaugeConfigReader( const std::string & configFile1,
                                            const bool * showMessages,
                                            const bool * showDebugMessages,
                                            const bool startVirtualMachine );
        ~invertedMagnetronGaugeConfigReader();

        bool readVacImgConfig( );
        const std::vector< invertedMagnetronGaugeStructs::vacImgObject >                 getVacImgObjects();

    private:

        const std::string & configFile1;
        const bool usingVirtualMachine;

        std::vector< invertedMagnetronGaugeStructs::vacImgObject > vacImgObjects;
        std::vector< invertedMagnetronGaugeStructs::pvStruct     > pvMonStructs;
        std::vector< invertedMagnetronGaugeStructs::pvStruct     > pvComStructs;

        void addPVStruct( std::vector< invertedMagnetronGaugeStructs::pvStruct > & pvStruct_v, std::vector<std::string> &keyVal );

        void addToVacImgObjectsV1( std::vector<std::string> &keyVal );
        void addToPVCommandMapV1  ( std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1  ( std::vector<std::string> &keyVal );

};
#endif //UTL_FILE_IO_H
