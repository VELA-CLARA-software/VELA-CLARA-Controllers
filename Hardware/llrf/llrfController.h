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

#ifndef LASER_CONTROLLER_H_
#define LASER_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "llrfInterface.h"
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

class llrfController : public controller
{
    public:
        /// New scheem - we just have 1 constructor, but we have a higher level class that create these objects
        llrfController();
        llrfController(const bool show_messages,const bool show_debug_messagese,
                       const std::string & laserConf,const bool startVirtualMachine,
                       const bool shouldStartEPICs,const llrfStructs::LLRF_TYPE type);
        ~llrfController( );

        llrfStructs::LLRF_TYPE getType();
        //in LLRF units
        long     getAmpRead();
        long     getAmpWrite();
        long     getPhiLLRF();

        double   getPhi();
        double   getAmp();

        bool     setPhiLLRF(long value);
        bool     setAmpLLLRF(long value);

        bool     setPhi(double value);// degrees relative to crest
        bool     setAmp(double value);// MV / m ampliutude
        // obj ref.
        const llrfStructs::llrfObject& getLLRFObjConstRef();

        double getPhiCalibration();
        double getAmpCalibration();
        double getCrestPhiLLRF(); // in LLRF units


      // These are pure virtual methods, so need to have some implmentation in derived classes
        virtual double get_CA_PEND_IO_TIMEOUT();
        virtual void   set_CA_PEND_IO_TIMEOUT( double val );
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );
      // any functions that return a map need a wrapper to convert to a python dictionary
      // (we need the functions that return std::map types when building c++ applications)
#ifdef BUILD_DLL
//        boost::python::dict getILockStates_Py( std::string magName );
//        boost::python::dict getILockStatesStr_Py( std::string magName );
//        boost::python::dict getMagPSUStateDefinition();
#endif // BUILD_DLL

    protected:
    private:

        void initialise();

        const bool shouldStartEPICs;

        llrfInterface localInterface;
};



#endif // VELA_MAG_CONTROLLER_H_
