// Convert csv to ROOT
// Sam Grant 
// April 2019
// samuel.grant.18@ucl.ac.uk

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h> 
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TDirectory.h"
#include "TLegend.h"


using namespace std;

int main() {

  
  string output_fname = "inFillGainParams_laser.root";
  TFile *output = new TFile(output_fname.c_str(), "recreate");
  
  string line_m;
  ifstream laserFile("inFillGainParams_laser.csv");

  if (!laserFile.is_open()){
    cout << "Error, files not opened" << endl;
    laserFile.close();
    return 0;
  }
  else {
    cout << "File open... starting\n" << endl;
  }

 vector<string> laser;
 char delim = ','; // Ddefine the delimiter to split by
 string tmp;
  if(laserFile.is_open()) {
   while (getline(laserFile, tmp, delim)) {
     laser.push_back(tmp);
   }
 }

 
 // The strange way the csv file gets read means these loops are the only workaroun

  TH1D *tau13 = new TH1D("tau 13","Calo 13",64,0,16);//-0.2,16.2);
  TH1D *tau19 = new TH1D("tau 19","Calo 19",64,0,16);//-0.2,16.2);
  TH1D *amp13 = new TH1D("amp 13","Calo 13",100,0,0.05);//0-0.002,0.052);
  TH1D *amp19 = new TH1D("amp 19","Calo 19",100,0,0.05);//-0.002,0.052);



 int j,k;
 for (int i = 1 ; i < 55 ; i++){
   // convert strings into doubles with atof
   j = i*2-1;
   k = i*2-1;
   double amp = atof(laser[j].c_str());
   double tau = atof(laser[j+1].c_str());
   // Convert clock ticks to mircro seconds
   tau = tau * 1.25;
   cout<<i-1<<" "<<amp<<" "<<tau<<endl;

   amp13->Fill(amp);
   tau13->Fill(tau);
 }

 for (int i = 55 ; i < 109 ; i++){
   j = i*2-1;
   k = i*2-1;
   double amp = atof(laser[j].c_str());
   double tau = atof(laser[j+1].c_str());
   tau = tau * 1.25;
   cout<<i-55<<" "<<amp<<" "<<tau<<endl;
  
   amp19->Fill(amp);
   tau19->Fill(tau);
  }

  TCanvas *c2 = new TCanvas();//"c2","c2",2000,1000);
  tau13->SetStats(0);
  tau13->SetLineColor(kOrange+2);
  tau19->SetLineColor(kBlue+2);
  tau13->SetLineWidth(2);
  tau19->SetLineWidth(2);
  tau13->Draw();
  tau19->Draw("same");
  c2->BuildLegend(0.78,0.79,0.89,0.89);
  tau13->SetTitle("Recovery times;In Fill Time [#mus];N");
  tau13->SetDirectory(output);
  tau19->SetDirectory(output);
  c2->SaveAs("tau_laser.png");
  delete c2;

  TCanvas *c3 = new TCanvas();//"c3","c3",2000,1000);
  amp13->SetStats(0);
  amp13->SetLineColor(kOrange+2);
  amp19->SetLineColor(kBlue+2);
  amp13->SetLineWidth(2);
  amp19->SetLineWidth(2);
  amp13->Draw();
  amp19->Draw("same");
  c3->BuildLegend(0.78,0.79,0.89,0.89);
  amp13->SetTitle("Amplitudes;Amplitude;N");
  amp13->SetDirectory(output);
  amp19->SetDirectory(output);
 
  c3->SaveAs("amp_laser.png");
  delete c3;

  output->Write();
  output->Close();

  laserFile.close();
 
  return 0;
}