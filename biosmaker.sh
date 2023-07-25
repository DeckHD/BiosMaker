#!/bin/bash

./uninsyde $1
rm DRV_IMG.bin BIOSCER.bin BIOSCR2.bin INI_IMG.bin
g++ patcher.cpp -o patcher
./patcher
if [ $? -ne 0 ] 
then
    echo "FAILED!!!"
	exit -1
fi
./UEFIReplace BIOSIMG_DeckHD_EC.bin F0DA323C-43A4-48DB-AEFE-CB314F7F5F6E 0x19 Splash.png -o  $(basename -- "$1")_DeckHD.bin -all
