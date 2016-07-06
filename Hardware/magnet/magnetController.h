#ifndef VELA_MAG_CONTROLLER_H_
#define VELA_MAG_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "magnetInterface.h"
#include "controller.h"

class magnetController : public controller
{
    public:
        /// we have overloaded constructors to specify config-file location
        magnetController( const bool show_messages = true , const bool show_debug_messages = true );
        magnetController( const std::string configFileLocation1,  const std::string configFileLocation2, const std::string configFileLocation3, const  bool show_messages = true, const bool show_debug_messages = true );
        ~magnetController( );

        /// These are pure virtual methods, so need to have some implmentation in derived classes

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );

        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );

        bool isAQuad( const std::string & magName );
        bool isABSol( const std::string & magName );
        bool isAHCor( const std::string & magName );
        bool isAVCor( const std::string & magName );
        bool isADip ( const std::string & magName );
        bool isASol ( const std::string & magName );
        bool isACor ( const std::string & magName );

        bool isNR( const std::string & magName );
        bool isBipolar( const std::string & magName );
        bool isNRGanged( const std::string & magName );
        bool isNRorNRGanged( const std::string & magName );

        bool isON_psuN( const std::string & magName );
        bool isON_psuR( const std::string & magName );
        bool isON ( const std::string & magName );
        bool isOFF_psuN( const std::string & magName );
        bool isOFF_psuR( const std::string & magName );
        bool isOFF ( const std::string & magName );


        std::vector< std::string > getMagnetNames();
        std::vector< std::string > getQuadNames();
        std::vector< std::string > getHCorNames();
        std::vector< std::string > getVCorNames();
        std::vector< std::string > getDipNames();
        std::vector< std::string > getSolNames();


        void setRITolerance( const std::string & magName, const double val);

        void showMagRevType();

        double getSI( const std::string & magName );
        double getRI( const std::string & magName );

        std::vector< double > getSI( const std::vector< std::string > & magNames );
        std::vector< double > getRI( const std::vector< std::string > & magNames );

        bool setSI( const std::string & magName, const double value);
        bool setSI( const  std::vector< std::string > & magNames, const std::vector< double >& values);
        bool setSI( const std::string & magNames, const double values, const double tolerances, const size_t timeOUT );
        std::vector< std::string >  setSI( const std::vector< std::string > & magNames, const std::vector< double > & values, const std::vector< double > & tolerances, const size_t timeOUT );

        /// These functions return wether the commands were sent to EPICS correctly, not if the oiperation was succesful

        bool switchONpsu ( const std::string & magName  );
        bool switchOFFpsu( const std::string & magName  );
        bool switchONpsu ( const std::vector< std::string >& magNames );
        bool switchOFFpsu( const std::vector< std::string >& magNames );

        size_t degauss( const std::string & mag, bool resetToZero = true );
        size_t degauss( const std::vector< std::string > & mag, bool resetToZero = true );
        size_t degaussAll( bool resetToZero = true );

        magnetStructs::magnetStateStruct getCurrentMagnetState();
        void applyMagnetStateStruct( const magnetStructs::magnetStateStruct & ms  );


        const magnetStructs::magnetObject &getMagObjConstRef( const std::string & magName  );
        const magnetStructs::magnetObject *getMagObjConstPtr( const std::string & magName  );

        magnetStructs::magnetStateStruct getDBURT( const std::string & fileName );
        magnetStructs::magnetStateStruct getDBURTCorOnly( const std::string & fileName );
        magnetStructs::magnetStateStruct getDBURTQuadOnly( const std::string & fileName );

        void applyDBURT( const std::string & fileName );
        void applyDBURTCorOnly( const std::string & fileName );
        void applyDBURTQuadOnly( const std::string & fileName );

        bool writeDBURT( const magnetStructs::magnetStateStruct & ms, const std::string & fileName = "", const std::string & comments = "" );
        bool writeDBURT( const std::string & fileName = "", const std::string & comments = "" );


        bool isDegaussing( const std::string & magName );
        bool isNotDegaussing( const std::string & magName );

    protected:
    private:

        void initialise();

        magnetInterface localInterface;
};



#endif // VELA_MAG_CONTROLLER_H_

