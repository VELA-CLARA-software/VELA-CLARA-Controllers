///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef CONFIG_READER_PyIL_SCREEN_H
#define CONFIG_READER_PyIL_SCREEN_H
//stl
#include <string>
#include <vector>
#include <map>
//me
#include "configReader.h"
#include "screenStructs.h"

class screenConfigReader : public configReader
{
public:
    screenConfigReader( const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
    screenConfigReader( const std::string & configFileLocation1,
                        const std::string & configFileLocation2, const bool * show_messages_ptr, const bool * show_debug_messages_ptr );
    ~screenConfigReader();

    bool screenConfigReader::readConfigFiles();
    screenStructs::screenObject screenConfigReader::getScreenObjects();

    double getPOS( const std::string & val );

private:

    typedef void( screenConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
    bool readConfig( screenConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );


    std::vector< screenStructs::screenObject > screenObjects;
    std::vector< screenStructs::pvStruct     > pvMonStructs;
    std::vector< screenStructs::pvStruct     > pvComStructs;


    void addPVStruct( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );
    void addCOUNT_MASK_OR_CHTYPE ( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal ) ;

    ///COMPLEX YAGs
    std::vector< screenStructs::COMPLEX_YAG_Object > complexYAGObjects;
    std::vector< screenStructs::pvStruct > complexYAGMonStructs;
    std::vector< screenStructs::pvStruct > complexYAGComStructs;
    void addToComplexYAGObjectsV1 ( const std::vector<std::string> &keyVal );
    void addToComplexYAGMonStructsV1 ( const std::vector<std::string> &keyVal );
    void addToComplexYAGComStructsV1 (const std::vector<std::string> &keyVal );

    ///SIMPLE YAGs
    std::vector< screenStructs::SIMPLE_YAG_Object > simpleYAGObjects;
    std::vector< screenStructs::pvStruct > simpleYAGMonStructs;
    std::vector< screenStructs::pvStruct > simpleYAGComStructs;
    void addToSimpleYAGObjectsV1 ( const std::vector<std::string> &keyVal );
    void addToSimpleYAGMonStructsV1 ( const std::vector<std::string> &keyVal );
    void addToSimpleYAGComStructsV1 ( const std::vector<std::string> &keyVal );

    void addToPVCommandMapV1 ( std::vector<std::string> &keyVal );
    void addToPVMonitorMapV1 ( std::vector<std::string> &keyVal );
};
#endif // UTL_FILE_IO_SCREEN_H

