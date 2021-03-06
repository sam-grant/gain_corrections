// Read E/p fit parameters per xtal and put them in a ROOT file
// May 2019
// Sam Grant
// samuel.grant.18@ucl.ac.uk

#include <iostream>
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

int main() {

  string inputName = "../ROOT/FitsTimeXtalFid_9day.root";
  string outputName = "../ROOT/EpParameters_9day.root";

  TFile *input = TFile::Open(inputName.c_str());

  cout<<"Reading... "<<inputName<<input<<endl;

  TFile *output = new TFile(outputName.c_str(),"RECREATE");

  // Book historgrams 
  TH1D *tau13 = new TH1D("tau_13","tau_13",54,-0.5,53.5);
  tau13->SetTitle("Station 12;Xtal;#tau [#mus]");
  TH1D *tau19 = new TH1D("tau_19","tau_19",54,-0.5,53.5);
  tau19->SetTitle("Station 18;Xtal;#tau [#mus]");
  TH1D *amp13 = new TH1D("amp_13","amp_13",54,-0.5,53.5);
  amp13->SetTitle("Station 12;Xtal;#amp");
  TH1D *amp19 = new TH1D("amp_19","amp_19",54,-0.5,53.5);
  amp19->SetTitle("Station 18;Xtal;#amp");
  TH1D *chi13 = new TH1D("chi_13","chi_13",54,-0.5,53.5);
  chi13->SetTitle("Station 12;xtal; #chi^{2}/ndf");
  TH1D *chi19 = new TH1D("chi_19","chi_19",54,-0.5,53.5);
  chi19->SetTitle("Station 18;xtal;#chi^{2}/ndf");
  TH1D *N13 = new TH1D("N_13","N_13",54,-0.5,53.5);
  N13->SetTitle("Station 12;Xtal; Entries");
  TH1D *N19 = new TH1D("N_19","N_19",54,-0.5,53.5);
  N19->SetTitle("Station 18;Xtal;Entries");

  // Book params
  double amp, amp_err, tau, tau_err, chiSq, N;

  // Station loop
  for (int stn = 13; stn < 20; stn = stn + 6) {
    for (int xtal = 0; xtal < 54; xtal++ ) {
      // Get input histogram
      string h = "St"+to_string(stn)+"_Ep_vs_t_"+to_string(xtal);
      TH1D *hist = (TH1D*)input->Get(h.c_str());
      // Chuck empty ones
      if (hist == 0) continue;
      // Get fit
      TF1 *fit = (TF1*)hist->GetFunction("f1");
      // Get parameters
      amp = fit->GetParameter(1);
      amp_err = fit->GetParError(1);
      tau = fit->GetParameter(2);
      tau_err = fit->GetParError(2);
      chiSq = fit -> GetChisquare() / fit->GetNDF();
      N = hist->GetEntries();

      //Fill histograms
      if (stn == 13) {
        if(xtal==17) continue;
        tau13->SetBinContent(xtal+1,tau);
        tau13->SetBinError(xtal+1,tau_err);
        amp13->SetBinContent(xtal+1,amp);
        amp13->SetBinError(xtal+1,amp_err);
        chi13->SetBinContent(xtal+1,chiSq);
        N13->SetBinContent(xtal+1,N);
      }
      else if (stn == 19) {
       tau19->SetBinContent(xtal+1,tau);
       tau19->SetBinError(xtal+1,tau_err);
       amp19->SetBinContent(xtal+1,amp);
       amp19->SetBinError(xtal+1,amp_err);
       chi19->SetBinContent(xtal+1,chiSq);
       N19->SetBinContent(xtal+1,N);
     }
   }
 }

 amp13->Draw();
 tau13->Draw();
 amp19->Draw();
 tau19->Draw();
 chi13->Draw();
 chi19->Draw();
 N13->Draw();
 N19->Draw();
 amp13->SetDirectory(output);
 tau13->SetDirectory(output);
 amp19->SetDirectory(output);
 tau19->SetDirectory(output);
 chi13->SetDirectory(output);
 chi19->SetDirectory(output);
 N13->SetDirectory(output);
 N19->SetDirectory(output);

 output->Write();
 output->Close();
 
 cout<<"Created "<<outputName<<" "<<output<<endl;
 
 return 0;
}
