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

#include "cameraIAController.h"

// stl
cameraIAController::cameraIAController(
    const bool show_messages,
    const bool show_debug_messages,
    const VELA_ENUM::CONTROLLER_TYPE type,
    const std::string & magConf,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const VELA_ENUM::MACHINE_AREA myMachineArea):
controller( show_messages, show_debug_messages, type),
localInterface(magConf,
               startVirtualMachine,
               &SHOW_MESSAGES,
               &SHOW_DEBUG_MESSAGES,
               shouldStartEPICs,
               myMachineArea ),
shouldStartEPICs(shouldStartEPICs),
myMachineArea(myMachineArea),
offlineIA(show_messages,show_debug_messages,&localInterface)
{
}
cameraIAController::~cameraIAController(){}    //dtor
std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> cameraIAController::getILockStates(const std::string &name)
{
    return localInterface.getILockStates(name);
}
std::map<VELA_ENUM::ILOCK_NUMBER,std::string> cameraIAController::getILockStatesStr(const std::string &name)
{
    return localInterface.getILockStatesStr(name);
}
double cameraIAController::get_CA_PEND_IO_TIMEOUT()
{
    return localInterface.get_CA_PEND_IO_TIMEOUT( );
}
void   cameraIAController::set_CA_PEND_IO_TIMEOUT( double val )
{
    localInterface.set_CA_PEND_IO_TIMEOUT( val );
}

///Functions Accessible to Python Controller///
//Generic Functions
bool cameraIAController::isON (const std::string &cam)
{
    return localInterface.isON(cam);
}
bool cameraIAController::isOFF(const std::string &cam)
{
    return localInterface.isOFF(cam);
}
bool cameraIAController::isAcquiring (const std::string &cam)
{
    return localInterface.isAcquiring(cam);
}
bool cameraIAController::isNotAcquiring (const std::string &cam)
{
    return localInterface.isNotAcquiring(cam);
}
std::string cameraIAController::selectedCamera()
{
    return localInterface.selectedCamera();
}
bool cameraIAController::setCamera(const std::string &cam)
{
    bool ans=false;
    ans=localInterface.setCamera(cam);
    //Set up mask ans center pisx and pixMM ratio
    setMaskX(localInterface.selectedCameraObj.IA.maskX);
    setMaskY(localInterface.selectedCameraObj.IA.maskY);
    setMaskXRad(localInterface.selectedCameraObj.IA.maskXRad);
    setMaskYRad(localInterface.selectedCameraObj.IA.maskYRad);
    setCenterXPixel(localInterface.selectedCameraObj.IA.xCenterPix);
    setCenterYPixel(localInterface.selectedCameraObj.IA.yCenterPix);
    return ans;
}
bool cameraIAController::startAcquiring()
{
    return localInterface.startAcquiring();
}
bool cameraIAController::stopAcquiring()
{
    return localInterface.stopAcquiring();
}
bool cameraIAController::startVCAcquiring()
{
    return localInterface.startVCAcquiring();
}
bool cameraIAController::stopVCAcquiring()
{
    return localInterface.stopVCAcquiring();
}

//IA Specific Functions
bool cameraIAController::setPixMM(const double pmm)
{
    return localInterface.setPixMM(pmm);
}
bool cameraIAController::setMaskX(const int x)
{
    return localInterface.setMaskX(x);
}
bool cameraIAController::setMaskY(const int y)
{
    return localInterface.setMaskY(y);
}
bool cameraIAController::setMaskXRad(const int xRad)
{
    return localInterface.setMaskXRad(xRad);
}
bool cameraIAController::setMaskYRad(const int yRad)
{
    return localInterface.setMaskYRad(yRad);
}
bool cameraIAController::setCenterXPixel(const int xC)
{
    return localInterface.setCenterXPixel(xC);
}
bool cameraIAController::setCenterYPixel(const int yC)
{
    return localInterface.setCenterYPixel(yC);
}
bool cameraIAController::setStepSize(const int step)
{
    return localInterface.setStepSize(step);
}
bool cameraIAController::useNPoint(const bool run)
{
    return localInterface.useNPoint(run);
}
bool cameraIAController::setBackground()
{
    return localInterface.setBackground();
}
bool cameraIAController::useBackground(const bool run)
{
    return localInterface.useBackground(run);
}
bool cameraIAController::startAnalysis()
{
    return localInterface.startAnalysis();
}
bool cameraIAController::stopAnalysis()
{
    return localInterface.stopAnalysis();
}
const cameraObject &cameraIAController::getCamIAObjConstRef(const std::string &camName)
{
    return localInterface.getCamIAObjConstRef(camName);
}
const cameraObject &cameraIAController::getSelectedIARef()
{
    return localInterface.getSelectedIARef();
}
const cameraObject &cameraIAController::getVCIARef()
{
    return localInterface.getVCIARef();
}
const offlineImageAnalyser& cameraIAController::getOfflineIARef()
{
    return offlineIA;
}
