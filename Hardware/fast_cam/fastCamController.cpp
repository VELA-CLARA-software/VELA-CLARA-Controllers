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
//  Last edit:   16-05-2018
//  FileName:    fastCamController.cpp
//  Description:
//
//
//
//
//*/
// project includes
#include "fastCamController.h"
// stl
#include <iostream>
//______________________________________________________________________________
fastCamController::fastCamController(bool& show_messages,
                         bool& show_debug_messages,
                         const std::string &conf,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const HWC_ENUM::MACHINE_AREA myMachineArea,
                         const std::string& name
                         ):
fastCamController(show_messages,
                  show_debug_messages,
                  conf,
                  UTL::UNKNOWN_STRING,
                  startVirtualMachine,
                  shouldStartEPICs,
                  myMachineArea,
                  name)
{

}
//______________________________________________________________________________
fastCamController::fastCamController(bool& show_messages,
                         bool& show_debug_messages,
                         const std::string &velaCamConf,
                         const std::string &claraCamConf,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const HWC_ENUM::MACHINE_AREA myMachineArea,
                         const std::string& name
                         ):
controller(show_messages,
           show_debug_messages,
           HWC_ENUM::CONTROLLER_TYPE::FAST_CAM,name),
localInterface(velaCamConf, claraCamConf, startVirtualMachine, show_messages, show_debug_messages, shouldStartEPICs, myMachineArea)
{}
//______________________________________________________________________________
fastCamController::~fastCamController()
{}
//______________________________________________________________________________
double fastCamController::get_CA_PEND_IO_TIMEOUT()const
{
    return 1.0;
}
//______________________________________________________________________________
void fastCamController::set_CA_PEND_IO_TIMEOUT(double val)
{

}
//______________________________________________________________________________
std::vector<double> fastCamController::getFastImage(const std::string& name)
{
    return localInterface.getFastImage(name);
}
//______________________________________________________________________________
boost::python::list fastCamController::getFastImage_Py(const std::string& name)
{
    return toPythonList(getFastImage(name));
}



