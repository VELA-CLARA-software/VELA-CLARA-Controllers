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
        bool disconnectPV(const std::string & id );

        boost::python::object getValue(const std::string& id );
        boost::python::dict getValue(const std::vector<std::string>& ids);

        boost::python::object getTotalValue(const std::string& id );
        size_t getCounter(const std::string& id );


        boost::python::dict getCounterAndValue(const std::string& id);
        boost::python::dict getCounterAndValue(const std::vector<std::string>& ids);

        boost::python::dict getCounterAndTotalValue(const std::string& id);

        boost::python::object getValue(const std::string& id, const  int position);
        boost::python::list getValue(const std::string& id, const  int start_position, const  int end_position);

        size_t getPVCount(const std::string & id  );

        bool isStringPV(const std::string& id);
        bool isIntPV(const std::string& id);
        bool isFloatPV(const std::string& id);
        bool isEnumPV(const std::string& id);
        bool isCharPV(const std::string& id);
        bool isLongPV(const std::string& id);
        bool isDoublePV(const std::string& id);
        bool isConnected(const std::string & id);
        bool isMonitoring(const std::string & id);

        bool isArrayDoublePV(const std::string& id);
        bool isArrayIntPV(const std::string& id);

        bool isArrayPV(const std::string& id);

        bool isValidID(const std::string& id);

      /// These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT(double val );
        std::map< VELA_ENUM::ILOCK_NUMBER, VELA_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< VELA_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );

    protected:

    private:
        // This is the only ca_client_context attach and detach to this when multi-threading
        ca_client_context * thisCaContext;

        bool shouldShowDebugMessage, shouldShowMessage;
        const std::string ca_chid_successmess,ca_chid_failuremess,ca_subs_successmess,
                          ca_subs_failuremess,ca_create_channel_str,ca_create_subscription_str,
                          returnFail,ca_clear_channel_str,ca_cchi_successmess,ca_cchi_failuremess,
                          ca_clear_subscription_str,ca_csub_successmess,ca_csub_failuremess;

        gmStructs::pvStruct* getCHTYPEandPrefix(const std::string & pvType);

        bool setUpChannel(gmStructs::pvStruct& pvs);
        bool setupMonitor(gmStructs::pvStruct& pvs);
        bool getandSetArraySize(gmStructs::pvStruct& pvs);


        std::vector< size_t > getArrayRegionOfInterest(const int start_position,const int end_position,const size_t vec_size );
         size_t getArrayIndex(const int index,const size_t vec_size );

        int sendToEpics(const std::string& ca,const std::string& mess1,const std::string& mess2 );

        // loose this function ... :
        void updateTimeAndValue(const std::string & id, const void * dbr);
        // ... and put it all in here.... (no really trust me)
        void updateValue(const std::string & id,const event_handler_args& argsr);

        void updateTime(const epicsTimeStamp & stamp, double & val, std::string & str);
        void printStatusResult(const int status,const char* success,const char* timeout);

        double CA_PEND_IO_TIMEOUT;
        unsigned long  defaultCOUNT, defaultMASK;

        gmStructs::pvStruct* getPVStruct_Ptr(const std::string& id);
        static void staticEntryMonitor(const event_handler_args args);

        // I can only get the callabcks to work properly if this is a vector of pointers
        std::vector<gmStructs::monitorStruct* > continuousMonitorStructs;

        template< class T >
        bool entryExists(std::map<std::string, T > & m,const std::string & name );
        template<class T >
        bool deleteMapEntry(std::map<std::string, T>& map,const std::string& id);

        void killMonitor(gmStructs::pvStruct& pvs);
        void killChannel(gmStructs::pvStruct& pvs);
        bool deleteID(const std::string& id);

        void printTimeStamp(const epicsTimeStamp & stamp);

        std::map<std::string,gmStructs::pvData<int>>                  intPVMap;
        std::map<std::string,gmStructs::pvData<float>>                floatPVMap;
        std::map<std::string,gmStructs::pvData<unsigned short>>       enumPVMap;//DBR_ENUM
        std::map<std::string,gmStructs::pvData<char>>                 charPVMap;
        std::map<std::string,gmStructs::pvData<long>>                 longPVMap;
        std::map<std::string,gmStructs::pvData<double>>               doublePVMap;
        std::map<std::string,gmStructs::pvData<std::vector<double>> > vec_doublePVMap;
        std::map<std::string,gmStructs::pvData<std::vector<int>>>     vec_intPVMap;

        const std::string stringPrefix,intPrefix,shortPrefix,floatPrefix,enumPrefix,
                          charPrefix,longPrefix,doublePrefix,vecdoublePrefix,vecintPrefix;

        void addSingleDouble(const std::string & id,const event_handler_args& args);
        void addArrayDouble(const std::string & id,const event_handler_args& args);

        int pvMonitorMapCount;

        gmStructs::pvStruct* addToIntPVMap();
        gmStructs::pvStruct* addToFloatPVMap();
        gmStructs::pvStruct* addToDoublePVMap();
        gmStructs::pvStruct* addToCharPVMap();
        gmStructs::pvStruct* addToLongPVMap();
        gmStructs::pvStruct* addToEnumPVMap();
        gmStructs::pvStruct* addToVecDoublePVMap();
        gmStructs::pvStruct* addToVecIntPVMap();

        template<typename T, size_t size>
        size_t GetArrLength(T(&)[size]){ return size; }
};//VCgeneralMonitor
// function pointers for overloads
boost::python::object(VCgeneralMonitor::*getValue_1)(const std::string&) = &VCgeneralMonitor::getValue;
boost::python::object(VCgeneralMonitor::*getValue_2)(const std::string&,const int) = &VCgeneralMonitor::getValue;
boost::python::list(VCgeneralMonitor::*getValue_3)(const std::string&,const int,const int) = &VCgeneralMonitor::getValue;
boost::python::dict(VCgeneralMonitor::*getValue_4)(const std::vector<std::string>&) = &VCgeneralMonitor::getValue;
boost::python::dict(VCgeneralMonitor::*getCounterAndValue_1)(const std::string&) = &VCgeneralMonitor::getCounterAndValue;
boost::python::dict(VCgeneralMonitor::*getCounterAndValue_2)(const std::vector<std::string>&) = &VCgeneralMonitor::getCounterAndValue;

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_General_Monitor )
{    // Things that you want to use in python must be exposed:
    /// containers
//    class_<std::vector<std::string>>("std_vector_string")
//            .def(vector_indexing_suite< std::vector< std::string >>() )
//            ;
    class_<std::vector<double> >("std_vector_double")
            .def( vector_indexing_suite< std::vector< double>>() )
            ;
    class_<std::vector<int> >("std_vector_int")
            .def( vector_indexing_suite< std::vector< int>>() )
            ;
    // Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller","controller Doc String", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        ;
    /// The main class that creates all the controller obejcts
    boost::python::class_<VCgeneralMonitor,boost::noncopyable> ("init")
        .def("setQuiet",        &VCgeneralMonitor::setQuiet )
        .def("setVerbose",      &VCgeneralMonitor::setVerbose )
        .def("setMessage",      &VCgeneralMonitor::setMessage )
        .def("setDebugMessage", &VCgeneralMonitor::setDebugMessage )
        .def("connectPV",       &VCgeneralMonitor::connectPV )
        .def("getPVCount",       &VCgeneralMonitor::getPVCount )
        .def("getValue", getValue_1,(boost::python::arg("id"),"get value from PV with id=id, if it is an arry PV then passing no extra values will return teh fill array, passing one index will return the elemtn at that index, passing two indeces will give an array ofer thta region (negtive indices counting from the end of the array should work."))
        .def("getValue", getValue_2,(boost::python::arg("id"),boost::python::arg("index"),"get value from PV with id=id, if it is an arry PV then passing no extra values will return teh fill array, passing one index will return the elemtn at that index, passing two indeces will give an array ofer thta region (negtive indices counting from the end of the array should work."))
        .def("getValue", getValue_3,(boost::python::arg("id"),boost::python::arg("start_index"),boost::python::arg("end_index"),"get value from PV with id=id, if it is an arry PV then passing no extra values will return the fill array, passing one index will return the elemtn at that index, passing two indeces will give an array ofer that region (negtive indices counting from the end of the array should work."))
        .def("getValue", getValue_4,(boost::python::arg("ids"),"get values from multiple ids."))
        .def("getTotalValue", &VCgeneralMonitor::getTotalValue,(boost::python::arg("id"),"get accumulated value of an array PV."))
        .def("getCounter", &VCgeneralMonitor::getCounter,(boost::python::arg("id"),"get current counter and value."))
        .def("getCounterAndValue", getCounterAndValue_1,(boost::python::arg("id"),"get current counter and value."))
        .def("getCounterAndValue", getCounterAndValue_2,(boost::python::arg("ids"),"get current counter and value for multiple ids."))
        .def("getCounterAndTotalValue", &VCgeneralMonitor::getCounterAndTotalValue,(boost::python::arg("id"),"get current accumulated value and counter."))
        .def("isStringPV",   &VCgeneralMonitor::isStringPV,(boost::python::arg("id"),"return true if PV type is a string."))
        .def("isIntPV",      &VCgeneralMonitor::isIntPV,(boost::python::arg("id"),"return true if PV type is an int."))
        .def("isFloatPV",    &VCgeneralMonitor::isFloatPV,(boost::python::arg("id"),"return true if PV type is a float."))
        .def("isEnumPV",     &VCgeneralMonitor::isEnumPV,(boost::python::arg("id"),"return true if PV type is an enum."))
        .def("isCharPV",     &VCgeneralMonitor::isCharPV,(boost::python::arg("id"),"return true if PV type is a char."))
        .def("isLongPV",     &VCgeneralMonitor::isLongPV,(boost::python::arg("id"),"return true if PV type is a long."))
        .def("isDoublePV",   &VCgeneralMonitor::isDoublePV,(boost::python::arg("id"),"return true if PV type is a double."))
        .def("isArrayPV",   &VCgeneralMonitor::isArrayPV,(boost::python::arg("id"),"return true if PV type is an array."))
        .def("isConnected",  &VCgeneralMonitor::isConnected,(boost::python::arg("id"),"return true if id is connected."))
        .def("isMonitoring", &VCgeneralMonitor::isMonitoring,(boost::python::arg("id"),"return true if id is monitoring."))
        .def("disconnectPV", &VCgeneralMonitor::disconnectPV,(boost::python::arg("id"),"disconnect ID and cancel all EPICS subscriptions."))
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        ;
}

#endif // _VCgeneralMonitor_H
