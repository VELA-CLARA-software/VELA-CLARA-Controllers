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
//  FileName:    pilaserController.h
//  Description:
//
//
//*/
#ifndef _PI_LASER_CONTROLLER_H_
#define _PI_LASER_CONTROLLER_H_
// stl includes
#include <string>
#include <vector>
// project includes
#include "pilaserInterface.h"
#include "pilaserInterface.h"
#include "cameraControllerBase.h"
#include "cameraStructs.h"
#include "shutterController.h"
//______________________________________________________________________________
/*
    !!!!WARNING MULTIPLE INHERITANCES!!!!
    https://stackoverflow.com/questions/406081/why-should-i-avoid-multiple-inheritance-in-c
*/
class pilaserController : public shutterController, public cameraControllerBase
{
    public:
        pilaserController(bool& show_messages,
                            bool& show_debug_messages,
                            const bool startVirtualMachine,
                            const bool shouldStartEPICs,
                            const std::string& name,
                            const std::string& pilaserConf,
                            const std::string& claraCamConfig,
                            const std::string& piLaserMirrorConf,
                            const std::string& velaCamConfig,
                            const HWC_ENUM::MACHINE_AREA area
                            );

        pilaserController& pilaserController::operator= ( const pilaserController& other ) = delete;
        ~pilaserController();
      // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val);

        int    setHWP(const double value);
        double getHWP();

        bool setCharge(const double value);
        bool setEnergy(const double value);
        double getEnergy() const;
        std::string getName() const;
        HWC_ENUM::STATE getStatus() const;
        double getStabilisationStatus() const;
        bool isOn() ;
        bool isOff();
        bool isStabilisationOff() const;
        bool isStabilisationOn() const;
        bool disableStabilisation();
        bool enableStabilisation();

        // laser mirror functions
        double getHpos() const;
        double getVpos() const;
        bool setHpos(double value);
        //bool setHpos(int value);
        bool setVpos(double value);
        //bool setVpos(int value);
        double getHstep() const;
        double getVstep() const;
        bool setHstep(double value);
        bool setVstep(double value);

        bool moveLeft(const double value);
        bool moveRight(const double value);
        bool moveUp(const double value);
        bool moveDown(const double value);

        void clearRunningValues();
        double getQ()const;
        std::vector<double> getQBuffer()const;
        std::vector<double> getEBuffer()const;


        pilaserStructs::VC_SET_POS_STATE getSetVCPosState()const;

        bool setVCPos(double h, double v);
        bool setVCPos(double h, double v, double h_prec, double v_prec,
                      double mirror_step_x_0, double mirror_step_y_0,
                      size_t num_points_x, size_t num_points_y,
                      size_t max_it, size_t time_out);
        bool setVCPosPy(double h, double v, double h_prec, double v_prec,
                      double mirror_step_x_0, double mirror_step_y_0,
                      size_t num_points_x, size_t num_points_y,
                      size_t max_it, size_t time_out);


        bool setEnergyRange200uJ();
        bool setEnergyRange20uJ();
        bool setEnergyRange2uJ();
        bool setEnergyRange200nJ();
        bool startEM();
        bool stopEM();
        bool isRunning() const;
        bool isOverRange() const;
        std::string getEnergyRange() const;


        bool isSettingPos()const;

#ifdef BUILD_DLL
        boost::python::list getQBuffer_Py()const;
        boost::python::list getEBuffer_Py()const;
#endif

        const pilaserStructs::pilMirrorObject&  getpilMirrorObjConstRef() const;
        const pilaserStructs::pilaserObject&    getPILObjConstRef() const;
        //const cameraStructs::camera_image_data& getVCDataObjConstRef() const;


        void setAllRSBufferSize(size_t buffer_size);
        bool isRSBufferFull()const;
        size_t getRSBufferSize()const;
        size_t getCurrentBufferSize()const;

    protected:
    private:
        pilaserInterface  localInterface;






        /*
            NOW (FOR THE FIRST TIME) WE HAVE A CONTROLLER WITH MULTIPLE INTERFACES,
            WE'RE GOING TO HAVE TO HAVE 'COMPLEX' FUNCTIONS IN THE CONTROLLER
            HERE WE CHECK IF THE SHUTTERS ARE OPEN AND THE ONLINE IMAGE ANALYSIS IS WORKING
            BEFORE WE LET YOU MOVE THE LASER MIRRORS
        */
        bool can_move_test_1();
        /*
            for setting a position on the VC we also require multiple interfaces
            so the
        */

        bool is_setting_position;
        pilaserStructs::set_vc_position set_pos_struct;

        static void staticEntrySetPosition(pilaserController* controller);
        void setPosition();

        bool VCPositionEssentialRequirements_areBad();

        const std::string name;
//        localMirrorName, localInterfaceName,
//                          localVirtualCathodeName;
};
//______________________________________________________________________________
#endif // _PI_LASER_CONTROLLER_H_
