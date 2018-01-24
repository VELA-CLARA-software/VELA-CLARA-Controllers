#include "VCimg.h"
// stl

#include <iostream>

VCimg::VCimg():
virtual_VELA_INJ_IMG_Controller_Obj(nullptr),
offline_VELA_INJ_IMG_Controller_Obj(nullptr),
physical_VELA_INJ_IMG_Controller_Obj(nullptr),

virtual_VELA_BA1_IMG_Controller_Obj(nullptr),
offline_VELA_BA1_IMG_Controller_Obj(nullptr),
physical_VELA_BA1_IMG_Controller_Obj(nullptr),

virtual_VELA_BA2_IMG_Controller_Obj(nullptr),
offline_VELA_BA2_IMG_Controller_Obj(nullptr),
physical_VELA_BA2_IMG_Controller_Obj(nullptr),

virtual_CLARA_S01_IMG_Controller_Obj(nullptr),
offline_CLARA_S01_IMG_Controller_Obj(nullptr),
physical_CLARA_S01_IMG_Controller_Obj(nullptr),

virtual_CLARA_PH1_IMG_Controller_Obj(nullptr),
offline_CLARA_PH1_IMG_Controller_Obj(nullptr),
physical_CLARA_PH1_IMG_Controller_Obj(nullptr),

virtual_CLARA_S02_IMG_Controller_Obj(nullptr),
offline_CLARA_S02_IMG_Controller_Obj(nullptr),
physical_CLARA_S02_IMG_Controller_Obj(nullptr),

virtual_C2V_IMG_Controller_Obj(nullptr),
offline_C2V_IMG_Controller_Obj(nullptr),
physical_C2V_IMG_Controller_Obj(nullptr),
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
CLARA_S01( VELA_ENUM::MACHINE_AREA::CLARA_S01),
CLARA_S02( VELA_ENUM::MACHINE_AREA::CLARA_S02),
CLARA_PH1( VELA_ENUM::MACHINE_AREA::CLARA_PH1),
CLARA_2_VELA( VELA_ENUM::MACHINE_AREA::CLARA_2_VELA ),
UNKNOWN_AREA( VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA )
{
    //ctor
}
//______________________________________________________________________________
VCimg::~VCimg()
{
/// VELA_INJ
    if(virtual_VELA_INJ_IMG_Controller_Obj)
    {
        std::cout << "Destructor for virtual_VELA_INJ_IMG_Controller_Obj called" << std::endl;
        delete virtual_VELA_INJ_IMG_Controller_Obj;
               virtual_VELA_INJ_IMG_Controller_Obj = nullptr;
    }
    if(offline_VELA_INJ_IMG_Controller_Obj)
    {
        std::cout << "Destructor for offline_VELA_INJ_IMG_Controller_Obj called" << std::endl;
        delete offline_VELA_INJ_IMG_Controller_Obj;
               offline_VELA_INJ_IMG_Controller_Obj = nullptr;
    }
    if(physical_VELA_INJ_IMG_Controller_Obj)
    {
        std::cout << "Destructor for physical_VELA_INJ_IMG_Controller_Obj called" << std::endl;
        delete physical_VELA_INJ_IMG_Controller_Obj;
               physical_VELA_INJ_IMG_Controller_Obj = nullptr;
    }

/// VELA_BA1
    if(virtual_VELA_BA1_IMG_Controller_Obj)
    {
        std::cout << "Destructor for virtual_VELA_BA1_IMG_Controller_Obj called" << std::endl;
        delete virtual_VELA_BA1_IMG_Controller_Obj;
               virtual_VELA_BA1_IMG_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA1_IMG_Controller_Obj)
    {
        std::cout << "Destructor for offline_VELA_BA1_IMG_Controller_Obj called" << std::endl;
        delete offline_VELA_BA1_IMG_Controller_Obj;
               offline_VELA_BA1_IMG_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA1_IMG_Controller_Obj)
    {
        std::cout << "Destructor for physical_VELA_BA1_IMG_Controller_Obj called" << std::endl;
        delete physical_VELA_BA1_IMG_Controller_Obj;
               physical_VELA_BA1_IMG_Controller_Obj = nullptr;
    }

/// VELA_BA2
    if(virtual_VELA_BA2_IMG_Controller_Obj)
    {
        std::cout << "Destructor for virtual_VELA_BA2_IMG_Controller_Obj called" << std::endl;
        delete virtual_VELA_BA2_IMG_Controller_Obj;
               virtual_VELA_BA2_IMG_Controller_Obj = nullptr;
    }
    if(offline_VELA_BA2_IMG_Controller_Obj)
    {
        std::cout << "Destructor for offline_VELA_BA2_IMG_Controller_Obj called" << std::endl;
        delete offline_VELA_BA2_IMG_Controller_Obj;
               offline_VELA_BA2_IMG_Controller_Obj = nullptr;
    }
    if(physical_VELA_BA2_IMG_Controller_Obj)
    {
        std::cout << "Destructor for physical_VELA_BA2_IMG_Controller_Obj called" << std::endl;
        delete physical_VELA_BA2_IMG_Controller_Obj;
               physical_VELA_BA2_IMG_Controller_Obj = nullptr;
    }

/// CLARA_S01
    if(virtual_CLARA_S01_IMG_Controller_Obj)
    {
        std::cout << "Destructor for physical_CLARA_S01_IMG_Controller_Obj called" << std::endl;
        delete virtual_CLARA_S01_IMG_Controller_Obj;
               virtual_CLARA_S01_IMG_Controller_Obj = nullptr;
    }
    if(offline_CLARA_S01_IMG_Controller_Obj)
    {
        std::cout << "Destructor for offline_CLARA_S01_IMG_Controller_Obj called" << std::endl;
        delete offline_CLARA_S01_IMG_Controller_Obj;
               offline_CLARA_S01_IMG_Controller_Obj = nullptr;
    }
    if(physical_CLARA_S01_IMG_Controller_Obj)
    {
//        THIS IS THE CULPRIT - ASK DUNCAN ABOUT DESTRUCTORS
        std::cout << "Destructor for physical_CLARA_S01_IMG_Controller_Obj called" << std::endl;
        delete physical_CLARA_S01_IMG_Controller_Obj;
               physical_CLARA_S01_IMG_Controller_Obj = nullptr;
    }

/// CLARA_S02
    if(virtual_CLARA_S02_IMG_Controller_Obj)
    {
        std::cout << "Destructor for virtual_CLARA_S02_IMG_Controller_Obj called" << std::endl;
        delete virtual_CLARA_S02_IMG_Controller_Obj;
               virtual_CLARA_S02_IMG_Controller_Obj = nullptr;
    }
    if(offline_CLARA_S02_IMG_Controller_Obj)
    {
        std::cout << "Destructor for offline_CLARA_S02_IMG_Controller_Obj called" << std::endl;
        delete offline_CLARA_S02_IMG_Controller_Obj;
               offline_CLARA_S02_IMG_Controller_Obj = nullptr;
    }
    if(physical_CLARA_S02_IMG_Controller_Obj)
    {
        std::cout << "Destructor for physical_CLARA_S02_IMG_Controller_Obj called" << std::endl;
        delete physical_CLARA_S02_IMG_Controller_Obj;
               physical_CLARA_S02_IMG_Controller_Obj = nullptr;
    }

///    CLARA_2_VELA
    if(virtual_C2V_IMG_Controller_Obj)
    {
        std::cout << "Destructor for virtual_C2V_IMG_Controller_Obj called" << std::endl;
        delete virtual_C2V_IMG_Controller_Obj;
               virtual_C2V_IMG_Controller_Obj = nullptr;
    }
    if(offline_C2V_IMG_Controller_Obj)
    {
        std::cout << "Destructor for offline_C2V_IMG_Controller_Obj called" << std::endl;
        delete offline_C2V_IMG_Controller_Obj;
               offline_C2V_IMG_Controller_Obj = nullptr;
    }
    if(physical_C2V_IMG_Controller_Obj)
    {
        std::cout << "Destructor for physical_C2V_IMG_Controller_Obj called" << std::endl;
        delete physical_C2V_IMG_Controller_Obj;
               physical_C2V_IMG_Controller_Obj = nullptr;
    }
}    //dtor
//______________________________________________________________________________
void VCimg::setQuiet()
{
    std::cout << "VCimg Quiet Mode Set." << std::endl;
    showDebugMessages = false;
    showMessages = false;
}
//______________________________________________________________________________
void VCimg::setVerbose()
{
    std::cout << "VCimg Verbose Mode Set." << std::endl;
    showDebugMessages = true;
    showMessages = true;
}
//______________________________________________________________________________
void VCimg::setMessage()
{
    std::cout << "VCimg Message Mode Set." << std::endl;
    showDebugMessages = false;
    showMessages = true;
}
//______________________________________________________________________________
void VCimg::setDebugMessage()
{
    std::cout << "VCimg DebugMessage Mode Set." << std::endl;
    showDebugMessages = true;
    showMessages = false;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_VELA_INJ_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_INJ_IMG_CONFIG;

    if( virtual_VELA_INJ_IMG_Controller_Obj )
    {
        std::cout << "virtual_VELA_INJ_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_VELA_INJ_IMG_Controller object" << std::endl;
        virtual_VELA_INJ_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, VELA_INJ );
    }
    return *virtual_VELA_INJ_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_VELA_INJ_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_INJ_IMG_CONFIG;

    if( offline_VELA_INJ_IMG_Controller_Obj )
    {
        std::cout << "offline_VELA_INJ_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_INJ_IMG_Controller object" << std::endl;
        offline_VELA_INJ_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_INJ );
    }
    return *offline_VELA_INJ_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_VELA_INJ_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_INJ_IMG_CONFIG;

    if( physical_VELA_INJ_IMG_Controller_Obj )
    {
        std::cout << "physical_VELA_INJ_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_INJ_IMG_Controller object" << std::endl;
        physical_VELA_INJ_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_INJ );
    }
    return *physical_VELA_INJ_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_VELA_BA1_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_BA1_IMG_CONFIG;

    if( virtual_VELA_BA1_IMG_Controller_Obj )
    {
        std::cout << "virtual_VELA_BA1_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_VELA_BA1_IMG_Controller object" << std::endl;
        virtual_VELA_BA1_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA1 );
    }
    return *virtual_VELA_BA1_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_VELA_BA1_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_BA1_IMG_CONFIG;

    if( offline_VELA_BA1_IMG_Controller_Obj )
    {
        std::cout << "offline_VELA_BA1_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_BA1_IMG_Controller object" << std::endl;
        offline_VELA_BA1_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA1 );
    }
    return *offline_VELA_BA1_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_VELA_BA1_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_BA1_IMG_CONFIG;

    if( physical_VELA_BA1_IMG_Controller_Obj )
    {
        std::cout << "physical_VELA_BA1_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_BA1_IMG_Controller object" << std::endl;
        physical_VELA_BA1_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA1 );
    }
    return *physical_VELA_BA1_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_VELA_BA2_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_BA2_IMG_CONFIG;

    if( virtual_VELA_BA2_IMG_Controller_Obj )
    {
        std::cout << "virtual_VELA_BA2_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_VELA_BA2_IMG_Controller object" << std::endl;
        virtual_VELA_BA2_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, VELA_BA2 );
    }
    return *virtual_VELA_BA2_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_VELA_BA2_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_BA2_IMG_CONFIG;

    if( offline_VELA_BA2_IMG_Controller_Obj )
    {
        std::cout << "offline_VELA_BA2_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_VELA_BA2_IMG_Controller object" << std::endl;
        offline_VELA_BA2_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, VELA_BA2 );
    }
    return *offline_VELA_BA2_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_VELA_BA2_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::VELA_BA2_IMG_CONFIG;

    if( physical_VELA_BA2_IMG_Controller_Obj )
    {
        std::cout << "physical_VELA_BA2_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_VELA_BA2_IMG_Controller object" << std::endl;
        physical_VELA_BA2_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, VELA_BA2 );
    }
    return *physical_VELA_BA2_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_CLARA_S01_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_S01_IMG_CONFIG;

    if( virtual_CLARA_S01_IMG_Controller_Obj )
    {
        std::cout << "virtual_CLARA_S01_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_CLARA_S01_IMG_Controller object" << std::endl;
        virtual_CLARA_S01_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S01 );
    }
    return *virtual_CLARA_S01_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_CLARA_S01_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_S01_IMG_CONFIG;

    if( offline_CLARA_S01_IMG_Controller_Obj )
    {
        std::cout << "offline_CLARA_S01_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_CLARA_S01_IMG_Controller object" << std::endl;
        offline_CLARA_S01_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S01 );
    }
    return *offline_CLARA_S01_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_CLARA_S01_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_S01_IMG_CONFIG;

    if( physical_CLARA_S01_IMG_Controller_Obj )
    {
        std::cout << "physical_CLARA_S01_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_CLARA_S01_IMG_Controller object" << std::endl;
        physical_CLARA_S01_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S01 );
    }
    return *physical_CLARA_S01_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_CLARA_S02_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_S02_IMG_CONFIG;

    if( virtual_CLARA_S02_IMG_Controller_Obj )
    {
        std::cout << "virtual_CLARA_S02_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_CLARA_S02_IMG_Controller object" << std::endl;
        virtual_CLARA_S02_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_S02 );
    }
    return *virtual_CLARA_S02_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_CLARA_S02_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_S02_IMG_CONFIG;

    if( offline_CLARA_S02_IMG_Controller_Obj )
    {
        std::cout << "offline_CLARA_S02_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_CLARA_S02_IMG_Controller object" << std::endl;
        offline_CLARA_S02_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_S02 );
    }
    return *offline_CLARA_S02_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_CLARA_S02_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_S02_IMG_CONFIG;

    if( physical_CLARA_S02_IMG_Controller_Obj )
    {
        std::cout << "physical_CLARA_S02_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_CLARA_S02_IMG_Controller object" << std::endl;
        physical_CLARA_S02_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_S02 );
    }
    return *physical_CLARA_S02_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_CLARA_PH1_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_PH1_IMG_CONFIG;

    if( virtual_CLARA_PH1_IMG_Controller_Obj )
    {
        std::cout << "virtual_CLARA_PH1_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_CLARA_PH1_IMG_Controller object" << std::endl;
        virtual_CLARA_PH1_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_PH1 );
    }
    return *virtual_CLARA_PH1_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_CLARA_PH1_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_PH1_IMG_CONFIG;

    if( offline_CLARA_PH1_IMG_Controller_Obj )
    {
        std::cout << "offline_CLARA_PH1_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating offline_CLARA_PH1_IMG_Controller object" << std::endl;
        offline_CLARA_PH1_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_PH1 );
    }
    return *offline_CLARA_PH1_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_CLARA_PH1_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_PH1_IMG_CONFIG;

    if( physical_CLARA_PH1_IMG_Controller_Obj )
    {
        std::cout << "physical_CLARA_PH1_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating physical_CLARA_PH1_IMG_Controller object" << std::endl;
        physical_CLARA_PH1_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_PH1 );
    }
    return *physical_CLARA_PH1_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::virtual_C2V_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_IMG_CONFIG;

    if( virtual_C2V_IMG_Controller_Obj )
    {
        std::cout << "virtual_C2V_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_C2V_IMG_Controller object" << std::endl;
        virtual_C2V_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withVM, CLARA_2_VELA );
    }
    return *virtual_C2V_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::offline_C2V_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_IMG_CONFIG;

    if( offline_C2V_IMG_Controller_Obj )
    {
        std::cout << "offline_C2V_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_C2V_IMG_Controller object" << std::endl;
        offline_C2V_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withoutEPICS, withoutVM, CLARA_2_VELA );
    }
    return *offline_C2V_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::physical_C2V_IMG_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string imgconf1 = UTL::CONFIG_PATH + UTL::CLARA_2_VELA_IMG_CONFIG;

    if( physical_C2V_IMG_Controller_Obj )
    {
        std::cout << "physical_C2V_IMG_Controller object already exists," << std::endl;
    }
    else
    {
        std::cout << "Creating virtual_C2V_IMG_Controller object" << std::endl;
        physical_C2V_IMG_Controller_Obj = new invertedMagnetronGaugeController( imgconf1, showMessages, showDebugMessages, withEPICS, withoutVM, CLARA_2_VELA );
    }
    return *physical_C2V_IMG_Controller_Obj;
}
//______________________________________________________________________________
invertedMagnetronGaugeController & VCimg::getIMGController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area )
{
    if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_INJ )
        return offline_VELA_INJ_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_INJ )
        return virtual_VELA_INJ_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_INJ )
        return physical_VELA_INJ_IMG_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA1 )
        return offline_VELA_BA1_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA1 )
        return virtual_VELA_BA1_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA1 )
        return physical_VELA_BA1_IMG_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::VELA_BA2 )
        return offline_VELA_BA2_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::VELA_BA2 )
        return virtual_VELA_BA2_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::VELA_BA2 )
        return physical_VELA_BA2_IMG_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S01 )
        return offline_CLARA_S01_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S01 )
        return virtual_CLARA_S01_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S01 )
        return physical_CLARA_S01_IMG_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_S02 )
        return offline_CLARA_S02_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_S02 )
        return virtual_CLARA_S02_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_S02 )
        return physical_CLARA_S02_IMG_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_PH1 )
        return offline_CLARA_PH1_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_PH1 )
        return virtual_CLARA_PH1_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_PH1 )
        return physical_CLARA_PH1_IMG_Controller();
    else if( mode == VELA_ENUM::OFFLINE && area == VELA_ENUM::CLARA_2_VELA )
        return offline_C2V_IMG_Controller();
    else if( mode == VELA_ENUM::VIRTUAL && area == VELA_ENUM::CLARA_2_VELA )
        return virtual_C2V_IMG_Controller();
    else if( mode == VELA_ENUM::PHYSICAL && area == VELA_ENUM::CLARA_2_VELA )
        return physical_C2V_IMG_Controller();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
#endif //BUILD_DLL



