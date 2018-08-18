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
//  Last edit:   18-06-2018
//  FileName:    VCcameras.h
//  Description:
//
//
//*/
#ifndef _VC_CAMERAS_H_
#define _VC_CAMERAS_H_
// stl includes
#include <string>
#include <map>
//
#include "cameraControllerBase.h"
#include "VCbase.h"
#include "VCheader.h"
//______________________________________________________________________________
class VCcameras : public VCbase
{
        /// These VC classes could all be done more elegantly, with less copypasta
        /// however we went for explicit functions as a quick fix
    public:
        VCcameras();
        ~VCcameras();

        cameraControllerBase& virtual_Camera_Controller();
        cameraControllerBase& offline_Camera_Controller();
        cameraControllerBase& physical_Camera_Controller();

        cameraControllerBase& virtual_CLARA_VC_Camera_Controller();
        cameraControllerBase& offline_CLARA_VC_Camera_Controller();
        cameraControllerBase& physical_CLARA_VC_Camera_Controller();

        cameraControllerBase& virtual_VELA_VC_Camera_Controller();
        cameraControllerBase& offline_VELA_VC_Camera_Controller();
        cameraControllerBase& physical_VELA_VC_Camera_Controller();


        cameraControllerBase& virtual_CLARA_Camera_Controller();
        cameraControllerBase& offline_CLARA_Camera_Controller();
        cameraControllerBase& physical_CLARA_Camera_Controller();

        cameraControllerBase& virtual_VELA_Camera_Controller();
        cameraControllerBase& offline_VELA_Camera_Controller();
        cameraControllerBase& physical_VELA_Camera_Controller();

        cameraControllerBase& getController(const HWC_ENUM::MACHINE_MODE mode,
                                            const HWC_ENUM::MACHINE_AREA area);

    protected:

    private:

        cameraControllerBase* virtual_CLARA_Camera_Controller_Obj;
        cameraControllerBase* offline_CLARA_Camera_Controller_Obj;
        cameraControllerBase* physical_CLARA_Camera_Controller_Obj;

        cameraControllerBase* virtual_VELA_Camera_Controller_Obj;
        cameraControllerBase* offline_VELA_Camera_Controller_Obj;
        cameraControllerBase* physical_VELA_Camera_Controller_Obj;

        cameraControllerBase* virtual_CLARA_VC_Camera_Controller_Obj;
        cameraControllerBase* offline_CLARA_VC_Camera_Controller_Obj;
        cameraControllerBase* physical_CLARA_VC_Camera_Controller_Obj;

        cameraControllerBase* virtual_VELA_VC_Camera_Controller_Obj;
        cameraControllerBase* offline_VELA_VC_Camera_Controller_Obj;
        cameraControllerBase* physical_VELA_VC_Camera_Controller_Obj;


        cameraControllerBase& getController(cameraControllerBase*& cont,
                                           const std::string& name,
                                           const bool shouldVM,
                                           const bool shouldEPICS,
                                           const HWC_ENUM::MACHINE_AREA area
                                           );

        const std::string claraCamConfig, velaCamConfig;
        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const cameraControllerBase*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();
};

using namespace boost::python;
BOOST_PYTHON_MODULE( VELA_CLARA_Camera_Control )
{
    docstring_options doc_options(false);
    doc_options.disable_cpp_signatures();
    /*
        Things that you want to use in python must be exposed:
        containers
    */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();


    /// The main class that creates all the controller obejcts
    class_<VCcameras,bases<VCbase>,boost::noncopyable> ("init","VELA/CLARA camera control, including online analysis"
                                                               "and  image saving where appropriate."
                                                               "CLARA objects have the CLARA VIRTUAL_CATHODE"
                                                               "VELA objects have the VELA VIRTUAL_CATHODE"
                                                               "Currenlty NO single object has both CLARA and VELA VIRTUAL CATHODE"
                                                               )
        .def("virtual_Camera_Controller",  &VCcameras::virtual_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual CLARA camera object.")
        .def("offline_Camera_Controller",  &VCcameras::offline_Camera_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the offline CLARA camera object.")
        .def("physical_Camera_Controller",  &VCcameras::physical_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the physical CLARA camera object.")
        .def("virtual_CLARA_Camera_Controller",  &VCcameras::virtual_CLARA_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual CLARA camera object.")
        .def("offline_CLARA_Camera_Controller",  &VCcameras::offline_CLARA_Camera_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the offline CLARA camera object.")
        .def("physical_CLARA_Camera_Controller",  &VCcameras::physical_CLARA_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the physical CLARA camera object.")
        .def("virtual_VELA_Camera_Controller",  &VCcameras::virtual_VELA_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual VELA camera object.")
        .def("offline_VELA_Camera_Controller",  &VCcameras::offline_VELA_Camera_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the offline VELA camera object.")
        .def("physical_VELA_Camera_Controller",  &VCcameras::physical_VELA_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the physical VELA camera object.")
        .def("virtual_CLARA_VC_Camera_Controller",  &VCcameras::virtual_CLARA_VC_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual CLARA Virtual Cathode camera object.")
        .def("offline_CLARA_VC_Camera_Controller",  &VCcameras::offline_CLARA_VC_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline CLARA Virtual Cathode camera object.")
        .def("physical_CLARA_VC_Camera_Controller",  &VCcameras::physical_CLARA_VC_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the Physical CLARA Virtual Cathode camera object.")
        .def("virtual_VELA_VC_Camera_Controller",  &VCcameras::virtual_VELA_VC_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual VELA Virtual Cathode camera object.")
        .def("offline_VELA_VC_Camera_Controller",  &VCcameras::offline_VELA_VC_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline VELA Virtual Cathode camera object.")
        .def("physical_VELA_VC_Camera_Controller",  &VCcameras::physical_VELA_VC_Camera_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the Physical VELA Virtual Cathode camera object.")
        ;
}
#endif // VC_CAMERAS_H
