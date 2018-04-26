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
//  Last edit:   19-03-2018
//  FileName:    magnetInterface.cpp
//  Description:
//
//
//*/
#include "magnetInterface.h"
//djs
#include "dburt.h"
#include "configDefinitions.h"
// stl
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <thread>

using IlockMap1 = std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>;
using IlockMap2 = std::map<HWC_ENUM::ILOCK_NUMBER,std::string>;
using vvec_d    = std::vector<std::vector<double>>;
using vec_s     = std::vector<std::string> ;
using vec_d     = std::vector<double>;
using vec_si    = std::vector<size_t>;
using vec_b     = std::vector<bool>;
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
magnetInterface::magnetInterface(const std::string&magConf,
                                 const bool startVirtualMachine,
                                 const bool& show_messages,
                                 const bool& show_debug_messages,
                                 const bool shouldStartEPICs,
                                 const HWC_ENUM::MACHINE_AREA myMachineArea):
configReader(magConf,startVirtualMachine, show_messages,show_debug_messages),
interface(show_messages, show_debug_messages),
dummyName(UTL::DUMMY_NAME),
shouldStartEPICs(shouldStartEPICs),
myMachineArea(myMachineArea),
PSU_ON(UTL::ONE_US),
PSU_OFF(UTL::ZERO_US),
wait_time(UTL::TIMET_45)
{
    initialise();
}
//______________________________________________________________________________
magnetInterface::~magnetInterface()
{
    debugMessage("magnetInterface DESTRUCTOR CALLED");
    for (auto&& it:degaussStructsMap)
    {
        debugMessage("in magnetInterface: delete degauss thread ", it.first);
        it.second.thread->join();
        delete it.second.thread;
    }
    killILockMonitors();
    for(auto&& it:continuousMonitorStructs)
    {
        killMonitor(it);
        debugMessage("delete ", it->objName, " ", ENUM_TO_STRING(it->monType),
                     " continuousMonitorStructs entry.");
        delete it;
    }
    debugMessage("magnetInterface DESTRUCTOR COMPLETE");
}
//______________________________________________________________________________
void magnetInterface::killMonitor(magnetStructs::monitorStruct * ms)
{
    int status = ca_clear_subscription(ms->EVID);
    if(status == ECA_NORMAL)
        debugMessage(ms->objName, " ", ENUM_TO_STRING(ms->monType),
                     " monitoring = false ");
    else
        debugMessage("ERROR magnetInterface: in killMonitor: "
                     "ca_clear_subscription failed for ",
                     ms->objName, " ", ENUM_TO_STRING(ms->monType));
}
//______________________________________________________________________________
void magnetInterface::initialise()
{
    /* The config file reader */
    message("The magnetInterface is going to try and read the config file.");
    configFileRead = configReader.readConfig();
    UTL::STANDARD_PAUSE;
    if(configFileRead)
    {
        message("The magnetInterface has read the config file, "
                "cquiring objects");
        /* initialise the objects based on what is read from the config file */
        bool getDataSuccess = initObjects();
        if(getDataSuccess)
        {
            if(shouldStartEPICs)
            {
                message("The magnetInterface has acquired objects, "
                        "connecting to EPICS");
                /* subscribe to the channel ids */
                initChids();
                /* start the monitors: set up the callback functions */
                startMonitors();
                /* The pause allows EPICS to catch up. */
                UTL::STANDARD_PAUSE;
            }
            else
            {
                message("The magnetInterface has acquired objects, "
                        "NOT connecting to EPICS");
            }
        }
        else
        {
            message("!!!The magnetInterface received an error while "
                    "getting magnet data!!!");
        }
    }
    else
    {
        message("!!!ERROR!!! The magnetInterface "
                "Failed to read the config file.");
    }
}
//______________________________________________________________________________
bool magnetInterface::initObjects()
{
    bool magDatSuccess = configReader.getMagData(allMagnetData);
    // needed for something... ;-)
    addDummyElementToAllMAgnetData();
    /* set the machine area on each magent,
       this allows for flavour switching functions, such as switchON etc..
    */
    for(auto&& it:allMagnetData)
    {
        it.second.machineArea = myMachineArea;
        it.second.interface   = this;
    }
    return magDatSuccess;
}
//______________________________________________________________________________
void magnetInterface::initChids()
{
    message("\n", "Searching for Magnet chids...");
    for(auto&& magObjIt:allMagnetData)
    {
        for(auto&& it2:magObjIt.second.pvComStructs)
        {
    /*
       the VELA correctors and BSOL are all on the same PSU,
       with the same PV ROOT(!)
       EBT-INJ-MAG-HVCOR-01:
       it would be nice if controls made them look the same as other magnets

     !!! THE ABOVE MAY HAVE NOW HAPPENED !!! 2018

       i.e. a dummy PV that works the same as a normal magnetPSU for each
       corrector. Anyway, here we have to test if its a magnet that has a
       different psuRoot to pvROOT AND if we're connecting a PSU suffix,
       which i will have to hardcode in here the above makes sense
      the below could be a bit neater, with less ifs and copy-pasta
    */
            if(Is_PSU_PV(it2.first))
            {
                if(Is_psuRoot_SAME_AS_PV_ROOT(magObjIt.second.psuRoot))
                {
                    addChannel(magObjIt.second.pvRoot, it2.second);
                }
                else
                {
                    addChannel(magObjIt.second.psuRoot, it2.second);
                }
            }
            else
            {
                addChannel(magObjIt.second.pvRoot, it2.second);
            }
        }
        for(auto&& it2:magObjIt.second.pvMonStructs)
        {
            if(Is_PSU_PV(it2.first))
            {
                if(Is_psuRoot_SAME_AS_PV_ROOT(magObjIt.second.psuRoot))
                {
                    addChannel(magObjIt.second.pvRoot, it2.second);
                }
                else
                {
                    addChannel(magObjIt.second.psuRoot, it2.second);
                }
            }
            else
            {
                addChannel(magObjIt.second.pvRoot, it2.second);
            }
        }
    }
    int status = sendToEpics("ca_create_channel", "Found Magnet ChIds.",
                             "!!TIMEOUT!! Not all Magnet ChIds found.");
    if(status == ECA_TIMEOUT)
    {
        UTL::PAUSE_500;
        for(const auto& magObjIt:allMagnetData)
        {
            message("\n", "Checking Chids for ", magObjIt.first);
            for(auto& it2:magObjIt.second.pvMonStructs)
                checkCHIDState(it2.second.CHID, ENUM_TO_STRING(it2.first));
            for(auto& it2:magObjIt.second.pvComStructs)
                checkCHIDState(it2.second.CHID, ENUM_TO_STRING(it2.first));
        }
        message("");
        UTL::STANDARD_PAUSE;
    }
    else if (status == ECA_NORMAL)
        allChidsInitialised = true;  /// interface base class member
}
//______________________________________________________________________________
void magnetInterface::addChannel(const std::string& pvRoot,
                                 magnetStructs::pvStruct& pv)
{
    std::string s1 = pvRoot + pv.pvSuffix;
    ca_create_channel(s1.c_str(), nullptr, nullptr, UTL::PRIORITY_0,&pv.CHID);
    debugMessage("Create channel to ", s1);
}
//______________________________________________________________________________
bool magnetInterface::Is_psuRoot_SAME_AS_PV_ROOT(const std::string& root) const
{
    if(root == magnetStructs::SAME_AS_PV_ROOT)
        return true;
    return false;
}
//______________________________________________________________________________
bool magnetInterface::Is_PSU_PV(magnetStructs::MAG_PV_TYPE pv)const
{
    switch(pv)
    {
        case magnetStructs::MAG_PV_TYPE::RPOWER:
            return true;
        case magnetStructs::MAG_PV_TYPE::SPOWER:
            return true;
        case magnetStructs::MAG_PV_TYPE::RILK:
            return true;
        default:
            return false;
    }
    return false;
}
//______________________________________________________________________________
void magnetInterface::startMonitors()
{
    continuousMonitorStructs.clear();
    for(auto&& it:allMagnetData)
    {
        for(auto&& it2:it.second.pvMonStructs)
        {
            continuousMonitorStructs.push_back(new magnetStructs::monitorStruct());
            continuousMonitorStructs.back()->monType = it2.first;
            continuousMonitorStructs.back()->objName = it.second.name;
            continuousMonitorStructs.back()->interface = this;
            ca_create_subscription(it2.second.CHTYPE,
                                   it2.second.COUNT,
                                   it2.second.CHID,
                                   it2.second.MASK,
                                   magnetInterface::staticEntryMagnetMonitor,
                                   (void*)continuousMonitorStructs.back(),
                                   &continuousMonitorStructs.back()->EVID);
            debugMessage("Adding monitor for ",it.second.name, " ",
                         ENUM_TO_STRING(it2.first));
        }
    }
    int status = sendToEpics("ca_create_subscription",
                             "Succesfully Subscribed to Magnet Monitors",
                             "!!TIMEOUT!! Subscription to Magnet monitors failed");
    if (status == ECA_NORMAL)
        allMonitorsStarted = true;
}
//______________________________________________________________________________
void magnetInterface::staticEntryMagnetMonitor(const event_handler_args args)
{
    magnetStructs::monitorStruct*ms =
        static_cast<magnetStructs::monitorStruct *>(args.usr);
    switch(ms->monType)
    {
        case magnetStructs::MAG_PV_TYPE::READI:
            ms->interface->updateRI( getDBRdouble(args), ms->objName);
            break;
        case magnetStructs::MAG_PV_TYPE::GETSETI:
            ms->interface->updateGetSI( getDBRdouble(args), ms->objName);
            break;
        case magnetStructs::MAG_PV_TYPE::RPOWER:
            // see manual DBR_ENUM	dbr_enum_t	16 bit unsigned integer
            ms->interface->updatePSUSta(getDBRunsignedShort(args), ms->objName);
            break;
        case magnetStructs::MAG_PV_TYPE::RILK:
            ms->interface->updateRILK(getDBRunsignedShort(args), ms->objName);
            break;
        default:
            ms->interface->debugMessage
                            ("!!! ERROR !!! Unknown Monitor Type passed to "
                             "magnetInterface::staticEntryMagnetMonitor");
            break;
    }
}
//______________________________________________________________________________
void magnetInterface::updateRI(const double value,const std::string& magName)
{
    allMagnetData.at(magName).riWithPol = roundToN(value,UTL::FOUR_SIZET);
    allMagnetData.at(magName).SETIequalREADI
        = areSame(allMagnetData.at(magName).riWithPol,
                  allMagnetData.at(magName).siWithPol,
                  allMagnetData.at(magName).riTolerance);
}
//______________________________________________________________________________
void magnetInterface::updateGetSI(const double value,const std::string&magName)
{
    allMagnetData.at(magName).siWithPol = value;
}
//______________________________________________________________________________
void magnetInterface::updateRILK(const unsigned short value,const std::string&magName)
{
    if(entryExists(allMagnetData,magName))
    {
        switch(value)
        {
            case UTL::ZERO_US:
                allMagnetData.at(magName).iLock
                    = magnetStructs::MAG_ILOCK_STATE::GOOD;
                break;
            case UTL::ONE_US:
                allMagnetData.at(magName).iLock
                    = magnetStructs::MAG_ILOCK_STATE::BAD;
                break;
            default:
                allMagnetData.at(magName).iLock
                    = magnetStructs::MAG_ILOCK_STATE::BAD;
        }
    }
}
//______________________________________________________________________________
void magnetInterface::updatePSUSta(const unsigned short value,const std::string&magName)
{
    if(entryExists(allMagnetData,magName))
    {
        switch(value)
        {
            case UTL::ZERO_US:
                allMagnetData.at(magName).psuState
                    = magnetStructs::MAG_PSU_STATE::OFF;
                break;
            case UTL::ONE_US:
                allMagnetData.at(magName).psuState
                    = magnetStructs::MAG_PSU_STATE::ON;
                break;
            default:
                allMagnetData.at(magName).psuState
                    = magnetStructs::MAG_PSU_STATE::ERROR;
        }
    }
}
//           ___  ____
//  ___     / __)(_  _)
// (___)    \__ \ _)(_
//          (___/(____)
//
// wrapper functions that interface to the main setSI function setSI_MAIN()
//______________________________________________________________________________
bool magnetInterface::setSI(const std::string& magName, const double value)
{// yay - c++11 has initilization lists ...
    const vec_s mags = {magName};
    const vec_d vals = {value  };
    return setSI(mags, vals);
}
//______________________________________________________________________________
bool magnetInterface::setSI(const vec_s& magNames,const vec_d& values)
{
    bool carryOn = false;
    vec_s mags;
    vec_d vals;
    if(magNames.size() == values.size())
    {
        size_t counter = UTL::ZERO_SIZET;
        for(auto&& it:magNames)
        {
            if(entryExists(allMagnetData,it))
            {
                if(isNotDegaussing(it))
                {
                    mags.push_back(it);
                    vals.push_back(values[counter]);
                    message("ADDED ", it, " value ",values[counter]);
                    carryOn = true;
                }
                else
                {
                    message("ERROR: In setSI: ", it, " is degaussing.");
                }
            }
            else
            {
                message("ERROR: In setSI: ", it, " does not exist.");
            }
            ++counter;
        }
        if(carryOn)
        {
            setSI_MAIN(mags,vals);
        }
    }
    else
    {
        message("ERROR: In setSI: Passed number of magnets "
                "!= Passed number of Values");
    }
    return carryOn;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::setSI(const vec_s& magNames,
                                              const vec_d& values,
                                              const vec_d& tolerances,
                                              const size_t timeOUT)
{
    bool carryOn = setSI(magNames, values);
    vec_s magsToReturn;
    if(carryOn)
    {
        magsToReturn =
            waitForMagnetsToSettle(magNames, values, tolerances, wait_time);

        if(magsToReturn.size() == magNames.size())
            message("All Magnets Probably Settled.");
        else
            message("All Magnets Did NOT Settle.");
    }
    return magsToReturn;
}
//______________________________________________________________________________
bool magnetInterface::setSI(const std::string& magName,
                            const double value,
                            const double tolerance,
                            const size_t timeOUT)
{
    const vec_s mags = {magName};
    const vec_d vals = {value  };
    const vec_d tols = {tolerance};
    const vec_s magsRet = setSI(mags, vals, tols, timeOUT);
    if(magsRet.front() == magName)
        return true;
    else
        return false;
}
//_____________________________________________________________
bool magnetInterface::setSI_MAIN(const vec_s& magNames,
                                 const vec_d& values)
{
    /*
        THIS IS THE MAIN SET SI FUNCTION
        all values sent here should have been checked that they exist
        THIS IS THE ONLY SI FUNCTION THAT ACTUALLY TALKS TO EPICS
    */
    //debugMessage("setSI_MAIN called");
    bool ret = false;
    using namespace magnetStructs;
    if(shouldStartEPICs)
    {
        if(magNames.size() == values.size())
        {
            auto m = magNames.cbegin();
            auto v = values.cbegin();
            for( ;
                m != magNames.cend() && v != values.cend();
                ++m, ++v)
            {
                //debugMessage("sending ", *m, " value ",  *v);
                ca_put(
                    allMagnetData.at(*m).pvComStructs.at(MAG_PV_TYPE::SETI).CHTYPE,
                    allMagnetData.at(*m).pvComStructs.at(MAG_PV_TYPE::SETI).CHID,
                    &(*v));
            }
            //debugMessage("calling sendToEpics");
            int status = sendToEpics("ca_put", "", "Timeout sending SI values");
            if (status == ECA_NORMAL)
                ret = true;
        }
    }
    return ret;
}
//______________________________________________________________________________
//
//  __   __
// |__) /__` |  |
// |    .__/ \__/
//
//______________________________________________________________________________
bool magnetInterface::switchONpsu(const std::string& magName)
{
    const vec_s mags = { magName };
    return switchONpsu(mags);
}
//______________________________________________________________________________
bool magnetInterface::switchONpsu(const vec_s& magNames)
{   message("switchONpsu called");
    vec_s mags;
    for(auto&& it:magNames)
        if(entryExists(allMagnetData, it))
            if(isOFF(it))
                mags.push_back(it);
    return togglePSU(mags,magnetStructs::MAG_PSU_STATE::ON);
}
//______________________________________________________________________________
bool magnetInterface::switchOFFpsu(const std::string&magName)
{
    const vec_s mags = {magName};
    return switchOFFpsu(mags);
}
//______________________________________________________________________________
bool magnetInterface::switchOFFpsu(const vec_s&magNames)
{   //message("switchOFFpsu called");
    vec_s mags;
    for(auto&& it:magNames)
        if(entryExists(allMagnetData, it))
            if(isON(it))
                mags.push_back(it);
    return togglePSU(mags,magnetStructs::MAG_PSU_STATE::OFF);
}
//______________________________________________________________________________
bool  magnetInterface::togglePSU(const vec_s& magNames,
                                 magnetStructs::MAG_PSU_STATE newState)
{
    /*  THIS ASSUMES magNames EXIST,
        as this function is callled from higher functions
    */
    message("togglePSU called");
    bool ret = false;
    unsigned short comm = (newState == magnetStructs::MAG_PSU_STATE::OFF)
                        ? PSU_OFF:PSU_ON;
    //message("comm = ", comm);
    using namespace magnetStructs;
    for(auto&& it:magNames)
    {
        //message("magNames ", it);
        ca_put(
            allMagnetData.at(it).pvComStructs.at(MAG_PV_TYPE::SPOWER).CHTYPE,
            allMagnetData.at(it).pvComStructs.at(MAG_PV_TYPE::SPOWER).CHID,
            &comm);
    }
    message("sendToEpics ");
    int status = sendToEpics("ca_put", "",
                             "Timeout trying to send new PSU states.");
    if(status == ECA_NORMAL)
    {
        ret = true;
        message("status return was ECA_NORMAL");
    }
    return ret;
}
//______________________________________________________________________________
//
//  __   ___  __             __   __
// |  \ |__  / _`  /\  |  | /__` /__`
// |__/ |___ \__> /~~\ \__/ .__/ .__/
//
//______________________________________________________________________________
bool magnetInterface::isDegaussing(const std::string& magName)const
{
    if (entryExists(isDegaussingMap, magName))
        return (bool)isDegaussingMap.at(magName);
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isNotDegaussing(const std::string& magName) const
{
    return !isDegaussing(magName);
}
//______________________________________________________________________________
bool magnetInterface::entryExistsAndIsDegaussing(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName)&& isDegaussing(magName))
        return true;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::entryExistsAndIsNotDegaussing(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName)&& isNotDegaussing(magName))
        return true;
    else
        return false;
}
//______________________________________________________________________________
size_t magnetInterface::degaussAll(bool resetToZero)
{
    return deGauss(getMagnetNames(),resetToZero);
}
//______________________________________________________________________________
size_t magnetInterface::deGauss(const std::string& mag, bool resetToZero)
{
    message("1");
    const vec_s mags = { mag };
    return deGauss(mags, resetToZero);
}
//______________________________________________________________________________
size_t magnetInterface::deGauss(const  vec_s& mag, bool resetToZero)
{
    message("2");

    killFinishedDegaussThreads();
    vec_s magToDegChecked;
    for(auto&& it:mag)
    {
        /* is it a real magnet */
        if(entryExists(allMagnetData, it))
        {
            /*/// it is a real magnet that is or has been degaussed  */
            //message(it, " exists");

            if(entryExists(isDegaussingMap, it))
            {
                //message(it, " exists");
                if(isDegaussingMap.at(it))
                {
                    message(it, " is still degaussing ");
                }
                else
                {
                    /*
                        it's not currently degaussing so add to the
                        checkedMagnet list
                    */
                    message(it, " is not degaussing");
                    magToDegChecked.push_back(it);
                }
            }
            else
            {
                isDegaussingMap[it] = true;
                magToDegChecked.push_back(it);
            }
        }
        else
        {
            message("ERROR: in deGauss", it, " name not known, can't degauss.");
        }
    }
    message("2AA");

    if(magToDegChecked.size()> UTL::ZERO_SIZET)//MAGIC_NUMBER
    {
        degaussStructsMap[degaussNum].resetToZero   = resetToZero;
        degaussStructsMap[degaussNum].interface     = this;
        degaussStructsMap[degaussNum].magsToDeguass = magToDegChecked;
        degaussStructsMap[degaussNum].key           = degaussNum;
        degaussStructsMap[degaussNum].machineArea   = myMachineArea;
        degaussStructsMap[degaussNum].thread        =
            new std::thread(staticEntryDeGauss, std::ref(degaussStructsMap[degaussNum]));
        ++degaussNum;
        message("3B");

    }
    return degaussNum;
}
//______________________________________________________________________________
void magnetInterface::killFinishedDegaussThreads()
{
    /*
    http://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it
    */
    for(auto&& it =  degaussStructsMap.cbegin();
               it != degaussStructsMap.cend() /* not hoisted */; /* no increment */)
    {
        bool shouldKill = true;
        for(auto&& it2:it->second.magsToDeguass)
            if(isDegaussingMap.at(it2))
                shouldKill = false;
        if(shouldKill)
        {
            /*
            join before deleting...
            http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
            */
            it->second.thread->join();
            delete it->second.thread;
            degaussStructsMap.erase(it++);
        }
        else
        {
            ++it;
        }
    }
    message("FIN killFinishedDegaussThreads");

}
//______________________________________________________________________________
void magnetInterface::killDegaussThread(size_t N)
{   /*
    Once you've started degaussing then you have to wait for it to finish...
    ha!
http://stackoverflow.com/questions/13893060/i-want-to-kill-a-stdthread-using-its-thread-object
    */
}
//______________________________________________________________________________
void magnetInterface::staticEntryDeGauss(const magnetStructs::degaussStruct& ds)
{
    /*
        This is the main degauss function
        Checks of input data have already been done
    */
    // start clock
    time_t timeStart = ds.interface->timeNow();

    ds.interface->attachTo_thisCAContext();
    ds.interface->printDegauss();

    ds.interface->debugMessage("\n",
                               "\tDEGAUSS UPDATE: Attempting to Degauss: ","\n");

    vec_s magToDegOriginal = ds.magsToDeguass;
    ds.interface->debugMessage("\n",
                               "\tDEGAUSS UPDATE: Saving Current Settings","\n");
    magnetStructs::magnetStateStruct originalState;
    originalState.machineArea = ds.machineArea;
    originalState = ds.interface->getCurrentMagnetState(magToDegOriginal);
    /*
        switch on the magnets that are to be degaussed
    */
    ds.interface->switchONpsu(magToDegOriginal);
    UTL::STANDARD_PAUSE;
    /*
        now check which magnets have actually switched on
        and add them to the magToDeg
    */
    vec_s magToDeg;
    vec_s lostMagnets;
    for(auto&& it:magToDegOriginal)
    {
        if(ds.interface->isON(it))
        {
            magToDeg.push_back(it);
        }
        else
        {
            ds.interface->message("\tDEGAUSS ERROR:", it,
                                  " is not switched on.");
        }
    }
    if(magToDeg.size() != magToDegOriginal.size())
    {
        ds.interface->getSetDifference(magToDegOriginal,
                                       magToDeg,
                                       lostMagnets);
        ds.interface->message("\n",
                              "\tDEGAUSS UPDATE: Error Switching on:");
        for(auto&& it:lostMagnets)
        {
            ds.interface->message("\t             :",it);
        }
        ds.interface->message("\n",
                              "\t             :These magnets will not be degaussed.");
    }
    ds.interface->message("\n",
                          "\tDEGAUSS UPDATE: Vectors Initialised Starting Degaussing",
                          "\n");
    ds.interface->message("num degauss steps = ",
                          ds.interface->allMagnetData.at(magToDeg.front()).numDegaussSteps);

    /*
        local copies of data to update during degauss routine
        magToDegOLD is so we can keep track of magnets
        lost during the degaussing process
    */
    vec_s magToDegOLD = ds.magsToDeguass;
    vec_d values, tolerances;
	for(size_t j = UTL::ZERO_SIZET;
               j <ds.interface->allMagnetData.at(magToDeg.front()).numDegaussSteps;
               ++j)
    {
        if(magToDeg.size() == UTL::ZERO_SIZET)
        {
           ds.interface->message("\n",
                                 "\tDEGAUSS UPDATE: Degaussing Has FAILED - "
                                 "all magnets lost during the procedure - ;o(","\n");
           break;
        }
        /* clear local data vectors */
        values.clear();
        tolerances.clear();
        /* assign next values */
        for(auto&& it:magToDeg)
        {
            values.push_back(ds.interface->allMagnetData.at(it).degValues[j]);
            tolerances.push_back(ds.interface->allMagnetData.at(it).degTolerance);
            ds.interface->message(it," ",values[j]," ",tolerances[j]);
        }
        //ds.interface->message("get vals");
        /* set th edesired current */
        ds.interface->setSI_MAIN(magToDeg, values);

        /* wait fo rth emagnets to settle */
        magToDeg = ds.interface->waitForMagnetsToSettle(magToDeg,
                                                        values,
                                                        tolerances,
                                                        ds.interface->wait_time);
        /* check for lost magnets */
        if(magToDeg.size() != magToDegOLD.size())
        {
            ds.interface->getSetDifference(magToDegOriginal, magToDeg, lostMagnets);
            for(auto&& it:lostMagnets)
            {
                ds.interface->message("\tDEGAUSS ERROR:",
                                      it,
                                      " lost during degaussing, "
                                      "SI probably did not settle.");
            }
        }
        /*
            set list old list of magnets, magToDegOLD,
            to compare with next iteration
        */
        magToDegOLD = magToDeg;
        ds.interface->message("\n",
                              "\tDEGAUSS UPDATE: STAGE ",
                              j + UTL::ONE_SIZET,
                              " COMPLETE", "\n");
    }
    /* show results */
    ds.interface->printDegaussResults(magToDeg, magToDegOriginal);
    /*
        reset to zero if required
        or, reapply previous settings
    */
    if(ds.resetToZero)
    {
        ds.interface->message("\tDEGAUSS UPDATE: Re-setting zero.");
        vec_d zeros(magToDeg.size(), UTL::ZERO_DOUBLE);
        ds.interface->setSI_MAIN(magToDeg, zeros);
    }
    else
    {
        ds.interface->applyMagnetStateStruct(originalState);
    }
    /*
        clean-up, set degaussing bit to false
    */
    ds.interface->detachFrom_thisCAContext() ;
    for(auto&& it:magToDegOriginal)
    {
        ds.interface->isDegaussingMap.at(it) = false;
    }
    /*
        print finish and time taken
    */
    time_t timeFinish = ds.interface->timeNow();
    ds.interface->printFinish();
    ds.interface->message("Degaussing took ",
                          timeFinish- timeStart,
                          " seconds.");
}
//______________________________________________________________________________
void magnetInterface::getDegaussValues(vec_s& magToDeg,
                                       vec_d& values,
                                       vec_d& tolerances,
                                       size_t step)const
{
    /*
        get the values used for SI when degaussing a magnet
        theres no need to check magnetType, but
    */
    for(auto&& it:magToDeg)
    {
        if(isACor(it))
        {
            values.push_back(allMagnetData.at(it).degValues[step]);
            tolerances.push_back(allMagnetData.at(it).degTolerance);
        }
        else if(isAQuad(it))
        {
            values.push_back(allMagnetData.at(it).degValues[step]);
            tolerances.push_back(allMagnetData.at(it).degTolerance);
        }
        else if(isADip(it))
        {
            values.push_back(allMagnetData.at(it).degValues[step]);
            tolerances.push_back(allMagnetData.at(it).degTolerance);
        }
        else if(isABSol(it))
        {
            values.push_back(allMagnetData.at(it).degValues[step]);
            tolerances.push_back(allMagnetData.at(it).degTolerance);;
        }
        else if(isASol(it))
        {
            values.push_back(allMagnetData.at(it).degValues[step]);
            tolerances.push_back(allMagnetData.at(it).degTolerance);
        }
        else
        {
            message("ERROR: From getDegaussValues: ",
                    it,
                    " MAG_TYPE not known!");
        }
    }
}
//______________________________________________________________________________
void magnetInterface::printDegaussResults(const vec_s& magToDegSuccess,
                                          const vec_s& magToDegOriginal)
{
    if(magToDegSuccess.size() == magToDegOriginal.size())
    {
        message("\n","\tDEGAUSS SUMMARY: ALL MAGNETS DEGUASSED","\n");
        printVec(  "\t        SUCCESS: ", magToDegSuccess, UTL::FOUR_SIZET);
    }
    else
    {
        message("\n","\tDEGAUSS SUMMARY: SOME MAGNETS FAILED TO DEGUASS ","\n");

        vec_s failedToDeg;

        getSetDifference(magToDegOriginal, magToDegSuccess, failedToDeg);

        message("\n","\tDEGAUSS SUMMARY: ALL MAGNETS ***DID NOT*** DEGUASS","\n");

        printVec("\t        SUCCESS: ", magToDegSuccess, UTL::FOUR_SIZET);
        printVec("\t         FAILED: ", failedToDeg, UTL::FOUR_SIZET);
    }
}
//______________________________________________________________________________
void magnetInterface::printVec(const std::string& p1,
                               const vec_s& v,
                               size_t numPerLine)const
{
    size_t counter = UTL::ZERO_SIZET;
    std::stringstream s;
    for(auto&& it:v)
    {
        ++counter;
        s <<it <<" ";
        if(counter % numPerLine == UTL::ZERO_SIZET)
        {
            message(1);
            message(p1 , s.str());
            counter = UTL::ZERO_SIZET;
            s.str(std::string());
        }
    }
    /* print remaining entries...*/
    if(UTL::ZERO_SIZET <counter && counter < numPerLine)
        message(p1 , s.str());
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::waitForMagnetsToSettle(const vec_s&mags,
                                                               const vec_d&values,
                                                               const vec_d&tolerances,
                                                               const time_t waitTime)
{
    /*
     This is surprisingly complicated.
     As there are NO guarantees that RI will get close to SI,
     we have to check if RI is changing.
     Also, we have to check if RI is supposed to be zero,
      because sometimes we hang around at zero while flipping
      before setting the SI value we actually want
     I've done the conditional flags very verbose
     just to make sure i follow what I'm doing
    */
    bool timeOut     = false;

    vec_d oldRIValues(mags.size(), UTL::DUMMY_DOUBLE);
    vec_d currentRIValues(mags.size());
    vec_b magnetSettledState(mags.size(), false);
    vec_s settledMags;

    /*
      To test if we are supposed to be setting zero.
      have a vector of bool indicating if they actually ARE being set to zero
    */

    vec_b settingZero;

    for(auto&& it:values)
    {
        settingZero.push_back(it == UTL::DUMMY_DOUBLE);
    }

    time_t timeStart = timeNow();

    while(true)
	{
        currentRIValues = getRI(mags);
        bool shouldBreak = true;

        for(size_t i = UTL::ZERO_SIZET; i < mags.size(); ++i)
        {
            /* magent has settled */
            if(magnetSettledState[ i ])
            {

            }
            else
            {
                if(isRIequalVal(mags[i], values[i], tolerances[i])) // The acid test!
                {
                    magnetSettledState[i] = true;
//                    debugMessage(mags[i], " RI == val. SETTLED = True."
//                       , currentRIValues[i], ", ", values[i], ", ", tolerances[i]);
                }
                else if(settingZero[i]) // We are supposed to be setting zero....
                {

                    if(areSame(oldRIValues[i], currentRIValues[i], tolerances[i]))
                    {
                        magnetSettledState[i] = true;
//                        debugMessage(mags[i],
//                          " is setting 0.0 and RI_new == RI_old. SETTLED = True ",
//                          currentRIValues[i], ", ", oldRIValues[i], ", ", tolerances[i]);
                    }
                }
                else // we are not setting zero...
                {
                    if(areSame(oldRIValues[i],  currentRIValues[i],tolerances[i]) &&
                       areSame(UTL::ZERO_DOUBLE,currentRIValues[i],tolerances[i]))
                    {
                        /*
                            RI is not changing, it's at zero, but zero isn't
                            the final RI value we expect.
                        */
                    }
                    else if(areSame(oldRIValues[i], currentRIValues[i], tolerances[i]) &&
                            areNotSame(UTL::ZERO_DOUBLE, currentRIValues[i], tolerances[i]))
                    {
                        magnetSettledState[i] = true;
//                        debugMessage(mags[i],
//                          " RI_new != 0.0&& RI_new == RI_old RI. SETTLED = True ",
//                           currentRIValues[i], ", ", oldRIValues[i],
//                           ", ", tolerances[i]);
                    }
                }
            }
        }
        /* check if all magnet shave settled */
        for(auto&& it:magnetSettledState)
        {
            if(!it)
            {
                shouldBreak = false;
            }
        }
        /* check if time ran out */
        if(timeNow() - timeStart> waitTime)
        {
            timeOut = true;
        }

        if(shouldBreak)
        {
            //debugMessage("I think the magnets have settled Breaking out of loop.");
            break;
        }
        /* save current values to see check if settled */
        oldRIValues = currentRIValues;
        if(timeOut)
        {
            debugMessage("Timeout waiting for the magnets To Settle.");
            break;
        }
        /*  really 2000 milliseconds while we wait for RI to update>>>> ?
            YES!!
        */
        pause_2000();
	} /// while

    for(size_t i = UTL::ZERO_SIZET; i <mags.size(); ++i)
    {
        if(magnetSettledState[i])
        {
            settledMags.push_back(mags[i]);
        }
    }
    return settledMags;
}
//______________________________________________________________________________
bool magnetInterface::isAMagType(const std::string& mag,
                                 const magnetStructs::MAG_TYPE t)const
{
    if(entryExists(allMagnetData, mag))
        return allMagnetData.at(mag).magType == t;
    else
        return false;
}
//______________________________________________________________________________
bool magnetInterface::isAQuad(const std::string& magName)const
{
    return isAMagType(magName,magnetStructs::MAG_TYPE::QUAD);
}
//______________________________________________________________________________
bool magnetInterface::isADip(const std::string& magName)const
{
    return isAMagType(magName,magnetStructs::MAG_TYPE::DIP);
}
//______________________________________________________________________________
bool magnetInterface::isABSol(const std::string& magName)const
{
    return isAMagType(magName,magnetStructs::MAG_TYPE::BSOL);
}
//______________________________________________________________________________
bool magnetInterface::isASol(const std::string& magName)const
{
    return isAMagType(magName,magnetStructs::MAG_TYPE::SOL);
}
//______________________________________________________________________________
bool magnetInterface::isAHCor(const std::string& magName)const
{
    return isAMagType(magName,magnetStructs::MAG_TYPE::HCOR);
}
//______________________________________________________________________________
bool magnetInterface::isAVCor(const std::string& magName)const
{
    return isAMagType(magName,magnetStructs::MAG_TYPE::VCOR);
}
//______________________________________________________________________________
bool magnetInterface::isACor(const std::string& magName)const
{
    if(isAVCor(magName) || isAHCor(magName))
        return  true;
    else
        return false;
}
//______________________________________________________________________________
void magnetInterface::setRITolerance(const std::string& magName, double val)
{
    if(entryExists(allMagnetData, magName))
        allMagnetData.at(magName).riTolerance = val;
}
//______________________________________________________________________________
void magnetInterface::setRITolerance(const vec_s& magNames, const vec_d& vals)
{
    for(auto i = UTL::ZERO_SIZET; i < magNames.size(); ++i)
        if(entryExists(allMagnetData, magNames[i]))
            allMagnetData.at(magNames[i]).riTolerance = vals[i];
}
//______________________________________________________________________________
double magnetInterface::getRITolerance(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).riTolerance;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
std::vector<double> magnetInterface::getRITolerance(const std::vector<std::string>&magNames)const
{
    std::vector<double> a;
    for(auto&& it:magNames)
        a.push_back(getRI(it));
    return a;
}
//_____________________________________________________________________________
bool magnetInterface::isON(const std::string& magName)const
{
    bool ans = false;
    if(entryExists(allMagnetData, magName))
        if(allMagnetData.at(magName).psuState == magnetStructs::MAG_PSU_STATE::ON)
            ans = true;
    return ans;
}
//_____________________________________________________________________________
bool magnetInterface::isOFF(const std::string& magName)const
{
    return !isON(magName);
}
//_____________________________________________________________________________
bool magnetInterface::isRIequalVal(const std::string& magName,
                                   const  double value,
                                   const double tolerance)const
{
    bool ret = false;
    if(entryExists(allMagnetData, magName))
        ret = areSame(allMagnetData.at(magName).riWithPol, value, tolerance);
    return ret;
}
//_____________________________________________________________________________
bool magnetInterface::isRIequalSI(const std::string& magName)const
{
    bool ret = false;
    if(entryExists(allMagnetData, magName))
    {
        ret = areSame(allMagnetData.at(magName).riWithPol,
                      allMagnetData.at(magName).siWithPol,
                      allMagnetData.at(magName).riTolerance);
    }
    return ret;
}
/////
/////   __   ___ ___ ___  ___  __   __
/////  / _` |__   |   |  |__  |__) /__`
/////  \__> |___  |   |  |___ |  \ .__/
/////
////____________________________________________________________________________
magnetInterface::vec_s magnetInterface::getMagnetNames()const
{
    vec_s r;
    for(auto&& it:allMagnetData)
        r.push_back(it.first);
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//_____________________________________________________________________________
magnetInterface::vec_s magnetInterface::getQuadNames()const
{
    vec_s r;
    for(auto&& it:allMagnetData)
        if(isAQuad(it.first))
            r.push_back(it.first);
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//_____________________________________________________________________________
magnetInterface::vec_s magnetInterface::getHCorNames()const
{
    vec_s r;
    for(auto&& it:allMagnetData)
        if(isAHCor(it.first))
            r.push_back(it.first);
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getVCorNames()const
{
    vec_s r;
    for(auto&& it:allMagnetData)
        if(isAVCor(it.first))
            r.push_back(it.first);
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getDipNames()const
{
    vec_s r;
    for(auto&& it:allMagnetData)
        if(isADip(it.first))
            r.push_back(it.first);
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
magnetInterface::vec_s magnetInterface::getSolNames()const
{
    vec_s r;
    for(auto&& it:allMagnetData)
        if(isASol(it.first) || isABSol(it.first))
            r.push_back(it.first);
    r.erase(std::remove(r.begin(), r.end(), dummyName), r.end());
    return r;
}
//______________________________________________________________________________
double magnetInterface::getSI(const std::string&magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).siWithPol;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
magnetInterface::vec_d magnetInterface::getSI(const vec_s&magNames)const
{
    vec_d a;
    for(auto&& it:magNames)
        a.push_back(getSI(it));
    return a;
}
//______________________________________________________________________________
double magnetInterface::getRI(const std::string&magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).riWithPol;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
magnetInterface::vec_d magnetInterface::getRI(const vec_s&magNames)const
{
    vec_d a;
    for(auto&& it:magNames)
        a.push_back(getRI(it));
    return a;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getCurrentMagnetState()const
{
    magnetStructs::magnetStateStruct ret;
    ret.machineArea = myMachineArea;
    for(auto&& it:allMagnetData)
    {
        if(it.first != dummyName )
        {
            ret.magNames.push_back(it.first);
            ret.siValues.push_back(it.second.siWithPol);
            ret.riValues.push_back(it.second.riWithPol);
            ret.psuStates.push_back(it.second.psuState);
            ++ret.numMags;
        }
    }
    return ret;
}
//______________________________________________________________________________
magnetStructs::magnetStateStruct magnetInterface::getCurrentMagnetState(const vec_s&s)const
{
    magnetStructs::magnetStateStruct ret;
    ret.machineArea = myMachineArea;
    for(auto&& it:s)
    {
        if(entryExists(allMagnetData, it))
        {
            if(it != dummyName )
            {
                ret.magNames.push_back(it);
                ret.siValues.push_back(allMagnetData.at(it).siWithPol);
                ret.riValues.push_back(allMagnetData.at(it).riWithPol);
                ret.psuStates.push_back(allMagnetData.at(it).psuState);
                ++ret.numMags;
            }
        }
    }
    return ret;
}
//______________________________________________________________________________
void magnetInterface::applyMagnetStateStruct(const magnetStructs::magnetStateStruct& magState)
{
    if(shouldStartEPICs)
    {
        if(magState.machineArea == myMachineArea)
        {
            message("applyMagnetStateStruct");
            vec_s magsToSwitchOn, magsToSwitchOff;
            for(size_t i = UTL::ZERO_SIZET; i <magState.numMags; ++i)
            {
                message("Found ", magState.magNames[i]);
                if(magState.psuStates[i] == magnetStructs::MAG_PSU_STATE::OFF)
                {
                    message("Switching Off ",magState.magNames[i]);
                    magsToSwitchOff.push_back(magState.magNames[i]);
                }
                else if(magState.psuStates[i]  == magnetStructs::MAG_PSU_STATE::ON)
                {
                    message("Switching ON ",magState.magNames[i]);
                    magsToSwitchOn.push_back(magState.magNames[i]);
                }
                else
                {
                    message("ERROR in PSU setting for ", magState.magNames[i]);
                }

            }
            switchONpsu(magsToSwitchOn);
            switchOFFpsu(magsToSwitchOff);
            setSI(magState.magNames, magState.siValues);
        }
        else
        {
            message("Can't apply magnet settings, this is a ",
                    ENUM_TO_STRING(myMachineArea),
                    " magnet controller and the magnet settings are for  ",
                    ENUM_TO_STRING(magState.machineArea));
        }
    }
    else
    {
        message("Can't apply magnet settings - we are in offline mode");
    }
}
//______________________________________________________________________________
using namespace magnetStructs;
magnetStateStruct magnetInterface::getDBURT(const std::string& fileName)const
{
    /// create a dburt object
    dburt dbr(SHOW_DEBUG_MESSAGES, SHOW_MESSAGES,myMachineArea);
    message("getting DBURT ",fileName);
    return dbr.readDBURT(fileName);
}
//______________________________________________________________________________
magnetStateStruct magnetInterface::getDBURTCorOnly(const std::string& fileName)const
{
    magnetStructs::magnetStateStruct ms1 = getDBURT(fileName);
    magnetStructs::magnetStateStruct ms2;
    ms2.machineArea = ms1.machineArea;
    size_t i = UTL::ZERO_SIZET;
    for(auto&& it:ms1.magNames)
    {
        if(isACor(it))
        {
            ms2.magNames.push_back(it);
            ms2.psuStates.push_back(ms1.psuStates[i]);
            ms2.siValues.push_back(ms1.siValues[i]);
            ++ms2.numMags;
        }
        ++i;
    }
    return ms2;
}
//______________________________________________________________________________
magnetStateStruct magnetInterface::getDBURTQuadOnly(const std::string& fileName)const
{
    magnetStructs::magnetStateStruct ms1 = getDBURT(fileName);
    magnetStructs::magnetStateStruct ms2;
    ms2.machineArea = ms1.machineArea;
    size_t i = UTL::ZERO_SIZET;
    for(auto&& it:ms1.magNames)
    {
        if(isAQuad(it))
        {
            ms2.magNames.push_back(it);
            ms2.psuStates.push_back(ms1.psuStates[i]);
            ms2.siValues.push_back(ms1.siValues[i]);
            ++ms2.numMags;
        }
        ++i;
    }
    return ms2;
}
//______________________________________________________________________________
void magnetInterface::applyDBURT(const std::string& fileName)
{
    applyMagnetStateStruct(getDBURT(fileName));
}
//______________________________________________________________________________
void magnetInterface::applyDBURTCorOnly(const std::string& fileName)
{
    applyMagnetStateStruct(getDBURTCorOnly(fileName));
}
//______________________________________________________________________________
void magnetInterface::applyDBURTQuadOnly(const std::string& fileName)
{
    applyMagnetStateStruct(getDBURTQuadOnly(fileName));
}
////______________________________________________________________________________
bool magnetInterface::writeDBURT(const magnetStructs::magnetStateStruct& ms,
                                 const std::string& fileName,
                                 const std::string& comments,
                                 const std::string& keywords)
{
    // create a dburt object
    dburt dbr(SHOW_DEBUG_MESSAGES, SHOW_MESSAGES,myMachineArea);
    // write the file with the dburt  and return the result
    return dbr.writeDBURT(ms, fileName, comments);
}
//______________________________________________________________________________
bool magnetInterface::writeDBURT(const std::string& fileName,
                                 const std::string& comments,
                                 const std::string& keywords)
{
    magnetStructs::magnetStateStruct ms = magnetInterface::getCurrentMagnetState();
    ms.machineArea = myMachineArea;
    return writeDBURT(ms, fileName, comments,keywords);
}
//______________________________________________________________________________
magnetStructs::MAG_TYPE magnetInterface::getMagType(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).magType;
    else
        return magnetStructs::UNKNOWN_MAGNET_TYPE;
}
//______________________________________________________________________________
std::vector<MAG_TYPE> magnetInterface::getMagType(const vec_s&magNames)const
{
    std::vector<magnetStructs::MAG_TYPE> a;
    for(auto&&it:magNames)
        a.push_back(getMagType(it));
    return a;
}
//______________________________________________________________________________
MAG_PSU_STATE magnetInterface::getMagPSUState(const std::string&magName)const
{
    if(entryExists(allMagnetData,magName))
        return allMagnetData.at(magName).psuState;
    else
        return magnetStructs::NONE;
}
//______________________________________________________________________________
std::vector<MAG_PSU_STATE> magnetInterface::getMagPSUState(const vec_s& magNames)const
{
    std::vector<magnetStructs::MAG_PSU_STATE> a;
    for(auto&&it:magNames)
        a.push_back(getMagPSUState(it));
    return a;
}
//______________________________________________________________________________
vec_d magnetInterface::getDegValues(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).degValues;
    else
    {
        std::vector<double> r;
        r.push_back(UTL::DUMMY_DOUBLE);
        return r;
    }
}
//______________________________________________________________________________
vvec_d magnetInterface::getDegValues(const vec_s& magNames)const
{
    std::vector<std::vector<double>> a;
    for(auto&& it:magNames)
        a.push_back(getDegValues(it));
    return a;
}
//______________________________________________________________________________
size_t magnetInterface::getNumDegSteps(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).numDegaussSteps;
    else
        return UTL::ZERO_SIZET;
}
//______________________________________________________________________________
std::vector<size_t> magnetInterface::getNumDegSteps(const vec_s& magNames)const
{
    std::vector<size_t> a;
    for(auto&& it:magNames)
        a.push_back(getNumDegSteps(it));
    return a;
}
/// BJAS ADDITIONS
//______________________________________________________________________________
double magnetInterface::getPosition(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).position;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
vec_d magnetInterface::getPosition(const vec_s& magNames)const
{
    std::vector<double> a;
    for(auto&& it:magNames)
        a.push_back(getPosition(it));
    return a;
}
//______________________________________________________________________________
vec_d magnetInterface::getFieldIntegralCoefficients(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).fieldIntegralCoefficients;
    else
    {
        std::vector<double> r;
        r.push_back(UTL::DUMMY_DOUBLE);
        return r;
    }
}
//______________________________________________________________________________
vvec_d magnetInterface::getFieldIntegralCoefficients(const vec_s& magNames)const
{
    std::vector<std::vector<double>> a;
    for(auto&& it:magNames)
        a.push_back(getFieldIntegralCoefficients(it));
    return a;
}
//______________________________________________________________________________
///
double magnetInterface::getMagneticLength(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).magneticLength;
    else
        return UTL::DUMMY_DOUBLE;
}
//______________________________________________________________________________
vec_d magnetInterface::getMagneticLength(const vec_s& magNames)const
{
    std::vector<double> a;
    for(auto&& it:magNames)
        a.push_back(getMagneticLength(it));
    return a;
}
//______________________________________________________________________________
std::string magnetInterface::getManufacturer(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).manufacturer;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string> magnetInterface::getManufacturer(const std::vector<std::string>& magNames)const
{
    std::vector<std::string>  a;
    for(auto&& it:magNames)
        a.push_back(getManufacturer(it));
    return a;
}
//______________________________________________________________________________
std::string magnetInterface::getSerialNumber(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).serialNumber;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string>  magnetInterface::getSerialNumber(const std::vector<std::string>& magNames)const
{
    std::vector<std::string>  a;
    for(auto&& it:magNames)
        a.push_back(getSerialNumber(it));
    return a;
}
//______________________________________________________________________________
std::string magnetInterface::getMagnetBranch(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return allMagnetData.at(magName).magnetBranch;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string> magnetInterface::getMagnetBranch(const std::vector<std::string>&magNames)const
{
    std::vector<std::string> a;
    for(auto&& it:magNames)
        a.push_back(getMagnetBranch(it));
    return a;

}
//______________________________________________________________________________
std::string magnetInterface::getMeasurementDataLocation(const std::string&magName)const
{
    if(entryExists(allMagnetData,magName))
        return allMagnetData.at(magName).measurementDataLocation;
    else
        return UTL::UNKNOWN_MAGNET_STRING;
}
//______________________________________________________________________________
std::vector<std::string> magnetInterface::getMeasurementDataLocation(const std::vector<std::string>&magNames)const
{
    std::vector<std::string>  a;
    for(auto&& it:magNames)
        a.push_back(getMeasurementDataLocation(it));
    return a;
}
//______________________________________________________________________________
void magnetInterface::printDegauss()const
{
    debugMessage("\n","\n");
    debugMessage("\t __   ___  __             __   __  ");
    debugMessage("\t|  \\ |__  / _`  /\\  |  | /__` /__` ");
    debugMessage("\t|__/ |___ \\__> /~~\\ \\__/ .__/ .__/ ");
    debugMessage("\n","\n");
}
//______________________________________________________________________________
void magnetInterface::printFinish()const
{
    debugMessage("\n","\n");
    debugMessage("\t  ___           __       ");
    debugMessage("\t |__  | |\\ | | /__` |__| ");
    debugMessage("\t |    | | \\| | .__/ |  | ");
    debugMessage("\n","\n");
}
//______________________________________________________________________________
const magnetStructs::magnetObject& magnetInterface::getMagObjConstRef(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
    {
        return allMagnetData.at(magName);
    }
    else
    {
        for(auto&& it:allMagnetData)
        {
            if(it.second.pvRoot == magName)
                return it.second;
        }
    }
    return allMagnetData.at(dummyName);
    /// HAVE A DUMMY MAGNET TO RETURN ON ERROR?
}
//______________________________________________________________________________
const magnetStructs::magnetObject * magnetInterface::getMagObjConstPtr(const std::string& magName)const
{
    if(entryExists(allMagnetData, magName))
        return&allMagnetData.at(magName);
    else
        return&allMagnetData.at(dummyName);
}
//______________________________________________________________________________
void magnetInterface::addDummyElementToAllMAgnetData()
{
    allMagnetData[dummyName].name = dummyName;
}
