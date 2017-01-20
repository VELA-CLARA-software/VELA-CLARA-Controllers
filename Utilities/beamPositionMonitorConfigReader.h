///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef beamPositionMonitorConfigReader_H_
#define beamPositionMonitorConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "beamPositionMonitorStructs.h"


class beamPositionMonitorConfigReader : public configReader
{
    public:
        beamPositionMonitorConfigReader();// const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        beamPositionMonitorConfigReader( const std::string & bpmConf,  const bool * showMessages,
                                         const bool * showDebugMessages, const bool startVirtualMachine );
        ~beamPositionMonitorConfigReader();

        bool readConfigFiles( );
        beamPositionMonitorStructs::bpmObject beamPositionMonitorConfigReader::getBPMObject();

    private:


        /// for RF object there are 3 types of hardware, we should be able to read in
        /// the conig files with 1 function and function pointers, this can't go in the
        /// base as the functions addToObjectsV1() etc. are in derived classes
        /// This seems like a "nice" use of function pointers

        typedef void (beamPositionMonitorConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
        bool readConfig( beamPositionMonitorConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );

        /// These are read into vectors as you can use the .back() member function, which i find handy.
        /// once all the data is read in we can construct the final map of objects

        // Data objects
        std::vector< beamPositionMonitorStructs::bpmDataObject > bpmDataObjects;
        std::vector< beamPositionMonitorStructs::pvStruct > bpmDataMonStructs;
        void addToBPMDataObjectsV1( const std::vector<std::string> &keyVal );
        void addToBPMPVMonStructsV1( const std::vector<std::string> &keyVal );
        void addToBPMPVComStructsV1( const std::vector<std::string> &keyVal );

        const std::string bpmConf;
        const bool usingVirtualMachine;


        void addToPVStruct( std::vector< beamPositionMonitorStructs::pvStruct > & pvStruct_v, const std::vector<std::string> &keyVal );
        void addCOUNT_MASK_OR_CHTYPE(  std::vector< beamPositionMonitorStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal );

};
#endif //beamPositionMonitorConfigReader2_H_
