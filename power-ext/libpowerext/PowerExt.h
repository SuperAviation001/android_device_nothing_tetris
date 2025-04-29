#ifndef _POWEREXT_H_
#define _POWEREXT_H_
#include <aidl/lmodroid/hardware/power/extension/IPowerExt.h>
#include <string>

using namespace aidl::lmodroid::hardware::power::extension;

namespace aidl::android::hardware::power {
class IPower;
}

using namespace aidl::android::hardware::power;

namespace android {
class PowerExt {
public:
    PowerExt();
    ~PowerExt();
    bool init() { return connectHAL(); };
    void setMode(const std::string& mode, bool enabled);
    bool isModeSupported(const std::string& mode);
    void setBoost(const std::string& boost, int durationMs);
    bool isBoostSupported(const std::string& boost);
    void notifyAppState(const std::string& packActName, int pid, int uid, bool active);

protected:
    bool connectHAL();
    int mDeathCount;
    std::shared_ptr<IPower> mPowerHalAidl;
    std::shared_ptr<IPowerExt> mPowerHalExtAidl;
};
}; // namespace android
#endif
