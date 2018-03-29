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
//  FileName:    configReader.cpp
//  Description:
//
//
//*/
#include "configReader.h"
//stl
#include <iostream>
#include <time.h>
#include <algorithm>
#include <fstream>
//djs
#include "configDefinitions.h"
#include "structs.h"
//______________________________________________________________________________
configReader::configReader(const std::string& configFile_Location1,
                           const bool* show_messages,
                           const bool* show_debug_messages,
                           const bool usingVM):
configVersion(UTL::MINUS_ONE_INT),
numObjs(UTL::MINUS_ONE_INT),
numIlocks(UTL::MINUS_ONE_INT),
configFile1(configFile_Location1),
configFile2(UTL::EMPTY_STRING),
configFile3(UTL::EMPTY_STRING),
configFile4(UTL::EMPTY_STRING),
configFile5(UTL::EMPTY_STRING),
useVM(usingVM),
baseObject(show_messages, show_debug_messages)
{}
//______________________________________________________________________________
configReader::configReader(const std::string& configFile_Location1,
                           const std::string& configFile_Location2,
                           const bool* show_messages,
                           const bool* show_debug_messages):
configReader(configFile_Location1, configFile_Location2,
             UTL::EMPTY_STRING, show_messages, show_debug_messages)
{}
//______________________________________________________________________________
configReader::configReader(const std::string& configFile_Location1,
                           const std::string& configFile_Location2,
                           const std::string& configFile_Location3,
                           const bool* show_messages,
                           const bool* show_debug_messages):
configReader(configFile_Location1, configFile_Location2,
             configFile_Location3,
             UTL::EMPTY_STRING, show_messages, show_debug_messages)
{}
//______________________________________________________________________________
configReader::configReader(const std::string & configFile_Location1,
                           const std::string & configFile_Location2,
                           const std::string & configFile_Location3,
                           const std::string & configFile_Location4,
                           const bool* show_messages,
                           const bool* show_debug_messages):
configReader(configFile_Location1, configFile_Location2,
             configFile_Location3, configFile_Location4,
             UTL::EMPTY_STRING, show_messages, show_debug_messages)
{}
//______________________________________________________________________________
configReader::configReader(const std::string & configFile_Location1,
                           const std::string & configFile_Location2,
                           const std::string & configFile_Location3,
                           const std::string & configFile_Location4,
                           const std::string & configFile_Location5,
                           const bool* show_messages,
                           const  bool* show_debug_messages):
configVersion(UTL::MINUS_ONE_INT),
numObjs(UTL::MINUS_ONE_INT),
numIlocks(UTL::MINUS_ONE_INT),
configFile1(configFile_Location1),
configFile2(configFile_Location2),
configFile3(configFile_Location3),
configFile4(configFile_Location4),
configFile5(configFile_Location5),
baseObject(show_messages, show_debug_messages),
useVM(false)
{}
//______________________________________________________________________________
configReader::~configReader(){}
//______________________________________________________________________________
void configReader::setConfigFilePath(const std::string & path)
{
    configFile1 = path;
}
//______________________________________________________________________________
std::string  configReader::getAfterEqualsSign(const std::string & str) const
{
    std::size_t found = str.find_last_of(UTL::EQUALS_SIGN);
    return str.substr(found + UTL::ONE_UINT, str.size());
}
//______________________________________________________________________________
void configReader::getVersion(const std::string & str)
{
    std::string num = getAfterEqualsSign(str);
    configVersion = atoi(num.c_str());
}
//______________________________________________________________________________
void configReader::getNumObjs(const  std::string & str)
{
    std::string num = getAfterEqualsSign(str);
    numObjs = atoi(num.c_str());
}
//______________________________________________________________________________
void configReader::getNumIlocks(const std::string & str)
{
    std::string num = getAfterEqualsSign(str);
    numIlocks = atoi(num.c_str());
}
//______________________________________________________________________________
std::string configReader::trimToDelimiter(std::string const & str,
                                          const std::string & STOPDELIMITER) const
{
    size_t last = str.find_first_of(STOPDELIMITER);
    return str.substr(UTL::ZERO_SIZET, last);
    //return str.substr(str.begin(), last);
}
//______________________________________________________________________________
std::string& configReader::trimAllWhiteSpace(std::string & str)
{
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
    return str;
}
//______________________________________________________________________________
std::string& configReader::trimAllWhiteSpaceExceptBetweenDoubleQuotes(std::string& str)
{   /*
        Does exactly what the function name says.
        Used so that user strings with spaces in can be passed
        from the config files,
        This will have to change if we want to inlcude special characters
        in the config file strings
    */
    // whether to ignore the current character or not
    bool ignore = false;

    // a kind of NULL pointer
    std::string::iterator endVal = str.end();
    // a  "
    std::string::iterator type = endVal;

    for(std::string::iterator it=str.begin(); it!=str.end();)
    {
        if((*it) == UTL::DOUBLE_QUOTE_C) // we're in a string!
        {
            if(ignore)
            {
                if (type != endVal && (*it) == (*type))
                {   // end of the string
                    ignore = false;
                    type = endVal;
                }
            }
            else /// this must be the start of a quoted string
            {
                ignore = true;
                type = it;
            }
            if((*it) == UTL::DOUBLE_QUOTE_C)
            {
                it = str.erase(it);
            }
            else
            {
                it++;
            }
        }
        else
        {
            if (!ignore)
            {
                if((*it) == UTL::SPACE_C ||  (*it) == UTL::TAB_C)
                {
                    it = str.erase(it);
                }
                else
                {
                    it++;
                }
            }
            else
            {
                it++;
            }
        }
    }
    return str;
}
//______________________________________________________________________________
std::string configReader::getSubString(const std::string& str,
                                       const std::string& STARTDELIMITER,
                                       const std::string& STOPDELIMITER) const
{
    unsigned first = str.find(STARTDELIMITER);
    unsigned last  = str.find(STOPDELIMITER);
    std::string temp = str.substr(first+UTL::ONE_UINT,last-first-UTL::ONE_UINT);
    return  trimWhiteSpaces(temp);
}
//______________________________________________________________________________
std::string configReader::trimWhiteSpaces(const std::string& str)  const
{
    size_t first = str.find_first_not_of(UTL::SPACE_C);
    size_t last  = str.find_last_not_of(UTL::SPACE_C);
    return str.substr(first, last - first + UTL::ONE_UINT);
}
//______________________________________________________________________________
std::string configReader::trimBetween(std::string & str,
                                      const std::string & STARTDELIMITER,
                                      const std::string & STOPDELIMITER)const
{
    size_t first = str.find_first_not_of(STARTDELIMITER);
    size_t last  = str.find_last_not_of(STOPDELIMITER);
    return str.substr(first, last - first + UTL::ONE_UINT);
    //return str.substr(first, (last-first+1));
}
//______________________________________________________________________________
std::vector<std::string> configReader::getKeyVal(const std::string & trimmedLine,
                                                 const char delim) const
{
    std::stringstream ss(trimmedLine);
    std::string item;
    std::vector<std::string> entry;
    while(std::getline(ss, item, delim))
        entry.push_back(item);
    return entry;
}
//______________________________________________________________________________
std::vector<double> configReader::getDoubleVector(const std::string& str) const
{
    return getNumVector<double>(str);
}
//______________________________________________________________________________
std::vector<int> configReader::getIntVector(const std::string& str) const
{
    return getNumVector<int>(str);
}
//______________________________________________________________________________
unsigned long configReader::getMASK(const  std::string & val) const
{
    unsigned long r = DBE_VALUE; /// init to something
        /// ladder it?
    if(val == UTL::DBE_VALUE_STR)
        r = DBE_VALUE;
    else if(val == UTL::DBE_LOG_STR)
        r = DBE_LOG;
    else if(val == UTL::DBE_ALARM_STR)
        r = DBE_ALARM;
    return r;
}
//______________________________________________________________________________
unsigned long configReader::getCOUNT(const std::string & val) const
{
    return  std::stoul(val.c_str());
}
//______________________________________________________________________________
chtype configReader::getCHTYPE(const std::string & val) const
{
    // init return value to something not often used?
    unsigned long r = DBR_CLASS_NAME;
    // ladder it?
    if(val == UTL::DBR_STRING_STR)
        r = DBR_STRING;
    else if(val == UTL::DBR_INT_STR)
        r = DBR_INT;
    else if(val == UTL::DBR_SHORT_STR)
        r = DBR_SHORT;
    else if(val == UTL::DBR_FLOAT_STR)
        r = DBR_FLOAT;
    else if(val == UTL::DBR_ENUM_STR)
        r = DBR_ENUM;
    else if(val == UTL::DBR_CHAR_STR)
        r = DBR_CHAR;
    else if(val == UTL::DBR_LONG_STR)
        r = DBR_LONG;
    else if(val == UTL::DBR_DOUBLE_STR)
        r = DBR_DOUBLE;
    else if(val == UTL::DBR_TIME_STRING_STR)
        r = DBR_TIME_STRING;
    else if(val == UTL::DBR_TIME_INT_STR)
        r = DBR_TIME_INT;
    else if(val == UTL::DBR_TIME_SHORT_STR)
        r = DBR_TIME_SHORT;
    else if(val == UTL::DBR_TIME_FLOAT_STR)
        r = DBR_TIME_FLOAT;
    else if(val == UTL::DBR_TIME_ENUM_STR)
        r = DBR_TIME_ENUM;
    else if(val == UTL::DBR_TIME_CHAR_STR)
        r = DBR_TIME_CHAR;
    else if(val == UTL::DBR_TIME_LONG_STR)
        r = DBR_TIME_LONG;
    else if(val == UTL::DBR_TIME_DOUBLE_STR)
        r = DBR_TIME_DOUBLE;
    return r;
}
//______________________________________________________________________________
bool configReader::isNO_CONFIG_FILE(const std::string& str) const
{
    return str == UTL::NO_CONFIG_FILE;
}
//______________________________________________________________________________
bool configReader::isnotNO_CONFIG_FILE(const std::string& str) const
{
    return !isNO_CONFIG_FILE(str);
}

