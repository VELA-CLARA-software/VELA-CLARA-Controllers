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
//  FileName:    pilaserMirrorConfigReader.cpp
//  Description:
//
//
//*/
#ifndef _PI_LASER_MIRROR_CONFIG_READER_H_
#define _PI_LASER_MIRROR_CONFIG_READER_H_
// stl
#include <string>
#include <vector>
#include <map>
// me
#include "configReader.h"
#include "pilaserStructs.h"


class pilaserMirrorConfigReader : public configReader
{
    public:
        pilaserMirrorConfigReader(const std::string& configFile,
                     const bool* show_messages,
                     const bool* show_debug_messages,
                     const bool usingVM);
        ~pilaserMirrorConfigReader();

//        bool readConfig( );
//        bool getpilaserObject(pilaserStructs::pilaserObject & obj);

    private:
//        pilaserStructs::pilaserObject pilaserObject;
//        std::vector<pilaserStructs::pvStruct> pvMonStructs;
//        std::vector<pilaserStructs::pvStruct> pvComStructs;
//
//        // hand pointer so we cna keep track of the last PV struct we were adding data to
//        //std::vector< pilaserStructs::pvStruct > * lastPVStruct;
//
//        void addToPVStruct(std::vector< pilaserStructs::pvStruct > & pvs, const pilaserStructs::PILASER_PV_TYPE pvtype, const std::string& pvSuffix);
//
//        void addTopilaserObjectsV1( const std::vector<std::string> &keyVal );
//        void addToPVCommandMapV1  ( const std::vector<std::string> &keyVal );
//        void addToPVMonitorMapV1  ( const std::vector<std::string> &keyVal );
//
//        const bool usingVirtualMachine;
};
#endif //_PI_LASER_MIRROR_CONFIG_READER_H_
