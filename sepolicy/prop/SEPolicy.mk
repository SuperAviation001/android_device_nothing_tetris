# Board specific SELinux policy variable definitions
MTK_SEPOLICY_PATH := device/nothing/tetris/sepolicy/prop

BOARD_VENDOR_SEPOLICY_DIRS += \
    $(MTK_SEPOLICY_PATH)/base/vendor \
    $(MTK_SEPOLICY_PATH)/debug/vendor \
    $(MTK_SEPOLICY_PATH)/third_party/vendor

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += \
    $(MTK_SEPOLICY_PATH)/base/private \
    $(MTK_SEPOLICY_PATH)/debug/private \
    $(MTK_SEPOLICY_PATH)/third_party/private

SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += \
    $(MTK_SEPOLICY_PATH)/base/public \
    $(MTK_SEPOLICY_PATH)/debug/public \
    $(MTK_SEPOLICY_PATH)/third_party/public
