#ifndef DBURT_DEFINITIONS_H
#define DBURT_DEFINITIONS_H

#include <string>

namespace UTL
{
    const std::string NUM_MAGNETS = "NUM_MAGNETS";
    const std::string ON     = "ON";
    const std::string OFF    = "OFF";
    const std::string TIMING = "TIMING";
    const std::string ERROR  = "ERROR";
    const std::string NONE   = "NONE";

    const std::string VELA_MAGNET_SAVE_FILE_v1 = "VELA MAGNET SAVE FILE v1.0"; /// this is the old version


    const std::string DBURT_PATH  = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Snapshots\\DBURT\\";


    const std::string DBURT_EOF_V1 = "END OF FILE";

    const std::string DBURT_NUM_MAGNETS_V1 = "NUM MAGNETS:";
    const std::string DBURT_PARAMETERS_V1  = "PARAMETERS;";
    //const std::string DBURT_PARAMETERS_V1  = "PARAMETERS;";


    const std::string DBURT_HEADER_V2   = "VELA DBURT (MAGNET SAVE FILE) v2.0";
    const std::string DBURT_HEADER_DT   = "DATE_TIME:\t";
    const std::string DBURT_HEADER_COM  = "COMMENTS:\t";
    const std::string DBURT_HEADER_AREA = "BEAM_AREA:\t";
    const std::string DBURT_HEADER_KEYW = "KEY_WORDS:\t";



    const std::string DBURT_HEADER_V3 = "VELA-CLARA DBURT (MAGNET SAVE FILE) v3";

    const std::string dotDBURT    = ".dburt";

}
#endif //DBURT_DEFINITIONS_H
