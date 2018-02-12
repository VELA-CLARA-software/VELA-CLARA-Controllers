
#ifndef __MAIN_H__
#define __MAIN_H__

#include <vector>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

class a
{
    public:
    a(){
        for(auto i =0; i < 10;++i){
            v.push_back(i);
        }
    }
    ~a(){}
    std::vector<double> v;
};

using namespace boost::python;
BOOST_PYTHON_MODULE(members_as_pyobjects_test)
{
    class_<std::vector<double> >("std_vector_double")
        .def(vector_indexing_suite< std::vector<double>>())
        ;
   class_<a>("a")
      .def_readonly("v", &a::v);
}

#endif // __MAIN_H__
