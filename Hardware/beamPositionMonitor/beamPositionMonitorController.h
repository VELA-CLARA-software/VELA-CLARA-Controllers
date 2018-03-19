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

#ifndef vela_BPM_CONTROLLER_H
#define vela_BPM_CONTROLLER_H

// project
#include "beamPositionMonitorStructs.h"
#include "beamPositionMonitorInterface.h"
#include "controller.h"
// stl
#include <string>
#include <vector>
// boost
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
#include <boost/circular_buffer.hpp>
#endif

class beamPositionMonitorController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        beamPositionMonitorController();// const bool show_messages = true, const bool show_debug_messages = true );
//        beamPositionMonitorController();
        beamPositionMonitorController( const std::string & configFileLocation, const bool show_messages,
                                       const bool show_debug_messages, const bool shouldStartEPICS,
                                       const bool startVirtualMachine, const VELA_ENUM::MACHINE_AREA myMachineArea );
        ~beamPositionMonitorController();

        bool isMonitoringBPMData( const std::string & name );
        bool isNotMonitoringBPMData( const std::string & name );

        double getX( const std::string & bpm );
        double getY( const std::string & bpm );
        double getQ( const std::string & bpm );
        double getXFromPV( const std::string & bpm );
        double getYFromPV( const std::string & bpm );
        double getBPMResolution( const std::string & name );
        const beamPositionMonitorStructs::bpmDataObject & getBPMDataObject( const std::string & name );
        std::vector< std::vector< double > > getBPMRawData( const std::string & bpmName );
        std::vector< double > getBPMXVec( const std::string & bpmName );
        std::vector< double > getBPMYVec( const std::string & bpmName );
        std::vector< double > getBPMQVec( const std::string & bpmName );
        std::vector< double > getTimeStamps( const std::string & bpmName );
        std::vector< std::string > getStrTimeStamps( const std::string & bpmName );
        boost::circular_buffer< std::vector< double > > getBPMRawDataBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMXBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMYBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getBPMQBuffer( const std::string & bpmName );
        boost::circular_buffer< double > getTimeStampsBuffer( const std::string & bpmName );
        boost::circular_buffer< std::string > getStrTimeStampsBuffer( const std::string & bpmName );
        long getRA1( const std::string & bpmName );
        long getRA2( const std::string & bpmName );
        long getRD1( const std::string & bpmName );
        long getRD2( const std::string & bpmName );
        long getSA1( const std::string & bpm );
        long getSA2( const std::string & bpm );
        long getSD1( const std::string & bpm );
        long getSD2( const std::string & bpm );
        void setSA1( const std::string & bpmName, long sa1 );
        void setSA2( const std::string & bpmName, long sa2 );
        void setSD1( const std::string & bpmName, long sd1 );
        void setSD2( const std::string & bpmName, long sd2 );
        void setX( const std::string & bpmName, double val );
        void setY( const std::string & bpmName, double val );
        void reCalAttenuation( const std::string & bpmName, double qScope );
        void monitorDataForNShots( size_t N, const std::string & name );
        void monitorDataForNShots( size_t N, const std::vector< std::string > & names );
        VELA_ENUM::MACHINE_AREA getMachineArea();
        VELA_ENUM::MACHINE_MODE getMachineMode();

        std::vector< std::string > getBPMNames();

        #ifdef BUILD_DLL
        boost::python::list getBPMNames_Py();
        boost::python::list getBPMRawData_Py( const std::string & bpmName );
        boost::python::list getBPMXVec_Py( const std::string & bpmName );
        boost::python::list getBPMYVec_Py( const std::string & bpmName );
        boost::python::list getBPMQVec_Py( const std::string & bpmName );
        boost::python::list getTimeStamps_Py( const std::string & bpmName );
        boost::python::list getStrTimeStamps_Py( const std::string & bpmName );
        boost::python::list getBPMRawDataBuffer_Py( const std::string & bpmName );
        boost::python::list getBPMXBuffer_Py( const std::string & bpmName );
        boost::python::list getBPMYBuffer_Py( const std::string & bpmName );
        boost::python::list getBPMQBuffer_Py( const std::string & bpmName );
        boost::python::list getTimeStampsBuffer_Py( const std::string & bpmName );
        #endif

        bool hasTrig( const std::string & bpmName );
        bool hasNoTrig( const std::string & bpmName );

        /// write a method that returns string version of enums using ENUM_TO_STRING

        VELA_ENUM::TRIG_STATE getBPMState( const std::string & bpmName );
        std::string getBPMStateStr( const std::string & name );

        double get_CA_PEND_IO_TIMEOUT();
        void set_CA_PEND_IO_TIMEOUT( double val );

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & objName );

    protected:
    private:

        void initialise();

        /// No singletons, no pointers, let's just have an object

        beamPositionMonitorInterface localInterface;

        bool shouldStartEPICS;
        const VELA_ENUM::MACHINE_AREA machineArea;

        std::vector< std::string > bpmNames;
};

#endif // beamPositionMonitorController_H
