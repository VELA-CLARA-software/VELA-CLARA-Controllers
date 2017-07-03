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

#ifndef _VELA_CONTOLLER_H
#define _VELA_CONTOLLER_H

/// project
#include "interface.h"
#include "baseObject.h"


#ifdef BUILD_DLL
//#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
/// ermmm... https://wiki.python.org/moin/boost.python/HowTo#Multithreading_Support_for_my_function
/// never got this to work
//class ScopedGILRelease
//{
//// C & D -------------------------------------------------------------------------------------------
//public:
//    inline ScopedGILRelease() { m_thread_state = PyEval_SaveThread(); }
//    inline ~ScopedGILRelease() { PyEval_RestoreThread(m_thread_state);m_thread_state = NULL;}
//private:
//    PyThreadState * m_thread_state;
//};
#endif

class controller : public baseObject
{
    public:

        //controller();
        controller(bool show_messages, bool show_debug_messages);
        ~controller();

        /// Turn on and off debug messages and general messages

        void debugMessagesOn();
        void debugMessagesOff();
        void messagesOn();
        void messagesOff();
        void silence();
        void verbose();

        bool isSilent();
        bool isVerbose();
        bool isMessageOn();
        bool isDebugMessageOn();


        /// This pure virtual method MUST be overwritten in the derived controller (making this an abstract base class)
        /// This also means the destructor need not be protected

        virtual std::map<VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE> getILockStates(const std::string & name) = 0;
        virtual std::map<VELA_ENUM::ILOCK_NUMBER, std::string>         getILockStatesStr(const std::string & name) = 0;
        virtual double get_CA_PEND_IO_TIMEOUT() = 0;
        virtual void   set_CA_PEND_IO_TIMEOUT(double val) = 0;

#ifdef BUILD_DLL
        boost::python::dict getILockStatesDefinition();
#endif

    protected:

        bool SHOW_DEBUG_MESSAGES, SHOW_MESSAGES;

#ifdef BUILD_DLL
        template<class T,  class U>
        boost::python::dict enumMapToPythonDict(std::map<T, U> m)
        {
            boost::python::dict dictionary;
            for(auto & it : m)
                dictionary[ (int)it.first] = (int)it.second;
            return dictionary;
        }
        template<class T>
        boost::python::dict enumStringMapToPythonDict(std::map<T,  std::string > m)
        {
            boost::python::dict dictionary;
            for(auto & it : m)
                dictionary[ (int)it.first] = it.second;
            return dictionary;
        }
        template <class T>
        boost::python::list toPythonList(std::vector<T> vector)
        {
            typename std::vector<T>::iterator iter;
            boost::python::list list;
            for (iter = vector.begin(); iter != vector.end(); ++iter)
            {
                list.append(*iter);
            }
            return list;
        }
#endif

    private:



}; // class controller

#endif // _VELA_CONTOLLER_H
