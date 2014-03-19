// to compile:
//   c++ -o ntupleMaker.exe `root-config --glibs --cflags` `lhapdf-config --cppflags  --ldflags` -lm ntupleMaker.cpp
//
// to use:
//   ./ntupleMaker.exe fileLHE    outputFileRoot
//

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>


#include "LHEF.h"


#include "LHAPDF/LHAPDF.h"



// #include "TNtuple.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"


struct ptsort: public std::binary_function<TLorentzVector, TLorentzVector, bool> {
 bool operator () (const TLorentzVector & x, const TLorentzVector & y) {
  return  (x.Perp () < y.Perp () ) ;
 }
} ;

TLorentzVector buildP (const LHEF::HEPEUP & event, int iPart) {
 TLorentzVector dummy ;
 dummy.SetPxPyPzE (
   event.PUP.at (iPart).at (0), // px
   event.PUP.at (iPart).at (1), // py
   event.PUP.at (iPart).at (2), // pz
   event.PUP.at (iPart).at (3) // E
   ) ;
 return dummy ;
}




void fillNtuple (std::string fileNameLHE,  TTree & ntuple, float globalScale) {
 std::ifstream ifs (fileNameLHE.c_str ()) ;
 LHEF::Reader reader (ifs) ;

 long ieve = 0 ;
 long selected = 0 ;
 double jetsNum = 0. ;

 float mH;
 float mWW;
 float mjj;
 float detajj;
 float jetpt1;
 float jetpt2;
 float jeteta1;
 float jeteta2; 
 float pt1;
 float pt2;
 float eta1;
 float eta2;
 float mll;
 float ptll;
 float sameflav;
 float w1; float w2; float w3; float w4; float w5; float w6; float w7; float w8;
 int numb;
 int numt;

 ntuple.Branch("mH",&mH,"mH/F");
 ntuple.Branch("mWW",&mWW,"mWW/F");
 ntuple.Branch("mjj",&mjj,"mjj/F");
 ntuple.Branch("detajj",&detajj,"detajj/F");
 ntuple.Branch("jetpt1",&jetpt1,"jetpt1/F");
 ntuple.Branch("jetpt2",&jetpt2,"jetpt2/F");
 ntuple.Branch("jeteta1",&jeteta1,"jeteta1/F");
 ntuple.Branch("jeteta2",&jeteta2,"jeteta2/F");
 ntuple.Branch("pt1",&pt1,"pt1/F");
 ntuple.Branch("pt2",&pt2,"pt2/F");
 ntuple.Branch("eta1",&eta1,"eta1/F");
 ntuple.Branch("eta2",&eta2,"eta2/F");
 ntuple.Branch("mll",&mll,"mll/F");
 ntuple.Branch("ptll",&ptll,"ptll/F");
 ntuple.Branch("sameflav",&sameflav,"sameflav/F");
 ntuple.Branch("w1",&w1,"w1/F");
 ntuple.Branch("w2",&w2,"w2/F");
 ntuple.Branch("w3",&w3,"w3/F");
 ntuple.Branch("w4",&w4,"w4/F");
 ntuple.Branch("w5",&w5,"w5/F");
 ntuple.Branch("w6",&w6,"w6/F");
 ntuple.Branch("w7",&w7,"w7/F");
 ntuple.Branch("w8",&w8,"w8/F");
 ntuple.Branch("numb",&numb,"numb/I");
 ntuple.Branch("numt",&numt,"numt/I");

 std::cout << " reading " << fileNameLHE << std::endl;

 // loop over events
 while ( reader.readEvent () ) {
  ieve++;
  if (ieve % 10000 == 0) std::cout << "event " << ieve << "\n" ;

  TLorentzVector Higgs;
  int iPartHiggs = -1;

  mH = -99;
  mWW = -99;
  mjj = -99;
  detajj = -99;
  jetpt1 = -99;
  jetpt2 = -99;
  jeteta1 = -99;
  jeteta2 = -99; 
  pt1 = -99;
  pt2 = -99;
  eta1 = -99;
  eta2 = -99;
  mll = -99;
  ptll = -99;
  sameflav = -99;
  w1 = -99;  w2 = -99;  w3 = -99;  w4 = -99;  w5 = -99;  w6 = -99;  w7 = -99;  w8 = -99;
  numb = 0;
  numt = 0;

  std::vector<int> finalJets ;
  std::vector<TLorentzVector> v_f_jets ;
  std::vector<TLorentzVector> v_f_quarks ;
  std::vector<TLorentzVector> v_f_leptons ;
  std::vector<TLorentzVector> v_f_neutrinos ;

  int nele = 0 ;
  int nmu = 0 ;

  // loop over particles in the event
  // and fill the variables of leptons and quarks
  for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) {

   // look for the Higgs
   if (abs (reader.hepeup.IDUP.at (iPart)) == 25) {
    iPartHiggs = iPart;
    Higgs.SetPxPyPzE  (
      reader.hepeup.PUP.at (iPart).at (0), // px
      reader.hepeup.PUP.at (iPart).at (1), // py
      reader.hepeup.PUP.at (iPart).at (2), // pz
      reader.hepeup.PUP.at (iPart).at (3) // E
      );
    mH = Higgs.M();
   }

   // outgoing particles
   if (reader.hepeup.ISTUP.at (iPart) == 1) {
    // quarks  or gluons
    if (abs (reader.hepeup.IDUP.at (iPart)) < 7 || abs (reader.hepeup.IDUP.at (iPart)) == 21) {
     finalJets.push_back (iPart) ;
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
       ) ;
     if (abs (reader.hepeup.IDUP.at (iPart)) < 7) {
      v_f_quarks.push_back (dummy) ;
     }
     v_f_jets.push_back (dummy) ;
    }
    else if (abs (reader.hepeup.IDUP.at (iPart)) == 11 || abs (reader.hepeup.IDUP.at (iPart)) == 13) {  // e = 11,   mu = 13
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
       ) ;
     v_f_leptons.push_back (dummy) ;
     nele += (abs (reader.hepeup.IDUP.at (iPart)) == 11) ;
     nmu  += (abs (reader.hepeup.IDUP.at (iPart)) == 13) ;
    }
    else if (abs (reader.hepeup.IDUP.at (iPart)) == 12 || abs (reader.hepeup.IDUP.at (iPart)) == 14) { // ve = 12,   vmu = 14
     TLorentzVector dummy
       (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
       ) ;
     v_f_neutrinos.push_back (dummy) ;
    }
   } // outgoing particles
  } // loop over particles in the event

  if (v_f_jets.size () < 2) {
   std::cout << " what !?!?!?! Not 2 jets? Are you kidding?" << std::endl;
   continue;
  }
  if (v_f_leptons.size () != 2) {
   std::cout << " what !?!?!?! Not 2 leptons? Are you kidding?" << std::endl;
   continue;
  }

  if (nele + nmu != 2) {
   std::cerr << "warning strange things happen\n" ;
   continue ;
  }

  int isSF = -1 ;
  if      (nele < 2 && nmu < 2)         isSF = 0 ;
  else if ((nele == 2) || (nmu == 2))   isSF = 1 ;
  else                              std::cerr << "warning strange things happen\n" ;

  // sorting in pt
  sort (v_f_jets.rbegin (), v_f_jets.rend (), ptsort ()) ;
  sort (v_f_quarks.rbegin (), v_f_quarks.rend (), ptsort ()) ;
  sort (v_f_leptons.rbegin (), v_f_leptons.rend (), ptsort ()) ;

  TLorentzVector diLepton = v_f_leptons.at (0) + v_f_leptons.at (1) ;
  TLorentzVector missingEnergy = v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;

  TLorentzVector dilepton_plus_dineutrinos = v_f_leptons.at (0) + v_f_leptons.at (1) + v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;

  // the sum pf the two jets
//   TLorentzVector dijet = v_f_quarks.at (0) + v_f_quarks.at (1) ;
  TLorentzVector dijet = v_f_jets.at (0) + v_f_jets.at (1) ;

  // sum output fermions
//   TLorentzVector dilepton_plus_dineutrinos_dijets = v_f_leptons.at (0) + v_f_leptons.at (1) + v_f_neutrinos.at (0) + v_f_neutrinos.at (1) +  v_f_quarks.at (0) + v_f_quarks.at (1) ;
  TLorentzVector dilepton_plus_dineutrinos_dijets = v_f_leptons.at (0) + v_f_leptons.at (1) + v_f_neutrinos.at (0) + v_f_neutrinos.at (1) +  v_f_jets.at (0) + v_f_jets.at (1) ;


  // weights
  // ---- the B (=h126) has been calculated with a scale at 126 GeV
  // ---- S+B has been calculated at the "S" scale
  // ---- need to scale B to "S" scale
  //

  double x[2] = {0., 0.} ;
  int flavour[2] = {0, 0} ;

  for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) {
   if (reader.hepeup.ISTUP.at (iPart) == -1) { // incoming particles
    TLorentzVector dummy = buildP (reader.hepeup, iPart);
    x[iPart] = dummy.P () / 4000. ;
    flavour[iPart] = reader.hepeup.IDUP.at (iPart) ;
   }
  }

  float weight[10] =         { 1.,  1.,  1.,  1.,   1.,   1.,   1.};
  float referenceScale[10] = {350, 500, 650, 800, 1000,  250,  300};
  float scale = reader.hepeup.SCALUP ;
  if (referenceScale != 0 ) {
//    std::cout << " scale = " << scale << " :: x[0] = " << x[0] << ", flavour[0] = " << flavour[0] << " x[1] = " << x[1] << ", flavour[1] = " << flavour[1] << std::endl;
   for (int iMass = 0; iMass < 7; iMass++) {
    weight[iMass] = LHAPDF::xfx (x[0], globalScale * referenceScale[iMass], flavour[0]) * LHAPDF::xfx (x[1], globalScale * referenceScale[iMass], flavour[1]) / (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;
//     std::cout << " >> weight[" << iMass << "] = " << weight[iMass] << " = " << LHAPDF::xfx (x[0], referenceScale[iMass], flavour[0])  << " * " << LHAPDF::xfx (x[1], referenceScale[iMass], flavour[1]) << " / " << " ( " << LHAPDF::xfx (x[0], scale, flavour[0]) << " * " << LHAPDF::xfx (x[1], scale, flavour[1]) << " ) " << std::endl;
   }
  }

//   float sum_vect_pt = dilepton_plus_dineutrinos_dijets.Pt();
//   weight[7] = LHAPDF::xfx (x[0], sum_vect_pt, flavour[0]) * LHAPDF::xfx (x[1], sum_vect_pt, flavour[1]) / (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;

  float sum_pt = sqrt( ( v_f_leptons.at(0).Pt()*v_f_leptons.at(0).Pt() + v_f_leptons.at(1).Pt()*v_f_leptons.at(1).Pt() + v_f_neutrinos.at(0).Pt()*v_f_neutrinos.at(0).Pt() + v_f_neutrinos.at(1).Pt()*v_f_neutrinos.at(1).Pt() +  v_f_quarks.at(0).Pt()*v_f_quarks.at(0).Pt() + v_f_quarks.at(1).Pt()*v_f_quarks.at(1).Pt() ) / 6. + 80.385*80.385) ;

  weight[7] = LHAPDF::xfx (x[0], sum_pt, flavour[0]) * LHAPDF::xfx (x[1], sum_pt, flavour[1]) / (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;

  //---- b quarks
  numb = 0;
  // loop over particles in the event
  for (unsigned int  iPart = 0 ; iPart < reader.hepeup.IDUP.size (); iPart++) {
   // outgoing particles
   if (reader.hepeup.ISTUP.at (iPart) == 1) {
    // b quarks
    if (abs (reader.hepeup.IDUP.at (iPart)) == 5) {
     numb++;
    }
   }
  }

  //---- top quarks
  numt = 0;
  // loop over particles in the event
  for (unsigned int  iPart = 0 ; iPart < reader.hepeup.IDUP.size (); iPart++) {
   // intermediate particles
   if (reader.hepeup.ISTUP.at (iPart) == 2) {
    // top quarks
    if (abs (reader.hepeup.IDUP.at (iPart)) == 6) {
     numt++;
    }
   }
  }


  //---- variables
  mWW = dilepton_plus_dineutrinos.M();
  mjj = dijet.M () ;
  detajj = fabs (v_f_jets.at (0).Eta() - v_f_jets.at (1).Eta());
  jetpt1 = v_f_quarks.at (0).Pt ();
  jetpt2 = v_f_quarks.at (1).Pt ();
  jeteta1 = v_f_quarks.at (0).Eta ();
  jeteta2 = v_f_quarks.at (1).Eta ();
  pt1 = v_f_leptons.at (0).Pt ();
  pt2 = v_f_leptons.at (1).Pt ();
  eta1 = v_f_leptons.at (0).Eta ();
  eta2 = v_f_leptons.at (1).Eta ();
  mll = diLepton.M ();
  ptll = diLepton.Pt ();
  sameflav = 1. * isSF;
  w1 = weight[0];  w2 = weight[1];  w3 = weight[2];  w4 = weight[3];
  w5 = weight[4];  w6 = weight[5];  w7 = weight[6];  w8 = weight[7];

  ntuple.Fill ();

 } // loop over events

}



int main (int argc, char **argv) { 
 // Open a stream connected to an event file:
 if (argc < 3) exit (1) ;

 std::cout << " Input  LHE  = " << argv[1] << std::endl;
 std::cout << " Output ROOT = " << argv[2] << std::endl;

 float globalScale;
 if (argc >=4) globalScale = atof(argv[3]);
 else globalScale = 1.;

 std::cout << " globalScale = " << globalScale << std::endl;

 const int SUBSET = 0 ;
 const std::string NAME = "cteq6ll" ; //"cteq6l1"

 LHAPDF::initPDFSet (NAME, LHAPDF::LHPDF, SUBSET) ;
 const int NUMBER = LHAPDF::numberPDF () ;

 LHAPDF::initPDF (0) ;


//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:detajj:jetpt1:jetpt2:jeteta1:jeteta2:pt1:pt2:mll:ptll:sameflav:w1:w2:w3:w4:w5");
//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:detajj:jetpt1:jetpt2:pt1:pt2:mll:sameflav:w1:w2:w3:w4:w5:w6:w7");
//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:jetpt1:jetpt2:pt1:pt2:sameflav:w1:w2:w3:w4:w5:w6:w7");
//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:jetpt1:jetpt2:pt1:pt2:w1:w2:w3:w4:w5:w6:w7:w8");
//  fillNtuple (argv[1], ntu, globalScale) ;

 TTree ntu("ntu","ntu");
 fillNtuple (argv[1], ntu, globalScale) ;

 TFile output (argv[2], "recreate") ;
 output.cd() ;
 ntu.Write();
 output.Close();

}



