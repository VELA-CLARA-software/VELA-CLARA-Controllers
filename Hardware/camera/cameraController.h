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

#ifndef CAM_CONTROLLER_H
#define CAM_CONTROLLER_H
// stl
#include <string>
#include <vector>
#include <map>
// djs
#include "cameraInterface.h"
#include "structs.h"
#include "cameraStructs.h"
#include "controller.h"
#include "controller.h"
#include "controller.h"


class cameraController  : public controller
{
    public:
        /// we have overloaded constructors to specify config-file location
        cameraController();
        /// New scheem - we just have 1 constructor, but we have a higher level class that create these objects
        cameraController( const bool show_messages,    const bool show_debug_messagese,
                          const std::string &camConf, const bool startVirtualMachine,
                          const bool shouldStartEPICs, const HWC_ENUM::MACHINE_AREA myMachineArea);

        ~cameraController( );

        // These are pure virtual methods, so need to have some implmentation in derived classes
        double get_CA_PEND_IO_TIMEOUT();
        void   set_CA_PEND_IO_TIMEOUT( double val );
        ///Query Functions
        bool isON ( const std::string & cam );
        bool isOFF( const std::string & cam );
        bool isMonitoring( const std::string & cam );
        bool isNotMonitoring( const std::string & cam );
        std::string cameraName();
        ///Set Functions
        bool setCamera(const std::string & cam);
        //bool setBlackLevel(const double l);
        //bool setFrequency(const double f);
        bool calibrate(); //Takes 10 successive x and y postitions, averages to set centre in which to determine all x and y position from
        bool setXRatio(const double & r);
        bool setYRatio(const double & r);
        ///Command Functions
        bool start();
        bool stop();
        bool collectAndSave (int & numbOfShots, const std::string & directory);
        ///Get Functions
        std::vector< ccameraStructs::camDataType > getRawData();
        std::vector< cameraStructs::camDataType > getBackgroundRawData();
        double getX();
        double getY();
        double getSigmaX();
        double getSigmaY();
        double getSigmaXY();
        /// This pure virtual method MUST be overwritten in the derived controller ( making this an abstract base class)
        std::map< HWC_ENUM::ILOCK_NUMBER, HWC_ENUM::ILOCK_STATE > getILockStates( const std::string & name );
        std::map< HWC_ENUM::ILOCK_NUMBER, std::string >         getILockStatesStr( const std::string & name );

        const cameraStructs::cameraObject &getCamObjConstRef( const std::string & camName  );
    protected:
    private:

    /// The interface to EPICS
        #cameraInterface  localInterface;
        void initialise();
        const bool shouldStartEPICs;
        const HWC_ENUM::MACHINE_AREA myMachineArea;




};

#endif // cameraController_H
