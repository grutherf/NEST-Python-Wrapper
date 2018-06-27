# Introduction
  The python wrapper was created by D.Q. Huang to call NEST from Python code. 
  The wrapper features two modes: standard and vectorized.
  In the standard mode, NEST behaves normally. In the vectorized mode, arguments are passed in as vectors. 
  This is helpful if, for example, each interaction takes place at a different energy level.
  While the same could be accomplished with the standard version and a for loop, the vectorized form is much faster
  
  These instructions have only been tested on MacOS 10.13.4 with gcc version 4.2 and LZ NEST version 180621
  
# Instructions

### 1: Install boost python
  * There are multiple ways to do this step, but this is one example. 
  
	  Download Anaconda: https://www.anaconda.com/download/#macos
    
	  - Consider creating an environment specifically for this wrapper	
    
	  Install boost: 
    ```conda install -c anaconda boost```

### 2: Download setup.py and place it in the folder with your NEST code
  * If you are not using MacOS, delete ```-mmacosx-version-min=10.7```
    from the extra_compile_args section. 
  
### 3: Modify testNEST.cpp
  * Look at testNEST_HOW_TO_MODIFY.cpp. 
    Copy the code blocks that are sandwiched between arrows into their relative positions in testNEST.cpp. 
    Portions of testNEST.cpp are omitted from testNEST_HOW_TO_MODIFY.cpp. 
    Where this is done, they have been replaced with “………………”
### 4: Create the wrapper module
  1. cd into the directory that contains the NEST code and setup.py
  
  2. Enter ```python setup.py build_ext --inplace``` into your terminal. 
		This will generates many warnings, they can all be ignored.
    
  3. There should now be a file called “nest_py_interface.cpython-36m-darwin.so” in the folder. 
  This is the wrapper module.
### 5: Import and use the module
  1. Make sure your code is in the same directory as the wrapper.
  
  2. Import the module: 
     ```python 
     import nest_py_interface
     ```
  
  3. Choose either the standard or vectorized form.
  
    - For the vectorized form: 
      ```python 
      npw = nest_py_wrapper_mass()
      ```
      
    - For the standard form:
      ```python 
      npw = nest_py_wrapper_std()
      ```
  4. It is advised that you turn off the print statements to save time with ```npw.print_results_or_not(0)```. 
  If you want turn them back on, ```npw.print_results_or_not(1)```
  
  5. Enter your inputs
    - If you’re using the standard version, entering the arguments is the same as it was in the C++ version:
      ```python
      results = npw.testNEST(‘numEvents’, ’interactionType’, ’minEnergy’, ’maxEnergy’, ’field’, ’position’)
      ```
    - If you’re using the vectorized version, first reset the input: ```npw.reset_input()```
      Input your arguments with the various ```npw.input_…(arg)``` methods. 
      You can see all of npw’s methods with ```dir(npw)```. 
      For methods that take in lists, remember that you cannot pass in numpy arrays. 
      If you want to use the default value, simply do not call the corresponding input method.
      -  Lists can either be of length 1 or numEvents. 
         If the length is 1, the value is used for all interactions. 
         If the length is numEvents, value[i] is used for interaction i.

    - In the vectorized case, the only argument testNEST takes in is numEvents:
	    ```python
      results = npw.testNEST(‘numEvents’)
      ```
  6. results stores the following variables: nph, ne, s1_nhits, s1_nphe, s1_area_raw_phe, s1c_area_phe,
  s1_area_raw_phd, s1c_area_phd, s1_raw_spike, s1c_spike, s2_ne_extract, 
  s2_nph, s2_nhits, s2_nphe, s2_area_raw_phe, s2c_area_phe, s2_area_raw_phd, and s2c_area_phd. 
  All of these can be accessed with ```results.variable```
  
  
# Example
```
python setup.py build_ext --inplace
```

```python
import nest_py_interface

npw = nest_py_interface.nest_py_wrapper_mass()
npw.reset_input()
npw.input_type_interaction('NR')
npw.print_result_or_not(0)

npw.input_field_list([310])
npw.input_energy_list(E_input_keV)

result = npw.testNEST(len(E_input_keV))
print(result.ne)

```
# Troubleshooting
  * If additional cpp files are added to NEST, they will most likely need to be added to the ```sources``` list in setup.py
