#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"
#include "NEST.h"
#include "TMath.h"
#include "TFile.h"
#include <iostream>
#include "TGraph.h"
#include "TGraphErrors.h"

using namespace std;


double DrawEnergyFromDDSpectrum(TRandom3 r);


int main() {

    TRandom3 r;
    double energy;
    int Nevents = 1000000;
    
    double S1c[1000];
    double S2c[1000];

    TFile * outfile = new TFile("dd_histograms.root","RECREATE");

    TH1F * h_S1bins[50];
    int S1bin;
    char histname[50];

    for(int i=0; i<50; i++) { 
        sprintf(histname,"h_S1bin_%d",i);
        h_S1bins[i] = new TH1F(histname,histname,200,1.,4.);
    }    

    TH2F * h_dd_band = new TH2F("h_dd_band","h_dd_band",100,0.,50.,200,0.,5.);
    TH1F * h_energy_spectrum = new TH1F("h_energy_spectrum",
                                        "h_energy_spectrum", 200, 0, 100.);
    TH1F * h_S1_spectrum = new TH1F("h_S1_spectrum","h_S1_spectrum",200,0.,200.);

    NEST myNEST(0,1.,1.,2.88,100.); //particle type (0==NR 1 ==ER), energy deposited (keV), e-field (V/cm), xe density (g/cm^3)
    Detector myDetector;
    myDetector.LUXRun04PlanBSettings();
    myNEST.SetDetectorParameters(myDetector);
   
    myNEST.SetElectricField(2000.);
    myNEST.SetFrPrefactor(0.032);
 

     for(int i=0; i<Nevents; i++) {
       
         energy = DrawEnergyFromDDSpectrum( r );

         myNEST.SetEnergy(energy);
         myNEST.DetectorResponse();

         if( myNEST.GetS1c() < 50. && myNEST.GetS1c() > 0.) { 
             S1bin = TMath::Floor( myNEST.GetS1c() );
             h_S1bins[S1bin]->Fill( TMath::Log10( myNEST.GetS2c() / myNEST.GetS1c() ) );
         } else
             S1bin = -1;
  
         h_dd_band->Fill( myNEST.GetS1c(), TMath::Log10( myNEST.GetS2c() / myNEST.GetS1c() ) );
         h_energy_spectrum->Fill( energy );
         h_S1_spectrum->Fill( myNEST.GetS1c() );
         //h_energy_spectrum->Fill( energy );
         cout << myNEST.GetS1c() << "\t" << myNEST.GetS2c() << endl;

     }


   ////////////////////////////////////////////////////////////////////////////////////////////////
   //   Do the fits to the simulated bands for comparison
   TF1 * gauss = new TF1("gauss","gaus(0)",0.,10.);
   TGraphErrors * sim_bandMean = new TGraphErrors();
   sim_bandMean->SetName("sim_bandMean");
   TGraph * sim_bandUpper = new TGraph();
   sim_bandUpper->SetName("sim_bandUpper");
   TGraph * sim_bandLower = new TGraph();
   sim_bandLower->SetName("sim_bandLower");
   
   for(int i=0; i<50; i++){
       
       gauss->SetParameters( h_S1bins[i]->GetBinContent( h_S1bins[i]->GetMaximumBin() ),
                             h_S1bins[i]->GetMean(),
                             h_S1bins[i]->GetRMS() );
       h_S1bins[i]->Fit(gauss,"Q","Q",0.,10.);

       sim_bandMean->SetPoint( i, (double) i+0.5, gauss->GetParameter(1) );
       sim_bandMean->SetPointError( i, 0., gauss->GetParError(1) );
       sim_bandUpper->SetPoint( i, (double) i+0.5, gauss->GetParameter(1) + gauss->GetParameter(2) );
       sim_bandLower->SetPoint( i, (double) i+0.5, gauss->GetParameter(1) - gauss->GetParameter(2) );

   }


   sim_bandMean->Write();
   sim_bandUpper->Write();
   sim_bandLower->Write();
   outfile->Write();
   outfile->Close();

}



//////////////////////////////////////////////////////////////////////////////////////////////////


double DrawEnergyFromDDSpectrum ( TRandom3 r ) { 

  r.SetSeed(0);
  double xMin = 0.; //keV
  double xMax = 75.; //keV  
  double yMin = 0.; //Some normalization; ask Matthew for deets.
  double yMax = 5.5; 
 
  double xTry = xMin+(xMax-xMin)*r.Rndm();
  double yTry = yMin+(yMax-yMin)*r.Rndm();
  
  double FuncValue;
  if ( xTry < 9.8 )
    FuncValue = 4.1411-3.1592*TMath::Log10(xTry);
  else
    FuncValue = 2.4348-0.19783*xTry+0.0060931*TMath::Power(xTry,2.)-
                8.2243e-5*TMath::Power(xTry,3.)+4.2505e-7*TMath::Power(xTry,4.);
  
  while ( yTry > FuncValue ) { 
    xTry = xMin+(xMax-xMin)*r.Rndm();
    yTry = yMin+(yMax-yMin)*r.Rndm();
    if ( xTry < 9.8 )
      FuncValue = 4.1411-3.1592*TMath::Log10(xTry);
    else
      FuncValue = 2.4348-0.19783*xTry+0.0060931*TMath::Power(xTry,2.)-
                  8.2243e-5*TMath::Power(xTry,3.)+4.2505e-7*TMath::Power(xTry,4.);
  }
  
  return xTry; //selection under curve made
}


