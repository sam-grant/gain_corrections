#include <iostream>                                                             
#include "TFile.h"                                                              
#include "TCanvas.h"
#include "TH2D.h"                                                               
#include "TAxis.h"
#include "TStyle.h"
#include "TProfile.h"                                                                                                                         
using namespace std;

// Colour map plotter                                                           
void cmap(TH2D *hist, string title, string fname) {    
  //Clone to stop bugs                                                          
  TH2D *hist_clone = (TH2D*)hist->Clone("hist_clone");                          
  //Force the ranges to be sensible if ROOT's stupid autoscale fails like norma\
  l                                                                               
  double xmin = hist_clone->GetXaxis()->GetBinCenter( hist_clone -> FindFirstBinAbove(0,1) );           
  double xmax = hist_clone->GetXaxis()->GetBinCenter( hist_clone -> FindLastBinAbove(0,1) );                                               
  hist_clone->GetXaxis()->SetRangeUser(xmin,xmax);                              
  double ymin = hist_clone->GetYaxis()->GetBinCenter( hist_clone -> FindFirstBinAbove(0,2) );
  double ymax = hist_clone->GetYaxis()->GetBinCenter( hist_clone -> FindLastBinAbove(0,2) );
  hist_clone->GetYaxis()->SetRangeUser(ymin,ymax);                                            
  TCanvas *c = new TCanvas("c", "c", 2000, 1000);                               
  gStyle->SetPalette(55);                                                       
  hist_clone->SetStats(0);                                                      
  hist_clone->SetTitle(title.c_str());                                          
  c->SetRightMargin(0.13);                                                      
  hist_clone->SetOption("COLZ");                                                
  gStyle->SetPalette(55);                                                       
  hist_clone->Draw();                                                           
  c->SaveAs(fname.c_str());
  delete c;                                                                     
  return;                                                                       
}

int main() {
  
  TFile *input = TFile::Open("gain_uncorrected_new.root");
  TH2D *t_early = (TH2D*)input->Get("cmap_Ep_vs_t_early_30");
  TH2D *E = (TH2D*)input->Get("cmap_Ep_vs_E_30");
  string ttitle = "E/p vs t (early), e^{+}, High Flux;Absolute Time [#mus];E/p";
  string Etitle = "E/p vs t (early), e^{+}, High Flux;Energy in Calo [GeV];E/p";
  
  cmap(t_early,ttitle,"cmap_t.png");
  cmap(E,Etitle,"cmap_E.png");

  delete t_early;
  delete E;
  
  return 0;
}
