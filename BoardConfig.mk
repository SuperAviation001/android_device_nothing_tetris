#
# Copyright (C) 2025 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

DEVICE_PATH := device/nothing/tetris

# A/B
AB_OTA_UPDATER := true
AB_OTA_PARTITIONS += \
    system \
    product \
    system_dlkm \
    vendor \
    vbmeta_system \
    odm \
    boot \
    vbmeta_vendor
BOARD_USES_RECOVERY_AS_BOOT := true

# Architecture
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := generic
TARGET_CPU_VARIANT_RUNTIME := cortex-a55

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := Tetris
TARGET_NO_BOOTLOADER := true

# Display
TARGET_SCREEN_DENSITY := 420

# Kernel
TARGET_KERNEL_CONFIG := mssi_64_64only_ww_armv82_defconfig

# Kernel - prebuilt
TARGET_FORCE_PREBUILT_KERNEL := true
ifeq ($(TARGET_FORCE_PREBUILT_KERNEL),true)
TARGET_PREBUILT_KERNEL := $(DEVICE_PATH)/prebuilts/kernel
TARGET_PREBUILT_DTB := $(DEVICE_PATH)/prebuilts/dtb.img
BOARD_MKBOOTIMG_ARGS += --dtb $(TARGET_PREBUILT_DTB)
BOARD_INCLUDE_DTB_IN_BOOTIMG := 
BOARD_PREBUILT_DTBOIMAGE := $(DEVICE_PATH)/prebuilts/dtbo.img
BOARD_KERNEL_SEPARATED_DTBO := false
endif

# Partitions
BOARD_FLASH_BLOCK_SIZE := 262144 # (BOARD_KERNEL_PAGESIZE * 64)
BOARD_BOOTIMAGE_PARTITION_SIZE := 67108864
BOARD_DTBOIMG_PARTITION_SIZE := 8388608
BOARD_INIT_BOOT_IMAGE_PARTITION_SIZE := 8388608
BOARD_VENDOR_BOOTIMAGE_PARTITION_SIZE := 67108864
BOARD_SUPER_PARTITION_SIZE := 9126805504 # TODO: Fix hardcoded value
BOARD_SUPER_PARTITION_GROUPS := alps_dynamic_partitions
BOARD_ALPS_DYNAMIC_PARTITIONS_PARTITION_LIST := \
    system \
    product \
    system_dlkm \
    vendor \
    odm
BOARD_ALPS_DYNAMIC_PARTITIONS_SIZE := 9122611200 # TODO: Fix hardcoded value

# Platform
TARGET_BOARD_PLATFORM := mt6878

# Properties
TARGET_SYSTEM_PROP += $(DEVICE_PATH)/system.prop
TARGET_VENDOR_PROP += $(DEVICE_PATH)/vendor.prop
TARGET_PRODUCT_PROP += $(DEVICE_PATH)/product.prop
TARGET_SYSTEM_EXT_PROP += $(DEVICE_PATH)/system_ext.prop
TARGET_SYSTEM_DLKM_PROP += $(DEVICE_PATH)/system_dlkm.prop
TARGET_ODM_PROP += $(DEVICE_PATH)/odm.prop
TARGET_ODM_DLKM_PROP += $(DEVICE_PATH)/odm_dlkm.prop
TARGET_VENDOR_DLKM_PROP += $(DEVICE_PATH)/vendor_dlkm.prop

# Recovery
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/rootdir/etc/fstab.mt6878
TARGET_RECOVERY_PIXEL_FORMAT := BGRA_8888
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true

# Security patch level
VENDOR_SECURITY_PATCH := 2024-05-05

# Verified Boot
BOARD_AVB_ENABLE := true
#BOARD_AVB_MAKE_VBMETA_IMAGE_ARGS += --flags 3
BOARD_AVB_VENDOR_BOOT_KEY_PATH := external/avb/test/data/testkey_rsa4096.pem
BOARD_AVB_VENDOR_BOOT_ALGORITHM := SHA256_RSA4096
BOARD_AVB_VENDOR_BOOT_ROLLBACK_INDEX := 1
BOARD_AVB_VENDOR_BOOT_ROLLBACK_INDEX_LOCATION := 1

BOARD_AVB_VBMETA_VENDOR_KEY_PATH := external/avb/test/data/testkey_rsa4096.pem
BOARD_AVB_VBMETA_VENDOR_ALGORITHM := SHA256_RSA4096
BOARD_AVB_VBMETA_VENDOR_ROLLBACK_INDEX := 1
BOARD_AVB_VBMETA_VENDOR_ROLLBACK_INDEX_LOCATION := 1


# Fix "overriding commands for target..."
BUILD_BROKEN_DUP_RULES := true

# Fix "ELF Prebuilts in product copy files ..."
BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true

# VINTF
 DEVICE_MANIFEST_FILE += $(DEVICE_PATH)/manifest.xml

# Define prebuilt ODM. Need commit from 'https://github.com/StagOS/android_build_make/commit/443f8e964e096f70be2dfd4a4f68610d530606b3' in build/core/Makefile for it to work.
BOARD_PREBUILT_ODMIMAGE := $(DEVICE_PATH)/prebuilts/odm.img
TARGET_COPY_OUT_ODM := odm

# Define prebuilt Product
BOARD_PREBUILT_PRODUCTIMAGE :=  $(DEVICE_PATH)/prebuilts/product.img
TARGET_COPY_OUT_PRODUCT := product

# Define prebuilt System_DLKM
BOARD_PREBUILT_SYSTEM_DLKMIMAGE :=  $(DEVICE_PATH)/prebuilts/system_dlkm.img
TARGET_COPY_OUT_SYSTEM_DLKM := system_dlkm

# Define prebuilt Vendor
BOARD_PREBUILT_VENDORIMAGE := $(DEVICE_PATH)/prebuilts/vendor.img
TARGET_COPY_OUT_VENDOR := vendor

# Define prebuilt DTB
BOARD_PREBUILT_DTBIMAGE := $(DEVICE_PATH)/prebuilts/dtb.img
TARGET_COPY_OUT_DTB := dtb

# Specify to build images. DO NOT UNCOMMENT PREBUILTS!!
  IMAGES_TO_BUILD += vendor_boot
#  IMAGES_TO_BUILD += vendor_dlkm
  IMAGES_TO_BUILD += vbmeta
  IMAGES_TO_BUILD += userdata
  IMAGES_TO_BUILD += system
#  IMAGES_TO_BUILD += system_dlkm
#  IMAGES_TO_BUILD += dtb
# IMAGES_TO_BUILD += odm_dlkm
  IMAGES_TO_BUILD += init_boot
  IMAGES_TO_BUILD += boot

# Inherit the proprietary files
#include vendor/nothing/tetris/BoardConfigVendor.mk
