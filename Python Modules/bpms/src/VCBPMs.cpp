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
withEPICS(true),
withoutEPICS(false),
withoutVM(false),
withVM(true),
showMessages(true),
dontShowMessages(false),
showDebugMessages(false),
dontShowDebugMessages(false),
VELA_INJ ( VELA_ENUM::MACHINE_AREA::VELA_INJ ),
VELA_BA1 ( VELA_ENUM::MACHINE_AREA::VELA_BA1 ),
VELA_BA2 ( VELA_ENUM::MACHINE_AREA::VELA_BA2 ),
CLARA_INJ( VELA_ENUM::MACHINE_AREA::CLARA_INJ),
CLARA_2_VELA( VELA_ENUM::MACHINE_AREA::CLARA_2_VELA ),
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
}    //dtor
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

}
//______________________________________________________________________________
#ifdef BUILD_DLL
//boost::python::dict velaINJBeamPositionMonitorController::getBPMStateDefinition()
//{
//    std::map< VELA_ENUM::TRIG_STATE,  std::string  > m;
//
//    m[ VELA_ENUM::TRIG_STATE::NOTRIG ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::NOTRIG  ); // ENUM_TO_STRING MACRO in structs.h
//    m[ VELA_ENUM::TRIG_STATE::TRIG   ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG    );
//    m[ VELA_ENUM::TRIG_STATE::TRIG_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::TRIG_STATE::TRIG_ERROR   );
//    return enumStringMapToPythonDict( m );
//}
//______________________________________________________________________________
//boost::python::dict velaINJBeamPositionMonitorController::getILockStates_Py( std::string name )
//{
//    return enumMapToPythonDict( getILockStates( name ) );
//}
////__________________________________________________a____________________________
//bool velaINJBeamPositionMonitorController::hasTrig_Py( const std::string & name )
//{
//    return hasTrig( name );
//}
////______________________________________________________________________________
//bool velaINJBeamPositionMonitorController::hasNoTrig_Py( const std::string & name )
//{
//    return hasNoTrig( name );
//}
//______________________________________________________________________________
//const beamPositionMonitorStructs::rawDataStruct & velaINJBeamPositionMonitorController::getAllBPMData_Py( const std::string & name )
//{
//    return getAllBPMData( name );
//}
//______________________________________________________________________________
//std::vector< std::vector< double > > velaINJBeamPositionMonitorController::getBPMRawData_Py( const std::string & name )
//{
//    return getBPMRawData( name );
//}
////______________________________________________________________________________
//std::vector< double > velaINJBeamPositionMonitorController::getBPMXVec_Py( const std::string & name )
//{
//    return getBPMXVec( name );
//}
////______________________________________________________________________________
//std::vector< double > velaINJBeamPositionMonitorController::getBPMYVec_Py( const std::string & name )
//{
//    return getBPMYVec( name );
//}
////______________________________________________________________________________
//std::vector< double > velaINJBeamPositionMonitorController::getBPMQVec_Py( const std::string & name )
//{
//    return getBPMQVec( name );
//}
////______________________________________________________________________________
//std::vector< double > velaINJBeamPositionMonitorController::getTimeStamps_Py( const std::string & name )
//{
//    return getTimeStamps( name );
//}
////______________________________________________________________________________
//std::vector< std::string > velaINJBeamPositionMonitorController::getStrTimeStamps_Py( const std::string & name )
//{
//    return getStrTimeStamps( name );
//}
////______________________________________________________________________________
//bool velaINJBeamPositionMonitorController::isMonitoringBPMData_Py( const std::string & name )
//{
//    return isMonitoringBPMData( name );
//}
////______________________________________________________________________________
//bool velaINJBeamPositionMonitorController::isNotMonitoringBPMData_Py( const std::string & name )
//{
//    return isNotMonitoringBPMData( name );
//}
////______________________________________________________________________________
//void velaINJBeamPositionMonitorController::monitorDataForNShots_Py( size_t N, const std::string & bpmNames )
//{
//    return monitorDataForNShots( N, bpmNames );
//}
////______________________________________________________________________________
//double velaINJBeamPositionMonitorController::getXFromPV_Py( const std::string & name )
//{
//    return getXFromPV( name );
//}
////______________________________________________________________________________
//double velaINJBeamPositionMonitorController::getYFromPV_Py( const std::string & name )
//{
//    return getYFromPV( name );
//}
////______________________________________________________________________________
//double velaINJBeamPositionMonitorController::getBPMResolution_Py( const std::string & name )
//{
//    return getBPMResolution( name );
//}
////______________________________________________________________________________
//double velaINJBeamPositionMonitorController::getX_Py( const std::string & name )
//{
//    return getX( name );
//}
////______________________________________________________________________________
//double velaINJBeamPositionMonitorController::getY_Py( const std::string & name )
//{
//    return getY( name );
//}
////______________________________________________________________________________
//double velaINJBeamPositionMonitorController::getQ_Py( const std::string & name )
//{
//    return getQ( name );
//}
////______________________________________________________________________________
//long velaINJBeamPositionMonitorController::getRA1_Py( const std::string & name )
//{
//    return getRA1( name );
//}
////______________________________________________________________________________
//long velaINJBeamPositionMonitorController::getRA2_Py( const std::string & name )
//{
//    return getRA2( name );
//}
////______________________________________________________________________________
//long velaINJBeamPositionMonitorController::getRD1_Py( const std::string & name )
//{
//    return getRD1( name );
//}
////______________________________________________________________________________
//long velaINJBeamPositionMonitorController::getRD2_Py( const std::string & name )
//{
//    return getRD2( name );
//}
////______________________________________________________________________________
//void velaINJBeamPositionMonitorController::setSA1_Py( const std::string & bpmName, long sa1 )
//{
//    setSA1( bpmName, sa1 );
//}
////______________________________________________________________________________
//void velaINJBeamPositionMonitorController::setSA2_Py( const std::string & bpmName, long sa2 )
//{
//    setSA2( bpmName, sa2 );
//}
////______________________________________________________________________________
//void velaINJBeamPositionMonitorController::setSD1_Py( const std::string & bpmName, long sd1 )
//{
//    setSD1( bpmName, sd1 );
//}
////______________________________________________________________________________
//void velaINJBeamPositionMonitorController::setSD2_Py( const std::string & bpmName, long sd2 )
//{
//    setSD2( bpmName, sd2 );
//}
////______________________________________________________________________________
//std::vector< std::string > velaINJBeamPositionMonitorController::getBPMNames_Py()
//{
//    return getBPMNames();
//}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE velaINJBeamPositionMonitorController::getBPMState_Py( const std::string & name )
//{
//    return getBPMState( name );
//}
#endif //BUILD_DLL
