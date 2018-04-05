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
//  FileName:    pilaserMirrorConfigReader.h
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
// project includes
#include "configReader.h"
#include "pilaserMirrorStructs.h"
//______________________________________________________________________________
class pilaserMirrorConfigReader : public configReader
{
    public:
        pilaserMirrorConfigReader(const std::string& configFile,
                                  const bool& show_messages,
                                  const bool& show_debug_messages,
                                  const bool usingVM);
        ~pilaserMirrorConfigReader();

        bool readConfig();
        bool getpilMirrorObject(pilaserMirrorStructs::pilMirrorObject& obj);

    private:
        pilaserMirrorStructs::pilMirrorObject pilMirrorObject;

        std::vector<pilaserMirrorStructs::pvStruct> pvMonStructs;
        std::vector<pilaserMirrorStructs::pvStruct> pvComStructs;

        void addToPVCommandMapV1(const std::vector<std::string>& keyVal);

        void addPVStruct(std::vector<pilaserMirrorStructs::pvStruct>& pvs,
                         const std::vector<std::string>& keyVal);

        void addTopilaserObjectsV1(const std::vector<std::string> &keyVal );
};
//______________________________________________________________________________
#endif //_PI_LASER_MIRROR_CONFIG_READER_H_
