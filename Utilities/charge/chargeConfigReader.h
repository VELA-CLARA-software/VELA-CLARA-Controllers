///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef chargeConfigReader_H_
#define chargeConfigReader_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "chargeStructs.h"
// boost
#include <boost/circular_buffer.hpp>


class chargeConfigReader : public configReader
{
    public:
        chargeConfigReader();// const bool* show_messages_ptr, const bool * show_debug_messages_ptr );
        chargeConfigReader( const std::string & chargeConf1,
                            const bool& showMessages,
                            const bool& showDebugMessages,
                            const bool startVirtualMachine );
        ~chargeConfigReader();

        bool readConfigFiles( );
        chargeStructs::chargeObject chargeConfigReader::getChargeObject();

    private:


        /// for RF object there are 3 types of hardware, we should be able to read in
        /// the conig files with 1 function and function pointers, this can't go in the
        /// base as the functions addToObjectsV1() etc. are in derived classes
        /// This seems like a "nice" use of function pointers

        const std::string chargeConf1;
        const bool startVirtualMachine;

        typedef void (chargeConfigReader::*aKeyValMemFn)( const std::vector<std::string> &keyVal );
        bool readConfig( chargeConfigReader & obj, const std::string fn, aKeyValMemFn f1, aKeyValMemFn f2, aKeyValMemFn f3 );

        /// These are read into vectors as you can use the .back() member function, which i find handy.
        /// once all the data is read in we can construct the final map of objects

        std::vector< chargeStructs::chargeObject > chargeObjects;
        std::vector< chargeStructs::pvStruct > chargeMonStructs;
        void addToChargeObjectsV1( const std::vector<std::string> &keyVal );
        void addToChargeMonStructsV1( const std::vector<std::string> &keyVal );
        void addToChargeComStructsV1( const std::vector<std::string> &keyVal );

        void addToPVStruct( std::vector< chargeStructs::pvStruct > & pvStruct_v, const  std::vector<std::string> &keyVal );
        void addCOUNT_MASK_OR_CHTYPE(  std::vector< chargeStructs::pvStruct >  & pvStruct_v, const std::vector<std::string> &keyVal );

        chargeStructs::CHARGE_PV_TYPE getDiagType( const std::string & val );

        std::vector< double >                           tstamps;
        std::vector< std::string >                      strtstamps;
        std::vector< double >                           vals;
        boost::circular_buffer< double >                valsbuffer;
        int                                             shotcounts;
        int                                             numshotcounts;
};
#endif //chargeConfigReader2_H_
