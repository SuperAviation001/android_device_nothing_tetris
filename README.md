# Android device tree for the Nothing CMF Phone 1 (nothing_tetris) 
## Usage:
Clone to device/nothing/tetris
then:

```
cd device/nothing/tetris
chmod +x setup-makefiles.sh
chmod +x extract-files.sh

```
## After this: execute these commands, make sure device is connected via ADB and booted into the system (if you wish to make your own vendor tree):
Instead of this, you could clone the vendor tree I am providing into vendor/nothing/tetris
```
./extract-files.sh
./setup-makefiles.sh

```
# WARNING 
# IF YOU DID THE ABOVE STEP, YOU DO NOT NEED TO CLONE THE VENDOR TREE !!

## Kernel Source
### Kernel source is available at:
https://github.com/NothingOSS/android_kernel_6.1_nothing_mt6878
### Clone it into device/nothing/tetris-kernel

# Enjoy!!
I will build some roms for this device btw.
```
#
# Copyright (C) 2025 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#
```
