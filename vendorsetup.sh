#!/bin/bash

if [ -d "hardware/mediatek/aidl/power-mediatek" ]; then
  rm -rf hardware/mediatek
  git clone https://github.com/SuperAviation001/android_hardware_mediatek -b lineage-22.2 hardware/mediatek
fi
if [ ! -d "vendor/nothing/tetris" ]; then
  git clone https://gitlab.com/SuperAviation001/android_vendor_nothing_tetris -b a15 vendor/nothing/tetris
fi
if [ ! -d "kernel/nothing/mt6878" ]; then
  git clone https://github.com/SuperAviation001/android_kernel_nothing_tetris -b lineage-22.2 kernel/nothing/mt6878
fi
if [ ! -d "device/mediatek/sepolicy_vndr" ]; then
  git clone https://github.com/LineageOS/android_device_mediatek_sepolicy_vndr.git -b lineage-22.2 device/mediatek/sepolicy_vndr
fi
