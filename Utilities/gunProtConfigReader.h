///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef gunProtConfigReader_H_
#define gunProtConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "rfProtStructs.h"


class gunProtConfigReader : public configReader
{
    public:
        gunProtConfigReader(const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        gunProtConfigReader(const std::string& configFileLocation1,const bool startVirtualMachine,const bool * showMessages,const  bool * showDebugMessages );
        ~gunProtConfigReader();

        bool readConfig();
        bool getrfGunProtObjects(std::map<std::string, rfProtStructs::rfGunProtObject>  mapToFill );
//
    private:

        // Gun Protection Objects
        std::vector< rfProtStructs::pvStruct > pvMonStructs;
        std::vector< rfProtStructs::pvStruct > pvComStructs;
        rfProtStructs::rfGunProtObject localrfGunProtObject;
        void addToModObjectsV1(const std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1(const std::vector<std::string> &keyVal );
        void addToPVCommandMapV1(const std::vector<std::string> &keyVal );


        std::vector<rfProtStructs::rfGunProtObject> gunProtObjects;


        void addToPVStruct(std::vector< rfProtStructs::pvStruct > & pvStruct_v, const  std::vector<std::string> &keyVal );
        void addCOUNT_MASK_OR_CHTYPE( std::vector< rfProtStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal );


//        void addToObjectsV1(std::vector<std::string> &keyVal );
//        void addPVCommandMapV1  (std::vector<std::string> &keyVal );
//        void addPVMonitorMapV1  (std::vector<std::string> &keyVal );
//

};
#endif //gunProtConfigReader_H_
