#include "ArkUIDialogHandler.h"

namespace rnoh {

static std::vector<ArkUIDialogHandler *> dialogs; // global dialogs, for OnDismiss event

ArkUIDialogHandler::ArkUIDialogHandler() : handler_(NativeDialogApi::getInstance()->create()) {
    dialogs.push_back(this);
    initDialogProperties();
}

ArkUIDialogHandler::~ArkUIDialogHandler() {
    dialogs.erase(std::remove(dialogs.begin(), dialogs.end(), this), dialogs.end());
    destroy();
}

void ArkUIDialogHandler::show() {
    if (isShow_) {
        DLOG(INFO) << "open dialog abort, it has already been opened";
        return;
    }
    NativeDialogApi::getInstance()->show(handler_, false); // false means show in main window
    isShow_ = true;
}

void ArkUIDialogHandler::attachContent(ArkUINode &node) {
    if (auto nodeHandler = node.getArkUINodeHandle()) {
        NativeDialogApi::getInstance()->attachContent(handler_, nodeHandler);
    } else {
        DLOG(WARNING) << "mount dialog failed, node handler is null";
    }
}

void ArkUIDialogHandler::close() {
    if (handler_) {
        NativeDialogApi::getInstance()->close(handler_);
    }
    isShow_ = false;
}

void ArkUIDialogHandler::destroy() {
    close();
    if (handler_) {
        NativeDialogApi::getInstance()->dispose(handler_);
    }
}

void ArkUIDialogHandler::initDialogProperties() {
    // set modal[true] and autoCancel[false]
    NativeDialogApi::getInstance()->setMode(handler_, true, false);
    NativeDialogApi::getInstance()->setContentAlignment(handler_, static_cast<int32_t>(ARKUI_ALIGNMENT_CENTER), 0, 0);
    NativeDialogApi::getInstance()->setCustomStyle(handler_, true);
    // set mask transparent
    NativeDialogApi::getInstance()->setMask(handler_, 0, nullptr);
    // register onWillDismiss event
    NativeDialogApi::getInstance()->registerOnWillDismiss(handler_, [](int32_t reason) -> bool {
        if (!dialogs.empty()) {
            dialogs.back()->close();
            dialogs.pop_back();
            return false;
        }
        return true;
    });
}

} // namespace rnoh
