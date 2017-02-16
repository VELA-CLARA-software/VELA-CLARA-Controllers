#include <string>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/docstring_options.hpp>
#include <boost/python.hpp>
struct World
{
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg;
};
int foo1(int i) { return i; }
int foo2(long l) { return static_cast<int>(l); }
int bar1(int i) { return i; }
int bar2(long l) { return static_cast<int>(l); }
namespace {
    void wrap_foos()
    {
        using namespace boost::python;
        def("foo1", foo1, arg("i"), "foo1 doc");
        def("foo2", foo2, arg("l"), "foo2 doc");
    }
    void wrap_bars()
    {
        using namespace boost::python;
        bool show_user_defined = true;
        bool show_signatures = false;
        docstring_options doc_options(show_user_defined, show_signatures);
        def("bar1", bar1, arg("i"), "bar1 doc");
        def("bar2", bar2, arg("l"), "bar2 doc");

        class_<World>("World")
            .def("greet", &World::greet)
            .def("set", &World::set)
        ;

    }
}
BOOST_PYTHON_MODULE(boost_py_doc_demo)
{
    boost::python::docstring_options doc_options(false);
    wrap_foos();
    wrap_bars();
}
