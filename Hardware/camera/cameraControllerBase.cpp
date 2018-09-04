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
//  Last edit:   16-06-2018
//  FileName:    cameraControllerBase.cpp
//  Description:
//
//
//*/
#include "cameraControllerBase.h"

//cameraControllerBase::cameraControllerBase(
//    bool& show_messages,
//    bool& show_debug_messages,
//    const bool startVirtualMachine,
//    const bool shouldStartEPICs,
//    const std::string& name,
//    const std::string& claraCamConfig,
//    const std::string& velaCamConfig,
//    HWC_ENUM::CONTROLLER_TYPE type
//    ):
//cameraControllerBase(show_messages, show_debug_messages,
//                     startVirtualMachine, shouldStartEPICs,
//                     name,
//                     claraCamConfig, velaCamConfig,
//                     type,false)
//{}
//---------------------------------------------------------------------------------
cameraControllerBase::cameraControllerBase(bool& show_messages,
                             bool& show_debug_messages,
                             const bool startVirtualMachine,
                             const bool shouldStartEPICs,
                             const std::string& name,
                             const std::string& claraCamConfig,
                             const std::string& velaCamConfig,
                             const HWC_ENUM::CONTROLLER_TYPE type,
                             const HWC_ENUM::MACHINE_AREA area,
                             const bool vcONly):
controller(show_messages,show_debug_messages, type, name)
{
    camBase = new cameraBase(show_messages,
                             show_debug_messages,
                             startVirtualMachine,
                             shouldStartEPICs,
                             claraCamConfig,
                             velaCamConfig,
                             area
                             );
    camBase->initialise(vcONly);
    shoudlDeletecamBase = true;
}
//---------------------------------------------------------------------------------
cameraControllerBase::cameraControllerBase(bool& show_messages,
                                           bool& show_debug_messages,
                                           const std::string& name,
                                           HWC_ENUM::CONTROLLER_TYPE type,
                                           cameraBase* camBaseIN):
controller(show_messages,show_debug_messages, type, name)
{
    shoudlDeletecamBase = false;
    camBase = camBaseIN;
}
//---------------------------------------------------------------------------------
cameraControllerBase::~cameraControllerBase()
{
    if(shoudlDeletecamBase)
        delete camBase;
}
//
//---------------------------------------------------------------------------------
void  cameraControllerBase::set_CA_PEND_IO_TIMEOUT(double val)
{
    camBase->set_CA_PEND_IO_TIMEOUT(val);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::get_CA_PEND_IO_TIMEOUT()const
{
    return camBase->get_CA_PEND_IO_TIMEOUT();
}
//-----------------------------------------------------------------------------------------
//  __   __                         __   __
// /  ` /  \  |\/|  |\/|  /\  |\ | |  \ /__`
// \__, \__/  |  |  |  | /~~\ | \| |__/ .__/
//
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::takeFastImage(const std::string& cam)
{
    return camBase->takeFastImage(cam);
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::takeFastImage_VC()
{
    return camBase->takeFastImage_VC();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::takeFastImage()
{
    return camBase->takeFastImage();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::startAcquiring_VC()
{
    return camBase->startAcquiring_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAcquiring(const std::string& cam)
{
    return camBase->startAcquiring(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAcquiring()
{
    return camBase->startAcquiring();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAcquiring_VC()
{
    return camBase->stopAcquiring_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAcquiring(const std::string& cam)
{
    return camBase->stopAcquiring(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAcquiring()
{
    return camBase->stopAcquiring();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAllAcquiring()
{
    return camBase->stopAllAcquiring();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAllAcquiringExceptVC()
{
    return camBase->stopAllAcquiringExceptVC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAnalysing_VC()
{
    return camBase->startAnalysing_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAnalysing(const std::string& cam)
{
    return camBase->startAnalysing(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAnalysing()
{
    return camBase->startAnalysing();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAnalysing_VC()
{
    return camBase->stopAnalysing_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAnalysing(const std::string& cam)
{
    return camBase->stopAnalysing(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAnalysing()
{
    return camBase->stopAnalysing();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAllAnalysing()
{
    return camBase->stopAllAnalysing();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAllAnalysingExceptVC()
{
    return camBase->stopAllAnalysingExceptVC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAcquireAndAnalysis_VC()
{
    return camBase->stopAcquireAndAnalysis_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAcquireAndAnalysis(const std::string& cam)
{
    return camBase->stopAcquireAndAnalysis();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAcquireAndAnalysis()
{
    return camBase->stopAcquireAndAnalysis();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAcquireAndAnalysis_VC()
{
    return camBase->startAcquireAndAnalysis_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAcquireAndAnalysis(const std::string& cam)
{
    return camBase->startAcquireAndAnalysis(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAcquireAndAnalysis()
{
    return camBase->startAcquireAndAnalysis();
}
//-----------------------------------------------------------------------------------------
//  __  ___      ___  ___     __                 ___    ___    __       ___    __
// /__`  |   /\   |  |__     /  \ |  |  /\  |\ |  |  | |__  | /  `  /\   |  | /  \ |\ |
// .__/  |  /~~\  |  |___    \__X \__/ /~~\ | \|  |  | |    | \__, /~~\  |  | \__/ | \|
//
//-----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBeam_x_Hi_VC()
{
    return camBase->isBeam_x_Hi_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBeam_x_Lo_VC()
{
    return camBase->isBeam_x_Lo_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBeam_y_Hi_VC()
{
    return camBase->isBeam_y_Hi_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBeam_y_Lo_VC()
{
    return camBase->isBeam_y_Lo_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysisUpdating_VC()const
{
    return camBase->isAnalysisUpdating_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysisUpdating()const
{
    return camBase->isAnalysisUpdating();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysisUpdating(const std::string& name)const
{
    return camBase->isAnalysisUpdating();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysisNotUpdating_VC()const
{
    return camBase->isAnalysisNotUpdating_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysisNotUpdating()const
{
    return camBase->isAnalysisNotUpdating();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysisNotUpdating(const std::string& name)const
{
    return camBase->isAnalysisNotUpdating();
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::hasBeam_VC()const
{
    return camBase->hasBeam_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::hasBeam(const std::string& cam)const
{
    return camBase->hasBeam();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::hasBeam()const
{
    return camBase->hasBeam();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::hasNoBeam_VC()const
{
    return camBase->hasNoBeam_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::hasNoBeam(const std::string& cam)const
{
    return camBase->hasNoBeam(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::hasNoBeam()const
{
    return camBase->hasNoBeam();
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::isVelaCam_VC()const
{
    return camBase->isVelaCam_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isVelaCam(const std::string& cam)const
{
    return camBase->isVelaCam(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isVelaCam()const
{
    return camBase->isVelaCam();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isClaraCam_VC()const
{
    return camBase->isClaraCam_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isClaraCam(const std::string& cam)const
{
    return camBase->isClaraCam(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isClaraCam()const
{
    return camBase->isClaraCam();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isBusy_VC()const
{
    return camBase->isBusy_VC();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isBusy(const std::string& cam)const
{
    return camBase->isBusy(cam);
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isBusy()const
{
    return camBase->isBusy();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isNotBusy_VC()const
{
    return camBase->isNotBusy_VC();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isNotBusy(const std::string& cam)const
{
    return camBase->isNotBusy(cam);
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isNotBusy()const
{
    return camBase->isNotBusy();
}
//-----------------------------------------------------------------------------------------
bool cameraControllerBase::isON_VC()const
{
    return camBase->isON_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isON(const std::string& cam)const
{
    return camBase->isON(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isON()const
{
    return camBase->isON();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isOFF_VC()const
{
    return camBase->isOFF_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isOFF(const std::string& cam)const
{
    return camBase->isOFF(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isOFF()const
{
    return camBase->isOFF();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAcquiring_VC()const
{
    return camBase->isAcquiring_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAcquiring(const std::string& cam)const
{
    return camBase->isAcquiring(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAcquiring()const
{
    return camBase->isAcquiring();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotAcquiring_VC()const
{
    return camBase->isNotAcquiring_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotAcquiring(const std::string& cam)const
{
    return camBase->isNotAcquiring(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotAcquiring()const
{
    return camBase->isNotAcquiring();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysing_VC()const
{
    return camBase->isAnalysing_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysing(const std::string& cam)const
{
    return camBase->isAnalysing(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isAnalysing()const
{
    return camBase->isAnalysing();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotAnalysing_VC()const
{
    return camBase->isNotAnalysing_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotAnalysing(const std::string& cam)const
{
    return camBase->isNotAnalysing(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotAnalysing()const
{
    return camBase->isNotAnalysing();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotCollecting_VC()const
{
    return camBase->isNotCollecting_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotCollecting(const std::string& cam)const
{
    return camBase->isNotCollecting(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotCollecting()const
{
    return camBase->isNotCollecting();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSelectedCamera(const std::string& name)const
{
    return camBase->isSelectedCamera(name);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotSelectedCamera(const std::string& name)const
{
    return camBase->isNotSelectedCamera(name);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isVC(const std::string& name)const
{
    return camBase->isVC(name);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotVC(const std::string& name)const
{
    return camBase->isNotVC(name);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotCollectingOrSaving_VC()const
{
    return camBase->isNotCollectingOrSaving_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotCollectingOrSaving(const std::string& cam)const
{
    return camBase->isNotCollectingOrSaving(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotCollectingOrSaving()const
{
    return camBase->isNotCollectingOrSaving();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isUsingBackground_VC()const
{
    return camBase->isUsingBackground_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isUsingBackground(const std::string& cam)const
{
    return camBase->isUsingBackground(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isUsingBackground()const
{
    return camBase->isUsingBackground();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotUsingBackground_VC()const
{
    return camBase->isNotUsingBackground_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotUsingBackground(const std::string& cam)const
{
    return camBase->isNotUsingBackground(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotUsingBackground()const
{
    return camBase->isNotUsingBackground();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isUsingNPoint_VC()const
{
    return camBase->isUsingNPoint_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isUsingNPoint(const std::string& cam)const
{
    return camBase->isUsingNPoint(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isUsingNPoint()const
{
    return camBase->isUsingNPoint();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotUsingNPoint_VC()const
{
    return camBase->isNotUsingNPoint_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotUsingNPoint(const std::string& cam)const
{
    return camBase->isNotUsingNPoint(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isNotUsingNPoint()const
{
    return camBase->isNotUsingNPoint();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollecting_VC()const
{
    return camBase->isCollecting_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollecting(const std::string& cam)const
{
    return camBase->isCollecting(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollecting()const
{
    return camBase->isCollecting();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSaving_VC()const
{
    return camBase->isSaving_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSaving(const std::string& cam)const
{
    return camBase->isSaving(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSaving()const
{
    return camBase->isSaving();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollectingJPG_VC()const
{
    return camBase->isCollectingJPG_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollectingJPG(const std::string& cam)const
{
    return camBase->isCollectingJPG(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollectingJPG()const
{
    return camBase->isCollectingJPG();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSavingJPG_VC()const
{
    return camBase->isSavingJPG_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSavingJPG(const std::string& cam)const
{
    return camBase->isSavingJPG(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isSavingJPG()const
{
    return camBase->isSavingJPG();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollectingOrSaving_VC()const
{
    return camBase->isCollectingOrSaving_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollectingOrSaving(const std::string& cam)const
{
    return camBase->isCollectingOrSaving(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCollectingOrSaving()const
{
    return camBase->isCollectingOrSaving();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::latestCollectAndSaveSuccess_VC()const
{
    return camBase->latestCollectAndSaveSuccess_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::latestCollectAndSaveSuccess()const
{
    return camBase->latestCollectAndSaveSuccess();
}
//---------------------------------------------------------------------------------


































//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskX_VC()const
{
    return camBase->getMaskX_VC();
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskX(const std::string& cam )const
{
    return camBase->getMaskX(cam);
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskX()const
{
    return camBase->getMaskX();
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskY_VC()const
{
    return camBase->getMaskY_VC();
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskY(const std::string& cam)const
{
    return camBase->getMaskY(cam);
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskY()const
{
    return camBase->getMaskY();
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskXrad_VC()const
{
    return camBase->getMaskXrad_VC();
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskXrad(const std::string& cam )const
{
    return camBase->getMaskXrad(cam);
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskXrad()const
{
    return camBase->getMaskXrad();
}
//---------------------------------------------------------------------------------

unsigned short cameraControllerBase::getMaskYrad_VC()const
{
    return camBase->getMaskYrad_VC();
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskYrad(const std::string& cam )const
{
    return camBase->getMaskYrad(cam);
}
//---------------------------------------------------------------------------------
unsigned short cameraControllerBase::getMaskYrad()const
{
    return camBase->getMaskYrad();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::claraLEDOn()
{
    return camBase->claraLEDOn();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::claraLEDOff()
{
    return camBase->claraLEDOff();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCLaraLEDOn()
{
    return camBase->isCLaraLEDOn();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isCLaraLEDOff()
{
    return camBase->isCLaraLEDOff();
}
//---------------------------------------------------------------------------------













bool cameraControllerBase::setMaskX_VC(unsigned short x)
{
    return camBase->setMaskX_VC(x);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskX(unsigned short x,const std::string& cam)
{
    return camBase->setMaskX(x,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskX(unsigned short x)
{
    return camBase->setMaskX(x);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setMaskY_VC(unsigned short x)
{
    return camBase->setMaskY_VC(x);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskY(unsigned short x,const std::string& cam)
{
    message("cameraControllerBase::setMaskY called ", cam);
    return camBase->setMaskY(x,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskY(unsigned short x)
{
    return camBase->setMaskY(x);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setMaskXrad_VC(unsigned short x)
{
    return camBase->setMaskXrad_VC(x);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskXrad(unsigned short x,const std::string& cam)
{
    return camBase->setMaskXrad(x,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskXrad(unsigned short x)
{
    return camBase->setMaskXrad(x);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setMaskYrad_VC(unsigned short x)
{
    return camBase->setMaskYrad_VC(x);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskYrad(unsigned short x,const std::string& cam)
{
    return camBase->setMaskYrad(x,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskYrad(unsigned short x)
{
    return camBase->setMaskYrad(x);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setMask_VC(unsigned short x,unsigned short y,unsigned short xr,unsigned short yr)
{
    return camBase->setMask_VC(x,y,xr,yr);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMask(unsigned short x,unsigned short y,unsigned short xr,unsigned short yr,const std::string& cam)
{
    return camBase->setMask(x,y,xr,yr,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMask(unsigned short x,unsigned short y,unsigned short xr,unsigned short yr)
{
    return camBase->setMask(x,y,xr,yr);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setMask_VC(const std::vector<unsigned short>& v)
{
    return camBase->setMask_VC(v);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMask(const std::vector<unsigned short>& v,const std::string& cam)
{
    return camBase->setMask(v,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMask(const std::vector<unsigned short>& v)
{
    return camBase->setMask(v);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::collectAndSave(const int numbOfShots)
{
    return camBase->collectAndSave(numbOfShots);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::collectAndSave(const std::string& name,const int numbOfShots)
{
    return camBase->collectAndSave(name,numbOfShots);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::collectAndSave_VC(const int numbOfShots)
{
    return camBase->collectAndSave_VC(numbOfShots);
}




#ifdef BUILD_DLL
bool cameraControllerBase::setMask_VC_Py(const boost::python::list& v)
{
    message("setMask_VC_Py called");
    return setMask_VC(to_std_vector<unsigned short>(v));
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMask_Py1(const boost::python::list v,const std::string& cam)
{
    return setMask(to_std_vector<unsigned short>(v),cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMask_Py2(const boost::python::list v)
{
    return setMask(to_std_vector<unsigned short>(v));
}
#endif
//---------------------------------------------------------------------------------
bool cameraControllerBase::setGain_VC(const long value)
{
    return setGain_VC(value);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setGain(const long value, const std::string& cam)
{
    return setGain(value, cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setGain(const long value)
{
    return setGain(value);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setBlacklevel_VC(const long value)
{
    return setBlacklevel(value);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setBlacklevel(const long value, const std::string& cam)
{
    return setBlacklevel(value, cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setBlacklevel(const long value)
{
    return setBlacklevel(value);
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
bool cameraControllerBase::setBackground_VC()
{
    return camBase->setBackground_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setBackground(const std::string& cam)
{
    return camBase->setBackground(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setBackground()
{
    return camBase->setBackground();
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setStepSize_VC(unsigned short step)
{
    return camBase->setStepSize_VC(step);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setStepSize(unsigned short step,const std::string& cam)
{
    return camBase->setStepSize(step,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setStepSize(unsigned short step)
{
    return camBase->setStepSize(step);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setCenterXPixel_VC(unsigned short xC)
{
    return camBase->setCenterXPixel_VC(xC);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setCenterXPixel(unsigned short xC,const std::string& cam )
{
    return camBase->setCenterXPixel(xC, cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setCenterXPixel(unsigned short xC)
{
    return camBase->setCenterXPixel(xC);
}
//---------------------------------------------------------------------------------



bool cameraControllerBase::setCenterYPixel_VC(unsigned short yC)
{
    return camBase->setCenterYPixel_VC(yC);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setCenterYPixel(unsigned short yC,const std::string& cam)
{
    return camBase->setCenterYPixel(yC,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setCenterYPixel(unsigned short yC)
{
    return camBase->setCenterYPixel(yC);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::setPixMM_VC(const double pmm)
{
    return camBase->setPixMM_VC(pmm);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setPixMM(const double pmm,const std::string& cam)
{
    return camBase->setPixMM(pmm,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setPixMM(const double pmm)
{
    return camBase->setPixMM(pmm);
}
//---------------------------------------------------------------------------------









bool cameraControllerBase::startAnalysis_VC()
{
    return camBase->startAnalysis_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAnalysis(const std::string& cam)
{
    return camBase->startAnalysis(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::startAnalysis()
{
    return camBase->startAnalysis();
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::stopAnalysis_VC()
{
    return camBase->stopAnalysis_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAnalysis(const std::string& cam)
{
    return camBase->stopAnalysis(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::stopAnalysis()
{
    return camBase->stopAnalysis();
}




bool cameraControllerBase::useBackground_VC(bool v)
{
    return camBase->useBackground_VC(v);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::useBackground(bool v,const std::string& cam)
{
    return camBase->useBackground(v,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::useBackground(bool v)
{
    return camBase->useBackground(v);
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::useNPoint_VC(bool v)
{
    return camBase->useNPoint_VC(v);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::useNPoint(bool v,const std::string& cam)
{
    return camBase->useNPoint(v,cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::useNPoint(bool v)
{
    return camBase->useNPoint(v);
}
//---------------------------------------------------------------------------------













std::deque<double> cameraControllerBase::getXBuffer_VC()const
{
    return camBase->getXBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getXBuffer(const std::string& cam)const
{
    return camBase->getXBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getXBuffer()const
{
    return camBase->getXBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getYBuffer_VC()const
{
    return camBase->getYBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getYBuffer(const std::string& cam)const
{
    return camBase->getYBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getYBuffer()const
{
    return camBase->getYBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getSigXBuffer_VC()const
{
    return camBase->getSigXBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXBuffer(const std::string& cam)const
{
    return camBase->getSigXBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXBuffer()const
{
    return camBase->getSigXBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getSigYBuffer_VC()const
{
    return camBase->getSigYBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigYBuffer(const std::string& cam)const
{
    return camBase->getSigYBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigYBuffer()const
{
    return camBase->getSigYBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getSigXYBuffer_VC()const
{
    return camBase->getSigXYBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXYBuffer(const std::string& cam)const
{
    return camBase->getSigXYBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXYBuffer()const
{
    return camBase->getSigXYBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getXPixBuffer_VC()const
{
    return camBase->getXPixBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getXPixBuffer(const std::string& cam)const
{
    return camBase->getXPixBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getXPixBuffer()const
{
    return camBase->getXPixBuffer();
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getYPixBuffer_VC()const
{
    return camBase->getYPixBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getYPixBuffer(const std::string& cam)const
{
    return camBase->getYPixBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getYPixBuffer()const
{
    return camBase->getYPixBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getSigXPixBuffer_VC()const
{
    return camBase->getSigXPixBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXPixBuffer(const std::string& cam)const
{
    return camBase->getSigXPixBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXPixBuffer()const
{
    return camBase->getSigXPixBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getSigYPixBuffer_VC()const
{
    return camBase->getSigYPixBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigYPixBuffer(const std::string& cam)const
{
    return camBase->getSigYPixBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigYPixBuffer()const
{
    return camBase->getSigYPixBuffer();
}
//---------------------------------------------------------------------------------

std::deque<double> cameraControllerBase::getSigXYPixBuffer_VC()const
{
    return camBase->getSigXYPixBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXYPixBuffer(const std::string& cam)const
{
    return camBase->getSigXYPixBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSigXYPixBuffer()const
{
    return camBase->getSigXYPixBuffer();
}
//---------------------------------------------------------------------------------

std::vector<double> cameraControllerBase::getPixelValues_VC()const
{
    return camBase->getPixelValues_VC();
}
//---------------------------------------------------------------------------------
std::vector<double> cameraControllerBase::getPixelValues(const std::string& cam)const
{
    return camBase->getPixelValues(cam);
}
//---------------------------------------------------------------------------------
std::vector<double> cameraControllerBase::getPixelValues()const
{
    return camBase->getPixelValues();
}
//---------------------------------------------------------------------------------

std::deque<std::vector<double>> cameraControllerBase::getPixelValuesBuffer_VC()const
{
    return camBase->getPixelValuesBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<std::vector<double>> cameraControllerBase::getPixelValuesBuffer(const std::string& cam)const
{
    return camBase->getPixelValuesBuffer(cam);
}
//---------------------------------------------------------------------------------
std::deque<std::vector<double>> cameraControllerBase::getPixelValuesBuffer()const
{
    return camBase->getPixelValuesBuffer();
}
//---------------------------------------------------------------------------------
std::vector<int> cameraControllerBase::getFastImage_VC()const
{
    return camBase->getFastImage_VC();
}
//---------------------------------------------------------------------------------
std::vector<int> cameraControllerBase::getFastImage(const std::string& cam)const
{
    return camBase->getFastImage(cam);
}
//---------------------------------------------------------------------------------
std::vector<int> cameraControllerBase::getFastImage()const
{
    return camBase->getFastImage();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSumIntensityBuffer_VC()const
{
    return camBase->getSumIntensityBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSumIntensityBuffer(const std::string& cam)const
{
    return camBase->getSumIntensityBuffer();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getSumIntensityBuffer()const
{
    return camBase->getSumIntensityBuffer();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getAvgIntensityBuffer_VC()const
{
    return camBase->getAvgIntensityBuffer_VC();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getAvgIntensityBuffer(const std::string& cam)const
{
    return camBase->getAvgIntensityBuffer();
}
//---------------------------------------------------------------------------------
std::deque<double> cameraControllerBase::getAvgIntensityBuffer()const
{
    return camBase->getAvgIntensityBuffer();
}
//---------------------------------------------------------------------------------

std::vector<std::string> cameraControllerBase::getCameraNames()const
{
    return camBase->getCameraNames();
}
//---------------------------------------------------------------------------------
std::vector<std::string> cameraControllerBase::getCameraScreenNames()const
{
    return camBase->getCameraScreenNames();
}
//---------------------------------------------------------------------------------
#ifdef BUILD_DLL
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getCameraNames_Py()const
{
    return toPythonList<std::string>(getCameraNames());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getCameraScreenNames_Py()const
{
    return toPythonList<std::string>(getCameraScreenNames());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getXBuffer_VC_Py()const
{
    return toPythonList<double>(getXBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getXBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getXBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getXBuffer_Py2()const
{
    return toPythonList<double>(getXBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getYBuffer_VC_Py()const
{
    return toPythonList<double>(getYBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getYBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getYBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getYBuffer_Py2()const
{
    return toPythonList<double>(getYBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXBuffer_VC_Py()const
{
    return toPythonList<double>(getSigXBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSigXBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXBuffer_Py2()const
{
    return toPythonList<double>(getSigXBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigYBuffer_VC_Py()const
{
    return toPythonList<double>(getSigYBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigYBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSigYBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigYBuffer_Py2()const
{
    return toPythonList<double>(getSigYBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXYBuffer_VC_Py()const
{
    return toPythonList<double>(getSigXYBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXYBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSigXYBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXYBuffer_Py2()const
{
    return toPythonList<double>(getSigXYBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getXPixBuffer_VC_Py()const
{
    return toPythonList<double>(getXPixBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getXPixBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getXPixBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getXPixBuffer_Py2()const
{
    return toPythonList<double>(getXPixBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getYPixBuffer_VC_Py()const
{
    return toPythonList<double>(getYPixBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getYPixBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getYPixBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getYPixBuffer_Py2()const
{
    return toPythonList<double>(getYPixBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXPixBuffer_VC_Py()const
{
    return toPythonList<double>(getSigXPixBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXPixBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSigXPixBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXPixBuffer_Py2()const
{
    return toPythonList<double>(getSigXPixBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigYPixBuffer_VC_Py()const
{
    return toPythonList<double>(getSigYPixBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigYPixBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSigYPixBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigYPixBuffer_Py2()const
{
    return toPythonList<double>(getSigYPixBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXYPixBuffer_VC_Py()const
{
    return toPythonList<double>(getSigXYPixBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXYPixBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSigXYPixBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSigXYPixBuffer_Py2()const
{
    return toPythonList<double>(getSigXYPixBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getPixelValues_VC_Py()const
{
    return toPythonList<double>(getPixelValues_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getPixelValues_Py1(const std::string& cam)const
{
    return toPythonList<double>(getPixelValues(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getPixelValues_Py2()const
{
    return toPythonList<double>(getPixelValues());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getPixelValuesBuffer_VC_Py()const
{
    return toPythonList<double>(getPixelValuesBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getPixelValuesBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getPixelValuesBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getPixelValuesBuffer_Py2()const
{
    return toPythonList<double>(getPixelValuesBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSumIntensityBuffer_VC_Py()const
{
    return toPythonList<double>(getSumIntensityBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSumIntensityBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getSumIntensityBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getSumIntensityBuffer_Py2()const
{
    return toPythonList<double>(getSumIntensityBuffer());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getAvgIntensityBuffer_VC_Py()const
{
    return toPythonList<double>(getAvgIntensityBuffer_VC());
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getAvgIntensityBuffer_Py1(const std::string& cam)const
{
    return toPythonList<double>(getAvgIntensityBuffer(cam));
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getAvgIntensityBuffer_Py2()const
{
    return toPythonList<double>(getAvgIntensityBuffer());
}



//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getFastImage_VC_Py()const
{
    return camBase->getFastImage_VC_Py();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getFastImage_Py1(const std::string& cam)const
{
    return camBase->getFastImage_Py(cam);
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getFastImage_Py2()const
{
    return camBase->getFastImage_Py();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getFastImage2D_VC_Py()const
{
    return camBase->getFastImage2D_VC_Py();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getFastImage2D_Py1(const std::string& cam)const
{
    return camBase->getFastImage2D_Py(cam);
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::getFastImage2D_Py2()const
{
    return camBase->getFastImage2D_Py();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::takeAndGetFastImage2D_VC()
{
    return camBase->takeAndGetFastImage2D_VC();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::takeAndGetFastImage2D(const std::string& cam)
{
    return camBase->takeAndGetFastImage2D();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::takeAndGetFastImage2D()
{
    return camBase->takeAndGetFastImage2D();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::takeAndGetFastImage_VC()
{
    return camBase->takeAndGetFastImage_VC();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::takeAndGetFastImage(const std::string& cam)
{
    return camBase->takeAndGetFastImage();
}
//---------------------------------------------------------------------------------
boost::python::list cameraControllerBase::takeAndGetFastImage()
{
    return camBase->takeAndGetFastImage();
}
//---------------------------------------------------------------------------------


#endif



void cameraControllerBase::clearRunningValues_VC()
{
    return camBase->clearRunningValues_VC();
}
//---------------------------------------------------------------------------------
void cameraControllerBase::clearRunningValues(const std::string& cam)
{
    return camBase->clearRunningValues(cam);
}
//---------------------------------------------------------------------------------
void cameraControllerBase::clearRunningValues()
{
    return camBase->clearRunningValues();
}
//---------------------------------------------------------------------------------









bool cameraControllerBase::isBufferFull_VC()const
{
    return camBase->isBufferFull_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBufferFull(const std::string& cam)const
{
    return camBase->isBufferFull(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBufferFull()const
{
    return camBase->isBufferFull();
}
//---------------------------------------------------------------------------------

bool cameraControllerBase::isBufferNotFull_VC()const
{
    return camBase->isBufferNotFull_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBufferNotFull(const std::string& cam)const
{
    return camBase->isBufferNotFull(cam);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isBufferNotFull()const
{
    return camBase->isBufferNotFull();
}
//---------------------------------------------------------------------------------

double cameraControllerBase::getX_VC()const
{
    return camBase->getX_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getX(const std::string& cam)const
{
    return camBase->getX(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getX()const
{
    return camBase->getX();
}
//---------------------------------------------------------------------------------


double cameraControllerBase::getY_VC()const
{
    return camBase->getY_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getY(const std::string& cam)const
{
    return camBase->getY(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getY()const
{
    return camBase->getY();
}
//---------------------------------------------------------------------------------

double cameraControllerBase::getSigX_VC()const
{
    return camBase->getSigX_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigX(const std::string& cam)const
{
    return camBase->getSigX(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigX()const
{
    return camBase->getSigX();
}
//---------------------------------------------------------------------------------

double cameraControllerBase::getSigY_VC()const
{
    return camBase->getSigY_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigY(const std::string& cam)const
{
    return camBase->getSigY(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigY()const
{
    return camBase->getSigY();
}
//---------------------------------------------------------------------------------

double cameraControllerBase::getSigXY_VC()const
{
    return camBase->getSigXY_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXY(const std::string& cam)const
{
    return camBase->getSigXY(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXY()const
{
    return camBase->getSigXY();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getXPix_VC()const
{
    return camBase->getXPix_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getXPix(const std::string& cam)const
{
    return camBase->getXPix(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getXPix()const
{
    return camBase->getXPix();
}
//---------------------------------------------------------------------------------

double cameraControllerBase::getYPix_VC()const
{
    return camBase->getYPix_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getYPix(const std::string& cam)const
{
    return camBase->getYPix(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getYPix()const
{
    return camBase->getYPix();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXPix_VC()const
{
    return camBase->getSigXPix_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXPix(const std::string& cam)const
{
    return camBase->getSigXPix(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXPix()const
{
    return camBase->getSigXPix();
}
//---------------------------------------------------------------------------------

double cameraControllerBase::getSigYPix_VC()const
{
    return camBase->getSigYPix_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigYPix(const std::string& cam)const
{
    return camBase->getSigYPix(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigYPix()const
{
    return camBase->getSigYPix();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXYPix_VC()const
{
    return camBase->getSigXYPix_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXYPix(const std::string& cam)const
{
    return camBase->getSigXYPix(cam);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getSigXYPix()const
{
    return camBase->getSigXYPix();
}
//---------------------------------------------------------------------------------
size_t cameraControllerBase::getBufferCount_VC()const
{
    return camBase->getBufferCount_VC();
}
//---------------------------------------------------------------------------------
size_t cameraControllerBase::getBufferCount(const std::string& cam)const
{
    return camBase->getBufferCount(cam);
}
//---------------------------------------------------------------------------------
size_t cameraControllerBase::getBufferCount()const
{
    return camBase->getBufferCount();
}
//---------------------------------------------------------------------------------

size_t cameraControllerBase::getBufferMaxCount_VC()const
{
    return camBase->getBufferMaxCount_VC();
}
//---------------------------------------------------------------------------------
size_t cameraControllerBase::getBufferMaxCount(const std::string& cam)const
{
    return camBase->getBufferMaxCount(cam);
}
//---------------------------------------------------------------------------------
size_t cameraControllerBase::getBufferMaxCount()const
{
    return camBase->getBufferMaxCount();
}
//---------------------------------------------------------------------------------











void cameraControllerBase::setBufferMaxCount_VC(const size_t s)
{
    return camBase->setBufferMaxCount_VC(s);
}
//---------------------------------------------------------------------------------
void cameraControllerBase::setBufferMaxCount(const size_t s,const std::string& cam)
{
    return camBase->setBufferMaxCount(s,cam);
}
//---------------------------------------------------------------------------------
void cameraControllerBase::setBufferMaxCount(const size_t s)
{
    return camBase->setBufferMaxCount(s);
}


//---------------------------------------------------------------------------------
void cameraControllerBase::clearBuffer_VC()
{
    return camBase->clearBuffer_VC();
}
//---------------------------------------------------------------------------------
void cameraControllerBase::clearBuffer(const std::string& cam)
{
    return camBase->clearBuffer();
}
//---------------------------------------------------------------------------------
void cameraControllerBase::clearBuffer()
{
    return camBase->clearBuffer();
}
//---------------------------------------------------------------------------------
///
///  __   __        ___  __  ___     __   __        __  ___     __   ___  ___  __
/// /  \ |__)    | |__  /  `  |     /  ` /  \ |\ | /__`  |     |__) |__  |__  /__`
/// \__/ |__) \__/ |___ \__,  |     \__, \__/ | \| .__/  |     |  \ |___ |    .__/
///
///
using namespace cameraStructs;
const analysis_mask& cameraControllerBase::getMaskObj(const std::string& name)const
{
    return camBase->getMaskObj(name);
}
//---------------------------------------------------------------------------------
const analysis_mask& cameraControllerBase::getMaskObj_VC()const
{
    return camBase->getMaskObj_VC();
}
//---------------------------------------------------------------------------------
const analysis_mask& cameraControllerBase::getMaskObj()const
{
    return camBase->getMaskObj();
}
//---------------------------------------------------------------------------------
const analysis_data& cameraControllerBase::getAnalysisObj(const std::string& name)const
{
    return camBase->getAnalysisObj(name);
}
//---------------------------------------------------------------------------------
const analysis_data& cameraControllerBase::getAnalysisObj_VC()const
{
    return camBase->getAnalysisObj_VC();
}
//---------------------------------------------------------------------------------
const analysis_data& cameraControllerBase::getAnalysisObj()const
{
    return camBase->getAnalysisObj();
}
//---------------------------------------------------------------------------------
const fast_image& cameraControllerBase::getImageObj(const std::string& name)const
{
    return camBase->getImageObj(name);
}
//---------------------------------------------------------------------------------
const fast_image& cameraControllerBase::getImageObj_VC()const
{
    return camBase->getImageObj_VC();
}
//---------------------------------------------------------------------------------
const fast_image& cameraControllerBase::getImageObj()const
{
    return camBase->getImageObj();
}
//---------------------------------------------------------------------------------
const camera_image_data& cameraControllerBase::getImageDataObj(const std::string& name)const
{
    return camBase->getImageDataObj(name);
}
//---------------------------------------------------------------------------------
const camera_image_data& cameraControllerBase::getImageDataObj_VC()const
{
    return camBase->getImageDataObj_VC();
}
//---------------------------------------------------------------------------------
const camera_image_data& cameraControllerBase::getImageDataObj()const
{
    return camBase->getImageDataObj();
}
//---------------------------------------------------------------------------------
const clara_DAQ& cameraControllerBase::getClaraDAQObj(const std::string& name)const
{
    return camBase->getClaraDAQObj(name);
}
//---------------------------------------------------------------------------------
const clara_DAQ& cameraControllerBase::getClaraDAQObj_VC()const
{
    return camBase->getClaraDAQObj_VC();
}
//---------------------------------------------------------------------------------
const clara_DAQ& cameraControllerBase::getClaraDAQObj()const
{
    return camBase->getClaraDAQObj();
}
//---------------------------------------------------------------------------------
const cameraObject& cameraControllerBase::getCameraObj(const std::string& name)const
{
    return camBase->getCameraObj(name);
}
//---------------------------------------------------------------------------------
const cameraObject& cameraControllerBase::getCameraObj_VC()const
{
    return camBase->getCameraObj_VC();
}
//---------------------------------------------------------------------------------
const cameraObject& cameraControllerBase::getCameraObj()const
{
    return camBase->getCameraObj();
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraControllerBase::getStateObj(const std::string& name)const
{
    return camBase->getStateObj(name);
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraControllerBase::getStateObj_VC()const
{
    return camBase->getStateObj_VC();
}
//---------------------------------------------------------------------------------
const cameraStructs::camera_state& cameraControllerBase::getStateObj()const
{
    return camBase->getStateObj();
}
//---------------------------------------------------------------------------------
runningStat& cameraControllerBase::get_x_private_rs_ref_VC()
{
    return camBase->get_x_private_rs_ref_VC();
}
//---------------------------------------------------------------------------------
runningStat& cameraControllerBase::get_y_private_rs_ref_VC()
{
    return camBase->get_y_private_rs_ref_VC();
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getStepSize_VC()const
{
    return camBase->getStepSize_VC();
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getStepSize(const std::string& cam)const
{
    return camBase->getStepSize(cam);
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getStepSize()const
{
    return camBase->getStepSize();
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getGain_VC()const
{
    return camBase->getGain_VC();
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getGain(const std::string& cam)const
{
    return camBase->getGain(cam);
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getGain()const
{
    return camBase->getGain();
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getBlacklevel_VC()const
{
    return camBase->getBlacklevel_VC();
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getBlacklevel(const std::string& cam)const
{
    return camBase->getBlacklevel(cam);
}
//---------------------------------------------------------------------------------
int cameraControllerBase::getBlacklevel()const
{
    return camBase->getBlacklevel();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskFeedBackOn_VC()
{
    return camBase->setMaskFeedBackOn_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskFeedBackOn()
{
    return camBase->setMaskFeedBackOn();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskFeedBackOn(const std::string& name)
{
    return camBase->setMaskFeedBackOn(name);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskFeedBackOff_VC()
{
    return camBase->setMaskFeedBackOff_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskFeedBackOff()
{
    return camBase->setMaskFeedBackOff();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::setMaskFeedBackOff(const std::string& name)
{
    return camBase->setMaskFeedBackOff(name);
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isMaskFeedbackOn_VC()const
{
    return camBase->isMaskFeedbackOn_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isMaskFeedbackOn()const
{
    return camBase->isMaskFeedbackOn();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isMaskFeedbackOn(const std::string& name)const
{
    return camBase->isMaskFeedbackOn();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isMaskFeedbackOff_VC()const
{
    return camBase->isMaskFeedbackOff_VC();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isMaskFeedbackOff()const
{
    return camBase->isMaskFeedbackOff();
}
//---------------------------------------------------------------------------------
bool cameraControllerBase::isMaskFeedbackOff(const std::string& name)const
{
    return camBase->isMaskFeedbackOff(name);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getPix2mm_VC() const
{
    return camBase->getPix2mm_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getPix2mm() const
{
    return camBase->getPix2mm();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getPix2mm(const std::string& name) const
{
    return camBase->getPix2mm(name);
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getPix2mmDef_VC() const
{
    return camBase->getPix2mmDef_VC();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getPix2mmDef() const
{
    return camBase->getPix2mmDef();
}
//---------------------------------------------------------------------------------
double cameraControllerBase::getPix2mmDef(const std::string& name) const
{
    return camBase->getPix2mmDef(name);
}
//---------------------------------------------------------------------------------
/*
    NumPy stuff, not yet working.
    have to link to boost.python.numpy lib
    somehow
*/
//
//
//boost::python::numpy::ndarray  cameraControllerBase::getXBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getXBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getXBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getXBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getXBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getXBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getYBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getYBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getYBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getYBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getYBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getYBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSigXBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSigXBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSigXBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigYBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSigYBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigYBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSigYBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigYBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSigYBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXYBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSigXYBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXYBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSigXYBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXYBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSigXYBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getXPixBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getXPixBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getXPixBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getXPixBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getXPixBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getXPixBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getYPixBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getYPixBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getYPixBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getYPixBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getYPixBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getYPixBuffer());
//}
////---------------------------------------------------------------------------------
//
//boost::python::numpy::ndarray  cameraControllerBase::getSigXPixBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSigXPixBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXPixBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSigXPixBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXPixBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSigXPixBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigYPixBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSigYPixBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigYPixBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSigYPixBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigYPixBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSigYPixBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXYPixBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSigXYPixBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXYPixBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSigXYPixBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSigXYPixBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSigXYPixBuffer());
//}
////---------------------------------------------------------------------------------
//
//boost::python::numpy::ndarray  cameraControllerBase::getPixelValues_VC_NumPy()
//{
//    return toNumPyArray<double>(getPixelValues_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getPixelValues_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getPixelValues(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getPixelValues_NumPy2()
//{
//    return toNumPyArray<double>(getPixelValues());
//}
//
//
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//////---------------------------------------------------------------------------------
////boost::python::numpy::ndarray  cameraControllerBase::getPixelValuesBuffer_VC_NumPy()
////{
////    return toNumPyArray<double>(getPixelValuesBuffer_VC());
////}
//////---------------------------------------------------------------------------------
////boost::python::numpy::ndarray  cameraControllerBase::getPixelValuesBuffer_NumPy1(const std::string& cam)
////{
////    return toNumPyArray<double>(getPixelValuesBuffer(cam));
////}
//////---------------------------------------------------------------------------------
////boost::python::numpy::ndarray  cameraControllerBase::getPixelValuesBuffer_NumPy2()
////{
////    return toNumPyArray<double>(getPixelValuesBuffer());
////}
//
//
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
//
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getFastImage_VC_NumPy()
//{
//    return toNumPyArray<int>(getFastImage_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getFastImage_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<int>(getFastImage(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getFastImage_NumPy2()
//{
//    return toNumPyArray<int>(getFastImage());
//}
////---------------------------------------------------------------------------------
//
//
//boost::python::numpy::ndarray  cameraControllerBase::getSumIntensityBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getSumIntensityBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSumIntensityBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getSumIntensityBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getSumIntensityBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getSumIntensityBuffer());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getAvgIntensityBuffer_VC_NumPy()
//{
//    return toNumPyArray<double>(getAvgIntensityBuffer_VC());
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getAvgIntensityBuffer_NumPy1(const std::string& cam)
//{
//    return toNumPyArray<double>(getAvgIntensityBuffer(cam));
//}
////---------------------------------------------------------------------------------
//boost::python::numpy::ndarray  cameraControllerBase::getAvgIntensityBuffer_NumPy2()
//{
//    return toNumPyArray<double>(getAvgIntensityBuffer());
//}
////---------------------------------------------------------------------------------
//
