#include <algorithm>

void DrawDistribution(std::string file = "file.root", std::string var = "jetpt1", int NBIN = 1000, int MIN = 0, int MAX = 1000, std::string varHR = ""){

 if (varHR == "") {
  varHR = var;
 }

 gStyle->SetOptStat(0);


 TFile* f  = new TFile (file.c_str(),"READ");
 TNtuple* t = (TNtuple*) f->Get ("ntu");


 //--------------------------
 TH1F* h   = new TH1F("CMS",varHR.c_str(),NBIN,MIN,MAX);

 TString toDraw;

 toDraw = Form ("%s >> CMS",var.c_str());       t    -> Draw(toDraw.Data(),"","goff");

 h->SetMarkerColor(kRed);
 h->SetLineWidth(2);
 h->SetLineColor(kRed);
 h->GetXaxis()->SetTitle(varHR.c_str());
 h->GetYaxis()->SetTitle("events");

 TCanvas* cn = new TCanvas ("cn","cn",800,600);
 h->Draw();
 cn->SetGrid();

}




