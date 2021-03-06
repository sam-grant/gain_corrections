#include <iostream>
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

// Drawing function 1D 
void draw(TH1D *hist1, TH1D *hist2, string fname, string title) {
  TCanvas *c = new TCanvas("c","c",3000,2500);
  TLegend *leg = new TLegend(0.75,0.75,0.89,0.89);
  leg->SetBorderSize(0);
  hist1->GetYaxis()->SetMaxDigits(2);
  //  hist1->GetYaxis()->SetTitleOffset(1.5);
  hist1->SetLineWidth(20);
  hist2->SetLineWidth(20);
  hist1->SetLineColor(kBlue);
  hist2->SetLineColor(kRed);
  hist1->SetStats(0);
  hist1->SetTitle("Stn 12");
  hist2->SetTitle("Stn 18");
  leg->AddEntry(hist1);
  leg->AddEntry(hist2);
  hist1->Draw();
  hist2->Draw("same");
  leg->Draw("same");
  hist1->SetTitle(title.c_str());
  c->SaveAs(fname.c_str());
  delete c;
  //  delete leg;
  return;
}

int main() {

  string inputFname = "RootFiles/NoCuts.root";//trkPositron.root";
  TFile *input = TFile::Open(inputFname.c_str());

  cout<<"Reading... "<<inputFname<<endl;

  TH1D *dt12 = (TH1D*)input->Get("St13_dt");
  TH1D *dt18 = (TH1D*)input->Get("St19_dt");
  TH1D *dR12 = (TH1D*)input->Get("St13_dR");
  TH1D *dR18 = (TH1D*)input->Get("St19_dR");
  TH1D *logEop12 = (TH1D*)input->Get("St13_logEop");
  TH1D *logEop18 = (TH1D*)input->Get("St19_logEop");
  //   dR12->GetXaxis()->SetRangeUser(0,50);
  draw(dt12,dt18,"Plots/dt.png",";Track-cluster #Deltat [ns];Entries");
  draw(dR12,dR18,"Plots/dR.png",";Track-cluster #DeltaR [mm];Entries");
  draw(logEop12,logEop18,"Plots/logEop.png",";Track-cluster log(cluster energy / track momentum);Entries");
  
  return 0;
}
