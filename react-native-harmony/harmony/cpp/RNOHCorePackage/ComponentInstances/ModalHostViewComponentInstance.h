#pragma once

#include "RNOH/arkui/StackNode.h"
#include "RNOH/arkui/ArkUIDialogHandler.h"
#include "RNOH/arkui/ArkUINodeRegistry.h"
#include "RNOH/CppComponentInstance.h"
#include "react/renderer/components/modal/ModalHostViewShadowNode.h"

namespace rnoh {
class ModalHostViewComponentInstance : public CppComponentInstance<facebook::react::ModalHostViewShadowNode>, public ArkUIDialogDelegate {
  private:
    StackNode m_virtualNode;
    StackNode m_rootStackNode;
    ArkUIDialogHandler m_dialogHandler;
    std::unique_ptr<TouchEventHandler> m_touchHandler;

  public:
    ModalHostViewComponentInstance(Context context);

    void onPropsChanged(SharedConcreteProps const &props) override;
    void onStateChanged(SharedConcreteState const &state) override;

    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
    void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

    void finalizeUpdates() override;

    void showDialog();

    void onShow() override;

    void onRequestClose() override;

    StackNode &getLocalRootArkUINode() override;
    
    friend class ModalHostTouchHandler;
};
} // namespace rnoh