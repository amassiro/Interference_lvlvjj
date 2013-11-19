#include "TString.h"

//           0 = em, 1 = mm
void Draw(int kind = 0,         int mass = 350) {

//  TFile* f1 = new TFile ("gen_126_jjmm.root","READ"); // ---- B
//  TFile* f2 = new TFile ("gen_500_jjmm.root","READ"); // ---- S+B

 int NBIN = 200;
 if (mass>500) NBIN = 100;
 if (mass>700) NBIN =  90;
 if (mass>900) NBIN =  40;

 int MAX = 2000;
 if (mass>500) MAX =  2000;
 if (mass>700) MAX =  2000;
 if (mass>900) MAX =  2000;


 TString name1;
 if (kind == 0) name1 = Form ("gen_126_jjme.root");
 else           name1 = Form ("gen_126_jjmm.root");

 TString name2;
 if (kind == 0) name2 = Form ("gen_%d_jjme.root",mass);
 else           name2 = Form ("gen_%d_jjmm.root",mass);

 TString name3;
 if (kind == 0) name3 = Form ("S_mH%d_jjme.root",mass);
 else           name3 = Form ("S_mH%d_jjmm.root",mass);

 TFile* f1 = new TFile (name1.Data(),"READ"); // ---- B
 TFile* f2 = new TFile (name2.Data(),"READ"); // ---- S+B
 TFile* f3 = new TFile (name3.Data(),"READ"); // ---- S

 std::cout << " nameB  = " << name1.Data() << std::endl;
 std::cout << " nameSB = " << name2.Data() << std::endl;
 std::cout << " nameS  = " << name3.Data() << std::endl;

 TNtuple* t1 = (TNtuple*) f1->Get ("ntu");
 TNtuple* t2 = (TNtuple*) f2->Get ("ntu");
 TNtuple* t3 = (TNtuple*) f3->Get ("ntu");

 TH1F* h_mWW_1 = new TH1F("h_mWW_1","h_mWW_1",NBIN,0,MAX);
 TH1F* h_mWW_2 = new TH1F("h_mWW_2","h_mWW_2",NBIN,0,MAX);
 TH1F* h_mWW_3 = new TH1F("h_mWW_3","h_mWW_3",NBIN,0,MAX);
 TH1F* h_Ratio = new TH1F("h_mWW_ratio","h_mWW_ratio",NBIN,0,MAX);
 TH1F* h_Subtraction = new TH1F("h_Subtraction","h_Subtraction",NBIN,0,MAX);

 // me                        126                  350                      500                      650                      800                      1000
//  float xsec[100] = {3.59354104959999920E-002, 3.30375324117999983E-002, 3.08369319673999914E-002, 3.04300778839999946E-002, 2.96306891660000002E-002, 2.96035751339999986E-002};
 // mm                        126                  350                      500                      650                      800                      1000
//  float xsec[100] = {1.81489087459999997E-002, 1.69895616909999971E-002, 1.56710346175999993E-002, 1.54292870582000020E-002, 1.50044308354000018E-002, 1.49635804075999978E-002};

 //---- me
 if (kind == 0) gROOT->ProcessLine ("float xsec[100] = {3.59354104959999920E-002, 3.30375324117999983E-002, 3.08369319673999914E-002, 3.04300778839999946E-002, 2.96306891660000002E-002, 2.96035751339999986E-002};");
 //---- mm
 if (kind == 1) gROOT->ProcessLine ("float xsec[100] = {1.81489087459999997E-002, 1.69895616909999971E-002, 1.56710346175999993E-002, 1.54292870582000020E-002, 1.50044308354000018E-002, 1.49635804075999978E-002};");

 //---- me
 if (kind == 0) gROOT->ProcessLine ("float xsec_S[100] = {0.23539E-01, 0.16986E-01, 0.65801E-02, 0.35875E-02, 0.20252E-02, 0.91164E-03};");
 //---- mm
 if (kind == 1) gROOT->ProcessLine ("float xsec_S[100] = {0.11912E-01, 0.99646E-02, 0.38685E-02, 0.20920E-02, 0.11599E-02, 0.50136E-03};");


 //  350  w1
 //  500  w2
 //  650  w3
 //  800  w4
 // 1000  w5

 TString weight;
 if (mass ==  350) weight = Form ("w1");
 if (mass ==  500) weight = Form ("w2");
 if (mass ==  650) weight = Form ("w3");
 if (mass ==  800) weight = Form ("w4");
 if (mass == 1000) weight = Form ("w5");

 float xsecToUse;
 if (mass ==  350) xsecToUse = xsec[1];
 if (mass ==  500) xsecToUse = xsec[2];
 if (mass ==  650) xsecToUse = xsec[3];
 if (mass ==  800) xsecToUse = xsec[4];
 if (mass == 1000) xsecToUse = xsec[5];

 float xsecToUse_S;
 if (mass ==  350) xsecToUse_S = xsec_S[1];
 if (mass ==  500) xsecToUse_S = xsec_S[2];
 if (mass ==  650) xsecToUse_S = xsec_S[3];
 if (mass ==  800) xsecToUse_S = xsec_S[4];
 if (mass == 1000) xsecToUse_S = xsec_S[5];


//  TString weightWithXsec126 = Form ("(mll>80 && mll<100 ) * %s * %f",weight.Data(),xsec[0]);
//  TString weightWithXsec    = Form ("(mll>80 && mll<100 ) * %f",xsecToUse);

 TString weightWithXsec126 = Form ("%s * %f",weight.Data(),xsec[0]);
 TString weightWithXsec    = Form ("%f",xsecToUse);

 TString weightWithXsec_S  = Form ("%f",xsecToUse_S);

 std::cout << " weightWithXsec126 = " << weightWithXsec126.Data() << std::endl;
 std::cout << " weightWithXsec    = " << weightWithXsec.Data()    << std::endl;
 std::cout << " weightWithXsec_S  = " << weightWithXsec_S.Data()  << std::endl;

 t1->Draw("mWW >> h_mWW_1",weightWithXsec126.Data(),"goff");
 t2->Draw("mWW >> h_mWW_2",weightWithXsec.Data(),   "goff");
 t3->Draw("mWW >> h_mWW_3",weightWithXsec_S.Data(), "goff");

 h_mWW_1->SetLineColor(kBlue);
 h_mWW_2->SetLineColor(kRed);
 h_mWW_3->SetLineColor(kGreen);

 h_mWW_1->SetLineStyle(1);
 h_mWW_2->SetLineStyle(2);
 h_mWW_3->SetLineStyle(2);

 h_mWW_1->SetLineWidth(2);
 h_mWW_2->SetLineWidth(2);
 h_mWW_3->SetLineWidth(3);

//  h_mWW_1->Scale (1.2);  //--- me
//  h_mWW_1->Scale (0.37);  //--- mm



 TCanvas* cc = new TCanvas("cc","cc",800,600);
 h_mWW_1 -> Draw();
 h_mWW_2 -> Draw("same");
 h_mWW_3 -> Draw("same");


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
//  h_Subtraction->GetYaxis()->SetRangeUser(-10,5000);
 h_Subtraction->GetYaxis()->SetRangeUser(0.001,5000);
 h_Subtraction->SetLineColor(kMagenta);
 h_Subtraction->SetLineStyle(1);
 h_Subtraction->SetLineWidth(2);
 h_Subtraction -> Draw();
 h_mWW_3 -> Draw("same");
 cc_Subtraction->SetGrid();

}


