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
#include "velaINJscreenInterface.h"
#include "controller.h"
#include "structs.h"
//stl
#include <string>
#include <vector>



class velaINJscreenController : public controller
{
    public:
        velaINJscreenController();
        velaINJscreenController(const bool show_messages,
                         const bool show_debug_messages,
                         const std::string & conf1,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const VELA_ENUM::MACHINE_AREA);
        ~velaINJscreenController();

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name );

        virtual double get_CA_PEND_IO_TIMEOUT();
        virtual void   set_CA_PEND_IO_TIMEOUT( double val );



  // this is the main move funciton, all higher level versions (i.e. screenIN) end up here, this does all the hard work / logic
        bool screenMoveTo( const std::vector< std::string > & names, const std::vector< screenStructs::SCREEN_STATE > & states);
        bool screenMoveTo( const std::string & name, const screenStructs::SCREEN_STATE & states);
        bool screenIN ( const std::string & name );
        bool screenOUT( const std::string & name );
        bool screenOUT( const std::vector< std::string > & names );
        bool screenIN ( const std::vector< std::string > & names );
        bool setPosition(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const double value );


        bool isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta);

        // 'existential quantification' - ahem
        bool isMoving(const std::string& name);
        bool isNotMoving(const std::string& name);
        bool is_VELA_PNEUMATIC(const std::string & name);
        bool is_VELA_HV_MOVER (const std::string & name);
        bool is_H_Element(screenStructs::SCREEN_STATE e);
        bool is_V_Element(screenStructs::SCREEN_STATE e);
        bool isScreenOUT(const std::string & name);
        bool isScreenIN(const std::string & name);
        std::vector<bool> isScreenIN(const std::vector<std::string> & name );
        std::vector<bool> isScreenOUT(const std::vector<std::string> & name );
        std::vector<bool> exists_and_isLocked(const std::string& name);
        std::vector<bool> exists_and_isNotLocked(const std::string& name);

        const std::vector<std::string> get_VELA_PNEUMATIC_Screens(const std::vector< std::string > & names );
        const std::vector<std::string> get_VELA_HV_MOVER_Screens( const std::vector< std::string > & names );

        // i want to make sure so python does not have access to the second arguemnt
        screenStructs::SCREEN_STATE getScreenState(const std::string & name);












#ifdef BUILD_DLL
        boost::python::dict getILockStatesStr_Py(std::string& name);
        boost::python::dict getILockStates_Py   (std::string& name);
        boost::python::dict get_SCREEN_STATE_Definition();
        boost::python::dict get_DRIVER_STATE_Definition();
#endif // BUILD_DLL

        // YOU CANNOT SET THE CONTROLLER TYPE, NO-WAY JOSE
        VELA_ENUM::MACHINE_AREA getmyMachineArea(){return myMachineArea;};
        // void setmyMachineArea(){ no chance matey };

    private:
        void initialise();
        ///No singletons, no pointers, let's just have an object
        velaINJscreenInterface localInterface;
        std::vector< std::string > ScreenNames;
        bool shouldStartEPICs, startVirtualMachine;
        const VELA_ENUM::MACHINE_AREA myMachineArea;

};
#endif //velaScreen_CONTROLLER_H
