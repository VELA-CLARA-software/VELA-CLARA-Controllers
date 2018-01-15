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
                                  const bool startVirtualMachine,
                                  const bool* show_messages_ptr,
                                  const bool* show_debug_messages_ptr,
                                  const bool shouldStartEPICs,
                                  const VELA_ENUM::MACHINE_AREA myMachineArea ):
configReader( conf1, startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface( show_messages_ptr, show_debug_messages_ptr ),
shouldStartEPICs( shouldStartEPICs ),
myMachineArea(myMachineArea),
dualMoveNum(0)
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
                std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); ///MAGIC NUMBER
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
        message("ERROR ECA_TIMEOUT");
    else if( status == ECA_NORMAL )
    {
        allChidsInitialised = true; /// interface base class member  not actually used but good to know
    }
}
//________________________________________________________________________________________________________________
void screenInterface::addChannel( const std::string & pvRoot, screenStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(),0,0,0, &pv.CHID );//MAGIC_NUMBER see epics CA manual, we're a 'casual user'
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
            std::cout << it1.first <<  " monitorScreens " << ENUM_TO_STRING( it2.first ) << std::endl;

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
        case  screenStructs::SCREEN_PV_TYPE::H_DEV_CENT:
            ms->interface->updateDevCent( ms, *(double*)args.dbr);
            break;
        case  screenStructs::SCREEN_PV_TYPE::V_DEV_CENT:
            ms->interface->updateDevCent( ms, *(double*) args.dbr );
            break;
        case  screenStructs::SCREEN_PV_TYPE::H_ACTPOS:
             ms->interface->updateACTPOS( ms, *(double*)args.dbr   );
            break;
        case  screenStructs::SCREEN_PV_TYPE::V_ACTPOS:
             ms->interface->updateACTPOS( ms, *(double*)args.dbr );
            break;
        case  screenStructs::SCREEN_PV_TYPE::H_EN:
             ms->interface->updateEN( ms, *(unsigned short*)args.dbr );
            break;
        case  screenStructs::SCREEN_PV_TYPE::V_EN:
             ms->interface->updateEN( ms, *(unsigned short*)args.dbr );
            break;
//        case  screenStructs::SCREEN_PV_TYPE::ACTPOS:
//             ms->interface->updateACTPOS( ms, *(double*)args.dbr );
//            break;
//        case  screenStructs::SCREEN_PV_TYPE::EN:
//             ms->interface->updateEN( ms, *(unsigned short*)args.dbr );
//            break;
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
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_MAX;
                    break;
                case 7:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_SLIT_1;
                    break;
                case 8:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_SLIT_2;
                    break;
                case 9:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_SLIT_3;
                    break;
                case 10:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_APT_1;
                    break;
                case 11:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_APT_2;
                    break;
                case 12:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::H_APT_3;
                    break;
                default:
                    allScreentData.at(screenName).screenSetHState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_MAX;
                    break;
                case 2:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_RF;
                    break;
                case 3:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_MIRROR;
                    break;
                case 4:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_YAG;
                    break;
                case 5:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_GRAT;
                    break;
                case 6:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::V_COL;
                    break;
                default:
                    allScreentData.at(screenName).screenSetVState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Set state is ", ENUM_TO_STRING(allScreentData.at(screenName).screenVState),
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::VERTICAL), " direction,");
        message("and ", ENUM_TO_STRING(allScreentData.at(screenName).screenHState),
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::HORIZONTAL), " direction");
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
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_MAX;
                    break;
                case 7:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_SLIT_1;
                    break;
                case 8:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_SLIT_2;
                    break;
                case 9:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_SLIT_3;
                    break;
                case 10:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_APT_1;
                    break;
                case 11:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_APT_2;
                    break;
                case 12:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::H_APT_3;
                    break;
                default:
                    allScreentData.at(screenName).screenHState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            switch( args )
            {
                case 0:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::V_RETRACTED;
                    break;
                case 1:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::V_MAX;
                    break;
                case 2:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::V_SLIT_1;
                    break;
                case 3:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::V_GRAT;
                    break;
                case 4:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::V_YAG;
                    break;
                case 5:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::V_RF;
                    break;
                default:
                    allScreentData.at(screenName).screenVState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Current state is ", ENUM_TO_STRING(allScreentData.at(screenName).screenVState),
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::VERTICAL), " direction,");
        message("and ", ENUM_TO_STRING(allScreentData.at(screenName).screenHState),
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::HORIZONTAL), " direction");
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
        if( ms->dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            allScreentData.at(screenName).devCentH = args;
        }
        else if( ms->dir == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            allScreentData.at(screenName).devCentV = args;
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Position of device (mm) is ", allScreentData.at(screenName).devCentV,
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::VERTICAL), " direction,");
        message("and ", allScreentData.at(screenName).devCentH,
                " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::HORIZONTAL), " direction");
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
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        if( allScreentData.at(screenName).screenHState == allScreentData.at(screenName).screenSetHState && allScreentData.at(screenName).screenVState == allScreentData.at(screenName).screenSetVState )
        {
            switch( ms->dir )
            {
                case screenStructs::DRIVER_DIRECTION::VERTICAL:
                {
                    message(screenName,": Actuator position (mm) is ", allScreentData.at(screenName).devCentV,
                    " in ", ENUM_TO_STRING(screenStructs::DRIVER_DIRECTION::VERTICAL), " direction,");
                }
                case screenStructs::DRIVER_DIRECTION::HORIZONTAL:
                {
                    message(screenName,": Actuator position (mm) is ", allScreentData.at(screenName).devCentH,
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
    message(args);
    message("!!!!!!!!!!!!!!!!!!!");
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
bool screenInterface::is_HandV_OUT(screenStructs::screenObject & scr)
{
    return isHOut(scr) && isVOut(scr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHOut(screenStructs::screenObject & scr)
{
    return scr.screenHState == screenStructs::SCREEN_STATE::H_RETRACTED;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVOut(screenStructs::screenObject & scr)
{
    return scr.screenVState == screenStructs::SCREEN_STATE::V_RETRACTED;
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_HandV_OUT(const std::string & name)
{
    return isHOut(name) && isVOut(name);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHOut(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).screenHState == screenStructs::SCREEN_STATE::H_RETRACTED;
    }
    else
    {
        message("ERROR!!!!!!!!! Screen not defined in config file!!!!!");
        return false;
    }
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVOut(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name).screenHState == screenStructs::SCREEN_STATE::V_RETRACTED;
    }
    else
    {
        message("ERROR!!!!!!!!! Screen not defined in config file!!!!!");
        return false;
    }
}
//_________________________________________________________________________________________________________________
bool screenInterface::isScreenIn(screenStructs::screenObject & scr)
{
    return isHIn(scr) || isVIn(scr);
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
bool screenInterface::isScreenIn(const std::string & name)
{
    return isHIn(name) && isVIn(name);
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
bool screenInterface::entryExists2(const std::string & name, bool weKnowEntryExists )
{// This version of the function is so we don't have to check the entry exists in the map each time
     if(weKnowEntryExists)
        return weKnowEntryExists;
    else
        return entryExists(allScreentData,name);
}
//___________________________________________________________________________________________________________
std::vector<bool> screenInterface::isScreenIN(const std::vector<std::string> & name)
{
    std::vector<bool> r;
    for( auto && it : name )
        r.push_back(isScreenIN(it,false));
    return r;
}
////___________________________________________________________________________________________________________
//std::vector<bool> screenInterface::isScreenOUT( const std::vector<std::string> & name )
//{
//    std::vector<bool> r;
//    for( auto && it : name )
//        r.push_back(isScreenOUT(it,false));
//    return r;
//}
//___________________________________________________________________________________________________________
bool screenInterface::isScreenIN(const std::string & name, bool weKnowEntryExists)
{
    if( entryExists2( name,weKnowEntryExists ) )
    {
        if( allScreentData.at(name).screenVState == screenStructs::SCREEN_STATE::V_RETRACTED )
            return false;
        if( allScreentData.at(name).screenHState == screenStructs::SCREEN_STATE::H_RETRACTED )
            return false;
    }
    return true;
}
////___________________________________________________________________________________________________________
//bool screenInterface::getScreenState(const std::string & name, const bool weKnowEntryExists)
//{
//    if( weKnowEntryExists )
//        return allScreentData.at(name).screenState;
//    else if( entryExists( allScreentData, name ) )
//        return allScreentData.at(name).screenState;
//    else
//        return screenStructs::SCREEN_STATE::SCREEN_UNKNOWN;
//}
//___________________________________________________________________________________________________________
const std::string screenInterface::getScreenState(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        if( isHIn(name) )
        {
            return ENUM_TO_STRING(allScreentData.at(name).screenHState);
        }
        else if( isVIn(name) )
        {
            return ENUM_TO_STRING(allScreentData.at(name).screenVState);
        }
        else if( is_HandV_OUT(name) )
        {
            return ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_OUT);
        }
    }
    else
    {
        return ENUM_TO_STRING(screenStructs::SCREEN_STATE::SCREEN_UNKNOWN);
    }
}
//___________________________________________________________________________________________________________
const screenStructs::screenObject & screenInterface::getScreenObject(const std::string & name)
{
    if( entryExists( allScreentData, name ) )
    {
        return allScreentData.at(name);
    }
}
//___________________________________________________________________________________________________________
bool screenInterface::isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        if( allScreentData.at(name).elementDirection.at(sta) == screenStructs::DRIVER_DIRECTION::VERTICAL )
        {
            r = allScreentData.at(name).screenVState == sta;
        }
        else if( allScreentData.at(name).elementDirection.at(sta) == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
        {
            r = allScreentData.at(name).screenHState == sta;
        }
    }
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
bool screenInterface::isYAGIn(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        if( isVIn(name) )
        {
            r = allScreentData.at(name).screenVState == screenStructs::SCREEN_STATE::V_YAG;
        }
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isHEnabled(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        r = allScreentData.at(name).hDriverState == screenStructs::DRIVER_STATE::H_DRIVER_ENABLED;
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isVEnabled(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
    {
        r = allScreentData.at(name).vDriverState == screenStructs::DRIVER_STATE::V_DRIVER_ENABLED;
    }
    return r;
}
//___________________________________________________________________________________________________________
double screenInterface::getACTPOS(const std::string & name)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
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
    return r;
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

////___________________________________________________________________________________________________________
//bool screenInterface::screenIN( const std::string & name  )
//{   message("screenInterface::screenIN(const std::string & name ) called");
//    const  std::vector< std::string > names = { name };
//    return screenIN( names );
//}
////___________________________________________________________________________________________________________
//bool screenInterface::screenOUT( const std::string & name  )
//{   message("screenInterface::screenOUT(const std::string & name ) called");
//    const  std::vector< std::string > names = { name };
//    return screenOUT( names );
//}
////___________________________________________________________________________________________________________
//bool screenInterface::screenIN( const std::vector< std::string > & names  )
//{
//    std::vector< screenStructs::SCREEN_STATE > v( names.size(), screenStructs::SCREEN_STATE::SCREEN_IN );
//    return screenMoveTo( names, v);
//}
////___________________________________________________________________________________________________________
//bool screenInterface::screenOUT( const std::vector< std::string > & names  )
//{
//    std::vector< screenStructs::SCREEN_STATE > v( names.size(), screenStructs::SCREEN_STATE::SCREEN_OUT );
//    bool r = screenMoveTo( names, v);
//    return r;
//}
//___________________________________________________________________________________________________________
//___________________________________________________________________________________________________________
//   CREATE ONE HIGH LEVEL SCREEN MOVER FUNCTION THAT IS CALLED BY ALL THE HIGHER LEVEL VERSIONS OF screenIN,
//   screenOUT AND MOVE TO CASSETTE ELEMENT POSITION
//___________________________________________________________________________________________________________
void screenInterface::moveScreenTo( const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    setScreenSDEV( name, state );
    setScreenTrigger( name );
}
//___________________________________________________________________________________________________________
void screenInterface::resetPosition( const std::string & name )
{
    setScreenTrigger( name );
}
//______________________________________________________________________________
bool screenInterface::setScreenSDEV(const std::string & name, const screenStructs::SCREEN_STATE & state )
{
    if( allScreentData.at(name).elementDirection.at(state) == screenStructs::DRIVER_DIRECTION::VERTICAL )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_SDEV).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_SDEV).CHID,
               &allScreentData.at(name).elementPositions.at(state) );
    }
    else if( allScreentData.at(name).elementDirection.at(state) == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_SDEV).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_SDEV).CHID,
               &allScreentData.at(name).elementPositions.at(state) );
    }
    else
    {
        message("ERROR! DRIVER DIRECTION UNKNOWN");
    }
    std::string m1;
    debugMessage("we here now");
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
    if( isVEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TRIGGER).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::V_TRIGGER).CHID,
               &send );
    }
    else if( isHEnabled(name) )
    {
        ca_put(allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TRIGGER).CHTYPE,
               allScreentData.at(name).pvComStructs.at(screenStructs::SCREEN_PV_TYPE::H_TRIGGER).CHID,
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
        message("setScreenTrigger recieved an unexpected error status from EPICS");
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

// Quantifiers

//___________________________________________________________________________________________________________
//___________________________________________________________________________________________________________
bool screenInterface::is_VELA_PNEUMATIC( const std::string & name )
{
    bool r = false;
    if( entryExists(allScreentData, name ) )
        if( allScreentData.at(name).screenType == screenStructs::SCREEN_TYPE::VELA_PNEUMATIC )
            r = true;
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::is_CLARA_PNEUMATIC( const std::string & name )
{
    bool r = false;
    if( entryExists(allScreentData, name ) )
        if( allScreentData.at(name).screenType == screenStructs::SCREEN_TYPE::CLARA_PNEUMATIC )
            r = true;
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::is_VELA_HV_MOVER ( const std::string & name )
{
    bool r = false;
    if( entryExists(allScreentData, name ) )
        if( allScreentData.at(name).screenType == screenStructs::SCREEN_TYPE::VELA_HV_MOVER )
            r = true;
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::is_CLARA_HV_MOVER ( const std::string & name )
{
    bool r = false;
    if( entryExists(allScreentData, name ) )
        if( allScreentData.at(name).screenType == screenStructs::SCREEN_TYPE::CLARA_HV_MOVER )
            r = true;
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::is_CLARA_V_MOVER ( const std::string & name )
{
    bool r = false;
    if( entryExists(allScreentData, name ) )
        if( allScreentData.at(name).screenType == screenStructs::SCREEN_TYPE::CLARA_V_MOVER )
            r = true;
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::get_VELA_PNEUMATIC_Screens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( is_VELA_PNEUMATIC( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::get_VELA_HV_MOVER_Screens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( is_VELA_HV_MOVER( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::get_CLARA_PNEUMATIC_Screens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( is_CLARA_PNEUMATIC( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::get_CLARA_HV_MOVER_Screens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( is_CLARA_HV_MOVER( it ) )
            r.push_back( it );
    return r;
}
//___________________________________________________________________________________________________________
const std::vector<std::string> screenInterface::get_CLARA_V_MOVER_Screens(  const std::vector< std::string > & names )
{
    std::vector<std::string> r;
    for( auto && it : names )
        if( is_CLARA_V_MOVER( it ) )
            r.push_back( it );
    return r;
}
////_________________________________________________________________________________________________________________
//bool screenInterface::yagOnV( const screenStructs::screenDriver & scrdr )
//{
//    return scrdr.vCassette.cassetteElements.at(screenStructs::SCREEN_STATE::V_YAG) == true;
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::isVDriveEnabled(const screenStructs::screenDriver & scrdr )
//{
//    return !isVDriveDisabled(scrdr);
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::isVDriveDisabled(const screenStructs::screenDriver & scrdr )
//{
//    return scrdr.vDriverSTA.state == screenStructs::DRIVER_STATE::DRIVER_DISABLED;
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::isHDriveEnabled(const screenStructs::screenDriver & scrdr )
//{
//    return !isHDriveDisabled(scrdr);
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::isHDriveDisabled(const screenStructs::screenDriver & scrdr )
//{
//    return scrdr.hDriverSTA.state == screenStructs::DRIVER_STATE::DRIVER_DISABLED;
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::is_H_Element(const std::string & name, const screenStructs::SCREEN_STATE e )
//{   // When moving a cassette element we need to know if it is an H or V element
//    if( allScreentData.at(name).elementDirection.at(e) == screenStructs::DRIVER_DIRECTION::HORIZONTAL )
//            return true;
//    return false;
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::is_V_Element(const std::string & name, const screenStructs::SCREEN_STATE e )
//{   // When moving a cassette element we need to know if it is an H or V element
//    if( allScreentData.at(name).elementDirection.at(e) == screenStructs::DRIVER_DIRECTION::VERTICAL )
//            return true;
//    return false;
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::is_OUT_AND_VDriveEnabled(const std::string & name)
//{
//    return allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_OUT
//           &&
//           isVDriveEnabled(allScreentData.at(name).driver);
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::is_H_element_AND_HDriveEnabled(const screenStructs::SCREEN_STATE e,const screenStructs::screenDriver& scrdr)
//{
//    return is_H_Element(e) && isHDriveEnabled(scrdr);
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::is_V_element_AND_VDriveEnabled(const screenStructs::SCREEN_STATE e,const screenStructs::screenDriver& scrdr)
//{
//    return is_V_Element(e) && isVDriveEnabled(scrdr);
//}
//_________________________________________________________________________________________________________________
bool screenInterface::is_IN_OR_OUT(const screenStructs::SCREEN_STATE sta )
{
    return sta == screenStructs::SCREEN_STATE::SCREEN_OUT || sta == screenStructs::SCREEN_STATE::SCREEN_IN;
}
////_________________________________________________________________________________________________________________
//bool screenInterface::isScreenOUT(const std::string & name, const bool weKnowEntryExists)
//{
//    if( entryExists2(name,weKnowEntryExists) )
//        return allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_OUT;
//    else
//        return false;//do we need a TRUE FALSE or UNKNOWN ??
//}
//_________________________________________________________________________________________________________________
std::vector<bool> screenInterface::exists_and_isLocked(const std::string& name)
{
    std::vector<bool> r;
    if( entryExists(allScreentData, name ))
    {
        r.push_back(true);
        r.push_back( isLockedMap.at(name) );
    }
    else
    {
        r.push_back(false);
        r.push_back(false);
    }
    debugMessage("exists_and_isLocked return ", r[0], "  ", r[1]);
    return r;
}
//_________________________________________________________________________________________________________________
std::vector<bool> screenInterface::exists_and_isNotLocked(const std::string& name)
{
    std::vector<bool> r;
    if( entryExists(allScreentData,name) )
    {
        r.push_back(true);
        r.push_back( !isLockedMap.at(name) );
    }
    else
    {
        r.push_back(false);
        r.push_back(false);
    }
    debugMessage("exists_and_isNotLocked return ", r[0], "  ", r[1]);
    return r;
}
////_________________________________________________________________________________________________________________
//bool screenInterface::screen_is_out_AND_sta_is_in(const std::string & name, const screenStructs::SCREEN_STATE sta )
//{
//    return sta == screenStructs::SCREEN_STATE::SCREEN_IN && isScreenOUT(name);
//}
////_________________________________________________________________________________________________________________
//bool screenInterface::screen_is_in_AND_sta_is_out(const std::string & name, const screenStructs::SCREEN_STATE sta )
//{
//    return sta == screenStructs::SCREEN_STATE::SCREEN_OUT && isScreenIN(name);
//}
////_________________________________________________________________________________________________________________
//double screenInterface::get_H_ACTPOS(const std::string & name)
//{
//    if(is_CLARA_HV_MOVER(name))
//    {
//        return allScreentData.at( name ).driver.hDriverSTA.position;
//    }
//}
////_________________________________________________________________________________________________________________
//double screenInterface::get_V_ACTPOS(const std::string & name)
//{
//    if(is_CLARA_HV_MOVER(name) || is_CLARA_V_MOVER(name))
//    {
//        return allScreentData.at( name ).driver.vDriverSTA.position;
//    }
//}
//___________________________________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > screenInterface::getILockStates( const std::string & name )
{
//    if( entryExists( velaINJscreenObject.simpleObjects, name ) )
//    {   std::cout << "Entry Exists getILockStates" << std::endl;
//        return velaINJscreenObject.simpleObjects[ name ].iLockStates;
//    }
//
//    else if( entryExists( velaINJscreenObject.complexObjects, name ) )
//    {   std::cout << "Entry Exists getILockStates" << std::endl;
//        return velaINJscreenObject.complexObjects[ name ].iLockStates;
//    }
//
//    else
//    {
//        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r = {{VELA_ENUM::ILOCK_NUMBER::ILOCK_ERR, VELA_ENUM::ILOCK_STATE::ILOCK_ERROR}};
//        return r;
//    }
    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
    return r;
}
////__________________________________________________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  screenInterface::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string  > r;

//    if( entryExists( velaINJscreenObject.simpleObjects, name ) )
//    {
//        std::cout << "Entry Exists getILockStatesStr" << std::endl;
//        for( auto it : velaINJscreenObject.simpleObjects[ name ].iLockStates )
//        {
//            message( "hello", it.second  );
//            r[ it.first ] = ENUM_TO_STRING( it.second );
//        }
//    }
//
//    if( entryExists( velaINJscreenObject.complexObjects, name ) )
//    {
//        std::cout << "Entry Exists getILockStatesStr" << std::endl;
//        for( auto it : velaINJscreenObject.complexObjects[ name ].iLockStates )
//            r[ it.first ] = ENUM_TO_STRING( it.second );
//    }
//
//    else
//        r[ VELA_ENUM::ILOCK_NUMBER::ILOCK_ERR ] = ENUM_TO_STRING( VELA_ENUM::ILOCK_STATE::ILOCK_ERROR );
    return r;
}
