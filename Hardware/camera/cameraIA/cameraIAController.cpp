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

#include "cameraController.h"
// stl
cameraController::cameraController(
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
cameraController::~cameraController(){}    //dtor
std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> cameraController::getILockStates(const std::string & name)
{
    return localInterface.getILockStates( name );
}
std::map< VELA_ENUM::ILOCK_NUMBER, std::string > cameraController::getILockStatesStr( const std::string & name )
{
    return localInterface.getILockStatesStr( name );
}
double cameraController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
void   cameraController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}
bool cameraController::isON ( const std::string & cam )
{
    return localInterface.isON( cam );
}
bool cameraController::isOFF( const std::string & cam )
{
    return localInterface.isOFF( cam );
}
bool cameraController::isMonitoring ( const std::string & cam )
{
    return localInterface.isMonitoring( cam );
}
bool cameraController::isNotMonitoring ( const std::string & cam )
{
    return localInterface.isNotMonitoring( cam );
}
std::string cameraController::cameraName()
{
    return localInterface.cameraName();
}
bool cameraController::setCamera(const std::string & cam)
{
    return localInterface.setCamera(cam);
}
bool cameraController::calibrate()
{
    return localInterface.calibrate();
}
bool cameraController::setXRatio(const double & r)
{
    return localInterface.setXRatio(r);
}
bool cameraController::setYRatio(const double & r)
{
    return localInterface.setYRatio(r);
}
bool cameraController::start()
{
    return localInterface.start();
}
bool cameraController::stop()
{
    return localInterface.stop();
}
bool cameraController::collectAndSave(int & numbOfShots, const std::string & directory)
{
    return localInterface.collectAndSave(numbOfShots, directory);
}
std::vector< cameraStructs::camDataType > cameraController::getRawData()
{
    return localInterface.getRawData();
}
std::vector< cameraStructs::camDataType > cameraController::getBackgroundRawData()
{
    return localInterface.getBackgroundRawData();
}
double cameraController::getX()
{
    return localInterface.getX();
}
double cameraController::getY()
{
    return localInterface.getY();
}
double cameraController::getSigmaX()
{
    return localInterface.getSigmaX();
}
double cameraController::getSigmaY()
{
    return localInterface.getSigmaY();
}
double cameraController::getSigmaXY()
{
    return localInterface.getSigmaXY();
}
const cameraStructs::cameraObject& cameraController::getCamObjConstRef( const std::string & camName  )
{
    return localInterface.getCamObjConstRef( camName );
}
