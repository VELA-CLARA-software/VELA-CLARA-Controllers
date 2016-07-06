#ifndef VELA_SCOPE_INTERFACE_H
#define VELA_SCOPE_INTERFACE_H
// djs
#include "interface.h"
#include "invertedMagnetronGaugeStructs.h"
#include "invertedMagnetronGaugeConfigReader.h"
//stl
#include <vector>
#include <string>

class invertedMagnetronGaugeInterface : public interface
{
    public:

        /// Not a singleton, two construction methods....

        invertedMagnetronGaugeInterface( const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );
        invertedMagnetronGaugeInterface( const std::string & configFileLocation, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr  );

        ~invertedMagnetronGaugeInterface();

        void getImgNames( std::vector< std::string >  & vacImgNames );
        double getImgP( const std::string & vacImg );
        std::vector< std::vector< double > > monitorForNCounts( const std::string & vacImgName, int numcounts );

        void getNewVacImgData( const std::string vacImg1, std::vector< std::vector< double > > & vacImgData, std::vector< invertedMagnetronGaugeStructs::dataCollectionResult > & status  );
        void cancelDataMonitors();
        char getTimestamp( std::string & vacImg );

        invertedMagnetronGaugeStructs::vacImgObject getVacImgObject( const std::string & vacImgName );

        VELA_ENUM::IMG_STATE getImgState( const std::string & vacImgName );

        /// This is a pure virtual method in the base class and MUST be overwritten in the derived interface...

        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE > getILockStates( const std::string & vacImgName );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >        getILockStatesStr( const std::string & vacImgName );
//
        enum ACQUIRE_RESULT{ k_ACQUIRED, k_TIMEOUT, k_ERR };
        ACQUIRE_RESULT getResult(){ return lastResult; }

    protected:
    private:

        /// called from constructor to set-up chids, montiros, etc.

        void initialise();

        invertedMagnetronGaugeConfigReader configReader;

        void initVacImgObjects();
        void initVacImgChids();
        //void addChannel( std::map< std::string, invertedMagnetronGaugeStructs::vacImgObject >::iterator & it1, std::map< invertedMagnetronGaugeStructs::IMG_PV_TYPE, std::string >::const_iterator & it2 );
        void addChannel( const std::string & pvRoot, invertedMagnetronGaugeStructs::pvStruct & pv );

        void monitorVacImgs();
        ACQUIRE_RESULT lastResult;

        /// As an overly complicated example let's try some function pointers. Toggling (open / close ) the vacImg is now easy
        /// https://isocpp.org/wiki/faq/pointers-to-members

        //typedef  bool (invertedMagnetronGaugeInterface::*isOCMemFn)(const std::string & );
        //typedef  void (invertedMagnetronGaugeInterface::*OCMemFn)  (const std::string & );
        //bool toggleAndWait( isOCMemFn f1, isOCMemFn f2, OCMemFn f3, const std::string & name, const time_t waitTime, invertedMagnetronGaugeInterface & obj );

        /// static function that can be called back from epics to update values

        static void staticEntryVacImgMonitor( const event_handler_args args );

        std::vector< invertedMagnetronGaugeStructs::monitorStruct* > temporaryMonitorStructs;

        void * addTemporaryMonitorStruct( invertedMagnetronGaugeStructs::vacImgObject * vacImgObjp , invertedMagnetronGaugeStructs::IMG_PV_TYPE monType, std::vector< std::vector< double > > & vacImgData, invertedMagnetronGaugeStructs::dataCollectionResult * stat);


        /// This is a vector of pointers... no you say !! let's follow  Bjarne Stroustrup's advice and "Store many objects in a container by value." ?
        /// http://stackoverflow.com/questions/24085931/is-using-stdvector-stdshared-ptrconst-t-an-antipattern
        /// tough... maybe one day we re-factor, for now remember to delete in the destructor

        std::vector< invertedMagnetronGaugeStructs::monitorStruct * > continuousMonitorStructs;

        std::map< std::string, invertedMagnetronGaugeStructs::vacImgObject > allVacImgData; /// All the vacImg data is stored in this map, keyed by the vacImg name

        std::vector< std::vector< double > > allVacImgMonitorData;
//
        bool invertedMagnetronGaugeInterface::allDataIsCollected( std::vector< invertedMagnetronGaugeStructs::dataCollectionResult > status );
};
#endif // VELA_PyIL_SHUTTER_INTERFACE_H
