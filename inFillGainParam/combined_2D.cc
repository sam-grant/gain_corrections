// Plot the combined 1D distributons (pull and frac. unc.)
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
#include "TAttMarker.h"

using namespace std;
// Define pull
double pull(double Ep_content,double laser_content, double Ep_error, double laser_error) {
  double sigma = sqrt(Ep_error*Ep_error + laser_error*laser_error);
  double result = (Ep_content - laser_content) / sigma;
  return result;
}
// Define fractional shift
double frac_shift(double laser_value, double Ep_value) {
  // Fractional shift
  double result = (laser_value - Ep_value) / laser_value;
  return result;
}
// Drawing function
void draw(TH1D *hist, TFile *output, string name, string title) {
  // Define the gaussian function
  // TF1 *gFunc = new TF1("gFunc", "gaus",-2,2);
  // hist->Fit(gFunc,"R");
  // gStyle->SetOptFit(11);
  TCanvas *c = new TCanvas("c","c",1500,1000);
  hist->SetNdivisions(54);
  hist->SetName("Combined");
  hist->SetMarkerStyle(kStar);
  hist->SetMarkerSize(3);
    hist->SetLineWidth(2);

  hist->SetTitle(title.c_str());
  hist->Draw("PE");
  gPad->SetGrid();
  c->SaveAs(name.c_str());
  hist->SetDirectory(output);
  delete c;
  return;
}

int main() {

  bool quality = true;//false;
  bool full = true;
 
  string cut[4] = {"Q","statCut","errCut","chiCut"};
  string cutName[4] = {" | All Cuts"," | Stat Cut"," | Error Cut"," | #chi^{2} / NDF Cut"};
  string all, input_Ep_name, input_laser_name, output_name, label;
  double Ep_content, laser_content, Ep_error, laser_error;

  if(full) all = "_full_";
  else if(!full) all = "_";

  // Book input histograms
  TH1D *Ep, *laser;
  
  const int nBins = 12*2;
  string h[4] = {"tau_13","tau_19","amp_13","amp_19"};

  if(quality) {
    output_name = "combined_shift_1D"+all+"Q.root";//"+cut[icut]+".root";
  }
    else if(!quality) {
    output_name = "combined_shift_1D"+all+"noQ.root";
  }  

  // Set output
  TFile *output = new TFile(output_name.c_str(),"RECREATE");
  
  for (int icut(0); icut < 4; icut++) {

    if(quality) {
      input_Ep_name = "inFillGainParams_Ep_xtal_errors_"+cut[icut]+".root";
      input_laser_name = "inFillGainParams_laser_xtal_errors"+all+"Q.root";
      label = all+cut[icut]+".png";
    }
    else if(!quality) {
      input_Ep_name = "inFillGainParams_Ep_xtal_errors_noQ.root";
      input_laser_name = "inFillGainParams_laser_xtal_errors"+all+"Q.root";
      label = all+"noQ.png";
    }  

    TFile *input_Ep = TFile::Open(input_Ep_name.c_str());
    TFile *input_laser = TFile::Open(input_laser_name.c_str());

    // Book histograms
    TH1D *pullHist = new TH1D("pullHist","pullHist",54,-0.5,54);//nBins,-3,3);
    TH1D *fracHist = new TH1D("fracHist","fracHist",54,-0.5,54);//nBins,-3,3);
    
    int counter = 0;

    // Start hist loop
    for (int ihist(0); ihist < 4; ihist++) {
    
      cout<<"hist : "<<h[ihist]<<endl;
      // Start xtal loop
      for(int xtal(0); xtal < 54; xtal++) {
	// Reset
	double pullValue = 0;
	double fracValue = 0;
	// Get histograms
	Ep = (TH1D*)input_Ep->Get(h[ihist].c_str());
	laser = (TH1D*)input_laser->Get(h[ihist].c_str());
      
	// Get pullValues and errors
	Ep_content = Ep->GetBinContent(xtal+1);
	Ep_error = Ep->GetBinError(xtal+1);
       	if (Ep_content == 0) continue;
	laser_content = laser->GetBinContent(xtal+1);
	laser_error = laser->GetBinError(xtal+1);
	if(laser_content == 0) continue;
	pullHist->SetMarkerColor(kBlack);
	fracHist->SetMarkerColor(kBlack);      
	// Remove zeros in a hacky way :)
       	// if(pullValue==0) {
	//   pullHist->SetMarkerColor(kWhite);
	// }
	// else {
	//    pullHist->SetMarkerColor(kBlack);
	// }
	// if(fracValue==0) {
	//   fracHist->SetMarkerColor(kWhite);
	// }
	// else {
	//   fracHist->SetMarkerColor(kBlack);
	// }
	
	pullValue=pull(Ep_content,laser_content,Ep_error,laser_error);
	fracValue=frac_shift(laser_content,Ep_content);
	// Print
      
	cout<<"xtal: "<<xtal<<"; pull: "<<pullValue<<"; frac unc: "<<fracValue<<endl;
      
	// Fill
	//	if(pullValue==0) xtal = -1;
	//if(fracValue==0) xtal = -1;
	if(fabs(pullValue) > 100) continue;
	if(fabs(fracValue) > 100) continue;
   	pullHist->SetBinContent(xtal+1,pullValue);
	pullHist->SetBinError(xtal+1,0.000001);
	fracHist->SetBinContent(xtal+1,fracValue);
	fracHist->SetBinError(xtal+1,0.000001);
	counter++;
      }
    }
    cout << label << " xtals : " << counter << endl;
    counter = 0;
    draw(pullHist,output,("pull_shift"+label).c_str(),"Calos 13 & 19 | Pull (All Parameters)"+cutName[icut]+";Crystal Number;Pull [#sigma]");
    draw(fracHist,output,("frac_shift"+label).c_str(),"Calos 13 & 19 | Fractional Uncertainty (All Parameters)"+cutName[icut]+";Crystal Number;Fractional Uncertainty");
  
    output->Write(); 
    delete pullHist;
    delete fracHist;
  }
  cout<<"\n--------------------\n"<<output_name<<" created"<<endl;
  return 0;
}