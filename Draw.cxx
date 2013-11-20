#include "TString.h"



double doubleGausCrystalBallLowHigh (double* x, double* par) {
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  //[5] = alpha2
  //[6] = n2

 double xx = x[0];
 double mean = par[1] ; // mean
 double sigmaP = par[2] ; // sigma of the positive side of the gaussian
 double sigmaN = par[3] ; // sigma of the negative side of the gaussian
 double alpha = par[4] ; // junction point on the positive side of the gaussian
 double n = par[5] ; // power of the power law on the positive side of the gaussian
 double alpha2 = par[6] ; // junction point on the negative side of the gaussian
 double n2 = par[7] ; // power of the power law on the negative side of the gaussian

 if ((xx-mean)/sigmaP > fabs(alpha)) {
  double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
  double B = n/fabs(alpha) - fabs(alpha);
    
  return par[0] * A * pow(B + (xx-mean)/sigmaP, -1.*n);
 }
  
 else if ((xx-mean)/sigmaN < -1.*fabs(alpha2)) {
  double A = pow(n2/fabs(alpha2), n2) * exp(-0.5 * alpha2*alpha2);
  double B = n2/fabs(alpha2) - fabs(alpha2);
    
  return par[0] * A * pow(B - (xx-mean)/sigmaN, -1.*n2);
 }
  
 else if ((xx-mean) > 0) {
  return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigmaP*sigmaP) );
 }
  
 else {
  return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigmaN*sigmaN) );
 }
  
}




//Crystal ball function for signal, parameters are 0:alpha,1:n,2:mean,3:sigma,4:normalization;

Double_t CrystalBall(Double_t *x,Double_t *par) {

 Double_t t = (x[0]-par[2])/par[3];
 if (par[0] < 0) t = -t;

 Double_t absAlpha = fabs((Double_t)par[0]);

 if (t >= -absAlpha) {
  return par[4]*exp(-0.5*t*t);
 }
 else {
  Double_t a =  TMath::Power(par[1]/absAlpha,par[1])*exp(-0.5*absAlpha*absAlpha);
  Double_t b= par[1]/absAlpha - absAlpha;

  return par[4]*(a/TMath::Power(b - t, par[1]));
 }
}


Double_t RightCrystalBall(Double_t *x,Double_t *par) {

 Double_t t = (x[0]-par[2])/par[3];
 if (par[0] > 0) t = -t;

 Double_t absAlpha = fabs((Double_t)par[0]);

 if (t >= -absAlpha) {
  return par[4]*exp(-0.5*t*t);
 }
 else {
  Double_t a =  TMath::Power(par[1]/absAlpha,par[1])*exp(-0.5*absAlpha*absAlpha);
  Double_t b= par[1]/absAlpha - absAlpha;

  return par[4]*(a/TMath::Power(b - t, par[1]));
 }
}


//Superposition of 2 gaussians

Double_t G1(Double_t *x, Double_t *par) {
 Double_t arg = 0;
 if (par[2]) arg = (x[0] - par[1])/par[2];

 Double_t sig = par[0]*TMath::Exp(-0.5*arg*arg);
 return sig;
}


Double_t G2(Double_t *x, Double_t *par) {
 Double_t arg = 0;
 if (par[2]) arg = (x[0] - par[1])/par[2];

 Double_t sig = par[0]*TMath::Exp(-0.5*arg*arg);
 return sig;
}


Double_t Total(Double_t *x, Double_t *par) {
 Double_t tot = G1(x,par) + G2(x,&par[3]);
 return tot;
}



//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


//           0 = em, 1 = mm
void Draw(int kind = 0,         int mass = 350) {

//  TFile* f1 = new TFile ("gen_126_jjmm.root","READ"); // ---- B
//  TFile* f2 = new TFile ("gen_500_jjmm.root","READ"); // ---- S+B

 int NBIN = 150;
 if (mass>500) NBIN =  70;
 if (mass>700) NBIN =  60;
 if (mass>900) NBIN =  40;

 int MAX = 500;
 if (mass>400) MAX =  1000;
 if (mass>500) MAX =  2000;
 if (mass>700) MAX =  2000;
 if (mass>900) MAX =  4000;


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
 h_Subtraction->GetYaxis()->SetRangeUser(0.001,h_Subtraction->GetMaximum()*2.5);
 h_Subtraction->SetLineColor(kMagenta);
 h_Subtraction->SetLineStyle(1);
 h_Subtraction->SetLineWidth(2);
 h_Subtraction -> Draw();
 h_mWW_3 -> Draw("same");
 cc_Subtraction->SetGrid();




 //---- fit with function ----

 std::cout << " -------------------------------- " << std::endl;
 std::cout << " ------------ SIGNAL ------------ " << std::endl;
 std::cout << " -------------------------------- " << std::endl;

 TCanvas* cc_Subtraction_fit = new TCanvas("cc_Subtraction_fit","cc_Subtraction_fit",800,600);
 cc_Subtraction_fit->cd();
 cc_Subtraction_fit->SetGrid();

 TF1 *crystal_S = new TF1("crystal_S",RightCrystalBall,200,MAX,5);
 crystal_S->SetParameters(1,1,mass,h_mWW_3->GetRMS(),h_mWW_3->Integral());
 crystal_S->SetParNames("#alpha","n","Mean","#sigma","N");

//  TF1 *crystal_S = new TF1("crystal_S",doubleGausCrystalBallLowHigh,200,MAX,7);
//  crystal_S->SetParameters(h_mWW_3->Integral(),mass,h_mWW_3->GetRMS(),1.,2,1.,2);
//  crystal_S->SetParNames("N","Mean","#sigma","#alpha","n","#alpha-2","n2");
 crystal_S->SetLineColor(kBlue);
 h_mWW_3->Fit(crystal_S,"r");


 std::cout << " ----------------------------------------------- " << std::endl;
 std::cout << " ------------ SIGNAL + INTERFERENCE ------------ " << std::endl;
 std::cout << " ----------------------------------------------- " << std::endl;

 TF1 *crystal_SI = new TF1("crystal_SI",CrystalBall,200,MAX,5);
//  800   ok:    crystal_SI->SetParameters(1,2,mass,h_Subtraction->GetRMS(),h_Subtraction->Integral());
//  800 em ok:  crystal_SI->SetParameters(1,1,mass,h_Subtraction->GetRMS(),h_Subtraction->Integral());
 crystal_SI->SetParameters(0.1,2.,mass,h_Subtraction->GetRMS(),h_Subtraction->Integral());
 crystal_SI->SetParNames("#alpha","n","Mean","#sigma","N");

//  TF1 *crystal_SI = new TF1("crystal_SI",doubleGausCrystalBallLowHigh,200,MAX,7);
//  crystal_SI->SetParameters(h_Subtraction->Integral(),mass,h_Subtraction->GetRMS(),1.,2,1.,2);
//  crystal_SI->SetParNames("N","Mean","#sigma","#alpha","n","#alpha-2","n2");
 h_Subtraction->Fit(crystal_SI,"r");
 crystal_SI->SetLineColor(kRed);

 h_mWW_3 -> Draw();
 h_Subtraction -> Draw("same");
 crystal_S->Draw("same");
 crystal_SI->Draw("same");
 cc_Subtraction_fit->SetGrid();





}


