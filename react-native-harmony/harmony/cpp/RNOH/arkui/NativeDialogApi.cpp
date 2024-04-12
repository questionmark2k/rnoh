#include "NativeDialogApi.h"
#include <glog/logging.h>

namespace rnoh {

ArkUI_NativeDialogAPI_1* NativeDialogApi::getInstance() {
#ifdef C_API_ARCH
  static ArkUI_NativeDialogAPI_1* INSTANCE = nullptr;
  if (INSTANCE == nullptr) {
    OH_ArkUI_GetModuleInterface(
        ARKUI_NATIVE_DIALOG, ArkUI_NativeDialogAPI_1, INSTANCE);
    if (INSTANCE == nullptr) {
      LOG(FATAL) << "Failed to get native dialog API instance.";
    }
  }
  return INSTANCE;
#endif
  LOG(FATAL)
      << "This method should only by used when C-API architecture is enabled.";
}

} // namespace rnoh