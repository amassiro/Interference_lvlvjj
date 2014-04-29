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

 xsec_B = 3.59354104959999920E-002 / 2.; //---- B (126)

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
 t_S_cp   -> Draw("mWW >> h_mWW_S_cp",weight_S_cp.Data(),"goff");
 t_SBI_bw -> Draw("mWW >> h_mWW_SBI_bw",weight_SBI_bw.Data(),"goff");
 t_S_bw   -> Draw("mWW >> h_mWW_S_bw",weight_S_bw.Data(),"goff");

 h_mWW_B->SetLineColor(kGreen);

 h_mWW_SBI_cp->SetLineColor(kBlue);
 h_mWW_S_cp->SetLineColor(kRed);
 h_mWW_SBI_bw->SetLineColor(kAzure);
 h_mWW_S_bw->SetLineColor(kOrange);

 h_mWW_B->SetLineStyle(1);

 h_mWW_SBI_cp->SetLineStyle(2);
 h_mWW_S_cp->SetLineStyle(2);
 h_mWW_SBI_bw->SetLineStyle(3);
 h_mWW_S_bw->SetLineStyle(3);

 h_mWW_B->SetLineWidth(3);
 h_mWW_SBI_cp->SetLineWidth(2);
 h_mWW_S_cp->SetLineWidth(2);
 h_mWW_SBI_bw->SetLineWidth(2);
 h_mWW_S_bw->SetLineWidth(2);



 TGraphAsymmErrors* gr_SBI_ratio = new TGraphAsymmErrors();
 gr_SBI_ratio->SetFillColor (kBlue);
 gr_SBI_ratio->SetLineColor (kBlue);
 gr_SBI_ratio->SetMarkerColor (kBlue);
 gr_SBI_ratio->SetMarkerSize (1);
 gr_SBI_ratio->SetLineWidth (2);
 gr_SBI_ratio->SetLineStyle (1);
 gr_SBI_ratio->SetFillStyle (3001);
 gr_SBI_ratio->SetMarkerStyle (20);

 for (int iMass = 0; iMass < h_mWW_SBI_cp->GetNbinsX(); iMass++) {
  double errYhi = 0;
  double errYlo = 0;
  double X = h_mWW_SBI_cp->GetXaxis()->GetBinCenter(iMass+1);
  double Y1 = h_mWW_SBI_cp->GetBinContent(iMass+1);
  double Y2 = h_mWW_SBI_bw->GetBinContent(iMass+1);
  if (Y2 != 0) {
   Y1 /= Y2;
  }
  else {
   Y1 = 1.;
  }
  double errXhi = 0;
  double errXlo = 0;  
  gr_SBI_ratio -> SetPoint      (iMass, X, Y1);
  gr_SBI_ratio -> SetPointError (iMass, errXlo, errXhi, errYlo, errYhi);
 }


 TGraphAsymmErrors* gr_S_ratio = new TGraphAsymmErrors();
 gr_S_ratio->SetFillColor (kRed);
 gr_S_ratio->SetLineColor (kRed);
 gr_S_ratio->SetMarkerColor (kRed);
 gr_S_ratio->SetMarkerSize (1);
 gr_S_ratio->SetLineWidth (2);
 gr_S_ratio->SetLineStyle (2);
 gr_S_ratio->SetFillStyle (3001);
 gr_S_ratio->SetMarkerStyle (20);

 for (int iMass = 0; iMass < h_mWW_S_cp->GetNbinsX(); iMass++) {
  double errYhi = 0;
  double errYlo = 0;
  double X = h_mWW_S_cp->GetXaxis()->GetBinCenter(iMass+1);
  double Y1 = h_mWW_S_cp->GetBinContent(iMass+1);
  double Y2 = h_mWW_S_bw->GetBinContent(iMass+1);
  if (Y2 != 0) {
   Y1 /= Y2;
  }
  else {
   Y1 = 1.;
  }
  double errXhi = 0;
  double errXlo = 0;
  gr_S_ratio -> SetPoint      (iMass, X, Y1);
  gr_S_ratio -> SetPointError (iMass, errXlo, errXhi, errYlo, errYhi);
 }

 TCanvas* cc = new TCanvas("cc","cc",800,600);
 cc->Divide(1,2);
 cc->cd(1);
 h_mWW_B -> Draw();
 h_mWW_SBI_cp -> Draw("same");
 h_mWW_S_cp -> Draw("same");
 h_mWW_SBI_bw -> Draw("same");
 h_mWW_S_bw -> Draw("same");

 TLegend* leg_SI = new TLegend (0.5,0.5,0.9,0.9);
 leg_SI -> SetFillColor(0);
 leg_SI -> AddEntry(h_mWW_B,       "B", "L");
 leg_SI -> AddEntry(h_mWW_SBI_cp,  "SBI cp",  "L");
 leg_SI -> AddEntry(h_mWW_S_cp,    "S cp",  "L");
 leg_SI -> AddEntry(h_mWW_SBI_bw,  "SBI bw",  "L");
 leg_SI -> AddEntry(h_mWW_S_bw,    "S bw",  "L");
 leg_SI -> Draw();

 cc->cd(2);
 gr_SBI_ratio->Draw();
 gr_S_ratio->Draw("same");
 gPad->SetGrid();


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

 TGraphAsymmErrors* gr_weight_ratio = new TGraphAsymmErrors();
 gr_weight_ratio->SetFillColor (kRed);
 gr_weight_ratio->SetLineColor (kRed);
 gr_weight_ratio->SetMarkerColor (kRed);
 gr_weight_ratio->SetMarkerSize (1);
 gr_weight_ratio->SetLineWidth (2);
 gr_weight_ratio->SetLineStyle (2);
 gr_weight_ratio->SetFillStyle (3001);
 gr_weight_ratio->SetMarkerStyle (20);

 for (int iMass = 0; iMass < h_weight_cp->GetNbinsX(); iMass++) {
  double errYhi = 0;
  double errYlo = 0;
  double X = h_weight_cp->GetXaxis()->GetBinCenter(iMass+1);
  double Y1 = h_weight_cp->GetBinContent(iMass+1);
  double Y2 = h_weight_bw->GetBinContent(iMass+1);
  if (Y2 != 0) {
   Y1 /= Y2;
  }
  else {
   Y1 = 1.;
  }
  double errXhi = 0;
  double errXlo = 0;
  gr_weight_ratio -> SetPoint      (iMass, X, Y1);
  gr_weight_ratio -> SetPointError (iMass, errXlo, errXhi, errYlo, errYhi);
 }

 TCanvas* cc_weight = new TCanvas("cc_weight","cc_weight",800,600);
 cc_weight->Divide(1,2);

 cc_weight->cd(1);
 h_weight_cp->SetLineColor(kGreen);
 h_weight_cp->SetLineStyle(1);
 h_weight_cp->SetLineWidth(2);

 h_weight_bw->SetLineColor(kRed);
 h_weight_bw->SetLineStyle(2);
 h_weight_bw->SetLineWidth(2);

 h_weight_bw -> Draw();
 h_weight_cp -> Draw("same");

 TLegend* leg_weight = new TLegend (0.5,0.5,0.9,0.9);
 leg_weight -> SetFillColor(0);
 leg_weight -> AddEntry(h_weight_cp,  "cp", "L");
 leg_weight -> AddEntry(h_weight_bw,  "bw", "L");
 leg_weight -> Draw();

 cc_weight->cd(2);
 gr_weight_ratio->Draw("APL");
 gPad->SetGrid();

}



