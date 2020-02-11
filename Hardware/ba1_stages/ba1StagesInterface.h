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
//  Last edit:   06-02-2019
//  FileName:    ba1StagesInterface.h
//  Description:
//
//
//*/
#ifndef VELA_CLARA_BA1_STAGES_INTERFACE_H
#define VELA_CLARA_BA1_STAGES_INTERFACE_H
// project includes
#include "interface.h"
#include "ba1StagesStructs.h"
#include "ba1StagesConfigReader.h"
// stl includes
#include <vector>
#include <string>
//______________________________________________________________________________
class ba1StagesInterface : public interface
{
    public:
        ba1StagesInterface(const bool& show_messages,
                         const bool& show_debug_messages,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const std::string& configFile
                        );
        ba1StagesInterface& ba1StagesInterface::operator= ( const ba1StagesInterface& other ) = delete;
        ~ba1StagesInterface();


        std::map<HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string& name)const;


        std::vector<std::string> getStageNames()const;

        double getStagePosition(const std::string& stage)const;

        size_t getStageNumber(const std::string& stage)const;
        size_t getStagePrecision(const std::string& stage)const;


        double getStageSetPosition(const std::string& stage)const;

        bool setStagePosition(const std::string& stage, double val);

        bool setDevice(const std::string& stage, const std::string& device);

        double getDevicePos(const std::string& stage, const std::string& device);

        bool screenIn(const std::string& stage);
        bool screenOut(const std::string& stage);

        bool isReadPosEqualSetPos(const std::string& stage, double tolerance);

        double getMaxPos(const std::string& stage)const;
        double getMinPos(const std::string& stage)const;

        std::vector<std::string> getDevices(const std::string& stage)const;
        std::map<std::string, double>  ba1StagesInterface::getStageDeviceAndPositionMap(const std::string& stage)const;

    protected:
    private:
        /*
            All the ba1Stages data is stored in this map, keyed by the ba1Stages name
        */
        std::map<std::string, ba1StagesStructs::ba1StagesObject> allBA1StageData;
        const ba1StagesStructs::ba1StagesObject dummObject;
        ba1StagesConfigReader configReader;

        static void staticEntryBA1StageMonitor(const event_handler_args args);

        void updateBa1StageValue(const ba1StagesStructs::BA1STAGE_PV_TYPE pv,
                                 ba1StagesStructs::ba1StagesObject& stageObj,
                                 const event_handler_args& args);

        void initialise();
        bool initObjects();
        void initChids();
        void addChannel(const std::string& pvRoot, ba1StagesStructs::pvStruct& pv );
        void startMonitors();


        bool moveTo(const ba1StagesStructs::pvStruct& pvs, double value);

        std::vector<ba1StagesStructs::monitorStruct*> continuousMonitorStructs;


};
#endif // VELA_CLARA_BA1_STAGES_INTERFACE_H
