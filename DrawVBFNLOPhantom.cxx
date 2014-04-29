#include "TString.h"

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


//           0 = em, 1 = mm
void DrawVBFNLOPhantom() {

 int mass = 800;


 int NBIN = 100; //---- 120

 int MIN = 200;
 int MAX =  2000; //---- 4000


 TString name_B;
 name_B = Form ("gen_126_jjme.root");

 TString name_SBI_cp;
 name_SBI_cp = Form ("CPS/2jemuvv_800/genh800_cp.root");

 TString name_S_cp;
 name_S_cp = Form ("CPS/2jemuvv_800/genh800_cp_sig.root");

 TString name_SBI_bw;
 name_SBI_bw = Form ("CPS/2jemuvv_800/genh800.root");

 TString name_S_bw;
 name_S_bw = Form ("CPS/2jemuvv_800/genh800_sig.root");

 TFile* f_B      = new TFile (name_B.Data(),"READ");
 TFile* f_SBI_cp = new TFile (name_SBI_cp.Data(),"READ");
 TFile* f_S_cp   = new TFile (name_S_cp.Data(),"READ");
 TFile* f_SBI_bw = new TFile (name_SBI_bw.Data(),"READ");
 TFile* f_S_bw   = new TFile (name_S_bw.Data(),"READ");

 TNtuple* t_B = (TNtuple*) f_B->Get ("ntu");
 TNtuple* t_SBI_cp = (TNtuple*) f_SBI_cp->Get ("ntu");
 TNtuple* t_S_cp   = (TNtuple*) f_S_cp->Get ("ntu");
 TNtuple* t_SBI_bw = (TNtuple*) f_SBI_bw->Get ("ntu");
 TNtuple* t_S_bw   = (TNtuple*) f_S_bw->Get ("ntu");

 TH1F* h_mWW_B = new TH1F("h_mWW_B","h_mWW_B",NBIN,0,MAX);
 TH1F* h_mWW_SBI_cp = new TH1F("h_mWW_SBI_cp","h_mWW_SBI_cp",NBIN,0,MAX);
 TH1F* h_mWW_S_cp   = new TH1F("h_mWW_S_cp","h_mWW_S_cp",NBIN,0,MAX);
 TH1F* h_mWW_SBI_bw = new TH1F("h_mWW_SBI_bw","h_mWW_SBI_bw",NBIN,0,MAX);
 TH1F* h_mWW_S_bw   = new TH1F("h_mWW_S_bw","h_mWW_S_bw",NBIN,0,MAX);

 TH1F* h_weight_cp = new TH1F("h_weight_cp","h_weight_cp",NBIN,0,MAX);
 TH1F* h_weight_bw = new TH1F("h_weight_bw","h_weight_bw",NBIN,0,MAX);

 float xsec_B;
 float xsec_SBI_cp;
 float xsec_S_cp;
 float xsec_SBI_bw;
 float xsec_S_bw;

 xsec_B = 3.59354104959999920E-002; //---- B (126)

 //---- cp
 xsec_SBI_cp = 3.735387534799999E-003;
 xsec_S_cp = 2.608006521936799E-004;

 //---- bw
 xsec_SBI_bw = 3.663679731200000E-003;
 xsec_S_bw = 1.929557442245400E-004;



 //  350  w1
 //  500  w2
 //  650  w3
 //  800  w4
 // 1000  w5
 //  250  w6
 //  300  w7


 TString weight;
 if (mass ==  350) weight = Form ("w1");
 if (mass ==  500) weight = Form ("w2");
 if (mass ==  650) weight = Form ("w3");
 if (mass ==  800) weight = Form ("w4");
 if (mass == 1000) weight = Form ("w5");
 if (mass ==  250) weight = Form ("w6");
 if (mass ==  300) weight = Form ("w7");

//  float xsecToUse;
//  if (mass ==  250) xsecToUse = xsec[1]/2.;
//  if (mass ==  300) xsecToUse = xsec[2]/2.;
//  if (mass ==  350) xsecToUse = xsec[3]/2.;
//  if (mass ==  500) xsecToUse = xsec[4]/2.;
//  if (mass ==  650) xsecToUse = xsec[5]/2.;
//  if (mass ==  800) xsecToUse = xsec[6]/2.;
//  if (mass == 1000) xsecToUse = xsec[7]/2.;
// 
//  float xsecToUse_S;
//  if (mass ==  250) xsecToUse_S = xsec_S[1]/0.5;
//  if (mass ==  300) xsecToUse_S = xsec_S[2]/0.5;
//  if (mass ==  350) xsecToUse_S = xsec_S[3]/0.5;
//  if (mass ==  500) xsecToUse_S = xsec_S[4]/0.5;
//  if (mass ==  650) xsecToUse_S = xsec_S[5]/0.5;
//  if (mass ==  800) xsecToUse_S = xsec_S[6]/0.5;
//  if (mass == 1000) xsecToUse_S = xsec_S[7]/0.5;


 //---- used:
//  TString cut = Form ("mjj>30 && pt1>5 && pt2>5 && jetpt1>10 && jetpt2>10");

 //---- to compare with MC@NLO
 TString cut = Form ("mjj>200 && pt2>8");


 TString weight_B = Form ("(%s) * (%s * %f) * (numt == 0) * (numb == 0)",cut.Data(),weight.Data(),xsec_B);
 TString weight_SBI_cp = Form ("(%s) * (%s * %f) * (numt == 0) * (numb == 0)",cut.Data(),weight.Data(),xsec_SBI_cp);
 TString weight_S_cp   = Form ("(%s) * (%s * %f) * (numt == 0) * (numb == 0)",cut.Data(),weight.Data(),xsec_S_cp);
 TString weight_SBI_bw = Form ("(%s) * (%s * %f) * (numt == 0) * (numb == 0)",cut.Data(),weight.Data(),xsec_SBI_bw);
 TString weight_S_bw   = Form ("(%s) * (%s * %f) * (numt == 0) * (numb == 0)",cut.Data(),weight.Data(),xsec_S_bw);

 t_B -> Draw("mWW >> h_mWW_B",weight_B.Data(),"goff");
 t_SBI_cp -> Draw("mWW >> h_mWW_SBI_cp",weight_SBI_cp.Data(),"goff");
 t_B_cp   -> Draw("mWW >> h_mWW_B_cp",weight_B_cp.Data(),"goff");
 t_SBI_bw -> Draw("mWW >> h_mWW_SBI_bw",weight_SBI_bw.Data(),"goff");
 t_B_bw   -> Draw("mWW >> h_mWW_B_bw",weight_B_bw.Data(),"goff");

 h_mWW_B->SetLineColor(kGreen);

 h_mWW_SBI_cp->SetLineColor(kBlue);
 h_mWW_B_cp->SetLineColor(kRed);
 h_mWW_SBI_bw->SetLineColor(kBlue+2);
 h_mWW_B_bw->SetLineColor(kRed+2);

 h_mWW_B->SetLineStyle(1);

 h_mWW_SBI_cp->SetLineStyle(2);
 h_mWW_B_cp->SetLineStyle(2);
 h_mWW_SBI_bw->SetLineStyle(3);
 h_mWW_B_bw->SetLineStyle(3);

 h_mWW_B->SetLineWidth(3);
 h_mWW_SBI_cp->SetLineWidth(2);
 h_mWW_B_cp->SetLineWidth(2);
 h_mWW_SBI_bw->SetLineWidth(2);
 h_mWW_B_bw->SetLineWidth(2);


 TCanvas* cc = new TCanvas("cc","cc",800,600);
 h_mWW_B -> Draw();
 h_mWW_SBI_cp -> Draw("same");
 h_mWW_B_cp -> Draw("same");
 h_mWW_SBI_bw -> Draw("same");
 h_mWW_B_bw -> Draw("same");


 for (int iBin = 0; iBin < h_mWW_B->GetNbinsX(); iBin++) {
  float sbi = h_mWW_SBI_cp->GetBinContent(iBin+1);
  float s   = h_mWW_S_cp->GetBinContent(iBin+1);
  float b   = h_mWW_B->GetBinContent(iBin+1);
  if (s != 0) h_weight_cp -> SetBinContent (iBin+1, (sbi - b) / s);
  else  h_weight_cp -> SetBinContent (iBin+1, 0);
 }

 for (int iBin = 0; iBin < h_mWW_B->GetNbinsX(); iBin++) {
  float sbi = h_mWW_SBI_bw->GetBinContent(iBin+1);
  float s   = h_mWW_S_bw->GetBinContent(iBin+1);
  float b   = h_mWW_B->GetBinContent(iBin+1);
  if (s != 0) h_weight_bw -> SetBinContent (iBin+1, (sbi - b) / s);
  else  h_weight_bw -> SetBinContent (iBin+1, 0);
 }


 TCanvas* cc_Ratio = new TCanvas("cc_Ratio","cc_Ratio",800,600);
 h_weight_cp->SetLineColor(kGreen);
 h_weight_cp->SetLineStyle(1);
 h_weight_cp->SetLineWidth(2);

 h_weight_bw->SetLineColor(kRed);
 h_weight_bw->SetLineStyle(2);
 h_weight_bw->SetLineWidth(2);

 h_Ratio -> Draw();

 TLegend* leg_SI = new TLegend (0.5,0.5,0.9,0.9);
 leg_SI -> SetFillColor(0);
 leg_SI -> AddEntry(h_mWW_B,       "B", "L");
 leg_SI -> AddEntry(h_mWW_SBI_cp,  "SBI cp",  "L");
 leg_SI -> AddEntry(h_mWW_S_cp,    "S cp",  "L");
 leg_SI -> AddEntry(h_mWW_SBI_bw,  "SBI bw",  "L");
 leg_SI -> AddEntry(h_mWW_S_bw,    "S bw",  "L");
 leg_SI -> Draw();


}



