/*
//              This file is part of VELA-CLARA-Controllers.                          //
//------------------------------------------------------------------------------------//
//    VELA-CLARA-Controllers is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by            //
//    the Free Software Foundation, either version 3 of the License, or               //
//    (at your option) any later version.                                             //
//    VELA-CLARA-Controllers is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   //
//    GNU General Public License for more details.                                    //
//                                                                                    //
//    You should have received a copy of the GNU General Public License               //
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
//
//  Author:      DJS
//  Last edit:   29-03-2018
//  FileName:    virtualCathodeConfigReader.h
//  Description:
//
//
//*/
#ifndef _VIRTUAL_CATHODE_CONFIG_READER_
#define _VIRTUAL_CATHODE_CONFIG_READER_
// stl
#include <string>
#include <vector>
#include <map>
#include <fstream>
// me
#include "configReader.h"
#include "pilaserStructs.h"
//______________________________________________________________________________
class virtualCathodeConfigReader:public configReader
{
    public:
        virtualCathodeConfigReader(const std::string& vcMirrorConfig,
                                   const std::string& vcDataConfig,
                                   const bool& show_messages,
                                   const bool& show_debug_messages,
                                   const bool usingVM);
        ~virtualCathodeConfigReader();
        bool readConfig();
        bool getVirtualCathodeObject(pilaserStructs::virtualCathodeDataObject & obj);
    private:

        bool readingMrror, readingData;

        bool readConfig(std::ifstream& inputFile, const std::string& configFile);

        void addToPVCommandMapV1(const std::vector<std::string>&keyVal);

        void addCOUNT_MASK_OR_CHTYPE(std::vector<pilaserStructs::pvStruct>& pvStruct_v,
                                                  const std::vector<std::string>& keyVal);
        pilaserStructs::virtualCathodeDataObject vcObject;

        std::vector<pilaserStructs::pvStruct> pvMonStructs;
        std::vector<pilaserStructs::pvStruct> pvComStructs;


        void addToPVStruct(std::vector< pilaserStructs::pvStruct >& pvs,
                           const pilaserStructs::PILASER_PV_TYPE pvtype, const std::string& pvSuffix);
        void addToObjectsV1(const std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1  (const std::vector<std::string> &keyVal );
};
//______________________________________________________________________________
#endif //_VIRTUAL_CATHODE_CONFIG_READER_
