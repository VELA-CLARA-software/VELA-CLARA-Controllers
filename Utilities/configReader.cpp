#include "configReader.h"
//stl
#include <iostream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <fstream>
//djs
#include "configDefinitions.h"
#include "structs.h"

configReader::configReader(const bool* show_messages_ptr, const  bool * show_debug_messages_ptr)
: configVersion(-1), numObjs(-1), numIlocks(-1), configFile1(""), baseObject(show_messages_ptr, show_debug_messages_ptr)
{}
configReader::configReader(const std::string & configFile_Location1, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr)
: configVersion(-1), numObjs(-1), numIlocks(-1), configFile1(configFile_Location1), baseObject(show_messages_ptr, show_debug_messages_ptr)
{}
configReader::configReader(const std::string & configFile_Location1,
                            const std::string & configFile_Location2, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr)
: configVersion(-1), numObjs(-1), numIlocks(-1), configFile1(configFile_Location1), configFile2(configFile_Location2), baseObject(show_messages_ptr, show_debug_messages_ptr)
{}
configReader::configReader(const std::string & configFile_Location1,
                            const std::string & configFile_Location2,
                            const std::string & configFile_Location3, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr)
: configVersion(-1), numObjs(-1), numIlocks(-1), configFile1(configFile_Location1), configFile2(configFile_Location2), configFile3(configFile_Location3), baseObject(show_messages_ptr, show_debug_messages_ptr)
{}
configReader::configReader(const std::string & configFile_Location1,
                            const std::string & configFile_Location2,
                            const std::string & configFile_Location3,
                            const std::string & configFile_Location4, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr)
: configVersion(-1), numObjs(-1), numIlocks(-1), configFile1(configFile_Location1), configFile2(configFile_Location2), configFile3(configFile_Location3), configFile4(configFile_Location4), baseObject(show_messages_ptr, show_debug_messages_ptr)
{}
configReader::configReader(const std::string & configFile_Location1,
                            const std::string & configFile_Location2,
                            const std::string & configFile_Location3,
                            const std::string & configFile_Location4,
                            const std::string & configFile_Location5, const bool* show_messages_ptr, const  bool * show_debug_messages_ptr)
: configVersion(-1), numObjs(-1), numIlocks(-1), configFile1(configFile_Location1), configFile2(configFile_Location2), configFile3(configFile_Location3), configFile4(configFile_Location4), configFile5(configFile_Location5), baseObject(show_messages_ptr, show_debug_messages_ptr)
{}
//______________________________________________________________________________
configReader::~configReader(){}
//______________________________________________________________________________
void configReader::setConfigFilePath(const std::string & path)
{
    configFile1 = path;
}
//______________________________________________________________________________
std::string  configReader::getAfterEqualsSign(const std::string & str)
{
    std::size_t found = str.find_last_of(UTL::EQUALS_SIGN);
    return str.substr(found + 1, str.size());
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
int configReader::getNum(const std::string & str)
{
    std::string num = getAfterEqualsSign(str);
    return atoi(num.c_str());
}
//______________________________________________________________________________
size_t configReader::getSize(const std::string & str)
{
    return (size_t)getNum(str);
}
//______________________________________________________________________________
double configReader::getNumD(const  std::string & str)
{
    return atof(str.c_str());
}
//______________________________________________________________________________
long configReader::getNumL(const  std::string & str)
{
    return (long)atof(str.c_str());
}
//______________________________________________________________________________
std::string configReader::trimToDelimiter(std::string const & str, const std::string & STOPDELIMITER)
{
    size_t last = str.find_first_of(STOPDELIMITER);
    return str.substr(0, last);
}
//______________________________________________________________________________
std::string & configReader::trimAllWhiteSpace(std::string & str)
{
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
    return str;
}
//______________________________________________________________________________
std::string & configReader::trimAllWhiteSpaceExceptBetweenDoubleQuotes(std::string & str)
{
    bool ignore = false; // whether to ignore the current character or not

    std::string::iterator endVal = str.end(); // a kind of NULL pointer
    std::string::iterator type = endVal; // a  "

    for(std::string::iterator it=str.begin(); it!=str.end();)
    {
        if((*it) == UTL::DOUBLE_QUOTE_C) // we're in a string!
        {
            if(ignore)
            {
                if (type != endVal && (*it) == (*type))
                {
                    // end of the string
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
                it = str.erase(it);
            else
                it++;
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
bool configReader::stringIsSubString(const std::string & stringToCheck, const std::string & stringToLookFor)
{
    return stringToCheck.find(stringToLookFor) != std::string::npos;
}
//______________________________________________________________________________
std::string configReader::getSubString(std::string & str, std::string & STARTDELIMITER, std::string & STOPDELIMITER)
{
    unsigned first = str.find(STARTDELIMITER);
    unsigned last  = str.find(STOPDELIMITER);

    std::string temp = str.substr(first + 1, last - first - 1);

    return   trimWhiteSpaces(temp);
}
//______________________________________________________________________________
std::string configReader::trimWhiteSpaces(const  std::string & str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last  = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}
std::string configReader::trimBetween(std::string & str, std::string & STARTDELIMITER, std::string & STOPDELIMITER)
{
    size_t first = str.find_first_not_of(STARTDELIMITER);
    size_t last  = str.find_last_not_of(STOPDELIMITER);
    return str.substr(first, (last-first+1));
}
//______________________________________________________________________________
std::vector<std::string> configReader::getKeyVal(const  std::string & trimmedLine, const char delim)
{
    std::stringstream ss(trimmedLine);
    std::string item;
    std::vector<std::string> entry;
    while(std::getline(ss, item, delim))
        entry.push_back(item);
//    for(int i = 0; i <entry.size(); ++i)
//        std::cout <<"entry "<<i <<" = " << entry[ i ] <<std::endl;
    return entry;
}
//______________________________________________________________________________
std::vector<double>  configReader::getDoubleVector(const std::string & str)
{
    std::vector<double> ret;
    std::stringstream ss(str);
    std::string s;
    std::stringstream os;
    double temp;

    while(ss)
    {
        if(!getline(ss, s, ','))
            break;
        else
        {
            //ret.push_back(atof(s.c_str()));
            // this way allows you to write numbers in scientific E notation, i.e. 6.3E+3 etc.
            os <<s;
            os>> temp;
            ret.push_back(temp);
            os.clear();
           //std::cout <<ret.back() <<" ";
        }
    }
//    std::cout <<std::endl;
    return ret;
}
//______________________________________________________________________________
unsigned long configReader::getMASK(const  std::string & val)
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
unsigned long configReader::getCOUNT(const std::string & val)
{
    return  std::stoul(val.c_str());
}
//______________________________________________________________________________
chtype configReader::getCHTYPE(const  std::string & val)
{
  unsigned long r = DBR_CLASS_NAME; /// init to something not often used?

    /// ladder it?

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
bool configReader::isNO_CONFIG_FILE(const  std::string & str)
{
    return str == UTL::NO_CONFIG_FILE;
}
//______________________________________________________________________________
bool configReader::isnotNO_CONFIG_FILE(const   std::string & str)
{
    return !isNO_CONFIG_FILE(str);
}
//______________________________________________________________________________
std::vector<int> configReader::getIntVector(const std::string & str)
{
    std::vector<int> ret;
    std::stringstream ss(str);
    std::string s;
    std::stringstream os;
    int temp;

    while(ss)
    {
        if(!getline(ss, s, ','))
            break;
        else
        {
            //ret.push_back(atof(s.c_str()));
            // this way allows you to write numbers in scientific E notation, i.e. 6.3E+3 etc.
            os <<s;
            os>> temp;
            ret.push_back(temp);
            os.clear();
           //std::cout <<ret.back() <<" ";
        }
    }
//    std::cout <<std::endl;
    return ret;
}

















