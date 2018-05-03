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
//  FileName:    baseobject.h
//  Description: The base of it all
//
//
//*/
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H
// stl includes
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <time.h>
// boost.python includes
#ifdef BUILD_DLL
#include <deque>
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/slice.hpp>
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/circular_buffer.hpp>
#endif
//______________________________________________________________________________
class baseObject
{
    public:
        /*  Each derived class has access to two bools, that define whether
            to print messages or not,
        */

        //baseObject();
        baseObject(const bool& show_messages,
                   const bool& show_debug_messages);
        baseObject(const bool& show_messages,
                   const bool& show_debug_messages,const std::string& message);
        baseObject& baseObject::operator= ( const baseObject& other ) = delete;
        /*  protected destructor to make sure this class is never instantiated
            the compiler won't let us call delete on any base class pointers */
   protected:
        ~baseObject();

        static size_t basecount;

        /* These are const references set at instantiation.
           They refer to bools held in the controller */
        const bool& SHOW_DEBUG_MESSAGES;
        const bool& SHOW_MESSAGES;

        /* the main mesdsage functions, i.e print to the sreen
           using default template types makes this easy, (c++11 feature !?)
           these functions allow you to pass up to 7 arguments for a message,
           if you would want more just extend etc.
           message can be enabled/disabled with silence, setVerbose, etc
        */
        void debugMessage(const std::stringstream& p) const
        {
            if(SHOW_DEBUG_MESSAGES)
            {
                printMessage(p.str());
            }
        }
        void message(const std::stringstream& p) const
        {
            if(SHOW_MESSAGES)
            {
                printMessage(p.str());
            }
        }

        template<typename T = std::string, typename U = std::string,
                 typename V = std::string, typename W = std::string,
                 typename X = std::string, typename Y = std::string,
                 typename Z = std::string, typename A = std::string,
                 typename B = std::string, typename C = std::string,
                 typename D = std::string, typename E = std::string,
                 typename F = std::string, typename G = std::string,
                 typename H = std::string
                 >
        void debugMessage(const T p1,
                          const U p2  = "", const V p3 = "", const W p4  = "",
                          const X p5  = "", const Y p6 = "", const Z p7  = "",
                          const A p8  = "", const B p9 = "", const C p10 = "",
                          const D p11 = "",const E p12 = "", const F p13 = "",
                          const G p14 = "",const H p15 = ""
                        ) const
        {
            if(SHOW_DEBUG_MESSAGES)
                printMessage(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15);
        }

        template<typename T = std::string,typename U = std::string,
                 typename V = std::string,typename W = std::string,
                 typename X = std::string,typename Y = std::string,
                 typename Z = std::string,typename A = std::string,
                 typename B = std::string,typename C = std::string,
                 typename D = std::string,typename E = std::string,
                 typename F = std::string,typename G = std::string,
                 typename H = std::string
                 >
        void message(const T p1,
                     const U p2  = "", const V p3 = "", const W p4  = "",
                     const X p5  = "", const Y p6 = "", const Z p7  = "",
                     const A p8  = "", const B p9 = "", const C p10 = "",
                     const D p11 = "",const E p12 = "", const F p13 = "",
                     const G p14 = "",const H p15 = ""
                    )const
        {
            if(SHOW_MESSAGES)
                printMessage(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15);
        }

        template<typename T = std::string,typename U = std::string,
                 typename V = std::string,typename W = std::string,
                 typename X = std::string,typename Y = std::string,
                 typename Z = std::string,typename A = std::string,
                 typename B = std::string,typename C = std::string,
                 typename D = std::string,typename E = std::string,
                 typename F = std::string,typename G = std::string,
                 typename H = std::string
                 >
        void printMessage(const T p1,
                          const U p2  = "", const V p3 = "", const W p4  = "",
                          const X p5  = "", const Y p6 = "", const Z p7  = "",
                          const A p8  = "", const B p9 = "", const C p10 = "",
                          const D p11 = "",const E p12 = "", const F p13 = "",
                          const G p14 = "",const H p15 = ""
                         )const
        {
            std::stringstream ss;
            ss<<p1<<p2<<p3<<p4<<p5<<p6<<p7<<p8<<p9<<p10<<p11<<p12<<p13<<p14<<p15;
            std::cout << ss.str() << std::endl;
        }

        template<typename T = int>
        bool areSame(const T a, const T b, const T epsilon = 0) const
        {
            if(a == b)
                return true;
            else
                return std::abs(a - b) <epsilon;
        }

        template<typename T = int>
        bool areNotSame(const  T a, const T b, const T epsilon = 0) const
        {
            return !areSame(a, b, epsilon);
        }

        template<typename T = double>
        double roundToN(const T a, const size_t n) const
        {
            double p = pow(UTL::TEN_DOUBLE, (double)n);
            return std::round(a * p) / p;
        }

        template<typename T = std::string>
        void getSetDifference(const std::vector<T>& a,
                              const std::vector<T>& b,
                              std::vector<T>& c)
        {
            c.clear();
            std::vector<T> aCopy = a;
            std::vector<T> bCopy = b;
            std::sort(aCopy.begin(), aCopy.end());
            std::sort(bCopy.begin(), bCopy.end());
            if(aCopy.size()> bCopy.size())
                c.resize(aCopy.size());
            else
                c.resize(bCopy.size());
            auto it = std::set_difference(aCopy.begin(),
                                          aCopy.end(),
                                          bCopy.begin(),
                                          bCopy.end(),
                                          c.begin());
            c.resize(it - c.begin());
        }

        time_t timeNow() const {return time(nullptr); }

        bool polaritiesMatch(const std::vector<double>& vals) const;

        std::string currentDateTime() const;
        unsigned short getNumUS(const std::string& str) const;
        double         getNumD (const std::string& str) const;
        size_t         getSize (const std::string& str) const;
        int            getNum  (const std::string& str) const;
        long           getNumL (const std::string& str) const;
        bool  stringIsSubString(const std::string& stringToCheck,
                                const std::string& stringToLookFor) const;

#ifdef BUILD_DLL
        template<typename T>
        std::vector<T> to_std_vector(const boost::python::object& iterable)
        {
            return std::vector<T>(boost::python::stl_input_iterator<T>(iterable),
                                  boost::python::stl_input_iterator<T>());
        }

        template<class T,  class U>
        boost::python::dict enumMapToPythonDict(std::map<T, U> m)
        {
            boost::python::dict dictionary;
            for(auto&& it : m)
                dictionary[(int)it.first] = (int)it.second;
            return dictionary;
        }

        template<class T,  class U>
        boost::python::dict toPythonDict(std::map<T, U> m) const
        {
            boost::python::dict dictionary;
            for(auto&&it:m)
            {
                dictionary[it.first] = it.second;
            }
            return dictionary;
        }

        template<class T>
        boost::python::dict enumStringMapToPythonDict(std::map<T,std::string> m) const
        {
            boost::python::dict dictionary;
            for(auto&& it : m)
                dictionary[(int)it.first] = it.second;
            return dictionary;
        }
        template <class T>
        boost::python::list toPythonList(std::vector<std::vector<T>> vector) const
        {
            typename std::vector<std::vector<T>>::iterator iter;
            typename std::vector<T>::iterator iter2;
            boost::python::list list;
            boost::python::list list2;
            for(iter = vector.begin(); iter != vector.end(); ++iter)
            {
                for(iter2 = iter->begin(); iter2 != iter->end(); ++iter2)
                {
                    list2.append(*iter2);
                }
                list.append(list2);
                list2[boost::python::slice()];
            }
            return list;
        }
        template <class T>
        boost::python::list toPythonList(std::vector<T> vector) const
        {
            typename std::vector<T>::iterator iter;
            boost::python::list list;
            for (iter = vector.begin(); iter != vector.end(); ++iter)
            {
                list.append(*iter);
            }
            return list;
        }
        template <class T>
        boost::python::list toPythonList(std::deque<T> deque) const
        {
            typename std::deque<T>::iterator iter;
            boost::python::list list;
            for (iter = deque.begin(); iter != deque.end(); ++iter)
            {
                list.append(*iter);
            }
            return list;
        }
        template <class T>
        boost::python::list toPythonList(boost::circular_buffer<T> circular_buffer) const
        {
            typename boost::circular_buffer<T>::iterator iter;
            boost::python::list list;
            for (iter = circular_buffer.begin(); iter != circular_buffer.end(); ++iter)
            {
                list.append(*iter);
            }
            return list;
        }
#endif
    private:
};
#endif //BASE_OBJECT_H
