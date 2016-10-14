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

#ifndef VELA_MAG_CONTROLLER_H_
#define VELA_MAG_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "magnetInterface.h"
#include "controller.h"
// boost.python
#ifdef BUILD_DLL
#include <boost/python.hpp>
#endif

class magnetController : public controller
{
    public:
        /// New scheem - we just have 1 constructor, but we h vae higher level class that create these objects 9hopefully
        magnetController();
        magnetController( const bool show_messages,    const bool show_debug_messagese,
                          const std::string & magConf, const std::string & NRConf,
                          const bool shouldStartEPICs );

        ~magnetController( );

      /// Magnet Type tests
        bool isAQuad( const std::string & magName );
        bool isABSol( const std::string & magName );
        bool isAHCor( const std::string & magName );
        bool isAVCor( const std::string & magName );
        bool isADip ( const std::string & magName );
        bool isASol ( const std::string & magName );
        bool isACor ( const std::string & magName );
      /// Magnet reverse Type tests
        bool isNRorNRGanged( const std::string & magName );
        bool isNRGanged    ( const std::string & magName );
        bool isBipolar     ( const std::string & magName );
        bool isNR          ( const std::string & magName );
      /// PSU State tests
        bool isOFF_psuN( const std::string & magName );
        bool isOFF_psuR( const std::string & magName );
        bool isON_psuN ( const std::string & magName );
        bool isON_psuR ( const std::string & magName );
        bool isOFF     ( const std::string & magName );
        bool isON      ( const std::string & magName );
      /// Deguassing tests
        bool isNotDegaussing( const std::string & magName );
        bool isDegaussing   ( const std::string & magName );
      /// Magnet Name getters
        std::vector< std::string > getMagnetNames();
        std::vector< std::string > getQuadNames();
        std::vector< std::string > getHCorNames();
        std::vector< std::string > getVCorNames();
        std::vector< std::string > getDipNames();
        std::vector< std::string > getSolNames();

      /// Get Magnet Data
      /// For c++ we allow access to the raw data objects,
        const magnetStructs::magnetObject &getMagObjConstRef( const std::string & magName  );
        const magnetStructs::magnetObject *getMagObjConstPtr( const std::string & magName  );
      /// you can also get copies of the data - these can be exposed with Python bindings
        magnetStructs::magnetStateStruct getCurrentMagnetState( const std::vector< std::string > & s );
        magnetStructs::magnetStateStruct getCurrentMagnetState();
      /// Get RI and SI
        double getSI( const std::string & magName );
        double getRI( const std::string & magName );
        std::vector< double > getSI( const std::vector< std::string > & magNames );
        std::vector< double > getRI( const std::vector< std::string > & magNames );
      /// Wriote magnet data in DBURT format to file
        bool writeDBURT( const magnetStructs::magnetStateStruct & ms, const std::string &fileName="", const std::string &comments="");
        bool writeDBURT( const std::string &fileName="", const std::string &comments="" );

      /// Set SI, 4 versions of this
        bool setSI( const std::string & magName, const double value);
        bool setSI( const std::vector< std::string > & magNames, const std::vector< double >& values);
        bool setSI( const std::string & magNames, const double values, const double tolerances, const size_t timeOUT );
        std::vector< std::string >  setSI( const std::vector< std::string > & magNames, const std::vector< double > & values, const std::vector< double > & tolerances, const size_t timeOUT );

      /// Switch On PSUs
      /// These functions return wether the commands were sent to EPICS correctly, not if the oiperation was succesful
        bool switchONpsu ( const std::string & magName  );
        bool switchOFFpsu( const std::string & magName  );
        bool switchONpsu ( const std::vector< std::string >& magNames );
        bool switchOFFpsu( const std::vector< std::string >& magNames );

      /// Degaussing
        size_t degauss( const std::string & mag, bool resetToZero = true );
        size_t degauss( const std::vector< std::string > & mag, bool resetToZero = true );
        size_t degaussAll( bool resetToZero = true );

      /// get magnet data from DBURT
        magnetStructs::magnetStateStruct getDBURT( const std::string & fileName );
        magnetStructs::magnetStateStruct getDBURTCorOnly( const std::string & fileName );
        magnetStructs::magnetStateStruct getDBURTQuadOnly( const std::string & fileName );

      /// apply DBURT  / magnetStateStruct
        void applyMagnetStateStruct( const magnetStructs::magnetStateStruct & ms  );
        void applyDBURT( const std::string & fileName );
        void applyDBURTCorOnly( const std::string & fileName );
        void applyDBURTQuadOnly( const std::string & fileName );

      /// This is an old function from setting up the project
        void showMagRevType();
      /// Manuallsettign RI tolerance...
        void setRITolerance( const std::string & magName, const double val);

      /// how long to wait when sending commands to EPICS
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );

      /// These are pure virtual methods, so need to have some implmentation in derived classes
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );
      /// any functions that return a map need a wrapper to convert to a python dictionary
      /// (we need the functions that return std::map types when building c++ applications)
#ifdef BUILD_DLL
        boost::python::dict getILockStates_Py( std::string magName );
        boost::python::dict getILockStatesStr_Py( std::string magName );
        boost::python::dict getMagPSUStateDefinition();
#endif // BUILD_DLL

    protected:
    private:

        void initialise();

        bool shouldStartEPICs;


        magnetInterface localInterface;
};



#endif // VELA_MAG_CONTROLLER_H_
