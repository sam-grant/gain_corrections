// Make plots of the fluctuation, combined between both stations
// Sam Grant
// samuel.grant.18@ucl.ac.uk
// May-June 2019

#include <iostream>
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TDirectory.h"
#include "TGraphErrors.h"

using namespace std;

// Fluctation is basically the fractional error
double fluc_val(TH1D *laser_hist, TH1D *ep_hist, int iter) {
    double laser_value = laser_hist->GetBinContent(iter+1);
    double ep_value = ep_hist->GetBinContent(iter+1);
    double laser_error = laser_hist->GetBinError(iter+1);
    double ep_error = ep_hist->GetBinError(iter+1);
    // Fractional shift
    double result = (laser_value - ep_value) / laser_value;
    return result;
}

double fluc_err(TH1D *laser_hist, TH1D *ep_hist, int iter) {
  double laser_value = laser_hist->GetBinContent(iter+1);
  double ep_value = ep_hist->GetBinContent(iter+1);
  // Fractional shift
  double result = (laser_value - ep_value) / laser_value;
  double laser_error = laser_hist->GetBinError(iter+1);
  double ep_error = ep_hist->GetBinError(iter+1);
  // Calcuate the uncertainty
  double term1 = (sqrt(laser_error*laser_error+ep_error*ep_error))/(laser_value-ep_value); // Checked
  double term2 = (laser_error/laser_value); // Checked
  double result_error = result * sqrt(term1*term1+term2*term2); // Checked
  return result_error;
 
}

// double shift(TH1D *laser_hist, TH1D *ep_hist, int iter) {
//   double laser_value = laser_hist->GetBinContent(iter+1);
//   double ep_value = ep_hist->GetBinContent(iter+1);
//   // Fractional shift
//   double diff = (laser_value - ep_value);
//   if(diff == laser_value) return 1000;
//   double laser_error = laser_hist->GetBinError(iter+1);
//   double ep_error = ep_hist->GetBinError(iter+1);
//   // Calcuate the uncertainty
//   double diff_err = sqrt(laser_error*laser_error+ep_error*ep_error);
//   double result = diff / fabs(diff_err);
//   return result;
// }


void draw(TH1D *hist, string name) {
  TH1D *hist_clone = (TH1D*)hist->Clone("hist_clone");
  // hist_clone->Fill(value);  
  TCanvas *c = new TCanvas();
  //  gStyle->SetOptStat(112211);
  hist_clone->SetStats(0);//111111);
  hist_clone->SetLineWidth(2);
  hist_clone->SetLineColor(kBlack);
  //hist_clone->SetTitle(title.c_str());
  hist_clone->Draw();
  gPad->SetGrid();
  //  gPad->SetGrid();
  c->SaveAs(name.c_str());
  //  hist_clone->SetDirectory(output);
  delete c;
}

int main() {

  TFile *laser_input = TFile::Open("inFillGainParams_laser_xtal_errors_Q.root");
  TFile *Ep_input = TFile::Open("inFillGainParams_Ep_xtal_errors_Q.root");
  string h[4] = {"tau_13","tau_19","amp_13","amp_19"};
  string name[2] = {"fluc_tau.png","fluc_amp.png"};//{"fluc_tau1D_13.png","fluc_tau1D_19.png","fluc_amp1D_13.png","fluc_amp1D_19.png"};
  //  string title[2] = {"Recovery Time: Fractional Shift per Crystal;Fractional Shift;Entries","Amplitude Fractional Shift per Crystal;Fractional Shift;Entries"};//{"Calo 13 | Recovery Time Fluctuation;Fluctuation;Entries","Calo 19 | Recovery Time Fluctuation;Fluctuation;Entries","Calo 13 | Amplitude Fluctuation;Fluctuation;Entries","Calo 19 | Amplitude Fluctuation;Fluctuation;Entries"};
  string title[4] = {"Calo 13 | Recovery Time: Fractional Shift per Crystal;Crystal Number;Fractional Shift","Calo 19 | Recovery Time: Fractional Shift per Crystal;Crystal Number;Fractional Shift","Calo 13 | Amplitude: Fractional Shift per Crystal;Crystal Number;Fractional Shift","Calo 19 | Amplitude: Fractional Shift per Crystal;Crystal Number;Fractional Shift"};

  // book historgrams
  // TH1D *fluc = new TH1D("fluc","fluc",54,-0.5,53.5);

  //  TH1D *tau_fluc = new TH1D("tau_fluc","tau_fluc",40,-5,3);//54,-0.5,53.5);
  //  TH1D *amp_fluc = new TH1D("amp_fluc","amp_fluc",15,-3,3);//54,-0.5,53.5);
  TH1D *tau13_fluc = new TH1D("tau13_fluc","tau13_fluc",54,-0.5,53.5);
  TH1D *amp13_fluc = new TH1D("amp13_fluc","amp13_fluc",54,-0.5,53.5);
  TH1D *tau19_fluc = new TH1D("tau19_fluc","tau19_fluc",54,-0.5,53.5);
  TH1D *amp19_fluc = new TH1D("amp19_fluc","amp19_fluc",54,-0.5,53.5);
  TH1D *laser_hist;
  TH1D *Ep_hist;
  double value;
  double error;

  for(int i(0); i<4; i++) {

    laser_hist = (TH1D*)laser_input->Get(h[i].c_str());

    Ep_hist = (TH1D*)Ep_input->Get(h[i].c_str());
    //    if(i!=0)continue;
    for (int xtal(0); xtal<54; xtal++) {

      //  value = fluc_val(laser_hist,Ep_hist,xtal);
      // Cut out station 19
      if(i==0 && xtal == 23) continue;
      error = fluc_err(laser_hist,Ep_hist,xtal);
      value = fluc_val(laser_hist,Ep_hist,xtal);
      if (value == 1) continue; // AKA, the Ep value is zero
      if( fabs(error) > 2 )continue;
      cout << xtal << " " << value << "+/-" << error << " " << (error/value) * 100 <<endl;


      if(i==0) {
	tau13_fluc->SetBinContent(xtal+1,value);//,xtal+1);//*(1/fabs(error)));
	tau13_fluc->SetBinError(xtal+1,error);//,xtal+1);
	tau13_fluc->SetTitle(title[i].c_str());
      }
      if(i==1){
	tau19_fluc->SetBinContent(xtal+1,value);//,xtal+1);//*(1/fabs(error)));
	tau19_fluc->SetBinError(xtal+1,error);//,xtal+1);
		tau19_fluc->SetTitle(title[i].c_str());
      }
       if(i==2) {
	amp13_fluc->SetBinContent(xtal+1,value);//,xtal+1);//*(1/fabs(error)));
	amp13_fluc->SetBinError(xtal+1,error);//,xtal+1);
	amp13_fluc->SetTitle(title[i].c_str());
      }
       if(i==3) {
	amp19_fluc->SetBinContent(xtal+1,value);//,xtal+1);//*(1/fabs(error)));
	amp19_fluc->SetBinError(xtal+1,error);//,xtal+1);
       	amp19_fluc->SetTitle(title[i].c_str());
      }	
 
      //      fluc->SetBinWidth(error);
    }

  }	  


  
  draw(tau13_fluc,"tau13_fluc_xtal.png");
  draw(amp13_fluc,"amp13_fluc_xtal.png");

  draw(tau19_fluc,"tau19_fluc_xtal.png");
  draw(amp19_fluc,"amp19_fluc_xtal.png");
  // for(int i(2); i<4; i++) {

//   laser_hist = (TH1D*)laser_input->Get(h[i].c_str());

//   Ep_hist = (TH1D*)Ep_input->Get(h[i].c_str());

//   fluctuation();
 

// fluctuation(fluc,laser_amp13,Ep_amp13,title[1],name[1]);






laser_input->Close();

Ep_input->Close();





return 0;



}

  
  // Combine calos
  // TH1D *laser_tau13 = (TH1D*)laser_input->Get(h[0].c_str());
  // TH1D *laser_tau19 = (TH1D*)laser_input->Get(h[1].c_str());
  // TH1D *laser_amp13 = (TH1D*)laser_input->Get(h[2].c_str());
  // TH1D *laser_amp19 = (TH1D*)laser_input->Get(h[3].c_str());
  // TH1D *Ep_tau13 = (TH1D*)Ep_input->Get(h[0].c_str());
  // TH1D *Ep_tau19 = (TH1D*)Ep_input->Get(h[1].c_str());
  // TH1D *Ep_amp13 = (TH1D*)Ep_input->Get(h[2].c_str());
  // TH1D *Ep_amp19 = (TH1D*)Ep_input->Get(h[3].c_str());

  // laser_tau13->Add(laser_tau19,1);
  // laser_amp13->Add(laser_amp19,1);
  // Ep_tau13->Add(Ep_tau19,1);
  // Ep_amp13->Add(Ep_amp19,1);
  
  
  // for(int calo(0); calo < 2; calo++) {
  //   laser_hist1 = (TH1D*)laser_input->Get(hTau[i].c_str());
  //   laser_hist2 = (TH1D*)laser_input->Get(hTau[i+1].c_str());
  //   Ep_hist1 = (TH1D*)Ep_input->Get(hTau


    
  // 				    }
  





    
  //     for (int i(0); i<4; i++) {
  // 				if (i < 2) {
  // 					    laser_hist1 = (TH1D*)laser_input->Get(h[i].c_str());
  // 					    Ep_hist1 = (TH1D*)Ep_input->Get(h[i].c_str());
  // 				}
  // 				else if(i => 2) {
  // 						 laser_hist2 = (TH1D*)laser_input->Get(h[i+1].c_str());
  // 						 Ep_hist2 = (TH1D*)Ep_input->Get(h[i+1].c_str());
  // 				}
  //     }
  
   
  //   fluctuation(fluc,laser_hist,ep_hist,title[i],name[i],i);
  // }
  
    
 