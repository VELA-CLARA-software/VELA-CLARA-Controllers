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

#include "llrfInterface.h"
//djs
#include "dburt.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>
#include <math.h>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
llrfInterface::llrfInterface( const std::string &llrfConf,
                              const bool startVirtualMachine,
                              const bool* show_messages_ptr, const bool* show_debug_messages_ptr,
                              const bool shouldStartEPICs ,
                              const llrfStructs::LLRF_TYPE type
                              ):
configReader(llrfConf,startVirtualMachine,show_messages_ptr,show_debug_messages_ptr),
interface(show_messages_ptr,show_debug_messages_ptr),
shouldStartEPICs(shouldStartEPICs),
usingVirtualMachine(startVirtualMachine),
myLLRFType(type)
{
//    if( shouldStartEPICs )
//    message("magnet llrfInterface shouldStartEPICs is true");
//    else
//    message("magnet llrfInterface shouldStartEPICs is false");
    initialise();
}
//______________________________________________________________________________
llrfInterface::~llrfInterface()
{
    killILockMonitors();
    for( auto && it : continuousMonitorStructs )
    {
        killMonitor( it );
        delete it;
    }
//    debugMessage( "llrfInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void llrfInterface::killMonitor( llrfStructs::monitorStruct * ms )
{
    int status = ca_clear_subscription( ms -> EVID );
    //if( status == ECA_NORMAL)
        //debugMessage( ms->objName, " ", ENUM_TO_STRING(ms->monType), " monitoring = false ");
//    else
        //debugMessage("ERROR llrfInterface: in killMonitor: ca_clear_subscription failed for ", ms->objName, " ", ENUM_TO_STRING(ms->monType) );
}
//______________________________________________________________________________
void llrfInterface::initialise()
{
    /// The config file reader
    configFileRead = configReader.readConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
    if( configFileRead )
    {
        message("The llrfInterface has read the config file, acquiring objects");
        /// initialise the objects based on what is read from the config file
        bool getDataSuccess = initObjects();
        if( getDataSuccess )
        {

            if( shouldStartEPICs )
            {
                message("The llrfInterface has acquired objects, connecting to EPICS");
                //std::cout << "WE ARE HERE" << std::endl;
                // subscribe to the channel ids
                initChids();
                // start the monitors: set up the callback functions
                debugMessage("Startign Monitors");
                startMonitors();
                // The pause allows EPICS to catch up.
                std::this_thread::sleep_for(std::chrono::milliseconds( 2000 )); // MAGIC_NUMBER
            }
            else
             message("The llrfInterface has acquired objects, NOT connecting to EPICS");
        }
        else
            message( "!!!The llrfInterface received an Error while getting llrf data!!!" );
    }
}
//______________________________________________________________________________
bool llrfInterface::initObjects()
{
    bool success = configReader.getllrfObject(llrf);
    llrf.type = myLLRFType;
    return success;
}
//______________________________________________________________________________
void llrfInterface::initChids()
{
    message( "\n", "Searching for LLRF chids...");
    for( auto && it : llrf.pvMonStructs )
    {
        addChannel( llrf.pvRoot, it.second );
    }
    // command only PVs for the LLRF to set "high level" phase and amplitide
    for( auto && it : llrf.pvComStructs )
    {
        addChannel( llrf.pvRoot, it.second );
    }
    addILockChannels( llrf.numIlocks, llrf.pvRoot, llrf.name, llrf.iLockPVStructs );
    int status=sendToEpics("ca_create_channel","Found LLRF ChIds.",
                           "!!TIMEOUT!! Not all LLRF ChIds found." );
    if(status==ECA_TIMEOUT)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//MAGIC_NUMBER
        message("\n","Checking LLRF ChIds ");
        for( auto && it : llrf.pvMonStructs )
        {
            checkCHIDState( it.second.CHID, ENUM_TO_STRING( it.first ) );
        }
        for( auto && it : llrf.pvComStructs)
        {
            checkCHIDState( it.second.CHID, ENUM_TO_STRING( it.first ) );
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // MAGIC_NUMBER
    }
    else if ( status == ECA_NORMAL )
        allChidsInitialised = true;  // interface base class member
}
//______________________________________________________________________________
void llrfInterface::addChannel( const std::string & pvRoot, llrfStructs::pvStruct & pv )
{
    std::string s1;
    // TEMPORARY HACK FOR HIGH LEVEL llrf PARAMATERS
    if( pv.pvType == llrfStructs::LLRF_PV_TYPE::AMP_MVM)
    {
        s1 =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else if(  pv.pvType == llrfStructs::LLRF_PV_TYPE::PHI_DEG )
    {
        s1 =  UTL::VM_PREFIX + pv.pvSuffix;
    }
    else
    {
        s1 = pvRoot + pv.pvSuffix;
    }
    ca_create_channel( s1.c_str(), 0, 0, 0, &pv.CHID );//MAGIC_NUMBER
    debugMessage( "Create channel to ", s1 );
}
//______________________________________________________________________________
void llrfInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for( auto && it : llrf.pvMonStructs )
    {   // if using the VM we don't monito AMP Read
//        if( usingVirtualMachine && it.first == llrfStructs::LLRF_PV_TYPE::AMP_R )
//        {
//            message("For VM AMP_R is not monitored");
//        }
//        else
//        {
            debugMessage("ca_create_subscription to ", ENUM_TO_STRING(it.first));
            continuousMonitorStructs.push_back( new llrfStructs::monitorStruct() );
            continuousMonitorStructs.back() -> monType    = it.first;
            continuousMonitorStructs.back() -> llrfObj = &llrf;
            continuousMonitorStructs.back() -> interface  = this;
            ca_create_subscription(it.second.CHTYPE, it.second.COUNT,  it.second.CHID,
                                   it.second.MASK, llrfInterface::staticEntryLLRFMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back() -> EVID);
//        }
    }
    int status = sendToEpics( "ca_create_subscription", "Succesfully Subscribed to LLRF Monitors", "!!TIMEOUT!! Subscription to LLRF monitors failed" );
    if ( status == ECA_NORMAL )
        allMonitorsStarted = true; /// interface base class member
}
//____________________________________________________________________________________________
void llrfInterface::staticEntryLLRFMonitor(const event_handler_args args)
{
    llrfStructs::monitorStruct*ms = static_cast<  llrfStructs::monitorStruct *>(args.usr);
    switch(ms -> monType)
    {
        case llrfStructs::LLRF_PV_TYPE::AMP_R:
            ms->interface->debugMessage("staticEntryLLRFMonitor LLRF AMP_R = ",*(long*)args.dbr);
            ms->llrfObj->ampR =  *(long*)args.dbr;
            ms->interface->setAMPMVM();
            break;
        case llrfStructs::LLRF_PV_TYPE::AMP_W:
            ms->interface->debugMessage("staticEntryLLRFMonitor LLRF AMP_W = ",*(long*)args.dbr);
            ms->llrfObj->ampW =  *(long*)args.dbr;
            // the VM doesn't update the AMP read, 23-02-2017
            if( ms->interface->usingVirtualMachine)
            {
               ms->interface->debugMessage("Using Virtual Machine, so setting AMP_R");
               ms->interface->setAmpRead();
            }
            break;
        case llrfStructs::LLRF_PV_TYPE::PHI:
            ms->interface->debugMessage("staticEntryLLRFMonitor LLRF PHI = ",*(long*)args.dbr);
            ms->llrfObj->phiLLRF =  *(long*)args.dbr;
            ms->interface->setPHIDEG();
            break;
        case llrfStructs::LLRF_PV_TYPE::PHI_DEG:
            ms->interface->debugMessage("staticEntryLLRFMonitor LLRF PHI_DEG = ",*(double*)args.dbr);
            ms->llrfObj->phi_DEG = *(double*)args.dbr;
            break;
        case llrfStructs::LLRF_PV_TYPE::AMP_MVM:
            ms->interface->debugMessage("staticEntryLLRFMonitor LLRF AMP_MVM = ",*(double*)args.dbr);
            ms->llrfObj->amp_MVM = *(double*)args.dbr;
            break;
        default:
            ;
            // ERROR
    }
}
//____________________________________________________________________________________________
double llrfInterface::getPhiCalibration()
{
    return llrf.phiCalibration;
}
//____________________________________________________________________________________________
double llrfInterface::getAmpCalibration()
{
    return llrf.ampCalibration;
}
//____________________________________________________________________________________________
double llrfInterface::getCrestPhiLLRF()// in LLRF units
{
    return llrf.crestPhi;
}
//____________________________________________________________________________________________
long llrfInterface::getAmpWrite()
{
    return llrf.ampW;
}
//____________________________________________________________________________________________
long llrfInterface::getAmpRead()
{
    return llrf.ampR;
}
//____________________________________________________________________________________________
double llrfInterface::getAmp()// physics units
{
    return llrf.amp_MVM;
}
//____________________________________________________________________________________________
long llrfInterface::getPhiLLRF()
{
    return llrf.phiLLRF;
}
//____________________________________________________________________________________________
double llrfInterface::getPhi()// physics units
{
    return llrf.phi_DEG;
}
//____________________________________________________________________________________________
bool llrfInterface::setPhiLLRF(long value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::PHI),value);
}
//____________________________________________________________________________________________
bool llrfInterface::setAmpLLLRF(long value)
{
    return setValue(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::AMP_W),value);
}
//____________________________________________________________________________________________
bool llrfInterface::setAmp(double value)// MV / m amplitude
{
    bool r = false;
    if(value<UTL::ZERO_DOUBLE)
    {
        message("Error!! you must set a positive amplitude for LLRF, not ", value);
    }
    else
    {
        long val = (long)std::round(value/llrf.ampCalibration);

        if( val > llrf.maxAmp )
        {
            message("Error!! Requested amplitude, ",val,"  too high");
        }
        else
        {
            debugMessage("Requested amplitude, ", value," MV / m = ", val," in LLRF units ");

            r = setAmpLLLRF(val);
        }
    }
    return r;
}
//____________________________________________________________________________________________
bool llrfInterface::setPhi(double value)// degrees relative to crest
{
    bool r = false;
    if(value<-180.0||value>180.0)//MAGIC_NUMBER
    {
        message("Error!! you must set phase between -180.0 and +180.0, not ", value);
    }
    else
    {
        long val = llrf.crestPhi + (long)std::round(value/llrf.phiCalibration);

        debugMessage("Requested PHI, ", value," degrees  = ", val," in LLRF units ");
        r = setPhiLLRF(val);
    }
    return r;
}
//____________________________________________________________________________________________
bool llrfInterface::setPHIDEG()
{// ONLY ever called from staticEntryLLRFMonitor
    double val = ( (double)llrf.phiLLRF) * (llrf.phiCalibration);
    debugMessage("setPHIDEG PHI_DEG to, ",val, ", calibration = ", llrf.phiCalibration);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::PHI_DEG),val);
}
//____________________________________________________________________________________________
bool llrfInterface::setAMPMVM()
{// ONLY ever called from staticEntryLLRFMonitor
    double val = ((double)llrf.ampR) * (llrf.ampCalibration);
    debugMessage("setAMPMVM AMP_MVM to, ",val, ", calibration = ", llrf.ampCalibration);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::AMP_MVM),val);
}
//____________________________________________________________________________________________
bool llrfInterface::setAmpRead()
{// ONLY ever called from staticEntryLLRFMonitor
    debugMessage("setAmpRead AMP_R to, ",llrf.ampW);
    return setValue2(llrf.pvMonStructs.at(llrfStructs::LLRF_PV_TYPE::AMP_R),llrf.ampW);
}
//____________________________________________________________________________________________
const llrfStructs::llrfObject& llrfInterface::getLLRFObjConstRef()
{
    return llrf;
}
//____________________________________________________________________________________________
template<typename T>
bool llrfInterface::setValue( llrfStructs::pvStruct& pvs, T value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE,pvs.CHID,&value);
    std::stringstream ss;
    ss << "setValue setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    message(ss);
    ss.str("");
    ss << "Timeout setting llrf, " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    int status = sendToEpics("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}
//____________________________________________________________________________________________
template<typename T>
bool llrfInterface::setValue2( llrfStructs::pvStruct& pvs, T value)
{
    bool ret = false;
    ca_put(pvs.CHTYPE,pvs.CHID,&value);
    std::stringstream ss;
    ss << "setValue2 setting " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    message(ss);
    ss.str("");
    ss << "Timeout setting llrf, " << ENUM_TO_STRING(pvs.pvType) << " value to " << value;
    int status = sendToEpics2("ca_put","",ss.str().c_str());
    if(status==ECA_NORMAL)
        ret=true;
    return ret;
}














