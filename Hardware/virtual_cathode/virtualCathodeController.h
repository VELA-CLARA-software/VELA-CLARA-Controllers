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

#ifndef pilaser_CONTROLLER_H_
#define pilaser_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "virtualCathodeInterface.h"
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

class virtualCathodeController : public controller
{
    public:
        // just have 1 constructor, but we have a higher level class that create these objects
        virtualCathodeController();
        virtualCathodeController(bool* show_messages, bool* show_debug_messagese,
                          const std::string & pilaserConf, const bool startVirtualMachine,
                          const bool shouldStartEPICs );
        ~virtualCathodeController( );
      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );
//        std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> getILockStates(const std::string& name);
//        std::map<HWC_ENUM::ILOCK_NUMBER,std::string> getILockStatesStr(const std::string& name);
      // any functions that return a map need a wrapper to convert to a python dictionary (?)
      // (we need the functions that return std::map types when building c++ applications)
#ifdef BUILD_DLL
//        boost::python::dict getILockStates_Py( std::string magName );
//        boost::python::dict getILockStatesStr_Py( std::string magName );
//        boost::python::dict getMagPSUStateDefinition();
#endif // BUILD_DLL
        // getters, not sure of type for these parameters (or if they will exist...)
//        double getHpos();
//        double getVpos();
//        double getIntensity();
//        // setters, not sure of type for these parameters (or if they will exist...)
//        bool setHpos(double value);
//        bool setHpos(int value);
//        bool setVpos(double value);
//        bool setVpos(int value);
//        bool setIntensity(double value);
//        bool setIntensity(int value);
//        const pilaserStructs::pilaserObject& getPILObjConstRef();
    protected:
    private:
        void initialise();
        //const bool shouldStartEPICs;
        virtualCathodeInterface localInterface;
};
#endif // VELA_MAG_CONTROLLER_H_
