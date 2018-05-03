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
//  FileName:    VCpilaser.cpp
//  Description:
//
//
//*/
#ifndef _PILASERMIRROR_CONTROLLER_H_
#define _PILASERMIRROR_CONTROLLER_H_
// stl includes
#include <string>
#include <vector>
#include <map>
// project includes
#include "pilaserMirrorInterface.h"
#include "controller.h"
//______________________________________________________________________________
class pilaserMirrorController : public controller
{
    public:
        pilaserMirrorController();
        pilaserMirrorController(bool& show_messages,
                                bool& show_debug_messages,
                                const bool startVirtualMachine,
                                const bool shouldStartEPICs,
                                const std::string& configFile,
                                const std::string& name
                               );
        pilaserMirrorController& pilaserMirrorController::operator= ( const pilaserMirrorController& other ) = delete;
        ~pilaserMirrorController();

        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT(double val );
        double getHpos() const;
        double getVpos() const;
        // setters , not sure of type for these parameters (or if they will exist...)
        bool setHpos(double value);
        bool setHpos(int value);
        bool setVpos(double value);
        bool setVpos(int value);
        // for the stepper movr
        double getHstep() const;
        double getVstep() const;
        bool setHstep(double value);
        bool setVstep(double value);
        // command to actually move the mirror
        bool moveH();
        bool moveV();
        //
        const pilaserMirrorStructs::pilMirrorObject& getpilMirrorObjConstRef() const;
        // used in higher level class
        bool interfaceInitReport();
    protected:
    private:
        pilaserMirrorInterface localInterface;
};
//______________________________________________________________________________
#endif // _PILASERMIRROR_CONTROLLER_H_
