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
//  FileName:    shutterInterface.h
//  Description:
//
//
//*/
#ifndef VELA_CLARA_SHUTTER_INTERFACE_H
#define VELA_CLARA_SHUTTER_INTERFACE_H
// project includes
#include "interface.h"
#include "shutterStructs.h"
#include "shutterConfigReader.h"
// stl includes
#include <vector>
#include <string>
//______________________________________________________________________________
class shutterInterface : public interface
{
    public:
        shutterInterface(bool& show_messages,
                         bool& show_debug_messages,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const std::string& configFile
                        );
        ~shutterInterface();

        bool closeAndWait(const std::string& name, const time_t waitTime);
        bool openAndWait (const std::string& name, const time_t waitTime);
        bool isClosed(const std::string& name) const;
        bool isOpen  (const std::string& name) const;
        void close(const std::string& name);
        void open (const std::string& name);

        std::vector<std::string> getShutterNames()const;
        shutterStructs::shutterObject* getShutterObject(const std::string& name);
        const shutterStructs::shutterObject&
            getShutterObjConstRef(const std::string& name )const;

        shutterStructs::SHUTTER_STATE getShutterState(const std::string& name);

        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string& name)const;


    protected:
    private:
        /*
            All the shutter data is stored in this map, keyed by the shutter name
        */
        std::map<std::string, shutterStructs::shutterObject> allShutterData;
        const shutterStructs::shutterObject dummObject;
        shutterConfigReader configReader;

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string& pvRoot, shutterStructs::pvStruct& pv );
        void startMonitors();

        bool is_in_state(const std::string& name,shutterStructs::SHUTTER_STATE sta)const;
        /*
            As an overly complicated example, let's try some function pointers.
            Toggling (open / close ) the shutter is now easy
            https://isocpp.org/wiki/faq/pointers-to-members
https://stackoverflow.com/questions/16498969/how-do-i-typedef-a-function-pointer-with-the-c11-using-syntax
            isOCMemFn => is_Open_or_Close_Member_Function
            OCMemFn   =>    Open_or_Close_Member_Function
        */
        using isOCMemFn = bool(shutterInterface::*)(const std::string&) const;
        using OCMemFn   = void(shutterInterface::*)(const std::string&);
        //typedef  bool (shutterInterface::*isOCMemFn)(const std::string & );
        //typedef  void (shutterInterface::*OCMemFn   )  (const std::string & );
//
        bool toggleShutterAndWait(isOCMemFn f1,
                                  isOCMemFn f2,
                                  OCMemFn f3,
                                  const std::string& name,
                                  const time_t waitTime,
                                  shutterInterface& obj );
//
        void toggleShutter(chtype& cht,chid& chi,const char* m1 = "",const char* m2 = "");

        static void staticEntryShutterMonitor(const event_handler_args args);

        std::vector<shutterStructs::monitorStuct*> continuousMonitorStructs;


};
#endif // VELA_CLARA_SHUTTER_INTERFACE_H
