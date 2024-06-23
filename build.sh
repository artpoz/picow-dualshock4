rm -rf build
git submodule update --init --recursive
sed -i -e "s/#define MAX_ATTRIBUTE_VALUE_SIZE 300/#define MAX_ATTRIBUTE_VALUE_SIZE 512/" ./pico-sdk/lib/btstack/src/classic/hid_host.c
mkdir build
cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=../pico-sdk ../
make
