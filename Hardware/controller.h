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
//               They are also a good place to put conversions from STL containers
//               to python lists and/or dictionaries
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

        controller(bool& show_messages,
                   bool& show_debug_messages,
                   const HWC_ENUM::CONTROLLER_TYPE type,
                   const std::string& name);

        controller(bool& show_messages,
                   bool& show_debug_messages,
                   const std::string& name,
                   const HWC_ENUM::CONTROLLER_TYPE type);

        controller(bool& show_messages,
                   bool& show_debug_messages,
                   const HWC_ENUM::CONTROLLER_TYPE type);

        controller(bool& show_messages,
                   bool& show_debug_messages);
        controller& controller::operator= ( const controller& other ) = delete;


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

        const std::string name;

        HWC_ENUM::CONTROLLER_TYPE getControllerType() const;
        /* The below seemed like a good idea when we first implemented it
           These pure virtual methods MUST be overwritten in the derived controller
           (making this an abstract base class)
           This also means the destructor need not be protected
        */
        virtual double get_CA_PEND_IO_TIMEOUT() const = 0;
        virtual void   set_CA_PEND_IO_TIMEOUT(const double val) = 0;

#ifdef BUILD_DLL
        boost::python::dict getILockStatesDefinition() const;
#endif
    protected:
        ~controller();
        /* these bools determine if messages can be printed or not,
           they are passed to the baseObject and used by
           all other classes in the project
        */
        bool& SHOW_DEBUG_MESSAGES;
        bool& SHOW_MESSAGES;

    private:
}; // class controller
#endif // _HWC_CONTOLLER_BASE_H_
