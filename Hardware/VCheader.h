#ifndef VCHEADER_H_INCLUDED
#define VCHEADER_H_INCLUDED


#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>


namespace BOOST_PYTHON_INCLUDE
{
    void export_BaseObjects()
    {
        using namespace boost::python;
        docstring_options doc_options(true, false, false);
        doc_options.disable_cpp_signatures();

        /// Things that you want to use in python muct be exposed:
        /// containers
        class_<std::vector<std::string> >("std_vector_string")
                .def( vector_indexing_suite< std::vector< std::string >>() )
                ;
        class_<std::vector<double> >("std_vector_double")
                .def( vector_indexing_suite< std::vector< double>>() )
                ;
        enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD  )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR)
            ;
        enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE","MACHINE_MODE: a named integer giving the")
            .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE )
            .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL )
            .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
            ;
        // Expose base classes
        boost::python::class_<baseObject, boost::noncopyable>("baseObject", boost::python::no_init)
            ;
        // we have to tell boost.python about pure virtual methods in abstract base classes
        boost::python::class_<controller,boost::python::bases<baseObject>,boost::noncopyable>
            ("controller", boost::python::no_init) /// force Python to not construct (init) this object
            .def("get_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT) )
            .def("set_CA_PEND_IO_TIMEOUT", boost::python::pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT) )
            .def("getILockStatesStr",      boost::python::pure_virtual(&controller::getILockStatesStr)      )
            .def("getILockStates",         boost::python::pure_virtual(&controller::getILockStates)         )
            ;
    }
}
#endif // VCHEADER_H_INCLUDED
