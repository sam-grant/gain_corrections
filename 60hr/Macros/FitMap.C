// Sam Grant - August 2019
// samuel.grant.18@ucl.ac.uk
// Draw all my gauss fits in corresponding crystal position.
// Y axis needed reversing 

#include <iostream>
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

int main() {
  string inputFname = "RootFiles/FitsTimeXtalFid_Q.root";
  TFile *input = TFile::Open(inputFname.c_str());
  cout<<"Reading... "<<inputFname<<endl;

  int place = 0;
  
  TCanvas *c1  = new TCanvas("","",9000,9000);    
  c1->Divide(3,6);
  for (int stn(13) ; stn < 20 ; stn = stn + 6 ) {

    // 18 xtals, so draw a 3 by 6 grid
    for(int xtal(0); xtal<54; xtal++) {

      string name = "St"+to_string(stn)+"_Ep_vs_t_"+to_string(xtal);
      // Change input if need be
      TH1D *h = (TH1D*)input->Get(name.c_str());
      if(h==0) continue;

      c1->cd(place+1);
      place++;
      cout<<place<<endl;
      h->SetStats(1);
      gStyle->SetOptStat(10);
      gStyle->SetOptFit(111);
      h->Draw();

    }

  }
    string fname = "Plots/FitXtalMap.pdf";
    c1->SaveAs(fname.c_str());
    delete c1;

  return 0;
}
