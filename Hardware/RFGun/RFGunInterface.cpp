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

#include "RFGunInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <time.h>



//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
RFGunInterface::RFGunInterface( const std::string configFileLocation1,
                                const std::string configFileLocation2,
                                const std::string configFileLocation3, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr )
: configReader( configFileLocation1, configFileLocation2, configFileLocation3, show_messages_ptr, show_debug_messages_ptr ), interface( show_messages_ptr, show_debug_messages_ptr )
, SET_MOD_OFF( RFGunStructs::MOD_SET_STATE::SET_MOD_OFF ),SET_MOD_STANDBY ( RFGunStructs::MOD_SET_STATE::SET_MOD_STANDBY),
SET_MOD_HV_ON (   RFGunStructs::MOD_SET_STATE::SET_MOD_HV_ON), SET_MOD_TRIG ( RFGunStructs::MOD_SET_STATE::SET_MOD_TRIG)
{
    initialise();
}
//______________________________________________________________________________
RFGunInterface::RFGunInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
: configReader( show_messages_ptr, show_debug_messages_ptr  ), interface( show_messages_ptr, show_debug_messages_ptr  )
, SET_MOD_OFF( RFGunStructs::MOD_SET_STATE::SET_MOD_OFF ),SET_MOD_STANDBY ( RFGunStructs::MOD_SET_STATE::SET_MOD_STANDBY),
SET_MOD_HV_ON (   RFGunStructs::MOD_SET_STATE::SET_MOD_HV_ON), SET_MOD_TRIG ( RFGunStructs::MOD_SET_STATE::SET_MOD_TRIG)
{
    initialise();
}
//______________________________________________________________________________
RFGunInterface::~RFGunInterface()
{
    for( auto it : continuousMonitorStructs )
    {
        debugMessage("delete RFGunInterface continuousMonitorStructs entry.");
        delete it;
    }
    deleteTraceMonitorStructs();
}
//______________________________________________________________________________
void RFGunInterface::deleteTraceMonitorStructs()
{
    for( auto it : traceMonitorStructs )
    {
        debugMessage("delete RFGunInterface traceMonitorStructs entry.");
        delete it;
    }
}
//______________________________________________________________________________
void RFGunInterface::initialise()
{
    /// The config file reader
    message("Attempting to parse RF config files");

    configFileRead = configReader.readConfigFiles();

    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        initObjects();
        /// subscribe to the channel ids
        initChids();
        /// start the monitors: set up the callback functions
        if( allChidsInitialised )
            startMonitors();

        /// The pause allows EPICS to catch up.
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBER
    }
    else
        message( "ERROR: in initialise: configFileRead = false."  );
}
//______________________________________________________________________________
void RFGunInterface::initObjects()
{
    RFObject = configReader.getRFObject();
}
//______________________________________________________________________________
void RFGunInterface::initChids()
{
    message( "\n", "Searching for RF Gun chids..." );
    // power objects
    for( auto && it1 : RFObject.powerObjects )
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );
    // modulator
    for( auto && it : RFObject.mod.pvMonStructs    )
        addChannel( RFObject.mod.pvRoot, it.second );
    for( auto && it : RFObject.mod.pvComStructs )
        addChannel( RFObject.mod.pvRoot, it.second );
    // llrf
    for( auto && it : RFObject.llrf.pvMonStructs )
        addChannel( RFObject.llrf.pvRoot, it.second );
    // send
    int status = sendToEpics( "ca_create_channel", "Found RF chids.", "!!TIMEOUT!! Not all RF ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBERR
        for( auto && it1 : RFObject.powerObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        for( auto && it : RFObject.mod.pvMonStructs )
            checkCHIDState(  it.second.CHID,  it.second.pvSuffix );
        for( auto && it : RFObject.mod.pvComStructs )
                checkCHIDState(  it.second.CHID,  it.second.pvSuffix );
        for( auto && it : RFObject.llrf.pvMonStructs )
            checkCHIDState(  it.second.CHID,  it.second.pvSuffix );
        message("");
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void RFGunInterface::addChannel( const std::string & pvRoot, RFGunStructs::pvStruct & pv )
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void RFGunInterface::startMonitors()
{
    continuousMonitorStructs.clear();

    for( auto && it1 : RFObject.powerObjects )
        for( auto && it2 : it1.second.pvMonStructs )
            /// TRACE PVs are only monitored if you ask...
            if( isNotATracePV( it2.first ) )
                addToMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );

    for( auto && it : RFObject.mod.pvMonStructs )
        addToMonitorStructs( continuousMonitorStructs, it.second );

    for( auto && it : RFObject.llrf.pvMonStructs )
        addToMonitorStructs( continuousMonitorStructs, it.second );

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to RF Monitors", "!!TIMEOUT!! Subscription to RF Monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
void RFGunInterface::addToMonitorStructs( std::vector< RFGunStructs::monitorStruct * > & msv, RFGunStructs::pvStruct & pv,  RFGunStructs::rfPowerObject * pwrObj   )
{
    msv.push_back( new RFGunStructs::monitorStruct() );
    msv.back() -> monType   = pv.pvType;
    msv.back() -> RFObject  = &RFObject;
    msv.back() -> interface = this;
    msv.back() -> CHTYPE    = pv.CHTYPE;

    switch( pv.pvType )
    {
        case RFGunStructs::RF_PV_TYPE::RF_PWR_REV:
            msv.back() -> val = (void*) &pwrObj -> rev ;
            msv.back() -> objName = pwrObj -> name;

            break;
        case RFGunStructs::RF_PV_TYPE::RF_PWR_FWD:
            msv.back() -> val = (void*) &pwrObj -> fwd ;
            msv.back() -> objName = pwrObj -> name;
            break;
        case RFGunStructs::RF_PV_TYPE::RF_PWR_REVT:
            // For the traces we can just pass in the powerObject for value, and figure everything out from them in the callback functions
            msv.back() -> val = (void*)&pwrObj -> revT;
            msv.back() -> objName = pwrObj -> name + " REVT";

            break;
        case RFGunStructs::RF_PV_TYPE::RF_PWR_FWDT:
            msv.back() -> val = (void*)&pwrObj -> fwdT;
            msv.back() -> objName = pwrObj -> name + " FWDT";
            break;
        case RFGunStructs::RF_PV_TYPE::RF_PWR_REVTM:
            msv.back() -> val = (void*) &pwrObj -> revM;
            msv.back() -> objName = pwrObj -> name;
            break;

        case RFGunStructs::RF_PV_TYPE::RF_PWR_FWDTM:
            msv.back() -> val = (void*) &pwrObj -> fwdM;
            msv.back() -> objName = pwrObj -> name;
            break;

        case RFGunStructs::RF_PV_TYPE::RF_PWR_RATIO:
            msv.back() -> val = (void*) &pwrObj -> ratio;
            msv.back() -> objName = pwrObj -> name;
            break;

        case RFGunStructs::RF_PV_TYPE::RF_AMP_READ:
            msv.back() -> val = (void*) &RFObject.llrf.ampR;
            msv.back() -> objName = RFObject.llrf.name;
            break;
        case RFGunStructs::RF_PV_TYPE::RF_AMP_WRITE:
            msv.back() -> val = (void*) &RFObject.llrf.ampW;
            msv.back() -> objName = RFObject.llrf.name;
            break;
        case RFGunStructs::RF_PV_TYPE::RF_PHI:
            msv.back() -> val = (void*) &RFObject.llrf.phi;
            msv.back() -> objName = RFObject.llrf.name;
            break;
        case RFGunStructs::RF_PV_TYPE::MOD_STATE_SET:
//            msv.back() -> val = (void*) &RFObject.llrf.phi;
//            msv.back() -> objName = RFObject.llrf.name;
            break;
        case RFGunStructs::RF_PV_TYPE::MOD_STATE_READ:
            msv.back() -> val = (void*) &RFObject.mod.state;
            msv.back() -> objName = RFObject.mod.name;
            break;
        case RFGunStructs::RF_PV_TYPE::MOD_ERROR_READ:
            msv.back() -> val = (void*) &RFObject.mod.ilck;
            msv.back() -> objName = RFObject.mod.name;
            break;
        case RFGunStructs::RF_PV_TYPE::MOD_WARMUP_TIME:
            msv.back() -> val = (void*) &RFObject.mod.warmuptime;
            msv.back() -> objName = RFObject.mod.name;
            break;

        case RFGunStructs::RF_PV_TYPE::MOD_ERROR_READ_STR:

            msv.back() -> val = (void*) &RFObject.mod.ilckstr;
            msv.back() -> objName = RFObject.mod.name;
            break;



//        case RFGunStructs::RF_PV_TYPE::MOD_RESET: /// is a PV_Command so not used here...
//            break;
        default:
            message("ERROR: in addToMonitorStructs: ", msv.back() -> objName," PV Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  RFGunInterface::staticEntryMonitor, (void*)msv.back(), &(msv.back()->EVID) );
}
//______________________________________________________________________________
void RFGunInterface::staticEntryMonitor( const event_handler_args args )
{
    RFGunStructs::monitorStruct * ms = static_cast< RFGunStructs::monitorStruct *> ( args.usr );

    switch( ms -> monType )
    {
        case RFGunStructs::RF_PV_TYPE::RF_PWR_REVT:

            ms->interface->updateTrace( ms, args );
            break;

        case RFGunStructs::RF_PV_TYPE::RF_PWR_FWDT:

            ms->interface->updateTrace( ms, args );
            break;

        case RFGunStructs::RF_PV_TYPE::MOD_STATE_SET:

            break;

        case RFGunStructs::RF_PV_TYPE::MOD_STATE_READ:

            ms->interface->setStateRead( args.dbr );
            break;

        case RFGunStructs::RF_PV_TYPE::MOD_RESET:
            //ms->interface->debugMessage(ENUM_TO_STRING(RFGunStructs::RF_PV_TYPE::MOD_RESET )," is a PV_COMMAND. You shouldn't see this... ");
            break;

        case RFGunStructs::RF_PV_TYPE::MOD_ERROR_READ:
            ms->interface->updateModIlock( ms, args.dbr );
            break;

        case RFGunStructs::RF_PV_TYPE::RF_PWR_REVTM:
            ms->interface->updateValue( ms, args );
            break;

        case RFGunStructs::RF_PV_TYPE::RF_PWR_FWDTM:
            ms->interface->updateValue( ms, args );
            break;

        case RFGunStructs::RF_PV_TYPE::MOD_ERROR_READ_STR:

            ms->interface->message( "MOD_ERROR_READ_STR" );

            ms->interface->updateModIlock( ms, args.dbr );
            break;

        case RFGunStructs::RF_PV_TYPE::MOD_WARMUP_TIME:
            ms->interface->updateValue( ms, args );

            ms->interface->message(ms->RFObject->name," ",ms->objName," ",ENUM_TO_STRING( ms -> monType )," = ",*(long*)ms->val);

            if( *(long*)ms -> val == 0 )
            {
                ms->interface->RFObject.mod.safelyWarmedUP = true;
                ms->interface->message( "Gun Modulator Safely Warmed Up:");
            }
            else
            {
                ms->interface->RFObject.mod.safelyWarmedUP = false;
            }
            break;
        default:
            ms->interface->updateValue( ms, args );
    }
}
//______________________________________________________________________________
void RFGunInterface::updateModIlock( RFGunStructs::monitorStruct * ms, const void * argsdbr )
{
    std::string s =  ms->RFObject->name + " " + ms->objName + " " + ENUM_TO_STRING( ms -> monType) + " = ";
    switch( ms -> CHTYPE )
    {

        case DBR_STRING:
            ms->RFObject->mod.ilckstr = convertModErrorReadStr( (char *)argsdbr );
            ms->interface->message( s , (char *)argsdbr," = ",ENUM_TO_STRING(ms->RFObject->mod.ilckstr), " --- NB This is defined rf_gun_modulator.config" );
            break;

        case DBR_DOUBLE:
            ms->RFObject->mod.ilck = convertModErrorRead( *(double*)argsdbr );
            ms->interface->message( s , *(double*)argsdbr," = ",ENUM_TO_STRING(ms->RFObject->mod.ilck), " --- NB THESE Numbers have not been well checked, use with caution !!!" );
            break;

        default:
            message("ERROR: in updateModIlock: unknown CHTYPE");
    }
}
//______________________________________________________________________________
RFGunStructs::MOD_EXILOCK1 RFGunInterface::convertModErrorReadStr( const char * epicsModErrCodeString )
{
    std::stringstream ss;
    ss << epicsModErrCodeString;

    if( isAGoodModErrorReadStr( ss.str() ) )
        return RFGunStructs::MOD_EXILOCK1::GOOD;
    else if( isABadModErrorReadStr( ss.str() ) )
        return RFGunStructs::MOD_EXILOCK1::BAD;
    else
        return RFGunStructs::MOD_EXILOCK1::UNKNOWN;
}
//______________________________________________________________________________
bool RFGunInterface::isAGoodModErrorReadStr( const std::string & s )
{
//    for( auto && it : RFObject.mod.goodModErrorReadStr )
//        message( "RFObject.mod.goodModErrorReadStr = ", it);
//
//    message("Compare with ", s );
//
//    message(  s == RFObject.mod.goodModErrorReadStr[0] ) ;

    if (std::find( RFObject.mod.goodModErrorReadStr.begin(), RFObject.mod.goodModErrorReadStr.end(), s ) != RFObject.mod.goodModErrorReadStr.end())
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::isABadModErrorReadStr( const std::string & s )
{
    if (std::find( RFObject.mod.badModErrorReadStr.begin(), RFObject.mod.badModErrorReadStr.end(), s ) != RFObject.mod.badModErrorReadStr.end())
        return true;
    else
        return false;
}
//______________________________________________________________________________
RFGunStructs::MOD_EXILOCK1 RFGunInterface::convertModErrorRead( const double v )
{
    switch( (int)v )
    {
        case 0:
            return RFGunStructs::MOD_EXILOCK1::GOOD;
            break;
        case 2000:
            return RFGunStructs::MOD_EXILOCK1::GOOD;
            break;
        default:
            return RFGunStructs::MOD_EXILOCK1::BAD;
            break;
    }
}
//______________________________________________________________________________
bool RFGunInterface::isModILockStateGood()
{
    /// OK WE'RE using the STRING ilock, EBT-INJ-RF-MOD-01:Sys:ErrorRead.SVAL
    /// the number one is dodge. Good and bad errro strigns can be defined in the config file

    if( RFObject.mod.ilckstr == RFGunStructs::MOD_EXILOCK1::GOOD )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::isModILockStateNotGood()
{
    return !isModILockStateGood();
}
//______________________________________________________________________________
void RFGunInterface::updateValue( RFGunStructs::monitorStruct * ms, const event_handler_args & args )
{
    switch( args.type )
    {
         case DBR_DOUBLE:
            *(double*)ms -> val = *(double*)args.dbr;
            //ms->interface->message(ms->RFObject->name," ",ms->objName," ",ENUM_TO_STRING( ms -> monType), " = ", *(double*)ms -> val );
            break;
        case DBR_LONG:
            *(long*)ms -> val = *(long*)args.dbr;
            //ms->interface->message(ms->RFObject->name," ",ms->objName," ",ENUM_TO_STRING( ms -> monType )," = ",*(long*)ms->val);
            break;
        default:
            ms->interface->debugMessage("ERROR: in updateValue: received unexpected chtype from ", ms->RFObject->name, " ", ms->objName,
                                        " ", ENUM_TO_STRING( ms -> monType), "  ", (int)args.type );
    }
}
//______________________________________________________________________________
void RFGunInterface::updateTrace( RFGunStructs::monitorStruct * ms, const event_handler_args args )
{
    /// this could be better, with the type passed from the config
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;

    RFGunStructs::rfTraceData * td = reinterpret_cast< RFGunStructs::rfTraceData *> (ms -> val);

    updateTime( p->stamp, td->   timeStamps[ td->shotCount ],
                          td->strTimeStamps[ td->shotCount ]  );

    const dbr_double_t * value = &(p  -> value);

    size_t i =0;
    for( auto && it : td->traceData[ td->shotCount ] )
    {
        it = *( &p->value + i);
//
//        if( i == 0 )
//            std::cout << ms -> objName << " DATA = " ;
//        if( i < 10 )
//            std::cout << it << ", ";
//
//        if( i == 9 )
//            std::cout << std::endl;
        ++i;
    }
    /// increment shot count
    if( td -> numShots > -1 )
        ++td -> shotCount;

    /// kill callback ?
    if( td->shotCount == td->numShots )
    {
        ms->interface->killCallBack( ms );
    }
}
//______________________________________________________________________________
void RFGunInterface::killCallBack( RFGunStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
        debugMessage( ms->objName, " monitoring = false ");

        isMonitoringMap[ ms->objName ] = false;

        delete ms;

    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->objName );
    }
}
//______________________________________________________________________________
bool RFGunInterface::killTraceMonitors()
{
    for( auto && it : traceMonitorStructs )
        killCallBack( it );
    return isMonitoringTraces();
}
//______________________________________________________________________________
void RFGunInterface::setStateRead( const void * argsdbr )
{
    switch( *(unsigned short*)argsdbr )
    {
        case 1:
            RFObject.mod.state = RFGunStructs::MOD_STATE::ERROR1;
            break;
        case 2:
            RFObject.mod.state = RFGunStructs::MOD_STATE::OFF;
            break;
        case 3:
            RFObject.mod.state = RFGunStructs::MOD_STATE::off_Request;
            break;
        case 4:
            RFObject.mod.state = RFGunStructs::MOD_STATE::HV_Intrlock;
            break;
        case 5:
            RFObject.mod.state = RFGunStructs::MOD_STATE::Standby_Request;
            break;
        case 6:
            RFObject.mod.state = RFGunStructs::MOD_STATE::Standby;
            break;
        case 7:
            RFObject.mod.state = RFGunStructs::MOD_STATE::HV_Off_Requ;
            break;
        case 8:
            RFObject.mod.state = RFGunStructs::MOD_STATE::Trigger_Interl;
            break;
        case 9:
            RFObject.mod.state = RFGunStructs::MOD_STATE::HV_Request;
            break;
        case 10:
            RFObject.mod.state = RFGunStructs::MOD_STATE::HV_On;
            break;
        case 11:
            RFObject.mod.state = RFGunStructs::MOD_STATE::Trig_Off_Req;
            break;
        case 12:
            RFObject.mod.state = RFGunStructs::MOD_STATE::Trig_Request;
            break;
        case 13:
            RFObject.mod.state = RFGunStructs::MOD_STATE::Trig;
            break;
        default:
            RFObject.mod.state = RFGunStructs::MOD_STATE::ERROR1;
            break;
    }
    message( RFObject.name," ",RFObject.mod.name," state changed to ",ENUM_TO_STRING(RFObject.mod.state) );
}
//______________________________________________________________________________
void RFGunInterface::monitorTracesForNShots( size_t N )
{
    if( isNotMonitoringTraces() )
    {
        traceMonitorStructs.clear();
        debugMessage( "monitorTracesForNShots: Starting RF Traces Monitor " );
        resetTraceVectors( N );
        debugMessage( "monitorTracesForNShots: Vectors Reset" );

        for( auto && it1 : RFObject.powerObjects )
            for( auto && it2 : it1.second.pvMonStructs )
                if( isATracePV( it2.first ) )
                   addToMonitorStructs( traceMonitorStructs, it2.second, &it1.second  );

        int status = sendToEpics( "ca_create_subscription", "", "ERROR: in  monitorTracesForNShots: !!TIMEOUT!! in subscription to RF Power Traces." );
            if ( status == ECA_NORMAL )
                for( auto && it : traceMonitorStructs )
                    isMonitoringMap[ it -> objName ] = true;
    }
    else
        message( "ERROR: in monitorTracesForNShots: Already Monitoring Taces" );
}
//______________________________________________________________________________
void RFGunInterface::resetTraceVectors( int N )
{
    size_t numShots = 0;
    if( N < 0 )
        numShots = 1;
    else
        numShots = N;

    traceMonitorStructs.clear();
    for( auto && it : RFObject.powerObjects)
    {   // Clear everything

        it.second.revT.shotCount = 0;
        it.second.revT.numShots  = N;
        it.second.fwdT.shotCount = 0;
        it.second.fwdT.numShots  = N;


        it.second.revT.timeStamps.clear();
        it.second.revT.strTimeStamps.clear();
        it.second.revT.traceData.clear();

        it.second.revT.timeStamps.resize( N );
        it.second.revT.strTimeStamps.resize( N );
        it.second.revT.traceData.resize( N );
        for( auto && it2 : it.second.revT.traceData )
            it2.resize( it.second.pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PWR_REVT ].COUNT );

        it.second.fwdT.timeStamps.clear();
        it.second.fwdT.strTimeStamps.clear();
        it.second.fwdT.traceData.clear();
        it.second.fwdT.timeStamps.resize( N );
        it.second.fwdT.strTimeStamps.resize( N );
        it.second.fwdT.traceData.resize( N );
        for( auto && it2 : it.second.fwdT.traceData )
            it2.resize( it.second.pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PWR_FWDT ].COUNT );
    }
}
//______________________________________________________________________________
bool RFGunInterface::setAmp( const long val )
{
    int status = caput( RFObject.llrf.pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_AMP_WRITE ].CHTYPE,
           RFObject.llrf.pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_AMP_WRITE ].CHID,
           val, "" , "!!RFGunInterface TIMEOUT!! In setRFAmp() ");
    if ( status == ECA_NORMAL )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::setPhi( const double val)
{
    int status = caput( RFObject.llrf.pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PHI ].CHTYPE,
                        RFObject.llrf.pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PHI ].CHID,
                        val, "" , "!!RFGunInterface TIMEOUT!! In setRFPhi() ");
    if ( status == ECA_NORMAL )
        return true;
    else
        return false;
}
//______________________________________________________________________________
double RFGunInterface::getFwd( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].fwd;
    else
        return DBL_ERR_NUM;
}
//______________________________________________________________________________
double RFGunInterface::getRev( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].rev;
    else
        return DBL_ERR_NUM;
}
//______________________________________________________________________________
double RFGunInterface::getRatio( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].ratio;
    else
        return DBL_ERR_NUM;
}
//______________________________________________________________________________
double RFGunInterface::getFwdCursor( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].fwdM;
    else
        return DBL_ERR_NUM;
}
//______________________________________________________________________________
double RFGunInterface::getRevCursor( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].revM;
    else
        return DBL_ERR_NUM;
}
//______________________________________________________________________________
bool RFGunInterface::setFwdCursor( const std::string & name,const  double val  )
{
    int status = -1;
    if( entryExists( RFObject.powerObjects, name ) )
    {
        status = caput( RFObject.powerObjects[name].pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PWR_FWDTM ].CHTYPE,
                        RFObject.powerObjects[name].pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PWR_FWDTM ].CHID,
                        val, "" , "!!RFGunInterface TIMEOUT!! In setFwdCursor() ");
    }
    if ( status == ECA_NORMAL )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::setRevCursor(const  std::string & name, const double val  )
{
    int status = -1;
    if( entryExists( RFObject.powerObjects, name ) )
    {
        status = caput( RFObject.powerObjects[name].pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PWR_REVTM ].CHTYPE,
                        RFObject.powerObjects[name].pvMonStructs[ RFGunStructs::RF_PV_TYPE::RF_PWR_REVTM ].CHID,
                        val, "" , "!!RFGunInterface TIMEOUT!! In setRevCursor() " );
    }
    if( status == ECA_NORMAL )
        return true;
    else
        return false;
}

bool RFGunInterface::setAllCursors(const double val  )
{
    bool r = true;
    bool t = true;
    for( auto && it : RFObject.powerObjects )
    {
        t = setRevCursor( it.first, val );
        if( !t )
            r = false;
        t = setFwdCursor( it.first, val );
        if( !t )
            r = false;

    }
    return r;
}
//______________________________________________________________________________
double RFGunInterface::getPhi( )
{
    return RFObject.llrf.phi;
}
//______________________________________________________________________________
double RFGunInterface::getAmpR()
{
    return RFObject.llrf.ampR;
}
//______________________________________________________________________________
long RFGunInterface::getAmpW()
{
    return RFObject.llrf.ampW;
}
//______________________________________________________________________________
std::vector< std::string > RFGunInterface::getRFPowerNames()
{
    std::vector< std::string > r;
    for( auto && it : RFObject.powerObjects )
        r.push_back( it.first );
    return r;
}


//______________________________________________________________________________
bool RFGunInterface::isModWarmedUp()
{
    return RFObject.mod.safelyWarmedUP;
}
//______________________________________________________________________________
bool RFGunInterface::isModNotWarmedUp()
{
    return !isModWarmedUp();
}
//______________________________________________________________________________
bool RFGunInterface::isModInTrig()
{
    if( getModState() == RFGunStructs::MOD_STATE::Trig )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::isModInHVOn()
{
    if( getModState() == RFGunStructs::MOD_STATE::HV_On )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::isModInStandby()
{
    if( getModState() == RFGunStructs::MOD_STATE::Standby )
        return true;
    else
        return false;
}
//______________________________________________________________________________
//bool RFGunInterface::isModIniLock()
//{
//    if( getModState() == MOD_EXILOCK1,  ( BAD ) ( GOODRFGunStructs::MOD_STATE::HV_Intrlock )
//        return true;
//    else
//        return false;
//}
//______________________________________________________________________________
//bool RFGunInterface::isModNotIniLock()
//{
//    return !isModIniLock();
//}
////______________________________________________________________________________
bool RFGunInterface::isModInOff()
{
    if( getModState() == RFGunStructs::MOD_STATE::OFF )
        return true;
    else
        return false;
}
//______________________________________________________________________________
void RFGunInterface::modReset()
{
    caput( RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_RESET ].CHTYPE,
           RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_RESET ].CHID,
           EPICS_RESET, "" , "!!RFGunInterface TIMEOUT!! In modReset() ");
}
//______________________________________________________________________________
bool RFGunInterface::modResetAndWait( const size_t waitTime )
{
    message("modreset");
    modReset();
    return waitFor( &RFGunInterface::isModILockStateGood, *this, "Timeout waiting for Modulator to reset ",  waitTime ); // MAGIC_NUMBER
}
//______________________________________________________________________________
RFGunStructs::MOD_STATE RFGunInterface::getModState()
{
    return RFObject.mod.state;
}
//______________________________________________________________________________
RFGunStructs::MOD_EXILOCK1 RFGunInterface::getModiLock()
{
    return RFObject.mod.ilck;
}
//______________________________________________________________________________
bool RFGunInterface::isATracePV( RFGunStructs::RF_PV_TYPE pv )
{
    bool ret = false;
    if( pv == RFGunStructs::RF_PV_TYPE::RF_PWR_REVT )
        ret = true;
    else if( pv == RFGunStructs::RF_PV_TYPE::RF_PWR_FWDT )
        ret = true;
    return ret;
}
//______________________________________________________________________________
bool RFGunInterface::isNotATracePV( RFGunStructs::RF_PV_TYPE pv )
{
    return !isATracePV(pv);
}
//______________________________________________________________________________
bool RFGunInterface::isMonitoringTraces()
{
    size_t c = 0;
    for( auto && it : isMonitoringMap )
        if( it.second )
            ++c;

    if( c == isMonitoringMap.size() && c > 0 )
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool RFGunInterface::isNotMonitoringTraces()
{
    return !isMonitoringTraces();
}
/// These are for python - if using c++ we can get pointers if we like...
//______________________________________________________________________________
std::vector< std::vector< double >> RFGunInterface::getRevT( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].revT.traceData;
    else
    {
        message("ERROR: in getRevT: ", name, " does not exist.");
        std::vector< std::vector< double >> t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< std::vector< double >> RFGunInterface::getFwdT(const std::string & name  )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].fwdT.traceData;
    else
    {
        message("ERROR: in getFwdT: ", name, " does not exist.");
        std::vector< std::vector< double >> t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< double > RFGunInterface::getRevTStamp(const std::string & name  )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].revT.timeStamps;
    else
    {
         message("ERROR: in getRevTStamp: ", name, " does not exist.");
        std::vector< double > t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< double > RFGunInterface::getFwdTStamp(const std::string & name  )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].fwdT.timeStamps;
    else
    {
         message("ERROR: in getFwdTStamp: ", name, " does not exist.");
        std::vector< double > t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< std::string > RFGunInterface::getRevTStampStr( const std::string & name )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].fwdT.strTimeStamps;
    else
    {
        message("ERROR: in getRevTStampStr: ", name, " does not exist.");
        std::vector< std::string > t;
        return t;
    }
}
//______________________________________________________________________________
std::vector< std::string > RFGunInterface::getFwdTStampStr( const std::string & name  )
{
    if( entryExists( RFObject.powerObjects, name ) )
        return RFObject.powerObjects[ name ].fwdT.strTimeStamps;
    else
    {
         message("ERROR: in getFwdTStampStr: ", name, " does not exist.");
        std::vector< std::string > t;
        return t;
    }
}
//______________________________________________________________________________
void RFGunInterface::setModState( RFGunStructs::MOD_SET_STATE state, const char * mess )
{
    caput( RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHTYPE,
           RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHID,
           state,  "" , mess );
}

/// These are general utility functions, they should only be called from high level functions if you know what yo uare doing...
/// e.g. they DO NOT check the interlock...
//______________________________________________________________________________
void RFGunInterface::setModToOff()
{
    if( isModInOff() )
        message( "setModToOff - BUT THE MOD STATE IS OFF");
    else
        setModState( RFGunStructs::MOD_SET_STATE::SET_MOD_OFF, "!!RFGunInterface TIMEOUT!! In setModToOff() " );
}
//______________________________________________________________________________
void RFGunInterface::setModToStandby()
{
    if( isModInStandby() )
        message( "setModToStandby - BUT THE MOD STATE IS Standby");
    else
        setModState( RFGunStructs::MOD_SET_STATE::SET_MOD_STANDBY, "!!RFGunInterface TIMEOUT!! In setModToStandby() " );
}
//______________________________________________________________________________
void RFGunInterface::setModToHVOn()
{
    if( isModInHVOn() )
        message( "setModToTrig - BUT THE MOD STATE IS HV On");
    else
    {
        message( "setModToHVOn()");

        setModState( RFGunStructs::MOD_SET_STATE::SET_MOD_HV_ON, "!!RFGunInterface TIMEOUT!! In setModToHVOn() " );
    }
}
//______________________________________________________________________________
void RFGunInterface::setModToTrig()
{
    if( isModInTrig() )
        message( "setModToTrig - BUT THE MOD STATE IS TRIG");
    else
        setModState( RFGunStructs::MOD_SET_STATE::SET_MOD_TRIG, "!!RFGunInterface TIMEOUT!! In setModToTrig() " );
}
//______________________________________________________________________________
bool RFGunInterface::setModStateAndWait( RFGunStructs::MOD_SET_STATE newState, time_t waitTime )
{
    bool success = false;
    switch( newState )
    {
        case RFGunStructs::MOD_SET_STATE::SET_MOD_OFF:
            setModToOff();
            //success = waitForState( RFGunStructs::MOD_STATE::OFF, waitTime );

            success = waitFor( &RFGunInterface::isModInOff, *this, "Timeout waiting for Modulator to set state off",  waitTime);

            break;

        case RFGunStructs::MOD_SET_STATE::SET_MOD_STANDBY:
            setModToStandby();

            success = waitFor( &RFGunInterface::isModInStandby, *this, "Timeout waiting for Modulator to set state standby",  waitTime);

            //success = waitForState( RFGunStructs::MOD_STATE::Standby, waitTime );
            break;

        case RFGunStructs::MOD_SET_STATE::SET_MOD_HV_ON:
            setModToHVOn();
            //success = waitForState( RFGunStructs::MOD_STATE::HV_On, waitTime );

            success = waitFor( &RFGunInterface::isModInHVOn, *this, "Timeout waiting for Modulator to set state HV On",  waitTime);
            break;

        case RFGunStructs::MOD_SET_STATE::SET_MOD_TRIG:
            setModToTrig();
            //success = waitForState( RFGunStructs::MOD_STATE::Trig, waitTime );

            success = waitFor( &RFGunInterface::isModInTrig, *this, "Timeout waiting for Modulator to set state Trig",  waitTime);

            break;
    }
    return success;
}
//______________________________________________________________________________
bool RFGunInterface::setOffToStandby( time_t waitTime )
{
    return setModStateAndWait( RFGunStructs::MOD_SET_STATE::SET_MOD_STANDBY, waitTime );
}
//______________________________________________________________________________
bool RFGunInterface::setStandbyToHVOn( time_t waitTime )
{
    time_t startTime = time( 0 );
    bool success = waitFor( &RFGunInterface::isModWarmedUp, *this, "WARNING: in setStandByToHVOn: Time-out while waiting for modulator to warmup",waitTime, 2000 );

    bool ret = false;

    if( success )
    {
        message( "setStandbyToHVOn success = true" );
        time_t timeremaining = waitTime - (  time( 0 ) - startTime );
        ret = setModStateAndWait( RFGunStructs::MOD_SET_STATE::SET_MOD_HV_ON, timeremaining );
    }
    else
        message( "setStandbyToHVOn timedOut" );
    return ret;
}
//______________________________________________________________________________
bool RFGunInterface::setHVOnToTrig( time_t waitTime )
{
    return setModStateAndWait( RFGunStructs::MOD_SET_STATE::SET_MOD_TRIG, waitTime );
}
//______________________________________________________________________________
bool RFGunInterface::setStandbyToOff( time_t waitTime )
{
    return setModStateAndWait( RFGunStructs::MOD_SET_STATE::SET_MOD_OFF, waitTime );
}
//______________________________________________________________________________
bool RFGunInterface::setHVOnToStandby( time_t waitTime )
{
    return setModStateAndWait( RFGunStructs::MOD_SET_STATE::SET_MOD_STANDBY, waitTime );
}
//______________________________________________________________________________
bool RFGunInterface::setTrigToHVOn( time_t waitTime )
{
    return setModStateAndWait( RFGunStructs::MOD_SET_STATE::SET_MOD_HV_ON, waitTime );
}
//______________________________________________________________________________
bool RFGunInterface::switchOnRF()
{
    // This needs to check that the the warmup is goo,

    // i.e. you can be SET HV_ON mode, but the warmup timer is still going (so the actual mode is standby_request...

    // basically it needs a bit of cross-checking...


    bool success = true;
    RFGunStructs::MOD_STATE currentModState = getModState();

    message("INIT state ======== ", ENUM_TO_STRING( currentModState) );
    message("isModIniLock() ======== ", isModILockStateGood() );
    if( isModILockStateNotGood() )
    {
        message("isModILockStateNotGood == true,      modreset");
        modReset();
        success = waitFor( &RFGunInterface::isModILockStateGood, *this, "Timeout waiting for Modulator to reset ",  2 ); // MAGIC_NUMBER
    }


    currentModState = getModState();
    message("New state ======== ", ENUM_TO_STRING( currentModState) );
    if( success )
    {

        switch( currentModState )
        {
            case RFGunStructs::MOD_STATE::Trig:
                success = true;
                break;

            case RFGunStructs::MOD_STATE::HV_On:
                success = setHVOnToTrig( 2 ); // MAGIC NUMBER
                break;

            case RFGunStructs::MOD_STATE::Standby:
                message("STARTING FROM STANDY " );
                success = setStandbyToHVOn( 999 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
                if( success )
                    success = setHVOnToTrig( 2 ); // MAGIC NUMBER

                break;

            case RFGunStructs::MOD_STATE::Standby_Request:

                success = setStandbyToHVOn( 999 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
                if( success )
                    success = setHVOnToTrig( 2 ); // MAGIC NUMBER

                break;

            case RFGunStructs::MOD_STATE::OFF:

                success = setOffToStandby( 2 ); // MAGIC NUMBER
                if( success )
                    success = setStandbyToHVOn( 999 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
                    if( success )
                        success = setHVOnToTrig( 2 ); // MAGIC NUMBER
                break;

            default:
                message("ERROR: in goToStandby: Mod State of ", ENUM_TO_STRING(currentModState), " not supported.");
        }
    }
    else
        message("Failed to reset mod");
    return success;
}
//______________________________________________________________________________
bool RFGunInterface::switchOffRF()
{
    bool success = true;
    RFGunStructs::MOD_STATE currentModState = getModState();

    currentModState = getModState();

    switch( currentModState )
    {
        case RFGunStructs::MOD_STATE::OFF:
            success = true;
            break;

        case RFGunStructs::MOD_STATE::Standby:
            success = setStandbyToOff( 2 ); // MAGIC NUMBER
            break;

        case RFGunStructs::MOD_STATE::Standby_Request:

            success = setStandbyToOff( 2 ); // MAGIC NUMBER
            break;

        case RFGunStructs::MOD_STATE::HV_On:

            success = setHVOnToStandby( 2 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
            if( success )
                success = setStandbyToOff( 2 ); // MAGIC NUMBER
            break;

        case RFGunStructs::MOD_STATE::Trig:

            success = setTrigToHVOn( 2 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
            if( success )
                success = setHVOnToStandby( 2 ); // MAGIC NUMBER
                if( success )
                    success = setStandbyToOff( 2 );
            break;

        default:
            message("Err");
    }
    return success;
}
//______________________________________________________________________________
bool RFGunInterface::goToStandby()
{
    bool success = true;
    RFGunStructs::MOD_STATE currentModState = getModState();
    currentModState = getModState();

    switch( currentModState )
    {
        case RFGunStructs::MOD_STATE::OFF:
            success = setOffToStandby( 2 ); // MAGIC NUMBER
            break;

        case RFGunStructs::MOD_STATE::Standby:
            success = true; // MAGIC NUMBER
            break;

        case RFGunStructs::MOD_STATE::Standby_Request:

            setModToStandby();
            break;

        case RFGunStructs::MOD_STATE::HV_On:

            success = setHVOnToStandby( 2 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
            break;

        case RFGunStructs::MOD_STATE::Trig:

            success = setTrigToHVOn( 2 ); // MAGIC NUMBER due to warmup time, should be in conifg FILE!!!!
            if( success )
                success = setHVOnToStandby( 2 ); // MAGIC NUMBER
            break;

        default:
            message("ERROR: in goToStandby: Mod State of ", ENUM_TO_STRING(currentModState), " not supported.");
    }
    return success;
}
//______________________________________________________________________________
bool RFGunInterface::waitFor( ABoolMemFn f1, RFGunInterface & obj, const std::string & m,  const  time_t waitTime, const size_t pause)
{
    return waitFor( f1, obj, m.c_str(), waitTime);
}
//______________________________________________________________________________
bool RFGunInterface::waitFor( ABoolMemFn f1, RFGunInterface & obj, const char * m,  const time_t waitTime,const  size_t pause )
{
    time_t startTime = time( 0 );

    bool timedOut    = false;

    bool state = false;

    while( true )
    {
        if( CALL_MEMBER_FN(obj, f1)() )
            break;

        else if( time(0) > startTime + waitTime )
            timedOut = true;

        if( timedOut )
        {
            message( m );
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds( pause )); // MAGIC NUMBER
    }
    return !timedOut;
}



//
//bool RFGunInterface::waitForModState( RFGunStructs::MOD_STATE state, time_t waitTime )
//{
//    time_t startTime = time( 0 );
//
//    bool timedOut    = false;
//
//    while( true )
//    {
//        if( state == getModState() )
//            break;
//
//        else if( time(0) > startTime + waitTime )
//            timedOut = true;
//
//        if( timedOut )
//        {
//            message("Timed Out While Waiting For Modulator Warmup");
//            break;
//        }
//    }
//    return !timedOut;
//}






//
////______________________________________________________________________________
//void RFGunInterface::setModToOff()
//{
//    if( RFObject.mod.state == RFGunStructs::MOD_STATE::OFF )
//        message( "setModToOff - BUT THE MOD STATE IS OFF");
//    else
//    {
//        switch( RFObject.mod.state )
//        {// first we step back higher states to standby
//            case RFGunStructs::MOD_STATE::Trig:
//                setModToHVOn();
//                setModToStandby();
//                break;
//            case RFGunStructs::MOD_STATE::HV_On:
//                setModToStandby();
//                break;
//        }// from standby we can go to off...
//        caput( RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHTYPE,
//               RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHID,
//               SET_MOD_OFF,  "" , "!!RFGunInterface TIMEOUT!! In setModToOff() " );
//    }
//}
////______________________________________________________________________________
//void RFGunInterface::setModToTrig()
//{
//    if( RFObject.mod.state == RFGunStructs::MOD_STATE::Trig )
//            debugMessage( "setModToTrig - BUT THE MOD STATE IS Trig");
//    else if( isModILockStateGood() )
//    {
//        switch( RFObject.mod.state )
//        {
//            case RFGunStructs::MOD_STATE::Standby:
//                setModToHVOn();
//                break;
//            case RFGunStructs::MOD_STATE::OFF:
//                setModToStandby();
//                setModToHVOn();
//                break;
//        }
//        caput( RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHTYPE,
//               RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHID,
//               SET_MOD_TRIG,  "" , "!!RFGunInterface TIMEOUT!! In setModToTrig() " );
//    }
//    else
//        debugMessage( "ERROR: in setModToTrig: Mod iLock is Not Good.");
//}
////______________________________________________________________________________
//void RFGunInterface::setModToStandby()
//{
//    if( RFObject.mod.state == RFGunStructs::MOD_STATE::Standby )
//            debugMessage( "setModToStandby - BUT THE MOD STATE IS Standby");
//    else if( isModILockStateGood() )
//    {
//        if( RFObject.mod.state == RFGunStructs::MOD_STATE::Standby )
//            setModToHVOn();
//
//        caput( RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHTYPE,
//               RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHID,
//               SET_MOD_STANDBY,  "" , "!!RFGunInterface TIMEOUT!! In setModToStandby() " );
//    }
//    else
//        debugMessage( "ERROR: in setModToStandby: Mod iLock is Not Good.");
//}
////______________________________________________________________________________
//void RFGunInterface::setModToHVOn()
//{
//    if( RFObject.mod.state == RFGunStructs::MOD_STATE::HV_On )
//        debugMessage( "setModToHVOn - BUT THE MOD STATE IS HV_On");
//    else if( isModILockStateGood() )
//    {
//        if( RFObject.mod.state == RFGunStructs::MOD_STATE::OFF )
//            setModToStandby();
//
//        caput( RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHTYPE,
//               RFObject.mod.pvComStructs[ RFGunStructs::RF_PV_TYPE::MOD_STATE_SET ].CHID,
//               SET_MOD_HV_ON,  "" , "!!RFGunInterface TIMEOUT!! In setModToHVOn() " );
//    }
//    else
//        debugMessage( "ERROR: in setModToHVOn: Mod iLock is Not Good.");
//}
