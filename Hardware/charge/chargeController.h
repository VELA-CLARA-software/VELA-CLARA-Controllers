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

#ifndef charge_CONTROLLER_H
#define charge_CONTROLLER_H

// project
#include "chargeInterface.h"
#include "controller.h"
// stl
#include <string>
#include <vector>
#include <map>
// boost
#include <boost/circular_buffer.hpp>
#ifdef BUILD_DLL
#endif

class chargeController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        chargeController();// const bool show_messages = true, const bool show_debug_messages = true );
        chargeController( const std::string &configFileLocation1,
                          bool& show_messages,
                          bool& show_debug_messages,
                          const bool shouldStartEPICs,
                          const bool startVirtualMachine,
                          const HWC_ENUM::MACHINE_AREA myMachineArea );
        ~chargeController();

        bool monitoringTraces;
        bool monitoringNums;
        bool isMonitoringchargeTrace( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        bool isMonitoringchargeNum( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        bool isNotMonitoringchargeTrace( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        bool isNotMonitoringchargeNum( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );

        double getchargeP1( const std::string & chargeName );
        double getchargeP2( const std::string & chargeName );
        double getchargeP3( const std::string & chargeName );
        double getchargeP4( const std::string & chargeName );
        double getWCMQ();
        double getICT1Q( const int part1, const int part2 );
        double getICT2Q( const int part1, const int part2 );
        double getFCUPQ();
        double getEDFCUPQ();
        void setBufferSize( size_t bufferSize );
        void setNumBufferSize( size_t bufferSize );
        void setTraceBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
        void setTimebase( const std:: string & name, const double timebase );
        double getTimebase( const std:: string & name );
        size_t getBufferSize( const std:: string & name );
        const chargeStructs::DIAG_TYPE getDiagType( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        const std::string getDiagTypeStr( const std::string & chargeName, chargeStructs::charge_PV_TYPE pvType );
        const chargeStructs::chargeTraceData & getchargeTraceDataStruct( const std::string & chargeName );
        const chargeStructs::chargeNumObject & getchargeNumDataStruct( const std::string & chargeName );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, chargeStructs::charge_PV_TYPE channel, size_t N );
        void monitorNumsForNShots( size_t N );
        void monitorANumForNShots( const std::string num, chargeStructs::charge_PV_TYPE channel, size_t N );

        std::vector< std::vector< double > > getchargeTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getchargeNums( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getchargeP1Vec( const std::string & name );
        std::vector< double > getchargeP2Vec( const std::string & name );
        std::vector< double > getchargeP3Vec( const std::string & name );
        std::vector< double > getchargeP4Vec( const std::string & name );
        boost::circular_buffer< double > getchargeNumBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::circular_buffer< double > getchargeP1Buffer( const std::string & name );
        boost::circular_buffer< double > getchargeP2Buffer( const std::string & name );
        boost::circular_buffer< double > getchargeP3Buffer( const std::string & name );
        boost::circular_buffer< double > getchargeP4Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTraceBuffer( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::circular_buffer< std::vector< double > > getchargeTR1Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTR2Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTR3Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getchargeTR4Buffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< std::string > getStrTimeStamps( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAvgNoise( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 );

        std::vector< std::string > getchargeNames();
        std::vector< std::string > getchargePVs();
        std::vector< std::string > getchargeTracePVs();
        std::vector< std::string > getchargeNumPVs();

        #ifdef BUILD_DLL
        boost::python::list getchargeNums_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getchargeP1Vec_Py( const std::string & name );
        boost::python::list getchargeP2Vec_Py( const std::string & name );
        boost::python::list getchargeP3Vec_Py( const std::string & name );
        boost::python::list getchargeP4Vec_Py( const std::string & name );
        boost::python::list getchargeNumBuffer_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getchargeP1Buffer_Py( const std::string & name );
        boost::python::list getchargeP2Buffer_Py( const std::string & name );
        boost::python::list getchargeP3Buffer_Py( const std::string & name );
        boost::python::list getchargeP4Buffer_Py( const std::string & name );
        boost::python::list getchargeTraceBuffer_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getchargeTR1Buffer_Py( const std::string & name );
        boost::python::list getchargeTR2Buffer_Py( const std::string & name );
        boost::python::list getchargeTR3Buffer_Py( const std::string & name );
        boost::python::list getchargeTR4Buffer_Py( const std::string & name );
        boost::python::list getMinOfTraces_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getMaxOfTraces_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getAreaUnderTraces_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getTimeStamps_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getStrTimeStamps_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType );
        boost::python::list getAreaUnderPartOfTrace_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getAvgNoise_Py( const std::string & name, chargeStructs::charge_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getchargeNames_Py();
        boost::python::list getchargePVs_Py();
        boost::python::list getchargeTracePVs_Py();
        boost::python::list getchargeNumPVs_Py();
        #endif

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING
        std::map<HWC_ENUM::ILOCK_NUMBER,std::string> getILockStatesStr(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE> getILockStates(const std::string& name)const;
        double get_CA_PEND_IO_TIMEOUT() const;
        void set_CA_PEND_IO_TIMEOUT( double val );

    protected:
    private:

        void initialise();

        /// No singletons, no pointers, let's just have an object
        chargeInterface localInterface;
        std::vector< std::string > chargeNames;

        const bool shouldStartEPICs;
        const HWC_ENUM::MACHINE_AREA machineArea;
};


#endif // chargeController_H
