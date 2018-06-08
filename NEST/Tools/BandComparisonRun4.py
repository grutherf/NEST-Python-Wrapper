import numpy as np
import matplotlib.pyplot as plt
import glob

trit_bins_s1 = []
trit_bins_s2 = []
dd_bins_s1 = []
dd_bins_s2 = []

for infile in glob.glob("Run4_CH3T_Output_2018-03-21/LUX_Run4_*.txt"):
	trit_s1c, trit_s2c = np.loadtxt(infile, unpack=True, usecols=(7, 11), skiprows=2)
	trit_bins_s1.append(trit_s1c)
	trit_bins_s2.append(trit_s2c)

for infile in glob.glob("Run4_DD_Output_2018-03-21/LUX_Run4_*.txt"):
	dd_s1c, dd_s2c = np.loadtxt(infile, unpack=True, usecols=(7, 11), skiprows=2)
	dd_bins_s1.append(dd_s1c)
	dd_bins_s2.append(dd_s2c)

trit_band_s1c, trit_band_mean, trit_band_sig = np.loadtxt("Comparison_Bands/Run3_Bands/tritiumBand_noSpike.txt", unpack=True, usecols=(0, 1, 3))
dd_band_s1c, dd_band_mean, dd_band_sig = np.loadtxt("Comparison_Bands/Run3_Bands/ddnrBand_noSpike.txt", unpack=True, usecols=(0, 1, 3), skiprows=1)


fig1, axes1 = plt.subplots(4, 4, sharex=True, sharey=True)
fig1.add_subplot(111, frameon=False)
fig1.suptitle("Run 4 Tritium Band Comparison to Updated LibNEST", fontsize=17)
for idx, row in enumerate(axes1):
	for idxn, col in enumerate(row):
		trit_s1c = trit_bins_s1[idxn+idx]
		trit_s2c = trit_bins_s2[idxn+idx]

		trit_s2c = trit_s2c[trit_s1c>0]
		trit_s1c = trit_s1c[trit_s1c>0]
		trit_s1c = trit_s1c[trit_s2c>0]
		trit_s2c = trit_s2c[trit_s2c>0]
		
		col.grid()
		col.scatter(trit_s1c, np.log10(trit_s2c/trit_s1c), s=1, facecolor='0.5', lw = 0)
		col.plot(trit_band_s1c, trit_band_mean, c='r')
		col.plot(trit_band_s1c, trit_band_mean-trit_band_sig, c='r')
		col.plot(trit_band_s1c, trit_band_mean+trit_band_sig, c='r')
		col.set_xlim([-5, 70])
		col.locator_params(nbins=6, axis='x')
		col.locator_params(nbins=5, axis='y')
		#if idx != 0 or idxn != 0:
		#	col.set_xticklabels([])
		#	col.set_yticklabels([])
fig1.subplots_adjust(hspace=0, wspace=0)
plt.tick_params(labelcolor='none', top='off', bottom='off', left='off', right='off')
plt.grid(False)
plt.xlabel("S1c")
plt.ylabel("log10(S2c/S1c)")
plt.show()


fig2, axes2 = plt.subplots(4, 4, sharex=True, sharey=True)
fig2.add_subplot(111, frameon=False)
fig2.suptitle("Run 4 DD Band Comparison to Updated LibNEST", fontsize=17)
for idx, row in enumerate(axes2):
	for idxn, col in enumerate(row):
		dd_s1c = dd_bins_s1[idxn+idx]
		dd_s2c = dd_bins_s2[idxn+idx]

		dd_s2c = dd_s2c[dd_s1c>0]
		dd_s1c = dd_s1c[dd_s1c>0]
		dd_s1c = dd_s1c[dd_s2c>0]
		dd_s2c = dd_s2c[dd_s2c>0]
		
		col.grid()
		col.scatter(dd_s1c, np.log10(dd_s2c/dd_s1c),s=1, facecolor='0.5', lw = 0)
		col.plot(dd_band_s1c, dd_band_mean, c='r')
		col.plot(dd_band_s1c, dd_band_mean-dd_band_sig, c='r')
		col.plot(dd_band_s1c, dd_band_mean+dd_band_sig, c='r')
		col.set_xlim([-5, 70])
		col.locator_params(nbins=6, axis='x')
		col.locator_params(nbins=5, axis='y')
		#if idx != 0 or idxn != 0:
		#	col.set_xticklabels([])
		#	col.set_yticklabels([])
fig2.subplots_adjust(hspace=0, wspace=0)
plt.tick_params(labelcolor='none', top='off', bottom='off', left='off', right='off')
plt.grid(False)
plt.xlabel("S1c")
plt.ylabel("log10(S2c/S1c)")
plt.show()



