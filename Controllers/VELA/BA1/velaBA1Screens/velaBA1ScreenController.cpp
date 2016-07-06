
#include "velaBA1ScreenController.h"
//stl
#include <iostream>
#include <time.h>


velaBA1ScreenController::velaBA1ScreenController( const std::string configFileLocation1,
                                    const std::string configFileLocation2, const bool show_messages, const bool show_debug_messages )
:screenController( configFileLocation1, configFileLocation2, show_messages, show_debug_messages )
{}
//__________________________________________________________________________
velaBA1ScreenController::velaBA1ScreenController( const bool show_messages, const bool show_debug_messages )
:screenController( "C:\\Users\\wln24624\\Documents\\VELA\\Software\\c++\\Config\\velaBA1ComplexScreens.config",
                   "C:\\Users\\wln24624\\Documents\\VELA\\Software\\c++\\Config\\velaBA1SimpleScreens.config",
                    show_messages, show_debug_messages )
{}
//_____________________________________________________________________________
velaBA1ScreenController::~velaBA1ScreenController(){}

#ifdef BUILD_DLL
boost::python::dict velaBA1ScreenController::getScreenStateDefinition()
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
boost::python::dict  velaBA1ScreenController::getILockStates_Py( std::string name )
{
    return enumMapToPythonDict( getILockStates( name ) );
}
//________________________________________________________________________________________________
void velaBA1ScreenController::Screen_Out_Py( const std::string name )
{
    return Screen_Out( name );
}
//________________________________________________________________________________________________
void velaBA1ScreenController::Screen_In_Py( const std::string name )
{
    return Screen_In( name );
}
//___________________________________________________________________________________________________
void velaBA1ScreenController::Screen_Move_Py( const std::string name, const std::string position )
{
    return Screen_Move( name,position );
}
//____________________________________________________________________________________________________
void velaBA1ScreenController::Screen_Stop_Py( const std::string name )
{
    return Screen_Stop( name );
}
//____________________________________________________________________________________________________
void velaBA1ScreenController::All_Out_Py()
{
    return All_Out();
}
//______________________________________________________________________________________________________
bool velaBA1ScreenController::IsOut_Py( const std::string name )
{
    return IsOut( name );
}
//__________________________________________________________________________________________________________
bool velaBA1ScreenController::IsIn_Py( const std::string name )
{
    return IsIn( name );
}
//____________________________________________________________________________________________________________
bool velaBA1ScreenController::horizontal_disabled_check_Py( const std::string name )
{
    return horizontal_disabled_check( name );
}
//____________________________________________________________________________________________________________
bool velaBA1ScreenController::vertical_disabled_check_Py( const std::string name )
{
    return vertical_disabled_check( name );
}
//___________________________________________________________________________________________________________
void velaBA1ScreenController::controller_move_to_position_Py( const std::string name, const std::string position )
{
    return controller_move_to_position( name, position );
}
//___________________________________________________________________________________
void velaBA1ScreenController::controller_move_to_Py( const std::string name, const std::string V_H, const double position )
{
    return controller_move_to( name, V_H, position );
}
//_______________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA1ScreenController::getComplexHorizontalScreenState_Py( const std::string name )
{
    return getComplexHorizontalScreenState( name );
}
//_______________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA1ScreenController::getComplexVerticalScreenState_Py( const std::string name )
{
    return getComplexVerticalScreenState( name );
}
//______________________________________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA1ScreenController::getSimpleScreenState_Py( const std::string name )
{
    return getSimpleScreenState( name );
}
//_________________________________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA1ScreenController::getScreenState_Py( const std::string name )
{
    return getScreenState( name );
}
//___________________________________________________________________________________________________________
screenStructs::SCREEN_STATE velaBA1ScreenController::getScreenState_Py( const std::string name, const std::string V_H )
{
    return getScreenState( name, V_H );
}
//______________________________________________________________________________________________________________
double velaBA1ScreenController::getComplexScreenHorizontalPosition_Py( const std::string name )
{
    return getComplexScreenHorizontalPosition( name );
}
//_________________________________________________________________________________________________
double velaBA1ScreenController::getComplexScreenVerticalPosition_Py( const std::string name )
{
    return getComplexScreenVerticalPosition( name );
}
//___________________________________________________________________________________________________________
double velaBA1ScreenController::getScreenPosition_Py( const std::string name, const std::string V_H )
{
    return getScreenPosition( name, V_H );
}
//______________________________________________________________________________________________________________
void velaBA1ScreenController::get_info_Py( const std::string name )
{
    return get_info( name );
}
//___________________________________________________________________________________________________________
void velaBA1ScreenController::get_config_values_Py( const std::string name )
{
    return get_config_values( name );
}

#endif // BUILD_DLL



