#! /bin/bash

#rm -rf build; mkdir build; cd build; cmake -DDEBUG=1 ..; reset; make -j4; cd ..;
#./build/examples/meemd_test
rm -rf build; mkdir build; cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; 
