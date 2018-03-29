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
// stl
#include <string>
#include <vector>
#include <map>
// project
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
        pilaserController(bool* show_messages,
                          bool* show_debug_messagese,
                          const bool startVirtualMachine,
                          const bool shouldStartEPICs,
                          const std::string& name,
                          const std::string& pilaserConf,
                          const std::string& vcAnalysisConf,
                          const std::string& piLaserMirrorConf,
                          const std::string& piLaserShutterConf
                         );
        ~pilaserController( );
      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT( double val );
        //std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> getILockStates(const std::string& name);
        //std::map<HWC_ENUM::ILOCK_NUMBER,std::string> getILockStatesStr(const std::string& name);
      // any functions that return a map need a wrapper to convert to a python dictionary (?)
      // (we need the functions that return std::map types when building c++ applications)
//#ifdef BUILD_DLL
//        boost::python::dict getILockStates_Py( std::string magName );
//        boost::python::dict getILockStatesStr_Py( std::string magName );
//        boost::python::dict getMagPSUStateDefinition();
//#endif // BUILD_DLL
        // getters, not sure of type for these parameters (or if they will exist...)
//        double getHpos();
//        double getVpos();
//        double getIntensity();
//        // setters, not sure of type for these parameters (or if they will exist...)
//        bool setHpos(double value);
//        bool setHpos(int value);
//        bool setVpos(double value);
//        bool setVpos(int value);
//        bool setIntensity(double value);
//        bool setIntensity(int value);
//        const pilaserStructs::pilaserObject& getPILObjConstRef();
//    protected:
//    private:
//        void initialise();
////        const bool shouldStartEPICs;
        pilaserMirrorController   localMirror;
        shutterController         localShutter;
        //virtualCathodeController  localVirtualCathode;
        pilaserInterface          localInterface;
        const std::string name, localMirrorName, localInterfaceName,
                          localShutterName, localVirtualCathodeName;
};
//______________________________________________________________________________
#endif // _PI_LASER_CONTROLLER_H_
