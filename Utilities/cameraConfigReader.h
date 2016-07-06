///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
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


class cameraConfigReader : public configReader
{
    public:
        cameraConfigReader( const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        cameraConfigReader( const std::string & configFile_Location, const std::string &configIOCFile_Location, const bool * showMessages, const  bool * showDebugMessages );
        ~cameraConfigReader();


        bool readConfig();

        bool getCamData(  std::map< std::string, cameraStructs::camObject > & mapToFill );
        bool getCamIOCData( cameraStructs::camIOCObject & c );

    private:


    /// I think i have three different ways of reading in multiple config files... see mags, RF and now this... hmmm :o(
        bool read_CAM_Config( );
        bool read_IOC_Config( );

        void addToPVStruct( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );

        void addToObjectsV1( const std::vector<std::string> &keyVal  );
        void addToCamIOCObjects( const std::vector<std::string> & keyVal );

        void addCOUNT_MASK_OR_CHTYPE( std::vector< cameraStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal  );

        std::vector< cameraStructs::camObject > camObjects;
        std::vector< cameraStructs::pvStruct > pvMonStructs;
        std::vector< cameraStructs::pvStruct > pvComStructs;


        std::vector< cameraStructs::camIOCObject > camIOCObjects;
        std::vector< cameraStructs::pvStruct > pvIOCMonStructs;
        std::vector< cameraStructs::pvStruct > pvIOCComStructs;

         void addToCamIOCObjectsV1( const std::vector<std::string> &keyVal  );


};
#endif //CONFIG_READER_CAM_H_
