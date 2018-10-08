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
        bool isMonitoringBLMTrace( const std::string & blmName, blmStructs::BLM_PV_TYPE pvType );
        bool isNotMonitoringBLMTrace( const std::string & blmName, blmStructs::BLM_PV_TYPE pvType );

        void setBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
        void setTimebase( const std:: string & name, const double timebase );
        double getTimebase( const std:: string & name );
        size_t getBufferSize( const std:: string & name );
        const blmStructs::blmTraceData & getBLMTraceDataStruct( const std::string & blmName );
        void monitorTracesForNShots( size_t N );
        void monitorATraceForNShots( const std::string trace, blmStructs::BLM_PV_TYPE channel, size_t N );

        std::vector< std::vector< double > > getBLMTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< double > getBLMTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< double > getBLMCH1Waveform( const std::string & name );
        std::vector< double > getBLMCH2Waveform( const std::string & name );
        std::vector< double > getBLMCH3Waveform( const std::string & name );
        std::vector< double > getBLMCH4Waveform( const std::string & name );
        std::vector< double > getBLMCH1Time( const std::string & name );
        std::vector< double > getBLMCH2Time( const std::string & name );
        std::vector< double > getBLMCH3Time( const std::string & name );
        std::vector< double > getBLMCH4Time( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMTraceBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::circular_buffer< std::vector< double > > getBLMCH1WaveformBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH2WaveformBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH3WaveformBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH4WaveformBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH1TimeBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH2TimeBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH3TimeBuffer( const std::string & name );
        boost::circular_buffer< std::vector< double > > getBLMCH4TimeBuffer( const std::string & name );
        std::vector< double > getMinOfTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< double > getMaxOfTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< double > getAreaUnderTraces( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< double > getTimeStamps( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::circular_buffer< double > getTimeStampsBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< std::string > getStrTimeStamps( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::circular_buffer< std::string > getStrTimeStampsBuffer( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        std::vector< std::vector< double > > getPartOfTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAreaUnderPartOfTrace( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 );
        std::vector< double > getAvgNoise( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 );

        std::vector< std::string > getBLMNames();
        std::vector< blmStructs::BLM_PV_TYPE > getBLMPVs();
        std::vector< blmStructs::BLM_PV_TYPE > getBLMTimePVs();
        std::vector< blmStructs::BLM_PV_TYPE > getBLMWaveformPVs();
        std::vector< std::string > getBLMPVStrings();
        std::vector< std::string > getBLMTimePVStrings();
        std::vector< std::string > getBLMWaveformPVStrings();
        std::vector< std::string > getBLMTracePVs();

        #ifdef BUILD_DLL
        boost::python::list getBLMTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getBLMTrace_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getBLMCH1Waveform_Py( const std::string & name );
        boost::python::list getBLMCH2Waveform_Py( const std::string & name );
        boost::python::list getBLMCH3Waveform_Py( const std::string & name );
        boost::python::list getBLMCH4Waveform_Py( const std::string & name );
        boost::python::list getBLMCH1Time_Py( const std::string & name );
        boost::python::list getBLMCH2Time_Py( const std::string & name );
        boost::python::list getBLMCH3Time_Py( const std::string & name );
        boost::python::list getBLMCH4Time_Py( const std::string & name );
        boost::python::list getBLMTraceBuffer_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getBLMCH1WaveformBuffer_Py( const std::string & name );
        boost::python::list getBLMCH2WaveformBuffer_Py( const std::string & name );
        boost::python::list getBLMCH3WaveformBuffer_Py( const std::string & name );
        boost::python::list getBLMCH4WaveformBuffer_Py( const std::string & name );
        boost::python::list getBLMCH1TimeBuffer_Py( const std::string & name );
        boost::python::list getBLMCH2TimeBuffer_Py( const std::string & name );
        boost::python::list getBLMCH3TimeBuffer_Py( const std::string & name );
        boost::python::list getBLMCH4TimeBuffer_Py( const std::string & name );
        boost::python::list getMinOfTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getMaxOfTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getAreaUnderTraces_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getTimeStamps_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getTimeStampsBuffer_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getStrTimeStamps_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getStrTimeStampsBuffer_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType );
        boost::python::list getAreaUnderPartOfTrace_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getAvgNoise_Py( const std::string & name, blmStructs::BLM_PV_TYPE pvType, const int part1, const int part2 );
        boost::python::list getBLMNames_Py();
        boost::python::list getBLMPVStrings_Py();
        boost::python::list getBLMTimePVStrings_Py();
        boost::python::list getBLMWaveformPVStrings_Py();
        boost::python::list getBLMPVs_Py();
        boost::python::list getBLMTimePVs_Py();
        boost::python::list getBLMWaveformPVs_Py();
        boost::python::list getBLMTracePVs_Py();
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
