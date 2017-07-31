
#ifndef CONFIG_READER_CAM_H_
#define CONFIG_READER_CAM_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "cameraStructs.h"


class cameraDAQConfigReader : public configReader
{
    public:
        cameraDAQConfigReader();
        cameraDAQConfigReader::cameraDAQConfigReader( const std::string&camConfig,
                                                const bool startVirtualMachine,
                                                const bool*show_messages_ptr,
                                                const bool*show_debug_messages_ptr );
        ~cameraDAQConfigReader();

        bool readConfig();
        bool getCamDAQData(  std::map< std::string, cameraStructs::cameraDAQObject > & mapToFill );

    private:
        bool readCameraConfig( );

        void addToPVStruct( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );

        void addToCameraObjects( const std::vector<std::string> &keyVal  );
        void addToCameraMonitorStructs( const std::vector<std::string> &keyVal  );
        void addToCameraCommandStructs( const std::vector<std::string> &keyVal  );

        void addCOUNT_MASK_OR_CHTYPE( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  );

        std::vector< cameraStructs::cameraDAQObject > camDAQObject;
        std::vector< cameraStructs::pvStruct > pvCameraMonStructs;
        std::vector< cameraStructs::pvStruct > pvCameraComStructs;


         const std::string camConfig;
         const bool usingVirtualMachine;


};
#endif //CONFIG_READER_CAM_H_
