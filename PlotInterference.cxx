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
  Double_t b = par[1]/absAlpha - absAlpha;

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
  Double_t b = par[1]/absAlpha - absAlpha;

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



//---- division of CB with rightCB ----

Double_t CrystalBallDivideRightCrystalBall(Double_t *x,Double_t *par) {
 Double_t num = 0;

 Double_t t = (x[0]-par[2])/par[3];
 if (par[0] < 0) t = -t;

 Double_t absAlpha = fabs((Double_t)par[0]);

 if (t >= -absAlpha) {
  num = par[4]*exp(-0.5*t*t);
 }
 else {
  Double_t a =  TMath::Power(par[1]/absAlpha,par[1])*exp(-0.5*absAlpha*absAlpha);
  Double_t b = par[1]/absAlpha - absAlpha;

  num = par[4]*(a/TMath::Power(b - t, par[1]));
 }



 Double_t den = 1;

 t = (x[0]-par[2+5])/par[3+5];
 if (par[0+5] > 0) t = -t;

 absAlpha = fabs((Double_t)par[0+5]);

 if (t >= -absAlpha) {
  den = par[4+5]*exp(-0.5*t*t);
 }
 else {
  Double_t a =  TMath::Power(par[1+5]/absAlpha,par[1+5])*exp(-0.5*absAlpha*absAlpha);
  Double_t b = par[1+5]/absAlpha - absAlpha;

  den =  par[4+5]*(a/TMath::Power(b - t, par[1+5]));
 }

 if (den != 0) return num/den;
 else return 1.;

}


//---- Subtraction CB with rightCB ----
Double_t CrystalBallMinusRightCrystalBall(Double_t *x,Double_t *par) {
 Double_t num = 0;

 Double_t t = (x[0]-par[2])/par[3];
 if (par[0] < 0) t = -t;

 Double_t absAlpha = fabs((Double_t)par[0]);

 if (t >= -absAlpha) {
  num = par[4]*exp(-0.5*t*t);
 }
 else {
  Double_t a =  TMath::Power(par[1]/absAlpha,par[1])*exp(-0.5*absAlpha*absAlpha);
  Double_t b = par[1]/absAlpha - absAlpha;

  num = par[4]*(a/TMath::Power(b - t, par[1]));
 }



 Double_t den = 1;

 t = (x[0]-par[2+5])/par[3+5];
 if (par[0+5] > 0) t = -t;

 absAlpha = fabs((Double_t)par[0+5]);

 if (t >= -absAlpha) {
  den = par[4+5]*exp(-0.5*t*t);
 }
 else {
  Double_t a =  TMath::Power(par[1+5]/absAlpha,par[1+5])*exp(-0.5*absAlpha*absAlpha);
  Double_t b = par[1+5]/absAlpha - absAlpha;

  den =  par[4+5]*(a/TMath::Power(b - t, par[1+5]));
 }

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
 float S_alpha[100];
 float S_n[100];
 float S_Mean[100];
 float S_sigma[100];
 float S_N[100];

 float SI_mass[100];
 float SI_alpha[100];
 float SI_n[100];
 float SI_Mean[100];
 float SI_sigma[100];
 float SI_N[100];

 std::ifstream file_S ("results_mm_S.txt");
 counter = 0;
 while(!file_S.eof()) {
  getline(file_S,buffer);
  std::cout << "buffer = " << buffer << std::endl;
  if (buffer != "" && buffer.at(0) != '#'){ ///---> save from empty line at the end! And comments!
   std::stringstream line( buffer );
   line >> S_mass[counter];
   line >> S_alpha[counter];
   line >> S_n[counter];
   line >> S_Mean[counter];
   line >> S_sigma[counter];
   line >> S_N[counter];
   counter++;
   std::cout << std::endl;
  }
 }

 std::ifstream file_SI ("results_mm_SI.txt");
 counter = 0;
 while(!file_SI.eof()) {
  getline(file_SI,buffer);
  std::cout << "buffer = " << buffer << std::endl;
  if (buffer != "" && buffer.at(0) != '#'){ ///---> save from empty line at the end! And comments!
   std::stringstream line( buffer );
   line >> SI_mass[counter];
   line >> SI_alpha[counter];
   line >> SI_n[counter];
   line >> SI_Mean[counter];
   line >> SI_sigma[counter];
   line >> SI_N[counter];
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
  crystal_S[i] = new TF1("crystal_S",RightCrystalBall,200,2000,5);
  crystal_S[i]->SetParameters(S_alpha[i],S_n[i],S_Mean[i],S_sigma[i],S_N[i]);
  crystal_S[i]->SetLineColor(kBlue+i);

  crystal_SI[i] = new TF1("cryStal_SI",CrystalBall,200,2000,5);
  crystal_SI[i]->SetParameters(SI_alpha[i],SI_n[i],SI_Mean[i],SI_sigma[i],SI_N[i]);
  crystal_SI[i]->SetLineColor(kRed-i);
  std::cout << " " << SI_alpha[i] << " , " << SI_n[i] << " , " << SI_Mean[i] << " , " << SI_sigma[i] << " , " << SI_N[i] << " , " <<      S_alpha[i] << " , " << S_n[i] << " , " << S_Mean[i] << " , " << S_sigma[i] << " , " << S_N[i] << std::endl;

 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_S[i]->Draw("PL");
   crystal_S[i]->GetYaxis()->SetRangeUser(0,200);
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
  crystal_Icorr[i] = new TF1("Icorr",CrystalBallDivideRightCrystalBall,200,2000,10);
  crystal_Icorr[i]->SetParameters(SI_alpha[i],SI_n[i],SI_Mean[i],SI_sigma[i],SI_N[i],     S_alpha[i],S_n[i],S_Mean[i],S_sigma[i],S_N[i]);
  std::cout << " " << SI_alpha[i] << " , " << SI_n[i] << " , " << SI_Mean[i] << " , " << SI_sigma[i] << " , " << SI_N[i] << " , " <<      S_alpha[i] << " , " << S_n[i] << " , " << S_Mean[i] << " , " << S_sigma[i] << " , " << S_N[i] << std::endl;
  crystal_Icorr[i]->SetLineColor(kGreen+i);
 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_Icorr[i]->Draw("PL");
   crystal_Icorr[i]->GetYaxis()->SetRangeUser(-10,100);
  }
  else crystal_Icorr[i]->Draw("PLsame");
 }



 TCanvas* cc_InterferenceDifference_fit = new TCanvas("cc_InterferenceDifference_fit","cc_InterferenceDifference_fit",800,600);
 cc_InterferenceDifference_fit->cd();
 cc_InterferenceDifference_fit->SetGrid();

 TF1 *crystal_IcorrDiff[100];
 for (int i=0; i<counter; i++) {
  crystal_IcorrDiff[i] = new TF1("Icorr",CrystalBallMinusRightCrystalBall,200,2000,10);
  crystal_IcorrDiff[i]->SetParameters(SI_alpha[i],SI_n[i],SI_Mean[i],SI_sigma[i],SI_N[i],     S_alpha[i],S_n[i],S_Mean[i],S_sigma[i],S_N[i]);
  crystal_IcorrDiff[i]->SetLineColor(kMagenta+i);
 }


 for (int i=0; i<counter; i++) {
  if (i==0) {
   crystal_IcorrDiff[i]->Draw("PL");
   crystal_IcorrDiff[i]->GetYaxis()->SetRangeUser(-10,100);
  }
  else crystal_IcorrDiff[i]->Draw("PLsame");
 }

}


