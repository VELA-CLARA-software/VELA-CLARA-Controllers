#ifndef RFGUN_CONTROLLER_H_
#define RFGUN_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>


#include "RFGunInterface.h"
#include "controller.h"

#ifdef BUILD_DLL
#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#endif

class RFGunController : public controller
{
    public:
        /// we have overloaded constructors to specify config-file location
        RFGunController( const bool show_messages = true , const bool show_debug_messages = true );
        RFGunController( const std::string configFileLocation1,
                         const std::string configFileLocation2,
                         const std::string configFileLocation3, const  bool show_messages = true, const bool show_debug_messages = true );
        ~RFGunController( );

        std::vector< std::vector< double >> getRevT( const std::string & name );
        std::vector< std::vector< double >> getFwdT(const std::string & name  );
        std::vector< double > getRevTStamp(const std::string & name  );
        std::vector< double > getFwdTStamp(const std::string & name  );
        std::vector< std::string > getRevTStampStr( const std::string & name );
        std::vector< std::string > getFwdTStampStr( const std::string & name  );

        std::vector< std::string > getRFPowerNames();

        void setAmp(long val);
        void setPhi(double val);

        double getAmpR();
        long   getAmpW();
        double getPhi();
        double getFwd( const std::string & name );
        double getRev( const std::string & name );
        double getRatio( const std::string & name );

        void monitorTracesForNShots( const size_t N );

        void monitorTracesForNShotsAndWait( const size_t N );

        bool killTraceMonitors();

        RFGunStructs::MOD_STATE    getModState();
        RFGunStructs::MOD_EXILOCK1 getModiLock();

        std::string getModStateStr();
        std::string getModiLockStr();

        /// These are pure virtual methods, so need to have some implmentation in derived classes

        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( const double val );

        void setModToTrig();
        void setModToHVOn();

        void modReset();
        bool modResetAndWait(const size_t waitTime );

        void setModToOff();
        void setModToStandby();
        bool switchOnRF();
        bool switchOffRF();

        bool isModILockStateGood();
        bool isModILockStateNotGood();
        bool isMonitoringTraces();
        bool isNotMonitoringTraces();
        bool isModWarmedUp();
        bool isModNotWarmedUp();
        bool isModInHVOn();
        bool isModInTrig();
        bool isModInOff();
        bool isModInStandby();

        double getFwdCursor( const std::string & name );
        bool   setFwdCursor( const std::string & name, const  double val );
        double getRevCursor( const std::string & name );
        bool   setRevCursor( const std::string & name, const  double val );
        bool   setAllCursors(const double val );



    protected:
        RFGunInterface localInterface;
    private:

        void initialise();



        /// These are pure virtual methods, so need to have some implmentation in derived classes

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name ) { std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > r; return r;}
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name )        { std::map< VELA_ENUM::ILOCK_NUMBER, std::string > r;            return r;}

};



#endif // RFGUN_CONTROLLER_H_

