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
//  Last edit:   16-05-2018
//  FileName:    VC_fastCam.h
//  Description:
//
//
//
//
//*/
#ifndef VC_FASTCAM_H
#define VC_FASTCAM_H

#include <VCbase.h>
#include <VCheader.h>
#include <fastCamController.h>


class VC_fastCam : public VCbase
{
    public:
        VC_fastCam();
        ~VC_fastCam();


        fastCamController& virtual_CLARA_FastCam_Controller();
        fastCamController& offline_CLARA_FastCam_Controller();
        fastCamController& physical_CLARA_FastCam_Controller();

        fastCamController& virtual_VELA_FastCam_Controller();
        fastCamController& offline_VELA_FastCam_Controller();
        fastCamController& physical_VELA_FastCam_Controller();

        fastCamController& virtual_CB1_FastCam_Controller();
        fastCamController& offline_CB1_FastCam_Controller();
        fastCamController& physical_CB1_FastCam_Controller();

        fastCamController& getController(HWC_ENUM::MACHINE_MODE mode,
                                         HWC_ENUM::MACHINE_AREA area);


    protected:

    private:

        fastCamController* virtual_VELA_FastCam_Controller_Obj;
        fastCamController* offline_VELA_FastCam_Controller_Obj;
        fastCamController* physical_VELA_FastCam_Controller_Obj;

        fastCamController* offline_CLARA_FastCam_Controller_Obj;
        fastCamController* physical_CLARA_FastCam_Controller_Obj;

        fastCamController* virtual_CLARA_FastCam_Controller_Obj;

        fastCamController* virtual_CB1_FastCam_Controller_Obj;
        fastCamController* offline_CB1_FastCam_Controller_Obj;
        fastCamController* physical_CB1_FastCam_Controller_Obj;

        fastCamController& getController(fastCamController*& cont,
                                        const std::string & name,
                                        const bool shouldVM,
                                        const bool shouldEPICS,
                                        const HWC_ENUM::MACHINE_AREA myMachineArea);

//        magnetController& getMagnetController(HWC_ENUM::MACHINE_MODE mode, HWC_ENUM::MACHINE_AREA area);

        std::map<fastCamController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();

        const std::string VELA_CONFIG,CLARA_CONFIG;
};


using namespace boost::python;
using namespace boost;
BOOST_PYTHON_MODULE(VELA_CLARA_FastCam_Control)
{
    /*  some doc-string options, to try an dreuce th eamount of
        blurb python adds to documentation
    */
    docstring_options local_docstring_options(true, true, false);
    local_docstring_options.disable_cpp_signatures();
    /// Include ALL the enums you want to expose to Python
    BOOST_PYTHON_INCLUDE::export_BaseObjects();


    class_<fastCamController, bases<controller>, noncopyable>
        ("fastCamController","fastCamController Doc String",no_init)

//        .def("getRemainingDegaussSteps", &fastCamController::getRemainingDegaussSteps,
//             (boost::python::arg("magnetname")),"Get remiaing stesp in magnetname degaussing, -1 = degaussing failed for some reason, -2 = magnetname does not exist.")
        ;


    /// Finally the main class that creates all the objects
      class_<VC_fastCam, bases<VCbase>, noncopyable>("init")
        .def("virtual_CLARA_FastCam_Controller",  &VC_fastCam::virtual_CLARA_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CLARA_FastCam_Controller",  &VC_fastCam::offline_CLARA_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CLARA_FastCam_Controller",  &VC_fastCam::physical_CLARA_FastCam_Controller,
             return_value_policy<reference_existing_object>())

        .def("virtual_VELA_FastCam_Controller",  &VC_fastCam::virtual_VELA_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_VELA_FastCam_Controller",  &VC_fastCam::offline_VELA_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_VELA_FastCam_Controller",  &VC_fastCam::physical_VELA_FastCam_Controller,
             return_value_policy<reference_existing_object>())

        .def("virtual_CB1_FastCam_Controller",  &VC_fastCam::virtual_CB1_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        .def("offline_CB1_FastCam_Controller",  &VC_fastCam::offline_CB1_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        .def("physical_CB1_FastCam_Controller",  &VC_fastCam::physical_CB1_FastCam_Controller,
             return_value_policy<reference_existing_object>())
        ;
}



#endif // VC_FASTCAM_H
