#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H
// stl
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iostream>
//
#ifdef BUILD_DLL
#include <deque>
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/circular_buffer.hpp>
#endif


class baseObject
{
    public:

        /// Each derived has access to two bools, that tell it whether to print messagesor not,
        /// We provide NO default constructors...

        baseObject();
        baseObject(const bool* show_messages_ptr,const bool * show_debug_messages_ptr);
//        /// protected destructor to make sure this class is never instantiated
//        ///  the compiler won't let us call delete on any base class pointers


    protected:

        ~baseObject();

        /// These are const pointers set at instantiation. They point to bools held in the controller

        const bool *SHOW_DEBUG_MESSAGES_PTR, *SHOW_MESSAGES_PTR;

        /// using default template types makes this easy, (c++11 feature !?)
        /// these functions allow you to pass up to 7 arguments for a message,
        /// if you would want more just extend etc.

        void debugMessage(const std::stringstream &  p1)
        {
            if(*SHOW_DEBUG_MESSAGES_PTR)
                printMessage(p1.str());
        }
        void message(const std::stringstream &  p1)
        {
            if(*SHOW_MESSAGES_PTR)
                printMessage(p1.str());
        }

        template<typename T = std::string, typename U = std::string,
                 typename V = std::string, typename W = std::string,
                 typename X = std::string, typename Y = std::string,
                 typename Z = std::string, typename A = std::string,
                 typename B = std::string, typename C = std::string,
                 typename D = std::string, typename E = std::string,
                 typename F = std::string,
                 typename G = std::string,
                 typename H = std::string
                 >
        void debugMessage(const T p1,
                          const U p2 = "",
                          const V p3 = "",
                          const W p4 = "",
                          const X p5 = "",
                          const Y p6 = "",
                          const Z p7 = "",
                          const A p8 = "",
                          const B p9 = "",
                          const C p10 = "",
                          const D p11 = "",
                          const E p12 = "",
                          const F p13 = "",
                          const G p14 = "",
                          const H p15 = ""
                          )
        {
            if(*SHOW_DEBUG_MESSAGES_PTR)
                printMessage(p1, p2, p3, p4, p5, p6, p7, p8,p9,p10,p11,p12,p13,p14,p15);
        }

        template<typename T = std::string,
                 typename U = std::string,
                 typename V = std::string,
                 typename W = std::string,
                 typename X = std::string,
                 typename Y = std::string,
                 typename Z = std::string,
                 typename A = std::string,
                 typename B = std::string,
                 typename C = std::string,
                 typename D = std::string,
                 typename E = std::string,
                 typename F = std::string,
                 typename G = std::string,
                 typename H = std::string
                 >
        void message(const T p1,
                     const U p2 = "",
                     const V p3 = "",
                     const W p4 = "",
                     const X p5 = "",
                     const Y p6 = "",
                     const Z p7 = "",
                     const A p8 = "",
                     const B p9 = "",
                     const C p10 = "",
                     const D p11 = "",
                     const E p12 = "",
                     const F p13 = "",
                     const G p14 = "",
                     const H p15 = ""
                     )
        {
            if(*SHOW_MESSAGES_PTR)
                printMessage(p1, p2, p3, p4, p5, p6, p7, p8,p9,p10,p11,p12,p13,p14,p15);
        }

        template<typename T = std::string,
                 typename U = std::string,
                 typename V = std::string,
                 typename W = std::string,
                 typename X = std::string,
                 typename Y = std::string,
                 typename Z = std::string,
                 typename A = std::string,
                 typename B = std::string,
                 typename C = std::string,
                 typename D = std::string,
                 typename E = std::string,
                 typename F = std::string,
                 typename G = std::string,
                 typename H = std::string
                 >
        void printMessage(const T p1,
                          const U p2 = "",
                          const V p3 = "",
                          const W p4 = "",
                          const X p5 = "",
                          const Y p6 = "",
                          const Z p7 = "",
                          const A p8 = "",
                          const B p9 = "",
                          const C p10 = "",
                          const D p11 = "",
                          const E p12 = "",
                          const F p13 = "",
                          const G p14 = "",
                          const H p15 = ""
                          )
        {
            std::stringstream ss;
            //ss <<p1 <<p2 <<p3 <<p4 <<p5 <<p6 <<p7 <<p8 <<"\n";
            //printf(ss.str().c_str());
            ss <<p1 <<p2 <<p3 <<p4 <<p5 <<p6 <<p7 <<p8 << p9 << p10 << p11 << p12 << p13 << p14 << p15;
            std::cout << ss.str() << std::endl;
        }

        template<typename T = int>
        bool areSame(const T a, const T b, const T epsilon = 0)
        {
            if(a == b)
                return true;
            else
                return std::abs(a - b) <epsilon;
        }

        template<typename T = int>
        bool areNotSame(const  T a, const T b, const T epsilon = 0)
        {
            return !areSame(a, b, epsilon);
        }

        bool polaritiesMatch(const std::vector<double> & vals);

        template<typename T = double>
        double roundToN(const T a, const size_t n)
        {
            double p = pow(10, n);  /// MAGIC_NUMBER
            return std::round(a * p) / p;
        }

        template<typename T = std::string>
        void getSetDifference(const std::vector<T> & a, const std::vector<T> & b, std::vector<T> & c)
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
            auto it = std::set_difference(aCopy.begin(), aCopy.end(), bCopy.begin(), bCopy.end(), c.begin());
            c.resize(it - c.begin());
        }
        std::string currentDateTime();


        int    getNum (const std::string & str);
        double getNumD(const std::string & str);
        size_t getSize(const std::string & str);
        long   getNumL(const std::string & str);
        unsigned short getNumUS(const std::string & str);

        bool   stringIsSubString(const std::string & stringToCheck, const std::string & stringToLookFor);

#ifdef BUILD_DLL
        template<typename T>
        std::vector<T> to_std_vector(const boost::python::object & iterable)
        {
            return std::vector<T>(boost::python::stl_input_iterator<T>(iterable),
                                  boost::python::stl_input_iterator<T>());
        }

        template<class T,  class U>
        boost::python::dict enumMapToPythonDict(std::map<T, U> m)
        {
            boost::python::dict dictionary;
            for(auto & it : m)
                dictionary[ (int)it.first] = (int)it.second;
            return dictionary;
        }
        template<class T>
        boost::python::dict enumStringMapToPythonDict(std::map<T,  std::string > m)
        {
            boost::python::dict dictionary;
            for(auto & it : m)
                dictionary[ (int)it.first] = it.second;
            return dictionary;
        }
        template <class T>
        boost::python::list toPythonList(std::vector<T> vector)
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
        boost::python::list toPythonList(std::deque<T> deque)
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
        boost::python::list toPythonList(boost::circular_buffer<T> circular_buffer)
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
