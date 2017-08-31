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

#include "invertedMagnetronGaugeController.h"
// stl
#include <iostream>
// djs
#include "invertedMagnetronGaugeInterface.h"

//______________________________________________________________________________
invertedMagnetronGaugeController::invertedMagnetronGaugeController( const std::string &configFileLocation1,
                                                                    const bool show_messages,
                                                                    const bool show_debug_messages,
                                                                    const bool shouldStartEPICS,
                                                                    const bool startVirtualMachine,
                                                                    const VELA_ENUM::MACHINE_AREA myMachineArea ):
controller( show_messages, show_debug_messages ),
localInterface( configFileLocation1, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES, shouldStartEPICS, startVirtualMachine, myMachineArea ),
shouldStartEPICS( shouldStartEPICS ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
void invertedMagnetronGaugeController::initialise()
{
    if( localInterface.interfaceInitReport( shouldStartEPICS ) )
        message("invertedMagnetronGaugeController instantiation success.");
}
//______________________________________________________________________________
invertedMagnetronGaugeController::~invertedMagnetronGaugeController(){}    //dtor
//______________________________________________________________________________
double invertedMagnetronGaugeController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
//______________________________________________________________________________
void invertedMagnetronGaugeController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
//______________________________________________________________________________
std::vector< std::vector< double > > invertedMagnetronGaugeController::monitorForNCounts( const std::string & vacImgName, int numcounts )
{
    return localInterface.monitorForNCounts( vacImgName, numcounts );
}
//______________________________________________________________________________
double invertedMagnetronGaugeController::getImgP( const std::string & name )
{
    return localInterface.getImgP( name );
}
//______________________________________________________________________________
VELA_ENUM::IMG_STATE invertedMagnetronGaugeController::getImgState( const std::string & name )
{
    return localInterface.getImgState( name );
}
//______________________________________________________________________________
const invertedMagnetronGaugeStructs::vacImgObject & invertedMagnetronGaugeController::getIMGObjConstRef( const std::string & name )
{
    return localInterface.getIMGObjConstRef( name );
}
//______________________________________________________________________________
std::string invertedMagnetronGaugeController::getImgStateStr( const std::string & name )
{
    return ENUM_TO_STRING(localInterface.getImgState( name ));
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > invertedMagnetronGaugeController::getILockStates( const std::string & objName )
{
    return localInterface.getILockStates( objName );
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > invertedMagnetronGaugeController::getILockStatesStr( const std::string & objName )
{
    return localInterface.getILockStatesStr( objName );
}
//______________________________________________________________________________
