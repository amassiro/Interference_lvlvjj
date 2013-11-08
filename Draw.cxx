
void Draw() {

 TFile* f1 = new TFile ("gen_126_jjme.root","READ");
 TFile* f2 = new TFile ("gen_500_jjme.root","READ");

 TNtuple* t1 = (TNtuple*) f1->Get ("ntu");
 TNtuple* t2 = (TNtuple*) f2->Get ("ntu");

 TH1F* h_mWW_1 = new TH1F("h_mWW_1","h_mWW_1",200,0,1000);
 TH1F* h_mWW_2 = new TH1F("h_mWW_2","h_mWW_2",200,0,1000);

 t1->Draw("mWW >> h_mWW_1","","goff");
 t2->Draw("mWW >> h_mWW_2","","goff");

 h_mWW_1->SetLineColor(kBlue);
 h_mWW_2->SetLineColor(kRed);

 h_mWW_1->SetLineStyle(1);
 h_mWW_2->SetLineStyle(2);

 h_mWW_1->SetLineWidth(2);
 h_mWW_2->SetLineWidth(2);

 TCanvas* cc = new TCanvas("cc","cc",800,600);
 h_mWW_1 -> Draw();
 h_mWW_2 -> Draw("same");

}


