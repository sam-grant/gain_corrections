// Plots the fluctuation

#include <iostream>
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TDirectory.h"

using namespace std;

void fluctuation(TH1D *fluc, TH1D *laser_hist, TH1D *ep_hist, string title, string name, int type) {
  //Clone input to be safe 
  TH1D *fluc_clone = (TH1D*)fluc->Clone("fluc_clone");
  int counter = 0;
  for (int xtal = 0; xtal < 54; xtal++) {

      //    // xtal 15 is bad for station 19, get rid of it
      // if((type == 1 && xtal == 15) || (type == 3 && xtal == 15)) continue;
      // // xtal 51 is bad for station 13, get rid of it
      // if((type == 0 && xtal == 51) || (type == 2 && xtal == 51)) continue;
      // // xtal 35 is bad for station 13, get rid of it
      //  if((type == 0 && xtal == 35) || (type == 2 && xtal == 35)) continue;

    double laser_value = laser_hist->GetBinContent(xtal+1);
    double ep_value = ep_hist->GetBinContent(xtal+1);
    // cout<<laser_value<<endl;
    // cout<<ep_value<<endl;
    if (ep_value==0) continue;
    counter++;
    //  cout<<laser_value<<" "<<ep_value<<endl;
    double laser_error = laser_hist->GetBinError(xtal+1);
    double ep_error = ep_hist->GetBinError(xtal+1);
    
    // if( (xtal == 16 || xtal == 31 || xtall == 23) ) continue;//&& (type == 1 || type == 3) )continue;
     double result = (laser_value - ep_value) / laser_value;
     cout<<xtal<<" "<<result<<endl;
    // double term1 = (sqrt(laser_error*laser_error+ep_error*ep_error))/(laser_value-ep_value);
    //   double term2 = (laser_error/laser_value);
    // double result_error = result * sqrt(term1*term1+term2*term2);
    //   cout<<result<<endl;
    // fluc_clone->SetBinContent(xtal+1,result);
    // fluc_clone->SetBinError(xtal+1,result_error);

    
    fluc_clone->Fill(result);
    
  }
  cout << counter << endl;
  TCanvas *c = new TCanvas();
  gStyle->SetOptStat(111111);
  // fluc_clone->SetStats(111111);
  fluc_clone->SetLineWidth(2);
  fluc_clone->SetLineColor(kBlack);
  fluc_clone->SetTitle(title.c_str());
  fluc_clone->Draw();
  //  gPad->SetGrid();
  c->SaveAs(name.c_str());
  //  fluc_clone->SetDirectory(output);
  delete c;
  delete fluc_clone;
  return;
}


int main() {


  TFile *laser_input = TFile::Open("inFillGainParams_laser_xtal_errors_Q.root");
  TFile *ep_input = TFile::Open("inFillGainParams_sam_xtal_errors_Q.root");

  string h[4] = {"tau_13","tau_19","amp_13","amp_19"};
  string name[4] = {"fluc_tau1D_13.png","fluc_tau1D_19.png","fluc_amp1D_13.png","fluc_amp1D_19.png"};
  string title[4] = {"Calo 13 | Recovery Time Fluctuation;Fluctuation;Entries","Calo 19 | Recovery Time Fluctuation;Fluctuation;Entries","Calo 13 | Amplitude Fluctuation;Fluctuation;Entries","Calo 19 | Amplitude Fluctuation;Fluctuation;Entries"};
  // book historgrams
  // TH1D *fluc = new TH1D("fluc","fluc",54,-0.5,53.5);
  TH1D *fluc = new TH1D("fluc","fluc",20,-10,10);

  TH1D *laser_hist;
  TH1D *ep_hist;

  for (int i(0); i<4; i++) {
    
    laser_hist = (TH1D*)laser_input->Get(h[i].c_str());
    ep_hist = (TH1D*)ep_input->Get(h[i].c_str());
    
    fluctuation(fluc,laser_hist,ep_hist,title[i],name[i],i);
  }
  
  laser_input->Close();
  ep_input->Close();
  

  return 0;

  }

    
 
