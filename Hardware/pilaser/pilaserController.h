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
#include "shutterController.h"
#include "pilaserMirrorController.h"
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
                          const std::string& piLaserMirrorConf,
                          const std::string& piLaserShutterConf
                         );
        ~pilaserController();
      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);

        // all functions for shutter
        bool closeAndWait(const std::string& name, const time_t waitTime);
        bool openAndWait (const std::string& name, const time_t waitTime);
        bool isClosed(const std::string& name) const;
        bool isOpen  (const std::string& name) const;
        void close(const std::string& name);
        void open (const std::string& name);
        std::vector<std::string> getShutterNames()const;
        const shutterStructs::shutterObject&
            getShutterObjConstRef(const std::string& name )const;
        shutterStructs::SHUTTER_STATE getShutterState(const std::string& name);
        std::string getShutterStateStr(const std::string& name );



        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string& name)const;

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
        const pilaserStructs::pilaserObject& getPILObjConstRef() const;

        // laser mirror functions
        double getHpos() const;
        double getVpos() const;
        bool setHpos(double value);
        bool setHpos(int value);
        bool setVpos(double value);
        bool setVpos(int value);
        double getHstep() const;
        double getVstep() const;
        bool setHstep(double value);
        bool setVstep(double value);
        bool moveH();
        bool moveV();
        const pilaserMirrorStructs::pilMirrorObject& getpilMirrorObjConstRef() const;

#ifdef BUILD_DLL


        boost::python::list getShutterNames_Py()const;

#endif


    protected:
    private:

        pilaserMirrorController   localMirror;
        shutterController         localShutter;
        virtualCathodeController  localVirtualCathode;
        pilaserInterface          localInterface;
        const std::string name, localMirrorName, localInterfaceName,
                          localShutterName, localVirtualCathodeName;
};
//______________________________________________________________________________
#endif // _PI_LASER_CONTROLLER_H_
