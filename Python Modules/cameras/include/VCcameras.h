#ifndef VC_CAMERAS_H
#define VC_CAMERAS_H
//
#include "cameraController.h"
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

class VCcameras
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for explicit functions as a quick fix
    public:
        VCcameras();
        ~VCcameras();

        cameraController& virtual_CLARA_Camera_Controller();
        cameraController& offline_CLARA_Camera_Controller();
        cameraController& physical_CLARA_Camera_Controller();

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();



    protected:

    private:

        cameraController * virtual_Camera_Controller_Obj;
        cameraController * offline_Camera_Controller_Obj;
        cameraController * physical_Camera_Controller_Obj;


        cameraController& getController(cameraController * cont,const std::string& conf,
                                       const std::string & name, const bool shouldVM, const bool shouldEPICS,                                       const VELA_ENUM::MACHINE_AREA myMachineArea );


        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

        const VELA_ENUM::MACHINE_AREA  VELA_INJ,VELA_BA1,VELA_BA2,CLARA_PH1,UNKNOWN_AREA;
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

//typedef cameraStructs::cameraStateStruct csst;
//typedef cameraStructs::CAM_TYPE     camt;

//typedef std::vector<cameraStructs::CAM_TYPE> vcamt;
typedef const boost::python::list & cbpl;

///This stuff allows us to read the output of pyd in python i think (Duncan's code stuff from magnets)
/*
doub(magnetController::*getRI_1)(cstr&) = &magnetController::getRI;
vecd(magnetController::*getRI_2)(cves&) = &magnetController::getRI;
vecd(magnetController::*getSI_2)(cves&) = &magnetController::getSI;
doub(magnetController::*getSI_1)(cstr&) = &magnetController::getSI;


doub(magnetController::*getRITolerance_1)(cstr&) = &magnetController::getRITolerance;
vecd(magnetController::*getRITolerance_2)(cves&) = &magnetController::getRITolerance;
void(magnetController::*setRITolerance_1)(cstr&, doub) = &magnetController::setRITolerance;
void(magnetController::*setRITolerance_2)(cves&, cved&) = &magnetController::setRITolerance;
//
bool(magnetController::*setSI_1)(cstr&, cdou ) = &magnetController::setSI;
bool(magnetController::*setSI_2)(cves&, cved&) = &magnetController::setSI;

bool(magnetController::*setSI_3)(cstr&, cdou , cdou , csiz) = &magnetController::setSI;
vecs(magnetController::*setSI_4)(cves&, cved&, cved&, csiz) = &magnetController::setSI;

bool(magnetController::*setSI_5)(cbpl&, cbpl&) = &magnetController::setSI;
//
bool(magnetController::*switchONpsu_1 )(cstr&) = &magnetController::switchONpsu;
bool(magnetController::*switchONpsu_2 )(cves&) = &magnetController::switchONpsu;
bool(magnetController::*switchOFFpsu_1)(cstr&) = &magnetController::switchOFFpsu;
bool(magnetController::*switchOFFpsu_2)(cves&) = &magnetController::switchOFFpsu;
//
size_t(magnetController::*degauss_1)(cstr&, bool ) = &magnetController::degauss;
size_t(magnetController::*degauss_2)(cves&, bool ) = &magnetController::degauss;
size_t(magnetController::*degauss_3)(cbpl&, bool ) = &magnetController::degauss;
//
bool(magnetController::*writeDBURT_1)( const msst&, cstr&, cstr&, cstr&) = &magnetController::writeDBURT;
bool(magnetController::*writeDBURT_2)(              cstr&, cstr&, cstr&) = &magnetController::writeDBURT;
//
msst(magnetController::*getCurrentMagnetState_1)( cves&) = &magnetController::getCurrentMagnetState;
msst(magnetController::*getCurrentMagnetState_2)(      ) = &magnetController::getCurrentMagnetState;
//
magt(magnetController::*getMagType_1)( cstr& ) = &magnetController::getMagType;
vmgt(magnetController::*getMagType_2)( cves& ) = &magnetController::getMagType;
//
mpsu(magnetController::*getMagPSUState_1)( cstr& ) = &magnetController::getMagPSUState;
vpsu(magnetController::*getMagPSUState_2)( cves& ) = &magnetController::getMagPSUState;
//
vecd (magnetController::*getDegValues_1)( cstr & ) = &magnetController::getDegValues;
vvcd (magnetController::*getDegValues_2)( cves & ) = &magnetController::getDegValues;
//
size (magnetController::*getNumDegSteps_1)( cstr & ) = &magnetController::getNumDegSteps;
vsiz (magnetController::*getNumDegSteps_2)( cves & ) = &magnetController::getNumDegSteps;
//
doub (magnetController::*getPosition_1)( cstr & ) = &magnetController::getPosition;
vecd (magnetController::*getPosition_2)( cves & ) = &magnetController::getPosition;
//
vecd (magnetController::*getFieldIntegralCoefficients_1)( cstr & ) = &magnetController::getFieldIntegralCoefficients;
vvcd (magnetController::*getFieldIntegralCoefficients_2)( cves & ) = &magnetController::getFieldIntegralCoefficients;
//
doub (magnetController::*getMagneticLength_1)( cstr & ) = &magnetController::getMagneticLength;
vecd (magnetController::*getMagneticLength_2)( cves & ) = &magnetController::getMagneticLength;
//
stri  (magnetController::*getManufacturer_1)( cstr & ) = &magnetController::getManufacturer;
vecs  (magnetController::*getManufacturer_2)( cves & ) = &magnetController::getManufacturer;
//
stri  (magnetController::*getSerialNumber_1)( cstr & ) = &magnetController::getSerialNumber;
vecs  (magnetController::*getSerialNumber_2)( cves & ) = &magnetController::getSerialNumber;
//
stri  (magnetController::*getMagnetBranch_1)( cstr & ) = &magnetController::getMagnetBranch;
vecs  (magnetController::*getMagnetBranch_2)( cves & ) = &magnetController::getMagnetBranch;
//
stri  (magnetController::*getMeasurementDataLocation_1)( cstr & ) = &magnetController::getMeasurementDataLocation;
vecs  (magnetController::*getMeasurementDataLocation_2)( cves & ) = &magnetController::getMeasurementDataLocation;
//
bool  (magnetController::*setSIZero_1)( cstr & ) = &magnetController::setSIZero;
bool  (magnetController::*setSIZero_2)( cves & ) = &magnetController::setSIZero;
*/


using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Camera_Control )
{
    docstring_options doc_options(false);
    doc_options.disable_cpp_signatures();

    /// Things that you want to use in python muct be exposed:
    /// containers
    class_<std::vector< std::string > >("std_vector_string")
        .def( vector_indexing_suite< std::vector< std::string >>() )
        ;
    class_<std::vector<double>>("std_vector_double")
        .def( vector_indexing_suite< std::vector<double>>() )
        ;
      ;

    /// Expose base classes
    boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
        ;
    /// we have to tell boost.python about pure virtual methods in abstract base classes
    boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
        ("controller","controller Doc String", boost::python::no_init) /// forces Python to not be able to construct (init) this object
        .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
        .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
        .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
        .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
        ;
    // magnet object struct to be exposed, used when returning a magnetobject reference
    ///Things to see
    boost::python::class_<cameraStructs::cameraObject,boost::noncopyable>
        ("cameraObject","cameraObject Doc String", boost::python::no_init)
        .def_readonly("pvRoot",       &cameraStructs::cameraObject::pvRoot)
        .def_readonly("imageStruct",    &cameraStructs::cameraObject::imageStruct)
        ;
    boost::python::class_<cameraStructs::camImageStruct,boost::noncopyable>
        ("camImageStruct","camImageStruct Doc String", boost::python::no_init)
        .def_readonly("bit_depth",    &cameraStructs::camImageStruct::bit_depth)
        .def_readonly("image_height",    &cameraStructs::camImageStruct::image_height)
        .def_readonly("image_width",    &cameraStructs::camImageStruct::image_width)
        .def_readonly("xPix2mm",    &cameraStructs::camImageStruct::xPix2mm)
        .def_readonly("yPix2mm",    &cameraStructs::camImageStruct::yPix2mm)
        .def_readonly("xCenterPix",    &cameraStructs::camImageStruct::xCenterPix)
        .def_readonly("yCenterPix",    &cameraStructs::camImageStruct::yCenterPix)
        .def_readonly("xRAD",    &cameraStructs::camImageStruct::xRAD)
        .def_readonly("yRAD",    &cameraStructs::camImageStruct::yRAD)
        ;

    ///Things to do
    boost::python::class_<cameraController, boost::python::bases<controller>, boost::noncopyable>
        ("cameraController","cameraController Doc String",boost::python::no_init)
        .def("getILockStates",           &cameraController::getILockStates          )
        .def("getILockStatesDefinition", &cameraController::getILockStatesDefinition)
        .def("get_CA_PEND_IO_TIMEOUT",   &cameraController::get_CA_PEND_IO_TIMEOUT  )
        .def("set_CA_PEND_IO_TIMEOUT",   &cameraController::set_CA_PEND_IO_TIMEOUT  )
        .def("cameraName",   &cameraController::cameraName  )
        .def("setCamera",   &cameraController::setCamera  )
        .def("getCamObjConstRef",        &cameraController::getCamObjConstRef,return_value_policy<reference_existing_object>())
        /*.def("getSI", getSI_1)
        .def("getSI", getSI_2)///Overrides!!! (Were define above I believe)
        .def("getRI", getRI_1)
        .def("switchONpsu", switchONpsu_1)
        .def("switchONpsu", switchONpsu_2)
        .def("switchOFFpsu", switchOFFpsu_1)
        .def("degauss", degauss_1,(
                        boost::python::arg("magnetname"),boost::python::arg("degaussToZero")=false),"deguass magnetname (single magnet), if degaussToZero = True then after degaussing the current will be left at zero, if degaussToZero = False the initial current will be reset.")
        .def("isAQuad",                  &magnetController::isAQuad    )
        .def("isABSol",                  &magnetController::isABSol    )
        .def("isAHCor",                  &magnetController::isAHCor    )
        .def("isAVCor",                  &magnetController::isAVCor    )*/
        ;

    /// Main class to make
    boost::python::class_<VCcameras,boost::noncopyable>("init")
        .def("virtual_CLARA_Camera_Controller",  &VCcameras::virtual_CLARA_Camera_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_Camera_Controller",  &VCcameras::offline_CLARA_Camera_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_Camera_Controller",  &VCcameras::physical_CLARA_Camera_Controller,
             return_value_policy<reference_existing_object>())

        .def("setQuiet",         &VCcameras::setQuiet   )
        .def("setVerbose",       &VCcameras::setVerbose )
        .def("setMessage",       &VCcameras::setMessage )
        .def("setDebugMessage",  &VCcameras::setDebugMessage )
        ;

}


#endif // VC_CAMERAS_H
