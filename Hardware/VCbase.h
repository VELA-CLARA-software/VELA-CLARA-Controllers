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
//  FileName:    VCbase.h
//  Description:
//
//
//*/
#ifndef _VCBASE_H_
#define _VCBASE_H_
// stl includes
#include <iostream>
#include <string>
//______________________________________________________________________________
class VCbase
{
    public:
        VCbase();
        VCbase(const std::string& name);

        void setQuiet();
        void setVerbose();
        void setMessage();
        void setDebugMessage();

    protected:
        ~VCbase();

        const std::string my_name;
        const bool withEPICS, withoutEPICS, withoutVM, withVM;
        bool  shouldShowDebugMessage, shouldShowMessage;

    private:

};
//______________________________________________________________________________
#endif // VCBASE_H
