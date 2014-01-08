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



#include "TNtuple.h"
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




void fillNtuple (std::string fileNameLHE,  TNtuple & ntuple) {
 std::ifstream ifs (fileNameLHE.c_str ()) ;
 LHEF::Reader reader (ifs) ;

 long ieve = 0 ;
 long selected = 0 ;
 double jetsNum = 0. ;

 std::cout << " reading " << fileNameLHE << std::endl;

 // loop over events
 while ( reader.readEvent () ) {
  ieve++;
  if (ieve % 10000 == 0) std::cout << "event " << ieve << "\n" ;

  TLorentzVector Higgs;
  int iPartHiggs = -1;
  float mH = 0;

  std::vector<int> finalJets ;
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
    // quarks
    if (abs (reader.hepeup.IDUP.at (iPart)) < 7) {
     finalJets.push_back (iPart) ;
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
       ) ;
     v_f_quarks.push_back (dummy) ;
    } // quarks
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

  if (v_f_quarks.size () < 2) {
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
  sort (v_f_quarks.rbegin (), v_f_quarks.rend (), ptsort ()) ;
  sort (v_f_leptons.rbegin (), v_f_leptons.rend (), ptsort ()) ;

  TLorentzVector diLepton = v_f_leptons.at (0) + v_f_leptons.at (1) ;
  TLorentzVector missingEnergy = v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;

  TLorentzVector dilepton_plus_dineutrinos = v_f_leptons.at (0) + v_f_leptons.at (1) + v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;

  // the sum pf the two quarks
  TLorentzVector dijet = v_f_quarks.at (0) + v_f_quarks.at (1) ;

  // sum output fermions
  TLorentzVector dilepton_plus_dineutrinos_dijets = v_f_leptons.at (0) + v_f_leptons.at (1) + v_f_neutrinos.at (0) + v_f_neutrinos.at (1) +  v_f_quarks.at (0) + v_f_quarks.at (1) ;


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
    weight[iMass] = LHAPDF::xfx (x[0], referenceScale[iMass], flavour[0]) * LHAPDF::xfx (x[1], referenceScale[iMass], flavour[1]) / (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;
//     std::cout << " >> weight[" << iMass << "] = " << weight[iMass] << " = " << LHAPDF::xfx (x[0], referenceScale[iMass], flavour[0])  << " * " << LHAPDF::xfx (x[1], referenceScale[iMass], flavour[1]) << " / " << " ( " << LHAPDF::xfx (x[0], scale, flavour[0]) << " * " << LHAPDF::xfx (x[1], scale, flavour[1]) << " ) " << std::endl;
   }
  }

//   float sum_vect_pt = dilepton_plus_dineutrinos_dijets.Pt();
//   weight[7] = LHAPDF::xfx (x[0], sum_vect_pt, flavour[0]) * LHAPDF::xfx (x[1], sum_vect_pt, flavour[1]) / (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;

  float sum_pt = sqrt( v_f_leptons.at(0).Pt()*v_f_leptons.at(0).Pt() + v_f_leptons.at(1).Pt()*v_f_leptons.at(1).Pt() + v_f_neutrinos.at(0).Pt()*v_f_neutrinos.at(0).Pt() + v_f_neutrinos.at(1).Pt()*v_f_neutrinos.at(1).Pt() +  v_f_quarks.at(0).Pt()*v_f_quarks.at(0).Pt() + v_f_quarks.at(1).Pt()*v_f_quarks.at(1).Pt() ) / 6. + 80.385 ;

  weight[7] = LHAPDF::xfx (x[0], sum_pt, flavour[0]) * LHAPDF::xfx (x[1], sum_pt, flavour[1]) / (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;

  ntuple.Fill (
    mH ,
    dilepton_plus_dineutrinos.M() ,
    dijet.M () ,
//     fabs (v_f_quarks.at (0).Eta() - v_f_quarks.at (1).Eta()),
    v_f_quarks.at (0).Pt (),
    v_f_quarks.at (1).Pt (),
//     v_f_quarks.at (0).Eta (),
//     v_f_quarks.at (1).Eta (), 
    v_f_leptons.at (0).Pt (),
    v_f_leptons.at (1).Pt (),
//     diLepton.M (),
//     diLepton.Pt (),
//     isSF,
    weight[0],  //  350  w1
    weight[1],  //  500  w2
    weight[2],  //  650  w3
    weight[3],  //  800  w4
    weight[4],  // 1000  w5
    weight[5],  //  250  w6
    weight[6],  //  300  w7
    weight[7]   //       w8 dynamic running scale
    ) ;

 } // loop over events

}



int main (int argc, char **argv) { 
 // Open a stream connected to an event file:
 if (argc < 3) exit (1) ;

 std::cout << " Input  LHE  =" << argv[1] << std::endl;
 std::cout << " Output ROOT =" << argv[2] << std::endl;

 
 const int SUBSET = 0 ;
 const std::string NAME = "cteq6ll" ; //"cteq6l1"

 LHAPDF::initPDFSet (NAME, LHAPDF::LHPDF, SUBSET) ;
 const int NUMBER = LHAPDF::numberPDF () ;

 LHAPDF::initPDF (0) ;


//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:detajj:jetpt1:jetpt2:jeteta1:jeteta2:pt1:pt2:mll:ptll:sameflav:w1:w2:w3:w4:w5");
//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:detajj:jetpt1:jetpt2:pt1:pt2:mll:sameflav:w1:w2:w3:w4:w5:w6:w7");
//  TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:jetpt1:jetpt2:pt1:pt2:sameflav:w1:w2:w3:w4:w5:w6:w7");
 TNtuple ntu ("ntu", "ntu", "mH:mWW:mjj:jetpt1:jetpt2:pt1:pt2:w1:w2:w3:w4:w5:w6:w7:w8");
 fillNtuple (argv[1], ntu) ;

 TFile output (argv[2], "recreate") ;
 output.cd() ;
 ntu.Write();
 output.Close();

}



