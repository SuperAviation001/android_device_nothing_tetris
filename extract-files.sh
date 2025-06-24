#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE=tetris
VENDOR=nothing

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

ANDROID_ROOT="${MY_DIR}/../../.."

HELPER="$PWD/extract-utils/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true

KANG=
SECTION=

while [ "${#}" -gt 0 ]; do
    case "${1}" in
        -n | --no-cleanup )
                CLEAN_VENDOR=false
                ;;
        -k | --kang )
                KANG="--kang"
                ;;
        -s | --section )
                SECTION="${2}"; shift
                CLEAN_VENDOR=false
                ;;
        * )
                SRC="${1}"
                ;;
    esac
    shift
done

if [ -z "${SRC}" ]; then
    SRC="adb"
fi

function blob_fixup() {
    case "${1}" in
        system_ext/etc/init/init.vtservice.rc)
            sed -i 's|start|enable|g' "$2"
            ;;
        system_ext/lib64/libsource.so)
            grep -q libui_shim.so "$2" || "$PATCHELF" --add-needed libui_shim.so "$2"
            ;;
        vendor/etc/init/android.hardware.graphics.allocator@4.0-service-mediatek.rc)
            sed -i 's|android.hardware.graphics.allocator@4.0-service-mediatek|mt6878/android.hardware.graphics.allocator@4.0-service-mediatek.mt6878|g' "${2}"
            ;;
        vendor/etc/init/android.hardware.graphics.allocator-V2-service-mediatek.rc)
            sed -i 's|android.hardware.graphics.allocator-V2-service-mediatek|mt6878/android.hardware.graphics.allocator-V2-service-mediatek.mt6878|g' "${2}"
            ;;
        vendor/etc/init/android.hardware.neuralnetworks-shim-service-mtk.rc)
            sed -i 's|start|enable|g' "$2"
            ;;
        vendor/lib64/hw/vendor.mediatek.hardware.pq_aidl-impl.so)
            grep -q libui_shim.so "$2" || "$PATCHELF" --add-needed libui_shim.so "$2"
            ;;
        vendor/etc/init/vendor.mediatek.hardware.mtkpower@1.0-service.rc)
            echo "$(cat ${2}) input" > "${2}"
            ;;
    esac
}

# Initialize the helper
setup_vendor "${DEVICE}" "${VENDOR}" "${ANDROID_ROOT}" false "${CLEAN_VENDOR}"

extract "${MY_DIR}/proprietary-files.txt" "${SRC}" "${KANG}" --section "${SECTION}"

if [ -z "${SECTION}" ]; then
    extract_firmware "${MY_DIR}/proprietary-firmware.txt" "${SRC}"
fi

"${MY_DIR}/setup-makefiles.sh"
