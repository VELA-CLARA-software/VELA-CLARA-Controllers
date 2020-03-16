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
//  Last edit:   07-06-2018
//  FileName:    cameraBase.h
//  Description: all classes that use cameras should inherit from this base class
//               this class just has camera functionality
//
//*/
#ifndef __VC_CAMERABASE_H__
#define __VC_CAMERABASE_H__
// project includes
#include "configDefinitions.h"
#include "cameraStructs.h"
#include "structs.h"
#include "interface.h"
#include "claraCameraConfigReader.h"
#include "velaCameraConfigReader.h"
// stl
#include <string>
#include <vector>
#include <map>
//______________________________________________________________________________
class cameraBase : public interface
{
    public:
        cameraBase(bool& show_messages,
                   bool& show_debug_messages,
                   const bool startVirtualMachine,
                   const bool shouldStartEPICs);
        cameraBase(bool& show_messages,
                   bool& show_debug_messages,
                   const bool startVirtualMachine,
                   const bool shouldStartEPICs,
                   const std::string& claraCamConfig);

        cameraBase(bool& show_messages,
                   bool& show_debug_messages,
                   const bool startVirtualMachine,
                   const bool shouldStartEPICs,
                   const std::string& claraCamConfig,
                   const std::string& velaCamConfig,
                   const HWC_ENUM::MACHINE_AREA area
                   );

        ~cameraBase();

//-----------------------------------------------------------------------------------------
        void initialise(bool VConly);
        bool readCamConfig();
        bool getCamObjects();
        bool vcOnly();
//-----------------------------------------------------------------------------------------

        bool startAcquiring_VC();
        bool startAcquiring(const std::string& cam);
        bool startAcquiring(cameraStructs::cameraObject& cam,bool stopall);
        bool startAcquiringMultiCam(const std::string& cam);

        bool startAcquiring();
        bool stopAcquiring_VC();
        bool stopAcquiring(const std::string& cam);
        bool stopAcquiring(cameraStructs::cameraObject& cam);
        bool stopAcquiring();
        bool stopAllAcquiring();
        bool stopAllAcquiringExceptVC();
        bool startAnalysing_VC();
        bool startAnalysing(const std::string& cam);
        bool startAnalysing(cameraStructs::cameraObject& cam);
        bool startAnalysing();
        bool stopAnalysing_VC();
        bool stopAnalysing(const std::string& cam);
        bool stopAnalysing(cameraStructs::cameraObject& cam);
        bool stopAnalysing();
        bool stopAllAnalysing();
        bool stopAllAnalysingExceptVC();
        bool useBackground_VC(bool v);
        bool useBackground(bool v,const std::string& cam);
        bool useBackground(bool v,cameraStructs::cameraObject& cam);
        bool useBackground(bool v);
        bool useNPoint_VC(bool v);
        bool useNPoint(bool v,const std::string& cam);
        bool useNPoint(bool v,cameraStructs::cameraObject& cam);
        bool useNPoint(bool v);
        bool startAnalysis_VC();
        bool startAnalysis(const std::string& cam);
        bool startAnalysis(cameraStructs::cameraObject& cam);
        bool startAnalysis();
        bool stopAnalysis_VC();
        bool stopAnalysis(const std::string& cam);
        bool stopAnalysis(cameraStructs::cameraObject& cam);
        bool stopAnalysis();
        bool stopAcquireAndAnalysis_VC();
        bool stopAcquireAndAnalysis(const std::string& cam);
        bool stopAcquireAndAnalysis(cameraStructs::cameraObject& cam);
        bool stopAcquireAndAnalysis();
        bool startAcquireAndAnalysis_VC();
        bool startAcquireAndAnalysis(const std::string& cam);
        bool startAcquireAndAnalysis(cameraStructs::cameraObject& cam);
        bool startAcquireAndAnalysis();
        void clearRunningValues_VC();
        void clearRunningValues(const std::string& cam);
        void clearRunningValues();
        void clearRunningValues(cameraStructs::cameraObject& cam);
        void clearBuffer_VC();
        void clearBuffer(const std::string& cam);
        void clearBuffer(cameraStructs::cameraObject& cam);
        void clearBuffer();

//-----------------------------------------------------------------------------------------
//  __  ___      ___  ___     __                 ___    ___    __       ___    __
// /__`  |   /\   |  |__     /  \ |  |  /\  |\ |  |  | |__  | /  `  /\   |  | /  \ |\ |
// .__/  |  /~~\  |  |___    \__X \__/ /~~\ | \|  |  | |    | \__, /~~\  |  | \__/ | \|
//
//-----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

        bool claraLEDOn();
        bool claraLEDOff();
        bool isClaraLEDOn();
        bool isClaraLEDOff();
        bool velaLEDOn();
        bool velaLEDOff();
        bool isVelaLEDOn();
        bool isVelaLEDOff();


        bool isBeam_x_Hi_VC();
        bool isBeam_x_Lo_VC();
        bool isBeam_y_Hi_VC();
        bool isBeam_y_Lo_VC();

        bool isAnalysisUpdating_VC()const;
        bool isAnalysisUpdating()const;
        bool isAnalysisUpdating(const std::string& name)const;
        bool isAnalysisUpdating(const cameraStructs::cameraObject& cam)const;

        bool isAnalysisNotUpdating_VC()const;
        bool isAnalysisNotUpdating()const;
        bool isAnalysisNotUpdating(const std::string& name)const;
        bool isAnalysisNotUpdating(const cameraStructs::cameraObject& cam)const;

        bool hasBeam_VC()const;
        bool hasBeam(const cameraStructs::cameraObject& cam)const;
        bool hasBeam(const std::string& cam)const;
        bool hasBeam()const;
        bool hasNoBeam_VC()const;
        bool hasNoBeam(const cameraStructs::cameraObject& cam)const;
        bool hasNoBeam(const std::string& cam)const;
        bool hasNoBeam()const;

        bool isBusy_VC()const;
        bool isBusy(const cameraStructs::cameraObject& cam)const;
        bool isBusy(const std::string& cam)const;
        bool isBusy()const;
        bool isNotBusy_VC()const;
        bool isNotBusy(const cameraStructs::cameraObject& cam)const;
        bool isNotBusy(const std::string& cam)const;
        bool isNotBusy()const;

        bool isVelaCam_VC()const;
        bool isVelaCam(const cameraStructs::cameraObject& cam)const;
        bool isVelaCam(const std::string& cam)const;
        bool isVelaCam()const;
        bool isClaraCam_VC()const;
        bool isClaraCam(const cameraStructs::cameraObject& cam)const;
        bool isClaraCam(const std::string& cam)const;
        bool isClaraCam()const;

        bool isVelaClaraHybridCam(const cameraStructs::cameraObject& cam)const;
        bool isVelaClaraHybridCam(const std::string& cam)const;
        bool isVelaClaraHybridCam()const;


        /* this is used to make sure settings are only sent to the correct camera type*/
        bool isAcquiringAndClaraCam(const cameraStructs::cameraObject& cam)const;
        bool isAcquiringAndVelaCam(const cameraStructs::cameraObject& cam)const;
        bool isAcquiringAndVelaClaraHybridCam(const cameraStructs::cameraObject& cam)const;

        bool isUsingBackground(const std::string& cam)const;
        bool isUsingBackground()const;
        bool isUsingBackground_VC()const;
        bool isUsingBackground(const cameraStructs::cameraObject& cam)const;
        bool isNotUsingBackground(const std::string& cam)const;
        bool isNotUsingBackground()const;
        bool isNotUsingBackground_VC()const;
        bool isNotUsingBackground(const cameraStructs::cameraObject& cam)const;
        bool isUsingNPoint(const std::string& cam)const;
        bool isUsingNPoint()const;
        bool isUsingNPoint_VC()const;
        bool isUsingNPoint(const cameraStructs::cameraObject& cam)const;
        bool isNotUsingNPoint(const std::string& cam)const;
        bool isNotUsingNPoint()const;
        bool isNotUsingNPoint_VC()const;
        bool isNotUsingNPoint(const cameraStructs::cameraObject& cam)const;
        bool isAnalysing(const std::string& cam)const;
        bool isAnalysing()const;
        bool isAnalysing_VC()const;
        bool isAnalysing(const cameraStructs::cameraObject& cam)const;
        bool isNotAnalysing(const std::string& cam)const;
        bool isNotAnalysing()const;
        bool isNotAnalysing_VC()const;
        bool isNotAnalysing(const cameraStructs::cameraObject& cam)const;
        bool isCollectingOrSaving(const cameraStructs::cameraObject& cam)const;
        bool isNotCollecting(const cameraStructs::cameraObject& cam)const;
        bool isCollectingJPG(const cameraStructs::cameraObject& cam)const;
        bool isNotAcquiring(const cameraStructs::cameraObject& cam)const;
        bool isCollecting(const cameraStructs::cameraObject& cam)const;
        bool isSavingJPG(const cameraStructs::cameraObject& cam)const;
        bool isAcquiring(const cameraStructs::cameraObject& cam)const;
        bool isSaving(const cameraStructs::cameraObject& cam)const;
        bool isOFF(const cameraStructs::cameraObject& cam)const;
        bool isCollectingOrSaving(const std::string& cam)const;
        bool isNotSelectedCamera(const std::string& name)const;
        bool isON(const cameraStructs::cameraObject& cam)const;
        bool isSelectedCamera(const std::string& name)const;
        bool isCollectingJPG(const std::string& cam)const;
        bool isNotCollecting(const std::string& cam)const;
        bool isNotAcquiring(const std::string& cam)const;
        bool isCollecting(const std::string& cam)const;
        bool isSavingJPG(const std::string& cam)const;
        bool isAcquiring(const std::string& cam)const;
        bool isSaving(const std::string& cam)const;
        bool isOFF(const std::string& cam)const;
        bool isON(const std::string& cam)const;
        bool isNotCollectingOrSaving_VC()const;
        bool isNotCollectingOrSaving(const cameraStructs::cameraObject& cam)const;
        bool isNotCollectingOrSaving(const std::string& cam)const;
        bool isNotCollectingOrSaving()const;
        bool isCollectingOrSaving_VC()const;
        bool isCollectingOrSaving()const;
        bool isCollectingJPG_VC()const;
        bool isNotCollecting_VC()const;
        bool isNotAcquiring_VC()const;
        bool isNotCollecting()const;
        bool isCollecting_VC()const;
        bool isCollectingJPG()const;
        bool isNotAcquiring()const;
        bool isAcquiring_VC()const;
        bool isSavingJPG_VC()const;
        bool isSaving_VC()const;
        bool isSavingJPG()const;
        bool isCollecting()const;
        bool isAcquiring()const;
        bool isSaving()const;
        bool isOFF_VC()const;
        bool isON_VC()const;
        bool isON()const;
        bool isOFF()const;
        bool isVC(const std::string& name)const;
        bool isNotVC(const std::string& name)const;
        bool latestCollectAndSaveSuccess_VC()const;
        bool latestCollectAndSaveSuccess()const;
        bool latestCollectAndSaveSuccess(const cameraStructs::cameraObject& cam)const;
        bool isBufferFull_VC();
        bool isBufferFull(const std::string& cam);
        bool isBufferFull(const cameraStructs::cameraObject& cam);
        bool isBufferFull();
        bool isBufferNotFull_VC();
        bool isBufferNotFull(const std::string& cam);
        bool isBufferNotFull(const cameraStructs::cameraObject& cam);
        bool isBufferNotFull();
//--------------------------------------------------------------------------------------------------------------------
//            __           __   ___ ___ ___  ___  __   __
// |\/|  /\  /__` |__/    / _` |__   |   |  |__  |__) /__`
// |  | /~~\ .__/ |  \    \__> |___  |   |  |___ |  \ .__/
//
//--------------------------------------------------------------------------------------------------------------------
        unsigned short getMaskX(const cameraStructs::cameraObject& cam)const;
        unsigned short getMaskX(const std::string& cam)const;
        unsigned short getMaskX_VC()const;
        unsigned short getMaskX()const;
        unsigned short getMaskY_VC()const;
        unsigned short getMaskY(const std::string& cam)const;
        unsigned short getMaskY(const cameraStructs::cameraObject& cam)const;
        unsigned short getMaskY()const;
        unsigned short getMaskXrad_VC()const;
        unsigned short getMaskXrad(const std::string& cam )const;
        unsigned short getMaskXrad(const cameraStructs::cameraObject& cam)const;
        unsigned short getMaskXrad()const;
        unsigned short getMaskYrad_VC()const;
        unsigned short getMaskYrad(const std::string& cam )const;
        unsigned short getMaskYrad(const cameraStructs::cameraObject& cam)const;
        unsigned short getMaskYrad()const;
//--------------------------------------------------------------------------------------------------------------------
//                          __     __      __   ___  __            ___  __      __   ___ ___ ___  ___  __   __
//  /\  |\ |  /\  |    \ / /__` | /__`    |__) |__  /__` |  | |     |  /__`    / _` |__   |   |  |__  |__) /__`
// /~~\ | \| /~~\ |___  |  .__/ | .__/    |  \ |___ .__/ \__/ |___  |  .__/    \__> |___  |   |  |___ |  \ .__/
//
//--------------------------------------------------------------------------------------------------------------------
        double getX_VC()const;
        double getX(const std::string& cam)const;
        double getX(const cameraStructs::cameraObject& cam)const;
        double getX()const;
        double getY_VC()const;
        double getY(const std::string& cam)const;
        double getY(const cameraStructs::cameraObject& cam)const;
        double getY()const;
        double getSigX_VC()const;
        double getSigX(const std::string& cam)const;
        double getSigX(const cameraStructs::cameraObject& cam)const;
        double getSigX()const;
        double getSigY_VC()const;
        double getSigY(const std::string& cam)const;
        double getSigY(const cameraStructs::cameraObject& cam)const;
        double getSigY()const;
        double getSigXY_VC()const;
        double getSigXY(const std::string& cam)const;
        double getSigXY(const cameraStructs::cameraObject& cam)const;
        double getSigXY()const;
        double getXPix_VC()const;
        double getXPix(const std::string& cam)const;
        double getXPix(const cameraStructs::cameraObject& cam)const;
        double getXPix()const;
        double getYPix_VC()const;
        double getYPix(const std::string& cam)const;
        double getYPix(const cameraStructs::cameraObject& cam)const;
        double getYPix()const;
        double getSigXPix_VC()const;
        double getSigXPix(const std::string& cam)const;
        double getSigXPix(const cameraStructs::cameraObject& cam)const;
        double getSigXPix()const;
        double getSigYPix_VC()const;
        double getSigYPix(const std::string& cam)const;
        double getSigYPix(const cameraStructs::cameraObject& cam)const;
        double getSigYPix()const;
        double getSigXYPix_VC()const;
        double getSigXYPix(const std::string& cam)const;
        double getSigXYPix(const cameraStructs::cameraObject& cam)const;
        double getSigXYPix()const;

        //double getSumIntensity_VC()const;
        double getSumIntensity(const std::string& cam)const;
        double getSumIntensity(const cameraStructs::cameraObject& cam)const;
        //double getSumIntensity()const;

        double getAvgIntensity_VC()const;
        double getAvgIntensity(const std::string& cam)const;
        double getAvgIntensity(const cameraStructs::cameraObject& cam)const;
        double getAvgIntensity()const;
        size_t getBufferCount_VC()const;
        size_t getBufferCount(const std::string& cam)const;
        size_t getBufferCount(const cameraStructs::cameraObject& cam)const;
        size_t getBufferCount()const;
        size_t getBufferMaxCount_VC()const;
        size_t getBufferMaxCount(const std::string& cam)const;
        size_t getBufferMaxCount(const cameraStructs::cameraObject& cam)const;
        size_t getBufferMaxCount()const;
//eraStructs::cameraObject& cam)constconst;
        int getStepSize()const;
        std::deque<double> getXBuffer_VC()const;
        std::deque<double> getXBuffer(const std::string& cam)const;
        std::deque<double> getXBuffer()const;
        std::deque<double> getXBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getYBuffer_VC()const;
        std::deque<double> getYBuffer(const std::string& cam)const;
        std::deque<double> getYBuffer()const;
        std::deque<double> getYBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSigXBuffer_VC()const;
        std::deque<double> getSigXBuffer(const std::string& cam)const;
        std::deque<double> getSigXBuffer()const;
        std::deque<double> getSigXBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSigYBuffer_VC()const;
        std::deque<double> getSigYBuffer(const std::string& cam)const;
        std::deque<double> getSigYBuffer()const;
        std::deque<double> getSigYBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSigXYBuffer_VC()const;
        std::deque<double> getSigXYBuffer(const std::string& cam)const;
        std::deque<double> getSigXYBuffer()const;
        std::deque<double> getSigXYBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getXPixBuffer_VC()const;
        std::deque<double> getXPixBuffer(const std::string& cam)const;
        std::deque<double> getXPixBuffer()const;
        std::deque<double> getXPixBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getYPixBuffer_VC()const;
        std::deque<double> getYPixBuffer(const std::string& cam)const;
        std::deque<double> getYPixBuffer()const;
        std::deque<double> getYPixBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSigXPixBuffer_VC()const;
        std::deque<double> getSigXPixBuffer(const std::string& cam)const;
        std::deque<double> getSigXPixBuffer()const;
        std::deque<double> getSigXPixBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSigYPixBuffer_VC()const;
        std::deque<double> getSigYPixBuffer(const std::string& cam)const;
        std::deque<double> getSigYPixBuffer()const;
        std::deque<double> getSigYPixBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSigXYPixBuffer_VC()const;
        std::deque<double> getSigXYPixBuffer(const std::string& cam)const;
        std::deque<double> getSigXYPixBuffer()const;
        std::deque<double> getSigXYPixBuffer(const cameraStructs::cameraObject& cam)const;
        std::vector<double> getPixelValues_VC()const;
        std::vector<double> getPixelValues(const std::string& cam)const;
        std::vector<double> getPixelValues()const;
        std::vector<double> getPixelValues(const cameraStructs::cameraObject& cam)const;
        std::deque<std::vector<double>> getPixelValuesBuffer_VC()const;
        std::deque<std::vector<double>> getPixelValuesBuffer(const std::string& cam)const;
        std::deque<std::vector<double>> getPixelValuesBuffer()const;
        std::deque<std::vector<double>> getPixelValuesBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSumIntensityBuffer_VC()const;
        std::deque<double> getSumIntensityBuffer(const std::string& cam)const;
        std::deque<double> getSumIntensityBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getSumIntensityBuffer()const;
        std::deque<double> getAvgIntensityBuffer_VC()const;
        std::deque<double> getAvgIntensityBuffer(const std::string& cam)const;
        std::deque<double> getAvgIntensityBuffer(const cameraStructs::cameraObject& cam)const;
        std::deque<double> getAvgIntensityBuffer()const;

        std::vector<int> getFastImage_VC()const;
        std::vector<int> getFastImage(const std::string& cam)const;
        std::vector<int> getFastImage()const;
        std::vector<int> getFastImage(const cameraStructs::cameraObject& cam)const;


        bool takeFastImage(cameraStructs::cameraObject& cam);
        bool takeFastImage(const std::string& cam);
        bool takeFastImage_VC();
        bool takeFastImage();

#ifdef BUILD_DLL
        boost::python::list takeAndGetFastImage2D_VC();
        boost::python::list takeAndGetFastImage2D(const std::string& cam);
        boost::python::list takeAndGetFastImage2D();
        boost::python::list takeAndGetFastImage2D(cameraStructs::cameraObject& cam);

        boost::python::list takeAndGetFastImage_VC();
        boost::python::list takeAndGetFastImage(const std::string& cam);
        boost::python::list takeAndGetFastImage();
        boost::python::list takeAndGetFastImage(cameraStructs::cameraObject& cam);

        boost::python::list getFastImage_VC_Py()const;
        boost::python::list getFastImage_Py(const std::string& cam)const;
        boost::python::list getFastImage_Py()const;
        boost::python::list getFastImage_Py(const cameraStructs::cameraObject& cam)const;

        boost::python::list getFastImage2D_VC_Py()const;
        boost::python::list getFastImage2D_Py(const std::string& cam)const;
        boost::python::list getFastImage2D_Py()const;
        boost::python::list getFastImage2D_Py(const cameraStructs::cameraObject& cam)const;
#endif

        double getPix2mm_VC() const;
        double getPix2mm() const;
        double getPix2mm(const std::string& name) const;
        double getPix2mm(const cameraStructs::cameraObject& cam) const;

        double getPix2mmDef_VC() const;
        double getPix2mmDef() const;
        double getPix2mmDef(const std::string& name) const;
        double getPix2mmDef(const cameraStructs::cameraObject& cam) const;


        int getGain_VC()const;
        int getGain(const std::string& cam)const;
        int getGain()const;
        int getGain(const cameraStructs::cameraObject& cam)const;
        int getBlacklevel_VC()const;
        int getBlacklevel(const std::string& cam)const;
        int getBlacklevel()const;
        int getBlacklevel(const cameraStructs::cameraObject& cam)const;

        int getStepSize_VC()const;
        int getStepSize(const std::string& cam)const;
        int getStepSize(const cameraStructs::cameraObject& cam)const;
        std::string getLatestDirectory_VC()const;
        std::string getLatestDirectory(const std::string& cam)const;
        std::string getLatestDirectory(const cameraStructs::cameraObject& cam)const;
        std::string getLatestDirectory()const;

        std::string getLatestFilename_VC()const;
        std::string getLatestFilename(const std::string& cam)const;
        std::string getLatestFilename(const cameraStructs::cameraObject& cam)const;
        std::string getLatestFilename()const;

        std::vector<std::string> getCameraNames()const;
        std::vector<std::string> getCameraScreenNames()const;
//--------------------------------------------------------------------------------------------------------------------
//  __   ___ ___ ___  ___  __   __
// /__` |__   |   |  |__  |__) /__`
// .__/ |___  |   |  |___ |  \ .__/
//
//--------------------------------------------------------------------------------------------------------------------
        void setBufferMaxCount_VC(const size_t s);
        void setBufferMaxCount(const size_t s,const std::string& cam);
        void setBufferMaxCount(const size_t s, cameraStructs::cameraObject& cam);
        void setBufferMaxCount(const size_t s);
        bool setMaskX_VC(unsigned short x);
        bool setMaskX(unsigned short x,const std::string& cam);
        bool setMaskX(unsigned short x,cameraStructs::cameraObject& cam);
        bool setMaskX(unsigned short x);
        bool setMaskY_VC(unsigned short x);
        bool setMaskY(unsigned short x,const std::string& cam);
        bool setMaskY(unsigned short x,cameraStructs::cameraObject& cam);
        bool setMaskY(unsigned short x);
        bool setMaskXrad_VC(unsigned short x);
        bool setMaskXrad(unsigned short x,const std::string& cam);
        bool setMaskXrad(unsigned short x,cameraStructs::cameraObject& cam);
        bool setMaskXrad(unsigned short x);
        bool setMaskYrad_VC(unsigned short x);
        bool setMaskYrad(unsigned short x,const std::string& cam);
        bool setMaskYrad(unsigned short x,cameraStructs::cameraObject& cam);
        bool setMaskYrad(unsigned short x);
        bool setMask_VC(unsigned short x, unsigned short y,unsigned short xr,unsigned short yr);
        bool setMask(unsigned short x,unsigned short y,unsigned short xr,unsigned short yr,const std::string& cam );
        bool setMask(unsigned short x, unsigned short y,unsigned short xr,unsigned short yr,cameraStructs::cameraObject& cam);
        bool setMask(unsigned short x, unsigned short y,unsigned short xr,unsigned short yr);
        bool setMask_VC(const std::vector<unsigned short>& vales);
        bool setMask(const std::vector<unsigned short>& vales,const std::string& cam);
        bool setMask(const std::vector<unsigned short>& vales,cameraStructs::cameraObject& cam);
        bool setMask(const std::vector<unsigned short>& vales);
        bool setBackground_VC();
        bool setBackground(const std::string& cam);
        bool setBackground(cameraStructs::cameraObject& cam);
        bool setBackground();
        bool setStepSize_VC(unsigned short step);
        bool setStepSize(unsigned short step,const std::string& cam);
        bool setStepSize(unsigned short step,cameraStructs::cameraObject& cam);
        bool setStepSize(unsigned short step);
        bool setCenterXPixel_VC(unsigned short  xC);
        bool setCenterXPixel(unsigned short  xC,const std::string& cam);
        bool setCenterXPixel(unsigned short  xC,cameraStructs::cameraObject& cam);
        bool setCenterXPixel(unsigned short  xC);
        bool setCenterYPixel_VC(unsigned short  yC);
        bool setCenterYPixel(unsigned short  yC,const std::string& cam);
        bool setCenterYPixel(unsigned short  yC,cameraStructs::cameraObject& cam);
        bool setCenterYPixel(unsigned short  yC);
        bool setPixMM_VC(const double pmm);
        bool setPixMM(const double pmm,const std::string& cam);
        bool setPixMM(const double pmm,cameraStructs::cameraObject& cam);
        bool setPixMM(const double pmm);

        bool setGain_VC(const long value);
        bool setGain(const std::string& cam,const long value);
        bool setGain(const long value);
        bool setGain(cameraStructs::cameraObject& cam, const long value);
        bool setBlacklevel_VC(const long value);
        bool setBlacklevel(const std::string& cam,const long value);
        bool setBlacklevel(const long value);
        bool setBlacklevel(cameraStructs::cameraObject& cam, const long value);


        //---------------------------------------------------------------------------------
         bool setMaskFeedBackOn_VC();
         bool setMaskFeedBackOn();
         bool setMaskFeedBackOn(const std::string& name);
         bool setMaskFeedBackOn(cameraStructs::cameraObject& cam);
         bool setMaskFeedBackOff_VC();
         bool setMaskFeedBackOff();
         bool setMaskFeedBackOff(const std::string& name);
         bool setMaskFeedBackOff(cameraStructs::cameraObject& cam);
         bool isMaskFeedbackOn_VC()const;
         bool isMaskFeedbackOn()const;
         bool isMaskFeedbackOn(const std::string& name)const;
         bool isMaskFeedbackOn(const cameraStructs::cameraObject& cam)const;
         bool isMaskFeedbackOff_VC()const;
         bool isMaskFeedbackOff()const;
         bool isMaskFeedbackOff(const std::string& name)const;
         bool isMaskFeedbackOff(const cameraStructs::cameraObject& cam)const;
         void maskFeedBack(cameraStructs::cameraObject& cam);

         std::string getSelectedCamName() const;
         std::string getSelectedCamScrName() const;
         size_t getNumPixX() const;
         size_t  getNumPixY() const;
//--------------------------------------------------------------------------------------------------------------------
//
//  __   __        ___  __  ___     __   __        __  ___     __   ___  ___  __
// /  \ |__)    | |__  /  `  |     /  ` /  \ |\ | /__`  |     |__) |__  |__  /__`
// \__/ |__) \__/ |___ \__,  |     \__, \__/ | \| .__/  |     |  \ |___ |    .__/
//
//--------------------------------------------------------------------------------------------------------------------
        const cameraStructs::analysis_mask& getMaskObj(const std::string& name)const;
        const cameraStructs::analysis_mask& getMaskObj_VC()const;
        const cameraStructs::analysis_mask& getMaskObj()const;
        const cameraStructs::analysis_mask& getMaskObj(const cameraStructs::cameraObject& cam)const;
        const cameraStructs::analysis_data& getAnalysisObj(const std::string& name)const;
        const cameraStructs::analysis_data& getAnalysisObj_VC()const;
        const cameraStructs::analysis_data& getAnalysisObj()const;
        const cameraStructs::analysis_data& getAnalysisObj(const cameraStructs::cameraObject& cam)const;
        const cameraStructs::fast_image& getImageObj(const std::string& name)const;
        const cameraStructs::fast_image& getImageObj_VC()const;
        const cameraStructs::fast_image& getImageObj()const;
        const cameraStructs::fast_image& getImageObj(const cameraStructs::cameraObject& cam)const;
        const cameraStructs::camera_image_data& getImageDataObj(const std::string& name)const;
        const cameraStructs::camera_image_data& getImageDataObj_VC()const;
        const cameraStructs::camera_image_data& getImageDataObj()const;
        const cameraStructs::camera_image_data& getImageDataObj(const cameraStructs::cameraObject& cam)const;
        const cameraStructs::clara_DAQ& getClaraDAQObj(const std::string& name)const;
        const cameraStructs::clara_DAQ& getClaraDAQObj_VC()const;
        const cameraStructs::clara_DAQ& getClaraDAQObj()const;
        const cameraStructs::clara_DAQ& getClaraDAQObj(const cameraStructs::cameraObject& cam)const;
        const cameraStructs::cameraObject& getCameraObj(const std::string& name)const;
        const cameraStructs::cameraObject& getCameraObj_VC()const;
        const cameraStructs::cameraObject& getCameraObj()const;
        const cameraStructs::cameraObject& getCameraObj(const cameraStructs::cameraObject& cam)const;
        const cameraStructs::camera_state& getStateObj(const std::string& name)const;
        const cameraStructs::camera_state& getStateObj_VC()const;
        const cameraStructs::camera_state& getStateObj()const;
        const cameraStructs::camera_state& getStateObj(const cameraStructs::cameraObject& cam)const;

        /*
            DONT EXPOSE TO PYTHON!!!
        */
        runningStat& get_x_private_rs_ref_VC();
        runningStat& get_y_private_rs_ref_VC();


//______________________________________________________________________________
        /*
            Image Collection functions
        */
        bool collectAndSave(const int numbOfShots);
        bool collectAndSave_VC(const int numbOfShots);
        bool collectAndSave(const std::string& camName, const int numbOfShots);
        std::map<std::string,cameraStructs::imageCollectStruct> imageCollectStructs;
        void killFinishedImageCollectThreads();
        static void staticEntryImageCollectAndSave(cameraStructs::imageCollectStruct& ics);
        void imageCollectAndSave(cameraStructs::imageCollectStruct& ics);
        bool setNumberOfCapture(cameraStructs::cameraObject& cam, int numberOfShots);
        bool collect(cameraStructs::cameraObject& cam);
        bool makeANewDirectoryAndName(cameraStructs::cameraObject& camera, int numbOfShots);
        bool write(cameraStructs::cameraObject& cam);
        bool setStartFileNumber(const int startNumber);
        std::string getWriteErrorMessage();
        /*
            Image Collection functions JPG veriosn not used
        */
        bool collectAndSaveJPG ();
        bool staticCollectAndSaveJPG(cameraStructs::cameraObject camera,const int & numbOfShots);
        bool killCollectAndSave();
        bool killCollectAndSaveJPG();
        bool killCollectAndSaveVC();
        bool collectJPG(cameraStructs::cameraObject camera,unsigned short &comm,const int & numbOfShots);
        bool saveJPG(cameraStructs::cameraObject camera,unsigned short &comm);
        //bool makeANewDirectoryAndNameJPG(cameraStructs::cameraObject &camera,const int &numbOfShots);
        //bool setNumberOfShotsJPG(const int &numberOfShots);
        //bool setStartFileNumberJPG(const int &startNumber);

        std::string getWriteErrorMessageJPG();

        bool setCamera_VC();
        bool setCamera(const std::string &cam);
        /*
            All camera data is held here ...
        */
        std::map<std::string,cameraStructs::cameraObject>  allCamData;
        // thes efunciton sneed to be called from class that inherits this
        void initCamChids(bool sendToEpics);
        void initImageCollectStructs();
        bool startCamMonitors(bool sendToEPICS);
        std::vector<cameraStructs::monitorStruct*> continuousCamMonitorStructs;
        /*
            all monitors route here:
        */
        static void staticEntryCamMonitor(const event_handler_args args);
        void updateCamValue(const cameraStructs::CAM_PV_TYPE pv, const std::string& objName, const event_handler_args& args);

        /*
            update  is the camera switched ON / OFF
        */
        void updateChannelCountandType(cameraStructs::pvStruct& pv);

        void addCamChannel(const std::string& pvRoot,cameraStructs::pvStruct& pv);
        /*
            update  is the camera switched ON / OFF
        */
        void updateCamState (const event_handler_args& args, cameraStructs::CAM_STATE& s);
        /*
            update is the camera acquiring images
        */
        void updateAcquiring(const event_handler_args& args, cameraStructs::ACQUIRE_STATE& s);
        void updateCollectingState (const event_handler_args& args, cameraStructs::COLLECTING_STATE& s);
        void updateWriteState(const event_handler_args& args, cameraStructs::SAVE_STATE& s);
        void updateWriteCheck(const event_handler_args& args, cameraStructs::WRITE_CHECK& w);
        void updateWriteErrorMessage(const event_handler_args& args, std::string& w);
        void updateSensorTemp(const event_handler_args& args, cameraStructs::cameraObject& cam);
        void updateSelectedCamRef(cameraStructs::cameraObject& cam);
        void updateWriteErrorMessageJPG(const void *value, cameraStructs::cameraObject& cam);
        void updatePixelResults(const event_handler_args& args, cameraStructs::cameraObject& data);
        void addToBuffer(const double val, std::deque<double>& buffer,cameraStructs::analysis_data& data);

        std::string useCameraFrom(const std::string& camOrScreen)const;
        /*
            active camera can switch so we have pointers to the selected
            camera. On init they point  to dummyObject (>>?)
        */
        cameraStructs::cameraObject  dummyCamObject;
        cameraStructs::cameraObject* selectedCamPtr;
        cameraStructs::cameraObject* vcCamPtr;
        /*
            getter, setters etc. that use names get objects
            from this function
        */
        cameraStructs::cameraObject& getCamObj(const std::string& cam);
        const cameraStructs::cameraObject& getCamObj(const std::string& cam)const;
        /*
            all caput functions call this
        */
        template<class T>
        bool cam_caput(cameraStructs::cameraObject& cam,  T& value, const cameraStructs::CAM_PV_TYPE pv)
        {
            if(isNotDummyName(cam.name))
            {
                if(entryExists<cameraStructs::pvStruct,cameraStructs::CAM_PV_TYPE>(cam.pvComStructs,pv))
                {
                    std::string timeout("!!TIMEOUT!! ");
                    timeout.append(cam.name);
                    timeout.append(" for pv = ");
                    timeout.append(ENUM_TO_STRING(pv));
//                    message("CHTYPE = ",cam.pvComStructs.at(pv).CHTYPE,
//                            " CHID   = ",cam.pvComStructs.at(pv).CHID,
//                            " VALUE  = ", value);
                    ca_put(DBR_ENUM, cam.pvComStructs.at(pv).CHID, &value);
                    int success = sendToEpics("ca_put", "", timeout.c_str());
                    if(success == ECA_NORMAL)
                        return true;
                    else
                        return false;
                }
            }
            else
                message("!!WARNING!! cam_caput passed cam name = ",cam.name);
            return false;
        }
        /*
            All analysis results call this function
        */
        void updateAnalysislResult(const event_handler_args& args,
                                   double& value_to_update,
                                   std::deque<double>& buffer_to_update,
                                   runningStat& rs_to_update,
                                   cameraStructs::analysis_data& data);

        claraCameraConfigReader claraCamConfigReader;
        velaCameraConfigReader  velaCamConfigReader;

        void updateSelectedOrVC(const std::string& cameraName);
    protected:
    private:

        static void mask_feedback(unsigned short x, unsigned short y,
                                  unsigned short x_rad, unsigned short y_rad,
                                  const std::string& name, cameraBase* interface);


        void updateCLaraLEDState(const event_handler_args& args);
        void updateVelaLEDState(const event_handler_args& args);
        HWC_ENUM::STATE clara_led_state;
        HWC_ENUM::STATE vela_led_state;


        std::string getLastDirectoryandFileName();


        /* called during destruction ... */
        void killMonitor(cameraStructs::monitorStruct * ms);

        // testing
        epicsTime et;
        epicsTime timestamp;

        bool isLED_PV(cameraStructs::CAM_PV_TYPE pv);
        bool toggelLED(cameraStructs::pvStruct& pvs);

        bool updateArrayData(cameraStructs::cameraObject& cam, const event_handler_args& args);

        const HWC_ENUM::MACHINE_AREA myarea;
};
#endif // __VC_CAMERABASE_H__
