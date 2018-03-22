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
//  FileName:    magnetInterface.h
//  Description:
//
//
//*/
#ifndef _MAGNET_INTERFACE_H_
#define _MAGNET_INTERFACE_H_
// project includes
#include "interface.h"
#include "magnetStructs.h"
#include "structs.h"
#include "magnetConfigReader.h"
// stl includes
#include <vector>
#include <string>
#include <atomic>
#include <map>
//______________________________________________________________________________
class magnetInterface : public interface
{
    public:

        using IlockMap1 = std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>;
        using IlockMap2 = std::map<HWC_ENUM::ILOCK_NUMBER,std::string>;
        using vvec_d    = std::vector<std::vector<double>>;
        using vec_s     = std::vector<std::string> ;
        using vec_d     = std::vector<double>;
        using vec_si    = std::vector<size_t>;
        using vec_b     = std::vector<bool>;

        magnetInterface::magnetInterface();
        magnetInterface(const std::string &magConf,
                         const bool startVirtualMachine,
                         const bool* show_messages_ptr,
                         const bool* show_debug_messages_ptr,
                         const bool shouldStartEPICs,
                         const HWC_ENUM::MACHINE_AREA myMachineArea);

        ~magnetInterface();
      //  magnetInterface(const magnetInterface& origin, const bool* show_messages_ptr, const bool * show_debug_messages_ptr); // add this line

        // /existentital quantifiers, and the like....
        bool isAMagType(const std::string& mag,
                        const magnetStructs::MAG_TYPE t)const;
        bool isAQuad(const std::string & magName)const;
        bool isABSol(const std::string & magName)const;
        bool isAHCor(const std::string & magName)const;
        bool isAVCor(const std::string & magName)const;
        bool isADip (const std::string & magName)const;
        bool isASol (const std::string & magName)const;
        bool isACor (const std::string & magName)const;
        bool isDegaussing(const std::string & magName)const;
        bool isNotDegaussing(const std::string & magName)const;
        bool entryExistsAndIsDegaussing(const std::string & magName)const;
        bool entryExistsAndIsNotDegaussing(const std::string & magName)const;
        bool isON(const std::string &magName)const;
        bool isOFF(const std::string &magName)const;
        bool isRIequalVal(const std::string & magName, const  double value, const double tolerance)const;
        bool isRIequalSI(const std::string & magName)const;

        /// RI tolerances set how many decimal places on RI values (irl EPICS has 9 and they are continually changing)
        void setRITolerance(const std::string & magName, const double val);
        void setRITolerance(const vec_s & magNames, const vec_d & vals);

        /// getters for names
        vec_s getMagnetNames()const;
        vec_s getQuadNames()const;
        vec_s getHCorNames()const;
        vec_s getVCorNames()const;
        vec_s getDipNames()const;
        vec_s getSolNames()const;
        /// get the states of the magnets: here in real life

        /// and get the states of the magnets: here from a file
        magnetStructs::magnetStateStruct getDBURT(const std::string & fileName)const;
        magnetStructs::magnetStateStruct getDBURTCorOnly(const std::string & fileName)const;
        magnetStructs::magnetStateStruct getDBURTQuadOnly(const std::string & fileName)const;
        // get const refs and pointers to magnetObjects
        // we allow c++ users to have full read access to the entire magnetObject data
        const magnetStructs::magnetObject &getMagObjConstRef(const std::string& magName)const;
        const magnetStructs::magnetObject *getMagObjConstPtr(const std::string& magName)const;

        /* getters for object parameters
        */
        // magnet currents
        double getSI(const std::string & magName)const;
        double getRI(const std::string & magName)const;
        vec_d getSI(const vec_s & magNames)const;
        vec_d getRI(const vec_s & magNames)const;
        // RI tolerances
        double getRITolerance(const std::string & magName)const;
        vec_d  getRITolerance(const std::vector<std::string> & magNames)const;

        magnetStructs::magnetStateStruct getCurrentMagnetState()const;
        magnetStructs::magnetStateStruct getCurrentMagnetState(const vec_s &s)const;
        magnetStructs::MAG_TYPE              getMagType(const std::string & magName)const;
        std::vector<magnetStructs::MAG_TYPE> getMagType(const vec_s & magNames)const;
//      ///
        magnetStructs::MAG_PSU_STATE              getMagPSUState(const std::string& magName)const;
        std::vector<magnetStructs::MAG_PSU_STATE> getMagPSUState(const vec_s& magNames)const;

        vec_d  getDegValues(const std::string & magName)const;
        vvec_d getDegValues(const std::vector<std::string> & magNames)const;
        size_t getNumDegSteps(const std::string & magName) const;
        vec_si getNumDegSteps(const std::vector<std::string> & magNames)const;

        vec_d              getFieldIntegralCoefficients(const std::string & magName)const;
        vvec_d getFieldIntegralCoefficients(const std::vector<std::string> & magNames)const;
      ///
        double                getPosition(const std::string & magName)const;
        vec_d getPosition(const std::vector<std::string> & magNames)const;
      ///
        double                getMagneticLength(const std::string & magName)const;
        vec_d getMagneticLength(const std::vector<std::string> & magNames)const;
      ///
        std::string              getManufacturer(const std::string & magName)const;
        vec_s getManufacturer(const std::vector<std::string> & magNames)const;
      ///
        std::string               getSerialNumber(const std::string & magName)const;
        vec_s  getSerialNumber(const std::vector<std::string> & magName)const;
      ///
        std::string               getMagnetBranch(const std::string & magName)const;
        vec_s getMagnetBranch(const std::vector<std::string> & magName)const;
      ///
        std::string               getMeasurementDataLocation(const std::string & magName)const;
        vec_s  getMeasurementDataLocation(const std::vector<std::string> & magName)const;



        /// setters for magnet current, in different flavours
        bool setSI(const std::string & magName,const double value);
        bool setSI(const vec_s & magNames     ,const vec_d& values);
        bool setSI (const std::string &magName, const double value  , const double tolerance  , const size_t timeOUT);
        vec_s setSI(const vec_s &magNames     , const vec_d & values, const vec_d & tolerances, const size_t timeOUT);
//
        /// These functions return wether the commands were sent to EPICS correctly, not if the oiperation was succesful
        bool switchONpsu (const std::string &magName);
        bool switchOFFpsu(const std::string &magName);
        bool switchONpsu (const vec_s       &magNames);
        bool switchOFFpsu(const vec_s       &magNames);
//
//        /// degaussing functions
        size_t deGauss(const std::string & mag, bool resetToZero = true);
        size_t deGauss(const vec_s & mag, bool resetToZero = true);
        size_t degaussAll(bool resetToZero = true);
        /// Force kill a degaussing thread, if you know its ID number
        void killDegaussThread(size_t N);
//


        /// apply a state struct to the machine
        void applyMagnetStateStruct(const magnetStructs::magnetStateStruct & ms );
        /// applyt a DBURT to the machine
        void applyDBURT(const std::string & fileName);
        void applyDBURTCorOnly(const std::string & fileName);
        void applyDBURTQuadOnly(const std::string & fileName);

        /// Write a DBURT
        bool writeDBURT(const magnetStructs::magnetStateStruct & ms, const std::string & fileName = "", const std::string & comments = ""  ,const std::string & keywords = "");
        bool writeDBURT(const std::string &fileName = "",const std::string &comments="",const std::string & keywords="");


//      /// Reverse types
//        magnetStructs::MAG_REV_TYPE                  getMagRevType(const std::string & magName);
//        std::vector< magnetStructs::MAG_REV_TYPE>  getMagRevType(const std::vector<std::string> & magNames);
//      ///


        /// These are pure virtual methods, so need to have some implmentation in derived classes
        IlockMap1 getILockStates(const std::string & name  )const{ IlockMap1 r;return r; }
        IlockMap2 getILockStatesStr(const std::string & name)const{ IlockMap2 r;return r; }

    private:
        // what flavor of controller am i ?
        const HWC_ENUM::MACHINE_AREA myMachineArea;
        // MOVE TO BASE CLASS
        const bool shouldStartEPICs;
        /// AllmagnetData gets a dummy magnet to return
        std::string dummyName;
        const time_t wait_time;

        const unsigned short PSU_ON,PSU_OFF;

        std::map<std::string, magnetStructs::magnetObject> allMagnetData;
        magnetConfigReader configReader; /// class member so we can pass in file path in ctor

        void killMonitor(magnetStructs::monitorStruct* ms);

        void initialise();
        bool initObjects();
        void addDummyElementToAllMAgnetData();
        void initChids();

        bool Is_PSU_PV(magnetStructs::MAG_PV_TYPE pv)const;
        bool Is_psuRoot_SAME_AS_PV_ROOT(const std::string& root)const;

        void addChannel(const std::string& pvRoot,magnetStructs::pvStruct &pv);
        void startMonitors();
//
//        magnetStructs::degaussValues degStruct;
        std::vector<magnetStructs::monitorStruct*> continuousMonitorStructs; /// vector of monitors to pass along with callback function

        static void staticEntryMagnetMonitor(const event_handler_args args);
//
        void updateRI(const double value,const std::string&magName);
        void updateGetSI(const double value,const std::string&magName);
//
//        void updateRI_WithPol(const std::string & magName);
//        void updateSI_WithPol(const std::string & magName);
        void updatePSUSta(const unsigned short value,const std::string&magName);
        void updateRILK(const unsigned short value,const std::string&magName);

        void updateRPower(const unsigned short value,const std::string&magName);
//
//
        bool setSI_MAIN(const vec_s & magNames, const vec_d& values);

        // THE MAIN TOGGLE PSU function
        bool togglePSU(const vec_s &magNames, magnetStructs::MAG_PSU_STATE newState);
        vec_s waitForMagnetsToSettle(const vec_s & mags, const vec_d & values,
                                     const vec_d & tolerances, const time_t waitTime = 45); /// MAGIC_NUMBER
//        /// DEGAUSS STUFF
        static void staticEntryDeGauss(const magnetStructs::degaussStruct & ds);
        size_t degaussNum;
//
        std::map<size_t, magnetStructs::degaussStruct> degaussStructsMap;
        std::map<std::string,  std::atomic<bool>> isDegaussingMap; // std::atomic<bool> are not CopyConstructible, so this is held locally
       // std::map<size_t, std::thread*> degaussThreadMap;
//
        void killFinishedDegaussThreads();
//
        void printDegaussResults(const vec_s & magToDegSuccess, const vec_s & magToDegOriginal);
        void getDegaussValues(vec_s & magToDeg, vec_d & values, vec_d & tolerances, size_t step)const;
//
        void printDegauss()const;
        void printFinish()const;
        void printVec(const std::string & p1, const vec_s & v, size_t numPerLine)const;
//
        ///message
};
#endif // _MAGNET_INTERFACE_H_
