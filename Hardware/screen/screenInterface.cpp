/*
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
//    along with VELA-CLARA-Controllers.  If not, see <http://www.gnu.org/licenses/>. //
//
//  Author:      ADB/DJS
//  Last edit:   07-08-2018
//  FileName:    screenInterface.h
//  Description:
//
//
//*/
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

screenInterface::screenInterface( const std::string & conf1,
                                  const bool& show_messages_ptr,
                                  const bool& show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const bool startVirtualMachine,
                                  const HWC_ENUM::MACHINE_AREA myMachineArea ):
configReader( conf1, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine ),
interface( show_messages_ptr, show_debug_messages_ptr, shouldStartEPICs ),
myMachineArea(myMachineArea)
{
    initialise();
}
//__________________________________________________________________________________
screenInterface::~screenInterface()///This is the destructor for the class
{
    //    debugMessage( "magnetInterface DESTRUCTOR CALLED");
//    for (auto && it : screenDualMoveStructsMap )
//    {
//        debugMessage("in screenInterface: delete screenDualMove thread ", it.first );
//        it.second.thread->join();
//        delete it.second.thread;
//    }
    killILockMonitors();
    for( auto && it : continuousMonitorStructsDEV )
    {
        debugMessage("delete screenInterface continuousMonitorStructs entry." );
        delete it;
    }
}
//____________________________________________________________________________________
void screenInterface::initialise()
{
    /// The config file reader
    message("Attempting to read Screen Config file." );
    configFileRead = configReader.readConfig();
    UTL::STANDARD_PAUSE;
    if( configFileRead )
    {
        message("Screen Config file read, getting screens" );
        ///initialise the objects based on what is read from the config file
        if( initScreenObjects() )
        {
            if( shouldStartEPICs )
            {
                message("Screen abjects acquired, starting EPICS" );
                ///subscribe to the channel ids
                initScreenChids();
                ///start the monitors: set up the callback functions
                //if( allChidsInitialised )
                monitorScreens();
                ///The pause allows EPICS to catch up.
                UTL::PAUSE_500;
            }
            else
                message("Screen abjects acquired, not starting EPICS" );
        }
        else
            message("Failed to get Screen objects" );
    }
    else
        message("Failed to read Screen Config file" );
    initIsLockedMap();
}
//_______________________________________________________________________________________
void screenInterface::initIsLockedMap()
{// this map is used to define if a screen has been locked by this app.
    for( auto && it: allScreentData)
    {
        isLockedMap[it.first] = false;
        debugMessage("Added ", it.first, " to locked map ");
    }
}
//_______________________________________________________________________________________
bool screenInterface::initScreenObjects()
{
    return configReader.getScreenObjects( allScreentData );
}
//_______________________________________________________________________________________________________________
void screenInterface::initScreenChids()
{
    message( "\n", "Searching for Screen Chids..." );

    for(auto && scr_IT : allScreentData ) // iterate over all screens
    {
        addILockChannels(scr_IT.second.numIlocks, scr_IT.second.pvRoot, scr_IT.first, scr_IT.second.iLockPVStructs);
        message(scr_IT.second.pvRoot);
        // iterate over all screen pvCom / pvMon structs,
        for( auto && it2 : scr_IT.second.pvComStructs )
        {
            addChannel( scr_IT.second.pvRoot, it2.second );
        }
        for( auto && it2 : scr_IT.second.pvMonStructs  )
        {
            addChannel( scr_IT.second.pvRoot, it2.second );
        }
    }
    ///SENDING TO EPICS
    int status = sendToEpics( "ca_create_channel", "Found Screen Chids.", "!!!TIMEOUT!!! Not All Screen Chids Found." );

    if( status == ECA_TIMEOUT )
    {
        message("ERROR ECA_TIMEOUT");
        for(auto && scr_IT : allScreentData ) // iterate over all screens
        {
            for(auto && it : scr_IT.second.pvComStructs )
            {
                checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first) );
            }
            for(auto && it : scr_IT.second.pvMonStructs )
            {
                checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first ) );
            }
        }
    }
    else if( status == ECA_NORMAL )
    {
        allChidsInitialised = true; /// interface base class member  not actually used but good to know
    }
}
//________________________________________________________________________________________________________________
void screenInterface::addChannel( const std::string & pvRoot, screenStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID );//MAGIC_NUMBER see epics CA manual, we're a 'casual user'
    debugMessage( "Create channel to ", s1 );
}
//__________________________________________________________________________________________________________________
void screenInterface::monitorScreens()
{
    continuousMonitorStructsDEV.clear();
    continuousMonitorStructsDEV.clear();

    for( auto && it1 : allScreentData )
    {
        monitorIlocks( it1.second.iLockPVStructs, it1.second.iLockStates );
        // iterate over the velaINJscreenObject PvMon
        for( auto && it2 : it1.second.pvMonStructs )
        {
            //std::cout << it1.first <<  " monitorScreens " << ENUM_TO_STRING( it2.first ) << std::endl;

            addScreenObjectMonitors( it2.second,  it1.second );

            ca_create_subscription( it2.second.CHTYPE,
                                    it2.second.COUNT,
                                    it2.second.CHID,
                                    it2.second.MASK,
                                    screenInterface::staticEntryScreenMonitor,
                                    (void*)continuousMonitorStructsDEV.back(),
                                    &continuousMonitorStructsDEV.back()->EVID );
        }
    }
    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to Screen Monitors", "!!TIMEOUT!! Subscription to Screen monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member, not actually used but good to know
}
//_______________________________________________________________________________________________________________
void screenInterface::addScreenObjectMonitors( screenStructs::pvStruct & pvs,  screenStructs::screenObject & obj )
{
    continuousMonitorStructsDEV.push_back( new screenStructs::monitorStruct() );
    continuousMonitorStructsDEV.back() -> interface = this;
    continuousMonitorStructsDEV.back() -> monType   = pvs.pvType;
    continuousMonitorStructsDEV.back() -> CHTYPE    = pvs.CHTYPE;
    continuousMonitorStructsDEV.back() -> dir       = pvs.dir;
    continuousMonitorStructsDEV.back() -> obj       = (void*)&obj;
}
//_______________________________________________________________________________________________________________
bool screenInterface::isHorizontal( screenStructs::DRIVER_DIRECTION dir )
{
    return dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL;
}
//_______________________________________________________________________________________________________________
bool screenInterface::isVertical(  screenStructs::DRIVER_DIRECTION dir )
{
    return dir == screenStructs::DRIVER_DIRECTION::VERTICAL;
}
//_______________________________________________________________________________________________________________
void screenInterface::staticEntryScreenMonitor( const event_handler_args args )
{
    screenStructs::monitorStruct * ms = reinterpret_cast< screenStructs::monitorStruct*>(args.usr);
    //std::cout << "staticEntryScreenMonitor " << std::endl;
    switch ( ms -> monType )// based on which monitor we call a differnwet update function
    {
        case screenStructs::SCREEN_PV_TYPE::H_GET_DEV:
            ms->interface->updateGetDev( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::V_GET_DEV:
            ms->interface->updateGetDev( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::H_DEV_STATE:
            ms->interface->updateDevState( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::V_DEV_STATE:
            ms->interface->updateDevState( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::H_DEV_CENT:
            ms->interface->updateDevCent( ms, *(double*)args.dbr);
            break;
        case screenStructs::SCREEN_PV_TYPE::V_DEV_CENT:
            ms->interface->updateDevCent( ms, *(double*) args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::H_ACTPOS:
             ms->interface->updateACTPOS( ms, *(double*)args.dbr   );
            break;
        case screenStructs::SCREEN_PV_TYPE::V_ACTPOS:
             ms->interface->updateACTPOS( ms, *(double*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::H_EN:
             ms->interface->updateEN( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::V_EN:
             ms->interface->updateEN( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::H_JDIFF:
             ms->interface->updateJDiff( ms, *(double*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::V_JDIFF:
             ms->interface->updateJDiff( ms, *(double*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::H_MOVING:
             ms->interface->updateMoving( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::V_MOVING:
             ms->interface->updateMoving( ms, *(unsigned short*)args.dbr );
            break;

        case screenStructs::SCREEN_PV_TYPE::DEV_STATE:
             ms->interface->updateDevState( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::MOVING:
            ms->interface->updateMoving( ms, *(unsigned short*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::READY:
            break;
        case screenStructs::SCREEN_PV_TYPE::GET_DEV:
            break;
        case screenStructs::SCREEN_PV_TYPE::MAX_POS:
            break;
        case screenStructs::SCREEN_PV_TYPE::DEV_CENT:
            ms->interface->updateDevCent( ms, *(double*)args.dbr);
            break;
        case screenStructs::SCREEN_PV_TYPE::ACTPOS:
            ms->interface->updateACTPOS( ms, *(double*)args.dbr );
            break;
        case screenStructs::SCREEN_PV_TYPE::EN:

            break;

    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateGetDev( screenStructs::monitorStruct * ms, const unsigned short args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
        if( ms->dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_MAX;
                    break;
                case 7:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_SLIT_1;
                    break;
                case 8:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_SLIT_2;
                    break;
                case 9:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_SLIT_3;
                    break;
                case 10:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_APT_1;
                    break;
                case 11:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_APT_2;
                    break;
                case 12:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::H_APT_3;
                    break;
                default:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_MAX;
                    break;
                case 2:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_RF;
                    break;
                case 3:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_MIRROR;
                    break;
                case 4:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_YAG;
                    break;
                case 5:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_GRAT;
                    break;
                case 6:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_COL;
                    break;
                case 7:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::V_SLIT_1;
                    break;
                default:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::PNEUMATIC )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::RETRACTED;
                    break;
                case 4:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::YAG;
                    break;
                default:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
            message(screenName,": Set state is ", ENUM_TO_STRING(allScreentData.at(screenName).screenSetState),
                    " in ", ENUM_TO_STRING(ms->dir), " direction,");
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateDevState( screenStructs::monitorStruct * ms, const unsigned short args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
        if( ms->dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_MAX;
                    break;
                case 7:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_SLIT_1;
                    break;
                case 8:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_SLIT_2;
                    break;
                case 9:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_SLIT_3;
                    break;
                case 10:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_APT_1;
                    break;
                case 11:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_APT_2;
                    break;
                case 12:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_APT_3;
                    break;
                default:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_MAX;
                    break;
                case 2:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_RF;
                    break;
                case 3:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_MIRROR;
                    break;
                case 4:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_YAG;
                    break;
                case 5:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_GRAT;
                    break;
                case 6:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_COL;
                    break;
                case 7:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::V_SLIT_1;
                    break;
                default:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::PNEUMATIC )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::RETRACTED;
                    break;
                case 4:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::YAG;
                    break;
                default:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
            message(screenName,": Screen state is ", ENUM_TO_STRING(allScreentData.at(screenName).screenState));
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateDevCent( screenStructs::monitorStruct * ms, const double args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
            allScreentData.at(screenName).devCent = args;
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Position of device (mm) is ", allScreentData.at(screenName).devCent,
                " in ", ENUM_TO_STRING(ms->dir), " direction,");
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateACTPOS( screenStructs::monitorStruct * ms, const double args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
        if( ms->dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            allScreentData.at(screenName).actPOSH = args;
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            allScreentData.at(screenName).actPOSV = args;
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::PNEUMATIC )
        {
            allScreentData.at(screenName).actPOSP = args;
        }
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateJDiff( screenStructs::monitorStruct * ms, const double args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
        if( ms->dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            allScreentData.at(screenName).jDiffH = args;
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            allScreentData.at(screenName).jDiffV = args;
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        if( allScreentData.at(screenName).screenState == allScreentData.at(screenName).screenSetState )
        {
            switch( ms->dir )
            {
                case screenStructs::DRIVER_DIRECTION::VERTICAL:
                {
                    message(screenName,": Difference from device centre (mm) is ", allScreentData.at(screenName).jDiffV,
                    " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::VERTICAL), " direction,");
                }
                case screenStructs::DRIVER_DIRECTION::HORIZONTAL:
                {
                    message(screenName,": Difference from device centre (mm) is ", allScreentData.at(screenName).jDiffH,
                    " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::HORIZONTAL), " direction,");
                }
            }
        }
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateEN( screenStructs::monitorStruct * ms, const unsigned short args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
        if( ms->dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).hDriverState = screenStructs::DRIVER_STATE::H_DRIVER_DISABLED;
                    break;
                case 1:
                    allScreentData.at(screenName).hDriverState = screenStructs::DRIVER_STATE::H_DRIVER_ENABLED;
                    break;
                default:
                    allScreentData.at(screenName).hDriverState = screenStructs::DRIVER_STATE::UNKNOWN_DRIVER_STATE;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).vDriverState = screenStructs::DRIVER_STATE::V_DRIVER_DISABLED;
                    break;
                case 1:
                    allScreentData.at(screenName).vDriverState = screenStructs::DRIVER_STATE::V_DRIVER_ENABLED;
                    break;
                default:
                    allScreentData.at(screenName).vDriverState = screenStructs::DRIVER_STATE::UNKNOWN_DRIVER_STATE;
            }
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Driver state is ", ENUM_TO_STRING(allScreentData.at(screenName).vDriverState),
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::VERTICAL), " direction,");
        message("and ", ENUM_TO_STRING(allScreentData.at(screenName).hDriverState),
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::HORIZONTAL), " direction");
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateMoving( screenStructs::monitorStruct * ms, const unsigned short args )
{
    screenStructs::screenObject * obj = reinterpret_cast<screenStructs::screenObject *> (ms->obj);
    //unsigned short value = *(unsigned short*)args.dbr;
    const std::string screenName = obj -> name;
    if( entryExists(allScreentData, screenName ) )
    {
        switch( args )
        {
            case 0:
                allScreentData.at(screenName).moving = false;
                break;
            case 1:
                allScreentData.at(screenName).moving = true;
                allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::SCREEN_MOVING;
                break;
        }
    }
    else
        std::cout << screenName << " !!ERRROR!! " << screenName <<  " is not a screen!!" << std::endl;
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_HandV_OUT(screenStructs::screenObject & scr)
{
    return isHOut(scr) && isVOut(scr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHOut(screenStructs::screenObject & scr)
{
    if( entryExists( allScreentData, scr.name ))
    {
        if( isHVMover(scr.name))
            return scr.screenState == screenStructs::SCREEN_STATE::H_RETRACTED;
    }
    else
        std::cout << scr.name << " !!ERRROR!! " << scr.name <<  " is not a screen!!" << std::endl;
    return true;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVOut(screenStructs::screenObject & scr)
{
    if( entryExists( allScreentData, scr.name ))
    {
        if(isMover(scr.name))
            return scr.screenState == screenStructs::SCREEN_STATE::V_RETRACTED;
        else if( isPneumatic(scr.name))
            return scr.screenState == screenStructs::SCREEN_STATE::RETRACTED;
    }
    else
        std::cout << scr.name << " !!ERRROR!! " << scr.name <<  " is not a screen!!" << std::endl;
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHOut(const std::string & name)
{
    return isHOut(allScreentData.at(name));
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVOut(const std::string & name)
{
    return isVOut(allScreentData.at(name));
}
//_________________________________________________________________________________________________________________
bool screenInterface::isMover(const std::string & name)
{
    if( entryExists( allScreentData, name ) && getScreenType(name) != screenStructs::SCREEN_TYPE::VELA_PNEUMATIC )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVMover(const std::string & name)
{
    if( entryExists( allScreentData, name ) && getScreenType(name) == screenStructs::SCREEN_TYPE::CLARA_V_MOVER )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHVMover(const std::string & name)
{
    if( entryExists( allScreentData, name ) && getScreenType(name) == screenStructs::SCREEN_TYPE::CLARA_HV_MOVER || getScreenType(name) == screenStructs::SCREEN_TYPE::VELA_HV_MOVER )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVELAHVMover(const std::string & name)
{
    if( entryExists( allScreentData, name ) && getScreenType(name) == screenStructs::SCREEN_TYPE::VELA_HV_MOVER )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isCLARAHVMover(const std::string & name)
{
    if( entryExists( allScreentData, name ) && getScreenType(name) == screenStructs::SCREEN_TYPE::CLARA_HV_MOVER )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isPneumatic(const std::string & name)
{
    if( entryExists( allScreentData, name ) && getScreenType(name) == screenStructs::SCREEN_TYPE::VELA_PNEUMATIC  )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::getVELAPneumaticScreens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( isPneumatic( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::getVELAHVMoverScreens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( isVELAHVMover( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::getCLARAHVMoverScreens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( isCLARAHVMover( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::getCLARAVMoverScreens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( isVMover( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::getMoverScreens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( isMover( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::getHVMoverScreens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( isHVMover( it ) )
            r.push_back( it );
    return r;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isClearForBeam(const std::string& name)
{
    if(entryExists(allScreentData, name))
        return isClearForBeam(allScreentData.at(name));
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isClearForBeam(screenStructs::screenObject & scr)
{
    using namespace screenStructs;
    if( entryExists( allScreentData, scr.name ) )
    {
        switch( scr.screenType )
        {
            case SCREEN_TYPE::VELA_PNEUMATIC:
                return scr.screenState == SCREEN_STATE::RETRACTED;
                break;
            case SCREEN_TYPE::CLARA_PNEUMATIC:
                return scr.screenState == SCREEN_STATE::RETRACTED;
                break;
            case SCREEN_TYPE::VELA_HV_MOVER:
                if( scr.screenState == SCREEN_STATE::V_RETRACTED || scr.screenState ==  SCREEN_STATE::V_RF)
                {
                    if( scr.screenState == SCREEN_STATE::H_RETRACTED || scr.screenState ==  SCREEN_STATE::H_RF)
                    {
                        return true;
                    }
                }
                break;
            case SCREEN_TYPE::CLARA_HV_MOVER:
                if( scr.screenState == SCREEN_STATE::V_RETRACTED || scr.screenState ==  SCREEN_STATE::V_RF )
                {
                    if( scr.screenState == SCREEN_STATE::H_RETRACTED || scr.screenState ==  SCREEN_STATE::H_RF)
                    {
                        return true;
                    }
                }
                break;
            case SCREEN_TYPE::CLARA_V_MOVER:
                if( scr.screenState == SCREEN_STATE::V_RETRACTED || scr.screenState ==  SCREEN_STATE::V_RF)
                        return true;
                break;
            default:
                return false;
            }
    }
    else
    {
        message("ERROR!!!!!!!!! Screen not defined in config file!!!!!");
        return false;
    }
    return false;
}

//_________________________________________________________________________________________________________________
bool screenInterface::isScreenIn(screenStructs::screenObject & scr)
{
    using namespace screenStructs;
    if( entryExists( allScreentData, scr.name ) )
    {
        switch( scr.screenState )
        {
            case SCREEN_STATE::YAG:
                return true;
                break;
            case SCREEN_STATE::V_YAG:
                return true;
                break;
            default:
                return false;
        }
    }
    else
    {
        message("ERROR!!!!!!!!! Screen not defined in config file!!!!!");
        return false;
    }
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHIn(screenStructs::screenObject & scr)
{
    return !isHOut(scr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVIn(screenStructs::screenObject & scr)
{
    return !isVOut(scr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHIn(const std::string & name)
{
    return !isHOut(name);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVIn(const std::string & name)
{
    return !isVOut(name);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isScreenMoving(screenStructs::screenObject & scr)
{
    return scr.moving == true;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isScreenMoving(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).moving == true;
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
    return r;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isScreenIn(const std::string & name)
{
    return isScreenIn( allScreentData.at(name) );
}
//_________________________________________________________________________________________________________________
bool screenInterface::entryExists2(const std::string & name, bool weKnowEntryExists )
{// This version of the function is so we don't have to check the entry exists in the map each time
     if(weKnowEntryExists)
        return weKnowEntryExists;
    else
        return entryExists(allScreentData,name);
}
//___________________________________________________________________________________________________________
std::vector<bool> screenInterface::isScreenIn(const std::vector<std::string> & name)
{
    std::vector<bool> r;
    for( auto && it : name )
        r.push_back(isScreenIn(it));
    return r;
}
//___________________________________________________________________________________________________________
const screenStructs::SCREEN_STATE screenInterface::getScreenState(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).screenState;
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;

    return screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
}
//___________________________________________________________________________________________________________
const screenStructs::screenObject & screenInterface::getScreenObject(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name);
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
}
//___________________________________________________________________________________________________________
const screenStructs::SCREEN_TYPE screenInterface::getScreenType(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).screenType;
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
}
//___________________________________________________________________________________________________________
const std::string screenInterface::getScreenTypeStr(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return ENUM_TO_STRING(allScreentData.at(name).screenType);
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
}
//___________________________________________________________________________________________________________
std::vector< std::string > screenInterface::getScreenNames()
{
    std::vector< std::string > names;
    for ( auto it : allScreentData )
    {
        names.push_back( it.first );
    }
    return names;
}
//___________________________________________________________________________________________________________
bool screenInterface::isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        return r = allScreentData.at(name).screenState;
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isHElement(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        r = allScreentData.at(name).elementDirection.at(sta) == screenStructs::DRIVER_DIRECTION::HORIZONTAL;
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isVElement(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        r = allScreentData.at(name).elementDirection.at(sta) == screenStructs::DRIVER_DIRECTION::VERTICAL;
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isPElement(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        r = allScreentData.at(name).elementDirection.at(sta) == screenStructs::DRIVER_DIRECTION::PNEUMATIC;
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isYAGIn(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        if( isMover(name) )
        {
            message("mover");
            r = allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::V_YAG;
        }

        else if( isPneumatic(name) )
        {
            message("pneumatic");
            r = allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::YAG;
        }
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isHEnabled(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) && isMover(name) )
    {
        r = allScreentData.at(name).hDriverState == screenStructs::DRIVER_STATE::H_DRIVER_ENABLED;
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isVEnabled(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) && isMover(name) )
    {
        r = allScreentData.at(name).vDriverState == screenStructs::DRIVER_STATE::V_DRIVER_ENABLED;
    }
    return r;
}
//___________________________________________________________________________________________________________
double screenInterface::getACTPOS(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) && isMover(name) )
    {
        if( isVEnabled(name) )
        {
            return allScreentData.at(name).actPOSV;
        }
        else if( isHEnabled(name) )
        {
            return allScreentData.at(name).actPOSH;
        }
    }
    else
    {
        message("ERROR!!!!!!!!! Screen not defined ----- OR, Screen is not Mover");
        return UTL::DUMMY_DOUBLE;
    }
    return r;
}
//___________________________________________________________________________________________________________
double screenInterface::getJDiff(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) && isMover(name) )
    {
        if( isVEnabled(name) )
        {
            return allScreentData.at(name).jDiffV;
        }
        else if( isHEnabled(name) )
        {
            return allScreentData.at(name).jDiffH;
        }
    }
    else
    {
        message("ERROR!!!!!!!!! Screen not defined ----- OR, Screen is not Mover");
        return UTL::DUMMY_DOUBLE;
    }
    return r;
}
//___________________________________________________________________________________________________________
double screenInterface::getDevicePosition(const std::string & name, const screenStructs::SCREEN_STATE state)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).devicePositions.at(state);
    }
    else
    {
        message("ERROR!!!! Screen not defined in config file!!!");
        return 0.0;
    }
}
//___________________________________________________________________________________________________________
double screenInterface::getPosition( const std::string & name )
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).position;
    }
    else
    {
        message("ERROR!!!! Screen not defined in config file!!!");
        return UTL::DUMMY_DOUBLE;
    }
}
//___________________________________________________________________________________________________________
std::vector< screenStructs::SCREEN_STATE > screenInterface::getAvailableDevices(const std::string & name)
{
    std::vector< screenStructs::SCREEN_STATE > devices;
    if( entryExists( allScreentData, name ) )
    {
        for( auto it : allScreentData.at(name).elementPositions )
        {
            devices.push_back( it.first );
        }
    }
    else
    {
        message("ERROR!!!! Screen not defined in config file!!!");
    }
    return devices;
}
////___________________________________________________________________________________________________________
//bool screenInterface::isScreenInPosition(const std::string & name, screenStructs::SCREEN_STATE sta)
//{
//    bool r = false;
//    if( entryExists( allScreentData, name ) )
//        r = allScreentData.at(name).screenState == allScreentData.at(name).screenSetState;
//    return r;
//}
//___________________________________________________________________________________________________________

/// COMMMANDS

//___________________________________________________________________________________________________________
//___________________________________________________________________________________________________________
//   CREATE ONE HIGH LEVEL SCREEN MOVER FUNCTION THAT IS CALLED BY ALL THE HIGHER LEVEL VERSIONS OF screenIN,
//   screenOUT AND MOVE TO CASSETTE ELEMENT POSITION
//___________________________________________________________________________________________________________
void screenInterface::moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state )
{
//    if( isHElement( name, state ) )
//    {
//        setEN( name, screenStructs::DRIVER_DIRECTION::HORIZONTAL );
//    }
//    else if( isVElement( name, state ) )
//    {
//        setEN( name, screenStructs::DRIVER_DIRECTION::VERTICAL );
//    }
    setScreenSDEV( name, state );
    setScreenTrigger( name, state );
}
//___________________________________________________________________________________________________________
void screenInterface::insertYAG( const std::string & name )
{
    if(entryExists(allScreentData,name))
    {
        if( isMover(name) )
        {
            moveScreenTo( name, screenStructs::SCREEN_STATE::V_YAG );
        }
        else if ( isPneumatic(name) )
        {
            moveScreenTo( name, screenStructs::SCREEN_STATE::YAG );
        }
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
}
//___________________________________________________________________________________________________________
void screenInterface::moveScreenOut( const std::string & name )
{
    if(entryExists(allScreentData,name))
    {
        if( isHElement( name, allScreentData.at(name).screenState ) )
        {
            moveScreenTo( name, screenStructs::SCREEN_STATE::H_RETRACTED );
        }
        else if( isVElement( name, allScreentData.at(name).screenState ) )
        {
            moveScreenTo( name, screenStructs::SCREEN_STATE::V_RETRACTED );
        }
        else if( isPElement( name, allScreentData.at(name).screenState ) )
        {
            moveScreenTo( name, screenStructs::SCREEN_STATE::RETRACTED );
        }
    }
    else
        std::cout << name << " !!ERRROR!! " << name <<  " is not a screen!!" << std::endl;
}
//___________________________________________________________________________________________________________
void screenInterface::resetPosition( const std::string & name )
{
    setScreenTrigger( name );
}
//______________________________________________________________________________
bool screenInterface::setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    if( isScreenMoving( allScreentData.at(name) ) )
    {
        message("Screen ", name, " is moving, please wait!!!!!");
    }
    else
    {
         if( isVElement( name, state ) )
        {
            ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_SDEV).CHTYPE,
                   allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_SDEV).CHID,
                   &allScreentData.at(name).elementPositions.at(state) );
        }
        else if( isHElement( name, state ) )
        {
            ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_SDEV).CHTYPE,
                   allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_SDEV).CHID,
                   &allScreentData.at(name).elementPositions.at(state) );
        }
        else if( isPElement( name, state ) )
        {
            ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::SDEV).CHTYPE,
                   allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::SDEV).CHID,
                   &allScreentData.at(name).elementPositions.at(state) );
        }
        else
        {
            message("ERROR! DRIVER DIRECTION UNKNOWN");
        }
    }
    std::string m1;
    m1 = "Timeout sending position in setScreenSDEV";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("setScreenSDEV recieved an unexpected error status from EPICS");
    return ret;
}
//______________________________________________________________________________
bool screenInterface::setScreenTrigger( const std::string & name )
{
    unsigned short send = 1;
    if( isMover(name) )
    {
        if( allScreentData.at(name).screenSetState != screenStructs::SCREEN_STATE::V_RETRACTED )
        {
            ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TRIGGER).CHTYPE,
                   allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TRIGGER).CHID,
                   &send );
        }
        else if( allScreentData.at(name).screenSetState != screenStructs::SCREEN_STATE::H_RETRACTED )
        {
            ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TRIGGER).CHTYPE,
                   allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TRIGGER).CHID,
                   &send );
        }
    }
    else if( isPneumatic(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::TRIGGER).CHTYPE,
        allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::TRIGGER).CHID,
        &send );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
    std::string m1;
    m1 = "Timeout sending position in setScreenTrigger";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("setScreenTrigger recieved an unexpected error status from EPICS");
    return ret;
}
//______________________________________________________________________________
bool screenInterface::setScreenTrigger( const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    message("sddedede");
    unsigned short send = 1;
    if( isVElement( name, state )  )
    {
        message("sddedede");
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TRIGGER).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TRIGGER).CHID,
               &send );
    }
    else if( isHElement( name, state ) )
    {
        message("sddedede!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TRIGGER).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TRIGGER).CHID,
               &send );
    }
    else if( isPElement( name, state ) )
    {
        message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::TRIGGER).CHTYPE,
        allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::TRIGGER).CHID,
        &send );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
    std::string m1;
    m1 = "Timeout sending position in setScreenTrigger";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("setScreenTrigger recieved an unexpected error status from EPICS");
    return ret;
}
//______________________________________________________________________________
bool screenInterface::setEX( const std::string & name )
{
    unsigned short send = 1;
    if( isVEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_EX).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_EX).CHID,
               &send );
    }
    else if( isHEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_EX).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_EX).CHID,
               &send );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
    std::string m1;
    debugMessage("we here now");
    m1 = "Timeout sending position in setScreenTrigger";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("setEX recieved an unexpected error status from EPICS");
    return ret;
}
//______________________________________________________________________________
bool screenInterface::setEN( const std::string & name, const screenStructs::DRIVER_DIRECTION direction )
{
    unsigned short send = 1;
    if( direction == screenStructs::DRIVER_DIRECTION::VERTICAL )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_EN).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_EN).CHID,
               &send );
    }
    else if( direction == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_EN).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_EN).CHID,
               &send );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
    std::string m1;
    m1 = "Timeout sending enable to driver";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("setEN recieved an unexpected error status from EPICS");
    return ret;
}
//___________________________________________________________________________________________________________
void screenInterface::jogScreen( const std::string & name, const double jog )
{
    if( isVEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_JOG).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_JOG).CHID,
               &jog );
        setScreenTrigger( name );
    }
    else if( isHEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_JOG).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_JOG).CHID,
               &jog );
        setScreenTrigger( name );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
}
//___________________________________________________________________________________________________________
void screenInterface::setPosition( const std::string & name, const double setPos )
{
    if( isVEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TGTPOS).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TGTPOS).CHID,
               &setPos );
        setEX( name );
    }
    else if( isHEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TGTPOS).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TGTPOS).CHID,
               &setPos );
        setEX( name );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
}
////___________________________________________________________________________________________________________
//bool screenInterface::screenMoveTo( const std::string & name, const screenStructs::SCREEN_STATE & states)
//{
//    std::vector< screenStructs::SCREEN_STATE > v( 1, states );
//    std::vector< std::string> n( 1, name );
//    return  screenMoveTo( n, v);
//}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > screenInterface::getILockStates( const std::string & objName )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > r;
    auto iter = allScreentData.find( objName );
    if( iter != allScreentData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  screenInterface::getILockStatesStr( const std::string & name )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allScreentData.find( name );
    if( iter != allScreentData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
