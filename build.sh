cd picow_ds4
git submodule update --init --recursive
mkdir build
cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=../pico-sdk .
make
