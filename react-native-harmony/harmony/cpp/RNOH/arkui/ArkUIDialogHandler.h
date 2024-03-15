#pragma once
#include "ArkUINode.h"
#include "NativeDialogApi.h"

namespace rnoh {

class ArkUIDialogHandler {

  public:
    ArkUIDialogHandler();

    ArkUIDialogHandler(ArkUIDialogHandler const &other) = delete;

    ArkUIDialogHandler &operator=(const ArkUIDialogHandler &other) = delete;

    ~ArkUIDialogHandler();

    void attachContent(ArkUINode &node);

    void show();

    void close();

  private:
    void destroy();

    void initDialogProperties();

    bool isShow_ = false;
    ArkUI_NativeDialogHandler handler_;
};

} // namespace rnoh