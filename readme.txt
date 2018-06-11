Comments on Install readme.txt - Gaitskell 180607 

1)Install anaconda: https://www.anaconda.com/download/#macos

Suggest you consider creating new python environment for install. Can call it Python3_NEST

Activate the environment at the command line
EXAMPLE
Gigi:NEST Python Wrapper gaitskel$ source activate Python3_NEST
(Python3_NEST) Gigi:NEST Python Wrapper gaitskel$ which python
/Users/gaitskel/anaconda/envs/Python3_NEST/bin/python


2) Install boost: conda install -c anaconda boost

Or use the GUI in Anaconda to install boost and its dependencies in the new environment


3)Look at setup.py and make sure the library name in the line "libraries=['library_name']" matches the name of your boost python library. So that line will likely need to be "libraries=['boost_python3']"
You can check your boost python name by going to the directory where anaconda is installed and looking for libboost_pythonX.dylib in the lib folder 

Can Probably Skip 3)


4)If you want to use a newer version of NEST than is already modified to accept the wrapper, you’ll have to modify testNEST.cpp. Look at the already modified version for comments about which pieces of code need to be added or changed.

Note on gcc version (stay <= 4.9). Testing to date has been on 4.2

(Python3_NEST) Gigi:NEST Python Wrapper gaitskel$ gcc --version
Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/4.2.1
Apple LLVM version 9.1.0 (clang-902.0.39.2)
Target: x86_64-apple-darwin17.5.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin


5) Use the "python setup.py build_ext --inplace" command to make a python module that is now importable. MAKE SURE ANY PREVIOUS WRAPPERS HAVE BEEN DELETED BEFORE TRYING TO MAKE A NEW ONE.

if you get the error “./NEST.hh:6:10: fatal error: 'random' file not found
#include <random>”, try changing the line in setup.py from "xtra_compile_args[‘_____’]" to  
"extra_compile_args = ['-O2', '-std=c++11', '-stdlib=libc++','-mmacosx-version-min=10.7']"
Note that this likely only works for macs, but by changing '-mmacosx-version-min=10.7', it may be applicable to other operating systems.

Ignore the cheeky gcc “warnings” and “notes” on NEST coding - none are likely serious.

The output will be wrapper.cpython-36m-darwin.so
 
 
6)In python, use "import wrapper" to import the new module

If you get the error “Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: dlopen(/Users/grantrutherford/Desktop/NEST/LibNEST/wrapper.cpython-36m-darwin.so, 2): Symbol not found: __ZTI9VDetector
  Referenced from: /Users/grantrutherford/Desktop/NEST/LibNEST/wrapper.cpython-36m-darwin.so
  Expected in: flat namespace in /Users/grantrutherford/Desktop/NEST/LibNEST/wrapper.cpython-36m-darwin.so”
or something similar, make sure in setup.py, the line “sources=['testNEST.cpp','TestSpectra.cpp','NEST.cpp','VDetector.cpp','RandomGen.cpp']” contains ALL .cpp files, then repeat steps 5 and 6.
	
EXAMPLE
>>> import wrapper
>>> dir(wrapper)
['__doc__', '__file__', '__loader__', '__name__', '__package__', '__spec__', 'double_vector1', 'double_vector2', 'nestOP', 'nest_py_wrapper']


7) create object "nestExec = wrapper.nest_py_wrapper()"


8)Finally, call "results = nestExec.testNest([])" to store the results of NEST

testNest is the wrapper function within .cpp file that is being called  
the first argument is ignored / dummy variable, 
pass all subsequent variables as strings. See NEST readme with each argument in ‘ ’ and commas separating arguments.

To have no reults printed out, call "nestExec.print_result_or_not(0)", before call "results = nestExec.testNest([])"
To set it back, simply call "nestExec.print_result_or_not(1)"

EXAMPLE

>>> nestExec = wrapper.nest_py_wrapper()

>>> results = nestExec.testNEST(['10', 'NR', '10', '10', '180', '-1'])
*** You are currently using the default LUX Run 3 detector settings. ***


g1 = 0.1167 phd per photon	g2 = 12.2107 phd per electron (e-EE = 48.8941%, while SE_mean = 24.9737)	Density = 2.88863 g/mL	central vDrift = 1.50473 mm/us
										Negative numbers are flagging things below threshold!
E [keV]		field [V/cm]	tDrift [us]	X,Y,Z [mm]	Nph	Ne-	S1_raw [PE]	S1_Zcorr	S1c_spike	Ne-Extr	S2_rawArea	S2_Zcorr [phd]
10.000000	180.000000	175.312309	38, 136, 280	94	66	10.568016	8.932196	8.438518	29	857.788116	910.444493
10.000000	180.000000	254.244577	7, 34, 162	67	68	9.818502	7.463758	6.525407	22	673.611404	789.101174
10.000000	180.000000	61.394626	73, -159, 452	63	62	5.577382	5.381535	7.524527	25	684.701924	630.278831
10.000000	180.000000	60.456184	-175, 59, 453	102	60	13.071624	12.702005	13.220133	21	597.100109	548.995611
10.000000	180.000000	289.093603	-162, 97, 109	95	70	14.403919	10.879235	11.235125	23	686.475014	839.975111
10.000000	180.000000	178.826627	-38, -156, 275	73	65	14.056868	11.889488	13.913699	23	665.877852	709.865108
10.000000	180.000000	155.743804	-144, -57, 310	86	70	5.271943	4.540114	6.737992	28	855.057535	885.616440
10.000000	180.000000	253.529051	-98, 25, 163	89	47	14.664390	11.218851	12.423113	19	579.422831	678.157276
10.000000	180.000000	126.855483	-112, 59, 353	59	66	4.241516	3.764724	4.371771	29	841.525625	840.688542
10.000000	180.000000	244.802879	180, 63, 176	67	52	11.524996	9.206390	10.826753	20	554.018852	641.390031
S1 Mean		S1 Res [%]	S2 Mean		S2 Res [%]	Ec [keVnr]	Ec Res[%]	Eff[%>thr]	Ec [keVee]
10.085254	37.123795	876.760330	16.399104	10.000000	0.000000	100.000000	0.000000
If your energy resolution is 0% then you probably still have MC truth energy on.




