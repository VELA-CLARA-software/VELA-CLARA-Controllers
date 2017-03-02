#ifndef _VC_GENERAL_MONITOR_H
#define _VCgeneralMonitor_H
//stl
#include <string>
#include <vector>
//
#include "controller.h"
#include "structs.h"
#include "gmStructs.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


class VCgeneralMonitor : public controller// inherits controller for messaging
{
    public:
        VCgeneralMonitor(const bool shouldShowMessage = false,const bool  shouldShowDebugMessage= false );
        ~VCgeneralMonitor();
        // should be base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        std::string connectPV(const std::string & PVName,const std::string & PYType );


      /// These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT(double val );
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );

        bool isStringPV(const std::string& id);
        bool isIntPV(const std::string& id);
        bool isFloatPV(const std::string& id);
        bool isEnumPV(const std::string& id);
        bool isCharPV(const std::string& id);
        bool isLongPV(const std::string& id);
        bool isDoublePV(const std::string& id);


        bool isConnected(const std::string & id);
        bool isMonitoring(const std::string & id);

        bool disconnectPV(const std::string & id );


        //template <typename T>
        boost::python::object getValue(const std::string & id );


    protected:

    private:

        // This is the only ca_client_context attach and detach to this when multi-threading
        ca_client_context * thisCaContext;

        bool shouldShowDebugMessage, shouldShowMessage;


        const std::string ca_chid_successmess,ca_chid_failuremess,ca_subs_successmess,
                          ca_subs_failuremess,ca_create_channel_str,ca_create_subscription_str,
                          returnFail,ca_clear_channel_str,ca_cchi_successmess,ca_cchi_failuremess,
                          ca_clear_subscription_str,ca_csub_successmess,ca_csub_failuremess;

        //gmStructs::pvStruct* getCHTYPEandPrefix(const std::string& pvFullName,const std::string & pvType, bool & r);
        gmStructs::pvStruct* getCHTYPEandPrefix(const std::string & pvType);


        bool setUpChannel(gmStructs::pvStruct& pvs);
        bool setupMonitor(gmStructs::pvStruct& pvs);
        //bool setupMonitor(chtype CHTYPE,const std::string & id);


        int sendToEpics(const std::string& ca,const std::string& mess1,const std::string& mess2 );

        // loose this function ... :
        void updateTimeAndValue(const std::string & id, const void * dbr);
        // ... and put it all in here.... (no really trust me)
        void updateValue(const std::string & id,const event_handler_args& argsr);


        void updateTime(const epicsTimeStamp & stamp, double & val, std::string & str );


        void printStatusResult(const int status,const char* success,const char* timeout);



        double CA_PEND_IO_TIMEOUT;

        unsigned long  defaultCOUNT, defaultMASK;


        gmStructs::pvStruct* getPVStruct_Ptr(const std::string& id);



        static void staticEntryMonitor(const event_handler_args args);

        //std::map<std::string,gmStructs::monitorStruct> continuousMonitorStructs;
        std::vector<gmStructs::monitorStruct> continuousMonitorStructs;


        template< class T >
        bool entryExists( std::map< std::string, T > & m, const std::string & name );

        template<class T >
        bool deleteMapEntry(std::map<std::string, T>& map, const std::string& id);

        void killMonitor(gmStructs::pvStruct& pvs);
        void killChannel(gmStructs::pvStruct& pvs);
        bool deleteID(const std::string& id);

        void printTimeStamp(const epicsTimeStamp & stamp);


        //std::map<std::string,gmStructs::pvData<std::string>> stringPVMap;
        std::map<std::string,gmStructs::pvData<int>> intPVMap;
//        std::map<std::string,gmStructs::monitorStruct<short>> shortPVMap;
        std::map<std::string,gmStructs::pvData<float>> floatPVMap;
        std::map<std::string,gmStructs::pvData<unsigned short>> enumPVMap;//DBR_ENUM
        std::map<std::string,gmStructs::pvData<char>> charPVMap;
        std::map<std::string,gmStructs::pvData<long>> longPVMap;
        std::map<std::string,gmStructs::pvData<double>> doublePVMap;

        const std::string stringPrefix,intPrefix,shortPrefix,floatPrefix,enumPrefix,
                          charPrefix,longPrefix,doublePrefix;

        int pvMonitorMapCount;

        //addToMonitorMap(const std::string & pvFullName,const std::string & pvType);

        gmStructs::pvStruct* addToIntPVMap();
        gmStructs::pvStruct* addToFloatPVMap();
        gmStructs::pvStruct* addToDoublePVMap();
        gmStructs::pvStruct* addToCharPVMap();
        gmStructs::pvStruct* addToLongPVMap();
        gmStructs::pvStruct* addToEnumPVMap();



};
/// FUNCTION OVERLOADING, if you have overloaded functions, or ones with default parameters
/// Create a load of different function pointers and use them in the bindings
/// !!! OR !!! You may be able to use this macro, BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/functions.html
/// I'm going to do it the function pointer way here...
/// The other alternative is to create lots of different function names in the c++ class
/// just to make the function pointer overloading neater, let's make some typedefs, generally i don't like doing this
typedef double doub;
typedef const double cdou;
typedef std::vector<double> vecd;
typedef std::vector<std::vector<double>> vvcd;
typedef const std::vector<double> cved;
typedef const size_t csiz;
typedef size_t size;
typedef std::vector<size_t> vsiz;
typedef std::string stri;
typedef const std::string cstr;
typedef std::vector<std::string> vecs;
typedef const std::vector<std::string> cves;
typedef std::vector<bool> vecb;
///
//bool(pilaserController::*screenIN_1)  (cstr& ) = &pilaserController::screenIN;

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_General_Monitor )
{
    /// Things that you want to use in python muct be exposed:
    /// containers
    class_<std::vector< std::string > >("std_vector_string")
            .def( vector_indexing_suite< std::vector< std::string >>() )
            ;
    class_<std::vector< double> >("std_vector_double")
            .def( vector_indexing_suite< std::vector< double>>() )
            ;
    // Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller","controller Doc String", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        ;
    /// The main class that creates all the controller obejcts
        boost::python::class_<VCgeneralMonitor,boost::noncopyable> ("init")
        .def("setQuiet",         &VCgeneralMonitor::setQuiet )
        .def("setVerbose",       &VCgeneralMonitor::setVerbose )
        .def("setMessage",       &VCgeneralMonitor::setMessage )
        .def("setDebugMessage",  &VCgeneralMonitor::setDebugMessage )
        .def("connectPV",  &VCgeneralMonitor::connectPV )
        .def("getValue",  &VCgeneralMonitor::getValue )
        .def("isStringPV",  &VCgeneralMonitor::isStringPV )
        .def("isIntPV",  &VCgeneralMonitor::isIntPV )
        .def("isFloatPV",  &VCgeneralMonitor::isFloatPV )
        .def("isEnumPV",  &VCgeneralMonitor::isEnumPV )
        .def("isCharPV",  &VCgeneralMonitor::isCharPV )
        .def("isLongPV",  &VCgeneralMonitor::isLongPV )
        .def("isDoublePV",  &VCgeneralMonitor::isDoublePV )
        .def("isConnected",  &VCgeneralMonitor::isConnected )
        .def("isMonitoring",  &VCgeneralMonitor::isMonitoring )
        .def("disconnectPV",  &VCgeneralMonitor::disconnectPV )
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        ;
}

#endif // _VCgeneralMonitor_H
