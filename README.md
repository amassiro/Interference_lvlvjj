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

    c++ -o ntupleMaker.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp
    ./ntupleMaker.exe   blabla.lhe    blabla.root

   ./ntupleMaker.exe   gen_126/gen2jmu-e+/total.lhe    jjme_126.root



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






