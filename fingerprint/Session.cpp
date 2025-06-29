/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include <android-base/file.h>
#include <android-base/stringprintf.h>

#include "Session.h"
#include "Legacy2Aidl.h"

#include "CancellationSignal.h"

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

#define FOD_HBM_PATH "/sys/panel_feature/hbm_node"
#define FOD_UI_STATUS "/sys/panel_feature/ui_status"
#define FOD_HBM_DELAY 60

void setFodHbm(bool status) {
    ::android::base::WriteStringToFile(status ? "1" : "0", FOD_HBM_PATH);
}

void setFodStatus(bool status) {
    ::android::base::WriteStringToFile(status ? "1" : "0", FOD_UI_STATUS);
}

void onClientDeath(void* cookie) {
    ALOGI("FingerprintService has died");
    Session* session = static_cast<Session*>(cookie);
    if (session && !session->isClosed()) {
        session->close();
    }
}

Session::Session(fingerprint_device_t* device, int32_t userId,
            std::shared_ptr<ISessionCallback> cb, LockoutTracker lockoutTracker)
            : mDevice(device), mUserId(userId), mLockoutTracker(lockoutTracker), mCb(cb) {
    mDeathRecipient = AIBinder_DeathRecipient_new(onClientDeath);

    std::string path = ::android::base::StringPrintf("/data/vendor_de/%d/fpdata/", mUserId);
    mDevice->set_active_group(mDevice, mUserId, path.c_str());
}

ndk::ScopedAStatus Session::generateChallenge() {
    uint64_t challenge = mDevice->generateChallenge(mDevice);
    ALOGI("generateChallenge: %ld", challenge);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::revokeChallenge(int64_t challenge) {
    ALOGI("revokeChallenge: %ld", challenge);
    setFodHbm(false);
    mDevice->goodix_extCmd(mDevice, 0, 0);
    setFodStatus(false);
    mDevice->revokeChallenge(mDevice, challenge);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::enroll(const HardwareAuthToken& hat,
                                   std::shared_ptr<ICancellationSignal>* out) {
    ALOGI("enroll");

    hw_auth_token_t authToken;
    translate(hat, authToken);

    int error = mDevice->enroll(mDevice, &authToken);
    if (error) {
        ALOGE("enroll failed: %d", error);
        mCb->onError(Error::UNABLE_TO_PROCESS, error);
    }

    *out = SharedRefBase::make<CancellationSignal>(this);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::authenticate(int64_t operationId,
                                         std::shared_ptr<ICancellationSignal>* out) {
    ALOGI("authenticate");

    int error = mDevice->authenticate(mDevice, operationId);
    if (error) {
        ALOGE("authenticate failed: %d", error);
        mCb->onError(Error::UNABLE_TO_PROCESS, error);
    }

    *out = SharedRefBase::make<CancellationSignal>(this);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::detectInteraction(std::shared_ptr<ICancellationSignal>* out) {
    ALOGI("detectInteraction");
    ALOGD("Detect interaction is not supported");
    mCb->onError(Error::UNABLE_TO_PROCESS, 0 /* vendorCode */);

    *out = SharedRefBase::make<CancellationSignal>(this);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::enumerateEnrollments() {
    ALOGI("enumerateEnrollments");

    int error = mDevice->enumerate(mDevice);
    if (error) {
        ALOGE("enumerate failed: %d", error);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::removeEnrollments(const std::vector<int32_t>& enrollmentIds) {
    ALOGI("removeEnrollments, size: %zu", enrollmentIds.size());

    std::vector<uint32_t> fids(enrollmentIds.begin(), enrollmentIds.end());
    int error = mDevice->remove(mDevice, fids.data(), static_cast<uint32_t>(fids.size()));
    if (error) {
        ALOGE("Failed to remove enrollments: %d", error);
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::getAuthenticatorId() {
    uint64_t auth_id = mDevice->getAuthenticatorId(mDevice);
    ALOGI("getAuthenticatorId: %ld", auth_id);
    
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::invalidateAuthenticatorId() {
    uint64_t auth_id = mDevice->invalidateAuthenticatorId(mDevice);
    ALOGI("invalidateAuthenticatorId: %ld", auth_id);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::resetLockout(const HardwareAuthToken& hat) {
    ALOGI("resetLockout");

    hw_auth_token_t authToken;
    translate(hat, authToken);

    int error = mDevice->resetLockout(mDevice, &authToken);
    if (error != 0) {
        ALOGE("Failed to reset lockout: %d", error);
    }

    clearLockout(true);
    mIsLockoutTimerAborted = true;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::onPointerDown(int32_t /*pointerId*/, int32_t x, int32_t y, float minor,
                                          float major) {
    ALOGI("onPointerDown: x=%d, y=%d, minor=%f, major=%f", x, y, minor, major);

    std::this_thread::sleep_for(std::chrono::milliseconds(FOD_HBM_DELAY));
    setFodHbm(true);
    mDevice->goodix_extCmd(mDevice, 1, 1);
    setFodStatus(true);

    checkSensorLockout();

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::onPointerUp(int32_t /*pointerId*/) {
    ALOGI("onPointerUp");

    mDevice->goodix_extCmd(mDevice, 0, 0);
    setFodHbm(false);
    setFodStatus(false);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::onUiReady() {
    ALOGI("onUiReady");

    // TODO: stub

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::authenticateWithContext(
        int64_t operationId, const common::OperationContext& /*context*/,
        std::shared_ptr<common::ICancellationSignal>* out) {
    return authenticate(operationId, out);
}

ndk::ScopedAStatus Session::enrollWithContext(const keymaster::HardwareAuthToken& hat,
                                              const common::OperationContext& /*context*/,
                                              std::shared_ptr<common::ICancellationSignal>* out) {
    return enroll(hat, out);
}

ndk::ScopedAStatus Session::detectInteractionWithContext(
        const common::OperationContext& /*context*/,
        std::shared_ptr<common::ICancellationSignal>* out) {
    return detectInteraction(out);
}

ndk::ScopedAStatus Session::onPointerDownWithContext(const PointerContext& context) {
    return onPointerDown(context.pointerId, context.x, context.y, context.minor, context.major);
}

ndk::ScopedAStatus Session::onPointerUpWithContext(const PointerContext& context) {
    return onPointerUp(context.pointerId);
}

ndk::ScopedAStatus Session::onContextChanged(const common::OperationContext& /*context*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::onPointerCancelWithContext(const PointerContext& /*context*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::setIgnoreDisplayTouches(bool /*shouldIgnore*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Session::cancel() {
    ALOGI("cancel");

    setFodHbm(false);
    mDevice->goodix_extCmd(mDevice, 0, 0);
    setFodStatus(false);

    int ret = mDevice->cancel(mDevice);

    if (ret == 0) {
        mCb->onError(Error::CANCELED, 0 /* vendorCode */);

        return ndk::ScopedAStatus::ok();
    } else {
        return ndk::ScopedAStatus::fromServiceSpecificError(ret);
    }
}

ndk::ScopedAStatus Session::close() {
    ALOGI("close");
    setFodHbm(false);
    mDevice->goodix_extCmd(mDevice, 0, 0);
    setFodStatus(false);
    mClosed = true;
    mCb->onSessionClosed();
    AIBinder_DeathRecipient_delete(mDeathRecipient);
    return ndk::ScopedAStatus::ok();
}

binder_status_t Session::linkToDeath(AIBinder* binder) {
    return AIBinder_linkToDeath(binder, mDeathRecipient, this);
}

bool Session::isClosed() {
    return mClosed;
}

// Translate from errors returned by traditional HAL (see fingerprint.h) to
// AIDL-compliant Error
Error Session::VendorErrorFilter(int32_t error, int32_t* vendorCode) {
    *vendorCode = 0;
    switch (error) {
        case FINGERPRINT_ERROR_HW_UNAVAILABLE:
            return Error::HW_UNAVAILABLE;
        case FINGERPRINT_ERROR_UNABLE_TO_PROCESS:
            return Error::UNABLE_TO_PROCESS;
        case FINGERPRINT_ERROR_TIMEOUT:
            return Error::TIMEOUT;
        case FINGERPRINT_ERROR_NO_SPACE:
            return Error::NO_SPACE;
        case FINGERPRINT_ERROR_CANCELED:
            return Error::CANCELED;
        case FINGERPRINT_ERROR_UNABLE_TO_REMOVE:
            return Error::UNABLE_TO_REMOVE;
        case FINGERPRINT_ERROR_LOCKOUT: {
            *vendorCode = FINGERPRINT_ERROR_LOCKOUT;
            return Error::VENDOR;
        }
        default:
            if (error >= FINGERPRINT_ERROR_VENDOR_BASE) {
                // vendor specific code.
                *vendorCode = error - FINGERPRINT_ERROR_VENDOR_BASE;
                return Error::VENDOR;
            }
    }
    ALOGE("Unknown error from fingerprint vendor library: %d", error);
    return Error::UNABLE_TO_PROCESS;
}

// Translate acquired messages returned by traditional HAL (see fingerprint.h)
// to AIDL-compliant AcquiredInfo
AcquiredInfo Session::VendorAcquiredFilter(int32_t info, int32_t* vendorCode) {
    *vendorCode = 0;
    switch (info) {
        case FINGERPRINT_ACQUIRED_GOOD:
            return AcquiredInfo::GOOD;
        case FINGERPRINT_ACQUIRED_PARTIAL:
            return AcquiredInfo::PARTIAL;
        case FINGERPRINT_ACQUIRED_INSUFFICIENT:
            return AcquiredInfo::INSUFFICIENT;
        case FINGERPRINT_ACQUIRED_IMAGER_DIRTY:
            return AcquiredInfo::SENSOR_DIRTY;
        case FINGERPRINT_ACQUIRED_TOO_SLOW:
            return AcquiredInfo::TOO_SLOW;
        case FINGERPRINT_ACQUIRED_TOO_FAST:
            return AcquiredInfo::TOO_FAST;
        default:
            if (info >= FINGERPRINT_ACQUIRED_VENDOR_BASE) {
                // vendor specific code.
                *vendorCode = info - FINGERPRINT_ACQUIRED_VENDOR_BASE;
                return AcquiredInfo::VENDOR;
            }
    }
    ALOGE("Unknown acquiredmsg from fingerprint vendor library: %d", info);
    return AcquiredInfo::INSUFFICIENT;
}

bool Session::checkSensorLockout() {
    LockoutMode lockoutMode = mLockoutTracker.getMode();

    if (lockoutMode != LockoutMode::NONE) {
	setFodHbm(false);
	mDevice->goodix_extCmd(mDevice, 0, 0);
        setFodStatus(false);
    }

    if (lockoutMode == LockoutMode::PERMANENT) {
        ALOGE("Fail: lockout permanent");
        mCb->onLockoutPermanent();
        mIsLockoutTimerAborted = true;
        return true;
    } else if (lockoutMode == LockoutMode::TIMED) {
        int64_t timeLeft = mLockoutTracker.getLockoutTimeLeft();
        ALOGE("Fail: lockout timed: %ld", timeLeft);
        mCb->onLockoutTimed(timeLeft);
        if (!mIsLockoutTimerStarted) startLockoutTimer(timeLeft);
        return true;
    }

    return false;
}

void Session::clearLockout(bool clearAttemptCounter) {
    mLockoutTracker.reset(clearAttemptCounter);
    mCb->onLockoutCleared();
}

void Session::startLockoutTimer(int64_t timeout) {
    mIsLockoutTimerAborted = false;
    std::function<void()> action =
            std::bind(&Session::lockoutTimerExpired, this);
    std::thread([timeout, action]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        action();
    }).detach();

    mIsLockoutTimerStarted = true;
}

void Session::lockoutTimerExpired() {
    if (!mIsLockoutTimerAborted)
        clearLockout(false);

    mIsLockoutTimerStarted = false;
    mIsLockoutTimerAborted = false;
}

void Session::notify(const fingerprint_msg_t* msg) {
    switch (msg->type) {
        case FINGERPRINT_ERROR: {
            int32_t vendorCode = 0;
            Error result = VendorErrorFilter(msg->data.error, &vendorCode);
            ALOGD("onError(%hhd, %d)", result, vendorCode);
            mCb->onError(result, vendorCode);
        } break;
        case FINGERPRINT_ACQUIRED: {
            int32_t vendorCode = 0;
            AcquiredInfo result =
                    VendorAcquiredFilter(msg->data.acquired.acquired_info, &vendorCode);
            if (result != AcquiredInfo::VENDOR) {
                ALOGD("onAcquired(%d, %d)", result, vendorCode);
                mCb->onAcquired(result, vendorCode);
            } else {
                ALOGW("onAcquired(AcquiredInfo::VENDOR, %d)", vendorCode);
                // Do not send onAcquired or illumination will be turned off prematurely
            }
        } break;
        case FINGERPRINT_TEMPLATE_ENROLLING: {
            ALOGD("onEnrollResult(fid=%d, gid=%d, rem=%d)", msg->data.enroll.finger.fid,
                  msg->data.enroll.finger.gid, msg->data.enroll.samples_remaining);
            mCb->onEnrollmentProgress(msg->data.enroll.finger.fid,
                                      msg->data.enroll.samples_remaining);
            if (msg->data.enroll.samples_remaining == 0) {
                setFodHbm(false);
                mDevice->goodix_extCmd(mDevice, 0, 0);
                setFodStatus(false);
            }
        } break;
        case FINGERPRINT_TEMPLATE_REMOVED: {
            ALOGD("onRemove(fid=%d, gid=%d, rem=%d)", msg->data.removed.finger.fid,
                  msg->data.removed.finger.gid, msg->data.removed.remaining_templates);
            std::vector<int> enrollments;
            enrollments.push_back(msg->data.removed.finger.fid);
            mCb->onEnrollmentsRemoved(enrollments);
        } break;
        case FINGERPRINT_AUTHENTICATED: {
            ALOGD("onAuthenticated(fid=%d, gid=%d)", msg->data.authenticated.finger.fid,
                msg->data.authenticated.finger.gid);
            if (msg->data.authenticated.finger.fid != 0) {
                const hw_auth_token_t hat = msg->data.authenticated.hat;
                HardwareAuthToken authToken;
                translate(hat, authToken);

                mCb->onAuthenticationSucceeded(msg->data.authenticated.finger.fid, authToken);
                mLockoutTracker.reset(true);
                setFodHbm(false);
                mDevice->goodix_extCmd(mDevice, 0, 0);
                setFodStatus(false);
            } else {
                mCb->onAuthenticationFailed();
                mLockoutTracker.addFailedAttempt();
                checkSensorLockout();
            }
        } break;
        case FINGERPRINT_TEMPLATE_ENUMERATING: {
            ALOGD("onEnumerate(fid=%d, gid=%d, rem=%d)", msg->data.enumerated.finger.fid,
                  msg->data.enumerated.finger.gid, msg->data.enumerated.remaining_templates);
            static std::vector<int> enrollments;
            enrollments.push_back(msg->data.enumerated.finger.fid);
            if (msg->data.enumerated.remaining_templates == 0) {
                mCb->onEnrollmentsEnumerated(enrollments);
                enrollments.clear();
            }
        } break;
        case FINGERPRINT_GENERATE_CHALLENGE: {
            ALOGD("onChallengeGenerated(%lu)", msg->data.data);
            mCb->onChallengeGenerated(msg->data.data);
        } break;
        case FINGERPRINT_REVOKE_CHALLENGE: {
            ALOGD("onChallengeRevoked(%lu)", msg->data.data);
            mCb->onChallengeRevoked(msg->data.data);
        } break;
        case FINGERPRINT_GET_AUTHENTICATOR_ID: {
            ALOGD("onAuthenticatorIdRetrieved(%lu)", msg->data.data);
            mCb->onAuthenticatorIdRetrieved(msg->data.data);
        } break;
        case FINGERPRINT_INVALIDATE_AUTHENTICATOR_ID: {
            ALOGD("onAuthenticatorIdInvalidated(%lu)", msg->data.data);
            mCb->onAuthenticatorIdInvalidated(msg->data.data);
        } break;
    }
}

} // namespace fingerprint
} // namespace biometrics
} // namespace hardware
} // namespace android
} // namespace aidl