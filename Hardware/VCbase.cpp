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
bool VCbase::withEPICS = true;
bool VCbase::withoutEPICS = false;
bool VCbase::withoutVM = false;
bool VCbase::withVM = true;
HWC_ENUM::MACHINE_AREA VCbase::VELA_INJ = HWC_ENUM::MACHINE_AREA::VELA_INJ;
HWC_ENUM::MACHINE_AREA VCbase::VELA_BA1 = HWC_ENUM::MACHINE_AREA::VELA_BA1;
HWC_ENUM::MACHINE_AREA VCbase::VELA_BA2 = HWC_ENUM::MACHINE_AREA::VELA_BA2;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_PH1 = HWC_ENUM::MACHINE_AREA::CLARA_PH1;
HWC_ENUM::MACHINE_AREA VCbase::USER = HWC_ENUM::MACHINE_AREA::USER;
HWC_ENUM::MACHINE_AREA VCbase::UNKNOWN_AREA = HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_INJ = HWC_ENUM::MACHINE_AREA::CLARA_INJ;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_2_VELA = HWC_ENUM::MACHINE_AREA::CLARA_2_VELA;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_S01 = HWC_ENUM::MACHINE_AREA::CLARA_S01;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_S02 = HWC_ENUM::MACHINE_AREA::CLARA_S02;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_L01 = HWC_ENUM::MACHINE_AREA::CLARA_L01;
HWC_ENUM::MACHINE_AREA VCbase::RF_GUN = HWC_ENUM::MACHINE_AREA::RF_GUN;
HWC_ENUM::MACHINE_AREA VCbase::RF_L01 = HWC_ENUM::MACHINE_AREA::RF_L01;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_2_BA1 = HWC_ENUM::MACHINE_AREA::CLARA_2_BA1;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_2_BA2 = HWC_ENUM::MACHINE_AREA::CLARA_2_BA2;
HWC_ENUM::MACHINE_AREA VCbase::CLARA_2_BA1_BA2 = HWC_ENUM::MACHINE_AREA::CLARA_2_BA1_BA2;
HWC_ENUM::MACHINE_MODE VCbase::OFFLINE = HWC_ENUM::MACHINE_MODE::OFFLINE;
HWC_ENUM::MACHINE_AREA VCbase::CLARA = HWC_ENUM::MACHINE_AREA::CLARA;
HWC_ENUM::MACHINE_AREA VCbase::VELA = HWC_ENUM::MACHINE_AREA::VELA;
//______________________________________________________________________________
HWC_ENUM::MACHINE_MODE VCbase::VIRTUAL = HWC_ENUM::MACHINE_MODE::VIRTUAL;
HWC_ENUM::MACHINE_MODE VCbase::PHYSICAL = HWC_ENUM::MACHINE_MODE::PHYSICAL;
HWC_ENUM::MACHINE_MODE VCbase::UNKNOWN_MACHINE_MODE = HWC_ENUM::MACHINE_MODE::UNKNOWN_MACHINE_MODE;
//______________________________________________________________________________
VCbase::VCbase(const std::string& name):
shouldShowDebugMessage(false), //default is quiet mode
shouldShowMessage(false),        //default is quiet mode
my_name(name)
//withEPICS(true),
//withoutEPICS(false),
//withoutVM(false),
//withVM(true),
//shouldShowDebugMessage(false),  //default is quiet mode
//shouldShowMessage(false),        //default is quiet mode
//VELA_INJ(HWC_ENUM::MACHINE_AREA::VELA_INJ),
//VELA_BA1(HWC_ENUM::MACHINE_AREA::VELA_BA1),
//VELA_BA2(HWC_ENUM::MACHINE_AREA::VELA_BA2),
//CLARA_PH1(HWC_ENUM::MACHINE_AREA::CLARA_PH1),
//USER(HWC_ENUM::MACHINE_AREA::USER),
//UNKNOWN_AREA(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA),
//CLARA_INJ(HWC_ENUM::MACHINE_AREA::CLARA_INJ),
//CLARA_2_VELA(HWC_ENUM::MACHINE_AREA::CLARA_2_VELA),
//CLARA_S01(HWC_ENUM::MACHINE_AREA::CLARA_S01),
//CLARA_S02(HWC_ENUM::MACHINE_AREA::CLARA_S02),
//CLARA_L01(HWC_ENUM::MACHINE_AREA::CLARA_L01),
//RF_GUN(HWC_ENUM::MACHINE_AREA::RF_GUN),
//RF_L01(HWC_ENUM::MACHINE_AREA::RF_L01),
//OFFLINE(HWC_ENUM::MACHINE_MODE::OFFLINE),
//VIRTUAL(HWC_ENUM::MACHINE_MODE::VIRTUAL),
//PHYSICAL(HWC_ENUM::MACHINE_MODE::PHYSICAL),
//UNKNOWN_MACHINE_MODE(HWC_ENUM::MACHINE_MODE::UNKNOWN_MACHINE_MODE)
{}
//______________________________________________________________________________
VCbase::~VCbase()
{}
//______________________________________________________________________________
void VCbase::setQuiet()
{
    std::cout <<my_name <<" Quiet Mode Set." <<std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
    updateMessageStates();
}
//______________________________________________________________________________
void VCbase::setVerbose()
{
    std::cout <<my_name <<" Verbose Mode Set." <<std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
    updateMessageStates();
}
//______________________________________________________________________________
void VCbase::setMessage()
{
    std::cout <<my_name <<" Messages Only Mode Set." <<std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
    updateMessageStates();
}
//______________________________________________________________________________
void VCbase::setDebugMessage()
{
    std::cout <<my_name <<" DebugMessages Only Mode Set." <<std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
    updateMessageStates();
}
//______________________________________________________________________________
