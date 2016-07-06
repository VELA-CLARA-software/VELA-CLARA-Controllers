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

#ifndef VELA_PIL_SHUTTER_INTERFACE_H
#define VELA_PIL_SHUTTER_INTERFACE_H
// djs
#include "interface.h"
#include "shutterStructs.h"
#include "shutterConfigReader.h"
//stl
#include <vector>
#include <string>

class shutterInterface : public interface
{
    public:


        shutterInterface( const bool* show_messages_ptr, const bool* show_debug_messages_ptr  );
        shutterInterface( const std::string &configFileLocation, const bool* show_messages_ptr, const bool* show_debug_messages_ptr  );

        ~shutterInterface();

        void close( const std::string &name );
        void open ( const std::string &name );

        bool isClosed( const std::string &name );
        bool isOpen  ( const std::string &name );

        bool closeAndWait( const std::string & name, const time_t waitTime );
        bool openAndWait ( const std::string & name, const time_t waitTime );

        void getShutterNames( std::vector< std::string > &names );
        VELA_ENUM::SHUTTER_STATE getShutterState( const std::string & name ); /// getShutterStateStr is in the controller

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & name );

        shutterStructs::shutterObject * getShutterObject( const std::string & name );


    protected:
    private:

        /// called from constructor to set-up chids, monitors, etc.
        void initialise();
        shutterConfigReader configReader; /// class member so we can pass in file path in ctor
        void initObjects();
        void initChids();
        void addChannel( const std::string & pvRoot, shutterStructs::pvStruct & pv );

        void startMonitors();

        void checkShutterCHIDStates();

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the shutter is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members

        typedef  bool ( shutterInterface::*isOCMemFn)(const std::string & );
        typedef  void ( shutterInterface::*OCMemFn   )  (const std::string & );

        bool toggleShutterAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, shutterInterface & obj );

        void toggleShutter( chtype & cht, chid & chi, const char * m1 = "", const char * m2 = "");

        /// static function that can be called back from epics to update values

        static void staticEntryShutterMonitor( const event_handler_args args );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        ///
        std::vector< shutterStructs::monitorStuct * > continuousMonitorStructs;
        std::map< std::string, shutterStructs::shutterObject > allShutterData; /// All the shutter data is stored in this map, keyed by the shutter name
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
