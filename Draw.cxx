
void Draw() {

 TFile* f1 = new TFile ("gen_126_jjmm.root","READ"); // ---- B
 TFile* f2 = new TFile ("gen_500_jjmm.root","READ"); // ---- S+B

//  TFile* f1 = new TFile ("gen_126_jjme.root","READ"); // ---- B
//  TFile* f2 = new TFile ("gen_500_jjme.root","READ"); // ---- S+B

 TNtuple* t1 = (TNtuple*) f1->Get ("ntu");
 TNtuple* t2 = (TNtuple*) f2->Get ("ntu");

 TH1F* h_mWW_1 = new TH1F("h_mWW_1","h_mWW_1",200,0,1000);
 TH1F* h_mWW_2 = new TH1F("h_mWW_2","h_mWW_2",200,0,1000);
 TH1F* h_Ratio = new TH1F("h_mWW_ratio","h_mWW_ratio",200,0,1000);
 TH1F* h_Subtraction = new TH1F("h_Subtraction","h_Subtraction",200,0,1000);

 t1->Draw("mWW >> h_mWW_1","","goff");
 t2->Draw("mWW >> h_mWW_2","","goff");

 h_mWW_1->SetLineColor(kBlue);
 h_mWW_2->SetLineColor(kRed);

 h_mWW_1->SetLineStyle(1);
 h_mWW_2->SetLineStyle(2);

 h_mWW_1->SetLineWidth(2);
 h_mWW_2->SetLineWidth(2);

 //  h_mWW_1->Scale (1.2);  //--- me
 h_mWW_1->Scale (0.37);  //--- mm



 TCanvas* cc = new TCanvas("cc","cc",800,600);
 h_mWW_1 -> Draw();
 h_mWW_2 -> Draw("same");


 for (int iBin = 0; iBin < h_mWW_1->GetNbinsX(); iBin++) {
  float num = h_mWW_2->GetBinContent(iBin+1);
  float den = h_mWW_1->GetBinContent(iBin+1);
  if (den != 0) h_Ratio -> SetBinContent (iBin+1, num / den);
  else  h_Ratio -> SetBinContent (iBin+1, 0);

  h_Subtraction ->  SetBinContent (iBin+1, num - den);

 }

 TCanvas* cc_Ratio = new TCanvas("cc_Ratio","cc_Ratio",800,600);
 h_Ratio->SetLineColor(kGreen);
 h_Ratio->SetLineStyle(1);
 h_Ratio->SetLineWidth(2);
 h_Ratio -> Draw();

 TCanvas* cc_Subtraction = new TCanvas("cc_Subtraction","cc_Subtraction",800,600);
 h_Subtraction->GetYaxis()->SetRangeUser(-10,5000);
 h_Subtraction->SetLineColor(kMagenta);
 h_Subtraction->SetLineStyle(1);
 h_Subtraction->SetLineWidth(2);
 h_Subtraction -> Draw();


}


