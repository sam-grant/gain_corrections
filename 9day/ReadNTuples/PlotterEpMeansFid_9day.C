// For the 9 day

#define Plotter_C
#include "Plotter.h"
#include "TMath.h"
#include "TFile.h"


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

  double ymin = 0.5;
  double ymax = 1.5;
  
  for (int stn = 13; stn < 20 ; stn = stn + 6) {
       plot2D("St"+std::to_string(stn)+"_Ep_vs_xtal",54,-0.5,53.5,1000,ymin,ymax,"Crystal Number","E/p");
   }
}

//=========================================================

//loop over the entries in the tree, making plots:

void Plotter::Run() {

  while( NextallmuonsEvent() ) {

    //loop over the matches in this event:
    for(int i=0; i<am->nmatches; i++) {
      if(am->trkPassPositronVertexQuality[i] == false) continue;
      if(am->nhits[i] != 1) continue;
      if(am->cluTime[i] < 60000) continue;
      
      double p = sqrt(am->trkMomX[i]*am->trkMomX[i] + am->trkMomY[i]*am->trkMomY[i] + am->trkMomZ[i]*am->trkMomZ[i]);

      int caloSt = am->cluCaloNum[i];
      if(caloSt > 19) continue;
      int trkSt = am->trkStationNum[i];
      double caloX_raw = am->cluX[i];
      double caloY_raw = am->cluY[i];

      int xtal = CaloNum(caloX_raw, caloY_raw);
      double E = am->cluEne[i];
      double Ep = E/p;

      if(!FiducialEdge(xtal)) continue;

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal",xtal,Ep);
     
    }
  }
  return;
}
