import numpy as np
import matplotlib.pyplot as plt

trit_s1c, trit_s2c = np.loadtxt("Run3_CH3T_Output_2018-03-21/LUX_Run3_CH3T.txt", unpack=True, usecols=(7, 11), skiprows=2)
dd_s1c, dd_s2c = np.loadtxt("Run3_DD_Output_2018-03-21/LUX_Run3_DD.txt", unpack=True, usecols=(7, 11), skiprows=2)
trit_band_s1c, trit_band_mean, trit_band_sig = np.loadtxt("Comparison_Bands/Run3_Bands/tritiumBand_noSpike.txt", unpack=True, usecols=(0, 1, 3))
dd_band_s1c, dd_band_mean, dd_band_sig = np.loadtxt("Comparison_Bands/Run3_Bands/ddnrBand_noSpike.txt", unpack=True, usecols=(0, 1, 3), skiprows=1)

fig1 = plt.figure()
plt.grid()
trit_s2c = trit_s2c[trit_s1c>0]
trit_s1c = trit_s1c[trit_s1c>0]
plt.scatter(trit_s1c, np.log10(trit_s2c/trit_s1c), s=1, facecolor='0.5', lw = 0)
plt.plot(trit_band_s1c, trit_band_mean, c='r')
plt.plot(trit_band_s1c, trit_band_mean-trit_band_sig, c='r')
plt.plot(trit_band_s1c, trit_band_mean+trit_band_sig, c='r')
plt.title("Run 3 Tritium Band Comparison to Updated LibNEST")
plt.xlabel("S1c")
plt.ylabel("log10(S2c/S1c)")
plt.show()

fig2 = plt.figure()
plt.grid()
dd_s2c = dd_s2c[dd_s1c>0]
dd_s1c = dd_s1c[dd_s1c>0]
dd_s1c = dd_s1c[dd_s2c>0]
dd_s2c = dd_s2c[dd_s2c>0]
plt.scatter(dd_s1c, np.log10(dd_s2c/dd_s1c),s=1, facecolor='0.5', lw = 0)
plt.plot(dd_band_s1c, dd_band_mean, c='r')
plt.plot(dd_band_s1c, dd_band_mean-dd_band_sig, c='r')
plt.plot(dd_band_s1c, dd_band_mean+dd_band_sig, c='r')
plt.title("Run 3 DD Band Comparison to Updated LibNEST")
plt.xlabel("S1c")
plt.ylabel("log10(S2c/S1c)")
plt.show()
