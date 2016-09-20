#include "velaInvertedMagnetronGaugeController.h"
// stl
#include <iostream>

//______________________________________________________________________________
velaInvertedMagnetronGaugeController::velaInvertedMagnetronGaugeController( const std::string configFileLocation, const  bool show_messages, const bool show_debug_messages )
: invertedMagnetronGaugeController(configFileLocation, show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaInvertedMagnetronGaugeController::velaInvertedMagnetronGaugeController(const bool RealMachine, const  bool show_messages, const bool show_debug_messages  )
: invertedMagnetronGaugeController((RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_IMG_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_IMG_CONFIG,
                                   show_messages, show_debug_messages )
{}
//______________________________________________________________________________
velaInvertedMagnetronGaugeController::~velaInvertedMagnetronGaugeController(){}    //dtor

#ifdef BUILD_DLL
boost::python::dict velaInvertedMagnetronGaugeController::getImgStateDefinition()
{
    std::map< VELA_ENUM::IMG_STATE,  std::string  > m;

    m[ VELA_ENUM::IMG_STATE::IMG_BAD    ] = ENUM_TO_STRING( VELA_ENUM::IMG_STATE::IMG_BAD     ); // ENUM_TO_STRING MACRO in structs.h
    m[ VELA_ENUM::IMG_STATE::IMG_GOOD   ] = ENUM_TO_STRING( VELA_ENUM::IMG_STATE::IMG_GOOD    );
    m[ VELA_ENUM::IMG_STATE::IMG_ERROR  ] = ENUM_TO_STRING( VELA_ENUM::IMG_STATE::IMG_ERROR   );
    return enumStringMapToPythonDict( m );
}
//______________________________________________________________________________
boost::python::dict velaInvertedMagnetronGaugeController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
//______________________________________________________________________________
std::vector< std::vector< double > > velaInvertedMagnetronGaugeController::monitorForNCounts_Py( const std::string & name, int numcounts )
{
    return monitorForNCounts( name, numcounts );
}
//______________________________________________________________________________
double velaInvertedMagnetronGaugeController::getImgP_Py( const std::string & name )
{
    return getImgP( name );
}
//______________________________________________________________________________
VELA_ENUM::IMG_STATE velaInvertedMagnetronGaugeController::getImgState_Py( const std::string & name )
{
    return getImgState( name );
}
#endif //BUILD_DLL


