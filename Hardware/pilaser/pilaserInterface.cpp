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
//  Author:      DJS
//  Last edit:   29-03-2018
//  FileName:    pilaserInterface.cpp
//  Description:
//
//
//*/
#include "pilaserInterface.h"
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
//______________________________________________________________________________
pilaserInterface::pilaserInterface(bool& show_messages,
                                   bool& show_debug_messages,
                                   const bool startVirtualMachine,
                                   const bool shouldStartEPICs,
                                   const std::string& pilaserConf,
                                   const std::string& claraCamConfig,
                                   const std::string& piLaserMirrorConf,
                                   const std::string& velaCamConfig,
                                   const HWC_ENUM::MACHINE_AREA area
                                  ):
configReader(pilaserConf,  show_messages, show_debug_messages,startVirtualMachine),
ltMirrorConfigReader(piLaserMirrorConf, show_messages, show_debug_messages,startVirtualMachine),
cameraBase(show_messages,show_debug_messages,startVirtualMachine,shouldStartEPICs,claraCamConfig,velaCamConfig,area)
{
    initialise();
    attachTo_thisCAContext();
    message("pilaserInterface, ",thisCaContext);
}
//______________________________________________________________________________
pilaserInterface::~pilaserInterface()
{
    debugMessage("pilaserInterface DESTRUCTOR");
    killILockMonitors();
    for(auto&& it:continuousMonitorStructs)
    {
        if(it)
        {
            killMonitor(it);
            delete it;
        }
    }
    debugMessage("pilaserInterface DESTRUCTOR COMPLETE ");
}
//______________________________________________________________________________
void pilaserInterface::killMonitor(pilaserStructs::monitorStruct* ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
        debugMessage(ms->object->name, " ",
                     ENUM_TO_STRING(ms->monType), " monitoring = false ");
    else
        debugMessage("ERROR pilaserInterface: in killMonitor: "
                     "ca_clear_subscription failed for ",
                     ms->object->name, " ",
                     ENUM_TO_STRING(ms->monType));
}
//______________________________________________________________________________
void pilaserInterface::initialise()
{
    std::cout <<"pilaserInterface::initialise()" <<std::endl;
    /* The config file reader */
    configFileRead = configReader.readConfig();

    bool cam_config_read = readCamConfig();
    bool mirror_con_read = ltMirrorConfigReader.readConfig();

    if( cam_config_read && configFileRead && mirror_con_read)
    {
        configFileRead = true;
    }
    else
    {
        configFileRead = false;
    }

//    if(configFileRead)
//        configFileRead = vcConfigReader.readConfig();

    if(configFileRead)
    {
        message("The pilaserInterface has read the config file, "
                "acquiring objects");
        /*initialise the objects based on what is read from the config file */
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The pilaserInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                startMonitors();
                /* The pause allows EPICS callbacks to catch up. */
                pause_2000();
                /* set defaults */
                if(setDefaults())
                {

                }
                else
                {
                    message("The pilaserInterface FAILED TO SET DEFAULT VALUES");
                }
            }
            else
             message("The pilaserInterface has acquired objects, "
                     "NOT connecting to EPICS");
        }
        else
            message("!!!The pilaserInterface received an Error "
                    "while getting laser data!!!");
    }
}
//______________________________________________________________________________
bool pilaserInterface::setDefaults()
{
//    cameraStructs::cameraObject& vc = getCamObj(UTL::VIRTUAL_CATHODE);
//    if(vc.name != UTL::UNKNOWN_NAME)
//    {
//        if(entryExists(vc.pvComStructs,cameraStructs::CAM_PV_TYPE::PIX_MM))
//        {
//            message(vc.data.analysis.pix_2_mm_def);
//
//            cameraStructs::pvStruct& pvs = vc.pvComStructs.at(cameraStructs::CAM_PV_TYPE::PIX_MM);
//            return move(pvs.CHTYPE,pvs.CHID, vc.data.analysis.pix_2_mm_def,
//                  "",
//                  "!!TIMEOUT!! FAILED TO SET DEFAULT PIX-2-MM");
//
//        }
//    }
    return true;
}
//______________________________________________________________________________
bool pilaserInterface::initObjects()
{
    bool gotobj = configReader.getpilaserObject(pilaser);
    bool gotMirobj = ltMirrorConfigReader.getMirrorObject(pilaser);
    bool gotCamobj = getCamObjects();
    if(gotCamobj)
    {
        gotCamobj  = vcOnly();
    }
    if(gotobj && gotCamobj && gotMirobj)
    {
        message("Successfully got objects");
        return true;
    }
    return false;
}
//______________________________________________________________________________
void pilaserInterface::initChids()
{
    message("\n", "Searching for PILaser/Camera chids...");
    /*
        This bit is shoe-horning a previous
        implementation in with minimum fuss :(
    */
    std::string s;
    // init chids
    debugMessage("\npilaserInterface Create channel to monitor PVs\n");
    for(auto&& it:pilaser.pvMonStructs)
    {
        /*
            the PILaser inerface is
        */
        if(it.first == pilaserStructs::PILASER_PV_TYPE::WCM_Q)
        {
            s = pilaser.pvRootQ;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY)
        {
            s = pilaser.pvRootE;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY_RANGE_RB)
        {
            s = pilaser.pvRootE;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY_RUN_RB)
        {
            s = pilaser.pvRootE;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY_OVERRANGE_RB)
        {
            s = pilaser.pvRootE;
        }
        else if(isVCMirror_PV(it.first))
        {
            //message(ENUM_TO_STRING(it.first), " is a mirror PV");
            s = pilaser.mirror.pvRoot;
        }
        else
            s = pilaser.pvRoot;
        addChannel(s, it.second);
    }
    debugMessage("\npilaserInterface Create channel to command PVs\n");
    for(auto&& it:pilaser.pvComStructs)
    {
        if(it.first == pilaserStructs::PILASER_PV_TYPE::WCM_Q)
        {
            s = pilaser.pvRootQ;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY)
        {
            s = pilaser.pvRootE;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY_RANGE_SP)
        {
            s = pilaser.pvRootE;
            std::cout<< "ENERGY_RANGE_SP s = " << s << std::endl;
        }
        else if(it.first == pilaserStructs::PILASER_PV_TYPE::ENERGY_RUN_SP)
        {
            s = pilaser.pvRootE;
        }
        else if(isVCMirror_PV(it.first))
        {
            //message(ENUM_TO_STRING(it.first), " is a mirror PV");
            s = pilaser.mirror.pvRoot;
        }
        else
        {
            s = pilaser.pvRoot;
        }
        addChannel(s, it.second);
    }
    initCamChids(false);
    /*
        This sends all PVs to epics, laser, mirror cmaera etc
    */
    int status = sendToEpics("ca_create_channel",
                             "Found PILaser & Virtual Cathode Camera ChIds.",
                             "!!TIMEOUT!! Not all PILaser & CLARA camera ChIds found.");
    if(status == ECA_TIMEOUT )
    {
        pause_500();
        message("\n", "Checking PILaser ChIds ");
        for(auto&& it:pilaser.pvMonStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        for(auto&& it:pilaser.pvComStructs)
        {
            checkCHIDState(it.second.CHID, ENUM_TO_STRING(it.first));
        }
        message("\n", "Checking CLARA camera ChIds ");
        for(auto&& it:allCamData)
        {
            for(auto&& mon_it:it.second.pvMonStructs)
            {
                checkCHIDState(mon_it.second.CHID, ENUM_TO_STRING(mon_it.first));
            }
            for(auto&& com_it:it.second.pvComStructs)
            {
                checkCHIDState(com_it.second.CHID, ENUM_TO_STRING(com_it.first));
            }
        }
        pause_2000();
    }
    else if (status == ECA_NORMAL)
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void pilaserInterface::addChannel(const std::string& pvRoot,
                                  pilaserStructs::pvStruct& pv)
{
    const std::string s = pvRoot + pv.pvSuffix;
    ca_create_channel(s.c_str(), nullptr, nullptr, UTL::PRIORITY_0, &pv.CHID);
    debugMessage("Create channel to ", s);
}
//______________________________________________________________________________
void pilaserInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    continuousILockMonitorStructs.clear();

    for(auto&& it:pilaser.pvMonStructs)
    {
        addToContinuousMonitorStructs(it.first,it.second);
    }
    for(auto&& it:pilaser.pvComStructs)
    {
        addToContinuousMonitorStructs(it.first,it.second);
    }
    bool start_cam_monitors = startCamMonitors(false);
    int status = sendToEpics("ca_create_subscription", "Succesfully Subscribed to PILaser & Virtual Cathode Camera Monitors",
                             "!!TIMEOUT!! Subscription to PILaser & Virtual Cathode Camera monitors failed" );
    if (status == ECA_NORMAL )
    {
        if(start_cam_monitors )
        {
            allMonitorsStarted = true; /// interface base class membe
        }
    }
}
//____________________________________________________________________________________________
void pilaserInterface::addToContinuousMonitorStructs(const pilaserStructs::PILASER_PV_TYPE pv, const pilaserStructs::pvStruct& st)
{
    continuousMonitorStructs.push_back(new pilaserStructs::monitorStruct());
    continuousMonitorStructs.back()->monType = pv;
    continuousMonitorStructs.back()->object  = &pilaser;
    continuousMonitorStructs.back()->interface  = this;
    ca_create_subscription(st.CHTYPE,
                           st.COUNT,
                           st.CHID,
                           st.MASK,
                           pilaserInterface::staticEntryMonitor,
                           (void*)continuousMonitorStructs.back(),
                           &continuousMonitorStructs.back() -> EVID);
}
//____________________________________________________________________________________________
void pilaserInterface::staticEntryMonitor(const event_handler_args args)
{
    using namespace pilaserStructs;
    monitorStruct*ms = static_cast<monitorStruct *>(args.usr);
    ms->interface->updateValue(args,ms -> monType);
}
//____________________________________________________________________________________________
void pilaserInterface::updateValue(const event_handler_args args,pilaserStructs::PILASER_PV_TYPE pv)
{
    using namespace pilaserStructs;
    switch(pv)
    {
        case PILASER_PV_TYPE::ENERGY_RANGE_RB:
            //std::cout << "ENERGY_RANGE_RB = " << getDBRunsignedShort(args) << std::endl;
            updateEnergyRange(getDBRunsignedShort(args));
            //pilaser.em_range = HWC_ENUM::STATE::ERR;
            break;
        case PILASER_PV_TYPE::ENERGY_RUN_RB:
            if(getDBRunsignedShort(args) == UTL::ONE_US)
            {
                pilaser.em_is_running = true;
                message("em_is_running = true " );
            }
            else
            {
                pilaser.em_is_running = false;
                message("em_is_running = false ");
            }
            break;
        case PILASER_PV_TYPE::ENERGY_OVERRANGE_RB:
            //unsigned short val = getDBRunsignedShort(args);
            if(  getDBRunsignedShort(args) == UTL::ONE_US)
            {
                pilaser.em_over_range = true;
                message("em_over_range = true ");
            }
            else
            {
                pilaser.em_over_range = false;
                message("em_over_range = false ");
            }
            break;
        case PILASER_PV_TYPE::STABILISATION:
            pilaser.stabilisation_status = HWC_ENUM::STATE::ERR;
            break;
        case PILASER_PV_TYPE::STATUS:
            break;
        case PILASER_PV_TYPE::HALF_WAVE_PLATE_SET:
            break;
        case PILASER_PV_TYPE::HALF_WAVE_PLATE_READ:
            pilaser.HWP = getDBRdouble(args);
            break;

        case PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE:
            {
                auto v = getDBRunsignedShort(args);
                if(v == UTL::ZERO_US)
                {
                    pilaser.hwp_enable = HWC_ENUM::STATE::OFF;
                }
                else if(v == UTL::ONE_US)
                {
                    pilaser.hwp_enable = HWC_ENUM::STATE::ON;
                }
                else
                {
                    pilaser.hwp_enable = HWC_ENUM::STATE::ERR;
                }
            }
            break;
        case PILASER_PV_TYPE::WCM_Q:
            pilaser.Q = getDBRdouble(args);
            pilaser.Q_rs.Push(pilaser.Q);
            addToBuffer(pilaser.Q, pilaser.Q_buf);
            break;
        case PILASER_PV_TYPE::ENERGY:
            pilaser.energy = getDBRdouble(args);
            pilaser.energy_rs.Push(pilaser.energy);
            addToBuffer(pilaser.energy, pilaser.E_buf);
            break;

        default:
            //message("ERROR nkown PV passed to pilaserInterface::updateValue, ",pv);
            break;
    }
}
//____________________________________________________________________________________________
void pilaserInterface::updatePixelResults(const event_handler_args& args)
{
    const double * pValue;
    /* if time _type get time and set where pValue points to */
    if(isTimeType(args.type))
    {
        const dbr_time_double* p = (const struct dbr_time_double*)args.dbr;
        pValue = &p->value;
        //updateTime(trace.etime, trace.time, trace.timeStr);

        /* for hints look in epicsTime.h */
        char timeString[UTL::BUFFER_36];
        epicsTimeToStrftime(timeString, sizeof(timeString),
                            "%a %b %d %Y %H:%M:%S.%f", &p->stamp);
        /*
            prove it works
            std::cout <<std::setprecision(15) <<std::showpoint<< val <<std::endl;
        */
//        pilaser.vcData.pix_values_time = timeString;
//        std::copy(pValue, pValue + pilaser.vcData.pix_values.size(), pilaser.vcData.pix_values.begin());
    }
    else /* set where pValue points to */
    {
        pValue = (dbr_double_t*)args.dbr;
    }
    /*
        Now we have the new values, update Buffers
    */
    updateAnalysisBuffers();
}
//____________________________________________________________________________________________
//____________________________________________________________________________________________
void pilaserInterface::updateEnergyRange(const unsigned short value)
{
    using namespace pilaserStructs;
    switch(value)
    {
        case UTL::ZERO_US:
            pilaser.em_range = EM_RANGE::TWO_HUNDRED_MICRO_J;
            break;
        case UTL::ONE_US:
            pilaser.em_range = EM_RANGE::TWENTY_MICRO_J;
            break;
        case UTL::TWO_US:
            pilaser.em_range = EM_RANGE::TWO_MICRO_J;
            break;
        case UTL::THREE_US:
            pilaser.em_range = EM_RANGE::TWO_HUNDRED_NANO_J;
            break;
        default:
            pilaser.em_range = EM_RANGE::TWO_HUNDRED_NANO_J;
    }
    message("em_range = ",ENUM_TO_STRING(pilaser.em_range));
}

bool pilaserInterface::setEnergyRange200uJ()
{
    return setEnergyRange(pilaserStructs::EM_RANGE::TWO_HUNDRED_MICRO_J);
}
bool pilaserInterface::setEnergyRange20uJ()
{
    return setEnergyRange(pilaserStructs::EM_RANGE::TWENTY_MICRO_J);
}
bool pilaserInterface::setEnergyRange2uJ()
{
    return setEnergyRange(pilaserStructs::EM_RANGE::TWO_MICRO_J);
}
bool pilaserInterface::setEnergyRange200nJ()
{
    return setEnergyRange(pilaserStructs::EM_RANGE::TWO_HUNDRED_NANO_J);
}

bool pilaserInterface::setEnergyRange(const pilaserStructs::EM_RANGE new_range)
{
    using namespace pilaserStructs;
    if( stopEM() )
    {
        pause_500();
        unsigned short new_val = UTL::ZERO_US;
        switch(new_range)
        {
            case EM_RANGE::TWO_HUNDRED_MICRO_J:
                new_val = UTL::ZERO_US;
                break;
            case EM_RANGE::TWENTY_MICRO_J:
                new_val = UTL::ONE_US;
                break;
            case EM_RANGE::TWO_MICRO_J:
                new_val = UTL::TWO_US;;
                break;
            case EM_RANGE::TWO_HUNDRED_NANO_J:
                new_val = UTL::THREE_US;
                break;
        }
        if (caput<unsigned short>
                (pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::ENERGY_RANGE_SP).CHTYPE,
                 pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::ENERGY_RANGE_SP).CHID,
                 new_val,
                 "", "Timeout sending setEnergyRange")
                 )
        {
            pause_500();
            return startEM();
        }

    }
    return false;
}

bool pilaserInterface::startEM()
{
    return caput<unsigned short>
            (pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::ENERGY_RUN_SP).CHTYPE,
             pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::ENERGY_RUN_SP).CHID,
             UTL::ONE_US,
             "", "Timeout sending EM START");
}
bool pilaserInterface::stopEM()
{
    return caput<unsigned short>
            (pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::ENERGY_RUN_SP).CHTYPE,
             pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::ENERGY_RUN_SP).CHID,
             UTL::ZERO_US,
             "", "Timeout sending EM START");
}

bool pilaserInterface::isRunning() const
{
    return pilaser.em_is_running;
}
bool pilaserInterface::isOverRange() const
{
    return pilaser.em_over_range;
}

std::string pilaserInterface::getEnergyRange() const
{
    return ENUM_TO_STRING(pilaser.em_range);
}







//______________________________________________________________________________
void pilaserInterface::clearRunningValues()
{
    pilaser.Q_clear();
    pilaser.energy_clear();
//    pilaser.buffer_full = false;
//    pilaser.buffer_count = UTL::ZERO_SIZET;
}

//____________________________________________________________________________________________
void pilaserInterface::addToBuffer(const double val,std::vector<double>& buffer)
{
    buffer.push_back(val);
    //pilaser.buffer_count += UTL::ONE_SIZET;
    if(buffer.size() > pilaser.max_buffer_count )
    {
        //message("buffer FULL, ", buffer.size() );
        buffer.erase(buffer.begin());
        pilaser.buffer_full = true;
        //message("BUFFER FULL!!! ");
        //pilaser.buffer_count -= UTL::ONE_SIZET;
    }
    else if(buffer.size() == pilaser.max_buffer_count )
    {
        //message("buffer FULL, ", buffer.size() );
        pilaser.buffer_full = false;
    }
    else
    {
        //message("buffer NOT FULL, ", buffer.size() );
        pilaser.buffer_full = false;
    }
    pilaser.buffer_count = buffer.size();

}
//____________________________________________________________________________________________
bool pilaserInterface::isRSBufferFull() const
{
    return pilaser.buffer_full;
}
//____________________________________________________________________________________________
size_t pilaserInterface::getRSBufferSize()const
{
    return pilaser.max_buffer_count;
}
//____________________________________________________________________________________________
size_t pilaserInterface::getCurrentBufferSize()const
{
    return pilaser.buffer_count;
}

bool pilaserInterface::setAllRSBufferSize(const size_t new_size)
{
    setBufferMaxCount_VC(new_size);
    pilaser.max_buffer_count = new_size;
    pilaser.Q_rs_buffer_size(new_size);
    pilaser.energy_rs_buffer_size(new_size);
    return true;
}



//____________________________________________________________________________________________
void pilaserInterface::updateAnalysisBuffers()
{
//    pilaser.vcData.pix_values_buffer.push_back(pilaser.vcData.pix_values);
//    if(pilaser.vcData.pix_values_buffer.size()> pilaser.vcData.buffer_size )
//    {
//        pilaser.vcData.pix_values_buffer.pop_front();
//    }
}




//____________________________________________________________________________________________
double pilaserInterface::getQ()const
{
    return pilaser.Q;
}
//____________________________________________________________________________________________
std::vector<double> pilaserInterface::getQBuffer()const
{
    return pilaser.Q_buf;
}
//____________________________________________________________________________________________
std::vector<double> pilaserInterface::getEBuffer()const
{
    return pilaser.E_buf;
}




//____________________________________________________________________________________________
int pilaserInterface::setHWP(const double value)
{
    // To Write
    return caput<double>
            (pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_SET).CHTYPE,
             pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_SET).CHID,
             value,
             "", "Timeout sending Half Wave Plate Value");
}
//____________________________________________________________________________________________
double pilaserInterface::getHWP()
{
    return pilaser.HWP;
}
//____________________________________________________________________________________________
bool pilaserInterface::setCharge(const double value)
{
    // To Write
    return false;
}
//____________________________________________________________________________________________
bool pilaserInterface::setEnergy(const double value)
{
    // To Write
    return false;
}
//____________________________________________________________________________________________
double pilaserInterface::getEnergy() const
{
    return pilaser.energy;
}
//____________________________________________________________________________________________
std::string pilaserInterface::getName() const
{
    return pilaser.name;
}
//____________________________________________________________________________________________
HWC_ENUM::STATE pilaserInterface::getStatus() const
{
    return pilaser.status;
}
//____________________________________________________________________________________________
double pilaserInterface::getStabilisationStatus() const
{
    return pilaser.stabilisation_status;
}
//____________________________________________________________________________________________
bool pilaserInterface::isOn() const
{
    return pilaser.status == HWC_ENUM::STATE::ON;
}
//____________________________________________________________________________________________
bool pilaserInterface::isOff() const
{
    return pilaser.status == HWC_ENUM::STATE::OFF;
}
//____________________________________________________________________________________________
bool pilaserInterface::isStabilisationOn() const
{
    return pilaser.stabilisation_status == HWC_ENUM::STATE::ON;
}
//____________________________________________________________________________________________
bool pilaserInterface::isStabilisationOff() const
{
    return pilaser.stabilisation_status == HWC_ENUM::STATE::OFF;
}
//____________________________________________________________________________________________
bool pilaserInterface::enableStabilisation()
{
    // to write
    return false;
}
//____________________________________________________________________________________________
bool pilaserInterface::disableStabilisation()
{
    // to write
    return false;
}
//____________________________________________________________________________________________
const pilaserStructs::pilaserObject& pilaserInterface::getPILObjConstRef() const
{
    return pilaser;
}
//____________________________________________________________________________________________
const pilaserStructs::pilMirrorObject& pilaserInterface::getpilMirrorObjConstRef() const
{
    return pilaser.mirror;
}
//____________________________________________________________________________________________
//const cameraStructs::analysis_data& pilaserInterface::getVCDataObjConstRef() const
//{
//    return pilaser.vcData;
//}
//____________________________________________________________________________________________
bool pilaserInterface::setValue(pilaserStructs::pvStruct& pvs,const double value)
{
    bool ret = false;
//    ca_put(pvs.CHTYPE,pvs.CHID,&value);
//    std::stringstream ss;
//    ss <<"Timeout setting pilaser," <<ENUM_TO_STRING(pvs.pvType) <<" value to " <<value;
//    int status = sendToEpics("ca_put","",ss.str().c_str());
//    if(status==ECA_NORMAL)
//        ret=true;
    return ret;
}
//______________________________________________________________________________
bool pilaserInterface::isVCMirror_PV(const pilaserStructs::PILASER_PV_TYPE& pv)const
{
    using namespace pilaserStructs;
    switch(pv)
    {
        case PILASER_PV_TYPE::H_POS:
            return true;
        case PILASER_PV_TYPE::V_POS:
            return true;
        case PILASER_PV_TYPE::H_STEP:
            return true;
        case PILASER_PV_TYPE::V_STEP:
            return true;
        case PILASER_PV_TYPE::H_MOVE:
            return true;
        case PILASER_PV_TYPE::V_MOVE:
            return true;
        case PILASER_PV_TYPE::H_STOP:
            return true;
        case PILASER_PV_TYPE::V_STOP:
            return true;
        case PILASER_PV_TYPE::POS_UPDATE:
            return true;
        case PILASER_PV_TYPE::H_MREL:
            return true;
        case PILASER_PV_TYPE::V_MREL:
            return true;
        case PILASER_PV_TYPE::UNKNOWN_PILASER_PV_TYPE:
            message("isVCMirror_PV passed UNKNOWN pv");
            break;
        default:
            return false;
    }
    return false;
}
//____________________________________________________________________________________________
double pilaserInterface::getHstep() const
{
    return pilaser.mirror.hStep;
}
//____________________________________________________________________________________________
bool pilaserInterface::isHWPEnabled() const
{
    return pilaser.hwp_enable == HWC_ENUM::STATE::ON;
}
//____________________________________________________________________________________________
bool pilaserInterface::isHWPDisabled()const
{
    return pilaser.hwp_enable == HWC_ENUM::STATE::OFF;
}
//____________________________________________________________________________________________
bool pilaserInterface::getHWPEnableState()const
{
    return pilaser.hwp_enable;
}

//____________________________________________________________________________________________
bool pilaserInterface::enableHWP()
{
    //need to figure out how to move th elaser from command line...
    pilaserStructs::pvStruct& pvs = pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE);
    dbr_enum_t c = (dbr_enum_t)UTL::ZERO_US;
    return caput<dbr_enum_t>(pvs.CHTYPE,pvs.CHID, UTL::ZERO_U_SHORT, "", "");
}
//____________________________________________________________________________________________
bool pilaserInterface::disableHWP()
{
    //need to figure out how to move the laser from command line...
    pilaserStructs::pvStruct& pvs = pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE);
    dbr_enum_t c = (dbr_enum_t)UTL::ZERO_US;
    return caput<dbr_enum_t>(pvs.CHTYPE,pvs.CHID, UTL::ONE_U_SHORT, "", "");
}
//____________________________________________________________________________________________
double pilaserInterface::getVstep() const
{
    return pilaser.mirror.vStep;
}
//____________________________________________________________________________________________
bool pilaserInterface::setHstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_STEP),value);
    }
    else
    {
        if( fabs(value) <= pilaser.mirror.STEP_MAX)
        {
            pilaser.mirror.hStep = value;
        }
        else
        {
            if(value <UTL::ZERO_DOUBLE )
                pilaser.mirror.hStep = -pilaser.mirror.STEP_MAX;
            else
                pilaser.mirror.hStep = pilaser.mirror.STEP_MAX;
        }
    }
    return s;
}
//____________________________________________________________________________________________
bool pilaserInterface::setVstep(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_STEP),value);
    }
    else
    {
        if(fabs(value) <= pilaser.mirror.STEP_MAX)
        {
            pilaser.mirror.vStep = -value;
        }
        else
        {
            if(value <UTL::ZERO_DOUBLE )
                pilaser.mirror.vStep = pilaser.mirror.STEP_MAX;
            else
                pilaser.mirror.vStep = -pilaser.mirror.STEP_MAX;

        }
    }
    return s;
}
//____________________________________________________________________________________________
double pilaserInterface::getHpos()const
{
    return pilaser.mirror.hPos;
}
//____________________________________________________________________________________________
double pilaserInterface::getVpos()const
{
    return pilaser.mirror.vPos;
}
//____________________________________________________________________________________________
bool pilaserInterface::setHpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_POS),value);
        return s;
    }
    // add in VM implementation at som epoint
    return false;
}
//____________________________________________________________________________________________
bool pilaserInterface::setVpos(const double value)
{
    bool s = true;
    if(startVirtualMachine)
    {
        bool s = setValue(
            pilaser.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_POS),value);
        return s;
    }
    // add in VM implementation at som epoint
    return false;
}
//____________________________________________________________________________________________
bool pilaserInterface::moveLeft(const double value)
{
    message("pilaserInterface::moveLeft, ", value, ", ", value * pilaser.mirror.left_sense);
    //message("pilaserInterface::moveLeft");
    setHstep( value * pilaser.mirror.left_sense);
    return moveH();
}
//____________________________________________________________________________________________
bool pilaserInterface::moveRight(const double value)
{
    message("pilaserInterface::moveRight, ", value, ", ", value * pilaser.mirror.right_sense);
    setHstep( value * pilaser.mirror.right_sense);
    return moveH();
}
//____________________________________________________________________________________________
bool pilaserInterface::moveUp(const double value)
{
    message("pilaserInterface::moveUp, ", value, ", ", value * pilaser.mirror.up_sense);
    setVstep(value * pilaser.mirror.up_sense);
    return moveV();
}
//____________________________________________________________________________________________
bool pilaserInterface::moveDown(const double value)
{
    message("pilaserInterface::moveDown, ", value, ", ", value * pilaser.mirror.down_sense);
    setVstep( value * pilaser.mirror.down_sense);
    return moveV();
}
//____________________________________________________________________________________________
bool pilaserInterface::moveH()
{
    pilaserStructs::pvStruct& pvs = pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::H_MREL);
    return move(pvs.CHTYPE,pvs.CHID,pilaser.mirror.hStep,
                  "",
                  "!!TIMEOUT!! FAILED TO SEND MOVE H");
}
//____________________________________________________________________________________________
bool pilaserInterface::moveV()
{
    //message("pilaserInterface::moveV() ", pilaser.mirror.vStep);
    pilaserStructs::pvStruct& pvs = pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::V_MREL);
    return move(pvs.CHTYPE,pvs.CHID,pilaser.mirror.vStep,
                  "",
                  "!!TIMEOUT!! FAILED TO SEND MOVE V");
}
//____________________________________________________________________________________________
bool pilaserInterface::move(chtype& cht, chid& chi,const double val,const char* m1,const char* m2)
{
    //need to figure out how to move th elaser from command line...
    int status = caput<double>(cht, chi, val, m1, m2);
    if(status == ECA_NORMAL)
    {
        return true;
    }
    return false;
}

//______________________________________________________________________________
bool pilaserInterface::check_data_timestamps(const cameraStructs::cameraObject& cam)
{
    /*
        these are the thing swe have to decide if the data is updating

            cam.data.analysis.pix_values_counter

            cam.data.analysis.pix_values_timestamp

            cam.state.is_camera_image_updating

            cam.data.image.array_data_timestamp

            cam.state.is_camera_analysis_updating

            cam.data.image.counter

            camObj.state.latest_avg_pix_has_beam

            cam.state.mask_feedback
    */
    if(cam.data.image.counter < UTL::TWO_SIZET)
    {
        message("Image counter < 2, i'm not sure images are updating ");
        //return false;
    }

    if(cam.data.analysis.pix_values_counter < UTL::TWO_SIZET)
    {
        message("Image counter < 2, i'm not sure analysis values are updating ");
         //return false;
    }
    // Compare TIMES TO CURRENT TIME
    /*
        check timestamp compared to current time
    */
    epicsTimeStamp tn = epicsTimeStamp( et.getCurrent() );
    std::string tn_time_str;
    double tn_time_d;
    updateTime(tn,tn_time_d,tn_time_str);

    message("Times: pix_values = ", cam.data.analysis.pix_values_timestamp.time_Str, " Now = ", tn_time_str);
    message("DIFF : ", cam.data.analysis.pix_values_timestamp.time_ns - tn_time_d);

    if(cam.state.latest_avg_pix_has_beam)
    {
        message("I think there is laser beam on the screen");
    }
    else
    {
        message("I think there is NO laser beam on the screen");
    }
    return false;

}



bool pilaserInterface::shortCaput(unsigned short comm, pilaserStructs::pvStruct& S)
{
    bool ans(false);
    ca_put(S.CHTYPE,S.CHID, &comm);
    int status = sendToEpics("ca_put", "", "Timeout trying to sendToEpics.");
    if(status == ECA_NORMAL)
    {
        ans = true;
    }
    return ans;
}






//void staticEntry_set_VC_xpos(const double xpos, const time_t waitTime, pilaserInterface* interface)
//{
//    interface->set_VC_xpos()
//}

//set_VC_xpos()
//
//    time_t timeStart = timeNow();
//
//        /* check if time ran out */
//        if(timeNow() - timeStart> waitTime)
//        {
//            timeOut = true;
//        }
//        runningStat& x_mean = cam.data.analysis.x_rs;
//        runningStat& y_mean = cam.data.analysis.y_rs;
//
//        size_t start_buffer_count = getBufferMaxCount_VC();
//
//        setBufferMaxCount_VC(10);//MAGIC_NUMBER
//
//        while( areNotSame(xpos) )
//        {
//
//            message()
//
//        }
//        setBufferMaxCount_VC(start_buffer_count);//MAGIC_NUMBER




////______________________________________________________________________________
//std::vector<int> pilaserInterface::getFastImage()
//{
////    pilaserStructs::PILASER_PV_TYPE cd = pilaserStructs::PILASER_PV_TYPE::ARRAY_DATA;
////    if(entryExists(pilaser.vcData.pvComStructs,cd))
////    {
////        //message(ENUM_TO_STRING(cd)," exists");
////        ca_array_get(pilaser.vcData.pvComStructs.at(cd).CHTYPE,
////                pilaser.vcData.pvComStructs.at(cd).COUNT,
////               pilaser.vcData.pvComStructs.at(cd).CHID,
////               //(void*)&pilaser.vcData.array_data[0]);
////               &pilaser.vcData.array_data[0]);
////        int a = sendToEpics("ca_get","","");
////        if(a==ECA_NORMAL)
////        {
//////            int max = 0;
//////            for(auto&& it: pilaser.vcData.array_data)
//////            {
//////                std::cout <<it <<" ";
//////                if(it> max)
//////                {
//////                    max = it;
//////                }
//////            }
//////            message("max = ", max);
//////
//////            message("caget fine");
////            return pilaser.vcData.array_data;
////        }
////        else
////            message("caget not fine");
////    }
//    std::vector<int> dummy;
//    return dummy;
//}
//
//

////____________________________________________________________________________________________
//unsigned short pilaserInterface::getMaskX()const
//{
//    return pilaser.vcCam.data.mask.mask_x;
//}
////____________________________________________________________________________________________
//unsigned short pilaserInterface::getMaskY()const
//{
//    return pilaser.vcCam.data.mask.mask_y;
//}
////____________________________________________________________________________________________
//unsigned short pilaserInterface::getMaskXrad()const
//{
//    return pilaser.vcCam.data.mask.mask_x_rad;
//}
////____________________________________________________________________________________________
//unsigned short pilaserInterface::getMaskYrad()const
//{
//    return pilaser.vcCam.data.mask.mask_y_rad;
//}

////____________________________________________________________________________________________
//bool pilaserInterface::setMaskX(unsigned short x)
//{
//    bool ans = false;
////    ans = shortCaput(x, pilaser.vcData.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::MASK_X));
//    message("Setting mask X center");
//    return ans;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setMaskY(unsigned short y)
//{
//    bool ans = false;
////    ans = shortCaput(y, pilaser.vcData.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::MASK_Y));
//    message("Setting mask Y center.");
//    return ans;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setMaskXrad(unsigned short xRad)
//{
//    bool ans = false;
////    ans=shortCaput(xRad, pilaser.vcData.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::MASK_X_RAD));
//    message("Setting mask X radius");
//    return ans;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setMaskYrad(unsigned short yRad)
//{
//    bool ans=false;
////    ans=shortCaput(yRad, pilaser.vcData.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::MASK_Y_RAD));
//    message("Setting mask Y radius");
//    return ans;
//}
//____________________________________________________________________________________________


//
//
////____________________________________________________________________________________________
//bool cameraIAInterface::setBackground()
//{
//    bool ans=false;
//    unsigned short on = 1;
//    unsigned short off = 1;
//    if( isAcquiring(selectedCamera()))
//    {
//        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::SET_BKGRND));
//        ans=shortCaput(off,S);
//        std::this_thread::sleep_for(std::chrono::milliseconds( 50 ));
//        ans=shortCaput(on,S);
//        std::this_thread::sleep_for(std::chrono::milliseconds( 50 ));
//        ans=shortCaput(off,S);
//        message("Setting background with next live image on ",
//                selectedCameraObj.name," camera.");
//    }
//    return ans;
//}
//bool cameraIAInterface::setCenterXPixel(const int xC)
//{
//    bool ans=false;
//    unsigned short comm = xC;
//
//    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::X_CENTER));
//    ans=shortCaput(comm,S);
//    //if (ans==true) {selectedCameraObj.IA.xCenterPix=xC;}
//    message("Setting x pixel center for",
//            selectedCameraObj.name," camera.");
//    return ans;
//}
//bool cameraIAInterface::setCenterYPixel(const int yC)
//{
//    bool ans=false;
//    unsigned short comm = yC;
//
//    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::Y_CENTER));
//    ans=shortCaput(comm,S);
//    //if (ans==true) {selectedCameraObj.IA.yCenterPix=yC;}
//    message("Setting y pixel center for",
//            selectedCameraObj.name," camera.");
//    return ans;
//}
//bool cameraIAInterface::setPixMM(const double pmm)
//{
//    bool ans=false;
//
//    pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::PIX_MM));
//    ans=doubleCaput(pmm,S);
//    if (ans==true) {selectedCameraObj.IA.pix2mm=pmm;}
//    message("Setting pix to mm ratio for ",
//            selectedCameraObj.name," camera.");
//    return ans;
//}
//bool cameraIAInterface::useBackground(const bool run)
//{
//    bool ans=false;
//    unsigned short comm = 1;
//    if (run==true){
//        comm=1;
//    }
//    else{
//        comm=0;
//    }
//
//    if( isAcquiring(selectedCamera()))
//    {
//        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::USE_BKGRND));
//        ans=shortCaput(comm,S);
//        message("Using background in online Image Analysis on  ",
//                selectedCameraObj.name," camera.");
//    }
//    return ans;
//}
//bool cameraIAInterface::startAnalysis()
//{
//    bool ans=false;
//    unsigned short comm = 1;
//    if( isAcquiring(selectedCamera()))
//    {
//        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::START_IA));
//        ans=shortCaput(comm,S);
//        message("Starting online Image Analysis on ",
//                selectedCameraObj.name," camera.");
//    }
//    return ans;
//}
//bool cameraIAInterface::stopAnalysis()
//{
//    bool ans=false;
//    unsigned short comm = 0;
//    if( isAcquiring(selectedCamera()))
//    {
//        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::START_IA));
//        ans=shortCaput(comm,S);
//        message("Stopping online Image Analysis on ",
//                selectedCameraObj.name," camera.");
//    }
//    return ans;
//}
//
//bool cameraIAInterface::useNPoint(const bool run)
//{
//    bool ans=false;
//    unsigned short comm = 1;
//    if (run==true){
//        comm=1;
//    }
//    else{
//        comm=0;
//    }
//
//    if( isAcquiring(selectedCamera()))
//    {
//        pvStruct S(selectedCameraObj.pvComStructs.at(CAM_PV_TYPE::USE_NPOINT));
//        ans=shortCaput(comm,S);
//        message("Using N-Point scaling in online Image Analysis on  ",
//                selectedCameraObj.name," camera.");
//    }
//    return ans;
//}
//
//
//bool cameraInterface::isON ( const std::string & cam )
//{
//    std::string cameraName = useCameraFrom(cam);
//    bool ans = false;
//    if(entryExists( allCamData, cameraName))
//        if(allCamData.at(cameraName).state == CAM_STATE::CAM_ON)
//            ans = true;
//    return ans;
//}
//bool cameraInterface::isOFF( const std::string & cam )
//{
//    std::string cameraName = useCameraFrom(cam);
//    bool ans = false;
//    if( entryExists( allCamData, cameraName ) )
//        if(allCamData.at(cameraName).state == CAM_STATE::CAM_OFF)
//            ans = true;
//    return ans;
//}
//bool cameraInterface::isAcquiring( const std::string & cam )
//{
//    std::string cameraName = useCameraFrom(cam);
//    bool ans = false;
//    if( entryExists( allCamData, cameraName ) )
//        if( allCamData.at(cameraName).acquireState == ACQUIRE_STATE::ACQUIRING )
//            ans = true;
//    return ans;
//}
//bool cameraInterface::isNotAcquiring ( const std::string & cam)
//{
//    std::string cameraName = useCameraFrom(cam);
//    bool ans = false;
//    if( entryExists( allCamData, cameraName ) )
//        if( allCamData.at(cameraName).acquireState == ACQUIRE_STATE::NOT_ACQUIRING )
//            ans = true;
//    return ans;
//}
//
//
//
//
//



//
//
////____________________________________________________________________________________________
//void pilaserInterface::setMaskX(unsigned short v)
//{
//    return pilaser.vcData.mask_y_rad;
//}
////____________________________________________________________________________________________
//void pilaserInterface::setMaskY(unsigned short v)
//{
//
//}
////____________________________________________________________________________________________
//void pilaserInterface::setMaskXrad(unsigned short v)
//{
//
//}
////____________________________________________________________________________________________
//void pilaserInterface::setMaskYrad(unsigned short v)
//{
//
//}
////____________________________________________________________________________________________



//____________________________________________________________________________________________
//double pilaserInterface::getVstep() const
//{
//    return pilaser.vcData.mirror.vStep;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setHstep(const double value)
//{
//    bool s = true;
//    if(startVirtualMachine)
//    {
//        bool s = setValue(
//            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_STEP),value);
//    }pilaser.vcData.pvComStructs.at(cd).COUNT
//    pilaser.vcData.mirror.hStep = value;
//    return s;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setVstep(const double value)
//{
//    bool s = true;
//    if(startVirtualMachine)
//    {
//        bool s = setValue(
//            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::V_STEP),value);
//    }
//    pilaser.vcData.mirror.vStep = value;
//    return s;
//}
////____________________________________________________________________________________________
//double pilaserInterface::getHpos() const
//{
//    return pilaser.vcData.mirror.hPos;
//}
////____________________________________________________________________________________________
//double pilaserInterface::getVpos() const
//{
//    return pilaser.vcData.mirror.vPos;
//}
////____________________________________________________________________________________________
//bool pilaserInterface::setHpos(const double value)
//{
//    bool s = true;
//    if(startVirtualMachine)
//    {
//        bool s = setValue(
//            pilaser.vcData.pvMonStructs.at(pilaserStructs::PILASER_PV_TYPE::H_POS),value);
//        return s;
//    }
//    //currently can't do ths on Real Machine
//    return false;
//}
////____________________________________________________________________________________________





////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getXBuffer()const
//{
//    return pilaser.vcData.x_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getYBuffer()const
//{
//    return pilaser.vcData.y_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getSigXBuffer()const
//{
//    return pilaser.vcData.sig_x_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getSigYBuffer()const
//{
//    return pilaser.vcData.sig_y_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getSigXYBuffer()const
//{
//    return pilaser.vcData.sig_xy_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getXPixBuffer()const
//{
//    return pilaser.vcData.x_pix_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getYPixBuffer()const
//{
//    return pilaser.vcData.y_pix_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getSigXPixBuffer()const
//{
//    return pilaser.vcData.sig_x_pix_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getSigYPixBuffer()const
//{
//    return pilaser.vcData.sig_y_pix_buf;
//}
////____________________________________________________________________________________________
//std::vector<double> pilaserInterface::getSigXYPixBuffer()const
//{
//    return pilaser.vcData.sig_xy_pix_buf;
//}



