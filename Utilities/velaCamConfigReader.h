#ifndef VELACAMCONFIGREADER_H
#define VELACAMCONFIGREADER_H


// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// me
#include "configReader.h"
#include "velaCamStructs.h"

class velaCamConfigReader : configReader
{
    public:
        velaCamConfigReader();
        velaCamConfigReader(const std::string& configFileLocation1,const bool startVirtualMachine,const bool * showMessages,const  bool * showDebugMessages );
        virtual ~velaCamConfigReader();

    protected:

    private:
};

#endif // VELACAMCONFIGREADER_H
