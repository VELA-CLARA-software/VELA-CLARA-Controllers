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
//  Last edit:   19-03-2018
//  FileName:    L01ProtConfigReader.h
//  Description:
//
//
//*/
#ifndef _L01_PROT_CONFIG_READER_H_
#define _L01_PROT_CONFIG_READER_H_
// stl includes
#include <string>
#include <vector>
#include <map>
#include <iostream>
// project includes
#include "configReader.h"
#include "rfProtStructs.h"
//______________________________________________________________________________
class L01ProtConfigReader : public configReader
{
    public:
        L01ProtConfigReader(const bool& show_messages_ptr,
                            const bool& show_debug_messages_ptr );
        L01ProtConfigReader(const std::string& configFileLocation1,
                            const bool startVirtualMachine,
                            const bool& showMessages,
                            const bool& showDebugMessages );
        ~L01ProtConfigReader();

        bool readConfig();
        bool getrfProtObjects(std::map<std::string,
                                 rfProtStructs::rfProtObject>& mapToFill);
    private:
        /* local virtual hardware object, copied to the interface in
            getrfProtObjects()
        */
        rfProtStructs::rfProtObject localrfProtObject;
        /*
            pvMonStructs hold the data for PVs to be monitored
            pvComStructs hold the data for PVs to be monitored and/or controlled
        */
        std::vector<rfProtStructs::pvStruct> pvMonStructs;
        std::vector<rfProtStructs::pvStruct> pvComStructs;

        /* Functions to add the config file data to objects/pvStructs */
        void addToProtObjectsV1(const std::vector<std::string> &keyVal );
        void addToPVMonitorMapV1(const std::vector<std::string> &keyVal );
        void addToPVCommandMapV1(const std::vector<std::string> &keyVal );

        std::vector<rfProtStructs::rfProtObject> gunProtObjects;

        void addToPVStruct(std::vector<rfProtStructs::pvStruct>& pvStruct_v,
                           const std::vector<std::string>& keyVal);

        void addCOUNT_MASK_OR_CHTYPE(std::vector<rfProtStructs::pvStruct>& pvStruct_v,
                                     const std::vector<std::string>& keyVal);
};
#endif //_GUN_PROT_CONFIG_READER_H_
