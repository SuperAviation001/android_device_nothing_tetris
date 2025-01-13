#
# Copyright (C) 2025 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit some common Lineage stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Inherit from Nothing Tetris (CMF Phone 1) device
$(call inherit-product, device/nothing/tetris/device.mk)

PRODUCT_DEVICE := tetris
PRODUCT_NAME := lineage_tetris
PRODUCT_BRAND := Nothing
PRODUCT_MODEL := A015
PRODUCT_MANUFACTURER := Nothing

PRODUCT_GMS_CLIENTID_BASE := android-nothing

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRIVATE_BUILD_DESC="tetris 14 UP1A.231005.007 2407022200 release-keys"

BUILD_FINGERPRINT := nothing/tetris/tetris:14/UP1A.231005.007/2407022200:user/release-keys
