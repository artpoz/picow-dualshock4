#! /bin/bash

#rm -rf build
#mkdir build
cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=/home/artpoz/pico-sdk ../
make
