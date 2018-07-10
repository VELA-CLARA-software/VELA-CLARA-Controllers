#ifndef VCMAGNETS_H
#define VCMAGNETS_H
//
#include "magnetController.h"
#include "VCheader.h"
#include "VCbase.h"
//
//
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>

class VCmagnets : public VCbase
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for explicit functions as a quick fix
    public:
        VCmagnets();
        ~VCmagnets();



        magnetController& virtual_CLARA_PH1_Magnet_Controller();
        magnetController& offline_CLARA_PH1_Magnet_Controller();
        magnetController& physical_CLARA_PH1_Magnet_Controller();

        magnetController& virtual_VELA_INJ_Magnet_Controller();
        magnetController& offline_VELA_INJ_Magnet_Controller();
        magnetController& physical_VELA_INJ_Magnet_Controller();

        magnetController& virtual_VELA_BA1_Magnet_Controller();
        magnetController& offline_VELA_BA1_Magnet_Controller();
        magnetController& physical_VELA_BA1_Magnet_Controller();

        magnetController& virtual_VELA_BA2_Magnet_Controller();
        magnetController& offline_VELA_BA2_Magnet_Controller();
        magnetController& physical_VELA_BA2_Magnet_Controller();

        magnetController& virtual_CB1_Magnet_Controller();
        magnetController& offline_CB1_Magnet_Controller();
        magnetController& physical_CB1_Magnet_Controller();

        magnetController& virtual_CB2_Magnet_Controller();
        magnetController& offline_CB2_Magnet_Controller();
        magnetController& physical_CB2_Magnet_Controller();

        magnetController& virtual_C2B_Magnet_Controller();
        magnetController& offline_C2B_Magnet_Controller();
        magnetController& physical_C2B_Magnet_Controller();


        magnetController& getMagnetController(HWC_ENUM::MACHINE_MODE mode, HWC_ENUM::MACHINE_AREA area);

        magnetController& virtual_USER_Magnet_Controller(const std::string & config_path);
        magnetController& offline_USER_Magnet_Controller(const std::string & config_path);
        magnetController& physical_USER_Magnet_Controller(const std::string & config_path);

//
//        magnetController& virtual_VELA_BA2_Magnet_Controller();
//        magnetController& offline_VELA_BA2_Magnet_Controller();
//        magnetController& physical_VELA_BA2_Magnet_Controller();
//
//        magnetController& virtual_CLARA_PH1_Magnet_Controller();
//        magnetController& offline_CLARA_PH1_Magnet_Controller();
//        magnetController& physical_CLARA_PH1_Magnet_Controller();
//
//        void setQuiet();
//        void setVerbose();
//        void setMessage();
//        void setDebugMessage();

    protected:

    private:

        magnetController* virtual_CLARA_PH1_Magnet_Controller_Obj;
        magnetController* offline_CLARA_PH1_Magnet_Controller_Obj;
        magnetController* physical_CLARA_PH1_Magnet_Controller_Obj;

        magnetController* offline_CLARA_BA1_Magnet_Controller_Obj;
        magnetController* physical_CLARA_BA1_Magnet_Controller_Obj;
        magnetController* virtual_CLARA_BA1_Magnet_Controller_Obj;

        magnetController* offline_CLARA_BA2_Magnet_Controller_Obj;
        magnetController* physical_CLARA_BA2_Magnet_Controller_Obj;
        magnetController* virtual_CLARA_BA2_Magnet_Controller_Obj;

        magnetController* virtual_VELA_INJ_Magnet_Controller_Obj;
        magnetController* offline_VELA_INJ_Magnet_Controller_Obj;
        magnetController* physical_VELA_INJ_Magnet_Controller_Obj;

        magnetController* virtual_VELA_BA1_Magnet_Controller_Obj;
        magnetController* offline_VELA_BA1_Magnet_Controller_Obj;
        magnetController* physical_VELA_BA1_Magnet_Controller_Obj;

        magnetController* virtual_VELA_BA2_Magnet_Controller_Obj;
        magnetController* offline_VELA_BA2_Magnet_Controller_Obj;
        magnetController* physical_VELA_BA2_Magnet_Controller_Obj;


        magnetController* virtual_CB1_Magnet_Controller_Obj;
        magnetController* offline_CB1_Magnet_Controller_Obj;
        magnetController* physical_CB1_Magnet_Controller_Obj;

        magnetController* virtual_CB2_Magnet_Controller_Obj;
        magnetController* offline_CB2_Magnet_Controller_Obj;
        magnetController* physical_CB2_Magnet_Controller_Obj;

        magnetController* virtual_USER_Magnet_Controller_Obj;
        magnetController* offline_USER_Magnet_Controller_Obj;
        magnetController* physical_USER_Magnet_Controller_Obj;

        magnetController* physical_C2B_Magnet_Controller_Obj;
        magnetController* offline_C2B_Magnet_Controller_Obj;
        magnetController* virtual_C2B_Magnet_Controller_Obj;


        magnetController& getController(magnetController*& cont,
                                        const std::string& conf,
                                        const std::string & name,
                                        const bool shouldVM,
                                        const bool shouldEPICS,
                                        const HWC_ENUM::MACHINE_AREA myMachineArea);

//        magnetController& getMagnetController(HWC_ENUM::MACHINE_MODE mode, HWC_ENUM::MACHINE_AREA area);

        std::map<magnetController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        const std::string CLARA_PH1_CONFIG,
                          VELA_INJ_MAG_CONFIG,
                          VELA_BA1_MAG_CONFIG,
                          VELA_BA2_MAG_CONFIG,
                          CB1_MAG_CONFIG,
                          CB2_MAG_CONFIG,
                          C2B_MAG_CONFIG;
};

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

typedef magnetStructs::magnetStateStruct msst;
typedef magnetStructs::MAG_TYPE     magt;
typedef magnetStructs::MAG_PSU_STATE    mpsu;

//typedef std::vector<magnetStructs::MAG_REV_TYPE> vmgrt;
typedef std::vector<magnetStructs::MAG_TYPE> vmgt;
typedef std::vector<magnetStructs::MAG_PSU_STATE>  vpsu;
typedef const boost::python::list & cbpl;

doub(magnetController::*getRI_1)(cstr&) = &magnetController::getRI;
vecd(magnetController::*getRI_2)(cves&) = &magnetController::getRI;
vecd(magnetController::*getSI_2)(cves&) = &magnetController::getSI;
doub(magnetController::*getSI_1)(cstr&) = &magnetController::getSI;


doub(magnetController::*getRITolerance_1)(cstr&) = &magnetController::getRITolerance;
vecd(magnetController::*getRITolerance_2)(cves&) = &magnetController::getRITolerance;
void(magnetController::*setRITolerance_1)(cstr&, doub) = &magnetController::setRITolerance;
void(magnetController::*setRITolerance_2)(cves&, cved&) = &magnetController::setRITolerance;
//
bool(magnetController::*setSI_1)(cstr&, cdou) = &magnetController::setSI;
bool(magnetController::*setSI_2)(cves&, cved&) = &magnetController::setSI;

bool(magnetController::*setSI_3)(cstr&, cdou , cdou , csiz) = &magnetController::setSI;
vecs(magnetController::*setSI_4)(cves&, cved&, cved&, csiz) = &magnetController::setSI;

//bool(magnetController::*setSI_5)(cbpl&, cbpl&) = &magnetController::setSI;
//
bool(magnetController::*switchONpsu_1)(cstr&) = &magnetController::switchONpsu;
bool(magnetController::*switchONpsu_2)(cves&) = &magnetController::switchONpsu;
bool(magnetController::*switchOFFpsu_1)(cstr&) = &magnetController::switchOFFpsu;
bool(magnetController::*switchOFFpsu_2)(cves&) = &magnetController::switchOFFpsu;
//
size_t(magnetController::*degauss_1)(cstr&, bool) = &magnetController::degauss;
size_t(magnetController::*degauss_2)(cves&, bool) = &magnetController::degauss;
//size_t(magnetController::*degauss_3)(cbpl&, bool) = &magnetController::degauss;
//
bool(magnetController::*writeDBURT_1)(const msst&, cstr&, cstr&, cstr&) = &magnetController::writeDBURT;
bool(magnetController::*writeDBURT_2)(             cstr&, cstr&, cstr&) = &magnetController::writeDBURT;
//
msst(magnetController::*getCurrentMagnetState_1)(cves&) = &magnetController::getCurrentMagnetState;
msst(magnetController::*getCurrentMagnetState_2)(    ) = &magnetController::getCurrentMagnetState;
//
magt(magnetController::*getMagType_1)(cstr&) = &magnetController::getMagType;
vmgt(magnetController::*getMagType_2)(cves&) = &magnetController::getMagType;
//
mpsu(magnetController::*getMagPSUState_1)(cstr&) = &magnetController::getMagPSUState;
vpsu(magnetController::*getMagPSUState_2)(cves&) = &magnetController::getMagPSUState;
//
vecd (magnetController::*getDegValues_1)(cstr &) = &magnetController::getDegValues;
vvcd (magnetController::*getDegValues_2)(cves &) = &magnetController::getDegValues;
//
size_t (magnetController::*getNumDegSteps_1)(cstr &) = &magnetController::getNumDegSteps;
vsiz (magnetController::*getNumDegSteps_2)(cves &) = &magnetController::getNumDegSteps;
//
doub (magnetController::*getPosition_1)(cstr &) = &magnetController::getPosition;
vecd (magnetController::*getPosition_2)(cves &) = &magnetController::getPosition;
//
vecd (magnetController::*getFieldIntegralCoefficients_1)(cstr &) = &magnetController::getFieldIntegralCoefficients;
vvcd (magnetController::*getFieldIntegralCoefficients_2)(cves &) = &magnetController::getFieldIntegralCoefficients;
//
doub (magnetController::*getMagneticLength_1)(cstr &) = &magnetController::getMagneticLength;
vecd (magnetController::*getMagneticLength_2)(cves &) = &magnetController::getMagneticLength;
//
stri  (magnetController::*getManufacturer_1)(cstr &) = &magnetController::getManufacturer;
vecs  (magnetController::*getManufacturer_2)(cves &) = &magnetController::getManufacturer;
//
stri  (magnetController::*getSerialNumber_1)(cstr &) = &magnetController::getSerialNumber;
vecs  (magnetController::*getSerialNumber_2)(cves &) = &magnetController::getSerialNumber;
//
stri  (magnetController::*getMagnetBranch_1)(cstr &) = &magnetController::getMagnetBranch;
vecs  (magnetController::*getMagnetBranch_2)(cves &) = &magnetController::getMagnetBranch;
//
stri  (magnetController::*getMeasurementDataLocation_1)(cstr &) = &magnetController::getMeasurementDataLocation;
vecs  (magnetController::*getMeasurementDataLocation_2)(cves &) = &magnetController::getMeasurementDataLocation;
//
bool  (magnetController::*setSIZero_1)(cstr &) = &magnetController::setSIZero;
bool  (magnetController::*setSIZero_2)(cves &) = &magnetController::setSIZero;


//reg_std_vector_string()
//{
//    class_<std::vector<std::string>>("std_vector_string")
//    .def(vector_indexing_suite<std::vector<std::string>>())
//    ;
//}

using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_CLARA_Magnet_Control)
{
    /*  some doc-string options, to try an dreuce th eamount of
        blurb python adds to documentation
    */
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();
    /// Include ALL the enums you want to expose to Python
    BOOST_PYTHON_INCLUDE::export_BaseObjects();

    class_<std::vector<magnetStructs::MAG_TYPE>>("std_vector_mag_type ")
        .def(vector_indexing_suite<std::vector<magnetStructs::MAG_TYPE>>())
        ;
    /// and enums, remember we have a enum to string python dictionary macro too!
    enum_<magnetStructs::MAG_PSU_STATE>("MAG_PSU_STATE","MAG_PSU_STATE Doc String")
        .value("MAG_PSU_ON",   magnetStructs::MAG_PSU_STATE::ON  )
        .value("MAG_PSU_OFF",    magnetStructs::MAG_PSU_STATE::OFF   )
        .value("MAG_PSU_ERROR", magnetStructs::MAG_PSU_STATE::ERROR)
        .value("MAG_PSU_NONE",  magnetStructs::MAG_PSU_STATE::NONE )
        ;

    /// and enums, remember we have a enum to string python dictionary macro too!
    enum_<magnetStructs::MAG_TYPE>("MAG_TYPE","MAG_TYPE Doc String")
        .value("QUAD", magnetStructs::MAG_TYPE::QUAD)
        .value("DIP" , magnetStructs::MAG_TYPE::DIP )
        .value("HCOR", magnetStructs::MAG_TYPE::HCOR)
        .value("VCOR", magnetStructs::MAG_TYPE::VCOR)
        .value("BSOL", magnetStructs::MAG_TYPE::BSOL)
        .value("SOL" , magnetStructs::MAG_TYPE::SOL )
        .value("SEXT", magnetStructs::MAG_TYPE::SEXT)
        .value("UNKNOWN_MAGNET_TYPE",  magnetStructs::MAG_TYPE::UNKNOWN_MAGNET_TYPE )
        ;
    /// and enums, remember we have a enum to string python dictionary macro too!
    enum_<magnetStructs::MAG_REV_TYPE>("MAG_REV_TYPE","MAG_REV_TYPE Doc String")
        .value("NR"       , magnetStructs::MAG_REV_TYPE::NR)
        .value("BIPOLAR"  , magnetStructs::MAG_REV_TYPE::BIPOLAR )
        .value("NR_GANGED", magnetStructs::MAG_REV_TYPE::NR_GANGED)/// hopefully never again....
        .value("POS"      , magnetStructs::MAG_REV_TYPE::POS)
        .value("UNKNOWN_MAG_REV_TYPE", magnetStructs::MAG_REV_TYPE::UNKNOWN_MAG_REV_TYPE)
        ;
    /// structs (this one is a one-stop shop for comomn parameters)
    boost::python::class_<magnetStructs::magnetStateStruct>
        ("magnetStateStruct","magnetStateStruct Doc String")
        .add_property("numMags",     &magnetStructs::magnetStateStruct::numMags)
        .add_property("machineArea", &magnetStructs::magnetStateStruct::machineArea)
        .add_property("magNames",    &magnetStructs::magnetStateStruct::magNames)
        .add_property("psuStates",   &magnetStructs::magnetStateStruct::psuStates)
        .add_property("siValues",    &magnetStructs::magnetStateStruct::siValues)
        .add_property("riValues",    &magnetStructs::magnetStateStruct::riValues)
        ;

    // magnet object struct to be exposed, used when returning a magnetobject reference
    boost::python::class_<magnetStructs::magnetObject,boost::noncopyable>
        ("magnetObject","magnetObject Doc String", boost::python::no_init)
        .def_readonly("pvRoot",       &magnetStructs::magnetObject::pvRoot)
        .def_readonly("psuRoot",      &magnetStructs::magnetObject::psuRoot)
        .def_readonly("revType",      &magnetStructs::magnetObject::revType)
        .def_readonly("maxWaitTime",  &magnetStructs::magnetObject::maxWaitTime)
        .def_readonly("degTolerance", &magnetStructs::magnetObject::degTolerance)
        .def_readonly("magType",      &magnetStructs::magnetObject::magType)
        .def_readonly("psuState",     &magnetStructs::magnetObject::psuState)
        .def_readonly("siWithPol",    &magnetStructs::magnetObject::siWithPol)
        .add_property("SI", &magnetStructs::magnetObject::siWithPol,
                            &magnetStructs::magnetObject::setSI)
        .add_property("PSU",&magnetStructs::magnetObject::psuState,
                            &magnetStructs::magnetObject::setPSU)
        .add_property("seti", &magnetStructs::magnetObject::seti,
                            &magnetStructs::magnetObject::seti_setter)
        .def_readonly("isDegaussing",          &magnetStructs::magnetObject::isDegaussing)
        .def_readonly("remainingDegaussSteps", &magnetStructs::magnetObject::remainingDegaussSteps)
        .def_readonly("riWithPol",    &magnetStructs::magnetObject::riWithPol)
        .def_readonly("riTolerance",  &magnetStructs::magnetObject::riTolerance)
        .def_readonly("name",         &magnetStructs::magnetObject::name)
        .def_readonly("degValues",    &magnetStructs::magnetObject::degValues)
        .def_readonly("position",     &magnetStructs::magnetObject::position)
        .def_readonly("fieldIntegralCoefficients",  &magnetStructs::magnetObject::fieldIntegralCoefficients )
        .def_readonly("numDegaussSteps", &magnetStructs::magnetObject::numDegaussSteps)
        .def_readonly("manufacturer",    &magnetStructs::magnetObject::manufacturer)
        .def_readonly("serialNumber",    &magnetStructs::magnetObject::serialNumber)
        .def_readonly("measurementDataLocation",  &magnetStructs::magnetObject::measurementDataLocation)
        .def_readonly("magneticLength",  &magnetStructs::magnetObject::magneticLength)
        .def_readonly("magnetBranch",    &magnetStructs::magnetObject::magnetBranch)
        .def_readonly("SETIequalREADI",    &magnetStructs::magnetObject::SETIequalREADI)
        .def_readonly("maxI",    &magnetStructs::magnetObject::maxI)
        .def_readonly("minI",    &magnetStructs::magnetObject::minI)
        .def_readonly("readi",    &magnetStructs::magnetObject::siWithPol)
        ;

    /// and the class member functions to expose to python,
    /// remmeber to include enum definitions as boost::python::dict <int, string>
    /// as well as boost::python::dict <int, int>

    boost::python::class_<magnetController, boost::python::bases<controller>, boost::noncopyable>
        ("magnetController","magnetController Doc String",boost::python::no_init)
        .def("getILockStates",           &magnetController::getILockStates         )
        .def("getILockStatesDefinition", &magnetController::getILockStatesDefinition)
        .def("get_CA_PEND_IO_TIMEOUT",   &magnetController::get_CA_PEND_IO_TIMEOUT )
        .def("set_CA_PEND_IO_TIMEOUT",   &magnetController::set_CA_PEND_IO_TIMEOUT )
        .def("getSI", getSI_1)
        .def("getSI", getSI_2)
        .def("getRI", getRI_1)
        .def("getRI", getRI_2)
        .def("setSI", setSI_1)
        .def("setSI", setSI_2)
        .def("setSI", setSI_3)
        .def("setSI", setSI_4)
        .def("setSI", &magnetController::setSI_Py1)
        .def("setSI", &magnetController::setSI_Py2)
        .def("getLastFailedSet", &magnetController::getLastFailedSet_Py,"Returns magnets that failed to be set [[m1,req_SI,read_si],[m2,..,..]...]")

        .def("getILockStates", &magnetController::getILockStates_Py)
        .def("getILockStatesStr", &magnetController::getILockStatesStr_Py)
        .def("getMagPSUStateDefinition", &magnetController::getMagPSUStateDefinition)
        .def("getILockStates", &magnetController::getILockStates_Py)
        .def("getMagnetNames", &magnetController::getMagnetNames_Py)
        .def("getQuadNames", &magnetController::getQuadNames_Py)
        .def("getHCorNames", &magnetController::getHCorNames_Py)
        .def("getVCorNames", &magnetController::getVCorNames_Py)
        .def("getDipNames", &magnetController::getDipNames_Py)
        .def("getSolNames", &magnetController::getSolNames_Py)
        .def("getCurrentMagnetState", &magnetController::getCurrentMagnetState_Py)
        .def("setSIZero", &magnetController::setSIZero_Py)
        .def("switchONpsu", &magnetController::switchONpsu_Py)
        .def("switchOFFpsu", &magnetController::switchOFFpsu_Py)
        .def("degauss", &magnetController::degauss_Py)
        .def("getSI", &magnetController::getSI_Py)
        .def("getRI", &magnetController::getRI_Py)
        .def("setRITolerance", &magnetController::setRITolerance_Py)
        .def("getMagType", &magnetController::getMagType_Py)
        .def("getMagPSUState", &magnetController::getMagPSUState_Py)
        .def("getNumDegSteps", &magnetController::getNumDegSteps_Py)
        .def("getDegValues", &magnetController::getDegValues_Py1)
        .def("getDegValues", &magnetController::getDegValues_Py2)
        .def("getFieldIntegralCoefficients", &magnetController::getFieldIntegralCoefficients_Py1)
        .def("getFieldIntegralCoefficients", &magnetController::getFieldIntegralCoefficients_Py2)
        .def("getPosition", &magnetController::getPosition_Py)
        .def("getMagneticLength", &magnetController::getMagneticLength_Py)
        .def("getManufacturer", &magnetController::getManufacturer_Py)
        .def("getPosition", &magnetController::getPosition_Py)
        .def("getMagnetBranch", &magnetController::getMagnetBranch_Py)
        .def("getMeasurementDataLocation", &magnetController::getMeasurementDataLocation_Py)


        .def("getRemainingDegaussSteps", &magnetController::getRemainingDegaussSteps,
             (boost::python::arg("magnetname")),"Get remiaing stesp in magnetname degaussing, -1 = degaussing failed for some reason, -2 = magnetname does not exist.")


        .def("entryExistsAndIsDegaussing", &magnetController::entryExistsAndIsDegaussing,
             (boost::python::arg("magnetname")),"magnetname exists, and is deaussing.")

        .def("entryExistsAndIsNotDegaussing", &magnetController::entryExistsAndIsNotDegaussing,
             (boost::python::arg("magnetname")),"magnetname exists, and is NOT deaussing.")


        .def("isNotDegaussing", &magnetController::isNotDegaussing,
             (boost::python::arg("magnetname")),"magnetname is NOT deaussing.")

        .def("isDegaussing", &magnetController::isDegaussing,
             (boost::python::arg("magnetname")),"magnetname is deaussing.")

        .def("setSIZero",  setSIZero_1)
        .def("setSIZero",  setSIZero_2)
        .def("switchONpsu", switchONpsu_1,(boost::python::arg("magnetname")),"Switch ON magnetname psu.")
        .def("switchONpsu", switchONpsu_2)
        .def("switchOFFpsu", switchOFFpsu_1)
        .def("switchOFFpsu", switchOFFpsu_2)
        .def("degauss", degauss_1,(
                        boost::python::arg("magnetname"),boost::python::arg("degaussToZero")=false),"deguass magnetname (single magnet), if degaussToZero = True then after degaussing the current will be left at zero, if degaussToZero = False the initial current will be reset.")
        .def("degauss", degauss_2,(
                        boost::python::arg("magnetnames"),boost::python::arg("degaussToZero")=false),"deguass magnetnames (mulitple magnets), if degaussToZero = True then after degaussing the current will be left at zero, if degaussToZero = False the initial current will be reset.")
//        .def("degauss", degauss_3,(
//                        boost::python::arg("magnetnames"),boost::python::arg("degaussToZero")=false),"deguass magnetnames (mulitple magnets), if degaussToZero = True then after degaussing the current will be left at zero, if degaussToZero = False the initial current will be reset.")
        .def("writeDBURT",               writeDBURT_1)
        .def("writeDBURT",               writeDBURT_2)
        .def("getCurrentMagnetState",    getCurrentMagnetState_1)
        .def("getCurrentMagnetState",    getCurrentMagnetState_2)
        .def("getMagObjConstRef",        &magnetController::getMagObjConstRef,return_value_policy<reference_existing_object>())
        .def("isAQuad",                  &magnetController::isAQuad   )
        .def("isABSol",                  &magnetController::isABSol   )
        .def("isAHCor",                  &magnetController::isAHCor   )
        .def("isAVCor",                  &magnetController::isAVCor   )
        .def("isADip",                   &magnetController::isADip   )
        .def("isASol",                   &magnetController::isASol   )
        .def("isACor",                   &magnetController::isACor   )
        .def("isON",                     &magnetController::isON         )
        .def("isOFF",                    &magnetController::isOFF        )
        .def("isDegaussing",             &magnetController::isDegaussing   )
        .def("isNotDegaussing",          &magnetController::isNotDegaussing)
        .def("getMagnetNames",           &magnetController::getMagnetNames_Py    )
        .def("getQuadNames",             &magnetController::getQuadNames_Py      )
        .def("getHCorNames",             &magnetController::getHCorNames_Py      )
        .def("getVCorNames",             &magnetController::getVCorNames_Py      )
        .def("setRITolerance",            setRITolerance_1    )
        .def("setRITolerance",            setRITolerance_2    )
        .def("getRITolerance",            getRITolerance_1    )
        .def("getRITolerance",            getRITolerance_2    )
        .def("getDipNames",              &magnetController::getDipNames_Py       )
        .def("getSolNames",              &magnetController::getSolNames_Py       )
        .def("applyDBURT",               &magnetController::applyDBURT        )
        .def("applyDBURTCorOnly",        &magnetController::applyDBURTCorOnly )
        .def("applyDBURTQuadOnly",       &magnetController::applyDBURTQuadOnly)
        .def("getDBURT",                 &magnetController::getDBURT          )
        .def("isRIequalVal",                 &magnetController::isRIequalVal          )
        .def("isRIequalSI",                 &magnetController::isRIequalSI          )
        .def("getmyMachineArea",   &magnetController::getmyMachineArea   )
        .def("getMagType",     getMagType_1     )
        .def("getMagType",     getMagType_2     )
        .def("getMagPSUState", getMagPSUState_1 )
        .def("getMagPSUState", getMagPSUState_2 )
        .def("getDegValues",   getDegValues_1   )
        .def("getDegValues",   getDegValues_2   )
        .def("getNumDegSteps",   getNumDegSteps_1   )
        .def("getNumDegSteps",   getNumDegSteps_2   )
        .def("getPosition",    getPosition_1    )
        .def("getPosition",    getPosition_2    )
        .def("getFieldIntegralCoefficients",    getFieldIntegralCoefficients_1    )
        .def("getFieldIntegralCoefficients",    getFieldIntegralCoefficients_2    )
        .def("getMagneticLength",    getMagneticLength_1    )
        .def("getMagneticLength",    getMagneticLength_2    )
        .def("getManufacturer",       getManufacturer_1       )
        .def("getManufacturer",       getManufacturer_2       )
        .def("getMagnetBranch",   getMagnetBranch_1   )
        .def("getMagnetBranch",   getMagnetBranch_2   )
        .def("getSerialNumber",   getSerialNumber_1   )
        .def("getSerialNumber",   getSerialNumber_2   )
        .def("getMeasurementDataLocation",   getMeasurementDataLocation_1   )
        .def("getMeasurementDataLocation",   getMeasurementDataLocation_2   )
        /// Don't forget functions in the base class we want to expose....
        .def("debugMessagesOff",         &magnetController::debugMessagesOff)
        .def("debugMessagesOn",          &magnetController::debugMessagesOn)
        .def("messagesOff",              &magnetController::messagesOff)
        .def("messagesOn",               &magnetController::messagesOn)
        .def("silence",                  &magnetController::silence)
        .def("verbose",                  &magnetController::verbose)
        .def("isSilent",                 &magnetController::isSilent)
        .def("isVerbose",                &magnetController::isVerbose)
        .def("isMessageOn",              &magnetController::isMessageOn)
        .def("isDebugMessageOn",         &magnetController::isDebugMessageOn)
        ;

    /// Finally the main class that creates all the objects
    boost::python::class_<VCmagnets, bases<VCbase>, boost::noncopyable>("init")
        .def("virtual_CLARA_PH1_Magnet_Controller",  &VCmagnets::virtual_CLARA_PH1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_PH1_Magnet_Controller",  &VCmagnets::offline_CLARA_PH1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_PH1_Magnet_Controller",  &VCmagnets::physical_CLARA_PH1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_INJ_Magnet_Controller",  &VCmagnets::virtual_VELA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_INJ_Magnet_Controller",  &VCmagnets::offline_VELA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_INJ_Magnet_Controller",  &VCmagnets::physical_VELA_INJ_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_USER_Magnet_Controller",  &VCmagnets::virtual_USER_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_USER_Magnet_Controller",  &VCmagnets::offline_USER_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_USER_Magnet_Controller",  &VCmagnets::physical_USER_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA2_Magnet_Controller",  &VCmagnets::virtual_VELA_BA2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA2_Magnet_Controller",  &VCmagnets::offline_VELA_BA2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA2_Magnet_Controller",  &VCmagnets::physical_VELA_BA2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_VELA_BA1_Magnet_Controller",  &VCmagnets::virtual_VELA_BA1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_BA1_Magnet_Controller",  &VCmagnets::offline_VELA_BA1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_BA1_Magnet_Controller",  &VCmagnets::physical_VELA_BA1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("getMagnetController",  &VCmagnets::getMagnetController,
             return_value_policy<reference_existing_object>())

        .def("virtual_CB1_Magnet_Controller",  &VCmagnets::virtual_CB1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CB1_Magnet_Controller",  &VCmagnets::offline_CB1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CB1_Magnet_Controller",  &VCmagnets::physical_CB1_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("virtual_CB2_Magnet_Controller",  &VCmagnets::virtual_CB2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CB2_Magnet_Controller",  &VCmagnets::offline_CB2_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CB2_Magnet_Controller",  &VCmagnets::physical_CB2_Magnet_Controller,
             return_value_policy<reference_existing_object>())

        .def("virtual_C2B_Magnet_Controller",  &VCmagnets::virtual_C2B_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_C2B_Magnet_Controller",  &VCmagnets::offline_C2B_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_C2B_Magnet_Controller",  &VCmagnets::physical_C2B_Magnet_Controller,
             return_value_policy<reference_existing_object>())
        ;
}


#endif // VCMAGNETS_H
