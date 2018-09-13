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

#ifndef SCOPE_CONTROLLER_H
#define SCOPE_CONTROLLER_H

// project
#include "blmInterface.h"
#include "controller.h"
// stl
#include <string>
#include <vector>
#include <map>
// boost
#include <boost/circular_buffer.hpp>
#ifdef BUILD_DLL
#endif

class blmController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        blmController();// const bool show_messages = true, const bool show_debug_messages = true );
        blmController( const std::string &configFileLocation1,
                         bool& show_messages,
                         bool& show_debug_messages,
                         const bool shouldStartEPICs,
                         const bool startVirtualMachine,
                         const HWC_ENUM::MACHINE_AREA myMachineArea );
        ~blmController();

        bool monitoringTraces;
        bool monitoringNums;
        bool isMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        bool isMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        bool isNotMonitoringBLMNum( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );

        double getBLMP1( const std::string & blmName );
        double getBLMP2( const std::string & blmName );
        double getBLMP3( const std::string & blmName );
        double getBLMP4( const std::string & blmName );
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
        const blmStructs::DIAG_TYPE getDiagType( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        const std::string getDiagTypeStr( const std::string & blmName, blmStructs::SCOPE_PV_TYPE pvType );
        const blmStructs::blmTraceData & getBLMTraceDataStruct( const std::string & blmName );
        const blmStructs::blmNumObject & getBLMNumDataStruct( const std::string & blmName );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, blmStructs::SCOPE_PV_TYPE channel, size_t N );
        void monitorNumsForNShots( size_t N );
        void monitorANumForNShots( const std::string num, blmStructs::SCOPE_PV_TYPE channel, size_t N );

        std::vector< std::vector< double > > getBLMTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getBLMNums( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getBLMP1Vec( const std::string & name );
        std::vector< double > getBLMP2Vec( const std::string & name );
        std::vector< double > getBLMP3Vec( const std::string & name );
        std::vector< double > getBLMP4Vec( const std::string & name );
        boost::circular_buffer< double > getBLMNumBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::circular_buffer< double > getBLMP1Buffer( const std::string & name );
        boost::circular_buffer< double > getBLMP2Buffer( const std::string & name );
        boost::circular_buffer< double > getBLMP3Buffer( const std::string & name );
        boost::circular_buffer< double > getBLMP4Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTraceBuffer( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::circular_buffer< std::vector< double > > getBLMTR1Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTR2Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTR3Buffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTR4Buffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::string > getStrTimeStamps( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAvgNoise( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );

        std::vector< std::string > getBLMNames();
        std::vector< std::string > getBLMPVs();
        std::vector< std::string > getBLMTracePVs();
        std::vector< std::string > getBLMNumPVs();

        #ifdef BUILD_DLL
        boost::python::list getBLMNums_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getBLMP1Vec_Py( const std::string & name );
        boost::python::list getBLMP2Vec_Py( const std::string & name );
        boost::python::list getBLMP3Vec_Py( const std::string & name );
        boost::python::list getBLMP4Vec_Py( const std::string & name );
        boost::python::list getBLMNumBuffer_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getBLMP1Buffer_Py( const std::string & name );
        boost::python::list getBLMP2Buffer_Py( const std::string & name );
        boost::python::list getBLMP3Buffer_Py( const std::string & name );
        boost::python::list getBLMP4Buffer_Py( const std::string & name );
        boost::python::list getBLMTraceBuffer_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getBLMTR1Buffer_Py( const std::string & name );
        boost::python::list getBLMTR2Buffer_Py( const std::string & name );
        boost::python::list getBLMTR3Buffer_Py( const std::string & name );
        boost::python::list getBLMTR4Buffer_Py( const std::string & name );
        boost::python::list getMinOfTraces_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getMaxOfTraces_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getAreaUnderTraces_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getTimeStamps_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getStrTimeStamps_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType );
        boost::python::list getAreaUnderPartOfTrace_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getAvgNoise_Py( const std::string & name, blmStructs::SCOPE_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getBLMNames_Py();
        boost::python::list getBLMPVs_Py();
        boost::python::list getBLMTracePVs_Py();
        boost::python::list getBLMNumPVs_Py();
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
        blmInterface localInterface;
        std::vector< std::string > blmNames;

        const bool shouldStartEPICs;
        const HWC_ENUM::MACHINE_AREA machineArea;
};


#endif // blmController_H
