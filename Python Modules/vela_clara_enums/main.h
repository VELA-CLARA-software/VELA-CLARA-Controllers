

#include "structs.h"

#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT SomeFunction(const LPCSTR sometext);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__


//#include <boost/python/detail/wrap_python.hpp>
//#define BOOST_PYTHON_STATIC_LIB /// !!! This should come before  #include <boost/python.hpp>
//#define BOOST_LIB_DIAGNOSTIC
//#include <boost/config.hpp>
#include <boost/python.hpp>
//#include <boost/python/class.hpp>
//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python/object/function.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
//#include <boost/python/suite/indexing/map_indexing_suite.hpp>
//#include <boost/python/docstring_options.hpp>

using namespace boost::python;


BOOST_PYTHON_MODULE(VELA_CLARA_enums)
{

    boost::python::type_info info = boost::python::type_id<VELA_ENUM::MACHINE_MODE>();
    const boost::python::converter::registration* reg = boost::python::converter::registry::query(info);
    if (reg == nullptr)  {
        enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE","MACHINE_MODE Doc String")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
        ;
    } else if ((*reg).m_to_python == nullptr) {
        enum_<VELA_ENUM::MACHINE_MODE>("MACHINE_MODE","MACHINE_MODE Doc String")
        .value("OFFLINE",  VELA_ENUM::MACHINE_MODE::OFFLINE )
        .value("VIRTUAL",  VELA_ENUM::MACHINE_MODE::VIRTUAL )
        .value("PHYSICAL", VELA_ENUM::MACHINE_MODE::PHYSICAL)
        ;
    }


    info = boost::python::type_id<VELA_ENUM::MACHINE_AREA>();
    reg = boost::python::converter::registry::query(info);
    if (reg == nullptr)  {
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
    } else if ((*reg).m_to_python == nullptr) {
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

    info = boost::python::type_id<VELA_ENUM::ILOCK_STATE>();
    reg = boost::python::converter::registry::query(info);
    if (reg == nullptr)  {
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE","ILOCK_STATE Doc String")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
            ;
    } else if ((*reg).m_to_python == nullptr) {
    enum_<VELA_ENUM::ILOCK_STATE>("ILOCK_STATE","ILOCK_STATE Doc String")
            .value("ILOCK_BAD",   VELA_ENUM::ILOCK_STATE::ILOCK_BAD   )
            .value("ILOCK_GOOD",  VELA_ENUM::ILOCK_STATE::ILOCK_GOOD  )
            .value("ILOCK_ERROR", VELA_ENUM::ILOCK_STATE::ILOCK_ERROR )
            ;
    }

    info = boost::python::type_id<VELA_ENUM::STATE>();
    reg = boost::python::converter::registry::query(info);
    if (reg == nullptr)  {
    enum_<VELA_ENUM::STATE>
    ("STATE","STATE Doc String")
            .value("BAD",    VELA_ENUM::STATE::BAD  )
            .value("GOOD",   VELA_ENUM::STATE::GOOD )
            .value("ERR",    VELA_ENUM::STATE::ERR)
            .value("UNKNOWN",VELA_ENUM::STATE::UNKNOWN)
            ;
    }
    else if ((*reg).m_to_python == nullptr) {
    enum_<VELA_ENUM::STATE>("STATE","STATE Doc String")
            .value("BAD",    VELA_ENUM::STATE::BAD  )
            .value("GOOD",   VELA_ENUM::STATE::GOOD )
            .value("ERR",    VELA_ENUM::STATE::ERR)
            .value("UNKNOWN",VELA_ENUM::STATE::UNKNOWN)
            ;
    }

}
