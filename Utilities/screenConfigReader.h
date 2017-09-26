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
                        const bool startVirtualMachine,
                        const bool* show_messages_ptr, const bool* show_debug_messages_ptr );

    ~screenConfigReader();

    bool readConfig();
    bool getScreenObjects( std::map< std::string, screenStructs::screenObject > & mapToFill );


private:

    // Screen  Objects
    std::vector< screenStructs::screenObject > scrObjects;
    // the screens come in differen flavours, with different parameters to monitor
    // this may have to be  extended if CLARA screens are different
    std::vector< screenStructs::pvStruct > pvScrComStructs_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrComStructs_VELA_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_VELA_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrComStructs_CLARA_PNEUMATIC;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_CLARA_PNEUMATIC;

    /// all the objects have their own command and monitor structs, as defined in teh config file
    std::vector< screenStructs::pvStruct > pvScrComStructs_HV_MOVER_H;
    std::vector< screenStructs::pvStruct > pvScrComStructs_HV_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_HV_MOVER_H;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_HV_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_V_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_V_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_V_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_V_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_VELA_HV_MOVER_H;
    std::vector< screenStructs::pvStruct > pvScrComStructs_VELA_HV_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_VELA_HV_MOVER_H;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_VELA_HV_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_VELA_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_VELA_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_CLARA_HV_MOVER_H;
    std::vector< screenStructs::pvStruct > pvScrComStructs_CLARA_HV_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_CLARA_HV_MOVER_H;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_CLARA_HV_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_CLARA_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_CLARA_HV_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_CLARA_V_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_CLARA_V_MOVER_V;
    std::vector< screenStructs::pvStruct > pvScrMonStructs_CLARA_V_MOVER;
    std::vector< screenStructs::pvStruct > pvScrComStructs_CLARA_V_MOVER;

    // we keep a record of the last pcstruct type added, so we know where to put PV_CHTYPE etc.
    std::vector< screenStructs::pvStruct > * lastPVStruct;
    void addToPVStructMain( const std::vector<std::string> &keyVal );
    void addToPVStruct( std::vector< screenStructs::pvStruct > & pvs, const screenStructs::SCREEN_PV_TYPE pvtype,
                        const std::string& pvSuffix);

    void screenConfigReader::addToScrObjectsV1( const std::vector<std::string> &keyVal );
    screenStructs::SCREEN_TYPE screenConfigReader::getScreenType( const std::string & val );
//    void setLasttoVELA_HV_MOVER();
//    void setLasttoVELA_PNEUMATIC();

//    void setCassetteElementExists( screenStructs::screenCassette & cas, const std::string & element );

    void setH(std::vector< screenStructs::pvStruct > & pvs );
    void setV(std::vector< screenStructs::pvStruct > & pvs );


    const bool usingVirtualMachine;

    // yay function pointers
    typedef void( screenConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
    bool readConfig( screenConfigReader & obj, const std::string & fn, aKeyValMemFn f1, aKeyValMemFn f2);


//    void addToPVCommandMapV1 ( std::vector<std::string> &keyVal );
//    void addToPVMonitorMapV1 ( std::vector<std::string> &keyVal );
//    void addToScrMonStructsV1( const std::vector<std::string> &keyVal );
//    void addToScComStructsV1( const std::vector<std::string> &keyVal );
//    void addToScrObjectsV1( const std::vector<std::string> &keyVal );

    //void addCOUNT_MASK_OR_CHTYPE(std::vector<screenStructs::pvStruct>& pvStruct_v, const std::vector<std::string> &keyVal);
    void addCOUNT_MASK_OR_CHTYPE(const std::vector<std::string> &keyVal);

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

