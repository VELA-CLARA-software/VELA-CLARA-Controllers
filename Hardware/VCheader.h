#ifndef VCHEADER_H_INCLUDED
#define VCHEADER_H_INCLUDED


/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!
/// !!!IF YOU CHANGE THIS FILE YOU SHOULD PROBABLY RECOMPILE EVERY HARDWARE CONTROLLER!!!


#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/docstring_options.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/manage_new_object.hpp>
using namespace boost::python;

namespace BOOST_PYTHON_INCLUDE
{

    void export_BaseObjects()
    {
        boost::python::type_info info = boost::python::type_id<std::vector<std::string>>();
        const boost::python::converter::registration* reg = boost::python::converter::registry::query(info);
        if (reg == NULL)  {
            class_<std::vector<std::string>>("std_vector_string")
                .def(vector_indexing_suite<std::vector<std::string>>())
                ;
        } else if ((*reg).m_to_python == NULL) {
            class_<std::vector<std::string>>("std_vector_string")
                .def(vector_indexing_suite<std::vector<std::string>>())
                ;
        }

        boost::python::type_info info2 = boost::python::type_id<std::vector<double>>();
        const boost::python::converter::registration* reg2 = boost::python::converter::registry::query(info2);
        if (reg2 == nullptr)  {
            class_<std::vector<double> >("std_vector_double")
                .def(vector_indexing_suite< std::vector<double>>())
                ;
        }
        else if ((*reg2).m_to_python == nullptr)
        {
            class_<std::vector<double> >("std_vector_double")
                .def(vector_indexing_suite< std::vector<double>>())
                ;
        }

        boost::python::type_info info3 = boost::python::type_id<VELA_ENUM::MACHINE_MODE>();
        const boost::python::converter::registration* reg3 = boost::python::converter::registry::query(info3);
        if (reg3 == nullptr)
        {
            enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE","an enum for different machien modes, virtual, physical or offline (no EPICS) ")
            .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE)
            .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL)
            .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
            ;
        }
        else if ((*reg3).m_to_python == nullptr) {
            enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE")
            .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE)
            .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL)
            .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
            ;
        }


        boost::python::type_info info4 = boost::python::type_id<VELA_ENUM::MACHINE_AREA>();
        const boost::python::converter::registration* reg4 = boost::python::converter::registry::query(info4);
        if (reg4 == nullptr)
        {
            enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA","MACHINE_AREA an enum for named areas in the machine")
                .value("VELA_INJ",     VELA_ENUM::MACHINE_AREA::VELA_INJ)
                .value("VELA_BA1",     VELA_ENUM::MACHINE_AREA::VELA_BA1)
                .value("VELA_BA2",     VELA_ENUM::MACHINE_AREA::VELA_BA2)
                .value("CLARA_INJ",    VELA_ENUM::MACHINE_AREA::CLARA_INJ)
                .value("CLARA_PH1",    VELA_ENUM::MACHINE_AREA::CLARA_PH1)
                .value("CLARA_2_VELA", VELA_ENUM::MACHINE_AREA::CLARA_2_VELA)
                .value("CLARA_S01",    VELA_ENUM::MACHINE_AREA::CLARA_S01)
                .value("CLARA_S02",    VELA_ENUM::MACHINE_AREA::CLARA_S02)
                .value("CLARA_L01",    VELA_ENUM::MACHINE_AREA::CLARA_L01)
                .value("USER",         VELA_ENUM::MACHINE_AREA::USER)
                .value("UNKNOWN_AREA", VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA)
                ;
        }
        else if ((*reg4).m_to_python == nullptr)
        {
            enum_<VELA_ENUM::MACHINE_AREA>("MACHINE_AREA","MACHINE_AREA Doc String")
                .value("VELA_INJ",     VELA_ENUM::MACHINE_AREA::VELA_INJ)
                .value("VELA_BA1",     VELA_ENUM::MACHINE_AREA::VELA_BA1)
                .value("VELA_BA2",     VELA_ENUM::MACHINE_AREA::VELA_BA2)
                .value("CLARA_INJ",    VELA_ENUM::MACHINE_AREA::CLARA_INJ)
                .value("CLARA_PH1",    VELA_ENUM::MACHINE_AREA::CLARA_PH1)
                .value("CLARA_2_VELA", VELA_ENUM::MACHINE_AREA::CLARA_2_VELA)
                .value("CLARA_S01",    VELA_ENUM::MACHINE_AREA::CLARA_S01)
                .value("CLARA_S02",    VELA_ENUM::MACHINE_AREA::CLARA_S02)
                .value("CLARA_L01",    VELA_ENUM::MACHINE_AREA::CLARA_L01)
                .value("USER",         VELA_ENUM::MACHINE_AREA::USER)
                .value("UNKNOWN_AREA", VELA_ENUM::MACHINE_AREA::UNKNOWN_AREA)
                ;
        }

        boost::python::type_info info5 = boost::python::type_id<VELA_ENUM::ILOCK_STATE>();
        const boost::python::converter::registration* reg5 = boost::python::converter::registry::query(info5);
        if (reg5 == nullptr)
        {
        enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD)
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD)
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR)
            ;
        }
        else if ((*reg5).m_to_python == nullptr)
        {
            enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE")
                .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD)
                .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD)
                .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR)
                ;
        }

        boost::python::type_info info6 = boost::python::type_id<VELA_ENUM::ILOCK_STATE>();
        const boost::python::converter::registration* reg6 = boost::python::converter::registry::query(info6);
        if (reg6 == nullptr)
        {
        enum_<VELA_ENUM::STATE>("STATE")
            .value("BAD",   VELA_ENUM::STATE::BAD)
            .value("GOOD",  VELA_ENUM::STATE::GOOD)
            .value("ERR", VELA_ENUM::STATE::ERR)
            .value("UNKNOWN", VELA_ENUM::STATE::UNKNOWN)
            ;
        }
        else if ((*reg6).m_to_python == nullptr)
        {
            enum_<VELA_ENUM::STATE>("STATE")
                .value("BAD",   VELA_ENUM::STATE::BAD)
                .value("GOOD",  VELA_ENUM::STATE::GOOD)
                .value("ERR", VELA_ENUM::STATE::ERR)
                .value("UNKNOWN", VELA_ENUM::STATE::UNKNOWN)
                ;
        }
            // Expose base classes
        class_<baseObject, boost::noncopyable>("baseObject", no_init)
            ;
        // we have to tell boost.python about pure virtual methods in abstract base classes
        class_<controller,bases<baseObject>,boost::noncopyable>
            ("controller", no_init) /// force Python to not construct (init) this object
            .def("get_CA_PEND_IO_TIMEOUT", pure_virtual(&controller::get_CA_PEND_IO_TIMEOUT))
            .def("set_CA_PEND_IO_TIMEOUT", pure_virtual(&controller::set_CA_PEND_IO_TIMEOUT))
            .def("getILockStatesStr",      pure_virtual(&controller::getILockStatesStr)   )
            .def("getILockStates",         pure_virtual(&controller::getILockStates)      )
            ;
    }

}
#endif // VCHEADER_H_INCLUDED
