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

//  double mean = par[1] ; // mean
//  double sigmaP = par[2] ; // sigma of the positive side of the gaussian
//  double sigmaN = par[3] ; // sigma of the negative side of the gaussian
//  double alpha = par[4] ; // junction point on the positive side of the gaussian
//  double n = par[5] ; // power of the power law on the positive side of the gaussian
//  double alpha2 = par[6] ; // junction point on the negative side of the gaussian
//  double n2 = par[7] ; // power of the power law on the negative side of the gaussian

 double mean = par[1] ; // mean
 double sigmaP = par[2] ; // sigma of the positive side of the gaussian  |  they are the same!!!
 double sigmaN = par[2] ; // sigma of the negative side of the gaussian  |
 double alpha = par[3] ; // junction point on the positive side of the gaussian
 double n = par[4] ; // power of the power law on the positive side of the gaussian
 double alpha2 = par[5] ; // junction point on the negative side of the gaussian
 double n2 = par[6] ; // power of the power law on the negative side of the gaussian

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



double crystalBallLowHigh (double* x, double* par) {
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha on the right-hand side
  //[4] = n
  //[5] = alpha2 on the left-hand side
  //[6] = n2

 double xx = x[0];
 double mean = par[1];
 double sigma = fabs (par[2]);
 double alpha = par[3];
 double n = par[4];
 double alpha2 = par[5];
 double n2 = par[6];

 if( (xx-mean)/sigma > fabs(alpha) ) {
  double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
  double B = n/fabs(alpha) - fabs(alpha);

  return par[0] * A * pow(B + (xx-mean)/sigma, -1.*n);
 }

 else if( (xx-mean)/sigma < -1.*fabs(alpha2) ) {
  double A = pow(n2/fabs(alpha2), n2) * exp(-0.5 * alpha2*alpha2);
  double B = n2/fabs(alpha2) - fabs(alpha2);

  return par[0] * A * pow(B - (xx-mean)/sigma, -1.*n2);
 }

 else {
  return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
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


//---- division of CBLowHigh with CBLowHigh ----
Double_t CrystalBallLowHighMinusCrystalBallLowHigh(Double_t *x,Double_t *par) {
 Double_t num = 0;
 num = crystalBallLowHigh(x,par);

 Double_t den = 1;
 den = crystalBallLowHigh(x,&par[7]);

 return num-den;
}




//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


//           0 = em, 1 = mm
void DrawVBFNLO(int kind = 0,         int mass = 350,   bool doFit = 1,     int scaleVariation = 0) {

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
 TH1F* h_Ratio = new TH1F("h_mWW_ratio","h_mWW_ratio",NBIN,0,MAX);
 TH1F* h_Subtraction = new TH1F("h_Subtraction","h_Subtraction",NBIN,0,MAX);
 TH1F* h_I = new TH1F("h_I","h_I",NBIN,0,MAX);

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
//  TString cut = Form ("mjj>30 && pt1>5 && pt2>5 && jetpt1>10 && jetpt2>10");

 //---- to compare with MC@NLO
//  TString cut = Form ("mjj>100 && pt1>8 && pt2>8 && jetpt1>10 && jetpt2>10");
 TString cut = Form ("mjj>100 && pt1>8 && pt2>8 && jetpt1>10 && jetpt2>10 && abs(jeteta1)<6.5 && abs(jeteta2)<6.5  && abs(eta1)<2.5 && abs(eta2)<2.5 && mll>8");


//    leptons min E   5 GeV
//    minimum delta R between the fwd and bkw jets    0.4
//    minimum delta R between jets and leptons        0.4
//    minimum delta R between two leptons     0.4


 
//  TString cut = Form ("1");

//  TString weightWithXsec126 = Form ("(mll>80 && mll<100 ) * %s * %f",weight.Data(),xsec[0]);
//  TString weightWithXsec    = Form ("(mll>80 && mll<100 ) * %f",xsecToUse);

 TString weightWithXsec126 = Form ("(%s) * (%s * %f)",cut.Data(),weight.Data(),xsec[0]/2.);
 TString weightWithXsec    ;
 if (scaleVariation == 0) weightWithXsec    = Form ("(%s) * (%f)",cut.Data(),xsecToUse);
 else                     weightWithXsec    = Form ("(%s) * (%s) * (%f)",weight.Data(), cut.Data(),xsecToUse);

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

  float S = h_mWW_3->GetBinContent(iBin+1);
  h_I ->  SetBinContent (iBin+1, num - den - S);

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


//  bool doFit = true;
//  bool doFit = false;

 if (doFit) {

 //---- fit with function ----

  std::cout << " -------------------------------- " << std::endl;
  std::cout << " ------------ SIGNAL ------------ " << std::endl;
  std::cout << " -------------------------------- " << std::endl;

  TCanvas* cc_Subtraction_fit = new TCanvas("cc_Subtraction_fit","cc_Subtraction_fit",800,600);
  cc_Subtraction_fit->cd();
  cc_Subtraction_fit->SetGrid();

//  TF1 *crystal_S = new TF1("crystal_S",RightCrystalBall,200,MAX,5);
//  crystal_S->SetParameters(1,1,mass,h_mWW_3->GetRMS(),h_mWW_3->Integral());
//  crystal_S->SetParNames("#alpha","n","Mean","#sigma","N");

  TF1 *crystal_S = new TF1("crystal_S",crystalBallLowHigh,MIN,MAX,7);
  crystal_S->SetParameters(h_mWW_3->Integral(),mass,h_mWW_3->GetRMS(),1.,2,1.,2);
  crystal_S->SetParNames("N","Mean","#sigma","#alpha","n","#alpha-2","n2");
 //                      0    1        2        3     4      5       6
 //                                           juncR        juncL
  crystal_S->SetNpx(2000);
  crystal_S->SetParameter (0, h_mWW_3->GetBinContent (h_mWW_3->GetMaximumBin ())) ;

  crystal_S->SetParameter (1, mass) ;
  crystal_S->SetParLimits (1, 0.90 * mass, 1.10 * mass ) ;

  crystal_S->SetParameter (2, 0.9 * h_mWW_3->GetRMS ()) ;
  crystal_S->SetParLimits (2, 0.00005 * h_mWW_3->GetRMS (), 10 * h_mWW_3->GetRMS ()) ;
  if (mass > 700) crystal_S->SetParLimits (2, 0.02 * h_mWW_3->GetRMS (), 10 * h_mWW_3->GetRMS ()) ;

  crystal_S->SetParameter (3, 1.0) ;
  crystal_S->SetParLimits (3, 0.5, 20.) ;
  if (mass == 500)  crystal_S->SetParLimits (3, 0.3, 20.) ;
  if (mass == 650)  crystal_S->SetParLimits (3, 0.1, 20.) ;
  if (mass > 700)   crystal_S->SetParLimits (3, 0.1, 20.) ;

  crystal_S->SetParameter (4, 1.5) ;
  crystal_S->SetParLimits (4, 1.0, 50) ;

  crystal_S->SetParameter (5, 1.0) ;
  crystal_S->SetParLimits (5, 0.5, 20.) ;
  if (mass == 650)  crystal_S->SetParLimits (5, 0.3, 20.) ;
  if (mass > 700)   crystal_S->SetParLimits (5, 0.3, 20.) ;

  crystal_S->SetParameter (6, 1.5) ;
  crystal_S->SetParLimits (6, 1.0, 50) ;

  crystal_S->SetLineColor(kCyan);
  h_mWW_3->Fit (crystal_S, "+", "",  mass - 0.5 * h_mWW_3->GetRMS (), mass + 0.5 * h_mWW_3->GetRMS ()) ;
  crystal_S->SetParameters (crystal_S->GetParameters ()) ;

//  crystal_S->FixParameter (1, crystal_S->GetParameters ()[1]) ; //---- mean
//  crystal_S->FixParameter (2, crystal_S->GetParameters ()[2]) ; //---- sigma

  crystal_S->SetLineColor(kBlue);
//   h_mWW_3->Fit (crystal_S, "+Lr", "",250, mass + 4 * h_mWW_3->GetRMS ()) ;
//   if (mass > 700) h_mWW_3->Fit (crystal_S, "+Lr", "",400, mass + 3 * h_mWW_3->GetRMS ());
//   else            h_mWW_3->Fit (crystal_S, "+Lr", "",250, mass + 4 * h_mWW_3->GetRMS ());
  h_mWW_3->Fit (crystal_S, "+Lr", "",MIN, mass + 4 * h_mWW_3->GetRMS ());
//   if (mass == 650) h_mWW_3->Fit (crystal_S, "+Lr", "",400, mass + 3 * h_mWW_3->GetRMS ());

  
  //  h_mWW_3->Fit (crystal_S, "+Lr", ""); //,250, mass + 4 * h_mWW_3->GetRMS ()) ;
//  h_mWW_3->Fit(crystal_S,"r");


 std::cout << " ----------------------------------------------- " << std::endl;
 std::cout << " ------------ SIGNAL + INTERFERENCE ------------ " << std::endl;
 std::cout << " ----------------------------------------------- " << std::endl;
  // 
//  TF1 *crystal_SI = new TF1("crystal_SI",CrystalBall,200,MAX,5);
//  800   ok:    crystal_SI->SetParameters(1,2,mass,h_Subtraction->GetRMS(),h_Subtraction->Integral());
//  800 em ok:  crystal_SI->SetParameters(1,1,mass,h_Subtraction->GetRMS(),h_Subtraction->Integral());
//  crystal_SI->SetParameters(0.1,2.,mass,h_Subtraction->GetRMS(),h_Subtraction->Integral());
//  crystal_SI->SetParNames("#alpha","n","Mean","#sigma","N");

  TF1 *crystal_SI = new TF1("crystal_SI",crystalBallLowHigh,MIN,MAX,7);
  crystal_SI->SetParameters(h_Subtraction->Integral(),mass,h_Subtraction->GetRMS(),1.,2,1.,2);
  crystal_SI->SetParNames("N","Mean","#sigma","#alpha","n","#alpha-2","n2");

  crystal_SI->SetNpx(2000);
  crystal_SI->SetParameter (0, h_Subtraction->GetBinContent (h_Subtraction->GetMaximumBin ())) ;

  crystal_SI->SetParameter (1, mass) ;
  crystal_SI->SetParLimits (1, 0.90 * mass, 1.10 * mass ) ;

  crystal_SI->SetParameter (2, 0.8 * h_mWW_3->GetRMS ()) ;
  crystal_SI->SetParLimits (2, 0.001 * h_mWW_3->GetRMS (), 10 * h_mWW_3->GetRMS ()) ;

  crystal_SI->SetParameter (3, 1.0) ;
  crystal_SI->SetParLimits (3, 0.5, 20.) ;

  crystal_SI->SetParameter (4, 1.5) ;
  crystal_SI->SetParLimits (4, 1.0, 50) ;

  crystal_SI->SetParameter (5, 1.0) ;
  crystal_SI->SetParLimits (5, 0.5, 20.) ;
  if (mass == 500) crystal_SI->SetParLimits (5, 0.1, 20.) ;
  if (mass == 650) crystal_SI->SetParLimits (5, 0.2, 20.) ;
  if (mass == 800) crystal_SI->SetParLimits (5, 0.2, 20.) ;

  crystal_SI->SetParameter (6, 1.5) ;
  crystal_SI->SetParLimits (6, 1.0, 50) ;

  crystal_SI->SetLineColor(kMagenta-10);
  h_Subtraction->Fit (crystal_SI, "+", "",  mass - 0.5 * h_Subtraction->GetRMS (), mass + 0.5 * h_Subtraction->GetRMS ()) ;
  crystal_SI->SetParameters (crystal_SI->GetParameters ()) ;

  crystal_SI->SetLineColor(kRed);
//  h_Subtrac  if (mass > 700) h_Subtraction->Fit (crystal_SI, "+Lr", ""tion->Fit (crystal_SI, "+Lr", "");
  if (mass > 700)       h_Subtraction->Fit (crystal_SI, "+Lr", "",500, mass + 3 * h_mWW_3->GetRMS ());
//   else if (mass >= 500) h_Subtraction->Fit (crystal_SI, "+Lr", "",300, mass + 3 * h_mWW_3->GetRMS ());
  else if (mass >= 500) h_Subtraction->Fit (crystal_SI, "+Lr", "",400, mass + 3 * h_mWW_3->GetRMS ());
  else                  h_Subtraction->Fit (crystal_SI, "+Lr", "",MIN, mass + 4 * h_mWW_3->GetRMS ());




  h_mWW_3 -> Draw();
  h_Subtraction -> Draw("same");
  crystal_S->Draw("same");
  crystal_SI->Draw("same");
  cc_Subtraction_fit->SetGrid();

  //----------------------
  //---- closure test ----

  TCanvas* cc_I = new TCanvas("cc_I","cc_I",800,600);
  cc_I->cd();
  cc_I->SetGrid();
  h_I -> SetLineColor(kBlue);
  h_I -> SetLineWidth(2);
  h_I -> Draw();
  TF1 *crystal_I = new TF1("crystal_I",CrystalBallLowHighMinusCrystalBallLowHigh,0,MAX,14);
  for (int i=0; i<7; i++) {
   crystal_I->SetParameter(i,crystal_SI->GetParameter(i));
   crystal_I->SetParameter(i+7,crystal_S->GetParameter(i));
  }
  crystal_I->Draw("same");

 //-----------------------------
 //---- to dump in txt file ----

  std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;
  std::cout << " ---------------------------------- " << std::endl;
  std::cout << " ---------------------------------- " << std::endl;
  std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;

  std::cout << " S " << std::endl;
  std::cout << mass << " ";
  for (int i=0; i<7; i++) {
   std::cout << " " << crystal_S->GetParameter (i);
  }
  std::cout << std::endl;
  std::cout << " SI " << std::endl;
  std::cout << mass << " ";
  for (int i=0; i<7; i++) {
   std::cout << " " << crystal_SI->GetParameter (i);
  }
  std::cout << std::endl;

  std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;
  std::cout << " ---------------------------------- " << std::endl;
  std::cout << " ---------------------------------- " << std::endl;
  std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;std::cout << std::endl;

 }

 //--------------------------------
 //---- Comparison with VBFNLO ----
 //  /home/amassiro/Interference/Interference_VBF/data/800/MCatNLO_Franziska.root
 //  /home/amassiro/Interference/Interference_VBF/data/800/mWW_SBI_dat.root
 //  /home/amassiro/Interference/Interference_VBF/data/800/mWW_B_dat.root

 //---- B ----
 TCanvas* cc_B = new TCanvas("cc_B","Background",800,600);
 TFile* f_B_VBFNLO = new TFile ("/home/amassiro/Interference/Interference_VBF/data/800/mWW_B_dat.root","READ");
 TH1F* h_B_VBFNLO = (TH1F*) f_B_VBFNLO->Get("h_mWW_B_dat");
 h_B_VBFNLO->SetLineColor(kRed);
 h_B_VBFNLO->SetLineWidth(2);
 h_B_VBFNLO->SetLineStyle(1);
 h_B_VBFNLO->Scale (40000.*2); //---- *2 for e+mu- to mu+e-

 h_mWW_1->SetLineColor(kBlue);
 h_mWW_1->SetLineWidth(2);
 h_mWW_1->SetLineStyle(2);

 h_mWW_1->Draw();
 h_B_VBFNLO->Draw("same");

//  h_mWW_1->DrawNormalized();
//  h_B_VBFNLO->DrawNormalized("same");

 TLegend* leg_B = new TLegend (0.5,0.5,0.9,0.9);
 leg_B -> SetFillColor(0);
 leg_B -> AddEntry(h_mWW_1,    "Phantom", "L");
 leg_B -> AddEntry(h_B_VBFNLO, "VBFNLO",  "L");
 leg_B -> Draw();

 cc_B->SetGrid();

 //---- SBI ----
 TCanvas* cc_SBI = new TCanvas("cc_SBI","S+I+B",800,600);
 TFile* f_SBI_VBFNLO = new TFile ("/home/amassiro/Interference/Interference_VBF/data/800/mWW_SBI_dat.root","READ");
 TH1F* h_SBI_VBFNLO = (TH1F*) f_SBI_VBFNLO->Get("h_mWW_SBI_dat");
 h_SBI_VBFNLO->SetLineColor(kRed);
 h_SBI_VBFNLO->SetLineWidth(2);
 h_SBI_VBFNLO->SetLineStyle(1);
 h_SBI_VBFNLO->Scale (40000.*2); //---- *2 for e+mu- to mu+e-

 h_mWW_2->SetLineColor(kBlue);
 h_mWW_2->SetLineWidth(2);
 h_mWW_2->SetLineStyle(2);

 h_mWW_2->Draw();
 h_SBI_VBFNLO->Draw("same");

//  h_mWW_2->DrawNormalized();
//  h_SBI_VBFNLO->DrawNormalized("same");

 TLegend* leg_SBI = new TLegend (0.5,0.5,0.9,0.9);
 leg_SBI -> SetFillColor(0);
 leg_SBI -> AddEntry(h_mWW_2,      "Phantom", "L");
 leg_SBI -> AddEntry(h_SBI_VBFNLO, "VBFNLO",  "L");
 leg_SBI -> Draw();

 cc_SBI->SetGrid();





}



