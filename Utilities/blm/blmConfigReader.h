///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef blmConfigReader_H_
#define blmConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "blmStructs.h"
// boost
#include <boost/circular_buffer.hpp>


class blmConfigReader : public configReader
{
    public:
        blmConfigReader();// const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        blmConfigReader( const std::string & blmConf1,
                           const bool& showMessages,    const bool& showDebugMessages,
                           const bool startVirtualMachine );
        ~blmConfigReader();

        bool readConfigFiles( );
        blmStructs::blmObject blmConfigReader::getBLMObject();

    private:


        /// for RF object there are 3 types of hardware, we should be able to read in
        /// the conig files with 1 function and function pointers, this can't go in the
        /// base as the functions addToObjectsV1() etc. are in derived classes
        /// This seems like a "nice" use of function pointers

        const std::string blmConf1;
        const std::string blmConf2;
        const bool startVirtualMachine;

        typedef void (blmConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
        bool readConfig( blmConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );

        /// These are read into vectors as you can use the .back() member function, which i find handy.
        /// once all the data is read in we can construct the final map of objects

        // TRACE objects
        std::vector< blmStructs::blmTraceData > blmTraceDataObjects;
        std::vector< blmStructs::pvStruct > blmTraceDataMonStructs;
        void addToBLMTraceDataObjectsV1( const std::vector<std::string> &keyVal );
        void addToBLMTraceDataMonStructsV1( const std::vector<std::string> &keyVal );
        // NUM objects
        std::vector< blmStructs::pvStruct > blmNumMonStructs;
        void addToBLMNumObjectsV1( const std::vector<std::string> &keyVal );
        void addToBLMNumMonStructsV1( const std::vector<std::string> &keyVal );

        void addToPVStruct( std::vector< blmStructs::pvStruct > & pvStruct_v, const  std::vector<std::string> &keyVal );
        void addCOUNT_MASK_OR_CHTYPE(  std::vector< blmStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal );

        std::vector< double >                           tstamps;
        std::vector< double >                           numtstamps;
        std::vector< std::string >                      strtstamps;
        std::vector< std::string >                      numstrtstamps;
        std::vector< std::vector< double > >            traces;
        std::vector< double >                           nums;
        boost::circular_buffer< double >                numsbuffer;
        boost::circular_buffer< std::vector< double > > tracesbuffer;
        int                                             shotcounts;
        int                                             numshotcounts;
};
#endif //blmConfigReader2_H_
