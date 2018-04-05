///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef DBURT_H_
#define DBURT_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "dburtDefinitions.h"
#include "magnetStructs.h"
#include "magnetInterface.h"


class dburt : public configReader
{
    public:
//        dburt(const bool* show_messages, const bool * show_debug_messages);
        dburt(const bool& show_messages,
              const bool& show_debug_messages,
             const HWC_ENUM::MACHINE_AREA myMachineArea );
        ~dburt();


        magnetStructs::magnetStateStruct readDBURT(const char* fileName);

        magnetStructs::magnetStateStruct readDBURT(const std::string & fileName);



        bool writeDBURT(const magnetStructs::magnetStateStruct & magState, const std::string & fileName = "", const std::string & comments = "", const std::string & keywords = "" );

    private:

        magnetStructs::magnetStateStruct readDBURTv1(const char* fileName, const std::string & path = UTL::DBURT_PATH);

        magnetStructs::magnetStateStruct dburt::readDBURTv3(const char* fileName, const std::string & path = UTL::DBURT_PATH);

        const HWC_ENUM::MACHINE_AREA myMachineArea;


};
#endif //DBURT_READER_H_
