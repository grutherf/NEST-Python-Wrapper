Installation instructions:

By now you've cloned the repo. You can build in this directory using simply:
	make clean; make


Run settings:
Two header files are needed in the main directory to be included in the NEST code:
	1) detector.hh
	2) analysis.hh

1) The updated code now requires a detector settings file called "detector.hh" in the main directory.
In order to run with the proper settings for LUX, make sure to copy the appropriate settings file to "detector.hh" as follows:

	cp LUX_Detector_Settings/LUXRun3/LUX_Run3.hh detector.hh (for example)
	cp LUX_Detector_Settings/LUXRun4/LUX_Run4_TB1.hh detector.hh (for example)

2) Analysis settings are handled in the "analysis.hh" file, which allows for tuning of the output
quantities, such as:
	- MCtruthE: whether true energy or "reconstructed"/statistically measured energy is expressed
	- usePE: whether pulse areas are given in pe, phd, or spike count.
	- useS2: whether bands are expressed in log(S2/S1) or log(S2)
	- min/maxS1, numBins: controls S1 analysis threshold cut and binning
	- min/maxS2: S2 analysis threshold cut

Then, REMEMBER TO REMAKE after modifications:
	make clean; make


Run instructions:

This program takes 6 (or 7) inputs, with Z position in mm from bottom of detector:
	./testNEST numEvts type_interaction E_min[keV] E_max[keV] field_drift[V/cm] x,y,z[mm] min_dt,max_dt[us] {optional:seed}

For 8B or WIMPs, numEvts is kg-days of exposure:
	./testNEST exposure[kg-days] {WIMP} m[GeV] x-sect[cm^2] field_drift[V/cm] x,y,z[mm] min_dt,max_dt[us] {optional:seed}

NOTES: 
	- If you want to use the default drift field from detector settings, put -1 as the argument for field_drift.
	- If you want to randomly distribute events in space, rather than specify a point source, put -1 as the argument for x,y,z.
	- If you want to override the drift range in detector.hh with run-time specified drift range (say, for Run 4 bins),
		one can use min/max_dt (in us). To use the default drift range, put -1.


libNEST Functionality:
Note that the updated NEST v2 version of libNEST does not yet include an updated Python wrapper.
This is in development and will be available soon, but for now it is merely a C++ implementation.


NOTE: How to calculate g2 from the detector parameters:
	ExtEff = -0.03754*pow(E_liq,2.)+0.52660*E_liq-0.84645 = -0.03754*pow(E_gas/1.848,2.)+0.52660*(E_gas/1.848)-0.84645
	SE = g1_gas*gasGap_mm*0.1*(0.137*E_gas*1000-177*p_bar-45.7)
	g2 = ExtEff*SE
where E_gas, g1_gas, gasGap_mm, and p_bar are provided in the settings file


Tools:
There are some very handy tools now included for generating sims and doing validations.

In the Tools folder, there are now "GenerateRun3Sims.sh" and "GenerateRun4Sims.sh" scripts.
At the top of either script, the number of events, source, and energy range can be specified.
The Run 4 script is especially useful, since it loops over all 16 Run 4 settings files and labels output
files appropriately.

For example, with "CH3T" specified in the GenerateRun4Sims.sh file, running 
	
	bash GenerateRun4Sims.sh

produces

	Run4_CH3T_Output_2018-03-20/LUX_Run4_TB[i]_DT[j].txt

for various (i, j) where "i" is the time bin and "j" is the drift time bin. 
Note that the date is also included in the output folder name.

If the folder already exists, you will get a warning, and will prompted on whether
you want to overwrite the data.

Run 3 and Run 4 ER/NR bands are also included in the folders:
	Comparison_Bands/Run3_Bands
	Comparison_Bands/Run4_Bands

These bands are used by the Python validation scripts which do band comparisons.
Currently, only Run 3 band comparison script exists: BandComparisonRun3.py.
This will overlay and display the hard-coded NEST output files with the comparison bands 
contained in the "Comparison_Bands" directory (for the appropriate Run).


