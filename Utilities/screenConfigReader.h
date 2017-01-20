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
    screenConfigReader( const std::string & configFileLocation1,
                        const std::string & configFileLocation2,
                        const bool startVirtualMachine,
                        const bool* show_messages_ptr, const bool* show_debug_messages_ptr );

    ~screenConfigReader();

    bool readConfig();
    bool getScreenObjects( std::map< std::string, screenStructs::screenObjectDEV > & mapToFill );


private:

    // Screen  Objects
    std::vector< screenStructs::screenObjectDEV > scrObjects;
    // the screens come in differen flavours, with different parameters to monitor
    // this may have to be  extended if CLARA screens are different
    std::vector< screenStructs::pvStruct > pvScrComStructs_VELA_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrComStructs_VELA_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_VELA_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_VELA_HV_MOVER;
    // we keep a record of the last pcstruct type added, so we know where to put PV_CHTYPE etc.
    screenStructs::SCREEN_TYPE lastPVStructScreenType;
    void addToPVStruct(std::vector<screenStructs::pvStruct> & pvStruct_v, const std::vector<std::string> &keyVal );
    void addToPVStruct(std::vector< screenStructs::pvStruct > & pvs, screenStructs::SCREEN_PV_TYPE pvtype,
                               screenStructs::SCREEN_TYPE scrType  )

//    void setLasttoVELA_HV_MOVER();
//    void setLasttoVELA_PNEUMATIC();

    const bool usingVirtualMachine;
    // yay function pointers
    typedef void( screenConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
    bool readConfig( screenConfigReader & obj, const std::string & fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3);

    void addToPVStruct( std::vector<screenStructs::pvStruct> & pvStruct_v, const std::vector<std::string> &keyVal );
    void addToPVCommandMapV1 ( std::vector<std::string> &keyVal );
    void addToPVMonitorMapV1 ( std::vector<std::string> &keyVal );

    void addToScrMonStructsV1( const std::vector<std::string> &keyVal );
    void addToScComStructsV1( const std::vector<std::string> &keyVal );
    void addToScrObjectsV1( const std::vector<std::string> &keyVal );
    void addCOUNT_MASK_OR_CHTYPE(std::vector<screenStructs::pvStruct>& pvStruct_v, const std::vector<std::string> &keyVal);

    /// OLD



//
//    bool readConfig( screenConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );
//
//

//
//
//    void addPVStruct( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );
//    void addCOUNT_MASK_OR_CHTYPE ( std::vector< screenStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal ) ;
//
//    ///COMPLEX YAGs
//    std::vector< screenStructs::COMPLEX_YAG_Object > complexYAGObjects;
//    std::vector< screenStructs::pvStruct > complexYAGMonStructs;
//    std::vector< screenStructs::pvStruct > complexYAGComStructs;
//    void addToComplexYAGObjectsV1 ( const std::vector<std::string> &keyVal );
//    void addToComplexYAGMonStructsV1 ( const std::vector<std::string> &keyVal );
//    void addToComplexYAGComStructsV1 (const std::vector<std::string> &keyVal );
//
//    ///SIMPLE YAGs
//    std::vector< screenStructs::SIMPLE_YAG_Object > simpleYAGObjects;
//    std::vector< screenStructs::pvStruct > simpleYAGMonStructs;
//    std::vector< screenStructs::pvStruct > simpleYAGComStructs;
//    void addToSimpleYAGObjectsV1 ( const std::vector<std::string> &keyVal );
//    void addToSimpleYAGMonStructsV1 ( const std::vector<std::string> &keyVal );
//    void addToSimpleYAGComStructsV1 ( const std::vector<std::string> &keyVal );


};
#endif // UTL_FILE_IO_SCREEN_H

