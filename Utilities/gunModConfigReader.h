///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef gunModConfigReader_H_
#define gunModConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "rfModStructs.h"


class gunModConfigReader : public configReader
{
    public:
        gunModConfigReader(const bool& show_messages_ptr, const bool& show_debug_messages_ptr);
        gunModConfigReader(const std::string& configFileLocation1,const bool startVirtualMachine,const bool& showMessages,const  bool& showDebugMessages);
        ~gunModConfigReader();

        bool readConfig();
        bool getGunModObject(rfModStructs::gunModObject& obj);
//
    private:

        // Gun Modulator Objects
        std::vector< rfModStructs::pvStruct > pvMonStructs;
        std::vector< rfModStructs::pvStruct > pvComStructs;
        rfModStructs::gunModObject localGunModObject;
        void addToModObjectsV1(const std::vector<std::string> &keyVal);
        void addToPVMonitorMapV1(const std::vector<std::string> &keyVal);
        void addToPVCommandMapV1(const std::vector<std::string> &keyVal);


        rfModStructs::gunModObject RFModObject;

        void addToPVStruct(std::vector< rfModStructs::pvStruct > & pvStruct_v, const  std::vector<std::string> &keyVal);
        void addCOUNT_MASK_OR_CHTYPE( std::vector< rfModStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal);


//        void addToObjectsV1(std::vector<std::string> &keyVal);
//        void addPVCommandMapV1  (std::vector<std::string> &keyVal);
//        void addPVMonitorMapV1  (std::vector<std::string> &keyVal);
//

};
#endif //gunModConfigReader_H_
