#include "velaCamConfigReader.h"

velaCamConfigReader::velaCamConfigReader(
                                         const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  )
: configReader(UTL::CONFIG_PATH, show_messages_ptr, show_debug_messages_ptr )
{
}

velaCamConfigReader::~velaCamConfigReader()
{
    //dtor
}
