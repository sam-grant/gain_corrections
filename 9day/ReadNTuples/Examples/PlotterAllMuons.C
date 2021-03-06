// Example: read the all muons branch

#define Plotter_C
#include "Plotter.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"

using namespace std;

int CaloNum(int caloX, int caloY) {
  return caloX+9*caloY;
}

void Plotter::InitTrees(TString input_file) {
  //initialise the trees you want to read
  //then enable the relevant branches in Reader.C
  // ctt = new clusterTrackerTrackReader(input_file, "clusterTracker");  
  // cr = new clusterTrackerClusterReader(input_file);  
  // cl = new clusterReader(input_file);  
  //  tr = new trackerReader(input_file);  
  am = new allmuonsReader(input_file);  
}

void Plotter::InitHistos() {

  plot2D("St"+std::to_string(stn)+"_E_vs_p",200,0,4000,200,0,4000,"Track Momentum [MeV]","Cluster Energy [MeV]"); 

  for (int stn(12); stn < 19; stn = stn + 6) {
    plot2D("E_vs_p",200,0,4000,200,0,4000,"Track Momentum [MeV]","Cluster Energy [MeV]"); 
  }

}

//=========================================================

//loop over the entries in the tree, making plots:

void Plotter::Run() {
  
  while( NextallmuonsEvent() ) {

    //loop over the matches in this event:
    for(int i=0; i<am->nmatches; i++) {

      if(am->nhits[i] != 1) continue;       

      int runNum = am->runNum;
      int subRunNum = am->subRunNum;
      int eventNum = am->eventNum;
      int bunchNum = am->bunchNum;
      int caloNum = am->cluCaloNum[i];
      int midasSeri = am->midasSerialNum;
      int trkSt = am->trkStationNum[i];
      double p = sqrt(am->trkMomX[i]*am->trkMomX[i] + am->trkMomY[i]*am->trkMomY[i] + am->trkMomZ[i]*am->trkMomZ[i]);
      double logEop = log(am->EovP[i]);
      double dt = am->Tdiff[i];
      double caloX_raw = am->cluX[i];
      double caloY_raw = am->cluY[i];
      int xtalNum = CaloNum(caloX_raw, caloY_raw);
      double caloX = 112.5 - 25*(caloX_raw);
      double caloY = SetCaloY(caloNum, caloY_raw);
      double trX = am->vX[i];
      double trY = am->vY[i];
      double dX = caloX - trX;
      double dY = caloY - trY;
      double dR = sqrt(dX*dX + dY*dY);
      double t = (am -> cluTime[i]);
      if(t<4200) continue;
      double E = am->cluEne[i];
      double Ep = E/p;

      Fill2D("E_vs_p",p,E);
      Fill2D("St"+std::to_string(trkSt)+"_E_vs_p",p,E);       

    }
  }
  
  return;
  
}
