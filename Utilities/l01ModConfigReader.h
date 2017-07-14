///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef l01ModConfigReader_H_
#define l01ModConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "rfModStructs.h"


class l01ModConfigReader : public configReader
{
    public:
        l01ModConfigReader(const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        l01ModConfigReader(const std::string& configFileLocation1,const bool startVirtualMachine,const bool * showMessages,const  bool * showDebugMessages );
        ~l01ModConfigReader();

        bool readConfig( );
        bool getL01ModObject(rfModStructs::l01ModObject& obj );
//
    private:

        // Gun Modulator Objects
        std::vector<rfModStructs::l01_pvStruct> pvMonStructs;
        std::vector<rfModStructs::l01_pvStruct> pvComStructs;
        rfModStructs::l01ModObject localModObject;
        void addToModObjectsV1(const std::vector<std::string> &keyVal);
        void addToPVMonitorMapV1(const std::vector<std::string> &keyVal);
        void addToPVCommandMapV1(const std::vector<std::string> &keyVal);


        rfModStructs::l01ModObject RFModObject;

        void addToPVStruct(std::vector<rfModStructs::l01_pvStruct > & pvStruct_v, const  std::vector<std::string> &keyVal );
        void addCOUNT_MASK_OR_CHTYPE(std::vector< rfModStructs::l01_pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal );


//        void addToObjectsV1( std::vector<std::string> &keyVal );
//        void addPVCommandMapV1  ( std::vector<std::string> &keyVal );
//        void addPVMonitorMapV1  ( std::vector<std::string> &keyVal );
//

};
#endif //l01ModConfigReader_H_
