#ifndef vela_INJ_BPM_CONTROLLER_H
#define vela_INJ_BPM_CONTROLLER_H

// project
#include "beamPositionMonitorController.h"


#include "VCheader.h"
//


// stl
#include <string>
#include <vector>
//boost

#include <boost/python/detail/wrap_python.hpp>
#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
#define BOOST_LIB_DIAGNOSTIC
#include <boost/config.hpp>
#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/object/function.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/docstring_options.hpp>

class VCBPMs// : public beamPositionMonitorController
{
    public:

        /// we have overloaded constructors to specify config-file location
//        velaINJBeamPositionMonitorController( const bool show_messages = true, const bool show_debug_messages = true, const std::string configFileLocation );
        VCBPMs();
        ~VCBPMs();
        beamPositionMonitorController & virtual_VELA_INJ_BPM_Controller();
        beamPositionMonitorController & offline_VELA_INJ_BPM_Controller();
        beamPositionMonitorController & physical_VELA_INJ_BPM_Controller();
        beamPositionMonitorController & virtual_VELA_BA1_BPM_Controller();
        beamPositionMonitorController & offline_VELA_BA1_BPM_Controller();
        beamPositionMonitorController & physical_VELA_BA1_BPM_Controller();
        beamPositionMonitorController & virtual_VELA_BA2_BPM_Controller();
        beamPositionMonitorController & offline_VELA_BA2_BPM_Controller();
        beamPositionMonitorController & physical_VELA_BA2_BPM_Controller();
        beamPositionMonitorController & virtual_CLARA_INJ_BPM_Controller();
        beamPositionMonitorController & offline_CLARA_INJ_BPM_Controller();
        beamPositionMonitorController & physical_CLARA_INJ_BPM_Controller();
        beamPositionMonitorController & virtual_CLARA_2_VELA_BPM_Controller();
        beamPositionMonitorController & offline_CLARA_2_VELA_BPM_Controller();
        beamPositionMonitorController & physical_CLARA_2_VELA_BPM_Controller();
        beamPositionMonitorController & getBPMController( VELA_ENUM::MACHINE_MODE mode, VELA_ENUM::MACHINE_AREA area );

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

#ifdef BUILD_DLL

#endif // BUILD_DLL

    protected:
    private:

        bool withEPICS;
        bool withoutEPICS;
        bool withVM;
        bool withoutVM;
        bool showDebugMessages;
        bool showMessages;
        VELA_ENUM::MACHINE_AREA VELA_INJ;
        VELA_ENUM::MACHINE_AREA VELA_BA1;
        VELA_ENUM::MACHINE_AREA VELA_BA2;
        VELA_ENUM::MACHINE_AREA CLARA_INJ;
        VELA_ENUM::MACHINE_AREA CLARA_2_VELA;
        VELA_ENUM::MACHINE_AREA UNKNOWN_AREA;

        beamPositionMonitorController * virtual_VELA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * offline_VELA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * physical_VELA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_VELA_BA1_BPM_Controller_Obj;
        beamPositionMonitorController * offline_VELA_BA1_BPM_Controller_Obj;
        beamPositionMonitorController * physical_VELA_BA1_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_VELA_BA2_BPM_Controller_Obj;
        beamPositionMonitorController * offline_VELA_BA2_BPM_Controller_Obj;
        beamPositionMonitorController * physical_VELA_BA2_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_CLARA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * offline_CLARA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * physical_CLARA_INJ_BPM_Controller_Obj;
        beamPositionMonitorController * virtual_CLARA_2_VELA_BPM_Controller_Obj;
        beamPositionMonitorController * offline_CLARA_2_VELA_BPM_Controller_Obj;
        beamPositionMonitorController * physical_CLARA_2_VELA_BPM_Controller_Obj;

};

typedef std::vector<std::string> vecString;

class getVecString
{
    public:

    vecString returnVecString()
    {
     return vecString();
    }
    void setVecString(vecString & lis, std::string & name)
    {
        lis.push_back(name);
    }
};

#ifdef BUILD_DLL

/// Boost has incorporated an "autolink" feature.
/// http://www.codeproject.com/Articles/11597/Building-Boost-libraries-for-Visual-Studio
/// or do it yourself!



/// FUNCTION OVERLOADING, if you have overloaded functions:
/// Create a load of different function pointers and use them in the bindings
/// For examples See magnets...

/// As another example we have overloaded constructors to enable / disable messages
/// and / or debug messages when instantiating. See here
/// http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/exposing.html
/// and beware of: http://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor
void(beamPositionMonitorController::*monitorMultipleDataForNShots)(size_t, const std::vector< std::string > &) = &beamPositionMonitorController::monitorDataForNShots;
void(beamPositionMonitorController::*monitorDataForNShots)(size_t, const std::string &) = &beamPositionMonitorController::monitorDataForNShots;


using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_BPM_Control )
{

    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();
    /// Include ALL the enums you want to expose to Python
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    class_< std::vector< std::vector< double > > > ("v2_double")
        .def( vector_indexing_suite< std::vector< std::vector< double > > >())
        ;

    /// member functiosn to expose to python, remmeber to include enum deifntions as boost::python::dict <int, string>

    boost::python::class_<beamPositionMonitorStructs::bpmDataObject, boost::shared_ptr<beamPositionMonitorStructs::bpmDataObject>, boost::noncopyable>
        ("bpmDataObject", "This object contains all of the EPICS PVs for a given bpmName",boost::python::no_init)
        .def_readonly("pvRoot",         &beamPositionMonitorStructs::bpmDataObject::pvRoot       )
        .def_readonly("appendingData",  &beamPositionMonitorStructs::bpmDataObject::appendingData)
        .def_readonly("xPV",            &beamPositionMonitorStructs::bpmDataObject::xPV          )
        .def_readonly("yPV",            &beamPositionMonitorStructs::bpmDataObject::yPV          )
        .def_readonly("sa1",            &beamPositionMonitorStructs::bpmDataObject::sa1          )
        .def_readonly("sa2",            &beamPositionMonitorStructs::bpmDataObject::sa2          )
        .def_readonly("ra1",            &beamPositionMonitorStructs::bpmDataObject::ra1          )
        .def_readonly("ra2",            &beamPositionMonitorStructs::bpmDataObject::ra2          )
        .def_readonly("sd1",            &beamPositionMonitorStructs::bpmDataObject::sd1          )
        .def_readonly("sd2",            &beamPositionMonitorStructs::bpmDataObject::sd2          )
        .def_readonly("rd1",            &beamPositionMonitorStructs::bpmDataObject::rd1          )
        .def_readonly("rd2",            &beamPositionMonitorStructs::bpmDataObject::rd2          )
        .def_readonly("q",              &beamPositionMonitorStructs::bpmDataObject::q            )
        ;

    boost::python::class_<beamPositionMonitorStructs::rawDataStruct, boost::shared_ptr<beamPositionMonitorStructs::rawDataStruct>, boost::noncopyable>
        ("rawDataStruct", "This struct contains all the 'raw' data from the BPMs, including raw voltages and timestamps", boost::python::no_init)
        .def_readonly("name",           &beamPositionMonitorStructs::rawDataStruct::name         )
        .def_readonly("appendingData",  &beamPositionMonitorStructs::rawDataStruct::appendingData)
        .def_readonly("p1",             &beamPositionMonitorStructs::rawDataStruct::p1           )
        .def_readonly("p2",             &beamPositionMonitorStructs::rawDataStruct::p2           )
        .def_readonly("pu1",            &beamPositionMonitorStructs::rawDataStruct::pu1          )
        .def_readonly("pu2",            &beamPositionMonitorStructs::rawDataStruct::pu2          )
        .def_readonly("pu3",            &beamPositionMonitorStructs::rawDataStruct::pu3          )
        .def_readonly("pu4",            &beamPositionMonitorStructs::rawDataStruct::pu4          )
        .def_readonly("c1",             &beamPositionMonitorStructs::rawDataStruct::c1           )
        .def_readonly("c2",             &beamPositionMonitorStructs::rawDataStruct::c2           )
        .def_readonly("x",              &beamPositionMonitorStructs::rawDataStruct::x            )
        .def_readonly("y",              &beamPositionMonitorStructs::rawDataStruct::y            )
        .def_readonly("q",              &beamPositionMonitorStructs::rawDataStruct::q            )
        .def_readonly("timeStamps",     &beamPositionMonitorStructs::rawDataStruct::timeStamps   )
        .def_readonly("strTimeStamps",  &beamPositionMonitorStructs::rawDataStruct::strTimeStamps)
        .def_readonly("rawBPMData",     &beamPositionMonitorStructs::rawDataStruct::rawBPMData   )
        ;

    char const* getXDocString = "Returns (as a float) the current calculated PV of X for str(bpmName) - these are defined in the config file.\n"
                                "In theory this is more accurate than the EPICS PV.";
    char const* getYDocString = "Returns (as a float) the current calculated PV of Y for str(bpmName) - these are defined in the config file.\n"
                                "In theory this is more accurate than the EPICS PV.";
    char const* getQDocString = "Returns (as a float) the current calculated PV of Q for str(bpmName) - these are defined in the config file.\n";
    char const* getBPMRawDataDocString = "Returns (as a vector of vectors) the raw voltages for str(bpmName) - these are defined in the config file.\n"
                                         "To be used in conjunction with function monitorDataForNShots.";
    char const* getXPVDocString = "Returns (as a float) the current EPICS PV of X for str(bpmName) - these are defined in the config file.";
    char const* getYPVDocString = "Returns (as a float) the current EPICS PV of Y for str(bpmName) - these are defined in the config file.";
    char const* getRA1DocString = "Returns the current EPICS PV of RA1 for str(bpmName) - these are defined in the config file.";
    char const* getRA2DocString = "Returns the current EPICS PV of RA2 for str(bpmName) - these are defined in the config file.";
    char const* getRD1DocString = "Returns the current EPICS PV of RD1 for str(bpmName) - these are defined in the config file.";
    char const* getRD2DocString = "Returns the current EPICS PV of RD2 for str(bpmName) - these are defined in the config file.";
    char const* setSA1DocString = "Allows the user to set EPICS PV of SA1 for str(bpmName) - these are defined in the config file. arg2 is a long type.";
    char const* setSA2DocString = "Allows the user to set EPICS PV of SA2 for str(bpmName) - these are defined in the config file. arg2 is a long type.";
    char const* setSD1DocString = "Allows the user to set EPICS PV of SD1 for str(bpmName) - these are defined in the config file. arg2 is a long type.";
    char const* setSD2DocString = "Allows the user to set EPICS PV of SD2 for str(bpmName) - these are defined in the config file. arg2 is a long type.";
    char const* setXDocString = "Allows the user to set EPICS PV of X for str(bpmName) - these are defined in the config file. ONLY FOR THE VIRTUAL MACHINE!!!";
    char const* setYDocString = "Allows the user to set EPICS PV of Y for str(bpmName) - these are defined in the config file. ONLY FOR THE VIRTUAL MACHINE!!!";
    char const* monitorSglDocString = "Monitors raw voltages for str(bpmName) - these are defined in the config file. This will fill up a vector of vectors with shot-to-shot raw voltages.\n"
                                      "Data can be accessed using getBPMRawData, getBPMXVec, getBPMYVec, getBPMQVec.\n"
                                      "arg1 is an int, arg2 is a string";
    char const* monitorMulDocString = "Monitors raw voltages for std_vector_string(str(bpmName)) - these are defined in the config file. This will fill up a vector of vectors with shot-to-shot raw voltages.\n"
                                      "Data can be accessed using getBPMRawData, getBPMXVec, getBPMYVec, getBPMQVec.\n"
                                      "arg1 is an int, arg2 is a std_vector_string (in python use VELA_CLARA_BPM_Control.std_vector_string().";
    char const* isMonitoringDocString = "Returns true if str(bpmName) is being monitored - these are defined in the config file.";
    char const* isNotMonitoringDocString = "Returns true if str(bpmName) is not being monitored - these are defined in the config file.";
    char const* getXVecDocString = "Returns a vector containing the X values for str(bpmName) - these are defined in the config file.\n"
                                   "To be used in conjunction with function monitorDataForNShots.";
    char const* getYVecDocString = "Returns a vector containing the Y values for str(bpmName) - these are defined in the config file.\n"
                                   "To be used in conjunction with function monitorDataForNShots.";
    char const* getQVecDocString = "Returns a vector containing the Q values for str(bpmName) - these are defined in the config file.\n"
                                   "To be used in conjunction with function monitorDataForNShots.";
    char const* getTimeStampsDocString = "Returns a vector containing the timestamps as doubles for str(bpmName) - these are defined in the config file.\n"
                                         "To be used in conjunction with function monitorDataForNShots.";
    char const* getStrTimeStampsDocString = "Returns a vector containing the timestamps as strings (if that's your thing) for str(bpmName) - these are defined in the config file.\n"
                                         "To be used in conjunction with function monitorDataForNShots.";
    char const* getResDocString = "Returns the resolution (in mm) for str(bpmName) - these are defined in the config file.\n"
                                  "To be used in conjunction with function monitorDataForNShots.";
    char const* reCalAttDocString = "Re-calibrates the attenuation for str(bpmName) - these are defined in the config file - based on a charge reading (arg2).\n"
                                   "The charge reading could be accessed using the scopeController module.";
    char const* getAllBPMDataDocString = "Returns the raw data struct for str(bpmName) - these are defined in the config file.\n"
                                         "To be used in conjunction with function monitorDataForNShots. Type help(VELA_CLARA_BPM_Control.rawDataStruct) to see what this contains.";
    char const* getBPMDataObjectDocString = "Returns the bpm data object for str(bpmName) - these are defined in the config file.\n"
                                            "To be used in conjunction with function monitorDataForNShots. Type help(VELA_CLARA_BPM_Control.bpmDataObject) to see what this contains.";
    char const* getBPMNamesDocString = "Returns all of the BPM names defined in the config file.";
    char const* getAreaDocString = "Returns, as a VELA_ENUM, the machine area for the controller.";
    char const* getModeDocString = "Returns, as a VELA_ENUM, the machine mode for the controller (OFFLINE, PHYSICAL, VIRTUAL).";
    char const* getILocksDocString = "Why are you here? BPMs don't have interlocks. At least as far as I'm aware. I'm not sure why they would.";
	boost::python::class_<beamPositionMonitorController, boost::python::bases<controller>, boost::noncopyable>
            ("beamPositionMonitorController","This class contains all the functions in the BPM controller for monitoring and controlling PVs",boost::python::no_init)
//            .def(boost::python::init<const std::string, optional<const bool, const bool > >())
//            .def(boost::python::init< optional<const bool, const bool, const bool >>())
            .def("getAllBPMData",                   &beamPositionMonitorController::getAllBPMData, getAllBPMDataDocString, return_value_policy<reference_existing_object>())
            .def("getBPMDataObject",                &beamPositionMonitorController::getBPMDataObject, getBPMDataObjectDocString, return_value_policy<reference_existing_object>()            )
//            .def("getBPMStateDefinition",           &beamPositionMonitorController::getBPMStateDefinition                     )
            .def("getILockStatesDefinition",        &beamPositionMonitorController::getILockStatesDefinition    )
            .def("get_CA_PEND_IO_TIMEOUT",          &beamPositionMonitorController::get_CA_PEND_IO_TIMEOUT      )
            .def("set_CA_PEND_IO_TIMEOUT",          &beamPositionMonitorController::set_CA_PEND_IO_TIMEOUT      )
//            .def("getBPMState",                     &velaINJBeamPositionMonitorController::getBPMState_Py                            )
            .def("getILockStates",                  &beamPositionMonitorController::getILockStates, getILocksDocString              )
//            .def("hasNoTrig",                       &velaINJBeamPositionMonitorController::hasNoTrig_Py, boost::python::args("name") )
//            .def("hasTrig",                         &velaINJBeamPositionMonitorController::hasTrig_Py, boost::python::args("name")   )
            .def("isMonitoringBPMData",             &beamPositionMonitorController::isMonitoringBPMData, isMonitoringDocString         )
            .def("isNotMonitoringBPMData",          &beamPositionMonitorController::isNotMonitoringBPMData, isNotMonitoringDocString      )
            .def("getX",                            &beamPositionMonitorController::getX, getXDocString         )
            .def("getY",                            &beamPositionMonitorController::getY, getQDocString                        )
            .def("getQ",                            &beamPositionMonitorController::getQ, getQDocString                        )
            .def("reCalAttenuation",                &beamPositionMonitorController::reCalAttenuation, reCalAttDocString            )
            .def("getXFromPV",                      &beamPositionMonitorController::getXFromPV, getXPVDocString                  )
            .def("getYFromPV",                      &beamPositionMonitorController::getYFromPV, getYPVDocString                  )
            .def("getBPMResolution",                &beamPositionMonitorController::getBPMResolution, getResDocString            )
            .def("getBPMXVec",                      &beamPositionMonitorController::getBPMXVec, getXVecDocString                  )
            .def("getBPMYVec",                      &beamPositionMonitorController::getBPMYVec, getYVecDocString                  )
            .def("getBPMQVec",                      &beamPositionMonitorController::getBPMQVec, getQVecDocString                  )
            .def("getTimeStamps",                   &beamPositionMonitorController::getTimeStamps, getTimeStampsDocString               )
            .def("getStrTimeStamps",                &beamPositionMonitorController::getStrTimeStamps, getStrTimeStampsDocString            )
            .def("getBPMRawData",                   &beamPositionMonitorController::getBPMRawData, getBPMRawDataDocString               )
            .def("getRA1",                          &beamPositionMonitorController::getRA1, getRA1DocString                      )
            .def("getRA2",                          &beamPositionMonitorController::getRA2, getRA2DocString                      )
            .def("getRD1",                          &beamPositionMonitorController::getRD1, getRD1DocString                      )
            .def("getRD2",                          &beamPositionMonitorController::getRD2, getRD2DocString                      )
            .def("setSA1",                          &beamPositionMonitorController::setSA1, setSA2DocString                      )
            .def("setSA2",                          &beamPositionMonitorController::setSA2, setSA2DocString                      )
            .def("setSD1",                          &beamPositionMonitorController::setSD1, setSD1DocString                      )
            .def("setSD2",                          &beamPositionMonitorController::setSD2, setSD2DocString                      )
            .def("setX",                            &beamPositionMonitorController::setX, setXDocString                        )
            .def("setY",                            &beamPositionMonitorController::setY, setYDocString                        )
            .def("getMachineArea",                  &beamPositionMonitorController::getMachineArea, getAreaDocString                        )
            .def("getMachineMode",                  &beamPositionMonitorController::getMachineMode, getModeDocString                        )
            .def("monitorDataForNShots",            monitorMultipleDataForNShots, monitorSglDocString                                )
            .def("monitorDataForNShots",            monitorDataForNShots, monitorMulDocString                                        )
            //.def("monitorDataForNShots", static_cast< void(beamPositionMonitorController::*) (size_t, const std::vector< std::string >&)>
            //    (&beamPositionMonitorController::monitorDataForNShots), monitorMultipleDataForNShots()          )
            //.def("monitorDataForNShots", static_cast< void(beamPositionMonitorController::*) (size_t, const std::string&)>
            //    (&beamPositionMonitorController::monitorDataForNShots), monitorDataForNShots()                  )
            .def("getBPMNames",                     &beamPositionMonitorController::getBPMNames, getBPMNamesDocString                 )
            /// Don't forget functions in the base class we want to expose....
            .def("debugMessagesOff",                &beamPositionMonitorController::debugMessagesOff            )
            .def("debugMessagesOn",                 &beamPositionMonitorController::debugMessagesOn             )
            .def("messagesOff",                     &beamPositionMonitorController::messagesOff                 )
            .def("messagesOn",                      &beamPositionMonitorController::messagesOn                  )
            .def("silence",                         &beamPositionMonitorController::silence                     )
            .def("verbose",                         &beamPositionMonitorController::verbose                     )
            .def("isSilent",                        &beamPositionMonitorController::isSilent                    )
            .def("isVerbose",                       &beamPositionMonitorController::isVerbose                   )
            .def("isMessageOn",                     &beamPositionMonitorController::isMessageOn                 )
            .def("isDebugMessageOn",                &beamPositionMonitorController::isDebugMessageOn            )
		;

    char const* getControllerDocString = "This will instantiate a controller based on beamline and controller type. This requires enums as arguments.\n"
                                         "arg1 - machine mode - can be used as follows: mode=VELA_CLARA_BPM_Control.MACHINE_MODE.VIRTUAL/PHYSICAL/OFFLINE.\n"
                                         "arg2 - machine area - can be used as follows: mode=VELA_CLARA_BPM_Control.MACHINE_AREA.VELA_INJ/VELA_BA1/VELA_BA2/CLARA_INJ/CLARA_2_VELA.";
    boost::python::class_<VCBPMs,boost::noncopyable> ("init")
        .def("virtual_VELA_INJ_BPM_Controller",  &VCBPMs::virtual_VELA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_BPM_Controller",  &VCBPMs::offline_VELA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_BPM_Controller", &VCBPMs::offline_VELA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_BPM_Controller",  &VCBPMs::virtual_VELA_BA1_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_BPM_Controller",  &VCBPMs::offline_VELA_BA1_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_BPM_Controller", &VCBPMs::offline_VELA_BA1_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_BPM_Controller",  &VCBPMs::virtual_VELA_BA2_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_BPM_Controller",  &VCBPMs::offline_VELA_BA2_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_BPM_Controller", &VCBPMs::offline_VELA_BA2_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_INJ_BPM_Controller",  &VCBPMs::virtual_CLARA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_INJ_BPM_Controller",  &VCBPMs::offline_CLARA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_INJ_BPM_Controller", &VCBPMs::offline_CLARA_INJ_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("virtual_CLARA_2_VELA_BPM_Controller",  &VCBPMs::virtual_CLARA_2_VELA_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("offline_CLARA_2_VELA_BPM_Controller",  &VCBPMs::offline_CLARA_2_VELA_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("physical_CLARA_2_VELA_BPM_Controller", &VCBPMs::offline_CLARA_2_VELA_BPM_Controller, return_value_policy<reference_existing_object>())
        .def("getBPMController", &VCBPMs::getBPMController, getControllerDocString, return_value_policy<reference_existing_object>())
        .def("setQuiet",         &VCBPMs::setQuiet )
        .def("setVerbose",       &VCBPMs::setVerbose )
        .def("setMessage",       &VCBPMs::setMessage )
        .def("setDebugMessage",  &VCBPMs::setDebugMessage )
        ;

};

#endif // BUILD_DLL

#endif // vela_INJ_BPM_CONTROLLER_H
