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
//  Last edit:   29-03-2018
//  FileName:    pilaserController.h
//  Description:
//
//
//*/
#ifndef _PI_LASER_CONTROLLER_H_
#define _PI_LASER_CONTROLLER_H_
// stl includes
#include <string>
#include <vector>
#include <map>
// project includes
//#include "shutterController.h"
//#include "pilaserMirrorController.h"
#include "virtualCathodeController.h"
#include "pilaserInterface.h"
#include "controller.h"
// boost.python
//______________________________________________________________________________
class pilaserController : public controller
{
    public:
        //pilaserController();
        pilaserController(bool& show_messages,
                          bool& show_debug_messages,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const std::string& name,
                          const std::string& pilaserConf,
                          const std::string& vcAnalysisConf,
                          const std::string& piLaserMirrorConf
                         );
        pilaserController& pilaserController::operator= ( const pilaserController& other ) = delete;
        ~pilaserController();
      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);

        int    setHWP(const double value);
        double getHWP();

        bool setCharge(const double value);
        bool setIntensity(const double valueL);
        double getIntensity() const;
        std::string getName() const;
        HWC_ENUM::STATE getStatus() const;
        double getStabilisationStatus() const;
        bool isOn() const;
        bool isOff() const;
        bool isStabilisationOff() const;
        bool isStabilisationOn() const;
        bool disableStabilisation();
        bool enableStabilisation();

        void clearRunningValues();
        std::vector<int> getFastImage();
        boost::python::list getFastImage_Py();

        // laser mirror functions
        double getHpos() const;
        double getVpos() const;
        bool setHpos(double value);
        //bool setHpos(int value);
        bool setVpos(double value);
        //bool setVpos(int value);
        double getHstep() const;
        double getVstep() const;
        bool setHstep(double value);
        bool setVstep(double value);
        bool moveH();
        bool moveV();

        double getX()const;
        double getY()const;
        double getSigX()const;
        double getSigY()const;
        double getSigXY()const;
        double getXPix()const;
        double getYPix()const;
        double getSigXPix()const;
        double getSigYPix()const;
        double getSigXYPix()const;
        double getQ()const;

        std::deque<double> getXBuffer()const;
        std::deque<double> getYBuffer()const;
        std::deque<double> getSigXBuffer()const;
        std::deque<double> getSigYBuffer()const;
        std::deque<double> getSigXYBuffer()const;
        std::deque<double> getXPixBuffer()const;
        std::deque<double> getYPixBuffer()const;
        std::deque<double> getSigXPixBuffer()const;
        std::deque<double> getSigYPixBuffer()const;
        std::deque<double> getSigXYPixBuffer()const;
        std::deque<double> getQBuffer()const;

        std::vector<double> getPixelValues()const;
        std::deque<std::vector<double>> getPixelValuesBuffer()const;

        void clearBuffer();
        bool isBufferFull();
        bool isBufferNotFull();
        void setBufferSize(const size_t s);
        size_t getBufferSize();
        size_t getBufferCount();

    #ifdef BUILD_DLL
        boost::python::list getPixelValues_Py()const;
        boost::python::list getPixelValuesBuffer_Py()const;
        boost::python::list getXBuffer_Py()const;
        boost::python::list getYBuffer_Py()const;
        boost::python::list getSigXBuffer_Py()const;
        boost::python::list getSigYBuffer_Py()const;
        boost::python::list getSigXYBuffer_Py()const;
        boost::python::list getXPixBuffer_Py()const;
        boost::python::list getYPixBuffer_Py()const;
        boost::python::list getSigXPixBuffer_Py()const;
        boost::python::list getSigYPixBuffer_Py()const;
        boost::python::list getSigXYPixBuffer_Py()const;
        boost::python::list getQBuffer_Py()const;
    #endif // BUILD_DLL


        const pilaserStructs::pilMirrorObject& getpilMirrorObjConstRef() const;
        const pilaserStructs::pilaserObject&   getPILObjConstRef() const;
        const pilaserStructs::virtualCathodeDataObject&   getVCDataObjConstRef() const;

    protected:
    private:
        pilaserInterface  localInterface;
        const std::string name, localMirrorName, localInterfaceName,
                          localVirtualCathodeName;
};
//______________________________________________________________________________
#endif // _PI_LASER_CONTROLLER_H_
