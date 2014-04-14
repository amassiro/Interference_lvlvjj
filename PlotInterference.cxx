#include "TString.h"


double doubleGausCrystalBallLowHighPlusExp (double* x, double* par) {
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  //[5] = alpha2
  //[6] = n2

  //[7] = R = ratio between exponential and CB
  //[8] = tau = tau falling of exponential

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

 double R = par[7] ;
 double tau = par[8] ;


 if ((xx-mean)/sigmaP > fabs(alpha)) {
  double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
  double B = n/fabs(alpha) - fabs(alpha);

  return par[0] * ( A * pow(B + (xx-mean)/sigmaP, -1.*n) + R * exp(-xx/tau));
 }

 else if ((xx-mean)/sigmaN < -1.*fabs(alpha2)) {
  double A = pow(n2/fabs(alpha2), n2) * exp(-0.5 * alpha2*alpha2);
  double B = n2/fabs(alpha2) - fabs(alpha2);

  return par[0] * ( A * pow(B - (xx-mean)/sigmaN, -1.*n2) + R * exp(-xx/tau));
 }

 else if ((xx-mean) > 0) {
  return par[0] * ( exp(-1. * (xx-mean)*(xx-mean) / (2*sigmaP*sigmaP) ) + R * exp(-xx/tau));
 }

 else {
  return par[0] * ( exp(-1. * (xx-mean)*(xx-mean) / (2*sigmaN*sigmaN) ) + R * exp(-xx/tau));
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


//---- division of CBLowHigh with CBLowHigh ----

Double_t CrystalBallLowHighDivideCrystalBallLowHigh(Double_t *x,Double_t *par) {
 Double_t num = 0;
 num = crystalBallLowHigh(x,par);

 Double_t den = 1;
 den = crystalBallLowHigh(x,&par[7]);

 if (den != 0) return num/den;
 else return 1.;

}


//---- subtraction of CBLowHigh with CBLowHigh ----
Double_t CrystalBallLowHighMinusCrystalBallLowHigh(Double_t *x,Double_t *par) {
 Double_t num = 0;
 num = crystalBallLowHigh(x,par);

 Double_t den = 1;
 den = crystalBallLowHigh(x,&par[7]);

 return num-den;
}


//---- subtraction of CBLowHighPlusExp with CBLowHigh ----
Double_t CrystalBallLowHighPlusExpMinusCrystalBallLowHigh(Double_t *x,Double_t *par) {
 Double_t num = 0;
 num = doubleGausCrystalBallLowHighPlusExp(x,par);

 Double_t den = 1;
 den = crystalBallLowHigh(x,&par[7+2]);

 return num-den;
}


//---- division of CBLowHighPlusExp with CBLowHigh ----
Double_t CrystalBallLowHighPlusExpDividedByCrystalBallLowHigh(Double_t *x,Double_t *par) {
 Double_t num = 0;
 num = doubleGausCrystalBallLowHighPlusExp(x,par);

 Double_t den = 1;
 den = crystalBallLowHigh(x,&par[7+2]);

 if (den != 0) return num/den;
 else return 1.;

}


//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


//           0 = em, 1 = mm
void PlotInterference(int kind = 0,     int scaleVariation = 0) {

//  scaleVariation = 0   nominal
//                  -1   scale down
//                   1   scale up

 std::string folder;
 if (scaleVariation ==  0) folder = "./";
 if (scaleVariation == -1) folder = "results_down/";
 if (scaleVariation ==  1) folder = "results_up/";

 std::string buffer;
 float num;
 int counter;

 float S_mass[100];
 float S_N[100];
 float S_Mean[100];
 float S_sigma[100];
 float S_alphaR[100];
 float S_nR[100];
 float S_alphaL[100];
 float S_nL[100];

 float SI_mass[100];
 float SI_N[100];
 float SI_Mean[100];
 float SI_sigma[100];
 float SI_alphaR[100];
 float SI_nR[100];
 float SI_alphaL[100];
 float SI_nL[100];
 float SI_R[100];
 float SI_tau[100];

 TString nameS;
 if (kind == 0) nameS = Form ("%sresults_em_S.txt",folder.c_str());
 if (kind == 1) nameS = Form ("%sresults_mm_S.txt",folder.c_str());
 std::ifstream file_S (nameS.Data());
 counter = 0;
 while(!file_S.eof()) {
  getline(file_S,buffer);
  std::cout << "buffer = " << buffer << std::endl;
  if (buffer != "" && buffer.at(0) != '#'){ ///---> save from empty line at the end! And comments!
   std::stringstream line( buffer );
   line >> S_mass[counter];
   line >> S_N[counter];
   line >> S_Mean[counter];
   line >> S_sigma[counter];
   line >> S_alphaR[counter];
   line >> S_nR[counter];
   line >> S_alphaL[counter];
   line >> S_nL[counter];
   counter++;
   std::cout << std::endl;
  }
 }

 TString nameSI;
 if (kind == 0) nameSI = Form ("%sresults_em_SI.txt",folder.c_str());
 if (kind == 1) nameSI = Form ("%sresults_mm_SI.txt",folder.c_str());
 std::ifstream file_SI (nameSI.Data());
 counter = 0;
 while(!file_SI.eof()) {
  getline(file_SI,buffer);
  std::cout << "buffer = " << buffer << std::endl;
  if (buffer != "" && buffer.at(0) != '#'){ ///---> save from empty line at the end! And comments!
   std::stringstream line( buffer );
   line >> SI_mass[counter];
   line >> SI_N[counter];
   line >> SI_Mean[counter];
   line >> SI_sigma[counter];
   line >> SI_alphaR[counter];
   line >> SI_nR[counter];
   line >> SI_alphaL[counter];
   line >> SI_nL[counter];
   line >> SI_R[counter];
   line >> SI_tau[counter];
   counter++;
   std::cout << std::endl;
  }
 }


 //---- fit with function ----

 TFile* fileOutput[100];
 TF1 *crystal_S[100];
 TF1 *crystal_SI[100];

 TCanvas* cc_Subtraction_fit = new TCanvas("cc_Subtraction_fit","cc_Subtraction_fit",800,600);
 cc_Subtraction_fit->cd();
 cc_Subtraction_fit->SetGrid();

 for (int i=0; i<counter; i++) {
  crystal_S[i] = new TF1("crystal_S",crystalBallLowHigh,200,2000,7);
  crystal_S[i]->SetParameters(S_N[i],S_Mean[i],S_sigma[i],S_alphaR[i],S_nR[i],S_alphaL[i],S_nL[i]);
  std::cout << " " << S_N[i] << " " << S_Mean[i] << " " << S_sigma[i] << " " << S_alphaR[i] << " " << S_nR[i] << " " << S_alphaL[i] << " " << S_nL[i] << std::endl;

  crystal_S[i]->SetLineColor(kAzure+i);

  crystal_SI[i] = new TF1("cryStal_SI",doubleGausCrystalBallLowHighPlusExp,200,2000,7+2);
  crystal_SI[i]->SetParameters(SI_N[i],SI_Mean[i],SI_sigma[i],SI_alphaR[i],SI_nR[i],SI_alphaL[i],SI_nL[i],SI_R[i],SI_tau[i]);
  std::cout << " " << SI_N[i] << " " << SI_Mean[i] << " " << SI_sigma[i] << " " << SI_alphaR[i] << " " << SI_nR[i] << " " << SI_alphaL[i] << " " << SI_nL[i] << " " << SI_R[i] << " " << SI_tau[i] << std::endl;
  crystal_SI[i]->SetLineColor(kPink-i);

  crystal_S[i]->SetNpx(300);
  crystal_SI[i]->SetNpx(300);

 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_S[i]->DrawClone("PL");
   crystal_S[i]->GetYaxis()->SetRangeUser(0,1000);
  }
  else crystal_S[i]->DrawClone("PLsame");

  crystal_SI[i]->DrawClone("PLsame");

  std::cout << " S_mass[" << i << "] = " << S_mass[i] << std::endl;
  TString nameFile;
  if (kind == 0) nameFile = Form ("output/%1.0f/WWlvlv_df.root",S_mass[i]);
  if (kind == 1) nameFile = Form ("output/%1.0f/WWlvlv_sf.root",S_mass[i]);
  fileOutput[i] = new TFile (nameFile.Data(),"RECREATE");
  TGraph gS(crystal_S[i]);
  gS.Write ("S");
  TGraph gSI(crystal_SI[i]);
  gSI.Write ("SI");
 }




 std::cout << " ------------------ " << std::endl;

 TCanvas* cc_Interference_fit = new TCanvas("cc_Interference_fit","cc_Interference_fit",800,600);
 cc_Interference_fit->cd();
 cc_Interference_fit->SetGrid();

 TF1 *crystal_Icorr[100];
 for (int i=0; i<counter; i++) {
  crystal_Icorr[i] = new TF1("Icorr",CrystalBallLowHighPlusExpDividedByCrystalBallLowHigh,0,3000,14);

  crystal_Icorr[i]->SetParameter(0,SI_N[i]);
  crystal_Icorr[i]->SetParameter(1,SI_Mean[i]);
  crystal_Icorr[i]->SetParameter(2,SI_sigma[i]);
  crystal_Icorr[i]->SetParameter(3,SI_alphaR[i]);
  crystal_Icorr[i]->SetParameter(4,SI_nR[i]);
  crystal_Icorr[i]->SetParameter(5,SI_alphaL[i]);
  crystal_Icorr[i]->SetParameter(6,SI_nL[i]);
  crystal_Icorr[i]->SetParameter(7,SI_R[i]);
  crystal_Icorr[i]->SetParameter(8,SI_tau[i]);
  crystal_Icorr[i]->SetParameter(9,S_N[i]);
  crystal_Icorr[i]->SetParameter(10,S_Mean[i]);
  crystal_Icorr[i]->SetParameter(11,S_sigma[i]);
  crystal_Icorr[i]->SetParameter(12,S_alphaR[i]);
  crystal_Icorr[i]->SetParameter(13,S_nR[i]);
  crystal_Icorr[i]->SetParameter(14,S_alphaL[i]);
  crystal_Icorr[i]->SetParameter(15,S_nL[i]);

  crystal_Icorr[i]->SetLineColor(kGreen+i);
  crystal_Icorr[i]->SetNpx(300);
 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_Icorr[i]->DrawClone("PL");
   crystal_Icorr[i]->GetYaxis()->SetRangeUser(0.001,50);
  }
  else crystal_Icorr[i]->DrawClone("PLsame");
 }



 TCanvas* cc_InterferenceDifference_fit = new TCanvas("cc_InterferenceDifference_fit","cc_InterferenceDifference_fit",800,600);
 cc_InterferenceDifference_fit->cd();
 cc_InterferenceDifference_fit->SetGrid();

 TF1 *crystal_IcorrDiff[100];
 for (int i=0; i<counter; i++) {
  crystal_IcorrDiff[i] = new TF1("Icorr",CrystalBallLowHighPlusExpMinusCrystalBallLowHigh,0,3000,14);
  crystal_IcorrDiff[i]->SetParameter(0,SI_N[i]);
  crystal_IcorrDiff[i]->SetParameter(1,SI_Mean[i]);
  crystal_IcorrDiff[i]->SetParameter(2,SI_sigma[i]);
  crystal_IcorrDiff[i]->SetParameter(3,SI_alphaR[i]);
  crystal_IcorrDiff[i]->SetParameter(4,SI_nR[i]);
  crystal_IcorrDiff[i]->SetParameter(5,SI_alphaL[i]);
  crystal_IcorrDiff[i]->SetParameter(6,SI_nL[i]);
  crystal_IcorrDiff[i]->SetParameter(7,SI_R[i]);
  crystal_IcorrDiff[i]->SetParameter(8,SI_tau[i]);
  crystal_IcorrDiff[i]->SetParameter(9,S_N[i]);
  crystal_IcorrDiff[i]->SetParameter(10,S_Mean[i]);
  crystal_IcorrDiff[i]->SetParameter(11,S_sigma[i]);
  crystal_IcorrDiff[i]->SetParameter(12,S_alphaR[i]);
  crystal_IcorrDiff[i]->SetParameter(13,S_nR[i]);
  crystal_IcorrDiff[i]->SetParameter(14,S_alphaL[i]);
  crystal_IcorrDiff[i]->SetParameter(15,S_nL[i]);

  crystal_IcorrDiff[i]->SetLineColor(kMagenta+i);

  crystal_IcorrDiff[i]->SetNpx(300);
 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_IcorrDiff[i]->DrawClone("PL");
   crystal_IcorrDiff[i]->GetYaxis()->SetRangeUser(-80,30);
  }
  else crystal_IcorrDiff[i]->DrawClone("PLsame");
 }


 //---------------------
 TCanvas* cc_Variables_S = new TCanvas("cc_Variables_S","cc_Variables_S",900,500);
 cc_Variables_S->Divide(4,2);
 float log_S_N[100];
 float log_SI_N[100];

 for (int i=0; i<counter; i++) {
  double Hmass = 0;
  if (i==0) Hmass = 350;
  if (i==1) Hmass = 500;
  if (i==2) Hmass = 650;
  if (i==3) Hmass = 800;
  if (i==4) Hmass = 1000;

  int NBIN = 500;
  if (Hmass==350)  NBIN = 500;
  if (Hmass==500)  NBIN = 120;
  if (Hmass==650)  NBIN =  70;
  if (Hmass==800)  NBIN = 120*3/4;
  if (Hmass==1000) NBIN =  80*3/4;

  int MAX = 800;
  if (Hmass==350)   MAX =   500;
  if (Hmass==500)   MAX =  1500;
  if (Hmass==650)   MAX =  2000;
  if (Hmass==800)   MAX =  3000;
  if (Hmass==1000)  MAX =  3500;

  int MIN = 200;
  if (Hmass<350) MIN = 200;


 float scale = 1./ (MAX/NBIN);
 log_S_N[i]  = log(S_N[i]  * scale);
 log_SI_N[i] = log(SI_N[i] * scale);
 }

 TGraph* variables_S[10];
 for (int iVar=0; iVar<7; iVar++) {
  if (iVar == 0) { variables_S[iVar] = new TGraph (5,S_mass,log_S_N);  variables_S[iVar]->SetTitle("logN");  }
  if (iVar == 1) { variables_S[iVar] = new TGraph (5,S_mass,S_Mean);   variables_S[iVar]->SetTitle("mean");  }
  if (iVar == 2) { variables_S[iVar] = new TGraph (5,S_mass,S_sigma);  variables_S[iVar]->SetTitle("sigma");  }
  if (iVar == 3) { variables_S[iVar] = new TGraph (5,S_mass,S_alphaR); variables_S[iVar]->SetTitle("#alpha R");  }
  if (iVar == 4) { variables_S[iVar] = new TGraph (5,S_mass,S_nR);     variables_S[iVar]->SetTitle("n R");  }
  if (iVar == 5) { variables_S[iVar] = new TGraph (5,S_mass,S_alphaL); variables_S[iVar]->SetTitle("#alpha L");  }
  if (iVar == 6) { variables_S[iVar] = new TGraph (5,S_mass,S_nL);     variables_S[iVar]->SetTitle("n R");  }
 }

 for (int iVar=0; iVar<7; iVar++) {
  cc_Variables_S->cd (iVar+1);
  variables_S[iVar]->SetMarkerSize(2);
  variables_S[iVar]->SetMarkerStyle(21);
  variables_S[iVar]->DrawClone("APL");
 }

 TCanvas* cc_Variables_SI = new TCanvas("cc_Variables_SI","cc_Variables_SI",900+200,500);
 cc_Variables_SI->Divide(5,2);

 TGraph* variables_SI[10];
 for (int iVar=0; iVar<7+2; iVar++) {
  if (iVar == 0) { variables_SI[iVar] = new TGraph (5,SI_mass,log_SI_N);     variables_SI[iVar]->SetTitle("logN");  }
  if (iVar == 1) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_Mean);      variables_SI[iVar]->SetTitle("mean");  }
  if (iVar == 2) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_sigma);     variables_SI[iVar]->SetTitle("sigma");  }
  if (iVar == 3) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_alphaR);    variables_SI[iVar]->SetTitle("#alpha R");  }
  if (iVar == 4) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_nR);        variables_SI[iVar]->SetTitle("n R");  }
  if (iVar == 5) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_alphaL);    variables_SI[iVar]->SetTitle("#alpha L");  }
  if (iVar == 6) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_nL);        variables_SI[iVar]->SetTitle("n R");  }
  if (iVar == 7) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_R);         variables_SI[iVar]->SetTitle("R");  }
  if (iVar == 8) { variables_SI[iVar] = new TGraph (5,SI_mass,SI_tau);       variables_SI[iVar]->SetTitle("tau");  }
 }

 for (int iVar=0; iVar<7+2; iVar++) {
  cc_Variables_SI->cd (iVar+1);
  variables_SI[iVar]->SetMarkerSize(2);
  variables_SI[iVar]->SetMarkerStyle(21);
  variables_SI[iVar]->DrawClone("APL");
 }

 for (int iVar=0; iVar<7+2; iVar++) {
  std::cout << "variables_SI[" << iVar << "] [H=450] = " << variables_SI[iVar]->Eval(450) << std::endl;
 }




 //---- test interpolation of variables ----
 //---- S
 TCanvas* cc_Interpolation_S = new TCanvas("cc_Interpolation_S","cc_Interpolation_S",800,600);
 TF1* S_crystal_qqH[100];
 for (int i=0; i<counter; i++) {
  double Hmass = 0;
  if (i==0) Hmass = 350;
  if (i==1) Hmass = 500;
  if (i==2) Hmass = 650;
  if (i==3) Hmass = 800;
  if (i==4) Hmass = 1000;


  int NBIN = 500;
  if (Hmass==350)  NBIN = 500;
  if (Hmass==500)  NBIN = 120;
  if (Hmass==650)  NBIN =  70;
  if (Hmass==800)  NBIN = 120*3/4;
  if (Hmass==1000) NBIN =  80*3/4;

  int MAX = 800;
  if (Hmass==350)   MAX =   500;
  if (Hmass==500)   MAX =  1500;
  if (Hmass==650)   MAX =  2000;
  if (Hmass==800)   MAX =  3000;
  if (Hmass==1000)  MAX =  3500;

  int MIN = 200;
  if (Hmass<350) MIN = 200;


  float scale = 1./ (MAX/NBIN);
  crystal_S[i]->SetParameter(0,crystal_S[i]->GetParameter(0) * scale);

  crystal_S[i] -> SetLineColor(kBlue);
  if (i==0) {
   crystal_S[i]->DrawClone("PL");
   crystal_S[i]->GetYaxis()->SetRangeUser(0,1000);
  }
  else crystal_S[i]->DrawClone("PLsame");
 }

 for (int iMass = 0; iMass < 13*2; iMass++) {
  double Hmass = 350+25*iMass;
  TString name = Form ("S_crystal_qqH_%d",iMass);
  S_crystal_qqH[iMass] = new TF1(name.Data(),crystalBallLowHigh,0,3000,7);
  for (int iVar = 0; iVar<7; iVar++) {
   if (iVar == 0) {
    S_crystal_qqH[iMass]->SetParameter(iVar,exp(variables_S[iVar]->Eval(Hmass)));
   }
   else {
    S_crystal_qqH[iMass]->SetParameter(iVar,variables_S[iVar]->Eval(Hmass));
   }
  }
  S_crystal_qqH[iMass] -> SetNpx(300);
  S_crystal_qqH[iMass] -> SetLineColor(kRed);
  S_crystal_qqH[iMass] -> DrawClone("Lsame");
 }
 cc_Interpolation_S->SetGrid();
 for (int i=0; i<counter; i++) {
  crystal_S[i]->DrawClone("PLsame");
 }

  //---- SI
 TCanvas* cc_Interpolation_SI = new TCanvas("cc_Interpolation_SI","cc_Interpolation_SI",800,600);
 TF1* SI_crystal_qqH[100];
 for (int i=0; i<counter; i++) {
  double Hmass = 0;
  if (i==0) Hmass = 350;
  if (i==1) Hmass = 500;
  if (i==2) Hmass = 650;
  if (i==3) Hmass = 800;
  if (i==4) Hmass = 1000;

  int NBIN = 500;
  if (Hmass==350)  NBIN = 500;
  if (Hmass==500)  NBIN = 120;
  if (Hmass==650)  NBIN =  70;
  if (Hmass==800)  NBIN = 120*3/4;
  if (Hmass==1000) NBIN =  80*3/4;

  int MAX = 800;
  if (Hmass==350)   MAX =   500;
  if (Hmass==500)   MAX =  1500;
  if (Hmass==650)   MAX =  2000;
  if (Hmass==800)   MAX =  3000;
  if (Hmass==1000)  MAX =  3500;

  int MIN = 200;
  if (Hmass<350) MIN = 200;


  float scale = 1./ (MAX/NBIN);
  crystal_SI[i]->SetParameter(0,crystal_SI[i]->GetParameter(0) * scale);

  crystal_SI[i] -> SetLineColor(kBlue);
  if (i==0) {
   crystal_SI[i]->DrawClone("PL");
   crystal_SI[i]->GetYaxis()->SetRangeUser(0,1000);
  }
  else crystal_SI[i]->DrawClone("PLsame");
 }

 for (int iMass = 0; iMass < 13*2; iMass++) {
  double Hmass = 350+25*iMass;
  TString name = Form ("SI_crystal_qqH_%d",iMass);
  SI_crystal_qqH[iMass] = new TF1(name.Data(),doubleGausCrystalBallLowHighPlusExp,0,3000,7+2);
  for (int iVar = 0; iVar<7+2; iVar++) {
   if (iVar == 0) {
    SI_crystal_qqH[iMass]->SetParameter(iVar,exp(variables_SI[iVar]->Eval(Hmass)));
   }
   else {
    SI_crystal_qqH[iMass]->SetParameter(iVar,variables_SI[iVar]->Eval(Hmass));
   }
  }
  SI_crystal_qqH[iMass] -> SetNpx(300);
  SI_crystal_qqH[iMass] -> SetLineColor(kRed);
  SI_crystal_qqH[iMass] -> DrawClone("Lsame");
 }
 cc_Interpolation_SI->SetGrid();
 for (int i=0; i<counter; i++) {
  crystal_SI[i]->DrawClone("PLsame");
 }



 //---- weight
 TCanvas* cc_Interpolation_Weight = new TCanvas("cc_Interpolation_Weight","cc_Interpolation_Weight",800,600);
 TF1* Weight_crystal_qqH[100];
 for (int i=0; i<counter; i++) {
  double Hmass = 0;
  if (i==0) Hmass = 350;
  if (i==1) Hmass = 500;
  if (i==2) Hmass = 650;
  if (i==3) Hmass = 800;
  if (i==4) Hmass = 1000;


  int NBIN = 500;
  if (Hmass==350)  NBIN = 500;
  if (Hmass==500)  NBIN = 120;
  if (Hmass==650)  NBIN =  70;
  if (Hmass==800)  NBIN = 120*3/4;
  if (Hmass==1000) NBIN =  80*3/4;

  int MAX = 800;
  if (Hmass==350)   MAX =   500;
  if (Hmass==500)   MAX =  1500;
  if (Hmass==650)   MAX =  2000;
  if (Hmass==800)   MAX =  3000;
  if (Hmass==1000)  MAX =  3500;

  int MIN = 200;
  if (Hmass<350) MIN = 200;


  float scale = 1./ (MAX/NBIN);

  for (int iMass = 0; iMass < 13*2; iMass++) {
   double Hmass = 350+25*iMass;
   TString name = Form ("Weight_crystal_qqH_%d",iMass);
   Weight_crystal_qqH[iMass] = new TF1(name.Data(),CrystalBallLowHighPlusExpDividedByCrystalBallLowHigh,200,3000,14);
   for (int iVar = 0; iVar<7+2; iVar++) {
    if (iVar == 0) {
     Weight_crystal_qqH[iMass]->SetParameter(iVar,exp(variables_SI[iVar]->Eval(Hmass)));
     Weight_crystal_qqH[iMass]->SetParameter(iVar+7,exp(variables_S[iVar]->Eval(Hmass)));
    }
    else {
     if (iVar<7) {
      Weight_crystal_qqH[iMass]->SetParameter(iVar+7,variables_S[iVar]->Eval(Hmass));
     }
     Weight_crystal_qqH[iMass]->SetParameter(iVar,variables_SI[iVar]->Eval(Hmass));
    }
   }
   Weight_crystal_qqH[iMass] -> SetNpx(300);
   Weight_crystal_qqH[iMass] -> SetLineColor(kMagenta+iMass);
   if (iMass == 0) Weight_crystal_qqH[iMass] -> DrawClone("L");
   else Weight_crystal_qqH[iMass] -> DrawClone("Lsame");
  }
 }
 cc_Interpolation_Weight->SetGrid();

}


