/*
 * File: testNEST.hh
 * Author: D.Q. Huang
 * Created on May 28, 2018,12:06 AM
*/
//#ifndef __testNEST_H__
//#define __testNEST_H__ 1

#include <iostream>
#include <string>
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
using namespace std;

class testNEST_for_python_wrapper
{
private:
  int argc;
  char** argv;
  int testNEST(int argc, char** argv);
public:
  void testNEST(boost::python::list command_input);
};

void testNEST_for_python_wrapper::testNEST(boost::python::list command_input){
  int argc;
  char** argv;
  int l = len(command_input);
  argc = l;
  argv = (char**)std::malloc(argc*sizeof(char *));
  for(int i = 0; i < argc; i++){
    std::string str_ = boost::python::extract<std::string>(command_input[i]);
    char* c_str_ = const_cast<char *>(str_.c_str());
    argv[i] = c_str_;
    argc = argc+1;
    testNEST(argc, argv);
  }
}
