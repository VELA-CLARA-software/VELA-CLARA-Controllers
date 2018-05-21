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
//  Last edit:   17-05-2018
//  FileName:    fastCamConfigReader.h
//  Description:
//
//
//
//*/
#ifndef _FAST_CAM_CONFIGREADER_H
#define _FAST_CAM_CONFIGREADER_H
// project includes
#include "fastCamStructs.h"
#include "configReader.h"
// stl includes
#include <string>
#include <map>
#include <vector>
//______________________________________________________________________________
class fastCamConfigReader : public configReader
{
    public:

        fastCamConfigReader::fastCamConfigReader(const std::string& velaConfig,
                                                 const std::string& claraConfig,
                                                 const bool startVirtualMachine,
                                                 const bool& show_messages,
                                                 const bool& show_debug_messages);
        ~fastCamConfigReader();

        bool readConfig();
        bool getCamData(std::map<std::string, fastCamStructs::fastCamObject> & mapToFill);

    protected:

    private:

        bool readConfig(const std::string& file);

        std::vector<fastCamStructs::fastCamObject> fCamObjects;
        std::vector<fastCamStructs::pvStruct> pvFCamMonStructs;
        std::vector<fastCamStructs::pvStruct> pvFCamComStructs;

        void addToObjectsV1(const std::vector<std::string> &keyVal);

        fastCamStructs::CAM_TYPE getCamType(const std::string& cam);

        void addToPVStruct(std::vector<fastCamStructs::pvStruct> & pvStruct_v, const  std::vector<std::string> &keyVal);
        void addCOUNT_MASK_OR_CHTYPE( std::vector<fastCamStructs::pvStruct>  & pvStruct_v, const std::vector<std::string> &keyVal);


};

#endif // FASTCAMCONFIGREADER_H
