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
//  FileName:    baseObject.cpp
//  Description:
//
//
//
//*/
// project includes
#include "baseObject.h"
#include "configDefinitions.h"
// stl includes
#include <time.h>
//  __  ___  __   __    /  __  ___  __   __
// /  `  |  /  \ |__)  /  |  \  |  /  \ |__)
// \__,  |  \__/ |  \ /   |__/  |  \__/ |  \
//
//______________________________________________________________________________
size_t baseObject::basecount = UTL::ZERO_SIZET;
//______________________________________________________________________________
baseObject::baseObject(const bool& show_messages,
                       const bool& show_debug_messages,
                       const std::string& message):
SHOW_MESSAGES(show_messages),
SHOW_DEBUG_MESSAGES(show_debug_messages)
{
    basecount += UTL::ONE_SIZET;
    debugMessage(message, " baseObject constructor, count = ", basecount);
}
//______________________________________________________________________________
baseObject::baseObject(const bool& show_messages,
                       const bool& show_debug_messages):
SHOW_MESSAGES(show_messages),
SHOW_DEBUG_MESSAGES(show_debug_messages)
{
    basecount += UTL::ONE_SIZET;
    debugMessage("baseObject constructor, count = ", basecount);
}
//______________________________________________________________________________
baseObject::~baseObject(){}
//______________________________________________________________________________
bool baseObject::polaritiesMatch(const std::vector<double>& vals) const
{
    bool ret = true;
    std::vector<bool> signs;
    for(auto&& it: vals)
        if(it != UTL::ZERO_DOUBLE) /// zero can have either polarity
            signs.push_back(std::signbit(it));
    for(auto&& it: signs)
        if(signs[UTL::ZERO_SIZET] != it)
            ret = false;
    return ret;
}
//______________________________________________________________________________
std::string baseObject::currentDateTime() const
{
    time_t     now = timeNow();
    struct tm  tstruct;
    char       buf[UTL::BUFFER_EIGHTY];
    localtime_s(&tstruct, &now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M",&tstruct);
    return buf;
}
//______________________________________________________________________________
int baseObject::getNum(const std::string& str) const
{
    return atoi(str.c_str());
}
//______________________________________________________________________________
size_t baseObject::getSize(const std::string& str) const
{
    return (size_t)getNum(str);
}
//______________________________________________________________________________
double baseObject::getNumD(const  std::string& str) const
{
    return atof(str.c_str());
}
//______________________________________________________________________________
long baseObject::getNumL(const  std::string& str) const
{
    return (long)atof(str.c_str());
}
//______________________________________________________________________________
unsigned short baseObject::getNumUS(const  std::string& str) const
{
    return (unsigned short)atof(str.c_str());
}
//______________________________________________________________________________
bool baseObject::stringIsSubString(const std::string& stringToCheck,
                                   const std::string& stringToLookFor) const
{
    return stringToCheck.find(stringToLookFor) != std::string::npos;
}
