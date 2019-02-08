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
//  Last edit:   29-03-2018
//  FileName:    VCba1Stages.h
//  Description:
//
//
//*/
#ifndef _VC_PILASER_H_
#define _VC_PILASER_H_
//stl
#include <string>
//
#include "ba1StagesController.h"
#include "expose_ba1Stages.h"

#include "VCbase.h"
//______________________________________________________________________________
class VCba1Stages : public VCbase
{
   public:
        VCba1Stages();
        ~VCba1Stages();

        ba1StagesController& virtual_BA1_Stages_Controller();
        ba1StagesController& physical_BA1_Stages_Controller();
        ba1StagesController& offline_BA1_Stages_Controller();

    protected:
    private:
        /* object names */
        ba1StagesController* virtual_BA1_Stages_Controller_Obj ;
        ba1StagesController* physical_BA1_Stages_Controller_Obj;
        ba1StagesController* offline_BA1_Stages_Controller_Obj;

        const std::string ba1StageConf;

        ba1StagesController& getController(ba1StagesController*& cont,
                                         const std::string & name,
                                         const bool shouldVM,
                                         const bool shouldEPICS,
                                         const std::string& config);

        /*
            map of showmessage showdebugmessage states
            pointers to these bools are passed down the class
            heirarchy
        */
        std::map<const ba1StagesController*, std::pair<bool,bool>> messageStates;
        void updateMessageStates();
};
//______________________________________________________________________________
BOOST_PYTHON_MODULE(VELA_CLARA_BA1_Stages_Control)
{
    /*
        Things that you want to use in python muct be exposed:
        containers
    */
    BOOST_PYTHON_INCLUDE::export_BaseObjects();
    /*
        ba1Stages objects are in expose_ba1Stages.h so they can be used by
        other controllers  (e.g. the pilasercontroller)
    */
    BOOST_PYTHON_INCLUDE::expose_ba1Stages();

    /// The main class that creates all the controller obejcts
    class_<VCba1Stages,bases<VCbase>,noncopyable> ("init","Doc string")
        .def("virtual_BA1_Stages_Controller",  &VCba1Stages::virtual_BA1_Stages_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the virtual BA1 Stages object.")
        .def("physical_BA1_Stages_Controller",  &VCba1Stages::physical_BA1_Stages_Controller,
             return_value_policy<reference_existing_object>(),
            "returns a reference to the physical BA1 Stages object.")
        .def("offline_BA1_Stages_Controller",  &VCba1Stages::offline_BA1_Stages_Controller,
             return_value_policy<reference_existing_object>(),
             "returns a reference to the offline BA1 Stages object.")
        ;

}


#endif // _VC_PILASER_H_
