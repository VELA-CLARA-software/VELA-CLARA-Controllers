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

#ifndef screenInterface_H
#define screenInterface_H
//djs

#include "interface.h"
#include "screenStructs.h"
#include "screenConfigReader.h"
//stl
#include <vector>
#include <string>



class screenInterface: public interface
{
    public:

            ///Not a singleton, two construction methods...


            screenInterface( const std::string & configFileLocation1, const std::string & configFileLocation2,
                             const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                             const bool shouldStartEPICs );
            ~screenInterface();

            void Screen_Out( const std::string & name );
            void Screen_In( const std::string & name );
            void Screen_Move( const std::string & name, const std::string & position );
            void All_Out();

            void Out( const std::string &name );
            void In( const std::string &name );
            void Stop( const std::string & name );

            bool IsOut( const std::string &name );
            bool IsIn ( const std::string &name );
            bool horizontal_disabled_check( const std::string &name );
            bool vertical_disabled_check( const std::string &name );
            bool position_value_check( double val );

            void move_to(const std::string &name, const std::string & V_H, const double & position );
            void move_to_position( const std::string & name, const std::string & position );

            void getScreenNames( std::vector< std::string > &names );

            screenStructs::SCREEN_STATE getScreenState( const std::string &name, const std::string &V_H );
            screenStructs::SCREEN_STATE getScreenState( const std::string &name );
            screenStructs::SCREEN_STATE getComplexHorizontalScreenState ( const std::string & name );
            screenStructs::SCREEN_STATE getComplexVerticalScreenState ( const std::string & name );
            screenStructs::SCREEN_STATE getSimpleScreenState ( const std::string & name );

            double getComplexScreenHorizontalPosition( const std::string &name );
            double getComplexScreenVerticalPosition( const std::string &name );
            double getScreenPosition( const std::string &name, const std::string &V_H );

            /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...
            std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
            std::map< VELA_ENUM::ILOCK_NUMBER, std::string >  getILockStatesStr( const std::string & name );

            void get_info( const std::string & name );
            void get_config_values( const std::string & name );

            void monitorScreens();

            bool Is_complex( const std::string & name );
            bool Is_simple( const std::string & name );

            bool screenExists( const std::string & name );





    protected:
    private:

        /// called from the constructor to set-up chids, monitors, etc.
        void initialise(const bool shouldStartEPIC);
        screenConfigReader configReader; ///class member so we can pass in file path in ctor

        void initScreenObjects();
        void initScreenChids();

        void addChannel( const std::string & pvRoot, screenStructs::pvStruct & pv );//if this doesn't work then try commented out function in shutter program

        void addToComplexMonitorStructs( std::vector< screenStructs::monitorStruct * > & cms, screenStructs::pvStruct & pv, screenStructs::COMPLEX_YAG_Object * COMPLEX_YAG  );
        void addToSimpleMonitorStructs( std::vector< screenStructs::monitorStruct * > & cms, screenStructs::pvStruct & pv, screenStructs::SIMPLE_YAG_Object * SIMPLE_YAG  );

        void checkScreenCHIDStates();


        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the shutter is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members

        typedef  bool (screenInterface::*isOCMemFn)(const std::string & );
        typedef  void (screenInterface::*OCMemFn)  (const std::string & );

        void toggleScreen( chtype & cht, chid & chi, const char * m1 = "", const char * m2 = "");

        ///static function that can be called back from epics to update values

        static void staticEntryScreenMonitor( const event_handler_args args );
        static void UpdateDouble( screenStructs::monitorStruct * ms, const void * argsdbr );
        static void UpdateEnum( screenStructs::monitorStruct * ms, const void * argsdbr );

        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor
        std::vector< screenStructs::monitorStruct * > continuousMonitorStructs;


        screenStructs::screenObject ScreenObject;

};
#endif //VELA_PyIL_SCREEN_INTERFACE_H
