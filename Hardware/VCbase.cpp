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
//  Last edit:   19-03-2018
//  FileName:    VCbase.cpp
//  Description:
//
//
//*/
#include "VCbase.h"
#include "configDefinitions.h"

VCbase::VCbase():
VCbase(UTL::UNKNOWN_NAME)
{}
//______________________________________________________________________________
VCbase::VCbase(const std::string& name):
my_name(name),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
shouldShowDebugMessage(false),  //default is quiet mode
shouldShowMessage(false)        //default is quiet mode
{}
//______________________________________________________________________________
VCbase::~VCbase()
{}
//______________________________________________________________________________
void VCbase::setQuiet()
{
    std::cout << my_name << " Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCbase::setVerbose()
{
    std::cout << my_name << " Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCbase::setMessage()
{
    std::cout << my_name << " Messages Only Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCbase::setDebugMessage()
{
    std::cout << my_name << " DebugMessages Only Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
