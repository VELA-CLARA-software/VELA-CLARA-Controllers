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
//  Last edit:   19-03-2018
//  FileName:    main.h
//  Description:
//
//
//*/
#ifndef __MAIN_H__
#define __MAIN_H__
#include "VCheader.h"

using namespace boost::python;
BOOST_PYTHON_MODULE(VELA_CLARA_enums)
{
    docstring_options doc_options(true, false, false);
    doc_options.disable_cpp_signatures();

    // https://wiki.python.org/moin/boost.python/module
    // set the docstring of the current module scope
    scope().attr("__doc__") = "\n*** VELA_CLARA_enums ***\n\n"
    "A .pyd that contains all the enums that are shared\n"
    "amongst many HWC. In Python programmes that use many\n"
    "HWC it can(should) be imported first in every file so\n"
    "that only the vela_clara_enums namespace has the\n"
    "defintions. (making accesing this enums consistant\n"
    "throughout the python programme.\n";
/*
    vela_clara_enums is a .pyd that contains all the
    enums that are shared amongst many HWC. In Python programmes
    that use many HWC it can(should) be imported first
    in every file so that only the vela_clara_enums namespace
    has the defintions. (making accesing this enums consistant
    throughout the python programme.
*/
    BOOST_PYTHON_INCLUDE::export_BaseObjects();
}
#endif // __MAIN_H__
