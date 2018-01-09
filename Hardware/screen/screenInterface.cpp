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
    for (auto && it : screenDualMoveStructsMap )
    {
        debugMessage("in screenInterface: delete screenDualMove thread ", it.first );
        it.second.thread->join();
        delete it.second.thread;
    }
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

//        // iterate over all screen pvCom / pvMon structs,
//        for( auto && it2 : scr_IT.second.driver.pvComStructs )
//            addChannel( scr_IT.second.pvRoot, it2.second );
//        for( auto && it2 : scr_IT.second.driver.pvMonStructs  )
//            addChannel( scr_IT.second.pvRoot, it2.second );
//
//        // iterate over all screenDriverStatus,
//        for( auto && it2 : scr_IT.second.driver.hDriverSTA.pvComStructs )
//        {
//            addChannel( scr_IT.second.pvRoot, it2.second );
//        }
//        for( auto && it2 : scr_IT.second.driver.hDriverSTA.pvMonStructs )
//        {
//            addChannel( scr_IT.second.pvRoot, it2.second );
//        }
//        for( auto && it2 : scr_IT.second.driver.vDriverSTA.pvComStructs )
//        {
//            addChannel( scr_IT.second.pvRoot, it2.second );
//        }
//        for( auto && it2 : scr_IT.second.driver.vDriverSTA.pvMonStructs )
//        {
//            addChannel( scr_IT.second.pvRoot, it2.second );
//        }

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
    std::string dir;
    std::string s1 = pvRoot + dir + pv.pvSuffix;
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
        // iterate over the acreenDriver PvMon
//        for( auto && it2 : it1.second.driver.pvMonStructs )
//        {
//            std::cout << it1.first <<  " monitorScreens " << ENUM_TO_STRING( it2.first ) << std::endl;
//
//            addScreenDriverMonitors( it2.second,  it1.second.driver  );
//
//            ca_create_subscription( it2.second.CHTYPE,
//                                    it2.second.COUNT,
//                                    it2.second.CHID,
//                                    it2.second.MASK,
//                                    screenInterface::staticEntryScreenMonitor,
//                                    (void*)continuousMonitorStructsDEV.back(),
//                                    &continuousMonitorStructsDEV.back()->EVID );
//        }
//        // iterate over the hDriverSTA screenDriverStatus PvMon
//        for( auto && it2 : it1.second.driver.hDriverSTA.pvMonStructs )
//        {
//            std::cout << it1.first <<  " monitorScreens " << ENUM_TO_STRING( it2.first ) << std::endl;
//
//            addScreenDriverStatusMonitors( it2.second,  it1.second.driver.hDriverSTA  );
//
//            ca_create_subscription( it2.second.CHTYPE,
//                                    it2.second.COUNT,
//                                    it2.second.CHID,
//                                    it2.second.MASK,
//                                    screenInterface::staticEntryScreenMonitor,
//                                    (void*)continuousMonitorStructsDEV.back(),
//                                    &continuousMonitorStructsDEV.back()->EVID );
//        }
//        // iterate over the vDriverSTA screenDriverStatus PvMon
//        for( auto && it2 : it1.second.driver.vDriverSTA.pvMonStructs )
//        {
//            std::cout << it1.first <<  " monitorScreens " << ENUM_TO_STRING( it2.first ) << std::endl;
//
//            addScreenDriverStatusMonitors( it2.second,  it1.second.driver.vDriverSTA  );
//
//            ca_create_subscription( it2.second.CHTYPE,
//                                    it2.second.COUNT,
//                                    it2.second.CHID,
//                                    it2.second.MASK,
//                                    screenInterface::staticEntryScreenMonitor,
//                                    (void*)continuousMonitorStructsDEV.back(),
//                                    &continuousMonitorStructsDEV.back()->EVID );
//        }
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
////_______________________________________________________________________________________________________________
//void screenInterface::addScreenObjectMonitors( screenStructs::pvStruct & pvs,  screenStructs::screenObject & obj  )
//{
//    continuousMonitorStructsDEV.push_back( new screenStructs::monitorStruct() );
//    continuousMonitorStructsDEV.back() -> interface = this;
//    continuousMonitorStructsDEV.back() -> monType   = pvs.pvType;
//    continuousMonitorStructsDEV.back() -> CHTYPE    = pvs.CHTYPE;
//    continuousMonitorStructsDEV.back() -> obj       = (void*)&obj;
//}
//_______________________________________________________________________________________________________________
void screenInterface::addScreenDriverMonitors( screenStructs::pvStruct & pvs, screenStructs::screenDriver & obj )// see screenstructs for a screenDriverStatus
{
    continuousMonitorStructsDEV.push_back( new screenStructs::monitorStruct() );
    continuousMonitorStructsDEV.back() -> interface = this;
    continuousMonitorStructsDEV.back() -> monType   = pvs.pvType;
    continuousMonitorStructsDEV.back() -> CHTYPE    = pvs.CHTYPE;
    continuousMonitorStructsDEV.back() -> obj       = (void*)&obj;
}
//_______________________________________________________________________________________________________________
void screenInterface::addScreenDriverStatusMonitors( screenStructs::pvStruct & pvs, screenStructs::screenDriverStatus & obj )// see screenstructs for a screenDriver
{
    continuousMonitorStructsDEV.push_back( new screenStructs::monitorStruct() );
    continuousMonitorStructsDEV.back() -> interface = this;
    continuousMonitorStructsDEV.back() -> monType   = pvs.pvType;
    continuousMonitorStructsDEV.back() -> CHTYPE    = pvs.CHTYPE;
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
//        case  screenStructs::SCREEN_PV_TYPE::Sta:
//            ms->interface->updateSta( ms, *(unsigned short*)args.dbr);
//            break;
//        case  screenStructs::SCREEN_PV_TYPE::STA:
//            ms->interface->update_STA_Bit_map( ms, *(int*) args.dbr );
//            break;
//        case  screenStructs::SCREEN_PV_TYPE::RPOS:
//             ms->interface->updateRPOS( ms, *(double*)args.dbr   );
//            break;
//        case  screenStructs::SCREEN_PV_TYPE::PROT01:
//             ms->interface->updatePROT01( ms, *(double*)args.dbr );
//            break;
//        case  screenStructs::SCREEN_PV_TYPE::PROT03:
//             ms->interface->updatePROT03( ms, *(double*)args.dbr );
//            break;
//        case  screenStructs::SCREEN_PV_TYPE::PROT05:
//             ms->interface->updatePROT05( ms, *(double*)args.dbr );
//            break;
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
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::H_APT_2;
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
                default:
                    allScreentData.at(screenName).screenSetState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Set state is ",
                ENUM_TO_STRING(allScreentData.at(screenName).screenSetState));
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
                default:
                    allScreentData.at(screenName).screenState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;
            }
        }
        // this is also copying the pvstructs maps that we know
        //will never be used (IS THIS A PROBLEM?)
        message(screenName,": Current state is ",
                ENUM_TO_STRING(allScreentData.at(screenName).screenState));
    }
}
//_________________________________________________________________________________________________________________
void screenInterface::updatePROT03( screenStructs::monitorStruct * ms, const double args )
{
    screenStructs::screenDriver * obj = reinterpret_cast<screenStructs::screenDriver *> (ms->obj);
    std::stringstream ss;
    ss << obj->parentScreen;
    if( args == 0.0 )//MAGIC_NUMBER
    {
        obj->positionError = false;
        ss<< " positionError = FALSE ";
    }
    else if( args == 1.0 )//MAGIC_NUMBER
    {
        obj->positionError = true;
        ss << " positionError = TRUE ";
    }
    else
        ss << " updatePROT03 ERROR ";
    ss << obj->positionError;
    message(ss);
}
//_________________________________________________________________________________________________________________
void screenInterface::updatePROT05( screenStructs::monitorStruct * ms, const double args )
{
    // evil evil evil
    screenStructs::screenDriver * obj = reinterpret_cast<screenStructs::screenDriver *> (ms->obj);
    std::stringstream ss;
    ss << obj->parentScreen;
    if( args == 0.0 )//MAGIC_NUMBER
    {
        obj->homeError = false;
        ss<< " homeError = FALSE ";
    }
    else if( args == 1.0 )//MAGIC_NUMBER
    {
        obj->homeError = true;
        ss << " homeError = TRUE ";
    }
    else
        ss << " updatePROT05 ERROR ";
    ss << obj->homeError;
    message(ss);
}
//_________________________________________________________________________________________________________________
void screenInterface::updateRPOS( screenStructs::monitorStruct * ms,  const double args )
{
    // evilness (?)
    screenStructs::screenDriverStatus * obj = reinterpret_cast<screenStructs::screenDriverStatus *> (ms->obj);
    obj->position = args;

// while the screen is moving ther eis no point updating the casseete position
//    screenStructs::SCREEN_STATE newscreenstate;
//    if( isHorizontal( obj->dir)  )
//        updateCassettePosition( allScreentData[obj->parentScreen].driver.hCassette,  obj->position );
//    else if( isVertical(obj->dir) )
//        updateCassettePosition( allScreentData[obj->parentScreen].driver.vCassette,  obj->position );

    std::stringstream ss;
    ss << obj->parentScreen;
    if( isHorizontal( obj->dir)  )
        ss << " Horizontal";
    else if( isVertical(obj->dir) )
        ss << " Vertical";
    else
        ss << " ERROR";
    ss << " Driver Position = " << obj->position;
    message(ss);
}
//_________________________________________________________________________________________________________________
void screenInterface::updateACTPOS( screenStructs::monitorStruct * ms,  const double args )
{
    // evilness (?)
    screenStructs::screenDriverStatus * obj = reinterpret_cast<screenStructs::screenDriverStatus *> (ms->obj);
    obj->position = args;

// while the screen is moving ther eis no point updating the casseete position
//    screenStructs::SCREEN_STATE newscreenstate;
//    if( isHorizontal( obj->dir)  )
//        updateCassettePosition( allScreentData[obj->parentScreen].driver.hCassette,  obj->position );
//    else if( isVertical(obj->dir) )
//        updateCassettePosition( allScreentData[obj->parentScreen].driver.vCassette,  obj->position );

    std::stringstream ss;
    ss << obj->parentScreen;
    if( isHorizontal( obj->dir)  )
        ss << " Horizontal";
    else if( isVertical(obj->dir) )
        ss << " Vertical";
    else
        ss << " ERROR";
    ss << " Driver Position = " << obj->position;
    message(ss);
}
////_________________________________________________________________________________________________________________
void screenInterface::updateEN( screenStructs::monitorStruct * ms, const unsigned short args )
{   //std::cout << "updateProtStatus(rfProtStructs::rfGunProtObject& obj,const long value) called" << std::endl;
    screenStructs::screenDriverStatus * obj = reinterpret_cast<screenStructs::screenDriverStatus *> (ms->obj);
//    obj->position = args;

     switch(args)
     {
        case 1:
            obj->state = screenStructs::DRIVER_STATE::DRIVER_ENABLED;
            //message(obj.name , " status = GOOD ");
            break;
        case 0:
            obj->state = screenStructs::DRIVER_STATE::DRIVER_DISABLED;
            //message(obj.name , " status = BAD ");
            break;
        default:
            obj->state = screenStructs::DRIVER_STATE::DRIVER_ERROR;
            //message(obj.name , " status = ERROR ");
     }
}
//_________________________________________________________________________________________________________________
void screenInterface::updateCassettePosition( screenStructs::screenCassette  & cas, const double pos  )
{//currently this only gets called from updateRPOS, so we KNOW the object exists!!
    //debugMessage("called updateCassettePosition");
    screenStructs::SCREEN_STATE st = screenStructs::SCREEN_STATE::SCREEN_UNKNOWN;
    bool matchedpos = false; //have we found a position that matches the position of a cassette element
    for( auto && it : cas.cassetteElements )
    {
        if( it.second )
        {
            //debugMessage("checking ", pos, " with ", it.first, " = ", cas.cassetteElementsPosition.at( it.first ), " tolerance  = ",  cas.posTolerance  );
            if( areSame( pos, cas.cassetteElementsPosition.at(it.first ), cas.posTolerance) )
            {
                //debugMessage("posmatch = ", ENUM_TO_STRING(it.first) );
                cas.screenState = it.first;
                matchedpos = true;
                break;
            }
        }
    }
    if(!matchedpos)
        cas.screenState = screenStructs::SCREEN_STATE::UNKNOWN_POSITION;

    debugMessage( cas.parentScreen, " hcas state ", ENUM_TO_STRING( allScreentData.at(cas.parentScreen).driver.hCassette.screenState ) );
    debugMessage( cas.parentScreen, " vcas state ", ENUM_TO_STRING( allScreentData.at(cas.parentScreen).driver.vCassette.screenState ) );
    // update the "highest level" screenState
    updateScreenState(allScreentData.at(cas.parentScreen));
    debugMessage(cas.parentScreen, " screen state is now ", ENUM_TO_STRING( getScreenState( cas.parentScreen ) ) );
}
//_________________________________________________________________________________________________________________
void screenInterface::updatePROT01( screenStructs::monitorStruct * ms, const double args )
{
    screenStructs::screenDriverStatus * obj = reinterpret_cast<screenStructs::screenDriverStatus *> (ms->obj);
    std::stringstream ss;
    ss << obj->parentScreen;
    if( isHorizontal( obj->dir)  )
        ss << " Horizontal Driver ";
    else if( isVertical(obj->dir) )
        ss<< " Vertical Driver ";
    else
        ss<< " ERROR";
    if( args == 0.0 )//MAGIC_NUMBER
    {
        obj->state = screenStructs::DRIVER_STATE::DRIVER_ENABLED;
    }
    else if( args == 1.0 )//MAGIC_NUMBER
    {
        obj->state = screenStructs::DRIVER_STATE::DRIVER_DISABLED;
    }
    else
        obj->state = screenStructs::DRIVER_ERROR;
    ss << "state = " << ENUM_TO_STRING( obj->state );
    debugMessage(ss);
}
//_________________________________________________________________________________________________________________
void screenInterface::updateSta( screenStructs::monitorStruct * ms,  const unsigned short args )
{   // Sta is for the VELA_PNEUMATIC screens ONLY, VELA_HV_MOVER are updated by updateScreenState()
    // probably evil
//    screenStructs::velaINJscreenObject * obj = reinterpret_cast<screenStructs::velaINJscreenObject*> (ms->obj);
//    switch( args )
//    {
//        case 0:
//            obj->screenState = screenStructs::SCREEN_STATE::SCREEN_OUT;
//            break;
//        case 1:
//            obj ->screenState = screenStructs::SCREEN_STATE::SCREEN_IN;
//            break;
//        case 2:
//            obj->screenState = screenStructs::SCREEN_STATE::SCREEN_MOVING;
//            break;
//        default:
//            obj->screenState = screenStructs::SCREEN_STATE::SCREEN_ERROR;
//    }
//    ms->interface->debugMessage( obj->name ," screenState = ", ENUM_TO_STRING(obj->screenState) );
}
////_________________________________________________________________________________________________________________
void screenInterface::updateScreenState( screenStructs::screenObject & scr  )
{// this function is for the VELA_HV_MOVER screens
//    screenStructs::SCREEN_STATE hpos = scr.driver.hCassette;
//    screenStructs::SCREEN_STATE vpos = scr.driver.vCassette;
    if( is_HandV_CassetteOUT( scr) )
        scr.screenState = screenStructs::SCREEN_STATE::SCREEN_OUT;
    else if( isHCassetteOUT(scr) )
        scr.screenState = scr.driver.vCassette.screenState;
    else if( isVCassetteOUT(scr) )
        scr.screenState = scr.driver.hCassette.screenState;
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_HandV_CassetteOUT(screenStructs::screenObject & scr)
{
    return isHCassetteOUT(scr) && isVCassetteOUT(scr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHCassetteOUT(screenStructs::screenObject & scr)
{
    return scr.driver.hCassette.screenState == screenStructs::SCREEN_STATE::H_OUT;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVCassetteOUT(screenStructs::screenObject & scr)
{
    return scr.driver.vCassette.screenState == screenStructs::SCREEN_STATE::V_OUT;
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
//___________________________________________________________________________________________________________
std::vector<bool> screenInterface::isScreenOUT( const std::vector<std::string> & name )
{
    std::vector<bool> r;
    for( auto && it : name )
        r.push_back(isScreenOUT(it,false));
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::isScreenIN(const std::string & name, bool weKnowEntryExists)
{
    if( entryExists2( name,weKnowEntryExists ) )
    {
        if( allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_IN )
            return true;
        if( allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::H_YAG )
            return true;
        if( allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::V_YAG )
            return true;
    }
    return false;
}
//___________________________________________________________________________________________________________
screenStructs::SCREEN_STATE screenInterface::getScreenState(const std::string & name, const bool weKnowEntryExists)
{
    if( weKnowEntryExists )
        return allScreentData.at(name).screenState;
    else if( entryExists( allScreentData, name ) )
        return allScreentData.at(name).screenState;
    else
        return screenStructs::SCREEN_STATE::SCREEN_UNKNOWN;
}
//___________________________________________________________________________________________________________
bool screenInterface::isScreenInState(const std::string & name, screenStructs::SCREEN_STATE sta)
{
    bool r = false;
    if( entryExists( allScreentData, name ) )
        r = allScreentData.at(name).screenState == sta;
    return r;
}
//___________________________________________________________________________________________________________


/// COMMMANDS


//___________________________________________________________________________________________________________
bool screenInterface::screenIN( const std::string & name  )
{   message("screenInterface::screenIN(const std::string & name ) called");
    const  std::vector< std::string > names = { name };
    return screenIN( names );
}
//___________________________________________________________________________________________________________
bool screenInterface::screenOUT( const std::string & name  )
{   message("screenInterface::screenOUT(const std::string & name ) called");
    const  std::vector< std::string > names = { name };
    return screenOUT( names );
}
//___________________________________________________________________________________________________________
bool screenInterface::screenIN( const std::vector< std::string > & names  )
{
    std::vector< screenStructs::SCREEN_STATE > v( names.size(), screenStructs::SCREEN_STATE::SCREEN_IN );
    return screenMoveTo( names, v);
}
//___________________________________________________________________________________________________________
bool screenInterface::screenOUT( const std::vector< std::string > & names  )
{
    std::vector< screenStructs::SCREEN_STATE > v( names.size(), screenStructs::SCREEN_STATE::SCREEN_OUT );
    bool r = screenMoveTo( names, v);
    return r;
}
//___________________________________________________________________________________________________________
//___________________________________________________________________________________________________________
//   CREATE ONE HIGH LEVEL SCREEN MOVER FUNCTION THAT IS CALLED BY ALL THE HIGHER LEVEL VERSIONS OF screenIN,
//   screenOUT AND MOVE TO CASSETTE ELEMENT POSITION
//___________________________________________________________________________________________________________
bool screenInterface::screenMoveTo( const std::string & name, const screenStructs::SCREEN_STATE & states)
{
    std::vector< screenStructs::SCREEN_STATE > v( 1, states );
    std::vector< std::string> n( 1, name );
    return  screenMoveTo( n, v);
}
//___________________________________________________________________________________________________________
bool screenInterface::screenMoveTo(const std::vector<std::string>& names,const std::vector<screenStructs::SCREEN_STATE>& states  )
{
    // all the (above?) client-side screen moving functions should funnel to here
    // this is where we actually start moving screens.
    // **** if the CLARA screens are significantly different this function will need to be edited ****
    // we need vectors divided up by screen type
    // it would be nice if c++ had a zip function

    killFinishedMoveThreads();

    // first cut out any screens that are "moving"

    if( names.size() != states.size() )
    {
        debugMessage("ERROR, trying to set  ", names.size(), " screens with ", states.size(), " states. Aborting screen movement");
    }
    else
    {
        debugMessage("screenInterface::screenMoveTo called with following parameters:");
        for( auto && i = 0; i < names.size(); ++i )
        {
            debugMessage(names[i], " to state ", ENUM_TO_STRING(states[i]) );
        }

        std::vector<std::string> nonMovingScr;
        std::vector<screenStructs::SCREEN_STATE> nonMovingSta;
        // get the screens that are not locked and are moving
        debugMessage("Getting not locked and not moving screens");
        bool carry_on =  get_NOT_Locked_and_NOT_MovingScreens(names, nonMovingScr, states, nonMovingSta);

        if( nonMovingScr.size() > 0 &&  nonMovingScr.size() == nonMovingSta.size()  )
        {
            // vectors to fill
            std::vector<screenStructs::SCREEN_STATE> VELA_PNEUMATIC_Sta,VELA_HV_MOVER_Sta;
            std::vector<std::string>                 VELA_PNEUMATIC_Scr,VELA_HV_MOVER_Scr;
            std::vector<screenStructs::SCREEN_STATE> CLARA_PNEUMATIC_Sta,CLARA_HV_MOVER_Sta,CLARA_V_MOVER_Sta;
            std::vector<std::string>                 CLARA_PNEUMATIC_Scr,CLARA_HV_MOVER_Scr,CLARA_V_MOVER_Scr;
            // sort the screens by type
            debugMessage("Sorting Pnuematic and mover screens types");

            bool carryon = sortPnuematicMoverScreen(nonMovingScr,
                                                    VELA_PNEUMATIC_Scr,VELA_HV_MOVER_Scr,CLARA_PNEUMATIC_Scr,CLARA_HV_MOVER_Scr,CLARA_V_MOVER_Scr,
                                                    nonMovingSta,
                                                    VELA_PNEUMATIC_Sta,VELA_HV_MOVER_Sta,CLARA_PNEUMATIC_Sta,CLARA_HV_MOVER_Sta,CLARA_V_MOVER_Sta);
            bool VELA_PNEUMATIC_Screens_suc = false;
            bool VELA_HV_MOVER_Screens_suc = false;
            bool CLARA_PNEUMATIC_Screens_suc = false;
            bool CLARA_HV_MOVER_Screens_suc = false;
            bool CLARA_V_MOVER_Screens_suc = false;
            if( carryon )
            {
                if(VELA_PNEUMATIC_Scr.size()>0)//MAGIC_NUMBER
                {
                    debugMessage("moving in VELA_PNEUMATIC_Scr ");
                    VELA_PNEUMATIC_Screens_suc = move_VELA_PNEUMATIC_Screens( VELA_PNEUMATIC_Scr, VELA_PNEUMATIC_Sta );
                }
                if(VELA_HV_MOVER_Scr.size() > 0  )//MAGIC_NUMBER
                {
                    debugMessage("moving in VELA_HV_MOVER__Scr ");
                    VELA_HV_MOVER_Screens_suc = move_VELA_HV_MOVER_Screens( VELA_HV_MOVER_Scr, VELA_HV_MOVER_Sta );
                }
                if(CLARA_PNEUMATIC_Scr.size()>0)//MAGIC_NUMBER
                {
                    debugMessage("moving in CLARA_PNEUMATIC_Scr ");
                    CLARA_PNEUMATIC_Screens_suc = move_CLARA_PNEUMATIC_Screens( CLARA_PNEUMATIC_Scr, CLARA_PNEUMATIC_Sta );
                }
                if(CLARA_HV_MOVER_Scr.size() > 0  )//MAGIC_NUMBER
                {
                    debugMessage("moving in CLARA_HV_MOVER_Scr ");
                    CLARA_HV_MOVER_Screens_suc = move_CLARA_HV_MOVER_Screens( CLARA_HV_MOVER_Scr, CLARA_HV_MOVER_Sta );
                }
                if(CLARA_V_MOVER_Scr.size() > 0  )//MAGIC_NUMBER
                {
                    debugMessage("moving in CLARA_V_MOVER__Scr ");
                    CLARA_V_MOVER_Screens_suc = move_CLARA_V_MOVER_Screens( CLARA_V_MOVER_Scr, CLARA_V_MOVER_Sta );
                }
            }
            if( VELA_HV_MOVER_Screens_suc && VELA_PNEUMATIC_Screens_suc && CLARA_HV_MOVER_Screens_suc && CLARA_V_MOVER_Screens_suc && CLARA_PNEUMATIC_Screens_suc )
                return true;
            else
                return false;
        }// if( nonMovingScr.size() > 0 )
        else
            message("Requested screens are currenlty moving or are locked");
    }// if( names.size() != states.size() )

    return false;
}
//___________________________________________________________________________________________________________
bool screenInterface::get_NOT_Locked_and_NOT_MovingScreens(const std::vector<std::string>& scrIN, std::vector<std::string>& scrOUT,
                                                      const std::vector<screenStructs::SCREEN_STATE>& staIN,
                                                      std::vector<screenStructs::SCREEN_STATE>& staOUT  )
{
    bool r = true;
    std::vector<bool> tests;
    auto itNam = scrIN.begin();
    auto itSta = staIN.begin();
    while(itNam != scrIN.end() || itSta != staIN.end())
    {
        tests.clear();
        tests = exists_and_isNotLocked( *itNam );

        if( tests[0] && tests[1] )
        {
            if( isMoving( *itNam ) )
                message( *itNam, " is still moving from previous move request. Please wait for that to finish.");
            else
            {
                scrOUT.push_back(*itNam);
                staOUT.push_back(*itSta);
                debugMessage("get_NOT_Locked_and_NOT_MovingScreens added ", *itNam, ", state, ", ENUM_TO_STRING(*itSta) ) ;
            }
        }
        if( !tests[1] )
            message( *itNam, " is locked from previous move request. Please wait for that to finish.");
//        if( entryExists( isLockedMap, *itNam) )
//        {
//            if( isLockedMap[*itNam] )
//                message( *itNam, " is locked from previous move request. Please wait for that to finish.");
//            else if( isMoving( *itNam ) )
//                message( *itNam, " is still moving from previous move request. Please wait for that to finish.");
//            else
//            {
//                scrOUT.push_back(*itNam);
//                staOUT.push_back(*itSta);
//            }
//        }
        if(itNam != scrIN.end())
            ++itNam;
        if(itSta != staIN.end())
            ++itSta;
    }
    if( scrOUT.size() != staOUT.size() )
    {
        message("ABORT! Major Error! in screenInterface::get_NOT_Locked_and_NOT_MovingScreens scrOUT.size() != staOUT.size() ",
                scrOUT.size()," != ",staOUT.size() );
        r = false;
    }
    return r;
}
//___________________________________________________________________________________________________________
bool screenInterface::sortPnuematicMoverScreen( const std::vector<std::string>& scrIN,
                                                std::vector<std::string>& VELA_PNEUMATIC_Scr,
                                                std::vector<std::string>& VELA_HV_MOVER_Scr,
                                                std::vector<std::string>& CLARA_PNEUMATIC_Scr,
                                                std::vector<std::string>& CLARA_HV_MOVER_Scr,
                                                std::vector<std::string>& CLARA_V_MOVER_Scr,
                                                const std::vector<screenStructs::SCREEN_STATE>& staIN,
                                                std::vector<screenStructs::SCREEN_STATE>& VELA_PNEUMATIC_Sta,
                                                std::vector<screenStructs::SCREEN_STATE>& VELA_HV_MOVER_Sta,
                                                std::vector<screenStructs::SCREEN_STATE>& CLARA_PNEUMATIC_Sta,
                                                std::vector<screenStructs::SCREEN_STATE>& CLARA_HV_MOVER_Sta,
                                                std::vector<screenStructs::SCREEN_STATE>& CLARA_V_MOVER_Sta   )
{
    bool carryon = true;
    auto itNam = scrIN.begin();
    auto itSta = staIN.begin();
    // vectors to fill
    while(itNam != scrIN.end() || itSta != staIN.end() )
    {
        debugMessage("Checking ", *itNam," with request state: ", *itSta);
        // this loop goes through every screen and checks why type it is
        // it also decideds if it needs moving, and where it should be moved to
        if( is_VELA_PNEUMATIC(*itNam))
        {
            // these can only be moved to IN or OUT, so check that is what has been asked for
            if( screen_is_out_AND_sta_is_in(*itNam,*itSta) || screen_is_in_AND_sta_is_out(*itNam,*itSta) )
            {
                VELA_PNEUMATIC_Sta.push_back(*itSta);
                VELA_PNEUMATIC_Scr.push_back(*itNam);
                debugMessage("Added ",*itNam," with state = ", ENUM_TO_STRING(*itSta)," to VELA_pnuematic ");
            }
            else
                message("ERROR ",*itNam, " can't be moved to state ", ENUM_TO_STRING(*itSta), ", current state is "
                        ,ENUM_TO_STRING(getScreenState(*itNam)) );
        }
        else if( is_VELA_HV_MOVER(*itNam)  )
        {
//            if( isNotLockedAndExists(*itNam) )
//            {
//                if( isNotMoving(*itNam,true) )
//                {
                    if( !isScreenInState(*itNam, *itSta ) ) // check is the screen is already in the state requested, is not carry on
                    {
                        VELA_HV_MOVER_Scr.push_back(*itNam);
                        // if the status is SCREEN_IN assume screens are always V_YAG
                        // clients will still be able to exactly specifiy elements with the geenral function
                        if( *itSta == screenStructs::SCREEN_STATE::SCREEN_IN )
                            VELA_HV_MOVER_Sta.push_back( screenStructs::SCREEN_STATE::V_YAG ); // MAGIC_NUMBER
                        else if( *itSta == screenStructs::SCREEN_STATE::SCREEN_OUT )
                        {
                            if(  isScreenOUT(*itNam)  ) // asked for out and it is out
                            {
                            }
                            else
                            {
                                //screenStructs::SCREEN_STATE currentSta = getScreenState(*itNam);
                                if( isHCassetteOUT( allScreentData.at(*itNam) ) )
                                    VELA_HV_MOVER_Sta.push_back(screenStructs::SCREEN_STATE::V_OUT); // MAGIC_NUMBER enum
                                if( isVCassetteOUT( allScreentData.at(*itNam) ) )
                                    VELA_HV_MOVER_Sta.push_back(screenStructs::SCREEN_STATE::H_OUT); // MAGIC_NUMBER
                            }
                        }
                        else // its not IN or OUT so just add what has been requested
                           VELA_HV_MOVER_Sta.push_back(*itSta);

                        debugMessage("Added ",*itNam," with state = ", ENUM_TO_STRING(*itSta)," to VELA_HV_MOVER_Sta");

                    }
                    else
                        message(*itNam," is already in state ", ENUM_TO_STRING(*itSta) );

//                }
//                else
//                 message( "Can't move ", *itNam, " it is already moving ");
//            }
//            else
//                message( "Can't move ", *itNam, " it is locked");
        }
        else if( is_CLARA_PNEUMATIC(*itNam))
        {
            // these can only be moved to IN or OUT, so check that is what has been asked for
            if( screen_is_out_AND_sta_is_in(*itNam,*itSta) || screen_is_in_AND_sta_is_out(*itNam,*itSta) )
            {
                CLARA_PNEUMATIC_Sta.push_back(*itSta);
                CLARA_PNEUMATIC_Scr.push_back(*itNam);
                debugMessage("Added ",*itNam," with state = ", ENUM_TO_STRING(*itSta)," to CLARA_pnuematic ");
            }
            else
                message("ERROR ",*itNam, " can't be moved to state ", ENUM_TO_STRING(*itSta), ", current state is "
                        ,ENUM_TO_STRING(getScreenState(*itNam)) );
        }
        else if( is_CLARA_HV_MOVER(*itNam)  )
        {
                    if( !isScreenInState(*itNam, *itSta ) ) // check is the screen is already in the state requested, is not carry on
                    {
                        CLARA_HV_MOVER_Scr.push_back(*itNam);
                        // if the status is SCREEN_IN assume screens are always V_YAG
                        // clients will still be able to exactly specifiy elements with the geenral function
                        if( *itSta == screenStructs::SCREEN_STATE::SCREEN_IN )
                            CLARA_HV_MOVER_Sta.push_back( screenStructs::SCREEN_STATE::V_YAG ); // MAGIC_NUMBER
                        else if( *itSta == screenStructs::SCREEN_STATE::SCREEN_OUT )
                        {
                            if(  isScreenOUT(*itNam)  ) // asked for out and it is out
                            {
                            }
                            else
                            {
                                //screenStructs::SCREEN_STATE currentSta = getScreenState(*itNam);
                                if( isHCassetteOUT( allScreentData.at(*itNam) ) )
                                    CLARA_HV_MOVER_Sta.push_back(screenStructs::SCREEN_STATE::V_OUT); // MAGIC_NUMBER enum
                                if( isVCassetteOUT( allScreentData.at(*itNam) ) )
                                    CLARA_HV_MOVER_Sta.push_back(screenStructs::SCREEN_STATE::H_OUT); // MAGIC_NUMBER
                            }
                        }
                        else // its not IN or OUT so just add what has been requested
                           CLARA_HV_MOVER_Sta.push_back(*itSta);

                        debugMessage("Added ",*itNam," with state = ", ENUM_TO_STRING(*itSta)," to CLARA_HV_MOVER_Sta");

                    }
                    else
                        message(*itNam," is already in state ", ENUM_TO_STRING(*itSta) );
        }
        else if( is_CLARA_V_MOVER(*itNam)  )
        {

                    if( !isScreenInState(*itNam, *itSta ) ) // check is the screen is already in the state requested, is not carry on
                    {
                        CLARA_V_MOVER_Scr.push_back(*itNam);
                        // if the status is SCREEN_IN assume screens are always V_YAG
                        // clients will still be able to exactly specifiy elements with the geenral function
                        if( *itSta == screenStructs::SCREEN_STATE::SCREEN_IN )
                            CLARA_V_MOVER_Sta.push_back( screenStructs::SCREEN_STATE::V_YAG ); // MAGIC_NUMBER
                        else if( *itSta == screenStructs::SCREEN_STATE::SCREEN_OUT )
                        {
                            if(  isScreenOUT(*itNam)  ) // asked for out and it is out
                            {
                            }
                            else
                            {
                                if( isVCassetteOUT( allScreentData.at(*itNam) ) )
                                    CLARA_HV_MOVER_Sta.push_back(screenStructs::SCREEN_STATE::H_OUT); // MAGIC_NUMBER
                            }
                        }
                        else // its not IN or OUT so just add what has been requested
                           CLARA_V_MOVER_Sta.push_back(*itSta);

                        debugMessage("Added ",*itNam," with state = ", ENUM_TO_STRING(*itSta)," to CLARA_V_MOVER_Sta");

                    }
                    else
                        message(*itNam," is already in state ", ENUM_TO_STRING(*itSta) );
        }
        if(itNam != scrIN.end())
            ++itNam;
        if(itSta != staIN.end())
            ++itSta;
        // sanity check
        if(VELA_HV_MOVER_Sta.size() != VELA_HV_MOVER_Scr.size())
        {
            message("ABORT! Major Error! in screenInterface::screenMoveTo VELA_HV_MOVER__Sta.size() != VELA_HV_MOVER__Scr.size() ",
                    VELA_HV_MOVER_Sta.size()," != ", VELA_HV_MOVER_Scr.size() );
            carryon = false;
        }
        if(CLARA_HV_MOVER_Sta.size() != CLARA_HV_MOVER_Scr.size())
        {
            message("ABORT! Major Error! in screenInterface::screenMoveTo CLARA_HV_MOVER__Sta.size() != CLARA_HV_MOVER__Scr.size() ",
                    CLARA_HV_MOVER_Sta.size()," != ", CLARA_HV_MOVER_Scr.size() );
            carryon = false;
        }
        if(CLARA_V_MOVER_Sta.size() != CLARA_V_MOVER_Scr.size())
        {
            message("ABORT! Major Error! in screenInterface::screenMoveTo CLARA_V_MOVER__Sta.size() != CLARA_V_MOVER__Scr.size() ",
                    CLARA_V_MOVER_Sta.size()," != ", CLARA_V_MOVER_Scr.size() );
            carryon = false;
        }
    }
    return carryon;
}
//___________________________________________________________________________________________________________
bool screenInterface::move_VELA_PNEUMATIC_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  )
{// follwoing how thigns are done in the magnetInterface, we set up a vector of pointers to chids and chtypes, then call the generic sendCommand
    bool success = false;
    std::vector< chid* > CHIDS;
    std::vector< chtype* > CHTYPE;
    // it would be nice if c++ had a zip function
    auto itNam = names.begin();
    auto itSta = states.begin();

    while(itNam != names.end() || itSta != states.end())
    {
        // we have checked if the screen exists in screenMoveTo.. is_VELA_PNEUMATIC(..)
        if(  *itSta == screenStructs::SCREEN_STATE::SCREEN_IN  )
        {
            message("adding on chids");
            CHTYPE.push_back( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::On ].CHTYPE );
            CHIDS.push_back ( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::On ].CHID   );
        }
        else if(  *itSta == screenStructs::SCREEN_STATE::SCREEN_OUT  )
        {
            message("adding on chids");
            CHTYPE.push_back( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::Off ].CHTYPE );
            CHIDS.push_back ( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::Off ].CHID   );
        }
        if(itNam != names.end())
            ++itNam;
        if(itSta != states.end())
            ++itSta;
    }
    if(CHTYPE.size()>0)// MAGIC_NUMBER
    {
        std::string m1,m2;
        m1 = "Timeout sending EPICS_ACTIVATE to move_OUT_VELA_PNEUMATIC_Screens";
        m2 = "Timeout sending EPICS_SEND to move_OUT_VELA_PNEUMATIC_Screens";
        success = send_VELA_PNEUMATIC_Command(CHTYPE, CHIDS, m1, m2);
    }
    return success;
}
//______________________________________________________________________________
bool screenInterface::send_VELA_PNEUMATIC_Command( const std::vector< chtype* > & CHTYPE, const std::vector< chid* > & CHID, const std::string & m1, const std::string & m2  )
{
    bool ret = false;
    for( auto i = 0; i < CHTYPE.size(); ++i )//MAGIC_NUMBERS and evilness
        ca_put( *CHTYPE[i], *CHID[i], &EPICS_ACTIVATE );
    message( "activate" );
    int status = sendToEpics( "ca_put", "", m1.c_str() );
    if ( status == ECA_NORMAL )
    {
        for( auto i = 0; i < CHTYPE.size(); ++i )//MAGIC_NUMBERS and evilness
            ca_put( *CHTYPE[i], *CHID[i], &EPICS_SEND );
        int status = sendToEpics( "ca_put", "", m2.c_str());
        if ( status == ECA_NORMAL )
            ret = true;
            //std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); // MAGIC_NUMBER
        else
            message( " status == ECA_NORMAL" );
    }
    else
        message( "EPICS_ACTIVATE did not return ECA_NORMAL" );
    return ret;
}
//___________________________________________________________________________________________________________
bool screenInterface::move_CLARA_PNEUMATIC_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  )
{// follwoing how thigns are done in the magnetInterface, we set up a vector of pointers to chids and chtypes, then call the generic sendCommand
    bool success = false;
    std::vector< chid* > CHIDS;
    std::vector< chtype* > CHTYPE;
    // it would be nice if c++ had a zip function
    auto itNam = names.begin();
    auto itSta = states.begin();

    while(itNam != names.end() || itSta != states.end())
    {
        // we have checked if the screen exists in screenMoveTo.. is_VELA_PNEUMATIC(..)
        if(  *itSta == screenStructs::SCREEN_STATE::SCREEN_IN  )
        {
            message("adding on chids");
            CHTYPE.push_back( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::On ].CHTYPE );
            CHIDS.push_back ( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::On ].CHID   );
        }
        else if(  *itSta == screenStructs::SCREEN_STATE::SCREEN_OUT  )
        {
            message("adding on chids");
            CHTYPE.push_back( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::Off ].CHTYPE );
            CHIDS.push_back ( &allScreentData.at(*itNam).pvComStructs[ screenStructs::SCREEN_PV_TYPE::Off ].CHID   );
        }
        if(itNam != names.end())
            ++itNam;
        if(itSta != states.end())
            ++itSta;
    }
    if(CHTYPE.size()>0)// MAGIC_NUMBER
    {
        std::string m1,m2;
        m1 = "Timeout sending EPICS_ACTIVATE to move_OUT_CLARA_PNEUMATIC_Screens";
        m2 = "Timeout sending EPICS_SEND to move_OUT_CLARA_PNEUMATIC_Screens";
        success = send_VELA_PNEUMATIC_Command(CHTYPE, CHIDS, m1, m2);
    }
    return success;
}
//______________________________________________________________________________
bool screenInterface::send_CLARA_PNEUMATIC_Command( const std::vector< chtype* > & CHTYPE, const std::vector< chid* > & CHID, const std::string & m1, const std::string & m2  )
{
    bool ret = false;
    for( auto i = 0; i < CHTYPE.size(); ++i )//MAGIC_NUMBERS and evilness
        ca_put( *CHTYPE[i], *CHID[i], &EPICS_ACTIVATE );
    message( "activate" );
    int status = sendToEpics( "ca_put", "", m1.c_str() );
    if ( status == ECA_NORMAL )
    {
        for( auto i = 0; i < CHTYPE.size(); ++i )//MAGIC_NUMBERS and evilness
            ca_put( *CHTYPE[i], *CHID[i], &EPICS_SEND );
        int status = sendToEpics( "ca_put", "", m2.c_str());
        if ( status == ECA_NORMAL )
            ret = true;
            //std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); // MAGIC_NUMBER
        else
            message( " status == ECA_NORMAL" );
    }
    else
        message( "EPICS_ACTIVATE did not return ECA_NORMAL" );
    return ret;
}
//______________________________________________________________________________
bool screenInterface::move_VELA_HV_MOVER_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  )
{   // to move in the screen we must first check if the screen is in
    // then  where the screen is, is it on H or V
    // there are screens which require complex movement on multiple notors
    // and screens that just require one movement on 1 motor, so split accordingly
    std::vector<std::string> twoMotorMoves_Scr;
    std::vector<screenStructs::SCREEN_STATE> twoMotorMoves_Sta;
    //i need to sort out the logic for the scren_out and screen_in states,
    //probably will be a switch or some nested ifs to handle the cases.

    auto itNam = names.begin();
    auto itSta = states.begin();
    while(itNam != names.end() || itSta != states.end())
    {
        // we have checked if the screen exists, and is not in the requested state  in screenMoveTo.. is_VELA_HV_MOVER(..)
        // now we need to check is the H/V driver is enabled and if the state is an H or V state
        if( is_H_element_AND_HDriveEnabled(*itSta, allScreentData.at(*itNam).driver) ) // h element and H enabled send it out
        {
            set_VELA_HV_MOVER_Position( *itNam, screenStructs::DRIVER_DIRECTION::HORIZONTAL,*itSta);
        }
        else if( is_V_element_AND_VDriveEnabled(*itSta, allScreentData.at(*itNam).driver) ) // v element and VH enabled send it out
        {
            set_VELA_HV_MOVER_Position(*itNam, screenStructs::DRIVER_DIRECTION::VERTICAL,*itSta);
        }
        else
        {
            //we can move out the enabled motor here
            if( isHDriveEnabled(allScreentData.at(*itNam).driver) )
            {
                debugMessage("set_VELA_HV_MOVER_Position to H_OUT");
                set_VELA_HV_MOVER_Position(*itNam,screenStructs::DRIVER_DIRECTION::HORIZONTAL,screenStructs::SCREEN_STATE::H_OUT);

            }
            //we can move out the enabled motor here
            if( isVDriveEnabled(allScreentData.at(*itNam).driver) )
            {
                debugMessage("set_VELA_HV_MOVER_Position to H_OUT");
                set_VELA_HV_MOVER_Position(*itNam,screenStructs::DRIVER_DIRECTION::VERTICAL,screenStructs::SCREEN_STATE::V_OUT);
            }
            twoMotorMoves_Scr.push_back(*itNam);
            twoMotorMoves_Sta.push_back(*itSta);
        }
        if(itNam != names.end())
            ++itNam;
        if(itSta != states.end())
            ++itSta;
    }
    if(twoMotorMoves_Scr.size() >0 )
    {// enter a new thread and set the LOCK on the screen
//        screenDualMoveStructsMap[ dualMoveNum ].interface   = this;
//        screenDualMoveStructsMap[ dualMoveNum ].key   = dualMoveNum;
//        screenDualMoveStructsMap[ dualMoveNum ].dualMotorMoves_Scr   = twoMotorMoves_Scr;
//        screenDualMoveStructsMap[ dualMoveNum ].dualMotorMoves_Sta   = twoMotorMoves_Sta;
//        screenDualMoveStructsMap[ dualMoveNum ].thread     = new std::thread( staticEntryDualMove, std::ref(screenDualMoveStructsMap[ dualMoveNum ] ) );
//        screenDualMoveStructsMap[ dualMoveNum ].isComplete = false;
//        ++dualMoveNum;
    }
    return false;
}
//______________________________________________________________________________
bool screenInterface::move_CLARA_HV_MOVER_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  )
{   // to move in the screen we must first check if the screen is in
    // then  where the screen is, is it on H or V
    // there are screens which require complex movement on multiple notors
    // and screens that just require one movement on 1 motor, so split accordingly
    std::vector<std::string> twoMotorMoves_Scr;
    std::vector<screenStructs::SCREEN_STATE> twoMotorMoves_Sta;
    //i need to sort out the logic for the scren_out and screen_in states,
    //probably will be a switch or some nested ifs to handle the cases.

    auto itNam = names.begin();
    auto itSta = states.begin();
    while(itNam != names.end() || itSta != states.end())
    {
        // we have checked if the screen exists, and is not in the requested state  in screenMoveTo.. is_VELA_HV_MOVER(..)
        // now we need to check is the H/V driver is enabled and if the state is an H or V state
        if( is_H_element_AND_HDriveEnabled(*itSta, allScreentData.at(*itNam).driver) ) // h element and H enabled send it out
        {
            set_CLARA_HV_MOVER_Position( *itNam, screenStructs::DRIVER_DIRECTION::HORIZONTAL,*itSta);
        }
        else if( is_V_element_AND_VDriveEnabled(*itSta, allScreentData.at(*itNam).driver) ) // v element and VH enabled send it out
        {
            set_CLARA_HV_MOVER_Position(*itNam, screenStructs::DRIVER_DIRECTION::VERTICAL,*itSta);
        }
        else
        {
            //we can move out the enabled motor here
            if( isHDriveEnabled(allScreentData.at(*itNam).driver) )
            {
                debugMessage("set_CLARA_HV_MOVER_Position to H_OUT");
                set_CLARA_HV_MOVER_Position(*itNam,screenStructs::DRIVER_DIRECTION::HORIZONTAL,screenStructs::SCREEN_STATE::H_OUT);

            }
            //we can move out the enabled motor here
            if( isVDriveEnabled(allScreentData.at(*itNam).driver) )
            {
                debugMessage("set_CLARA_HV_MOVER_Position to H_OUT");
                set_CLARA_HV_MOVER_Position(*itNam,screenStructs::DRIVER_DIRECTION::VERTICAL,screenStructs::SCREEN_STATE::V_OUT);
            }
            twoMotorMoves_Scr.push_back(*itNam);
            twoMotorMoves_Sta.push_back(*itSta);
        }
        if(itNam != names.end())
            ++itNam;
        if(itSta != states.end())
            ++itSta;
    }
    if(twoMotorMoves_Scr.size() >0 )
    {
    }
    return false;
}
//______________________________________________________________________________
bool screenInterface::move_CLARA_V_MOVER_Screens(const std::vector<std::string> & names, const std::vector< screenStructs::SCREEN_STATE > & states  )
{   // to move in the screen we must first check if the screen is in
    // then  where the screen is, is it on H or V
    // there are screens which require complex movement on multiple notors
    // and screens that just require one movement on 1 motor, so split accordingly
    std::vector<std::string> twoMotorMoves_Scr;
    std::vector<screenStructs::SCREEN_STATE> twoMotorMoves_Sta;
    //i need to sort out the logic for the scren_out and screen_in states,
    //probably will be a switch or some nested ifs to handle the cases.

    auto itNam = names.begin();
    auto itSta = states.begin();
    while(itNam != names.end() || itSta != states.end())
    {
        if( is_V_element_AND_VDriveEnabled(*itSta, allScreentData.at(*itNam).driver) ) // v element and VH enabled send it out
        {
            set_CLARA_HV_MOVER_Position(*itNam, screenStructs::DRIVER_DIRECTION::VERTICAL,*itSta);
        }
        else
        {
            if( isVDriveEnabled(allScreentData.at(*itNam).driver) )
            {
                debugMessage("set_CLARA_HV_MOVER_Position to H_OUT");
                set_CLARA_HV_MOVER_Position(*itNam,screenStructs::DRIVER_DIRECTION::VERTICAL,screenStructs::SCREEN_STATE::V_OUT);
            }
            twoMotorMoves_Scr.push_back(*itNam);
            twoMotorMoves_Sta.push_back(*itSta);
        }
        if(itNam != names.end())
            ++itNam;
        if(itSta != states.end())
            ++itSta;
    }
    if(twoMotorMoves_Scr.size() >0 )
    {
    }
    return false;
}
//______________________________________________________________________________
void screenInterface::staticEntryDualMove(screenStructs::HV_dualMoveStruct & ms )
{
//    ms.interface -> debugMessage("staticEntryDualMove called ! ");
//    time_t timeStart = time( 0 ); /// start clock  //MAGIC_NUMBER
//    time_t TIMEOUT = 100; //MAGIC_NUMBER
//    ms.interface -> attachTo_thisCAContext(); /// base member function
//
//    std::map< std::string, bool  > startedSecondMovement;
//    for( auto && it : ms.dualMotorMoves_Scr )
//        startedSecondMovement[ it ] = false;
//
//    bool success = false;
//    bool timeOut = false;
//
//    auto itNam = ms.dualMotorMoves_Scr.begin();
//    auto itSta = ms.dualMotorMoves_Sta.begin();
//
//    // wait for element to get to a position
//    while(true)
//    {
//        // each loop set iterators back to begin
//        itNam = ms.dualMotorMoves_Scr.begin();
//        itSta = ms.dualMotorMoves_Sta.begin();
//
//        while(itNam != ms.dualMotorMoves_Scr.end() || itSta != ms.dualMotorMoves_Sta.end())
//        {   // check if started second movement
//            if( !startedSecondMovement.at(*itNam) )
//            {    // check if drives are out
//                ms.interface->debugMessage(*itNam, " not started second movement ");
//                if(ms.interface->isScreenOUT(*itNam,true) )
//                {
//                    // there is a slight delay while the driver enabled flag is set, so we'll pause
//                    std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); // MAGIC_NUMBER
//                    // move to new position
//                    ms.interface->debugMessage(*itNam," SCREEN_OUT, start second momement ");
//                    ms.interface->set_VELA_HV_MOVER_Position(*itNam,*itSta );
//                    // set started second movement to true
//                    startedSecondMovement.at(*itNam) = true;
//                    // i can release the lock, you can't move it until it has stopped "Trajectory in Progress"
//                    //ms.interface->allScreentData.at(*itNam).isLocked = false;
//                }
//            }
//            // check each started Second Movement to see if the yare all true
//            success = true;
//            for(auto it:startedSecondMovement)
//            {
//                if(!it.second)
//                    success = false;
//            }
//            if(success)
//                break;
//            else
//                ms.interface->debugMessage(" NOT ALL SCREENS STARTED SECOND MOVEMENT, TRY AGAIN");
//
//            if(time(0) - timeStart > TIMEOUT) // MAGIC_NUMBER
//            {
//                timeOut = true;
//                break;
//            }
//            // increment iterators
//            if(itNam != ms.dualMotorMoves_Scr.end() )
//                ++itNam;
//            if(itSta != ms.dualMotorMoves_Sta.end() )
//                ++itSta;
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000) ); // MAGIC_NUMBER
//        }
//    }
//    // we can release all locks now, success or failure
//    for(auto && it : ms.dualMotorMoves_Scr)
//        ms.interface->isLockedMap.at(it) = false;
//
//    if(timeOut)
//        ms.interface -> message("ERROR TIMEOUT WHILE WAITING FOR SCREENS TO MOVE");
//    ms.isComplete = true;
}
//______________________________________________________________________________
void screenInterface::killFinishedMoveThreads()
{
    //  do the loop this way because:
    // http://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it
    for (auto && it = screenDualMoveStructsMap.cbegin(); it != screenDualMoveStructsMap.cend() /* not hoisted */; /* no increment */)
    {
        if( it->second.isComplete  )
        {
            //message("killFinishedMoveThreads got here");
            it->second.thread->join();
            delete it -> second.thread;
            screenDualMoveStructsMap.erase( it++ );
        }
        else
        {
            ++it;
            //message("killFinishedMoveThreads actually got here");
        }
    }
    message("FIN killFinishedMoveThreads");
}
//______________________________________________________________________________
bool screenInterface::setPosition(const std::string & name, const screenStructs::DRIVER_DIRECTION dir, const double value )
{
    bool r = false;
    if( entryExists(allScreentData,name))
    {
        if( dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL && isHDriveEnabled( allScreentData.at(name).driver ) )
            r = set_VELA_HV_MOVER_Position(allScreentData.at(name).driver.hDriverSTA, value);
        else if( dir == screenStructs::DRIVER_DIRECTION::VERTICAL && isVDriveEnabled( allScreentData.at(name).driver ) )
            r = set_VELA_HV_MOVER_Position(allScreentData.at(name).driver.vDriverSTA, value);
    }
    return r;
}
//______________________________________________________________________________
bool screenInterface::set_VELA_HV_MOVER_Position(const std::string& name,  const screenStructs::SCREEN_STATE& sta )
{
    if( is_H_Element( sta) )
    {
        debugMessage("Moving H to ", ENUM_TO_STRING(sta) );
        return set_VELA_HV_MOVER_Position(name,screenStructs::DRIVER_DIRECTION::HORIZONTAL,sta);
    }
    else if( is_V_Element(sta) )
    {
        debugMessage("Moving V to ", ENUM_TO_STRING(sta) );
        return set_VELA_HV_MOVER_Position(name,screenStructs::DRIVER_DIRECTION::VERTICAL,  sta);
    }

}
//______________________________________________________________________________
bool screenInterface::set_VELA_HV_MOVER_Position(const std::string& name, const screenStructs::DRIVER_DIRECTION dir, const screenStructs::SCREEN_STATE& sta )
{
    bool r = false;
    double p;
    if( dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL)
    {
        p = getElementPosition(allScreentData.at(name).driver.hCassette, sta );
        debugMessage("Moving H to ", p );
        return set_VELA_HV_MOVER_Position(allScreentData.at(name).driver.hDriverSTA, p );
    }
    else if( dir == screenStructs::DRIVER_DIRECTION::VERTICAL  )
    {
        p = getElementPosition(allScreentData.at(name).driver.vCassette, sta );
        debugMessage("Moving V to ", p );
        return set_VELA_HV_MOVER_Position(allScreentData.at(name).driver.vDriverSTA, p );
    }
}
//______________________________________________________________________________
bool screenInterface::set_VELA_HV_MOVER_Position(const screenStructs::screenDriverStatus& driver, const double value)
{
    ca_put(driver.pvComStructs.at(screenStructs::SCREEN_PV_TYPE::MABS).CHTYPE,
           driver.pvComStructs.at(screenStructs::SCREEN_PV_TYPE::MABS).CHID, &value );
    std::string m1;
    m1 = "Timeout sending position in set_VELA_HV_MOVER_Position";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("set_VELA_HV_MOVER_Position recieved an unexpected error status from EPICS");
    return ret;
}
//______________________________________________________________________________
bool screenInterface::set_CLARA_HV_MOVER_Position(const std::string& name,  const screenStructs::SCREEN_STATE& sta )
{
    if( is_H_Element( sta ) )
    {
        debugMessage("Moving H to ", ENUM_TO_STRING(sta) );
        return set_CLARA_HV_MOVER_Position(name,screenStructs::DRIVER_DIRECTION::HORIZONTAL,sta);
    }
    else if( is_V_Element( sta ) )
    {
        debugMessage("Moving V to ", ENUM_TO_STRING(sta) );
        return set_CLARA_HV_MOVER_Position(name,screenStructs::DRIVER_DIRECTION::VERTICAL,  sta);
    }

}
//______________________________________________________________________________
bool screenInterface::set_CLARA_HV_MOVER_Position(const std::string& name, const screenStructs::DRIVER_DIRECTION dir, const screenStructs::SCREEN_STATE& sta )
{
    bool r = false;
    double p;
    if( dir == screenStructs::DRIVER_DIRECTION::HORIZONTAL)
    {
        p = getElementPos(allScreentData.at(name).driver.hCassette, sta );
        debugMessage("Moving H to ", p );
        return set_CLARA_HV_MOVER_Position(allScreentData.at(name).driver.hDriverSTA, p );
    }
    else if( dir == screenStructs::DRIVER_DIRECTION::VERTICAL  )
    {
        p = getElementPos(allScreentData.at(name).driver.vCassette, sta );
        debugMessage("Moving V to ", p );
        return set_CLARA_HV_MOVER_Position(allScreentData.at(name).driver.vDriverSTA, p );
    }
}
//______________________________________________________________________________
bool screenInterface::set_CLARA_HV_MOVER_Position(const screenStructs::screenDriverStatus& driver, const double value)
{
    for( auto && it : driver.pvComStructs )
    {
        message("ssssssss");
        message(ENUM_TO_STRING(it.first));
    }
    ca_put(driver.pvComStructs.at(screenStructs::SCREEN_PV_TYPE::POS).CHTYPE,
           driver.pvComStructs.at(screenStructs::SCREEN_PV_TYPE::POS).CHID, &value );
    std::string m1;
    debugMessage("we here now");
    m1 = "Timeout sending position in set_CLARA_HV_MOVER_Position";
    int status = sendToEpics("ca_put", "", m1.c_str() );
    bool ret = false;
    if (status == ECA_NORMAL)
    {
        ret = true;
    }
    else
        message("set_CLARA_HV_MOVER_Position recieved an unexpected error status from EPICS");
    return ret;
}
//______________________________________________________________________________
double screenInterface::getElementPosition(const screenStructs::screenCassette & scrcas, screenStructs::SCREEN_STATE e )
{
    return scrcas.cassetteElementsPosition.at(e);
}
//______________________________________________________________________________
double screenInterface::getElementPos(const screenStructs::screenCassette & scrcas, screenStructs::SCREEN_STATE e )
{
    return scrcas.cassetteElementsPos.at(e);
}
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
//_________________________________________________________________________________________________________________
bool screenInterface::yagOnV( const screenStructs::screenDriver & scrdr )
{
    return scrdr.vCassette.cassetteElements.at(screenStructs::SCREEN_STATE::V_YAG) == true;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVDriveEnabled(const screenStructs::screenDriver & scrdr )
{
    return !isVDriveDisabled(scrdr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isVDriveDisabled(const screenStructs::screenDriver & scrdr )
{
    return scrdr.vDriverSTA.state == screenStructs::DRIVER_STATE::DRIVER_DISABLED;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHDriveEnabled(const screenStructs::screenDriver & scrdr )
{
    return !isHDriveDisabled(scrdr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isHDriveDisabled(const screenStructs::screenDriver & scrdr )
{
    return scrdr.hDriverSTA.state == screenStructs::DRIVER_STATE::DRIVER_DISABLED;
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_H_Element(const screenStructs::SCREEN_STATE e )
{   // When moving a cassette element we need to know if it is an H or V element
    for( auto && it :screenStructs::hCassetteElementMap )
        if( e == it.second )
            return true;
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_V_Element(const  screenStructs::SCREEN_STATE e )
{   // When moving a cassette element we need to know if it is an H or V element
    for( auto && it :screenStructs::vCassetteElementMap )
        if( e == it.second )
            return true;
    return false;
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_OUT_AND_VDriveEnabled(const std::string & name)
{
    return allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_OUT
           &&
           isVDriveEnabled(allScreentData.at(name).driver);
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_H_element_AND_HDriveEnabled(const screenStructs::SCREEN_STATE e,const screenStructs::screenDriver& scrdr)
{
    return is_H_Element(e) && isHDriveEnabled(scrdr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_V_element_AND_VDriveEnabled(const screenStructs::SCREEN_STATE e,const screenStructs::screenDriver& scrdr)
{
    return is_V_Element(e) && isVDriveEnabled(scrdr);
}
//_________________________________________________________________________________________________________________
bool screenInterface::is_IN_OR_OUT(const screenStructs::SCREEN_STATE sta )
{
    return sta == screenStructs::SCREEN_STATE::SCREEN_OUT || sta == screenStructs::SCREEN_STATE::SCREEN_IN;
}
//_________________________________________________________________________________________________________________
bool screenInterface::isScreenOUT(const std::string & name, const bool weKnowEntryExists)
{
    if( entryExists2(name,weKnowEntryExists) )
        return allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_OUT;
    else
        return false;//do we need a TRUE FALSE or UNKNOWN ??
}
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
//_________________________________________________________________________________________________________________
bool screenInterface::screen_is_out_AND_sta_is_in(const std::string & name, const screenStructs::SCREEN_STATE sta )
{
    return sta == screenStructs::SCREEN_STATE::SCREEN_IN && isScreenOUT(name);
}
//_________________________________________________________________________________________________________________
bool screenInterface::screen_is_in_AND_sta_is_out(const std::string & name, const screenStructs::SCREEN_STATE sta )
{
    return sta == screenStructs::SCREEN_STATE::SCREEN_OUT && isScreenIN(name);
}
//_________________________________________________________________________________________________________________
double screenInterface::get_H_ACTPOS(const std::string & name)
{
    if(is_CLARA_HV_MOVER(name))
    {
        return allScreentData.at( name ).driver.hDriverSTA.position;
    }
}
//_________________________________________________________________________________________________________________
double screenInterface::get_V_ACTPOS(const std::string & name)
{
    if(is_CLARA_HV_MOVER(name) || is_CLARA_V_MOVER(name))
    {
        return allScreentData.at( name ).driver.vDriverSTA.position;
    }
}
//___________________________________________________________________________________________________________
void screenInterface::update_STA_Bit_map( screenStructs::monitorStruct * ms, const int argsdbr  )
{   // we're going to assume that each bit in the numebr is where the status is on / off
    // the first bit is "Trajectory in Porgress"
    // for other bits assume as in /home/controls/edl/EBT-YAG.edl
    //  "Trajectory in Progress"     //  "Historical Pos HW Limit"    //  "Historical Neg HW Limit"    //  "Index Report Avalable"
    //  "Wraparound Occurred"        //  "Excessive Position Error"   //  "Temperature Fault"          //  "Motor is Off"
    //  "Index Input"                //  "Pos HW Limit Asserted"      //  "Neg HW Limit Asserted"      //  "Maths Overflow"
    //  "Index Error"                //  "Syntax Error"               //  "Over Current"               //  "Programme Checksum Error"
    // probably more evil
    screenStructs::screenDriverStatus * obj = reinterpret_cast<screenStructs::screenDriverStatus *> (ms->obj);
    bool value = false;
    size_t k =0;
    obj->STA =argsdbr;
    for( auto && it : obj->STA_bit_label )// STA_Bit_order must contain the same entreis as STA_Bit_map
    {
        value = ( argsdbr  &(1<<k) ) >> k;
        //std::cout << value << " ";
        obj->STA_bit_map[ it ] = value;
        ++k;
    }
    //std::cout << std::endl;
    // update trajectory
    switch( obj->STA_bit_map.at( UTL::TRAJECTORY_IN_PROGRESS )  )
    {
        case 0:
            obj->trajectory = screenStructs::DRIVER_STATE::DRIVER_STATIONARY;
            // while the screen is moving there is no point updating the cassette position
            // screenStructs::SCREEN_STATE newscreenstate;
            if( isHorizontal( obj->dir )  )
                updateCassettePosition( allScreentData.at(obj->parentScreen).driver.hCassette,  obj->position );
            else if( isVertical(obj->dir) )
                updateCassettePosition( allScreentData.at(obj->parentScreen).driver.vCassette,  obj->position );
            break;
        case 1:
            obj->trajectory = screenStructs::DRIVER_STATE::DRIVER_MOVING;
            if( isHorizontal( obj->dir )  )
                allScreentData.at(obj->parentScreen).driver.hCassette.screenState = screenStructs::SCREEN_STATE::SCREEN_MOVING;
            else if( isVertical(obj->dir) )
                allScreentData.at(obj->parentScreen).driver.vCassette.screenState = screenStructs::SCREEN_STATE::SCREEN_MOVING;
            allScreentData.at(obj->parentScreen).screenState = screenStructs::SCREEN_STATE::SCREEN_MOVING;
            break;
        default:
            obj->trajectory = screenStructs::DRIVER_STATE::DRIVER_ERROR;
    }
    /// Print a debug message
//    std::stringstream ss;
//    ss << obj->parentScreen;
//    if( isHorizontal( obj->dir) )
//    {
//        ss << " Horizontal ";
//        debugMessage( obj->parentScreen, " H:STA ", obj->STA);
//    }
//    else if( isVertical( obj->dir)  )
//    {
//        debugMessage( obj->parentScreen, " V:STA ", obj->STA);
//        ss << " Vertical ";
//    }
//    else
//        ss << " ERROR";
//    ss << ENUM_TO_STRING( obj->trajectory);
//    debugMessage(ss);
}
//_________________________________________________________________________________________________________________
bool screenInterface::isMoving(const std::string & name, const bool weKnowEntryExists)
{
    bool r = false;
    if( entryExists2(name,weKnowEntryExists ) )
    {// isMoving refers to the driver that moves the screen in and out, so we switch on the screen(drive) type
        switch( allScreentData[name].screenType )
        {
        case screenStructs::SCREEN_TYPE::VELA_PNEUMATIC:
            if( allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_MOVING )
                r = true;
            break;
        case screenStructs::SCREEN_TYPE::CLARA_PNEUMATIC:
            if( allScreentData.at(name).screenState == screenStructs::SCREEN_STATE::SCREEN_MOVING )
                r = true;
            break;
        case screenStructs::SCREEN_TYPE::VELA_HV_MOVER:
            if( allScreentData.at(name).driver.hDriverSTA.trajectory == screenStructs::DRIVER_STATE::DRIVER_MOVING )
                r = true;
            if( allScreentData.at(name).driver.vDriverSTA.trajectory == screenStructs::DRIVER_STATE::DRIVER_MOVING )
                r = true;
            break;
        case screenStructs::SCREEN_TYPE::CLARA_HV_MOVER:
            if( allScreentData.at(name).driver.hDriverSTA.trajectory == screenStructs::DRIVER_STATE::DRIVER_MOVING )
                r = true;
            if( allScreentData.at(name).driver.vDriverSTA.trajectory == screenStructs::DRIVER_STATE::DRIVER_MOVING )
                r = true;
            break;
        case screenStructs::SCREEN_TYPE::CLARA_V_MOVER:
            if( allScreentData.at(name).driver.hDriverSTA.trajectory == screenStructs::DRIVER_STATE::DRIVER_MOVING )
                r = true;
            if( allScreentData.at(name).driver.vDriverSTA.trajectory == screenStructs::DRIVER_STATE::DRIVER_MOVING )
                r = true;
            break;
        }
    }
    return r;
}
//___________________________________________________________________________________________________________________
bool screenInterface::isNotMoving(const std::string & name, const bool weKnowEntryExists)
{
    return !isMoving(name);
}
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
