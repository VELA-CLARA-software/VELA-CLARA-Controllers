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
#ifndef pilaser_CONTROLLER_H_
#define pilaser_CONTROLLER_H_
// stl
#include <string>
#include <vector>
#include <map>
// project
#include "pilaserMirrorInterface.h"
#include "controller.h"
//// boost.python
//#ifdef BUILD_DLL
//#include <boost/python.hpp>
//#include <boost/python/detail/wrap_python.hpp>
//#include <boost/python.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python/args.hpp>
//#include <boost/python/class.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/return_value_policy.hpp>
//#include <boost/python/overloads.hpp>
//#endif

class pilaserMirrorController : public controller
{
    public:
        // just have 1 constructor, but we have a higher level class that create these objects
        pilaserMirrorController();
        pilaserMirrorController(bool* show_messages,
                                bool* show_debug_messages,
                                const bool startVirtualMachine,
                                const bool shouldStartEPICs,
                                const std::string& configFile,
                                const std::string& name
                               );
        ~pilaserMirrorController( );

        double get_CA_PEND_IO_TIMEOUT()const;
        void   set_CA_PEND_IO_TIMEOUT( double val );

//        double getHpos();
//        double getVpos();
//
//        bool setVpos(double value);
//        bool setVpos(int value);
//        bool setIntensity(double value);
//        bool setIntensity(int value);
    protected:
    private:
        //void initialise();
        pilaserMirrorInterface localInterface;
};
#endif // VELA_MAG_CONTROLLER_H_
