#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"
#include "NEST.h"
#include "TMath.h"
#include "TFile.h"
#include <iostream>


using namespace std;

int main() {

   ////////////////////////////////////////////////////////////////////////
   // Shaun's Tritium spectrum generator
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
   //////////////////////////////////////////////////////////////////////////////////
   //







    TRandom3 r;
    double energy;
    int Nevents = 100000;
    
    double S1c[1000];
    double S2c[1000];

    TFile * outfile = new TFile("NESTHistograms.root","RECREATE");

    TH1F * h_S1bins[50];
    int S1bin;
    char histname[50];

    for(int i=0; i<50; i++) { 
        sprintf(histname,"h_S1bin_%d",i);
        h_S1bins[i] = new TH1F(histname,histname,200,1.,4.);
    }    

    TH2F * h_er_band = new TH2F("h_er_band","h_er_band",100,0.,50.,200,0.,5.);
    TH1F * h_energy_spectrum = new TH1F("h_energy_spectrum",
                                        "h_energy_spectrum", 200, 0, 19.);


    NEST myNEST(1,1.,1.,2.88,100.); //particle type (0==NR 1 ==ER), energy deposited (keV), e-field (V/cm), xe density (g/cm^3)
    Detector myDetector;
    myDetector.LUXRun04PlanBSettings();
    myDetector.cathodeVoltage=100; //Example to modify the cathodeVoltage for Skin Response
    myNEST.SetDetectorParameters(myDetector);
    
     double rx, ry;  
     int binToTheLeft; 

     for(int i=0; i<Nevents; i++) {
       
        while(true) {
          rx = r.Uniform(0.001, Q);
          ry = r.Uniform(0, maxCnts);
          binToTheLeft = int(floor((rx)/step_T));
          if (ry < cnts[binToTheLeft]) {
            energy = T[binToTheLeft];
            //      cout << "draw succeeded!  (" << energy << ", " << ry << ") " <<  endl;
            break;
          }
        }

         myNEST.SetEnergy(energy);
         myNEST.DetectorResponse();

         if( myNEST.GetS1c() < 50. && myNEST.GetS1c() > 0.) { 
             S1bin = TMath::Floor( myNEST.GetS1c() );
             h_S1bins[S1bin]->Fill( TMath::Log10( myNEST.GetS2c() / myNEST.GetS1c() ) );
         } else
             S1bin = -1;
  
         h_er_band->Fill( myNEST.GetS1c(), TMath::Log10( myNEST.GetS2c() / myNEST.GetS1c() ) );
         if myNEST.Get
         h_energy_spectrum->Fill( 0.0137*( myNEST.GetS1c()/0.1035 + myNEST.GetS2c()/19.1875 ) );
         //h_energy_spectrum->Fill( energy );

     }


   ////////////////////////////////////////////////////////////////////////////////////////////////
   //   Do the fits to the simulated bands for comparison
   TF1 * gauss = new TF1("gauss","gaus(0)",0.,10.);
   TGraphErrors * sim_bandMean = new TGraphErrors();
   sim_bandMean->SetName("sim_bandMean");
   TGraph * sim_bandUpper = new TGraph();
   sim_bandUpper->SetName("sim_bandUpper");
   TGraph * sim_bandLower = new TGraph();
   
   for(int i=0; i<50; i++){
       
       gauss->SetParameters( h_S1bins[i]->GetBinContent( h_S1bins[i]->GetMaximumBin() ),
                             h_S1bins[i]->GetMean(),
                             h_S1bins[i]->GetRMS() );
       h_S1bins[i]->Fit(gauss);

       sim_bandMean->SetPoint( i, gauss->GetParameter(1) );
       sim_bandMean->SetPointError( i, gauss->GetParError(1) );
       sim_bandUpper->SetPoint( i, gauss->GetParameter(1) + gauss->GetParameter(2) );
       sim_bandLower->SetPoint( i, gauss->GetParameter(1) - gauss->GetParameter(2) );

   }


   outfile->Write();
   outfile->Close();

}
