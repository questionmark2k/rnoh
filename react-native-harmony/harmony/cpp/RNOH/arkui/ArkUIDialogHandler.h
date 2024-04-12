#pragma once
#include "ArkUINode.h"
#include "NativeDialogApi.h"

namespace rnoh {

class ArkUIDialogDelegate {
 public:
  virtual ~ArkUIDialogDelegate() = default;
  virtual void onShow(){};
  virtual void onRequestClose(){};
};

class ArkUIDialogHandler {
 public:
  ArkUIDialogHandler();

  ArkUIDialogHandler(ArkUIDialogHandler const& other) = delete;

  ArkUIDialogHandler& operator=(const ArkUIDialogHandler& other) = delete;

  ~ArkUIDialogHandler();

  void setContent(ArkUINode& node);

  void show();

  void close();

  void onShow();

  void onRequestClose();

  void setDialogDelegate(ArkUIDialogDelegate* dialogDelegate) {
    m_dialogDelegate = dialogDelegate;
  }

  bool isShow() {
    return isShow_;
  }

 private:
  void destroy();

  void initDialogProperties();

  bool isShow_ = false;
  ArkUI_NativeDialogHandle handler_;
  ArkUIDialogDelegate* m_dialogDelegate{};
};

} // namespace rnoh