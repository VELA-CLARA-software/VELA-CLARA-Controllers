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

#ifndef IMG_CONTROLLER_H
#define IMG_CONTROLLER_H

// project
#include "invertedMagnetronGaugeStructs.h"
#include "invertedMagnetronGaugeInterface.h"
#include "controller.h"
// stl
#include <string>
#include <vector>


class invertedMagnetronGaugeController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        invertedMagnetronGaugeController();
        invertedMagnetronGaugeController( const std::string &configFileLocation1,
                                          const bool show_messages,
                                          const bool show_debug_messages,
                                          const bool shouldStartEPICS,
                                          const bool startVirtualMachine,
                                          const VELA_ENUM::MACHINE_AREA myMachineArea );
        ~invertedMagnetronGaugeController();

        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );

        double getImgP( const std::string & vacImgName );
        const invertedMagnetronGaugeStructs::vacImgObject & getIMGObjConstRef(const std::string & imgName);

        /// write a method that returns string version of enums using ENUM_TO_STRING

        invertedMagnetronGaugeStructs::IMG_STATE getImgState( const std::string & vacImgName );
        std::string getImgStateStr( const std::string & name );

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & objName );



    protected:
    private:

        void initialise();

        /// No singletons, no pointers, let's just have an object

        invertedMagnetronGaugeInterface localInterface;

        std::vector< std::string > vacImgNames;

        const bool shouldStartEPICS;
        const VELA_ENUM::MACHINE_AREA machineArea;
};


#endif // invertedMagnetronGaugeController_H
