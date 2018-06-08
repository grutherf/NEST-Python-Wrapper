
/* The defaults are the WIMP search values.

   If you want CH3T (December 2013, NOT August) then
   g1 = 0.116 (best fit, uncertainty is 0.115 +/- 0.005 from tritium paper)
   E_gas = 6.14 kV/cm (makes ee = 0.509 as per tritium spectral fit)
   g1_gas = 0.0927 (makes SE ~ 23.77 phd/e-)

   If you want DD gun then
   g1 = 0.115 (best fit, uncertainty is 0.115 +/- 0.004 from D-D paper)
   E_gas = 6.11 kV/cm (makes ee = 0.484 as per paper)
   g1_gas = 0.0935 (makes SE = 23.77 phe/e-)
   Also, DD is non-fiducial (80-130 us) and e- lifetime is exactly 650 us

   Second-order effect ignored: different SE widths

 */

 // Primary Scintillation (S1) parameters

double g1 = 0.1167; //phd per S1 phot at dtCntr (not phe). Divide out 2-PE effect
double sPEres = 0.37; //single phe resolution (Gaussian assumed)
double sPEthr = (0.3*1.173)/0.915; //POD threshold in phe, usually used IN PLACE of sPEeff
double sPEeff = 1.0; //actual efficiency, can be used in lieu of POD threshold
double noise[2] = {-0.01,0.08}; //baseline noise mean and width in PE (Gaussian)
double P_dphe = 0.173; //chance 1 photon makes 2 phe instead of 1 in Hamamatsu PMT

int coinLevel = 2; //how many PMTs have to fire for an S1 to count
int numPMTs = 119; //For coincidence calculation

//S1 PDE custom fit for function of z
//s1polA + s1polB*z[mm] + s1polC*z^2+... (QE included, for binom dist) e.g.
double FitS1 ( double xPos_mm, double yPos_mm, double zPos_mm ) {
  
  double radius = sqrt(pow(xPos_mm,2.)+pow(yPos_mm,2.));
  double amplitude = 307.9-0.3071*zPos_mm+0.0002257*pow(zPos_mm,2.);
  double shape = 1.1525e-7*sqrt(fabs(zPos_mm-318.84));
  //3.7393e-11*pow(zPos_mm-316.42,2.);
  return -shape * pow ( radius, 3. ) + amplitude;
  
}

//Drift electric field as function of Z in mm
//For example, use a high-order poly spline
double FitEF ( double xPos_mm, double yPos_mm, double zPos_mm ) { // in V/cm

  return 158.92
    -0.2209000 *pow(zPos_mm,1.)
    +0.0024485 *pow(zPos_mm,2.)
    -8.7098e-6 *pow(zPos_mm,3.)
    +1.5049e-8 *pow(zPos_mm,4.)
    -1.0110e-11*pow(zPos_mm,5.);
  
}

// Ionization and Secondary Scintillation (S2) parameters

double g1_gas = 0.0965; //phd per S2 photon in gas, used to get SE size
double s2Fano = 3.7; //Fano-like fudge factor for SE width
double s2_thr = (150.*1.173)/0.915; //the S2 threshold in phe or PE, *not* phd. Affects NR most
double S2botTotRatio = 0.449; //S2 bottom-to-total ratio, not really used anymore
double E_gas = 6.14; //field in kV/cm between liquid/gas border and anode
double eLife_us = 800.; //the drift electron mean lifetime in micro-seconds

//S2 PDE custom fit for function of r
//s2polA + s2polB*r[mm] + s2polC*r^2+... (QE included, for binom dist) e.g.
double FitS2 ( double xPos_mm, double yPos_mm ) {
  
  double radius = sqrt(pow(xPos_mm,2.)+pow(yPos_mm,2.));
  
  return // unitless, 1.000 at detector center
    9156.3
    +6.22750*pow(radius,1.)
    +0.38126*pow(radius,2.)
    -0.017144*pow(radius,3.)+
    0.0002474*pow(radius,4.)-
    1.6953e-6*pow(radius,5.)+
    5.6513e-9*pow(radius,6.)
 -7.3989e-12*pow(radius,7.);
  
}

 // Thermodynamic Properties

double T_Kelvin = 173.; //for liquid drift speed calculation
double p_bar = 1.57; //gas pressure in units of bars, it controls S2 size

 // Data Analysis Parameters and Geometry

double dtCntr = 160.; //center of detector for S1 corrections, in usec.
double dt_min = 38.; //minimum. Top of detector fiducial volume
double dt_max = 305.; //maximum. Bottom of detector fiducial volume
double radius = 200.; //mm (fid.)

 double TopDrift = 544.2; //mm not cm or us (but, this *is* where dt=0)
//a z-axis value of 0 means the bottom of the detector (cathode OR bottom PMTs)
//In 2-phase, TopDrift=liquid/gas border. In gas detector it's GATE, not anode!
double anode = 549.2; //the level of the anode grid-wire plane in mm
//In a gas TPC, this is not TopDrift (top of drift region), but a few mm above it
 double gate = 539.2; //mm. This is where the E-field changes (higher)
 // in gas detectors, the gate is still the gate, but it's where S2 starts

 // 2-D (X & Y) Position Reconstruction

double PosResExp = 0.015; // exp increase in pos recon res at hi r, 1/mm
double PosResBase = 70.8364; // baseline unc in mm, see NEST.cpp for usage
