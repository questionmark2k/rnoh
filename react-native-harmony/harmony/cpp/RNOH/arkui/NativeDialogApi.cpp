#include "NativeDialogApi.h"
#include <glog/logging.h>

namespace rnoh {

    ArkUI_NativeDialogAPI_1 *NativeDialogApi::getInstance() {
        #ifdef C_API_ARCH
        static ArkUI_NativeDialogAPI_1 *INSTANCE = nullptr;
        if (INSTANCE == nullptr) {
            INSTANCE = reinterpret_cast<ArkUI_NativeDialogAPI_1 *>(OH_ArkUI_GetNativeAPI(ARKUI_NATIVE_DIALOG, 1));
        }
        return INSTANCE;
        #endif
        LOG(FATAL) << "This method should only by used when C-API architecture is enabled.";
    }


} // namespace rnoh