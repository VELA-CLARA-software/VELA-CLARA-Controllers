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
//  FileName:    configReader.h
//  Description: The base class for all config readers, it knows how to manipulate
//               strings, and has the memebr variables used by derived classes
//               currently, there is no general overview on how we read config files
//               If you're interested, derived configReaders should have a readConfig()
//               function that follows the general pattern
//*/
#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_
// project includes
#include "configDefinitions.h"
#include "baseObject.h"
// stl includes
#include <string>
#include <sstream>
#include <vector>
// epics includes
#include <cadef.h>
//______________________________________________________________________________
class configReader : public baseObject
{
    public:
        /*  There are many overloaded config file constructors to handle multiple
            config files
        */
        //configReader();
        configReader(const std::string& configFile_Location1,
                     const bool& show_messages,
                     const bool& show_debug_messages,
                     const bool usingVM);
        configReader(const bool& show_messages,
                     const bool& show_debug_messages);
        configReader(const std::string& configFile_Location1,
                     const bool& show_messages,
                     const bool& show_debug_messages);
        configReader(const std::string& configFile_Location1,
                     const std::string& configFile_Location2,
                    const bool& show_messages,
                    const bool& show_debug_messages,const bool usingVM);
        configReader(const std::string& configFile_Location1,
                     const std::string& configFile_Location2,
                     const std::string& configFile_Location3,
                     const bool& show_messages,
                     const bool& show_debug_messages,const bool usingVM);
        configReader(const std::string& configFile_Location1,
                     const std::string& configFile_Location2,
                     const std::string& configFile_Location3,
                     const std::string& configFile_Location4,
                     const bool& show_messages,
                     const bool& show_debug_messages,const bool usingVM);
        configReader(const std::string& configFile_Location1,
                     const std::string& configFile_Location2,
                     const std::string& configFile_Location3,
                     const std::string& configFile_Location4,
                     const std::string& configFile_Location5,
                     const bool& show_messages,
                     const bool& show_debug_messages,const bool usingVM);

        void setConfigFilePath(const std::string& path);

    protected:
        /* protected destructor to make sure this class is never instantiated
           the compiler won't let us call delete on any base class pointers
        */
        ~configReader();
        static size_t counter;

        const bool useVM;
        int configVersion, numObjs, numIlocks;

        void getVersion(const std::string& str);
        void getNumObjs(const std::string& str);
        void getNumIlocks (const std::string& str);

        std::string  getAfterEqualsSign(const std::string& str) const;
        std::vector<double> getDoubleVector(const std::string& str) const;
        std::vector<int>    getIntVector(const std::string& str) const;

        std::vector<std::string> getKeyVal(const std::string& trimmedLine,
                                           const char delim=UTL::EQUALS_SIGN_C)const;
        std::string& trimAllWhiteSpace(std::string& source);
        std::string& trimAllWhiteSpaceExceptBetweenDoubleQuotes(std::string& str);
        std::string  trimWhiteSpaces(const std::string& str) const;

        std::string configFile1, configFile2, configFile3, configFile4, configFile5;

        unsigned long getCOUNT(const std::string& val) const;
        unsigned long getMASK (const std::string& val) const;
        chtype getCHTYPE(const  std::string& val) const;

        std::string getSubString(const std::string& str,
                                 const std::string& STARTDELIMITER,
                                 const std::string& STOPDELIMITER) const;
        std::string trimBetween(std::string& str,
                                const std::string& STARTDELIMITER,
                                const std::string& STOPDELIMITER) const;

        std::string trimToDelimiter(const std::string& str,
                                    const std::string& STOPDELIMITER) const;

        bool isNO_CONFIG_FILE(const std::string& str) const;
        bool isnotNO_CONFIG_FILE(const  std::string& str) const;
    private:
        template<typename T>
        std::vector<T> getNumVector(const std::string& str) const
        {
            std::vector<T> ret;
            std::stringstream ss(str);
            std::string s;
            std::stringstream os;
            T temp;
            while(ss)
            {
                if(!getline(ss, s, UTL::COMMA_C))
                    break;
                else
                {
                    os <<s;
                    os>> temp;
                    ret.push_back(temp);
                    os.clear();
                }
            }
            return ret;
        }

};
#endif //CONFIG_READER_H_
