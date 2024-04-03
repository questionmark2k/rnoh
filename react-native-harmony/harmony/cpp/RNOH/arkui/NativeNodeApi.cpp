#include "NativeNodeApi.h"
#include <glog/logging.h>

namespace rnoh {

ArkUI_NativeNodeAPI_1* NativeNodeApi::getInstance() {
#ifdef C_API_ARCH
  static ArkUI_NativeNodeAPI_1* INSTANCE = nullptr;
  if (INSTANCE == nullptr) {
    INSTANCE = reinterpret_cast<ArkUI_NativeNodeAPI_1*>(
        OH_ArkUI_GetNativeAPI(ARKUI_NATIVE_NODE, 1));
  }
  return INSTANCE;
#endif
  LOG(FATAL)
      << "This method should only by used when C-API architecture is enabled.";
}

} // namespace rnoh