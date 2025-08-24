
#
# Copyright (C) 2022 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit common lineage stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Inherit from tetris device
$(call inherit-product, device/nothing/tetris/device.mk)

PRODUCT_NAME := lineage_tetris
PRODUCT_DEVICE := tetris
PRODUCT_BRAND := Nothing
PRODUCT_MANUFACTURER := Nothing
PRODUCT_MODEL := A015

PRODUCT_GMS_CLIENTID_BASE := android-nothing

DEVICE_CODENAME := tetris

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="Tetris 14 UP1A.231005.007 2412042338 release-keys" \
    BuildFingerprint=Nothing/Tetris/Tetris:14/UP1A.231005.007/2412042338:user/release-keys \
    DeviceProduct=$(DEVICE_CODENAME)
