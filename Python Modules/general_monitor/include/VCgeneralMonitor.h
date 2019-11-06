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
//  Last edit:   06-06-2018
//  FileName:    VCgeneralMonitor.h
//  Description:
//
//
//*/
#ifndef _VC_GENERAL_MONITOR_H
#define _VC_GENERAL_MONITOR_H
//stl
#include <string>
//
#include "VCheader.h"
#include "VCbase.h"
#include "gmStructs.h"
//______________________________________________________________________________
class VCgeneralMonitor : public controller,  public  VCbase// inherits controller for messaging
{
        // this class allows a user to connect and monitor to a PVname purely by name
        // it is to cover all the cases where a generic hardware controller is not required
        // if you've looked at the code then, yes, much cancer is to be found
        // i'm sure this can be neatened up with templates etc.
        // no, i have no plans to do that at the moment

    public:
        VCgeneralMonitor(const bool showmess = false,const bool  showdebug = false);
        ~VCgeneralMonitor();
        // should be base class functions?
//        void setQuiet();
//        void setVerbose();
//        void setMessage();
//        void setDebugMessage();

        std::string connectPV(const std::string & PVName);
        std::string connectPV(const std::string & PVName,const std::string & PYType );

        bool disconnectPV(const std::string & id );
        size_t getCounter(const std::string& id );

        void clearRunningValues(const std::string& id );

        bool isRunningStatComplete(const std::string& id);


        bool isBufferFull(const std::string& id);
        void clearBuffer(const std::string& id);
        void setBufferSize(const std::string& id , const size_t value );
        size_t getBufferCount(const std::string& id );
        size_t getBufferSize(const std::string & id);


        bool isRunningStatNotComplete(const std::string& id);

        void   setRunningStatCountMax(const std::string& id, const size_t value );
        size_t getRunningStatCountMax(const std::string & id);

        size_t getRunningStatCount(const std::string & id);




#ifdef BUILD_DLL
        boost::python::object getValue(const std::string& id );
        boost::python::object getRunningMean(const std::string& id );
        boost::python::object getRunningStandardDeviation(const std::string& id );
        boost::python::object getRunningVariance(const std::string& id );


        boost::python::dict getValue(const boost::python::list& ids);
        //boost::python::dict getValue(const std::vector<std::string>& ids);
        boost::python::object getTotalValue(const std::string& id );
        boost::python::dict getCounterAndValue(const std::string& id);
        //boost::python::dict getCounterAndValue(const std::vector<std::string>& ids);
        boost::python::dict getCounterAndValue(const boost::python::list& ids);
        boost::python::dict getCounterAndTotalValue(const std::string& id);
        boost::python::object getValue(const std::string& id, const int position);
        boost::python::list getValue(const std::string& id,
                                     const int start_position,
                                     const int end_position);

        boost::python::dict getBuffer(const std::string& id);


#endif

        size_t getPVCount(const std::string & id);
        bool isStringPV(const std::string& id);
        bool isIntPV(const std::string& id);
        bool isFloatPV(const std::string& id);
        bool isEnumPV(const std::string& id);
        bool isCharPV(const std::string& id);
        bool isLongPV(const std::string& id);
        bool isDoublePV(const std::string& id);
        bool isConnected(const std::string& id);
        bool isMonitoring(const std::string& id);
        bool isArrayDoublePV(const std::string& id);
        bool isArrayIntPV(const std::string& id);
        bool isArrayFloatPV(const std::string& id);
        bool isArrayPV(const std::string& id);
        bool isValidID(const std::string& id);
      /// These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val );
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE >  getILockStates( const std::string & name );
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string  >  getILockStatesStr( const std::string & name );

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


        std::vector<size_t> getArrayRegionOfInterest(const int start_position,const int end_position,const size_t vec_size );
        size_t getArrayIndex(const int index,const size_t vec_size);

        int sendToEpics(const std::string& ca,const std::string& mess1,const std::string& mess2 );

        // loose this function ... :
        void updateTimeAndValue(const std::string& id, const void* dbr);
        // ... and put it all in here.... (no really trust me)
        void updateValue(const std::string& id,const event_handler_args& argsr);

        void updateTime(const epicsTimeStamp& stamp, double& val, std::string& str);
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
        std::map<std::string,gmStructs::pvData<std::vector<float>> >  vec_floatPVMap;
        std::map<std::string,gmStructs::pvData<std::vector<int>>>     vec_intPVMap;
        std::map<std::string,gmStructs::pvData<std::string>>          stringPVMap;

        const std::string stringPrefix,intPrefix,shortPrefix,floatPrefix,enumPrefix,
                          charPrefix,longPrefix,doublePrefix,vecdoublePrefix,vecintPrefix,
                          vecfloatPrefix;

        void addSingleDouble(const std::string & id,const event_handler_args& args);
        void addSingleFloat(const std::string & id,const event_handler_args& args);
        void addArrayDouble(const std::string & id,const event_handler_args& args);
        void addArrayFloat(const std::string & id,const event_handler_args& args);
        void addArrayInt(const std::string & id,const event_handler_args& args);

        int pvMonitorMapCount;

        gmStructs::pvStruct* addToIntPVMap();
        gmStructs::pvStruct* addToFloatPVMap();
        gmStructs::pvStruct* addToDoublePVMap();
        gmStructs::pvStruct* addToCharPVMap();
        gmStructs::pvStruct* addToLongPVMap();
        gmStructs::pvStruct* addToEnumPVMap();
        gmStructs::pvStruct* addToVecDoublePVMap();
        gmStructs::pvStruct* addToVecIntPVMap();
        gmStructs::pvStruct* addToVecFloatPVMap();
        gmStructs::pvStruct* addToStringPVMap();

        void updateTime_ns(const epicsTimeStamp & stamp, double& s);
        void updateTime_str(const epicsTimeStamp & stamp, std::string& s);


        template<typename T, size_t size>
        size_t GetArrLength(T(&)[size]){ return size; }

        //virtual
        void updateMessageStates(){};

};//VCgeneralMonitor

#ifdef BUILD_DLL
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

const char * clearRunningValues_docstring =
    "Clear the running mean, variance and standard deviation (automatically re-starts calculating running values";
const char * getRunningMean_docstring =
    "Get the running mean value for this id ";
const char * getRunningStandardDeviation_docstring =
    "Get the running standard devaition for this id ";
const char * getRunningVariance_docstring =
    "Get the running variance for this id ";


const char * isRunningStatComplete_docstring =
   "Returns true if this 'id' has reached the shot count requested for the Running-Statistics calculation.";

const char * isRunningStatNotComplete_docstring =
   "Returns tyrue if this 'id' has NOT yet reached the shot count requested for the Running-Statistics calculation.";

const char * setRunningStatShotCount_docstring =
   "Sets the number of shots to include in for the running stats calcualtion for this 'id'";
const char * getRunningStatCountMax_doc =
 "Get the running stat. shot count for this id";
const char * getRunningStatCount_doc =
 "Get the running stat. maximum number of shots for this id";


const char * isBufferFull_docstring = "returns if buffer is full for 'id'";
const char * clearBuffer_docstring = "clears buffer for 'id'";
const char * setBufferSize_docstring = "sets buffer size to 'value' for 'id'";
const char * getBuffer_docstring = "returns buffer (dictionary of <timestamp, value>) for 'id'";
const char * getBufferCount_docstring = "returns number of entries in buffer for 'id'";
const char * getBufferSize_docstring = "returns max size of buffer for 'id'";

using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_CLARA_General_Monitor)
{
    //using namespace boost::python;
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();
    /*
        Things that you want to use in python muct be exposed:
        containers
    */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    /*
        // IF WE BUILD INDIVIDUAL CONTRLLER S FOR EACH SUB-HARDWRAE SYSTEM
        // THEN THE DEFINITIONS NEED TO MOVE TO THOSE CLASSES
        // I.E A VClasermirror.pyd
        // we can then use pre-processor commands to include thwe desired defintions
        // similar to   BOOST_PYTHON_INCLUDE::export_BaseObjects();
    */
    using namespace UTL;
    using namespace boost::python;
    using namespace boost;

    /// The main class that creates all the controller obejcts
    //class_<VCgeneralMonitor,boost::noncopyable> ("init")
    class_<VCgeneralMonitor,bases<controller,VCbase>, boost::noncopyable> ("init")

        .def("getRunningStatCount",  &VCgeneralMonitor::getRunningStatCount,
                    (ID_ARG,
                    getRunningStatCountMax_doc))

        .def("getRunningStatCountMax",  &VCgeneralMonitor::getRunningStatCountMax,
                    (ID_ARG,
                    getRunningStatCountMax_doc))

        .def("setRunningStatCountMax",  &VCgeneralMonitor::setRunningStatCountMax,
                        (ID_ARG,VALUE_ARG,
                        setRunningStatShotCount_docstring))

        .def("isBufferFull",  &VCgeneralMonitor::isBufferFull,
                        (ID_ARG,
                        isBufferFull_docstring))

        .def("clearBuffer",  &VCgeneralMonitor::clearBuffer,
                        (ID_ARG,
                        clearBuffer_docstring))
        .def("setBufferSize",  &VCgeneralMonitor::setBufferSize,
                        (ID_ARG,VALUE_ARG,
                        setBufferSize_docstring))

        .def("getBuffer",  &VCgeneralMonitor::getBuffer,
                        (ID_ARG,
                        getBuffer_docstring))
        .def("getBufferCount",  &VCgeneralMonitor::getBufferCount,
                        (ID_ARG,
                        getBufferCount_docstring))
        .def("getBufferSize",  &VCgeneralMonitor::getBufferSize,
                        (ID_ARG,
                        getBufferSize_docstring))

        .def("isRunningStatComplete",  &VCgeneralMonitor::isRunningStatComplete,
                          (ID_ARG,
                          isRunningStatComplete_docstring))
        .def("isRunningStatNotComplete",  &VCgeneralMonitor::isRunningStatNotComplete,
                          (ID_ARG,
                          isRunningStatNotComplete_docstring))

        .def("clearRunningValues",  &VCgeneralMonitor::clearRunningValues,
                          (ID_ARG,
                          clearRunningValues_docstring))

        .def("getRunningMean",  &VCgeneralMonitor::getRunningMean,
                          (ID_ARG,
                          getRunningMean_docstring))

        .def("getRunningStandardDeviation",  &VCgeneralMonitor::getRunningStandardDeviation,
                          (ID_ARG,
                          getRunningStandardDeviation_docstring))

        .def("getRunningVariance",  &VCgeneralMonitor::getRunningVariance,
                          (ID_ARG,
                          getRunningVariance_docstring))

        .def("getValue",  getValue_3,
                          (ID_ARG,boost::python::arg("start_index"),
                          boost::python::arg("end_index")),
                          getValue_3_docstring)

        .def("getValue",  getValue_2,
                          (ID_ARG,boost::python::arg("index")),
                          getValue_2_docstring)

        .def("getValue",  getValue_4,
                          (IDS_ARG),
                          getValue_4_docstring)

        .def("getValue",  getValue_1,
                          (ID_ARG,
                          getValue_1_docstring))

        .def("connectPV", connectPV_1,
                          (boost::python::arg("pv_name")),
                          connectPV_1_docstring)

        .def("connectPV", connectPV_2,
                          (boost::python::arg("pv_name"),boost::python::arg("pv_type")),
                          connectPV_2_docstring )

        .def("getCounterAndValue", getCounterAndValue_1,
                                   (ID_ARG,
                                   getCounterAndValue_1_docstring))

        .def("getCounterAndValue", getCounterAndValue_2,
                                   (IDS_ARG,
                                   getCounterAndValue_2_docstring))

        .def("getPVCount", &VCgeneralMonitor::getPVCount,
                           (ID_ARG),
                           getPVCount_docstring)

        .def("getTotalValue", &VCgeneralMonitor::getTotalValue,
                              (ID_ARG,
                              getTotalValue_docstring))

        .def("getCounter", &VCgeneralMonitor::getCounter,
                           (ID_ARG,
                           getCount_docstring))

        .def("getCounterAndTotalValue", &VCgeneralMonitor::getCounterAndTotalValue,
                                        (ID_ARG,
                                        getCounterAndTotalValue_docstring))

        .def("isStringPV", &VCgeneralMonitor::isStringPV,
                           (ID_ARG,
                           isStringPV_docstring))

        .def("isIntPV",    &VCgeneralMonitor::isIntPV,
                           (ID_ARG,
                           isIntPV_docstring))

        .def("isFloatPV",  &VCgeneralMonitor::isFloatPV,
                           (ID_ARG,
                           isFloatPV_docstring))

        .def("isEnumPV",   &VCgeneralMonitor::isEnumPV,
                           (ID_ARG,
                           isEnumPV_docstring))


        .def("isCharPV",   &VCgeneralMonitor::isCharPV,
                           (ID_ARG,
                           isCharPV_docstring))

        .def("isLongPV",   &VCgeneralMonitor::isLongPV,
                           (ID_ARG,
                           isLongPV_docstring))

        .def("isDoublePV", &VCgeneralMonitor::isDoublePV,
                           (ID_ARG,
                           isDoublePV_docstring))

        .def("isArrayPV",  &VCgeneralMonitor::isArrayPV,
                           (ID_ARG,
                           isArrayPV_docstring))

        .def("isConnected",&VCgeneralMonitor::isConnected,
                           (ID_ARG,
                           isConnected_docstring))

        .def("isMonitoring",&VCgeneralMonitor::isMonitoring,
                            (ID_ARG,
                            isMonitoring_docstring))

        .def("disconnectPV",&VCgeneralMonitor::disconnectPV,
                            (ID_ARG,
                            disconnectPV_docstring))

        .def("get_CA_PEND_IO_TIMEOUT", pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )

        .def("set_CA_PEND_IO_TIMEOUT", pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        ;
}
#endif //#ifdef BUILD_DLL
#endif // _VC_GENERAL_MONITOR_H
