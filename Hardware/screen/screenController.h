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

#ifndef velaScreen_CONTROLLER_H
#define velaScreen_CONTROLLER_H
//project
#include "screenStructs.h"
#include "screenInterface.h"
#include "controller.h"
#include "structs.h"
//stl
#include <string>
#include <vector>



class screenController : public controller
{
    public:
        screenController();
        screenController( const bool show_messages,    const bool show_debug_messagese,
                          const std::string & magConf, const std::string & NRConf,
                          const bool shouldStartEPICs );
        ~screenController();

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name );

        virtual double get_CA_PEND_IO_TIMEOUT();
        virtual void   set_CA_PEND_IO_TIMEOUT( double val );

        void Screen_Out( const std::string & name );
        void Screen_In( const std::string & name );
        void Screen_Move( const std::string & name, const std::string & position );
        void Screen_Stop( const std::string & name );
        void All_Out();

        void controller_move_to_position( const std::string & name, const std::string & position );
        void controller_move_to( const std::string & name, const std::string & V_H, const double & position );

        bool IsOut( const std::string & name );
        bool IsIn( const std::string & name );
        bool horizontal_disabled_check( const std::string & name );
        bool vertical_disabled_check( const std::string &  name );

        std::vector< std::string > getScreenNames();

        std::string getComplexHorizontalScreenStateStr( const std::string & name );
        std::string getComplexVerticalScreenStateStr( const std::string & name );
        std::string getSimpleScreenStateStr( const std::string & name );
        std::string getScreenStateStr( const std::string & name );
        std::string getScreenStateStr( const std::string & name, const std::string & V_H );

    ///write a method that returns string version of enums using ENUM_TO_STRING

        screenStructs::SCREEN_STATE getComplexHorizontalScreenState( const std::string & name );
        screenStructs::SCREEN_STATE getComplexVerticalScreenState( const std::string & name );
        screenStructs::SCREEN_STATE getSimpleScreenState( const std::string & name );
        screenStructs::SCREEN_STATE getScreenState( const std::string & name );
        screenStructs::SCREEN_STATE getScreenState( const std::string & name, const std::string & V_H );

        double getComplexScreenHorizontalPosition( const std::string & name );
        double getComplexScreenVerticalPosition( const std::string & name );
        double getScreenPosition( const std::string & name, const std::string & V_H );

        void get_info( const std::string & name );
        void get_config_values( const std::string & name );




    private:
        void initialise();
        ///No singletons, no pointers, let's just have an object
        screenInterface localInterface;
        std::vector< std::string > ScreenNames;
        bool shouldStartEPICs;
};


#endif //velaScreen_CONTROLLER_H
