//djs
#include "dburt.h"
#include "dburtDefinitions.h"
#include "magnetStructs.h"
//stl
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <ctype.h>

dburt::dburt( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader( UTL::DBURT_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
dburt::dburt( const std::string & configFile_Location, const bool* show_messages_ptr, const bool * show_debug_messages_ptr  )
:  configReader( configFile_Location, show_messages_ptr, show_debug_messages_ptr )
{
}
//______________________________________________________________________________
dburt::~dburt(){}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURT( const std::string & fileName)
{
    return readDBURT( fileName.c_str() );
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURT( const char* fileName )
{
    message( "\n", "**** Attempting to Read ", UTL::DBURT_PATH+fileName, " ****" );

    std::string line, trimmedLine;

    std::ifstream inputFile;

    configVersion = -1;

    inputFile.open( UTL::DBURT_PATH+fileName, std::ios::in );
    if( inputFile )
    {
        message( "File Opened from ",  UTL::DBURT_PATH );
        while( std::getline( inputFile, line ) ) /// Go through, reading file line by line
        {
            trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );

            if( stringIsSubString( line, UTL::VELA_MAGNET_SAVE_FILE_v1 ) )
            {
                configVersion = 1;
                break;
            }
            if( stringIsSubString( line, UTL::VERSION ) )
            {
                if( stringIsSubString( trimmedLine, UTL::VERSION ) )
                    getVersion( trimmedLine );
            }
        }
    }
    debugMessage("Finished preprocessing file");
    inputFile.close( );
    magnetStructs::magnetStateStruct magState;
    switch( configVersion )
    {
        case -1:
            debugMessage("NO DBURT VERSION FOUND EXIT");

            break;

        case 1:
            debugMessage("VERSION 1 DBURT FOUND");
            magState = readDBURTv1( fileName );

            break;

        case 2:
            debugMessage("VERSION 2 DBURT FOUND");
            break;
        default:
            debugMessage("UNEXPECTED DBURT VERSION, ", configVersion, ", FOUND");

    }

    return magState;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct dburt::readDBURTv1( const char* fileName, const std::string & path )
{
    std::string pathToDBURT;
    if( path == "" )
        pathToDBURT =  UTL::DBURT_PATH;
    else
        pathToDBURT =  UTL::DBURT_PATH;


    magnetStructs::magnetStateStruct magState;


    std::string line, trimmedLine;
    std::ifstream inputFile;

    std::vector<std::string> keyvalval;

    inputFile.open( pathToDBURT+fileName, std::ios::in);
    if( inputFile )
    {
        bool readingParameters = false;
        int  linenumber        = 0;

        while( std::getline( inputFile, line ) ) /// Go through line by line
        {
            std::stringstream iss( line ); /// make a stream of the line and then do some tests
            ++linenumber;
            if( stringIsSubString( iss.str(), UTL::DBURT_EOF_V1 ) )
            {
                message( "FOUND END OF FILE ");
                readingParameters = false;
                break;
            }
            if( readingParameters )
            {
                trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );

                keyvalval = getKeyVal( trimmedLine, UTL::COLON_C );

                if( keyvalval.size() == 3 )
                {
                    magState.magNames.push_back( keyvalval[0] );

                    if( keyvalval[1] == UTL::ON )
                        magState.psuStates.push_back( VC_ENUM::MAG_PSU_STATE::MAG_PSU_ON );
                    else if( keyvalval[1] == UTL::OFF )
                        magState.psuStates.push_back( VC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF );
                    else
                        magState.psuStates.push_back( VC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR );

                    magState.siValues.push_back( getNumD( keyvalval[2] ) );
                }
            } // if( readingParameters )_END

            if( stringIsSubString( iss.str(), UTL::DBURT_NUM_MAGNETS_V1 )  )
            {
                trimmedLine = trimAllWhiteSpace( trimToDelimiter( line, UTL::END_OF_LINE ) );
                keyvalval = getKeyVal( trimmedLine, UTL::COLON_C );

                magState.numMags = getSize( keyvalval[1] );
                message( "FOUND NUM MAGNETS = ",  magState.numMags );
                readingParameters = true;
            }
            if( stringIsSubString( iss.str(), UTL::DBURT_PARAMETERS_V1 )  )
            {
                message( "FOUND START OF DATA" );
            }
        } // while
    } // main if
    else
        message( "ERROR: In importDBURT: failed to open ",  pathToDBURT, fileName );

    return magState;
}
//______________________________________________________________________________
bool dburt::writeDBURT( const magnetStructs::magnetStateStruct & magState, const std::string & fileName, const std::string & comments )
{
    bool success = false;

    std::string fn;
    std::string cdt = currentDateTime();

    message("cdt = ", cdt);

    if( fileName == "" )
        fn = UTL::DBURT_PATH + UTL::SLASH_SLASH + cdt + UTL::dotDBURT;
    else
        fn = UTL::DBURT_PATH+fileName + UTL::dotDBURT;

    std::ofstream outputFile;

    outputFile.open( fn, std::ios::out);

    if( outputFile )
    {
        outputFile << UTL::DBURT_HEADER_V2 << std::endl;
        outputFile << std::endl;
        outputFile << UTL::DBURT_HEADER_DT  << cdt << UTL::END_OF_LINE << std::endl;
        outputFile << std::endl;
        outputFile << UTL::DBURT_HEADER_COM << comments << UTL::END_OF_LINE << std::endl;
        outputFile << std::endl;
        outputFile << UTL::START_OF_DATA << std::endl;

        outputFile << UTL::NUMBER_OF_OBJECTS << UTL::EQUALS_SIGN << magState.numMags << std::endl;
        for(size_t i = 0; i < magState.numMags; ++i)
        {
            outputFile << magState.magNames[i] << UTL::COLON_C;
            switch( magState.psuStates[i] )
            {
                case VC_ENUM::MAG_PSU_STATE::MAG_PSU_OFF:
                    outputFile << UTL::OFF << UTL::COLON_C;
                    break;

                case VC_ENUM::MAG_PSU_STATE::MAG_PSU_ON:
                    outputFile << UTL::ON << UTL::COLON_C;
                    break;

                case VC_ENUM::MAG_PSU_STATE::MAG_PSU_TIMING:
                    outputFile << UTL::TIMING<< UTL::COLON_C;
                    break;

                case VC_ENUM::MAG_PSU_STATE::MAG_PSU_ERROR:
                    outputFile << UTL::ERROR<< UTL::COLON_C;
                    break;

                case VC_ENUM::MAG_PSU_STATE::MAG_PSU_NONE:
                     outputFile << UTL::NONE<< UTL::COLON_C;
                    break;

                default:
                    outputFile << UTL::ERROR<< UTL::COLON_C;
            }
            outputFile << magState.siValues[i] << ";" << std::endl;
        }
        outputFile << UTL::END_OF_DATA;
        outputFile.close();
        success = true;
    }
    else{

        std::cout << "Can't create output file..." << std::endl;
    }
    return success;
}







