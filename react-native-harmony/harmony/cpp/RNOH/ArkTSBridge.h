#pragma once

#include "ArkJS.h"

namespace rnoh {

struct PhysicalPixels {
    float width;
    float height;
    float scale;
    float fontScale;
    float densityDpi;

    static PhysicalPixels fromNapiValue(napi_env env, napi_value value);
};

struct DisplayMetrics {
    PhysicalPixels windowPhysicalPixels;
    PhysicalPixels screenPhysicalPixels;

    static DisplayMetrics fromNapiValue(napi_env env, napi_value value);
};

class ArkTSBridge final {
    static std::unique_ptr<ArkTSBridge> instance;
    ArkTSBridge(napi_env env, napi_ref napiBridgeRef);

  public:
    static void initializeInstance(napi_env env, napi_ref napiBridgeRef);

    static ArkTSBridge &getInstance();

    ArkTSBridge(ArkTSBridge const &) = delete;
    ArkTSBridge &operator=(ArkTSBridge const &) = delete;

    ~ArkTSBridge();

    DisplayMetrics getDisplayMetrics();

  protected:
    ArkJS m_arkJs;
    napi_ref m_napiBridgeRef;
};
} // namespace rnoh