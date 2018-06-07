#include <boost/python.hpp>
#include "testNEST.hh"

using namespace boost::python;
BOOST_PYTHON_MODULE(wrapper)
{
  class_<testNEST_for_python_wrapper>("testNEST_for_python_wrapper")
    .def("testNEST", &testNEST_for_python_wrapper::testNEST)
  ;
}
