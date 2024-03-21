#include "ArkTSBridge.h"
#include <glog/logging.h>

namespace rnoh {

std::unique_ptr<ArkTSBridge> ArkTSBridge::instance = nullptr;

ArkTSBridge::ArkTSBridge(napi_env env, napi_ref napiBridgeRef)
    : m_arkJs(ArkJS(env)), m_napiBridgeRef(napiBridgeRef) {
    LOG(INFO) << "ArkTSBridge::ArkTSBridge";
}

void ArkTSBridge::initializeInstance(napi_env env, napi_ref napiBridgeRef) {
    if (instance) {
        throw std::runtime_error("ArkTSBridge can only be initialized once");
    }
    instance = std::unique_ptr<ArkTSBridge>(new ArkTSBridge(env, napiBridgeRef));
}

ArkTSBridge &ArkTSBridge::getInstance() {
    if (!instance) {
        throw std::runtime_error("ArkTSBridge is not initialized");
    }
    return *instance;
}

ArkTSBridge::~ArkTSBridge() {
    m_arkJs.deleteReference(m_napiBridgeRef);
}

auto ArkTSBridge::getDisplayMetrics() -> DisplayMetrics {
    auto napiBridgeObject = m_arkJs.getReferenceValue(m_napiBridgeRef);
    auto methodImpl = m_arkJs.getObjectProperty(napiBridgeObject, "getDisplayMetrics");
    auto napiResult = m_arkJs.call<0>(methodImpl, {});
    return DisplayMetrics::fromNapiValue(m_arkJs.getEnv(), napiResult);
}

auto PhysicalPixels::fromNapiValue(napi_env env, napi_value value) -> PhysicalPixels {
    ArkJS arkJs(env);
    return {
        static_cast<float>(arkJs.getDouble(arkJs.getObjectProperty(value, "width"))),
        static_cast<float>(arkJs.getDouble(arkJs.getObjectProperty(value, "height"))),
        static_cast<float>(arkJs.getDouble(arkJs.getObjectProperty(value, "scale"))),
        static_cast<float>(arkJs.getDouble(arkJs.getObjectProperty(value, "fontScale"))),
        static_cast<float>(arkJs.getDouble(arkJs.getObjectProperty(value, "densityDpi"))),
    };
}

auto DisplayMetrics::fromNapiValue(napi_env env, napi_value value) -> DisplayMetrics {
    ArkJS arkJs(env);
    return {
        PhysicalPixels::fromNapiValue(env, arkJs.getObjectProperty(value, "windowPhysicalPixels")),
        PhysicalPixels::fromNapiValue(env, arkJs.getObjectProperty(value, "screenPhysicalPixels")),
    };
}

} // namespace rnoh