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
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //  s

#include "screenInterface.h"
//djs
#include "configDefinitions.h"
//stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

///Initialising
///Monitors
///Moving
///Checks
///Get Stuff
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//

screenInterface::screenInterface( const std::string & configFileLocation1,
                                  const std::string & configFileLocation2, const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
:configReader( configFileLocation1, configFileLocation2, show_messages_ptr, show_debug_messages_ptr ), interface( show_messages_ptr, show_debug_messages_ptr )

{
    initialise();
}
//_____________________________________________________________________________________
screenInterface::screenInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
:configReader( show_messages_ptr, show_debug_messages_ptr ), interface( show_messages_ptr, show_debug_messages_ptr )

{
    initialise();
}
//__________________________________________________________________________________
screenInterface::~screenInterface()///This is the destructor for the class
{
    for( auto it : continuousMonitorStructs )
    {
        debugMessage("delete screenInterface continuousMonitorStructs entry." );
        delete it;
    }
}
//____________________________________________________________________________________
void screenInterface::initialise()
{
    /// The config file reader
    message("Attempting to read through Screen Config files." );
    configFileRead = configReader.readConfigFiles();
    if( configFileRead )
    {
        ///initialise the objects based on what is read from the config file
        initScreenObjects();
        ///subscribe to the channel ids
        initScreenChids();
        ///start the monitors: set up the callback functions
        if( allChidsInitialised )
            monitorScreens();
        ///The pause allows EPICS to catch up.
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); ///MAGIC NUMBER
    }
}

//_______________________________________________________________________________________
void screenInterface::initScreenObjects()
{
    ScreenObject = configReader.getScreenObjects();
}
//_______________________________________________________________________________________________________________
void screenInterface::initScreenChids()
{
    message( "\n", "Searching for Screen Chids..." );

    ///COMPLEX SCREENS
    for(auto && it1 : ScreenObject.complexObjects )
    {
    ///Add the ILock Channels

        addILockChannels( it1.second.numIlocks, it1.second.pvRoot, it1.second.name, it1.second.iLockPVStructs );

    ///Creating Monitor Channels
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );

    ///Creating Command Channels
        for( auto && it2 : it1.second.pvComStructs )
            addChannel( it1.second.pvRoot, it2.second );
    }
    ///SIMPLE SCREENS
    for( auto && it1 : ScreenObject.simpleObjects )
    {
    ///Add ILock Channels
        addILockChannels( it1.second.numIlocks, it1.second.pvRoot, it1.second.name, it1.second.iLockPVStructs );
    ///Creating Monitor Channels
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );

    ///Creating Command Channels
        for( auto && it2 : it1.second.pvComStructs )
            addChannel( it1.second.pvRoot, it2.second );
    }
    ///SENDING TO EPICS
    int status = sendToEpics( "ca_create_channel", "Found Screen Chids.", "!!!TIMEOUT!!! Not All Screen Chids Found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); ///MAGIC NUMBER
        ///COMPLEX OBJECTS
        for( auto && it1 : ScreenObject.complexObjects )
        {
        ///Checking ILock Channels
        for( auto & it2 : it1.second.iLockPVStructs )
            checkCHIDState(it2.second.CHID, it2.second.pv );

        ///Checking Monitor Channels
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );

        ///Checking Command Channels
            for( auto && it2 : it1.second.pvComStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
        }
        ///SIMPLE OBJECTS
        for( auto && it1 : ScreenObject.simpleObjects )
        {
        ///Checking ILock Channels
        for( auto & it2 : it1.second.iLockPVStructs )
            checkCHIDState(it2.second.CHID, it2.second.pv );

        ///Checking Monitor Channels
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );

        ///Checking Command Channels
            for( auto && it2 : it1.second.pvComStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix );
        }
        message("");
    }

    else if ( status == ECA_NORMAL )
        allChidsInitialised = true; /// interface base class member




}
//________________________________________________________________________________________________________________
void screenInterface::addChannel( const std::string & pvRoot, screenStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(),0,0,0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}
//__________________________________________________________________________________________________________________
void screenInterface::monitorScreens()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for( auto && it1 : ScreenObject.complexObjects )
    {
        monitorIlocks( it1.second.iLockPVStructs, it1.second.iLockStates );
        for( auto && it2 : it1.second.pvMonStructs )
            addToComplexMonitorStructs( continuousMonitorStructs, it2.second, &it1.second );
    }

    for( auto && it1 : ScreenObject.simpleObjects )
    {
        monitorIlocks( it1.second.iLockPVStructs, it1.second.iLockStates );
        for( auto && it2 : it1.second.pvMonStructs )
            addToSimpleMonitorStructs( continuousMonitorStructs, it2.second, &it1.second );
    }

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Screen Monitors", "!!TIMEOUT!! Subscription to Screen monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member


}
//___________________________________________________________________________________________________________________
void screenInterface::addToComplexMonitorStructs( std::vector< screenStructs::monitorStruct * > & cms, screenStructs::pvStruct & pv, screenStructs::COMPLEX_YAG_Object * COMPLEX_YAG  )
{
    cms.push_back( new screenStructs::monitorStruct() );
    cms.back() -> compObj =  COMPLEX_YAG;
    cms.back() -> interface = this;
    cms.back() -> monType = pv.pvType;
    cms.back() -> CHTYPE = pv.CHTYPE;

    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK, screenInterface::staticEntryScreenMonitor, (void*)cms.back(), &pv.EVID );
}
//____________________________________________________________________________________________________________________
void screenInterface::addToSimpleMonitorStructs( std::vector< screenStructs::monitorStruct * > & cms, screenStructs::pvStruct & pv, screenStructs::SIMPLE_YAG_Object * SIMPLE_YAG  )
{
    cms.push_back( new screenStructs::monitorStruct() );
    cms.back() -> simpObj = SIMPLE_YAG;
    cms.back() -> interface = this;
    cms.back() -> monType = pv.pvType;
    cms.back() -> CHTYPE = pv.CHTYPE;

    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK, screenInterface::staticEntryScreenMonitor, (void*)cms.back(), &pv.EVID );
}
//_______________________________________________________________________________________________________________
void screenInterface::staticEntryScreenMonitor( const event_handler_args args )
{
    screenStructs::monitorStruct * ms = reinterpret_cast< screenStructs::monitorStruct*>( args.usr );

    switch ( ms -> CHTYPE )
    {
    case DBR_DOUBLE:
        UpdateDouble( ms, args.dbr);

    case DBR_ENUM:
        UpdateEnum( ms, args.dbr );
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::UpdateDouble( screenStructs::monitorStruct * ms, const void * argsdbr )
{
    double val = *(double*)argsdbr;

    switch( ms -> monType )
    {
    case screenStructs::SCREEN_PV_TYPE::H_RPOS:

        ms -> compObj -> h_position_value = val;
        ms -> interface -> message( ms -> compObj -> name, " Horizontal Position = ", ms -> compObj -> h_position_value );

        if( ms -> compObj -> h_position_value == ms -> compObj -> H_OUT )
            {
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_OUT;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> h_screenState));
            }


        else if( ms -> compObj -> h_position_value == ms -> compObj -> H_MIRROR )
            {
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_H_MIRROR;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> h_screenState));
            }

        else if( ms -> compObj -> h_position_value == ms -> compObj -> H_50U_SLIT )
            {
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_H_50U_SLIT;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> h_screenState));
            }

        else if( ms -> compObj -> h_position_value == ms -> compObj -> H_25U_SLIT )
            {
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_H_25U_SLIT;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> h_screenState));
            }

        else if( ms -> compObj -> h_position_value == ms -> compObj -> H_63MM_HOLE )
            {
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_H_63MM_HOLE;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> h_screenState));
            }

        else if( ms -> compObj -> h_position_value == ms -> compObj -> H_10MM_HOLE )
            {
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_H_10MM_HOLE;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> h_screenState));
            }

        else
           {
            ms -> interface -> message("");
            ms -> compObj -> h_screenState = screenStructs::SCREEN_STATE::SCREEN_UNKNOWN;
           }

        break;


    case screenStructs::SCREEN_PV_TYPE::V_RPOS:

        ms -> compObj -> v_position_value = val;
        ms -> interface -> message( ms -> compObj -> name, " Vertical Position = ", ms -> compObj -> v_position_value );

        if( ms -> compObj -> v_position_value == ms -> compObj -> V_OUT )
            {
            ms -> compObj -> v_screenState = screenStructs::SCREEN_STATE::SCREEN_OUT;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> v_screenState));
            }

        else if( ms -> compObj -> v_position_value == ms -> compObj -> V_YAG )
            {
            ms -> compObj -> v_screenState = screenStructs::SCREEN_STATE::SCREEN_V_YAG;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> v_screenState));
            }

        else if( ms -> compObj -> v_position_value == ms -> compObj -> V_SLIT )
            {
            ms -> compObj -> v_screenState = screenStructs::SCREEN_STATE::SCREEN_V_SLIT;
            ms -> interface -> message(ENUM_TO_STRING(ms -> compObj -> v_screenState));
            }

        else
            {
                ms -> interface -> message("");
            ms -> compObj -> v_screenState = screenStructs::SCREEN_STATE::SCREEN_UNKNOWN;
            }


        break;
    }

}
//_________________________________________________________________________________________________________________
void screenInterface::UpdateEnum( screenStructs::monitorStruct * ms, const void * argsdbr )
{
    unsigned short val = *(unsigned short*)argsdbr;

     switch ( ms -> monType )
    {
       case screenStructs::SCREEN_PV_TYPE::Sta:

        ms -> simpObj -> position_value = val;

        if( ms -> simpObj -> position_value == ms -> simpObj -> OUT )
        {
            ms -> simpObj -> screenState = screenStructs::SCREEN_STATE::SCREEN_OUT;
            ms -> interface -> message( ms -> simpObj -> name, " = ",ENUM_TO_STRING(ms -> simpObj -> screenState)  );

        }
        else if ( ms -> simpObj -> position_value == ms -> simpObj -> IN )
        {
            ms -> simpObj -> screenState = screenStructs::SCREEN_STATE::SCREEN_IN;
            ms -> interface -> message( ms -> simpObj -> name, " = ",ENUM_TO_STRING(ms -> simpObj -> screenState) );

        }
        else
        {
            ms -> simpObj  -> screenState = screenStructs::SCREEN_STATE::SCREEN_UNKNOWN;
            ms -> interface -> message( ms -> simpObj -> name," = ",ENUM_TO_STRING(ms -> simpObj -> screenState) );
        }
        break;

        case screenStructs::SCREEN_PV_TYPE::H_PROT01:

        if( val == 0 )
        {
            ms -> compObj -> horizontal_disabled = false;
            ms -> interface -> message( ms -> compObj -> name, " Horizontal Disabled = FALSE" );
        }

        else if( val == 1 )
        {
            ms -> compObj -> horizontal_disabled = true;
            ms -> interface -> message( ms -> compObj -> name, " Horizontal Disabled = TRUE" );
        }

        break;

    case screenStructs::SCREEN_PV_TYPE::V_PROT01:

        if( val == 0 )
        {
            ms -> compObj -> vertical_disabled = false;
            ms -> interface -> message( ms -> compObj -> name, " Vertical Disabled = FALSE" );

        }

        else if( val == 1 )
        {
            ms -> compObj -> vertical_disabled = true;
            ms -> interface -> message( ms -> compObj -> name, " Vertical Disabled = TRUE" );
        }

        break;

    case screenStructs::SCREEN_PV_TYPE::PROT03:

        if( val == 0 )
        {
            ms -> compObj -> position_error = false;
            ms -> interface -> message( ms -> compObj -> name, " Position Error = FALSE" );
        }

        else if( val == 1 )
        {
            ms -> compObj -> position_error = true;
            ms -> interface -> message( ms -> compObj -> name, " Position Error = TRUE" );
        }

        break;

    case screenStructs::SCREEN_PV_TYPE::PROT05:

         if( val == 0 )
         {
            ms -> compObj -> home_error = false;
            ms -> interface -> message( ms -> compObj -> name, " Home Error = FALSE" );
        }

        else if( val == 1 )
        {
            ms -> compObj -> home_error = true;
            ms -> interface -> message( ms -> compObj -> name, " Home Error = TRUE" );
//        }

    case screenStructs::SCREEN_PV_TYPE::H_RPWRLOSS:

        ms -> compObj -> H_RPWRLOSS = val;

        if( val == 0 )
            ms -> interface -> message(ms -> compObj -> name, " H_RPWRLOSS  = Power Lost Home");

        else if( val == 1 )
            ms -> interface -> message(ms -> compObj -> name, " H_RPWRLOSS  = Homeing");

        else if( val == 2 )
            ms -> interface -> message(ms -> compObj -> name, " H_RPWRLOSS  = Homed");

        break;

    case screenStructs::SCREEN_PV_TYPE::V_RPWRLOSS:

        ms -> compObj -> V_RPWRLOSS = val;

        if( val == 0 )
            ms -> interface -> message(ms -> compObj -> name, " V_RPWRLOSS  = Power Lost Home");

        else if( val == 1 )
            ms -> interface -> message(ms -> compObj -> name, " V_RPWRLOSS  = Homeing");

        else if( val == 2 )
            ms -> interface -> message(ms -> compObj -> name, " V_RPWRLOSS  = Homed");

        break;
    }
    }
}
//__________________________________________________________________________________________________________
void screenInterface::Screen_Out( const std::string & name )
{
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            if( horizontal_disabled_check( name ) )
            {
                move_to_position( name, "V_OUT" );
                time_t start = time(0);
                int timeout = 100;
                while( getComplexVerticalScreenState( name ) != screenStructs::SCREEN_STATE::SCREEN_OUT )
                {
                    if( difftime( time(0), start ) >= timeout )
                    {
                        debugMessage( name, " has timed out");
                        return;
                    }
                    else
                        debugMessage("Screen Moving");
                        continue;
                }
                debugMessage( name, " = ", ENUM_TO_STRING(getComplexVerticalScreenState( name )) );
                std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                debugMessage("Other screens now safe to move");
            }
            else if( vertical_disabled_check( name ) )
            {
                move_to_position( name, "H_OUT" );
                time_t start = time(0);
                int timeout = 100;
                while( getComplexHorizontalScreenState( name ) != screenStructs::SCREEN_STATE::SCREEN_OUT )
                {
                    if( difftime( time(0), start ) >= timeout )
                    {
                        debugMessage( name, " has timed out");
                        return;
                    }
                    else
                        continue;
                }
                debugMessage( name, " = ", ENUM_TO_STRING(getComplexHorizontalScreenState( name )) );
                std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                debugMessage("Other screens now safe to move");
            }
            else
                debugMessage( "Both horizontal and vertical movement enabled, ", name, " must already be out" );
        }
        else if( Is_simple( name ) )
        {
            Out( name );
            time_t start = time(0);
            int timeout = 100;
            while( getSimpleScreenState( name ) != screenStructs::SCREEN_STATE::SCREEN_OUT )
            {
                if( difftime( time(0), start ) >= timeout )
                {
                    debugMessage( name, " has timed out");
                    return;
                }
                else
                    continue;
            }
            debugMessage( name, " = ", ENUM_TO_STRING(getSimpleScreenState( name )) );
            std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
            debugMessage("Other screens now safe to move");
        }
    }
    else
        debugMessage( name, " does not exist" );
}

//___________________________________________________________________________________________________________
void screenInterface::Screen_In( const std::string & name )
{
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            if( vertical_disabled_check( name ) )
            {
                move_to_position( name, "H_OUT" );
                time_t start = time(0);
                int timeout = 100;
                while ( vertical_disabled_check( name ) )
                {
                    if( difftime( time(0), start ) >= timeout )
                    {
                        debugMessage( name, " has timed out");
                        return;
                    }
                    else
                    continue;
                }
                move_to_position( name, "V_YAG" );
                start = time(0);
                timeout = 100;
                while( getComplexVerticalScreenState( name ) != screenStructs::SCREEN_STATE::SCREEN_V_YAG )
                {
                    if( difftime( time(0), start ) >= timeout )
                    {
                        debugMessage( name, " has timed out");
                        return;
                    }
                    else
                    continue;
                }
                debugMessage( name, " = ", ENUM_TO_STRING(getComplexVerticalScreenState( name )) );
                std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                debugMessage("Other screens now safe to move");
            }
            else
                move_to_position( name, "V_YAG" );
        }
        else if( Is_simple( name ) )
        {
            In( name );
            time_t start = time(0);
            int timeout = 100;
            while( getSimpleScreenState( name ) != screenStructs::SCREEN_STATE::SCREEN_IN )
            {
                if( difftime( time(0), start ) >= timeout )
                {
                    debugMessage( name, " has timed out");
                    return;
                }
                else
                continue;
            }
            debugMessage( name, " = ", ENUM_TO_STRING(getSimpleScreenState( name )) );
            std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
            debugMessage("Other screens now safe to move");
        }
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
    else
        debugMessage( name, " does not exist" );
}
//__________________________________________________________________________________________________________
void screenInterface::Screen_Move( const std::string & name, const std::string & position )
{
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            if( position == "V_SLIT"|| position == "V_YAG"|| position == "V_OUT" )
            {
                debugMessage(position);
                if( vertical_disabled_check( name ) )
                {
                    move_to_position( name, "H_OUT" );
                    time_t start = time(0);
                    int timeout = 100;
                    while ( ENUM_TO_STRING(ScreenObject.complexObjects[ name ].h_screenState) != "H_OUT" )
                    {
                        if( difftime( time(0), start ) >= timeout )
                        {
                            debugMessage( name, " has timed out");
                            return;
                        }
                        else if( !vertical_disabled_check( name ) )
                        {
                            break;
                        }
                        else
                            continue;
                    }
                    std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                    debugMessage("Screen now moving to ", position);
                    move_to_position( name, position );
                    start = time(0);
                    timeout = 100;
                    while( ENUM_TO_STRING(getComplexVerticalScreenState( name )) != position )
                    {
                        if( difftime( time(0), start ) >= timeout )
                        {
                            debugMessage( name, " has timed out");
                            return;
                        }
                        else
                            continue;
                    }
                    debugMessage( name, " = ", ENUM_TO_STRING(getComplexVerticalScreenState( name )));
                    std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                    debugMessage("Other screens now safe to move");

                }
                else
                    move_to_position( name, position );
                    time_t start = time(0);
                    int timeout = 100;
                    while( ENUM_TO_STRING(getComplexVerticalScreenState( name )) != position )
                    {
                        if( difftime( time(0), start ) >= timeout )
                        {
                            debugMessage( name, " has timed out");
                            return;
                        }
                        else
                            continue;
                    }
                    debugMessage( name, " = ", ENUM_TO_STRING(getComplexVerticalScreenState( name )));
                    std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                    debugMessage("Other screens now safe to move");
            }
            else if( position == "H_SLIT"|| position == "H_MIRROR"|| position == "H_50U_SLIT"|| position == "H_25U_SLIT"|| position == "H_63MM_HOLE"|| position == "H_10MM_HOLE"||position =="H_OUT" )
            {
              if( horizontal_disabled_check( name ) )
                {
                    move_to_position( name, "V_OUT" );
                    time_t start = time(0);
                    int timeout = 100;
                    while ( ENUM_TO_STRING(ScreenObject.complexObjects[ name ].h_screenState) != "V_OUT" )
                    {
                        if( difftime( time(0), start ) >= timeout )
                        {
                            debugMessage( name, " has timed out");
                            return;
                        }
                        else if( !horizontal_disabled_check( name ) )
                        {
                            break;
                        }
                        else
                            continue;
                    }
                    std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                    debugMessage("Screen now moving to ", position);
                    move_to_position( name, position );
                    start = time(0);
                    timeout = 100;
                    while( ENUM_TO_STRING(getComplexHorizontalScreenState( name )) != position )
                    {
                        if( difftime( time(0), start ) >= timeout )
                        {
                            debugMessage( name, " has timed out");
                            return;
                        }
                        else
                            continue;
                    }
                    debugMessage( name, " = ", ENUM_TO_STRING(getComplexHorizontalScreenState( name )));
                    std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                    debugMessage("Other screens now safe to move");

                }
                else
                    move_to_position( name, position );
                    time_t start = time(0);
                    int timeout = 100;
                    while( ENUM_TO_STRING(getComplexHorizontalScreenState( name )) != position )
                    {
                        if( difftime( time(0), start ) >= timeout )
                        {
                            debugMessage( name, " has timed out");
                            return;
                        }
                        else
                            continue;
                    }
                    debugMessage( name, " = ", ENUM_TO_STRING(getComplexHorizontalScreenState( name )));
                    std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
                    debugMessage("Other screens now safe to move");
            }
            else
                debugMessage(position," is not a valid position to move to" );
        }
        else if( Is_simple( name ) )
            debugMessage( name, " can only be moved in or out, there are no other positions available to move to" );
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
    else
        debugMessage( name, " does not exist" );
}
//___________________________________________________________________________________________________________
void screenInterface::All_Out()
{
    for( auto && it : ScreenObject.complexObjects )
    {
        Screen_Out( it.first );
        std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
    }
    for( auto && it: ScreenObject.simpleObjects )
    {
        Screen_Out( it.first );
        std::this_thread::sleep_for(std::chrono::seconds( 3 ));///MAGIC NUMBER
    }
}
//____________________________________________________________________________________________________________
void screenInterface::Out( const std::string & name )
{
    if( screenExists( name ) )
        if( IsIn( name ) )
            toggleScreen( ScreenObject.simpleObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::Off ].CHTYPE,
                         ScreenObject.simpleObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::Off ].CHID,
                         "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO SCREEN OUT",
                         "!!TIMEOUT!! FAILED TO SEND SCREEN OUT");
        else
            debugMessage( name, " is already out" );
    else
        debugMessage( name, " does not exist" );
}
//__________________________________________________________________________________________________________________
void screenInterface::In( const std::string & name )
{
    if( screenExists( name ) )
        if( IsOut( name ) )
            toggleScreen( ScreenObject.simpleObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::On ].CHTYPE,
                         ScreenObject.simpleObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::On ].CHID,
                         "!!TIMEOUT!! FAILED TO SEND ACTIVATE TO SCREEN IN",
                         "!!TIMEOUT!! FAILED TO SEND SCREEN IN");
        else
            debugMessage( name, " is aleady in" );
    else
        debugMessage( name, " does not exist" );
}
//__________________________________________________________________________________________________________________
void screenInterface::toggleScreen( chtype & cht, chid & chi, const char * m1, const char * m2 )
{
    int status = caput( cht, chi, EPICS_ACTIVATE, "", m1 );
    if( status == ECA_NORMAL )
        caput( cht, chi, EPICS_SEND, "", m2 );
}
//__________________________________________________________________________________________________________________
void screenInterface::Stop( const std::string & name )
{
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::STOP ].CHTYPE,
              ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::STOP ].CHID,
              EPICS_SEND,"", "!!!TIMEOUT FAILED TO STOP SCREEN!!!");
        }
        else
            debugMessage( "Cannot stop screen ", name );
    }
    else
        debugMessage( name, " does not exist" );
}
//__________________________________________________________________________________________________________________
void screenInterface::move_to( const std::string & name, const std::string & V_H, const double & position )
{
    if( screenExists( name ) )
    {
        if( V_H == "VERTICAL" )
        {
            if( !vertical_disabled_check( name ) )
            {
                    if(position_value_check ( position ) )
                    {
                    caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHTYPE,
                        ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHID,
                        position,"","!!!TIMEOUT FAILED TO MOVE SCREEN!!!" );
                    }

                    else
                        debugMessage("Cannot move screen to that position, only move to less than 155 " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }

        if( V_H == "HORIZONTAL" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                    if(position_value_check ( position ) )
                    {
                    caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                        ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                        position,"","!!!TIMEOUT FAILED TO MOVE SCREEN!!!" );
                    }

                    else
                        debugMessage("Cannot move screen to ",position," , can only move to less than 155 " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
  }
}
//__________________________________________________________________________________________________________________
void screenInterface::move_to_position( const std::string & name, const std::string & position )
{
    if( screenExists( name ) )
    {

        if( position == "V_OUT" )
        {
            if( !vertical_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].V_OUT ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].V_OUT,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].V_OUT, " is this error value 9999 or greater than 155? " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "V_YAG" )
        {
                if( !vertical_disabled_check( name ) )
                {
                    if( position_value_check( ScreenObject.complexObjects[ name ].V_YAG ) )
                    {
                    caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHTYPE,
                        ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHID,
                        ScreenObject.complexObjects[ name ].V_YAG,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                    }
                    else
                        debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].V_YAG, " is this error value 9999 or greater than 155? " );
                }
                else
                    debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "V_SLIT" )
        {
            if( !vertical_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].V_SLIT ) )
                {
                    caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHTYPE,
                        ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::V_MABS ].CHID,
                        ScreenObject.complexObjects[ name ].V_SLIT,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ", ScreenObject.complexObjects[ name ].V_SLIT, " is this error value 9999 or greater than 155? " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
       }
       if( position == "H_OUT" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_OUT ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_OUT,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_OUT, " is this error value 9999 or greater than 155? " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "H_MIRROR" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_MIRROR ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_MIRROR,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_MIRROR, " is this error value 9999 or greater than 155? " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "H_50U_SLIT" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_50U_SLIT ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_50U_SLIT,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_50U_SLIT, " is this error value 9999 or greater than 155? " );
             }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "H_25U_SLIT" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_25U_SLIT ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_25U_SLIT,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_25U_SLIT, " is this error value 9999 or greater than 155? " );
             }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "H_63MM_HOLE" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_63MM_HOLE ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_63MM_HOLE,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_63MM_HOLE, " is this error value 9999 or greater than 155? " );
            }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "H_10MM_HOLE" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_10MM_HOLE ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_10MM_HOLE,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_10MM_HOLE, " is this error value 9999 or greater than 155? " );
             }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
        if( position == "H_SLIT" )
        {
            if( !horizontal_disabled_check( name ) )
            {
                if( position_value_check( ScreenObject.complexObjects[ name ].H_SLIT ) )
                {
                caput( ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHTYPE,
                    ScreenObject.complexObjects[ name ].pvComStructs[ screenStructs::SCREEN_PV_TYPE::H_MABS ].CHID,
                    ScreenObject.complexObjects[ name ].H_SLIT,"", "!!!TIMEOUT FAILED TO MOVE SCREEN!!!");
                }
                else
                    debugMessage("Screen position not valid, found ",ScreenObject.complexObjects[ name ].H_SLIT, " is this error value 9999 or greater than 155? " );
             }
            else
                debugMessage(ScreenObject.complexObjects[ name ].name, " cannot move in this direction, movement disabled " );
        }
  }
}
//__________________________________________________________________________________________________________________
bool screenInterface::screenExists( const std::string & name )
{
    bool ret = false;

    auto it = ScreenObject.complexObjects.find( name );
        if( it!= ScreenObject.complexObjects.end() )
            ret = true;
    auto it1 = ScreenObject.simpleObjects.find( name );
        if( it1!= ScreenObject.simpleObjects.end() )
            ret = true;
    return ret;
}
//_____________________________________________________________________________________________________________
bool screenInterface::Is_complex( const std::string & name )
{
    bool ret = false;
    if( screenExists( name ) )
    {
        auto it = ScreenObject.complexObjects.find( name );
            if( it!= ScreenObject.complexObjects.end() )
                ret = true;
    }
    else
        debugMessage( name, " does not exist" );

    return ret;
}
//_____________________________________________________________________________________________________________
bool screenInterface::Is_simple( const std::string & name )
{
    bool ret = false;
    if( screenExists( name ) )
    {
        auto it = ScreenObject.simpleObjects.find( name );
            if( it != ScreenObject.simpleObjects.end() )
                ret = true;
    }
    else
        debugMessage( name, " does not exist" );

    return ret;
}
//______________________________________________________________________________________________________________
bool screenInterface::vertical_disabled_check( const std::string & name )
{
    bool ret = true;
    if( screenExists( name ) )
    {
        if( ScreenObject.complexObjects[name].vertical_disabled == true )
        {
            ret =  true;
        }
        else
        {
            ret = false;
        }
        return ret;
    }
    else
        debugMessage(name, "does not exist" );


    return ret;

}
//______________________________________________________________________________________________________________
bool screenInterface::horizontal_disabled_check( const std::string & name )
{
    bool ret = true;
    if( screenExists( name ) )
    {
        if( ScreenObject.complexObjects[name].horizontal_disabled == true )
        {
            ret =  true;
        }
        else
        {
            ret = false;
        }
        return ret;
    }
    else
        debugMessage(name, "does not exist" );


    return ret;

}
//_________________________________________________________________________________________________________________
bool screenInterface::position_value_check( double val )
{
    bool ret = false;
    if( val == 9999.99 )
    {
        ret =  false;
        debugMessage(val, " is not a valid position to move to");
    }
    else if( val > 155 )
    {
        ret = false;
        debugMessage(val, " is not a valid position to move to");
    }
    else
        ret = true;

    return ret;

}
//_______________________________________________________________________________________________________________
bool screenInterface::IsOut( const std::string & name )
{
    bool ret = false;
    if( screenExists( name ) )
        if( ScreenObject.simpleObjects[name].screenState == screenStructs::SCREEN_STATE::SCREEN_OUT )
            ret = true;
        if( ret )
            debugMessage( name, " is out");
        else
            debugMessage( name, " is NOT out");

    return ret;
}
//___________________________________________________________________________________________________________________
bool screenInterface::IsIn( const std::string & name )
{
    bool ret = false;
    if( screenExists( name ) )
        if( ScreenObject.simpleObjects[name].screenState == screenStructs::SCREEN_STATE::SCREEN_IN )
            ret = true;
        if( ret )
            debugMessage( name, " is in");
        else
            debugMessage( name, " is NOT in");

    return ret;
}
//____________________________________________________________________________________________________________
void screenInterface::getScreenNames( std::vector< std::string > & screenNames )
{
    screenNames.clear();
    for( auto const & it : ScreenObject.complexObjects )
        screenNames.push_back( it.first );
    for( auto const & it2 : ScreenObject.simpleObjects )
        screenNames.push_back( it2.first );
}
//_____________________________________________________________________________________________________
double screenInterface::getComplexScreenHorizontalPosition( const std::string &name )
{
        return ScreenObject.complexObjects[ name ].h_position_value;
}
//_____________________________________________________________________________________________________
double screenInterface::getComplexScreenVerticalPosition( const std::string &name )
{
    return ScreenObject.complexObjects[ name ].v_position_value;
}
//______________________________________________________________________________________________________
double screenInterface::getScreenPosition( const std::string &name, const std::string &V_H )
{
    double position = 9999.99;
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            if( V_H == "VERTICAL" )
            {
                position = getComplexScreenVerticalPosition( name );
                debugMessage(position);
            }
            else if( V_H == "HORIZONTAL" )
            {
                position = getComplexScreenHorizontalPosition( name );
                debugMessage(position);
            }
            else
                debugMessage( V_H," is not valid. Enter either VERTICAL or HORIZONTAL" );
        }
        else if( Is_simple( name ) )
            debugMessage(name, " does not have a vertical or horizontal position, it is either in or out, use getScreenState to get the state" );
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
    else
        debugMessage(name, " does not exist" );

    return position;
}
//____________________________________________________________________________________________________________________
screenStructs::SCREEN_STATE screenInterface::getComplexHorizontalScreenState ( const std::string &name )
{
    screenStructs::SCREEN_STATE r = screenStructs::SCREEN_STATE::SCREEN_ERROR;
    if( screenExists( name ) )
        r = ScreenObject.complexObjects[ name ].h_screenState;
    return r;
}
//___________________________________________________________________________________________________________________
screenStructs::SCREEN_STATE screenInterface::getComplexVerticalScreenState( const std::string &name )
{
    screenStructs::SCREEN_STATE r = screenStructs::SCREEN_STATE::SCREEN_ERROR;
    if( screenExists( name ) )
        r = ScreenObject.complexObjects[ name ].v_screenState;
    return r;
}
//____________________________________________________________________________________________________________________
screenStructs::SCREEN_STATE screenInterface::getSimpleScreenState ( const std::string &name )
{
    screenStructs::SCREEN_STATE r = screenStructs::SCREEN_STATE::SCREEN_ERROR;
    if( screenExists( name ) )
        r = ScreenObject.simpleObjects[ name ].screenState;
    else
        debugMessage( name, " does not exist" );
    return r;
}
//________________________________________________________________________________________________________________
screenStructs::SCREEN_STATE screenInterface::getScreenState( const std::string &name, const std::string &V_H )
{
    screenStructs::SCREEN_STATE State = screenStructs::SCREEN_STATE::SCREEN_ERROR;
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            if(V_H == "VERTICAL" )
            {
                State = getComplexVerticalScreenState( name );
                message(name, " Screen State = ",ENUM_TO_STRING(State));

            }
            else if(V_H == "HORIZONTAL" )
            {
                State = getComplexHorizontalScreenState( name );
                message(name, " Screen State = ",ENUM_TO_STRING(State));

            }
            else
                debugMessage( V_H," is not valid. Enter either VERTICAL or HORIZONTAL" );
        }
        else if( Is_simple( name ) )
        {
            debugMessage(name, " does not have vertical or horizontal components, just enter ",name, " into the fucntion" );
        }
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
     else
        debugMessage( name, " does not exist" );

    return State;
}
//__________________________________________________________________________________________________________________
screenStructs::SCREEN_STATE screenInterface::getScreenState( const std::string &name )
{
    screenStructs::SCREEN_STATE State = screenStructs::SCREEN_STATE::SCREEN_ERROR;
    if( screenExists( name ) )
    {
        if( Is_simple( name ) )
        {
            State = getSimpleScreenState( name );
            message(name, " Screen State = ",ENUM_TO_STRING(State));
        }
        else if( Is_complex( name ) )
        {
            debugMessage( name, " does not have a vertical or horizontal component, just enter ",name );
        }
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
    else
        debugMessage( name, " does not exist" );

    return State;
}
//___________________________________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > screenInterface::getILockStates( const std::string & name )
{
    if( entryExists( ScreenObject.simpleObjects, name ) )
    {   std::cout << "Entry Exists getILockStates" << std::endl;
        return ScreenObject.simpleObjects[ name ].iLockStates;
    }

    else if( entryExists( ScreenObject.complexObjects, name ) )
    {   std::cout << "Entry Exists getILockStates" << std::endl;
        return ScreenObject.complexObjects[ name ].iLockStates;
    }

    else
    {
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r = {{VELA_ENUM::ILOCK_NUMBER::ILOCK_ERR, VELA_ENUM::ILOCK_STATE::ILOCK_ERROR}};
        return r;
    }

}
////__________________________________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  screenInterface::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string  > r;

    if( entryExists( ScreenObject.simpleObjects, name ) )
    {
        std::cout << "Entry Exists getILockStatesStr" << std::endl;
        for( auto it : ScreenObject.simpleObjects[ name ].iLockStates )
        {
            message( "hello", it.second  );
            r[ it.first ] = ENUM_TO_STRING( it.second );
        }
    }

    if( entryExists( ScreenObject.complexObjects, name ) )
    {
        std::cout << "Entry Exists getILockStatesStr" << std::endl;
        for( auto it : ScreenObject.complexObjects[ name ].iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    }

    else
        r[ VELA_ENUM::ILOCK_NUMBER::ILOCK_ERR ] = ENUM_TO_STRING( VELA_ENUM::ILOCK_STATE::ILOCK_ERROR );
    return r;

}
//_________________________________________________________________________________________________________________
void screenInterface::get_info( const std::string & name )
{
    if( screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            message( "Name = ", ScreenObject.complexObjects[ name ].name );
            message( "PV Root = ", ScreenObject.complexObjects[ name ].pvRoot );
            message( "Horizontal Position = ", ScreenObject.complexObjects[ name ].h_position_value );
            message( "Horizontal State = ", ENUM_TO_STRING(ScreenObject.complexObjects[ name ].h_screenState) );
            message( "Vertical Position = ", ScreenObject.complexObjects[ name ].v_position_value );
            message( "Vertical State = ", ENUM_TO_STRING(ScreenObject.complexObjects[ name ].v_screenState) );
            if(ScreenObject.complexObjects[ name ].vertical_disabled)
                message( "Vertical Disabled = TRUE" );
            else if(!ScreenObject.complexObjects[ name ].vertical_disabled)
                message( "Vertical Disabled = FALSE" );
            else
                debugMessage( "Vertical Disabled = ", ScreenObject.complexObjects[ name ].vertical_disabled );
            if(ScreenObject.complexObjects[ name ].horizontal_disabled)
                message( "Horizontal Disabled = TRUE" );
            else if(!ScreenObject.complexObjects[ name ].horizontal_disabled)
                message( "Horizontal Disabled = FALSE" );
            else
                debugMessage( "Horizontal Disabled = ", ScreenObject.complexObjects[ name ].horizontal_disabled );
            if(ScreenObject.complexObjects[ name ].position_error)
                message( "Position Error = TRUE" );
            else if(!ScreenObject.complexObjects[ name ].position_error)
                message( "Position Error = FALSE" );
            else
                debugMessage( "Position Error = ", ScreenObject.complexObjects[ name ].position_error );
            if(ScreenObject.complexObjects[ name ].home_error)
                message( "Home Error = TRUE" );
            else if(!ScreenObject.complexObjects[ name ].home_error)
                message( "Home Error = FALSE" );
            else
                debugMessage( "Home Error = ", ScreenObject.complexObjects[ name ].home_error );
            message( "Horizontal Reverse Power Loss = ", ScreenObject.complexObjects[ name ].H_RPWRLOSS );
            message( "Vertical Reverse Power Loss = ", ScreenObject.complexObjects[ name ].V_RPWRLOSS );
        }
        else if( Is_simple( name ) )
        {
            message( "Name = ", ScreenObject.simpleObjects[ name ].name );
            message( "PV Root = ", ScreenObject.simpleObjects[ name ].pvRoot );
            message( "Screen State = ", ENUM_TO_STRING(ScreenObject.simpleObjects[ name ].screenState) );
            message( "Position Value = ", ScreenObject.simpleObjects[ name ].position_value );
        }
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
    else
        debugMessage(name, " does not exist");
}
//__________________________________________________________________________________________________________________
void screenInterface::get_config_values( const std::string & name )
{
    if(screenExists( name ) )
    {
        if( Is_complex( name ) )
        {
            message( "Name = ", ScreenObject.complexObjects[ name ].name );
            message( "PV Root = ", ScreenObject.complexObjects[ name ].pvRoot );
            message( "H_MIRROR = ", ScreenObject.complexObjects[ name ].H_MIRROR );
            message( "H_50U_SLIT = ", ScreenObject.complexObjects[ name ].H_50U_SLIT );
            message( "H_25U_SLIT = ", ScreenObject.complexObjects[ name ].H_25U_SLIT );
            message( "H_63MM_HOLE = ", ScreenObject.complexObjects[ name ].H_63MM_HOLE );
            message( "H_10MM_HOLE = ", ScreenObject.complexObjects[ name ].H_10MM_HOLE );
            message( "H_SLIT = ", ScreenObject.complexObjects[ name ].H_SLIT );
            message( "V_YAG = ", ScreenObject.complexObjects[ name ].V_YAG );
            message( "V_SLIT = ", ScreenObject.complexObjects[ name ].V_SLIT );
            message( "H_OUT = ", ScreenObject.complexObjects[ name ].H_OUT );
            message( "V_OUT = ", ScreenObject.complexObjects[ name ].V_OUT );
            message( "numIlocks = ", ScreenObject.complexObjects[ name ].numIlocks );
        }
        else if( Is_simple( name ) )
        {
            message( "Name = ", ScreenObject.simpleObjects[ name ].name );
            message( "PV Root = ", ScreenObject.simpleObjects[ name ].pvRoot );
            message( "IN = ", ScreenObject.simpleObjects[ name ].IN );
            message( "OUT = ", ScreenObject.simpleObjects[ name ].OUT );
            message( "numIlocks = ", ScreenObject.simpleObjects[ name ].numIlocks );
        }
        else
            debugMessage("Cannot find ",name," in complex or simple screens lists, but screen exists, there is a problem!" );
    }
    else
        debugMessage(name, " does not exist");
}
//__________________________________________________________________________________________________________________
