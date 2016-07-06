#ifndef _VELA_CONTOLLER_H
#define _VELA_CONTOLLER_H

/// project
#include "interface.h"
#include "baseObject.h"


#ifdef BUILD_DLL
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

/// ermmm... https://wiki.python.org/moin/boost.python/HowTo#Multithreading_Support_for_my_function
class ScopedGILRelease
{
// C & D -------------------------------------------------------------------------------------------
public:
    inline ScopedGILRelease() { m_thread_state = PyEval_SaveThread(); }
    inline ~ScopedGILRelease() { PyEval_RestoreThread(m_thread_state);m_thread_state = NULL;}
private:
    PyThreadState * m_thread_state;
};
#endif

class controller : public baseObject
{
    public:

        controller();
        controller( bool show_messages, bool show_debug_messages );
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


        /// This pure virtual method MUST be overwritten in the derived controller ( making this an abstract base class)
        /// This also means the destructor need not be protected

        virtual std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name ) = 0;
        virtual std::map< VELA_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name ) = 0;
        virtual double get_CA_PEND_IO_TIMEOUT() = 0;
        virtual void   set_CA_PEND_IO_TIMEOUT( double val ) = 0;

#ifdef BUILD_DLL
        boost::python::dict getILockStatesDefinition();
#endif

    protected:

        bool SHOW_DEBUG_MESSAGES, SHOW_MESSAGES;

#ifdef BUILD_DLL
        template< class T,  class U >
        boost::python::dict enumMapToPythonDict( std::map< T, U > m )
        {
            boost::python::dict dictionary;
            for( auto & it : m )
                dictionary[ (int)it.first] = (int)it.second;
            return dictionary;
        }
        template< class T >
        boost::python::dict enumStringMapToPythonDict( std::map< T,  std::string  > m )
        {
            boost::python::dict dictionary;
            for( auto & it : m )
                dictionary[ (int)it.first] = it.second;
            return dictionary;
        }
#endif

    private:



}; // class controller


//#ifdef BUILD_DLL
//struct Base
//{
//    virtual ~Base() {}
//    virtual int f() = 0;
//};
//
//struct BaseWrap : Base, boost::python::wrapper<Base>
//{
//    int f()
//    {
//        return this->get_override("f")();
//    }
//};
//class controllerWrap : public controller, public boost::python::wrapper<controller>
//{
//    public:
//
////    controllerWrap() {}
////
////  controllerWrap(const controller& rhs)
////    : controller(rhs)
////  {}
//
//    std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name )
//    {return this->get_override("getILockStates")();}
//
//    std::map< VELA_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & name )
//    {return this->get_override("getILockStatesStr")();}
//
//    double get_CA_PEND_IO_TIMEOUT()
//    {return this->get_override("get_CA_PEND_IO_TIMEOUT")();}
//
//    void   set_CA_PEND_IO_TIMEOUT( double val )
//    { this->get_override("set_CA_PEND_IO_TIMEOUT")();}
//
//};
//#endif
//

#endif // _VELA_CONTOLLER_H
