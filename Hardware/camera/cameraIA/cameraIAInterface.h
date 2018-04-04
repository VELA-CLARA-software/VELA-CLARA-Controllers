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

#ifndef CAM_IA_INTERFACE_H
#define CAM_IA_INTERFACE_H

#include <string>
#include <vector>
#include <atomic>
#include <map>

#include "cameraStructs.h"
#include "cameraInterface.h"

#include "cameraIAConfigReader.h"

using namespace cameraStructs;

class cameraIAInterface : public cameraInterface
{
    public:
        cameraIAInterface::cameraIAInterface();
        cameraIAInterface(const std::string &Conf,
                          const bool startVirtualMachine,
                          const bool* show_messages_ptr,
                          const bool* show_debug_messages_ptr,
                          const bool shouldStartEPICs,
                          const HWC_ENUM::MACHINE_AREA myMachineArea);
        ~cameraIAInterface();

        ///Functions Accessible to Python Controller///
        bool setBackground();
        bool useBackground(const bool run);
        bool setStepSize(const int step);
        bool setMaskX(const int x);
        bool setMaskY(const int y);
        bool setMaskXRad(const int xRad);
        bool setMaskYRad(const int yRad);
        bool setCenterXPixel(const int xC);
        bool setCenterYPixel(const int yC);
        bool setPixMM(const double pmm);
        bool useNPoint(const bool run);
        bool startAnalysis();
        bool stopAnalysis();
        const cameraObject &getCamIAObjConstRef(const std::string &camName);
        const cameraObject &getSelectedIARef();
        const cameraObject &getVCIARef();

    protected:

    private:
        const HWC_ENUM::MACHINE_AREA myMachineArea;
        const bool shouldStartEPICs;
        void initialise();
        cameraIAConfigReader configReader; /// class member so we can pass in file path in ctor
        bool initObjects();
        void initChids();
        void startMonitors();
        void killMonitor( cameraStructs::monitorDAQStruct * ms );
        static void staticEntryIAMonitor(const event_handler_args args);
        void updateSelectedOrVC(const std::string cameraName);

        void updateState(const unsigned short value,const std::string&cameraName);
        void updateAcquiring(const unsigned short value,const std::string &cameraName);
        void updateX(const double value,const std::string& cameraName);
        void updateY(const double value,const std::string& cameraName);
        void updateSigmaX(const double value,const std::string& cameraName);
        void updateSigmaY(const double value,const std::string& cameraName);
        void updateCovXY(const double value,const std::string& cameraName);
        void updateXPix(const unsigned long value,const std::string& cameraName);
        void updateYPix(const unsigned long value,const std::string& cameraName);
        void updateSigmaXPix(const unsigned long value,const std::string& cameraName);
        void updateSigmaYPix(const unsigned long value,const std::string& cameraName);
        void updateCovXYPix(const unsigned long value,const std::string& cameraName);
        void updateXRad(const unsigned long value,const std::string&cameraName);
        void updateYRad(const unsigned long value,const std::string&cameraName);
        void updateXCenterPix(const unsigned long value,const std::string&cameraName);
        void updateYCenterPix(const unsigned long value,const std::string&cameraName);
        void updateBitDepth(const unsigned long value,const std::string&cameraName);
        //void updateImageWidth(const unsigned long value,const std::string&cameraName);
        //void updateImageHeight(const unsigned long value,const std::string&cameraName);
        void updateXMask(const unsigned long value,const std::string&cameraName);
        void updateYMask(const unsigned long value,const std::string&cameraName);
        void updateSummedIntensity(const unsigned long value,const std::string&cameraName);
        void updateAnalyseState(const unsigned short value,const std::string&cameraName);
        void updateUseBkgrnd(const unsigned short value,const std::string&cameraName);
        void updateUseNPoint(const unsigned short value,const std::string&cameraName);

};
#endif // CAM_IA_INTERFACE_H
