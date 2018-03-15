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

#ifndef gunMod_CONTROLLER_H_
#define gunMod_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "gunModInterface.h"
#include "controller.h"
// boost.python
#ifdef BUILD_DLL
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/overloads.hpp>
#endif

class gunModController : public controller
{
    public:
        // just have 1 constructor, but we have a higher level class that create these objects
        gunModController();
        gunModController(const bool show_messages, const bool show_debug_messagese,
                          const std::string & gunModConf, const bool startVirtualMachine,
                          const bool shouldStartEPICs);
        ~gunModController();
      // These are pure virtual methods, so need to have some implmentation in derived classes
        virtual double get_CA_PEND_IO_TIMEOUT();
        virtual void   set_CA_PEND_IO_TIMEOUT(double val);
        std::map<VELA_ENUM::ILOCK_NUMBER,VELA_ENUM::ILOCK_STATE> getILockStates(const std::string& name);
        std::map<VELA_ENUM::ILOCK_NUMBER,std::string> getILockStatesStr(const std::string& name);

        const rfModStructs::gunModObject& getGunObjConstRef();


        bool isErrorStateGood() const;
        bool isWarmedUp() const;
        bool isNotWarmedUp() const;
        bool isInTrig() const;
        bool isInHVOn() const;
        bool isInStandby() const;
        bool isInOff() const;
        void reset();
        bool resetAndWait(const size_t waitTime);
        rfModStructs::GUN_MOD_STATE getMainState() const;
        rfModStructs::GUN_MOD_ERR_STATE getErrorState() const;

        bool waitForModState(rfModStructs::GUN_MOD_STATE state, const time_t waitTime);

//    protected:
//    private:
//        void initialise();
        const bool shouldStartEPICs;
        gunModInterface localInterface;
};
#endif // gunMod_CONTROLLER_H_
