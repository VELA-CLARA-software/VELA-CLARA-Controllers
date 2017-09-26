#include "VCBPMs.h"
// stl

#include <iostream>

VCBPMs::VCBPMs():
virtual_VELA_INJ_BPM_Controller_Obj(nullptr),
offline_VELA_INJ_BPM_Controller_Obj(nullptr),
physical_VELA_INJ_BPM_Controller_Obj(nullptr),
virtual_VELA_BA1_BPM_Controller_Obj(nullptr),
offline_VELA_BA1_BPM_Controller_Obj(nullptr),
physical_VELA_BA1_BPM_Controller_Obj(nullptr),
virtual_VELA_BA2_BPM_Controller_Obj(nullptr),
offline_VELA_BA2_BPM_Controller_Obj(nullptr),
physical_VELA_BA2_BPM_Controller_Obj(nullptr),
virtual_CLARA_INJ_BPM_Controller_Obj(nullptr),
offline_CLARA_INJ_BPM_Controller_Obj(nullptr),
physical_CLARA_INJ_BPM_Controller_Obj(nullptr),
virtual_CLARA_2_VELA_BPM_Controller_Obj(nullptr),
offline_CLARA_2_VELA_BPM_Controller_Obj(nullptr),
physical_CLARA_2_VELA_BPM_Controller_Obj(nullptr),
virtual_CLARA_S01_BPM_Controller_Obj(nullptr),
offline_CLARA_S01_BPM_Controller_Obj(nullptr),
physical_CLARA_S01_BPM_Controller_Obj(nullptr),
virtual_CLARA_S02_BPM_Controller_Obj(nullptr),
offline_CLARA_S02_BPM_Controller_Obj(nullptr),
physical_CLARA_S02_BPM_Controller_Obj(nullptr),
virtual_CLARA_PH1_BPM_Controller_Obj(nullptr),
offline_CLARA_PH1_BPM_Controller_Obj(nullptr),
physical_CLARA_PH1_BPM_Controller_Obj(nullptr),
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
showDebugMessages(false),
showMessages(true),
VELA_INJ ( VELA_ENUM::MACHINE_AREA::VELA_INJ ),
VELA_BA1 ( VELA_ENUM::MACHINE_AREA::VELA_BA1 ),
VELA_BA2 ( VELA_ENUM::MACHINE_AREA::VELA_BA2 ),
CLARA_INJ( VELA_ENUM::MACHINE_AREA::CLARA_INJ),
CLARA_2_VELA( VELA_ENUM::MACHINE_AREA::CLARA_2_VELA ),
CLARA_S01( VELA_ENUM::MACHINE_AREA::CLARA_S01 ),
CLARA_S02( VELA_ENUM::MACHINE_AREA::CLARA_S02 ),
CLARA_PH1( VELA_ENUM::MACHINE_AREA::CLARA_PH1 ),
UNKNOWN_AREA( VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA )
{
    //ctor
}
//______________________________________________________________________________
VCBPMs::~VCBPMs()
{
    if(virtual_VELA_INJ_BPM_Controller_Obj)
    {
        delete virtual_VELA_INJ_BPM_Controller_Obj;
               virtual_VELA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_BPM_Controller_Obj)
    {
        delete offline_VELA_INJ_BPM_Controller_Obj;
               offline_VELA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_BPM_Controller_Obj)
    {
        delete physical_VELA_INJ_BPM_Controller_Obj;
               physical_VELA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA1_BPM_Controller_Obj)
    {
        delete virtual_VELA_BA1_BPM_Controller_Obj;
               virtual_VELA_BA1_BPM_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_BPM_Controller_Obj)
    {
        delete offline_VELA_BA1_BPM_Controller_Obj;
               offline_VELA_BA1_BPM_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_BPM_Controller_Obj)
    {
        delete physical_VELA_BA1_BPM_Controller_Obj;
               physical_VELA_BA1_BPM_Controller_Obj = nullptr;
    }
    if(virtual_VELA_BA2_BPM_Controller_Obj)
    {
        delete virtual_VELA_BA2_BPM_Controller_Obj;
               virtual_VELA_BA2_BPM_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_BPM_Controller_Obj)
    {
        delete offline_VELA_BA2_BPM_Controller_Obj;
               offline_VELA_BA2_BPM_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_BPM_Controller_Obj)
    {
        delete physical_VELA_BA2_BPM_Controller_Obj;
               physical_VELA_BA2_BPM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_INJ_BPM_Controller_Obj)
    {
        delete virtual_CLARA_INJ_BPM_Controller_Obj;
               virtual_CLARA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_INJ_BPM_Controller_Obj)
    {
        delete offline_CLARA_INJ_BPM_Controller_Obj;
               offline_CLARA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_INJ_BPM_Controller_Obj)
    {
        delete physical_CLARA_INJ_BPM_Controller_Obj;
               physical_CLARA_INJ_BPM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_2_VELA_BPM_Controller_Obj)
    {
        delete virtual_CLARA_2_VELA_BPM_Controller_Obj;
               virtual_CLARA_2_VELA_BPM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_2_VELA_BPM_Controller_Obj)
    {
        delete offline_CLARA_2_VELA_BPM_Controller_Obj;
               offline_CLARA_2_VELA_BPM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_2_VELA_BPM_Controller_Obj)
    {
        delete physical_CLARA_2_VELA_BPM_Controller_Obj;
               physical_CLARA_2_VELA_BPM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_S01_BPM_Controller_Obj)
    {
        delete virtual_CLARA_S01_BPM_Controller_Obj;
               virtual_CLARA_S01_BPM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_S01_BPM_Controller_Obj)
    {
        delete offline_CLARA_S01_BPM_Controller_Obj;
               offline_CLARA_S01_BPM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_S01_BPM_Controller_Obj)
    {
        delete physical_CLARA_S01_BPM_Controller_Obj;
               physical_CLARA_S01_BPM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_S02_BPM_Controller_Obj)
    {
        delete virtual_CLARA_S02_BPM_Controller_Obj;
               virtual_CLARA_S02_BPM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_S02_BPM_Controller_Obj)
    {
        delete offline_CLARA_S02_BPM_Controller_Obj;
               offline_CLARA_S02_BPM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_S02_BPM_Controller_Obj)
    {
        delete physical_CLARA_S02_BPM_Controller_Obj;
               physical_CLARA_S02_BPM_Controller_Obj = nullptr;
    }
    if(virtual_CLARA_PH1_BPM_Controller_Obj)
    {
        delete virtual_CLARA_PH1_BPM_Controller_Obj;
               virtual_CLARA_PH1_BPM_Controller_Obj = nullptr;
    }
    if(offline_CLARA_PH1_BPM_Controller_Obj)
    {
        delete offline_CLARA_PH1_BPM_Controller_Obj;
               offline_CLARA_PH1_BPM_Controller_Obj = nullptr;
    }
    if(physical_CLARA_PH1_BPM_Controller_Obj)
    {
        delete physical_CLARA_PH1_BPM_Controller_Obj;
               physical_CLARA_PH1_BPM_Controller_Obj = nullptr;
    }

}    //dtor
//______________________________________________________________________________
void VCBPMs::setQuiet()
{
    std::cout << "VCBPMs Quiet Mode Set." << std::endl;
    showDebugMessages = false;
    showMessages = false;
}
//______________________________________________________________________________
void VCBPMs::setVerbose()
{
    std::cout << "VCBPMs Verbose Mode Set." << std::endl;
    showDebugMessages = true;
    showMessages = true;
}
//______________________________________________________________________________
void VCBPMs::setMessage()
{
    std::cout << "VCBPMs Message Mode Set." << std::endl;
    showDebugMessages = false;
    showMessages = true;
}
//______________________________________________________________________________
void VCBPMs::setDebugMessage()
{
    std::cout << "VCBPMs DebugMessage Mode Set." << std::endl;
    showDebugMessages = true;
    showMessages = false;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_VELA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_INJ_BPM_CONFIG;

    if( virtual_VELA_INJ_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, VELA_INJ );
    }
    return *virtual_VELA_INJ_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_VELA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_INJ_BPM_CONFIG;

    if( offline_VELA_INJ_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_INJ );
    }
    return *offline_VELA_INJ_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_VELA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_INJ_BPM_CONFIG;

    if( physical_VELA_INJ_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_INJ );
    }
    return *physical_VELA_INJ_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_VELA_BA1_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_BA1_BPM_CONFIG;

    if( virtual_VELA_BA1_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA1_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA1 );
    }
    return *virtual_VELA_BA1_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_VELA_BA1_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_BA1_BPM_CONFIG;

    if( offline_VELA_BA1_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA1_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA1 );
    }
    return *offline_VELA_BA1_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_VELA_BA1_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_BA1_BPM_CONFIG;

    if( physical_VELA_BA1_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA1_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA1 );
    }
    return *physical_VELA_BA1_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_VELA_BA2_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_BA2_BPM_CONFIG;

    if( virtual_VELA_BA2_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_VELA_BA2_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA2 );
    }
    return *virtual_VELA_BA2_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_VELA_BA2_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_BA2_BPM_CONFIG;

    if( offline_VELA_BA2_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_VELA_BA2_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA2 );
    }
    return *offline_VELA_BA2_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_VELA_BA2_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::VELA_BA2_BPM_CONFIG;

    if( physical_VELA_BA2_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_VELA_BA2_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA2 );
    }
    return *physical_VELA_BA2_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_CLARA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_BPM_CONFIG;

    if( virtual_CLARA_INJ_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, CLARA_INJ );
    }
    return *virtual_CLARA_INJ_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_CLARA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_BPM_CONFIG;

    if( offline_CLARA_INJ_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_INJ );
    }
    return *offline_CLARA_INJ_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_CLARA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_INJ_BPM_CONFIG;

    if( physical_CLARA_INJ_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_INJ );
    }
    return *physical_CLARA_INJ_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_CLARA_2_VELA_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_BPM_CONFIG;

    if( virtual_CLARA_2_VELA_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_2_VELA_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, CLARA_2_VELA );
    }
    return *virtual_CLARA_2_VELA_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_CLARA_2_VELA_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_BPM_CONFIG;

    if( offline_CLARA_2_VELA_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_2_VELA_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_2_VELA );
    }
    return *offline_CLARA_2_VELA_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_CLARA_2_VELA_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_BPM_CONFIG;

    if( physical_CLARA_2_VELA_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_2_VELA_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_2_VELA );
    }
    return *physical_CLARA_2_VELA_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_CLARA_S01_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_S01_BPM_CONFIG;

    if( virtual_CLARA_S01_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_S01_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S01 );
    }
    return *virtual_CLARA_S01_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_CLARA_S01_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_S01_BPM_CONFIG;

    if( offline_CLARA_S01_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_S01_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S01 );
    }
    return *offline_CLARA_S01_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_CLARA_S01_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_S01_BPM_CONFIG;

    if( physical_CLARA_S01_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_S01_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S01 );
    }
    return *physical_CLARA_S01_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_CLARA_S02_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_S02_BPM_CONFIG;

    if( virtual_CLARA_S02_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_S02_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S02 );
    }
    return *virtual_CLARA_S02_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_CLARA_S02_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_S02_BPM_CONFIG;

    if( offline_CLARA_S02_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_S02_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S02 );
    }
    return *offline_CLARA_S02_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_CLARA_S02_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_S02_BPM_CONFIG;

    if( physical_CLARA_S02_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_S02_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S02 );
    }
    return *physical_CLARA_S02_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_CLARA_PH1_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_PH1_BPM_CONFIG;

    if( virtual_CLARA_PH1_BPM_Controller_Obj )
    {

    }
    else
    {
        virtual_CLARA_PH1_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withVM, CLARA_PH1 );
    }
    return *virtual_CLARA_PH1_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::offline_CLARA_PH1_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_PH1_BPM_CONFIG;

    if( offline_CLARA_PH1_BPM_Controller_Obj )
    {

    }
    else
    {
        offline_CLARA_PH1_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_PH1 );
    }
    return *offline_CLARA_PH1_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::physical_CLARA_PH1_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH + UTL::CLARA_PH1_BPM_CONFIG;

    if( physical_CLARA_PH1_BPM_Controller_Obj )
    {

    }
    else
    {
        physical_CLARA_PH1_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_PH1 );
    }
    return *physical_CLARA_PH1_BPM_Controller_Obj;
}
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::getBPMController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area )
{
    if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_INJ )
        return offline_VELA_INJ_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_INJ )
        return virtual_VELA_INJ_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_INJ )
        return physical_VELA_INJ_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA1 )
        return offline_VELA_BA1_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA1 )
        return physical_VELA_BA1_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA2 )
        return offline_VELA_BA2_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA2 )
        return physical_VELA_BA2_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_INJ )
        return offline_CLARA_INJ_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_INJ )
        return virtual_CLARA_INJ_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_INJ )
        return physical_CLARA_INJ_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_2_VELA )
        return offline_CLARA_2_VELA_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_2_VELA )
        return virtual_CLARA_2_VELA_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_2_VELA )
        return physical_CLARA_2_VELA_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S01 )
        return offline_CLARA_S01_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S01 )
        return virtual_CLARA_S01_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S01 )
        return physical_CLARA_S01_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S02 )
        return offline_CLARA_S02_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S02 )
        return virtual_CLARA_S02_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S02 )
        return physical_CLARA_S02_BPM_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_BPM_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_BPM_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_BPM_Controller();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
#endif //BUILD_DLL
