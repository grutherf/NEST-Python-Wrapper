#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TRandom3.h"
#include "NEST.h"

using namespace std;

int main(int argc, char* argv[]){
  
unsigned long int numEvts = atoi(argv[1]);
// get a tritium spectrum
int c=1;  // speed of light
double C=1.*pow(10.,-6.);  //some coefficient
double me = 511./pow(c,2.);  //rest mass of the electron in keV
double Q=18.6;  //endpoint of tritium Beta decay
double mv=0.;  //neutrino mass, close enough 
double step_T=0.001;  //step energy by 0.001 keV
int numSteps = int(Q/step_T + 1);  //determine number of steps
double T[numSteps];
double TTmp = 0.001;
double G[numSteps];
double B[numSteps];
double x[numSteps];
double F_Kat[numSteps];
double KE[numSteps];
double E[numSteps];
double P[numSteps];
double N[numSteps];
int Z = 1 + 1;  //After beta decay, Z=1+1

for (int aa=0; aa<numSteps; aa++) {
  T[aa] = TTmp;
  G[aa] = (T[aa] + me*pow(c,2.))/(me*pow(c,2.));
  B[aa] = sqrt((pow(G[aa],2.)-1)/pow(G[aa],2.));
  x[aa] = (2.*M_PI*Z/137.)/B[aa];  //137 is fine structure constant
  F_Kat[aa] = x[aa]*(1/(1-exp(-x[aa])));  //from Katrin
  KE[aa] = T[aa];
  E[aa] = KE[aa] + me*pow(c,2.);  //total energy
  P[aa] = sqrt(pow(E[aa],2.) - pow(me,2.)*pow(c,4.))/c;  //momentum of electron
  N[aa] = C*F_Kat[aa]*(KE[aa]+me)*sqrt(pow((KE[aa]+me),2.)-pow(me,2.))*pow((Q-KE[aa]),2.);
  TTmp = TTmp + step_T;
}
double halflife = 12.32;  //half life of tritium beta decay in years
double tau = 365.*halflife/0.69;  //time constant in days
int n = 1*pow(10,6);  // Assume 1e6 atoms are decaying
double s = 0;
for (int aa=0; aa<numSteps; aa++) {
  s = s + N[aa];
}
double cnts[numSteps];
for (int aa=0; aa<numSteps; aa++) {
  cnts[aa] = N[aa]*(1./s * n/tau);  //Decays/keV/day/1e6 atoms
  //  cout << T[aa] << " " << cnts[aa] << endl;  // used to verify spectrum
}
double maxCnts = 0;
for (int aa=0; aa<numSteps; aa++) {
  if (cnts[aa] > maxCnts) {
    maxCnts = cnts[aa];
  }
}

double df = 0.;  //initialize to an arbitrary value to use in constructor
double dt = -1.;  //-1 should make it random (this is drift time)
int particleType = 1;
double energy = 3.;  //initialize to an arbitrary value to use in constructor
double liquidDensity = 2.88;

TRandom3 rand;
rand.SetSeed(0);


//make the NEST object
NEST myNEST(particleType, energy, df, liquidDensity, dt);
Detector myDetector;
myDetector.LUXRun04PlanBSettings();
myNEST.SetDetectorParameters(myDetector);

//run through many events
double rx;
double ry;
int binToTheLeft;
for (int aa=0; aa<numEvts; aa++) {
  while(true) {
    rx = rand.Uniform(0.001, Q);
    ry = rand.Uniform(0, maxCnts);
    binToTheLeft = int(floor((rx)/step_T));
    cout << "attempting spectrum draw..." << endl;
    if (ry < cnts[binToTheLeft]) {
      energy = T[binToTheLeft];
      //      cout << "draw succeeded!  (" << energy << ", " << ry << ") " <<  endl;
      break;
    }
  }
  myNEST.SetEnergy(energy);
  myNEST.DetectorResponse();

  //  cout << myNEST.GetEnergy() << " " << myNEST.GetS1c() << " " << myNEST.GetS2c() << endl;
}

}
