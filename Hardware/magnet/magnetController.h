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
//  Last edit:   18-04-2018
//  FileName:    magnetController.cpp
//  Description:
//
//
//*/
#ifndef VELA_MAG_CONTROLLER_H_
#define VELA_MAG_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "magnetInterface.h"
#include "magnetStructs.h"
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
//______________________________________________________________________________
class magnetController : public controller
{
    public:
        magnetController();

        magnetController(bool& show_messages,
                         bool& show_debug_messagese,
                         const std::string &magConf,
                         const bool startVirtualMachine,
                         const bool shouldStartEPICs,
                         const HWC_ENUM::MACHINE_AREA myMachineArea,
                         const std::string& name
                         );

        ~magnetController();

      /// Magnet Type tests
        bool isAQuad(const std::string & magName);
        bool isABSol(const std::string & magName);
        bool isAHCor(const std::string & magName);
        bool isAVCor(const std::string & magName);
        bool isADip (const std::string & magName);
        bool isASol (const std::string & magName);
        bool isACor (const std::string & magName);
      /// PSU State tests
        bool isOFF_psuN(const std::string & magName);
        bool isOFF_psuR(const std::string & magName);
        bool isON_psuN (const std::string & magName);
        bool isON_psuR (const std::string & magName);
        bool isOFF     (const std::string & magName);
        bool isON      (const std::string & magName);

        int  getRemainingDegaussSteps(const std::string& magName);
        bool entryExistsAndIsDegaussing(const std::string& magName)const;
        bool entryExistsAndIsNotDegaussing(const std::string& magName)const;

      /// Deguassing tests
        bool isNotDegaussing(const std::string & magName);
        bool isDegaussing   (const std::string & magName);
      /// RI test
        bool isRIequalVal(const std::string & magName, const  double value, const double tolerance);
        bool isRIequalSI(const std::string & magName);
      /// Magnet Name getters
        std::vector<std::string> getMagnetNames()const;
        std::vector<std::string> getQuadNames();
        std::vector<std::string> getHCorNames();
        std::vector<std::string> getVCorNames();
        std::vector<std::string> getDipNames();
        std::vector<std::string> getSolNames();
        magnetStructs::magnetsNotSetCorrectly getLastFailedSet()const;

      /// Get Magnet Data
      /// For c++ we allow access to the raw data objects, Python read-only acces
        const magnetStructs::magnetObject &getMagObjConstRef(const std::string & magName );
        const magnetStructs::magnetObject *getMagObjConstPtr(const std::string & magName );
      /// you can also get copies of the data - these can be exposed with Python bindings
        magnetStructs::magnetStateStruct getCurrentMagnetState(const std::vector<std::string>& s);
        magnetStructs::magnetStateStruct getCurrentMagnetState();

      /// Write magnet data in DBURT format to file
        bool writeDBURT(const magnetStructs::magnetStateStruct & ms, const std::string &fileName="", const std::string &comments="",const std::string & keywords = "");
        bool writeDBURT(const std::string &fileName="", const std::string &comments="",const std::string & keywords = "");

      /// Set SI, 4 versions of this
        bool setSI(const std::string & magName, const double value);
        bool setSI(const std::vector<std::string> &magName, const std::vector<double>& value);






        bool setSI(const std::string & magNames, const double values, const double tolerances, const size_t timeOUT);
        std::vector<std::string>  setSI(const std::vector<std::string> & magNames, const std::vector<double> & values, const std::vector<double> & tolerances, const size_t timeOUT);
        bool setSIZero(const std::string & magName);
        bool setSIZero(const std::vector<std::string> & magNames);


      /// Switch On PSUs
      /// These functions return wether the commands were sent to EPICS correctly, not if the oiperation was succesful
        bool switchONpsu (const std::string & magName );
        bool switchOFFpsu(const std::string & magName );
        bool switchONpsu (const std::vector<std::string>& magNames);
        bool switchOFFpsu(const std::vector<std::string>& magNames);

      /// Degaussing
        size_t degauss(const std::string & mag, bool resetToZero = true);
        size_t degauss(const std::vector<std::string> & mag, bool resetToZero = true);
        size_t degaussAll(bool resetToZero = true);

      /// get magnet data from DBURT
        magnetStructs::magnetStateStruct getDBURT(const std::string & fileName);
        magnetStructs::magnetStateStruct getDBURTCorOnly(const std::string & fileName);
        magnetStructs::magnetStateStruct getDBURTQuadOnly(const std::string & fileName);

      /// apply DBURT  / magnetStateStruct
        bool applyMagnetStateStruct(const magnetStructs::magnetStateStruct & ms );
        bool applyDBURT(const std::string & fileName);
        bool applyDBURTCorOnly(const std::string & fileName);
        bool applyDBURTQuadOnly(const std::string & fileName);

      /// get objectdata

      /// Get RI and SI
        double getSI(const std::string & magName);
        double getRI(const std::string & magName);
        std::vector<double> getSI(const std::vector<std::string> & magNames);
        std::vector<double> getRI(const std::vector<std::string> & magNames);

      /// Manual get/set RI tolerance
        void setRITolerance(const std::string & magName, const double val);
        void setRITolerance(const std::vector<std::string> & magNames, const std::vector<double> & vals);
        double getRITolerance(const std::string & magName);
        std::vector<double>  getRITolerance(const std::vector<std::string> & magNames);

        magnetStructs::MAG_TYPE                  getMagType(const std::string & magName);
        std::vector< magnetStructs::MAG_TYPE>  getMagType(const std::vector<std::string> & magNames);
      ///
        magnetStructs::MAG_PSU_STATE                 getMagPSUState(const std::string & magName);
        std::vector< magnetStructs::MAG_PSU_STATE> getMagPSUState(const std::vector<std::string> & magNames);
      ///
        size_t              getNumDegSteps(const std::string & magName);
        std::vector<size_t> getNumDegSteps(const std::vector<std::string> & magNames);
      ///
        std::vector<double>              getDegValues(const std::string & magName);
        std::vector<std::vector<double>> getDegValues(const std::vector<std::string> & magName);
      ///
      /// BJAS additions
        std::vector<double>              getFieldIntegralCoefficients(const std::string & magName);
        std::vector<std::vector<double>> getFieldIntegralCoefficients(const std::vector<std::string> & magNames);
      ///
        double              getPosition(const std::string & magName);
        std::vector<double> getPosition(const std::vector<std::string> & magNames);
      ///
        double              getMagneticLength(const std::string & magName);
        std::vector<double> getMagneticLength(const std::vector<std::string> & magNames);
      ///
        std::string              getManufacturer(const std::string & magName);
        std::vector<std::string> getManufacturer(const std::vector<std::string> & magNames);
      ///
        std::string               getSerialNumber(const std::string & magName);
        std::vector<std::string>  getSerialNumber(const std::vector<std::string> & magName);
      ///
        std::string               getMagnetBranch(const std::string & magName);
        std::vector<std::string>  getMagnetBranch(const std::vector<std::string> & magName);
      ///
        std::string               getMeasurementDataLocation(const std::string & magName);
        std::vector<std::string>  getMeasurementDataLocation(const std::vector<std::string> & magName);

      /// any functions that return a map need a wrapper to convert to a python dictionary
      /// (we need the functions that return std::map types when building c++ applications)
#ifdef BUILD_DLL

        bool setSI_Py1(const boost::python::list & magNames,
                      const boost::python::list & values);
        boost::python::list setSI_Py2(const boost::python::list& magNames,
                                     const boost::python::list& values,
                                     const boost::python::list& tolerances,
                                     const size_t timeOUT);

        boost::python::dict getILockStates_Py(std::string magName);
        boost::python::dict getILockStatesStr_Py(std::string magName);
        boost::python::dict getMagPSUStateDefinition();

        boost::python::list getMagnetNames_Py();
        boost::python::list getQuadNames_Py();
        boost::python::list getHCorNames_Py();
        boost::python::list getVCorNames_Py();
        boost::python::list getDipNames_Py();
        boost::python::list getSolNames_Py();


        boost::python::list getLastFailedSet_Py()const;


        magnetStructs::magnetStateStruct getCurrentMagnetState_Py(const boost::python::list& s);

        bool setSIZero_Py(const boost::python::list& magNames);
        bool switchONpsu_Py(const boost::python::list& magNames);
        bool switchOFFpsu_Py(const boost::python::list& magNames);
        size_t degauss_Py(const boost::python::list& mag, bool resetToZero = true);
        boost::python::list getSI_Py(const boost::python::list& magNames);
        boost::python::list getRI_Py(const boost::python::list& magNames);
        void setRITolerance_Py(const boost::python::list& magNames, const boost::python::list& vals);
        boost::python::list getRITolerance_Py(const boost::python::list& magNames);
        boost::python::list getMagType_Py(const boost::python::list& magNames);
        boost::python::list getMagPSUState_Py(const boost::python::list& magNames);
        boost::python::list getNumDegSteps_Py(const boost::python::list& magNames);
        boost::python::list getDegValues_Py1(const std::string & magName);
        boost::python::list getDegValues_Py2(const boost::python::list& magName);
        boost::python::list getFieldIntegralCoefficients_Py1(const std::string& magName);
        boost::python::list getFieldIntegralCoefficients_Py2(const boost::python::list& magNames);
        boost::python::list getPosition_Py(const boost::python::list& magNames);
        boost::python::list getMagneticLength_Py(const boost::python::list& magNames);
        boost::python::list getManufacturer_Py(const boost::python::list& magNames);
        boost::python::list getSerialNumber_Py(const boost::python::list& magName);
        boost::python::list getMagnetBranch_Py(const boost::python::list& magName);
        boost::python::list getMeasurementDataLocation_Py(const boost::python::list& magName);

#endif // BUILD_DLL

        /// YOU CANNOT SET THE CONTROLLER TYPE, NO-WAY JOSE
        HWC_ENUM::MACHINE_AREA getmyMachineArea(){return myMachineArea;};
        ///void setmyMachineArea(){ no chance matey };

        // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT() const;
        void   set_CA_PEND_IO_TIMEOUT(double val);
        std::map<HWC_ENUM::ILOCK_NUMBER,HWC_ENUM::ILOCK_STATE>
            getILockStates(const std::string& name)const;
        std::map<HWC_ENUM::ILOCK_NUMBER,std::string>
            getILockStatesStr(const std::string& name)const;


    protected:
    private:

        void initialise();

//        const bool shouldStartEPICs;
//
//        /// what flavour of controller am i ?
        const HWC_ENUM::MACHINE_AREA myMachineArea;
        magnetInterface localInterface;
};



#endif // VELA_MAG_CONTROLLER_H_
