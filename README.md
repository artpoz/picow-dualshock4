# picow-dualshock4
```
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init 
cd ..
git clone https://github.com/artpoz/picow-dualshock4.git
cd picow-dualshock4
rm -rf build
mkdir build
cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=/home/artpoz/pico-sdk ..
make

```

The .uf2 file will be build/src/picow_dualshock4.uf2

## VIDEO

https://www.youtube.com/watch?v=-3g0xX3HrAQ
