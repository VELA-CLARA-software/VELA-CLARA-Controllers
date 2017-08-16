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

#include "cameraDAQController.h"
// stl
cameraDAQController::cameraDAQController(
    const bool show_messages,
    const bool show_debug_messages,
    const std::string & magConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const VELA_ENUM::MACHINE_AREA myMachineArea):
controller( show_messages, show_debug_messages ),
localInterface(magConf, startVirtualMachine, &SHOW_MESSAGES, &SHOW_DEBUG_MESSAGES, shouldStartEPICs, myMachineArea ),
shouldStartEPICs(shouldStartEPICs),
myMachineArea(myMachineArea)
{
//    if( shouldStartEPICs )
//    message("magnet controller shouldStartEPICs is true");
//    else
//    message("magnet controller shouldStartEPICs is false");
//    initialise();
}
cameraDAQController::~cameraDAQController(){}    //dtor
std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> cameraDAQController::getILockStates(const std::string & name)
{
    return localInterface.getILockStates( name );
}
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > cameraDAQController::getILockStatesStr( const std::string & name )
{
    return localInterface.getILockStatesStr( name );
}
double cameraDAQController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
void   cameraDAQController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
///Functions Accessible to Python Controller///
//Generic Functions
bool cameraDAQController::isON ( const std::string & cam )
{
    return localInterface.isON( cam );
}
bool cameraDAQController::isOFF( const std::string & cam )
{
    return localInterface.isOFF( cam );
}
bool cameraDAQController::isAquiring ( const std::string & cam )
{
    return localInterface.isAquiring( cam );
}
bool cameraDAQController::isNotAquiring ( const std::string & cam )
{
    return localInterface.isNotAquiring( cam );
}
std::string cameraDAQController::selectedCamera()
{
    return localInterface.selectedCamera();
}
bool cameraDAQController::setCamera(const std::string & cam)
{
    return localInterface.setCamera(cam);
}
bool cameraDAQController::startAquiring()
{
    return localInterface.startAquiring();
}
bool cameraDAQController::stopAquiring()
{
    return localInterface.stopAquiring();
}
bool cameraDAQController::startVCAquiring()
{
    return localInterface.startVCAquiring();
}
bool cameraDAQController::stopVCAquiring()
{
    return localInterface.stopVCAquiring();
}
//DAQ Specific Functions
bool cameraDAQController::collect(const std::string &io, const int & numbOfShots)
{
    unsigned short comm(0);
    if(io=="ON"||io=="On"||io=="on")
        comm=1;
    else if(io=="OFF"||io=="Off"||io=="off")
        comm=0;
    else
        message("Collect input not recognised, please use 'ON' or 'OFF'");
    return localInterface.collect(comm, numbOfShots);
}
bool cameraDAQController::save(const std::string &io)
{
    unsigned short comm(0);
    if(io=="ON"||io=="On"||io=="on")
        comm=1;
    else if(io=="OFF"||io=="Off"||io=="off")
        comm=0;
    else
        message("Write input not recognised, please use 'ON' or 'OFF'");
    return localInterface.save(comm);
}
bool cameraDAQController::collectAndSave(const int & numbOfShots)
{
    return localInterface.collectAndSave(numbOfShots);
}
bool cameraDAQController::killCollectAndSave()
{
    return localInterface.killCollectAndSave();
}
const cameraStructs::cameraDAQObject& cameraDAQController::getCamDAQObjConstRef( const std::string & camName  )
{
    return localInterface.getCamDAQObjConstRef( camName );
}
const cameraStructs::cameraDAQObject& cameraDAQController::getSelectedDAQRef()
{
    return localInterface.getSelectedDAQRef();
}
const cameraStructs::cameraDAQObject& cameraDAQController::getVCDAQRef()
{
    return localInterface.getVCDAQRef();
}
