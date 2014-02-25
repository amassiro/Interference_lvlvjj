Interference lvlvjj
===================

Interference for vbf h>WW>lvlv with ww+2jets

Working here:

    cmsneu
    /home/amassiro/Interference/Interference_lvlvjj/

Copy the LHE utilities from

    https://github.com/govoni/LHEActions

In particular

    mergeLHEfiles.cpp
    c++ -o mergeLHEfiles mergeLHEfiles.cpp


Dump sensitive information into a root file

    c++ -o ntupleMaker.exe `root-config --glibs --cflags` `lhapdf-config --cppflags  --ldflags` -lm ntupleMaker.cpp
    ./ntupleMaker.exe   blabla.lhe    blabla.root

   ./ntupleMaker.exe   gen_126/gen2jmu-e+/total.lhe    jjme_126.root



NB: additional packages needed:

    LHAPDF

it is needed to rescale the "B" only ( = h126) to "S+B", that has been
generated at "S" scale

Install from :

    https://lhapdf.hepforge.org/install
    lhapdf-5.9.1.tar.gz

and do:

    export BOOST_ROOT=/afs/cern.ch/sw/lcg/external/Boost/1.50.0_python2.6/x86_64-slc5-gcc43-opt/
    export BOOST_INCLUDEDIR=/afs/cern.ch/sw/lcg/external/Boost/1.50.0_python2.6/x86_64-slc5-gcc43-opt/include/boost-1_50/
    export BOOST_LIBRARYDIR=$BOOST_ROOT/lib
    export LD_LIBRARY_PATH=$BOOST_LIBRARYDIR:$LD_LIBRARY_PATH
    export BOOSTFLAGS_ENV="-I${BOOST_ROOT}include/boost-1_50"
    export BOOSTLIBS_ENV="-L${BOOST_ROOT}lib -lboost_program_options-gcc43-mt-1_50"
    ./configure  --with-boost=/afs/cern.ch/sw/lcg/external/Boost/1.50.0_python2.6/x86_64-slc5-gcc43-opt/  
    ./configure  --with-boost=/afs/cern.ch/sw/lcg/external/Boost/1.50.0_python2.6/x86_64-slc5-gcc43-opt/  -enable-pdfsets=mrst,cteq
    lhapdf-getdata
    lhapdf-getdata CTEQ6ll CTEQ66 cteq6ll lomod MCal --dest=/usr/local/share/lhapdf
    lhapdf-getdata CTEQ6ll CTEQ66 cteq6ll lomod MCal --dest=/usr/share/lhapdf/PDFsets/
    make
    su
    make install


To do all:

B and S+B

    eos ls  /eos/cms/store/user/govoni/LHE/phantom/lvlv
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_1000.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_800.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_650.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_500.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_350.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_300.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_250.tgz ./
    eos cp  /eos/cms/store/user/govoni/LHE/phantom/lvlv/gen_126.tgz ./

    ls *.tgz --color=none  | awk '{print "tar -xzf "$1}'
    ls *.tgz --color=none  | awk '{print "tar -xzf "$1}' | /bin/sh

    cd gen_126/gen2jmu-e+/
    ../../../LHEActions/mergeLHEfiles  `find . -name "phamom.dat"
    cd -
    cd gen_126/gen2jmu-mu+/
    ../../../LHEActions/mergeLHEfiles  `find . -name "phamom.dat"
    cd -


    ls -d  gen_*/ --color=none | awk '{print "cd "$1"/gen2jmu-mu+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -;"}'
    ls -d  gen_*/ --color=none | awk '{print "cd "$1"/gen2jmu-e+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -; cd "$1"/gen2jmu-mu+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -;"}'
    ls -d  gen_*/ --color=none | awk '{print "cd "$1"/gen2jmu-e+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -; cd "$1"/gen2jmu-mu+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -;"}' | /bin/sh


    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-e+/total.lhe  "$1"_jjme.root"}'
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-e+/total.lhe  "$1"_jjme.root"}' | /bin/sh

    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-mu+/total.lhe  "$1"_jjmm.root"}'
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-mu+/total.lhe  "$1"_jjmm.root"}' | /bin/sh


    # old
    ls qqHWWevuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   qqHWWevuv/"$1"_"$2"."$3"  S_"$2"_jjme.root"}'
    ls qqHWWuvuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   qqHWWuvuv/"$1"_"$2"."$3"  S_"$2"_jjmm.root"}'
    # old (end)

    ls qqHWWevuv/ --color=none  | awk '{print "cat   qqHWWevuv/"$1" | grep Integrated"}' 
    ls qqHWWuvuv/ --color=none  | awk '{print "cat   qqHWWuvuv/"$1" | grep Integrated"}' 

    ls HWWevuv/ --color=none  | awk '{print "cat   HWWevuv/"$1" | grep Integrated"}' 
    ls HWWuvuv/ --color=none  | awk '{print "cat   HWWuvuv/"$1" | grep Integrated"}' 

    ls HWWevuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWevuv/"$1"_"$2"_"$3".lhe  S_"$2"_jjme.root"}'
    ls HWWuvuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWuvuv/"$1"_"$2"_"$3".lhe  S_"$2"_jjmm.root"}'
    ls HWWevuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWevuv/"$1"_"$2"_"$3".lhe  S_"$2"_jjme.root"}' | /bin/sh
    ls HWWuvuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWuvuv/"$1"_"$2"_"$3".lhe  S_"$2"_jjmm.root"}' | /bin/sh


Scale up/down:

    rm doScaleVariation.sh
    touch  doScaleVariation.sh
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-e+/total.lhe  scaleDown/"$1"_jjme.root   0.5"}' >> doScaleVariation.sh 
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-mu+/total.lhe  scaleDown/"$1"_jjmm.root   0.5"}'  >> doScaleVariation.sh
    ls HWWevuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWevuv/"$1"_"$2"_"$3".lhe  scaleDown/S_"$2"_jjme.root   0.5"}' >> doScaleVariation.sh
    ls HWWuvuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWuvuv/"$1"_"$2"_"$3".lhe  scaleDown/S_"$2"_jjmm.root   0.5"}' >> doScaleVariation.sh

    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-e+/total.lhe  scaleUp/"$1"_jjme.root   2.0"}' >> doScaleVariation.sh
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-mu+/total.lhe  scaleUp/"$1"_jjmm.root   2.0"}' >> doScaleVariation.sh
    ls HWWevuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWevuv/"$1"_"$2"_"$3".lhe  scaleUp/S_"$2"_jjme.root   2.0"}' >> doScaleVariation.sh
    ls HWWuvuv/ --color=none  | tr "_" " " | tr "." " " | awk '{print "./ntupleMaker.exe   HWWuvuv/"$1"_"$2"_"$3".lhe  scaleUp/S_"$2"_jjmm.root   2.0"}' >> doScaleVariation.sh

    sh doScaleVariation.sh



Plot: (0 = em, 1 = mm)

    root -l Draw.cxx

    root -l Draw.cxx\(0,250\)
    root -l Draw.cxx\(1,250\)

    root -l Draw.cxx\(0,300\)
    root -l Draw.cxx\(1,300\)

    root -l Draw.cxx\(0,350,0\)
    root -l Draw.cxx\(0,350\)
    root -l Draw.cxx\(1,350\)

    root -l Draw.cxx\(0,500\)
    root -l Draw.cxx\(1,500\)

    root -l Draw.cxx\(0,650\)
    root -l Draw.cxx\(1,650\)

    root -l Draw.cxx\(0,800\)
    root -l Draw.cxx\(1,800\)

    root -l Draw.cxx\(0,1000\)
    root -l Draw.cxx\(1,1000\)



Scale up: (0 = em, 1 = mm)

    root -l Draw.cxx\(0,350,1,1\)
    root -l Draw.cxx\(1,350,1,1\)

    root -l Draw.cxx\(0,500,1,1\)
    root -l Draw.cxx\(1,500,1,1\)

    root -l Draw.cxx\(0,650,1,1\)
    root -l Draw.cxx\(1,650,1,1\)

    root -l Draw.cxx\(0,800,1,1\)
    root -l Draw.cxx\(1,800,1,1\)

    root -l Draw.cxx\(0,1000,1,1\)
    root -l Draw.cxx\(1,1000,1,1\)


Scale down: (0 = em, 1 = mm)

    root -l Draw.cxx\(0,350,1,-1\)
    root -l Draw.cxx\(1,350,1,-1\)

    root -l Draw.cxx\(0,500,1,-1\)
    root -l Draw.cxx\(1,500,1,-1\)

    root -l Draw.cxx\(0,650,1,-1\)
    root -l Draw.cxx\(1,650,1,-1\)

    root -l Draw.cxx\(0,800,1,-1\)
    root -l Draw.cxx\(1,800,1,-1\)

    root -l Draw.cxx\(0,1000,1,-1\)
    root -l Draw.cxx\(1,1000,1,-1\)



To get the cross-sections then hardcoded in Draw.cxx for phantom

    cd gen_350

    for fol in `ls --color=none | grep -v gen` ; do \
      echo $fol ; \
      tail -n 1 $fol/result ; \
      tail -n 1 gen$fol/result ; \
    done;




To plot the result:

    root -l PlotInterference.cxx
    root -l PlotInterference.cxx\(1\)

scale up:

    root -l PlotInterference.cxx\(0,1\)
    root -l PlotInterference.cxx\(1,1\)

scale down:

    root -l PlotInterference.cxx\(0,-1\)
    root -l PlotInterference.cxx\(1,-1\)




Then copy to common repository

    ls output/ | awk '{print "cp output/"$1"/*.root ../Interference_VBF/data/"$1}'


To be then uploaded to:

    /home/amassiro/Latinos/CMSSW_5_3_13_patch1/src/HWWAnalysis/ShapeAnalysis/ewksinglet/data/InterferenceVBF/




Plot
=======

Just draw:

    r99t DrawDistribution.cxx\(\"gen_126_jjme.root\",\"jetpt1\",200,0,2000,\"jetpt1\ [GeV]\"\)
    r99t DrawDistribution.cxx\(\"gen_126_jjme.root\",\"mjj\",200,10,2000,\"mjj\"\)
    r99t DrawDistribution.cxx\(\"gen_800_jjme.root\",\"mjj\",200,10,2000,\"mjj\"\)
    r99t DrawDistribution.cxx\(\"S_mH800_jjme.root\",\"mjj\",400,10,4000,\"mjj\"\)



Comparison with VBFNLO: (0 = em, 1 = mm)

    root -l DrawVBFNLO.cxx\(0,800\)
    root -l DrawVBFNLO.cxx\(1,800\)



