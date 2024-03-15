#pragma once
#include <arkui/native_interface.h>
#include <arkui/native_dialog.h>

namespace rnoh {

    class NativeDialogApi {
    public:
        static ArkUI_NativeDialogAPI_1 *getInstance();

    private:
        NativeDialogApi() {}

        static ArkUI_NativeDialogAPI_1 *api;
    };

} // namespace rnoh
