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

        void getChargeNames( std::vector< std::string >  & chargeNames );
        void setBufferSize( size_t bufferSize );
        void restartContinuousMonitoring();
//        void monitorForNShots_Vec( const std::vector< std::string > charges, size_t N );
//        void monitorForNShots( const std::string charge, size_t N );
        void cancelDataMonitors();
        size_t getBufferSize( const std::string & chargeName );
        const chargeStructs::CHARGE_DIAG_TYPE getDiagType( const std::string & chargeName );
        const std::string getDiagTypeStr( const std::string & chargeName );
        const chargeStructs::dataObject & getChargeDataStruct( const std::string & chargeName );
        std::vector< std::vector< double > > getChargeData( const std::string & name );
        boost::circular_buffer< double > getChargeBuffer( const std::string & name );
        boost::circular_buffer< double > getVoltageBuffer( const std::string & name );
        boost::circular_buffer< double > getWCMChargeBuffer();
        boost::circular_buffer< double > getWCMVoltageBuffer();
        boost::circular_buffer< double > getS02FCUPChargeBuffer();
        boost::circular_buffer< double > getS02FCUPVoltageBuffer();
        boost::circular_buffer< double > getSP1FCUPChargeBuffer();
        boost::circular_buffer< double > getSP1FCUPVoltageBuffer();
        std::vector< double > getTimeStamps( const std::string & name );
        std::vector< std::string > getStrTimeStamps( const std::string & name );
        std::vector< std::string > getChargeDiagnosticNames();
        std::vector< std::string > getChargePVs();
        double getCharge( const std::string & name );
        double getVoltage( const std::string & name );
        double getWCMCharge();
        double getWCMVoltage();
        double getS02FCUPCharge();
        double getS02FCUPVoltage();
        double getSP1FCUPCharge();
        double getSP1FCUPVoltage();
        bool monitoringCharge = false;
        const bool isChargeBufferFull( const std::string & chargeName );
        const bool isChargeBufferNotFull( const std::string & chargeName );
        const bool isVoltageBufferFull( const std::string & chargeName );
        const bool isVoltageBufferNotFull( const std::string & chargeName );
        const bool isWCMBufferFull();
        const bool isWCMBufferNotFull();
        const bool isS02FCUPBufferFull();
        const bool isS02FCUPBufferNotFull();
        const bool isSP1FCUPBufferFull();
        const bool isSP1FCUPBufferNotFull();

        #ifdef BUILD_DLL
        boost::python::list getChargeBuffer_Py( const std::string & name );
        boost::python::list getVoltageBuffer_Py( const std::string & name );
        boost::python::list getWCMChargeBuffer_Py();
        boost::python::list getWCMVoltageBuffer_Py();
        boost::python::list getS02FCUPChargeBuffer_Py();
        boost::python::list getS02FCUPVoltageBuffer_Py();
        boost::python::list getSP1FCUPChargeBuffer_Py();
        boost::python::list getSP1FCUPVoltageBuffer_Py();
        boost::python::list getTimeStamps_Py( const std::string & name );
        boost::python::list getStrTimeStamps_Py( const std::string & name );
        boost::python::list getChargeDiagnosticNames_Py();
        boost::python::list getChargePVs_Py();
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
