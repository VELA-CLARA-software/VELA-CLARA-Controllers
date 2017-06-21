#include "VCllrf.h"
#include <iostream>

VCllrf::VCllrf():
GUN_LLRF_Controller_Obj(nullptr),
L01_LLRF_Controller_Obj(nullptr),
CLARA_LRRG_LLRF_CONFIG(UTL::CONFIG_PATH + UTL::CLARA_LRRG_LLRF_CONFIG),
CLARA_L01_LLRF_CONFIG(UTL::CONFIG_PATH + UTL::CLARA_L01_LLRF_CONFIG),
VELA_HRRG_LLRF_CONFIG (UTL::CONFIG_PATH + UTL::VELA_HRRG_LLRF_CONFIG),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
shouldShowDebugMessage(false),//default is quiet mode
shouldShowMessage(false)//default is quiet mode
{
    std::cout << "Instantiated a VCllrf in Quiet Mode" << std::endl;
    //ctor
}
//______________________________________________________________________________
void VCllrf::setQuiet()
{
    std::cout << "VCllrf Quiet Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = false;
}
//______________________________________________________________________________
void VCllrf::setVerbose()
{
    std::cout << "VCllrf Verbose Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCllrf::setMessage()
{
    std::cout << "VCllrf Message Mode Set." << std::endl;
    shouldShowDebugMessage = false;
    shouldShowMessage = true;
}
//______________________________________________________________________________
void VCllrf::setDebugMessage()
{
    std::cout << "VCllrf DebugMessage Mode Set." << std::endl;
    shouldShowDebugMessage = true;
    shouldShowMessage = false;
}
//______________________________________________________________________________
VCllrf::~VCllrf()
{    //dtor
    killGun();
    if(L01_LLRF_Controller_Obj)
    {
        delete L01_LLRF_Controller_Obj;
               L01_LLRF_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
void VCllrf::killGun()
{
    if(GUN_LLRF_Controller_Obj)
    {
        delete GUN_LLRF_Controller_Obj;
               GUN_LLRF_Controller_Obj = nullptr;
    }
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_CLARA_LRRG_LLRF_Controller()
{
    std::cout << "Creating virtual_CLARA_LRRG_LLRF_Controller object" << std::endl;
    return getController(withVM,withEPICS,llrfStructs::CLARA_LRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_CLARA_LRRG_LLRF_Controller()
{
    std::cout << "Creating offline_CLARA_LRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withoutEPICS,llrfStructs::CLARA_LRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_CLARA_LRRG_LLRF_Controller()
{
    std::cout << "Creating physical_CLARA_LRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withEPICS,llrfStructs::CLARA_LRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_CLARA_HRRG_LLRF_Controller()
{
    std::cout << "Creating virtual_CLARA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withVM,withEPICS,llrfStructs::CLARA_HRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_CLARA_HRRG_LLRF_Controller()
{
    std::cout << "Creating offline_CLARA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withoutEPICS,llrfStructs::CLARA_HRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_CLARA_HRRG_LLRF_Controller()
{
    std::cout << "Creating physical_CLARA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withEPICS,llrfStructs::CLARA_HRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_VELA_LRRG_LLRF_Controller()
{
    std::cout << "Creating virtual_VELA_LRRG_LLRF_Controller object" << std::endl;
    return getController(withVM,withEPICS,llrfStructs::VELA_LRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_VELA_LRRG_LLRF_Controller()
{
    std::cout << "Creating offline_VELA_LRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withoutEPICS,llrfStructs::VELA_LRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_VELA_LRRG_LLRF_Controller()
{
    std::cout << "Creating physical_VELA_LRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withEPICS,llrfStructs::VELA_LRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_VELA_HRRG_LLRF_Controller()
{
    std::cout << "Creating virtual_VELA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withVM,withEPICS,llrfStructs::VELA_HRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_VELA_HRRG_LLRF_Controller()
{
    std::cout << "Creating offline_VELA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withoutEPICS,llrfStructs::VELA_HRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_VELA_HRRG_LLRF_Controller()
{
    std::cout << "Creating physical_VELA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withEPICS,llrfStructs::VELA_HRRG);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::virtual_L01_LLRF_Controller()
{
    std::cout << "Creating physical_VELA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withVM,withEPICS,llrfStructs::L01);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::physical_L01_LLRF_Controller()
{
    std::cout << "Creating physical_VELA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withEPICS,llrfStructs::L01);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::offline_L01_LLRF_Controller()
{
    std::cout << "Creating physical_VELA_HRRG_LLRF_Controller object" << std::endl;
    return getController(withoutVM,withoutEPICS,llrfStructs::L01);
}
//______________________________________________________________________________
liberaLLRFController& VCllrf::getController(bool shouldVM, bool shouldEPICS,llrfStructs::LLRF_TYPE llrfType)
{
    bool createobject = true;
    if( isaGUN(llrfType)&&GUN_LLRF_Controller_Obj )
    {
        std::cout << "A Gun object already exists. Please use killGun() to delete current Gun first." << std::endl;
        createobject = false;
    }
    if( createobject )
    {
        switch(llrfType)
        {
//            case llrfStructs::CLARA_HRRG:
//                GUN_LLRF_Controller_Obj =
//                    new liberaLLRFController(shouldShowMessage,shouldShowDebugMessage,CLARA_HRRG_LLRF_CONFIG,
//                               shouldVM,shouldEPICS,llrfType);
//                break;
            case llrfStructs::CLARA_LRRG:
                GUN_LLRF_Controller_Obj =
                    new liberaLLRFController(shouldShowMessage,shouldShowDebugMessage,CLARA_LRRG_LLRF_CONFIG,
                               shouldVM,shouldEPICS,llrfType);
                break;
//            case llrfStructs::VELA_HRRG:
//                GUN_LLRF_Controller_Obj =
//                    new liberaLLRFController(shouldShowMessage,shouldShowDebugMessage,VELA_HRRG_LLRF_CONFIG,
//                               shouldVM,shouldEPICS,llrfType);
//                break;
//            case llrfStructs::VELA_LRRG:
//                GUN_LLRF_Controller_Obj =
//                    new liberaLLRFController(shouldShowMessage,shouldShowDebugMessage,VELA_LRRG_LLRF_CONFIG,
//                               shouldVM,shouldEPICS,llrfType);
//                break;
            case llrfStructs::L01:
                L01_LLRF_Controller_Obj =
                    new liberaLLRFController(shouldShowMessage,shouldShowDebugMessage,CLARA_L01_LLRF_CONFIG,
                               shouldVM,shouldEPICS,llrfType);
                return *L01_LLRF_Controller_Obj;
                break;
        }
    }
    return *GUN_LLRF_Controller_Obj;
}
//______________________________________________________________________________
bool VCllrf::isaGUN(llrfStructs::LLRF_TYPE llrfType)
{
    bool r = true;
    switch(llrfType)
    {
        case llrfStructs::L01:
            r = false;
            break;
    }
    return r;
}













