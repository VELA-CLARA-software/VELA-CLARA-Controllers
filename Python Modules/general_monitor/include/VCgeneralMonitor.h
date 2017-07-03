#ifndef _VC_GENERAL_MONITOR_H
#define _VCgeneralMonitor_H
// djs part of VELA_CLARA_CONTROLLERS
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
        // this class allows a user to connect and monitor to a PVname purely by name
        // it is to cover all the cases where a generic hardware controller is not required
        // if you've looked at the code then, yes, much cancer is to be found
        // i'm sure this can be neatened up with templates etc.
        // no, i have no plans to do that at the moment

    public:
        VCgeneralMonitor(const bool shouldShowMessage = false,const bool  shouldShowDebugMessage= false );
        ~VCgeneralMonitor();
        // should be base class functions?
        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

        std::string connectPV(const std::string & PVName);
        std::string connectPV(const std::string & PVName,const std::string & PYType );

        bool disconnectPV(const std::string & id );

        boost::python::object getValue(const std::string& id );
        boost::python::dict getValue(const boost::python::list& ids);
        //boost::python::dict getValue(const std::vector<std::string>& ids);

        boost::python::object getTotalValue(const std::string& id );
        size_t getCounter(const std::string& id );


        boost::python::dict getCounterAndValue(const std::string& id);
        //boost::python::dict getCounterAndValue(const std::vector<std::string>& ids);
        boost::python::dict getCounterAndValue(const boost::python::list& ids);

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
        std::map<std::string,gmStructs::pvData<std::string>>          stringPVMap;

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
        gmStructs::pvStruct* addToStringPVMap();


//        std::vector<std::string> getVecStr(const boost::python::list& ids){
//            std::vector<std::string> r;
//            for (int i = 0; i < len(ids); ++i)
//                    r.push_back(boost::python::extract<std::string>(ids[i]);
//            return r;
//            }


        template<typename T, size_t size>
        size_t GetArrLength(T(&)[size]){ return size; }
};//VCgeneralMonitor
// function pointers for overloads
// and docstrings

//______________________________________________________________________________
std::string(VCgeneralMonitor::*connectPV_1)(const std::string&) =
    &VCgeneralMonitor::connectPV;
const char *connectPV_1_docstring =
    "Connects to pv_name process variable, where pv_name is the full PV name.\n"
    "On success returns a unique id that can be used to access values.";
//______________________________________________________________________________
std::string(VCgeneralMonitor::*connectPV_2)(const std::string&,const std::string&) =
    &VCgeneralMonitor::connectPV;
const char *connectPV_2_docstring =
    "Connects to pv_name process variable, where pv_name is the full PV name.\n"
    "On success returns a unique id that can be used to access values.";
//______________________________________________________________________________
const char *setQuiet_docstring =
    "Minimise message noise.";
const char *setVerbose_docstring =
    "Maximise message noise.";
const char *setMessage_docstring =
    "Show messages only.";
const char *setDebugMessage_docstring =
    "Show debugging messages only.";
//______________________________________________________________________________
const char *getPVCount_docstring =
    "Returns number of values for id.";
//______________________________________________________________________________
boost::python::object(VCgeneralMonitor::*getValue_1)(const std::string&) =
    &VCgeneralMonitor::getValue;
const char *getValue_1_docstring =
    "Returns current value for id.";

boost::python::object(VCgeneralMonitor::*getValue_2)(const std::string&,const int) =
    &VCgeneralMonitor::getValue;
const char *getValue_2_docstring =
    "Returns current values for an array PV at position index.";
boost::python::list(VCgeneralMonitor::*getValue_3)(const std::string&,const int,const int) =
    &VCgeneralMonitor::getValue;
const char *getValue_3_docstring =
    "Returns current values for an array PV in a region of interest defined by start_index"
    "and end_index, negtive indices counting from the end of the array should work.";

boost::python::dict(VCgeneralMonitor::*getValue_4)(const boost::python::list&) =
    &VCgeneralMonitor::getValue;
const char *getValue_4_docstring =
    "Returns dictionary of current values for list of ids.";
//______________________________________________________________________________
boost::python::dict(VCgeneralMonitor::*getCounterAndValue_1)(const std::string&) =
    &VCgeneralMonitor::getCounterAndValue;
const char *getCounterAndValue_1_docstring =
    "Get current counter and value for id.";
boost::python::dict(VCgeneralMonitor::*getCounterAndValue_2)(const boost::python::list&) =
    &VCgeneralMonitor::getCounterAndValue;
const char *getCounterAndValue_2_docstring =
    "Get current counter and value for list of ids.";
//______________________________________________________________________________

const char *getCount_docstring =
    "Get current count for id.";
const char *getTotalValue_docstring =
    "Get current counter and value id.";
const char * getCounterAndTotalValue_docstring =
    "Get current accumulated value and counter for id.";
const char *isStringPV_docstring =
    "return true if PV type is a string.";
const char *isIntPV_docstring =
    "return true if PV type is an int.";
const char *isFloatPV_docstring =
    "return true if PV type is a float.";
const char *isEnumPV_docstring =
    "return true if PV type is an enum.";
const char *isCharPV_docstring =
    "return true if PV type is a char.";
const char *isLongPV_docstring =
    "return true if PV type is a long.";
const char *isDoublePV_docstring =
    "return true if PV type is a double.";
const char *isArrayPV_docstring =
    "return true if PV type is an array.";
const char *isConnected_docstring =
    "return true if id is connected.";
const char *isMonitoring_docstring =
    "return true if id is monitoring.";
const char *disconnectPV_docstring =
    "disconnect ID and cancel EPICS subscriptions.";


using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_CLARA_General_Monitor)
{

    // dispable c++ signatures from docstrings...
    docstring_options local_docstring_options(true, true, false);

    /// containers
//    class_<std::vector<std::string>>("std_vector_string")
//            .def(vector_indexing_suite< std::vector< std::string >>() )
//            ;
//    class_<std::vector<double> >("std_vector_double")
//            .def( vector_indexing_suite< std::vector< double>>() )
//            ;
//    class_<std::vector<int> >("std_vector_int")
//            .def( vector_indexing_suite< std::vector< int>>() )
//            ;
    // Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller","", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        ;
    /// The main class that creates all the controller obejcts
    boost::python::class_<VCgeneralMonitor,boost::noncopyable> ("init")
        .def("setDebugMessage", &VCgeneralMonitor::setDebugMessage, setDebugMessage_docstring )
        .def("setVerbose",      &VCgeneralMonitor::setVerbose,      setVerbose_docstring )
        .def("setMessage",      &VCgeneralMonitor::setMessage,      setMessage_docstring )
        .def("setQuiet",        &VCgeneralMonitor::setQuiet,        setQuiet_docstring   )
        .def("getValue",  getValue_3,(boost::python::arg("id"),boost::python::arg("start_index"),boost::python::arg("end_index")),
                          getValue_3_docstring)
        .def("getValue",  getValue_2,(boost::python::arg("id"),boost::python::arg("index")),
                          getValue_2_docstring)
        .def("getValue",  getValue_4,(boost::python::arg("ids")),
                          getValue_4_docstring)
        .def("getValue",  getValue_1,(boost::python::arg("id"),
                          getValue_1_docstring))

        .def("connectPV", connectPV_1,(boost::python::arg("pv_name")),
                          connectPV_1_docstring)

        .def("connectPV", connectPV_2,(boost::python::arg("pv_name"),boost::python::arg("pv_type")),
                          connectPV_2_docstring )

        .def("getCounterAndValue", getCounterAndValue_1,(boost::python::arg("id"),
                                   getCounterAndValue_1_docstring))
        .def("getCounterAndValue", getCounterAndValue_2,(boost::python::arg("ids"),
                                   getCounterAndValue_2_docstring))

        .def("getPVCount",&VCgeneralMonitor::getPVCount,(boost::python::arg("id")),getPVCount_docstring)
        .def("getTotalValue", &VCgeneralMonitor::getTotalValue,(boost::python::arg("id"),getTotalValue_docstring))
        .def("getCounter",    &VCgeneralMonitor::getCounter,(boost::python::arg("id"),getCount_docstring))
        .def("getCounterAndTotalValue",
                             &VCgeneralMonitor::getCounterAndTotalValue,(boost::python::arg("id"),getCounterAndTotalValue_docstring))
        .def("isStringPV",   &VCgeneralMonitor::isStringPV,(boost::python::arg("id"),isStringPV_docstring))
        .def("isIntPV",      &VCgeneralMonitor::isIntPV,(boost::python::arg("id"),isIntPV_docstring))
        .def("isFloatPV",    &VCgeneralMonitor::isFloatPV,(boost::python::arg("id"),isFloatPV_docstring))
        .def("isEnumPV",     &VCgeneralMonitor::isEnumPV,(boost::python::arg("id"),isEnumPV_docstring))
        .def("isCharPV",     &VCgeneralMonitor::isCharPV,(boost::python::arg("id"),isCharPV_docstring))
        .def("isLongPV",     &VCgeneralMonitor::isLongPV,(boost::python::arg("id"),isLongPV_docstring))
        .def("isDoublePV",   &VCgeneralMonitor::isDoublePV,(boost::python::arg("id"),isDoublePV_docstring))
        .def("isArrayPV",    &VCgeneralMonitor::isArrayPV,(boost::python::arg("id"),isArrayPV_docstring))
        .def("isConnected",  &VCgeneralMonitor::isConnected,(boost::python::arg("id"),isConnected_docstring))
        .def("isMonitoring", &VCgeneralMonitor::isMonitoring,(boost::python::arg("id"),isMonitoring_docstring))
        .def("disconnectPV", &VCgeneralMonitor::disconnectPV,(boost::python::arg("id"),disconnectPV_docstring))
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        ;
}

#endif // _VCgeneralMonitor_H
