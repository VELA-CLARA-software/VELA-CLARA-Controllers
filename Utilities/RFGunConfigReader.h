///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef RFGunConfigReader_H_
#define RFGunConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "RFGunStructs.h"


class RFGunConfigReader : public configReader
{
    public:
        RFGunConfigReader( const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        RFGunConfigReader( const std::string configFileLocation1,
                           const std::string configFileLocation2,
                           const std::string configFileLocation3, const bool * showMessages, const  bool * showDebugMessages );
        ~RFGunConfigReader();

        bool readConfigFiles( );
        RFGunStructs::rfObject RFGunConfigReader::getRFObject();

    private:


        /// for RF object there are 3 types of hardware, we should be able to read in
        /// the conig files with 1 function and function pointers, this can't go in the
        /// base as the functions addToObjectsV1() etc. are in derived classes
        /// This seems like a "nice" use of function pointers

        typedef void (RFGunConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
        bool readConfig( RFGunConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );

        /// These are read into vectors as you can use the .back() member function, which i find handy.
        /// once all the data is read in we can construct the final map of objects

        // POWER objects
        std::vector< RFGunStructs::rfPowerObject > rfPowerObjects;
        std::vector< RFGunStructs::pvStruct > pvPWRMonStructs;
        void addToRFPowObjectsV1( const std::vector<std::string> &keyVal );
        void addToRFPowPVMonStructsV1( const std::vector<std::string> &keyVal );
        // LLRF objects
        std::vector< RFGunStructs::pvStruct > pvLLRFMonStructs;
        RFGunStructs::rfLLRFObject RFLLRFObject;
        void addToLLRFObjectsV1( const std::vector<std::string> &keyVal );
        void addToLLRFPVMonStructsV1( const std::vector<std::string> &keyVal );
        // Modulator Objects
        std::vector< RFGunStructs::pvStruct > pvModMonStructs;
        std::vector< RFGunStructs::pvStruct > pvModComStructs;
        RFGunStructs::rfModObject RFModObject;
        void addToModObjectsV1( const std::vector<std::string> &keyVal );
        void addToModPVMonStructsV1( const std::vector<std::string> &keyVal );
        void addToModPVComStructsV1( const std::vector<std::string> &keyVal );



        void addToPVStruct( std::vector< RFGunStructs::pvStruct > & pvStruct_v, const  std::vector<std::string> &keyVal );
        void addCOUNT_MASK_OR_CHTYPE(  std::vector< RFGunStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal );


//        void addToObjectsV1( std::vector<std::string> &keyVal );
//        void addPVCommandMapV1  ( std::vector<std::string> &keyVal );
//        void addPVMonitorMapV1  ( std::vector<std::string> &keyVal );


};
#endif //RFGunConfigReader_H_
