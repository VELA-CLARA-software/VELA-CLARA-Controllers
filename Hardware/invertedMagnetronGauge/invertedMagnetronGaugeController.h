#ifndef IMG_CONTROLLER_H
#define IMG_CONTROLLER_H

// project
#include "invertedMagnetronGaugeStructs.h"
#include "invertedMagnetronGaugeInterface.h"
#include "controller.h"
// stl
#include <string>
#include <vector>


class invertedMagnetronGaugeController : public controller
{
    public:

        /// we have overloaded constructors to specify config-file location

        invertedMagnetronGaugeController( const bool show_messages = true, const bool show_debug_messages = true );
        invertedMagnetronGaugeController( const std::string configFileLocation, const  bool show_messages = true, const bool show_debug_messages = true );
        ~invertedMagnetronGaugeController();

        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );

        double getImgP( const std::string & vacImgName );
        std::vector< std::vector< double > > monitorForNCounts( const std::string & vacImgName, int numcounts );

        /// write a method that returns string version of enums using ENUM_TO_STRING

        VELA_ENUM::IMG_STATE getImgState( const std::string & vacImgName );
        std::string getImgStateStr( const std::string & name );

        /// These are pure virtual method in the base class and MUST be overwritten in the derived Controller...
        /// write a method that returns string version of enums using ENUM_TO_STRING

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string > getILockStatesStr( const std::string & objName );



    protected:
    private:

        void initialise();

        /// No singletons, no pointers, let's just have an object

        invertedMagnetronGaugeInterface localInterface;

        std::vector< std::string > vacImgNames;
};


#endif // invertedMagnetronGaugeController_H
