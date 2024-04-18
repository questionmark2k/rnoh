#pragma once
#include <arkui/native_dialog.h>
#include <arkui/native_interface.h>

namespace rnoh {

class NativeDialogApi {
 public:
  static ArkUI_NativeDialogAPI_1* getInstance();

 private:
  NativeDialogApi() {}

  static ArkUI_NativeDialogAPI_1* api;
};

} // namespace rnoh
