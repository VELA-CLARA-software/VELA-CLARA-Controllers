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
//  Last edit:   16-11-2018
//  FileName:    llrfStructs.h
//  Description:
//
//
//*/
#include "llrfStructs.h"
#include "liberallrfInterface.h"
//#include <iostream>
//______________________________________________________________________________
#ifdef BUILD_DLL
boost::python::list llrfStructs::liberallrfObject::getTimeVector()
{
    return interface->toPythonList(time_vector);
}
void llrfStructs::liberallrfObject::setTimeVector()
{
}
#endif
