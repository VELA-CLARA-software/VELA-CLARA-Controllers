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
//  FileName:    controller.h
//  Description: The base class for all controller objects
//               Controller objects link the interface to higher level code
//               (e.g. the python modulec classes, or other c++ applications)
//*/
#ifndef _HWC_CONTOLLER_BASE_H_
#define _HWC_CONTOLLER_BASE_H_
// project includes
#include "structs.h"
#include "baseObject.h"
#include <map>
#ifdef BUILD_DLL
// boost.python includes
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class controller : public baseObject
{
    public:

        controller(bool show_messages,
                   bool show_debug_messages,
                   const HWC_ENUM::CONTROLLER_TYPE type);
        controller::controller(bool show_messages, bool show_debug_messages);
        ~controller();

        /* Turn on and off debug messages and general messages */
        void debugMessagesOn();
        void debugMessagesOff();
        void messagesOn();
        void messagesOff();
        void silence();
        void verbose();
        /* state of messaging */
        bool isSilent() const;
        bool isVerbose() const;
        bool isMessageOn() const;
        bool isDebugMessageOn() const;
        const HWC_ENUM::CONTROLLER_TYPE controllerType;

        HWC_ENUM::CONTROLLER_TYPE getControllerType() const;

        /* The below seemed like a good idea when we first implemented it
           This pure virtual method MUST be overwritten in the derived controller
           (making this an abstract base class)
           This also means the destructor need not be protected
        */
        virtual std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE>
                getILockStates(const std::string& name) = 0;
        virtual std::map<HWC_ENUM::ILOCK_NUMBER, std::string>
            getILockStatesStr(const std::string & name) = 0;
        virtual double get_CA_PEND_IO_TIMEOUT() = 0;
        virtual void   set_CA_PEND_IO_TIMEOUT(double val) = 0;

#ifdef BUILD_DLL
        boost::python::dict getILockStatesDefinition() const;
#endif

    protected:

        /* these bools determine if messages can be printed or not,
           pointers to them are passed to th ebaseObject and used by
           all other classes in th eproject
        */
        bool SHOW_DEBUG_MESSAGES, SHOW_MESSAGES;

    private:
}; // class controller
#endif // _HWC_CONTOLLER_BASE_H_
