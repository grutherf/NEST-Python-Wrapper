#include <iostream>
#include "TMath.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include <fstream>
#include "TLatex.h"

using namespace std;

/*Macro to create .root files with histogram maps from text files given*/

void drawHists(){
  ifstream infile("LCEAndEField.txt");
  double r, z, eField, lce;
    double rMin = 0;
    double zMin = -50;
    double nR = 84;
    double nZ = 200;
    TH2D* lceMap = new TH2D("LCE","LCE",nR,rMin,rMin+nR,nZ,zMin,zMin+nZ);
    TH2D* eFieldMap = new TH2D("EField","EField",nR,rMin,rMin+nR,nZ,zMin,zMin+nZ);
    double rBinWidth, zBinWidth;
    rBinWidth=zBinWidth=1;
    int n = 0;
    while(n<16800){
      infile >> r >> z >> lce >> eField;
      lceMap->SetBinContent(TMath::CeilNint((r-rMin)/rBinWidth),TMath::CeilNint((z-zMin)/zBinWidth),lce); 
      eFieldMap->SetBinContent(TMath::CeilNint((r-rMin)/rBinWidth),TMath::CeilNint((z-zMin)/zBinWidth),eField);
      ++n;
    }
    infile.close();
    
    TCanvas* c1 = new TCanvas();
    lceMap->SetStats(0);
    lceMap->GetXaxis()->SetTitle("r [cm]");
    lceMap->GetYaxis()->SetTitle("z [cm]");
    lceMap->GetZaxis()->SetLabelSize(0.03);
    eFieldMap->SetStats(0);
    eFieldMap->GetXaxis()->SetTitle("r [cm]");
    eFieldMap->GetYaxis()->SetTitle("z [cm]");
    eFieldMap->GetZaxis()->SetLabelSize(0.03);
    lceMap->Draw("colz");
    c1->Print("lceMap.Root");
    c1->Print("lceMap.C");
    c1->Clear();
    eFieldMap->Draw("colz");
    c1->Print("eFieldMap.Root");
    c1->Print("eFieldMap.C");
}
