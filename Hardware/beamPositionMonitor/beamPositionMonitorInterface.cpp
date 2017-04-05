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

#include "beamPositionMonitorInterface.h"
//#include "velaScopeInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <numeric>
#include <thread>
#include <ctime>
#include <string>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include <epicsTime.h>

beamPositionMonitorInterface::beamPositionMonitorInterface( const std::string & configFileLocation, const bool* show_messages_ptr,
                                                            const bool * show_debug_messages_ptr,   const bool shouldStartEPICS,
                                                            const bool startVirtualMachine, const VELA_ENUM::MACHINE_AREA myMachineArea ):
configReader( configFileLocation, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine ),
interface( show_messages_ptr, show_debug_messages_ptr ),
shouldStartEPICS( shouldStartEPICS ),
startVM( startVirtualMachine ),
machineArea( myMachineArea )
{
    initialise();
}
//______________________________________________________________________________
//beamPositionMonitorInterface::beamPositionMonitorInterface( const bool* show_messages_ptr, const bool * show_debug_messages_ptr )
//: configReader( show_messages_ptr, show_debug_messages_ptr  ), interface( show_messages_ptr, show_debug_messages_ptr  )
//{
//    initialise();
//}
//______________________________________________________________________________
beamPositionMonitorInterface::~beamPositionMonitorInterface()
{
//    debugMessage( "magnetInterface DESTRUCTOR CALLED");
    killILockMonitors();
    for( auto && it : continuousMonitorStructs )
    {
        debugMessage("delete ", it -> objName, " ", ENUM_TO_STRING(it->monType), " continuousMonitorStructs entry.");
        delete it;
    }
    for( auto && it : dataMonitorStructs )
    {
        debugMessage("delete ", it -> objName, " ", ENUM_TO_STRING(it->monType), " dataMonitorStructs entry.");
        delete it;
    }
//    debugMessage( "magnetInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void beamPositionMonitorInterface::killMonitor( beamPositionMonitorStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( *ms -> EVID );
    if( status == ECA_NORMAL)
        debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR bpmInterface: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}
//______________________________________________________________________________
void beamPositionMonitorInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfigFiles();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initBPMObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICS )
            {
                std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initBPMChids();
                /// start the monitors: set up the callback functions
                monitorBPMs();
                /// The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); // MAGIC_NUMBER
            }
            else
                message("The bpmInterface Read Config files, Not Starting EPICS Monitors" );
        }
        else
            message( "!!!The bpmInterface received an Error while getting BPM data!!!" );
    }
}
////______________________________________________________________________________
//
//void beamPositionMonitorInterface::getBPMNames( std::vector< std::string >  & bpmNames )
//{
//    bpmNames.clear();
//    for( auto const & it : allBPMData )
//        bpmNames.push_back( it.first );
//}
//______________________________________________________________________________
bool beamPositionMonitorInterface::initBPMObjects()
{
    bpmObj = configReader.getBPMObject();
    return true;
}
//______________________________________________________________________________
void beamPositionMonitorInterface::initBPMChids()
{
    message( "\n", "Searching for bpm chids..." );
    // monitor objects
    for( auto && it1 : bpmObj.dataObjects )
        for( auto && it2 : it1.second.pvMonStructs )
            addChannel( it1.second.pvRoot, it2.second );
    // command objects
    for( auto && it1 : bpmObj.dataObjects )
        for( auto && it2 : it1.second.pvComStructs )
            addChannel( it1.second.pvRoot, it2.second );
    // send
    int status = sendToEpics( "ca_create_channel", "Found bpm chids.", "!!TIMEOUT!! Not all bpm ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds( 500 )); /// MAGIC NUMBERR
        for( auto && it1 : bpmObj.dataObjects )
        {
            for( auto && it2 : it1.second.pvMonStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
            for( auto && it2 : it1.second.pvComStructs )
                checkCHIDState( it2.second.CHID, it2.second.pvSuffix  );
        }
        message("");
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void beamPositionMonitorInterface::addChannel( const std::string & pvRoot, beamPositionMonitorStructs::pvStruct & pv )
{
    std::stringstream s1;
    s1 << pvRoot << ":" << pv.pvSuffix;
    ca_create_channel( s1.str().c_str(), 0, 0, 0, &pv.CHID );
    debugMessage( "Create channel to ", s1.str() );
}
//______________________________________________________________________________
void beamPositionMonitorInterface::monitorBPMs()
{
    continuousMonitorStructs.clear();
    dataMonitorStructs.clear();

    for( auto && it1 : bpmObj.dataObjects )
    {
        it1.second.machineArea = machineArea;
        for( auto && it2 : it1.second.pvMonStructs )
        {
//            if( !isADataPV( it2.second.pvType ) )
//            {   //NOT SURE ABOUT THIS "IF" -----> I WANT TO SET THIS BOOL TO TRUE EVEN IF IT IS A DATA PV
                it1.second.bpmRawData.isAContinuousMonitorStruct = true;
                it1.second.bpmRawData.isATemporaryMonitorStruct = false;
                it1.second.bpmRawData.appendingData = false;
                if( it1.second.bpmRawData.isAContinuousMonitorStruct )
                {
                    resetDataVectors( 1 );
                    addToMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );
                }
//            }
        }
    }


    for( auto && it1 : bpmObj.dataObjects )
        for( auto && it2 : it1.second.pvComStructs )
            addToMonitorStructs( continuousMonitorStructs, it2.second, &it1.second  );

    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to bpm Monitors", "!!TIMEOUT!! Subscription to bpm Monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//______________________________________________________________________________
void beamPositionMonitorInterface::addToMonitorStructs( std::vector< beamPositionMonitorStructs::monitorStruct * > & msv, beamPositionMonitorStructs::pvStruct & pv,  beamPositionMonitorStructs::bpmDataObject * bpmDataObj )
{
    msv.push_back( new beamPositionMonitorStructs::monitorStruct() );
    msv.back() -> monType    = pv.pvType;
    msv.back() -> bpmObject  = &bpmObj;
    msv.back() -> interface  = this;
    msv.back() -> CHTYPE     = pv.CHTYPE;
    msv.back() -> EVID       = &pv.EVID;
    switch( pv.pvType )
    {
        case beamPositionMonitorStructs::BPM_PV_TYPE::SA1:
        {
            msv.back() -> val = (void*) &bpmDataObj -> sa1 ;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SA2:
        {
            msv.back() -> val = (void*) &bpmDataObj -> sa2 ;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::DATA:
        {
            // For the traces we can just pass in the powerObject for value, and figure everything out from them in the callback functions
            msv.back() -> val = (void*) &bpmDataObj -> bpmRawData;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SD1:
        {
            msv.back() -> val = (void*) &bpmDataObj -> sd1 ;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SD2:
        {
            msv.back() -> val = (void*) &bpmDataObj -> sd2 ;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA1:
        {
            msv.back() -> val = (void*) &bpmDataObj -> ra1;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }

        case beamPositionMonitorStructs::BPM_PV_TYPE::RA2:
        {
            msv.back() -> val = (void*) &bpmDataObj -> ra2;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD1:
        {
            msv.back() -> val = (void*) &bpmDataObj -> rd1;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD2:
        {
            msv.back() -> val = (void*) &bpmDataObj -> rd2;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::X:
        {
            msv.back() -> val = (void*) &bpmDataObj -> xPV;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::Y:
        {
            msv.back() -> val = (void*) &bpmDataObj -> yPV;
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  beamPositionMonitorInterface::staticEntryrMonitor, (void*)msv.back(), &pv.EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void beamPositionMonitorInterface::staticEntryrMonitor( const event_handler_args args )
{
    beamPositionMonitorStructs::monitorStruct * ms = static_cast< beamPositionMonitorStructs::monitorStruct *> ( args.usr );
    switch( ms -> monType )
    {
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA1:
        {
            ms->interface->updateValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA2:
        {
            ms->interface->updateValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD1:
        {
            ms->interface->updateValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD2:
        {
            ms->interface->updateValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::X:
        {
            ms->interface->updateValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::Y:
        {
            ms->interface->updateValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::DATA:
        {
            ms->interface->updateData( ms, args );
            break;
        }
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateData( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    /// this could be better, with the type passed from the config
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;
    beamPositionMonitorStructs::rawDataStruct * bpmdo = reinterpret_cast< beamPositionMonitorStructs::rawDataStruct *> (ms -> val);
    if( bpmdo->isAContinuousMonitorStruct )
    {
        for( auto && it1 : bpmObj.dataObjects )
        {
            it1.second.numShots = 1;
            it1.second.shotCount = 0;
        }
    }
    const dbr_double_t * value = &(p  -> value);
    size_t i = 0;
    updateTime( p->stamp, bpmdo->timeStamps[ bpmdo->shotCount ], bpmdo->strTimeStamps[ bpmdo->shotCount ]  );

    for( auto && it : bpmdo->rawBPMData[ bpmdo->shotCount ] )
    {
        it = *( &p->value + i);
        ++i;
    }

    bpmdo->pu1[ bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 1 ];
    bpmdo->pu2[ bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 2 ];
    bpmdo->c1[  bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 3 ];
    bpmdo->p1[  bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 4 ];
    bpmdo->pu3[ bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 5 ];
    bpmdo->pu4[ bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 6 ];
    bpmdo->c2[  bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 7 ];
    bpmdo->p2[  bpmdo->shotCount ] = bpmdo->rawBPMData[ bpmdo->shotCount ][ 8 ];

    bpmdo->x[ bpmdo->shotCount ] = calcX( bpmdo->name, bpmdo->pu1[ bpmdo->shotCount ], bpmdo->pu2[ bpmdo->shotCount ], bpmdo->c1[ bpmdo->shotCount ], bpmdo->p1[ bpmdo->shotCount ] );
    bpmdo->y[ bpmdo->shotCount ] = calcY( bpmdo->name, bpmdo->pu3[ bpmdo->shotCount ], bpmdo->pu4[ bpmdo->shotCount ], bpmdo->c2[ bpmdo->shotCount ], bpmdo->p2[ bpmdo->shotCount ] );
    bpmdo->q[ bpmdo->shotCount ] = calcQ( bpmdo->name, bpmdo->rawBPMData[ bpmdo -> shotCount ] );

    if( bpmdo -> isATemporaryMonitorStruct )
    {
        if( bpmdo -> numShots > -1 )
        {
            ++bpmdo -> shotCount;
        }
        if( bpmdo->shotCount == bpmdo->numShots )
        {
            message( "Collected ", bpmdo->shotCount, " shots for ", bpmdo->name );
            ms->interface->killCallBack( ms, bpmdo );//, bpmdo );
            monitoringData = false;
            bpmdo -> shotCount = 0;
        }
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    switch( ms -> CHTYPE )
    {
        case DBR_DOUBLE:
        {
            *(double*)ms -> val = *(double*)args.dbr;
            break;
        }
        case DBR_LONG:
        {
            *(long*)ms -> val = *(long*)args.dbr;
            break;
        }
        case DBR_TIME_DOUBLE:
        {
            ms->interface->debugMessage("beamPositionMonitorInterface::updateValue recieved unexpected chtype");
            break;
        }
        default:
            ms->interface->debugMessage("beamPositionMonitorInterface::updateValue recieved unexpected chtype");
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::killCallBack( beamPositionMonitorStructs::monitorStruct * ms, beamPositionMonitorStructs::rawDataStruct * bpmdo )///, beamPositionMonitorStructs::bpmDataObject * bpmdo )
{
    int status = ca_clear_subscription( *ms -> EVID );
    if( status == ECA_NORMAL)
    {
//        debugMessage( ms -> scopeObject, " monitoring = false ");

        isMonitoringMap[ ms -> bpmObject ] = false;
        bpmdo -> appendingData = false;

        delete ms;
        message( bpmdo->name, "callback deleted" );
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->bpmObject );
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::monitorDataForNShots( size_t N, const std::string & name  )
{
    if( !bpmObj.dataObjects.at( name ).appendingData )
    {
        dataMonitorStructs.clear();
//        debugMessage( "Starting bpm data Monitor " );
        resetDataVectors( N );
//        debugMessage( "Vectors Reset" );
//        for( auto && it1 : bpmObj.dataObjects.at( bpmNames ) )
//        {
//            it1.second.numShots = N;
//            for( auto && it2 : it1.second.pvMonStructs.at( bpmNames ) )
//            {
//                if( isADataPV( it2.first ) )
//                {
//                    monitoringData = true;
//                    it1.second.isAContinuousMonitorStruct=false;
//                    it1.second.isATemporaryMonitorStruct=true;
//                    addToMonitorStructs( dataMonitorStructs, it2.second, &it1.second  );
//
//                }
//            }
//        }
        bpmObj.dataObjects.at( name ).bpmRawData.numShots = N;
//                if( isADataPV( bpmObj.dataObjects.at( name ).pvMonStructs.at( name ) ) )
//                {
        bpmObj.dataObjects.at( name ).bpmRawData.appendingData=true;
        bpmObj.dataObjects.at( name ).bpmRawData.isAContinuousMonitorStruct=false;
        bpmObj.dataObjects.at( name ).bpmRawData.isATemporaryMonitorStruct=true;
        addToMonitorStructs( dataMonitorStructs, bpmObj.dataObjects.at( name ).pvMonStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::DATA ) , &bpmObj.dataObjects.at( name )  );
//            }
//        }
        int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to bpm data Monitors failed" );
        if ( status == ECA_NORMAL )
            monitoringData = true; /// interface base class member
    }
    else
    {
        message( "Already Monitoring Data " ); /// make more useful
    }

}
//______________________________________________________________________________
void beamPositionMonitorInterface::monitorDataForNShots( size_t N, const std::vector< std::string > & names )
{
    resetDataVectors( N );
    for( auto it : names )
    {
        if( !bpmObj.dataObjects.at( it ).bpmRawData.appendingData )
        {
            message( "Starting bpm data Monitor " );



            bpmObj.dataObjects.at( it ).bpmRawData.numShots = N;
            bpmObj.dataObjects.at( it ).bpmRawData.isAContinuousMonitorStruct=false;
            bpmObj.dataObjects.at( it ).bpmRawData.isATemporaryMonitorStruct=true;
            bpmObj.dataObjects.at( it ).bpmRawData.appendingData=true;


            addToMonitorStructs( dataMonitorStructs, bpmObj.dataObjects.at( it ).pvMonStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::DATA ) , &bpmObj.dataObjects.at( it )  );
    //            }
    //        }
            int status = sendToEpics( "ca_create_subscription", "", "!!TIMEOUT!! Subscription to bpm data Monitors failed" );
            if ( status == ECA_NORMAL )
                monitoringData = true; /// interface base class member
        }
        else
        {
            message( "Already Monitoring Data " ); /// make more useful
        }
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::resetDataVectors( size_t N )
{
    dataMonitorStructs.clear();
    for( auto && it : bpmObj.dataObjects )
    {   // Clear everything
        it.second.bpmRawData.rawBPMData.clear();
        it.second.bpmRawData.timeStamps.clear();
        it.second.bpmRawData.strTimeStamps.clear();
        it.second.bpmRawData.p1.clear();
        it.second.bpmRawData.p2.clear();
        it.second.bpmRawData.pu1.clear();
        it.second.bpmRawData.pu2.clear();
        it.second.bpmRawData.pu3.clear();
        it.second.bpmRawData.pu4.clear();
        it.second.bpmRawData.c1.clear();
        it.second.bpmRawData.c2.clear();
        it.second.bpmRawData.x.clear();
        it.second.bpmRawData.y.clear();
        it.second.bpmRawData.q.clear();
        // Resize to N shots
        it.second.bpmRawData.rawBPMData.resize( N );
        it.second.bpmRawData.timeStamps.resize( N );
        it.second.bpmRawData.strTimeStamps.resize( N );
        it.second.bpmRawData.p1.resize( N );
        it.second.bpmRawData.p2.resize( N );
        it.second.bpmRawData.pu1.resize( N );
        it.second.bpmRawData.pu2.resize( N );
        it.second.bpmRawData.pu3.resize( N );
        it.second.bpmRawData.pu4.resize( N );
        it.second.bpmRawData.c1.resize( N );
        it.second.bpmRawData.c2.resize( N );
        it.second.bpmRawData.x.resize( N );
        it.second.bpmRawData.y.resize( N );
        it.second.bpmRawData.q.resize( N );
        // resize sub-vectors to COUNT elements
        for( auto && it2 : it.second.bpmRawData.rawBPMData )
        {
            it2.resize( it.second.pvMonStructs[ beamPositionMonitorStructs::BPM_PV_TYPE::DATA ].COUNT );
        }
    }
}
//______________________________________________________________________________
bool beamPositionMonitorInterface::isADataPV( beamPositionMonitorStructs::BPM_PV_TYPE pv )
{
    bool ret = false;
    if( pv == beamPositionMonitorStructs::BPM_PV_TYPE::DATA )
        ret = true;
    return ret;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::calcX( const std::string & name, double u11, double u12, double u13, double u14 )
{
    double x;
    double mn, v11, v12, xn, num, den, t1;

    mn  = bpmObj.dataObjects.at( name ).mn ;
    xn  = bpmObj.dataObjects.at( name ).xn ;

    double u1114, u1214, u1314;
    u1114 = u11 - u14;
    u1214 = u12 - u14;
    u1314 = u13 - u14;

    t1 = ( ( u1214 + sqrt( pow( u1214 , 2 ) - ( 4 * u1114 * u1314 ) )  ) / ( 2 * u1114 ) );

    v11 = u1114;
    v12 = u1214;// - ( t1 * u2124 );

    num = v12 - v11;
    den = v12 + v11;

    x = -1 * ( ( num / den ) * mn ) - xn;

    return x;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::calcY( const std::string & name, double u21, double u22, double u23, double u24 )
{
    double y;
    double mn, v21, v22, yn, num, den, t2;

    mn  = bpmObj.dataObjects.at( name ).mn ;
    yn  = bpmObj.dataObjects.at( name ).yn ;

    double u2124, u2224, u2324;
    u2124 = u21 - u24;
    u2224 = u22 - u24;
    u2324 = u23 - u24;

    t2 = ( ( u2224 + sqrt( pow( u2224 , 2 ) - ( 4 * u2124 * u2324 ) )  ) / ( 2 * u2124 ) );

    v21 = u2124;
    v22 = u2224;// - ( t1 * u2124 );

    num = v22 - v21;
    den = v22 + v21;

    y = -1 * ( ( num / den ) * mn ) - yn;

    return y;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::calcQ( const std::string & bpmName, std::vector< double > rawData )
{
    double q;

    double v1Cal, v2Cal, att1Cal, att2Cal, qCal, v1, v2, q1, q2;
    double u11, u12, u13, u14, u21, u22, u23, u24;
    long ra1, ra2;
    u11 = rawData[ 1 ];
    u12 = rawData[ 2 ];
    u13 = rawData[ 3 ];
    u14 = rawData[ 4 ];
    u21 = rawData[ 5 ];
    u22 = rawData[ 6 ];
    u23 = rawData[ 7 ];
    u24 = rawData[ 8 ];

    ra1       = beamPositionMonitorInterface::getRA1( bpmName )     ;
    ra2       = beamPositionMonitorInterface::getRA2( bpmName )     ;
    v1Cal     = bpmObj.dataObjects.at( bpmName ).v1cal  ;
    v2Cal     = bpmObj.dataObjects.at( bpmName ).v2cal  ;
    att1Cal   = bpmObj.dataObjects.at( bpmName ).att1cal;
    att2Cal   = bpmObj.dataObjects.at( bpmName ).att2cal;
    qCal      = bpmObj.dataObjects.at( bpmName ).qcal   ;

    v1 = ( std::abs( u11 - u14 ) + std::abs( u12 - u14 ) ) / 2;
    v2 = ( std::abs( u21 - u24 ) + std::abs( u22 - u24 ) ) / 2;
    q1 = ( qCal * ( v1 / v1Cal ) ) * ( pow( 10, - ( ( att1Cal - double( ra1 ) ) / 20 ) ) );
    q2 = ( qCal * ( v2 / v2Cal ) ) * ( pow( 10, - ( ( att2Cal - double( ra2 ) ) / 20 ) ) );

    q = ( ( q1 + q2 ) / 2 );

    return q;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getBPMResolution( const std::string & bpmName )
{
    double res;
    double u11, u12, u13, u14, u21, u22, u23, u24, v11, v12, v21, v22;
    std::vector< double > rmsVals;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData.size() != 0 )
    {
        for( unsigned int i = 0; i < bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData.size(); i++ )
        {
            u11 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 1 ];
            u12 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 2 ];
            u13 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 3 ];
            u14 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 4 ];
            u21 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 5 ];
            u22 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 6 ];
            u23 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 7 ];
            u24 = bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData[ i ][ 8 ];
            v11 = u11 - u14;
            v12 = u12 - u14;
            v21 = u21 - u24;
            v22 = u22 - u24;
            if( v11 && v12 && v21 && v22 != 0 )
            {
                rmsVals.push_back( ( ( v11 + v12 ) - ( v21 + v22 ) ) / ( ( v11 + v12 ) + ( v21 + v22 ) ) );
            }
        }
        double rms = sqrt( ( std::inner_product( rmsVals.begin(), rmsVals.end(), rmsVals.begin(), 0 ) ) / static_cast<double>( rmsVals.size() ) );
        res = rms * sqrt( 2 ) * bpmObj.dataObjects.at( bpmName ).mn;
        return res;
    }
    else
    {
        message( "BPM resolution calculation didn't work" );
        return res;
    }
}
//______________________________________________________________________________
bool beamPositionMonitorInterface::isMonitoringBPMData( const std::string & name )
{
    if( bpmObj.dataObjects.at( name ).bpmRawData.appendingData == true )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//______________________________________________________________________________
bool beamPositionMonitorInterface::isNotMonitoringBPMData( const std::string & name )
{
    return !isMonitoringBPMData( name );
}
//______________________________________________________________________________
const beamPositionMonitorStructs::rawDataStruct & beamPositionMonitorInterface::getAllBPMData( const std::string & bpmName )
{
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData.size() != 0 )
    {
        return bpmObj.dataObjects.at( bpmName ).bpmRawData;
    }
}
//______________________________________________________________________________
const beamPositionMonitorStructs::bpmDataObject & beamPositionMonitorInterface::getBPMDataObject( const std::string & bpmName )
{
//    if( entryExists( bpmObj.dataObjects, bpmName ) )
//    {
        return bpmObj.dataObjects.at( bpmName );
//    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::setSA1( const std::string & bpmName, long val )
{
    caput( bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SA1 ).CHTYPE,
           bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SA1 ).CHID,
           val, "" , "!!beamPositionMonitorInterface TIMEOUT!! In setSA1() ");
}
//______________________________________________________________________________
void beamPositionMonitorInterface::setSA2( const std::string & bpmName, long val )
{
    caput( bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SA2 ).CHTYPE,
           bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SA2 ).CHID,
           val, "" , "!!beamPositionMonitorInterface TIMEOUT!! In setSA2() ");
}
//______________________________________________________________________________
void beamPositionMonitorInterface::setSD1( const std::string & bpmName, long val )
{
    caput( bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SD1 ).CHTYPE,
           bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SD1 ).CHID,
           val, "" , "!!beamPositionMonitorInterface TIMEOUT!! In setSD1() ");
}
//______________________________________________________________________________
void beamPositionMonitorInterface::setSD2( const std::string & bpmName, long val )
{
    caput( bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SD2 ).CHTYPE,
           bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::SD2 ).CHID,
           val, "" , "!!beamPositionMonitorInterface TIMEOUT!! In setSD2() ");
}
//______________________________________________________________________________
void beamPositionMonitorInterface::setX( const std::string & bpmName, double val )
{
    caput( bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::X ).CHTYPE,
           bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::X ).CHID,
           val, "" , "!!beamPositionMonitorInterface TIMEOUT!! In setX() ");
}
//______________________________________________________________________________
void beamPositionMonitorInterface::setY( const std::string & bpmName, double val )
{
    caput( bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::Y ).CHTYPE,
           bpmObj.dataObjects.at( bpmName ).pvComStructs.at( beamPositionMonitorStructs::BPM_PV_TYPE::Y ).CHID,
           val, "" , "!!beamPositionMonitorInterface TIMEOUT!! In setX() ");
}
//______________________________________________________________________________
std::vector< std::vector< double > > beamPositionMonitorInterface::getBPMRawData( const std::string & bpmName )
{
    std::vector< std::vector< double > > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.rawBPMData;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getBPMXVec( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.x.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.x;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getBPMYVec( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.y.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.y;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getBPMQVec( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.q.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.q;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getTimeStamps( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.timeStamps.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.timeStamps;
    else
        return r;
}
//______________________________________________________________________________
std::vector< std::string > beamPositionMonitorInterface::getStrTimeStamps( const std::string & bpmName )
{
    std::vector< std::string > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.strTimeStamps.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.strTimeStamps;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getRA1( const std::string & bpmName )
{
    /// djs 10.11.15, i think this needs to be more like:
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).ra1;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getRA2( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).ra2;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getRD1( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).rd1;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getRD2( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).rd2;
    else
        return r;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getX( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.x.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.x.back();
    else
        return r;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getY( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.y.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.y.back();
    else
        return r;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getQ( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).bpmRawData.q.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).bpmRawData.q.back();
    else
        return r;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getXFromPV( const std::string & bpmName )
{
    /// The EPICS X/Y values (EBT-INJ-DIA-BPMC-[xx]:X / Y) neglect certain factors which
    /// should be considered to get an accurate value for x/y. This function returns the
    /// x value directly from EPICS. Use the function getX/Y for the calculated values.
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).xPV;
    else
        return r;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getYFromPV( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).yPV;
    else
        return r;
}
//______________________________________________________________________________
void beamPositionMonitorInterface::reCalAttenuation( const std::string & bpmName, double qScope )
{
    /// For this function we get a charge value from somewhere else ( e.g. a scopeInterface ) and pass it in
    double att1Cal, att2Cal, v1Cal, v2Cal, qCal, qqC;

    v1Cal     = bpmObj.dataObjects.at( bpmName ).v1cal  ;
    v2Cal     = bpmObj.dataObjects.at( bpmName ).v2cal  ;
    att1Cal   = bpmObj.dataObjects.at( bpmName ).att1cal;
    att2Cal   = bpmObj.dataObjects.at( bpmName ).att2cal;
    qCal      = bpmObj.dataObjects.at( bpmName ).qcal   ;

    qqC     = qScope / qCal;

    long currAtt1 = beamPositionMonitorInterface::getRA1( bpmName );
    long currAtt2 = beamPositionMonitorInterface::getRA2( bpmName );

    long newAtt1 = ( 20 * log10( qqC ) ) + att1Cal;
    long newAtt2 = ( 20 * log10( qqC ) ) + att2Cal;

    if( 0 <= newAtt1 && newAtt1 <= 20 )
    {
        beamPositionMonitorInterface::setSA1( bpmName, newAtt1 );
        message( "New SA1 for ", bpmName, " = ", beamPositionMonitorInterface::getRA1( bpmName ) );
    }
    else if( qScope < 20 )
    {
        beamPositionMonitorInterface::setSA1( bpmName, 0 );
        message( "New SA1 for ", bpmName, " = ", beamPositionMonitorInterface::getRA1( bpmName ) );
    }
    else
    {
        beamPositionMonitorInterface::setSA1( bpmName, currAtt1 );
        message( "Old SA1 for ", bpmName, " = ", beamPositionMonitorInterface::getRA1( bpmName ) );
    }


    if( 0 <= newAtt2 && newAtt2 <= 20 )
    {
        beamPositionMonitorInterface::setSA2( bpmName, newAtt2 );
        message( "New SA2 for ", bpmName, " = ", beamPositionMonitorInterface::getRA1( bpmName ) );
    }
    else if( qScope < 20 )
    {
        beamPositionMonitorInterface::setSA2( bpmName, 0 );
        message( "New SA2 for ", bpmName, " = ", beamPositionMonitorInterface::getRA2( bpmName ) );
    }
    else
    {
        beamPositionMonitorInterface::setSA2( bpmName, currAtt2 );
        message( "Old SA2 for ", bpmName, " = ", beamPositionMonitorInterface::getRA2( bpmName ) );
    }

}
//______________________________________________________________________________
VELA_ENUM::MACHINE_AREA beamPositionMonitorInterface::getMachineArea()
{
    return machineArea;
}
//______________________________________________________________________________
VELA_ENUM::MACHINE_MODE beamPositionMonitorInterface::getMachineMode()
{
    if ((startVM) && (shouldStartEPICS))
        return VELA_ENUM::MACHINE_MODE::VIRTUAL;
    else if ((startVM) && (!shouldStartEPICS))
        return VELA_ENUM::MACHINE_MODE::OFFLINE;
    else if (!startVM)
        return VELA_ENUM::MACHINE_MODE::PHYSICAL;
}
////______________________________________________________________________________
//bool beamPositionMonitorInterface::hasTrig( const std::string & bpm )
//{
//    bool ret = false;
//    if( allBPMData.count( bpm ) )
//        if( allBPMData[ bpm ].bpmState == VELA_ENUM::TRIG_STATE::TRIG )
//            ret = true;
//
//    if( ret )
//        debugMessage( bpm, " has trigger");
//    else
//        debugMessage( bpm, " has no trigger");
//
//    return ret;
//}
////______________________________________________________________________________
//bool beamPositionMonitorInterface::hasNoTrig( const std::string & bpm )
//{
//    bool ret = false;
//    if( allBPMData.count( bpm ) )
//        if( allBPMData[ bpm ].bpmState == VELA_ENUM::TRIG_STATE::NOTRIG )
//            ret = true;
//
//    if( ret )
//        debugMessage( bpm, " has trigger");
//    else
//        debugMessage( bpm, " has no trigger");
//
//    return ret;
//}
//______________________________________________________________________________
std::vector< std::string > beamPositionMonitorInterface::getBPMNames()
{
    std::vector< std::string > bpmNames;
    for( auto && iter : bpmObj.dataObjects )
    {
        message( "BPM Name ", iter.first );
        bpmNames.push_back( iter.first );
    }

    return bpmNames;
}
////______________________________________________________________________________
//VELA_ENUM::TRIG_STATE beamPositionMonitorInterface::getBPMState( const  std::string & objName )
//{
//    VELA_ENUM::TRIG_STATE r =  VELA_ENUM::TRIG_STATE::TRIG_ERROR;
//    auto iter = allBPMData.find( objName );
//    if (iter != allBPMData.end() )
//        r = iter -> second.bpmState;
//    return r;
//}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > beamPositionMonitorInterface::getILockStates( const std::string & objName )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r;
    auto iter = allBPMData.find( objName );
    if( iter != allBPMData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  beamPositionMonitorInterface::getILockStatesStr( const std::string & name )
{
    std::map< VELA_ENUM::ILOCK_NUMBER, std::string  > r;
    auto iter = allBPMData.find( name );
    if( iter != allBPMData.end() )
        for( auto it : iter -> second.iLockStates )
            r[ it.first ] = ENUM_TO_STRING( it.second );
    return r;
}
//______________________________________________________________________________
bool beamPositionMonitorInterface::toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, beamPositionMonitorInterface & obj )
{
    if( CALL_MEMBER_FN(obj, f1)( name ) ) /// CALL_MEMBER_FN MACRO in structs.h
    {
        CALL_MEMBER_FN(obj, f3)( name ) ;

        time_t timeStart = time( 0 );  /// Start Clock

        while( CALL_MEMBER_FN(obj, f1)( name ) )
        {
            std::this_thread::sleep_for(std::chrono::milliseconds( 50 )); /// MAGIC NUMBER
            time_t currentTime = time( 0 );
            time_t timeDif = currentTime - timeStart;
            if( timeDif > waitTime )
            {
                message( "Toggle ", name, " timed out after ", waitTime," seconds");
                break;
            }
        }
    }
    /// Prevously we have still needed a sleep here, they shouldn't be needed, THIS NEEDS CHECKING
// std::this_thread::sleep_for(std::chrono::milliseconds( 20 ));
    return CALL_MEMBER_FN(obj, f2)( name );
}
