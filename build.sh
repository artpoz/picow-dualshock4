rm -rf build
rm -rf picow_dualshock4.uf2

# git submodule update --init --recursive
sed -i -e "s/#define MAX_ATTRIBUTE_VALUE_SIZE 300/#define MAX_ATTRIBUTE_VALUE_SIZE 512/" ./pico-sdk/lib/btstack/src/classic/hid_host.c
mkdir build
cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=../pico-sdk ../
JOBS=$(($(grep cpu.cores /proc/cpuinfo | sort -u | sed 's/[^0-9]//g') + 1))
make -j${JOBS}
# mv ./build/src/picow_dualshock4.uf2 .
