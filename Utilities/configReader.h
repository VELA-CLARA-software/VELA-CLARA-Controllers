///
/// Duncan Scott July 2015
///
/// For reading in parameters
/// Input files will be plain text
///
#ifndef CONFIG_READER
#define CONFIG_READER
//
#include "configDefinitions.h"
#include "baseObject.h"
// stl
#include <string>
#include <vector>
// epics
#include <cadef.h>

class configReader : public baseObject
{
    public:
        configReader();
        configReader( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );
        configReader( const std::string & configFile_Location1, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );
        configReader( const std::string & configFile_Location1,
                      const std::string & configFile_Location2, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );
        configReader( const std::string & configFile_Location1,
                      const std::string & configFile_Location2,
                      const std::string & configFile_Location3, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );
        configReader( const std::string & configFile_Location1,
                      const std::string & configFile_Location2,
                      const std::string & configFile_Location3,
                      const std::string & configFile_Location4, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );
        configReader( const std::string & configFile_Location1,
                      const std::string & configFile_Location2,
                      const std::string & configFile_Location3,
                      const std::string & configFile_Location4,
                      const std::string & configFile_Location5, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr );


        void setConfigFilePath( const std::string & path );

    protected:

        /// protected destructor to make sure this class is never instantiated
        ///  the compiler won't let us call delete on any base class pointers

        ~configReader();

        int configVersion, numObjs, numIlocks;

        void getVersion( const std::string & str );
        void getNumObjs( const std::string & str );
        void getNumIlocks ( const std::string & str );

        std::string  getAfterEqualsSign( const std::string & str );

        int    getNum ( const std::string & str );
        double getNumD( const std::string & str );
        size_t getSize( const std::string & str );

        std::vector< double >  getDoubleVector( const std::string & str );

        std::vector<std::string> getKeyVal( const std::string & trimmedLine, const char delim = UTL::EQUALS_SIGN_C );
        std::string & trimAllWhiteSpace( std::string & source );
        std::string & trimAllWhiteSpaceExceptBetweenDoubleQuotes( std::string & str);
        std::string trimWhiteSpaces(const  std::string & str );
        ///Allowed upto 5 config file paths
        std::string configFile1, configFile2, configFile3, configFile4, configFile5;

         unsigned long getMASK( const std::string & val );
         unsigned long getCOUNT(const  std::string & val );
         chtype getCHTYPE( const  std::string & val );

        /// Most of these are now unescessary ???

        std::string getSubString( std::string & str, std::string & STARTDELIMITER, std::string & STOPDELIMITER );
        std::string trimBetween( std::string & str, std::string & STARTDELIMITER, std::string & STOPDELIMITER );

        std::string trimToDelimiter( const  std::string & str, const std::string & STOPDELIMITER );

        bool stringIsSubString( const std::string & stringToCheck, const std::string & stringToLookFor );

        bool isNO_CONFIG_FILE( const  std::string & str );
        bool isnotNO_CONFIG_FILE( const  std::string & str );


    private:
};
#endif //UTL_FILE_IO_H
