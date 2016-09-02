#include "velaRFGunController.h"
#include <fstream>
#include <iostream>

// stl
velaRFGunController::velaRFGunController( const std::string configFileLocation1,
                                  const std::string configFileLocation2,
                                  const std::string configFileLocation3, const  bool show_messages, const bool show_debug_messages )
: RFGunController( configFileLocation1, configFileLocation2, configFileLocation3, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaRFGunController::velaRFGunController( const  bool show_messages, const bool show_debug_messages)
: RFGunController( "\\\\fed.cclrc.ac.uk\\Org\\NLab\\ASTeC\\Projects\\VELA\\Software\\VM-Controllers\\VELA-CLARA-Controllers\\Config\\velaRFGunPower.config",
                   "\\\\fed.cclrc.ac.uk\\Org\\NLab\\ASTeC\\Projects\\VELA\\Software\\VM-Controllers\\VELA-CLARA-Controllers\\Config\\velaRFGunLLRF.config",
                   "\\\\fed.cclrc.ac.uk\\Org\\NLab\\ASTeC\\Projects\\VELA\\Software\\VM-Controllers\\VELA-CLARA-Controllers\\Config\\velaRFGunModulator.config",
                   show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaRFGunController::~velaRFGunController(){}    //dtor


#ifdef BUILD_DLL
std::vector< std::vector< double >> velaRFGunController::getRevT_Py( const std::string name )
{
    return localInterface.getRevT( name );
}
//______________________________________________________________________________
std::vector< std::vector< double >> velaRFGunController::getFwdT_Py(const std::string name  )
{
    return localInterface.getFwdT( name );
}
//______________________________________________________________________________
std::vector< double > velaRFGunController::getRevTStamp_Py(const std::string name  )
{
    return localInterface.getRevTStamp( name );
}
//______________________________________________________________________________
std::vector< double > velaRFGunController::getFwdTStamp_Py(const std::string name  )
{
    return localInterface.getFwdTStamp( name );
}
//______________________________________________________________________________
std::vector< std::string > velaRFGunController::getRevTStampStr_Py( const std::string name )
{
    return localInterface.getRevTStampStr( name );
}
//______________________________________________________________________________
std::vector< std::string > velaRFGunController::getFwdTStampStr_Py( const std::string name  )
{
    return localInterface.getFwdTStampStr( name );
}
//______________________________________________________________________________
double velaRFGunController::getFwd_Py( std::string name )
{
    return localInterface.getFwd( name );
}
//______________________________________________________________________________
double velaRFGunController::getRev_Py( std::string name )
{
    return localInterface.getRev( name );
}
//______________________________________________________________________________
double velaRFGunController::getRatio_Py( std::string name )
{
    return localInterface.getRatio( name );
}
//______________________________________________________________________________
boost::python::dict velaRFGunController::getModStateDefintions( )
{
    std::map< RFGunStructs::MOD_STATE,  std::string  > m;
    m[ RFGunStructs::MOD_STATE::UNKNOWN1 ]       = ENUM_TO_STRING( RFGunStructs::MOD_STATE::UNKNOWN1 ); // ENUM_TO_STRING MACRO in velaStructs.h
    m[ RFGunStructs::MOD_STATE::ERROR1 ]         = ENUM_TO_STRING( RFGunStructs::MOD_STATE::ERROR1 );
    m[ RFGunStructs::MOD_STATE::OFF ]            = ENUM_TO_STRING( RFGunStructs::MOD_STATE::OFF );
    m[ RFGunStructs::MOD_STATE::off_Request ]    = ENUM_TO_STRING( RFGunStructs::MOD_STATE::off_Request );
    m[ RFGunStructs::MOD_STATE::HV_Intrlock ]    = ENUM_TO_STRING( RFGunStructs::MOD_STATE::HV_Intrlock );
    m[ RFGunStructs::MOD_STATE::Standby_Request ]= ENUM_TO_STRING( RFGunStructs::MOD_STATE::Standby_Request );
    m[ RFGunStructs::MOD_STATE::Standby ]        = ENUM_TO_STRING( RFGunStructs::MOD_STATE::Standby );
    m[ RFGunStructs::MOD_STATE::HV_Off_Requ ]    = ENUM_TO_STRING( RFGunStructs::MOD_STATE::HV_Off_Requ );
    m[ RFGunStructs::MOD_STATE::HV_Intrlock ]    = ENUM_TO_STRING( RFGunStructs::MOD_STATE::HV_Intrlock );
    m[ RFGunStructs::MOD_STATE::Trigger_Interl ] = ENUM_TO_STRING( RFGunStructs::MOD_STATE::Trigger_Interl );
    m[ RFGunStructs::MOD_STATE::HV_Request ]     = ENUM_TO_STRING( RFGunStructs::MOD_STATE::HV_Request );
    m[ RFGunStructs::MOD_STATE::HV_On ]          = ENUM_TO_STRING( RFGunStructs::MOD_STATE::HV_On );
    m[ RFGunStructs::MOD_STATE::Trig_Off_Req ]   = ENUM_TO_STRING( RFGunStructs::MOD_STATE::Trig_Off_Req );
    m[ RFGunStructs::MOD_STATE::Trigger_Interl ] = ENUM_TO_STRING( RFGunStructs::MOD_STATE::Trigger_Interl );
    m[ RFGunStructs::MOD_STATE::Trig_Request ]   = ENUM_TO_STRING( RFGunStructs::MOD_STATE::Trig_Request );
    m[ RFGunStructs::MOD_STATE::Trig ]           = ENUM_TO_STRING( RFGunStructs::MOD_STATE::Trig );
    return enumStringMapToPythonDict( m );

}
//______________________________________________________________________________
boost::python::dict velaRFGunController::getModEXILOCK1Defintions( )
{
    std::map< RFGunStructs::MOD_EXILOCK1,  std::string  > m;
    m[ RFGunStructs::MOD_EXILOCK1::BAD ]      = ENUM_TO_STRING( RFGunStructs::MOD_EXILOCK1::BAD ); // ENUM_TO_STRING MACRO in velaStructs.h
    m[ RFGunStructs::MOD_EXILOCK1::GOOD ]     = ENUM_TO_STRING( RFGunStructs::MOD_EXILOCK1::GOOD );
    m[ RFGunStructs::MOD_EXILOCK1::ERROR ]    = ENUM_TO_STRING( RFGunStructs::MOD_EXILOCK1::ERROR );
    m[ RFGunStructs::MOD_EXILOCK1::UNKNOWN ]  = ENUM_TO_STRING( RFGunStructs::MOD_EXILOCK1::UNKNOWN );
    return enumStringMapToPythonDict( m );
}
#endif // BUILD_DLL




