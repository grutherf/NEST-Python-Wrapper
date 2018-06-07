1) install anaconda: https://www.anaconda.com/download/#macos

2) install boost: conda install -c anaconda boost

3) look at setup.py
make sure the library name in the line libraries=['library_name'] matches the name of your boost python library. So that line will likely need to be libraries=['boost_python3']
You can check your boost python name by going to the directory where anaconda is installed and looking for libboost_pythonX.dylib in the lib folder 

4) If you want to use a newer version of NEST than is already modified to accept the wrapper, you’ll have to modify testNEST.cpp. Look at the already modified version for comments about which pieces of code need to be added or changed.
5) use the "python setup.py build_ext --inplace" command to make a python module that is now importable
if you get the error “./NEST.hh:6:10: fatal error: 'random' file not found
#include <random>”, try changing the line in setup.py from extra_compile_args[‘_____’] to  
extra_compile_args = ['-O2', '-std=c++11', '-stdlib=libc++','-mmacosx-version-min=10.7']
Note that this likely only works for macs, but by changing '-mmacosx-version-min=10.7', it may be applicable to other operating systems.

6) in python, use import wrapper  to gain access to the wrapper in python

7) set some variable var = wrapper.nest_py_wrapper()

8) finally, call var 1 = var.testNest([‘dummy’, …]) to store the results of NEST
after the dummy variables, pass in the variables asked for in NEST readme with each argument in ‘ ’ and commas separating arguments.