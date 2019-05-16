// Code to put MIP peaks in ROOT format
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
  string fname = "MIP_no_gain.root";
  TFile *output = new TFile(fname.c_str(), "RECREATE");
  // string line_g;
  string line_m;
 
  ifstream mipFile("../MIP_no_gain.csv");

  if (!mipFile.is_open()){
    cout << "Error, files not opened" << endl;
    mipFile.close();
    return 0;
  }
  else {
    cout << "Files open... starting\n" << endl;
  }

 vector<string> mip;
 char delim = ','; // Ddefine the delimiter to split by
 string tmp;
  if(mipFile.is_open()) {
   while (getline(mipFile, tmp, delim)) {
     mip.push_back(tmp);
   }
 }

 
 // The strange way the csv file gets read means these loops are the only workaround
 double mip_value;
 double mip_error;

 TH1D *mip_h_13 = new TH1D("St13_mips","MIP Peaks",54,-0.5,53.5);
 TH1D *mip_h_19 = new TH1D("St19_mips","MIP Peaks",54,-0.5,53.5);

 int j,k;
 for (int i = 1 ; i < 55 ; i++){

   j = i*2-1;
   k = i*2-1;
   mip_value = atof(mip[j].c_str());
   mip_error = atof(mip[j+1].c_str());
   if(mip_error>0.05*mip_value)continue;
   mip_h_13->SetBinContent(i,mip_value);
   mip_h_13->SetBinError(i,mip_error);
   cout << i <<" "<<mip_value << endl;
  
 }


 for (int i = 55 ; i < 109 ; i++){
   j = i*2-1;
   k = i*2-1;
   mip_value = atof(mip[j].c_str());
   mip_error = atof(mip[j+1].c_str());
   if(mip_error>0.05*mip_value)continue;

   mip_h_19->SetBinContent(i-54,mip_value);
   mip_h_19->SetBinError(i-54,mip_error);
   
   cout << i <<" "<<mip_value << endl;

 }


 
  TCanvas *c1 = new TCanvas("c1", "c1", 2000, 1000);
  mip_h_13->SetTitle("Calo 13;Crystal;Cluster Energy [MeV]");
  mip_h_19->SetTitle("Calo 19;Crystal;Cluster Energy [MeV]");
  mip_h_13->SetLineColor(kBlue);
  mip_h_13->SetLineWidth(2);
  mip_h_19->SetLineColor(kRed);
  mip_h_19->SetLineWidth(2);

  /// mip_h_13->GetYaxis()->SetRangeUser(0.7,1.1);//xmin,xmax);
  gPad->SetGrid();
  mip_h_13->SetStats(0);  
  mip_h_19->SetStats(0);
  mip_h_13->Draw();
  mip_h_19->Draw("same");
  // mip_h_13->GetYaxis()->SetRangeUser(155,185);
  mip_h_13->SetDirectory(output);
  mip_h_19->SetDirectory(output);
  output->Write();
  c1->BuildLegend(0.79,0.79,0.89,0.89);
  mip_h_13->SetTitle("MIP peaks, no gain corrections;Crystal Number;Cluster Energy [MeV]");
  c1->SaveAs("MIP_no_gain.png");
  delete c1;
  output->Close();
   mipFile.close();
   cout<<"\nCreated: "+fname<<endl;
  return 0;
}