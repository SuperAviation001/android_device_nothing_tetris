#
# Copyright (C) 2022 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit some common AOSP stuff.
$(call inherit-product, vendor/aosp/config/common_full_phone.mk)

# Inherit from tetris device
$(call inherit-product, device/nothing/tetris/device.mk)

PRODUCT_NAME := aosp_tetris
PRODUCT_DEVICE := tetris
PRODUCT_BRAND := Nothing
PRODUCT_MANUFACTURER := Nothing
PRODUCT_MODEL := A015

PRODUCT_GMS_CLIENTID_BASE := android-nothing

DEVICE_CODENAME := tetris

TARGET_SCREEN_WIDTH := 1080

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="Tetris 15 AP3A.240905.015 2502082015 release-keys" \
    BuildFingerprint=Nothing/Tetris/Tetris:15/AP3A.240905.015.A2/2502082015:user/release-keys \
    DeviceProduct=$(DEVICE_CODENAME)
