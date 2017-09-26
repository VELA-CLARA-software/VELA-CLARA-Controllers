
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
#ifndef CONFIG_READER_CAM_H_
#define CONFIG_READER_CAM_H_
// stl
#include <string>
#include <vector>
#include <map>
#include <iostream>
// tp
#include "configReader.h"
#include "cameraStructs.h"
using namespace cameraStructs;

class cameraIAConfigReader : public configReader
{
    public:
        cameraIAConfigReader();
        cameraIAConfigReader::cameraIAConfigReader(const std::string&camConfig,
                                                const bool startVirtualMachine,
                                                const bool*show_messages_ptr,
                                                const bool*show_debug_messages_ptr);
        ~cameraIAConfigReader();

        bool readConfig();
        bool getCamData(std::map<std::string,cameraObject> &mapToFill);

    private:
        bool readCameraConfig();

        void addToPVStruct(std::vector<pvStruct> &pvStruct_v,
                           const std::vector<std::string> &keyVal );

        void addToCameraObjects(const std::vector<std::string> &keyVal  );
        void addToCameraMonitorStructs(const std::vector<std::string> &keyVal);
        void addToCameraCommandStructs(const std::vector<std::string> &keyVal);

        void addCOUNT_MASK_OR_CHTYPE(std::vector<pvStruct> &pvStruct_v,
                                     const std::vector<std::string> &keyVal  );

        std::vector< cameraObject > camObject;
        std::vector< pvStruct > pvCameraMonStructs;
        std::vector< pvStruct > pvCameraComStructs;


         const std::string camConfig;
         const bool usingVirtualMachine;


};
#endif //CONFIG_READER_CAM_H_
