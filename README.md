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
    ls -d  gen_*/ --color=none | awk '{print "cd "$1"/gen2jmu-e+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\";  cd -; cd "$1"/gen2jmu-mu+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -;"}'
    ls -d  gen_*/ --color=none | awk '{print "cd "$1"/gen2jmu-e+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\";  cd -; cd "$1"/gen2jmu-mu+/; ../../../LHEActions/mergeLHEfiles  `find . -name \"phamom.dat\"`;  cd -;"}' | /bin/sh


    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-e+/total.lhe  "$1"_jjme.root"}'
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-e+/total.lhe  "$1"_jjme.root"}' | /bin/sh

    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-mu+/total.lhe  "$1"_jjmm.root"}'
    ls -d  gen_*/ --color=none  | tr "/" " " | awk '{print "./ntupleMaker.exe   "$1"/gen2jmu-mu+/total.lhe  "$1"_jjmm.root"}' | /bin/sh



Plot:

    root -l Draw.cxx








cfg 1, 2
S 1 SF
S 1 DF
S 2 SF
S 2 DF
S 3 SF
S 3 DF
126
xx
x
x
x
x
1.81380874131530090E-002  +/-  1.28243141770084274E-005 
3.58829916258534554E-002  +/-  2.53689338772571570E-005 
350
xx
x
x
x
x
1.69704956339057321E-002  +/-  1.19989820384445323E-005 
3.30573359105044151E-002  +/-  2.33671980835533764E-005 
500
xx
x
x
x
x
1.56725248231346197E-002  +/-  1.12515631381070598E-005 
3.08353695382097698E-002  +/-  2.17995313962022446E-005 
650
xx
x
x
x
x
1.54272529321224164E-002  +/-  1.09071518849106450E-005 
3.04248483997987193E-002  +/-  2.15059855817530323E-005 
800
xx
x
x
x
x
1.50076130007520663E-002  +/-  1.06101411767053291E-005 
2.96581397883867830E-002  +/-  2.09646224095354691E-005 
1000
xx
x
x
x
x
1.49636481256262849E-002  +/-  1.05794974486291771E-005 
2.96046079632492319E-002  +/-  2.09289686253768846E-005




for fol in `ls | grep -v gen` ; do \
  echo $fol ; \
  tail -n 1 $fol/result ; \
  tail -n 1 gen$fol/result ; \
done;