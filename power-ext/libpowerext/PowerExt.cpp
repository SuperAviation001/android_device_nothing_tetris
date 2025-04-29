#include "PowerExt.h"
#include <android/binder_manager.h>
#include <android/log.h>

#define LOG_TAG "PowerExtClient"
#include <log/log.h>

#include <aidl/android/hardware/power/IPower.h>

#define PLOGE(...) \
    ALOGE(__VA_ARGS__); \
    mDeathCount++;

namespace android {
// Construction Workers
PowerExt::PowerExt() {
    init();
    mDeathCount = 0;
}
PowerExt::~PowerExt() {
    ALOGE("Destroying!");
    mPowerHalAidl = nullptr;
    mPowerHalExtAidl = nullptr;
}
bool PowerExt::connectHAL() {
    if (mDeathCount > 5) {
        ALOGE("Failed to connect power HAL too many times, reconnecting");
        mDeathCount = 0;
    } else if (mPowerHalAidl && mPowerHalExtAidl) {
        return true;
    }

    const std::string kInstance = std::string(IPower::descriptor) + "/default";
    ndk::SpAIBinder pwBinder = ndk::SpAIBinder(AServiceManager_getService(kInstance.c_str()));

    mPowerHalAidl = IPower::fromBinder(pwBinder);

    if (!mPowerHalAidl) {
        ALOGE("failed to connect power HAL");
        return false;
    }

    ndk::SpAIBinder pwExtBinder;
    AIBinder_getExtension(pwBinder.get(), pwExtBinder.getR());

    mPowerHalExtAidl = IPowerExt::fromBinder(pwExtBinder);

    if (!mPowerHalExtAidl) {
        mPowerHalAidl = nullptr;
        ALOGE("failed to connect power HAL extension");
        return false;
    }

    ALOGE("connected power HAL successfully");
    return true;
}
// Main functions
void PowerExt::setMode(const std::string& mode, bool enabled) {
    if (!connectHAL()) {
        ALOGE("setMode failed to connect power HAL");
        return;
    }
    auto ret = mPowerHalExtAidl->setMode(mode, enabled);
    if (!ret.isOk()) {
        PLOGE("failed to set mode: %s", mode.c_str());
    }
}

bool PowerExt::isModeSupported(const std::string& mode) {
    if (!connectHAL()) {
        ALOGE("isModeSupported failed to connect power HAL");
        return false;
    }
    bool supported;
    auto ret = mPowerHalExtAidl->isModeSupported(mode, &supported);
    if (!ret.isOk()) {
        PLOGE("failed to check mode: %s", mode.c_str());
        return false;
    }
    ALOGE("isModeSupported: %s", supported ? "true" : "false");
    return supported;
}

void PowerExt::setBoost(const std::string& boost, int durationMs) {
    if (!connectHAL()) {
        ALOGE("setBoost failed to connect power HAL");
        return;
    }
    auto ret = mPowerHalExtAidl->setBoost(boost, durationMs);
    if (!ret.isOk()) {
        PLOGE("failed to set boost: %s", boost.c_str());
    }
}

bool PowerExt::isBoostSupported(const std::string& boost) {
    if (!connectHAL()) {
        ALOGE("isBoostSupported failed to connect power HAL");
        return false;
    }
    bool supported;
    auto ret = mPowerHalExtAidl->isBoostSupported(boost, &supported);
    if (!ret.isOk()) {
        PLOGE("failed to check boost: %s", boost.c_str());
        return false;
    }
    ALOGE("isBoostSupported: %s", supported ? "true" : "false");
    return supported;
}

void PowerExt::notifyAppState(const std::string& packActName, int pid, int uid, bool active) {
    if (!connectHAL()) {
        ALOGE("notifyAppState failed to connect power HAL");
        return;
    }
    auto ret = mPowerHalExtAidl->notifyAppState(packActName, pid, uid, active);
    if (!ret.isOk()) {
        PLOGE("failed to notify app state: %s", packActName.c_str());
    }
}
} // namespace android
