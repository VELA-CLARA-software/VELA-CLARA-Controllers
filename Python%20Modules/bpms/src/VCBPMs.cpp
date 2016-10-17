#include "VCBPMs.h"
// stl

#include <iostream>

////______________________________________________________________________________
//velaINJBeamPositionMonitorController::velaINJBeamPositionMonitorController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
//: beamPositionMonitorController( configFileLocation, show_messages, show_debug_messages )
//    virtual_VELA_INJ_BPM_Controller_Obj_Exists(false),
//    virtual_VELA_INJ_BPM_Controller_Obj(nullptr),
//
//{}
////______________________________________________________________________________
//velaINJBeamPositionMonitorController::velaINJBeamPositionMonitorController(const bool RealMachine, const  bool show_messages, const bool show_debug_messages  )
//: beamPositionMonitorController((RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_INJ_BPM_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_INJ_BPM_CONFIG, show_messages, show_debug_messages )
//    virtual_VELA_INJ_BPM_Controller_Obj_Exists(false),
//    virtual_VELA_INJ_BPM_Controller_Obj(nullptr),
//{
//
//}
VCBPMs::VCBPMs():
virtual_VELA_INJ_BPM_Controller_Obj_Exists(false),
virtual_VELA_INJ_BPM_Controller_Obj(nullptr)
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
}    //dtor
//______________________________________________________________________________
beamPositionMonitorController & VCBPMs::virtual_VELA_INJ_BPM_Controller()
{
    std::cout << "creating object" << std::endl;
    const std::string bpmconf = UTL::CONFIG_PATH_VM + UTL::VELA_INJ_BPM_CONFIG;

    if( virtual_VELA_INJ_BPM_Controller_Obj_Exists )
    {

    }
    else
    {
        virtual_VELA_INJ_BPM_Controller_Obj = new beamPositionMonitorController( bpmconf, true, true, true );
    }
    return *virtual_VELA_INJ_BPM_Controller_Obj;
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


