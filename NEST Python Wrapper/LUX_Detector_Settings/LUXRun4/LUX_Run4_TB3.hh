
 // Primary Scintillation (S1) parameters

double g1 = 0.1013; //phd per S1 photon in liquid at dtCntr (not phe)
double sPEres = 0.37; //single phe resolution (Gaussian assumed)
double sPEthr = (0.3*1.173)/0.915; //POD threshold in phe
double sPEeff = 1.0; //actual efficiency, can be used in lieu of POD threshold
double noise[2] = {-0.01,0.08}; //baseline noise mean and width in PE (Gaussian)
double P_dphe = 0.173; //chance 1 photon makes 2 phe instead of 1 in Hamamatsu PMT

int coinLevel = 2; //how many PMTs have to fire for an S1 to count
int numPMTs = 118; //For coincidence calculation

//S1 PDE custom fit for function of z
//s1polA + s1polB*z[mm] + s1polC*z^2+... (QE included, for binom dist) e.g.
double FitS1 ( double xPos_mm, double yPos_mm, double zPos_mm ) {

  return 1.1775 - 9.9623e-4*zPos_mm + 7.7049e-7*pow(zPos_mm, 2.0); // unitless, 1.000 at detector center

}

//Drift electric field as function of Z in mm
//For example, use a high-order poly spline
double FitEF ( double xPos_mm, double yPos_mm, double zPos_mm ) { // in V/cm

  return 103.47236631 - 1.82104229385*zPos_mm + 0.0222141312792*pow(zPos_mm, 2.0) - 0.000105722325686*pow(zPos_mm, 3.0) + 2.38267089834e-7*pow(zPos_mm, 4.0) - 1.87687319572e-10*pow(zPos_mm, 5.0);

}

 // Ionization and Secondary Scintillation (S2) parameters

double g1_gas = 0.086; //phd per S2 photon in gas, used to get SE size
double s2Fano = 1.14; //Fano-like fudge factor for SE width
double s2_thr = 0.; //the S2 threshold in phe or PE, *not* phd. Affects NR most
double S2botTotRatio = 0.449; //S2 bottom-to-total ratio, not really used anymore
double E_gas = 7.8; //field in kV/cm between liquid/gas border and anode
double eLife_us = 871.; //the drift electron mean lifetime in micro-seconds

//S2 PDE custom fit for function of r
//s2polA + s2polB*r[mm] + s2polC*r^2+... (QE included, for binom dist) e.g.
double FitS2 ( double xPos_mm, double yPos_mm ) {

  return 1.; // unitless, 1.000 at detector center

}

 // Thermodynamic Properties

double T_Kelvin = 178.; //for liquid drift speed calculation
double p_bar = 1.95; //gas pressure in units of bars, it controls S2 size

 // Data Analysis Parameters and Geometry

double dtCntr = 162.535; //center of detector for S1 corrections, in usec.
double dt_min = 40.; //minimum. Top of detector fiducial volume
double dt_max = 300.; //maximum. Bottom of detector fiducial volume
double radius = 180.;
 
 double TopDrift = 544.2198; //mm not cm or us (but, this *is* where dt=0)
//a z-axis value of 0 means the bottom of the detector (cathode OR bottom PMTs)
//In 2-phase, TopDrift=liquid/gas border. In gas detector it's GATE, not anode!
double anode = 549.2468; //the level of the anode grid-wire plane in mm
//In a gas TPC, this is not TopDrift (top of drift region), but a few mm above it
 double gate = 539.1928; //mm. This is where the E-field changes (higher)
 // in gas detectors, the gate is still the gate, but it's where S2 starts

 // 2-D (X & Y) Position Reconstruction

double PosResExp = 0.015; // exp increase in pos recon res at hi r, 1/mm
double PosResBase = 70.8364; // baseline unc in mm, see NEST.cpp for usage
