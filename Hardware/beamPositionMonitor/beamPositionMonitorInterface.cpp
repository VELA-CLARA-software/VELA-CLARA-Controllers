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
#include <math.h>
#include <stdlib.h>
#include <epicsTime.h>
#include <boost/circular_buffer.hpp>

beamPositionMonitorInterface::beamPositionMonitorInterface( const std::string & configFileLocation,
                                                            const bool& show_messages_ptr,
                                                            const bool& show_debug_messages_ptr,
                                                            const bool shouldStartEPICs,
                                                            const bool startVirtualMachine,
                                                            const HWC_ENUM::MACHINE_AREA myMachineArea ):
configReader( configFileLocation, show_messages_ptr, show_debug_messages_ptr, startVirtualMachine ),
interface( show_messages_ptr, show_debug_messages_ptr, shouldStartEPICs ),
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
    int status = ca_clear_subscription( ms -> EVID );
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
    UTL::STANDARD_PAUSE;
    if( configFileRead )
    {
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initBPMObjects();
        if( getDataSuccess )
        {
            if( shouldStartEPICs )
            {
                std::cout << "WE ARE HERE" << std::endl;
                /// subscribe to the channel ids
                initBPMChids();
                /// start the monitors: set up the callback functions
                monitorBPMs();
                /// The pause allows EPICS to catch up.
                UTL::PAUSE_500;
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
    {
        for( auto && it2 : it1.second.pvMonStructs )
        {
            addChannel( it1.second.pvRoot, it2.second );
        }
        for( auto && it2 : it1.second.pvComStructs )
        {
            addChannel( it1.second.pvRoot, it2.second );
        }
    }
    // send
    int status = sendToEpics( "ca_create_channel", "Found bpm chids.", "!!TIMEOUT!! Not all bpm ChIds found." );
    if( status == ECA_TIMEOUT )
    {
        UTL::PAUSE_500;
        for( auto && it1 : bpmObj.dataObjects )
        {
            for( auto && it2 : it1.second.pvMonStructs )
            {
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING(it2.first)  );
            }
            for( auto && it2 : it1.second.pvComStructs )
            {
                checkCHIDState( it2.second.CHID, ENUM_TO_STRING(it2.first)  );
            }
        }
        message("");
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void beamPositionMonitorInterface::addChannel( const std::string & pvRoot, beamPositionMonitorStructs::pvStruct & pv )
{
    std::string s1;
//    s1 << pvRoot << ":" << pv.pvSuffix;
    s1 = pvRoot + pv.pvSuffix;
    ca_create_channel( s1.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID );
    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void beamPositionMonitorInterface::monitorBPMs()
{
    continuousMonitorStructs.clear();
    dataMonitorStructs.clear();

    for( auto && it1 : bpmObj.dataObjects )
    {
        it1.second.machineArea = machineArea;
        it1.second.xPVBuffer.resize(UTL::BUFFER_TEN);
        it1.second.yPVBuffer.resize(UTL::BUFFER_TEN);
        it1.second.awakTStampBuffer.resize(UTL::BUFFER_TEN);
        for( auto && it2 : it1.second.pvMonStructs )
        {
            continuousMonitorStructs.push_back(new beamPositionMonitorStructs::monitorStruct());
            continuousMonitorStructs.back()->monType = it2.first;
            continuousMonitorStructs.back()->objName = it1.second.name;
            continuousMonitorStructs.back()->interface = this;
            continuousMonitorStructs.back()->val = &it1.second;
            it1.second.isAContinuousMonitorStruct = true;
            it1.second.isATemporaryMonitorStruct = false;
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   beamPositionMonitorInterface::staticEntryrMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back()->EVID);
            debugMessage("Adding monitor for ",it1.second.name, " ",ENUM_TO_STRING(it2.first));
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
//    msv.back() -> EVID       = &pv.EVID;

    switch( pv.pvType )
    {
        case beamPositionMonitorStructs::BPM_PV_TYPE::SA1:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SA2:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::DATA:
        {
            // For the traces we can just pass in the powerObject for value, and figure everything out from them in the callback functions
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SD1:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SD2:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA1:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA2:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD1:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD2:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::X:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::Y:
        {
            msv.back() -> objName = bpmDataObj -> name;
            break;
        }
        default:
            message("addToMonitorStructs ERROR PV_Type unknown");
    }
    ca_create_subscription( pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,  beamPositionMonitorInterface::staticEntryrMonitor, (void*)msv.back(), &msv.back() -> EVID); // &continuousMonitorStructs.back().EventID );
}
//______________________________________________________________________________
void beamPositionMonitorInterface::staticEntryrMonitor( const event_handler_args args )
{
    beamPositionMonitorStructs::monitorStruct * ms = static_cast< beamPositionMonitorStructs::monitorStruct *> ( args.usr );
    switch( ms -> monType )
    {
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA1:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).ra1 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RA2:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).ra2 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD1:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).rd1 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RD2:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).rd2 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SA1:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).sa1 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SA2:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).sa1 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SD1:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).sd1 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::SD2:
        {
            ms->interface->bpmObj.dataObjects.at(ms->objName).sd2 = *(long*)args.dbr;
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::X:
        {
            ms->interface->updateXValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::Y:
        {
            ms->interface->updateYValue( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::DATA:
        {
            ms->interface->updateData( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::AWAK:
        {
            ms->interface->updateDBRLong( ms, args );
            break;
        }
        case beamPositionMonitorStructs::BPM_PV_TYPE::RDY:
        {
            ms->interface->updateDBRLong( ms, args );
            break;
        }
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateData( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    /// this could be better, with the type passed from the config
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;

//    beamPositionMonitorStructs::bpmDataObject * bpmdo = reinterpret_cast< beamPositionMonitorStructs::bpmDataObject *> (ms -> val);
    beamPositionMonitorStructs::bpmDataObject * bpmdo = &ms->interface->bpmObj.dataObjects.at( ms->objName );

    if( bpmdo->isAContinuousMonitorStruct )
    {
        bpmdo->numShots = 1;
        bpmdo->shotCount = 0;
        if( bpmdo->rawBPMData.size() == 0 )
        {
            setBufferSize(UTL::TEN_SIZET);
            bpmdo->timeStamps.push_back(1);
            bpmdo->strTimeStamps.push_back(UTL::UNKNOWN_STRING);
            bpmdo->rawBPMData.resize(1);
            bpmdo->rawBPMData.back().resize( bpmdo->pvMonStructs[ beamPositionMonitorStructs::BPM_PV_TYPE::DATA ].COUNT );
            bpmdo->rawBPMDataBuffer.resize(bpmdo->buffer);
            bpmdo->xVec.push_back(UTL::DUMMY_DOUBLE);
            bpmdo->xBuffer.resize(bpmdo->buffer);
            bpmdo->yVec.push_back(UTL::DUMMY_DOUBLE);
            bpmdo->yBuffer.resize(bpmdo->buffer);
            bpmdo->qVec.push_back(UTL::DUMMY_DOUBLE);
            bpmdo->qBuffer.resize(bpmdo->buffer);
            bpmdo->pu1Buffer.resize(bpmdo->buffer);
            bpmdo->pu2Buffer.resize(bpmdo->buffer);
            bpmdo->c1Buffer.resize(bpmdo->buffer);
            bpmdo->p1Buffer.resize(bpmdo->buffer);
            bpmdo->pu3Buffer.resize(bpmdo->buffer);
            bpmdo->pu4Buffer.resize(bpmdo->buffer);
            bpmdo->c2Buffer.resize(bpmdo->buffer);
            bpmdo->p2Buffer.resize(bpmdo->buffer);
            bpmdo->statusBuffer.resize(bpmdo->buffer);
        }
    }

    const dbr_double_t * value = &(p  -> value);
    size_t i = 0;
    updateTime( p->stamp, bpmdo->timeStamps.back(), bpmdo->strTimeStamps.back() );
    std::vector< double > rawVectorContainer(9);
    for( auto && it : rawVectorContainer )
    {
        it = *( &p->value + i);
        ++i;
    }
    bpmdo->rawBPMDataBuffer.push_back(rawVectorContainer);
    bpmdo->timeStampsBuffer.push_back(bpmdo->timeStamps.back());

    if( bpmdo->xytype == UTL::Y_FIRST )
    {
        bpmdo->pu1Buffer.push_back(rawVectorContainer[ 1 ]);
        bpmdo->pu2Buffer.push_back(rawVectorContainer[ 2 ]);
        bpmdo->c1Buffer.push_back(rawVectorContainer[ 3 ]);
        bpmdo->p1Buffer.push_back(rawVectorContainer[ 4 ]);
        bpmdo->pu3Buffer.push_back(rawVectorContainer[ 5 ]);
        bpmdo->pu4Buffer.push_back(rawVectorContainer[ 6 ]);
        bpmdo->c2Buffer.push_back(rawVectorContainer[ 7 ]);
        bpmdo->p2Buffer.push_back(rawVectorContainer[ 8 ]);
    }
    else if( bpmdo->xytype == UTL::X_FIRST )
    {
        bpmdo->pu1Buffer.push_back(rawVectorContainer[ 5 ]);
        bpmdo->pu2Buffer.push_back(rawVectorContainer[ 6 ]);
        bpmdo->c1Buffer.push_back(rawVectorContainer[ 7 ]);
        bpmdo->p1Buffer.push_back(rawVectorContainer[ 8 ]);
        bpmdo->pu3Buffer.push_back(rawVectorContainer[ 1 ]);
        bpmdo->pu4Buffer.push_back(rawVectorContainer[ 2 ]);
        bpmdo->c2Buffer.push_back(rawVectorContainer[ 3 ]);
        bpmdo->p2Buffer.push_back(rawVectorContainer[ 4 ]);
    }

    double x = calcX( bpmdo->name, bpmdo->pu3Buffer.back(), bpmdo->pu4Buffer.back(), bpmdo->c2Buffer.back(), bpmdo->p2Buffer.back(), bpmdo->mn, bpmdo->xn );
    double y = calcY( bpmdo->name, bpmdo->pu1Buffer.back(), bpmdo->pu2Buffer.back(), bpmdo->c1Buffer.back(), bpmdo->p1Buffer.back(), bpmdo->mn, bpmdo->yn );
    double q = calcQ( bpmdo->name, bpmdo->rawBPMDataBuffer.back(), bpmdo->att1cal, bpmdo->att2cal, bpmdo->v1cal, bpmdo->v2cal, bpmdo->qcal );

    bpmdo->xBuffer.push_back(x);
    bpmdo->yBuffer.push_back(y);
    bpmdo->qBuffer.push_back(q);

    bpmdo->x = bpmdo->xBuffer.back();
    bpmdo->y = bpmdo->yBuffer.back();
    bpmdo->q = bpmdo->qBuffer.back();

    checkBPMStatus( bpmdo );
    if( bpmdo -> isATemporaryMonitorStruct )
    {
        if( bpmdo -> numShots > -1 )
        {
            ++bpmdo -> shotCount;
        }
        if( bpmdo->shotCount == bpmdo->numShots )
        {
//            bpmdo->xVec = bpmdo->xBuffer;
            message( "Collected ", bpmdo->shotCount, " shots for ", bpmdo -> pvRoot, ENUM_TO_STRING( ms->monType ) );
            setMonitorVectors( bpmdo-> name );
            ms->interface->killCallBack( ms, bpmdo );//, bpmdo );
            monitoringData = false;
        }
    }
    ++bpmdo->dataShots;
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateXValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;

//    beamPositionMonitorStructs::bpmDataObject * bpmdo = reinterpret_cast< beamPositionMonitorStructs::bpmDataObject* > (ms -> val);
    beamPositionMonitorStructs::bpmDataObject * bpmdo = &ms->interface->bpmObj.dataObjects.at( ms->objName );
    if( bpmdo->isAContinuousMonitorStruct )
    {
        bpmdo->shotCount = 0;
        if( bpmdo->timeStamps.size() == 0 )
        {
            bpmdo->timeStamps.push_back(1);
            bpmdo->strTimeStamps.push_back(UTL::UNKNOWN_STRING);
            bpmdo->xPVBuffer.push_back(UTL::DUMMY_DOUBLE);
            bpmdo->xPVBuffer.resize(bpmdo->buffer);
        }
    }

    const dbr_double_t * val = &(p  -> value);
    size_t i = 0;

    updateTime( p->stamp, bpmdo->timeStamps[ bpmdo->shotCount ],
               bpmdo->strTimeStamps[ bpmdo->shotCount ]  );

    bpmdo->xPV = *( &p -> value );
    bpmdo->xPVBuffer.push_back( bpmdo->xPV );
    ++bpmdo->xPVShots;
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateYValue( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_double * p = ( const struct dbr_time_double * ) args.dbr;

//    beamPositionMonitorStructs::bpmDataObject * bpmdo = reinterpret_cast< beamPositionMonitorStructs::bpmDataObject* > (ms -> val);
    beamPositionMonitorStructs::bpmDataObject * bpmdo = &ms->interface->bpmObj.dataObjects.at( ms->objName );
    if( bpmdo->isAContinuousMonitorStruct )
    {
        bpmdo->shotCount = 0;
        if( bpmdo->timeStamps.size() == 0 )
        {
            bpmdo->timeStamps.push_back(1);
            bpmdo->strTimeStamps.push_back(UTL::UNKNOWN_STRING);
            bpmdo->yPVBuffer.push_back(UTL::DUMMY_DOUBLE);
            bpmdo->yPVBuffer.resize(bpmdo->buffer);
        }
    }

    const dbr_double_t * val = &(p  -> value);
    size_t i = 0;

    updateTime( p->stamp, bpmdo->timeStamps[ bpmdo->shotCount ],
               bpmdo->strTimeStamps[ bpmdo->shotCount ]  );

    bpmdo->yPV = *( &p -> value );
    bpmdo->yPVBuffer.push_back( bpmdo->yPV );
    ++bpmdo->yPVShots;
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateDBRLong( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    const dbr_time_long * p = ( const struct dbr_time_long * ) args.dbr;

//    beamPositionMonitorStructs::bpmDataObject * bpmdo = reinterpret_cast< beamPositionMonitorStructs::bpmDataObject* > (ms -> val);
    beamPositionMonitorStructs::bpmDataObject * bpmdo = &ms->interface->bpmObj.dataObjects.at( ms->objName );

    const dbr_long_t * val = &(p  -> value);
    size_t i = 0;
    switch( ms->monType )
    {
        case beamPositionMonitorStructs::BPM_PV_TYPE::AWAK:
            updateTime( p->stamp, bpmdo->awakTStamp, bpmdo->awakStrTStamp );
            bpmdo->awakTStampBuffer.push_back(bpmdo->awakTStamp);
            bpmdo->awakCount++;
        case beamPositionMonitorStructs::BPM_PV_TYPE::RDY:
            updateTime( p->stamp, bpmdo->rdyTStamp, bpmdo->rdyStrTStamp );
            bpmdo->rdyCount++;
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::updateLong( beamPositionMonitorStructs::monitorStruct * ms, const event_handler_args args )
{
    *(long*)ms -> val = *(long*)args.dbr;
}
////______________________________________________________________________________
//void beamPositionMonitorInterface::updateLong1( std::string name, long args, beamPositionMonitorStructs::BPM_PV_TYPE pvType )
//{
//    bpmObj.dataObjects.at( name ).ra1 = args;
//}
//______________________________________________________________________________
void beamPositionMonitorInterface::killCallBack( beamPositionMonitorStructs::monitorStruct * ms, beamPositionMonitorStructs::bpmDataObject * bpmdo )///, beamPositionMonitorStructs::bpmDataObject * bpmdo )
{
    int status = ca_clear_subscription( ms -> EVID );
    if( status == ECA_NORMAL)
    {
//        debugMessage( ms -> scopeObject, " monitoring = false ");
        bpmdo -> isMonitoring = false;

        delete ms;
        message( bpmdo->name, " callback deleted" );
    }
    else
    {
        message("ERROR: in killCallBack: ca_clear_subscription failed for ", ms->bpmObject );
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::monitorDataForNShots( size_t N, const std::string & name  )
{
    if( !monitoringData )
    {
        dataMonitorStructs.clear();
//        debugMessage( "Starting bpm data Monitor " );
        resetDataVectors( N );
        bpmObj.dataObjects.at( name ).numShots = N;
//                if( isADataPV( bpmObj.dataObjects.at( name ).pvMonStructs.at( name ) ) )
//                {
        bpmObj.dataObjects.at( name ).appendingData=true;
        bpmObj.dataObjects.at( name ).isAContinuousMonitorStruct=false;
        bpmObj.dataObjects.at( name ).isATemporaryMonitorStruct=true;
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
        if( !bpmObj.dataObjects.at( it ).appendingData )
        {
            message( "Starting bpm data Monitor " );



            bpmObj.dataObjects.at( it ).numShots = N;
            bpmObj.dataObjects.at( it ).isAContinuousMonitorStruct=false;
            bpmObj.dataObjects.at( it ).isATemporaryMonitorStruct=true;
            bpmObj.dataObjects.at( it ).appendingData=true;
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
void beamPositionMonitorInterface::setMonitorVectors( const std::string name )
{
    doubleBufferToVector( bpmObj.dataObjects.at( name ).xBuffer, bpmObj.dataObjects.at( name ).xVec );
    doubleBufferToVector( bpmObj.dataObjects.at( name ).yBuffer, bpmObj.dataObjects.at( name ).yVec );
    doubleBufferToVector( bpmObj.dataObjects.at( name ).qBuffer, bpmObj.dataObjects.at( name ).qVec );
    doubleBufferToVector( bpmObj.dataObjects.at( name ).xPVBuffer, bpmObj.dataObjects.at( name ).xPVVec );
    doubleBufferToVector( bpmObj.dataObjects.at( name ).yPVBuffer, bpmObj.dataObjects.at( name ).yPVVec );
    statusBufferToVector( bpmObj.dataObjects.at( name ).statusBuffer, bpmObj.dataObjects.at( name ).statusVec );
}
//______________________________________________________________________________
void beamPositionMonitorInterface::checkBPMStatus( beamPositionMonitorStructs::bpmDataObject * bpmdo )
{
    if( bpmdo->awakTStampBuffer[bpmdo->awakTStampBuffer.size()] - bpmdo->timeStampsBuffer[bpmdo->timeStampsBuffer.size()] > 1 )
    {
        bpmdo -> status = beamPositionMonitorStructs::BPM_STATUS::BAD;
        bpmdo -> statusBuffer.push_back( bpmdo->status );
    }
    else if( checkBPMBuffer( bpmdo -> xBuffer ) || checkBPMBuffer( bpmdo -> yBuffer ) )
    {
        bpmdo -> status = beamPositionMonitorStructs::BPM_STATUS::BAD;
        bpmdo -> statusBuffer.push_back( bpmdo->status );
    }
    else if ( isnan( bpmdo -> xBuffer.back() ) || isnan( bpmdo-> yBuffer.back() ) )
    {
        bpmdo -> status = beamPositionMonitorStructs::BPM_STATUS::BAD;
        bpmdo -> statusBuffer.push_back( bpmdo->status );
    }
    else if( abs(bpmdo -> pu1Buffer.back()) > 1.0 || abs(bpmdo -> pu2Buffer.back()) > 1.0 || abs(bpmdo -> pu3Buffer.back()) > 1.0 || abs(bpmdo -> pu4Buffer.back()) > 1.0 )
    {
        bpmdo -> status = beamPositionMonitorStructs::BPM_STATUS::NONLINEAR;
//        message( bpmdo->name, " status is BAD, X or Y > 10!!!!!!!!!!!!!!!!!! " );
    }
    else if( abs(bpmdo -> pu1Buffer.back()) < 1.0 || abs(bpmdo -> pu2Buffer.back()) < 1.0 || abs(bpmdo -> pu3Buffer.back()) < 1.0 || abs(bpmdo -> pu4Buffer.back()) < 1.0 )
    {
        bpmdo -> status = beamPositionMonitorStructs::BPM_STATUS::GOOD;
        bpmdo -> statusBuffer.push_back( bpmdo->status );
    }
    else
    {
        bpmdo -> status = beamPositionMonitorStructs::BPM_STATUS::UNKNOWN;
        bpmdo -> statusBuffer.push_back( bpmdo->status );
    }
}
//______________________________________________________________________________
bool beamPositionMonitorInterface::checkBPMBuffer( boost::circular_buffer< double > buf )
{
        if( buf[ buf.size() - 1 ] == buf[ buf.size() ] )
        {
            return true;
        }
    return false;
}
//______________________________________________________________________________
void beamPositionMonitorInterface::doubleBufferToVector( boost::circular_buffer< double > buf, std::vector< double > vec )
{
    for( auto && it : buf )
    {
        vec.push_back(it);
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::statusBufferToVector( boost::circular_buffer< beamPositionMonitorStructs::BPM_STATUS > buf, std::vector< beamPositionMonitorStructs::BPM_STATUS > vec )
{
    for( auto && it : buf )
    {
        vec.push_back(it);
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::resetDataVectors( size_t N )
{
    dataMonitorStructs.clear();
    for( auto && it : bpmObj.dataObjects )
    {   // Clear everything
        it.second.rawBPMData.clear();
        it.second.timeStamps.clear();
        it.second.strTimeStamps.clear();
        it.second.p1.clear();
        it.second.p2.clear();
        it.second.pu1.clear();
        it.second.pu2.clear();
        it.second.pu3.clear();
        it.second.pu4.clear();
        it.second.c1.clear();
        it.second.c2.clear();
        it.second.xPVVec.clear();
        it.second.xVec.clear();
        it.second.yPVVec.clear();
        it.second.yVec.clear();
        it.second.qVec.clear();
        it.second.statusVec.clear();
        // Resize to N shots
        it.second.rawBPMData.resize( N );
        it.second.timeStamps.resize( N );
        it.second.strTimeStamps.resize( N );
        it.second.p1.resize( N );
        it.second.p2.resize( N );
        it.second.pu1.resize( N );
        it.second.pu2.resize( N );
        it.second.pu3.resize( N );
        it.second.pu4.resize( N );
        it.second.c1.resize( N );
        it.second.c2.resize( N );
        it.second.xPVVec.resize( N );
        it.second.xVec.resize( N );
        it.second.yPVVec.resize( N );
        it.second.qVec.resize( N );
        it.second.qVec.resize( N );
        it.second.statusVec.resize( N );
        // resize sub-vectors to COUNT elements
        for( auto && it2 : it.second.rawBPMData )
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
double beamPositionMonitorInterface::calcX( const std::string & name, double u11, double u12, double u13, double u14, double mn, double xn )
{
    double x;
    double v11, v12, num, den, t1;

    double u1114, u1214, u1314;
    u1114 = u11 - u14;
    u1214 = u12 - u14;
    u1314 = u13 - u14;

    t1 = ( ( u1214 + sqrt( pow( u1214 , 2 ) - ( 4 * u1114 * u1314 ) )  ) / ( 2 * u1114 ) );

    v11 = u1114;
    v12 = u1214 - ( t1 * u1114 );

    num = v12 - v11;
    den = v12 + v11;

    x = ( ( u1214 - u1114 ) / ( u1214 + u1114 ) * mn ) - xn;

    return x;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::calcY( const std::string & name, double u21, double u22, double u23, double u24, double mn, double yn )
{
    double y;
    double v21, v22, num, den, t2;

    double u2124, u2224, u2324;
    u2124 = u21 - u24;
    u2224 = u22 - u24;
    u2324 = u23 - u24;

    t2 = ( ( u2224 + sqrt( pow( u2224 , 2 ) - ( 4 * u2124 * u2324 ) )  ) / ( 2 * u2124 ) );

    v21 = u2124;
    v22 = u2224;// - ( t1 * u2124 );

    num = v22 - v21;
    den = v22 + v21;

    y = -1 * ( ( u2224 - u2124 ) / ( u2224 + u2124 ) * mn ) - yn;

    return y;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::calcQ( const std::string & bpmName, std::vector< double > rawData, double att1Cal, double att2Cal, double v1Cal, double v2Cal, double qCal )
{
    double q;

    double v1, v2, q1, q2;
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
//    v1Cal     = bpmObj.dataObjects.at( bpmName ).v1cal  ;
//    v2Cal     = bpmObj.dataObjects.at( bpmName ).v2cal  ;
//    att1Cal   = bpmObj.dataObjects.at( bpmName ).att1cal;
//    att2Cal   = bpmObj.dataObjects.at( bpmName ).att2cal;
//    qCal      = bpmObj.dataObjects.at( bpmName ).qcal   ;
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
    boost::circular_buffer< double > u11Buffer = bpmObj.dataObjects.at( bpmName ).pu1Buffer;
    boost::circular_buffer< double > u12Buffer = bpmObj.dataObjects.at( bpmName ).pu2Buffer;
    boost::circular_buffer< double > u13Buffer = bpmObj.dataObjects.at( bpmName ).c1Buffer;
    boost::circular_buffer< double > u14Buffer = bpmObj.dataObjects.at( bpmName ).p1Buffer;
    boost::circular_buffer< double > u21Buffer = bpmObj.dataObjects.at( bpmName ).pu3Buffer;
    boost::circular_buffer< double > u22Buffer = bpmObj.dataObjects.at( bpmName ).pu4Buffer;
    boost::circular_buffer< double > u23Buffer = bpmObj.dataObjects.at( bpmName ).c2Buffer;
    boost::circular_buffer< double > u24Buffer = bpmObj.dataObjects.at( bpmName ).p2Buffer;
    double rmsVals;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
    {
        u11 = std::accumulate( u11Buffer.begin(), u11Buffer.end(), 0.0)/u11Buffer.size();
        u12 = std::accumulate( u12Buffer.begin(), u12Buffer.end(), 0.0)/u12Buffer.size();
        u13 = std::accumulate( u13Buffer.begin(), u13Buffer.end(), 0.0)/u13Buffer.size();
        u14 = std::accumulate( u14Buffer.begin(), u14Buffer.end(), 0.0)/u14Buffer.size();
        u21 = std::accumulate( u21Buffer.begin(), u21Buffer.end(), 0.0)/u21Buffer.size();
        u22 = std::accumulate( u22Buffer.begin(), u22Buffer.end(), 0.0)/u22Buffer.size();
        u23 = std::accumulate( u23Buffer.begin(), u23Buffer.end(), 0.0)/u23Buffer.size();
        u24 = std::accumulate( u24Buffer.begin(), u24Buffer.end(), 0.0)/u24Buffer.size();
        v11 = u11 - u14;
        v12 = u12 - u14;
        v21 = u21 - u24;
        v22 = u22 - u24;
        if( v11 && v12 && v21 && v22 != 0 )
        {
            rmsVals = ( ( v11 + v12 ) - ( v21 + v22 ) ) / ( ( v11 + v12 ) + ( v21 + v22 ) );
        }
        message(rmsVals);
        res = rmsVals * sqrt( 2 ) * ( 0.001 * bpmObj.dataObjects.at( bpmName ).mn );
        return res;
    }
    else
    {
        message( "BPM resolution calculation didn't work" );
        return UTL::DUMMY_DOUBLE;
    }
}
//______________________________________________________________________________
const bool beamPositionMonitorInterface::isDataBufferFull( const std::string & bpmName )
{
    bool isFull = false;
    if( bpmObj.dataObjects.at( bpmName ).dataShots > bpmObj.dataObjects.at( bpmName ).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool beamPositionMonitorInterface::isXBufferFull( const std::string & bpmName )
{
    bool isFull = false;
    if( bpmObj.dataObjects.at( bpmName ).xPVShots > bpmObj.dataObjects.at( bpmName ).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool beamPositionMonitorInterface::isYBufferFull( const std::string & bpmName )
{
    bool isFull = false;
    if( bpmObj.dataObjects.at( bpmName ).yPVShots > bpmObj.dataObjects.at( bpmName ).buffer )
    {
        isFull = true;
    }
    return isFull;
}
//______________________________________________________________________________
const bool beamPositionMonitorInterface::isDataBufferNotFull( const std::string & bpmName )
{
    return !isDataBufferFull(bpmName);
}
//______________________________________________________________________________
const bool beamPositionMonitorInterface::isXBufferNotFull( const std::string & bpmName )
{
    return !isXBufferFull(bpmName);
}
//______________________________________________________________________________
const bool beamPositionMonitorInterface::isYBufferNotFull( const std::string & bpmName )
{
    return !isYBufferFull(bpmName);
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
void beamPositionMonitorInterface::setBufferSize( size_t bufferSize )
{
    for( auto && it : bpmObj.dataObjects )
    {
        clearBuffers();
        it.second.xBuffer.resize( bufferSize );
        it.second.yBuffer.resize( bufferSize );
        it.second.xPVBuffer.resize( bufferSize );
        it.second.yPVBuffer.resize( bufferSize );
        it.second.qBuffer.resize( bufferSize );
        it.second.p1Buffer.resize( bufferSize );
        it.second.p2Buffer.resize( bufferSize );
        it.second.pu1Buffer.resize( bufferSize );
        it.second.pu2Buffer.resize( bufferSize );
        it.second.pu3Buffer.resize( bufferSize );
        it.second.pu4Buffer.resize( bufferSize );
        it.second.c1Buffer.resize( bufferSize );
        it.second.c2Buffer.resize( bufferSize );
        it.second.timeStampsBuffer.resize( bufferSize );
        it.second.rawBPMDataBuffer.resize( bufferSize );
        it.second.statusBuffer.resize( bufferSize );
        for( auto && it2 : it.second.rawBPMDataBuffer )
        {
            it2.resize( it.second.pvMonStructs[ beamPositionMonitorStructs::BPM_PV_TYPE::DATA ].COUNT );
        }
        it.second.buffer = bufferSize;
        it.second.dataShots = 0;
        it.second.xPVShots = 0;
        it.second.yPVShots = 0;
    }
}
//______________________________________________________________________________
const size_t beamPositionMonitorInterface::getBufferSize()
{
    for( auto && it : bpmObj.dataObjects )
    {
        return it.second.buffer;
    }
}
//______________________________________________________________________________
void beamPositionMonitorInterface::clearBuffers()
{
    for( auto && it : bpmObj.dataObjects )
    {
        it.second.xPVBuffer.clear();
        it.second.yPVBuffer.clear();
        it.second.xBuffer.clear();
        it.second.yBuffer.clear();
        it.second.qBuffer.clear();
        it.second.pu1Buffer.clear();
        it.second.pu2Buffer.clear();
        it.second.pu3Buffer.clear();
        it.second.pu4Buffer.clear();
        it.second.p1Buffer.clear();
        it.second.p2Buffer.clear();
        it.second.c1Buffer.clear();
        it.second.c2Buffer.clear();
        it.second.timeStampsBuffer.clear();
        it.second.rawBPMDataBuffer.clear();
        it.second.statusBuffer.clear();
        it.second.dataShots = 0;
        it.second.xPVShots = 0;
        it.second.yPVShots = 0;
    }
}
//______________________________________________________________________________
std::vector< std::vector< double > > beamPositionMonitorInterface::getBPMRawData( const std::string & bpmName )
{
    std::vector< std::vector< double > > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).rawBPMData.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).rawBPMData;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getBPMXVec( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).xVec.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).xVec;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getBPMYVec( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).yVec.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).yVec;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getBPMQVec( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).qVec.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).qVec;
    else
        return r;
}
//______________________________________________________________________________
std::vector< double > beamPositionMonitorInterface::getTimeStamps( const std::string & bpmName )
{
    std::vector< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).timeStamps.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).timeStamps;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< std::vector< double > > beamPositionMonitorInterface::getBPMRawDataBuffer( const std::string & bpmName )
{
    boost::circular_buffer< std::vector< double > > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).rawBPMDataBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).rawBPMDataBuffer;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorInterface::getBPMXBuffer( const std::string & bpmName )
{
    boost::circular_buffer< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).xBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).xBuffer;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorInterface::getBPMYBuffer( const std::string & bpmName )
{
    boost::circular_buffer< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).yBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).yBuffer;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorInterface::getBPMXPVBuffer( const std::string & bpmName )
{
    boost::circular_buffer< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).xPVBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).xPVBuffer;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorInterface::getBPMYPVBuffer( const std::string & bpmName )
{
    boost::circular_buffer< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).yPVBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).yPVBuffer;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorInterface::getBPMQBuffer( const std::string & bpmName )
{
    boost::circular_buffer< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).qBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).qBuffer;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< double > beamPositionMonitorInterface::getTimeStampsBuffer( const std::string & bpmName )
{
    boost::circular_buffer< double > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).timeStampsBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).timeStampsBuffer;
    else
        return r;
}
//______________________________________________________________________________
std::vector< std::string > beamPositionMonitorInterface::getStrTimeStamps( const std::string & bpmName )
{
    std::vector< std::string > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).strTimeStamps.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).strTimeStamps;
    else
        return r;
}
//______________________________________________________________________________
boost::circular_buffer< beamPositionMonitorStructs::BPM_STATUS > beamPositionMonitorInterface::getStatusBuffer( const std::string & bpmName )
{
    boost::circular_buffer< beamPositionMonitorStructs::BPM_STATUS > r;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).statusBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).statusBuffer;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getRA1( const std::string & bpmName )
{
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
long beamPositionMonitorInterface::getSA1( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).sa1;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getSA2( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).sa2;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getSD1( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).sd1;
    else
        return r;
}
//______________________________________________________________________________
long beamPositionMonitorInterface::getSD2( const std::string & bpmName )
{
    long r = UTL::DUMMY_LONG;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).sd2;
    else
        return r;
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getX( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).xBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).x;
    else
    {
        message("WARNING!!! x buffer is empty, are BPM values updating??");
        return r;
    }
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getY( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).yBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).y;
    else
    {
        message("WARNING!!! y buffer is empty, are BPM values updating??");
        return r;
    }
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getQ( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) && bpmObj.dataObjects.at( bpmName ).qBuffer.size() != 0 )
        return bpmObj.dataObjects.at( bpmName ).q;
    else
    {
        message("WARNING!!! q buffer is empty, are BPM values updating??");
        return r;
    }
}
//______________________________________________________________________________
double beamPositionMonitorInterface::getPosition( const std::string & bpmName )
{
    double r = UTL::DUMMY_DOUBLE;
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).position;
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
const beamPositionMonitorStructs::BPM_STATUS beamPositionMonitorInterface::getBPMStatus( const std::string & bpmName )
{
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return bpmObj.dataObjects.at( bpmName ).status;
    else
    {
        message("ERROR!!!!! BPM ", bpmName, " not defined in config file!!!!");
        return beamPositionMonitorStructs::BPM_STATUS::UNKNOWN;
    }
}
//______________________________________________________________________________
const std::string beamPositionMonitorInterface::getBPMStatusStr( const std::string & bpmName )
{
    if( entryExists( bpmObj.dataObjects, bpmName ) )
        return ENUM_TO_STRING( bpmObj.dataObjects.at( bpmName ).status );
    else
    {
        message("ERROR!!!!! BPM ", bpmName, " not defined in config file!!!!");
        return ENUM_TO_STRING( beamPositionMonitorStructs::BPM_STATUS::UNKNOWN );
    }
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
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > beamPositionMonitorInterface::getILockStates( const std::string & objName )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > r;
    auto iter = allBPMData.find( objName );
    if( iter != allBPMData.end() )
        r = iter -> second.iLockStates;
    return r;
}
//______________________________________________________________________________
std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  beamPositionMonitorInterface::getILockStatesStr( const std::string & name )const
{
    std::map< HWC_ENUM::ILOCK_NUMBER, std::string  > r;
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
