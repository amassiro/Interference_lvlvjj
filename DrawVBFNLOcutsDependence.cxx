#include "TString.h"



//           0 = em, 1 = mm
void DrawVBFNLOcutsDependence(int kind = 0,         int mass = 350,   bool doFit = 1,     int scaleVariation = 0) {

//  scaleVariation = 0   nominal
//                  -1   scale down
//                   1   scale up

 std::string folder;
 if (scaleVariation ==  0) folder = "./";
 if (scaleVariation == -1) folder = "scaleDown/";
 if (scaleVariation ==  1) folder = "scaleUp/";

//  TFile* f1 = new TFile ("gen_126_jjmm.root","READ"); // ---- B
//  TFile* f2 = new TFile ("gen_500_jjmm.root","READ"); // ---- S+B

//  int NBIN = 350;
 int NBIN = 500;
 if (mass<350) NBIN = 500;
 if (mass>400) NBIN = 120;
 if (mass>500) NBIN =  70;
 if (mass>700) NBIN = 100; //---- 120
 if (mass>900) NBIN =  80;

 int MAX = 800;
 if (mass<350) MAX =   500;
 if (mass>400) MAX =  1500;
 if (mass>500) MAX =  2000;
 if (mass>700) MAX =  2000; //---- 4000
 if (mass>900) MAX =  4000;


 int MIN = 200;
 if (mass<350) MIN = 200;


 TString name1;
 if (kind == 0) name1 = Form ("%sgen_126_jjme.root",folder.c_str());
 else           name1 = Form ("%sgen_126_jjmm.root",folder.c_str());

 TString name2;
 if (kind == 0) name2 = Form ("%sgen_%d_jjme.root",folder.c_str(),mass);
 else           name2 = Form ("%sgen_%d_jjmm.root",folder.c_str(),mass);

 TString name3;
 if (kind == 0) name3 = Form ("%sS_mH%d_jjme.root",folder.c_str(),mass);
 else           name3 = Form ("%sS_mH%d_jjmm.root",folder.c_str(),mass);

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
 TH1F* h_Subtraction = new TH1F("h_Subtraction","h_Subtraction",NBIN,0,MAX);
 TH1F* h_I = new TH1F("h_I","h_I",NBIN,0,MAX);

 TH1F* h_Ratio[10];
 for (int i=0; i<10; i++) {
  TString name = Form ("ratio_SI_S_%d",i);
  h_Ratio[i] = new TH1F(name.Data(),"",NBIN,0,MAX);
 }

 // me                        126                  350                      500                      650                      800                      1000
//  float xsec[100] = {3.59354104959999920E-002, 3.30375324117999983E-002, 3.08369319673999914E-002, 3.04300778839999946E-002, 2.96306891660000002E-002, 2.96035751339999986E-002};
 // mm                        126                  350                      500                      650                      800                      1000
//  float xsec[100] = {1.81489087459999997E-002, 1.69895616909999971E-002, 1.56710346175999993E-002, 1.54292870582000020E-002, 1.50044308354000018E-002, 1.49635804075999978E-002};

 //---- me
 if (kind == 0) gROOT->ProcessLine ("float xsec[100] = {3.59354104959999920E-002, 3.67919577779999979E-002, 3.44869588220000078E-002, 3.30375324117999983E-002, 3.08369319673999914E-002, 3.04300778839999946E-002, 2.96306891660000002E-002, 2.96035751339999986E-002};");
 //---- mm                                                  126                           250                             300                   350                        500                     650               800        1000
 if (kind == 1) gROOT->ProcessLine ("float xsec[100] = {1.81489087459999997E-002, 1.38655337210000004E-002, 1.23882828156000029E-002, 1.69895616909999971E-002, 1.56710346175999993E-002, 1.54292870582000020E-002, 1.50044308354000018E-002, 1.49635804075999978E-002};");

 //---- me
//  if (kind == 0) gROOT->ProcessLine ("float xsec_S[100] = {1.0, 1.0, 0.23539E-01, 0.16986E-01, 0.65801E-02, 0.35875E-02, 0.20252E-02, 0.91164E-03};");
 //---- mm
//  if (kind == 1) gROOT->ProcessLine ("float xsec_S[100] = {1.0, 1.0, 0.11912E-01, 0.99646E-02, 0.38685E-02, 0.20920E-02, 0.11599E-02, 0.50136E-03};");

 //---- me                                                  126          250          300          350              500      650           800        1000
 if (kind == 0) gROOT->ProcessLine ("float xsec_S[100] = {0.80433E-02, 0.78312E-02, 0.52913E-02, 0.36612E-02, 0.10750E-02, 0.45337E-03, 0.19850E-03, 0.62934E-04};");
 //---- mm
 if (kind == 1) gROOT->ProcessLine ("float xsec_S[100] = {0.40555E-02, 0.45444E-02, 0.30896E-02, 0.21460E-02, 0.63336E-03, 0.26665E-03, 0.11531E-03, 0.35402E-04};");



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

 float xsecToUse;
 if (mass ==  250) xsecToUse = xsec[1]/2.;
 if (mass ==  300) xsecToUse = xsec[2]/2.;
 if (mass ==  350) xsecToUse = xsec[3]/2.;
 if (mass ==  500) xsecToUse = xsec[4]/2.;
 if (mass ==  650) xsecToUse = xsec[5]/2.;
 if (mass ==  800) xsecToUse = xsec[6]/2.;
 if (mass == 1000) xsecToUse = xsec[7]/2.;

 float xsecToUse_S;
 if (mass ==  250) xsecToUse_S = xsec_S[1]/0.5;
 if (mass ==  300) xsecToUse_S = xsec_S[2]/0.5;
 if (mass ==  350) xsecToUse_S = xsec_S[3]/0.5;
 if (mass ==  500) xsecToUse_S = xsec_S[4]/0.5;
 if (mass ==  650) xsecToUse_S = xsec_S[5]/0.5;
 if (mass ==  800) xsecToUse_S = xsec_S[6]/0.5;
 if (mass == 1000) xsecToUse_S = xsec_S[7]/0.5;

 //---- used:
 TString vcut[10];
 vcut[0] = Form ("mjj>200 && pt2>8");
 vcut[1] = Form ("mjj>300 && pt2>8");
 vcut[2] = Form ("mjj>500 && pt2>8");
 vcut[3] = Form ("mjj>1000 && pt2>8");
 vcut[4] = Form ("mjj>200 && detajj>3.5 && pt2>8");
 vcut[5] = Form ("mjj>500 && detajj>3.5 && pt2>8");
 vcut[6] = Form ("mjj>1000 && detajj>3.5 && pt2>8");
 vcut[7] = Form ("mjj>100 && pt2>8");
 vcut[8] = Form ("mjj>50 && pt2>8");

 int maxVector = 7+2;

 TLegend* leg_B = new TLegend (0.5,0.5,0.9,0.9);
 leg_B -> SetFillColor(0);

 for (int i=0; i<maxVector; i++) {
  TString cut = vcut[i];

//  TString weightWithXsec126 = Form ("(%s) * (%s * %f)",cut.Data(),weight.Data(),xsec[0]/2.);
  TString weightWithXsec126 = Form ("(%s) * (%s * %f) * (numt == 0) * (numb == 0)",cut.Data(),weight.Data(),xsec[0]/2.);
  TString weightWithXsec    ;
//  if (scaleVariation == 0) weightWithXsec    = Form ("(%s) * (%f)",cut.Data(),xsecToUse);
//  else                     weightWithXsec    = Form ("(%s) * (%s) * (%f)",weight.Data(), cut.Data(),xsecToUse);
  if (scaleVariation == 0) weightWithXsec    = Form ("(%s) * (%f) * (numb == 0) * (numt == 0)",cut.Data(),xsecToUse);
  else                     weightWithXsec    = Form ("(%s) * (%s) * (%f) * (numb == 0) * (numt == 0)",weight.Data(), cut.Data(),xsecToUse);

  TString weightWithXsec_S  ;
  if (scaleVariation == 0) weightWithXsec_S  = Form ("(%s) * (%f)",cut.Data(),xsecToUse_S);
  else                     weightWithXsec_S  = Form ("(%s) * (%s) * (%f)",weight.Data(), cut.Data(),xsecToUse_S);

  std::cout << " weightWithXsec126 = " << weightWithXsec126.Data() << std::endl;
  std::cout << " weightWithXsec    = " << weightWithXsec.Data()    << std::endl;
  std::cout << " weightWithXsec_S  = " << weightWithXsec_S.Data()  << std::endl;

//  t1->Draw("mjj >> h_mWW_1",weightWithXsec126.Data(),"goff");
//  t2->Draw("mjj >> h_mWW_2",weightWithXsec.Data(),   "goff");
//  t3->Draw("mjj >> h_mWW_3",weightWithXsec_S.Data(), "goff");

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


  for (int iBin = 0; iBin < h_mWW_1->GetNbinsX(); iBin++) {
   float num = h_mWW_2->GetBinContent(iBin+1);
   float den = h_mWW_1->GetBinContent(iBin+1);
   float S = h_mWW_3->GetBinContent(iBin+1);

//    if (S != 0) h_Ratio[i] -> SetBinContent (iBin+1, num - den);
//    else  h_Ratio[i] -> SetBinContent (iBin+1, 0);
   if (S != 0) h_Ratio[i] -> SetBinContent (iBin+1, (num - den) / S);
   else  h_Ratio[i] -> SetBinContent (iBin+1, 0);
  }

//   h_Ratio[i]->SetLineColor(kMagenta+i);
  if (i<4) h_Ratio[i]->SetLineColor(kViolet+i);
  else h_Ratio[i]->SetLineColor(kGreen+i-4);

  leg_B -> AddEntry(h_Ratio[i],    cut.Data(), "L");

 }


 TCanvas* cc_Correction = new TCanvas("cc_Correction","cc_Correction",800,600);
 for (int i=0; i<maxVector; i++) {
  h_Ratio[i]->GetYaxis()->SetRangeUser(0.001,h_Ratio[i]->GetMaximum()*1.1);
  h_Ratio[i]->SetLineStyle(1);
  h_Ratio[i]->SetLineWidth(2);
  if (i==0) h_Ratio[i] -> Draw();
  else  h_Ratio[i] -> Draw("same");
 }
 leg_B -> Draw();
 cc_Correction->SetGrid();


}



