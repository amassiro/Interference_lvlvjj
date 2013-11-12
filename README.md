Interference lvlvjj
===================

Interference for vbf h>WW>lvlv with ww+2jets

Working here:

    /home/amassiro/Interference


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



Plot:

    root -l Draw.cxx
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





To get the cross-sections then hardcoded in Draw.cxx

    cd gen_350

    for fol in `ls --color=none | grep -v gen` ; do \
      echo $fol ; \
      tail -n 1 $fol/result ; \
      tail -n 1 gen$fol/result ; \
    done;



