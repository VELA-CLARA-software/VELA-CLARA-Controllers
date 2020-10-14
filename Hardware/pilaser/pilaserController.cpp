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
//  Last edit:   11-09-2018
//  FileName:    pilaserController.cpp
//  Description:
//
//
//*/
// project includes
#include "pilaserController.h"
#include "running_stat.h"
// stl includes
//#include <fstream>
//#include <iostream>
//______________________________________________________________________________
pilaserController::pilaserController(
    bool& show_messages,
    bool& show_debug_messages,
    const bool startVirtualMachine,
    const bool shouldStartEPICs,
    const std::string& name,
    const std::string& pilaserConf,
    const std::string& claraCamConfig,
    const std::string& piLaserMirrorConf,
    const std::string& velaCamConfig,
    const HWC_ENUM::MACHINE_AREA area
    ):
localInterface(show_messages,
               show_debug_messages,
               startVirtualMachine,
               shouldStartEPICs,
               pilaserConf,
               claraCamConfig,
               piLaserMirrorConf,
               velaCamConfig,
               area),
name(name),
is_setting_position(false),
cameraControllerBase(show_messages,show_debug_messages, name, HWC_ENUM::CONTROLLER_TYPE::PI_LASER, &localInterface),
shutterController(show_messages,
                 show_debug_messages,
                 startVirtualMachine,
                 shouldStartEPICs,
                 //UTL::APCLARA1_CONFIG_PATH + UTL::PIL_SHUTTER_CONFIG,
                 UTL::CLARANET_CONFIG_PATH + UTL::PIL_SHUTTER_CONFIG,
                 "physical_PIL_Shutter_Controller")
{
    if(localInterface.interfaceInitReport(shouldStartEPICs))
    {
        camBase = &localInterface;
        cameraControllerBase::message("pilaserController instantiation success.");
    }
}
//______________________________________________________________________________
pilaserController::~pilaserController()
{
    cameraControllerBase::debugMessage("pilaserController DESTRUCTOR");
    if(set_pos_struct.thread)
    {
        cameraControllerBase::message("Joining Thread");
        set_pos_struct.thread->join();

        cameraControllerBase::message("Deleting Thread");

        delete set_pos_struct.thread;

        cameraControllerBase::message("Set  Thread to nullptr");

        set_pos_struct.thread = nullptr;
    }
    cameraControllerBase::debugMessage("pilaserController DESTRUCTOR Complete");
}    //dtor
//______________________________________________________________________________
void  pilaserController::set_CA_PEND_IO_TIMEOUT(double val)
{
    localInterface.set_CA_PEND_IO_TIMEOUT(val);
}
//______________________________________________________________________________
double pilaserController::get_CA_PEND_IO_TIMEOUT()const
{
    return localInterface.get_CA_PEND_IO_TIMEOUT();
}
//______________________________________________________________________________
int  pilaserController::setHWP(const double value)
{
    return localInterface.setHWP(value);
}
//______________________________________________________________________________
double pilaserController::getHWP()
{
    return localInterface.getHWP();
}
//______________________________________________________________________________
bool pilaserController::setCharge(const double value)
{
    return localInterface.setCharge(value);
}
//______________________________________________________________________________
bool pilaserController::setEnergy(const double value)
{
    return localInterface.setEnergy(value);
}
//______________________________________________________________________________
double pilaserController::getEnergy() const
{
    return localInterface.getEnergy();
}
//______________________________________________________________________________
std::string pilaserController::getName() const
{
    return localInterface.getName();
}
//______________________________________________________________________________
HWC_ENUM::STATE pilaserController::getStatus() const
{
    return localInterface.getStatus();
}
//______________________________________________________________________________
double pilaserController::getStabilisationStatus() const
{
    return localInterface.getStabilisationStatus();
}
//______________________________________________________________________________
bool pilaserController::isOn()
{
    return localInterface.isOn();
}
//______________________________________________________________________________
bool pilaserController::isOff()
{
    return localInterface.isOff();
}
//______________________________________________________________________________
bool pilaserController::isStabilisationOff() const
{
    return localInterface.isStabilisationOff();
}
//______________________________________________________________________________
bool pilaserController::isStabilisationOn() const
{
    return localInterface.isStabilisationOn();
}
//______________________________________________________________________________
bool pilaserController::disableStabilisation()
{
    return localInterface.disableStabilisation();
}
//______________________________________________________________________________
bool pilaserController::enableStabilisation()
{
    return localInterface.enableStabilisation();
}
//______________________________________________________________________________
double pilaserController::getQ()const
{
    return localInterface.getQ();
}
//______________________________________________________________________________
std::vector<double> pilaserController::getQBuffer()const
{
    return localInterface.getQBuffer();
}
//______________________________________________________________________________
std::vector<double> pilaserController::getEBuffer()const
{
    return localInterface.getEBuffer();
}
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list pilaserController::getQBuffer_Py()const
{
    return cameraControllerBase::toPythonList(getQBuffer());
}
//______________________________________________________________________________
boost::python::list pilaserController::getEBuffer_Py()const
{
    return cameraControllerBase::toPythonList(getEBuffer());
}
#endif // BUILD_DLL
//______________________________________________________________________________
//
//   _____  .__
//  /     \ |__|_____________  ___________
// /  \ /  \|  \_  __ \_  __ \/  _ \_  __ \
///    Y    \  ||  | \/|  | \(  <_> )  | \/
//\____|__  /__||__|   |__|   \____/|__|
//        \/
//
//______________________________________________________________________________
double pilaserController::getHpos() const
{
    return localInterface.getHpos();
}
//______________________________________________________________________________
double pilaserController::getVpos() const
{
    return localInterface.getVpos();
}
//______________________________________________________________________________
bool pilaserController::setHpos(double value)
{
     return localInterface.setHpos(value);
}
//______________________________________________________________________________
bool pilaserController::setVpos(double value)
{
    return localInterface.setVpos(value);
}
//______________________________________________________________________________
double pilaserController::getHstep() const
{
    return localInterface.getHstep();
}
//______________________________________________________________________________
double pilaserController::getVstep() const
{
    return localInterface.getVstep();
}
//______________________________________________________________________________
bool pilaserController::setHstep(double value)
{
    return localInterface.setHstep(value);
}
//______________________________________________________________________________
bool pilaserController::setVstep(double value)
{
    return localInterface.setVstep(value);
}
//______________________________________________________________________________
bool pilaserController::moveLeft(const double value)
{
    if( can_move_test_1() )
    {
        if( cameraControllerBase::isBeam_x_Lo_VC())
        {
            //cameraControllerBase::message("isBeamy_x_Lo_VC = true");
            return false;
        }
        cameraControllerBase::message("pilaserController::moveLeft val = ",value);
        return localInterface.moveLeft(value);
    }
    return false;
}
//______________________________________________________________________________
bool pilaserController::moveRight(const double value)
{
    if( can_move_test_1() )
    {
        if( cameraControllerBase::isBeam_x_Hi_VC())
        {
            //cameraControllerBase::message("isBeamy_x_Hi_VC = true");
            return false;
        }
        cameraControllerBase::message("pilaserController::moveRight val = ",value);
        return localInterface.moveRight(value);
    }
    return false;
}
//______________________________________________________________________________
bool pilaserController::moveUp(const double value)
{
    if( can_move_test_1() )
    {
        if( cameraControllerBase::isBeam_y_Hi_VC())
        {
            //cameraControllerBase::message("isBeam_y_Hi_VC = true");
            return false;
        }
        cameraControllerBase::message("pilaserController::moveUp val = ",value);
        return localInterface.moveUp(value);
    }
    return false;
}
//______________________________________________________________________________
bool pilaserController::moveDown(const double value)
{
    if( can_move_test_1() )
    {
        if( cameraControllerBase::isBeam_y_Lo_VC())
        {
            //cameraControllerBase::message("isBeamy_y_Lo_VC = true");
            return false;
        }
        cameraControllerBase::message("pilaserController::moveDown val = ",value);
        return localInterface.moveDown(value);
    }
    return false;
}
//______________________________________________________________________________
const pilaserStructs::pilMirrorObject& pilaserController::getpilMirrorObjConstRef() const
{
    return localInterface.getpilMirrorObjConstRef();
}
//______________________________________________________________________________
const pilaserStructs::pilaserObject& pilaserController::getPILObjConstRef() const
{
    return localInterface.getPILObjConstRef();
}

//______________________________________________________________________________
bool pilaserController::can_move_test_1()
{
    /*
        simple test 1 to decide if we can move laser mirrors
    */
    // check shutters are open
    if( areAllOpen() )
    //if( true )
    {
        if( isAnalysisUpdating())
        {
            if( hasBeam_VC() )
            {
                return true;
            }
        }
    }
    cameraControllerBase::message("can_move_test_1 failed");
    return false;
}
//---------------------------------------------------------------------------------------------
bool pilaserController::isSettingPos()const
{
    return is_setting_position;
}
//

//---------------------------------------------------------------------------------------------
bool pilaserController::setVCPos(double h, double v)
{
    // 0.06 is about 1 pixel rpecision
    return setVCPos(h,v, 0.06, 0.06, 100.0, 100.0,  10, 10, 100, 120); // MAGIC_NUMBER
}
//---------------------------------------------------------------------------------------------
bool pilaserController::setVCPosPy(double h, double v, double h_prec, double v_prec,
                                 double mirror_step_x_0, double mirror_step_y_0,
                                 size_t num_points_x, size_t num_points_y,
                                 size_t max_it, size_t time_out)
{
    // 0.06 is about 1 pixel rpecision
    return setVCPos(h,v, h_prec, v_prec, mirror_step_x_0, mirror_step_y_0,  num_points_x, num_points_y, max_it, time_out); // MAGIC_NUMBER
}
//---------------------------------------------------------------------------------------------
bool pilaserController::setVCPos(double h, double v, double h_prec, double v_prec,
                                 double mirror_step_x_0, double mirror_step_y_0,
                                 size_t num_points_x, size_t num_points_y,
                                 size_t max_it, size_t time_out)
{
    cameraControllerBase::message("pilaserController::setVCPos");
    using namespace pilaserStructs;
    if(is_setting_position)
    {
        cameraControllerBase::message( "is_setting_position == true");
        return false;
    }

    if(set_pos_struct.thread)
    {
        cameraControllerBase::message("Joining Thread");
        set_pos_struct.thread->join();

        cameraControllerBase::message("Deleting Thread");

        delete set_pos_struct.thread;

        cameraControllerBase::message("Set  Thread to nullptr");

        set_pos_struct.thread = nullptr;
    }
    /*
        set flags
    */
    is_setting_position = true;

    /*
        init struct values
        I'm going to force a conversion from mm to pixels
        as sometimes the camera system looses its calibration
    */


    double mm_2_pix = UTL::ONE_DOUBLE / getPix2mmDef_VC();

    set_pos_struct.x_pos = h * mm_2_pix;
    set_pos_struct.y_pos = v * mm_2_pix;
    set_pos_struct.x_precision    = h_prec * mm_2_pix;
    set_pos_struct.y_precision    = v_prec * mm_2_pix;
    set_pos_struct.mirror_step_x = mirror_step_x_0;
    set_pos_struct.mirror_step_y = mirror_step_y_0;
    set_pos_struct.num_points_x   = num_points_x;
    set_pos_struct.num_points_y   = num_points_y;

    set_pos_struct.max_iterations = max_it;
    set_pos_struct.time_out = time_out;
    set_pos_struct.time_start = cameraControllerBase::timeNow();
    set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::STARTUP;

    // only start if essential conditions are met
    if(VCPositionEssentialRequirements_areBad())
    {
        cameraControllerBase::message("Exiting setPosition because: ", ENUM_TO_STRING(set_pos_struct.state) );
        is_setting_position = false;
    }
    else
    {

        cameraControllerBase::message("VCPositionEssentialRequirements ARE GOOD PROCEEDING TO setPosition function.");
        cameraControllerBase::message("Using pixels instead of mm:\nhorizontal ",
                                        h," (mm) = ", set_pos_struct.x_pos, "(pix)/nvertical ",
                                        v," (mm) = ", set_pos_struct.y_pos, "(pix)");
        set_pos_struct.thread = new std::thread(staticEntrySetPosition, this);
    }
    return true;
}
//---------------------------------------------------------------------------------------------
void pilaserController::staticEntrySetPosition(pilaserController* controller)
{
    controller->setPosition();
}
//---------------------------------------------------------------------------------------------
void pilaserController::setPosition()
{
    runningStat& x_rs = get_x_private_rs_ref_VC();
    runningStat& y_rs = get_y_private_rs_ref_VC();

    x_rs.setMaxCount(set_pos_struct.num_points_x);
    y_rs.setMaxCount(set_pos_struct.num_points_y);

    cameraControllerBase::message("x_rs.getMaxCount = ", x_rs.getMaxCount() );
    cameraControllerBase::message("y_rs.getMaxCount = ", y_rs.getMaxCount() );

    x_rs.Clear();
    y_rs.Clear();


    bool move_horizontal = true;
    bool move_vertical   = true;

    bool move_left = false;
    bool move_right = false;
    bool move_up = false;
    bool move_down = false;

    bool old_move_left = false;
    bool old_move_right= false;
    bool old_move_up= false;
    bool old_move_down= false;


    bool vertical_change_direction = false;
    bool horizontal_change_direction = false;

    set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::RUNNING;
    for(auto&& i = 0; i < set_pos_struct.max_iterations; ++i )//MAGIC_STRING
    {
        cameraControllerBase::message("pilaserController::setPosition(), iteration = ", i + 1);

        /*
            clear running stats
        */
        x_rs.Clear();
        y_rs.Clear();
        /*
            wait for running stats
        */
        while( x_rs.NotFull() && y_rs.NotFull() )
        {
            // if the running stats are not updatying, VCPositionEssentialRequirements_areBad checks
            if(VCPositionEssentialRequirements_areBad())
            {
                break;
            }
        }
        cameraControllerBase::message("new mean x / y :  ",x_rs.Mean()," / ",y_rs.Mean());
        if(VCPositionEssentialRequirements_areBad())
        {
            cameraControllerBase::message("Exiting setPosition because: ", ENUM_TO_STRING(set_pos_struct.state) );
            break;
        }
        /*
            decide which way to move the beam by comparing
            running_stat to requested position
        */
        if( cameraControllerBase::areNotSame( x_rs.Mean(), set_pos_struct.x_pos, set_pos_struct.x_precision ) )
        {

            if( x_rs.Mean() < set_pos_struct.x_pos  )
            {
                cameraControllerBase::message(x_rs.Mean(), " < ", set_pos_struct.x_pos, "(", set_pos_struct.x_precision, ") let's move right");
                move_left = false;
                move_right = true;
            }
            else
            {
                cameraControllerBase::message(x_rs.Mean(), " > ", set_pos_struct.x_pos, "(", set_pos_struct.x_precision, "), let's move left");
                move_right = false;
                move_left = true;
            }
        }
//        else
//        {
//            cameraControllerBase::message("Reached Horizontal position ");
//            move_horizontal = false;
//        }
        if( cameraControllerBase::areNotSame( y_rs.Mean(), set_pos_struct.y_pos, set_pos_struct.y_precision ) )
        {
            if( y_rs.Mean() < set_pos_struct.y_pos  )
            {
                cameraControllerBase::message(y_rs.Mean(), " < ", set_pos_struct.y_pos, "(", set_pos_struct.y_precision, ") let's move Up");
                move_up = true;
                move_down = false;
            }
            else
            {
                cameraControllerBase::message(y_rs.Mean(), " > ", set_pos_struct.y_pos, "(", set_pos_struct.y_precision, ") let's move Down");
                move_up = false;
                move_down = true;
            }
        }

        if( move_up  )
        {
            if(moveUp( set_pos_struct.mirror_step_y ))
            {}
            else
            {
                cameraControllerBase::message("Failed to send moveUp");
            }
        }
        if( move_down  )
        {
            if(moveDown( set_pos_struct.mirror_step_y ))
            {}
            else
            {
                cameraControllerBase::message("Failed to send moveDown");
            }
        }
        /*
            wait because the motor controller can't take two commands in quick succession
        */
        std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!

        if( move_left )
        {
            if(moveLeft( set_pos_struct.mirror_step_x))
            {
            }
            else
            {
                cameraControllerBase::message("Failed to send MoveLeft");
            }
        }
        if( move_right )
        {
            if(moveRight( set_pos_struct.mirror_step_x ))
            {}
            else
            {
                cameraControllerBase::message("Failed to send MoveLeft");
            }

        }


        if(VCPositionEssentialRequirements_areBad())
        {
            cameraControllerBase::message("Exiting setPosition because: ", ENUM_TO_STRING(set_pos_struct.state) );
            break;
        }

        // after the first iteration, check if we have moved past the requested position, if so decrease step size
        if( i > 1)
        {
            if( move_left != old_move_left && move_right != old_move_right )
            {
                set_pos_struct.mirror_step_x = set_pos_struct.mirror_step_x / 2.0;
                cameraControllerBase::message("Changed H step size to ", set_pos_struct.mirror_step_x );
            }
            if( move_up != old_move_up && move_down != old_move_down )
            {
                set_pos_struct.mirror_step_y = set_pos_struct.mirror_step_y / 2.0;
                cameraControllerBase::message("Changed V step size to ", set_pos_struct.mirror_step_y );
            }
        }


        if(cameraControllerBase::areSame( y_rs.Mean(), set_pos_struct.y_pos, set_pos_struct.y_precision ))
        {
            if(cameraControllerBase::areSame( x_rs.Mean(), set_pos_struct.x_pos, set_pos_struct.x_precision ))
            {

                cameraControllerBase::message("Reached Requested Position with desired tolerance Finished!" );
                set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::SUCCESS;

                break;
            }
            else
            {
                cameraControllerBase::message("Y position reached, but NOT X position, moving to next iteration");
            }

        }
        else
        {
            cameraControllerBase::message("Y position not reached, moving to next iteration");
        }

        // save old values
        old_move_right =  move_right;
        old_move_left  =  move_left;
        old_move_up    =  move_up;
        old_move_down  =  move_down;


        /* set reached max_iteratios state */
        if(i == set_pos_struct.max_iterations - 1)
        {
            cameraControllerBase::message("Reached max number of iterations, cancelling setPosition");
            set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::MAX_ITERATIONS;
        }
    }

    if( set_pos_struct.state != pilaserStructs::VC_SET_POS_STATE::SUCCESS )
    {
        set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::FAIL;
    }
    is_setting_position = false;
    cameraControllerBase::message("Set Position FInished");
}
//---------------------------------------------------------------------------------------------
pilaserStructs::VC_SET_POS_STATE pilaserController::getSetVCPosState()const
{
    return set_pos_struct.state;
}
//---------------------------------------------------------------------------------------------
bool pilaserController::VCPositionEssentialRequirements_areBad()
{
    if( !areAllOpen())
    //if( true )
    {
        cameraControllerBase::message("setPosition !!!SHUTTER_CLOSED!!!");
        set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::SHUTTER_CLOSED;
        return true;
    }
    else if( ! isAnalysisUpdating() )
    {
        cameraControllerBase::message("setPosition !!!CAMERA_ANALYSIS_NOT_WORKING!!!");
        set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::CAMERA_ANALYSIS_NOT_WORKING;
        return true;
    }
    else if( hasNoBeam_VC() )
    {
        cameraControllerBase::message("setPosition !!!SUSPECT NO LASER BEAM ON VIRTUAL CATHODE!!!");
        set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::LASER_NOT_IN_IMAGE;
        return true;
    }
    else if(cameraControllerBase::timeNow() - set_pos_struct.time_start > set_pos_struct.time_out )
    {
        cameraControllerBase::message("setPosition !!!TIIMEOUT!!!");
        set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::TIME_OUT;
        return true;
    }
    else if(isMaskFeedbackOff_VC() )
    {
        cameraControllerBase::message("setPosition !!!MASK FEEDBACK IS OFF!!!");
        set_pos_struct.state = pilaserStructs::VC_SET_POS_STATE::NO_MASK_FEEDBACK;
        return true;
    }
    return false;
}
//______________________________________________________________________________
bool pilaserController::setEnergyRange200uJ()
{
    return localInterface.setEnergyRange200uJ();
}
bool pilaserController::setEnergyRange20uJ()
{
    return localInterface.setEnergyRange20uJ();
}
bool pilaserController::setEnergyRange2uJ()
{
    return localInterface.setEnergyRange2uJ();
}
bool pilaserController::setEnergyRange200nJ()
{
    return localInterface.setEnergyRange200nJ();
}
bool pilaserController::startEM()
{
    return localInterface.startEM();
}
bool pilaserController::stopEM()
{
    return localInterface.stopEM();
}
bool pilaserController::isRunning() const
{
    return localInterface.isRunning();

}
bool pilaserController::isOverRange() const
{
    return localInterface.isOverRange();
}
std::string pilaserController::getEnergyRange() const
{
    return localInterface.getEnergyRange();
}
//______________________________________________________________________________
bool pilaserController::isRSBufferFull()const
{
    return localInterface.isRSBufferFull();
}
//______________________________________________________________________________
size_t pilaserController::getRSBufferSize()const
{
    return localInterface.getRSBufferSize();
}
//______________________________________________________________________________
size_t pilaserController::getCurrentBufferSize()const
{
    return localInterface.getCurrentBufferSize();
}

//______________________________________________________________________________
void pilaserController::setAllRSBufferSize(size_t buffer_size)
{
    localInterface.setAllRSBufferSize(buffer_size);
}
//______________________________________________________________________________
void pilaserController::clearRunningValues()
{
    localInterface.clearRunningValues();
    cameraControllerBase::clearRunningValues();
}
//______________________________________________________________________________
//bool pilaserInterface::setVCPosition(const double xpos, const double ypos)
//{
//    bool proceed = true;
//    if(xpos < 0.0) //MAGIC_NUMBER
//    {
//        proceed = false;
//        message("setVCPosition not proceeding xpos too lo, xpos =  ", xpos);
//    }
//    if(ypos < 0.0) //MAGIC_NUMBER
//    {
//        proceed = false;
//        message("setVCPosition not proceeding xpos too lo, ypos =  ", ypos);
//    }
//    if(xpos > 10.0) //MAGIC_NUMBER
//    {
//        proceed = false;
//        message("setVCPosition not proceeding xpos too hi, xpos =  ", xpos);
//    }
//    if(ypos < 10.0) //MAGIC_NUMBER
//    {
//        proceed = false;
//        message("setVCPosition not proceeding ypos too hi, ypos =  ", ypos);
//    }
//
//    // check time stamps of fast image and analysis and curren time
//
//    if(entryExists(allCamData, UTL::VIRTUAL_CATHODE))
//    {
//        cameraStructs::cameraObject& cam = allCamData.at(UTL::VIRTUAL_CATHODE);
//
//        check_data_timestamps(cam);
//    }
//    else
//    {
//        proceed = false;
//    }
//
//    if(proceed)
//    {
//       message( "proceeding to main set position function" );
//
//    }
//    return false;
//}



