#include "TString.h"


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
void PlotInterference(int kind = 0) {

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

 TString nameS;
 if (kind == 0) nameS = Form ("results_em_S.txt");
 if (kind == 1) nameS = Form ("results_mm_S.txt");
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
 if (kind == 0) nameSI = Form ("results_em_SI.txt");
 if (kind == 1) nameSI = Form ("results_mm_SI.txt");
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
   counter++;
   std::cout << std::endl;
  }
 }


 //---- fit with function ----

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

  crystal_SI[i] = new TF1("cryStal_SI",crystalBallLowHigh,200,2000,7);
  crystal_SI[i]->SetParameters(SI_N[i],SI_Mean[i],SI_sigma[i],SI_alphaR[i],SI_nR[i],SI_alphaL[i],SI_nL[i]);
  std::cout << " " << SI_N[i] << " " << SI_Mean[i] << " " << SI_sigma[i] << " " << SI_alphaR[i] << " " << SI_nR[i] << " " << SI_alphaL[i] << " " << SI_nL[i] << std::endl;
  crystal_SI[i]->SetLineColor(kPink-i);

  crystal_S[i]->SetNpx(2000);
  crystal_SI[i]->SetNpx(2000);

 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_S[i]->Draw("PL");
   crystal_S[i]->GetYaxis()->SetRangeUser(0,1000);
  }
  else crystal_S[i]->Draw("PLsame");

  crystal_SI[i]->Draw("PLsame");
 }




 std::cout << " ------------------ " << std::endl;

 TCanvas* cc_Interference_fit = new TCanvas("cc_Interference_fit","cc_Interference_fit",800,600);
 cc_Interference_fit->cd();
 cc_Interference_fit->SetGrid();

 TF1 *crystal_Icorr[100];
 for (int i=0; i<counter; i++) {
  crystal_Icorr[i] = new TF1("Icorr",CrystalBallLowHighDivideCrystalBallLowHigh,0,3000,14);

  crystal_Icorr[i]->SetParameter(0,SI_N[i]);
  crystal_Icorr[i]->SetParameter(1,SI_Mean[i]);
  crystal_Icorr[i]->SetParameter(2,SI_sigma[i]);
  crystal_Icorr[i]->SetParameter(3,SI_alphaR[i]);
  crystal_Icorr[i]->SetParameter(4,SI_nR[i]);
  crystal_Icorr[i]->SetParameter(5,SI_alphaL[i]);
  crystal_Icorr[i]->SetParameter(6,SI_nL[i]);
  crystal_Icorr[i]->SetParameter(7,S_N[i]);
  crystal_Icorr[i]->SetParameter(8,S_Mean[i]);
  crystal_Icorr[i]->SetParameter(9,S_sigma[i]);
  crystal_Icorr[i]->SetParameter(10,S_alphaR[i]);
  crystal_Icorr[i]->SetParameter(11,S_nR[i]);
  crystal_Icorr[i]->SetParameter(12,S_alphaL[i]);
  crystal_Icorr[i]->SetParameter(13,S_nL[i]);

  crystal_Icorr[i]->SetLineColor(kGreen+i);
 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_Icorr[i]->Draw("PL");
   crystal_Icorr[i]->GetYaxis()->SetRangeUser(0.001,50);
  }
  else crystal_Icorr[i]->Draw("PLsame");
 }



 TCanvas* cc_InterferenceDifference_fit = new TCanvas("cc_InterferenceDifference_fit","cc_InterferenceDifference_fit",800,600);
 cc_InterferenceDifference_fit->cd();
 cc_InterferenceDifference_fit->SetGrid();

 TF1 *crystal_IcorrDiff[100];
 for (int i=0; i<counter; i++) {
  crystal_IcorrDiff[i] = new TF1("Icorr",CrystalBallLowHighMinusCrystalBallLowHigh,0,3000,14);
  crystal_IcorrDiff[i]->SetParameter(0,SI_N[i]);
  crystal_IcorrDiff[i]->SetParameter(1,SI_Mean[i]);
  crystal_IcorrDiff[i]->SetParameter(2,SI_sigma[i]);
  crystal_IcorrDiff[i]->SetParameter(3,SI_alphaR[i]);
  crystal_IcorrDiff[i]->SetParameter(4,SI_nR[i]);
  crystal_IcorrDiff[i]->SetParameter(5,SI_alphaL[i]);
  crystal_IcorrDiff[i]->SetParameter(6,SI_nL[i]);
  crystal_IcorrDiff[i]->SetParameter(7,S_N[i]);
  crystal_IcorrDiff[i]->SetParameter(8,S_Mean[i]);
  crystal_IcorrDiff[i]->SetParameter(9,S_sigma[i]);
  crystal_IcorrDiff[i]->SetParameter(10,S_alphaR[i]);
  crystal_IcorrDiff[i]->SetParameter(11,S_nR[i]);
  crystal_IcorrDiff[i]->SetParameter(12,S_alphaL[i]);
  crystal_IcorrDiff[i]->SetParameter(13,S_nL[i]);
  crystal_IcorrDiff[i]->SetLineColor(kMagenta+i);
 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_IcorrDiff[i]->Draw("PL");
   crystal_IcorrDiff[i]->GetYaxis()->SetRangeUser(-80,30);
  }
  else crystal_IcorrDiff[i]->Draw("PLsame");
 }


 //---------------------
 TCanvas* cc_Variables_S = new TCanvas("cc_Variables_S","cc_Variables_S",900,500);
 cc_Variables_S->Divide(4,2);

 TGraph* variables_S[10];
 for (int iVar=0; iVar<7; iVar++) {
  if (iVar == 0) variables_S[iVar] = new TGraph (5,S_mass,S_N);
  if (iVar == 1) variables_S[iVar] = new TGraph (5,S_mass,S_Mean);
  if (iVar == 2) variables_S[iVar] = new TGraph (5,S_mass,S_sigma);
  if (iVar == 3) variables_S[iVar] = new TGraph (5,S_mass,S_alphaR);
  if (iVar == 4) variables_S[iVar] = new TGraph (5,S_mass,S_nR);
  if (iVar == 5) variables_S[iVar] = new TGraph (5,S_mass,S_alphaL);
  if (iVar == 6) variables_S[iVar] = new TGraph (5,S_mass,S_nL);
 }

 for (int iVar=0; iVar<7; iVar++) {
  cc_Variables_S->cd (iVar+1);
  variables_S[iVar]->Draw("APL");
 }

 TCanvas* cc_Variables_SI = new TCanvas("cc_Variables_SI","cc_Variables_SI",900,500);
 cc_Variables_SI->Divide(4,2);

 TGraph* variables_SI[10];
 for (int iVar=0; iVar<7; iVar++) {
  if (iVar == 0) variables_SI[iVar] = new TGraph (5,SI_mass,SI_N);
  if (iVar == 1) variables_SI[iVar] = new TGraph (5,SI_mass,SI_Mean);
  if (iVar == 2) variables_SI[iVar] = new TGraph (5,SI_mass,SI_sigma);
  if (iVar == 3) variables_SI[iVar] = new TGraph (5,SI_mass,SI_alphaR);
  if (iVar == 4) variables_SI[iVar] = new TGraph (5,SI_mass,SI_nR);
  if (iVar == 5) variables_SI[iVar] = new TGraph (5,SI_mass,SI_alphaL);
  if (iVar == 6) variables_SI[iVar] = new TGraph (5,SI_mass,SI_nL);
 }

 for (int iVar=0; iVar<7; iVar++) {
  cc_Variables_SI->cd (iVar+1);
  variables_SI[iVar]->Draw("APL");
 }

 for (int iVar=0; iVar<7; iVar++) {
  std::cout << "variables_SI[" << iVar << "] [H=450] = " << variables_SI[iVar]->Eval(450) << std::endl;
 }

}


