
#include "velaBA2ScreenController.h"
//stl
#include <iostream>
#include <time.h>


velaBA2ScreenController::velaBA2ScreenController( const std::string configFileLocation1,
                                    const std::string configFileLocation2, const bool show_messages, const bool show_debug_messages )
:screenController( configFileLocation1, configFileLocation2, show_messages, show_debug_messages )
{}
//__________________________________________________________________________
velaBA2ScreenController::velaBA2ScreenController(const bool RealMachine, const bool show_messages, const bool show_debug_messages )
:screenController((RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_BA2_COMPLEX_SCREENS_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_BA2_COMPLEX_SCREENS_CONFIG,
                   (RealMachine==false)? UTL::CONFIG_PATH_VM+UTL::VELA_BA2_SIMPLE_SCREENS_CONFIG : UTL::CONFIG_PATH_VM+UTL::VELA_BA2_SIMPLE_SCREENS_CONFIG,
                    show_messages, show_debug_messages )
{}
//_____________________________________________________________________________
velaBA2ScreenController::~velaBA2ScreenController(){}

#ifdef BUILD_DLL
boost::python::dict velaBA2ScreenController::getScreenStateDefinition()
{
    std::map< screenStructs::SCREEN_STATE, std::string >m;

    m[ screenStructs::SCREEN_STATE::SCREEN_IN ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_IN ); // ENUM_TO_STRING MACRO in structs.h
    m[ screenStructs::SCREEN_STATE::SCREEN_OUT   ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_OUT   );
    m[ screenStructs::SCREEN_STATE::SCREEN_ERROR  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_ERROR  );
    m[ screenStructs::SCREEN_STATE::SCREEN_UNKNOWN  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_UNKNOWN  );
    m[ screenStructs::SCREEN_STATE::SCREEN_H_MIRROR  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_H_MIRROR  );
    m[ screenStructs::SCREEN_STATE::SCREEN_H_50U_SLIT  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_H_50U_SLIT  );
    m[ screenStructs::SCREEN_STATE::SCREEN_H_25U_SLIT  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_H_25U_SLIT  );
    m[ screenStructs::SCREEN_STATE::SCREEN_H_63MM_HOLE  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_H_63MM_HOLE  );
    m[ screenStructs::SCREEN_STATE::SCREEN_H_10MM_HOLE  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_H_10MM_HOLE  );
    m[ screenStructs::SCREEN_STATE::SCREEN_H_YAG  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_H_YAG  );
    m[ screenStructs::SCREEN_STATE::SCREEN_V_YAG ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_V_YAG  );
    m[ screenStructs::SCREEN_STATE::SCREEN_V_SLIT  ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_V_SLIT  );
    m[ screenStructs::SCREEN_STATE::SCREEN_MOVING ] = ENUM_TO_STRING( screenStructs::SCREEN_STATE::SCREEN_MOVING );

    return enumStringMapToPythonDict( m );
}
//________________________________________________________________________________________________
boost::python::dict  velaBA2ScreenController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
//________________________________________________________________________________________________
void velaBA2ScreenController::Screen_Out_Py( const std::string name )
{
    return Screen_Out( name );
}
//________________________________________________________________________________________________
void velaBA2ScreenController::Screen_In_Py( const std::string name )
{
    return Screen_In( name );
}
//___________________________________________________________________________________________________
void velaBA2ScreenController::Screen_Move_Py( const std::string name, const std::string position )
{
    return Screen_Move( name,position );
}
//____________________________________________________________________________________________________
void velaBA2ScreenController::Screen_Stop_Py( const std::string name )
{
    return Screen_Stop( name );
}
//____________________________________________________________________________________________________
void velaBA2ScreenController::All_Out_Py()
{
    return All_Out();
}
//______________________________________________________________________________________________________
bool velaBA2ScreenController::IsOut_Py( const std::string name )
{
    return IsOut( name );
}
//__________________________________________________________________________________________________________
bool velaBA2ScreenController::IsIn_Py( const std::string name )
{
    return IsIn( name );
}
//____________________________________________________________________________________________________________
bool velaBA2ScreenController::horizontal_disabled_check_Py( const std::string name )
{
    return horizontal_disabled_check( name );
}
//____________________________________________________________________________________________________________
bool velaBA2ScreenController::vertical_disabled_check_Py( const std::string name )
{
    return vertical_disabled_check( name );
}
//___________________________________________________________________________________________________________
void velaBA2ScreenController::controller_move_to_position_Py( const std::string name, const std::string position )
{
    return controller_move_to_position( name, position );
}
//___________________________________________________________________________________
void velaBA2ScreenController::controller_move_to_Py( const std::string name, const std::string V_H, const double position )
{
    return controller_move_to( name, V_H, position );
}
//_______________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA2ScreenController::getComplexHorizontalScreenState_Py( const std::string name )
{
    return getComplexHorizontalScreenState( name );
}
//_______________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA2ScreenController::getComplexVerticalScreenState_Py( const std::string name )
{
    return getComplexVerticalScreenState( name );
}
//______________________________________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA2ScreenController::getSimpleScreenState_Py( const std::string name )
{
    return getSimpleScreenState( name );
}
//_________________________________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA2ScreenController::getScreenState_Py( const std::string name )
{
    return getScreenState( name );
}
//___________________________________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA2ScreenController::getScreenState_Py( const std::string name, const std::string V_H )
{
    return getScreenState( name, V_H );
}
//______________________________________________________________________________________________________________
double velaBA2ScreenController::getComplexScreenHorizontalPosition_Py( const std::string name )
{
    return getComplexScreenHorizontalPosition( name );
}
//_________________________________________________________________________________________________
double velaBA2ScreenController::getComplexScreenVerticalPosition_Py( const std::string name )
{
    return getComplexScreenVerticalPosition( name );
}
//___________________________________________________________________________________________________________
double velaBA2ScreenController::getScreenPosition_Py( const std::string name, const std::string V_H )
{
    return getScreenPosition( name, V_H );
}
//______________________________________________________________________________________________________________
void velaBA2ScreenController::get_info_Py( const std::string name )
{
    return get_info( name );
}
//___________________________________________________________________________________________________________
void velaBA2ScreenController::get_config_values_Py( const std::string name )
{
    return get_config_values( name );
}

#endif // BUILD_DLL



