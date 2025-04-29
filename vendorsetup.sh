#!/bin/bash
if [ ! -d "hardware/mediatek" ]; then
  git clone https://github.com/SuperAviation001/android_hardware_mediatek -b lineage-22.2 hardware/mediatek
fi
if [ ! -d "vendor/nothing/tetris" ]; then
  git clone https://gitlab.com/SuperAviation001/android_vendor_nothing_tetris -b a15 vendor/nothing/tetris
fi
if [ ! -d "kernel/nothing/tetris" ]; then
  git clone https://github.com/NothingOSS/android_kernel_6.1_nothing_mt6878 -b mt6878/Tetris/u kernel/nothing/tetris
fi
if [ ! -d "device/nothing/tetris-kernel" ]; then
  git clone https://github.com/SuperAviation001/android_device_nothing_tetris-kernel -b 15_v2 device/nothing/tetris-kernel
fi
