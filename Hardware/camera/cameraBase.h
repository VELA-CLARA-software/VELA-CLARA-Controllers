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
//               this class just has cmaera functionality it doesn't connect to epics
//               or set up monitors etc. that should happen in the dervied class
//*/
#ifndef __VC_CAMERABASE_H__
#define __VC_CAMERABASE_H__
// project includes
#include "configDefinitions.h"
#include "cameraStructs.h"
#include "interface.h"
#include "claraCameraConfigReader.h"
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
                   const std::string& velaCamConfig);

        ~cameraBase();

//-----------------------------------------------------------------------------------------
        void initialise(bool VConly);
        bool readCamConfig();
        bool getCamObjects();
        bool vcOnly();
//-----------------------------------------------------------------------------------------


        bool startAcquiring_VC();
        bool startAcquiring(const std::string& cam);
        bool startAcquiring(cameraStructs::cameraObject& cam);
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
        int getMaskX(const cameraStructs::cameraObject& cam)const;
        int getMaskX(const std::string& cam)const;
        int getMaskX_VC()const;
        int getMaskX()const;
        int getMaskY_VC()const;
        int getMaskY(const std::string& cam)const;
        int getMaskY(const cameraStructs::cameraObject& cam)const;
        int getMaskY()const;
        int getMaskXrad_VC()const;
        int getMaskXrad(const std::string& cam )const;
        int getMaskXrad(const cameraStructs::cameraObject& cam)const;
        int getMaskXrad()const;
        int getMaskYrad_VC()const;
        int getMaskYrad(const std::string& cam )const;
        int getMaskYrad(const cameraStructs::cameraObject& cam)const;
        int getMaskYrad()const;
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
        double getSumIntensity_VC()const;
        double getSumIntensity(const std::string& cam)const;
        double getSumIntensity(const cameraStructs::cameraObject& cam)const;
        double getSumIntensity()const;
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
        std::vector<int> getFastImage_VC();
        std::vector<int> getFastImage(const std::string& cam);
        std::vector<int> getFastImage();
        std::vector<int> getFastImage(cameraStructs::cameraObject& cam);
        bool cagetFastImage(cameraStructs::cameraObject& cam);
        bool cagetFastImage(const std::string& cam);
#ifdef BUILD_DLL
        boost::python::list getFastImage2D_VC();
        boost::python::list getFastImage2D(const std::string& cam);
        boost::python::list getFastImage2D();
        boost::python::list getFastImage2D(cameraStructs::cameraObject& cam);
#endif
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
        bool setMaskX_VC(int x);
        bool setMaskX(int x,const std::string& cam);
        bool setMaskX(int x,cameraStructs::cameraObject& cam);
        bool setMaskX(int x);
        bool setMaskY_VC(int x);
        bool setMaskY(int x,const std::string& cam);
        bool setMaskY(int x,cameraStructs::cameraObject& cam);
        bool setMaskY(int x);
        bool setMaskXrad_VC(int x);
        bool setMaskXrad(int x,const std::string& cam);
        bool setMaskXrad(int x,cameraStructs::cameraObject& cam);
        bool setMaskXrad(int x);
        bool setMaskYrad_VC(int x);
        bool setMaskYrad(int x,const std::string& cam);
        bool setMaskYrad(int x,cameraStructs::cameraObject& cam);
        bool setMaskYrad(int x);
        bool setMask_VC(int x, int y,int xr,int yr);
        bool setMask(int x,int y,int xr,int yr,const std::string& cam );
        bool setMask(int x, int y,int xr,int yr,cameraStructs::cameraObject& cam);
        bool setMask(int x, int y,int xr,int yr);
        bool setMask_VC(const std::vector<int>& vales);
        bool setMask(const std::vector<int>& vales,const std::string& cam);
        bool setMask(const std::vector<int>& vales,cameraStructs::cameraObject& cam);
        bool setMask(const std::vector<int>& vales);
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
//--------------------------------------------------------------------------------------------------------------------
//
//  __   __        ___  __  ___     __   __        __  ___     __   ___  ___  __
// /  \ |__)    | |__  /  `  |     /  ` /  \ |\ | /__`  |     |__) |__  |__  /__`
// \__/ |__) \__/ |___ \__,  |     \__, \__/ | \| .__/  |     |  \ |___ |    .__/
//
//--------------------------------------------------------------------------------------------------------------------
        const cameraStructs::analysis_mask& getMaskObj(const std::string& name);
        const cameraStructs::analysis_mask& getMaskObj_VC();
        const cameraStructs::analysis_mask& getMaskObj();
        const cameraStructs::analysis_mask& getMaskObj(const cameraStructs::cameraObject& cam);
        const cameraStructs::analysis_data& getAnalysisObj(const std::string& name);
        const cameraStructs::analysis_data& getAnalysisObj_VC();
        const cameraStructs::analysis_data& getAnalysisObj();
        const cameraStructs::analysis_data& getAnalysisObj(const cameraStructs::cameraObject& cam);
        const cameraStructs::fast_image& getImageObj(const std::string& name);
        const cameraStructs::fast_image& getImageObj_VC();
        const cameraStructs::fast_image& getImageObj();
        const cameraStructs::fast_image& getImageObj(const cameraStructs::cameraObject& cam);
        const cameraStructs::camera_image_data& getImageDataObj(const std::string& name);
        const cameraStructs::camera_image_data& getImageDataObj_VC();
        const cameraStructs::camera_image_data& getImageDataObj();
        const cameraStructs::camera_image_data& getImageDataObj(const cameraStructs::cameraObject& cam);
        const cameraStructs::clara_DAQ& getClaraDAQObj(const std::string& name);
        const cameraStructs::clara_DAQ& getClaraDAQ_VC();
        const cameraStructs::clara_DAQ& getClaraDAQObj();
        const cameraStructs::clara_DAQ& getClaraDAQObj(const cameraStructs::cameraObject& cam);
        const cameraStructs::cameraObject& getCameraObj(const std::string& name);
        const cameraStructs::cameraObject& getCameraObj_VC();
        const cameraStructs::cameraObject& getCameraObj();
        const cameraStructs::cameraObject& getCameraObj(const cameraStructs::cameraObject& cam);
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
        void updateWriteCheck(const event_handler_args& args, cameraStructs::SAVE_CHECK& w);
        void updateWriteErrorMessage(const event_handler_args& args, std::string& w);
        void updateSensorTemp(const event_handler_args& args, cameraStructs::cameraObject& cam);
        void updateSelectedCamRef(cameraStructs::cameraObject& cam);
        void updateWriteErrorMessageJPG(const void *value, cameraStructs::cameraObject& cam);
        void updatePixelResults(const event_handler_args& args, cameraStructs::analysis_data& data);
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

        void updateSelectedOrVC(const std::string& cameraName);
    protected:
    private:
};
#endif // __VC_CAMERABASE_H__
