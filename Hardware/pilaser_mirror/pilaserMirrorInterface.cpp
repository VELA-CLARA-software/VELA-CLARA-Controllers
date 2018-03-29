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

#include "pilaserMirrorInterface.h"
//djs
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
pilaserMirrorInterface::pilaserMirrorInterface( const std::string &pilaserConf,
                                const bool startVirtualMachine,
                                const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                                const bool shouldStartEPICs ):
configReader(pilaserConf,startVirtualMachine, show_messages_ptr, show_debug_messages_ptr ),
interface(show_messages_ptr,show_debug_messages_ptr)
//shouldStartEPICs( shouldStartEPICs )
{
//    if( shouldStartEPICs )
//    message("magnet pilaserMirrorInterface shouldStartEPICs is true");
//    else
//    message("magnet pilaserMirrorInterface shouldStartEPICs is false");
//    initialise();
}
//______________________________________________________________________________
pilaserMirrorInterface::~pilaserMirrorInterface()
{
//    killILockMonitors();
//    for( auto && it : continuousMonitorStructs )
//    {
//        killMonitor( it );
//        delete it;
//    }
//    debugMessage( "pilaserMirrorInterface DESTRUCTOR COMPLETE ");
}
////______________________________________________________________________________
//void pilaserMirrorInterface::killMonitor( pilaserStructs::monitorStruct * ms )
//{
//    int status = ca_clear_subscription( ms -> EVID );
//}
////______________________________________________________________________________
//void pilaserMirrorInterface::initialise()
//{
//    /// The config file reader
//    configFileRead = configReader.readConfig();
//    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
//    if( configFileRead )
//    {
//        message("The pilaserMirrorInterface has read the config file, acquiring objects");
//        /// initialise the objects based on what is read from the config file
//        bool getDataSuccess = initObjects();
//        if( getDataSuccess )
//        {
//            if( shouldStartEPICs )
//            {
//                message("The pilaserMirrorInterface has acquired objects, connecting to EPICS");
//                //std::cout << "WE ARE HERE" << std::endl;
//                /// subscribe to the channel ids
//                initChids();
//                /// start the monitors: set up the callback functions
//                startMonitors();
//                /// The pause allows EPICS to catch up.
//                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
//            }
//            else
//             message("The pilaserMirrorInterface has acquired objects, NOT connecting to EPICS");
//        }
//        else
//            message( "!!!The pilaserMirrorInterface received an Error while getting laser data!!!" );
//    }
//}
////______________________________________________________________________________
//bool pilaserMirrorInterface::initObjects()
//{
//    bool ans = configReader.getpilaserObject(pilaser);
//    debugMessage( "pilaser.pvComStructs.size() = ", pilaser.pvComStructs.size() );
//    debugMessage( "pilaser.pvMonStructs.size() = ", pilaser.pvMonStructs.size() );
//    return ans;
//}
////______________________________________________________________________________
//void pilaserMirrorInterface::initChids()
//{
//    message( "\n", "Searching for PILaser chids...");
//
//    for( auto && it : pilaser.pvMonStructs )
//    {
//        addChannel( pilaser.pvRoot, it.second );
//    }
//    // currently there are no command only PVs for the PIL
//    for( auto && it : pilaser.pvComStructs )
//    {
//        addChannel( pilaser.pvRoot, it.second );
//    }
//    addILockChannels( pilaser.numIlocks, pilaser.pvRoot, pilaser.name, pilaser.iLockPVStructs );
//    int status = sendToEpics( "ca_create_channel", "Found PILaser ChIds.", "!!TIMEOUT!! Not all PILaser ChIds found." );
//    if( status == ECA_TIMEOUT )
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds( 500 ));//MAGIC_NUMBER
//        message("\n", "Checking PILaser ChIds ");
//        for( auto && it : pilaser.pvMonStructs )
//        {
//            checkCHIDState( it.second.CHID, ENUM_TO_STRING( it.first ) );
//        }
//        for( auto && it : pilaser.pvComStructs)
//        {
//            checkCHIDState( it.second.CHID, ENUM_TO_STRING( it.first ) );
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds( 5000 )); // MAGIC_NUMBER
//    }
//    else if ( status == ECA_NORMAL )
//        allChidsInitialised = true;  /// interface base class member
//}
////______________________________________________________________________________
//void pilaserMirrorInterface::addChannel( const std::string & pvRoot, pilaserStructs::pvStruct & pv )
//{
//    std::string s1 = pvRoot + pv.pvSuffix;
//    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );//MAGIC_NUMBER
//    debugMessage( "Create channel to ", s1 );
//}
////______________________________________________________________________________
//void pilaserMirrorInterface::startMonitors()
//{
//    continuousMonitorStructs.clear();
//    continuousILockMonitorStructs.clear();
//
//    for( auto && it : pilaser.pvMonStructs )
//    {
//        continuousMonitorStructs.push_back( new pilaserStructs::monitorStruct() );
//        continuousMonitorStructs.back() -> monType    = it.first;
//        continuousMonitorStructs.back() -> pilaserObj = &pilaser;
//        continuousMonitorStructs.back() -> interface  = this;
//        ca_create_subscription(it.second.CHTYPE,
//                               it.second.COUNT,
//                               it.second.CHID,
//                               it.second.MASK,
//                               pilaserMirrorInterface::staticEntryPILMonitor,
//                               (void*)continuousMonitorStructs.back(),
//                               &continuousMonitorStructs.back() -> EVID);
//    }
//    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to PILaser Monitors", "!!TIMEOUT!! Subscription to PILaser monitors failed" );
//    if ( status == ECA_NORMAL )
//        allMonitorsStarted = true; /// interface base class member
//}
////____________________________________________________________________________________________
//void pilaserMirrorInterface::staticEntryPILMonitor(const event_handler_args args)
//{
//    pilaserStructs::monitorStruct*ms = static_cast<pilaserStructs::monitorStruct *>(args.usr);
//    switch(ms -> monType)
//    {
//        case pilaserStructs::PILASER_PV_TYPE::H_POS:
//            //ms->interface->debugMessage("PILaser H_pos = ",*(double*)args.dbr);
//            ms->pilaserObj->hPos =  *(double*)args.dbr;
//            break;
//        case pilaserStructs::PILASER_PV_TYPE::V_POS:
//            //ms->interface->debugMessage("PILaser V_POS = ",*(double*)args.dbr);
//            ms->pilaserObj->vPos =  *(double*)args.dbr;
//            break;
//        case pilaserStructs::PILASER_PV_TYPE::INTENSITY:
//            //ms->interface->debugMessage("PILaser intensity = ",*(double*)args.dbr);
//            ms->pilaserObj->intensity =  *(double*)args.dbr;
//            break;
//        default:
//            ms->interface->message("!!! ERROR !!! Unknown Monitor Type passed to pilaserMirrorInterface::staticEntryPILMonitor");
//            break;
//    }
//}
////____________________________________________________________________________________________
//double pilaserMirrorInterface::getHpos()
//{
//    return pilaser.hPos;
//}
////____________________________________________________________________________________________
//double pilaserMirrorInterface::getVpos()
//{
//    return pilaser.vPos;
//}
////____________________________________________________________________________________________
//double pilaserMirrorInterface::getIntensity()
//{
//    return pilaser.intensity;
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setHpos(double value)
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_POS),value);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setHpos(int value)
//{
//    return setHpos((double)value);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setVpos(double value)
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_POS),value);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setVpos(int value)
//{
//    return setVpos((double)value);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setIntensity(double value)
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::INTENSITY),value);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setIntensity(int value)
//{
//    return setIntensity((double)value);
//}
////____________________________________________________________________________________________
//const pilaserStructs::pilaserObject& pilaserMirrorInterface::getPILObjConstRef()
//{
//    return pilaser;
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::setValue( pilaserStructs::pvStruct& pvs, double value)
//{
//    bool ret = false;
//    ca_put(pvs.CHTYPE,pvs.CHID,&value);
//    std::stringstream ss;
//    ss << "Timeout setting pilaser," << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
//    int status = sendToEpics("ca_put","",ss.str().c_str());
//    if(status==ECA_NORMAL)
//        ret=true;
//    return ret;
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::openShutter()
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::ON),1);
//}
////____________________________________________________________________________________________
//bool pilaserMirrorInterface::closeShutter()
//{
//    return setValue(pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::OFF),1);
//}
////____________________________________________________________________________________________
//



