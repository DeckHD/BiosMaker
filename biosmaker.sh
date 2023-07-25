#!/bin/bash

./uninsyde $1
if [ $? -ne 0 ]
then
    echo "FAILED!!!"
    exit -1
fi
rm DRV_IMG.bin BIOSCER.bin BIOSCR2.bin INI_IMG.bin

./UEFIReplace BIOSIMG.bin F0DA323C-43A4-48DB-AEFE-CB314F7F5F6E 0x19 Splash.png -o  BIOSIMG_SPLASH.bin -all
if [ $? -ne 0 ]
then
    echo "FAILED!!!"
    exit -1
fi

biosver=$(basename -- "$1")
biosver=${biosver/_sign.fd/""}

g++ patcher.cpp -o patcher
if [ $? -ne 0 ]
then
    echo "FAILED!!!"
    exit -1
fi
./patcher BIOSIMG_SPLASH.bin ${biosver}_DeckHD.bin
if [ $? -ne 0 ]
then
    echo "FAILED!!!"
    exit -1
fi
rm BIOSIMG.bin BIOSIMG_SPLASH.bin
