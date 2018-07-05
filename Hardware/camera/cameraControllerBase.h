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
//  FileName:    cameraControllerBase.h
//  Description: all modules that use cameras should inherit from this
//
//
//*/

#ifndef _CAMERA_CONTROLLER_BASE_H_
#define _CAMERA_CONTROLLER_BASE_H_
// project includes
#include "cameraBase.h"
#include "controller.h"
// stl includes
#include <vector>
#include <deque>

class cameraControllerBase : public controller
{
    public:
        cameraControllerBase(bool& show_messages,
                             bool& show_debug_messages,
                             const bool startVirtualMachine,
                             const bool shouldStartEPICs,
                             const std::string& name,
                             const std::string& claraCamConfig,
                             HWC_ENUM::CONTROLLER_TYPE type);
        cameraControllerBase(bool& show_messages,
                             bool& show_debug_messages,
                             const bool startVirtualMachine,
                             const bool shouldStartEPICs,
                             const std::string& name,
                             const std::string& claraCamConfig,
                             HWC_ENUM::CONTROLLER_TYPE type,
                             bool vcONly);

        cameraControllerBase(bool& show_messages,
                             bool& show_debug_messages,
                             const std::string& name,
                             HWC_ENUM::CONTROLLER_TYPE type,
                             cameraBase* camBaseIN
                             );

        virtual ~cameraControllerBase();
        /*
            virtual overloads
        */
        void  set_CA_PEND_IO_TIMEOUT(double val);
        double get_CA_PEND_IO_TIMEOUT()const;
//-----------------------------------------------------------------------------------------
//  __   __                         __   __
// /  ` /  \  |\/|  |\/|  /\  |\ | |  \ /__`
// \__, \__/  |  |  |  | /~~\ | \| |__/ .__/
//
//-----------------------------------------------------------------------------------------
        bool takeFastImage(const std::string& cam);
        bool takeFastImage_VC();
        bool takeFastImage();
        bool startAnalysis_VC();
        bool startAnalysis(const std::string& cam);
        bool startAnalysis();
        bool stopAnalysis_VC();
        bool stopAnalysis(const std::string& cam);
        bool stopAnalysis();
        bool startAcquiring_VC();
        bool startAcquiring(const std::string& cam);
        bool startAcquiring();
        bool stopAcquiring_VC();
        bool stopAcquiring(const std::string& cam);
        bool stopAcquiring();
        bool stopAllAcquiring();
        bool stopAllAcquiringExceptVC();
        bool startAnalysing_VC();
        bool startAnalysing(const std::string& cam);
        bool startAnalysing();
        bool stopAnalysing_VC();
        bool stopAnalysing(const std::string& cam);
        bool stopAnalysing();
        bool stopAllAnalysing();
        bool stopAllAnalysingExceptVC();
        bool stopAcquireAndAnalysis_VC();
        bool stopAcquireAndAnalysis(const std::string& cam);
        bool stopAcquireAndAnalysis();
        bool startAcquireAndAnalysis_VC();
        bool startAcquireAndAnalysis(const std::string& cam);
        bool startAcquireAndAnalysis();
        bool useBackground_VC(bool v);
        bool useBackground(bool v,const std::string& cam);
        bool useBackground(bool v);
        bool useNPoint_VC(bool v);
        bool useNPoint(bool v,const std::string& cam);
        bool useNPoint(bool v);
        void clearRunningValues_VC();
        void clearRunningValues(const std::string& cam);
        void clearRunningValues();
        bool collectAndSave(const int numbOfShots);
        bool collectAndSave(const std::string& name,const int numbOfShots);
        bool collectAndSave_VC(const int numbOfShots);
        void clearBuffer_VC();
        void clearBuffer(const std::string& cam);
        void clearBuffer();
//-----------------------------------------------------------------------------------------
//  __  ___      ___  ___     __                 ___    ___    __       ___    __
// /__`  |   /\   |  |__     /  \ |  |  /\  |\ |  |  | |__  | /  `  /\   |  | /  \ |\ |
// .__/  |  /~~\  |  |___    \__X \__/ /~~\ | \|  |  | |    | \__, /~~\  |  | \__/ | \|
//
//-----------------------------------------------------------------------------------------
        bool isBusy_VC()const;
        bool isBusy(const std::string& cam)const;
        bool isBusy()const;
        bool isNotBusy_VC()const;
        bool isNotBusy(const std::string& cam)const;
        bool isNotBusy()const;
        bool isNotCollectingOrSaving(const std::string& cam)const;
        bool isNotUsingBackground(const std::string& cam)const;
        bool isCollectingOrSaving(const std::string& cam)const;
        bool isNotSelectedCamera(const std::string& name)const;
        bool isUsingBackground_VC()const;
        bool isUsingBackground(const std::string& cam)const;
        bool isUsingBackground()const;
        bool isNotUsingBackground_VC()const;
        bool isNotUsingBackground()const;
        bool isUsingNPoint_VC()const;
        bool isUsingNPoint(const std::string& cam)const;
        bool isUsingNPoint()const;
        bool isNotUsingNPoint_VC()const;
        bool isNotUsingNPoint(const std::string& cam)const;
        bool isNotUsingNPoint()const;
        bool isNotCollectingOrSaving_VC()const;
        bool isSelectedCamera(const std::string& name)const;
        bool isNotCollectingOrSaving()const;
        bool isVC(const std::string& name)const;
        bool isNotVC(const std::string& name)const;
        bool isON_VC()const;
        bool isON(const std::string& cam)const;
        bool isON()const;
        bool isOFF_VC()const;
        bool isOFF(const std::string& cam)const;
        bool isOFF()const;
        bool isAcquiring_VC()const;
        bool isAcquiring(const std::string& cam)const;
        bool isAcquiring()const;
        bool isNotAcquiring_VC()const;
        bool isNotAcquiring(const std::string& cam)const;
        bool isNotAcquiring()const;
        bool isAnalysing(const std::string& cam)const;
        bool isAnalysing()const;
        bool isAnalysing_VC()const;
        bool isNotAnalysing(const std::string& cam)const;
        bool isNotAnalysing()const;
        bool isNotAnalysing_VC()const;
        bool isBufferFull_VC()const;
        bool isBufferFull(const std::string& cam)const;
        bool isBufferFull()const;
        bool isBufferNotFull_VC()const;
        bool isBufferNotFull(const std::string& cam)const;
        bool isBufferNotFull()const;
        bool isCollecting_VC()const;
        bool isCollecting(const std::string& cam)const;
        bool isCollecting()const;
        bool isSaving_VC()const;
        bool isSaving(const std::string& cam)const;
        bool isSaving()const;
        bool isCollectingJPG_VC()const;
        bool isCollectingJPG(const std::string& cam)const;
        bool isCollectingJPG()const;
        bool isSavingJPG_VC()const;
        bool isSavingJPG(const std::string& cam)const;
        bool isSavingJPG()const;
        bool isCollectingOrSaving_VC()const;
        bool isCollectingOrSaving()const;
        bool isNotCollecting_VC()const;
        bool isNotCollecting(const std::string& cam)const;
        bool isNotCollecting()const;
        bool latestCollectAndSaveSuccess_VC()const;
        bool latestCollectAndSaveSuccess()const;
//-----------------------------------------------------------------------------------------
//  __   ___ ___ ___  ___  __   __
// / _` |__   |   |  |__  |__) /__`
// \__> |___  |   |  |___ |  \ .__/
//
//-----------------------------------------------------------------------------------------
        int getMaskX_VC()const;
        int getMaskX(const std::string& cam )const;
        int getMaskX()const;
        int getMaskY_VC()const;
        int getMaskY(const std::string& cam)const;
        int getMaskY()const;
        int getMaskXrad_VC()const;
        int getMaskXrad(const std::string& cam )const;
        int getMaskXrad()const;
        int getMaskYrad_VC()const;
        int getMaskYrad(const std::string& cam )const;
        int getMaskYrad()const;
        std::deque<double> getXBuffer_VC()const;
        std::deque<double> getXBuffer(const std::string& cam)const;
        std::deque<double> getXBuffer()const;
        std::deque<double> getYBuffer_VC()const;
        std::deque<double> getYBuffer(const std::string& cam)const;
        std::deque<double> getYBuffer()const;
        std::deque<double> getSigXBuffer_VC()const;
        std::deque<double> getSigXBuffer(const std::string& cam)const;
        std::deque<double> getSigXBuffer()const;
        std::deque<double> getSigYBuffer_VC()const;
        std::deque<double> getSigYBuffer(const std::string& cam)const;
        std::deque<double> getSigYBuffer()const;
        std::deque<double> getSigXYBuffer_VC()const;
        std::deque<double> getSigXYBuffer(const std::string& cam)const;
        std::deque<double> getSigXYBuffer()const;
        std::deque<double> getXPixBuffer_VC()const;
        std::deque<double> getXPixBuffer(const std::string& cam)const;
        std::deque<double> getXPixBuffer()const;
        std::deque<double> getYPixBuffer_VC()const;
        std::deque<double> getYPixBuffer(const std::string& cam)const;
        std::deque<double> getYPixBuffer()const;
        std::deque<double> getSigXPixBuffer_VC()const;
        std::deque<double> getSigXPixBuffer(const std::string& cam)const;
        std::deque<double> getSigXPixBuffer()const;
        std::deque<double> getSigYPixBuffer_VC()const;
        std::deque<double> getSigYPixBuffer(const std::string& cam)const;
        std::deque<double> getSigYPixBuffer()const;
        std::deque<double> getSigXYPixBuffer_VC()const;
        std::deque<double> getSigXYPixBuffer(const std::string& cam)const;
        std::deque<double> getSigXYPixBuffer()const;
        std::vector<double> getPixelValues_VC()const;
        std::vector<double> getPixelValues(const std::string& cam)const;
        std::vector<double> getPixelValues()const;
        std::deque<std::vector<double>> getPixelValuesBuffer_VC()const;
        std::deque<std::vector<double>> getPixelValuesBuffer(const std::string& cam)const;
        std::deque<std::vector<double>> getPixelValuesBuffer()const;
        std::deque<double> getSumIntensityBuffer_VC()const;
        std::deque<double> getSumIntensityBuffer(const std::string& cam)const;
        std::deque<double> getSumIntensityBuffer()const;
        std::deque<double> getAvgIntensityBuffer_VC()const;
        std::deque<double> getAvgIntensityBuffer(const std::string& cam)const;
        std::deque<double> getAvgIntensityBuffer()const;
        std::vector<int> getFastImage_VC()const;
        std::vector<int> getFastImage(const std::string& cam)const;
        std::vector<int> getFastImage()const;
#ifdef BUILD_DLL
        boost::python::list getXBuffer_VC_Py()const;
        boost::python::list getXBuffer_Py1(const std::string& cam)const;
        boost::python::list getXBuffer_Py2()const;
        boost::python::list getYBuffer_VC_Py()const;
        boost::python::list getYBuffer_Py1(const std::string& cam)const;
        boost::python::list getYBuffer_Py2()const;
        boost::python::list getSigXBuffer_VC_Py()const;
        boost::python::list getSigXBuffer_Py1(const std::string& cam)const;
        boost::python::list getSigXBuffer_Py2()const;
        boost::python::list getSigYBuffer_VC_Py()const;
        boost::python::list getSigYBuffer_Py1(const std::string& cam)const;
        boost::python::list getSigYBuffer_Py2()const;
        boost::python::list getSigXYBuffer_VC_Py()const;
        boost::python::list getSigXYBuffer_Py1(const std::string& cam)const;
        boost::python::list getSigXYBuffer_Py2()const;
        boost::python::list getXPixBuffer_VC_Py()const;
        boost::python::list getXPixBuffer_Py1(const std::string& cam)const;
        boost::python::list getXPixBuffer_Py2()const;
        boost::python::list getYPixBuffer_VC_Py()const;
        boost::python::list getYPixBuffer_Py1(const std::string& cam)const;
        boost::python::list getYPixBuffer_Py2()const;
        boost::python::list getSigXPixBuffer_VC_Py()const;
        boost::python::list getSigXPixBuffer_Py1(const std::string& cam)const;
        boost::python::list getSigXPixBuffer_Py2()const;
        boost::python::list getSigYPixBuffer_VC_Py()const;
        boost::python::list getSigYPixBuffer_Py1(const std::string& cam)const;
        boost::python::list getSigYPixBuffer_Py2()const;
        boost::python::list getSigXYPixBuffer_VC_Py()const;
        boost::python::list getSigXYPixBuffer_Py1(const std::string& cam)const;
        boost::python::list getSigXYPixBuffer_Py2()const;
        boost::python::list getPixelValues_VC_Py()const;
        boost::python::list getPixelValues_Py1(const std::string& cam)const;
        boost::python::list getPixelValues_Py2()const;
        boost::python::list getPixelValuesBuffer_VC_Py()const;
        boost::python::list getPixelValuesBuffer_Py1(const std::string& cam)const;
        boost::python::list getPixelValuesBuffer_Py2()const;
        boost::python::list getFastImage_VC_Py()const;
        boost::python::list getFastImage_Py1(const std::string& cam)const;
        boost::python::list getFastImage_Py2()const;
        boost::python::list getFastImage2D_VC_Py()const;
        boost::python::list getFastImage2D_Py1(const std::string& cam)const;
        boost::python::list getFastImage2D_Py2()const;
        boost::python::list getSumIntensityBuffer_VC_Py()const;
        boost::python::list getSumIntensityBuffer_Py1(const std::string& cam)const;
        boost::python::list getSumIntensityBuffer_Py2()const;
        boost::python::list getAvgIntensityBuffer_VC_Py()const;
        boost::python::list getAvgIntensityBuffer_Py1(const std::string& cam)const;
        boost::python::list getAvgIntensityBuffer_Py2()const;
#endif
        double getX_VC()const;
        double getX(const std::string& cam)const;
        double getX()const;
        double getY_VC()const;
        double getY(const std::string& cam)const;
        double getY()const;
        double getSigX_VC()const;
        double getSigX(const std::string& cam)const;
        double getSigX()const;
        double getSigY_VC()const;
        double getSigY(const std::string& cam)const;
        double getSigY()const;
        double getSigXY_VC()const;
        double getSigXY(const std::string& cam)const;
        double getSigXY()const;
        double getXPix_VC()const;
        double getXPix(const std::string& cam)const;
        double getXPix()const;
        double getYPix_VC()const;
        double getYPix(const std::string& cam)const;
        double getYPix()const;
        double getSigXPix_VC()const;
        double getSigXPix(const std::string& cam)const;
        double getSigXPix()const;
        double getSigYPix_VC()const;
        double getSigYPix(const std::string& cam)const;
        double getSigYPix()const;
        double getSigXYPix_VC()const;
        double getSigXYPix(const std::string& cam)const;
        double getSigXYPix()const;
        size_t getBufferCount_VC()const;
        size_t getBufferCount(const std::string& cam)const;
        size_t getBufferCount()const;
        size_t getBufferMaxCount_VC()const;
        size_t getBufferMaxCount(const std::string& cam)const;
        size_t getBufferMaxCount()const;
        int getStepSize_VC()const;
        int getStepSize(const std::string& cam)const;
        int getStepSize()const;
//-----------------------------------------------------------------------------------------
//  __   ___ ___ ___  ___  __   __
// / _` |__   |   |  |__  |__) /__`
// \__> |___  |   |  |___ |  \ .__/
//
//-----------------------------------------------------------------------------------------
        const cameraStructs::analysis_mask& getMaskObj(const std::string& name)const;
        const cameraStructs::analysis_mask& getMaskObj_VC()const;
        const cameraStructs::analysis_mask& getMaskObj()const;
        const cameraStructs::analysis_data& getAnalysisObj(const std::string& name)const;
        const cameraStructs::analysis_data& getAnalysisObj_VC()const;
        const cameraStructs::analysis_data& getAnalysisObj()const;
        const cameraStructs::fast_image& getImageObj(const std::string& name)const;
        const cameraStructs::fast_image& getImageObj_VC()const;
        const cameraStructs::fast_image& getImageObj()const;
        const cameraStructs::camera_image_data& getImageDataObj(const std::string& name)const;
        const cameraStructs::camera_image_data& getImageDataObj_VC()const;
        const cameraStructs::camera_image_data& getImageDataObj()const;
        const cameraStructs::clara_DAQ& getClaraDAQObj(const std::string& name)const;
        const cameraStructs::clara_DAQ& getClaraDAQObj_VC()const;
        const cameraStructs::clara_DAQ& getClaraDAQObj()const;
        const cameraStructs::cameraObject& getCameraObj(const std::string& name)const;
        const cameraStructs::cameraObject& getCameraObj_VC()const;
        const cameraStructs::cameraObject& getCameraObj()const;
        const cameraStructs::camera_state& getStateObj(const std::string& name)const;
        const cameraStructs::camera_state& getStateObj_VC()const;
        const cameraStructs::camera_state& getStateObj()const;
//-----------------------------------------------------------------------------------------
//  __   ___ ___ ___  ___  __   __
// /__` |__   |   |  |__  |__) /__`
// .__/ |___  |   |  |___ |  \ .__/
//
//-----------------------------------------------------------------------------------------
        bool setMaskX_VC(int x);
        bool setMaskX(int x,const std::string& cam);
        bool setMaskX(int x);
        bool setMaskY_VC(int x);
        bool setMaskY(int x,const std::string& cam);
        bool setMaskY(int x);
        bool setMaskXrad_VC(int x);
        bool setMaskXrad(int x,const std::string& cam);
        bool setMaskXrad(int x);
        bool setMaskYrad_VC(int x);
        bool setMaskYrad(int x,const std::string& cam);
        bool setMaskYrad(int x);
        bool setMask_VC(int x,int y,int xr,int yr);
        bool setMask(int x,int y,int xr,int yr,const std::string& cam);
        bool setMask(int x,int y,int xr,int yr);
        bool setMask_VC(const std::vector<int>& v);
        bool setMask(const std::vector<int>& v,const std::string& cam);
        bool setMask(const std::vector<int>& v);
#ifdef BUILD_DLL
        bool setMask_VC_Py(const boost::python::list& v);
        bool setMask_Py1(const boost::python::list v,const std::string& cam);
        bool setMask_Py2(const boost::python::list v);
#endif
        bool setBackground_VC();
        bool setBackground(const std::string& cam);
        bool setBackground();
        void setBufferMaxCount_VC(const size_t s);
        void setBufferMaxCount(const size_t s,const std::string& cam);
        void setBufferMaxCount(const size_t s);
        bool setStepSize_VC(unsigned short step);
        bool setStepSize(unsigned short step,const std::string& cam);
        bool setStepSize(unsigned short step);
        bool setCenterXPixel_VC(unsigned short  xC);
        bool setCenterXPixel(unsigned short  xC,const std::string& cam);
        bool setCenterXPixel(unsigned short  xC);
        bool setCenterYPixel_VC(unsigned short  yC);
        bool setCenterYPixel(unsigned short  yC,const std::string& cam);
        bool setCenterYPixel(unsigned short  yC);
        bool setPixMM_VC(const double pmm);
        bool setPixMM(const double pmm,const std::string& cam);
        bool setPixMM(const double pmm);
        /*
            the main interface is held as a pointer because sometimes
            this class instantiates it
            and sometimes it spassed on on construction
        */
        cameraBase*  camBase;
        bool shoudlDeletecamBase;
    protected:

    private:


//        boost::python::numpy::ndarray getXBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getXBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getXBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getYBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getYBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getYBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getSigXBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getSigXBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getSigXBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getSigYBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getSigYBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getSigYBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getSigXYBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getSigXYBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getSigXYBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getXPixBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getXPixBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getXPixBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getYPixBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getYPixBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getYPixBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getSigXPixBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getSigXPixBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getSigXPixBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getSigYPixBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getSigYPixBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getSigYPixBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getSigXYPixBuffer_VC_NumPy();
//        boost::python::numpy::ndarray getSigXYPixBuffer_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getSigXYPixBuffer_NumPy2();
//
//        boost::python::numpy::ndarray getPixelValues_VC_NumPy();
//        boost::python::numpy::ndarray getPixelValues_NumPy1(const std::string& cam);
//        boost::python::numpy::ndarray getPixelValues_NumPy2();
//
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
////        boost::python::numpy::ndarray getPixelValuesBuffer_VC_NumPy();
////        boost::python::numpy::ndarray getPixelValuesBuffer_NumPy1(const std::string& cam);
////        boost::python::numpy::ndarray getPixelValuesBuffer_NumPy2();
//
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
////        boost::python::numpy::ndarray getFastImage_VC_NumPy();
////        boost::python::numpy::ndarray getFastImage_NumPy1(const std::string& cam);
////        boost::python::numpy::ndarray getFastImage_NumPy2();
////
////        boost::python::numpy::ndarray getSumIntensityBuffer_VC_NumPy();
////        boost::python::numpy::ndarray getSumIntensityBuffer_NumPy1(const std::string& cam);
////        boost::python::numpy::ndarray getSumIntensityBuffer_NumPy2();
////
////        boost::python::numpy::ndarray getAvgIntensityBuffer_VC_NumPy();
////        boost::python::numpy::ndarray getAvgIntensityBuffer_NumPy1(const std::string& cam);
////        boost::python::numpy::ndarray getAvgIntensityBuffer_NumPy2();

};
#endif // CAMERACONTROLLERBASE_H
