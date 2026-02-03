#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.file import File
from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixups,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/nothing/Tetris',
    'hardware/mediatek',
    'hardware/mediatek/libmtkperf_client',
]


def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_{partition}' if partition == 'vendor' else None


lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
    'vendor.mediatek.hardware.videotelephony-V1-ndk'
    ): lib_fixup_vendor_suffix,
}


blob_fixups: blob_fixups_user_type = {
    (
        'system_ext/etc/init/init.vtservice.rc',
        'vendor/etc/init/android.hardware.neuralnetworks-shim-service-mtk.rc'
    ): blob_fixup()
        .regex_replace('start', 'enable'),
    'system_ext/lib64/libimsma.so': blob_fixup()
        .replace_needed('libsink.so', 'libsink-mtk.so'),
    'vendor/bin/hw/mt6878/camerahalserver': blob_fixup()
        .add_needed('libcamera_metadata_shim.so'),
    (
        'system_ext/lib64/libsource.so',
        'vendor/lib64/hw/vendor.mediatek.hardware.pq_aidl-impl.so'
    ): blob_fixup()
        .add_needed('libui_shim.so'),
    'vendor/etc/init/init.thermal_core.rc': blob_fixup()
        .regex_replace('ro.vendor.mtk_thermal_2_0', 'vendor.thermal.link_ready'),
    'vendor/lib64/hw/audio.primary.mt6878.so': blob_fixup()
        .replace_needed('libalsautils.so', 'libalsautils-stock.so'),
    'vendor/etc/init/android.hardware.graphics.allocator@4.0-service-mediatek.rc': blob_fixup()
        .regex_replace('android.hardware.graphics.allocator@4.0-service-mediatek', 'mt6878/android.hardware.graphics.allocator@4.0-service-mediatek.mt6878'),
    'vendor/etc/init/android.hardware.graphics.allocator-V2-service-mediatek.rc': blob_fixup()
        .regex_replace('android.hardware.graphics.allocator-V2-service-mediatek', 'mt6878/android.hardware.graphics.allocator-V2-service-mediatek.mt6878'),
    'vendor/etc/init/vendor.mediatek.hardware.mtkpower@1.0-service.rc': blob_fixup()
        .regex_replace('audio', 'audio input'),
    'vendor/lib64/hw/hwcomposer.mtk_common.so': blob_fixup()
        .add_needed('libprocessgroup_shim.so'),
}  # fmt: skip

module = ExtractUtilsModule(
    'Tetris',
    'nothing',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
