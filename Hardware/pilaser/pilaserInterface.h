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
//  FileName:    pilaserInterface.h
//  Description:
//
//
//*/
#ifndef _PI_LASER_INTERFACE_H
#define _PI_LASER_INTERFACE_H
// djs
#include "interface.h"
#include "pilaserStructs.h"
#include "structs.h"
#include "pilaserConfigReader.h"
#include "virtualCathodeConfigReader.h"
//stl
#include <vector>
#include <string>
#include <atomic>
#include <map>
//______________________________________________________________________________
class pilaserInterface : public interface
{
    public:

        typedef std::vector<bool> vec_b;
        typedef std::vector< std::string > vec_s;
        typedef std::vector<double> vec_d;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE> IlockMap1;
        typedef std::map<HWC_ENUM::ILOCK_NUMBER,std::string> IlockMap2;

        //pilaserInterface::pilaserInterface();
        pilaserInterface(bool& show_messages,
                         bool& show_debug_messages,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const std::string& vcMirrorConfig,
                         const std::string& vcDataConfig,
                         const std::string& pilaserConfig
                         );
        pilaserInterface& pilaserInterface::operator= ( const pilaserInterface& other ) = delete;
        ~pilaserInterface();

        int  setHWP(const double value);
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

        const pilaserStructs::pilMirrorObject& getpilMirrorObjConstRef() const;
        const pilaserStructs::pilaserObject&   getPILObjConstRef() const;
        const pilaserStructs::virtualCathodeDataObject&   getVCDataObjConstRef() const;

        double getHpos() const;
        double getVpos() const;

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

        bool isBufferFull();
        bool isBufferNotFull();

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

        bool setHpos(const double value);
        bool setVpos(const double value);

        double getHstep() const;
        double getVstep() const;
        bool setHstep(const double value);
        bool setVstep(const double value);

        bool moveH();
        bool moveV();

        void setBufferSize(const size_t s);
        size_t getBufferCount();
        size_t getBufferSize();
        void clearBuffer();

        bool isVCMirror_PV(const pilaserStructs::PILASER_PV_TYPE& pv)const;
    private:

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string& pvRoot, pilaserStructs::pvStruct& pv);
        void startMonitors();

        void killMonitor(pilaserStructs::monitorStruct* ms);
//
//
        std::vector<pilaserStructs::monitorStruct*> continuousMonitorStructs;
//        // all EPICS callbacks route here
        void addToContinuousMonitorStructs(const pilaserStructs::PILASER_PV_TYPE pv, const pilaserStructs::pvStruct& st);
        static void staticEntryMonitor(const event_handler_args args);
        void updateValue(const event_handler_args args,pilaserStructs::PILASER_PV_TYPE pv);

        pilaserStructs::pilaserObject pilaser;
        pilaserConfigReader configReader;

        virtualCathodeConfigReader vcConfigReader;

        void updateAnalysisBuffers();

        void addToBuffer(const double val,std::deque<double>& buffer);
        void updatePixelResults(const event_handler_args& args);

        //bool isVCMirror_PV(const pilaserStructs::PILASER_PV_TYPE& pv)const;
        /*
            all client mirrro set functions route to here
        */
        bool setValue(pilaserStructs::pvStruct& pvs,const double value);
        bool move(chtype& cht, chid& chi, const double val, const char* m1, const char* m2);
};
//______________________________________________________________________________
#endif // _PI_LASER_INTERFACE_H
