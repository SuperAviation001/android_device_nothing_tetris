#
# Copyright (C) 2024 E FOUNDATION
#
# SPDX-License-Identifier: Apache-2.0
#

MODULES_PATH := device/nothing/tetris/configs/kernel

# system_dlkm modules
DLKM_MODULES_ORIG := $(MODULES_PATH)/system_dlkm
DLKM_MODULES_DEST := $(TARGET_COPY_OUT_SYSTEM_DLKM)/lib/modules

PRODUCT_COPY_FILES += \
    $(call find-copy-subdir-files,*,$(DLKM_MODULES_ORIG)/,$(DLKM_MODULES_DEST))

# DLKM modules
DLKM_MODULES_ORIG := $(MODULES_PATH)/vendor_dlkm
DLKM_MODULES_DEST := $(TARGET_COPY_OUT_VENDOR_DLKM)/lib/modules

PRODUCT_COPY_FILES += \
    $(call find-copy-subdir-files,*,$(DLKM_MODULES_ORIG)/,$(DLKM_MODULES_DEST))

# vendor_boot modules
VENDOR_RAMDISK_KERNEL_MODULES_ORIG := $(MODULES_PATH)/vendor_boot
VENDOR_RAMDISK_KERNEL_MODULES_DEST := $(TARGET_COPY_OUT_VENDOR_RAMDISK)/lib/modules

PRODUCT_COPY_FILES += \
    $(call find-copy-subdir-files,*,$(VENDOR_RAMDISK_KERNEL_MODULES_ORIG)/,$(VENDOR_RAMDISK_KERNEL_MODULES_DEST))
